# Support for reading frequency samples from ldc1612 (v2)
#
# Copyright (C) 2020-2024  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2025  Vladimir Vukicevic <vladimir@pobox.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
import logging
import struct
from dataclasses import dataclass
from typing import List, Optional

try:
    from klippy.extras import bus, bulk_sensor
    from klippy.printer import Printer

    IS_KALICO = True
except ImportError:
    from . import bus, bulk_sensor
    from klippy import Printer

    IS_KALICO = False

MIN_MSG_TIME = 0.100

BATCH_UPDATES = 0.100

LDC1612_ADDR = 0x2A

# TODO: configure these as part of calibration
DEGLITCH_1_0MHZ = 0x01
DEGLITCH_3_3MHZ = 0x04
DEGLITCH_10MHZ = 0x05
DEGLITCH_33MHZ = 0x07

LDC1612_MANUF_ID = 0x5449
LDC1612_DEV_ID = 0x3055

REG_RCOUNT0 = 0x08
REG_OFFSET0 = 0x0C
REG_SETTLECOUNT0 = 0x10
REG_CLOCK_DIVIDERS0 = 0x14
REG_ERROR_CONFIG = 0x19
REG_CONFIG = 0x1A
REG_MUX_CONFIG = 0x1B
REG_DRIVE_CURRENT0 = 0x1E
REG_MANUFACTURER_ID = 0x7E
REG_DEVICE_ID = 0x7F

# Device product (match sensor_ldc1612_ng.c)
PRODUCT_UNKNOWN = 0
PRODUCT_BTT_EDDY = 1
PRODUCT_CARTOGRAPHER = 2
PRODUCT_MELLOW_FLY = 3

HOME_MODE_NONE = 0
HOME_MODE_HOME = 1
HOME_MODE_WMA = 2
HOME_MODE_SOS = 3


@dataclass
class LDC1612_ng_value:
    status: int
    freqval: int
    freq: float


@dataclass
class LDC1612_ng_homing_result:
    trigger_time: float
    tap_start_time: float
    error: int


