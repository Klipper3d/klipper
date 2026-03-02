# Support for reading acceleration data from an ADXL355 chip
#
# Copyright (C) 2026  Sven
# Copyright (C) 2020-2023  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import adxl345, bus, bulk_sensor

# ADXL355 registers
REG_DEVID_AD = 0x00
REG_DEVID_MST = 0x01
REG_PARTID = 0x02
REG_FILTER = 0x28
REG_RANGE = 0x2C
REG_POWER_CTL = 0x2D

REG_MOD_READ = 0x01

QUERY_RATES = [
    (4000., 0x00), (2000., 0x01), (1000., 0x02), (500., 0x03),
    (250., 0x04), (125., 0x05), (62.5, 0x06), (31.25, 0x07),
    (15.625, 0x08), (7.813, 0x09), (3.906, 0x0a),
]

ADXL355_DEV_ID = (0xAD, 0x1D, 0xED)

POWER_CTL_STANDBY = 0x01
POWER_CTL_TEMP_OFF = 0x02
POWER_CTL_DRDY_OFF = 0x04

RANGE_2G = 0x01

FREEFALL_ACCEL = 9.80665 * 1000.
SCALE = FREEFALL_ACCEL / 256000.

BATCH_UPDATES = 0.100


def _lookup_query_rate(config):
    req_rate = config.getfloat('rate', 4000.)
    for data_rate, bits in QUERY_RATES:
        if abs(req_rate - data_rate) <= .001:
            return data_rate, bits
    valid = ", ".join(["%g" % (r,) for r, _bits in QUERY_RATES])
    raise config.error("Invalid rate parameter: %s (valid: %s)"
                       % (req_rate, valid))


# Printer class that controls ADXL355 chip
class ADXL355:
    def __init__(self, config):
        self.printer = config.get_printer()
        adxl345.AccelCommandHelper(config, self)
        self.axes_map = adxl345.read_axes_map(config, SCALE, SCALE, SCALE)
        self.data_rate, self.filter_rate_bits = _lookup_query_rate(config)
        # Setup mcu sensor_adxl355 bulk query code
        self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=5000000)
        self.mcu = mcu = self.spi.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.query_adxl355_cmd = None
        mcu.add_config_cmd("config_adxl355 oid=%d spi_oid=%d"
                           % (oid, self.spi.get_oid()))
        mcu.add_config_cmd("query_adxl355 oid=%d rest_ticks=0"
                           % (oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        # Bulk sample message reading
        chip_smooth = self.data_rate * BATCH_UPDATES * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth,
                                                     "BBBBBBBBB")
        self.last_error_count = 0
        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch,
            self._start_measurements, self._finish_measurements, BATCH_UPDATES)
        self.name = config.get_name().split()[-1]
        hdr = ('time', 'x_acceleration', 'y_acceleration', 'z_acceleration')
        self.batch_bulk.add_mux_endpoint("adxl355/dump_adxl355", "sensor",
                                         self.name, {'header': hdr})

    def _build_config(self):
        cmdqueue = self.spi.get_command_queue()
        self.query_adxl355_cmd = self.mcu.lookup_command(
            "query_adxl355 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_adxl355_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)

    def read_reg(self, reg):
        params = self.spi.spi_transfer([((reg & 0x7f) << 1) | REG_MOD_READ,
                                        0x00])
        response = bytearray(params['response'])
        return response[1]

    def set_reg(self, reg, val, minclock=0):
        reg_addr = ((reg & 0x7f) << 1) & ~REG_MOD_READ
        self.spi.spi_send([reg_addr, val & 0xFF], minclock=minclock)
        stored_val = self.read_reg(reg)
        if stored_val != val:
            raise self.printer.command_error(
                "Failed to set ADXL355 register [0x%x] to 0x%x: got 0x%x. "
                "This is generally indicative of connection problems "
                "(e.g. faulty wiring) or a faulty adxl355 chip." % (
                    reg, val, stored_val))

    def start_internal_client(self):
        aqh = adxl345.AccelQueryHelper(self.printer)
        self.batch_bulk.add_client(aqh.handle_batch)
        return aqh

    @staticmethod
    def _decode_axis(msb, csb, lsb):
        raw = ((msb << 16) | (csb << 8) | lsb) >> 4
        if raw & 0x80000:
            raw -= (1 << 20)
        return raw

    # Measurement decoding
    def _convert_samples(self, samples):
        (x_pos, x_scale), (y_pos, y_scale), (z_pos, z_scale) = self.axes_map
        count = 0
        for ptime, x3, x2, x1, y3, y2, y1, z3, z2, z1 in samples:
            if (x1 & 0x0f) or (y1 & 0x0f) or (z1 & 0x0f):
                # Invalid framing in the 20-bit sample payload.
                self.last_error_count += 1
                continue
            rx = self._decode_axis(x3, x2, x1)
            ry = self._decode_axis(y3, y2, y1)
            rz = self._decode_axis(z3, z2, z1)
            raw_xyz = (rx, ry, rz)
            x = round(raw_xyz[x_pos] * x_scale, 6)
            y = round(raw_xyz[y_pos] * y_scale, 6)
            z = round(raw_xyz[z_pos] * z_scale, 6)
            samples[count] = (round(ptime, 6), x, y, z)
            count += 1
        del samples[count:]

    def _check_device_id(self):
        dev_id = self.read_reg(REG_DEVID_AD)
        mst_id = self.read_reg(REG_DEVID_MST)
        part_id = self.read_reg(REG_PARTID)
        if (dev_id, mst_id, part_id) != ADXL355_DEV_ID:
            raise self.printer.command_error(
                "Invalid adxl355 id (got ad=%x mst=%x part=%x "
                "vs expected ad=%x mst=%x part=%x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty adxl355 chip."
                % ((dev_id, mst_id, part_id) + ADXL355_DEV_ID))

    # Start, stop, and process message batches
    def _start_measurements(self):
        # In case of miswiring, test ADXL355 device IDs before measuring.
        self._check_device_id()
        # Setup chip in standby and configure output data rate and range.
        self.set_reg(REG_POWER_CTL,
                     POWER_CTL_STANDBY | POWER_CTL_TEMP_OFF | POWER_CTL_DRDY_OFF)
        self.set_reg(REG_RANGE, RANGE_2G)
        self.set_reg(REG_FILTER, self.filter_rate_bits)
        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(1. / self.data_rate)
        self.query_adxl355_cmd.send([self.oid, rest_ticks])
        self.set_reg(REG_POWER_CTL, POWER_CTL_TEMP_OFF | POWER_CTL_DRDY_OFF)
        logging.info("ADXL355 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()
        self.last_error_count = 0

    def _finish_measurements(self):
        # Halt bulk reading
        self.set_reg(REG_POWER_CTL,
                     POWER_CTL_STANDBY | POWER_CTL_TEMP_OFF | POWER_CTL_DRDY_OFF)
        self.query_adxl355_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("ADXL355 finished '%s' measurements", self.name)

    def _process_batch(self, eventtime):
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        if not samples:
            return {}
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}


def load_config(config):
    return ADXL355(config)


def load_config_prefix(config):
    return ADXL355(config)
