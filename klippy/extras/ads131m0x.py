# ADS131M0x support (ADS131M02, ADS131M04)
#
# Copyright (C) 2024-2025 Gareth Farrington <gareth@waves.ky>
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

T_REGACQ = 5e-6  # 5us minimum delay after RESET

# Register addresses
REG_ID     = 0x00
REG_STATUS = 0x01
REG_MODE   = 0x02
REG_CLOCK  = 0x03
REG_GAIN1  = 0x04
REG_CFG    = 0x06

# Sensor configuration parameters
WORD24_MODE = 0x100  # 24-bit words
PWR_MODE    = 0x2    # High-resolution mode
GC_MODE     = 0x100  # Global-chop mode

# A mask for STATUS fields to validate
STATUS_REG_MASK = 0xBFFC  # Ignore F_RESYNC and DRDYx bits

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
    16384: 7,  # OSR code 111b
}

# GAIN settings and corresponding GAIN1 register values
GAIN_TO_REG = {
    1:   0x00,
    2:   0x01,
    4:   0x02,
    8:   0x03,
    16:  0x04,
    32:  0x05,
    64:  0x06,
    128: 0x07,
}

# Global-chop delay settings (modulator clock periods)
GC_DLY_TO_REG = {
    2:     0x00,
    4:     0x01,
    8:     0x02,
    16:    0x03,
    32:    0x04,
    64:    0x05,
    128:   0x06,
    256:   0x07,
    512:   0x08,
    1024:  0x09,
    2048:  0x0a,
    4096:  0x0b,
    8192:  0x0c,
    16384: 0x0d,
    32768: 0x0e,
    65536: 0x0f,
}

UPDATE_INTERVAL = 0.100
MINIMUM_CLOCK_FREQ = 300000
MAXIMUM_CLOCK_FREQ = 8400000
NOMINAL_CLOCK_FREQ = 8192000
MAX_SAMPLE_RATE_DEVIATION = 0.5

