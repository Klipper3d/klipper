# PRTouch Z-Offset Probe — Klipper plugin
#
# Detects nozzle contact via HX711 load-cell readings and
# correlates them with stepper-Z position to compute Z-offset.
#
# Copyright (C) 2022-12-09  CC <wangyulong878@sina.com>
# Distributed under the GNU GPLv3 license.

import logging
import math
import random
import time

from . import probe

log = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

_TICK_OVERFLOW   = 4_294_967_296   # 32-bit MCU tick rollover
_LIMT_UP_MM      = 10.0            # maximum safe upward-travel after trigger
_PROBE_SAMPLES   = 2               # default probe samples for Z-reference
_REPROBE_Z_BUMP  = 2.0             # mm to raise rdy_pos when first probe misses


# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

class PRTouchConfig:
    """Reads and validates all prtouch config values once at startup."""

    def __init__(self, config):
        gi  = config.getint
        gf  = config.getfloat
        gb  = config.getboolean
        gs  = config.get

        self.base_count   = gi('base_count',  default=40,   minval=10,  maxval=100)
        self.pi_count     = gi('pi_count',    default=32,   minval=16,  maxval=128)
        self.min_hold     = gi('min_hold',    default=3000, minval=100, maxval=50_000)
        self.max_hold     = gi('max_hold',    default=50_000, minval=100, maxval=100_000)

        self.sensor_x     = gf('sensor_x',   minval=0, maxval=300)
        self.sensor_y     = gf('sensor_y',   minval=0, maxval=300)
        self.random_offset = gf('sensor_random_offset', default=5, minval=0, maxval=10)

        self.hot_min_temp = gf('s_hot_min_temp', default=140, minval=80,  maxval=200)
        self.hot_max_temp = gf('s_hot_max_temp', default=200, minval=170, maxval=300)
        self.bed_max_temp = gf('s_bed_max_temp', default=60,  minval=45,  maxval=100)

        self.pa_clr_dis_mm   = gi('pa_clr_dis_mm',  default=5,    minval=2, maxval=100)
        self.pa_clr_down_mm  = gf('pa_clr_down_mm', default=-0.1, minval=-1, maxval=1)

        self.clr_noz_start_x    = gf('clr_noz_start_x',    default=0, minval=-5, maxval=1000)
        self.clr_noz_start_y    = gf('clr_noz_start_y',    default=0, minval=-5, maxval=1000)
        self.clr_noz_len_x      = gf('clr_noz_len_x',      default=0, minval=0, maxval=1000)
        self.clr_noz_len_y      = gf('clr_noz_len_y',      default=0,
                                      minval=self.pa_clr_dis_mm + 5, maxval=1000)
        self.clr_noz_wipe_speed = gf('clr_noz_wipe_speed', default=10, minval=10, maxval=50)

        self.bed_max_err              = gi('bed_max_err',              default=4,   minval=2,    maxval=10)
        self.max_z                    = config.getsection('stepper_z').getfloat(
                                            'position_max', default=300, minval=100, maxval=500)
        self.g29_xy_speed             = gf('g29_xy_speed',             default=150, minval=10,   maxval=1000)
        self.g29_rdy_speed            = gf('g29_rdy_speed',            default=2.5, minval=0.1,  maxval=10)
        self.probe_speed              = gf('probe_speed',              default=2.0, minval=0.1,  maxval=10)
        self.check_bed_mesh_max_err   = gf('check_bed_mesh_max_err',   default=0.2, minval=0.01, maxval=1)
        self.wipe_retract_distance    = gf('wipe_retract_distance',    default=0,   minval=0,    maxval=50)
        self.show_msg                 = gb('show_msg', default=False)
        self.probe_name               = gs('probe_name', default='bltouch')

        stored = config.get_prefix_sections('prtouch')
        self.stored_profs = stored[1] if len(stored) == 2 else None


# ---------------------------------------------------------------------------
# Probe state (reset between operations)
# ---------------------------------------------------------------------------

