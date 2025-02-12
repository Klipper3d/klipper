# EDDY-ng
#
# Copyright (C) 2025  Vladimir Vukicevic <vladimir@pobox.com>
# Copyright (C) 2020-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from __future__ import annotations

import logging, math, bisect, re
import numpy as np
import traceback
import pickle, base64
from itertools import combinations

import mcu
import pins

from dataclasses import dataclass, field
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    final,
    ClassVar,
)

try:
    import plotly  # noqa

    HAS_PLOTLY = True
except:
    HAS_PLOTLY = False

try:
    import scipy  # noqa

    HAS_SCIPY = True
except:
    HAS_SCIPY = False

from configfile import ConfigWrapper
from configfile import error as configerror
from gcode import GCodeCommand
from klippy import Printer
from stepper import MCU_stepper
from toolhead import ToolHead

from .homing import HomingMove

from . import ldc1612_ng, probe, manual_probe

# In this file, a couple of conventions are used (for sanity).
# Variables are named according to:
# - "height" is always a physical height as detected by the probe in mm
# - "z" is always a z axis position (which may or may not match height)
# - "freq" is always a frequency value (float)
# - "freqval" is always an encoded frequency value, as communicated to/from the sensor (int)

# There are three distinct operations/phases. Homing Z via the virtual
# endstop is the only operation that can happen while Z is not homed:
#
# 1. Homing Z using a virtual probe endstop. This is largely handled by
#    ProbeEddyEndstopWrapper. It sets up the sensor to trigger when a certain
#    frequency is crossed, and then lets a HomingMove continue that moves the
#    toolhead down. When that frequency is hit, it triggers, and Klipper stops
#    the toolhead from moving down. The time point when it triggers is set as
#    the z=trigger_height (which is home_trigger_height in the configurable
#    params). Z should be accurate enough at this point. This operation can be run
#    when the bed/toolhead are cold or hot.
#
# Once Z is homed, two additional operations become available:
#
# 2. Probing at either a single point or multiple points. This is used for
#    Quad Gantry Leveling, Bed Mesh, and other similar operations. This is
#    largely handled by the ProbeEddyScannigProbe class -- one is returned
#    from the ProbeEddy `probe` object when `start_probe_session` is called.
#    For Eddy probes, there is no reason to move the toolhead up and down at
#    each probe point: the measured distance between the sensor and the build
#    plate can be read directly. This class starts gathering sample data when
#    the session starts and records the times when there's a sample that we
#    care about, along with the toolhead position, whenever a caller calls
#    `run_probe`. If this is a `rapid_scan` scan, then a callback is attached
#    to the current motion so that we can save the movement's time and position
#    without actually waiting for it. If it's in normal mode, then the toolhead
#    will pause at each position. In both cases, the results are obtained by
#    calling `pull_probed_results`, which returns an array of results at each
#    point that `run_probe` was called for, in order.
#
#    PROBE_STATIC HOME_Z=1 can be used to set the toolhead's Z position
#    based on the current height reading from the probe while the toolhead is
#    static, leading to a more accurate result than a regular homing operation
#    (which involves movement).
#
# 3. A "tap" to fine-tune the Z offset. This should be run with the bed at print
#    temperature and soaked for a bit. The nozzle should also be warm but not so
#    hot that filament risks oozing out. The nozzle also must be clean. 150C
#    is a good temperature to both clean and tap at.
#
#    This operation will identify the exact position of the Z axis
#    when the nozzle touches the bed, which means that a precise Z offset
#    can be set.
#
# The eddy current response and readings depend on temperature of both the target
# (bed) and the sensor (coil). EddyNG does not do any temperature compensation. Instead
# it relies on the "tap" operation to get an accurate reference point for z=0 regardless
# of temperatures. Empirically, small offsets from a reference point can still be read
# accurately from the sensor, even if the absolute value is incorrect at temperature.
# For example, taking sensor readings at Z=2 when perfectly homed via tap may read as
# 1.9 due to temperatures, which is not correct. However, raising the toolhead to Z=2.1
# will raise the sensor reading to 2.0; likewise, lowering the toolhead to Z=1.9 will
# lower the sensor reading to 1.8.
#
# Care in macros should be taken to not invalidate the Z offset set after a tap
# by relying on absolute sensor readings.
#


@dataclass
class ProbeEddyParams:
    # The speed at which to perform normal homing operations
    probe_speed: float = 5.0
    # The speed at which to lift the toolhead during probing operations
    lift_speed: float = 10.0
    # The speed at which to move in the xy plane (typically only for calibration)
    move_speed: float = 50.0
    # The height at which the virtual endstop should trigger. A value
    # between 1.0 and 3.0 is recommended, with 2.0 or 2.5 being good
    # choices.
    home_trigger_height: float = 2.0
    # The amount higher the probe needs to detect the toolhead is at  in order to
    # allow homing to begin. For example, if the trigger height is 2.0, and the
    # start offset is 1.5, then homing will abort if the sensor detects the
    # toolhead is below 3.5mm off the print bed.
    home_trigger_safe_start_offset: float = 1.0
    # The amount of time that must elapse from the start of probing until the
    # safe start position is crossed. This is to make sure there are some values
    # that are above the safe position before it's crossed, to ensure that homing
    # doesn't begin with the toolhead too low.
    home_trigger_safe_time_offset: float = 0.100
    # The maximum z value to calibrate from. 15.0 is fine as a default, calibrating
    # at higher values is not needed. Calibration will start with the first
    # valid height.
    calibration_z_max: float = 15.0
    # The "drive current" for the LDC1612 sensor. This value is typically
    # sensor specific and depends on the coil design and the operating distance.
    # A good starting value for BTT Eddy is 15. A good value can be obtained
    # by placing the toolhead ~10mm above the bed and running LDC_NG_CALIBRATE_
    # DRIVE_CURRENT.
    reg_drive_current: int = 0
    # The drive current to use for tap operations. If not set, the `reg_drive_current`
    # value will be used. Tapping involves reading values much closer to the print
    # bed than basic homing, and may require a different, typically higher,
    # drive current. For example, BTT Eddy performs best with this value at 16.
    # Note that the sensor needs to be calibrated for both drive currents separately.
    # Pass the DRIVE_CURRENT argument to EDDY_NG_CALIBRATE.
    tap_drive_current: int = 0
    # The Z position at which to start a tap-home operation. This height may
    # need to be fine-tuned to ensure that the sensor can provide readings across the
    # entire tap range (i.e. from this value down to tap_target_z), which in turn
    # will depend on the tap_drive_current. When the tap_drive_current is
    # increased, the sensor may not be able to read values at higher heights.
    # For example, BTT Eddy typically cannot work with heights above 3.5mm with
    # a drive current of 16.
    #
    # Note that all of these values are in terms of offsets from the nozzle
    # to the toolhead. The actual sensor coil is mounted higher -- but must be placed
    # between 2.5 and 3mm above the nozzle, ideally around 2.75mm. If there are
    # amplitude errors, try raising or lowering the sensor coil slightly.
    tap_start_z: float = 3.0
    # The target Z position for a tap operation. This is the lowest position that
    # the toolhead may travel to in case of a failed tap. Do not set this very low,
    # as it will cause your toolhead to try to push through your build plate in
    # the case of a failed tap. A value like -0.250 is no worse than moving the
    # nozzle down one or two notches too far when doing manual Z adjustment.
    tap_target_z: float = -0.250
    # the tap mode to use. 'wma' is a derivative of weighted moving average,
    # 'butter' is a butterworth filter
    tap_mode: str = "butter"
    # The threshold at which to detect a tap. This value is raw sensor value
    # specific. A good value can be obtained by running [....] and examining
    # the graph. See [calibration docs coming soon].
    #
    # The meaning of this depends on tap_mode, and the value will be different
    # if a different tap_mode is used.  You can experiment to arrive at this
    # value. Typically, a lower value will make tap detection more sensitive,
    # but might lead to false positives (too early detections). A higher value
    # may cause the detection to wait too long or miss a tap entirely.
    # You can pass a THRESHOLD parameter to the TAP command to experiment to
    # find a good value.
    #
    # You may also need to use different thresholds for different build plates.
    # Note that the default value of this threshold depends on the tap_mode.
    tap_threshold: float = 250.0
    # The speed at which a tap operation should be performed at. This shouldn't
    # be much slower than 3.0, but you can experiment with lower or higher values.
    # Don't go too high though, because Klipper needs some small amount of time
    # to react to a tap trigger, and the toolhead will still be moving at this
    # speed even past the tap point. So, consider any speed you'd feel comfortable
    # triggering a toolhead move to tap_target_z at.
    tap_speed: float = 3.0
    # A static additional amount to add to the computed tap Z offset. Use this if
    # the computed tap is a bit too high or too low for your taste. Positive
    # values will raise the toolhead, negative values will lower it.
    tap_adjust_z: float = 0.0
    # The number of times to do a tap, averaging the results.
    tap_samples: int = 3
    # The maximum number of tap samples.
    tap_max_samples: int = 5
    # The maximum standard deviation for any 3 samples to be considered valid.
    tap_samples_stddev: float = 0.020

    # When probing multiple points (not rapid scan), how long to sample for at each probe point,
    # after a scan_sample_time_delay delay. The total dwell time at each probe point is
    # scan_sample_time + scan_sample_time_delay.
    scan_sample_time: float = 0.100
    # When probing multiple points (not rapid scan), how long to delay at each probe point
    # before the scan_sample_time kicks in.
    scan_sample_time_delay: float = 0.050
    # number of points to save for calibration
    calibration_points: int = 150
    # configuration for butterworth filter
    tap_butter_lowcut: float = 5.0
    tap_butter_highcut: float = 25.0
    tap_butter_order: int = 2
    # Probe position relative to toolhead
    x_offset: float = 0.0
    y_offset: float = 0.0
    # remove some safety checks, largely for testing/development
    allow_unsafe: bool = False
    # whether to write the tap plot after each tap
    write_tap_plot: bool = True

    tap_trigger_safe_start_height: float = 1.5

    _config_reg_drive_current: int = 0
    _config_tap_drive_current: int = 0

    _warning_msgs: List[str] = field(default_factory=list)

    @staticmethod
    def str_to_floatlist(s):
        if s is None:
            return None
        try:
            return [float(v) for v in re.split(r"\s*,\s*|\s+", s)]
        except:
            raise configerror(f"Can't parse '{s}' as list of floats")

    def is_default_butter_config(self):
        return (
            self.tap_butter_lowcut == 5.0
            and self.tap_butter_highcut == 25.0
            and self.tap_butter_order == 2
        )

    def load_from_config(self, config: ConfigWrapper):
        bool_choices = {
            "true": True,
            "True": True,
            "false": False,
            "False": False,
            "1": True,
            "0": False,
        }
        mode_choices = ["wma", "butter"]

        self.probe_speed = config.getfloat(
            "probe_speed", self.probe_speed, above=0.0
        )
        self.lift_speed = config.getfloat(
            "lift_speed", self.lift_speed, above=0.0
        )
        self.move_speed = config.getfloat(
            "move_speed", self.move_speed, above=0.0
        )
        self.home_trigger_height = config.getfloat(
            "home_trigger_height", self.home_trigger_height, minval=1.0
        )
        self.home_trigger_safe_start_offset = config.getfloat(
            "home_trigger_safe_start_offset",
            self.home_trigger_safe_start_offset,
            minval=0.5,
        )
        self.calibration_z_max = config.getfloat(
            "calibration_z_max", self.calibration_z_max, above=0.0
        )

        # "saved_" is the values that save_config will write
        saved_reg_drive_current = config.getint("saved_reg_drive_current", 0)
        saved_tap_drive_current = config.getint("saved_tap_drive_current", 0)
        reg_drive_current = self._config_reg_drive_current = config.getint(
            "reg_drive_current", 0, minval=0, maxval=31
        )
        tap_drive_current = self._config_tap_drive_current = config.getint(
            "tap_drive_current", 0, minval=0, maxval=31
        )

        logging.info(f"saved {saved_reg_drive_current} reg {reg_drive_current}")
        if (
            saved_reg_drive_current != 0
            and reg_drive_current != 0
            and reg_drive_current != saved_reg_drive_current
        ):
            printer = config.get_printer()
            msg = f"probe_eddy_ng has reg_drive_current specified in config and in saved variables. Config value ({reg_drive_current}) is taking precedence. Remove one of these to remove this warning."
            logging.warning(msg)
            self._warning_msgs.append(msg)

        if (
            saved_tap_drive_current != 0
            and tap_drive_current != 0
            and tap_drive_current != saved_tap_drive_current
        ):
            printer = config.get_printer()
            msg = f"probe_eddy_ng has tap_drive_current specified in config and in saved variables. Config value ({tap_drive_current}) is taking precedence. Remove one of these to remove this warning."
            logging.warning(msg)
            self._warning_msgs.append(msg)

        # the config value overrides a saved value, if any
        if reg_drive_current == 0:
            reg_drive_current = saved_reg_drive_current
        if tap_drive_current == 0:
            tap_drive_current = saved_tap_drive_current

        logging.info(f"saved reg {reg_drive_current} set")
        self.reg_drive_current = reg_drive_current
        self.tap_drive_current = tap_drive_current

        self.tap_start_z = config.getfloat(
            "tap_start_z", self.tap_start_z, above=0.0
        )
        self.tap_target_z = config.getfloat("tap_target_z", self.tap_target_z)
        self.tap_speed = config.getfloat("tap_speed", self.tap_speed, above=0.0)
        self.tap_adjust_z = config.getfloat("tap_adjust_z", self.tap_adjust_z)
        self.calibration_points = config.getint(
            "calibration_points", self.calibration_points
        )

        self.tap_mode = config.getchoice(
            "tap_mode", mode_choices, self.tap_mode
        )
        default_tap_threshold = 1000.0  # for wma
        if self.tap_mode == "butter":
            default_tap_threshold = 250.0
        self.tap_threshold = config.getfloat(
            "tap_threshold", default_tap_threshold
        )

        # for 'butter'
        self.tap_butter_lowcut = config.getfloat(
            "tap_butter_lowcut", self.tap_butter_lowcut, above=0.0
        )
        self.tap_butter_highcut = config.getfloat(
            "tap_butter_highcut",
            self.tap_butter_highcut,
            above=self.tap_butter_lowcut,
        )
        self.tap_butter_order = config.getint(
            "tap_butter_order", self.tap_butter_order, minval=1
        )

        self.tap_samples = config.getint(
            "tap_samples", self.tap_samples, minval=1
        )
        self.tap_max_samples = config.getint(
            "tap_max_samples", self.tap_max_samples, minval=self.tap_samples
        )
        self.tap_samples_stddev = config.getfloat(
            "tap_samples_stddev", self.tap_samples_stddev, above=0.0
        )
        self.tap_trigger_safe_start_height = config.getfloat(
            "tap_trigger_safe_start_height",
            -1.0,
            above=0.0,
        )

        if self.tap_trigger_safe_start_height == -1.0:  # sentinel
            self.tap_trigger_safe_start_height = self.home_trigger_height / 2.0

        self.allow_unsafe = config.getchoice(
            "allow_unsafe", bool_choices, default="False"
        )
        self.write_tap_plot = config.getchoice(
            "write_tap_plot", bool_choices, default="True"
        )
        self.x_offset = config.getfloat("x_offset", self.x_offset)
        self.y_offset = config.getfloat("y_offset", self.y_offset)

        self.validate(config)

    def validate(self, config: ConfigWrapper = None):
        printer = config.get_printer()
        req_cal_z_max = (
            self.home_trigger_safe_start_offset + self.home_trigger_height + 1.0
        )
        if self.calibration_z_max < req_cal_z_max:
            raise printer.config_error(
                f"calibration_z_max must be at least home_trigger_safe_start_offset+home_trigger_height+1.0 ({self.home_trigger_safe_start_offset:.3f}+{self.home_trigger_height:.3f}+1.0={req_cal_z_max:.3f})"
            )
        if (
            self.x_offset == 0.0
            and self.y_offset == 0.0
            and not self.allow_unsafe
        ):
            raise printer.config_error(
                "ProbeEddy: x_offset and y_offset are both 0.0; is the sensor really mounted at the nozzle?"
            )

        if self.home_trigger_height <= self.tap_trigger_safe_start_height:
            raise printer.config_error(
                "ProbeEddy: home_trigger_height must be greater than tap_trigger_safe_start_height"
            )

        need_scipy = False
        if self.tap_mode == "butter" and not self.is_default_butter_config():
            need_scipy = True

        if need_scipy and not HAS_SCIPY:
            raise printer.config_error(
                "ProbeEddy: butter mode with custom filter parameters requires scipy, which is not available; please install scipy, use the defaults, or use wma mode"
            )