class ADS131M0X:
    def __init__(self, config, sensor_type, num_channels):
        self.printer = printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.last_error_count = 0
        self.consecutive_fails = 0
        self.sensor_type = sensor_type
        self.sensor_id = 0x20 | num_channels
        self._sensor_errors = {}

        # Channel selection
        self.num_channels = num_channels
        self.channel = config.getint('adc_channel', 0,
                                    minval=0, maxval=num_channels-1)

        # Clock frequency
        pwm_clock_name = config.get('pwm_clock', None)
        if pwm_clock_name is not None:
            pwm_config = config.getsection(' '.join(pwm_clock_name.split()))
            self.clock_freq = pwm_config.getfloat('frequency', None,
                                                  note_valid=False,
                                                  minval=MINIMUM_CLOCK_FREQ,
                                                  maxval=MAXIMUM_CLOCK_FREQ)
            if self.clock_freq is None:
                raise config.error(
                        "pwm_clock '%s' must support and specify a 'frequency'"
                        " parameter" % (pwm_clock_name,))
        else:
            self.clock_freq = config.getint('clock_freq',
                                            minval=MINIMUM_CLOCK_FREQ,
                                            maxval=MAXIMUM_CLOCK_FREQ)

        # Gain setting
        self.gain = config.getchoice('gain', list(GAIN_TO_REG.keys()), 128)

        # Global-chop configuration
        self.enable_global_chop = config.getboolean('enable_global_chop', False)
        if self.enable_global_chop:
            self.gc_dly = config.getchoice('global_chop_delay',
                                           list(GC_DLY_TO_REG.keys()), 16)
        else:
            self.gc_dly = None

        # Sample rate setting
        config_sample_rate = config.getfloat('sample_rate', above=0.0,
                                             default=500.0)

        # Find OSR that produces SPS closest to requested sample rate
        best_osr = best_sps = None
        for osr in OSR_TO_REG:
            sps = self._calc_sample_rate(osr)
            if best_sps is None or abs(sps - config_sample_rate) < \
                    abs(best_sps - config_sample_rate):
                best_osr, best_sps = osr, sps
        if abs(best_sps - config_sample_rate) >= \
                MAX_SAMPLE_RATE_DEVIATION * config_sample_rate:
            raise config.error(
                "Requested sample rate %.1f Hz is not available with"
                " the configured parameters" % config_sample_rate)
        logging.info("%s '%s' configured sample_rate = %.1f SPS",
                     self.sensor_type, self.name, best_sps)
        self.osr = best_osr
        self.sps = best_sps

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
            raise config.error("%s config error: SPI communication and"
                               " data_ready_pin must be on the same MCU"
                               % (self.sensor_type,))

        # Clock tracking
        chip_smooth = self.sps * UPDATE_INTERVAL * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, "<i")

        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch, self._start_measurements,
            self._finish_measurements, UPDATE_INTERVAL)

        # Command Configuration
        mcu.add_config_cmd(
            "config_ads131m0x oid=%d spi_oid=%d channel=%d num_channels=%d "
            "data_ready_pin=%s"
            % (self.oid, self.spi.get_oid(), self.channel, self.num_channels,
               self.data_ready_pin))
        mcu.add_config_cmd("query_ads131m0x oid=%d rest_ticks=0"
                           % (self.oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        self.query_ads131m0x_cmd = None

    def setup_trigger_analog(self, trigger_analog_oid):
        self.mcu.add_config_cmd(
            "ads131m0x_attach_trigger_analog oid=%d trigger_analog_oid=%d"
            % (self.oid, trigger_analog_oid), is_init=True)

    def _build_config(self):
        cmdqueue = self.spi.get_command_queue()
        self.query_ads131m0x_cmd = self.mcu.lookup_command(
            "query_ads131m0x oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_ads131m0x_status oid=%c",
                                          oid=self.oid, cq=cmdqueue)
        errors = self.mcu.get_enumerations().get("ads131m0x_error:", {})
        self._sensor_errors = {v: k for k, v in errors.items()}

    def get_mcu(self):
        return self.mcu

    def get_samples_per_second(self):
        return self.sps

    def get_status(self, eventtime):
        return {
            'errors': self.last_error_count,
            'overflows': self.ffreader.get_last_overflows(),
            'sample_rate': self.get_samples_per_second(),
        }

    def lookup_sensor_error(self, error_code):
        return self._sensor_errors.get(error_code,
                                       "Unknown %s error" % (self.sensor_type,))

    # Returns a tuple of the minimum and maximum value of the sensor,
    # used to detect if a data value is saturated
    def get_range(self):
        return -0x800000, 0x7FFFFF

    def _calc_sample_rate(self, osr):
        if self.enable_global_chop:
            return self.clock_freq / (2. * (self.gc_dly + 3. * osr))
        else:
            return self.clock_freq / (2. * osr)

    # add_client interface, direct pass through to bulk_sensor API
    def add_client(self, callback):
        self.batch_bulk.add_client(callback)

    # Measurement decoding
    def _convert_samples(self, samples):
        adc_factor = 1. / (1 << 23)
        count = 0
        for ptime, val in samples:
            top_byte = (val >> 24) & 0xFF
            if top_byte != 0x00 and top_byte != 0xFF:
                self.last_error_count += 1
                logging.error("'%s' sample error: %s"
                              % (self.name,
                                 self.lookup_sensor_error(top_byte)))
                continue
            samples[count] = (round(ptime, 6), val, round(val * adc_factor, 9))
            count += 1
        del samples[count:]

    # Start, stop, and process message batches
    def _start_measurements(self):
        self.last_error_count = 0
        self.consecutive_fails = 0
        # Be sure to halt bulk reading before resetting
        self.query_ads131m0x_cmd.send_wait_ack([self.oid, 0])
        self.reset_chip()
        self.setup_chip()
        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(1. / (10. * self.sps))
        self.query_ads131m0x_cmd.send([self.oid, rest_ticks])
        logging.info("%s starting '%s' measurements",
                     self.sensor_type, self.name)
        # Initialize clock tracking
        self.ffreader.note_start()

    def _finish_measurements(self):
        # Don't use serial connection after shutdown
        if self.printer.is_shutdown():
            return
        # Halt bulk reading
        self.query_ads131m0x_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("%s finished '%s' measurements",
                     self.sensor_type, self.name)

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
        is_batch_mode = self.mcu.is_fileoutput()
        # Read ID register to validate the sensor type (upper byte)
        id_val = self.read_reg(REG_ID)
        if not is_batch_mode and (id_val >> 8) != self.sensor_id:
            raise self.printer.command_error(
                    "Invalid %s ID register (got 0x%x vs 0x%x).\n"
                    "This is generally indicative of connection problems\n"
                    "(e.g. faulty wiring) or a faulty chip."
                    % (self.sensor_type, id_val, self.sensor_id))
        # Send RESET command
        self.send_command_16(RESET_CMD)
        # Wait for tREGACQ after RESET before reading acknowledgment
        curtime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(curtime)
        minclock = self.mcu.print_time_to_clock(print_time + T_REGACQ)
        ack = self.send_command_16(NULL_CMD, minclock=minclock)
        if not is_batch_mode and ack != RESET_ACK:
            raise self.printer.command_error(
                    "Failed to reset %s (got 0x%x vs 0x%x).\n"
                    "This is generally indicative of connection problems\n"
                    "(e.g. faulty wiring) or a faulty chip."
                    % (self.sensor_type, ack, RESET_ACK))

    def setup_chip(self):
        is_batch_mode = self.mcu.is_fileoutput()
        mode_val = WORD24_MODE
        self.write_reg(REG_MODE, mode_val)
        actual_mode_val = self.read_reg(REG_MODE)
        if not is_batch_mode and actual_mode_val != mode_val:
            raise self.printer.command_error(
                    "Failed to set MODE register to %x, got %x"
                    % (mode_val, actual_mode_val))

        # Configure CLOCK register
        # Bits 11-8: channel mask, bit 5: TBM, bits 4:2: OSR, bits 1:0: PWR
        osr_code = OSR_TO_REG[self.osr]
        ch_en = 1 << (self.channel + 8)
        clock_val = (osr_code << 2) | ch_en | PWR_MODE
        self.write_reg(REG_CLOCK, clock_val)
        actual_clock_val = self.read_reg(REG_CLOCK)
        if not is_batch_mode and actual_clock_val != clock_val:
            raise self.printer.command_error(
                    "Failed to set CLOCK register to %x, got %x"
                    % (clock_val, actual_clock_val))

        # Configure GAIN1 register
        gain_val = 0
        for i in range(self.num_channels):
            gain_val |= GAIN_TO_REG[self.gain] << (i * 4)
        self.write_reg(REG_GAIN1, gain_val)
        actual_gain_val = self.read_reg(REG_GAIN1)
        if not is_batch_mode and actual_gain_val != gain_val:
            raise self.printer.command_error(
                    "Failed to set GAIN1 register to %x, got %x"
                    % (gain_val, actual_gain_val))

        # Configure CFG register (global chop settings)
        cfg_val = 0
        if self.enable_global_chop:
            cfg_val = GC_DLY_TO_REG[self.gc_dly] << 9 | GC_MODE
        self.write_reg(REG_CFG, cfg_val)
        actual_cfg_val = self.read_reg(REG_CFG)
        if not is_batch_mode and actual_cfg_val != cfg_val:
            raise self.printer.command_error(
                    "Failed to set CFG register to %x, got %x"
                    % (cfg_val, actual_cfg_val))

        # Validate STATUS register
        actual_status_val = self.read_reg(REG_STATUS)
        if not is_batch_mode and \
                (actual_status_val & STATUS_REG_MASK) != WORD24_MODE:
            raise self.printer.command_error(
                    "Invalid STATUS register value %x" % actual_status_val)
        # NULL_CMD being the last sent command during initialization sequence
        # ensures subsequent data reads by MCU will always read STATUS register

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

    def send_command_16(self, cmd, minclock=0):
        # Send 16-bit command as a full-frame SPI transaction
        cmd_bytes = self._convert_to_spi_frame([cmd])
        response = self.spi.spi_transfer(cmd_bytes, minclock=minclock)
        resp = response['response']
        if not resp:
            return None
        return (resp[0] << 8) | resp[1]


def ADS131M02(config):
    return ADS131M0X(config, sensor_type="ADS131M02", num_channels=2)

def ADS131M04(config):
    return ADS131M0X(config, sensor_type="ADS131M04", num_channels=4)


ADS131M0X_SENSOR_TYPES = {
    "ads131m02": ADS131M02,
    "ads131m04": ADS131M04,
}
