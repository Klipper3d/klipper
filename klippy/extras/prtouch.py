# Support for button detection and callbacks
#
# Copyright (C) 2022-12-09  CC <wangyulong878@sina.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from __future__ import annotations

import logging
import math
import random
import statistics
import time
from typing import Any, List, Optional, Tuple

from . import probe

logger = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

# Timing
HX711_CAPTURE_DELAY_SEC = 0.015
HX711_STATUS_POLL_SEC = 0.005

# Motion
PROBE_RETRY_Z_INCREMENT_MM = 2.0

# Nozzle cleaning
NOZZLE_CLEAR_FAN_SPEED_OFF = 0
NOZZLE_CLEAR_HOME_Z_OFFSET_MM = 10.0

# MCU
MCU_TICK_ROLLOVER = 2 ** 32  # 32-bit counter rollover at 4294967296

# Status codes for algorithm phases
CLEAN_STATUS_OK = 0
CLEAN_STATUS_TEMP_TIMEOUT = 1
CLEAN_STATUS_MEASURE_FAILED = 2
CLEAN_STATUS_MOVE_FAILED = 3
CLEAN_STATUS_UNKNOWN_ERROR = 4

# Trigger detection thresholds
TRIGGER_SLOPE_THRESHOLD = 0.8


# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

class PRTouchCFG:
    """Configuration wrapper for the PRTouch extra module."""

    def __init__(self, config: Any) -> None:
        # Probe / Z-offset
        self.probe_name = config.get('probe_name', default='bltouch')
        self.probe_sample_count = config.getint('probe_sample_count', default=3, minval=1, maxval=10)
        self.z_offset_samples = config.getint('z_offset_samples', default=2, minval=2, maxval=10)
        self.z_offset_samples_speed = config.getfloat('z_offset_samples_speed', default=2.5, minval=0.1, maxval=10)
        self.bed_max_err = config.getint('bed_max_err', default=4, minval=2, maxval=10)
        self.check_bed_mesh_max_err = config.getfloat('check_bed_mesh_max_err', default=0.2, minval=0.01, maxval=1)

        # Probe motion speeds
        self.probe_xy_travel_speed = config.getfloat('probe_xy_travel_speed', default=150, minval=60, maxval=250)
        self.probe_approach_speed = config.getfloat('probe_approach_speed', default=2.5, minval=0.1, maxval=10)

        # Sensor / calibration
        self.tare_samples = config.getint('tare_samples', default=40, minval=10, maxval=100)
        self.filter_window = config.getint('filter_window', default=32, minval=16, maxval=128)
        self.min_hold = config.getint('min_hold', default=3000, minval=100, maxval=50000)
        self.max_hold = config.getint('max_hold', default=50000, minval=100, maxval=100000)
        self.sensor_x = config.getfloat('sensor_x', minval=0, maxval=300)
        self.sensor_y = config.getfloat('sensor_y', minval=0, maxval=300)
        self.sensor_random_offset = config.getfloat('sensor_random_offset', default=5, minval=0, maxval=10)

        # Temperature limits
        self.nozzle_clean_min_temp = config.getfloat('s_nozzle_clean_min_temp', default=140, minval=80, maxval=200)
        self.nozzle_clean_max_temp = config.getfloat('s_nozzle_clean_max_temp', default=200, minval=180, maxval=300)
        self.bed_max_temp = config.getfloat('s_bed_max_temp', default=60, minval=45, maxval=100)

        # Nozzle cleaning algorithm
        self.nozzle_clean_main_temp = config.getfloat('nozzle_clean_main_temp', default=180, minval=150, maxval=250)
        self.nozzle_clean_cool_target_temp = config.getfloat('nozzle_clean_cool_target_temp', default=140, minval=80, maxval=200)
        self.nozzle_clean_safe_z_height = config.getfloat('nozzle_clean_safe_z_height', default=10.0, minval=1.0, maxval=50.0)
        self.nozzle_clean_working_height = config.getfloat('nozzle_clean_working_height', default=0.0, minval=-1.0, maxval=5.0)
        self.nozzle_clean_temp_poll_interval_sec = config.getfloat('nozzle_clean_temp_poll_interval_sec', default=0.2, minval=0.05, maxval=1.0)
        self.nozzle_clean_cool_fan_speed = config.getint('nozzle_clean_cool_fan_speed', default=255, minval=0, maxval=255)

        # Nozzle cleaning geometry
        self.pa_clr_dis_mm = config.getint('pa_clr_dis_mm', default=5, minval=2, maxval=100)
        self.pa_clr_down_mm = config.getfloat('pa_clr_down_mm', default=-0.1, minval=-1, maxval=1)
        self.nozzle_clean_start_pos_x = config.getfloat('nozzle_clean_start_pos_x', default=0, minval=-8, maxval=220)
        self.nozzle_clean_start_pos_y = config.getfloat('nozzle_clean_start_pos_y', default=0, minval=-5, maxval=220)
        self.nozzle_clean_len_x = config.getfloat('nozzle_clean_len_x', default=0, minval=0, maxval=1000)
        self.nozzle_clean_len_y = config.getfloat('nozzle_clean_len_y', default=0, minval=self.pa_clr_dis_mm + 5, maxval=1000)
        self.nozzle_clean_speed = config.getfloat('nozzle_clean_speed', default=30, minval=10, maxval=100)
        self.nozzle_clean_home_z_offset_mm = config.getfloat('nozzle_clean_home_z_offset_mm', default=NOZZLE_CLEAR_HOME_Z_OFFSET_MM, minval=1.0, maxval=50.0)
        self.wipe_retract_distance = config.getfloat('wipe_retract_distance', default=0, minval=0, maxval=50)

        # Machine limits
        self.max_z = config.getsection('stepper_z').getfloat('position_max', default=300, minval=100, maxval=500)

        # Debug
        self.show_msg = config.getboolean('show_msg', default=False)

        # Stored profiles
        stored = config.get_prefix_sections('prtouch')
        self.stored_profs = stored[1] if len(stored) == 2 else None


