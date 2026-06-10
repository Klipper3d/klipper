# Support for button detection and callbacks
#
# Copyright (C) 2022-12-09  CC <wangyulong878@sina.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import math
import random
import mcu
import time
from . import probe

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------
 
_PROBE_SAMPLES   = 2               # default probe samples for Z-reference

# ---------------------------------------------------------------------------
# Configuration
# --------------------------------------------------------------------------- 

class PRTouchConfig:
    """Конфигурация модуля PRTouch"""

    def __init__(self, config):
        # Основные параметры датчика
        self.base_count = config.getint('base_count', default=40, minval=10, maxval=100)
        self.pi_count = config.getint('pi_count', default=32, minval=16, maxval=128)
        self.min_hold = config.getint('min_hold', default=3000, minval=100, maxval=50000)
        self.max_hold = config.getint('max_hold', default=50000, minval=100, maxval=100000)

        # Позиция датчика
        self.sensor_x = config.getfloat('sensor_x', minval=0, maxval=300)
        self.sensor_y = config.getfloat('sensor_y', minval=0, maxval=300)
        self.random_offset = config.getfloat('sensor_random_offset', default=5, minval=0, maxval=10)

        # Температуры
        self.nozzle_clean_min_temp = config.getfloat('s_nozzle_clean_min_temp', default=140, minval=80, maxval=200)
        self.nozzle_clean_max_temp = config.getfloat('s_nozzle_clean_max_temp', default=180, minval=170, maxval=300)
        self.bed_max_temp = config.getfloat('s_bed_max_temp', default=60, minval=45, maxval=100)

        # Очистка сопла
        self.pa_clr_dis_mm = config.getint('pa_clr_dis_mm', default=5, minval=2, maxval=100)
        self.pa_clr_down_mm = config.getfloat('pa_clr_down_mm', default=-0.1, minval=-1, maxval=1)

        self.nozzle_clean_start_pos_x = config.getfloat('nozzle_clean_start_pos_x', default=0, minval=-8, maxval=220)
        self.nozzle_clean_start_pos_y = config.getfloat('nozzle_clean_start_pos_y', default=0, minval=-5, maxval=220)

        self.nozzle_clean_len_x = config.getfloat('nozzle_clean_len_x', default=0, minval=0, maxval=200)
        self.nozzle_clean_len_y = config.getfloat('nozzle_clean_len_y', default=0, minval=0, maxval=200)
        self.nozzle_clean_speed = config.getfloat('nozzle_clean_speed', default=30, minval=10, maxval=100)

        # Дополнительные параметры
        self.bed_max_err = config.getint('bed_max_err', default=4, minval=2, maxval=10)
        self.max_z = config.getsection('stepper_z').getfloat('position_max', default=300, minval=100, maxval=500)
        self.g29_xy_speed = config.getfloat('g29_xy_speed', default=150, minval=10, maxval=1000)
        self.g29_rdy_speed = config.getfloat('g29_rdy_speed', default=2.5, minval=0.1, maxval=10)
        self.probe_speed = config.getfloat('probe_speed', default=2.0, minval=0.1, maxval=10)

        self.show_msg = config.getboolean('show_msg', default=False)
        self.check_bed_mesh_max_err = config.getfloat('check_bed_mesh_max_err', default=0.2, minval=0.01, maxval=1)
        self.wipe_retract_distance = config.getfloat('wipe_retract_distance', default=0, minval=0, maxval=50)
        self.probe_name = config.get('probe_name', default='bltouch')

        # Сохранённые профили
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
        self.out_index = 0
        self.out_state_mm = 0.
        self.re_probe_cnt = 0
        self.home_xy = None

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
        self.cfg = PRTouchConfig(config)
        self.state = PRTouchState(config)
        self.obj = PRTouchObjects(config)

        self.obj.printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self.obj.gcode.register_command('PRTOUCH_PROBE_ZOFFSET', self.cmd_PRTOUCH_PROBE_ZOFFSET, desc=self.cmd_PRTOUCH_PROBE_ZOFFSET_help)
        self.obj.gcode.register_command('NOZZLE_CLEAR', self.cmd_NOZZLE_CLEAR, desc=self.cmd_NOZZLE_CLEAR_help)
        self.obj.gcode.register_command('PRTOUCH_ACCURACY', self.cmd_PRTOUCH_ACCURACY, desc=self.cmd_PRTOUCH_ACCURACY_help)
        pass


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

    def _pnt_tri_msg(self, index, msg,  ary):
        if self.cfg.show_msg:
            self.pnt_msg('TRI SUCCESS BY: ' + msg)
            self.pnt_array('TRI CH=%d ARY=' % index, ary)
        pass

    def _check_trigger(self, arg_index, fit_states, unfit_states, min_hold, max_hold):
        all_params, tick = self.obj.dirzctl.get_params()
        if len(all_params) == 2:
            self._pnt_tri_msg(arg_index, 'Tri by Dirzctl run over!', fit_states)
            return True
        fit_states_t = [x for x in fit_states]
        self.state.out_index = self.cfg.pi_count - 1
        if len(fit_states) >= (self.cfg.pi_count / 2) and math.fabs(fit_states[-1]) >= max_hold and \
                math.fabs(fit_states[-2]) >= max_hold and math.fabs(fit_states[-3]) >= max_hold:
            self._pnt_tri_msg(arg_index, 'Tri by out max_hold!', fit_states)
            return True
        if len(fit_states) != self.cfg.pi_count:
            return False           
        for i in range(1, self.cfg.pi_count - 1):
            if fit_states_t[i] >= max_hold and fit_states_t[i - 1] < (max_hold / 2) and fit_states_t[i + 1] < (max_hold / 2):
                fit_states_t[i] = fit_states_t[i - 1]

        states_p = [x for x in fit_states_t]
        max_state = max(states_p)
        min_state = min(states_p)
        max_state += 1 if (max_state - min_state) == 0 else 0
        for i in range(len(states_p)):
            states_p[i] = (states_p[i] - min_state) / (max_state - min_state)
        angle = math.atan((states_p[-1] - states_p[0]) / len(states_p))
        sin_angle = math.sin(-angle)
        cos_angle = math.cos(-angle)
        for i in range(len(states_p)):
            states_p[i] = (i - 0) * sin_angle + (states_p[i] - 0) * cos_angle + 0
        self.state.out_index = states_p.index(min(states_p))
        if(self.state.out_index > 0):
            for i in range(self.state.out_index, self.cfg.pi_count):
                fit_states_t[self.state.out_index] = fit_states_t[self.state.out_index] * (self.obj.filter.lft_k1_oft / 2) + fit_states_t[self.state.out_index - 1] * (1 - (self.obj.filter.lft_k1_oft / 2))
        states_p = [x for x in fit_states_t]

        if not (fit_states_t[-1] > fit_states_t[-2] > fit_states_t[-3]):
            return False
        max_state = max(fit_states_t[0:(self.cfg.pi_count - 3)])
        if not ((fit_states_t[-1] > max_state) and (fit_states_t[-2] > max_state) and (fit_states_t[-3] > max_state)):
            return False
        max_state = max(fit_states_t)
        min_state = min(fit_states_t)
        for i in range(0, self.cfg.pi_count):
            fit_states_t[i] = (fit_states_t[i] - min_state) / (max_state - min_state)
        for i in range(0, self.cfg.pi_count - 1):
            if (fit_states_t[-1] - fit_states_t[i]) / ((self.cfg.pi_count - i) * 1. / self.cfg.pi_count) < 0.8:
                return False
        if fit_states[-1] < min_hold or fit_states[-2] < (min_hold / 2) or fit_states[-3] < (min_hold / 3):
            return False
        self._pnt_tri_msg(arg_index, 'Tri by fit all rule!', states_p)
        return True

    def _set_hot_temps(self, temp, fan_spd, wait=False, err=5):
        self.obj.pheaters.set_temperature(self.obj.heater_hot, temp, False)
        self.obj.gcode.run_script_from_command('M106 P0 S%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('M106 P1 S%d' % (fan_spd))
        self.obj.gcode.run_script_from_command('M106 P2 S%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('SET_PIN PIN=fan0 stateUE=%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('SET_PIN PIN=fan1 stateUE=%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('SET_PIN PIN=fan2 stateUE=%d' % (fan_spd))
        if wait:
            while self._system_ok() and abs(self.obj.heater_hot.target_temp - self.obj.heater_hot.smoothed_temp) > err and self.obj.heater_hot.target_temp > 0:
                self.obj.hx711s.delay_s(0.100) 
        pass

    def _set_bed_temps(self, temp, wait=False, err=5):
        self.obj.pheaters.set_temperature(self.obj.heater_bed, temp, False)           
        if wait:
            while self._system_ok() and abs(self.obj.heater_bed.target_temp - self.obj.heater_bed.smoothed_temp) > err and self.obj.heater_bed.target_temp > 0:
                self.obj.hx711s.delay_s(0.100)          
        pass

    def _calc_mean(self, positions):
        count = float(len(positions))
        return sum(positions) / count

    def _calc_median(self, positions):
        z_sorted = sorted(positions)
        middle = len(positions) // 2
        if (len(positions) & 1) == 1:
            # odd number of samples
            return z_sorted[middle]
        # even number of samples
        return self._calc_mean(z_sorted[middle-1:middle+1])

    def pnt_msg(self, msg):
        logging.info(msg)
        if self.cfg.show_msg:
            self.obj.gcode.respond_info(msg)
        pass

    def pnt_array(self, title, ary, lent=32):
        logging.info('[%s] %s' , title, str(ary))
        if self.cfg.show_msg:
            st = title + ' ['
            for i in range(len(ary) - lent, len(ary)):
                st = st + "%.2f, " % (ary[i])
            self.pnt_msg(st + ']')
        pass

    def _probe_times(self, max_times, rdy_pos, speed_mm, min_dis_mm, max_z_err, min_hold, max_hold):
        o_mm = 0
        rdy_pos_z = rdy_pos[2]
        now_pos = self.obj.toolhead.get_position()
        self._move(now_pos[:2] + [rdy_pos[2]], self.cfg.g29_rdy_speed)        
        self._move(rdy_pos, self.cfg.g29_xy_speed)
        for i in range(max_times):
            o_index0, o_mm0, deal_sta = self.probe_by_step(rdy_pos[:2] + [rdy_pos_z], speed_mm, min_dis_mm, min_hold, max_hold, True)
            if not deal_sta and rdy_pos_z == rdy_pos[2]:
                rdy_pos_z += 2
                continue
            o_index1, o_mm1, deal_sta = self.probe_by_step(rdy_pos[:2] + [rdy_pos_z], speed_mm, min_dis_mm, min_hold, max_hold, True)
            o_mm = (o_mm0 + o_mm1) / 2
            if math.fabs(o_mm0 - o_mm1) <= max_z_err or not self._system_ok():
                break
            self.state.re_probe_cnt += 1
            self.pnt_msg('***_probe_times must be reprobe= o_mm0=%.2f, o_mm1=%.2f' % (o_mm0, o_mm1))
        return o_mm

    def clear_nozzle(self, nozzle_clean_min_temp, nozzle_clean_max_temp, bed_max_temp, min_hold, max_hold):

        self.pnt_msg("=== NOZZLE CLEAN START ===")

        min_x = self.cfg.nozzle_clean_start_pos_x
        min_y = self.cfg.nozzle_clean_start_pos_y
        max_x = min_x + self.cfg.nozzle_clean_len_x
        max_y = min_y + self.cfg.nozzle_clean_len_y

        # 1. safe preheat
        self._set_bed_temps(bed_max_temp, wait=True)
        self._set_hot_temps(nozzle_clean_min_temp, fan_spd=0, wait=True, err=10)

        self._ensure_homed(False)

        # 2. safe Z height
        safe_z = self.cfg.bed_max_err + 5

        cur = self.obj.toolhead.get_position()

        src = [min_x, min_y, safe_z, cur[3]]
        end = [max_x, max_y, safe_z, cur[3]]

        # 3. stabilize nozzle temp
        self._set_hot_temps(nozzle_clean_min_temp, fan_spd=0, wait=True, err=5)

        # 4. probing start point
        src[2] = self._probe_times(
            3,
            [src[0], src[1], safe_z],
            self.cfg.probe_speed,
            10,
            0.2,
            min_hold,
            max_hold
        )

        # 5. slightly hotter for flow control
        self._set_hot_temps(nozzle_clean_min_temp + 30, fan_spd=0, wait=True, err=5)
        self._set_hot_temps(nozzle_clean_max_temp, fan_spd=0, wait=True, err=5)

        # 6. probing end point
        end[2] = self._probe_times(
            3,
            [end[0], end[1], safe_z],
            self.cfg.probe_speed,
            10,
            0.2,
            min_hold,
            max_hold
        )

        # 7. move above start
        self._move([src[0], src[1], safe_z], self.cfg.g29_xy_speed)
        self._move([src[0], src[1], src[2] + 0.2], self.cfg.g29_rdy_speed)

        # 8. purge BEFORE wipe (IMPORTANT FIX)
        self.obj.gcode.run_script_from_command('G91')
        self.obj.gcode.run_script_from_command('G1 E3 F300')  # small purge
        self.obj.gcode.run_script_from_command('G90')

        # 9. full hot wipe temp
        self._set_hot_temps(nozzle_clean_max_temp, fan_spd=0, wait=True, err=5)

        # 10. main wipe pass (slow + stable)
        wipe_z = end[2] + self.cfg.pa_clr_down_mm

        self._move([end[0], end[1], wipe_z], self.cfg.probe_speed)
        self._move([end[0], end[1], wipe_z], self.cfg.nozzle_clean_speed)

        # 11. second pass (improves cleaning)
        self._move([src[0], src[1], wipe_z], self.cfg.probe_speed)
        self._move([end[0], end[1], wipe_z], self.cfg.probe_speed)
        self._move([src[0], src[1], wipe_z], self.cfg.nozzle_clean_speed)
        self._move([end[0], end[1], wipe_z], self.cfg.nozzle_clean_speed)

        # 12. controlled retract (reduce stringing)
        if self.cfg.wipe_retract_distance > 0:
            self.obj.gcode.run_script_from_command('G91')
            self.obj.gcode.run_script_from_command(
                'G1 E-%.2f F600' % self.cfg.wipe_retract_distance
            )
            self.obj.gcode.run_script_from_command('G90')

        # 13. cool down while moving away
        self._set_hot_temps(nozzle_clean_min_temp, fan_spd=255, wait=False)

        self._move([max_x, max_y + 10, safe_z + 5], self.cfg.g29_xy_speed)

        # 14. final bed stabilization
        self._set_bed_temps(bed_max_temp, wait=True, err=5)

        # 15. final home Z
        self.obj.gcode.run_script_from_command('G28 Z')

        self.pnt_msg("=== NOZZLE CLEAN END ===")
        pass

    def probe_z_offset(self, x, y):
        self._ensure_homed()
        z_offset = self._probe_times(3, [x, y, self.cfg.bed_max_err + 1.], self.cfg.probe_speed, 10, self.cfg.check_bed_mesh_max_err, self.cfg.min_hold, self.cfg.max_hold)
        return z_offset

    def _cal_min_z(self, start_z, hx711_states):
        hx711_params, hx711_start_tick = self.obj.hx711s.get_params()
        dirzctl_params, dirzctl_start_tick = self.obj.dirzctl.get_params()
        if dirzctl_params is None or len(dirzctl_params) != 2:     
            raise self.obj.printer.command_error("""{"code":"key502", "msg":"probe_by_step: Can not recv stepper-z status."}""")
        if len(hx711_states) < self.cfg.pi_count or len(hx711_params) < self.cfg.pi_count:
            up_all_cnt = dirzctl_params[0]['step'] - dirzctl_params[1]['step'] + 1
            return up_all_cnt, up_all_cnt, False
        del hx711_params[0:(len(hx711_params) - self.cfg.pi_count)]

        states_p = [x for x in hx711_states]
        max_state = max(states_p)
        min_state = min(states_p)
        for i in range(len(states_p)):
            states_p[i] = (states_p[i] - min_state) / (max_state - min_state)
        angle = math.atan((states_p[-1] - states_p[0]) / len(states_p))
        sin_angle = math.sin(-angle)
        cos_angle = math.cos(-angle)
        for i in range(len(states_p)):
            states_p[i] = (i - 0) * sin_angle + (states_p[i] - 0) * cos_angle + 0
        self.state.out_index = states_p.index(min(states_p))

        dirzctl_params[0]['tick'] = (dirzctl_params[0]['tick'] - dirzctl_start_tick) / self.obj.dirzctl.mcu_freq
        dirzctl_params[1]['tick'] = ((4294967296 if dirzctl_params[1]['tick'] < dirzctl_start_tick else 0) + dirzctl_params[1]['tick'] - dirzctl_start_tick) / self.obj.dirzctl.mcu_freq
        dirzctl_params[0]['z'] = start_z
        dirzctl_params[1]['z'] = start_z - (dirzctl_params[0]['step'] - dirzctl_params[1]['step'] + 1) * (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base)
        tick_p = ((4294967296 if hx711_params[self.state.out_index]['nt'] < hx711_start_tick else 0) + hx711_params[self.state.out_index]['nt'] - hx711_start_tick) / self.obj.hx711s.mcu_freq
        self.state.out_state_mm = self._get_linear2([dirzctl_params[0]['tick'], 0, dirzctl_params[0]['z']], [dirzctl_params[1]['tick'], 0, dirzctl_params[1]['z']], [tick_p, 0, 0], True)[2]
        self.pnt_msg('call_min_z, re_probe_cnt=%d, out_index=%d, out_state_mm=%.2f' % (self.state.re_probe_cnt, self.state.out_index, self.state.out_state_mm))
        up_min_cnt = int((self.state.out_state_mm - dirzctl_params[1]['z']) / (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base))
        up_all_cnt = dirzctl_params[0]['step'] - dirzctl_params[1]['step'] + 1
        limt_up_cnt = int(10 / (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base))
        up_min_cnt = up_min_cnt if up_min_cnt < limt_up_cnt else limt_up_cnt  
        up_all_cnt = up_all_cnt if up_all_cnt < limt_up_cnt else limt_up_cnt
        return (up_min_cnt if up_min_cnt >= 0 else 0), up_all_cnt, True

    def probe_by_step(self, rdy_pos, speed_mm, min_dis_mm, min_hold, max_hold, up_after=True):
        self.obj.hx711s.read_base(int(self.cfg.base_count / 2), max_hold)
        step_cnt = int(min_dis_mm / (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base))
        step_us = int(((min_dis_mm / speed_mm) * 1000 * 1000) / step_cnt)
        self.obj.hx711s.query_start(self.cfg.pi_count * 2, int(65535), del_dirty=True, show_msg=False, is_ck_con=True)        
        self.obj.dirzctl.check_and_run(0, int(step_us), int(step_cnt), wait_finish=False, is_ck_con=True)
        self.obj.hx711s.delay_s(0.015)
        self.pnt_msg('*********************************************************')
        self.pnt_msg('PROBE_BY_STEP x=%.2f y=%.2f z=%.2f speed_mm=%.2f step_us=%d step_cnt=%d' % (rdy_pos[0], rdy_pos[1], rdy_pos[2], speed_mm, step_us, step_cnt))
        while self._system_ok():
            self.obj.hx711s.send_heart_beat()
            self.obj.dirzctl.send_heart_beat()
            all_statess = self.obj.hx711s.get_states()
            if all_statess is None or len(all_statess[0]) == 0:
                self.obj.hx711s.delay_s(0.005)
                continue
            unfit_states, tmp_unfit_states = self.obj.filter.cal_offset_by_states(self.obj.hx711s.s_count, all_statess, self.obj.filter.lft_k1_oft, self.cfg.pi_count)
            fit_states, tmp_fit_states = self.obj.filter.cal_filter_by_states(self.obj.hx711s.s_count, all_statess, self.obj.filter.hft_hz, self.obj.filter.lft_k1, self.cfg.pi_count)

            for i in range(self.obj.hx711s.s_count):
                if not self._check_trigger(i, tmp_fit_states[i], tmp_unfit_states[i], min_hold, max_hold):
                    continue
                self.obj.dirzctl.check_and_run(0, 0, 0, wait_finish=False)
                self.obj.hx711s.query_start(self.cfg.pi_count * 2, int(0), del_dirty=False, show_msg=False)
                self.obj.hx711s.delay_s(0.015)
                for j in range(int(self.obj.hx711s.s_count)):
                    self.pnt_array('TRIGGER_USE_CH=%d, FIT_stateS=' % (j), tmp_fit_states[j], 16)
                self.obj.hx711s.delay_s(0.2)
                all_statess = self.obj.hx711s.get_states()
                self.pnt_array('WAIT_AND_CAL_CH=%d, ARY=' % (i), all_statess[i])
                hx711_states, tmp_hx711_vs = self.obj.filter.cal_filter_by_states(self.obj.hx711s.s_count, all_statess, self.obj.filter.hft_hz, self.obj.filter.lft_k1_cal, self.cfg.pi_count)
                self.pnt_array('WAIT_AND_CAL_CH=%d, ARY=' % (i), tmp_hx711_vs[i])
                up_min_cnt, up_all_cnt, deal_sta = self._cal_min_z(rdy_pos[2], tmp_hx711_vs[i])
                if up_after:
                    self.obj.dirzctl.check_and_run(1, int(step_us / 2), int(up_all_cnt))
                return self.state.out_index, self.state.out_state_mm, deal_sta
            self.obj.hx711s.delay_s(0.005)
        return self.state.out_index, self.state.out_state_mm, True

    def probe_calibrate_finalize(self, kin_pos):
        if kin_pos is None:
            return
        z_offset = -kin_pos[2]
        probe_name = self.cfg.probe_name
        gcode = self.obj.printer.lookup_object('gcode')
        gcode.respond_info(
            "%s: z_offset: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer." % (probe_name, z_offset))
        configfile = self.obj.printer.lookup_object('configfile')
        configfile.set(probe_name, 'z_offset', "%.3f" % (z_offset,))

    cmd_NOZZLE_CLEAR_help = "Clear the nozzle on bed."
    def cmd_NOZZLE_CLEAR(self, gcmd):
        nozzle_clean_min_temp = gcmd.get_float('nozzle_clean_min_temp', self.cfg.nozzle_clean_min_temp)
        nozzle_clean_max_temp = gcmd.get_float('nozzle_clean_max_temp', self.cfg.nozzle_clean_max_temp)
        bed_max_temp = gcmd.get_float('BED_MAX_TEMP', self.cfg.bed_max_temp)
        min_hold = gcmd.get_int('MIN_HOLD', self.cfg.min_hold)
        max_hold = gcmd.get_int('MAX_HOLD', self.cfg.max_hold)
        self.clear_nozzle(nozzle_clean_min_temp, nozzle_clean_max_temp, bed_max_temp, min_hold, max_hold)

    cmd_PRTOUCH_PROBE_ZOFFSET_help = "Probe the z-offset"
    def cmd_PRTOUCH_PROBE_ZOFFSET(self, gcmd):
        self._ensure_homed()

        homing_origin = self.obj.gcode_move.get_status()['homing_origin']
        self.pnt_array('homing_origin: ', homing_origin, len(homing_origin))
        start_z_offset = self.obj.probe.get_offsets()[2]
        self.pnt_msg('Start z_offset: %.3f' % start_z_offset)

        if gcmd.get_int('CLEAR_NOZZLE', 0) == 1:
            self.clear_nozzle(self.cfg.nozzle_clean_min_temp, self.cfg.nozzle_clean_max_temp, self.cfg.bed_max_temp,
                            self.cfg.min_hold, self.cfg.max_hold)

        x = self.cfg.sensor_x + random.uniform(-self.cfg.random_offset, self.cfg.random_offset)
        y = self.cfg.sensor_y + random.uniform(-self.cfg.random_offset, self.cfg.random_offset)
        probe_x_offset, probe_y_offset = self.obj.probe.get_offsets()[:2]
        probe_x = x - probe_x_offset
        probe_y = y - probe_y_offset
        self.pnt_msg("Checking z-position of probe (%.2f, %.2f)" % (probe_x, probe_y))
        self._move([probe_x, probe_y, self.cfg.bed_max_err + 1.], self.cfg.g29_xy_speed)
        probe_gcmd = self.obj.gcode.create_gcode_command("PROBE", "PROBE", {'SAMPLES': _PROBE_SAMPLES})
        z_probe = probe.run_single_probe(self.obj.probe, probe_gcmd)
        self.pnt_msg('Probe at sensor: %.3f' % z_probe[2])

        nozzle_z_offset = self.probe_z_offset(x, y)
        self.pnt_msg('Nozzle z_offset: %.3f' % nozzle_z_offset)

        z_offset = nozzle_z_offset - z_probe[2]
        self.pnt_msg('Calculated z_offset: %.3f' % z_offset)

        z_adjust = start_z_offset - z_offset
        self.pnt_msg('z_adjust: %.3f' % z_adjust)
        if gcmd.get_int('APPLY_Z_ADJUST', 0) == 1:
            self.obj.gcode.run_script_from_command('SET_GCODE_OFFSET Z_ADJUST=%f MOVE=1' % (z_adjust))

        #prtouch_result = (z_probe[0], z_probe[1], homing_origin[2] + z_adjust - start_z_offset, z_probe[3], z_probe[4], z_probe[5])
        prtouch_result = list(z_probe)
        prtouch_result[2] = -z_adjust
        self.probe_calibrate_finalize(prtouch_result)

    cmd_PRTOUCH_ACCURACY_help = "Probe Z-height accuracy at sensoor position"
    def cmd_PRTOUCH_ACCURACY(self, gcmd):
        self._ensure_homed()
        speed = gcmd.get_float("PROBE_SPEED", self.cfg.probe_speed, above=0.)
        sample_count = gcmd.get_int("SAMPLES", 10, minstate=1)
        gcmd.respond_info("PRTOUCH_ACCURACY at X:%.3f Y:%.3f"
                          " (samples=%d speed=%.1f)\n"
                          % (self.cfg.sensor_x, self.cfg.sensor_y,
                             sample_count, speed))
        sensor_pos = [self.cfg.sensor_x, self.cfg.sensor_y, self.cfg.bed_max_err]
        # Move to sensor location
        self._move(sensor_pos, self.cfg.g29_xy_speed)
        # Probe bed sample_count times
        positions = []
        while len(positions) < sample_count:
            # Probe position
            _index1, pos, _sta = self.probe_by_step(sensor_pos, speed, 10, self.cfg.min_hold, self.cfg.max_hold, True)
            positions.append(pos)
            gcmd.respond_info(
                "probe #%d at (%.3f, %.3f): %.3f\n"
                % (len(positions), sensor_pos[0], sensor_pos[1], pos))
        # Calculate maximum, minimum and average stateues
        max_stateue = max(positions)
        min_stateue = min(positions)
        range_stateue = max_stateue - min_stateue
        avg_stateue = self._calc_mean(positions)
        median = self._calc_median(positions)
        # calculate the standard deviation
        deviation_sum = 0
        for i in range(len(positions)):
            deviation_sum += pow(positions[i] - avg_stateue, 2.)
        sigma = (deviation_sum / len(positions)) ** 0.5
        # Show information
        gcmd.respond_info(
            "probe accuracy results: maximum %.6f, minimum %.6f, range %.6f, "
            "average %.6f, median %.6f, standard deviation %.6f" % (
            max_stateue, min_stateue, range_stateue, avg_stateue, median, sigma))


def load_config(config):
    prt = PRTouchZOffsetWrapper(config)
    return prt

# PRTOUCH_PROBE_ZOFFSET APPLY_Z_ADJUST=1