# Interface class to LDC1612 mcu support
class LDC1612_ng:
    def __init__(self, config):
        self.printer: Printer = config.get_printer()

        self._name = config.get_name().split()[-1]
        self._verbose = config.getboolean("debug", False)

        device_choices = {
            "ldc1612": PRODUCT_UNKNOWN,
            "btt_eddy": PRODUCT_BTT_EDDY,
            "cartographer": PRODUCT_CARTOGRAPHER,
            "mellow_fly": PRODUCT_MELLOW_FLY,
        }
        self._device_product = config.getchoice("sensor_type", device_choices, PRODUCT_UNKNOWN)

        # Fin0 = Fsensor0 / FIN_DIVIDER0
        # Fref0 = Fclk / FREF_DIVIDER0
        if self._device_product == PRODUCT_CARTOGRAPHER:
            self._ldc_freq_clk = 24_000_000
            self._ldc_fin_divider = 1
            self._ldc_fref_divider = 1
            self._ldc_settle_time = 0.0001706
            self._default_drive_current = 26
        elif self._device_product == PRODUCT_MELLOW_FLY:
            self._ldc_freq_clk = 40_000_000
            self._ldc_fin_divider = 1
            self._ldc_fref_divider = 2
            self._ldc_settle_time = 0.00125
            self._default_drive_current = 15
        else:  # Generic/BTT Eddy using external 12MHz clock source
            # TODO add a generic setup that usees internal ldc1612 clock
            self._ldc_freq_clk = 12_000_000
            self._ldc_settle_time = 0.005
            self._ldc_fin_divider = 1
            self._ldc_fref_divider = 1
            self._default_drive_current = 15

        self._ldc_freq_ref = round(self._ldc_freq_clk / self._ldc_fref_divider)

        drive_current: int = config.getint("reg_drive_current", 0, minval=0, maxval=31)
        saved_drive_current: int = config.getint("saved_reg_drive_current", 0, minval=0, maxval=31)
        if drive_current == 0:
            drive_current = saved_drive_current
        if drive_current == 0:
            drive_current = self._default_drive_current
        self._drive_current = drive_current

        self._deglitch: str = config.get("ldc_deglitch", "default").lower()
        self._data_rate: int = config.getint("samples_per_second", 250, minval=50)
        self._ldc_settle_time = min(self._ldc_settle_time, 1.0 / self._data_rate)

        # Setup mcu sensor_ldc1612 bulk query code
        self._i2c = bus.MCU_I2C_from_config(config, default_addr=LDC1612_ADDR, default_speed=400000)
        self._mcu = mcu = self._i2c.get_mcu()
        self._oid = oid = mcu.create_oid()

        logging.info(f"LDC1612ng {self._name} oid: {oid} i2c_oid {self._i2c.get_oid()}")

        # params ending in "_pin" are magic and are a %s on this side, but %c on
        # the native side. There doesn't seem to be a way to pass "no pin". So we
        # need two separate config commands.
        if config.get("intb_pin", None) is not None:
            ppins = config.get_printer().lookup_object("pins")
            pin_params = ppins.lookup_pin(config.get("intb_pin"))
            if pin_params["chip"] != mcu:
                raise config.error("ldc1612 intb_pin must be on same mcu")
            mcu.add_config_cmd(
                "config_ldc1612_ng_with_intb oid=%d i2c_oid=%d product=%i intb_pin=%s"
                % (
                    oid,
                    self._i2c.get_oid(),
                    self._device_product,
                    pin_params["pin"],
                )
            )
        else:
            mcu.add_config_cmd("config_ldc1612_ng oid=%d i2c_oid=%d product=%i" % (oid, self._i2c.get_oid(), self._device_product))

        # Make sure the sensor is stopped on restart
        mcu.add_config_cmd(
            "ldc1612_ng_start_stop oid=%d rest_ticks=0" % (oid,),
            on_restart=True,
        )
        mcu.register_config_callback(self._build_config)

        self._start_count = 0
        self._chip_initialized = False

        # Bulk sample message reading
        chip_smooth = self._data_rate * BATCH_UPDATES * 2
        self._ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, ">I")
        # Process messages in batches
        self._batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer,
            self._process_batch,
            self._start_measurements,
            self._finish_measurements,
            BATCH_UPDATES,
        )
        hdr = ("time", "frequency", "z")
        self._batch_bulk.add_mux_endpoint("ldc1612_ng/dump_ldc1612", "sensor", self._name, {"header": hdr})

        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command(
            "LDC_NG_CALIBRATE_DRIVE_CURRENT",
            "CHIP",
            self._name,
            self.cmd_LDC_CALIBRATE,
            desc=self.cmd_LDC_CALIBRATE_help,
        )
        gcode.register_mux_command(
            "LDC_NG_SET_DRIVE_CURRENT",
            "CHIP",
            self._name,
            self.cmd_LDC_SET_DC,
            desc=self.cmd_LDC_SET_DC_help,
        )

    cmd_LDC_SET_DC_help = "Set LDC1612 DRIVE_CURRENT register (idrive value only)"

    def cmd_LDC_SET_DC(self, gcmd):
        drive_cur = gcmd.get_int("VAL", minval=0, maxval=31)
        self.set_drive_current(drive_cur)

    cmd_LDC_CALIBRATE_help = "Calibrate LDC1612 DRIVE_CURRENT register"

    def cmd_LDC_CALIBRATE(self, gcmd):
        is_in_progress = True

        def handle_batch(msg):
            return is_in_progress

        self.add_bulk_sensor_data_client(handle_batch)
        toolhead = self.printer.lookup_object("toolhead")
        toolhead.dwell(0.100)
        toolhead.wait_moves()
        old_config = self.read_reg(REG_CONFIG)
        self.set_reg(REG_CONFIG, 0x001 | (1 << 9))
        toolhead.wait_moves()
        toolhead.dwell(0.100)
        toolhead.wait_moves()
        reg_drive_current0 = self.read_reg(REG_DRIVE_CURRENT0)
        self.set_reg(REG_CONFIG, old_config)
        is_in_progress = False
        # Report found value to user
        drive_cur = (reg_drive_current0 >> 6) & 0x1F
        gcmd.respond_info(
            f"{self._name}: Estimated reg_drive_current: {drive_cur}\n"
            "Add this to your config as either reg_drive_current or tap_drive_current\n"
            "then restart."
        )

    def _build_config(self):
        cmdqueue = self._i2c.get_command_queue()

        self._ldc1612_ng_start_stop_cmd = self._mcu.lookup_command("ldc1612_ng_start_stop oid=%c rest_ticks=%u", cq=cmdqueue)

        self._ffreader.setup_query_command("ldc1612_ng_query_bulk_status oid=%c", oid=self._oid, cq=cmdqueue)

        self._ldc1612_ng_latched_status_cmd = self._mcu.lookup_query_command(
            "query_ldc1612_ng_latched_status_v2 oid=%c",
            "ldc1612_ng_latched_status oid=%c status=%u lastval=%u",
            oid=self._oid,
            cq=cmdqueue,
        )

        self._ldc1612_ng_setup_home_cmd = self._mcu.lookup_command(
            "ldc1612_ng_setup_home oid=%c"
            " trsync_oid=%c trigger_reason=%c other_reason_base=%c"
            " trigger_freq=%u start_freq=%u start_time=%u mode=%c tap_threshold=%i"
            " err_max=%c",
            cq=cmdqueue,
        )

        self._ldc1612_ng_finish_home_cmd = self._mcu.lookup_query_command(
            "ldc1612_ng_finish_home oid=%c",
            "ldc1612_ng_finish_home_reply oid=%c trigger_clock=%u tap_start_clock=%u error=%u",
            oid=self._oid,
            cq=cmdqueue,
        )

        self._ldc1612_ng_set_sos_section = self._mcu.lookup_command(
            "ldc1612_ng_set_sos_section oid=%c section=%c values=%*s",
            cq=cmdqueue,
        )

        # XXX move this to a totally separate thing at some point
        self._mcu.register_response(self._handle_debug_print, "debug_print")

    def _handle_debug_print(self, params):
        logging.info(params["m"])

    def _clock32_to_print_time(self, clock) -> float:
        return self._mcu.clock_to_print_time(self._mcu.clock32_to_clock64(clock))

    def get_mcu(self):
        return self._i2c.get_mcu()

    def read_reg(self, reg):
        params = self._i2c.i2c_read([reg], 2)
        response = bytearray(params["response"])
        return (response[0] << 8) | response[1]

    def set_reg(self, reg, val, minclock=0):
        self._i2c.i2c_write([reg, (val >> 8) & 0xFF, val & 0xFF], minclock=minclock)

    def add_bulk_sensor_data_client(self, cb):
        self._batch_bulk.add_client(cb)

    def latched_status(self):
        response = self._ldc1612_ng_latched_status_cmd.send([self._oid])
        return response["status"]

    def latched_status_str(self):
        s = self.latched_status()
        return self.status_to_str(s)

    def status_to_str(self, s: int):
        status_bits = [
            "0",
            "1",
            "2",
            "UNREADCONV1",
            "4",
            "5",
            "DRDY",
            "7",
            "ERR_ZC",
            "ERR_ALE",
            "ERR_AHE",
            "ERR_WD",
            "ERR_OR",
            "ERR_UR",
            "14",
            "ERR_CH1",
        ]
        flags = []
        for bit, flag in enumerate(status_bits):
            if s & (1 << bit):
                flags.append(flag)
        return " ".join(flags)

    def data_error_to_str(self, d: int):
        err_bits = [
            "Under-range Error",
            "Over-range Error",
            "Watchdog Error",
            "Amplitude Error",
        ]
        d = d >> 12  # shift out the data bits
        errors = []
        for bit, err in enumerate(err_bits):
            if d & (1 << bit):
                errors.append(err)
        return " ".join(errors)

    def read_one_value(self):
        self._init_chip()
        # the status command also returns the last value
        res = self._ldc1612_ng_latched_status_cmd.send([self._oid])
        status = res["status"]
        lastval = res["lastval"]

        return LDC1612_ng_value(
            status=status,
            freqval=lastval,
            freq=self.from_ldc_freqval(lastval, ignore_err=True),
        )

    def to_ldc_freqval(self, freq):
        return int(freq * (1 << 28) / float(self._ldc_freq_ref) + 0.5)

    def from_ldc_freqval(self, val, ignore_err=False):
        if val >= 0x0FFFFFFF and not ignore_err:
            raise self.printer.command_error(f"LDC1612 frequency value has error bits: {hex(val)}")
        return round(val * (float(self._ldc_freq_ref) / (1 << 28)), 3)

    #
    # Homing
    #
    def setup_home(
        self,
        trsync_oid: int,
        hit_reason: int,
        other_reason_base: int,
        trigger_freq: float,
        start_freq: float,
        start_time: float,
        mode: str = "home",
        tap_threshold: Optional[float] = None,
        max_errors: int = 0,
    ):
        MODES = {
            "home": HOME_MODE_HOME,
            "wma": HOME_MODE_WMA,
            "sos": HOME_MODE_SOS,
        }
        mode_val = MODES.get(mode.lower(), None)
        if mode_val is None:
            raise self.printer.command_error(f"Invalid mode: {mode}")

        t_freqvl = self.to_ldc_freqval(trigger_freq)
        s_freqval = self.to_ldc_freqval(start_freq)
        start_time_mcu = self._mcu.print_time_to_clock(start_time) if start_time > 0 else 0
        tap_threshold_val = int(tap_threshold * 65536.0) if tap_threshold is not None else 0

        if self._verbose:
            logging.info(
                f"LDC1612_ng setup_home: {mode} trigger: {trigger_freq:.2f} ({t_freqvl}) "
                f"safe: {start_freq:.2f} ({s_freqval}) @ {start_time:.2f} ({start_time_mcu}) "
                f"trsync: {trsync_oid} {hit_reason} {other_reason_base} TAP: {tap_threshold} ({tap_threshold_val:x})"
            )

        self._ldc1612_ng_setup_home_cmd.send(
            [
                self._oid,
                trsync_oid,
                hit_reason,
                other_reason_base,
                t_freqvl,
                s_freqval,
                start_time_mcu,
                mode_val,
                tap_threshold_val,
                max_errors,
            ]
        )

    def _convert_clock(self, c):
        if c == 0:
            return 0
        return self._clock32_to_print_time(c)

    def finish_home(self):
        # "ldc1612_finish_home2_reply oid=%c homing=%c trigger_clock=%u tap_start_clock=%u",
        reply = self._ldc1612_ng_finish_home_cmd.send([self._oid])
        trigger_clock = reply["trigger_clock"]
        tap_start_clock = reply["tap_start_clock"]
        error = reply["error"]
        trigger_time = self._convert_clock(trigger_clock)
        tap_start_time = self._convert_clock(tap_start_clock)

        return LDC1612_ng_homing_result(trigger_time, tap_start_time, error)

    def set_sos_section(self, sect_num: int, sect_vals: List[float]):
        # pack sect_vals into a byte array using struct.pack
        sect_bytes = [b for b in struct.pack("<6f", *sect_vals)]
        self._ldc1612_ng_set_sos_section.send([self._oid, sect_num, sect_bytes])

    # The value that freqvals are multiplied by to get a float frequency
    def freqval_conversion_value(self):
        return float(self._ldc_freq_ref) / (1 << 28)

    def _verify_chip(self):
        # In case of miswiring, testing LDC1612 device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        manuf_id = self.read_reg(REG_MANUFACTURER_ID)
        dev_id = self.read_reg(REG_DEVICE_ID)
        if manuf_id != LDC1612_MANUF_ID or dev_id != LDC1612_DEV_ID:
            raise self.printer.command_error(
                "Invalid ldc1612 id (got %x,%x vs %x,%x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty ldc1612 chip." % (manuf_id, dev_id, LDC1612_MANUF_ID, LDC1612_DEV_ID)
            )

    def _init_chip(self):
        if self._chip_initialized:
            return

        self._verify_chip()

        # TODO: have a max_frequency and pick the best deglitch for it
        if self._deglitch == "1mhz":
            deglitch = DEGLITCH_1_0MHZ
        elif self._deglitch == "3.3mhz":
            deglitch = DEGLITCH_3_3MHZ
        elif self._deglitch == "10mhz" or self._deglitch == "default":
            deglitch = DEGLITCH_10MHZ
        elif self._deglitch == "33mhz":
            deglitch = DEGLITCH_33MHZ
        else:
            raise self.printer.error(f"Invalid {self._name} deglitch value: {self._deglitch}")

        # This is the TI-recommended register configuration order
        # Setup chip in requested query rate
        rcount0 = self._ldc_freq_ref / (16.0 * (self._data_rate - 4))
        self.set_reg(REG_RCOUNT0, int(rcount0 + 0.5))
        self.set_reg(REG_OFFSET0, 0)
        self.set_reg(
            REG_SETTLECOUNT0,
            int(self._ldc_settle_time * self._ldc_freq_ref / 16.0 + 0.5),
        )
        self.set_reg(
            REG_CLOCK_DIVIDERS0,
            (self._ldc_fin_divider << 12) | (self._ldc_fref_divider),
        )
        self.set_reg(REG_ERROR_CONFIG, 0b1111_1100_1111_1001)  # report everything to STATUS and INTB except ZC
        self.set_reg(REG_MUX_CONFIG, 0x0208 | deglitch)
        # RP_OVERRIDE_EN | AUTO_AMP_DIS | REF_CLK_SRC=clkin | reserved
        self.set_reg(REG_CONFIG, (1 << 12) | (1 << 10) | (1 << 9) | 0x001)
        self.set_reg(REG_DRIVE_CURRENT0, self._drive_current << 11)

        self._chip_initialized = True

    def get_deglitch(self):
        return self.read_reg(REG_MUX_CONFIG) & ~0x0208

    def set_deglitch(self, val: int):
        logging.info(f"LDC1612ng {self._name} deglitch set {val}")
        self.set_reg(REG_MUX_CONFIG, val | 0x0208)

    def get_drive_current(self) -> int:
        return self._drive_current

    def set_drive_current(self, cval: int, maxfreq: float = None):
        if cval < 0 or cval > 31:
            raise self.printer.command_error("Drive current must be between 0 and 31")
        if self._drive_current == cval:
            return

        if maxfreq is not None:
            if maxfreq < 1_000_000.0:
                self.set_deglitch(DEGLITCH_1_0MHZ)
            elif maxfreq < 3_300_000.0:
                self.set_deglitch(DEGLITCH_3_3MHZ)
            elif maxfreq < 10_000_000.0:
                self.set_deglitch(DEGLITCH_10MHZ)
            else:
                self.set_deglitch(DEGLITCH_33MHZ)

        logging.info(f"LDC1612ng {self._name} set drive current {cval}")
        self._drive_current = cval
        self.set_reg(REG_DRIVE_CURRENT0, cval << 11)

    # Start, stop, and process message batches
    def _start_measurements(self):
        self._init_chip()
        self._start_count += 1

        if self._start_count > 1:
            logging.info("LDC1612 start count: %d", self._start_count)
            return

        # Start bulk reading
        rest_ticks = self._mcu.seconds_to_clock(0.5 / self._data_rate)
        self._ldc1612_ng_start_stop_cmd.send([self._oid, rest_ticks])
        # logging.info("LDC1612 starting '%s' measurements", self._name)
        # Initialize clock tracking
        self._ffreader.note_start()

    def _finish_measurements(self):
        self._start_count -= 1

        if self._start_count > 0:
            logging.info("LDC1612 stop, start count now: %d", self._start_count)
            return

        # Halt bulk reading
        self._ldc1612_ng_start_stop_cmd.send_wait_ack([self._oid, 0])
        self._ffreader.note_end()
        # logging.info("LDC1612 finished '%s' measurements", self._name)

    def _process_batch(self, eventtime):
        samples = self._ffreader.pull_samples()
        count = 0
        err_count = 0
        last_err_kind = 0
        for ptime, val in samples:
            if val > 0x0FFFFFFF:  # high nibble indicates an error
                err_kind = (val >> 28)
                err_count += 1
                if last_err_kind != err_kind:
                    if self._verbose:
                        logging.info(f"LDC1612 error: {hex(val)}")
                    last_err_kind = err_kind
            else:
                # val is a raw value
                samples[count] = (ptime, val)
                count += 1
        # remove the samples we didn't fill in because of errors
        del samples[count:]
        return {
            "data": samples,
            "errors": err_count,
            "overflows": self._ffreader.get_last_overflows(),
        }