# ---------------------------------------------------------------------------
# Runtime state
# ---------------------------------------------------------------------------

class PRTouchState:
    """Mutable runtime state for PRTouch operations."""

    def __init__(self) -> None:
        self.out_index: int = 0
        self.out_val_mm: float = 0.0
        self.re_probe_cnt: int = 0
        self.home_xy: Optional[List[float]] = None


# ---------------------------------------------------------------------------
# Klipper object references
# ---------------------------------------------------------------------------

class PRTouchOBJ:
    """Klipper object references required by the PRTouch module."""

    def __init__(self, config: Any) -> None:
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.lookup_object('gcode_move')

        # Resolved after MCU identify
        self.toolhead: Optional[Any] = None
        self.hx711s: Optional[Any] = None
        self.pheaters: Optional[Any] = None
        self.heater_hot: Optional[Any] = None
        self.heater_bed: Optional[Any] = None
        self.bed_mesh: Optional[Any] = None
        self.mcu: Optional[Any] = None
        self.dirzctl: Optional[Any] = None
        self.kin: Optional[Any] = None
        self.probe: Optional[Any] = None
        self.filter: Optional[Any] = None

    def resolve(self) -> None:
        """Resolve all Klipper objects after MCU identification."""
        self.toolhead = self.printer.lookup_object('toolhead')
        self.hx711s = self.printer.lookup_object('hx711s')
        self.pheaters = self.printer.lookup_object('heaters')
        self.heater_hot = self.printer.lookup_object('extruder').heater
        self.heater_bed = self.printer.lookup_object('heater_bed').heater
        self.bed_mesh = self.printer.lookup_object('bed_mesh')
        self.dirzctl = self.printer.lookup_object('dirzctl')
        self.mcu = self.hx711s.mcu
        self.filter = self.printer.lookup_object('filter')
        self.kin = self.toolhead.get_kinematics()
        self.probe = self.printer.lookup_object('probe', None)


# ---------------------------------------------------------------------------
# Main wrapper
# ---------------------------------------------------------------------------

