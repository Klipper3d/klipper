# Support for reading acceleration data from an adxl345 chip
#
# Copyright (C) 2020-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from .. import bus
from .motion_sensor import MotionSensorBase, FREEFALL_ACCEL

# ADXL345 registers
REG_DEVID = 0x00
REG_BW_RATE = 0x2C
REG_POWER_CTL = 0x2D
REG_DATA_FORMAT = 0x31
REG_FIFO_CTL = 0x38
REG_MOD_READ = 0x80
REG_MOD_MULTI = 0x40

ADXL345_DEV_ID = 0xe5
SET_FIFO_CTL = 0x90

# Printer class that controls ADXL345 chip
class ADXL345 (MotionSensorBase):
    # SCALE = 3.9mg/LSB * Earth gravity in mm/s**2
    SCALE = 0.0039 * FREEFALL_ACCEL
    BYTES_PER_SAMPLE = 5
    SAMPLES_PER_BLOCK = 10
    FIFO_SIZE = 32 # in number of samples

    SAMPLE_RATES = {
        25: 0x8, 50: 0x9, 100: 0xa, 200: 0xb, 400: 0xc,
        800: 0xd, 1600: 0xe, 3200: 0xf,
    }

    def __init__(self, config):
        super(ADXL345, self).__init__(config)
        self.data_rate = config.getint('rate', 3200)
        if self.data_rate not in self.SAMPLE_RATES:
            raise config.error("Invalid rate parameter: %d" % (self.data_rate))
        self.mcu.register_response(self._handle_motion_sensor_data,
                                    "adxl345_data", self.oid)

    def _init_conn(self, config):
        logging.log(logging.INFO, "Setting up SPI connection for ADXL345")
        # Setup mcu sensor_mpu9250 bulk query code
        self.conn = bus.MCU_SPI_from_config(config, 3, default_speed=5000000)

    def _build_config(self):
        cmdqueue = self.conn.get_command_queue()
        self.query_adxl345_cmd = self.mcu.lookup_command(
            "query_adxl345 oid=%c clock=%u rest_ticks=%u", cq=cmdqueue)
        self.query_adxl345_end_cmd = self.mcu.lookup_query_command(
            "query_adxl345 oid=%c clock=%u rest_ticks=%u",
            "adxl345_status oid=%c clock=%u query_ticks=%u next_sequence=%hu"
            " buffered=%c fifo=%c limit_count=%hu", oid=self.oid, cq=cmdqueue)
        self.query_adxl345_status_cmd = self.mcu.lookup_query_command(
            "query_adxl345_status oid=%c",
            "adxl345_status oid=%c clock=%u query_ticks=%u next_sequence=%hu"
            " buffered=%c fifo=%c limit_count=%hu", oid=self.oid, cq=cmdqueue)

    def read_reg(self, reg):
        params = self.conn.spi_transfer([reg | REG_MOD_READ, 0x00])
        response = bytearray(params['response'])
        return response[1]

    def set_reg(self, reg, val, minclock=0):
        self.conn.spi_send([reg, val & 0xFF], minclock=minclock)
        stored_val = self.read_reg(reg)
        if stored_val != val:
            raise self.printer.command_error(
                    "Failed to set ADXL345 register [0x%x] to 0x%x: got 0x%x. "
                    "This is generally indicative of connection problems "
                    "(e.g. faulty wiring) or a faulty adxl345 chip." % (
                        reg, val, stored_val))

    def _extract_samples(self, raw_samples):
        # Load variables to optimize inner loop below
        (x_pos, x_scale), (y_pos, y_scale), (z_pos, z_scale) = self.axes_map
        last_sequence = self.last_sequence
        time_base, chip_base, inv_freq = self.clock_sync.get_time_translation()
        # Process every message in raw_samples
        count = seq = 0
        samples = [None] * (len(raw_samples) * self.SAMPLES_PER_BLOCK)
        for params in raw_samples:
            seq_diff = (last_sequence - params['sequence']) & 0xffff
            seq_diff -= (seq_diff & 0x8000) << 1
            seq = last_sequence - seq_diff
            d = bytearray(params['data'])
            msg_cdiff = seq * self.SAMPLES_PER_BLOCK - chip_base
            for i in range(len(d) // self.BYTES_PER_SAMPLE):
                d_xyz = d[i*self.BYTES_PER_SAMPLE:(i+1)*self.BYTES_PER_SAMPLE]
                xlow, ylow, zlow, xzhigh, yzhigh = d_xyz
                if yzhigh & 0x80:
                    self.last_error_count += 1
                    continue
                rx = (xlow | ((xzhigh & 0x1f) << 8)) - ((xzhigh & 0x10) << 9)
                ry = (ylow | ((yzhigh & 0x1f) << 8)) - ((yzhigh & 0x10) << 9)
                rz = ((zlow | ((xzhigh & 0xe0) << 3) | ((yzhigh & 0xe0) << 6))
                      - ((yzhigh & 0x40) << 7))
                raw_xyz = (rx, ry, rz)
                x = round(raw_xyz[x_pos] * x_scale, 6)
                y = round(raw_xyz[y_pos] * y_scale, 6)
                z = round(raw_xyz[z_pos] * z_scale, 6)
                ptime = round(time_base + (msg_cdiff + i) * inv_freq, 6)
                samples[count] = (ptime, x, y, z)
                count += 1
        self.clock_sync.set_last_chip_clock(seq * self.SAMPLES_PER_BLOCK + i)
        del samples[count:]
        return samples

    def _update_clock(self, minclock=0):
        # Query current state
        for retry in range(5):
            params = self.query_adxl345_status_cmd.send([self.oid],
                                                        minclock=minclock)
            fifo = params['fifo'] & 0x7f
            if fifo <= 32:
                break
        else:
            raise self.printer.command_error("Unable to query adxl345 fifo")
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
        msg_count = (sequence * self.SAMPLES_PER_BLOCK
                     + buffered // self.BYTES_PER_SAMPLE + fifo)
        # The "chip clock" is the message counter plus .5 for average
        # inaccuracy of query responses and plus .5 for assumed offset
        # of adxl345 hw processing time.
        chip_clock = msg_count + 1
        self.clock_sync.update(mcu_clock + duration // 2, chip_clock)

    def _start_measurements(self):
        if self.is_measuring():
            return
        # In case of miswiring, testing ADXL345 device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        dev_id = self.read_reg(REG_DEVID)
        if dev_id != ADXL345_DEV_ID:
            raise self.printer.command_error(
                "Invalid adxl345 id (got %x vs %x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty adxl345 chip."
                % (dev_id, ADXL345_DEV_ID))
        super(ADXL345, self)._start_measurements()

    def _configure_sensor(self):
        # Setup chip with requested query rate
        self.set_reg(REG_POWER_CTL, 0x00)
        self.set_reg(REG_DATA_FORMAT, 0x0B)
        self.set_reg(REG_FIFO_CTL, 0x00)
        self.set_reg(REG_BW_RATE, self.SAMPLE_RATES[self.data_rate])
        self.set_reg(REG_FIFO_CTL, SET_FIFO_CTL)
