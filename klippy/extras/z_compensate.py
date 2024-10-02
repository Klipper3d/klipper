# prtouch support
#
# Copyright (C) 2018-2021  Creality <wangyulong878@sina.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import time
import socket
from . import probe
import math
import random
import threading


PR_ERR_CODE_CAL_DATA_ERROR          = {'code':'key401', 'msg':'PR_ERR_CODE_CAL_DATA_ERROR: An error occurred during data calculation..', 'values':[]}
PR_ERR_CODE_Z_OFFSET_CALIBRATION    = {'code':'key401', 'msg':'PR_ERR_CODE_Z_OFFSET_CALIBRATION: Ceramic pressure sensitive mistrigger..', 'values':[]}
PR_ERR_CODE_SSAP_TEST_ERROR         = {'code':'key402', 'msg':'PR_ERR_CODE_SSAP_TEST_ERROR: Synchronization line communication is not available, please check the connection..', 'values':[]}
MAX_BUF_LEN = 32
MAX_PRES_CNT = 4
MAX_STEP_CNT = 4
class ZCompensateInit:
    def __init__(self, config):
        self.config         = config
        self.printer        = config.get_printer()
        self.prtouch        = self.printer.lookup_object('prtouch_v2')
        self.gcode          = self.printer.lookup_object('gcode')
        self.tri_wave_ip    = config.get('tri_wave_ip', None)   
        self.shut_down = False
        
        self.tri_expand_mm  = config.getfloat('tri_expand_mm', default=0.05, minval=-0.2, maxval=0.1)
        self.g29_down_min_z     = config.getfloat('g29_down_min_z', default=25, minval=5, maxval=500)
        self.z_offset_cex_axis = config.getfloat('z_offset_cex_axis', default=3, minval=1, maxval=10)
        self.z_offset_move  = 0
        self.dbg_msg = {}
        self.sys_max_velocity, self.sys_max_accel, self.sys_max_z_velocity, self.sys_max_z_accel = 0, 0, 0, 0

        self.gcode.register_command('Z_OFFSET_CALIBRATION', self.cmd_Z_OFFSET_CALIBRATION, desc=self.cmd_Z_OFFSET_CALIBRATION_help)
        self.gcode.register_command('Z_OFFSET_AUTO', self.cmd_Z_OFFSET_AUTO, desc=self.cmd_Z_OFFSET_AUTO_help)
        self.gcode.register_command('CRTENSE_NOZZLE_CLEAR', self.cmd_CR10SE_NOZZLE_CLEAR, desc=self.cmd_CR10SE_NOZZLE_CLEAR_help)

        self.vs_start_z_pos     = config.getfloat('vs_start_z_pos', default=3, minval=0, maxval=10)
        self.show_msg = config.getboolean('show_msg', default=False)
        # self.bl_offset =[28,-2]
        self.bl_offset = [0,0]
        self.bl_offset[0], self.bl_offset[1] = config.getfloatlist('bl_offset', count=2)
        
        self.noz_pos_center = config.getfloatlist('noz_pos_center', count=2)
        self.noz_pos_offset = config.getfloatlist('noz_pos_offset', count=2)
        self.pumpback_mm = config.getfloat('pumpback_mm', default=5, minval=0, maxval=80)
        self.pr_probe_cnt = config.getint('pr_probe_cnt', default=5, minval=1, maxval=10)
        self.pr_clear_probe_cnt = config.getint('pr_clear_probe_cnt', default=5, minval=1, maxval=10)
        self.type_nozz = config.getint('type_nozz', default=0, minval=0, maxval=5)

        self.use_adc            = config.getboolean('use_adc', default=False)
        # 4. Clear Nozzle Cfg
        self.hot_start_temp     = config.getfloat('hot_start_temp', default=140, minval=80, maxval=200)
        self.hot_end_temp       = config.getfloat('hot_end_temp', default=140, minval=80, maxval=200)
        self.hot_rub_temp       = config.getfloat('hot_rub_temp', default=200, minval=180, maxval=300)
        self.bed_add_temp       = config.getfloat('bed_add_temp', default=60, minval=45, maxval=100)
        self.pa_clr_dis_mm      = config.getfloat('pa_clr_dis_mm', default=30, minval=2, maxval=100)
        self.pa_clr_down_mm     = config.getfloat('pa_clr_down_mm', default=-0.15, minval=-1, maxval=1)
        self.clr_noz_start_x    = config.getfloat('clr_noz_start_x', default=0, minval=-10, maxval=1000)
        self.clr_noz_start_y    = config.getfloat('clr_noz_start_y', default=0, minval=0, maxval=1000)
        self.clr_noz_len_x      = config.getfloat('clr_noz_len_x', default=0, minval=-10, maxval=1000)
        self.clr_noz_len_y      = config.getfloat('clr_noz_len_y', default=0, minval=-10, maxval=1000)    
        self.clr_xy_spd         = config.getfloat('clr_xy_spd', default=2.0, minval=0.1, maxval=10)
        self.tri_min_hold       = config.getint('tri_min_hold', default=(3 if self.use_adc else 2000), minval=1, maxval=200000)
        self.tri_max_hold       = config.getint('tri_max_hold', default=(3072 if self.use_adc else 6000), minval=self.tri_min_hold, maxval=600000)
        self.bed_max_err        = config.getfloat('bed_max_err', default=5, minval=1, maxval=10)
        self.pa_clr_dis_mm_x      = config.getfloat('pa_clr_dis_mm_x', default=0, minval=0, maxval=100)
        self.pa_clr_dis_mm_y      = config.getfloat('pa_clr_dis_mm_y', default=30, minval=0, maxval=100)
        # 5. Speed Cfg
        self.tri_z_down_spd     = config.getfloat('speed', default=10, minval=0.1, maxval=100) # speed
        self.tri_z_up_spd       = config.getfloat('lift_speed', default=self.tri_z_down_spd * 1.0 , minval=0.1, maxval=100)
        self.rdy_xy_spd         = config.getfloat('rdy_xy_spd', default=250 , minval=1, maxval=1000)
        self.rdy_z_spd          = config.getfloat('rdy_z_spd', default=self.tri_z_up_spd, minval=1, maxval=50)
        self.acc_ctl_mm         = config.getfloat('acc_ctl_mm', default=0.5, minval=0, maxval=10)
        self.low_spd_nul        = config.getint('low_spd_nul', 5, minval=1, maxval=10)
        self.send_step_duty     = config.getint('send_step_duty', 16, minval=0, maxval=10)
        self.run_max_velocity   = config.getfloat('run_max_velocity', default=500, minval=1, maxval=5000)
        self.run_max_accel      = config.getfloat('run_max_accel', default=500, minval=1, maxval=50000)  
        self.run_max_z_velocity = config.getfloat('run_max_z_velocity', default=20, minval=1, maxval=5000)
        self.run_max_z_accel    = config.getfloat('run_max_z_accel', default=200, minval=1, maxval=50000) 

    def delay_s(self, delay_s):
        reactor = self.printer.get_reactor()
        eventtime = reactor.monotonic()
        if not self.printer.is_shutdown():
            self.toolhead.get_last_move_time()
            eventtime = reactor.pause(eventtime + delay_s)
            pass
    def TEST_SWAP(self):
        self.toolhead   = self.printer.lookup_object("toolhead")
        self.prtouch.write_swap_prtouch_cmd.send([self.prtouch.pres_oid, 0])
        params0 = self.prtouch.read_swap_prtouch_cmd.send([self.prtouch.step_oid])

        self.prtouch.write_swap_prtouch_cmd.send([self.prtouch.pres_oid, 1])
        params1 = self.prtouch.read_swap_prtouch_cmd.send([self.prtouch.step_oid])       
        if not params0 or not params1 or params0['sta'] != 0 or params1['sta'] != 1:
            self.print_msg('SWAP_TEST', '!!!Swap Test ERROR!!!', True)
            self.ck_and_raise_error(True,PR_ERR_CODE_SSAP_TEST_ERROR)
        else:
            self.print_msg('SWAP_TEST', '---Swap Test Success---', True)
        pass

    def print_msg(self, title, msg, force=False):
        if not self.show_msg and not force:
            return
        if title != 'SHOW_WAVE':
            self.gcode.respond_info('[' + title + ']' + msg)
        if self.tri_wave_ip is None:
            return
        if title not in self.dbg_msg:   
            self.dbg_msg[title] = len(self.dbg_msg)
        ss = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        ss.sendto((str(self.dbg_msg[title]) + '$' + title + '$' + time.strftime("%H:%M:%S ")  + '$' +  msg + '\n').encode(), (str(self.tri_wave_ip), 21021))
        ss.close() 
        pass

    def ck_and_raise_error(self, ck, err_code, vals=[]):  
        if not ck:
            return
        self.prtouch.enable_steps()
        now_pos = self.toolhead.get_position()
        step_cnt, step_us, acc_ctl_cnt = self.prtouch.get_step_cnts(self.g29_down_min_z, self.tri_z_down_spd)
        self.prtouch.start_step_prtouch_cmd.send([self.prtouch.step_oid, 1, self.prtouch.tri_send_ms, step_cnt, step_us, acc_ctl_cnt, self.low_spd_nul, self.send_step_duty, 0])
        t_last = time.time()
        while (time.time() - t_last < 20) and (len(self.prtouch.step_res) != MAX_BUF_LEN):
            self.delay_s(0.010)
        self.prtouch.start_step_prtouch_cmd.send([self.prtouch.step_oid, 0, 0, 0, 0, 0, self.low_spd_nul, self.send_step_duty, 0])
        self.toolhead.set_position(now_pos[:2] + [0, now_pos[3]], homing_axes=[2])
      
        self.prtouch.disable_steps()
        err_code['values'] = vals
        self.print_msg('RAISE_ERROR', str(err_code), True)
        err_code['msg'] = 'Shutdown due to ' + err_code['msg']
        self.printer.invoke_shutdown(str(err_code))
        # while True:
        #     self.delay_s(1.)
        #     self.print_msg('RAISE_ERROR', str(err_code), True)
        raise self.printer.command_error(str(err_code))
        pass

    def move(self, pos, speed, wait=True):
        if not self.shut_down:
            self.gcode.run_script_from_command('G1 F%d X%.3f Y%.3f Z%.3f' % (speed * 60, pos[0], pos[1], pos[2]) if len(pos) >= 3 else 'G1 F%d X%.3f Y%.3f' % (speed * 60, pos[0], pos[1]))
            if wait:
                self.toolhead.wait_moves()
            pass
    def set_hot_temps(self, temp, wait=False, err=5):
        self.pheaters.set_temperature(self.heater_hot, temp, False)
        if wait:
            while not self.shut_down and abs(self.heater_hot.target_temp - self.heater_hot.smoothed_temp) > err and self.heater_hot.target_temp > 0:
                self.delay_s(0.100) 
        pass

    def set_bed_temps(self, temp, wait=False, err=5):
        self.pheaters.set_temperature(self.heater_bed, temp, False)           
        if wait:
            while not self.shut_down and abs(self.heater_bed.target_temp - self.heater_bed.smoothed_temp) > err and self.heater_bed.target_temp > 0:
                self.delay_s(0.100)          
        pass
    def set_step_par(self, load_sys=True):
        if load_sys:
            self.toolhead.max_velocity = self.sys_max_velocity
            self.toolhead.max_accel = self.sys_max_accel
            self.toolhead.kin.max_z_velocity = self.sys_max_z_velocity
            self.toolhead.kin.max_z_accel = self.sys_max_z_accel
            return
        self.sys_max_velocity = self.toolhead.max_velocity
        self.sys_max_accel = self.toolhead.max_accel
        self.sys_max_z_velocity = self.toolhead.kin.max_z_velocity
        self.sys_max_z_accel = self.toolhead.kin.max_z_accel

        self.toolhead.max_velocity = self.run_max_velocity
        self.toolhead.max_accel = self.run_max_accel
        self.toolhead.kin.max_z_velocity = self.run_max_z_velocity
        self.toolhead.kin.max_z_accel = self.run_max_z_accel
        pass
    def pnt_msg(self, msg):
        if self.show_msg:
            self.gcode.respond_info(msg)
        pass
    def cr10se_clear_nozzle(self, hot_start_temp, hot_rub_temp, hot_end_temp, bed_add_temp):
        min_x, min_y = self.clr_noz_start_x, self.clr_noz_start_y
        # max_x, max_y = self.clr_noz_start_x + self.clr_noz_len_x, self.clr_noz_start_y + self.clr_noz_len_y
        if self.type_nozz == 1 :
            hot_start_temp = hot_end_temp = hot_rub_temp
        self.set_bed_temps(temp=bed_add_temp, wait=False)
        self.set_hot_temps(temp=hot_start_temp, wait=False, err=10)
        mesh = self.bed_mesh.get_mesh()
        self.bed_mesh.set_mesh(None)
        self.set_step_par(load_sys=False)
        random.seed(time.time())  
        cur_pos = self.toolhead.get_position()
        src_pos = [min_x + random.uniform(0, self.clr_noz_len_x - self.pa_clr_dis_mm_x), 
                   min_y + random.uniform(0, self.clr_noz_len_y - self.pa_clr_dis_mm_y), self.bed_max_err, cur_pos[3]]
        end_pos = [src_pos[0] + self.pa_clr_dis_mm_x, src_pos[1] + self.pa_clr_dis_mm_y, src_pos[2], src_pos[3]]

        self.set_hot_temps(temp=hot_start_temp, wait=True, err=10)  
        self.move([src_pos[0], src_pos[1], src_pos[2]], self.rdy_xy_spd)
        src_pos[2] = self.prtouch.run_step_prtouch(self.g29_down_min_z, 0, False, self.pr_clear_probe_cnt, self.pr_clear_probe_cnt, True, self.tri_min_hold, self.tri_max_hold)
        self.move([end_pos[0], end_pos[1], end_pos[2]], self.rdy_xy_spd)
        # self.set_fan_speed('fan', 0.0)
        end_pos[2] = self.prtouch.run_step_prtouch(self.g29_down_min_z, 0, False, self.pr_clear_probe_cnt, self.pr_clear_probe_cnt, True, self.tri_min_hold, self.tri_max_hold) 
        self.move([src_pos[0], src_pos[1], self.bed_max_err], self.rdy_xy_spd)
        self.move([src_pos[0], src_pos[1] + 5, src_pos[2] - self.pa_clr_down_mm], self.rdy_z_spd)#移到擦喷嘴起始点
        self.set_hot_temps(temp=hot_rub_temp, wait=True, err=10) #等待加热
        self.gcode.run_script_from_command('G91')
        self.gcode.run_script_from_command('G0 E-%f' %(self.pumpback_mm)) #回抽
        self.gcode.run_script_from_command('G90')
        cur_pos = self.toolhead.get_position()
        end_pos[3] = cur_pos[3]
        self.move([end_pos[0], end_pos[1] - 5, end_pos[2] + self.pa_clr_down_mm], self.clr_xy_spd)# 擦喷嘴
        # self.set_fan_speed('fan', 1.0)
        self.gcode.run_script_from_command('M106 S255')
        self.set_hot_temps(temp=hot_end_temp, wait=True, err=5)
        self.set_bed_temps(temp=bed_add_temp, wait=True, err=5) #等待降温
        self.move([end_pos[0] + self.pa_clr_dis_mm, end_pos[1], end_pos[2] + self.bed_max_err], 100) #上抬
        # self.set_fan_speed('fan', 0.)
        self.gcode.run_script_from_command('M107')
        self.set_bed_temps(temp=bed_add_temp, wait=True, err=5)
        self.set_step_par(load_sys=True)
        self.bed_mesh.set_mesh(mesh)
        self.gcode.run_script_from_command('G28 Z')
        pass

    cmd_CR10SE_NOZZLE_CLEAR_help = "Clear the nozzle on bed."
    def cmd_CR10SE_NOZZLE_CLEAR(self, gcmd):
        self.TEST_SWAP()
        self.gcode.run_script_from_command('SET_GCODE_OFFSEt Z_ADJUST=%f MOVE=1' %(0 - self.z_offset_move))
        self.z_offset_move = 0
        self.bed_mesh   = self.printer.lookup_object('bed_mesh')
        self.toolhead   = self.printer.lookup_object("toolhead")
        self.pheaters   = self.printer.lookup_object('heaters')
        self.heater_hot = self.printer.lookup_object('extruder').heater
        self.heater_bed = self.printer.lookup_object('heater_bed').heater
        hot_start_temp = gcmd.get_float('HOT_START_TEMP', self.hot_start_temp)
        hot_rub_temp = gcmd.get_float('HOT_RUB_TEMP', self.hot_rub_temp)
        hot_end_temp = gcmd.get_float('HOT_END_TEMP', self.hot_end_temp)
        bed_add_temp = gcmd.get_float('BED_ADDTEMP', self.bed_add_temp)
        self.cr10se_clear_nozzle(hot_start_temp, hot_rub_temp, hot_end_temp, bed_add_temp)


    cmd_Z_OFFSET_CALIBRATION_help = "Z offset calibration"
    def cmd_Z_OFFSET_CALIBRATION(self, gcmd):   # PRTOUCH_TEST X=20 Y=20 Z=-4 S=0.0125
        self.bed_mesh       = self.printer.lookup_object('bed_mesh')
        self.toolhead       = self.printer.lookup_object("toolhead")
        #清除调平数据
        mesh = self.bed_mesh.get_mesh()
        self.bed_mesh.set_mesh(None)
        self.gcode.run_script_from_command('SET_GCODE_OFFSEt Z_ADJUST=%f MOVE=1' %(0 - self.z_offset_move))
        self.z_offset_move = 0
        pr_pos_x = random.uniform(self.noz_pos_offset[0], self.noz_pos_offset[1])
        pr_pos_y = random.uniform(self.noz_pos_offset[0], self.noz_pos_offset[1])
        pr_pos = [ [self.noz_pos_center[0] + pr_pos_x , self.noz_pos_center[1] + pr_pos_y],
                   [self.noz_pos_center[0] - pr_pos_x , self.noz_pos_center[1] - pr_pos_y],
                   [self.noz_pos_center[0] + pr_pos_x , self.noz_pos_center[1] - pr_pos_y],
                   [self.noz_pos_center[0] - pr_pos_x , self.noz_pos_center[1] + pr_pos_y],
                 ]
        # 喷嘴探下
        
        pr_data = [0,0,0,0]
        z_offset = [0,0,0,0]
        bl_data = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
        now_pos = self.toolhead.get_position()
        for i in [0,1]:
            self.move([pr_pos[i][0] - self.bl_offset[0], pr_pos[i][1] - self.bl_offset[1]] + [8, now_pos[3]], 100)
            bl_data[i] = self.printer.lookup_object('probe').run_probe(gcmd)
            
            self.prtouch.move([pr_pos[i][0], pr_pos[i][1]] + [self.vs_start_z_pos, now_pos[3]], 100)
            self.prtouch.get_mm_per_step()
            pr_data[i]= self.prtouch.run_step_prtouch(self.g29_down_min_z, 0, False, self.pr_probe_cnt, self.pr_probe_cnt, True)
            z_offset[i] =  bl_data[i][2]  - pr_data[i] 

        # self.move([self.z_offset_pos2[0] + self.bl_offset[0], self.z_offset_pos2[1] + self.bl_offset[1]] + [5, now_pos[3]], 100)
        # bl_data3 = self.printer.lookup_object('probe').run_probe(gcmd)
    
        # self.prtouch.move([self.z_offset_pos2[0], self.z_offset_pos2[1]] + [3, now_pos[3]], 100)
        # self.prtouch.get_mm_per_step()
        # pr_data[1]= self.prtouch.run_step_prtouch(self.g29_down_min_z, 0, False, 5, 5, True)
        # z_offset[1] = bl_data3[2]  - pr_data[1]

        z_offset_old = self.printer.lookup_object('probe').z_offset

        for i in [0,1]:
            self.pnt_msg('bl_data[%d]= %f, pr_data[%d] = %f, z_offset[%d] = %f' %(i,bl_data[i][2] - z_offset_old,i, pr_data[i] - z_offset_old, i, z_offset[i]))
            

        z_offset_new = (z_offset[0] + z_offset[1])/2 - self.tri_expand_mm
        self.pnt_msg('z_offset_new = %f ' %(z_offset_new))

        self.z_offset_move = z_offset_old - z_offset_new
        self.gcode.run_script_from_command('SET_GCODE_OFFSEt Z_ADJUST=%f MOVE=1' % (self.z_offset_move))
        # self.print_ary('z_offset_move = %f\n' %(0-z_offset_move), 1, True)

        # #把Z轴补偿写到cfg文件中
        configfile = self.printer.lookup_object('configfile')
        # new_calibrate = z_offset_old - res_z
        new_calibrate = z_offset_new
        self.pnt_msg('z_offset_new = %f ' %(new_calibrate))

        #如果有错误
        if new_calibrate < 0:
            self.gcode.run_script_from_command('SET_GCODE_OFFSEt Z_ADJUST=%f MOVE=1' % (-self.z_offset_move))
            self.z_offset_move = 0
            self.ck_and_raise_error(new_calibrate < 0,PR_ERR_CODE_Z_OFFSET_CALIBRATION)

        self.gcode.respond_info(
                "%s: z_offset: %.3f\n"
                "The SAVE_CONFIG command will update the printer config file\n"
                "with the above and restart the printer."
                % (self.printer.lookup_object('probe').name, new_calibrate))
        
        configfile.set(self.printer.lookup_object('probe').name, 'z_offset', "%.3f" % (new_calibrate,))
        #写入调平数据
        self.bed_mesh.set_mesh(mesh)
        pass
    
    cmd_Z_OFFSET_AUTO_help = "Z offset auto"
    def cmd_Z_OFFSET_AUTO(self, gcmd):   # PRTOUCH_TEST X=20 Y=20 Z=-4 S=0.0125
        self.TEST_SWAP()
        self.gcode.run_script_from_command('SET_GCODE_OFFSEt Z_ADJUST=%f MOVE=1' %(0 - self.z_offset_move))
        self.z_offset_move = 0
        self.cmd_CR10SE_NOZZLE_CLEAR(gcmd)
        self.cmd_Z_OFFSET_CALIBRATION(gcmd)
        self.printer.lookup_object('configfile').cmd_CX_SAVE_CONFIG(gcmd)
        pass

def load_config(config):

    return ZCompensateInit(config)