class PRTouchState:
    """Mutable probe state; reset explicitly before each probe sequence."""

    def __init__(self):
        self.reset()

    def reset(self):
        self.out_index:    int   = 0
        self.out_val_mm:   float = 0.0
        self.re_probe_cnt: int   = 0
        self.home_xy:      list  = None


# ---------------------------------------------------------------------------
# Hardware object cache
# ---------------------------------------------------------------------------

class PRTouchObjects:
    """Lazy references to Klipper subsystems, populated at mcu_identify."""

    def __init__(self, printer):
        self.printer    = printer
        self.gcode      = printer.lookup_object('gcode')
        self.gcode_move = printer.lookup_object('gcode_move')
        # Filled by find_objects():
        self.toolhead   = None
        self.hx711s     = None
        self.pheaters   = None
        self.heater_hot = None
        self.heater_bed = None
        self.bed_mesh   = None
        self.dirzctl    = None
        self.filter     = None
        self.mcu        = None
        self.kin        = None
        self.probe      = None

    def find_objects(self):
        lkp = self.printer.lookup_object
        self.toolhead   = lkp('toolhead')
        self.hx711s     = lkp('hx711s')
        self.pheaters   = lkp('heaters')
        self.heater_hot = lkp('extruder').heater
        self.heater_bed = lkp('heater_bed').heater
        self.bed_mesh   = lkp('bed_mesh')
        self.dirzctl    = lkp('dirzctl')
        self.filter     = lkp('filter')
        self.mcu        = self.hx711s.mcu
        self.kin        = self.toolhead.get_kinematics()
        self.probe      = lkp('probe', None)


# ---------------------------------------------------------------------------
# Main plugin class
# ---------------------------------------------------------------------------

