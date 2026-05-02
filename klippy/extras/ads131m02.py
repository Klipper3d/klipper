# ADS131M02 2-Channel, Simultaneously-Sampling, 24-Bit ADC
#
# Copyright (C) 2026  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bulk_sensor, bus

#
# Constants
#

# SPI Commands
NULL_CMD    = 0x00
RESET_CMD   = 0x11
STANDBY_CMD = 0x22
WAKEUP_CMD  = 0x33

RESET_ACK = 0xFF22

# Register addresses
REG_ID     = 0x00
REG_STATUS = 0x01
REG_MODE   = 0x02
REG_CLOCK  = 0x03
REG_GAIN1  = 0x04

# Sensor configuration parameters
MODE_VAL = 0x100  # 24-bit words, CRC and timeout disabled
PWR_MODE = 0x2    # High-resolution mode

# OSR settings and corresponding CLOCK register values
OSR_TO_REG = {
    64:    8,  # TBM=1
    128:   0,  # OSR code 000b
    256:   1,  # OSR code 001b
    512:   2,  # OSR code 010b
    1024:  3,  # OSR code 011b
    2048:  4,  # OSR code 100b
    4096:  5,  # OSR code 101b
    8192:  6,  # OSR code 110b
    16256: 7,  # OSR code 111b
}

DEFAULT_CLOCK_FREQ = 8192000
UPDATE_INTERVAL = 0.100
ADS131M02_ID = 0x22