class PRTouchZOffsetWrapper:

    def __init__(self, config: Any) -> None:
        self.cfg = PRTouchCFG(config)
        self.val = PRTouchState()
        self.obj = PRTouchOBJ(config)

        self.obj.printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self.obj.gcode.register_command('PRTOUCH_PROBE_ZOFFSET', self.cmd_PRTOUCH_PROBE_ZOFFSET, desc=self.cmd_PRTOUCH_PROBE_ZOFFSET_help)
        self.obj.gcode.register_command('NOZZLE_CLEAR', self.cmd_NOZZLE_CLEAR, desc=self.cmd_NOZZLE_CLEAR_help)
        self.obj.gcode.register_command('PRTOUCH_ACCURACY', self.cmd_PRTOUCH_ACCURACY, desc=self.cmd_PRTOUCH_ACCURACY_help)

    # ------------------------------------------------------------------
    # Initialisation
    # ------------------------------------------------------------------

    def _handle_mcu_identify(self) -> None:
        self.obj.resolve()
        min_x, min_y = self.obj.bed_mesh.bmc.mesh_min
        max_x, max_y = self.obj.bed_mesh.bmc.mesh_max
        self.val.home_xy = [(max_x - min_x) / 2 + min_x, (max_y - min_y) / 2 + min_y]

    # ------------------------------------------------------------------
    # Klipper interface helpers
    # ------------------------------------------------------------------

    def get_mcu(self) -> Any:
        return self.obj.mcu

    def get_steppers(self) -> Any:
        return self.obj.dirzctl.steppers

    def _is_system_ok(self) -> bool:
        """Return True when neither HX711 nor dirzctl reports a fault."""
        return (
            not self.obj.hx711s.is_shutdown
            and not self.obj.hx711s.is_timeout
            and not self.obj.dirzctl.is_shutdown
            and not self.obj.dirzctl.is_timeout
        )

    def _ensure_homed(self) -> None:
        """Run a full home if any axis reports as unhomed."""
        if self.obj.kin is None:
            self.obj.resolve()
        if self.obj.kin is None:
            return
        for axis_index in range(3):
            axis_min, axis_max = self.obj.kin.limits[axis_index]
            if axis_min > axis_max:
                self.obj.gcode.run_script_from_command('G28')
                break

    # ------------------------------------------------------------------
    # Motion helpers
    # ------------------------------------------------------------------

    def _execute_move(self, pos: List[float], speed: float, wait: bool = True) -> None:
        """Issue a G1 move and optionally wait for completion."""
        if not self._is_system_ok():
            return
        if len(pos) >= 3:
            cmd = 'G1 F%d X%.3f Y%.3f Z%.3f' % (speed * 60, pos[0], pos[1], pos[2])
        else:
            cmd = 'G1 F%d X%.3f Y%.3f' % (speed * 60, pos[0], pos[1])
        self.obj.gcode.run_script_from_command(cmd)
        if wait:
            self.obj.toolhead.wait_moves()

    def _interpolate_linear_z(
        self,
        p1: List[float],
        p2: List[float],
        po: List[float],
        is_base_x: bool,
    ) -> List[float]:
        """Linearly interpolate Z at point *po* between *p1* and *p2*."""
        axis = 0 if is_base_x else 1
        if math.fabs(p1[axis] - p2[axis]) < 0.001:
            return po
        slope = (p2[2] - p1[2]) / (p2[axis] - p1[axis])
        intercept = p1[2] - p1[axis] * slope
        po[2] = slope * po[axis] + intercept
        return po

    # ------------------------------------------------------------------
    # Logging helpers
    # ------------------------------------------------------------------

    def _log_info(self, msg: str) -> None:
        logger.info(msg)
        if self.cfg.show_msg:
            self.obj.gcode.respond_info(msg)

    def _log_array(self, title: str, ary: List[float], lent: int = 32) -> None:
        logger.info('[%s] %s', title, str(ary))
        if self.cfg.show_msg:
            tail = ary[max(0, len(ary) - lent):]
            values = ', '.join('%.2f' % v for v in tail)
            self._log_info('%s [%s]' % (title, values))

    # ------------------------------------------------------------------
    # Temperature helpers
    # ------------------------------------------------------------------

    def _set_nozzle_temperature(self, temp: float, fan_spd: int, wait: bool = False, err: float = 5.0) -> None:
        self.obj.pheaters.set_temperature(self.obj.heater_hot, temp, False)
        self.obj.gcode.run_script_from_command('M106 P0 S%d' % fan_spd)
        self.obj.gcode.run_script_from_command('M106 P2 S%d' % fan_spd)
        if wait:
            while (
                self._is_system_ok()
                and abs(self.obj.heater_hot.target_temp - self.obj.heater_hot.smoothed_temp) > err
                and self.obj.heater_hot.target_temp > 0
            ):
                self.obj.hx711s.delay_s(0.1)

    def _set_bed_temperature(self, temp: float, wait: bool = False, err: float = 5.0) -> None:
        self.obj.pheaters.set_temperature(self.obj.heater_bed, temp, False)
        if wait:
            while (
                self._is_system_ok()
                and abs(self.obj.heater_bed.target_temp - self.obj.heater_bed.smoothed_temp) > err
                and self.obj.heater_bed.target_temp > 0
            ):
                self.obj.hx711s.delay_s(0.1)

    def _wait_for_temperature(
        self,
        heater: Any,
        target_temp: float,
        err_tolerance: float = 5.0
    ) -> bool:
        """Poll *heater* until it reaches *target_temp*."""

        poll_interval = self.cfg.nozzle_clean_temp_poll_interval_sec

        while self._is_system_ok():
            if (
                abs(target_temp - heater.smoothed_temp) <= err_tolerance
                and heater.target_temp > 0
            ):
                self._log_info(
                    'Temperature reached: %.1f°C ≈ %.1f°C'
                    % (heater.smoothed_temp, target_temp)
                )
                return True

            self.obj.hx711s.delay_s(poll_interval)

        return False

    # ------------------------------------------------------------------
    # Trigger detection
    # ------------------------------------------------------------------

    def _normalize(self, vals: List[float]) -> List[float]:
        """Return a min-max normalised copy of *vals*."""
        min_val = min(vals)
        max_val = max(vals)
        spread = (max_val - min_val) or 1.0
        return [(v - min_val) / spread for v in vals]

    def _remove_spikes(self, vals: List[float], max_hold: int) -> List[float]:
        """Replace isolated spikes above *max_hold* with the previous value."""
        result = list(vals)
        for i in range(1, len(result) - 1):
            if result[i] >= max_hold and result[i - 1] < (max_hold / 2) and result[i + 1] < (max_hold / 2):
                result[i] = result[i - 1]
        return result

    def _rotate_by_trend(self, vals: List[float]) -> Tuple[List[float], int]:
        """Detrend *vals* by rotating against the overall slope; return rotated values and the index of the minimum."""
        angle = math.atan((vals[-1] - vals[0]) / len(vals))
        sin_a = math.sin(-angle)
        cos_a = math.cos(-angle)
        rotated = [i * sin_a + v * cos_a for i, v in enumerate(vals)]
        return rotated, rotated.index(min(rotated))

    def _detect_trigger(
        self,
        arg_index: int,
        fit_vals: List[float],
        _unfit_vals: List[float],
        min_hold: int,
        max_hold: int,
    ) -> bool:
        """Return True when the sensor signal indicates a probe trigger."""
        all_params, _tick = self.obj.dirzctl.get_params()

        # Dirzctl already stopped — counts as trigger
        if len(all_params) == 2:
            if self.cfg.show_msg:
                self._log_info('TRI SUCCESS BY: Tri by Dirzctl run over!')
                self._log_array('TRI CH=%d ARY=' % arg_index, fit_vals)
            return True

        window = self.cfg.filter_window

        # Early exit: raw values saturate max_hold
        if (
            len(fit_vals) >= window // 2
            and math.fabs(fit_vals[-1]) >= max_hold
            and math.fabs(fit_vals[-2]) >= max_hold
            and math.fabs(fit_vals[-3]) >= max_hold
        ):
            if self.cfg.show_msg:
                self._log_info('TRI SUCCESS BY: Tri by out max_hold!')
                self._log_array('TRI CH=%d ARY=' % arg_index, fit_vals)
            self.val.out_index = window - 1
            return True

        if len(fit_vals) != window:
            return False

        cleaned = self._remove_spikes(fit_vals, max_hold)

        # Detrend and find minimum index
        norm_rotated, min_idx = self._rotate_by_trend(self._normalize(cleaned))
        self.val.out_index = min_idx

        # Smooth values after the minimum using the filter coefficient
        if min_idx > 0:
            k = self.obj.filter.lft_k1_oft / 2
            for i in range(min_idx, window):
                cleaned[i] = cleaned[i] * k + cleaned[i - 1] * (1 - k)

        # Monotone rising tail check
        if not (cleaned[-1] > cleaned[-2] > cleaned[-3]):
            return False

        # Tail must exceed all earlier values
        tail_max = max(cleaned[-1], cleaned[-2], cleaned[-3])
        if tail_max <= max(cleaned[: window - 3]):
            return False

        # Slope uniformity check on normalised values
        norm = self._normalize(cleaned)
        for i in range(window - 1):
            if (norm[-1] - norm[i]) / ((window - i) / window) < TRIGGER_SLOPE_THRESHOLD:
                return False

        # Absolute hold thresholds
        if fit_vals[-1] < min_hold or fit_vals[-2] < (min_hold / 2) or fit_vals[-3] < (min_hold / 3):
            return False

        if self.cfg.show_msg:
            self._log_info('TRI SUCCESS BY: Tri by fit all rule!')
            self._log_array('TRI CH=%d ARY=' % arg_index, norm_rotated)
        return True

    # ------------------------------------------------------------------
    # Z measurement
    # ------------------------------------------------------------------

    def _calculate_min_z(self, start_z: float, hx711_vals: List[float]) -> Tuple[int, int, bool]:
        hx711_params, hx711_start_tick = self.obj.hx711s.get_params()
        dirzctl_params, dirzctl_start_tick = self.obj.dirzctl.get_params()

        if dirzctl_params is None or len(dirzctl_params) != 2:
            raise self.obj.printer.command_error(
                '{"code":"key502", "msg":"probe_by_step: Can not recv stepper-z status."}'
            )

        up_all_cnt = dirzctl_params[0]['step'] - dirzctl_params[1]['step'] + 1
        if len(hx711_vals) < self.cfg.filter_window or len(hx711_params) < self.cfg.filter_window:
            return up_all_cnt, up_all_cnt, False

        hx711_params = hx711_params[-self.cfg.filter_window:]

        # Detrend sensor values and find minimum index
        norm_vals = self._normalize(hx711_vals)
        _rotated, self.val.out_index = self._rotate_by_trend(norm_vals)

        # Convert dirzctl ticks to seconds and map to Z positions
        def tick_to_seconds(tick: int, ref: int, freq: float) -> float:
            rollover = MCU_TICK_ROLLOVER if tick < ref else 0
            return (rollover + tick - ref) / freq

        dirzctl_params[0]['tick'] = tick_to_seconds(dirzctl_params[0]['tick'], dirzctl_start_tick, self.obj.dirzctl.mcu_freq)
        dirzctl_params[1]['tick'] = tick_to_seconds(dirzctl_params[1]['tick'], dirzctl_start_tick, self.obj.dirzctl.mcu_freq)
        dirzctl_params[0]['z'] = start_z
        step_dist = self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base
        dirzctl_params[1]['z'] = start_z - up_all_cnt * step_dist

        trigger_tick = tick_to_seconds(
            hx711_params[self.val.out_index]['nt'],
            hx711_start_tick,
            self.obj.hx711s.mcu_freq,
        )
        self.val.out_val_mm = self._interpolate_linear_z(
            [dirzctl_params[0]['tick'], 0, dirzctl_params[0]['z']],
            [dirzctl_params[1]['tick'], 0, dirzctl_params[1]['z']],
            [trigger_tick, 0, 0],
            True,
        )[2]

        self._log_info(
            'call_min_z, re_probe_cnt=%d, out_index=%d, out_val_mm=%.2f'
            % (self.val.re_probe_cnt, self.val.out_index, self.val.out_val_mm)
        )

        limt_up_cnt = int(10 / step_dist)
        up_min_cnt = min(
            max(int((self.val.out_val_mm - dirzctl_params[1]['z']) / step_dist), 0),
            limt_up_cnt,
        )
        up_all_cnt = min(up_all_cnt, limt_up_cnt)
        return up_min_cnt, up_all_cnt, True

    def _probe_step(
        self,
        rdy_pos: List[float],
        speed_mm: float,
        min_dis_mm: float,
        min_hold: int,
        max_hold: int,
        up_after: bool = True,
    ) -> Tuple[int, float, bool]:
        """Descend, detect trigger, and optionally retract."""
        self.obj.hx711s.read_base(self.cfg.tare_samples // 2, max_hold)

        step_dist = self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base
        step_cnt = int(min_dis_mm / step_dist)
        step_us = int((min_dis_mm / speed_mm * 1_000_000) / step_cnt)

        self.obj.hx711s.query_start(self.cfg.filter_window * 2, 65535, del_dirty=True, show_msg=False, is_ck_con=True)
        self.obj.dirzctl.check_and_run(0, step_us, step_cnt, wait_finish=False, is_ck_con=True)
        self.obj.hx711s.delay_s(HX711_CAPTURE_DELAY_SEC)

        self._log_info(
            'PROBE_BY_STEP x=%.2f y=%.2f z=%.2f speed_mm=%.2f step_us=%d step_cnt=%d'
            % (rdy_pos[0], rdy_pos[1], rdy_pos[2], speed_mm, step_us, step_cnt)
        )

        while self._is_system_ok():
            self.obj.hx711s.send_heart_beat()
            self.obj.dirzctl.send_heart_beat()
            all_valss = self.obj.hx711s.get_vals()

            if not all_valss or len(all_valss[0]) == 0:
                self.obj.hx711s.delay_s(HX711_STATUS_POLL_SEC)
                continue

            unfit_vals, tmp_unfit_vals = self.obj.filter.cal_offset_by_vals(
                self.obj.hx711s.s_count, all_valss, self.obj.filter.lft_k1_oft, self.cfg.filter_window
            )
            _fit_vals, tmp_fit_vals = self.obj.filter.cal_filter_by_vals(
                self.obj.hx711s.s_count, all_valss, self.obj.filter.hft_hz, self.obj.filter.lft_k1, self.cfg.filter_window
            )

            for i in range(self.obj.hx711s.s_count):
                if not self._detect_trigger(i, tmp_fit_vals[i], tmp_unfit_vals[i], min_hold, max_hold):
                    continue

                # Stop motion and capture final sensor data
                self.obj.dirzctl.check_and_run(0, 0, 0, wait_finish=False)
                self.obj.hx711s.query_start(self.cfg.filter_window * 2, 0, del_dirty=False, show_msg=False)
                self.obj.hx711s.delay_s(HX711_CAPTURE_DELAY_SEC)

                for j in range(self.obj.hx711s.s_count):
                    self._log_array('TRIGGER_USE_CH=%d, FIT_VALS=' % j, tmp_fit_vals[j], 16)

                self.obj.hx711s.delay_s(0.2)
                all_valss = self.obj.hx711s.get_vals()
                self._log_array('WAIT_AND_CAL_CH=%d, ARY=' % i, all_valss[i])

                _hx711_vals, tmp_hx711_vs = self.obj.filter.cal_filter_by_vals(
                    self.obj.hx711s.s_count, all_valss, self.obj.filter.hft_hz, self.obj.filter.lft_k1_cal, self.cfg.filter_window
                )
                self._log_array('WAIT_AND_CAL_CH=%d, ARY=' % i, tmp_hx711_vs[i])

                up_min_cnt, up_all_cnt, deal_sta = self._calculate_min_z(rdy_pos[2], tmp_hx711_vs[i])
                if up_after:
                    self.obj.dirzctl.check_and_run(1, step_us // 2, up_all_cnt)
                return self.val.out_index, self.val.out_val_mm, deal_sta

            self.obj.hx711s.delay_s(HX711_STATUS_POLL_SEC)

        return self.val.out_index, self.val.out_val_mm, True

    def _measure_probe_position(
        self,
        max_times: int,
        rdy_pos: List[float],
        speed_mm: float,
        min_dis_mm: float,
        max_z_err: float,
        min_hold: int,
        max_hold: int,
    ) -> float:
        """Probe a position repeatedly and return a stable Z measurement.

        Returns the averaged Z value, or 0 if no measurement could be obtained.
        """
        fallback_values: List[float] = []
        retry_z = float(rdy_pos[2])

        self._execute_move(self.obj.toolhead.get_position()[:2] + [rdy_pos[2]], self.cfg.probe_approach_speed)
        self._execute_move(rdy_pos, self.cfg.probe_xy_travel_speed)

        for attempt in range(1, max_times + 1):
            probe_results: List[float] = []
            retry_needed = False

            for _ in range(self.cfg.z_offset_samples):
                _index, measured_z, success = self._probe_step(
                    rdy_pos[:2] + [retry_z], speed_mm, min_dis_mm, min_hold, max_hold, True
                )
                if not success:
                    retry_z = retry_z + PROBE_RETRY_Z_INCREMENT_MM if retry_z == rdy_pos[2] else retry_z
                    retry_needed = True
                    break
                probe_results.append(measured_z)

            if retry_needed or not probe_results:
                self._log_info('_measure_probe_position: probe failed on attempt %d, retrying' % attempt)
                continue

            fallback_values = probe_results
            avg_z = statistics.mean(probe_results)
            spread = max(probe_results) - min(probe_results)
            self._log_info(
                '_measure_probe_position: attempt %d values=%s avg=%.3f spread=%.3f'
                % (attempt, probe_results, avg_z, spread)
            )

            if spread <= max_z_err or not self._is_system_ok():
                self._log_info('_measure_probe_position: success, final=%.3f' % avg_z)
                return avg_z

            self.val.re_probe_cnt += 1
            self._log_info(
                '_measure_probe_position: spread too high (%.3f > %.3f), reprobing' % (spread, max_z_err)
            )

        if fallback_values:
            fallback_avg = statistics.mean(fallback_values)
            self._log_info('_measure_probe_position: using fallback value=%.3f' % fallback_avg)
            return fallback_avg

        self._log_info('_measure_probe_position: failed to obtain any probe values')
        return 0.0

    # ------------------------------------------------------------------
    # Nozzle cleaning phases
    # ------------------------------------------------------------------

    def _measure_z_offset_point(self, position: List[float], label: str = '') -> Optional[float]:
        """Measure Z-offset at a single point; returns None on failure."""
        try:
            z_offset = self._measure_probe_position(
                max_times=1,
                rdy_pos=position,
                speed_mm=self.cfg.z_offset_samples_speed,
                min_dis_mm=10,
                max_z_err=self.cfg.check_bed_mesh_max_err,
                min_hold=self.cfg.min_hold,
                max_hold=self.cfg.max_hold,
            )
            if z_offset == 0.0:
                self._log_info('Z-offset measurement failed at %s: zero value' % label)
                return None
            self._log_info('Z-offset at %s: %.3f' % (label, z_offset))
            return z_offset
        except Exception as exc:
            self._log_info('Exception during Z-offset measurement at %s: %s' % (label, exc))
            return None

    def _phase_pre_measurement(self, _min_hold: int, _max_hold: int) -> Tuple[Optional[float], Optional[float], int]:
        """Phase 1 — measure Z-offset at wipe start and end positions."""
        self._log_info('=== Phase 1: Pre-measurement ===')
        z_start: Optional[float] = None
        z_end: Optional[float] = None
        try:
            safe_z = self.cfg.bed_max_err + 1
            cur = self.obj.toolhead.get_position()
            self._execute_move(cur[:2] + [safe_z], self.cfg.probe_xy_travel_speed)

            start_pos = [self.cfg.nozzle_clean_start_pos_x, self.cfg.nozzle_clean_start_pos_y, safe_z]
            self._execute_move(start_pos, self.cfg.probe_xy_travel_speed)
            z_start = self._measure_z_offset_point(start_pos, 'START')
            if z_start is None:
                return None, None, CLEAN_STATUS_MEASURE_FAILED

            end_pos = [
                self.cfg.nozzle_clean_start_pos_x + self.cfg.nozzle_clean_len_x,
                self.cfg.nozzle_clean_start_pos_y + self.cfg.nozzle_clean_len_y,
                safe_z,
            ]
            self._execute_move(end_pos, self.cfg.probe_xy_travel_speed)
            z_end = self._measure_z_offset_point(end_pos, 'END')
            if z_end is None:
                return z_start, None, CLEAN_STATUS_MEASURE_FAILED

            self._execute_move(end_pos[:2] + [self.cfg.nozzle_clean_safe_z_height], self.cfg.probe_xy_travel_speed)
            self._log_info('Pre-measurement complete: start=%.3f, end=%.3f' % (z_start, z_end))
            return z_start, z_end, CLEAN_STATUS_OK
        except Exception as exc:
            self._log_info('Phase 1 error: %s' % exc)
            return z_start, z_end, CLEAN_STATUS_UNKNOWN_ERROR

    def _phase_preparation(self, target_temp: float) -> bool:
        """Phase 2 — heat nozzle to main cleaning temperature."""
        self._log_info('=== Phase 2: Preparation ===')
        try:
            self._set_nozzle_temperature(temp=target_temp, fan_spd=NOZZLE_CLEAR_FAN_SPEED_OFF)
            if not self._wait_for_temperature(self.obj.heater_hot, target_temp):
                self._log_info('Failed to heat nozzle to %.1f°C' % target_temp)
                return False
            self._log_info('Nozzle heated to %.1f°C' % target_temp)
            return True
        except Exception as exc:
            self._log_info('Phase 2 error: %s' % exc)
            return False

    def _phase_cleaning_execution(self) -> bool:
        """Phase 3 — perform wipe motion and optional filament retraction."""
        self._log_info('=== Phase 3: Cleaning execution ===')
        try:
            safe_z = self.cfg.nozzle_clean_safe_z_height
            cur = self.obj.toolhead.get_position()
            self._execute_move(cur[:2] + [safe_z], self.cfg.probe_xy_travel_speed)

            start_pos = [self.cfg.nozzle_clean_start_pos_x, self.cfg.nozzle_clean_start_pos_y, safe_z]
            self._execute_move(start_pos, self.cfg.probe_xy_travel_speed)

            working_pos = start_pos[:2] + [self.cfg.nozzle_clean_working_height]
            self._execute_move(working_pos, self.cfg.probe_approach_speed)

            end_pos = [
                self.cfg.nozzle_clean_start_pos_x + self.cfg.nozzle_clean_len_x,
                self.cfg.nozzle_clean_start_pos_y + self.cfg.nozzle_clean_len_y,
                self.cfg.nozzle_clean_working_height,
            ]
            self._execute_move(end_pos, self.cfg.nozzle_clean_speed)
            self._execute_move(end_pos[:2] + [safe_z], self.cfg.probe_approach_speed)

            if self.cfg.wipe_retract_distance > 0:
                self.obj.gcode.run_script_from_command('G91')
                self.obj.gcode.run_script_from_command('G1 E-%.2f F600' % self.cfg.wipe_retract_distance)
                self.obj.gcode.run_script_from_command('G90')

            self._execute_move(end_pos[:2] + [self.cfg.nozzle_clean_home_z_offset_mm], self.cfg.probe_xy_travel_speed)
            self._log_info('Cleaning execution complete')
            return True
        except Exception as exc:
            self._log_info('Phase 3 error: %s' % exc)
            return False

    def _phase_cooling(self, target_temp: float) -> bool:
        """Phase 4 — cool nozzle with fan to target temperature."""
        self._log_info('=== Phase 4: Cooling ===')
        try:
            self._set_nozzle_temperature(temp=target_temp, fan_spd=self.cfg.nozzle_clean_cool_fan_speed)
            if not self._wait_for_temperature(self.obj.heater_hot, target_temp):
                self._log_info('Cooling timeout: nozzle did not cool to %.1f°C' % target_temp)
                return False
            self._log_info('Nozzle cooled to %.1f°C' % target_temp)
            self._set_nozzle_temperature(temp=target_temp, fan_spd=NOZZLE_CLEAR_FAN_SPEED_OFF)
            return True
        except Exception as exc:
            self._log_info('Phase 4 error: %s' % exc)
            return False

    # ------------------------------------------------------------------
    # Public operations
    # ------------------------------------------------------------------

    def clear_nozzle(
        self,
        nozzle_clean_min_temp: float,
        nozzle_clean_max_temp: float,
        bed_max_temp: float,
        min_hold: int,
        max_hold: int,
    ) -> None:
        """Execute the complete 4-phase nozzle cleaning procedure."""
        try:
            self._log_info('========== NOZZLE CLEANING START ==========')
            self._log_info(
                'Config: temp_range=[%.1f°C, %.1f°C], bed=%.1f°C'
                % (nozzle_clean_min_temp, nozzle_clean_max_temp, bed_max_temp)
            )
            self._ensure_homed()

            if bed_max_temp > 0:
                self._log_info('Heating bed to %.1f°C ...' % bed_max_temp)
                self._set_bed_temperature(temp=bed_max_temp, wait=True)

            self._log_info('Preheating nozzle to %.1f°C ...' % nozzle_clean_min_temp)
            self._set_nozzle_temperature(temp=nozzle_clean_min_temp, fan_spd=NOZZLE_CLEAR_FAN_SPEED_OFF)
            if not self._wait_for_temperature(self.obj.heater_hot, nozzle_clean_min_temp):
                self._log_info('Failed to preheat nozzle — aborting')
                return

            z_start, z_end, status = self._phase_pre_measurement(min_hold, max_hold)
            if status != CLEAN_STATUS_OK:
                self._log_info('Pre-measurement failed (status=%d) — aborting' % status)
                return
            self._log_info('Pre-measurement: start=%.3f, end=%.3f' % (z_start, z_end))

            if not self._phase_preparation(self.cfg.nozzle_clean_main_temp):
                self._log_info('Preparation phase failed — aborting')
                return

            if not self._phase_cleaning_execution():
                self._log_info('Cleaning execution failed — aborting')
                return

            if not self._phase_cooling(self.cfg.nozzle_clean_cool_target_temp):
                self._log_info('Cooling phase completed with warnings')

            self._log_info('========== NOZZLE CLEANING COMPLETE ==========')
        except Exception as exc:
            import traceback
            self._log_info('CRITICAL ERROR in clear_nozzle: %s\n%s' % (exc, traceback.format_exc()))

    def probe_z_offset(self, x: float, y: float) -> float:
        """Return the Z-offset measured at (x, y)."""
        self._ensure_homed()
        return self._measure_probe_position(
            max_times=3,
            rdy_pos=[x, y, self.cfg.bed_max_err + 1.0],
            speed_mm=self.cfg.z_offset_samples_speed,
            min_dis_mm=10,
            max_z_err=self.cfg.check_bed_mesh_max_err,
            min_hold=self.cfg.min_hold,
            max_hold=self.cfg.max_hold,
        )

    def _finalize_probe_calibration(self, kin_pos: Optional[List[float]]) -> None:
        if kin_pos is None:
            return
        z_offset = -kin_pos[2]
        self.obj.gcode.respond_info(
            '%s: z_offset: %.3f\n'
            'The SAVE_CONFIG command will update the printer config file\n'
            'with the above and restart the printer.' % (self.cfg.probe_name, z_offset)
        )
        configfile = self.obj.printer.lookup_object('configfile')
        configfile.set(self.cfg.probe_name, 'z_offset', '%.3f' % z_offset)

    # ------------------------------------------------------------------
    # G-code commands
    # ------------------------------------------------------------------

    cmd_NOZZLE_CLEAR_help = 'Clear the nozzle on bed.'

    def cmd_NOZZLE_CLEAR(self, gcmd: Any) -> None:
        self.clear_nozzle(
            nozzle_clean_min_temp=gcmd.get_float('nozzle_clean_min_temp', self.cfg.nozzle_clean_min_temp),
            nozzle_clean_max_temp=gcmd.get_float('nozzle_clean_max_temp', self.cfg.nozzle_clean_max_temp),
            bed_max_temp=gcmd.get_float('BED_MAX_TEMP', self.cfg.bed_max_temp),
            min_hold=gcmd.get_int('MIN_HOLD', self.cfg.min_hold),
            max_hold=gcmd.get_int('MAX_HOLD', self.cfg.max_hold),
        )

    cmd_PRTOUCH_PROBE_ZOFFSET_help = 'Probe the z-offset.'

    def cmd_PRTOUCH_PROBE_ZOFFSET(self, gcmd: Any) -> None:
        self._ensure_homed()

        homing_origin = self.obj.gcode_move.get_status()['homing_origin']
        self._log_array('homing_origin:', homing_origin, len(homing_origin))

        start_z_offset = self.obj.probe.get_offsets()[2]
        self._log_info('Start z_offset: %.3f' % start_z_offset)

        if gcmd.get_int('CLEAR_NOZZLE', 0) == 1:
            self.clear_nozzle(
                self.cfg.nozzle_clean_min_temp,
                self.cfg.nozzle_clean_max_temp,
                self.cfg.bed_max_temp,
                self.cfg.min_hold,
                self.cfg.max_hold,
            )

        self._ensure_homed()

        x = self.cfg.sensor_x + random.uniform(-self.cfg.sensor_random_offset, self.cfg.sensor_random_offset)
        y = self.cfg.sensor_y + random.uniform(-self.cfg.sensor_random_offset, self.cfg.sensor_random_offset)
        probe_x_offset, probe_y_offset = self.obj.probe.get_offsets()[:2]
        probe_x = x - probe_x_offset
        probe_y = y - probe_y_offset

        self._log_info('Checking z-position of probe (%.2f, %.2f)' % (probe_x, probe_y))
        self._execute_move([probe_x, probe_y, self.cfg.bed_max_err + 1.0], self.cfg.probe_xy_travel_speed)

        probe_gcmd = self.obj.gcode.create_gcode_command('PROBE', 'PROBE', {'SAMPLES': self.cfg.probe_sample_count})
        z_probe = probe.run_single_probe(self.obj.probe, probe_gcmd)
        self._log_info('Probe at sensor: %.3f' % z_probe[2])

        nozzle_z_offset = self.probe_z_offset(x, y)
        self._log_info('Nozzle z_offset: %.3f' % nozzle_z_offset)

        z_offset = nozzle_z_offset - z_probe[2]
        self._log_info('Calculated z_offset: %.3f' % z_offset)

        z_adjust = start_z_offset - z_offset
        self._log_info('z_adjust: %.3f' % z_adjust)

        if gcmd.get_int('APPLY_Z_ADJUST', 0) == 1:
            self.obj.gcode.run_script_from_command('SET_GCODE_OFFSET Z_ADJUST=%f MOVE=1' % z_adjust)

        prtouch_result = list(z_probe)
        prtouch_result[2] = -z_adjust
        self._finalize_probe_calibration(prtouch_result)

    cmd_PRTOUCH_ACCURACY_help = 'Probe Z-height accuracy at sensor position.'

    def cmd_PRTOUCH_ACCURACY(self, gcmd: Any) -> None:
        self._ensure_homed()
        speed = gcmd.get_float('z_offset_samples_speed', self.cfg.z_offset_samples_speed, above=0.0)
        sample_count = gcmd.get_int('SAMPLES', 10, minval=1)
        gcmd.respond_info(
            'PRTOUCH_ACCURACY at X:%.3f Y:%.3f (samples=%d speed=%.1f)'
            % (self.cfg.sensor_x, self.cfg.sensor_y, sample_count, speed)
        )

        sensor_pos = [self.cfg.sensor_x, self.cfg.sensor_y, self.cfg.bed_max_err]
        self._execute_move(sensor_pos, self.cfg.probe_xy_travel_speed)

        positions: List[float] = []
        while len(positions) < sample_count:
            _index, pos, _sta = self._probe_step(sensor_pos, speed, 10, self.cfg.min_hold, self.cfg.max_hold, True)
            positions.append(pos)
            gcmd.respond_info('probe #%d at (%.3f, %.3f): %.3f' % (len(positions), sensor_pos[0], sensor_pos[1], pos))

        avg = statistics.mean(positions)
        median = statistics.median(positions)
        sigma = statistics.pstdev(positions)
        gcmd.respond_info(
            'probe accuracy results: maximum %.6f, minimum %.6f, range %.6f, '
            'average %.6f, median %.6f, standard deviation %.6f'
            % (max(positions), min(positions), max(positions) - min(positions), avg, median, sigma)
        )


def load_config(config: Any) -> PRTouchZOffsetWrapper:
    return PRTouchZOffsetWrapper(config)


# PRTOUCH_PROBE_ZOFFSET CLEAR_NOZZLE=1