class PRTouchZOffsetWrapper:
    """
    Klipper plugin that measures nozzle Z-offset using load-cell feedback.

    Exposed G-code commands:
        PRTOUCH_PROBE_ZOFFSET  — measure and (optionally) apply Z-offset
        NOZZLE_CLEAR           — heat-clean the nozzle tip on the wipe pad
        PRTOUCH_ACCURACY       — repeatability test at the sensor position
    """

    def __init__(self, config):
        self.cfg   = PRTouchConfig(config)
        self.state = PRTouchState()
        self.obj   = PRTouchObjects(config.get_printer())

        self.obj.printer.register_event_handler(
            'klippy:mcu_identify', self._handle_mcu_identify)

        cmds = [
            ('PRTOUCH_PROBE_ZOFFSET', self.cmd_PRTOUCH_PROBE_ZOFFSET,
             "Probe the Z-offset between nozzle and bed sensor"),
            ('NOZZLE_CLEAR',          self.cmd_NOZZLE_CLEAR,
             "Heat-wipe the nozzle on the cleaning pad"),
            ('PRTOUCH_ACCURACY',      self.cmd_PRTOUCH_ACCURACY,
             "Probe Z-height accuracy at the sensor position"),
        ]
        for name, fn, desc in cmds:
            self.obj.gcode.register_command(name, fn, desc=desc)

    # ------------------------------------------------------------------
    # Klipper event handlers
    # ------------------------------------------------------------------

    def _handle_mcu_identify(self):
        self.obj.find_objects()
        bmc = self.obj.bed_mesh.bmc
        min_x, min_y = bmc.mesh_min
        max_x, max_y = bmc.mesh_max
        self.state.home_xy = [
            (min_x + max_x) / 2.0,
            (min_y + max_y) / 2.0,
        ]

    # ------------------------------------------------------------------
    # Klipper interface helpers
    # ------------------------------------------------------------------

    def get_mcu(self):
        return self.obj.mcu

    def get_steppers(self):
        return self.obj.dirzctl.steppers

    # ------------------------------------------------------------------
    # Status helpers
    # ------------------------------------------------------------------

    def _system_ok(self) -> bool:
        """Return True when neither HX711 nor dirzctl has faulted."""
        hx  = self.obj.hx711s
        drz = self.obj.dirzctl
        return not (hx.is_shutdown or hx.is_timeout
                    or drz.is_shutdown or drz.is_timeout)

    def _ensure_homed(self):
        """Home all axes if any axis limit is uninitialised."""
        limits = self.obj.kin.limits
        if any(limits[i][0] > limits[i][1] for i in range(3)):
            self.obj.gcode.run_script_from_command('G28')

    # ------------------------------------------------------------------
    # Motion
    # ------------------------------------------------------------------

    def _move(self, pos, speed, wait=True):
        """Issue a G1 move and optionally block until complete."""
        if not self._system_ok():
            return
        if len(pos) >= 3:
            cmd = 'G1 F%d X%.3f Y%.3f Z%.3f' % (speed * 60, pos[0], pos[1], pos[2])
        else:
            cmd = 'G1 F%d X%.3f Y%.3f'        % (speed * 60, pos[0], pos[1])
        self.obj.gcode.run_script_from_command(cmd)
        if wait:
            self.obj.toolhead.wait_moves()

    # ------------------------------------------------------------------
    # Maths / signal processing
    # ------------------------------------------------------------------

    @staticmethod
    def _linear_interp(p1, p2, po, use_x_axis: bool) -> list:
        """
        Linearly interpolate po[2] from two reference points along one axis.

        p1, p2, po are [axis_a, axis_b, z].  use_x_axis selects axis_a vs axis_b.
        Returns po unchanged when the two reference points share the same axis value.
        """
        axis = 0 if use_x_axis else 1
        delta = p2[axis] - p1[axis]
        if math.fabs(delta) < 1e-3:
            return po
        a = (p2[2] - p1[2]) / delta
        b = p1[2] - p1[axis] * a
        po[2] = a * po[axis] + b
        return po

    @staticmethod
    def _normalise(vals: list) -> list:
        """Return a copy of vals scaled to [0, 1]."""
        lo, hi = min(vals), max(vals)
        if hi == lo:
            hi = lo + 1
        return [(v - lo) / (hi - lo) for v in vals]

    @staticmethod
    def _detrend(vals: list) -> list:
        """Rotate vals so the overall slope is removed (tilt correction)."""
        n = len(vals)
        angle = math.atan((vals[-1] - vals[0]) / n)
        sin_a, cos_a = math.sin(-angle), math.cos(-angle)
        return [i * sin_a + v * cos_a for i, v in enumerate(vals)]

    @staticmethod
    def _calc_mean(values) -> float:
        return sum(values) / float(len(values))

    @staticmethod
    def _calc_median(values) -> float:
        s = sorted(values)
        mid = len(s) // 2
        return s[mid] if len(s) & 1 else (s[mid - 1] + s[mid]) / 2.0

    # ------------------------------------------------------------------
    # Logging
    # ------------------------------------------------------------------

    def _log(self, msg: str):
        log.info(msg)
        if self.cfg.show_msg:
            self.obj.gcode.respond_info(msg)

    def _log_array(self, title: str, ary, tail: int = 32):
        log.info('[%s] %s', title, ary)
        if self.cfg.show_msg:
            body = ', '.join('%.2f' % ary[i] for i in range(len(ary) - tail, len(ary)))
            self._log('%s [%s]' % (title, body))

    # ------------------------------------------------------------------
    # Trigger detection
    # ------------------------------------------------------------------

    def _remove_spikes(self, vals: list) -> list:
        """Replace isolated max_hold spikes with their left neighbour."""
        out = list(vals)
        for i in range(1, len(out) - 1):
            if (out[i] >= self.cfg.max_hold
                    and out[i - 1] < self.cfg.max_hold / 2
                    and out[i + 1] < self.cfg.max_hold / 2):
                out[i] = out[i - 1]
        return out

    def _check_trigger(self, ch: int, fit_vals: list, _unfit_vals: list,
                       min_hold: int, max_hold: int) -> bool:
        """
        Return True when the load-cell signal in fit_vals meets trigger criteria.

        Early-exit paths (dirzctl finished, or sustained max_hold) take priority.
        The main path detrends and normalises the window, then checks slope and
        absolute-force thresholds.
        """
        all_params, _tick = self.obj.dirzctl.get_params()
        if len(all_params) == 2:
            self._log('TRI: dirzctl run complete on ch %d' % ch)
            self.state.out_index = self.cfg.pi_count - 1
            return True

        pi = self.cfg.pi_count
        half_pi = pi // 2

        # Sustained over-max-hold trigger
        if (len(fit_vals) >= half_pi
                and all(math.fabs(fit_vals[-k]) >= max_hold for k in (1, 2, 3))):
            self._log('TRI: sustained max_hold on ch %d' % ch)
            self.state.out_index = pi - 1
            return True

        if len(fit_vals) != pi:
            return False

        # Despike → detrend → find minimum index
        clean = self._remove_spikes(fit_vals)
        normed = self._detrend(self._normalise(clean))
        self.state.out_index = normed.index(min(normed))

        # Smooth values beyond the minimum index using filter coefficient
        if self.state.out_index > 0:
            k = self.obj.filter.lft_k1_oft / 2.0
            for i in range(self.state.out_index, pi):
                clean[i] = clean[i] * k + clean[i - 1] * (1.0 - k)

        # Monotonic rise at the tail
        if not (clean[-1] > clean[-2] > clean[-3]):
            return False

        # Tail must exceed everything before it
        pre_max = max(clean[0 : pi - 3])
        if not (clean[-1] > pre_max and clean[-2] > pre_max and clean[-3] > pre_max):
            return False

        # Normalised slope uniformity check
        normed2 = self._normalise(clean)
        for i in range(pi - 1):
            if (normed2[-1] - normed2[i]) / ((pi - i) / float(pi)) < 0.8:
                return False

        # Absolute force thresholds
        if (fit_vals[-1] < min_hold
                or fit_vals[-2] < min_hold / 2
                or fit_vals[-3] < min_hold / 3):
            return False

        self._log('TRI: all rules passed on ch %d' % ch)
        return True

    # ------------------------------------------------------------------
    # Temperature control
    # ------------------------------------------------------------------

    def _set_hotend_temp(self, temp: float, fan_pwm: int,
                         wait: bool = False, tol: float = 5.0):
        self.obj.pheaters.set_temperature(self.obj.heater_hot, temp, False)
        self.obj.gcode.run_script_from_command('M106 P0 S%d' % fan_pwm)
        self.obj.gcode.run_script_from_command('M106 P2 S%d' % fan_pwm)
        if wait:
            heater = self.obj.heater_hot
            while (self._system_ok()
                   and heater.target_temp > 0
                   and abs(heater.target_temp - heater.smoothed_temp) > tol):
                self.obj.hx711s.delay_s(0.1)

    def _set_bed_temp(self, temp: float, wait: bool = False, tol: float = 5.0):
        self.obj.pheaters.set_temperature(self.obj.heater_bed, temp, False)
        if wait:
            heater = self.obj.heater_bed
            while (self._system_ok()
                   and heater.target_temp > 0
                   and abs(heater.target_temp - heater.smoothed_temp) > tol):
                self.obj.hx711s.delay_s(0.1)

    # ------------------------------------------------------------------
    # Z calculation
    # ------------------------------------------------------------------

    def _calc_trigger_z(self, start_z: float, hx711_vals: list):
        """
        Cross-correlate HX711 timestamps with stepper-Z motion to find
        the Z coordinate at which the nozzle first made contact.

        Returns (up_min_cnt, up_all_cnt, success).
        """
        hx_params,  hx_start_tick  = self.obj.hx711s.get_params()
        drz_params, drz_start_tick = self.obj.dirzctl.get_params()

        if drz_params is None or len(drz_params) != 2:
            raise self.obj.printer.command_error(
                '{"code":"key502","msg":"probe_by_step: cannot receive stepper-Z status."}')

        step_dist = (self.obj.dirzctl.steppers[0].get_step_dist()
                     * self.obj.dirzctl.step_base)
        limt_up_cnt = int(_LIMT_UP_MM / step_dist)
        up_all_cnt  = min(drz_params[0]['step'] - drz_params[1]['step'] + 1, limt_up_cnt)

        if len(hx711_vals) < self.cfg.pi_count or len(hx_params) < self.cfg.pi_count:
            return up_all_cnt, up_all_cnt, False

        # Trim HX711 params to the last pi_count samples
        hx_params = hx_params[len(hx_params) - self.cfg.pi_count :]

        # Find the minimum of the detrended, normalised signal
        normed = self._detrend(self._normalise(list(hx711_vals)))
        self.state.out_index = normed.index(min(normed))

        # Convert MCU ticks to seconds (handle 32-bit rollover)
        def ticks_to_s(tick, ref, freq):
            if tick < ref:
                tick += _TICK_OVERFLOW
            return (tick - ref) / freq

        t0 = ticks_to_s(drz_params[0]['tick'], drz_start_tick, self.obj.dirzctl.mcu_freq)
        t1 = ticks_to_s(drz_params[1]['tick'], drz_start_tick, self.obj.dirzctl.mcu_freq)
        z0 = start_z
        z1 = start_z - up_all_cnt * step_dist

        t_trigger = ticks_to_s(
            hx_params[self.state.out_index]['nt'],
            hx_start_tick,
            self.obj.hx711s.mcu_freq,
        )
        trigger_z = self._linear_interp(
            [t0, 0, z0], [t1, 0, z1], [t_trigger, 0, 0], use_x_axis=True)[2]

        self.state.out_val_mm = trigger_z
        self._log('_calc_trigger_z: re_probe=%d idx=%d z=%.3f'
                  % (self.state.re_probe_cnt, self.state.out_index, trigger_z))

        up_min_cnt = min(max(int((trigger_z - z1) / step_dist), 0), limt_up_cnt)
        return up_min_cnt, up_all_cnt, True

    # ------------------------------------------------------------------
    # Core probe primitives
    # ------------------------------------------------------------------

    def probe_by_step(self, rdy_pos: list, speed_mm: float, min_dis_mm: float,
                      min_hold: int, max_hold: int,
                      up_after: bool = True) -> tuple:
        """
        Drive Z downward by stepper pulses while streaming HX711 data.
        Stops as soon as a trigger is detected, then optionally retracts.

        Returns (out_index, out_val_mm, success).
        """
        self.obj.hx711s.read_base(self.cfg.base_count // 2, max_hold)

        step_dist = (self.obj.dirzctl.steppers[0].get_step_dist()
                     * self.obj.dirzctl.step_base)
        step_cnt = int(min_dis_mm / step_dist)
        step_us  = int((min_dis_mm / speed_mm * 1_000_000) / step_cnt)

        self.obj.hx711s.query_start(
            self.cfg.pi_count * 2, 65535,
            del_dirty=True, show_msg=False, is_ck_con=True)
        self.obj.dirzctl.check_and_run(
            0, step_us, step_cnt, wait_finish=False, is_ck_con=True)
        self.obj.hx711s.delay_s(0.015)

        self._log('probe_by_step x=%.2f y=%.2f z=%.2f speed=%.2f step_us=%d step_cnt=%d'
                  % (rdy_pos[0], rdy_pos[1], rdy_pos[2], speed_mm, step_us, step_cnt))

        while self._system_ok():
            self.obj.hx711s.send_heart_beat()
            self.obj.dirzctl.send_heart_beat()

            all_vals = self.obj.hx711s.get_vals()
            if not all_vals or not all_vals[0]:
                self.obj.hx711s.delay_s(0.005)
                continue

            filt   = self.obj.filter
            unfit, tmp_unfit = filt.cal_offset_by_vals(
                self.obj.hx711s.s_count, all_vals, filt.lft_k1_oft, self.cfg.pi_count)
            fit, tmp_fit = filt.cal_filter_by_vals(
                self.obj.hx711s.s_count, all_vals, filt.hft_hz, filt.lft_k1, self.cfg.pi_count)

            triggered_ch = next(
                (ch for ch in range(self.obj.hx711s.s_count)
                 if self._check_trigger(ch, tmp_fit[ch], tmp_unfit[ch], min_hold, max_hold)),
                None,
            )
            if triggered_ch is not None:
                self._on_trigger(triggered_ch, rdy_pos, step_us, tmp_fit, up_after)
                return self.state.out_index, self.state.out_val_mm, True

            self.obj.hx711s.delay_s(0.005)

        return self.state.out_index, self.state.out_val_mm, True

    def _on_trigger(self, ch: int, rdy_pos: list, step_us: int,
                    tmp_fit: list, up_after: bool):
        """Handle the moment of trigger: stop motion, collect final data, retract."""
        # Stop stepper immediately
        self.obj.dirzctl.check_and_run(0, 0, 0, wait_finish=False)

        # Collect extra samples after trigger
        self.obj.hx711s.query_start(
            self.cfg.pi_count * 2, 0,
            del_dirty=False, show_msg=False)
        self.obj.hx711s.delay_s(0.015)

        for j in range(self.obj.hx711s.s_count):
            self._log_array('POST_TRIGGER ch=%d fit=' % j, tmp_fit[j], 16)

        self.obj.hx711s.delay_s(0.2)
        all_vals = self.obj.hx711s.get_vals()
        self._log_array('COLLECT ch=%d raw=' % ch, all_vals[ch])

        filt = self.obj.filter
        _hx_vals, tmp_cal = filt.cal_filter_by_vals(
            self.obj.hx711s.s_count, all_vals,
            filt.hft_hz, filt.lft_k1_cal, self.cfg.pi_count)
        self._log_array('COLLECT ch=%d cal=' % ch, tmp_cal[ch])

        up_min, up_all, _ok = self._calc_trigger_z(rdy_pos[2], tmp_cal[ch])
        if up_after:
            self.obj.dirzctl.check_and_run(1, step_us // 2, up_all)

    def _probe_times(self, max_tries: int, rdy_pos: list, speed_mm: float,
                     min_dis_mm: float, max_z_err: float,
                     min_hold: int, max_hold: int) -> float:
        """
        Probe at rdy_pos up to max_tries times, repeating until two consecutive
        readings agree within max_z_err.  Returns the averaged Z value.
        """
        now_pos = self.obj.toolhead.get_position()
        self._move(now_pos[:2] + [rdy_pos[2]], self.cfg.g29_rdy_speed)
        self._move(rdy_pos, self.cfg.g29_xy_speed)

        rdy_z   = rdy_pos[2]
        out_mm  = 0.0

        for _ in range(max_tries):
            _i0, mm0, ok = self.probe_by_step(
                rdy_pos[:2] + [rdy_z], speed_mm, min_dis_mm, min_hold, max_hold, True)
            if not ok:
                rdy_z += _REPROBE_Z_BUMP
                continue

            _i1, mm1, _ok = self.probe_by_step(
                rdy_pos[:2] + [rdy_z], speed_mm, min_dis_mm, min_hold, max_hold, True)
            out_mm = (mm0 + mm1) / 2.0

            if math.fabs(mm0 - mm1) <= max_z_err or not self._system_ok():
                break

            self.state.re_probe_cnt += 1
            self._log('_probe_times: reprobe needed mm0=%.3f mm1=%.3f' % (mm0, mm1))

        return out_mm

    def probe_z_offset(self, x: float, y: float) -> float:
        """Return the nozzle Z at (x, y) using the load-cell probe."""
        self._ensure_homed()
        return self._probe_times(
            3,
            [x, y, self.cfg.bed_max_err + 1.0],
            self.cfg.probe_speed,
            10,
            self.cfg.check_bed_mesh_max_err,
            self.cfg.min_hold,
            self.cfg.max_hold,
        )

    # ------------------------------------------------------------------
    # Nozzle cleaning
    # ------------------------------------------------------------------

    def clear_nozzle(self, hot_min_temp: float, hot_max_temp: float,
                     bed_max_temp: float, min_hold: int, max_hold: int):
        """
        Heat the nozzle, measure the wipe-pad height at both ends, drag the
        nozzle across the pad, then re-home Z.
        """
        src_x = self.cfg.clr_noz_start_x
        src_y = self.cfg.clr_noz_start_y
        end_x = src_x + self.cfg.clr_noz_len_x
        end_y = src_y + self.cfg.clr_noz_len_y
        safe_z = self.cfg.bed_max_err + 1.0

        self._set_bed_temp(bed_max_temp, wait=False)
        self._set_hotend_temp(hot_min_temp, fan_pwm=0, wait=False, tol=10)
        self._ensure_homed()

        # Probe wipe-pad heights
        self._set_hotend_temp(hot_min_temp, fan_pwm=0, wait=True, tol=10)
        src_z = self._probe_times(3, [src_x, src_y, safe_z],
                                  self.cfg.probe_speed, 10, 0.2, min_hold, max_hold)
        self._set_hotend_temp(hot_min_temp + 40, fan_pwm=0, wait=False, tol=10)
        end_z = self._probe_times(3, [end_x, end_y, safe_z],
                                  self.cfg.probe_speed, 10, 0.2, min_hold, max_hold)

        # Position nozzle just above pad start
        self._move([src_x, src_y, safe_z], self.cfg.g29_xy_speed)
        self._move([src_x, src_y, src_z + 0.1], self.cfg.g29_rdy_speed)

        # Heat to max, then drop back to min (ooze + wipe)
        self._set_hotend_temp(hot_max_temp, fan_pwm=0, wait=True, tol=10)
        self._set_hotend_temp(hot_min_temp, fan_pwm=0, wait=False)

        # Retract filament if configured
        if self.cfg.wipe_retract_distance > 0:
            gcode = self.obj.gcode.run_script_from_command
            gcode('G91')
            gcode('G1 E-%.2f F600' % self.cfg.wipe_retract_distance)
            gcode('G90')

        # Wipe across pad
        self._move([end_x, end_y, end_z + self.cfg.pa_clr_down_mm],
                   self.cfg.clr_noz_wipe_speed)

        # Cool down with fan
        self._set_hotend_temp(hot_min_temp, fan_pwm=255, wait=True, tol=5)
        self._move([end_x, end_y + 10, end_z + 10], self.cfg.clr_noz_wipe_speed)
        self._set_hotend_temp(hot_min_temp, fan_pwm=0, wait=False)

        # Wait for bed, re-home Z
        self._set_bed_temp(bed_max_temp, wait=True, tol=5)
        self._move(self.state.home_xy + [10], self.cfg.g29_xy_speed)
        self.obj.gcode.run_script_from_command('G28 Z')

    # ------------------------------------------------------------------
    # Config persistence
    # ------------------------------------------------------------------

    def _save_z_offset(self, kin_pos):
        """Write z_offset back to the printer config file."""
        if kin_pos is None:
            return
        z_offset = -kin_pos[2]
        gcode = self.obj.printer.lookup_object('gcode')
        gcode.respond_info(
            "%s: z_offset: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file "
            "with the above and restart the printer." % (self.cfg.probe_name, z_offset))
        configfile = self.obj.printer.lookup_object('configfile')
        configfile.set(self.cfg.probe_name, 'z_offset', '%.3f' % z_offset)

    # ------------------------------------------------------------------
    # G-code commands
    # ------------------------------------------------------------------

    def cmd_NOZZLE_CLEAR(self, gcmd):
        self.clear_nozzle(
            hot_min_temp = gcmd.get_float('HOT_MIN_TEMP', self.cfg.hot_min_temp),
            hot_max_temp = gcmd.get_float('HOT_MAX_TEMP', self.cfg.hot_max_temp),
            bed_max_temp = gcmd.get_float('BED_MAX_TEMP', self.cfg.bed_max_temp),
            min_hold     = gcmd.get_int('MIN_HOLD',       self.cfg.min_hold),
            max_hold     = gcmd.get_int('MAX_HOLD',       self.cfg.max_hold),
        )

    def cmd_PRTOUCH_PROBE_ZOFFSET(self, gcmd):
        """
        Measure the nozzle-to-sensor height difference, compute the Z-offset,
        and optionally apply it via SET_GCODE_OFFSET.

        Parameters:
            CLEAR_NOZZLE=1   — run NOZZLE_CLEAR before probing
            APPLY_Z_ADJUST=1 — apply the computed adjustment immediately
        """
        self._ensure_homed()
        self.state.reset()

        homing_origin  = self.obj.gcode_move.get_status()['homing_origin']
        start_z_offset = self.obj.probe.get_offsets()[2]
        self._log('cmd_PRTOUCH_PROBE_ZOFFSET: homing_origin=%s start_z_offset=%.3f'
                  % (list(homing_origin), start_z_offset))

        if gcmd.get_int('CLEAR_NOZZLE', 0):
            self.clear_nozzle(self.cfg.hot_min_temp, self.cfg.hot_max_temp,
                              self.cfg.bed_max_temp, self.cfg.min_hold, self.cfg.max_hold)

        # Randomise sensor position slightly to average surface variation
        x = self.cfg.sensor_x + random.uniform(-self.cfg.random_offset, self.cfg.random_offset)
        y = self.cfg.sensor_y + random.uniform(-self.cfg.random_offset, self.cfg.random_offset)

        # --- Reference probe reading (BLTouch / CR Touch / etc.) ---
        px_off, py_off = self.obj.probe.get_offsets()[:2]
        probe_x, probe_y = x - px_off, y - py_off
        self._log('Probe reference at (%.2f, %.2f)' % (probe_x, probe_y))
        self._move([probe_x, probe_y, self.cfg.bed_max_err + 1.0], self.cfg.g29_xy_speed)
        probe_gcmd = self.obj.gcode.create_gcode_command(
            'PROBE', 'PROBE', {'SAMPLES': str(_PROBE_SAMPLES)})
        z_probe = probe.run_single_probe(self.obj.probe, probe_gcmd)
        self._log('Reference probe z=%.3f' % z_probe[2])

        # --- Load-cell nozzle reading ---
        nozzle_z = self.probe_z_offset(x, y)
        self._log('Nozzle z=%.3f' % nozzle_z)

        z_offset  = nozzle_z - z_probe[2]
        z_adjust  = start_z_offset - z_offset
        self._log('z_offset=%.3f  z_adjust=%.3f' % (z_offset, z_adjust))

        if gcmd.get_int('APPLY_Z_ADJUST', 0):
            self.obj.gcode.run_script_from_command(
                'SET_GCODE_OFFSET Z_ADJUST=%f MOVE=1' % z_adjust)

        result    = list(z_probe)
        result[2] = -z_adjust
        self._save_z_offset(result)

    def cmd_PRTOUCH_ACCURACY(self, gcmd):
        """
        Probe Z at the sensor position multiple times and report statistics.

        Parameters:
            PROBE_SPEED=<mm/s>
            SAMPLES=<count>
        """
        self._ensure_homed()
        speed        = gcmd.get_float('PROBE_SPEED', self.cfg.probe_speed, above=0.0)
        sample_count = gcmd.get_int('SAMPLES', 10, minval=1)
        sensor_pos   = [self.cfg.sensor_x, self.cfg.sensor_y, self.cfg.bed_max_err]

        gcmd.respond_info(
            'PRTOUCH_ACCURACY at X=%.3f Y=%.3f samples=%d speed=%.1f'
            % (sensor_pos[0], sensor_pos[1], sample_count, speed))

        self._move(sensor_pos, self.cfg.g29_xy_speed)

        positions = []
        for _ in range(sample_count):
            _idx, z, _ok = self.probe_by_step(
                sensor_pos, speed, 10,
                self.cfg.min_hold, self.cfg.max_hold, True)
            positions.append(z)
            gcmd.respond_info('  sample %d at (%.3f, %.3f): z=%.3f'
                              % (len(positions), sensor_pos[0], sensor_pos[1], z))

        mx  = max(positions)
        mn  = min(positions)
        avg = self._calc_mean(positions)
        med = self._calc_median(positions)
        sigma = math.sqrt(sum((v - avg) ** 2 for v in positions) / len(positions))

        gcmd.respond_info(
            'Results: max=%.6f min=%.6f range=%.6f '
            'avg=%.6f median=%.6f stddev=%.6f'
            % (mx, mn, mx - mn, avg, med, sigma))


# ---------------------------------------------------------------------------
# Klipper entry point
# ---------------------------------------------------------------------------

def load_config(config):
    return PRTouchZOffsetWrapper(config)