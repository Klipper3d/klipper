# Support for button detection and callbacks
#
# Copyright (C) 2022-12-09  CC <wangyulong878@sina.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import math
import random
from . import probe
import mcu
import time
import socket

class PRTouchCFG:
    def __init__(self, config):
        self.base_count = config.getint('base_count', default=40, minval=10, maxval=100)
        self.pi_count = config.getint('pi_count', default=32, minval=16, maxval=128)
        self.min_hold = config.getint('min_hold', default=3000, minval=100, maxval=50000)
        self.max_hold = config.getint('max_hold', default=50000, minval=100, maxval=100000)
        self.hot_min_temp = config.getfloat('s_hot_min_temp', default=140, minval=80, maxval=200)
        self.hot_max_temp = config.getfloat('s_hot_max_temp', default=200, minval=180, maxval=300)
        self.bed_max_temp = config.getfloat('s_bed_max_temp', default=60, minval=45, maxval=100)
        self.pa_fil_len_mm = config.getint('pa_fil_len_mm', default=2, minval=2, maxval=100)
        self.pa_fil_dis_mm = config.getint('pa_fil_dis_mm', default=30, minval=2, maxval=100)
        self.pa_clr_dis_mm = config.getint('pa_clr_dis_mm', default=20, minval=2, maxval=100)
        self.pa_clr_down_mm = config.getfloat('pa_clr_down_mm', default=-0.1, minval=-1, maxval=1)
        self.clr_noz_start_x = config.getfloat('clr_noz_start_x', default=0, minval=0, maxval=1000)
        self.clr_noz_start_y = config.getfloat('clr_noz_start_y', default=0, minval=0, maxval=1000)
        self.clr_noz_len_x = config.getfloat('clr_noz_len_x', default=0, minval=self.pa_clr_dis_mm + 6, maxval=1000)
        self.clr_noz_len_y = config.getfloat('clr_noz_len_y', default=0, minval=0, maxval=1000)
        self.bed_max_err = config.getint('bed_max_err', default=2, minval=2, maxval=10)
        self.max_z = config.getsection('stepper_z').getfloat('position_max', default=300, minval=100, maxval=500)
        self.g29_xy_speed = config.getfloat('g29_xy_speed', default=150, minval=10, maxval=1000)
        self.fix_z_offset = config.getfloat('fix_z_offset', default=0.0, minval=-1, maxval=1)
        self.max_dis_bef_g28 = config.getfloat('max_dis_bef_g28', default=10, minval=0, maxval=50)
        self.dead_zone_bef_g28 = config.getfloat('dead_zone_bef_g28', default=self.max_dis_bef_g28 / 2, minval=0, maxval=50) 
        self.g28_sta0_speed = config.getfloat('g28_sta0_speed', default=2.0, minval=0.1, maxval=10)
        self.g28_sta1_speed = config.getfloat('g28_sta1_speed', default=2.5, minval=0.1, maxval=10)
        self.g29_rdy_speed = config.getfloat('g29_rdy_speed', default=2.5, minval=0.1, maxval=10)
        self.g29_speed = config.getfloat('g29_speed', default=2.0, minval=0.1, maxval=10)
        self.show_msg = config.getboolean('show_msg', default=False)
        self.best_above_z = config.getfloat('best_above_z', default=1.5, minval=0.5, maxval=10)
        self.g28_wait_cool_down = config.getboolean('g28_wait_cool_down', default=False)
        self.shake_cnt = config.getint('shake_cnt', default=4, minval=1, maxval=512)
        self.shake_range = config.getint('shake_range', default=0.5, minval=0.1, maxval=2)
        self.shake_max_velocity = config.getfloat('shake_max_velocity', default=100, minval=1, maxval=5000)
        self.shake_max_accel = config.getfloat('shake_max_accel', default=1000, minval=1, maxval=50000)
        self.g28_sta0_min_hold = config.getint('g28_sta0_min_hold', default=self.min_hold * 2, minval=100, maxval=100000)
        self.need_measure_gap = config.getboolean('need_measure_gap', default=True)
        self.gap_dis_range = config.getfloat('gap_dis_range', default=0.6, minval=0.2, maxval=2)
        self.z_gap_00 = config.getfloat('z_gap_00', default=0, minval=-1, maxval=1)
        self.z_gap_01 = config.getfloat('z_gap_01', default=0, minval=-1, maxval=1)
        self.z_gap_10 = config.getfloat('z_gap_10', default=0, minval=-1, maxval=1)
        self.z_gap_11 = config.getfloat('z_gap_11', default=0, minval=-1, maxval=1)
        self.check_bed_mesh_max_err = config.getfloat('check_bed_mesh_max_err', default=0.2, minval=0.01, maxval=1)
        self.tri_wave_ip    = config.get('tri_wave_ip', None)
        self.self_z_offset = config.getfloat('self_z_offset', default=0.0, minval=-2, maxval=2)

        self.stored_profs = config.get_prefix_sections('prtouch')
        self.stored_profs = self.stored_profs[1] if (len(self.stored_profs) == 2 and self.need_measure_gap) else None
        pass


class PRTouchVAL:
    def __init__(self, config):
        self.out_index = 0
        self.out_val_mm = 0.
        self.rdy_pos = [[0., 0., 0.] * 4]
        self.gap_pos = None
        self.g29_cnt = int(0)
        self.re_probe_cnt = 0
        self.home_xy = None
        self.jump_probe_ready = False
        pass


class PRTouchOBJ:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.toolhead = None
        self.hx711s = None
        self.pheaters = None
        self.heater_hot = None
        self.heater_bed = None
        self.bed_mesh = None
        self.mcu = None
        self.dirzctl = None
        self.kin = None
        self.gcode = self.printer.lookup_object('gcode')
        pass

    def find_objs(self):
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
        pass


