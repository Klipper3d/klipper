# Support for reading acceleration data from an icm20948 chip
#
# Copyright (C) 2024 Paul Hansel <github@paulhansel.com>
# Copyright (C) 2022  Harry Beyel <harry3b9@gmail.com>
# Copyright (C) 2020-2021 Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

# From https://invensense.tdk.com/wp-content/uploads/
#               2016/06/DS-000189-ICM-20948-v1.3.pdf

import logging
from . import bus, adxl345, bulk_sensor

ICM20948_ADDR =         0x68

ICM_DEV_IDS = {
    0xEA: "icm-20948",
    #everything above are normal ICM IDs
}

# ICM20948 registers
REG_DEVID =             0x00 # 0xEA
REG_FIFO_EN =           0x67 # FIFO_EN_2
REG_ACCEL_SMPLRT_DIV1 = 0x10 # MSB
REG_ACCEL_SMPLRT_DIV2 = 0x11 # LSB
REG_ACCEL_CONFIG =      0x14
REG_USER_CTRL =         0x03
REG_PWR_MGMT_1 =        0x06
REG_PWR_MGMT_2 =        0x07
REG_INT_STATUS =        0x19
REG_BANK_SEL =          0x7F

SAMPLE_RATE_DIVS = { 4500: 0x00 }

SET_BANK_0 =            0x00
SET_BANK_1 =            0x10
SET_BANK_2 =            0x20
SET_BANK_3 =            0x30
SET_ACCEL_CONFIG =      0x06 # 16g full scale, 1209Hz BW, 4.5kHz samp rate
SET_PWR_MGMT_1_WAKE =   0x01
SET_PWR_MGMT_1_SLEEP =  0x41
SET_PWR_MGMT_2_ACCEL_ON = 0x07
SET_PWR_MGMT_2_OFF =    0x3F
SET_USER_FIFO_RESET =   0x0E
SET_USER_FIFO_EN =      0x40
SET_ENABLE_FIFO  =      0x10
SET_DISABLE_FIFO =      0x00

FREEFALL_ACCEL = 9.80665 * 1000.
# SCALE = 1/2048 g/LSB @16g scale * Earth gravity in mm/s**2
SCALE = 0.00048828125 * FREEFALL_ACCEL

FIFO_SIZE = 512

BATCH_UPDATES = 0.100

# Printer class that controls ICM20948 chip
class ICM20948:
    def __init__(self, config):
        self.printer = config.get_printer()
        adxl345.AccelCommandHelper(config, self)
        self.axes_map = adxl345.read_axes_map(config, SCALE, SCALE, SCALE)
        self.data_rate = config.getint('rate', 4500)
        if self.data_rate not in SAMPLE_RATE_DIVS:
            raise config.error("Invalid rate parameter: %d" % (self.data_rate,))
        # Setup mcu sensor_icm20948 bulk query code
        self.i2c = bus.MCU_I2C_from_config(config,
                                           default_addr=ICM20948_ADDR,
                                           default_speed=400000)
        self.mcu = mcu = self.i2c.get_mcu()
        self.oid = mcu.create_oid()
        self.query_icm20948_cmd = None
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
        self.batch_bulk.add_mux_endpoint("icm20948/dump_icm20948", "sensor",
                                         self.name, {'header': hdr})
    def _build_config(self):
        cmdqueue = self.i2c.get_command_queue()
        self.mcu.add_config_cmd("config_icm20948 oid=%d i2c_oid=%d"
                           % (self.oid, self.i2c.get_oid()))
        self.mcu.add_config_cmd("query_icm20948 oid=%d rest_ticks=0"
                           % (self.oid,), on_restart=True)
        self.query_icm20948_cmd = self.mcu.lookup_command(
            "query_icm20948 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_icm20948_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)
    def read_reg(self, reg):
        params = self.i2c.i2c_read([reg], 1)
        return bytearray(params['response'])[0]
    def set_reg(self, reg, val, minclock=0):
        self.i2c.i2c_write([reg, val & 0xFF], minclock=minclock)
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
        # In case of miswiring, testing ICM20948 device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        dev_id = self.read_reg(REG_DEVID)
        if dev_id not in ICM_DEV_IDS.keys():
            raise self.printer.command_error(
                "Invalid mpu id (got %x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty chip."
                % (dev_id))
        else:
            logging.info("Found %s with id %x"% (ICM_DEV_IDS[dev_id], dev_id))
        # Setup chip in requested query rate
        self.set_reg(REG_PWR_MGMT_1, SET_PWR_MGMT_1_WAKE)
        self.set_reg(REG_PWR_MGMT_2, SET_PWR_MGMT_2_ACCEL_ON)
        # Don't add 20ms pause for accelerometer chip wake up
        self.read_reg(REG_DEVID) # Dummy read to ensure queues flushed
        self.set_reg(REG_ACCEL_SMPLRT_DIV1, SAMPLE_RATE_DIVS[self.data_rate])
        self.set_reg(REG_ACCEL_SMPLRT_DIV2, SAMPLE_RATE_DIVS[self.data_rate])
        self.set_reg(REG_BANK_SEL, SET_BANK_2)
        self.set_reg(REG_ACCEL_CONFIG, SET_ACCEL_CONFIG)
        self.set_reg(REG_BANK_SEL, SET_BANK_0)
        # Reset fifo
        self.set_reg(REG_FIFO_EN, SET_DISABLE_FIFO)
        self.set_reg(REG_USER_CTRL, SET_USER_FIFO_RESET)
        self.set_reg(REG_USER_CTRL, SET_USER_FIFO_EN)
        self.read_reg(REG_INT_STATUS) # clear FIFO overflow flag
        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(4. / self.data_rate)
        self.query_icm20948_cmd.send([self.oid, rest_ticks])
        self.set_reg(REG_FIFO_EN, SET_ENABLE_FIFO)
        logging.info("ICM20948 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()
        self.last_error_count = 0
    def _finish_measurements(self):
        # Halt bulk reading
        self.set_reg(REG_FIFO_EN, SET_DISABLE_FIFO)
        self.query_icm20948_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("ICM20948 finished '%s' measurements", self.name)
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
    return ICM20948(config)

def load_config_prefix(config):
    return ICM20948(config)
