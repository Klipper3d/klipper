# Support for reading acceleration data from an mpu9250 chip
#
# Copyright (C) 2022  Harry Beyel <harry3b9@gmail.com>
# Copyright (C) 2020-2021 Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, time, collections, threading, multiprocessing, os
from . import bus, motion_report, adxl345

MPU9250_ADDR =      0x68

MPU_DEV_IDS = {
    0x74: "mpu-9515",
    0x73: "mpu-9255",
    0x71: "mpu-9250",
    0x70: "mpu-6500",
    0x68: "mpu-6050",
    #everything above are normal MPU IDs
    0x75: "mpu-unknown (DEFECTIVE! USE WITH CAUTION!)",
    0x69: "mpu-unknown (DEFECTIVE! USE WITH CAUTION!)",
    }

# MPU9250 registers
REG_DEVID =         0x75
REG_FIFO_EN =       0x23
REG_SMPLRT_DIV =    0x19
REG_CONFIG =        0x1A
REG_ACCEL_CONFIG =  0x1C
REG_ACCEL_CONFIG2 = 0x1D
REG_USER_CTRL =     0x6A
REG_PWR_MGMT_1 =    0x6B
REG_PWR_MGMT_2 =    0x6C

SAMPLE_RATE_DIVS = { 4000:0x00 }

SET_CONFIG =        0x01 # FIFO mode 'stream' style
SET_ACCEL_CONFIG =  0x10 # 8g full scale
SET_ACCEL_CONFIG2 = 0x08 # 1046Hz BW, 0.503ms delay 4kHz sample rate
SET_PWR_MGMT_1_WAKE =     0x00
SET_PWR_MGMT_1_SLEEP=     0x40
SET_PWR_MGMT_2_ACCEL_ON = 0x07
SET_PWR_MGMT_2_OFF  =     0x3F

FREEFALL_ACCEL = 9.80665 * 1000.
# SCALE = 1/4096 g/LSB @8g scale * Earth gravity in mm/s**2
SCALE = 0.000244140625 * FREEFALL_ACCEL

FIFO_SIZE = 512

Accel_Measurement = collections.namedtuple(
    'Accel_Measurement', ('time', 'accel_x', 'accel_y', 'accel_z'))

MIN_MSG_TIME = 0.100

BYTES_PER_SAMPLE = 6
SAMPLES_PER_BLOCK = 8

