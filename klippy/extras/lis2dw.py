# Support for reading acceleration data from an LIS2DW chip
#
# Copyright (C) 2023  Zhou.XianMing <zhouxm@biqu3d.com>
# Copyright (C) 2020-2023  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus, adxl345, bulk_sensor

# LIS2DW registers
REG_LIS2DW_WHO_AM_I_ADDR = 0x0F
REG_LIS2DW_CTRL_REG1_ADDR = 0x20
REG_LIS2DW_CTRL_REG2_ADDR = 0x21
REG_LIS2DW_CTRL_REG3_ADDR = 0x22
REG_LIS2DW_CTRL_REG6_ADDR = 0x25
REG_LIS2DW_STATUS_REG_ADDR = 0x27
REG_LIS2DW_OUT_XL_ADDR = 0x28
REG_LIS2DW_OUT_XH_ADDR = 0x29
REG_LIS2DW_OUT_YL_ADDR = 0x2A
REG_LIS2DW_OUT_YH_ADDR = 0x2B
REG_LIS2DW_OUT_ZL_ADDR = 0x2C
REG_LIS2DW_OUT_ZH_ADDR = 0x2D
REG_LIS2DW_FIFO_CTRL   = 0x2E
REG_LIS2DW_FIFO_SAMPLES = 0x2F
REG_MOD_READ = 0x80
# REG_MOD_MULTI = 0x40

LIS2DW_DEV_ID = 0x44

FREEFALL_ACCEL = 9.80665
SCALE = FREEFALL_ACCEL * 1.952 / 4

BATCH_UPDATES = 0.100

# Printer class that controls LIS2DW chip
class LIS2DW:
    def __init__(self, config):
        self.printer = config.get_printer()
        adxl345.AccelCommandHelper(config, self)
        self.axes_map = adxl345.read_axes_map(config, SCALE, SCALE, SCALE)
        self.data_rate = 1600
        # Setup mcu sensor_lis2dw bulk query code
        self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=5000000)
        self.mcu = mcu = self.spi.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.query_lis2dw_cmd = None
        mcu.add_config_cmd("config_lis2dw oid=%d spi_oid=%d"
                           % (oid, self.spi.get_oid()))
        mcu.add_config_cmd("query_lis2dw oid=%d rest_ticks=0"
                           % (oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        # Bulk sample message reading
        chip_smooth = self.data_rate * BATCH_UPDATES * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, "<hhh")
        self.last_error_count = 0
        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch,
            self._start_measurements, self._finish_measurements, BATCH_UPDATES)
        self.name = config.get_name().split()[-1]
        hdr = ('time', 'x_acceleration', 'y_acceleration', 'z_acceleration')
        self.batch_bulk.add_mux_endpoint("lis2dw/dump_lis2dw", "sensor",
                                         self.name, {'header': hdr})

    def _build_config(self):
        cmdqueue = self.spi.get_command_queue()
        self.query_lis2dw_cmd = self.mcu.lookup_command(
            "query_lis2dw oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_lis2dw_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)
    def read_reg(self, reg):
        params = self.spi.spi_transfer([reg | REG_MOD_READ, 0x00])
        response = bytearray(params['response'])
        return response[1]
    def set_reg(self, reg, val, minclock=0):
        self.spi.spi_send([reg, val & 0xFF], minclock=minclock)
        stored_val = self.read_reg(reg)
        if stored_val != val:
            raise self.printer.command_error(
                    "Failed to set LIS2DW register [0x%x] to 0x%x: got 0x%x. "
                    "This is generally indicative of connection problems "
                    "(e.g. faulty wiring) or a faulty lis2dw chip." % (
                        reg, val, stored_val))
    def start_internal_client(self):
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
        # In case of miswiring, testing LIS2DW device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        dev_id = self.read_reg(REG_LIS2DW_WHO_AM_I_ADDR)
        logging.info("lis2dw_dev_id: %x", dev_id)
        if dev_id != LIS2DW_DEV_ID:
            raise self.printer.command_error(
                "Invalid lis2dw id (got %x vs %x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty lis2dw chip."
                % (dev_id, LIS2DW_DEV_ID))
        # Setup chip in requested query rate
        # ODR/2, +-16g, low-pass filter, Low-noise abled
        self.set_reg(REG_LIS2DW_CTRL_REG6_ADDR, 0x34)
        # Continuous mode: If the FIFO is full
        # the new sample overwrites the older sample.
        self.set_reg(REG_LIS2DW_FIFO_CTRL, 0xC0)
        # High-Performance / Low-Power mode 1600/200 Hz
        # High-Performance Mode (14-bit resolution)
        self.set_reg(REG_LIS2DW_CTRL_REG1_ADDR, 0x94)

        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(4. / self.data_rate)
        self.query_lis2dw_cmd.send([self.oid, rest_ticks])
        self.set_reg(REG_LIS2DW_FIFO_CTRL, 0xC0)
        logging.info("LIS2DW starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()
        self.last_error_count = 0
    def _finish_measurements(self):
        # Halt bulk reading
        self.set_reg(REG_LIS2DW_FIFO_CTRL, 0x00)
        self.query_lis2dw_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("LIS2DW finished '%s' measurements", self.name)
        self.set_reg(REG_LIS2DW_FIFO_CTRL, 0x00)
    def _process_batch(self, eventtime):
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        if not samples:
            return {}
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}

def load_config(config):
    return LIS2DW(config)

def load_config_prefix(config):
    return LIS2DW(config)
