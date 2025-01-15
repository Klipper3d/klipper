from __future__ import annotations

import logging, math, bisect, re
import numpy as np
import numpy.polynomial as npp
import traceback
import codecs, pickle, base64

import mcu
import pins

from dataclasses import dataclass
from enum import IntEnum
from typing import Callable, Dict, List, Optional, Tuple, TypedDict, final, ClassVar

try:
    import plotly as plt
    import plotly.graph_objects as go
    HAS_PLOTLY=True
except:
    HAS_PLOTLY=False

from clocksync import SecondarySync
from configfile import ConfigWrapper
from configfile import error as configerror
from gcode import GCodeCommand
from klippy import Printer
from mcu import MCU, MCU_trsync
from stepper import MCU_stepper
from toolhead import ToolHead

from .homing import HomingMove

from . import ldc1612_ng, probe, manual_probe

from .ldc1612_ng import SETTLETIME as LDC1612_SETTLETIME

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
#    A `PROBE`, `SET_Z_FROM_PROBE` can be used to set the toolhead's Z position
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
    # Backlash compensation -- if we need to move the toolhead above the
    # current position, how much to add to the movement before coming back
    # down to the requested position
    backlash_comp: float = 0.0
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
    # The maximum z value to calibrate from. 5.0 is fine as a default,  calibrating
    # at higher values is not needed. You may need to lower  this value while
    # calibrating a separate tap drive current, but do that directly in the
    # CALIBRATE command by passing a Z_MAX parameter.
    calibration_z_max: float = 5.0
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
    # The Z position at which to start a tap-home operation. This height will
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
    tap_start_z: float = 3.2
    # The target Z position for a tap operation. This is the lowest position that
    # the toolhead may travel to in case of a failed tap. Do not set this very low,
    # as it will cause your toolhead to try to push through your build plate in
    # the case of a failed tap. A value like -0.250 is no worse than moving the
    # nozzle down one or two notches too far when doing manual Z adjustment.
    tap_target_z: float = -0.250
    # The threshold at which to detect a tap. This value is raw sensor value
    # specific. A good value can be obtained by running [....] and examining
    # the graph. See [calibration docs coming soon].
    # You can also experiment to arrive at this value. A lower value will
    # make tap detection more sensitive. A higher value will cause the
    # detection to wait too long before recognizing a tap. You can use a manual
    # THRESHOLD parameter to the TAP command to experiment to find a good value.
    #
    # You may also need to use different thresholds for different build plates.
    tap_threshold: int = 1000
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
    # When probing multiple points (not rapid scan), how long to sample for at each probe point,
    # after a scan_sample_time_delay delay. The total dwell time at each probe point is
    # scan_sample_time + scan_sample_time_delay.
    scan_sample_time: float = 0.100
    # When probing multiple points (not rapid scan), how long to delay at each probe point
    # before the scan_sample_time kicks in.
    scan_sample_time_delay: float = 0.050
    # At what XY toolhead position to do a probe. If not set, homing is done at the current
    # toolhead position. In both cases, the probe x/y_offset will be applied to move the probe over
    # the point where the toolhead was. Two numbers separated by a comma or a space.
    home_xy: Optional[Tuple[float, float]] = None
    # number of points to save for calibration
    calibration_points: int = 150

    x_offset: float = 0.0
    y_offset: float = 0.0

    @staticmethod
    def str_to_floatlist(s):
        if s is None:
            return None
        try:
            return [float(v) for v in re.split(r'\s*,\s*|\s+', s)]
        except:
            raise configerror(f"Can't parse '{s}' as list of floats")
    
    def load_from_config(self, config: ConfigWrapper):
        self.probe_speed = config.getfloat('probe_speed', self.probe_speed, above=0.0)
        self.lift_speed = config.getfloat('lift_speed', self.lift_speed, above=0.0)
        self.move_speed = config.getfloat('move_speed', self.move_speed, above=0.0)
        self.backlash_comp = config.getfloat('backlash_comp', self.backlash_comp)
        self.home_trigger_height = config.getfloat('home_trigger_height', self.home_trigger_height, above=0.0)
        self.home_trigger_safe_start_offset = config.getfloat('home_trigger_safe_start_offset', self.home_trigger_safe_start_offset, minval=0.5)
        self.calibration_z_max = config.getfloat('calibration_z_max', self.calibration_z_max, above=0.0)
        self.reg_drive_current = config.getint('reg_drive_current', self.reg_drive_current, minval=0, maxval=31)
        self.tap_drive_current = config.getint('tap_drive_current', self.reg_drive_current, minval=0, maxval=31) # note default same as reg_drive_current
        self.tap_start_z = config.getfloat('tap_start_z', self.tap_start_z, above=0.0)
        self.tap_target_z = config.getfloat('tap_target_z', self.tap_target_z)
        self.tap_threshold = config.getint('tap_threshold', self.tap_threshold, minval=0)
        self.tap_speed = config.getfloat('tap_speed', self.tap_speed, above=0.0)
        self.home_xy = self.str_to_floatlist(config.get('home_xy', None))
        self.tap_adjust_z = config.getfloat('tap_adjust_z', self.tap_adjust_z)

        self.x_offset = config.getfloat('x_offset', self.x_offset)
        self.y_offset = config.getfloat('y_offset', self.y_offset)

        self.validate(config)
    
    def validate(self, config: ConfigWrapper = None):
        req_cal_z_max = self.home_trigger_safe_start_offset + self.home_trigger_height + 1.0
        if self.calibration_z_max < req_cal_z_max:
            raise config.get_printer().config_error(f"calibration_z_max must be at least home_trigger_safe_start_offset+home_trigger_height+1.0 ({self.home_trigger_safe_start_offset:.3f}+{self.home_trigger_height:.3f}+1.0={req_cal_z_max:.3f})")
        if self.x_offset == 0.0 and self.y_offset == 0.0:
            raise config.get_printer().config_error(f"ProbeEddy: x_offset and y_offset are both 0.0; is the sensor really mounted at the nozzle?")

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
    def value(self):
        return self.mean if self.USE_MEAN_FOR_VALUE else self.median
    
    @property
    def stddev(self):
        stddev_sum = np.sum([(s-self.value)**2.0 for s in self.samples])
        return (stddev_sum / len(self.samples)) ** 0.5

    def __format__(self, spec):
        if spec == 'v':
            return f"{self.value:.3f}"
        mean_star = '*' if self.USE_MEAN_FOR_VALUE else ''
        median_star = '*' if not self.USE_MEAN_FOR_VALUE else ''
        return f"mean{mean_star}={self.mean:.3f}, median{median_star}={self.median:.3f}, range={self.min_value:.3f} to {self.max_value:.3f}, stddev={self.stddev:.3f}"

