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

class PRTouchCFG:
    def __init__(self, config):
        self.base_count = config.getint('base_count', default=40, minval=10, maxval=100)
        self.pi_count = config.getint('pi_count', default=32, minval=16, maxval=128)
        self.min_hold = config.getint('min_hold', default=3000, minval=100, maxval=50000)
        self.max_hold = config.getint('max_hold', default=50000, minval=100, maxval=100000)
        self.sensor_x = config.getfloat('sensor_x', minval=0, maxval=300)
        self.sensor_y = config.getfloat('sensor_y', minval=0, maxval=300)
        self.random_offset = config.getfloat('sensor_random_offset', default=5, minval=0, maxval=10)
        self.hot_min_temp = config.getfloat('s_hot_min_temp', default=140, minval=80, maxval=200)
        self.hot_max_temp = config.getfloat('s_hot_max_temp', default=200, minval=180, maxval=300)
        self.bed_max_temp = config.getfloat('s_bed_max_temp', default=60, minval=45, maxval=100)
        self.pa_clr_dis_mm = config.getint('pa_clr_dis_mm', default=5, minval=2, maxval=100)
        self.pa_clr_down_mm = config.getfloat('pa_clr_down_mm', default=-0.1, minval=-1, maxval=1)
        self.clr_noz_start_x = config.getfloat('clr_noz_start_x', default=0, minval=0, maxval=1000)
        self.clr_noz_start_y = config.getfloat('clr_noz_start_y', default=0, minval=0, maxval=1000)
        self.clr_noz_len_x = config.getfloat('clr_noz_len_x', default=0, minval=0, maxval=1000)
        self.clr_noz_len_y = config.getfloat('clr_noz_len_y', default=0, minval=self.pa_clr_dis_mm + 5, maxval=1000)
        self.bed_max_err = config.getint('bed_max_err', default=4, minval=2, maxval=10)
        self.max_z = config.getsection('stepper_z').getfloat('position_max', default=300, minval=100, maxval=500)
        self.g29_xy_speed = config.getfloat('g29_xy_speed', default=150, minval=10, maxval=1000)
        self.g29_rdy_speed = config.getfloat('g29_rdy_speed', default=2.5, minval=0.1, maxval=10)
        self.probe_speed = config.getfloat('probe_speed', default=2.0, minval=0.1, maxval=10)
        self.show_msg = config.getboolean('show_msg', default=False)
        self.check_bed_mesh_max_err = config.getfloat('check_bed_mesh_max_err', default=0.2, minval=0.01, maxval=1)
        self.wipe_retract_distance = config.getfloat('wipe_retract_distance', default=0, minval=0, maxval=50)

        self.stored_profs = config.get_prefix_sections('prtouch')
        self.stored_profs = self.stored_profs[1] if (len(self.stored_profs) == 2) else None
        pass


class PRTouchVAL:
    def __init__(self, config):
        self.out_index = 0
        self.out_val_mm = 0.
        self.re_probe_cnt = 0
        self.home_xy = None
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
        self.probe = None
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.lookup_object('gcode_move')
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
        self.probe = self.printer.lookup_object('probe', None)
        pass