class PRTouchEndstopWrapper:
    def __init__(self, config):
        self.cfg = PRTouchCFG(config)
        self.val = PRTouchVAL(config)
        self.obj = PRTouchOBJ(config)

        self.obj.printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self.obj.gcode.register_command('PRTOUCH_TEST', self.cmd_PRTOUCH_TEST, desc=self.cmd_PRTOUCH_TEST_help)
        self.obj.gcode.register_command('PRTOUCH_READY', self.cmd_PRTOUCH_READY, desc=self.cmd_PRTOUCH_READY_help)
        self.obj.gcode.register_command('NOZZLE_CLEAR', self.cmd_NOZZLE_CLEAR, desc=self.cmd_NOZZLE_CLEAR_help)
        self.obj.gcode.register_command('CHECK_BED_MESH', self.cmd_CHECK_BED_MESH, desc=self.cmd_CHECK_BED_MESH_help)
        self.obj.gcode.register_command('MEASURE_GAP_TEST', self.cmd_MEASURE_GAP_TEST, desc=self.cmd_MEASURE_GAP_TEST_help)
        pass

    def _handle_mcu_identify(self):
        self.obj.find_objs()
        min_x, min_y = self.obj.bed_mesh.bmc.mesh_min
        max_x, max_y = self.obj.bed_mesh.bmc.mesh_max
        self.val.rdy_pos = [[min_x, min_y, self.cfg.bed_max_err + 1.],
                            [min_x, max_y, self.cfg.bed_max_err + 1.],
                            [max_x, max_y, self.cfg.bed_max_err + 1.],
                            [max_x, min_y, self.cfg.bed_max_err + 1.]]
        self.val.gap_pos = [[min_x + 1., min_y + 1., self.cfg.z_gap_00], # if self.cfg.stored_profs is None else self.cfg.stored_profs.getfloat('z_gap_00', default=self.cfg.z_gap_00, minval=0, maxval=1)],
                            [min_x + 1., max_y - 1., self.cfg.z_gap_01], # if self.cfg.stored_profs is None else self.cfg.stored_profs.getfloat('z_gap_01', default=self.cfg.z_gap_01, minval=0, maxval=1)],
                            [max_x - 1., max_y - 1., self.cfg.z_gap_11], # if self.cfg.stored_profs is None else self.cfg.stored_profs.getfloat('z_gap_11', default=self.cfg.z_gap_11, minval=0, maxval=1)],
                            [max_x - 1., min_y + 1., self.cfg.z_gap_10]] # if self.cfg.stored_profs is None else self.cfg.stored_profs.getfloat('z_gap_10', default=self.cfg.z_gap_10, minval=0, maxval=1)]]
        if self.cfg.clr_noz_start_x <= 0 or self.cfg.clr_noz_start_y <= 0 or self.cfg.clr_noz_len_x <= 0 or self.cfg.clr_noz_len_y <= 0:
            self.cfg.clr_noz_start_x = (max_x - min_x) * 1 / 3 + min_x
            self.cfg.clr_noz_start_y = max_y - 6
            self.cfg.clr_noz_len_x = (max_x - min_x) * 1 / 3
            self.cfg.clr_noz_len_y = 5
        self.val.home_xy = [(max_x - min_x) / 2 + min_x, (max_y - min_y) / 2 + min_y]
        pass

    def get_mcu(self):
        return self.obj.mcu

    def multi_probe_begin(self):
        pass

    def multi_probe_end(self):
        pass

    def probe_prepare(self, hmove):
        pass

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        return True

    def add_stepper(self, stepper):
        pass

    def get_steppers(self):
        return self.obj.dirzctl.steppers

    def ck_sys_sta(self):
        return not self.obj.hx711s.is_shutdown and not self.obj.hx711s.is_timeout and  not self.obj.dirzctl.is_shutdown and not self.obj.dirzctl.is_timeout

    def _ck_g28ed(self, is_precision=True):
        for i in range(3):
            if self.obj.kin.limits[i][0] > self.obj.kin.limits[i][1]:
                self.obj.gcode.run_script_from_command('G28')
                break
        pass

    def _move(self, pos, speed, wait=True):
        if not self.obj.hx711s.is_shutdown and not self.obj.hx711s.is_timeout and  not self.obj.dirzctl.is_shutdown and not self.obj.dirzctl.is_timeout:
            # self.obj.toolhead.manual_move(pos, speed)
            self.obj.gcode.run_script_from_command('G1 F%d X%.3f Y%.3f Z%.3f' % (speed * 60, pos[0], pos[1], pos[2]) if len(pos) >= 3 else 'G1 F%d X%.3f Y%.3f' % (speed * 60, pos[0], pos[1]))
            if wait:
                self.obj.toolhead.wait_moves()
            pass

    def _check_index(self, index):
        if (index <= self.cfg.pi_count - 3) and (index >= self.cfg.pi_count * 2 / 3):
            return True
        return False

    def _get_linear2(self, p1, p2, po, is_base_x):
        if (math.fabs(p1[0] - p2[0]) < 0.001 and is_base_x) or (math.fabs(p1[1] - p2[1]) < 0.001 and not is_base_x):
            return po
        a = (p2[2] - p1[2]) / ((p2[0] - p1[0]) if is_base_x else (p2[1] - p1[1]))
        b = p1[2] - (p1[0] if is_base_x else p1[1]) * a
        po[2] = a * (po[0] if is_base_x else po[1]) + b
        return po

    def _pnt_tri_msg(self, index, msg,  ary):
        if self.cfg.show_msg:
            self.pnt_msg('TRI SUCCESS BY: ' + msg)
            self.pnt_array('TRI CH=%d ARY=' % index, ary)
        pass

    def _check_trigger(self, arg_index, fit_vals, unfit_vals, min_hold, max_hold):
        all_params, tick = self.obj.dirzctl.get_params()
        if len(all_params) == 2:
            self._pnt_tri_msg(arg_index, 'Tri by Dirzctl run over!', fit_vals)
            return True
        fit_vals_t = [x for x in fit_vals]
        self.val.out_index = self.cfg.pi_count - 1
        if len(fit_vals) >= (self.cfg.pi_count / 2) and math.fabs(fit_vals[-1]) >= max_hold and \
                math.fabs(fit_vals[-2]) >= max_hold and math.fabs(fit_vals[-3]) >= max_hold:
            self._pnt_tri_msg(arg_index, 'Tri by out max_hold!', fit_vals)
            return True
        if len(fit_vals) != self.cfg.pi_count:
            return False           
        for i in range(1, self.cfg.pi_count - 1):
            if fit_vals_t[i] >= max_hold and fit_vals_t[i - 1] < (max_hold / 2) and fit_vals_t[i + 1] < (max_hold / 2):
                fit_vals_t[i] = fit_vals_t[i - 1]
        
        vals_p = [x for x in fit_vals_t]
        max_val = max(vals_p)
        min_val = min(vals_p)
        max_val += 1 if (max_val - min_val) == 0 else 0
        for i in range(len(vals_p)):
            vals_p[i] = (vals_p[i] - min_val) / (max_val - min_val)
        angle = math.atan((vals_p[-1] - vals_p[0]) / len(vals_p))
        sin_angle = math.sin(-angle)
        cos_angle = math.cos(-angle)
        for i in range(len(vals_p)):
            vals_p[i] = (i - 0) * sin_angle + (vals_p[i] - 0) * cos_angle + 0
        self.val.out_index = vals_p.index(min(vals_p))
        if(self.val.out_index > 0):
            for i in range(self.val.out_index, self.cfg.pi_count):
                fit_vals_t[self.val.out_index] = fit_vals_t[self.val.out_index] * (self.obj.filter.lft_k1_oft / 2) + fit_vals_t[self.val.out_index - 1] * (1 - (self.obj.filter.lft_k1_oft / 2))
        vals_p = [x for x in fit_vals_t]

        if not (fit_vals_t[-1] > fit_vals_t[-2] > fit_vals_t[-3]):
            return False
        max_val = max(fit_vals_t[0:(self.cfg.pi_count - 3)])
        if not ((fit_vals_t[-1] > max_val) and (fit_vals_t[-2] > max_val) and (fit_vals_t[-3] > max_val)):
            return False
        max_val = max(fit_vals_t)
        min_val = min(fit_vals_t)
        for i in range(0, self.cfg.pi_count):
            fit_vals_t[i] = (fit_vals_t[i] - min_val) / (max_val - min_val)
        for i in range(0, self.cfg.pi_count - 1):
            if (fit_vals_t[-1] - fit_vals_t[i]) / ((self.cfg.pi_count - i) * 1. / self.cfg.pi_count) < 0.8:
                return False
        if fit_vals[-1] < min_hold or fit_vals[-2] < (min_hold / 2) or fit_vals[-3] < (min_hold / 3):
            return False
        self._pnt_tri_msg(arg_index, 'Tri by fit all rule!', vals_p)
        return True

    def _set_hot_temps(self, temp, fan_spd, wait=False, err=5):
        self.obj.pheaters.set_temperature(self.obj.heater_hot, temp, False)
        self.obj.gcode.run_script_from_command('M106 P0 S%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('M106 P1 S%d' % (fan_spd))
        self.obj.gcode.run_script_from_command('M106 P2 S%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('SET_PIN PIN=fan0 VALUE=%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('SET_PIN PIN=fan1 VALUE=%d' % (fan_spd))
        # self.obj.gcode.run_script_from_command('SET_PIN PIN=fan2 VALUE=%d' % (fan_spd))
        if wait:
            while self.ck_sys_sta() and abs(self.obj.heater_hot.target_temp - self.obj.heater_hot.smoothed_temp) > err and self.obj.heater_hot.target_temp > 0:
                self.obj.hx711s.delay_s(0.100) 
        pass

    def _set_bed_temps(self, temp, wait=False, err=5):
        self.obj.pheaters.set_temperature(self.obj.heater_bed, temp, False)           
        if wait:
            while self.ck_sys_sta() and abs(self.obj.heater_bed.target_temp - self.obj.heater_bed.smoothed_temp) > err and self.obj.heater_bed.target_temp > 0:
                self.obj.hx711s.delay_s(0.100)          
        pass

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
            if math.fabs(o_mm0 - o_mm1) <= max_z_err or not self.ck_sys_sta():
                break
            self.val.re_probe_cnt += 1
            self.pnt_msg('***_probe_times must be reprobe= o_mm0=%.2f, o_mm1=%.2f' % (o_mm0, o_mm1))
        return o_mm
    
    def send_wave_tri(self, ch, ary):
        if self.cfg.tri_wave_ip is None:
            return
        msg = '%d' % ch
        for i in range(len(ary)):
            msg += ',%d' % ary[i]
        ss = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        ss.sendto(msg.encode(), (str(self.cfg.tri_wave_ip), 21021))
        ss.close()    

    def get_best_rdy_z(self, rdy_x, rdy_y, base_pos=None):
        if not base_pos:
            base_pos = self.val.rdy_pos
        p_left = [base_pos[0][0], rdy_y, 0]
        p_right = [base_pos[2][0], rdy_y, 0]
        p_mid = [rdy_x, rdy_y, 0]
        p_left = self._get_linear2(base_pos[0], base_pos[1], p_left, False)
        p_right = self._get_linear2(base_pos[2], base_pos[3], p_right, False)
        p_mid = self._get_linear2(p_left, p_right, p_mid, True)
        self.pnt_msg("Get best rdy z: Src=%s, x=%.2f, y=%.2f, cal_z=%.2f" % (('RDY' if base_pos == self.val.rdy_pos else 'GAP'), rdy_x, rdy_y, p_mid[2]))
        return p_mid[2] if p_mid[2] < self.cfg.bed_max_err else self.cfg.bed_max_err
                
    def shake_motor(self, cnt):
        max_velocity = self.obj.toolhead.max_velocity
        max_accel = self.obj.toolhead.max_accel
        max_z_velocity = self.obj.toolhead.kin.max_z_velocity
        max_z_accel = self.obj.toolhead.kin.max_z_accel

        self.obj.toolhead.max_velocity = self.cfg.shake_max_velocity
        self.obj.toolhead.max_accel = self.cfg.shake_max_accel
        self.obj.toolhead.kin.max_z_velocity = self.cfg.shake_max_velocity
        self.obj.toolhead.kin.max_z_accel = self.cfg.shake_max_accel

        now_pos = self.obj.toolhead.get_position()
        for i in range(int(cnt / 2)):
            self.obj.gcode.run_script_from_command('G1 X%.2f Y%.2f Z%.2f F600' % (now_pos[0] - self.cfg.shake_range, now_pos[1] - self.cfg.shake_range, now_pos[2] - self.cfg.shake_range / 2))
            self.obj.gcode.run_script_from_command('G1 X%.2f Y%.2f Z%.2f F600' % (now_pos[0] + self.cfg.shake_range, now_pos[1] - self.cfg.shake_range, now_pos[2] + self.cfg.shake_range / 2))
            self.obj.gcode.run_script_from_command('G1 X%.2f Y%.2f Z%.2f F600' % (now_pos[0] + self.cfg.shake_range, now_pos[1] + self.cfg.shake_range, now_pos[2] - self.cfg.shake_range / 2))
            self.obj.gcode.run_script_from_command('G1 X%.2f Y%.2f Z%.2f F600' % (now_pos[0] - self.cfg.shake_range, now_pos[1] + self.cfg.shake_range, now_pos[2] + self.cfg.shake_range / 2))
            while len(self.obj.toolhead.move_queue.queue) >= 4 and self.ck_sys_sta():
                self.obj.hx711s.delay_s(0.010)
        self._move(now_pos, self.cfg.g29_xy_speed)

        self.obj.toolhead.max_velocity = max_velocity
        self.obj.toolhead.max_accel = max_accel
        self.obj.toolhead.kin.max_z_velocity = max_z_velocity
        self.obj.toolhead.kin.max_z_accel = max_z_accel
        pass

    def clear_nozzle(self, hot_min_temp, hot_max_temp, bed_max_temp, min_hold, max_hold):
        min_x, min_y = self.cfg.clr_noz_start_x, self.cfg.clr_noz_start_y
        max_x, max_y = self.cfg.clr_noz_start_x + self.cfg.clr_noz_len_x, self.cfg.clr_noz_start_y + self.cfg.clr_noz_len_y
        self._set_bed_temps(temp=bed_max_temp, wait=False)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=False, err=10)
        self._ck_g28ed(False)
        random.seed(time.time())  
        cur_pos = self.obj.toolhead.get_position()
        src_pos = [min_x + random.uniform(0, self.cfg.clr_noz_len_x - self.cfg.pa_clr_dis_mm - 5), 
                   min_y + random.uniform(0, self.cfg.clr_noz_len_y), self.cfg.bed_max_err + 1, cur_pos[3]]
        end_pos = [src_pos[0] + self.cfg.pa_clr_dis_mm, src_pos[1], src_pos[2], src_pos[3]]
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=True, err=10)   
        src_pos[2] = self._probe_times(3, [src_pos[0] - 5, src_pos[1], src_pos[2]], self.cfg.g29_speed, 10, 0.2, min_hold * 2, max_hold)
        self._set_hot_temps(temp=hot_min_temp + 40, fan_spd=0, wait=False, err=10)
        end_pos[2] = self._probe_times(3, [end_pos[0] - 5, end_pos[1], end_pos[2]], self.cfg.g29_speed, 10, 0.2, min_hold * 2, max_hold)     
        self._move(src_pos[:2] + [self.cfg.bed_max_err + 1], self.cfg.g29_xy_speed) 
        self._move(src_pos[:2] + [src_pos[2] + 0.2], self.cfg.g29_rdy_speed) 
        self._set_hot_temps(temp=hot_max_temp, fan_spd=0, wait=True, err=10)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=False)
        self._move(end_pos[:2] + [end_pos[2] + self.cfg.pa_clr_down_mm], self.cfg.g29_speed)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=255, wait=True, err=5)
        self._move([end_pos[0] + 10, end_pos[1], end_pos[2] + 10], self.cfg.g29_speed)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=False) 
        self._set_bed_temps(temp=bed_max_temp, wait=True, err=5)

        self._move(self.val.home_xy + [10], self.cfg.g29_xy_speed)
        # self.obj.gcode.run_script_from_command('G28 Z')
        pass

    def measure_gap(self, zero_z):
        min_dis_mm = self.cfg.gap_dis_range
        speed_mm = self.cfg.gap_dis_range
        p0_vals = None
        p1_vals = None

        rd_cnt = int(2 * 80 * (min_dis_mm / speed_mm))            
        step_cnt = int(min_dis_mm / (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base))
        step_us = int(((min_dis_mm / speed_mm) * 1000 * 1000) / step_cnt)

        now_pos = self.obj.toolhead.get_position()  
        self._move(now_pos[:2] + [zero_z + min_dis_mm / 2, now_pos[3]], self.cfg.g29_rdy_speed)
        self.obj.hx711s.read_base(int(self.cfg.base_count / 2), self.cfg.max_hold)

        self.obj.hx711s.query_start(rd_cnt, rd_cnt, del_dirty=False, show_msg=False)       
        self.obj.dirzctl.check_and_run(0, int(step_us), int(step_cnt), wait_finish=True)
        p0_valss = self.obj.hx711s.get_vals()

        self.obj.hx711s.query_start(rd_cnt, rd_cnt, del_dirty=False, show_msg=False)       
        self.obj.dirzctl.check_and_run(1, int(step_us), int(step_cnt), wait_finish=True)
        p1_valss = self.obj.hx711s.get_vals()

        self.obj.hx711s.query_start(rd_cnt, int(0), del_dirty=False, show_msg=False)
        if len(p0_valss[0]) == 0 or len(p1_valss[0]) == 0:
            self.pnt_msg('measure_gap: Error! Cannot recv datas from hx711s!!!.')
            return 0
        
        self.pnt_msg('---------------------------------')
        for i in range(int(self.obj.hx711s.s_count)):
            self.pnt_array('p0_%d_valss = ' % i, p0_valss[i], len(p0_valss[i]))
        self.pnt_msg('---------------------------------')
        for i in range(int(self.obj.hx711s.s_count)):
            self.pnt_array('p1_%d_valss = ' % i, p1_valss[i], len(p1_valss[i]))
        self.pnt_msg('---------------------------------')

        p0_diss = []
        p1_diss = []
        gaps = []
        for gap_index in range(int(self.obj.hx711s.s_count)):
            p0_vals = p0_valss[gap_index]
            p1_vals = p1_valss[gap_index]

            if p0_vals[0] > p0_vals[-1]:
                p0_vals = [item * -1 for item in p0_vals]

            if p1_vals[0] < p1_vals[-1]:
                p1_vals = [item * -1 for item in p1_vals]

            max_val = max(p0_vals)
            min_val = min(p0_vals)
            for i in range(len(p0_vals)):
                p0_vals[i] = (p0_vals[i] - min_val) / (max_val - min_val)
            angle = math.atan((p0_vals[-1] - p0_vals[0]) / len(p0_vals))
            sin_angle = math.sin(-angle)
            cos_angle = math.cos(-angle)
            for i in range(len(p0_vals)):
                p0_vals[i] = (i - 0) * sin_angle + (p0_vals[i] - 0) * cos_angle + 0
            p0_out_index = p0_vals.index(min(p0_vals))
            p0_dis = (len(p0_vals) - p0_out_index) * 0.012 * speed_mm
            p0_diss.append(p0_dis)

            p1_vals = p1_vals[::-1]
            max_val = max(p1_vals)
            min_val = min(p1_vals)
            for i in range(len(p1_vals)):
                p1_vals[i] = (p1_vals[i] - min_val) / (max_val - min_val)
            angle = math.atan((p1_vals[-1] - p1_vals[0]) / len(p1_vals))
            sin_angle = math.sin(-angle)
            cos_angle = math.cos(-angle)
            for i in range(len(p1_vals)):
                p1_vals[i] = (i - 0) * sin_angle + (p1_vals[i] - 0) * cos_angle + 0
            p1_out_index = p1_vals.index(min(p1_vals))
            p1_dis = (len(p1_vals) - p1_out_index) * 0.012 * speed_mm
            p1_diss.append(p1_dis)

            gaps.append(p1_dis - p0_dis)
            pass

        self.pnt_array('measure_gap: p0_diss = ', p0_diss, len(p0_diss))
        self.pnt_array('measure_gap: p1_diss = ', p1_diss, len(p1_diss))
        self.pnt_array('measure_gap: gaps = ', gaps, len(gaps))      

        v_cnt = int(0)
        v_gap = 0.
        for i in range(int(self.obj.hx711s.s_count)):
            if not ((0 < p0_diss[i] < min_dis_mm) and (0 < p1_diss[i] < min_dis_mm) and (0 <= gaps[i] < 0.2)):
                continue
            v_cnt += 1
            v_gap += (gaps[i] if gaps[i] <= 0.1 else 0.1)
            pass

        v_gap = (0 if v_cnt == 0 else (v_gap / v_cnt))


        self.pnt_msg('measure_gap: v_cnt=%d v_gap = %.2f' % (v_cnt, v_gap)) 

        return v_gap

    def _gap_times(self, max_times, zero_pos):
        gaps = []
        now_pos = self.obj.toolhead.get_position()          
        for i in range(max_times):
            self._move(now_pos[:2] + [zero_pos + 2, now_pos[3]], self.cfg.g29_rdy_speed)
            self.shake_motor(int(self.cfg.shake_cnt / 2))
            gaps.append(self.measure_gap(zero_pos))
        gaps.sort()
        self.pnt_array('Gap measure vals = ', gaps, len(gaps))
        now_pos = self.obj.toolhead.get_position()  
        self._move(now_pos[:2] + [self.cfg.bed_max_err + 1., now_pos[3]], self.cfg.g29_rdy_speed)
        return gaps[int((max_times + 1) / 2)]

    def probe_ready(self):
        if self.val.jump_probe_ready:
            self.val.jump_probe_ready = False
            return False
        self._ck_g28ed()
        min_x, min_y = self.obj.bed_mesh.bmc.mesh_min
        max_x, max_y = self.obj.bed_mesh.bmc.mesh_max
        random.seed(time.time()) 
        self.val.rdy_pos = [[min_x, min_y, self.cfg.bed_max_err + 1.],
                            [min_x, max_y, self.cfg.bed_max_err + 1.],
                            [max_x, max_y, self.cfg.bed_max_err + 1.],
                            [max_x, min_y, self.cfg.bed_max_err + 1.]]
        mesh = self.obj.bed_mesh.get_mesh()
        self.obj.bed_mesh.set_mesh(None)
        for i in range(4):
            self.val.rdy_pos[i][2] = self._probe_times(3, self.val.rdy_pos[i], self.cfg.g29_speed, 10, 0.2, self.cfg.min_hold, self.cfg.max_hold)
            # if self.cfg.need_measure_gap:
            #     self.val.gap_pos[i] = [x for x in self.val.rdy_pos[i]]
            #     self.val.gap_pos[i][2] = self._gap_times(3, self.val.rdy_pos[i][2])
            # else:
            #     self.val.gap_pos[i][2] = 0
            pass
        if self.cfg.need_measure_gap:        
            configfile = self.obj.printer.lookup_object('configfile')
            configfile.set('prtouch default', 'z_gap_00', self.val.gap_pos[0][2])
            configfile.set('prtouch default', 'z_gap_01', self.val.gap_pos[1][2])
            configfile.set('prtouch default', 'z_gap_11', self.val.gap_pos[2][2])
            configfile.set('prtouch default', 'z_gap_10', self.val.gap_pos[3][2])
        self.pnt_msg("RDY_POS = [00=%.2f, 01=%.2f, 11=%.2f, 10=%.2f]" % (self.val.rdy_pos[0][2], self.val.rdy_pos[1][2], self.val.rdy_pos[2][2], self.val.rdy_pos[3][2]))
        self.pnt_msg("GAP_POS = [00=%.2f, 01=%.2f, 11=%.2f, 10=%.2f]" % (self.val.gap_pos[0][2], self.val.gap_pos[1][2], self.val.gap_pos[2][2], self.val.gap_pos[3][2]))
        self.obj.bed_mesh.set_mesh(mesh)
        pass

    def check_bed_mesh(self, auto_g29=True):
        min_x, min_y = self.obj.bed_mesh.bmc.mesh_min
        max_x, max_y = self.obj.bed_mesh.bmc.mesh_max
        self.val.rdy_pos = [[min_x + random.uniform(2.0, +5.0), min_y + random.uniform(2.0, +5.0), self.cfg.bed_max_err + 1.],
                            [min_x + random.uniform(2.0, +5.0), max_y - random.uniform(2.0, +5.0), self.cfg.bed_max_err + 1.],
                            [max_x - random.uniform(2.0, +5.0), max_y - random.uniform(2.0, +5.0), self.cfg.bed_max_err + 1.],
                            [max_x - random.uniform(2.0, +5.0), min_y + random.uniform(2.0, +5.0), self.cfg.bed_max_err + 1.]]
        err_cnt = int(0)
        self.val.jump_probe_ready = True
        mesh = self.obj.bed_mesh.get_mesh()
        if mesh is None:
            if auto_g29:
                self.pnt_msg("The bed_mesh data is invalid and cannot be verified.")
                self._ck_g28ed()   
                self.obj.gcode.run_script_from_command('BED_MESH_CALIBRATE')
                self.obj.gcode.run_script_from_command('CXSAVE_CONFIG')
            else:
                #raise self.obj.printer.command_error("The bed_mesh data is invalid and cannot be verified.")
                raise self.obj.printer.command_error("""{"code":"key504", "msg":"The bed_mesh data is invalid and cannot be verified."}""")
            return

        self.obj.bed_mesh.set_mesh(None)
        self._ck_g28ed()        
        self._move([self.val.home_xy[0], self.val.home_xy[1], self.cfg.bed_max_err + 1.0, self.obj.toolhead.get_position()[3]], self.cfg.g29_xy_speed)     
        for i in range(4):
            self.val.rdy_pos[i][2] = self._probe_times(3, self.val.rdy_pos[i], self.cfg.g29_speed, 10, self.cfg.check_bed_mesh_max_err * 2, self.cfg.min_hold, self.cfg.max_hold) + self.cfg.fix_z_offset
            self.val.rdy_pos[i][2] += self.get_best_rdy_z(self.val.rdy_pos[i][0], self.val.rdy_pos[i][1], self.val.gap_pos) if self.cfg.need_measure_gap else 0
            pass       
        self.obj.bed_mesh.set_mesh(mesh)
        errs = []
        for i in range(4):
            mesh_z = self.obj.bed_mesh.z_mesh.calc_z(self.val.rdy_pos[i][0], self.val.rdy_pos[i][1])
            errs.append(abs(self.val.rdy_pos[i][2] - mesh_z))
            err_cnt += (1 if errs[i] > self.cfg.check_bed_mesh_max_err else 0)
            self.pnt_msg('P%d = [x=%.2f, y=%.2f, mest_z=%.2f, probe_z=%.2f, err_z=%.2f]' % (i, self.val.rdy_pos[i][0], self.val.rdy_pos[i][1], mesh_z, self.val.rdy_pos[i][2], errs[i]))        
        if err_cnt >= 2:
            if auto_g29:
                self.pnt_array("check_bed_mesh: Due to the great change of the hot bed, it needs to be re-leveled. errs = ", errs, len(errs))
                self.val.g29_cnt = 0
                self.obj.gcode.run_script_from_command('BED_MESH_CALIBRATE')
                self.obj.gcode.run_script_from_command('CXSAVE_CONFIG')
            else:
                # raise self.obj.printer.command_error("check_bed_mesh: Due to the large change of the hot bed, it can not print normally!! Errs = [%.2f, %.2f, %.2f, %.2f]" \
                #                                         % (errs[0], errs[1], errs[2], errs[3]))
                raise self.obj.printer.command_error("""{"code":"key501", "msg":"check_bed_mesh: Due to the large change of the hot bed, it can not print normally. Errs = [%.2f, %.2f, %.2f, %.2f]"}""" \
                                                        % (errs[0], errs[1], errs[2], errs[3]))
                                                        
        else:
            self.pnt_array("check_bed_mesh: Errs = ", errs, len(errs))
        pass

    def raise_z_bef_g28(self):
        if self.obj.kin.limits[2][0] <= self.obj.kin.limits[2][1] and self.obj.toolhead.get_position()[2] < 5:
            self.obj.toolhead.manual_move(self.obj.toolhead.get_position()[:2] + [5], 5)
            self.obj.toolhead.wait_moves()
            pass

    def _cal_min_z(self, start_z, hx711_vals):
        hx711_params, hx711_start_tick = self.obj.hx711s.get_params()
        dirzctl_params, dirzctl_start_tick = self.obj.dirzctl.get_params()
        if dirzctl_params is None or len(dirzctl_params) != 2:     
            raise self.obj.printer.command_error("""{"code":"key502", "msg":"probe_by_step: Can not recv stepper-z status."}""")
        if len(hx711_vals) < self.cfg.pi_count or len(hx711_params) < self.cfg.pi_count:
            up_all_cnt = dirzctl_params[0]['step'] - dirzctl_params[1]['step'] + 1
            return up_all_cnt, up_all_cnt, False
        del hx711_params[0:(len(hx711_params) - self.cfg.pi_count)]

        vals_p = [x for x in hx711_vals]
        max_val = max(vals_p)
        min_val = min(vals_p)
        for i in range(len(vals_p)):
            vals_p[i] = (vals_p[i] - min_val) / (max_val - min_val)
        angle = math.atan((vals_p[-1] - vals_p[0]) / len(vals_p))
        sin_angle = math.sin(-angle)
        cos_angle = math.cos(-angle)
        for i in range(len(vals_p)):
            vals_p[i] = (i - 0) * sin_angle + (vals_p[i] - 0) * cos_angle + 0
        self.val.out_index = vals_p.index(min(vals_p))

        dirzctl_params[0]['tick'] = (dirzctl_params[0]['tick'] - dirzctl_start_tick) / self.obj.dirzctl.mcu_freq
        dirzctl_params[1]['tick'] = ((4294967296 if dirzctl_params[1]['tick'] < dirzctl_start_tick else 0) + dirzctl_params[1]['tick'] - dirzctl_start_tick) / self.obj.dirzctl.mcu_freq
        dirzctl_params[0]['z'] = start_z
        dirzctl_params[1]['z'] = start_z - (dirzctl_params[0]['step'] - dirzctl_params[1]['step'] + 1) * (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base)
        tick_p = ((4294967296 if hx711_params[self.val.out_index]['nt'] < hx711_start_tick else 0) + hx711_params[self.val.out_index]['nt'] - hx711_start_tick) / self.obj.hx711s.mcu_freq
        self.val.out_val_mm = self._get_linear2([dirzctl_params[0]['tick'], 0, dirzctl_params[0]['z']], [dirzctl_params[1]['tick'], 0, dirzctl_params[1]['z']], [tick_p, 0, 0], True)[2]
        self.pnt_msg('call_min_z, re_probe_cnt=%d, out_index=%d, out_val_mm=%.2f' % (self.val.re_probe_cnt, self.val.out_index, self.val.out_val_mm))
        up_min_cnt = int((self.val.out_val_mm - dirzctl_params[1]['z']) / (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base))
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
        while self.ck_sys_sta():
            self.obj.hx711s.send_heart_beat()
            self.obj.dirzctl.send_heart_beat()
            all_valss = self.obj.hx711s.get_vals()
            if all_valss is None or len(all_valss[0]) == 0:
                self.obj.hx711s.delay_s(0.005)
                continue
            unfit_vals, tmp_unfit_vals = self.obj.filter.cal_offset_by_vals(self.obj.hx711s.s_count, all_valss, self.obj.filter.lft_k1_oft, self.cfg.pi_count)
            fit_vals, tmp_fit_vals = self.obj.filter.cal_filter_by_vals(self.obj.hx711s.s_count, all_valss, self.obj.filter.hft_hz, self.obj.filter.lft_k1, self.cfg.pi_count)
            
            for i in range(self.obj.hx711s.s_count):
                if not self._check_trigger(i, tmp_fit_vals[i], tmp_unfit_vals[i], min_hold, max_hold):
                    continue
                self.obj.dirzctl.check_and_run(0, 0, 0, wait_finish=False)
                self.obj.hx711s.query_start(self.cfg.pi_count * 2, int(0), del_dirty=False, show_msg=False)
                self.obj.hx711s.delay_s(0.015)
                for j in range(int(self.obj.hx711s.s_count)):
                    self.pnt_array('TRIGGER_USE_CH=%d, FIT_VALS=' % (j), tmp_fit_vals[j], 16)
                self.obj.hx711s.delay_s(0.2)
                all_valss = self.obj.hx711s.get_vals()
                self.pnt_array('WAIT_AND_CAL_CH=%d, ARY=' % (i), all_valss[i])
                hx711_vals, tmp_hx711_vs = self.obj.filter.cal_filter_by_vals(self.obj.hx711s.s_count, all_valss, self.obj.filter.hft_hz, self.obj.filter.lft_k1_cal, self.cfg.pi_count)
                self.pnt_array('WAIT_AND_CAL_CH=%d, ARY=' % (i), tmp_hx711_vs[i])
                for m in range(self.obj.hx711s.s_count):
                    self.send_wave_tri(m, tmp_hx711_vs[m])
                up_min_cnt, up_all_cnt, deal_sta = self._cal_min_z(rdy_pos[2], tmp_hx711_vs[i])
                if up_after:
                    self.obj.dirzctl.check_and_run(1, int(step_us / 2), int(up_all_cnt))
                return self.val.out_index, self.val.out_val_mm, deal_sta
            self.obj.hx711s.delay_s(0.005)
        return self.val.out_index, self.val.out_val_mm, True

    def run_G28_Z(self):
        self.pnt_msg('***run_G28_Z*** Start...')
        self.val.re_probe_cnt = 0
        self.val.g29_cnt = 0
        self.obj.toolhead.wait_moves()
        target_temp = self.obj.heater_hot.target_temp
        now_pos = self.obj.toolhead.get_position()
        self.val.home_xy = [now_pos[0], now_pos[1]]
        now_pos[2] = 2
        random.seed(int(time.time()))  
        now_pos_sta0 = [now_pos[0] + (1 if (int(time.time() * 1000) % 2 == 0) else -1) * random.uniform(10, 20),
                        now_pos[1] + (1 if (int(time.time() * 100) % 2 == 0) else -1) * random.uniform(10, 20),
                        now_pos[2], now_pos[3]]
        mesh = self.obj.bed_mesh.get_mesh()
        self.obj.bed_mesh.set_mesh(None)                
        self.obj.toolhead.set_position(now_pos, homing_axes=[2])
        self._move(now_pos_sta0, 20)
        if self.cfg.g28_wait_cool_down and self.obj.heater_hot.smoothed_temp > (self.cfg.hot_min_temp + 5):
            self.pnt_msg('G28_Z: Wait for Nozzle to cool down[%.2f -> %.2f]...' % (target_temp, self.cfg.hot_min_temp))
            self._set_hot_temps(temp=self.cfg.hot_min_temp, fan_spd=255, wait=True, err=5) 
            self._set_hot_temps(temp=self.cfg.hot_min_temp, fan_spd=0, wait=False, err=5) 
        if self.cfg.max_dis_bef_g28 != 0:
            step_cnt = int(self.cfg.max_dis_bef_g28 / (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base))
            step_us = int(((self.cfg.max_dis_bef_g28 / self.cfg.g28_sta0_speed) * 1000 * 1000) / step_cnt)
            self.obj.dirzctl.check_and_run(1, step_us, step_cnt)
        if self.cfg.dead_zone_bef_g28 != 0:
            step_cnt = int(self.cfg.dead_zone_bef_g28 / (self.obj.dirzctl.steppers[0].get_step_dist() * self.obj.dirzctl.step_base))
            step_us = int(((self.cfg.dead_zone_bef_g28 / self.cfg.g28_sta0_speed) * 1000 * 1000) / step_cnt)
            self.obj.dirzctl.check_and_run(0, step_us, step_cnt)
        is_uped = False
        for i in range(10):
            self.obj.toolhead.set_position(now_pos_sta0[:2] + [(2 if i == 0 else 0), now_pos_sta0[3]], homing_axes=[2])
            self._move(now_pos_sta0[:3], self.cfg.g29_rdy_speed)
            out_index, out_mm, deal_sta = self.probe_by_step(now_pos_sta0, self.cfg.g28_sta0_speed, self.cfg.max_z, self.cfg.g28_sta0_min_hold, self.cfg.max_hold, up_after=False)
            if not deal_sta and not is_uped:
                is_uped = True
                self.obj.toolhead.set_position(now_pos_sta0[:2] + [0, now_pos_sta0[3]], homing_axes=[2])
                self._move(now_pos_sta0[:2] + [5, now_pos_sta0[3]], self.cfg.g29_rdy_speed)
                pass
            if (-1 < out_mm < 1 and deal_sta) or not self.ck_sys_sta():
                break
        self.obj.toolhead.set_position(now_pos_sta0[:2] + [-0.5, now_pos[3]], homing_axes=[2])
        self._move(now_pos_sta0[:2] + [2, now_pos[3]], self.cfg.g29_rdy_speed)

        now_pos = [now_pos[0] + random.uniform(-1.0, +1.0), now_pos[1] + random.uniform(-1.0, +1.0), 2, now_pos[3]]
        self._move(now_pos, 30)
        self.shake_motor(int(self.cfg.shake_cnt * 2))
        out_mms = []
        for i in range(5):
            o_index0, o_mm0, deal_sta = self.probe_by_step(now_pos, self.cfg.g28_sta1_speed, self.cfg.max_z, self.cfg.min_hold, self.cfg.max_hold, True)
            out_mms.append(o_mm0)
        out_mms.sort()
        self.pnt_array('G28_ZS = ', out_mms, len(out_mms))
        if math.fabs(out_mms[3] - out_mms[1]) > 5.0:
             raise self.obj.printer.command_error("""{"code":"key504", "msg":"run_G28_Z: Homing Z failure, During zeroing, please place the machine on a stable platform and do not touch the hot bed."}""")
        self.obj.toolhead.set_position(now_pos[:2] + [now_pos[2] - out_mms[2] - self.cfg.self_z_offset, now_pos[3]], homing_axes=[2])

        self.obj.bed_mesh.set_mesh(mesh)

        # self.obj.gcode.run_script_from_command('G1 F300 Z10')
        self._move(now_pos[:2] + [10, now_pos[3]], self.cfg.g29_rdy_speed)

        if self.cfg.g28_wait_cool_down:
            self.pnt_msg('G28_Z: Wait for Nozzle to recovery[%.2f -> %.2f]...' % (self.cfg.hot_min_temp, target_temp))
            self._set_hot_temps(temp=target_temp, fan_spd=0, wait=True if target_temp > self.cfg.hot_min_temp else False, err=5)
        pass

    def run_G29_Z(self):
        x_cnt = self.obj.bed_mesh.bmc.mesh_config['x_count']
        y_cnt = self.obj.bed_mesh.bmc.mesh_config['y_count']
        self.obj.toolhead.wait_moves()
        now_pos = self.obj.toolhead.get_position()
        self.val.jump_probe_ready = False

        if self.val.g29_cnt == 0:
            self.shake_motor(self.cfg.shake_cnt)
            self.probe_ready()
            now_pos[2] = self.get_best_rdy_z(now_pos[0], now_pos[1], self.val.rdy_pos) + self.cfg.best_above_z
            self._move(now_pos, self.cfg.g29_xy_speed)        

        if (int(self.val.g29_cnt) % int(x_cnt)) == 0:
            self.shake_motor(self.cfg.shake_cnt)
            pass

        now_pos[2] = self._probe_times(10, now_pos, self.cfg.g29_speed, self.cfg.bed_max_err, 0.06, self.cfg.min_hold, self.cfg.max_hold) + self.get_best_rdy_z(now_pos[0], now_pos[1], self.val.gap_pos) + self.cfg.fix_z_offset
       
        self.val.g29_cnt += 1
        if self.val.g29_cnt == x_cnt * y_cnt:
            self.val.g29_cnt = 0
            self._move([now_pos[0], now_pos[1], self.cfg.bed_max_err + 1], self.cfg.g29_rdy_speed) 
            self._move([self.val.home_xy[0], self.val.home_xy[1], self.cfg.bed_max_err + 1], self.cfg.g29_xy_speed)  
            home_z = self._probe_times(3, [self.val.home_xy[0], self.val.home_xy[1], self.cfg.bed_max_err + 1], 
                                       self.cfg.g29_speed, self.cfg.bed_max_err * 2, 0.20, self.cfg.min_hold, self.cfg.max_hold)
            self.pnt_msg('CHECK_STEP_LOST need=0, tri=%.2f' % home_z)
            if home_z > 1.0:
                raise self.obj.printer.command_error("""{"code":"key503", "msg":"run_G29_Z: Z-axis motor lost of step detected, Please restart the machine and try again"}""")

        return now_pos

    cmd_PRTOUCH_TEST_help = "Test the PR-Touch."
    def cmd_PRTOUCH_TEST(self, gcmd):   # PRTOUCH_TEST X=20 Y=20 Z=-4 S=0.0125
        pos = self.obj.toolhead.get_position()
        rdy_x = gcmd.get_float('X', pos[0])
        rdy_y = gcmd.get_float('Y', pos[1])
        rdy_z = gcmd.get_float('Z', pos[2])
        speed = gcmd.get_float('SPEED', 1.0)
        min_hold = gcmd.get_int('MIN_HOLD', self.cfg.min_hold)
        max_hold = gcmd.get_int('MAX_HOLD', self.cfg.max_hold)
        self._move([rdy_x, rdy_y, rdy_z], self.cfg.g29_xy_speed)
        self.probe_by_step([rdy_x, rdy_y, rdy_z], speed, 50, min_hold, max_hold, True)
        pass

    cmd_PRTOUCH_READY_help = "Test the ready point."
    def cmd_PRTOUCH_READY(self, gcmd):
        self.probe_ready()
        pass

    cmd_CHECK_BED_MESH_help = "Check the bed mesh."
    def cmd_CHECK_BED_MESH(self, gcmd):
        self.check_bed_mesh(gcmd.get_int('AUTO_G29', 0) > 0)
        pass

    cmd_MEASURE_GAP_TEST_help = "Measure z gap."
    def cmd_MEASURE_GAP_TEST(self, gcmd):
        self._ck_g28ed()
        gap_index = gcmd.get_int('INDEX', 0)
        n_index = [0, 3, 1, 2]
        self._move([self.val.rdy_pos[n_index[gap_index]][0], self.val.rdy_pos[n_index[gap_index]][1], self.cfg.bed_max_err + 1.], self.cfg.g29_xy_speed)  
        zero_z = self._probe_times(10, [self.val.rdy_pos[n_index[gap_index]][0], self.val.rdy_pos[n_index[gap_index]][1], self.cfg.bed_max_err + 1.], 
                                        self.cfg.g29_speed, 10, 0.05, self.cfg.min_hold, self.cfg.max_hold) 
        self._gap_times(5, zero_z)
        pass

    cmd_NOZZLE_CLEAR_help = "Clear the nozzle on bed."
    def cmd_NOZZLE_CLEAR(self, gcmd):
        hot_min_temp = gcmd.get_float('HOT_MIN_TEMP', self.cfg.hot_min_temp)
        hot_max_temp = gcmd.get_float('HOT_MAX_TEMP', self.cfg.hot_max_temp)
        bed_max_temp = gcmd.get_float('BED_MAX_TEMP', self.cfg.bed_max_temp)
        min_hold = gcmd.get_int('MIN_HOLD', self.cfg.min_hold)
        max_hold = gcmd.get_int('MAX_HOLD', self.cfg.max_hold)
        self.clear_nozzle(hot_min_temp, hot_max_temp, bed_max_temp, min_hold, max_hold)

    def change_hot_min_temp(self, temp):
        self.cfg.hot_min_temp = temp

def load_config(config):
    prt = PRTouchEndstopWrapper(config)
    config.get_printer().add_object('probe', probe.PrinterProbe(config, prt))
    return prt

# G28
# READ_HX711 C=1
# NOZZLE_CLEAR
# PRTOUCH_READY
# BED_MESH_CALIBRATE
# CHECK_BED_MESH AUTO_G29=0
# MEASURE_GAP_TEST INDEX=0
# FORCE_MOVE STEPPER=stepper_x DISTANCE=10 VELOCITY=5

# g28_sta0_speed、g28_sta1_speed和g29_speed都改为1.8