# Printer class that controls MPU9250 chip
class MPU9250:
    def __init__(self, config):
        self.printer = config.get_printer()
        adxl345.AccelCommandHelper(config, self)
        self.query_rate = 0
        am = {'x': (0, SCALE), 'y': (1, SCALE), 'z': (2, SCALE),
              '-x': (0, -SCALE), '-y': (1, -SCALE), '-z': (2, -SCALE)}
        axes_map = config.getlist('axes_map', ('x','y','z'), count=3)
        if any([a not in am for a in axes_map]):
            raise config.error("Invalid mpu9250 axes_map parameter")
        self.axes_map = [am[a.strip()] for a in axes_map]
        self.data_rate = config.getint('rate', 4000)
        if self.data_rate not in SAMPLE_RATE_DIVS:
            raise config.error("Invalid rate parameter: %d" % (self.data_rate,))
        # Measurement storage (accessed from background thread)
        self.lock = threading.Lock()
        self.raw_samples = []
        # Setup mcu sensor_mpu9250 bulk query code
        self.i2c = bus.MCU_I2C_from_config(config,
                                           default_addr=MPU9250_ADDR,
                                           default_speed=400000)
        self.mcu = mcu = self.i2c.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.query_mpu9250_cmd = self.query_mpu9250_end_cmd = None
        self.query_mpu9250_status_cmd = None
        mcu.register_config_callback(self._build_config)
        mcu.register_response(self._handle_mpu9250_data, "mpu9250_data", oid)
        # Clock tracking
        self.last_sequence = self.max_query_duration = 0
        self.last_limit_count = self.last_error_count = 0
        self.clock_sync = adxl345.ClockSyncRegression(self.mcu, 640)
        # API server endpoints
        self.api_dump = motion_report.APIDumpHelper(
            self.printer, self._api_update, self._api_startstop, 0.100)
        self.name = config.get_name().split()[-1]
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint("mpu9250/dump_mpu9250", "sensor", self.name,
                                 self._handle_dump_mpu9250)
    def _build_config(self):
        cmdqueue = self.i2c.get_command_queue()
        self.mcu.add_config_cmd("config_mpu9250 oid=%d i2c_oid=%d"
                           % (self.oid, self.i2c.get_oid()))
        self.mcu.add_config_cmd("query_mpu9250 oid=%d clock=0 rest_ticks=0"
                           % (self.oid,), on_restart=True)
        self.query_mpu9250_cmd = self.mcu.lookup_command(
            "query_mpu9250 oid=%c clock=%u rest_ticks=%u", cq=cmdqueue)
        self.query_mpu9250_end_cmd = self.mcu.lookup_query_command(
            "query_mpu9250 oid=%c clock=%u rest_ticks=%u",
            "mpu9250_status oid=%c clock=%u query_ticks=%u next_sequence=%hu"
            " buffered=%c fifo=%u limit_count=%hu", oid=self.oid, cq=cmdqueue)
        self.query_mpu9250_status_cmd = self.mcu.lookup_query_command(
            "query_mpu9250_status oid=%c",
            "mpu9250_status oid=%c clock=%u query_ticks=%u next_sequence=%hu"
            " buffered=%c fifo=%u limit_count=%hu", oid=self.oid, cq=cmdqueue)
    def read_reg(self, reg):
        params = self.i2c.i2c_read([reg], 1)
        return bytearray(params['response'])[0]

    def set_reg(self, reg, val, minclock=0):
        self.i2c.i2c_write([reg, val & 0xFF], minclock=minclock)

    # Measurement collection
    def is_measuring(self):
        return self.query_rate > 0
    def _handle_mpu9250_data(self, params):
        with self.lock:
            self.raw_samples.append(params)
    def _extract_samples(self, raw_samples):
        # Load variables to optimize inner loop below
        (x_pos, x_scale), (y_pos, y_scale), (z_pos, z_scale) = self.axes_map
        last_sequence = self.last_sequence
        time_base, chip_base, inv_freq = self.clock_sync.get_time_translation()
        # Process every message in raw_samples
        count = seq = 0
        samples = [None] * (len(raw_samples) * SAMPLES_PER_BLOCK)
        for params in raw_samples:
            seq_diff = (last_sequence - params['sequence']) & 0xffff
            seq_diff -= (seq_diff & 0x8000) << 1
            seq = last_sequence - seq_diff
            d = bytearray(params['data'])
            msg_cdiff = seq * SAMPLES_PER_BLOCK - chip_base

            for i in range(len(d) // BYTES_PER_SAMPLE):
                d_xyz = d[i*BYTES_PER_SAMPLE:(i+1)*BYTES_PER_SAMPLE]
                xhigh, xlow, yhigh, ylow, zhigh, zlow = d_xyz
                # Merge and perform twos-complement
                rx = ((xhigh << 8) | xlow) - ((xhigh & 0x80) << 9)
                ry = ((yhigh << 8) | ylow) - ((yhigh & 0x80) << 9)
                rz = ((zhigh << 8) | zlow) - ((zhigh & 0x80) << 9)

                raw_xyz = (rx, ry, rz)
                x = round(raw_xyz[x_pos] * x_scale, 6)
                y = round(raw_xyz[y_pos] * y_scale, 6)
                z = round(raw_xyz[z_pos] * z_scale, 6)
                ptime = round(time_base + (msg_cdiff + i) * inv_freq, 6)
                samples[count] = (ptime, x, y, z)
                count += 1
        self.clock_sync.set_last_chip_clock(seq * SAMPLES_PER_BLOCK + i)
        del samples[count:]
        return samples

    def _update_clock(self, minclock=0):
        # Query current state
        for retry in range(5):
            params = self.query_mpu9250_status_cmd.send([self.oid],
                                                        minclock=minclock)
            fifo = params['fifo'] & 0x1fff
            if fifo <= FIFO_SIZE:
                break
        else:
            raise self.printer.command_error("Unable to query mpu9250 fifo")
        mcu_clock = self.mcu.clock32_to_clock64(params['clock'])
        sequence = (self.last_sequence & ~0xffff) | params['next_sequence']
        if sequence < self.last_sequence:
            sequence += 0x10000
        self.last_sequence = sequence
        buffered = params['buffered']
        limit_count = (self.last_limit_count & ~0xffff) | params['limit_count']
        if limit_count < self.last_limit_count:
            limit_count += 0x10000
        self.last_limit_count = limit_count
        duration = params['query_ticks']
        if duration > self.max_query_duration:
            # Skip measurement as a high query time could skew clock tracking
            self.max_query_duration = max(2 * self.max_query_duration,
                                          self.mcu.seconds_to_clock(.000005))
            return
        self.max_query_duration = 2 * duration
        msg_count = (sequence * SAMPLES_PER_BLOCK
                     + buffered // BYTES_PER_SAMPLE + fifo)
        # The "chip clock" is the message counter plus .5 for average
        # inaccuracy of query responses and plus .5 for assumed offset
        # of mpu9250 hw processing time.
        chip_clock = msg_count + 1
        self.clock_sync.update(mcu_clock + duration // 2, chip_clock)
    def _start_measurements(self):
        if self.is_measuring():
            return
        # In case of miswiring, testing MPU9250 device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        dev_id = self.read_reg(REG_DEVID)
        if dev_id not in MPU_DEV_IDS.keys():
            raise self.printer.command_error(
                "Invalid mpu id (got %x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty chip."
                % (dev_id))
        else:
            logging.info("Found %s with id %x"% (MPU_DEV_IDS[dev_id], dev_id))
        # Setup chip in requested query rate
        self.set_reg(REG_PWR_MGMT_1, SET_PWR_MGMT_1_WAKE)
        self.set_reg(REG_PWR_MGMT_2, SET_PWR_MGMT_2_ACCEL_ON)
        time.sleep(20. / 1000) # wait for accelerometer chip wake up
        self.set_reg(REG_SMPLRT_DIV, SAMPLE_RATE_DIVS[self.data_rate])
        self.set_reg(REG_CONFIG, SET_CONFIG)
        self.set_reg(REG_ACCEL_CONFIG, SET_ACCEL_CONFIG)
        self.set_reg(REG_ACCEL_CONFIG2, SET_ACCEL_CONFIG2)

        # Setup samples
        with self.lock:
            self.raw_samples = []
        # Start bulk reading
        systime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(systime) + MIN_MSG_TIME
        reqclock = self.mcu.print_time_to_clock(print_time)
        rest_ticks = self.mcu.seconds_to_clock(1. / self.data_rate)
        self.query_rate = self.data_rate
        self.query_mpu9250_cmd.send([self.oid, reqclock, rest_ticks],
                                    reqclock=reqclock)
        logging.info("MPU9250 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.last_sequence = 0
        self.last_limit_count = self.last_error_count = 0
        self.clock_sync.reset(reqclock, 0)
        self.max_query_duration = 1 << 31
        self._update_clock(minclock=reqclock)
        self.max_query_duration = 1 << 31
    def _finish_measurements(self):
        if not self.is_measuring():
            return
        # Halt bulk reading
        params = self.query_mpu9250_end_cmd.send([self.oid, 0, 0])
        self.query_rate = 0
        with self.lock:
            self.raw_samples = []
        logging.info("MPU9250 finished '%s' measurements", self.name)
        self.set_reg(REG_PWR_MGMT_1, SET_PWR_MGMT_1_SLEEP)
        self.set_reg(REG_PWR_MGMT_2, SET_PWR_MGMT_2_OFF)

    # API interface
    def _api_update(self, eventtime):
        self._update_clock()
        with self.lock:
            raw_samples = self.raw_samples
            self.raw_samples = []
        if not raw_samples:
            return {}
        samples = self._extract_samples(raw_samples)
        if not samples:
            return {}
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.last_limit_count}
    def _api_startstop(self, is_start):
        if is_start:
            self._start_measurements()
        else:
            self._finish_measurements()
    def _handle_dump_mpu9250(self, web_request):
        self.api_dump.add_client(web_request)
        hdr = ('time', 'x_acceleration', 'y_acceleration', 'z_acceleration')
        web_request.send({'header': hdr})
    def start_internal_client(self):
        cconn = self.api_dump.add_internal_client()
        return adxl345.AccelQueryHelper(self.printer, cconn)

def load_config(config):
    return MPU9250(config)

def load_config_prefix(config):
    return MPU9250(config)
