# ADS1220 Support
#
# Copyright (C) 2024 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bulk_sensor, bus

#
# Constants
#
BYTES_PER_SAMPLE = 4  # samples are 4 byte wide unsigned integers
MAX_SAMPLES_PER_MESSAGE = bulk_sensor.MAX_BULK_MSG_SIZE // BYTES_PER_SAMPLE
UPDATE_INTERVAL = 0.10
RESET_CMD = 0x06
START_SYNC_CMD = 0x08
RREG_CMD = 0x20
WREG_CMD = 0x40
NOOP_CMD = 0x0
RESET_STATE = bytearray([0x0, 0x0, 0x0, 0x0])

# turn bytearrays into pretty hex strings: [0xff, 0x1]
def hexify(byte_array):
    return "[%s]" % (", ".join([hex(b) for b in byte_array]))


class ADS1220():
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.last_error_count = 0
        self.consecutive_fails = 0
        # Chip options
        # Gain
        self.gain_options = {'1': 0x0, '2': 0x1, '4': 0x2, '8': 0x3, '16': 0x4,
                             '32': 0x5, '64': 0x6, '128': 0x7}
        self.gain = config.getchoice('gain', self.gain_options, default='128')
        # Sample rate
        self.sps_normal = {'20': 20, '45': 45, '90': 90, '175': 175,
                           '330': 330, '600': 600, '1000': 1000}
        self.sps_turbo = {'40': 40, '90': 90, '180': 180, '350': 350,
                          '660': 660, '1200': 1200, '2000': 2000}
        self.sps_options = self.sps_normal.copy()
        self.sps_options.update(self.sps_turbo)
        self.sps = config.getchoice('sps', self.sps_options, default='660')
        self.is_turbo = str(self.sps) in self.sps_turbo
        # SPI Setup
        spi_speed = 512000 if self.is_turbo else 256000
        self.spi = bus.MCU_SPI_from_config(config, 1, default_speed=spi_speed)
        self.mcu = mcu = self.spi.get_mcu()
        self.oid = mcu.create_oid()
        # Data Ready (DRDY) Pin
        drdy_pin = config.get('data_ready_pin')
        ppins = printer.lookup_object('pins')
        drdy_ppin = ppins.lookup_pin(drdy_pin)
        self.data_ready_pin = drdy_ppin['pin']
        drdy_pin_mcu = drdy_ppin['chip']
        if drdy_pin_mcu != self.mcu:
            raise config.error("ADS1220 config error: SPI communication and"
                               " data_ready_pin must be on the same MCU")
        # Bulk Sensor Setup
        self.bulk_queue = bulk_sensor.BulkDataQueue(self.mcu, oid=self.oid)
        # Clock tracking
        chip_smooth = self.sps * UPDATE_INTERVAL * 2
        # Measurement conversion
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, "<i")
        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch, self._start_measurements,
            self._finish_measurements, UPDATE_INTERVAL)
        # publish raw samples to the socket
        self.batch_bulk.add_mux_endpoint("ads1220/dump_ads1220", "sensor",
                                         self.name,
                                         {'header': ('time', 'counts')})
        # Command Configuration
        mcu.add_config_cmd(
            "config_ads1220 oid=%d spi_oid=%d data_ready_pin=%s"
            % (self.oid, self.spi.get_oid(), self.data_ready_pin))
        mcu.add_config_cmd("query_ads1220 oid=%d rest_ticks=0"
                           % (self.oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        self.query_ads1220_cmd = None

    def _build_config(self):
        cmdqueue = self.spi.get_command_queue()
        self.query_ads1220_cmd = self.mcu.lookup_command(
            "query_ads1220 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_ads1220_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)

    def get_mcu(self):
        return self.mcu

    def get_samples_per_second(self):
        return self.sps

    # returns a tuple of the minimum and maximum value of the sensor, used to
    # detect if a data value is saturated
    def get_range(self):
        return -0x800000, 0x7FFFFF

    # add_client interface, direct pass through to bulk_sensor API
    def add_client(self, callback):
        self.batch_bulk.add_client(callback)

    # Measurement decoding
    def _convert_samples(self, samples):
        adc_factor = 1. / (1 << 23)
        count = 0
        for ptime, val in samples:
            samples[count] = (round(ptime, 6), val, round(val * adc_factor, 9))
            count += 1
        del samples[count:]

    # Start, stop, and process message batches
    def _start_measurements(self):
        self.last_error_count = 0
        self.consecutive_fails = 0
        # Start bulk reading
        self.reset_chip()
        self.setup_chip()
        rest_ticks = self.mcu.seconds_to_clock(1. / (10. * self.sps))
        self.query_ads1220_cmd.send([self.oid, rest_ticks])
        logging.info("ADS1220 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()

    def _finish_measurements(self):
        # don't use serial connection after shutdown
        if self.printer.is_shutdown():
            return
        # Halt bulk reading
        self.query_ads1220_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("ADS1220 finished '%s' measurements", self.name)

    def _process_batch(self, eventtime):
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}

    def reset_chip(self):
        # the reset command takes 50us to complete
        self.send_command(RESET_CMD)
        # read startup register state and validate
        val = self.read_reg(0x0, 4)
        if val != RESET_STATE:
            raise self.printer.command_error(
                "Invalid ads1220 reset state (got %s vs %s).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty ADS1220 chip."
                % (hexify(val), hexify(RESET_STATE)))

    def setup_chip(self):
        continuous = 0x1  # enable continuous conversions
        mode = 0x2 if self.is_turbo else 0x0  # turbo mode
        sps_list = self.sps_turbo if self.is_turbo else self.sps_normal
        data_rate = list(sps_list.keys()).index(str(self.sps))
        reg_values = [(self.gain << 1),
                      (data_rate << 5) | (mode << 3) | (continuous << 2)]
        self.write_reg(0x0, reg_values)
        # start measurements immediately
        self.send_command(START_SYNC_CMD)

    def read_reg(self, reg, byte_count):
        read_command = [RREG_CMD | (reg << 2) | (byte_count - 1)]
        read_command += [NOOP_CMD] * byte_count
        params = self.spi.spi_transfer(read_command)
        return bytearray(params['response'][1:])

    def send_command(self, cmd):
        self.spi.spi_send([cmd])

    def write_reg(self, reg, register_bytes):
        write_command = [WREG_CMD | (reg << 2) | (len(register_bytes) - 1)]
        write_command.extend(register_bytes)
        self.spi.spi_send(write_command)
        stored_val = self.read_reg(reg, len(register_bytes))
        if register_bytes != stored_val:
            raise self.printer.command_error(
                "Failed to set ADS1220 register [0x%x] to %s: got %s. "
                "This may be a connection problem (e.g. faulty wiring)" % (
                    reg, hexify(register_bytes), hexify(stored_val)))


ADS1220_SENSOR_TYPE = {"ads1220": ADS1220}