@final
class ProbeEddy:
    def __init__(self, config: ConfigWrapper):
        logging.info("Hello from ProbeEddyNG")

        self._printer: Printer = config.get_printer()
        self._full_name = config.get_name()
        self._name = self._full_name.split()[-1]

        sensors = {
            "ldc1612": ldc1612_ng.LDC1612_ng,
            "btt_eddy" : ldc1612_ng.LDC1612_ng,
            }
        sensor_type = config.getchoice('sensor_type', {s: s for s in sensors})

        self._sensor = sensors[sensor_type](config, sensor_type)
        self._mcu = self._sensor.get_mcu()

        self.params = ProbeEddyParams()
        self.params.load_from_config(config)

        # at what minimum physical height to start homing. It must be above the safe start position,
        # because we need to move from the start through the safe start position
        self._home_start_height = self.params.home_trigger_height + self.params.home_trigger_safe_start_offset + 1.0

        # physical offsets between probe and nozzle
        self.offset = {
            "x": self.params.x_offset,
            "y": self.params.y_offset,
        }

        # drive current to frequency map
        self._dc_to_fmap: Dict[int, ProbeEddyFrequencyMap] = {}

        version = config.getint(f"calibration_version", default=-1)
        calibration_bad = False
        if version == -1:
            if config.get('calibrated_drive_currents', None) is not None:
                calibration_bad = True
        elif version != ProbeEddyFrequencyMap.calibration_version:
            calibration_bad = True

        if calibration_bad:
            raise configerror(f"EDDYng calibration: calibration data invalid, please delete the calibrated_drive_currents, calibrated_version, and calibration data in your saved data in printer.cfg.")

        calibrated_drive_currents = config.getintlist('calibrated_drive_currents', [])
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
        self._printer.add_object('probe', self)

        # TODO: get rid of this
        self._cmd_helper = probe.ProbeCommandHelper(config, self, self._endstop_wrapper.query_endstop)

        # when doing a scan, what's the offset between probe readings at the bed
        # scan height and the accurate bed height, based on the last tap.
        self._tap_offset = 0.0
        self._last_probe_result = 0.0

        # define our own commands
        self._gcode = self._printer.lookup_object('gcode')
        self._dummy_gcode_cmd = self._gcode.create_gcode_command("", "", {})
        self.define_commands(self._gcode)

    def _log_error(self, msg):
        logging.error(f"{self._name}: {msg}")
        self._gcode.respond_raw(f"!! {msg}\n")
    
    def _log_info(self, msg):
        logging.info(f"{self._name}: {msg}")
        self._gcode.respond_info(msg)

    def _log_trace(self, msg):
        logging.debug(f"{self._name}: {msg}")

    def define_commands(self, gcode):
        gcode.register_command("PROBE_EDDY_NG_STATUS", self.cmd_STATUS, self.cmd_STATUS_help)
        gcode.register_command("PROBE_EDDY_NG_CALIBRATE", self.cmd_CALIBRATE, self.cmd_CALIBRATE_help)
        gcode.register_command("PROBE_EDDY_NG_CLEAR_CALIBRATION", self.cmd_CLEAR_CALIBRATION, self.cmd_CLEAR_CALIBRATION_help)
        gcode.register_command("PROBE_EDDY_NG_PROBE", self.cmd_PROBE, self.cmd_PROBE_help)
        gcode.register_command("PROBE_EDDY_NG_PROBE_STATIC", self.cmd_PROBE_STATIC, self.cmd_PROBE_STATIC_help)
        gcode.register_command("PROBE_EDDY_NG_PROBE_ACCURACY", self.cmd_PROBE_ACCURACY, self.cmd_PROBE_ACCURACY_help)
        gcode.register_command("PROBE_EDDY_NG_TAP", self.cmd_TAP, self.cmd_TAP_help)
        gcode.register_command("PROBE_EDDY_NG_SET_TAP_OFFSET", self.cmd_SET_TAP_OFFSET, "Set the tap offset for the bed mesh scan and other probe operations")
        gcode.register_command("PROBE_EDDY_NG_TEST_DRIVE_CURRENT", self.cmd_TEST_DRIVE_CURRENT, "Test a drive current.")

        # some handy aliases while I'm debugging things to save my fingers
        gcode.register_command("PES", self.cmd_STATUS, self.cmd_STATUS_help + " (alias for PROBE_EDDY_NG_STATUS)")
        gcode.register_command("PEP", self.cmd_PROBE, self.cmd_PROBE_help + " (alias for PROBE_EDDY_NG_PROBE)")
        gcode.register_command("PEPS", self.cmd_PROBE_STATIC, self.cmd_PROBE_STATIC_help + " (alias for PROBE_EDDY_NG_PROBE_STATIC)")
        gcode.register_command("PETAP", self.cmd_TAP, self.cmd_TAP_help + " (alias for PROBE_EDDY_NG_TAP)")

    def current_drive_current(self) -> int:
        return self._sensor.get_drive_current()

    def map_for_drive_current(self, dc: int = None) -> ProbeEddyFrequencyMap:
        if dc is None:
            dc = self.current_drive_current()
        if dc not in self._dc_to_fmap:
            raise self._printer.command_error(f"Drive current {dc} not calibrated")
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
        return drive_current in self._dc_to_fmap and self._dc_to_fmap[drive_current].calibrated()

    def _z_homed(self):
        curtime_r = self._printer.get_reactor().monotonic()
        kin_status = self._printer.lookup_object('toolhead').get_kinematics().get_status(curtime_r)
        logging.info(f"homed_axes: {kin_status['homed_axes']}")
        return 'z' in kin_status['homed_axes']

    def _xy_homed(self):
        curtime_r = self._printer.get_reactor().monotonic()
        kin_status = self._printer.lookup_object('toolhead').get_kinematics().get_status(curtime_r)
        return 'x' in kin_status['homed_axes'] and 'y' in kin_status['homed_axes']

    def _z_hop(self, by=5.0):
        if by < 0.0:
            raise self._printer.command_error("Z hop must be positive")
        toolhead: ToolHead = self._printer.lookup_object('toolhead')
        curpos = toolhead.get_position()
        curpos[2] = curpos[2] + by
        toolhead.manual_move(curpos, self.params.probe_speed)

    def save_config(self):
        for _, fmap in self._dc_to_fmap.items():
            fmap.save_calibration()

        configfile = self._printer.lookup_object('configfile')
        configfile.set(self._full_name, "calibrated_drive_currents", str.join(', ', [str(dc) for dc in self._dc_to_fmap.keys()]))
        configfile.set(self._full_name, "calibration_version", str(ProbeEddyFrequencyMap.calibration_version))

        self._log_info("Calibration saved. Issue a SAVE_CONFIG to write the values to your config file and restart Klipper.")

    def start_sampler(self, *args, **kwargs) -> ProbeEddySampler:
        if self._sampler:
            raise self._printer.command_error("EDDYng Already sampling")
        self._sampler = ProbeEddySampler(self, *args, **kwargs)
        self._sampler.start()
        return self._sampler

    def sampler_is_active(self):
        return self._sampler is not None and self._sampler.active()

    # Called by samplers when they're finished    
    def _sampler_finished(self, sampler: ProbeEddySampler, **kwargs):
        if self._sampler is not sampler:
            raise self._printer.command_error("EDDYng finishing sampler that's not active")

        self._sampler = None

        if self.save_samples_path is not None:
            with open(self.save_samples_path, "w") as data_file:
                samples = sampler.get_samples()
                raw_samples = sampler.get_raw_samples()
                data_file.write(f"time,frequency,z,kin_z,kin_v,raw_f,trigger_time,tap_end_time\n")
                for i in range(len(samples)):
                    trigger_time = kwargs.get('trigger_time', '')
                    tap_end_time = kwargs.get('tap_end_time', '')
                    s_t, s_freq, s_z = samples[i]
                    _, raw_f , _= raw_samples[i]
                    past_pos, past_v = get_toolhead_kin_pos(self._printer, s_t)
                    past_k_z = past_pos[2] if past_pos is not None else None 
                    if past_k_z is None or past_v is None:
                        past_k_z = ""
                        past_v = ""
                    data_file.write(f"{s_t},{s_freq},{s_z},{past_k_z},{past_v},{raw_f},{tap_end_time},{trigger_time}\n")
            logging.info(f"Wrote {len(samples)} samples to {self.save_samples_path}")
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
        if freqval > 0x0fffffff:
            height = -math.inf
            freq = 0.0
            err = f"ERROR: {bin(freqval >> 28)} "
        if not self.calibrated():
            err += "(Not calibrated) "

        gcmd.respond_info(f"Last coil value: {freq:.2f} ({height:.3f}mm) raw: {hex(freqval)} {err}status: {hex(status)} {self._sensor.status_to_str(status)}")

    cmd_PROBE_ACCURACY_help = "Probe accuracy"
    def cmd_PROBE_ACCURACY(self, gcmd: GCodeCommand):
        if not self._z_homed():
            raise self._printer.command_error(f"Must home Z before PROBE_ACCURACY")

        # How long to read at each sample time
        duration: float = gcmd.get_float('DURATION', 0.100, above=0.0)
        # whether to check +/- 1mm positions for accuracy
        start_z: float = gcmd.get_float('Z', self.params.home_trigger_height)
        offsets = gcmd.get('OFFSETS', None)

        probe_speed = gcmd.get_float('SPEED', self.params.probe_speed, above=0.0)
        lift_speed = gcmd.get_float('LIFT_SPEED', self.params.lift_speed, above=0.0)

        probe_zs = [start_z]

        if offsets is not None:
            probe_zs.extend([float(v)+start_z for v in offsets.split(',')])
        
        probe_zs.sort()
        probe_zs.reverse()

        # drive current to use
        old_drive_current = self.current_drive_current()    
        drive_current: int = gcmd.get_int('DRIVE_CURRENT', old_drive_current, minval=0, maxval=31)

        if not self.calibrated(drive_current):
            raise self._printer.command_error(f"Drive current {drive_current} not calibrated")

        try:
            self._sensor.set_drive_current(drive_current)
            th = self._printer.lookup_object('toolhead')
            th_pos = th.get_position()

            if th_pos[2] != probe_zs[0]:
                th.manual_move([None, None, probe_zs[0] + self.params.backlash_comp], lift_speed)
                th.wait_moves()
                th_pos = th.get_position()

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
                stddev_sum = np.sum([(s-result.value)**2.0 for s in result.samples])

                gcmd.respond_info("Probe at z={pz:.3f} is {result:v}")

                stddev_sums.append(stddev_sum)
                stddev_count += len(result.samples)
                results.append(result)
                ranges.append(rangev)
                from_zs.append(from_z)
            
            if len(results) > 1:
                avg_range = np.mean(ranges)
                avg_from_z = np.mean(from_zs)
                stddev = (np.sum(stddev_sums) / stddev_count) ** 0.5
                gcmd.respond_info("Probe overall avg range: {avg_range:.3f}, avg z deviation: {avg_from_z:.3f}, stddev: {stddev:.3f}")

        finally:
            self._sensor.set_drive_current(old_drive_current)

    cmd_CLEAR_CALIBRATION_help = "Clear calibration for all drive currents"
    def cmd_CLEAR_CALIBRATION(self, gcmd: GCodeCommand):
        drive_current: int = gcmd.get_int('DRIVE_CURRENT', -1)
        if drive_current == -1:
            self._dc_to_fmap = {}
            gcmd.respond_info(f"Cleared calibration for all drive currents")
        else:
            if drive_current not in self._dc_to_fmap:
                raise self._printer.command_error(f"Drive current {drive_current} not calibrated")
            del self._dc_to_fmap[drive_current]
            gcmd.respond_info(f"Cleared calibration for drive current {drive_current}")
        self.save_config()

    def cmd_SET_TAP_OFFSET(self, gcmd: GCodeCommand):
        value = gcmd.get_float('VALUE', None)
        adjust = gcmd.get_float('ADJUST', None)
        tap_offset = self._tap_offset
        if value is not None:
            tap_offset = value
        if adjust is not None:
            tap_offset += adjust
        self._tap_offset = tap_offset
        gcmd.respond_info(f"Set tap offset: {tap_offset:.3f}")

    def probe_static_height(self, duration: float = 0.100) -> ProbeEddyProbeResult:
        reactor = self._printer.get_reactor()
        now = self._mcu.estimated_print_time(reactor.monotonic())

        with self.start_sampler() as sampler:
            sampler.wait_for_sample_at_time(now + (duration + LDC1612_SETTLETIME))
            sampler.finish()

            samples = sampler.get_samples()
            if len(samples) == 0:
                return ProbeEddyProbeResult([])

            # skip LDC1612_SETTLETIME samples at start and end by looking
            # at the time values
            stime = samples[0][0] + LDC1612_SETTLETIME
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
                errors=sampler.get_error_count()
            )

    cmd_PROBE_help = "Probe the height using the eddy current sensor, moving the toolhead to the home trigger height, or Z if specified."
    def cmd_PROBE(self, gcmd: GCodeCommand):
        z: float = gcmd.get_float('Z', self.params.home_trigger_height)
        duration: float = gcmd.get_float('DURATION', 0.100, above=0.0)

        if not self._z_homed():
            raise self._printer.command_error(f"Must home Z before PROBE")

        th = self._printer.lookup_object('toolhead')
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
        drive_current: int = gcmd.get_int('DRIVE_CURRENT', old_drive_current, minval=0, maxval=31)
        duration: float = gcmd.get_float('DURATION', 0.100, above=0.0)
        save: bool = gcmd.get_int('SAVE', 0) == 1
        home_z: bool = gcmd.get_int('HOME_Z', 0) == 1

        if not self.calibrated(drive_current):
            raise self._printer.command_error(f"Drive current {drive_current} not calibrated")

        try:
            self._sensor.set_drive_current(drive_current)

            if save:
                self.save_samples_path = "/tmp/eddy-probe-static.csv"
            r = self.probe_static_height(duration)

            self._cmd_helper.last_z_result = float(r.value)

            if home_z:
                th = self._printer.lookup_object('toolhead')
                th_pos = th.get_position()
                th_pos[2] = r.value
                th.set_position(th_pos, [2])
                self._log_info(f"Homed Z to {r}")
            else:
                self._log_info(f"Probed {r}")

        finally:
            self._sensor.set_drive_current(drive_current)

    cmd_CALIBRATE_help = "Calibrate the eddy current sensor. Specify DRIVE_CURRENT to calibrate for a different drive current " + \
        "than the default. Specify Z_MAX to set a different calibration start point."
    def cmd_CALIBRATE(self, gcmd: GCodeCommand):
        if not self._xy_homed():
            raise self._printer.command_error("X and Y must be homed before calibrating")

        was_homed = self._z_homed()
        if was_homed:
            # z-hop so that manual probe helper doesn't complain if we're already
            # at the right place
            self._z_hop()
        else:
            logging.info("Z not homed, forcing position before calibration")
            # Z is probably not homed. So we'll set up a useful position so that
            # ManualProbeHelper can work
            th = self._printer.lookup_object('toolhead')
            th_pos = th.get_position()
            # This is proably not correct for some printers
            zrange = th.get_kinematics().rails[2].get_range()
            th_pos[2] = zrange[1] - 20.0
            th.set_position(th_pos, [2])

        manual_probe.ManualProbeHelper(self._printer, gcmd,
                                       lambda kin_pos: self.cmd_CALIBRATE_next(gcmd, kin_pos, was_homed))

    def cmd_CALIBRATE_next(self, gcmd: GCodeCommand, kin_pos: List[float], was_homed: bool):
        if kin_pos is None:
            if not was_homed and hasattr(th.get_kinematics(), "note_z_not_homed"):
                th.get_kinematics().note_z_not_homed()
            # User cancelled ManualProbeHelper
            return

        old_drive_current = self.current_drive_current()
        drive_current: int = gcmd.get_int('DRIVE_CURRENT', old_drive_current, minval=0, maxval=31)
        cal_z_max: float = gcmd.get_float('Z_MAX', self.params.calibration_z_max, above=2.0)
        z_target: float = gcmd.get_float('Z_TARGET', 0.010)

        probe_speed: float = gcmd.get_float('SPEED', self.params.probe_speed, above=0.0)
        lift_speed: float = gcmd.get_float('LIFT_SPEED', self.params.lift_speed, above=0.0)

        th = self._printer.lookup_object('toolhead')

        # We just did a ManualProbeHelper, so we're going to zero the z-axis
        # to make the following code easier, so it can assume z=0 is actually real zero.
        # The Eddy sensor calibration is done to nozzle height (not sensor or trigger height).
        th_pos = th.get_position() 
        th_pos[2] = 0.0
        th.set_position(th_pos, homing_axes=[2])

        th.wait_moves()

        logging.info(f"EDDYng calibrating from {kin_pos}, {th_pos}")

        mapping, fth_fit, htf_fit = self._create_mapping(cal_z_max,
                                                         z_target,
                                                         drive_current,
                                                         probe_speed,
                                                         drive_current,
                                                         for_calibration=True)
        if mapping is None or fth_fit is None or htf_fit is None:
            self._log_error("Calibration failed")
            return

        self._dc_to_fmap[drive_current] = mapping
        self.save_config()

        if not was_homed and hasattr(th.get_kinematics(), "note_z_not_homed"):
            th.get_kinematics().note_z_not_homed()

    def _create_mapping(self, z_start: float, z_target: float,
                       probe_speed: float, lift_speed: float,
                       drive_current: int,
                       for_calibration: bool) -> Tuple[ProbeEddyFrequencyMap, float, float]:
        th = self._printer.lookup_object('toolhead')
        th_pos = th.get_position() 

        # move to the start z of the mapping, going up first if we need to for backlash
        if th_pos[2] < z_start:
            th.manual_move([None, None, z_start + 3.0], lift_speed)
        th.manual_move([None, None, z_start], lift_speed)
        if for_calibration:
            th.manual_move([th_pos[0]-self.offset['x'], th_pos[1]-self.offset['y'], None], self.params.move_speed)

        first_sample_time = None
        last_sample_time = None

        old_drive_current = self.current_drive_current()
        try:
            self._sensor.set_drive_current(drive_current)
            times, freqs, heights = self._capture_samples_down_to(z_target, probe_speed)
            th.manual_move([None, None, z_start], lift_speed)
        finally:
            self._sensor.set_drive_current(old_drive_current)

        if times is None:
            self._log_error("No samples collected. This could be a hardware issue or an incorrect drive current.")
            return None, None, None

        # and build a map
        mapping = ProbeEddyFrequencyMap(self)
        fth_fit, htf_fit = mapping.calibrate_from_values(drive_current, times, freqs, heights, for_calibration)
        return mapping, fth_fit, htf_fit

    def _capture_samples_down_to(self, z_target: float, probe_speed: float) -> tuple[List[float], List[float], List[float]]:
        first_sample_time = None
        last_sample_time = None

        th = self._printer.lookup_object('toolhead')
        th.dwell(0.500) # give the sensor a bit to settle
        th.wait_moves()

        with self.start_sampler(calculate_heights=False) as sampler:
            first_sample_time = th.get_last_move_time()
            th.manual_move([None, None, z_target], probe_speed)
            th.wait_moves()
            last_sample_time = th.get_last_move_time()
            sampler.finish()

        # the samples are a list of [print_time, freq, dummy_height] tuples
        samples = sampler.get_samples()
        if len(samples) == 0:
            return None, None, None

        freqs = []
        heights = []
        times = []

        for s_t, s_freq, _ in samples:
            # TODO use get_past_toolhead_goal_z
            s_pos, _ = get_toolhead_kin_pos(self._printer, at=s_t)
            s_z = s_pos[2]
            if first_sample_time < s_t < last_sample_time and s_z >= z_target:
                times.append(s_t)
                freqs.append(s_freq)
                heights.append(s_z)
        
        return times, freqs, heights
    
    def cmd_TEST_DRIVE_CURRENT(self, gcmd: GCodeCommand):
        drive_current: int = gcmd.get_int('DRIVE_CURRENT', self.params.reg_drive_current, minval=1, maxval=31)
        z_start: float = gcmd.get_float('Z_MAX', self.params.calibration_z_max, above=2.0)
        z_end: float = gcmd.get_float('Z_TARGET', 0.010)

        mapping, fth, htf = self._create_mapping(z_start,
                                                 z_end,
                                                 self.params.probe_speed,
                                                 self.params.lift_speed,
                                                 drive_current,
                                                 False)
        if mapping is None or fth is None or htf is None:
            self._log_error("Test failed: likely no samples received, check the log for more clues")
            return

        hmin, hmax = mapping._height_range
        fmin, fmax = mapping._freq_range

        self._log_info(f"Drive current {drive_current}: height range: {hmin:.3f} to {hmax:.3f}, " + \
                       f"freq range: {fmin:.1f} to {fmax:.1f}. (Fit {fth:.4f},{htf:.4f})")

    #
    # PrinterProbe interface
    #

    def get_offsets(self):
        # the z offset is the trigger height, because the probe will trigger
        # at z=trigger_height (not at z=0)
        return self.offset["x"], self.offset["y"], self.params.home_trigger_height

    def get_probe_params(self, gcmd=None):
        return {
            'probe_speed': self.params.probe_speed,
            'lift_speed': self.params.lift_speed,
            'sample_retract_dist': 0.0,
        }

    def start_probe_session(self, gcmd):
        session = ProbeEddyScanningProbe(self, gcmd)
        session._start_session()
        return session
        #method = gcmd.get('METHOD', 'automatic').lower()
        #if method in ('scan', 'rapid_scan'):
        #    session = ProbeEddyScanningProbe(self, gcmd)
        #    session._start_session()
        #    return session
        #
        #return self._probe_session.start_probe_session(gcmd)

    def get_status(self, eventtime):
        status = self._cmd_helper.get_status(eventtime)
        status.update({
            'name': self._full_name,
            'home_trigger_height': self.params.home_trigger_height,
            'tap_offset': self._tap_offset,
            'last_probe_result': self._last_probe_result,
        })
        return status

    #
    # Moving the sensor to the correct position
    #
    def _probe_to_start_position_unhomed(self, move_home=False):
        if not self._xy_homed():
            raise self._printer.command_error("xy must be homed")
        if not self.sampler_is_active():
            raise self._printer.command_error("probe_to_start_position_unhomed: no sampler active")
        if not self.calibrated():
            raise self._printer.command_error("EDDYng not calibrated!")

        th = self._printer.lookup_object('toolhead')
        th_pos = th.get_position()

        # debug logging
        th_kin = th.get_kinematics()
        zlim = th_kin.limits[2]
        rail_range = th_kin.rails[2].get_range()
        logging.info(f"EDDYng probe to start unhomed: before movement: Z pos {th_pos[2]:.3f}, Z limits {zlim[0]:.2f}-{zlim[1]:.2f}, rail range {rail_range[0]:.2f}-{rail_range[1]:.2f}")

        if move_home and self.params.home_xy is not None:
            th.manual_move([self.params.home_xy[0], self.params.home_xy[1], None], self.params.lift_speed)
            th.wait_moves()

        start_height_ok_factor = 0.100

        # This is where we want to get to        
        start_height = self._home_start_height
        # This is where the probe thinks we are
        now_height = self._sampler.get_height_now()

        # If we can't get a value at all for right now, for safety, just abort.
        if now_height is None:
            raise self._printer.command_error("Couldn't get any valid samples from sensor. " + \
                  "If the toolhead is high off the build plate, this usually indicates a bad reg_drive_current.")

        logging.info(f"EDDYng probe_to_start_position_unhomed: now: {now_height} (start {start_height})")
        if abs(now_height - start_height) <= start_height_ok_factor:
            return

        th = self._printer.lookup_object('toolhead')
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
                logging.info(f"EDDYng probe_to_start_position_unhomed: moving toolhead down by {move_dist:.3f} to {th_pos[2]:.3f}")
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
            logging.info(f"EDDYng probe_to_start_position_unhomed: resetting toolhead to z {th_pos[2]:.3f}")
            th.set_position(th_pos, [2])

            n_pos = th.get_position()

            zlim = th_kin.limits[2]
            rail_range = th_kin.rails[2].get_range()
            logging.info(f"EDDYng: after reset: Z pos {n_pos[2]:.3f}, Z limits {zlim[0]:.2f}-{zlim[1]:.2f}, rail range {rail_range[0]:.2f}-{rail_range[1]:.2f}")

            th_pos[2] += move_up_by
            logging.info(f"EDDYng probe_to_start_position_unhomed: moving toolhead up by {move_up_by:.3f} to {th_pos[2]:.3f}")
            th.manual_move([None, None, th_pos[2]], self.params.probe_speed)
            self._sampler.wait_for_sample_at_time(th.get_last_move_time())
            now_height = self._sampler.get_last_height()

    def probe_to_start_position(self, z_pos=None):
        logging.info(f"EDDYng probe_to_start_position (tt: {self.params.tap_threshold}, z-homed: {self._z_homed()})")

        # If we're not homed at all, rely on the sensor values to bring us to
        # a good place to start a diving probe from
        if not self._z_homed():
            if z_pos is not None:
                raise self._printer.command_error("Can't probe_to_start_position with an explicit Z without homed Z")
            self._probe_to_start_position_unhomed()
            return

        th = self._printer.lookup_object('toolhead')
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
            logging.info(f"EDDYng probe_to_start_position: moving toolhead from {th_pos[2]:.3f} to {(start_z+1.0):.3f}")
            th_pos[2] = start_z + 1.0
            th.manual_move(th_pos, self.params.lift_speed)

        logging.info(f"EDDYng probe_to_start_position: moving toolhead from {th_pos[2]:.3f} to {start_z:.3f}")
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
            self.tapping_home(gcmd)
        finally:
            self._sensor.set_drive_current(drive_current)

    def tapping_home(self, gcmd: Optional[GCodeCommand]=None):
        logging.info("\nEDDYng Tapping move begin")

        if gcmd is None:
            gcmd = self._dummy_gcode_cmd

        tap_drive_current: int = gcmd.get_int(name='DRIVE_CURRENT', default=self.params.tap_drive_current, minval=0, maxval=31)
        tap_speed: float = gcmd.get_float('SPEED', self.params.tap_speed, above=0.0)
        lift_speed: float = gcmd.get_float('RETRACT_SPEED', self.params.lift_speed, above=0.0)
        tap_start_z: float = gcmd.get_float('START_Z', self.params.tap_start_z, above=2.0)
        target_z: float = gcmd.get_float('TARGET_Z', self.params.tap_target_z)
        tap_threshold: int = gcmd.get_int('THRESHOLD', self.params.tap_threshold, minval=0)
        tap_threshold = gcmd.get_int('TT', tap_threshold, minval=0) # alias for THRESHOLD

        do_retract = gcmd.get_int('RETRACT', 1) == 1

        self._sensor.set_drive_current(tap_drive_current)

        if not self._z_homed():
            raise self._printer.command_error("Z axis must be homed before tapping")

        # move to tap_start_z
        self.probe_to_start_position(tap_start_z)

        th = self._printer.lookup_object('toolhead')
        initial_position = th.get_position()
        target_position = initial_position[:]
        target_position[2] = target_z

        phoming = self._printer.lookup_object('homing')
        try:
            self.save_samples_path = "/tmp/tap-samples.csv"
            self._endstop_wrapper.tap_threshold = tap_threshold

            # so, we want something like a probing_move, but we want it 
            # this does a HomingMove.homing_move with a probe_pos = True
            #probe_position = phoming.probing_move(self._endstop_wrapper, target_position, tap_speed)
            endstops = [(self._endstop_wrapper, "probe")]
            hmove = HomingMove(self._printer, endstops)
            try:
                probe_position = hmove.homing_move(target_position, tap_speed, probe_pos=True)
            except self._printer.command_error:
                if self._printer.is_shutdown():
                    raise self._printer.command_error("Probing failed due to printer shutdown")
                th_pos = th.get_position()
                gcmd.respond_raw(f"!! Tap failed at {th_pos[2]:.3f}\n")
                if th_pos[2] < 1.0:
                    th_pos[2] = tap_start_z
                    th.manual_move(th_pos, lift_speed)
                raise
            if hmove.check_no_movement() is not None:
                raise self._printer.command_error("Probe triggered prior to movement")
        finally:
            self._endstop_wrapper.tap_threshold = 0
            # Need to do this right after to make sure we pick up the right samples
            try:
                self._write_tap_plot()
            except:
                logging.info(f"Failed to write tap plot.")

        # same as what's returned    
        probe_position = hmove.trigpos
        haltpos = hmove.haltpos

        th_pos = th.get_position()
        logging.info(f"HMOVE: trigpos: {probe_position[2]:.3f} haltpos: {haltpos[2]:.3f} thpos: {th_pos[2]:.3f}")

        # this is the real height=0 position
        probe_trigger_z = probe_position[2]

        now_z = th_pos[2]

        # we're at now_z, but probe_z is the actual zero. We expect now_z
        # to be below or equal to probe_z because there will always be
        # a bit of overshoot due to trigger delay, and because we actually
        # fire the trigger later than when the tap starts (and the tap start
        # time is what's used to compute probe_position)
        if now_z > probe_trigger_z:
            raise self._printer.command_error(f"Unexpected: now_z {now_z:.3f} is above probe_z {probe_trigger_z:.3f} after tap")

        # How much the toolhead overshot the real z=0 position. This is the amount
        # the toolhead is pushing into the build plate.
        overshoot = probe_trigger_z - now_z

        logging.info(f"Tap triggered at z: {probe_trigger_z:.4f}, now z: {now_z:.4f}, overshoot: {overshoot:.4f}")

        # To re-home the toolhead to true zero, we need to adjust its position up by the overshoot,
        # which means the real current toolhead position is just how much it overshot zero by.
        #toolhead_real_z = -overshoot
        # This is just for logging
        #th_adjust = now_z - toolhead_real_z
        # Actually set the position for the toolhead
        #gcmd.respond_info(f"Adjusting z by {th_adjust:.4f}")
        #logging.info(f"Changing current toolhead z by {th_adjust:.4f} from {now_z:.3f} to {toolhead_real_z:.3f} {'' if do_adjust else '(no adjust set; no change)'}")
        #th_pos[2] = toolhead_real_z
        #th.set_position(th_pos)

        # Set the probe_trigger_z as the z offset
        gcode_move = self._printer.lookup_object('gcode_move')
        logging.info(f"  Pre: gcode homing position: {gcode_move.homing_position[2]:.3f}, base position: {gcode_move.base_position[2]:.3f}")
        gcode_delta = probe_trigger_z - gcode_move.homing_position[2] + self.params.tap_adjust_z
        gcode_move.base_position[2] += gcode_delta
        gcode_move.homing_position[2] = probe_trigger_z + self.params.tap_adjust_z
        logging.info(f"After: gcode homing position: {gcode_move.homing_position[2]:.3f}, base position: {gcode_move.base_position[2]:.3f}")

        # Now the Z axis is in the proper physical-aligned system. Retract back to tap_start_z
        if tap_start_z < th_pos[2]:
            raise self._printer.command_error(f"Initial position {tap_start_z:.3f} is below current, not moving down!")

        #
        # Figure out the offset to apply for future bed mesh calibrations and non-tap Z-homes
        #
        self._sensor.set_drive_current(self.params.reg_drive_current)

        # Move to the home trigger height, which should be the height that the sensor triggers
        # and that we do bed meshes at
        th.manual_move([None, None, self.params.home_trigger_height], lift_speed)
        th.dwell(0.500)
        th.wait_moves()

        result = self.probe_static_height()
        self._tap_offset = self.params.home_trigger_height - result.value
        gcmd.respond_info(f"Tap sensor offset at z={self.params.home_trigger_height:.3f}: {self._tap_offset:.4f} [probed {result}]")

        if abs(self._tap_offset) > 0.250:
            gcmd.respond_raw("!! WARNING: Tap offset is high; consider recalibrating sensor\n")

        if do_retract:
            th_pos[2] = tap_start_z
            th.manual_move(th_pos, lift_speed)
            th.wait_moves()
            th.flush_step_generation()

        logging.info("EDDYng Tapping move end\n")

    def _write_tap_plot(self, gcmd=None):
        if not HAS_PLOTLY:
            return
        
        samples = self._last_sampler_samples
        raw_samples = self._last_sampler_raw_samples
        memos = self._last_sampler_memos
        if samples is None or raw_samples is None:
            return

        th = self._printer.lookup_object('toolhead')

        s_t = np.asarray([s[0] for s in samples])
        #s_f = np.array([s[1] for s in samples])
        # Note: we're using raw freqs
        s_rf = s_f = np.asarray([s[1] for s in raw_samples])
        s_z = np.asarray([s[2] for s in samples])
        s_kinz = np.asarray([get_toolhead_kin_pos(self._printer, s[0])[0][2] for s in samples])

        time_start = s_t.min()

        # normalize times to start at 0
        s_t = s_t - time_start
        trigger_time = memos.get('trigger_time', time_start) - time_start
        tap_end_time = memos.get('tap_end_time', time_start) - time_start

        tap_threshold = memos.get('tap_threshold', 0)

        s_f_wma = np_wma(s_rf, 16)
        s_f_wma_d = np.gradient(s_f_wma)

        avg_window = 4
        s_f_wma_d_avg = []
        for i in range(len(s_f_wma_d)):
            if i < avg_window:
                s_f_wma_d_avg.append(0)
            else:
                s_f_wma_d_avg.append(np.sum(s_f_wma_d[i-avg_window+1:i+1])/avg_window)
    
        tap_accums = []
        accum_val = 0
        for i in range(len(s_f_wma_d)):
            val = s_f_wma_d_avg[i]
            last_val = 0 if i == 0 else s_f_wma_d_avg[i-1]
            if val < last_val:
                accum_val += last_val - val
            else:
                accum_val = 0
            tap_accums.append(accum_val)

        import plotly.graph_objects as go 
        fig = go.Figure()
        fig.add_trace(go.Scatter(x=s_t, y=s_z, mode='lines', name='Z', yaxis='y3'))
        fig.add_trace(go.Scatter(x=s_t, y=s_kinz, mode='lines', name='KinZ', yaxis='y3'))

        fig.add_trace(go.Scatter(x=s_t, y=s_f, mode='lines', name='Freq'))
        #fig.add_trace(go.Scatter(x=tt, y=tfraw, mode='lines', name='Fraw', yaxis='y4'))

        fig.add_trace(go.Scatter(x=s_t, y=s_f_wma, mode='lines', name='wma'))
        fig.add_trace(go.Scatter(x=s_t, y=s_f_wma_d, mode='lines', name='wma_d', yaxis='y2'))
        fig.add_trace(go.Scatter(x=s_t, y=s_f_wma_d_avg, mode='lines', name='wma_d_avg', yaxis='y2'))

        fig.add_trace(go.Scatter(x=s_t, y=tap_accums, mode='lines', name='accum', yaxis='y2'))

        if trigger_time > 0:
            fig.add_shape(type='line', x0=trigger_time, x1=trigger_time, y0=0, y1=1, xref="x", yref="paper", line=dict(color='orange', width=1))
        if tap_end_time > 0:
            fig.add_shape(type='line', x0=tap_end_time, x1=tap_end_time, y0=0, y1=1, xref="x", yref="paper", line=dict(color='green', width=1))
        if tap_threshold > 0:
            fig.add_shape(type='line', x0=0, x1=1, y0=tap_threshold, y1=tap_threshold, xref="paper", yref="y2", line=dict(color='gray', width=1, dash='dash'))


        fig.update_layout(hovermode='x unified',
            # Freq
            yaxis=dict(title="Freq", tickformat='d'),
            # WMA derivatives, WMA deriv averages, tap accum
            yaxis2=dict(overlaying="y", side="right", tickformat='d'),
            # Z axis
            yaxis3=dict(title="Z", overlaying="y", side="right"),
            height=800)
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
        self._toolhead = self._printer.lookup_object('toolhead')
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
        self._sample_time: float = gcmd.get_float("SAMPLE_TIME", self.eddy.params.scan_sample_time, above=0.0)
        self._is_rapid = gcmd.get("METHOD", "automatic").lower() == 'rapid_scan'

        self._sampler: ProbeEddySampler = None

        self._notes = []

    def get_probe_params(self, gcmd):
        # this seems to be all that external users of get_probe_params
        # use (bed_mesh, axis_twist_compensation)
        return {
            'lift_speed': self.eddy.params.lift_speed,
            'probe_speed': self.eddy.params.probe_speed,
        }

    def _start_session(self):
        if not self.eddy._z_homed():
            raise self._printer.command_error("Z axis must be homed before probing")

        self.eddy.probe_to_start_position()
        self._sampler = self.eddy.start_sampler()

    def end_probe_session(self):
        self._sampler.finish()
        self._sampler = None

    def _lookup_toolhead_pos(self, time):
        pos, _ = get_toolhead_kin_pos(self._printer, time)
        return pos

        #kin_spos = {s.get_name(): s.mcu_to_commanded_position(
        #                              s.get_past_mcu_position(time))
        #            for s in self._toolhead_kin.get_steppers()}
        #return self._toolhead_kin.calc_position(kin_spos)

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
            logging.info(f"ProbeEddyScanningProbe: toolhead at {th_pos[2]:.3f}!") #, fixing")
            #th.manual_move([None, None, self._scan_z + 2.0], self.eddy.params.lift_speed)
            #th.manual_move([None, None, self._scan_z], self.eddy.params.probe_speed)
            #th.wait_moves()

        start_time = th.get_last_move_time()
        self._toolhead.dwell(self._sample_time + self._sample_time_delay)

        th_pos = self._lookup_toolhead_pos(start_time)
        self._notes.append((start_time, start_time, th_pos))

    def pull_probed_results(self):
        if self._is_rapid:
            # Flush lookahead (so all lookahead callbacks are invoked)
            self._toolhead.get_last_move_time()

        # make sure we get the sample for the final move
        self._sampler.wait_for_sample_at_time(self._notes[-1][0] + self._sample_time)

        # note: we can't call finish() here! this session can continue to be used
        # to probe additional points and pull them, because that's what QGL does.

        results = []

        logging.info(f"ProbeEddyScanningProbe: pulling {len(self._notes)} results")
        for start_time, sample_time, th_pos in self._notes:
            if th_pos is None:
                # this is giving a totally wrong position. I don't know what's up with that.
                th_pos = self._lookup_toolhead_pos(sample_time)
                #logging.info(f"th @ {sample_time:.3f}: {th_pos[0]:.2f} {th_pos[1]:.2f} {th_pos[2]:.2f}")

            end_time = start_time + self._sample_time
            height = self._sampler.find_height_at_time(start_time, end_time)

            if not math.isclose(th_pos[2], self._scan_z, rel_tol=1e-3):
                logging.info(f"ProbeEddyScanningProbe warning: toolhead not at home_trigger_height ({self._scan_z:.3f}) during probes (saw {th_pos[2]:.3f})")

            # adjust the sensor height value based on the fine-tuned tap offset amount
            height += self._tap_offset

            # the delta between where the toolhead thinks it should be (since it
            # should be homed), and the actual physical offset (height)
            z_deviation = th_pos[2] - height

            # what callers want to know is "what Z would the toolhead be at, if it was at the height
            # the probe would 'trigger'", because this is all done in terms of klicky-type probes
            th_pos[2] = float(self._scan_z + z_deviation)
            #toolhead_pos[2] = height

            results.append(th_pos)

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
        # if tap threshold is > 0, then we do a tap when we home
        self.tap_threshold = 0
        # if not None, after a probe session is finished we'll
        # write all samples here
        self.save_samples_path: Optional[str] = None

        self._multi_probe_in_progress = False

        self._dispatch = mcu.TriggerDispatch(self._mcu)

        # the times of the 
        self._trigger_time = 0.
        self._tap_end_time = 0.0

        self._homing_in_progress = False
        self._sampler: ProbeEddySampler = None

        # Register z_virtual_endstop pin
        self._printer.lookup_object('pins').register_chip('probe', self)
        # Register event handlers
        self._printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self._printer.register_event_handler("homing:homing_move_begin", self._handle_homing_move_begin)
        self._printer.register_event_handler("homing:homing_move_end", self._handle_homing_move_end)
        self._printer.register_event_handler("homing:home_rails_begin", self._handle_home_rails_begin)
        self._printer.register_event_handler("homing:home_rails_end", self._handle_home_rails_end)
        self._printer.register_event_handler("gcode:command_error", self._handle_command_error)

        # copy some things in for convenience
        self._home_trigger_height = self.eddy.params.home_trigger_height
        self._home_trigger_safe_start_offset = self.eddy.params.home_trigger_safe_start_offset
        self._home_start_height = self.eddy._home_start_height # this is trigger + safe_start + 1.0
        self._probe_speed = self.eddy.params.probe_speed
        self._lift_speed = self.eddy.params.lift_speed

        self._tap_speed = self.eddy.params.tap_speed
        self._tap_threshold = self.eddy.params.tap_threshold
        self._tap_z_target = self.eddy.params.tap_target_z
        self._tap_start_height = self.eddy.params.tap_start_z

    def _handle_mcu_identify(self):
        kin = self._printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self.add_stepper(stepper)

    def _handle_home_rails_begin(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        if not self in endstops:
            return
        # Nothing to do
        pass

    def _handle_homing_move_begin(self, hmove):
        if not self in hmove.get_mcu_endstops():
            return
        self._sampler = self.eddy.start_sampler()
        self._homing_in_progress = True
        # if we're doing a tap, we're already in the right position
        if self.tap_threshold == 0:
            self.eddy._probe_to_start_position_unhomed(move_home=True)

    def _handle_homing_move_end(self, hmove):
        if not self in hmove.get_mcu_endstops():
            return
        self._sampler.finish()
        self._homing_in_progress = False

    def _handle_home_rails_end(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        if not self in endstops:
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
            logging.exception("EDDYng handle_command_error: sampler.finish() failed")

    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'z_virtual_endstop':
            raise pins.error("Probe virtual endstop only useful as endstop pin")
        if pin_params['invert'] or pin_params['pullup']:
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
        logging.info(f"EDDYng get_position_endstop -> {0.0 if self.tap_threshold > 0.0 else self._home_trigger_height}")
        if self.tap_threshold > 0:
            return 0.0
        else:
            return self._home_trigger_height

    # The Z homing sequence is:
    #   - multi_probe_begin
    #   - probe_prepare
    #   - home_start
    #   - home_wait
    #   - probe_finish
    #   - multi_probe_end
    #
    # Note no probing_move during homing! `HomingMove.homing_move` drives the above sequence.
    # probing_move is only used during actual probe operations. 

    def home_start(self, print_time, sample_time, sample_count, rest_time, triggered=True):
        if not self._sampler.active():
            raise self._printer.command_error("home_start called without a sampler active")

        self._trigger_time = 0.
        self._tap_end_time = 0.

        trigger_height = self._home_trigger_height
        safe_height = trigger_height + self._home_trigger_safe_start_offset

        trigger_freq = self.eddy.height_to_freq(trigger_height)
        safe_freq = self.eddy.height_to_freq(safe_height)

        #start_time = print_time + HOME_TRIGGER_START_TIME_OFFSET
        #start_time = 0.025 #print_time + 0.025
        safe_time = 0 if self.tap_threshold > 0 else print_time + self.eddy.params.home_trigger_safe_time_offset

        trigger_completion = self._dispatch.start(print_time)

        logging.info(f"EDDYng home_start: {print_time:.3f} freq: {trigger_freq:.2f} safe-start: {safe_freq:.2f}")
        # setup homing -- will start scanning and trigger when we hit
        # trigger_freq
        self._sensor.setup_home(self._dispatch.get_oid(),
                                mcu.MCU_trsync.REASON_ENDSTOP_HIT, self.REASON_BASE,
                                trigger_freq, safe_freq, safe_time,
                                tap_threshold=self.tap_threshold)

        return trigger_completion

    def home_wait(self, home_end_time):
        logging.info(f"EDDYng home_wait until {home_end_time:.3f}")
        #logging.info(f"EDDYng home_wait {home_end_time} cur {curtime} ept {est_print_time} ehe {est_he_time}")
        self._dispatch.wait_end(home_end_time)

        # make sure homing is stopped, and grab the trigger_time from the mcu
        home_result = self._sensor.finish_home()
        trigger_time = home_result.trigger_time
        tap_end_time = home_result.tap_end_time

        self._sampler.memo("trigger_time", trigger_time)
        self._sampler.memo("tap_end_time", tap_end_time)
        if self.tap_threshold > 0:
            self._sampler.memo("tap_threshold", self.tap_threshold)

        logging.info(f"EDDYng trigger_time {trigger_time} (mcu: {self._mcu.print_time_to_clock(trigger_time)}) tap_end_time: {tap_end_time}")

        # nb: _dispatch.stop() will treat anything >= REASON_COMMS_TIMEOUT as an error,
        # and will only return those results. Fine for us since we only have one trsync,
        # but annoying in general.
        res = self._dispatch.stop()
        is_tap = self.tap_threshold > 0

        if is_tap:
            logging.info(f"EDDYng mcu_probe: resetting tap_threshold")
            self._tap_threshold = 0

        last_sample_time = trigger_time
        if self.tap_threshold > 0:
            last_sample_time = tap_end_time
        self._sampler.wait_for_sample_at_time(last_sample_time)

        # success?
        if res == mcu.MCU_trsync.REASON_ENDSTOP_HIT:
            self._trigger_time = trigger_time
            self._tap_end_time = tap_end_time
            return trigger_time

        # various errors
        if res == mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
            raise self._printer.command_error("Communication timeout during homing")
        if res == self.REASON_ERROR_SENSOR:
            status = self._sensor.latched_status_str()
            raise self._printer.command_error(f"Sensor error (ldc status: {status})")
        if res == self.REASON_ERROR_PROBE_TOO_LOW:
            raise self._printer.command_error(f"Probe too low at start of homing, did not clear safe threshold.")
        if res == self.REASON_ERROR_TOO_EARLY:
            raise self._printer.command_error(f"Probe did not clear safe time threshold.")
        if res == mcu.MCU_trsync.REASON_PAST_END_TIME:
            raise self._printer.command_error(f"Probe completed movement before triggering. If this is a tap, try lowering the threshold. See the docs for more information.")

        raise self._printer.command_error(f"Unknown homing error: {res}")

    def query_endstop(self, print_time):
        # XXX TODO is this valuable at all? Should we just always return False? Or True?
        # query_endstops is a web request, meaning this might keep getting hit, which means
        # it'll keep starting/stopping measurement

        SUPPORT_QUERY_ENDSTOP = False
        if not SUPPORT_QUERY_ENDSTOP:
            return False

        if self.tap_threshold > 0:
            # XXX unclear how to do this with tap? It's basically always going to be false
            # unless the toolhead is somehow at 0.0, and we will never know if that's true 0.0
            # unless we tap.  We could remember the tap freq?
            return False

        curtime = self._reactor.monotonic()
        est_print_time = self._mcu.estimated_print_time(curtime)
        if est_print_time < (print_time-0.050) or est_print_time > (print_time+0.050):
            logging.warning(f"query_endstop: print_time {print_time} is too far from current {est_print_time}")
            return True

        self._sensor._start_measurements()
        result = self._sensor.read_one_value()
        self._sensor._finish_measurements()

        freqval = result.freqval
        freq = result.freq

        # if in error, return -inf height to make endstop checks easier
        if freqval > 0x0fffffff:
            logging.warning(f"query_endstop: got error")
            return True

        height = self.eddy.freq_to_height(freq)
        return bool(height < self._home_trigger_height)

    def _setup_sampler(self):
        self._sampler = self.eddy.start_sampler()

    def _finish_sampler(self):
        self._sampler.finish()
        self._sampler = None

# Helper to gather samples and convert them to probe positions
@final
class ProbeEddySampler:
    def __init__(self, eddy: ProbeEddy, calculate_heights: bool = True, trace: bool = False):
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

        self._errors += msg['errors']
        self._raw_samples.extend(msg['data'])
        return True

    def start(self):
        if self._stopped:
            raise self._printer.command_error("ProbeEddySampler.start() called after finish()")
        if not self._started:
            self._raw_samples = []
            self._sensor.add_bulk_sensor_data_client(self._add_hw_measurement)
            self._started = True

    def finish(self):
        if self._stopped:
            return
        if not self._started:
            raise self._printer.command_error("ProbeEddySampler.finish() called without start()")
        if self.eddy._sampler is not self:
            raise self._printer.command_error("ProbeEddySampler.finish(): eddy._sampler is not us!")
        self.eddy._sampler_finished(self)
        self._stopped = True

    def _update_samples(self):
        if len(self._samples) == len(self._raw_samples):
            return

        start_idx = len(self._samples)
        conv_ratio = self._sensor.freqval_conversion_value()
        if self._fmap is not None:
            new_samples = [(
                t,
                round(conv_ratio * f, ndigits=3),
                self._fmap.freq_to_height(round(conv_ratio * f, ndigits=3)))
                for t, f, _ in self._raw_samples[start_idx:]]
            self._samples.extend(new_samples)
        else:
            self._samples.extend([(t, round(conv_ratio * f, ndigits=3), math.inf) for t, f, _ in self._raw_samples[start_idx:]])

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
            raise self._printer.command_error("ProbeEddySampler: no height mapping")
        self._update_samples()
        if len(self._samples) == 0:
            raise self._printer.command_error("ProbeEddySampler: no samples")
        return self._samples[-1][2]

    # wait for a sample for the current time and get a new height 
    def get_height_now(self, average=False) -> Optional[float]:
        start = end = self._mcu.estimated_print_time(self._reactor.monotonic())
        if average:
            end = start + 0.100
        if not self.wait_for_sample_at_time(end, max_wait_time=0.250, raise_error=False):
            return None
        if average:
            return self.find_height_at_time(start, end)
        return self.get_last_height()

   # Wait until a sample for the given time arrives
    def wait_for_sample_at_time(self, sample_print_time, max_wait_time=0.250, raise_error=True) -> bool:
        def report_no_samples():
            if raise_error:
                log_traceback(5)
                raise self._printer.command_error(f"No samples received for time {sample_print_time:.3f} (waited for {max_wait_time:.3f})")
            return False

        if self._stopped:
            # if we're not getting any more samples, we can check directly
            if len(self._raw_samples) == 0:
                return report_no_samples()
            return self._raw_samples[-1][0] >= sample_print_time

        wait_start_time = self._mcu.estimated_print_time(self._reactor.monotonic())

        # if sample_print_time is in the future, make sure to wait max_wait_time
        # past the expected time
        if sample_print_time > wait_start_time:
            max_wait_time = max_wait_time + (sample_print_time - wait_start_time)

        # this is just a sanity check, there's no reason to ever wait this long
        if max_wait_time > 5.0:
            traceback.print_stack()
            logging.info(f"ProbeEddyFrequencySampler: max_wait_time {max_wait_time:.3f} is too far into the future")
            raise self._printer.command_error(f"ProbeEddyFrequencySampler: max_wait_time {max_wait_time:.3f} is too far into the future")

        if self._trace:
            logging.info(f"EDDYng waiting for sample at {sample_print_time:.3f} (now: {wait_start_time:.3f}, max_wait_time: {max_wait_time:.3f})")
        while len(self._raw_samples) == 0 or self._raw_samples[-1][0] < sample_print_time:
            now = self._mcu.estimated_print_time(self._reactor.monotonic())
            if now - wait_start_time > max_wait_time:
                return report_no_samples()
            self._reactor.pause(now + 0.010)

        return True

    # Wait for some samples to be collected, even if errors
    # TODO: there's a minimum wait time -- we need to fill up the buffer before data is sent, and that
    # depends on the data rate
    def wait_for_samples(self, max_wait_time=0.250, count_errors=False, min_samples=1, new_only=False, raise_error=True):
        # Make sure enough samples have been collected
        wait_start_time = self._mcu.estimated_print_time(self._reactor.monotonic())

        start_error_count = self._errors
        if new_only:
            start_count = len(self._raw_samples) + (self._errors if count_errors else 0)
        else:
            start_count = 0

        while (len(self._raw_samples) + (self._errors if count_errors else 0)) - start_count < min_samples:
            now = self._mcu.estimated_print_time(self._reactor.monotonic())
            if now - wait_start_time > max_wait_time:
                if raise_error:
                    raise self._printer.command_error(f"probe_eddy_ng sensor outage: no samples for {max_wait_time:.2f}s (got {self._errors - start_error_count} errors)")
                return False
            self._reactor.pause(now + 0.010)

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
            raise self._printer.command_error("find_height_at_time: end_time is before start_time")

        self._update_samples()

        if len(self._samples) == 0:
            raise self._printer.command_error("No samples at all, so none in time range")

        if self._trace:
            logging.info(f"EDDYng find_height_at_time: {len(self._samples)} samples, time range {self._samples[0][0]:.3f} to {self._samples[-1][0]:.3f}")

        # find the first sample that is >= start_time
        start_idx = bisect.bisect_left([t for t, _, _ in self._samples], start_time)
        if start_idx >= len(self._samples):
            raise self._printer.command_error("Nothing after start_time?")

        # find the last sample that is <= end_time
        end_idx = bisect.bisect_right([t for t, _, _ in self._samples], end_time)
        if end_idx == 0:
            raise self._printer.command_error("found something at start_time, but not before end_time?")

        # average the heights of the samples in the range
        heights = [h for _, _, h in self._samples[start_idx:end_idx]]
        hmin, hmax = np.min(heights), np.max(heights)
        mean = np.mean(heights)
        median = np.median(heights)
        if self._trace:
            logging.info(f"EDDYng find_height_at_time: {len(heights)} samples, median: {median:.3f}, mean: {mean:.3f} (range {hmin:.3f}-{hmax:.3f})")

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
        self._height_range = (-math.inf, math.inf)
        self._freq_range = (-math.inf, math.inf)

    def _str_to_exact_floatlist(self, str):
        return [float.fromhex(v) for v in str.split(',')]

    def _exact_floatlist_to_str(self, vals):
        return str.join(', ', [float.hex(v) for v in vals])

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
        return float(self._interp_extrapolate(height, self._heights[::-1], self._freqs[::-1]))

    def load_from_config(self, config: ConfigWrapper, drive_current: int):
        calibstr = config.get(f"calibration_{drive_current}", None)
        if calibstr is None:
            self.drive_current = 0
            self._freqs = None
            self._heights = None
            self._height_range = (-math.inf, math.inf)
            self._freq_range = (-math.inf, math.inf)
            return

        data = pickle.loads(base64.b64decode(calibstr))
        freqs = np.asarray(data['f'])
        heights = np.asarray(data['h'])
        dc = data['dc']
        h_range = data.get('h_range', (-math.inf, math.inf))
        f_range = data.get('f_range', (-math.inf, math.inf))

        if dc != drive_current:
            raise configerror(f"ProbeEddyFrequencyMap: drive current mismatch: loaded {dc} != requested {drive_current}")

        self._freqs = freqs
        self._heights = heights
        self._height_range = h_range
        self._freq_range = f_range
        self.drive_current = drive_current

        logging.info(f"EDDYng Loaded calibration for drive current {drive_current} ({len(self._freqs)} points)")

    def save_calibration(self):
        if self._freqs is None or self._heights is None:
            return

        configfile = self._eddy._printer.lookup_object('configfile')
        data = {
            'f': self._freqs.tolist(),
            'h': self._heights.tolist(),
            'h_range': self._height_range,
            'f_range': self._freq_range,
            'dc': self.drive_current
        }
        calibstr = base64.b64encode(pickle.dumps(data)).decode()
        configfile.set(self._eddy._full_name, f"calibration_{self.drive_current}", calibstr)

    # Compute calibration polynomials from a set of raw data points.
    def calibrate_from_values(self, drive_current: int,
                              times: List[float],
                              raw_freqs_list: List[float],
                              raw_heights_list: List[float],
                              for_calibration: bool = True):
        if len(raw_freqs_list) != len(raw_heights_list):
            raise ValueError("freqs and heights must be the same length")

        # everything must be a np.array or things get confused below
        raw_freqs = np.asarray(raw_freqs_list)
        raw_heights = np.asarray(raw_heights_list)

        # smooth out the data; a simple rolling average does the job,
        # but centered to give more accurate data (since we have the full
        # data set). The edges of the data just use a smaller window.
        avg_freqs = np_rolling_mean(raw_freqs, 16)
        avg_heights = np_rolling_mean(raw_heights, 16)

        if for_calibration:
            with open("/tmp/eddy-calibration.csv", "w") as data_file:
                data_file.write(f"time,frequency,avg_freq,z,avg_z\n")
                for i in range(len(raw_freqs)):
                    s_t = times[i]
                    s_f = raw_freqs[i]
                    s_z = raw_heights[i]
                    s_af = avg_freqs[i]
                    s_az = avg_heights[i]
                    data_file.write(f"{s_t},{s_f},{s_af},{s_z},{s_az}\n")
                logging.info(f"Wrote {len(raw_freqs)} samples to /tmp/eddy-calibration.csv")
                #gcmd.respond_info(f"Wrote {len(raw_freqs)} samples to /tmp/eddy-calibration.csv")

        max_height = float(avg_heights.max())
        min_height = float(avg_heights.min())
        min_freq = float(avg_freqs.min())
        max_freq = float(avg_freqs.max())
        freq_spread = ((max_freq / min_freq) - 1.0) * 100.0

        # Check if our calibration is good enough
        if for_calibration:
            if max_height < 2.5: # we really can't do anything with this
                self._eddy._log_error(f"Error: max height for valid samples is too low: {max_height:.3f} < 2.5. Refer to the documentation for troubleshooting.")
                return None, None

            if min_height > 0.25: # likewise can't do anything with this
                self._eddy._log_error(f"Error: min height for valid samples is too high: {min_height:.3f} > 0.25. Refer to the documentation for troubleshooting.")
                return None, None

            if min_height > 0.025:
                self._eddy._log_info(f"Warning: min height is {min_height:.3f}, which is too high for tap. This drive current will likely not work for tap, but should be fine for homing.")

            # somewhat arbitrary spread
            if freq_spread < 0.85:
                self._eddy._log_info(f"Warning: frequency spread is low ({freq_spread:.2f}%, {min_freq:.1f}-{max_freq:.1f}), consider adjusting your sensor height")

        # Calculate RMSE

        qf = np.linspace(avg_freqs.min(), avg_freqs.max(), self._eddy.params.calibration_points)
        qz = np.interp(qf, avg_freqs, avg_heights)
        # 25: because we don't care too much about the early (high z) values
        rmse_fth = np_rmse(lambda v: np.interp(v, qf, qz), avg_freqs[25:], avg_heights[25:])
        rmse_htf = np_rmse(lambda v: np.interp(v, qz[::-1], qf[::-1]), avg_heights[25:], avg_freqs[25:])

        self._freqs = qf
        self._heights = qz
        self._height_range = (min_height, max_height)
        self._freq_range = (min_freq, max_freq)
        self.drive_current = drive_current

        if for_calibration:
            self._eddy._log_info(f"Calibration for drive current {drive_current}: height: {min_height:.3f} to {max_height:.3f}, " + \
                                f"freq spread {freq_spread:.2f}% (max freq: {max_freq:.1f}), RMSE F->H: {rmse_fth:.4f}, H->F: {rmse_htf:.2f}")
            self._save_calibration_plot(avg_freqs, avg_heights, qf, qz, rmse_fth, rmse_htf)

        return rmse_fth, rmse_htf

    def _save_calibration_plot(self, freqs, heights, qf, qz, rmse_fth, rmse_htf):
        if not HAS_PLOTLY:
            return
        
        import plotly.graph_objects as go 
        fig = go.Figure()
        fig.add_trace(go.Scatter(x=freqs, y=heights, mode='lines', name='Z'))
        fig.add_trace(go.Scatter(x=freqs, y=np.interp(freqs, qf, qz), mode='lines', name=f'Z {rmse_fth:.4f}'))

        fig.add_trace(go.Scatter(x=heights, y=freqs, mode='lines', name='F', xaxis='x2', yaxis='y2'))
        fig.add_trace(go.Scatter(x=heights, y=np.interp(heights, qz[::-1], qf[::-1]), mode='lines', name=f'F ({rmse_htf:.2f})',
                                 xaxis='x2', yaxis='y2'))

        fig.update_layout(
            hovermode='x unified',
            title=f"Calibration for drive current {self.drive_current}",
            xaxis2=dict(title='Height', overlaying='x', side='top'),
            yaxis2=dict(title='Freq', overlaying='y', side='right'),
            )
        fig.write_html("/tmp/eddy-calibration.html")

    def raw_freqval_to_height(self, raw_freq: int) -> float:
        if raw_freq > 0x0fffffff:
            return -math.inf #error bits set
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
        return self._freq_to_height is not None and self._height_to_freq is not None

def get_toolhead_kin_pos(printer, at=None):
    toolhead = printer.lookup_object("toolhead")
    toolhead_kin = toolhead.get_kinematics()
    toolhead.flush_step_generation()
    if at is None:
        kin_spos = {s.get_name(): s.get_commanded_position()
                    for s in toolhead_kin.get_steppers()}
    else:
        kin_spos = {s.get_name(): s.mcu_to_commanded_position(s.get_past_mcu_position(at))
                    for s in toolhead_kin.get_steppers()}
    return toolhead_kin.calc_position(kin_spos), 0

def get_past_toolhead_goal(printer, at):
    motion_report = printer.lookup_object("motion_report")
    dump_trapq = motion_report.trapqs.get("toolhead")
    if dump_trapq is None:
        raise printer.command_error("No dump trapq for toolhead")

    position, velocity = dump_trapq.get_trapq_position(at)
    if position is None:
        return None, None
    return list(position), velocity

def np_rolling_mean(data, window, center=True):
    half_window = (window - 1) // 2 if center else 0
    result = np.empty(len(data), dtype=float)

    for i in range(len(data)):
        start = max(0, i - half_window)
        end = min(len(data), i + half_window + 1)
        result[i] = np.mean(data[start:end])

    return result

def np_wma(series, window_size):
    weights = np.arange(1, window_size + 1)
    weight_sum = weights.sum()
    wma = []
    for i in range(len(series)):
        if i < window_size:
            wma.append(np.nan)  # Not enough data for WMA
        else:
            window = series[i - window_size:i]
            wma.append(np.dot(window, weights) / weight_sum)
    return wma

def np_rmse(p, x, y):
    y_hat = p(x)
    return np.sqrt(np.mean((y - y_hat)**2))

def log_traceback(limit=100):
    pass

def load_config_prefix(config: ConfigWrapper):
    return ProbeEddy(config)

def format_th_pos_list(l):
    return str.join(", ", [f"({v[0]:.2f}, {v[1]:.2f}, {v[2]:.3f})" for v in l])
