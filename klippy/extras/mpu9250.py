# Support for reading acceleration data from an mpu9250 chip
#
# Copyright (C) 2022  Harry Beyel <harry3b9@gmail.com>
# Copyright (C) 2020-2021 Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus, adxl345, bulk_sensor

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
REG_INT_STATUS =    0x3A

SAMPLE_RATE_DIVS = { 4000:0x00 }

SET_CONFIG =        0x01 # FIFO mode 'stream' style
SET_ACCEL_CONFIG =  0x10 # 8g full scale
SET_ACCEL_CONFIG2 = 0x08 # 1046Hz BW, 0.503ms delay 4kHz sample rate
SET_PWR_MGMT_1_WAKE =     0x00
SET_PWR_MGMT_1_SLEEP=     0x40
SET_PWR_MGMT_2_ACCEL_ON = 0x07
SET_PWR_MGMT_2_OFF  =     0x3F
SET_USER_FIFO_RESET = 0x04
SET_USER_FIFO_EN    = 0x40
SET_ENABLE_FIFO  = 0x08
SET_DISABLE_FIFO = 0x00

FREEFALL_ACCEL = 9.80665 * 1000.
# SCALE = 1/4096 g/LSB @8g scale * Earth gravity in mm/s**2
SCALE = 0.000244140625 * FREEFALL_ACCEL

FIFO_SIZE = 512

BATCH_UPDATES = 0.100

# Printer class that controls MPU9250 chip
class MPU9250:
    def __init__(self, config):
        self.printer = config.get_printer()
        adxl345.AccelCommandHelper(config, self)
        self.axes_map = adxl345.read_axes_map(config, SCALE, SCALE, SCALE)
        self.data_rate = config.getint('rate', 4000)
        if self.data_rate not in SAMPLE_RATE_DIVS:
            raise config.error("Invalid rate parameter: %d" % (self.data_rate,))
        # Setup mcu sensor_mpu9250 bulk query code
        self.i2c = bus.MCU_I2C_from_config(config,
                                           default_addr=MPU9250_ADDR,
                                           default_speed=400000)
        self.mcu = mcu = self.i2c.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.query_mpu9250_cmd = None
        mcu.register_config_callback(self._build_config)
        # Bulk sample message reading
        chip_smooth = self.data_rate * BATCH_UPDATES * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, ">hhh")
        self.last_error_count = 0
        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch,
            self._start_measurements, self._finish_measurements, BATCH_UPDATES)
        self.name = config.get_name().split()[-1]
        hdr = ('time', 'x_acceleration', 'y_acceleration', 'z_acceleration')
        self.batch_bulk.add_mux_endpoint("mpu9250/dump_mpu9250", "sensor",
                                         self.name, {'header': hdr})
    def _build_config(self):
        cmdqueue = self.i2c.get_command_queue()
        self.mcu.add_config_cmd("config_mpu9250 oid=%d i2c_oid=%d"
                           % (self.oid, self.i2c.get_oid()))
        self.mcu.add_config_cmd("query_mpu9250 oid=%d rest_ticks=0"
                           % (self.oid,), on_restart=True)
        self.query_mpu9250_cmd = self.mcu.lookup_command(
            "query_mpu9250 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_mpu9250_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)

    def check_connected(self):
        if self.mcu.non_critical_disconnected:
            raise self.printer.command_error(
                f"MPU: {self.name} could not connect because mcu: {self.mcu.get_name()} is non_critical_disconnected!"
            )

    def read_reg(self, reg):
        params = self.i2c.i2c_read([reg], 1)
        return bytearray(params['response'])[0]
    def set_reg(self, reg, val, minclock=0):
        self.i2c.i2c_write([reg, val & 0xFF], minclock=minclock)
    def start_internal_client(self):
        self.check_connected()
        aqh = adxl345.AccelQueryHelper(self.printer)
        self.batch_bulk.add_client(aqh.handle_batch)
        return aqh
    # Measurement decoding
    def _convert_samples(self, samples):
        (x_pos, x_scale), (y_pos, y_scale), (z_pos, z_scale) = self.axes_map
        count = 0
        for ptime, rx, ry, rz in samples:
            raw_xyz = (rx, ry, rz)
            x = round(raw_xyz[x_pos] * x_scale, 6)
            y = round(raw_xyz[y_pos] * y_scale, 6)
            z = round(raw_xyz[z_pos] * z_scale, 6)
            samples[count] = (round(ptime, 6), x, y, z)
            count += 1
    # Start, stop, and process message batches
    def _start_measurements(self):
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
        # Add 20ms pause for accelerometer chip wake up
        self.read_reg(REG_DEVID) # Dummy read to ensure queues flushed
        systime = self.printer.get_reactor().monotonic()
        next_time = self.mcu.estimated_print_time(systime) + 0.020
        self.set_reg(REG_SMPLRT_DIV, SAMPLE_RATE_DIVS[self.data_rate],
                     minclock=self.mcu.print_time_to_clock(next_time))
        self.set_reg(REG_CONFIG, SET_CONFIG)
        self.set_reg(REG_ACCEL_CONFIG, SET_ACCEL_CONFIG)
        self.set_reg(REG_ACCEL_CONFIG2, SET_ACCEL_CONFIG2)
        # Reset fifo
        self.set_reg(REG_FIFO_EN, SET_DISABLE_FIFO)
        self.set_reg(REG_USER_CTRL, SET_USER_FIFO_RESET)
        self.set_reg(REG_USER_CTRL, SET_USER_FIFO_EN)
        self.read_reg(REG_INT_STATUS) # clear FIFO overflow flag

        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(4. / self.data_rate)
        self.query_mpu9250_cmd.send([self.oid, rest_ticks])
        self.set_reg(REG_FIFO_EN, SET_ENABLE_FIFO)
        logging.info("MPU9250 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()
        self.last_error_count = 0
    def _finish_measurements(self):
        # Halt bulk reading
        self.set_reg(REG_FIFO_EN, SET_DISABLE_FIFO)
        self.query_mpu9250_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("MPU9250 finished '%s' measurements", self.name)
        self.set_reg(REG_PWR_MGMT_1, SET_PWR_MGMT_1_SLEEP)
        self.set_reg(REG_PWR_MGMT_2, SET_PWR_MGMT_2_OFF)
    def _process_batch(self, eventtime):
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        if not samples:
            return {}
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}

def load_config(config):
    return MPU9250(config)

def load_config_prefix(config):
    return MPU9250(config)