class PRTouchZOffsetWrapper:
    def __init__(self, config):
        self.cfg = PRTouchCFG(config)
        self.val = PRTouchVAL(config)
        self.obj = PRTouchOBJ(config)

        self.obj.printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self.obj.gcode.register_command('PRTOUCH_PROBE_ZOFFSET', self.cmd_PRTOUCH_PROBE_ZOFFSET, desc=self.cmd_PRTOUCH_PROBE_ZOFFSET_help)
        self.obj.gcode.register_command('NOZZLE_CLEAR', self.cmd_NOZZLE_CLEAR, desc=self.cmd_NOZZLE_CLEAR_help)
        self.obj.gcode.register_command('PRTOUCH_ACCURACY', self.cmd_PRTOUCH_ACCURACY, desc=self.cmd_PRTOUCH_ACCURACY_help)
        pass

    def _handle_mcu_identify(self):
        self.obj.find_objs()
        min_x, min_y = self.obj.bed_mesh.bmc.mesh_min
        max_x, max_y = self.obj.bed_mesh.bmc.mesh_max
        self.val.home_xy = [(max_x - min_x) / 2 + min_x, (max_y - min_y) / 2 + min_y]
        pass

    def get_mcu(self):
        return self.obj.mcu

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
            if math.fabs(o_mm0 - o_mm1) <= max_z_err or not self.ck_sys_sta():
                break
            self.val.re_probe_cnt += 1
            self.pnt_msg('***_probe_times must be reprobe= o_mm0=%.2f, o_mm1=%.2f' % (o_mm0, o_mm1))
        return o_mm
    
    def clear_nozzle(self, hot_min_temp, hot_max_temp, bed_max_temp, min_hold, max_hold):
        min_x, min_y = self.cfg.clr_noz_start_x, self.cfg.clr_noz_start_y
        max_x, max_y = self.cfg.clr_noz_start_x + self.cfg.clr_noz_len_x, self.cfg.clr_noz_start_y + self.cfg.clr_noz_len_y
        self._set_bed_temps(temp=bed_max_temp, wait=False)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=False, err=10)
        self._ck_g28ed(False)
        random.seed(time.time())  
        cur_pos = self.obj.toolhead.get_position()
        src_pos = [min_x, min_y, self.cfg.bed_max_err + 1, cur_pos[3]]
        end_pos = [max_x, max_y, src_pos[2], src_pos[3]]
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=True, err=10)   
        src_pos[2] = self._probe_times(3, [src_pos[0], src_pos[1], src_pos[2]], self.cfg.probe_speed, 10, 0.2, min_hold, max_hold)
        self._set_hot_temps(temp=hot_min_temp + 40, fan_spd=0, wait=False, err=10)
        end_pos[2] = self._probe_times(3, [end_pos[0], end_pos[1], end_pos[2]], self.cfg.probe_speed, 10, 0.2, min_hold, max_hold)     
        self._move(src_pos[:2] + [self.cfg.bed_max_err + 1], self.cfg.g29_xy_speed) 
        self._move(src_pos[:2] + [src_pos[2] + 0.1], self.cfg.g29_rdy_speed) 
        self._set_hot_temps(temp=hot_max_temp, fan_spd=0, wait=True, err=10)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=False)
        # retract filament
        if self.cfg.wipe_retract_distance > 0:
            self.obj.gcode.run_script_from_command('G91')
            self.obj.gcode.run_script_from_command('G1 E-%.2f F600' % self.cfg.wipe_retract_distance)
            self.obj.gcode.run_script_from_command('G90')
        self._move(end_pos[:2] + [end_pos[2] + self.cfg.pa_clr_down_mm], self.cfg.probe_speed)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=255, wait=True, err=5)
        self._move([end_pos[0], end_pos[1] + 10, end_pos[2] + 10], self.cfg.probe_speed)
        self._set_hot_temps(temp=hot_min_temp, fan_spd=0, wait=False) 
        self._set_bed_temps(temp=bed_max_temp, wait=True, err=5)

        self._move(self.val.home_xy + [10], self.cfg.g29_xy_speed)
        self.obj.gcode.run_script_from_command('G28 Z')
        pass

    def probe_z_offset(self, x, y):
        self._ck_g28ed()
        z_offset = self._probe_times(3, [x, y, self.cfg.bed_max_err + 1.], self.cfg.probe_speed, 10, self.cfg.check_bed_mesh_max_err, self.cfg.min_hold, self.cfg.max_hold)
        return z_offset

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
                up_min_cnt, up_all_cnt, deal_sta = self._cal_min_z(rdy_pos[2], tmp_hx711_vs[i])
                if up_after:
                    self.obj.dirzctl.check_and_run(1, int(step_us / 2), int(up_all_cnt))
                return self.val.out_index, self.val.out_val_mm, deal_sta
            self.obj.hx711s.delay_s(0.005)
        return self.val.out_index, self.val.out_val_mm, True

    cmd_NOZZLE_CLEAR_help = "Clear the nozzle on bed."
    def cmd_NOZZLE_CLEAR(self, gcmd):
        hot_min_temp = gcmd.get_float('HOT_MIN_TEMP', self.cfg.hot_min_temp)
        hot_max_temp = gcmd.get_float('HOT_MAX_TEMP', self.cfg.hot_max_temp)
        bed_max_temp = gcmd.get_float('BED_MAX_TEMP', self.cfg.bed_max_temp)
        min_hold = gcmd.get_int('MIN_HOLD', self.cfg.min_hold)
        max_hold = gcmd.get_int('MAX_HOLD', self.cfg.max_hold)
        self.clear_nozzle(hot_min_temp, hot_max_temp, bed_max_temp, min_hold, max_hold)

    cmd_PRTOUCH_PROBE_ZOFFSET_help = "Probe the z-offset"
    def cmd_PRTOUCH_PROBE_ZOFFSET(self, gcmd):
        self._ck_g28ed()

        homing_origin = self.obj.gcode_move.get_status()['homing_origin']
        self.pnt_array('homing_origin: ', homing_origin, len(homing_origin))
        start_z_offset = self.obj.probe.get_offsets()[2]
        self.pnt_msg('Start z_offset: %.3f' % start_z_offset)

        if gcmd.get_int('CLEAR_NOZZLE', 0) == 1:
            self.clear_nozzle(self.cfg.hot_min_temp, self.cfg.hot_max_temp, self.cfg.bed_max_temp,
                            self.cfg.min_hold, self.cfg.max_hold)

        x = self.cfg.sensor_x + random.uniform(-self.cfg.random_offset, self.cfg.random_offset)
        y = self.cfg.sensor_y + random.uniform(-self.cfg.random_offset, self.cfg.random_offset)
        probe_x_offset, probe_y_offset = self.obj.probe.get_offsets()[:2]
        probe_x = x - probe_x_offset
        probe_y = y - probe_y_offset
        self.pnt_msg("Checking z-position of probe (%.2f, %.2f)" % (probe_x, probe_y))
        self._move([probe_x, probe_y, self.cfg.bed_max_err + 1.], self.cfg.g29_xy_speed)
        probe_gcmd = self.obj.gcode.create_gcode_command("PROBE", "PROBE", {'SAMPLES': '2'})
        z_probe = self.obj.probe.run_probe(probe_gcmd)
        self.pnt_msg('Probe at sensor: %.3f' % z_probe[2])

        nozzle_z_offset = self.probe_z_offset(x, y)
        self.pnt_msg('Nozzle z_offset: %.3f' % nozzle_z_offset)

        z_offset = nozzle_z_offset - z_probe[2]
        self.pnt_msg('Calculated z_offset: %.3f' % z_offset)

        z_adjust = z_offset + start_z_offset
        self.pnt_msg('z_adjust: %.3f' % z_adjust)
        if gcmd.get_int('APPLY_Z_ADJUST', 0) == 1:
            self.obj.gcode.run_script_from_command('SET_GCODE_OFFSET Z_ADJUST=%f MOVE=1' % (z_adjust))

        z_probe[2] = homing_origin[2] + z_adjust - start_z_offset
        self.obj.probe.probe_calibrate_finalize(z_probe)

    cmd_PRTOUCH_ACCURACY_help = "Probe Z-height accuracy at sensoor position"
    def cmd_PRTOUCH_ACCURACY(self, gcmd):
        self._ck_g28ed()
        speed = gcmd.get_float("PROBE_SPEED", self.cfg.probe_speed, above=0.)
        sample_count = gcmd.get_int("SAMPLES", 10, minval=1)
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
        # Calculate maximum, minimum and average values
        max_value = max(positions)
        min_value = min(positions)
        range_value = max_value - min_value
        avg_value = self._calc_mean(positions)
        median = self._calc_median(positions)
        # calculate the standard deviation
        deviation_sum = 0
        for i in range(len(positions)):
            deviation_sum += pow(positions[i] - avg_value, 2.)
        sigma = (deviation_sum / len(positions)) ** 0.5
        # Show information
        gcmd.respond_info(
            "probe accuracy results: maximum %.6f, minimum %.6f, range %.6f, "
            "average %.6f, median %.6f, standard deviation %.6f" % (
            max_value, min_value, range_value, avg_value, median, sigma))


def load_config(config):
    prt = PRTouchZOffsetWrapper(config)
    return prt

# PRTOUCH_PROBE_ZOFFSET APPLY_Z_ADJUST=1