class ADS131M02:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.last_error_count = 0
        self.consecutive_fails = 0

        # Channel selection
        self.channel = config.getint('adc_channel', 0, minval=0, maxval=1)

        # OSR setting
        osr_choices = {str(k): v for k, v in OSR_TO_REG.items()}
        self.osr = config.getchoice('oversampling_ratio',
                                    {str(k): k for k in OSR_TO_REG},
                                    default='1024')

        # Clock frequency
        self.clock_freq = config.getint('clock_freq', DEFAULT_CLOCK_FREQ,
                                        minval=1000000, maxval=16000000)

        # Calculate data rate: fCLKIN / (2 * OSR)
        self.sps = self.clock_freq / (2. * self.osr)

        # Gain setting
        self.gain_options = {'1': 0x00, '2': 0x01, '4': 0x02, '8': 0x03,
                             '16': 0x04, '32': 0x05, '64': 0x06, '128': 0x07}
        self.gain = config.getchoice('gain', self.gain_options, default='1')

        # SPI Setup
        self.spi = bus.MCU_SPI_from_config(config, mode=1,
                                           default_speed=4000000)
        self.mcu = mcu = self.spi.get_mcu()
        self.oid = mcu.create_oid()

        # Data Ready (DRDY) Pin
        drdy_pin = config.get('data_ready_pin')
        ppins = printer.lookup_object('pins')
        drdy_ppin = ppins.lookup_pin(drdy_pin)
        self.data_ready_pin = drdy_ppin['pin']
        drdy_pin_mcu = drdy_ppin['chip']
        if drdy_pin_mcu != self.mcu:
            raise config.error("ADS131M02 config error: SPI communication and"
                               " data_ready_pin must be on the same MCU")

        # Clock tracking
        chip_smooth = self.sps * UPDATE_INTERVAL * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, "<i")

        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch, self._start_measurements,
            self._finish_measurements, UPDATE_INTERVAL)

        # Command Configuration
        mcu.add_config_cmd(
            "config_ads131m02 oid=%d spi_oid=%d channel=%d data_ready_pin=%s"
            % (self.oid, self.spi.get_oid(), self.channel, self.data_ready_pin))
        mcu.add_config_cmd("query_ads131m02 oid=%d rest_ticks=0"
                           % (self.oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        self.query_ads131m02_cmd = None

    def setup_trigger_analog(self, trigger_analog_oid):
        self.mcu.add_config_cmd(
            "ads131m02_attach_trigger_analog oid=%d trigger_analog_oid=%d"
            % (self.oid, trigger_analog_oid), is_init=True)

    def _build_config(self):
        cmdqueue = self.spi.get_command_queue()
        self.query_ads131m02_cmd = self.mcu.lookup_command(
            "query_ads131m02 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_ads131m02_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)

    def get_mcu(self):
        return self.mcu

    def get_samples_per_second(self):
        return round(self.sps)

    def lookup_sensor_error(self, error_code):
        return "Unknown ads131m02 error"

    # Returns a tuple of the minimum and maximum value of the sensor,
    # used to detect if a data value is saturated
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
        self.query_ads131m02_cmd.send([self.oid, rest_ticks])
        logging.info("ADS131M02 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()

    def _finish_measurements(self):
        # Don't use serial connection after shutdown
        if self.printer.is_shutdown():
            return
        # Halt bulk reading
        self.query_ads131m02_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("ADS131M02 finished '%s' measurements", self.name)

    def _process_batch(self, eventtime):
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}

    def _convert_to_spi_frame(self, vals_16):
        result = []
        # Minimum frame length is 4 words
        for val in vals_16 + [0x00] * max(0, 4 - len(vals_16)):
            result.append((val & 0xFF00) >> 8)
            result.append(val & 0xFF)
            result.append(0x00)
        return result

    def reset_chip(self):
        # Read ID register to validate the sensor (upper byte = 0x22)
        id_val = self.read_reg(REG_ID)
        if (id_val >> 8) != ADS131M02_ID:
            if self.mcu.is_fileoutput():
                return
            raise self.printer.command_error(
                    "Invalid ads131m02 ID register (got 0x%x vs 0x22xx).\n"
                    "This is generally indicative of connection problems\n"
                    "(e.g. faulty wiring) or a faulty ADS131M02 chip."
                    % (id_val,))
        # Send RESET command and check acknowledgment
        self.send_command_16(RESET_CMD)
        ack = self.send_command_16(NULL_CMD)
        if ack != RESET_ACK:
            raise self.printer.command_error(
                    "Failed to reset ads131m02 (got 0x%x vs 0x%x).\n"
                    "This is generally indicative of connection problems\n"
                    "(e.g. faulty wiring) or a faulty ADS131M02 chip."
                % (ack, RESET_ACK))

    def setup_chip(self):
        self.write_reg(REG_MODE, MODE_VAL)
        actual_mode_val = self.read_reg(REG_MODE)
        if actual_mode_val != MODE_VAL:
            raise self.printer.command_error(
                    "Failed to set MODE register to %x, got %x"
                    % (MODE_VAL, actual_mode_val))

        # Configure CLOCK register
        # Bit 9: CH1_EN, bit 8: CH0_EN, bit 5: TBM, bits 4:2: OSR, bits 1:0: PWR
        osr_code = OSR_TO_REG[self.osr]
        ch_en = 0x200 if self.channel else 0x100
        clock_val = (osr_code << 2) | ch_en | PWR_MODE
        self.write_reg(REG_CLOCK, clock_val)
        actual_clock_val = self.read_reg(REG_CLOCK)
        if actual_clock_val != clock_val:
            raise self.printer.command_error(
                    "Failed to set CLOCK register to %x, got %x"
                    % (clock_val, actual_clock_val))

        # Configure GAIN1 register
        gain_val = (self.gain << 4) | self.gain
        self.write_reg(REG_GAIN1, gain_val)
        actual_gain_val = self.read_reg(REG_GAIN1)
        if actual_gain_val != gain_val:
            raise self.printer.command_error(
                    "Failed to set GAIN1 register to %x, got %x"
                    % (gain_val, actual_gain_val))

        # Start conversions
        self.send_command_16(WAKEUP_CMD)

    def read_reg(self, reg):
        # RREG command: 101a aaaa annn nnnn
        # a[6:0] = register address (bits 13:7), n[6:0] = count-1
        cmd = 0xA000 | (reg << 7)
        # First transaction: send RREG command as a full-frame SPI transaction
        cmd_bytes = self._convert_to_spi_frame([cmd])
        self.spi.spi_send(cmd_bytes)
        # Second transaction: run NULL_CMD and read the register value
        return self.send_command_16(NULL_CMD)

    def write_reg(self, reg, value):
        # WREG command: 011a aaaa annn nnnn
        # a[6:0] = start address (bits 13:7), n[6:0] = count-1
        cmd = 0x6000 | (reg << 7)
        data = self._convert_to_spi_frame([cmd, value])
        self.spi.spi_send(data)

    def send_command_16(self, cmd):
        # Send 16-bit command as a full-frame SPI transaction
        cmd_bytes = self._convert_to_spi_frame([cmd])
        response = self.spi.spi_transfer(cmd_bytes)
        resp = response['response']
        return (resp[0] << 8) | resp[1]


ADS131M02_SENSOR_TYPE = {"ads131m02": ADS131M02}
