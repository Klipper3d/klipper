# Support for reading acceleration data from an ADXL355 chip
#
# Copyright (C) 2026 Sven Thiele <thiele61@gmx.de>
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

RANGE_2G = 0x01
RANGE_MASK = 0x03
FILTER_ODR_MASK = 0x0F

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
        # ADXL355 requires SPI mode 0 (CPOL=0, CPHA=0).
        self.spi = bus.MCU_SPI_from_config(config, 0, default_speed=5000000)
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
        # Some ADXL355 bits are read-only/reserved and may not round-trip.
        if reg == REG_POWER_CTL and (
                (stored_val & POWER_CTL_STANDBY)
                == (val & POWER_CTL_STANDBY)):
            return
        if reg == REG_RANGE and (stored_val & RANGE_MASK) == (val & RANGE_MASK):
            return
        if reg == REG_FILTER and (
                (stored_val & FILTER_ODR_MASK)
                == (val & FILTER_ODR_MASK)):
            return
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
            # FIFO entries contain a 2-bit marker in the low nibble:
            # bit0 marks x-axis entries, bit1 marks empty FIFO reads.
            entries = ((x3, x2, x1), (y3, y2, y1), (z3, z2, z1))
            markers = [(e[2] & 0x03) for e in entries]
            if any(m & 0x02 for m in markers):
                self.last_error_count += 1
                continue
            xmarkers = [i for i, m in enumerate(markers) if m & 0x01]
            if len(xmarkers) != 1:
                self.last_error_count += 1
                continue
            xidx = xmarkers[0]
            xraw = entries[xidx]
            yraw = entries[(xidx + 1) % 3]
            zraw = entries[(xidx + 2) % 3]
            rx = self._decode_axis(*xraw)
            ry = self._decode_axis(*yraw)
            rz = self._decode_axis(*zraw)
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
        self.set_reg(REG_POWER_CTL, POWER_CTL_STANDBY)
        self.set_reg(REG_RANGE, RANGE_2G)
        self.set_reg(REG_FILTER, self.filter_rate_bits)
        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(5. / self.data_rate)
        self.query_adxl355_cmd.send([self.oid, rest_ticks])
        self.set_reg(REG_POWER_CTL, 0x00)
        logging.info("ADXL355 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()
        self.last_error_count = 0

    def _finish_measurements(self):
        # Halt bulk reading
        self.set_reg(REG_POWER_CTL, POWER_CTL_STANDBY)
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