@dataclass
class ProbeEddyProbeResult:
    samples: List[float]
    mean: float = 0.0
    median: float = 0.0
    min_value: float = 0.0
    max_value: float = 0.0
    tstart: float = 0.0
    tend: float = 0.0
    errors: int = 0

    USE_MEAN_FOR_VALUE: ClassVar[bool] = False

    @property
    def valid(self):
        return len(self.samples) > 0

    @property
    def value(self):
        return self.mean if self.USE_MEAN_FOR_VALUE else self.median

    @property
    def stddev(self):
        stddev_sum = np.sum([(s - self.value) ** 2.0 for s in self.samples])
        return (stddev_sum / len(self.samples)) ** 0.5

    def __format__(self, spec):
        if spec == "v":
            return f"{self.value:.3f}"
        if self.USE_MEAN_FOR_VALUE:
            value = f"{self.mean:.3f}"
            extra = f"med={self.median:.3f}"
        else:
            value = f"{self.median:.3f}"
            extra = f"avg={self.mean:.3f}"

        return f"{value} ({extra}, {self.min_value:.3f} to {self.max_value:.3f}, [{self.stddev:.3f}])"


@final
class ProbeEddy:
    def __init__(self, config: ConfigWrapper):
        logging.info("Hello from ProbeEddyNG")

        self._printer: Printer = config.get_printer()
        self._gcode = self._printer.lookup_object("gcode")
        self._full_name = config.get_name()
        self._name = self._full_name.split()[-1]

        sensors = {
            "ldc1612": ldc1612_ng.LDC1612_ng,
            "btt_eddy": ldc1612_ng.LDC1612_ng,
            "cartographer": ldc1612_ng.LDC1612_ng,
            "mellow_fly": ldc1612_ng.LDC1612_ng,
        }
        sensor_type = config.getchoice("sensor_type", {s: s for s in sensors})

        self._sensor_type = sensor_type
        self._sensor = sensors[sensor_type](config)
        self._mcu = self._sensor.get_mcu()
        self._toolhead: ToolHead = None  # filled in _handle_connect

        self.params = ProbeEddyParams()
        self.params.load_from_config(config)
        if self.params.reg_drive_current == 0:
            logging.info(f" is zero, overriding {self._sensor._drive_current}")
            # set as default
            self.params.reg_drive_current = self._sensor._drive_current

        # at what minimum physical height to start homing. It must be above the safe start position,
        # because we need to move from the start through the safe start position
        self._home_start_height = (
            self.params.home_trigger_height
            + self.params.home_trigger_safe_start_offset
            + 1.0
        )

        # physical offsets between probe and nozzle
        self.offset = {
            "x": self.params.x_offset,
            "y": self.params.y_offset,
        }

        # drive current to frequency map
        self._dc_to_fmap: Dict[int, ProbeEddyFrequencyMap] = {}

        version = config.getint("calibration_version", default=-1)
        calibration_bad = False
        if version == -1:
            if config.get("calibrated_drive_currents", None) is not None:
                calibration_bad = True
        elif version != ProbeEddyFrequencyMap.calibration_version:
            calibration_bad = True

        if calibration_bad:
            raise configerror(
                "EDDYng calibration: calibration data invalid, please delete the calibrated_drive_currents, calibrated_version, and calibration data in your saved data in printer.cfg."
            )

        calibrated_drive_currents = config.getintlist(
            "calibrated_drive_currents", []
        )
        for dc in calibrated_drive_currents:
            self._dc_to_fmap[dc] = ProbeEddyFrequencyMap(self)
            self._dc_to_fmap[dc].load_from_config(config, dc)

        # Our virtual endstop wrapper -- used for homing.
        self._endstop_wrapper = ProbeEddyEndstopWrapper(self)

        # There can only be one active sampler at a time
        self._sampler: ProbeEddySampler = None
        self.save_samples_path = None
        # This is a hack to keep the last set of data around so that we can
        # do plots and things. It's updated after finish()
        self._last_sampler_samples = None
        self._last_sampler_raw_samples = None
        self._last_sampler_memos = None

        # This class emulates "PrinterProbe". We use some existing helpers to implement
        # functionality like start_session
        self._printer.add_object("probe", self)

        # TODO: get rid of this
        if hasattr(probe, "ProbeCommandHelper"):
            self._cmd_helper = probe.ProbeCommandHelper(
                config, self, self._endstop_wrapper.query_endstop
            )
        else:
            self._cmd_helper = None

        # when doing a scan, what's the offset between probe readings at the bed
        # scan height and the accurate bed height, based on the last tap.
        self._tap_offset = 0.0
        self._last_probe_result = 0.0

        # runtime configurable
        self._tap_adjust_z = self.params.tap_adjust_z

        # define our own commands
        self._dummy_gcode_cmd = self._gcode.create_gcode_command("", "", {})
        self.define_commands(self._gcode)

        self._printer.register_event_handler(
            "gcode:command_error", self._handle_command_error
        )
        self._printer.register_event_handler(
            "klippy:connect", self._handle_connect
        )

    def _log_error(self, msg):
        logging.error(f"{self._name}: {msg}")
        self._gcode.respond_raw(f"!! {msg}\n")

    def _log_warning(self, msg):
        logging.warning(f"{self._name}: {msg}")
        self._gcode.respond_raw(f"!! {msg}\n")

    def _log_info(self, msg):
        logging.info(f"{self._name}: {msg}")
        self._gcode.respond_info(msg)

    def _log_trace(self, msg):
        logging.debug(f"{self._name}: {msg}")

    def define_commands(self, gcode):
        gcode.register_command(
            "PROBE_EDDY_NG_STATUS", self.cmd_STATUS, self.cmd_STATUS_help
        )
        gcode.register_command(
            "PROBE_EDDY_NG_CALIBRATE",
            self.cmd_CALIBRATE,
            self.cmd_CALIBRATE_help,
        )
        gcode.register_command(
            "PROBE_EDDY_NG_SETUP",
            self.cmd_SETUP,
            self.cmd_SETUP_help,
        )
        gcode.register_command(
            "PROBE_EDDY_NG_CLEAR_CALIBRATION",
            self.cmd_CLEAR_CALIBRATION,
            self.cmd_CLEAR_CALIBRATION_help,
        )
        gcode.register_command(
            "PROBE_EDDY_NG_PROBE", self.cmd_PROBE, self.cmd_PROBE_help
        )
        gcode.register_command(
            "PROBE_EDDY_NG_PROBE_STATIC",
            self.cmd_PROBE_STATIC,
            self.cmd_PROBE_STATIC_help,
        )
        gcode.register_command(
            "PROBE_EDDY_NG_PROBE_ACCURACY",
            self.cmd_PROBE_ACCURACY,
            self.cmd_PROBE_ACCURACY_help,
        )
        gcode.register_command(
            "PROBE_EDDY_NG_TAP", self.cmd_TAP, self.cmd_TAP_help
        )
        gcode.register_command(
            "PROBE_EDDY_NG_SET_TAP_OFFSET",
            self.cmd_SET_TAP_OFFSET,
            "Set or clear the tap offset for the bed mesh scan and other probe operations",
        )
        gcode.register_command(
            "PROBE_EDDY_NG_SET_TAP_ADJUST_Z",
            self.cmd_SET_TAP_ADJUST_Z,
            "Set the tap adjustment value",
        )
        gcode.register_command(
            "PROBE_EDDY_NG_TEST_DRIVE_CURRENT",
            self.cmd_TEST_DRIVE_CURRENT,
            "Test a drive current.",
        )

        # some handy aliases while I'm debugging things to save my fingers
        gcode.register_command(
            "PES",
            self.cmd_STATUS,
            self.cmd_STATUS_help + " (alias for PROBE_EDDY_NG_STATUS)",
        )
        gcode.register_command(
            "PEP",
            self.cmd_PROBE,
            self.cmd_PROBE_help + " (alias for PROBE_EDDY_NG_PROBE)",
        )
        gcode.register_command(
            "PEPS",
            self.cmd_PROBE_STATIC,
            self.cmd_PROBE_STATIC_help
            + " (alias for PROBE_EDDY_NG_PROBE_STATIC)",
        )
        gcode.register_command(
            "PETAP",
            self.cmd_TAP,
            self.cmd_TAP_help + " (alias for PROBE_EDDY_NG_TAP)",
        )

    def _handle_command_error(self, gcmd=None):
        try:
            if self._sampler is not None:
                self._sampler.finish()
        except:
            logging.exception(
                "EDDYng handle_command_error: sampler.finish() failed"
            )

    def _handle_connect(self):
        self._toolhead = self._printer.lookup_object("toolhead")
        for msg in self.params._warning_msgs:
            self._log_warning(msg)

    def current_drive_current(self) -> int:
        return self._sensor.get_drive_current()

    def map_for_drive_current(self, dc: int = None) -> ProbeEddyFrequencyMap:
        if dc is None:
            dc = self.current_drive_current()
        if dc not in self._dc_to_fmap:
            raise self._printer.command_error(
                f"Drive current {dc} not calibrated"
            )
        return self._dc_to_fmap[dc]

    # helpers to forward to the map
    def height_to_freq(self, height: float, drive_current: int = None) -> float:
        if drive_current is None:
            drive_current = self.current_drive_current()
        return self.map_for_drive_current(drive_current).height_to_freq(height)

    def freq_to_height(self, freq: float, drive_current: int = None) -> float:
        if drive_current is None:
            drive_current = self.current_drive_current()
        return self.map_for_drive_current(drive_current).freq_to_height(freq)

    def calibrated(self, drive_current: int = None) -> bool:
        if drive_current is None:
            drive_current = self.current_drive_current()
        return (
            drive_current in self._dc_to_fmap
            and self._dc_to_fmap[drive_current].calibrated()
        )

    def _z_homed(self):
        curtime_r = self._printer.get_reactor().monotonic()
        kin_status = (
            self._printer.lookup_object("toolhead")
            .get_kinematics()
            .get_status(curtime_r)
        )
        return "z" in kin_status["homed_axes"]

    def _xy_homed(self):
        curtime_r = self._printer.get_reactor().monotonic()
        kin_status = (
            self._printer.lookup_object("toolhead")
            .get_kinematics()
            .get_status(curtime_r)
        )
        return (
            "x" in kin_status["homed_axes"] and "y" in kin_status["homed_axes"]
        )

    def _z_hop(self, by=5.0):
        if by < 0.0:
            raise self._printer.command_error("Z hop must be positive")
        toolhead: ToolHead = self._printer.lookup_object("toolhead")
        curpos = toolhead.get_position()
        curpos[2] = curpos[2] + by
        toolhead.manual_move(curpos, self.params.probe_speed)

    def _set_toolhead_position(self, pos, homing_axes):
        # klipper changed homing_axes to be a "xyz" string instead
        # of a tuple randomly on jan10 without support for the old
        # syntax
        func = self._toolhead.set_position
        kind = type(func.__defaults__[0])
        if kind is str:
            # new
            homing_axes_str = "".join(["xyz"[axis] for axis in homing_axes])
            return self._toolhead.set_position(pos, homing_axes=homing_axes_str)
        else:
            # old
            return self._toolhead.set_position(pos, homing_axes=homing_axes)

    def _z_not_homed(self):
        kin = self._toolhead.get_kinematics()
        # klipper got rid of this
        if hasattr(kin, "note_z_not_homed"):
            kin.note_z_not_homed()
        else:
            try:
                kin.clear_homing_state("z")
            except TypeError:
                raise self._printer.command_error(
                    "clear_homing_state failed: please update Klipper, your klipper is from the brief 5 day window where this was broken"
                )

    def save_config(self):
        for _, fmap in self._dc_to_fmap.items():
            fmap.save_calibration()

        configfile = self._printer.lookup_object("configfile")
        configfile.set(
            self._full_name,
            "calibrated_drive_currents",
            str.join(", ", [str(dc) for dc in self._dc_to_fmap.keys()]),
        )
        configfile.set(
            self._full_name,
            "calibration_version",
            str(ProbeEddyFrequencyMap.calibration_version),
        )
        if (
            self.params._config_reg_drive_current == 0
            or self.params.reg_drive_current
            != self.params._config_reg_drive_current
        ):
            if self.params._config_reg_drive_current != 0:
                self._log_warning(
                    f"Warning: reg_drive_current set in config ({self.params._config_reg_drive_current}) is different the value that is being saved. Please remove the config value, as it will override this one."
                )
            if self.params.reg_drive_current != 0:
                configfile.set(
                    self._full_name,
                    "saved_reg_drive_current",
                    str(self.params.reg_drive_current),
                )
        if (
            self.params._config_tap_drive_current == 0
            or self.params.tap_drive_current
            != self.params._config_tap_drive_current
        ):
            if self.params._config_tap_drive_current != 0:
                self._log_warning(
                    f"Warning: tap_drive_current set in config ({self.params._config_tap_drive_current}) is different the value that is being saved. Please remove the config value, as it will override this one."
                )
            if self.params.tap_drive_current != 0:
                configfile.set(
                    self._full_name,
                    "saved_tap_drive_current",
                    str(self.params.tap_drive_current),
                )

        self._log_info(
            "Calibration saved. Issue a SAVE_CONFIG to write the values to your config file and restart Klipper."
        )

    def start_sampler(self, *args, **kwargs) -> ProbeEddySampler:
        if self._sampler:
            raise self._printer.command_error(
                "EDDYng: Already sampling! (This shouldn't happen; FIRMWARE_RESTART to fix)"
            )
        self._sampler = ProbeEddySampler(self, *args, **kwargs)
        self._sampler.start()
        return self._sampler

    def sampler_is_active(self):
        return self._sampler is not None and self._sampler.active()

    # Called by samplers when they're finished
    def _sampler_finished(self, sampler: ProbeEddySampler, **kwargs):
        if self._sampler is not sampler:
            raise self._printer.command_error(
                "EDDYng finishing sampler that's not active"
            )

        self._sampler = None

        if self.save_samples_path is not None:
            with open(self.save_samples_path, "w") as data_file:
                samples = sampler.get_samples()
                raw_samples = sampler.get_raw_samples()
                data_file.write(
                    "time,frequency,z,kin_z,kin_v,raw_f,trigger_time,tap_end_time\n"
                )
                for i in range(len(samples)):
                    trigger_time = kwargs.get("trigger_time", "")
                    tap_end_time = kwargs.get("tap_end_time", "")
                    s_t, s_freq, s_z = samples[i]
                    _, raw_f, _ = raw_samples[i]
                    past_pos, past_v = get_toolhead_kin_pos(self._printer, s_t)
                    past_k_z = past_pos[2] if past_pos is not None else None
                    if past_k_z is None or past_v is None:
                        past_k_z = ""
                        past_v = ""
                    data_file.write(
                        f"{s_t},{s_freq},{s_z},{past_k_z},{past_v},{raw_f},{tap_end_time},{trigger_time}\n"
                    )
            logging.info(
                f"Wrote {len(samples)} samples to {self.save_samples_path}"
            )
            self.save_samples_path = None

        self._last_sampler_samples = sampler.get_samples()
        self._last_sampler_raw_samples = sampler.get_raw_samples()
        self._last_sampler_memos = sampler.memos

    cmd_STATUS_help = "Query the last raw coil value and status"

    def cmd_STATUS(self, gcmd: GCodeCommand):
        result = self._sensor.read_one_value()

        status = result.status
        freqval = result.freqval
        freq = result.freq
        height = self.freq_to_height(freq) if self.calibrated() else -math.inf

        err = ""
        if freqval > 0x0FFFFFFF:
            height = -math.inf
            freq = 0.0
            err = f"ERROR: {bin(freqval >> 28)} "
        if not self.calibrated():
            err += "(Not calibrated) "

        gcmd.respond_info(
            f"Last coil value: {freq:.2f} ({height:.3f}mm) raw: {hex(freqval)} {err}status: {hex(status)} {self._sensor.status_to_str(status)}"
        )

    cmd_PROBE_ACCURACY_help = "Probe accuracy"

    def cmd_PROBE_ACCURACY(self, gcmd: GCodeCommand):
        if not self._z_homed():
            raise self._printer.command_error(
                "Must home Z before PROBE_ACCURACY"
            )

        # How long to read at each sample time
        duration: float = gcmd.get_float("DURATION", 0.100, above=0.0)
        # whether to check +/- 1mm positions for accuracy
        start_z: float = gcmd.get_float("Z", 5.0)
        offsets = gcmd.get("OFFSETS", None)

        probe_speed = gcmd.get_float(
            "SPEED", self.params.probe_speed, above=0.0
        )
        lift_speed = gcmd.get_float(
            "LIFT_SPEED", self.params.lift_speed, above=0.0
        )

        probe_zs = [start_z]

        if offsets is not None:
            probe_zs.extend([float(v) + start_z for v in offsets.split(",")])
        else:
            probe_zs.extend(np.arange(0.5, start_z, 0.5).tolist())

        probe_zs.sort()
        probe_zs.reverse()

        # drive current to use
        old_drive_current = self.current_drive_current()
        drive_current: int = gcmd.get_int(
            "DRIVE_CURRENT", old_drive_current, minval=0, maxval=31
        )

        if not self.calibrated(drive_current):
            raise self._printer.command_error(
                f"Drive current {drive_current} not calibrated"
            )

        try:
            self._sensor.set_drive_current(drive_current)
            th = self._printer.lookup_object("toolhead")

            th.manual_move(
                [None, None, probe_zs[0] + 1.0],
                lift_speed,
            )
            th.wait_moves()

            results = []
            ranges = []
            from_zs = []
            stddev_sums = []
            stddev_count = 0

            for pz in probe_zs:
                th.manual_move([None, None, pz], probe_speed)
                th.dwell(0.050)
                th.wait_moves()

                result = self.probe_static_height(duration=duration)
                rangev = result.max_value - result.min_value
                from_z = result.value - pz
                stddev_sum = np.sum(
                    [(s - result.value) ** 2.0 for s in result.samples]
                )

                self._log_info(f"Probe at z={pz:.3f} is {result}")

                stddev_sums.append(stddev_sum)
                stddev_count += len(result.samples)
                results.append(result)
                ranges.append(rangev)
                from_zs.append(from_z)

            if len(results) > 1:
                avg_range = np.mean(ranges)
                avg_from_z = np.mean(from_zs)
                stddev = (np.sum(stddev_sums) / stddev_count) ** 0.5
                gcmd.respond_info(
                    f"Probe spread: {avg_range:.3f}, "
                    f"z deviation: {avg_from_z:.3f}, "
                    f"stddev: {stddev:.3f}"
                )

        finally:
            self._sensor.set_drive_current(old_drive_current)
            th.manual_move(
                [None, None, start_z],
                lift_speed,
            )

    cmd_CLEAR_CALIBRATION_help = "Clear calibration for all drive currents"

    def cmd_CLEAR_CALIBRATION(self, gcmd: GCodeCommand):
        drive_current: int = gcmd.get_int("DRIVE_CURRENT", -1)
        if drive_current == -1:
            self._dc_to_fmap = {}
            gcmd.respond_info("Cleared calibration for all drive currents")
        else:
            if drive_current not in self._dc_to_fmap:
                raise self._printer.command_error(
                    f"Drive current {drive_current} not calibrated"
                )
            del self._dc_to_fmap[drive_current]
            gcmd.respond_info(
                f"Cleared calibration for drive current {drive_current}"
            )
        self.save_config()

    def cmd_SET_TAP_OFFSET(self, gcmd: GCodeCommand):
        value = gcmd.get_float("VALUE", None)
        adjust = gcmd.get_float("ADJUST", None)
        tap_offset = self._tap_offset
        if value is not None:
            tap_offset = value
        if adjust is not None:
            tap_offset += adjust
        self._tap_offset = tap_offset
        gcmd.respond_info(f"Set tap offset: {tap_offset:.3f}")

    def cmd_SET_TAP_ADJUST_Z(self, gcmd: GCodeCommand):
        value = gcmd.get_float("VALUE", None)
        adjust = gcmd.get_float("ADJUST", None)
        tap_adjust_z = self._tap_adjust_z
        if value is not None:
            tap_adjust_z = value
        if adjust is not None:
            tap_adjust_z += adjust
        self._tap_adjust_z = tap_adjust_z

        if self.params.tap_adjust_z != self._tap_adjust_z:
            configfile = self._printer.lookup_object("configfile")
            configfile.set(
                self._full_name, "tap_adjust_z", str(float(self._tap_adjust_z))
            )

        gcmd.respond_info(
            f"Set tap_adjust_z: {tap_adjust_z:.3f} (SAVE_CONFIG to make it permanent)"
        )

    def probe_static_height(
        self, duration: float = 0.100
    ) -> ProbeEddyProbeResult:
        reactor = self._printer.get_reactor()
        now = self._mcu.estimated_print_time(reactor.monotonic())

        with self.start_sampler() as sampler:
            sampler.wait_for_sample_at_time(
                now + (duration + self._sensor._ldc_settle_time)
            )
            sampler.finish()

            samples = sampler.get_samples()
            if len(samples) == 0:
                return ProbeEddyProbeResult([])

            # skip LDC1612_SETTLETIME samples at start and end by looking
            # at the time values
            stime = samples[0][0] + self._sensor._ldc_settle_time
            etime = samples[-1][0]

            samples = [s[2] for s in samples if s[0] > stime]

            min_value = min(samples)
            max_value = max(samples)

            mean: float = float(np.mean(samples))
            median: float = float(np.median(samples))

            return ProbeEddyProbeResult(
                samples=samples,
                mean=float(mean),
                median=float(median),
                min_value=float(min_value),
                max_value=float(max_value),
                tstart=float(stime),
                tend=float(etime),
                errors=sampler.get_error_count(),
            )

    cmd_PROBE_help = "Probe the height using the eddy current sensor, moving the toolhead to the home trigger height, or Z if specified."

    def cmd_PROBE(self, gcmd: GCodeCommand):
        z: float = gcmd.get_float("Z", self.params.home_trigger_height)
        duration: float = gcmd.get_float("DURATION", 0.100, above=0.0)

        if not self._z_homed():
            raise self._printer.command_error("Must home Z before PROBE")

        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()
        if th_pos[2] < z:
            th.manual_move([None, None, z + 3.0], self.params.lift_speed)
        th.manual_move([None, None, z], self.params.lift_speed)
        th.dwell(0.100)
        th.wait_moves()

        self.cmd_PROBE_STATIC(gcmd)

    cmd_PROBE_STATIC_help = "Probe the current height using the eddy current sensor without moving the toolhead."

    def cmd_PROBE_STATIC(self, gcmd: GCodeCommand):
        old_drive_current = self.current_drive_current()
        drive_current: int = gcmd.get_int(
            "DRIVE_CURRENT", old_drive_current, minval=0, maxval=31
        )
        duration: float = gcmd.get_float("DURATION", 0.100, above=0.0)
        save: bool = gcmd.get_int("SAVE", 0) == 1
        home_z: bool = gcmd.get_int("HOME_Z", 0) == 1

        if not self.calibrated(drive_current):
            raise self._printer.command_error(
                f"Drive current {drive_current} not calibrated"
            )

        try:
            self._sensor.set_drive_current(drive_current)

            if save:
                self.save_samples_path = "/tmp/eddy-probe-static.csv"
            r = self.probe_static_height(duration)

            if self._cmd_helper is not None:
                self._cmd_helper.last_z_result = float(r.value)

            self._last_probe_result = r.value

            if home_z:
                th = self._printer.lookup_object("toolhead")
                th_pos = th.get_position()
                th_pos[2] = r.value
                self._set_toolhead_position(th_pos, [2])
                self._log_trace(f"Homed Z to {r}")
            else:
                self._log_info(f"Probed {r}")

        finally:
            self._sensor.set_drive_current(drive_current)

    cmd_SETUP_help = "Setup"

    def cmd_SETUP(self, gcmd: GCodeCommand):
        if not self._xy_homed():
            raise self._printer.command_error(
                "X and Y must be homed before setup"
            )

        if self._z_homed():
            # z-hop so that manual probe helper doesn't complain if we're already
            # at the right place
            self._z_hop()

        # Now reset the axis so that we have a full range to calibrate with
        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()
        # XXX This is proably not correct for some printers?
        zrange = th.get_kinematics().rails[2].get_range()
        th_pos[2] = zrange[1] - 20.0
        self._set_toolhead_position(th_pos, [2])

        manual_probe.ManualProbeHelper(
            self._printer,
            gcmd,
            lambda kin_pos: self.cmd_SETUP_next(gcmd, kin_pos),
        )

    def cmd_SETUP_next(self, gcmd: GCodeCommand, kin_pos: List[float]):
        if kin_pos is None:
            # User cancelled ManualProbeHelper
            self._z_not_homed()
            return

        # We just did a ManualProbeHelper, so we're going to zero the z-axis
        # to make the following code easier, so it can assume z=0 is actually real zero.
        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()
        th_pos[2] = 0.0
        self._set_toolhead_position(th_pos, [2])

        old_drive_current = self.current_drive_current()
        # Note that the default is the default drive current
        drive_current: int = gcmd.get_int(
            "DRIVE_CURRENT",
            self._sensor._default_drive_current,
            minval=0,
            maxval=31,
        )

        max_drive_current_increase = 0
        if self._sensor_type == "ldc1612" or self._sensor_type == "btt_eddy":
            max_drive_current_increase = 5
        max_drive_current_increase = gcmd.get_int(
            "MAX_DC_INCREASE", max_drive_current_increase, minval=0, maxval=30
        )

        # lift up above cal_z_max, and then move over so the probe
        # is over the nozzle position
        th.manual_move(
            [None, None, self.params.calibration_z_max + 3.0],
            self.params.lift_speed,
        )
        th.manual_move(
            [
                th_pos[0] - self.offset["x"],
                th_pos[1] - self.offset["y"],
                None,
            ],
            self.params.move_speed,
        )

        # This is going to automate setup.
        # The setup state machine looks like this:
        # 1. Finding homing drive current
        # 2. Finding tapping drive current
        FINDING_HOMING = 1
        FINDING_TAP = 2
        DONE = 3

        start_drive_current = drive_current
        result_msg = None

        self._log_info("EDDYng setup: calibrating homing")
        state = FINDING_HOMING
        while state < DONE:
            mapping, fth_rms, htf_rms = self._create_mapping(
                self.params.calibration_z_max,
                0.0,  # z_target
                self.params.probe_speed,
                self.params.lift_speed,
                drive_current,
                report_errors=False,
                write_debug_files=False,
            )

            homing_req_min = 0.5
            homing_req_max = 5.0
            tap_req_min = 0.025
            tap_req_max = 3.0

            ok_for_homing = mapping is not None
            ok_for_tap = mapping is not None

            if ok_for_homing and (
                mapping.height_range[0] > homing_req_min
                or mapping.height_range[1] < homing_req_max
            ):
                ok_for_homing = False
            if ok_for_tap and (
                mapping.height_range[0] > tap_req_min
                or mapping.height_range[1] < tap_req_max
            ):
                ok_for_tap = False

            if ok_for_homing or ok_for_tap:
                if mapping.freq_spread() < 0.30:
                    self._log_warning(
                        f"EDDYng warning: frequency spread {mapping.freq_spread()} is very low at drive current {drive_current}. If setup fails completely, the sensor is probably mounted too high."
                    )
                    ok_for_homing = ok_for_tap = False
                if fth_rms > 0.050:
                    self._log_info(
                        f"EDDYng: calibration error rate is too high ({fth_rms}) at drive current {drive_current}. Ignoring."
                    )
                    ok_for_homing = ok_for_tap = False

            if (state == FINDING_HOMING and not ok_for_homing) or (
                state == FINDING_TAP and not ok_for_tap
            ):
                # calibration failed, have we exceeded our max DC increase?
                if (
                    drive_current - start_drive_current
                    < max_drive_current_increase
                ):
                    drive_current += 1
                    continue

                if state == FINDING_HOMING:
                    result_msg = "Failed to find homing drive current."
                elif state == FINDING_TAP:
                    result_msg = "Failed to find tap drive current, but homing is set up."
                else:
                    result_msg = "Unknown state?"
                self._log_error(state)
                break

            # success
            self._dc_to_fmap[drive_current] = mapping
            if state == FINDING_HOMING and ok_for_homing:
                self.params.reg_drive_current = drive_current
                self._log_info(f"EDDYng using {drive_current} for homing.")
                state = FINDING_TAP
            if state == FINDING_TAP and ok_for_tap:
                self.params.tap_drive_current = drive_current
                self._log_info(f"EDDYng using {drive_current} for tap.")
                state = DONE

            result_msg = "EDDYng setup success. Please check whether homing works with G28 Z, then check if tap works with PROBE_EDDY_NG_TAP."

        if state == DONE:
            self._log_info(result_msg)
        else:
            self._log_error(result_msg)

        if state > FINDING_HOMING:
            self.save_config()

        self._z_not_homed()

    cmd_CALIBRATE_help = (
        "Calibrate the eddy current sensor. Specify DRIVE_CURRENT to calibrate for a different drive current "
        + "than the default. Specify Z_MAX to set a different calibration start point."
    )

    def cmd_CALIBRATE(self, gcmd: GCodeCommand):
        if not self._xy_homed():
            raise self._printer.command_error(
                "X and Y must be homed before calibrating"
            )

        if self._z_homed():
            # z-hop so that manual probe helper doesn't complain if we're already
            # at the right place
            self._z_hop()

        # Now reset the axis so that we have a full range to calibrate with
        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()
        # XXX This is proably not correct for some printers?
        zrange = th.get_kinematics().rails[2].get_range()
        th_pos[2] = zrange[1] - 20.0
        self._set_toolhead_position(th_pos, [2])

        manual_probe.ManualProbeHelper(
            self._printer,
            gcmd,
            lambda kin_pos: self.cmd_CALIBRATE_next(gcmd, kin_pos),
        )

    def cmd_CALIBRATE_next(self, gcmd: GCodeCommand, kin_pos: List[float]):
        th = self._printer.lookup_object("toolhead")
        if kin_pos is None:
            # User cancelled ManualProbeHelper
            self._z_not_homed()
            return

        old_drive_current = self.current_drive_current()
        drive_current: int = gcmd.get_int(
            "DRIVE_CURRENT", old_drive_current, minval=0, maxval=31
        )
        cal_z_max: float = gcmd.get_float(
            "Z_MAX", self.params.calibration_z_max, above=2.0
        )
        z_target: float = gcmd.get_float("Z_TARGET", 0.0)

        probe_speed: float = gcmd.get_float(
            "SPEED", self.params.probe_speed, above=0.0
        )
        lift_speed: float = gcmd.get_float(
            "LIFT_SPEED", self.params.lift_speed, above=0.0
        )

        # We just did a ManualProbeHelper, so we're going to zero the z-axis
        # to make the following code easier, so it can assume z=0 is actually real zero.
        # The Eddy sensor calibration is done to nozzle height (not sensor or trigger height).
        th_pos = th.get_position()
        th_pos[2] = 0.0
        self._set_toolhead_position(th_pos, [2])

        th.wait_moves()

        logging.info(f"EDDYng calibrating from {kin_pos}, {th_pos}")

        # lift up above cal_z_max, and then move over so the probe
        # is over the nozzle position
        th.manual_move([None, None, cal_z_max + 3.0], lift_speed)
        th.manual_move(
            [
                th_pos[0] - self.offset["x"],
                th_pos[1] - self.offset["y"],
                None,
            ],
            self.params.move_speed,
        )

        mapping, fth_fit, htf_fit = self._create_mapping(
            cal_z_max,
            z_target,
            probe_speed,
            lift_speed,
            drive_current,
            report_errors=True,
            write_debug_files=True,
        )
        if mapping is None or fth_fit is None or htf_fit is None:
            self._log_error("Calibration failed")
            return

        self._dc_to_fmap[drive_current] = mapping
        self.save_config()

        # reset the Z homing state after alibration
        self._z_not_homed()

    def _create_mapping(
        self,
        z_start: float,
        z_target: float,
        probe_speed: float,
        lift_speed: float,
        drive_current: int,
        report_errors: bool,
        write_debug_files: bool,
    ) -> Tuple[ProbeEddyFrequencyMap, float, float]:
        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()

        # move to the start z of the mapping, going up first if we need to for backlash
        if th_pos[2] < z_start:
            th.manual_move([None, None, z_start + 3.0], lift_speed)
        th.manual_move([None, None, z_start], lift_speed)

        old_drive_current = self.current_drive_current()
        try:
            self._sensor.set_drive_current(drive_current)
            times, freqs, heights = self._capture_samples_down_to(
                z_target, probe_speed
            )
            th.manual_move([None, None, z_start + 3.0], lift_speed)
        finally:
            self._sensor.set_drive_current(old_drive_current)

        if times is None:
            if report_errors:
                self._log_error(
                    "No samples collected. This could be a hardware issue or an incorrect drive current."
                )
            return None, None, None

        # and build a map
        mapping = ProbeEddyFrequencyMap(self)
        fth_fit, htf_fit = mapping.calibrate_from_values(
            drive_current,
            times,
            freqs,
            heights,
            report_errors,
            write_debug_files,
        )
        return mapping, fth_fit, htf_fit

    def _capture_samples_down_to(
        self, z_target: float, probe_speed: float
    ) -> tuple[List[float], List[float], List[float]]:
        th = self._printer.lookup_object("toolhead")
        th.dwell(0.500)  # give the sensor a bit to settle
        th.wait_moves()

        with self.start_sampler(calculate_heights=False) as sampler:
            first_sample_time = th.get_last_move_time()
            th.manual_move([None, None, z_target], probe_speed)
            last_sample_time = th.get_last_move_time()
            # Can't use wait_for_sample_at_time here, because the tail end of
            # samples might be errors so they won't be passed to the sampler.
            # Should fix that, but for now just wait an extra half second which
            # should be more than enough.
            # sampler.wait_for_sample_at_time(last_sample_time)
            th.dwell(0.500)
            th.wait_moves()
            sampler.finish()

        # the samples are a list of [print_time, freq, dummy_height] tuples
        samples = sampler.get_samples()
        if len(samples) == 0:
            return None, None, None

        freqs = []
        heights = []
        times = []

        for s_t, s_freq, _ in samples:
            s_pos, _ = get_toolhead_kin_pos(self._printer, at=s_t)
            s_z = s_pos[2]
            if first_sample_time < s_t < last_sample_time and s_z >= z_target:
                times.append(s_t)
                freqs.append(s_freq)
                heights.append(s_z)

        return times, freqs, heights

    def cmd_TEST_DRIVE_CURRENT(self, gcmd: GCodeCommand):
        drive_current: int = gcmd.get_int(
            "DRIVE_CURRENT", self.params.reg_drive_current, minval=1, maxval=31
        )
        z_start: float = gcmd.get_float(
            "Z_MAX", self.params.calibration_z_max, above=2.0
        )
        z_end: float = gcmd.get_float("Z_TARGET", 0.0)
        self._log_info(f"Testing Z={z_start:.3f} to Z={z_end:.3f}")

        mapping, fth, htf = self._create_mapping(
            z_start,
            z_end,
            self.params.probe_speed,
            self.params.lift_speed,
            drive_current,
            report_errors=False,
            write_debug_files=False,
        )
        if mapping is None or fth is None or htf is None:
            self._log_error(
                f"Test failed: drive current {drive_current} is not usable."
            )
            return

    #
    # PrinterProbe interface
    #

    def get_offsets(self):
        # the z offset is the trigger height, because the probe will trigger
        # at z=trigger_height (not at z=0)
        return (
            self.offset["x"],
            self.offset["y"],
            self.params.home_trigger_height,
        )

    def get_probe_params(self, gcmd=None):
        return {
            "probe_speed": self.params.probe_speed,
            "lift_speed": self.params.lift_speed,
            "sample_retract_dist": 0.0,
        }

    def start_probe_session(self, gcmd):
        session = ProbeEddyScanningProbe(self, gcmd)
        session._start_session()
        return session
        # method = gcmd.get('METHOD', 'automatic').lower()
        # if method in ('scan', 'rapid_scan'):
        #    session = ProbeEddyScanningProbe(self, gcmd)
        #    session._start_session()
        #    return session
        #
        # return self._probe_session.start_probe_session(gcmd)

    def get_status(self, eventtime):
        if self._cmd_helper is not None:
            status = self._cmd_helper.get_status(eventtime)
        else:
            status = dict()
        status.update(
            {
                "name": self._full_name,
                "home_trigger_height": self.params.home_trigger_height,
                "tap_offset": self._tap_offset,
                "last_probe_result": self._last_probe_result,
            }
        )
        return status

    # Old Probe interface, for Kalico

    def get_lift_speed(self, gcmd=None):
        if gcmd is not None:
            return gcmd.get_float(
                "LIFT_SPEED", self.params.lift_speed, above=0.0
            )
        return self.params.lift_speed

    def multi_probe_begin(self):
        pass

    def multi_probe_end(self):
        pass

    # This is a mishmash of cmd_PROBE and cmd_PROBE_STATIC. This run_probe
    # is the old one, different than the scanning session run_probe.
    def run_probe(self, gcmd=None):
        z = self.params.home_trigger_height
        duration = 0.100

        if not self._z_homed():
            raise self._printer.command_error("Must home Z before PROBE")

        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()
        if th_pos[2] < z:
            th.manual_move([None, None, z + 3.0], self.params.lift_speed)
        th.manual_move([None, None, z], self.params.lift_speed)
        th.dwell(0.100)
        th.wait_moves()

        if not self.calibrated():
            raise self._printer.command_error("Eddy probe not calibrated!")

        r = self.probe_static_height(duration)
        if not r.valid:
            raise self._printer.command_error("Probe captured no samples!")

        # At what Z position would the toolhead be at for the probe to read
        # _home_trigger_height? In other words, if the probe tells us
        # the height is 1.5 when the toolhead is at z=2.0, if the toolhead
        # was moved up to 2.5, then the probe should read 2.0.
        probe_z = z + (z - r.value)

        return [th_pos[0], th_pos[1], probe_z]

    #
    # Moving the sensor to the correct position
    #
    def _probe_to_start_position_unhomed(self, move_home=False):
        if not self._xy_homed():
            raise self._printer.command_error("xy must be homed")
        if not self.sampler_is_active():
            raise self._printer.command_error(
                "probe_to_start_position_unhomed: no sampler active"
            )
        if not self.calibrated():
            raise self._printer.command_error("EDDYng not calibrated!")

        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()

        # debug logging
        th_kin = th.get_kinematics()
        zlim = th_kin.limits[2]
        rail_range = th_kin.rails[2].get_range()
        logging.info(
            f"EDDYng probe to start unhomed: before movement: Z pos {th_pos[2]:.3f}, "
            f"Z limits {zlim[0]:.2f}-{zlim[1]:.2f}, "
            f"rail range {rail_range[0]:.2f}-{rail_range[1]:.2f}"
        )

        start_height_ok_factor = 0.100

        # This is where we want to get to
        start_height = self._home_start_height
        # This is where the probe thinks we are
        now_height = self._sampler.get_height_now()

        # If we can't get a value at all for right now, for safety, just abort.
        if now_height is None:
            raise self._printer.command_error(
                "Couldn't get any valid samples from sensor. "
                "If the toolhead is high off the build plate, this usually "
                "indicates a bad reg_drive_current."
            )

        logging.info(
            f"EDDYng probe_to_start_position_unhomed: now: {now_height} (start {start_height})"
        )
        if abs(now_height - start_height) <= start_height_ok_factor:
            return

        th = self._printer.lookup_object("toolhead")
        th_pos = th.get_position()

        MOVE_DOWN = False
        if MOVE_DOWN:
            # the max amount to move down while moving to a useful home start
            # position
            MAX_HOME_MOVE_DIST = 2.0

            # First move down if we need to, checking the sensor every 2mm.
            # TODO: we don't really need to do this at all -- if the sensor is giving
            # us valid values for high off the build plate, even if they're way off
            # we can still just do a homing move down and we'll be fine.
            # The only time this is necessary is if the sensor is giving errors for too
            # high up (e.g. amplitude too low)
            while now_height > start_height:
                move_dist = min(now_height - start_height, MAX_HOME_MOVE_DIST)
                th_pos[2] -= move_dist
                logging.info(
                    f"EDDYng probe_to_start_position_unhomed: moving toolhead down by {move_dist:.3f} to {th_pos[2]:.3f}"
                )
                th.manual_move([None, None, th_pos[2]], self.params.probe_speed)
                self._sampler.wait_for_sample_at_time(th.get_last_move_time())
                now_height = self._sampler.get_last_height()

        # This one we do need to do though -- if the sensor thinks we're too low we need
        # to pop back up.
        if now_height < start_height:
            move_up_by = start_height - now_height
            # give ourselves some room to do so, homing typically doesn't move up,
            # and we should know that we have this room because the sensor tells us we're too low
            th_pos[2] = rail_range[1] - (move_up_by + 10.0)
            logging.info(
                f"EDDYng probe_to_start_position_unhomed: resetting toolhead to z {th_pos[2]:.3f}"
            )
            self._set_toolhead_position(th_pos, [2])

            n_pos = th.get_position()

            zlim = th_kin.limits[2]
            rail_range = th_kin.rails[2].get_range()
            logging.info(
                f"EDDYng: after reset: Z pos {n_pos[2]:.3f}, Z limits {zlim[0]:.2f}-{zlim[1]:.2f}, "
                f"rail range {rail_range[0]:.2f}-{rail_range[1]:.2f}"
            )

            th_pos[2] += move_up_by
            logging.info(
                f"EDDYng probe_to_start_position_unhomed: moving toolhead up by {move_up_by:.3f} to {th_pos[2]:.3f}"
            )
            th.manual_move([None, None, th_pos[2]], self.params.probe_speed)
            self._sampler.wait_for_sample_at_time(th.get_last_move_time())
            now_height = self._sampler.get_last_height()

    def probe_to_start_position(self, z_pos=None):
        logging.info(
            f"EDDYng probe_to_start_position (tt: {self.params.tap_threshold}, z-homed: {self._z_homed()})"
        )

        # If we're not homed at all, rely on the sensor values to bring us to
        # a good place to start a diving probe from
        if not self._z_homed():
            if z_pos is not None:
                raise self._printer.command_error(
                    "Can't probe_to_start_position with an explicit Z without homed Z"
                )
            self._probe_to_start_position_unhomed()
            return

        th = self._printer.lookup_object("toolhead")
        th.wait_moves()
        th_pos = th.get_position()

        # Note home_trigger_height and not home_start_height: if we're homed,
        # we don't need to do another dive and we just want to move to
        # the right position for probing.
        if z_pos is not None:
            start_z = z_pos
        else:
            start_z = self.params.home_trigger_height

        # If we're below, move up a bit beyond and the back down
        # to compensate for backlash
        if th_pos[2] < start_z:
            logging.info(
                f"EDDYng probe_to_start_position: moving toolhead from {th_pos[2]:.3f} to {(start_z + 1.0):.3f}"
            )
            th_pos[2] = start_z + 1.0
            th.manual_move(th_pos, self.params.lift_speed)

        logging.info(
            f"EDDYng probe_to_start_position: moving toolhead from {th_pos[2]:.3f} to {start_z:.3f}"
        )
        th_pos[2] = start_z
        th.manual_move(th_pos, self.params.probe_speed)

        th.wait_moves()

    #
    # Tap probe
    #
    cmd_TAP_help = "Calculate a z-offset by touching the build plate."

    def cmd_TAP(self, gcmd: GCodeCommand):
        drive_current = self._sensor.get_drive_current()
        try:
            self.cmd_TAP_next(gcmd)
        finally:
            self._sensor.set_drive_current(drive_current)

    @dataclass
    class TapResult:
        error: Exception
        probe_z: float
        toolhead_z: float
        overshoot: float
        tap_start_time: float
        tap_end_time: float

    @dataclass
    class TapConfig:
        mode: str
        threshold: float
        sos: List[List[float]] = None

    # self.save_samples_path = "/tmp/tap-samples.csv"

    def do_one_tap(
        self,
        start_z: float,
        target_z: float,
        tap_speed: float,
        lift_speed: float,
        tapcfg: ProbeEddy.TapConfig,
    ) -> TapResult:
        self.probe_to_start_position(start_z)

        th = self._printer.lookup_object("toolhead")

        target_position = th.get_position()
        target_position[2] = target_z

        error = None

        try:
            # configure the endstop for tap (gets reset at the end of a tap sequence,
            # also in finally just in case
            self._endstop_wrapper.tap_config = tapcfg

            endstops = [(self._endstop_wrapper, "probe")]
            hmove = HomingMove(self._printer, endstops)

            try:
                probe_position = hmove.homing_move(
                    target_position, tap_speed, probe_pos=True
                )

                if hmove.check_no_movement() is not None:
                    raise self._printer.command_error(
                        "Probe triggered prior to movement"
                    )

                th_pos_z = th.get_position()[2]

                if probe_position[2] - target_z < 0.050:
                    # we detected a tap but it was too close to our target z
                    # to be trusted
                    return ProbeEddy.TapResult(
                        error=Exception("Tap detected too close to target z"),
                        toolhead_z=th_pos_z,
                        probe_z=probe_position[2],
                        overshoot=0.0,
                        tap_start_time=0.0,
                        tap_end_time=0.0,
                    )

            except self._printer.command_error as err:
                if self._printer.is_shutdown():
                    raise self._printer.command_error(
                        "Probing failed due to printer shutdown"
                    )

                # in case of failure don't leave the toolhead in a bad spot (i.e. in bed)
                th_pos_z = th.get_position()[2]
                if th_pos_z < 1.0:
                    th.manual_move([None, None, start_z], lift_speed)

                # If just sensor errors, let the caller handle it
                if "Sensor error" or "Probe completed movement" in str(err):
                    return ProbeEddy.TapResult(
                        error=err,
                        toolhead_z=th_pos_z,
                        probe_z=0.0,
                        overshoot=0.0,
                        tap_start_time=0.0,
                        tap_end_time=0.0,
                    )
                else:
                    self._log_error(f"Tap failed at {th_pos_z:.3f}")
                    raise
        finally:
            self._endstop_wrapper.tap_config = None

        # this Z value is what this tap detected as "true height=0"
        probe_z = probe_position[2]
        # where the toolhead is now (note: this is set in both the success and failure cases;
        # on failure, before we retract)
        now_z = th_pos_z

        # we're at now_z, but probe_z is the actual zero. We expect now_z
        # to be below or equal to probe_z because there will always be
        # a bit of overshoot due to trigger delay, and because we actually
        # fire the trigger later than when the tap starts (and the tap start
        # time is what's used to compute probe_position)
        if now_z > probe_z:
            raise self._printer.command_error(
                f"Unexpected: now_z {now_z:.3f} is above probe_z {probe_z:.3f} after tap"
            )

        # How much the toolhead overshot the real z=0 position. This is the amount
        # the toolhead is pushing into the build plate.
        overshoot = probe_z - now_z

        tap_start_time = self._endstop_wrapper.last_trigger_time
        tap_end_time = self._endstop_wrapper.last_tap_end_time

        return ProbeEddy.TapResult(
            error=error,
            probe_z=probe_z,
            toolhead_z=now_z,
            overshoot=overshoot,
            tap_start_time=tap_start_time,
            tap_end_time=tap_end_time,
        )

    def _compute_butter_tap(self, samples, raw_samples):
        if not HAS_SCIPY:
            return None, None

        trigger_freq = self.height_to_freq(self.params.home_trigger_height)

        s_f = np.asarray([s[1] for s in samples])
        first_one = np.argmax(s_f >= trigger_freq)
        logging.info(f"{first_one}")
        s_t = np.asarray([s[0] for s in samples[first_one:]])
        s_f = np.asarray([s[1] for s in samples[first_one:]])

        lowcut = self.params.tap_butter_lowcut
        highcut = self.params.tap_butter_highcut
        order = self.params.tap_butter_order

        sos = scipy.signal.butter(
            order,
            [lowcut, highcut],
            btype="bandpass",
            fs=self._sensor._data_rate,
            output="sos",
        )
        filtered = scipy.signal.sosfilt(sos, s_f - s_f[0])

        return s_t, filtered

    def cmd_TAP_next(self, gcmd: Optional[GCodeCommand] = None):
        self._log_trace("\nEDDYng Tap begin")

        if gcmd is None:
            gcmd = self._dummy_gcode_cmd

        tap_drive_current = self.params.tap_drive_current
        if tap_drive_current == 0:
            tap_drive_current = self.params.reg_drive_current
        tap_drive_current: int = gcmd.get_int(
            name="DRIVE_CURRENT",
            default=tap_drive_current,
            minval=0,
            maxval=31,
        )
        tap_speed: float = gcmd.get_float(
            "SPEED", self.params.tap_speed, above=0.0
        )
        lift_speed: float = gcmd.get_float(
            "RETRACT_SPEED", self.params.lift_speed, above=0.0
        )
        tap_start_z: float = gcmd.get_float(
            "START_Z", self.params.tap_start_z, above=2.0
        )
        target_z: float = gcmd.get_float("TARGET_Z", self.params.tap_target_z)
        tap_threshold: int = gcmd.get_float(
            "THRESHOLD", None
        )  # None so we have a sentinel value
        # tap_threshold = gcmd.get_int('TT', tap_threshold, minval=0) # alias for THRESHOLD
        tap_threshold = gcmd.get_float(
            "TT", tap_threshold
        )  # alias for THRESHOLD
        tap_adjust_z = gcmd.get_float("ADJUST_Z", self._tap_adjust_z)
        do_retract = gcmd.get_int("RETRACT", 1) == 1
        samples = gcmd.get_int("SAMPLES", self.params.tap_samples, minval=1)
        max_samples = gcmd.get_int(
            "MAX_SAMPLES", self.params.tap_max_samples, minval=samples
        )
        samples_stddev = gcmd.get_float(
            "SAMPLES_STDDEV", self.params.tap_samples_stddev, above=0.0
        )

        mode = gcmd.get("MODE", self.params.tap_mode).lower()
        if mode not in ("wma", "butter"):
            raise self._printer.command_error(f"Invalid mode: {mode}")

        # if the mode is different than the params, then require
        # specifying threshold
        if tap_threshold is None:
            if mode != self.params.tap_mode:
                raise self._printer.command_error(
                    f"THRESHOLD required when mode ({mode}) is different than configured default ({self.params.tap_mode})"
                )
            tap_threshold = self.params.tap_threshold

        if not self._z_homed():
            raise self._printer.command_error(
                "Z axis must be homed before tapping"
            )

        tapcfg = ProbeEddy.TapConfig(mode=mode, threshold=tap_threshold)
        if mode == "butter":
            if (
                self.params.is_default_butter_config()
                and self._sensor._data_rate == 250
            ):
                sos = [
                    [
                        0.046131802093312926,
                        0.09226360418662585,
                        0.046131802093312926,
                        1.0,
                        -1.3297767184682712,
                        0.5693902189294331,
                    ],
                    [
                        1.0,
                        -2.0,
                        1.0,
                        1.0,
                        -1.845000600983779,
                        0.8637525213328747,
                    ],
                ]
            elif (
                self.params.is_default_butter_config()
                and self._sensor._data_rate == 500
            ):
                sos = [
                    [
                        0.013359200027856505,
                        0.02671840005571301,
                        0.013359200027856505,
                        1.0,
                        -1.686278256753083,
                        0.753714473246724,
                    ],
                    [
                        1.0,
                        -2.0,
                        1.0,
                        1.0,
                        -1.9250515947328444,
                        0.9299234737648037,
                    ],
                ]
            elif HAS_SCIPY:
                sos = scipy.signal.butter(
                    self.params.tap_butter_order,
                    [
                        self.params.tap_butter_lowcut,
                        self.params.tap_butter_highcut,
                    ],
                    btype="bandpass",
                    fs=self._sensor._data_rate,
                    output="sos",
                ).tolist()
            else:
                raise self._printer.command_error(
                    "Scipy is not available, cannot use custom filter, or data rate is not 250 or 500"
                )
            tapcfg.sos = sos

        results = []
        tap_z = None
        tap_stddev = None
        sample_err_count = 0
        tap = None

        try:
            self._sensor.set_drive_current(tap_drive_current)

            sample_i = 0
            sample_last_err = None

            while sample_i < max_samples:
                self.save_samples_path = "/tmp/tap-samples.csv"
                tap = self.do_one_tap(
                    start_z=tap_start_z,
                    target_z=target_z,
                    tap_speed=tap_speed,
                    lift_speed=lift_speed,
                    tapcfg=tapcfg,
                )
                sample_i += 1

                if tap.error:
                    if "too close to target z" in str(tap.error):
                        self._log_info(
                            f"Tap {sample_i}: failed: try lowering TARGET_Z by 0.100 (to {target_z - 0.100:.3f})"
                        )
                    else:
                        self._log_info(f"Tap {sample_i}: failed ({tap.error})")
                    sample_err_count += 1
                    sample_last_err = tap
                    continue

                results.append(tap)

                self._log_info(f"Tap {sample_i}: z={tap.probe_z:.3f}")
                self._log_trace(
                    f"EDDYng tap[{sample_i}]: {tap.probe_z:.3f} toolhead at: {tap.toolhead_z:.3f} "
                    f"overshoot: {tap.overshoot:.3f} at {tap.tap_start_time:.4f}s"
                )

                if samples == 1:
                    # only one sample, we're done
                    tap_z = tap.probe_z
                    tap_stddev = 0.0
                    break

                if len(results) >= samples:
                    tap_z, tap_stddev = self._compute_tap_z(
                        results, samples, samples_stddev
                    )
                    if tap_z is not None:
                        break
        finally:
            self._sensor.set_drive_current(self.params.reg_drive_current)
            # This only writes the plot for the very last tap
            if self.params.write_tap_plot:
                self._write_tap_plot(tap)

        # If we didn't compute a tap_z report the error
        if tap_z is None:
            err_msg = "Tap failed:"
            if tap_stddev is not None:
                err_msg += f" stddev {tap_stddev:.3f} > {samples_stddev:.3f}."
                err_msg += " Consider adjusting tap_samples, tap_max_samples, or tap_samples_stddev."
            if sample_err_count > 0:
                err_msg += f" {sample_err_count} errors, last: {sample_last_err.error} at toolhead z={sample_last_err.toolhead_z:.3f}"
            self._log_error(err_msg)
            raise self._printer.command_error("Tap failed")

        # Adjust the computed tap_z by the user's tap_adjust_z, typically to raise
        # it to account for flex in the system (otherwise the Z would be too low)
        adjusted_tap_z = tap_z + tap_adjust_z

        gcode_move = self._printer.lookup_object("gcode_move")
        gcode_delta = adjusted_tap_z - gcode_move.homing_position[2]
        gcode_move.base_position[2] += gcode_delta
        gcode_move.homing_position[2] = adjusted_tap_z

        #
        # Figure out the offset to apply to sensor readings at the home trigger height
        # for future probes.
        #
        # This is actually unrelated to tap, but is related to temperature compensation.
        # Bed mesh is going to read values relative to the probe's z_offset (home_trigger_height).
        # But we can't trust the probe's values directly, because of temperature effects.
        #
        # What we can do though is move the toolhead to that height, take a probe reading,
        # then save the delta there to apply as an offset for bed mesh in the future.
        # That makes this bed height effectively "0", which is fine, because this is
        # what we did tap at to get a height zero reading.
        #
        # Toolhead moves are absolute; they don't take into account the gcode offset.
        # Probes happen at absolute z=z_offset, so this doesn't take into account the
        # tap_z computed above. This does mean that the actual physical height probing happens at
        # is not likely to be exactly the same as the Z position, but all we care about is
        # variance from that position so this should be fine.
        th = self._printer.lookup_object("toolhead")
        th.manual_move(
            [None, None, self.params.home_trigger_height + 1.0], lift_speed
        )
        th.manual_move([None, None, self.params.home_trigger_height], tap_speed)
        th.dwell(0.500)
        th.wait_moves()

        result = self.probe_static_height()
        self._tap_offset = self.params.home_trigger_height - result.value

        self._log_info(
            f"Probe computed z offset {adjusted_tap_z:.3f} (tap at z={tap_z:.3f}, stddev {tap_stddev:.3f}),"
            f" sensor offset {self._tap_offset:.3f} at z={self.params.home_trigger_height:.3f}"
        )

        if abs(self._tap_offset) > 0.300:  # arbitrary
            self._log_error(
                f"WARNING: Sensor offset is high ({self._tap_offset:.3f}); consider recalibrating sensor"
            )

        if do_retract:
            th.manual_move([None, None, self._home_start_height], lift_speed)
            th.wait_moves()
            th.flush_step_generation()

        self._log_trace("EDDYng Tap end\n")

    # Compute the average tap_z from a set of tap results, taking a cluster of samples
    # from the result that has the lowest standard deviation
    def _compute_tap_z(
        self, taps: List[ProbeEddy.TapResult], samples: int, req_stddev: float
    ) -> Tuple[float, float]:
        if len(taps) < samples:
            return None, None

        tap_z = math.inf
        std_min = math.inf
        for cluster in combinations(taps, samples):
            tap_zs = np.array([t.probe_z for t in cluster])
            mean = np.mean(tap_zs)
            std = np.std(tap_zs)
            if std < std_min:
                std_min = std
                tap_z = mean

        if std_min <= req_stddev:
            return float(tap_z), float(std_min)
        else:
            return None, float(std_min)

    def _write_tap_plot(self, tap: ProbeEddy.TapResult):
        if not HAS_PLOTLY:
            return

        samples = self._last_sampler_samples
        raw_samples = self._last_sampler_raw_samples
        memos = self._last_sampler_memos
        if samples is None or raw_samples is None:
            return

        th = self._printer.lookup_object("toolhead")

        s_t = np.asarray([s[0] for s in samples])
        s_rf = s_f = np.asarray([s[1] for s in raw_samples])
        s_true_f = np.asarray([s[1] for s in samples])
        s_z = np.asarray([s[2] for s in samples])
        s_kinz = np.asarray(
            [get_toolhead_kin_pos(self._printer, s[0])[0][2] for s in samples]
        )

        time_start = s_t.min()

        # normalize times to start at 0
        s_t = s_t - time_start
        trigger_time = memos.get("trigger_time", time_start) - time_start
        tap_end_time = memos.get("tap_end_time", time_start) - time_start
        tap_threshold = memos.get("tap_threshold", 0)

        # compute the butterworth filter, if we have scipy
        if tap is not None and HAS_SCIPY:
            butter_s_t, butter_s_v = self._compute_butter_tap(
                samples, raw_samples
            )
            butter_s_t = butter_s_t - time_start
        else:
            butter_s_t = butter_s_v = None

        # Compute exactly (mostly) how the C code does it, so that the accum
        # values are identical
        FREQ_WINDOW_SIZE = 16
        WMA_D_WINDOW_SIZE = 4
        s_freq_weight_sum = (FREQ_WINDOW_SIZE * (FREQ_WINDOW_SIZE + 1)) / 2

        c_freq_buffer = np.zeros(FREQ_WINDOW_SIZE, np.uint32)
        c_wma_d_buf = np.zeros(WMA_D_WINDOW_SIZE, np.int32)
        c_freq_i = 0
        c_wma_d_i = 0

        c_last_wma = np.uint32(0)
        c_last_wma_d_avg = np.int32(0)

        c_tap_accum = np.int32(0)

        c_wmas = np.zeros(len(s_rf), np.uint32)
        c_wma_ds = np.zeros(len(s_rf), np.int32)
        c_wma_d_avgs = np.zeros(len(s_rf), np.int32)
        c_tap_accums = np.zeros(len(s_rf), np.int32)

        # pre-fill so that the values don't blow up
        for i in range(FREQ_WINDOW_SIZE):
            c_freq_buffer[i] = s_rf[0]

        for sample_i, rawf in enumerate(np.array(s_rf)):
            c_freq_buffer[c_freq_i] = rawf
            c_freq_i = (c_freq_i + 1) % FREQ_WINDOW_SIZE

            c_wma_numerator = np.uint64(0)
            for i in range(FREQ_WINDOW_SIZE):
                j = (c_freq_i + i) % FREQ_WINDOW_SIZE
                weight = np.uint64(i + 1)
                val = c_freq_buffer[j]
                c_wma_numerator += val * weight

            c_wma = np.uint32(c_wma_numerator // s_freq_weight_sum)
            c_wma_d = np.int32(c_wma) - np.int32(c_last_wma)

            c_wma_d_buf[c_wma_d_i] = c_wma_d
            c_wma_d_i = (c_wma_d_i + 1) % WMA_D_WINDOW_SIZE
            c_wma_d_avg = np.int32(0)
            for i in range(WMA_D_WINDOW_SIZE):
                c_wma_d_avg += c_wma_d_buf[i]
            c_wma_d_avg = c_wma_d_avg // WMA_D_WINDOW_SIZE

            if c_wma_d_avg < c_last_wma_d_avg:
                c_tap_accum += c_last_wma_d_avg - c_wma_d_avg
            else:
                c_tap_accum = 0

            c_last_wma = c_wma
            c_last_wma_d_avg = c_wma_d_avg

            c_wmas[sample_i] = c_wma
            # the first few blow up because they
            # start at 0 and go up to the large int freqval
            if sample_i < 32:
                c_tap_accum = 0
                c_wma_d_avg = 0
                c_wma_d = 0
            c_wma_ds[sample_i] = c_wma_d
            c_wma_d_avgs[sample_i] = c_wma_d_avg
            c_tap_accums[sample_i] = c_tap_accum

        butter_accum = None
        if butter_s_v is not None:
            # Note: we don't handle freq offset or
            # start this at same point as the C code does
            butter_accum = np.zeros(len(butter_s_v))
            last_value = butter_s_v[0]
            falling = False
            accum_val = 0.0
            for bi, bv in enumerate(butter_s_v):
                if bv <= last_value:
                    falling = True
                    accum_val += last_value - bv
                elif falling and bv > last_value:
                    falling = False
                    accum_val = 0.0
                butter_accum[bi] = accum_val
                last_value = bv

        import plotly.graph_objects as go

        fig = go.Figure()

        # toolhead and sensor Z
        fig.add_trace(go.Scatter(x=s_t, y=s_z, mode="lines", name="Z"))
        fig.add_trace(go.Scatter(x=s_t, y=s_kinz, mode="lines", name="KinZ"))

        # the butter tap if we have the data
        if butter_s_t is not None:
            fig.add_trace(
                go.Scatter(
                    x=butter_s_t,
                    y=butter_s_v,
                    mode="lines",
                    name="butter",
                    yaxis="y4",
                )
            )
            fig.add_trace(
                go.Scatter(
                    x=butter_s_t,
                    y=butter_accum,
                    mode="lines",
                    line=dict(color="#626b73"),
                    name="butter_threshold",
                    yaxis="y3",
                )
            )

        # the frequency value from the sensor, and their WMA
        fig.add_trace(
            go.Scatter(
                x=s_t,
                y=s_true_f,
                mode="lines",
                name="Freq",
                yaxis="y2",
                visible="legendonly",
            )
        )
        fig.add_trace(
            go.Scatter(
                x=s_t,
                y=c_wmas,
                mode="lines",
                name="wma",
                yaxis="y2",
                visible="legendonly",
            )
        )

        # the derivative and deriv averages
        fig.add_trace(
            go.Scatter(
                x=s_t,
                y=c_wma_ds,
                mode="lines",
                name="wma_d",
                yaxis="y3",
                visible="legendonly",
            )
        )
        fig.add_trace(
            go.Scatter(
                x=s_t,
                y=c_wma_d_avgs,
                mode="lines",
                name="wma_d_avg",
                yaxis="y3",
                visible="legendonly",
            )
        )
        fig.add_trace(
            go.Scatter(
                x=s_t,
                y=c_tap_accums,
                mode="lines",
                name="wma_threshold",
                yaxis="y3",
                visible="legendonly",
            )
        )

        if trigger_time > 0:
            fig.add_shape(
                type="line",
                x0=trigger_time,
                x1=trigger_time,
                y0=0,
                y1=1,
                xref="x",
                yref="paper",
                line=dict(color="orange", width=1),
            )
        if tap_end_time > 0:
            fig.add_shape(
                type="line",
                x0=tap_end_time,
                x1=tap_end_time,
                y0=0,
                y1=1,
                xref="x",
                yref="paper",
                line=dict(color="green", width=1),
            )
        if tap_threshold > 0:
            fig.add_shape(
                type="line",
                x0=0,
                x1=1,
                y0=tap_threshold,
                y1=tap_threshold,
                xref="paper",
                yref="y3",
                line=dict(color="gray", width=1, dash="dash"),
            )

        fig.update_layout(
            hovermode="x unified",
            yaxis=dict(title="Z", side="right"),  # Z axis
            yaxis2=dict(
                overlaying="y", title="Freq", tickformat="d", side="left"
            ),  # Freq + WMA
            yaxis3=dict(
                overlaying="y", side="left", tickformat="d", position=0.2
            ),  # derivatives, tap accum
            yaxis4=dict(
                overlaying="y", side="right", showticklabels=False
            ),  # alt
            height=800,
        )
        fig.write_html("/tmp/tap.html")
        logging.info("Wrote tap plot")


#
# Probe interface that does only scanning, no up/down movement.
# It assumes the probe is at an appropriate scan height,
# which is the same as the home trigger height.
#
@final
class ProbeEddyScanningProbe:
    def __init__(self, eddy: ProbeEddy, gcmd: GCodeCommand):
        self.eddy = eddy
        self._printer = eddy._printer
        self._toolhead = self._printer.lookup_object("toolhead")
        self._toolhead_kin = self._toolhead.get_kinematics()

        # we're going to scan at this height; pull_probed_results
        # also expects to return values based on this height
        self._scan_z = eddy.params.home_trigger_height

        # sensor thinks is _home_trigger_height vs. what it actually is.
        # For example, if we do a tap, adjust, and then we move the toolhead up
        # to 2.0 but the sensor says 1.950, then this would be +0.050.
        self._tap_offset = eddy._tap_offset

        # how much to dwell at each sample position in addition to sample_time
        self._sample_time_delay = self.eddy.params.scan_sample_time_delay
        self._sample_time: float = gcmd.get_float(
            "SAMPLE_TIME", self.eddy.params.scan_sample_time, above=0.0
        )
        self._is_rapid = gcmd.get("METHOD", "automatic").lower() == "rapid_scan"

        self._sampler: ProbeEddySampler = None

        self._notes = []

    def get_probe_params(self, gcmd):
        # this seems to be all that external users of get_probe_params
        # use (bed_mesh, axis_twist_compensation)
        return {
            "lift_speed": self.eddy.params.lift_speed,
            "probe_speed": self.eddy.params.probe_speed,
        }

    def _start_session(self):
        if not self.eddy._z_homed():
            raise self._printer.command_error(
                "Z axis must be homed before probing"
            )

        self.eddy.probe_to_start_position()
        self._sampler = self.eddy.start_sampler()

    def end_probe_session(self):
        self._sampler.finish()
        self._sampler = None

    def _lookup_toolhead_pos(self, time):
        pos, _ = get_toolhead_kin_pos(self._printer, time)
        return pos

        # kin_spos = {s.get_name(): s.mcu_to_commanded_position(
        #                              s.get_past_mcu_position(time))
        #            for s in self._toolhead_kin.get_steppers()}
        # return self._toolhead_kin.calc_position(kin_spos)

    def _rapid_lookahead_cb(self, time):
        start_time = time - self._sample_time / 2
        self._notes.append([start_time, time, None])

    def run_probe(self, gcmd):
        if self._is_rapid:
            # this callback is attached to the last move in the queue, so that
            # we can grab the toolhead position when the toolhead actually hits it
            self._toolhead.register_lookahead_callback(self._rapid_lookahead_cb)
            return

        th = self._toolhead

        # not sure I need this wait_moves, need to understand what position
        # get_position returns
        th.dwell(self._sample_time_delay)
        th.wait_moves()

        th_pos = th.get_position()
        if not math.isclose(th_pos[2], self._scan_z, rel_tol=1e-3):
            logging.info(
                f"ProbeEddyScanningProbe: toolhead at {th_pos[2]:.3f}!"
            )  # , fixing")
            # th.manual_move([None, None, self._scan_z + 2.0], self.eddy.params.lift_speed)
            # th.manual_move([None, None, self._scan_z], self.eddy.params.probe_speed)
            # th.wait_moves()

        start_time = th.get_last_move_time()
        self._toolhead.dwell(self._sample_time + self._sample_time_delay)

        th_pos = self._lookup_toolhead_pos(start_time)
        self._notes.append((start_time, start_time, th_pos))

    def pull_probed_results(self):
        if self._is_rapid:
            # Flush lookahead (so all lookahead callbacks are invoked)
            self._toolhead.get_last_move_time()

        # make sure we get the sample for the final move
        self._sampler.wait_for_sample_at_time(
            self._notes[-1][0] + self._sample_time
        )

        # note: we can't call finish() here! this session can continue to be used
        # to probe additional points and pull them, because that's what QGL does.

        results = []

        logging.info(
            f"ProbeEddyScanningProbe: pulling {len(self._notes)} results"
        )
        for start_time, sample_time, th_pos in self._notes:
            if th_pos is None:
                # this is giving a totally wrong position. I don't know what's up with that.
                th_pos = self._lookup_toolhead_pos(sample_time)
                # logging.info(f"th @ {sample_time:.3f}: {th_pos[0]:.2f} {th_pos[1]:.2f} {th_pos[2]:.2f}")

            end_time = start_time + self._sample_time
            height = self._sampler.find_height_at_time(start_time, end_time)

            if not math.isclose(th_pos[2], self._scan_z, rel_tol=1e-3):
                logging.info(
                    f"ProbeEddyScanningProbe warning: toolhead not at home_trigger_height ({self._scan_z:.3f}) during probes (saw {th_pos[2]:.3f})"
                )

            # adjust the sensor height value based on the fine-tuned tap offset amount
            height += self._tap_offset

            # the delta between where the toolhead thinks it should be (since it
            # should be homed), and the actual physical offset (height)
            z_deviation = th_pos[2] - height

            # what callers want to know is "what Z would the toolhead be at, if it was at the height
            # the probe would 'trigger'", because this is all done in terms of klicky-type probes
            th_pos[2] = float(self._scan_z + z_deviation)
            # toolhead_pos[2] = height

            results.append([th_pos[0], th_pos[1], th_pos[2]])

        # reset notes so that this session can continue to be used
        self._notes = []

        # Allow axis_twist_compensation to update results
        for epos in results:
            self._printer.send_event("probe:update_results", epos)

        return results


# This is a ProbeEndstopWrapper-compatible class,
# which also forwards the "mcu_probe" methods.
@final
class ProbeEddyEndstopWrapper:
    REASON_BASE = mcu.MCU_trsync.REASON_COMMS_TIMEOUT + 1
    REASON_ERROR_SENSOR = REASON_BASE + 0
    REASON_ERROR_PROBE_TOO_LOW = REASON_BASE + 1
    REASON_ERROR_TOO_EARLY = REASON_BASE + 2

    def __init__(self, eddy: ProbeEddy):
        self.eddy = eddy
        self._sensor = eddy._sensor
        self._printer = eddy._printer
        self._mcu = eddy._mcu
        self._reactor = eddy._printer.get_reactor()

        # these two are filled in by the outside.
        self.tap_config: Optional[ProbeEddy.TapConfig] = None
        # if not None, after a probe session is finished we'll
        # write all samples here
        self.save_samples_path: Optional[str] = None

        self._multi_probe_in_progress = False

        self._dispatch = mcu.TriggerDispatch(self._mcu)

        # the times of the last successful endstop home_wait
        self.last_trigger_time = 0.0
        self.last_tap_end_time = 0.0

        self._homing_in_progress = False
        self._sampler: ProbeEddySampler = None

        # Register z_virtual_endstop pin
        self._printer.lookup_object("pins").register_chip("probe", self)
        # Register event handlers
        self._printer.register_event_handler(
            "klippy:mcu_identify", self._handle_mcu_identify
        )
        self._printer.register_event_handler(
            "homing:homing_move_begin", self._handle_homing_move_begin
        )
        self._printer.register_event_handler(
            "homing:homing_move_end", self._handle_homing_move_end
        )
        self._printer.register_event_handler(
            "homing:home_rails_begin", self._handle_home_rails_begin
        )
        self._printer.register_event_handler(
            "homing:home_rails_end", self._handle_home_rails_end
        )
        self._printer.register_event_handler(
            "gcode:command_error", self._handle_command_error
        )

        # copy some things in for convenience
        self._home_trigger_height = self.eddy.params.home_trigger_height
        self._home_trigger_safe_start_offset = (
            self.eddy.params.home_trigger_safe_start_offset
        )
        self._home_start_height = (
            self.eddy._home_start_height
        )  # this is trigger + safe_start + 1.0
        self._probe_speed = self.eddy.params.probe_speed
        self._lift_speed = self.eddy.params.lift_speed

    def _handle_mcu_identify(self):
        kin = self._printer.lookup_object("toolhead").get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis("z"):
                self.add_stepper(stepper)

    def _handle_home_rails_begin(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        if self not in endstops:
            return
        # Nothing to do
        pass

    def _handle_homing_move_begin(self, hmove):
        if self not in hmove.get_mcu_endstops():
            return
        self._sampler = self.eddy.start_sampler()
        self._homing_in_progress = True
        # if we're doing a tap, we're already in the right position;
        # otherwise move there
        if self.tap_config is None:
            self.eddy._probe_to_start_position_unhomed(move_home=True)

    def _handle_homing_move_end(self, hmove):
        if self not in hmove.get_mcu_endstops():
            return
        self._sampler.finish()
        self._homing_in_progress = False

    def _handle_home_rails_end(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        if self not in endstops:
            return
        # Nothing to do
        pass

    def _handle_command_error(self, gcmd=None):
        if self._homing_in_progress:
            self._homing_in_progress = False
        try:
            if self._sampler is not None:
                self._sampler.finish()
        except:
            logging.exception(
                "EDDYng handle_command_error: sampler.finish() failed"
            )

    def setup_pin(self, pin_type, pin_params):
        if pin_type != "endstop" or pin_params["pin"] != "z_virtual_endstop":
            raise pins.error("Probe virtual endstop only useful as endstop pin")
        if pin_params["invert"] or pin_params["pullup"]:
            raise pins.error("Can not pullup/invert probe virtual endstop")
        return self

    # these are the "MCU Probe" methods
    def get_mcu(self):
        return self._mcu

    def add_stepper(self, stepper: MCU_stepper):
        self._dispatch.add_stepper(stepper)

    def get_steppers(self):
        return self._dispatch.get_steppers()

    def get_position_endstop(self):
        if self.tap_config is None:
            return self._home_trigger_height
        else:
            return 0.0

    def home_start(
        self, print_time, sample_time, sample_count, rest_time, triggered=True
    ):
        if not self._sampler.active():
            raise self._printer.command_error(
                "home_start called without a sampler active"
            )

        self.last_trigger_time = 0.0
        self.last_tap_end_time = 0.0

        trigger_height = self._home_trigger_height
        safe_height = trigger_height + self._home_trigger_safe_start_offset

        if self.tap_config is None:
            safe_time = (
                print_time + self.eddy.params.home_trigger_safe_time_offset
            )
            trigger_freq = self.eddy.height_to_freq(trigger_height)
            safe_freq = self.eddy.height_to_freq(safe_height)
        else:
            # TODO: the home trigger safe time won't work, because we'll pass
            # the home_trigger_height maybe by default given where tap might
            # start
            safe_time = 0
            # initial freq to pass through
            safe_freq = self.eddy.height_to_freq(self._home_trigger_height)
            # second freq to pass through; toolhead acceleration
            # must be smooth after this point
            trigger_freq = self.eddy.height_to_freq(
                self.eddy.params.tap_trigger_safe_start_height
            )

        trigger_completion = self._dispatch.start(print_time)

        if self.tap_config is not None:
            if self.tap_config.mode == "butter":
                sos = self.tap_config.sos
                for i in range(len(sos)):
                    logging.info(f"EDDYng sos[{i}]: {sos[i]}")
                    self.eddy._sensor.set_sos_section(i, sos[i])
                mode = "sos"
            elif self.tap_config.mode == "wma":
                mode = "wma"
            else:
                raise self._printer.command_error(
                    f"Invalid tap mode: {self.tap_config.mode}"
                )
            tap_threshold = self.tap_config.threshold
        else:
            mode = "home"
            tap_threshold = None

        logging.info(
            f"EDDYng home_start {mode}: {print_time:.3f} freq: {trigger_freq:.2f} safe-start: {safe_freq:.2f} @ {safe_time:.3f}"
        )
        # setup homing -- will start scanning and trigger when we hit
        # trigger_freq
        self._sensor.setup_home(
            self._dispatch.get_oid(),
            mcu.MCU_trsync.REASON_ENDSTOP_HIT,
            self.REASON_BASE,
            trigger_freq,
            safe_freq,
            safe_time,
            mode=mode,
            tap_threshold=tap_threshold,
        )

        return trigger_completion

    def home_wait(self, home_end_time):
        logging.info(f"EDDYng home_wait until {home_end_time:.3f}")
        # logging.info(f"EDDYng home_wait {home_end_time} cur {curtime} ept {est_print_time} ehe {est_he_time}")
        self._dispatch.wait_end(home_end_time)

        # make sure homing is stopped, and grab the trigger_time from the mcu
        home_result = self._sensor.finish_home()
        trigger_time = home_result.trigger_time
        tap_end_time = home_result.tap_end_time

        self._sampler.memo("trigger_time", trigger_time)
        if self.tap_config is not None:
            self._sampler.memo("tap_end_time", tap_end_time)
            self._sampler.memo("tap_threshold", self.tap_config.threshold)

        logging.info(
            f"EDDYng trigger_time {trigger_time} (mcu: {self._mcu.print_time_to_clock(trigger_time)}) tap_end_time: {tap_end_time}"
        )

        # nb: _dispatch.stop() will treat anything >= REASON_COMMS_TIMEOUT as an error,
        # and will only return those results. Fine for us since we only have one trsync,
        # but annoying in general.
        res = self._dispatch.stop()

        # clean these up, and only update them if successful
        self.last_trigger_time = 0.0
        self.last_tap_end_time = 0.0

        # always reset this; taps are one-shot usages of the endstop wrapper
        self.tap_config = None

        # if we're doing at ap, we wait for samples for the end as well so that we can get
        # beter data for analysis
        self._sampler.wait_for_sample_at_time(max(trigger_time, tap_end_time))

        # success?
        if res == mcu.MCU_trsync.REASON_ENDSTOP_HIT:
            self.last_trigger_time = trigger_time
            self.last_tap_end_time = tap_end_time
            return trigger_time

        # various errors
        if res == mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
            raise self._printer.command_error(
                "Communication timeout during homing"
            )
        if res == self.REASON_ERROR_SENSOR:
            status = self._sensor.latched_status_str()
            raise self._printer.command_error(
                "Sensor error" + (f" (ldc status: {status})" if status else "")
            )
        if res == self.REASON_ERROR_PROBE_TOO_LOW:
            raise self._printer.command_error(
                "Probe too low at start of homing, did not clear safe height."
            )
        if res == self.REASON_ERROR_TOO_EARLY:
            raise self._printer.command_error(
                "Probe did not clear safe height after safe time."
            )
        if res == mcu.MCU_trsync.REASON_PAST_END_TIME:
            raise self._printer.command_error(
                "Probe completed movement before triggering. If this is a tap, try lowering TARGET_Z or adjusting the THRESHOLD."
            )

        raise self._printer.command_error(f"Unknown homing error: {res}")

    def query_endstop(self, print_time):
        return False

    def _setup_sampler(self):
        self._sampler = self.eddy.start_sampler()

    def _finish_sampler(self):
        self._sampler.finish()
        self._sampler = None


# Helper to gather samples and convert them to probe positions
@final
class ProbeEddySampler:
    def __init__(
        self,
        eddy: ProbeEddy,
        calculate_heights: bool = True,
        trace: bool = False,
    ):
        self.eddy = eddy
        self._sensor = eddy._sensor
        self._printer = self.eddy._printer
        self._reactor = self._printer.get_reactor()
        self._mcu = self._sensor.get_mcu()
        self._stopped = False
        self._started = False
        self._errors = 0
        self._trace = trace
        self._fmap = eddy.map_for_drive_current() if calculate_heights else None

        # this will hold the raw samples coming in from the sensor,
        # with an empty 3rd (height) value
        self._raw_samples = []
        # this will hold samples with a height filled in (if we're doing that)
        self._samples = []

        self.memos = dict()

    # this is just a handy way to communicate values between different parts of the system,
    # specifically to record things like trigger times for plotting
    def memo(self, name, value):
        self.memos[name] = value

    def __enter__(self):
        self.start()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.finish()

    def active(self):
        return self._started and not self._stopped

    # bulk sample callback for when new data arrives
    # from the probe
    def _add_hw_measurement(self, msg):
        if self._stopped:
            return False

        self._errors += msg["errors"]
        self._raw_samples.extend(msg["data"])
        return True

    def start(self):
        if self._stopped:
            raise self._printer.command_error(
                "ProbeEddySampler.start() called after finish()"
            )
        if not self._started:
            self._raw_samples = []
            self._sensor.add_bulk_sensor_data_client(self._add_hw_measurement)
            self._started = True

    def finish(self):
        if self._stopped:
            return
        if not self._started:
            raise self._printer.command_error(
                "ProbeEddySampler.finish() called without start()"
            )
        if self.eddy._sampler is not self:
            raise self._printer.command_error(
                "ProbeEddySampler.finish(): eddy._sampler is not us!"
            )
        self.eddy._sampler_finished(self)
        self._stopped = True

    def _update_samples(self):
        if len(self._samples) == len(self._raw_samples):
            return

        start_idx = len(self._samples)
        conv_ratio = self._sensor.freqval_conversion_value()
        if self._fmap is not None:
            new_samples = [
                (
                    t,
                    round(conv_ratio * f, ndigits=3),
                    self._fmap.freq_to_height(round(conv_ratio * f, ndigits=3)),
                )
                for t, f, _ in self._raw_samples[start_idx:]
            ]
            self._samples.extend(new_samples)
        else:
            self._samples.extend(
                [
                    (t, round(conv_ratio * f, ndigits=3), math.inf)
                    for t, f, _ in self._raw_samples[start_idx:]
                ]
            )

    def get_raw_samples(self):
        return self._raw_samples.copy()

    def get_samples(self):
        self._update_samples()
        return self._samples.copy()

    def get_error_count(self):
        return self._errors

    # get the last sampled height
    def get_last_height(self) -> float:
        if self._fmap is None:
            raise self._printer.command_error(
                "ProbeEddySampler: no height mapping"
            )
        self._update_samples()
        if len(self._samples) == 0:
            raise self._printer.command_error("ProbeEddySampler: no samples")
        return self._samples[-1][2]

    # wait for a sample for the current time and get a new height
    def get_height_now(self, average=False) -> Optional[float]:
        start = end = self._mcu.estimated_print_time(self._reactor.monotonic())
        if average:
            end = start + 0.100
        if not self.wait_for_sample_at_time(
            end, max_wait_time=0.250, raise_error=False
        ):
            return None
        if average:
            return self.find_height_at_time(start, end)
        return self.get_last_height()

    # Wait until a sample for the given time arrives
    def wait_for_sample_at_time(
        self, sample_print_time, max_wait_time=0.250, raise_error=True
    ) -> bool:
        def report_no_samples():
            if raise_error:
                raise self._printer.command_error(
                    f"No samples received for time {sample_print_time:.3f} (waited for {max_wait_time:.3f})"
                )
            return False

        if self._stopped:
            # if we're not getting any more samples, we can check directly
            if len(self._raw_samples) == 0:
                return report_no_samples()
            return self._raw_samples[-1][0] >= sample_print_time

        wait_start_time = self._mcu.estimated_print_time(
            self._reactor.monotonic()
        )

        # if sample_print_time is in the future, make sure to wait max_wait_time
        # past the expected time
        if sample_print_time > wait_start_time:
            max_wait_time = max_wait_time + (
                sample_print_time - wait_start_time
            )

        # this is just a sanity check, there's no reason to ever wait this long
        if max_wait_time > 5.0:
            traceback.print_stack()
            msg = f"ProbeEddyFrequencySampler: max_wait_time {max_wait_time:.3f} is too far into the future"
            logging.info(msg)
            raise self._printer.command_error(msg)

        if self._trace:
            logging.info(
                f"EDDYng waiting for sample at {sample_print_time:.3f} (now: {wait_start_time:.3f}, max_wait_time: {max_wait_time:.3f})"
            )
        while (
            len(self._raw_samples) == 0
            or self._raw_samples[-1][0] < sample_print_time
        ):
            now = self._mcu.estimated_print_time(self._reactor.monotonic())
            if now - wait_start_time > max_wait_time:
                return report_no_samples()
            self._reactor.pause(self._reactor.monotonic() + 0.010)

        return True

    # Wait for some samples to be collected, even if errors
    # TODO: there's a minimum wait time -- we need to fill up the buffer before data is sent, and that
    # depends on the data rate
    def wait_for_samples(
        self,
        max_wait_time=0.250,
        count_errors=False,
        min_samples=1,
        new_only=False,
        raise_error=True,
    ):
        # Make sure enough samples have been collected
        wait_start_time = self._mcu.estimated_print_time(
            self._reactor.monotonic()
        )

        start_error_count = self._errors
        if new_only:
            start_count = len(self._raw_samples) + (
                self._errors if count_errors else 0
            )
        else:
            start_count = 0

        while (
            len(self._raw_samples) + (self._errors if count_errors else 0)
        ) - start_count < min_samples:
            now = self._mcu.estimated_print_time(self._reactor.monotonic())
            if now - wait_start_time > max_wait_time:
                if raise_error:
                    raise self._printer.command_error(
                        f"probe_eddy_ng sensor outage: no samples for {max_wait_time:.2f}s (got {self._errors - start_error_count} errors)"
                    )
                return False
            self._reactor.pause(self._reactor.monotonic() + 0.010)

        return True

    def find_closest_height_at_time(self, time) -> Optional[float]:
        self._update_samples()
        if len(self._samples) == 0:
            return None
        # find the closest sample to the given time
        idx = np.argmin([abs(t - time) for t, _, _ in self._samples])
        return self._samples[idx][2]

    def find_height_at_time(self, start_time, end_time):
        if end_time < start_time:
            raise self._printer.command_error(
                "find_height_at_time: end_time is before start_time"
            )

        self._update_samples()

        if len(self._samples) == 0:
            raise self._printer.command_error(
                "No samples at all, so none in time range"
            )

        if self._trace:
            logging.info(
                f"EDDYng find_height_at_time: {len(self._samples)} samples, time range {self._samples[0][0]:.3f} to {self._samples[-1][0]:.3f}"
            )

        # find the first sample that is >= start_time
        start_idx = bisect.bisect_left(
            [t for t, _, _ in self._samples], start_time
        )
        if start_idx >= len(self._samples):
            raise self._printer.command_error("Nothing after start_time?")

        # find the last sample that is <= end_time
        end_idx = bisect.bisect_right(
            [t for t, _, _ in self._samples], end_time
        )
        if end_idx == 0:
            raise self._printer.command_error(
                "found something at start_time, but not before end_time?"
            )

        # average the heights of the samples in the range
        heights = [h for _, _, h in self._samples[start_idx:end_idx]]
        if len(heights) == 0:
            raise self._printer.command_error(
                f"no samples between time {start_time:.1f} and {end_time:.1f}!"
            )
        hmin, hmax = np.min(heights), np.max(heights)
        mean = np.mean(heights)
        median = np.median(heights)
        if self._trace:
            logging.info(
                f"EDDYng find_height_at_time: {len(heights)} samples, median: {median:.3f}, mean: {mean:.3f} (range {hmin:.3f}-{hmax:.3f})"
            )

        return float(median)


@final
class ProbeEddyFrequencyMap:
    calibration_version = 3

    def __init__(self, eddy: ProbeEddy):
        self._eddy = eddy
        self._sensor = eddy._sensor

        self.drive_current = 0
        self._freqs = None
        self._heights = None
        self.height_range = (-math.inf, math.inf)
        self.freq_range = (-math.inf, math.inf)

    def _str_to_exact_floatlist(self, str):
        return [float.fromhex(v) for v in str.split(",")]

    def _exact_floatlist_to_str(self, vals):
        return str.join(", ", [float.hex(v) for v in vals])

    def _coefs_to_str(self, coefs):
        return ", ".join([format(c, ".3f") for c in coefs])

    def _interp_extrapolate(self, v, a, b):
        if a[0] > a[-1]:
            raise ValueError("a must be increasing")
        if v < a[0]:
            left_slope = (b[1] - b[0]) / (a[1] - a[0])
            return b[0] + left_slope * (v - a[0])
        if v > a[-1]:
            right_slope = (b[-1] - b[-2]) / (a[-1] - a[-2])
            return b[-1] + right_slope * (v - a[-1])
        return np.interp(v, a, b)

    def _freq_to_height(self, freq: float):
        return float(self._interp_extrapolate(freq, self._freqs, self._heights))

    def _height_to_freq(self, height: float):
        return float(
            self._interp_extrapolate(
                height, self._heights[::-1], self._freqs[::-1]
            )
        )

    def freq_spread(self) -> float:
        return ((self.freq_range[1] / self.freq_range[0]) - 1.0) * 100.0

    def load_from_config(self, config: ConfigWrapper, drive_current: int):
        calibstr = config.get(f"calibration_{drive_current}", None)
        if calibstr is None:
            self.drive_current = 0
            self._freqs = None
            self._heights = None
            self.height_range = (-math.inf, math.inf)
            self.freq_range = (-math.inf, math.inf)
            return

        data = pickle.loads(base64.b64decode(calibstr))
        freqs = np.asarray(data["f"])
        heights = np.asarray(data["h"])
        dc = data["dc"]
        h_range = data.get("h_range", (-math.inf, math.inf))
        f_range = data.get("f_range", (-math.inf, math.inf))

        if dc != drive_current:
            raise configerror(
                f"ProbeEddyFrequencyMap: drive current mismatch: loaded {dc} != requested {drive_current}"
            )

        self._freqs = freqs
        self._heights = heights
        self.height_range = h_range
        self.freq_range = f_range
        self.drive_current = drive_current

        logging.info(
            f"EDDYng Loaded calibration for drive current {drive_current} ({len(self._freqs)} points)"
        )

    def save_calibration(self):
        if self._freqs is None or self._heights is None:
            return

        configfile = self._eddy._printer.lookup_object("configfile")
        data = {
            "f": self._freqs.tolist(),
            "h": self._heights.tolist(),
            "h_range": self.height_range,
            "f_range": self.freq_range,
            "dc": self.drive_current,
        }
        calibstr = base64.b64encode(pickle.dumps(data)).decode()
        configfile.set(
            self._eddy._full_name, f"calibration_{self.drive_current}", calibstr
        )

    def calibrate_from_values(
        self,
        drive_current: int,
        times: List[float],
        raw_freqs_list: List[float],
        raw_heights_list: List[float],
        report_errors: bool,
        write_debug_files: bool,
    ):
        if len(raw_freqs_list) != len(raw_heights_list):
            raise ValueError("freqs and heights must be the same length")

        if len(raw_freqs_list) == 0:
            return None, None

        # everything must be a np.array or things get confused below
        raw_freqs = np.asarray(raw_freqs_list)
        raw_heights = np.asarray(raw_heights_list)

        # smooth out the data; a simple rolling average does the job,
        # but centered to give more accurate data (since we have the full
        # data set). The edges of the data just use a smaller window.
        avg_freqs = np_rolling_mean(raw_freqs, 16)
        avg_heights = np_rolling_mean(raw_heights, 16)

        if write_debug_files:
            with open("/tmp/eddy-calibration.csv", "w") as data_file:
                data_file.write("time,frequency,avg_freq,z,avg_z\n")
                for i in range(len(raw_freqs)):
                    s_t = times[i]
                    s_f = raw_freqs[i]
                    s_z = raw_heights[i]
                    s_af = avg_freqs[i]
                    s_az = avg_heights[i]
                    data_file.write(f"{s_t},{s_f},{s_af},{s_z},{s_az}\n")
                self._eddy._log_trace(
                    f"Wrote {len(raw_freqs)} samples to /tmp/eddy-calibration.csv"
                )

        if len(avg_freqs) == 0 or len(avg_heights) == 0:
            if report_errors:
                self._eddy._log_error(
                    f"Error: Calibration failed, couldn't compute averages ({len(raw_freqs_list)}, {len(raw_heights_list)}, {len(avg_freqs)}, {len(avg_heights)}), probably due to no samples received."
                )
            return None, None

        max_height = float(avg_heights.max())
        min_height = float(avg_heights.min())
        min_freq = float(avg_freqs.min())
        max_freq = float(avg_freqs.max())
        freq_spread = ((max_freq / min_freq) - 1.0) * 100.0

        # Check if our calibration is good enough
        if report_errors:
            if max_height < 2.5:  # we really can't do anything with this
                self._eddy._log_error(
                    f"Error: max height for valid samples is too low: {max_height:.3f} < 2.5. Possible causes: bad drive current, bad sensor mount height."
                )
                if not self._eddy.params.allow_unsafe:
                    return None, None

            if (
                min_height > 0.65
            ):  # this is a bit arbitrary; but if it's this far off we shouldn't trust it
                self._eddy._log_error(
                    f"Error: min height for valid samples is too high: {min_height:.3f} > 0.65. Possible causes: bad drive current, bad sensor mount height."
                )
                if not self._eddy.params.allow_unsafe:
                    return None, None

            if min_height > 0.025:
                self._eddy._log_info(
                    f"Warning: min height is {min_height:.3f} (> 0.025) is too high for tap. This calibration will work fine for homing, but may not for tap."
                )

            # somewhat arbitrary spread
            if freq_spread < 0.30:
                extremely = "EXTREMELY " if freq_spread < 0.15 else ""
                self._eddy._log_warning(
                    f"Warning: frequency spread is {extremely}low ({freq_spread:.2f}%, {min_freq:.1f}-{max_freq:.1f}), which will greatly impact accuracy. Your sensor may be too high."
                )

        # Calculate RMSE

        qf = np.linspace(
            avg_freqs.min(),
            avg_freqs.max(),
            self._eddy.params.calibration_points,
        )
        qz = np.interp(qf, avg_freqs, avg_heights)
        # calculate rms using later values because we don't care much about the precision of the
        # early (high z) values
        rmse_start_index = len(avg_freqs) // 3 if len(avg_freqs) > 50 else 0
        rmse_fth = np_rmse(
            lambda v: np.interp(v, qf, qz),
            avg_freqs[rmse_start_index:],
            avg_heights[rmse_start_index:],
        )
        rmse_htf = np_rmse(
            lambda v: np.interp(v, qz[::-1], qf[::-1]),
            avg_heights[rmse_start_index:],
            avg_freqs[rmse_start_index:],
        )

        if report_errors:
            if rmse_fth > 0.050:
                self._eddy._log_error(
                    f"Error: calibration error margin is too high ({rmse_fth:.3f}). Possible causes: bad drive current, bad sensor mount height."
                )
                if not self._eddy.params.allow_unsafe:
                    return None, None

        self._freqs = qf
        self._heights = qz
        self.height_range = (min_height, max_height)
        self.freq_range = (min_freq, max_freq)
        self.drive_current = drive_current

        self._eddy._log_info(
            f"Drive current {drive_current}: valid height: {min_height:.3f} to {max_height:.3f}, "
            f"freq spread {freq_spread:.2f}% ({min_freq:.1f} - {max_freq:.1f}), "
            f"Fit {rmse_fth:.4f} ({rmse_htf:.2f})"
        )

        if write_debug_files:
            self._save_calibration_plot(
                avg_freqs, avg_heights, qf, qz, rmse_fth, rmse_htf
            )

        return rmse_fth, rmse_htf

    def _save_calibration_plot(
        self, freqs, heights, qf, qz, rmse_fth, rmse_htf
    ):
        if not HAS_PLOTLY:
            return

        f_to_z_err = heights - np.interp(freqs, qf, qz)

        import plotly.graph_objects as go

        fig = go.Figure()
        fig.add_trace(go.Scatter(y=heights, mode="lines", name="Z"))
        fig.add_trace(
            go.Scatter(
                y=np.interp(freqs, qf, qz),
                mode="lines",
                name=f"Z {rmse_fth:.4f}",
            )
        )

        fig.add_trace(go.Scatter(y=freqs, mode="lines", name="F", yaxis="y2"))
        fig.add_trace(
            go.Scatter(
                y=np.interp(heights, qz[::-1], qf[::-1]),
                mode="lines",
                name=f"F ({rmse_htf:.2f})",
                yaxis="y2",
            )
        )

        fig.add_trace(
            go.Scatter(y=f_to_z_err, mode="lines", name="Err", yaxis="y3")
        )

        fig.update_layout(
            hovermode="x unified",
            title=f"Calibration for drive current {self.drive_current}",
            yaxis2=dict(title="Freq", overlaying="y", side="right"),
            yaxis3=dict(overlaying="y", side="right"),
        )
        fig.write_html("/tmp/eddy-calibration.html")

    def raw_freqval_to_height(self, raw_freq: int) -> float:
        if raw_freq > 0x0FFFFFFF:
            return -math.inf  # error bits set
        return self.freq_to_height(self._sensor.from_ldc_freqval(raw_freq))

    def height_to_raw_freqval(self, height: float) -> int:
        if self._height_to_freq is None:
            return 0
        return self._sensor.to_ldc_freqval(self._height_to_freq(height))

    def freq_to_height(self, freq: float) -> float:
        if self._freq_to_height is None:
            return math.inf
        return self._freq_to_height(freq)

    def height_to_freq(self, height: float) -> float:
        if self._height_to_freq is None:
            return math.inf
        return self._height_to_freq(height)

    def calibrated(self) -> bool:
        return (
            self._freq_to_height is not None
            and self._height_to_freq is not None
        )


def get_toolhead_kin_pos(printer, at=None):
    toolhead = printer.lookup_object("toolhead")
    toolhead_kin = toolhead.get_kinematics()
    toolhead.flush_step_generation()
    if at is None:
        kin_spos = {
            s.get_name(): s.get_commanded_position()
            for s in toolhead_kin.get_steppers()
        }
    else:
        kin_spos = {
            s.get_name(): s.mcu_to_commanded_position(
                s.get_past_mcu_position(at)
            )
            for s in toolhead_kin.get_steppers()
        }
    return toolhead_kin.calc_position(kin_spos), 0


def np_rolling_mean(data, window, center=True):
    half_window = (window - 1) // 2 if center else 0
    result = np.empty(len(data), dtype=float)

    for i in range(len(data)):
        start = max(0, i - half_window)
        end = min(len(data), i + half_window + 1)
        result[i] = np.mean(data[start:end])

    return result


def np_rmse(p, x, y):
    y_hat = p(x)
    return np.sqrt(np.mean((y - y_hat) ** 2))


def load_config_prefix(config: ConfigWrapper):
    return ProbeEddy(config)
