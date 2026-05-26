# Support for reading acceleration data from a BMI160 chip
#
# Copyright (C) 2025  Francisco Stephens <francisco.stephens.g@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus, adxl345, bulk_sensor

# BMI160 registers
REG_CHIPID          = 0x00
REG_ACC_DATA_START  = 0x12
REG_ACC_CONF        = 0x40
REG_ACC_RANGE       = 0x41
REG_FIFO_DOWNS      = 0x45
REG_FIFO_CONFIG_0   = 0x46
REG_FIFO_CONFIG_1   = 0x47
REG_FIFO_DATA       = 0x24
REG_FIFO_LENGTH_0   = 0x22
REG_CMD             = 0x7E

REG_MOD_READ        = 0x80

# BMI160 commands for CMD register
CMD_ACC_PM_SUSPEND  = 0x10
CMD_ACC_PM_NORMAL   = 0x11
CMD_FIFO_FLUSH      = 0xB0

# BMI160 constants
BMI160_DEV_ID       = 0xD1
# Target 1600Hz ODR, normal bandwidth, no undersampling
SET_ACC_CONF_1600HZ = 0x2C
# Set accelerometer range to +/-16g
SET_ACC_RANGE_16G   = 0x0C
# Enable accelerometer FIFO, headerless mode
SET_FIFO_CONFIG_1   = 0x40
# No FIFO downsampling
SET_FIFO_DOWNS      = 0x00

# Scale factor for +/-16g range (Datasheet: 2048 LSB/g)
FREEFALL_ACCEL = 9.80665 * 1000.
SCALE = FREEFALL_ACCEL / 2048.

BATCH_UPDATES = 0.100

BMI_I2C_ADDR = 0x69

# Printer class that controls BMI160 chip
class BMI160:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        adxl345.AccelCommandHelper(config, self)
        self.axes_map = adxl345.read_axes_map(config, SCALE, SCALE, SCALE)
        self.data_rate = 1600
        # Setup mcu sensor_bmi160 bulk query code
        # Check for SPI or I2C
        if config.get('cs_pin', None) is not None:
            # Using 1MHz to match working Arduino test
            self.bus = bus.MCU_SPI_from_config(config, 0, default_speed=1000000)
            self.bus_type = 'spi'
        else:
            self.bus = bus.MCU_I2C_from_config(config,
                                default_addr=BMI_I2C_ADDR, default_speed=400000)
            self.bus_type = 'i2c'
        self.mcu = mcu = self.bus.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.query_bmi160_cmd = None
        mcu.add_config_cmd("config_bmi160 oid=%d bus_oid=%d bus_oid_type=%s"
                           % (oid, self.bus.get_oid(), self.bus_type))
        mcu.add_config_cmd("query_bmi160 oid=%d rest_ticks=0"
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
        self.batch_bulk.add_mux_endpoint("bmi160/dump_bmi160", "sensor",
                                         self.name, {'header': hdr})
    def _build_config(self):
        cmdqueue = self.bus.get_command_queue()
        self.query_bmi160_cmd = self.mcu.lookup_command(
            "query_bmi160 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_bmi160_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)
    def read_reg(self, reg):
        if self.bus_type == 'spi':
            params = self.bus.spi_transfer([reg | REG_MOD_READ, 0x00])
            response = bytearray(params['response'])
            return response[1]
        else:
            params = self.bus.i2c_read([reg], 1)
            return bytearray(params['response'])[0]
    def set_reg(self, reg, val, minclock=0):
        if self.bus_type == 'spi':
            # spi_transfer to ensure command completes on MCU before continuing
            self.bus.spi_transfer([reg, val & 0xFF], minclock=minclock)
        else:
            # I2C already waits for completion by default
            self.bus.i2c_write([reg, val & 0xFF], minclock=minclock)
        # Small delay between register writes for stability
        self.reactor.pause(0.002)
        # Don't verify CMD register (0x7E) or registers below 0x40
        if reg >= 0x40 and reg != REG_CMD:
            stored_val = self.read_reg(reg)
            if stored_val != val:
                raise self.printer.command_error(
                        "Failed to set BMI160 register [0x%x] to 0x%x: "
                        "got 0x%x. This is generally indicative of connection "
                        "problems (e.g. faulty wiring) or a faulty bmi160 "
                        "chip." % (reg, val, stored_val))
    def start_internal_client(self):
        aqh = adxl345.AccelQueryHelper(self.printer)
        self.batch_bulk.add_client(aqh.handle_batch)
        return aqh
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
        del samples[count:]
    def _start_measurements(self):
        # 1. Force SPI Mode (Dummy Read)
        if self.bus_type == 'spi':
            self.read_reg(0x7F)
            self.reactor.pause(0.010) # 10ms for mode switch

        # 2. Verify ID
        dev_id = self.read_reg(REG_CHIPID)
        if dev_id != BMI160_DEV_ID:
            raise self.printer.command_error(
                "Invalid bmi160 id (got %x vs %x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty bmi160 chip."
                % (dev_id, BMI160_DEV_ID))

        # 3. Wake Up FIRST
        # Send Normal Mode command
        self.set_reg(REG_CMD, CMD_ACC_PM_NORMAL)
        # CRITICAL: Wait 50ms for startup/PLL locking
        self.reactor.pause(0.050)

        # 4. Configure Registers (While Awake)
        self.set_reg(REG_ACC_CONF, SET_ACC_CONF_1600HZ)
        self.set_reg(REG_ACC_RANGE, SET_ACC_RANGE_16G)
        self.set_reg(REG_FIFO_DOWNS, SET_FIFO_DOWNS)
        self.set_reg(REG_FIFO_CONFIG_1, SET_FIFO_CONFIG_1)

        # 5. Flush FIFO
        self.set_reg(REG_CMD, CMD_FIFO_FLUSH)
        self.reactor.pause(0.010)

        # 6. Start Bulk Reading
        # Start timer roughly immediately
        rest_ticks = self.mcu.seconds_to_clock(4. / self.data_rate)
        self.query_bmi160_cmd.send([self.oid, rest_ticks])
        logging.info("BMI160 starting '%s' measurements", self.name)
        self.ffreader.note_start()
        self.last_error_count = 0

    def _finish_measurements(self):
        self.set_reg(REG_CMD, CMD_ACC_PM_SUSPEND)
        self.query_bmi160_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("BMI160 finished '%s' measurements", self.name)
    def _process_batch(self, eventtime):
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        if not samples:
            return {}
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}

def load_config(config):
    return BMI160(config)

def load_config_prefix(config):
    return BMI160(config)
