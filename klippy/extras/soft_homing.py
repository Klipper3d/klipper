import logging, math
SOFT_HOMING_X_ERR_CODE  = {'code':'key403', 'msg':'Homing failed due to printer shutdown,X sotf homing err', 'values':[]}
SOFT_HOMING_Y_ERR_CODE  = {'code':'key404', 'msg':'Homing failed due to printer shutdown,Y sotf homing err', 'values':[]}
class SoftHomingInit:
    
    def __init__(self, config):
        self.config         = config
        self.printer        = config.get_printer()
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('SOFTX_G28', self.cmd_SOFT_G28_X)
        gcode.register_command('SOFTY_G28', self.cmd_SOFT_G28_Y)
        gcode.register_command('SOFT_CHECK_ERROR', self.cmd_SOFTX_G28_CHECK_ERROR)
        self.zeroHomeLen = config.getfloat('zeroHomeLen', default=20, minval=5, maxval=50)
        self.waitingTime_ms = config.getint('waitingTime_ms', default=2000, minval=1000, maxval=5000)
        self.diff_step = config.getint('diff_step', default=2, minval=1, maxval=10)
        self.home_cnt_max = config.getint('home_cnt_max', default=15, minval=5, maxval=30)
        self.home_mode = config.getint('home_mode', default=0, minval=0, maxval=1)
        self.check_home_falg = 0
   

    def ck_and_raise_error(self, err_code, vals=[]): 
        err_code['values'] = vals
        # self.print_msg('RAISE_ERROR', str(err_code), True)
        err_code['msg'] = 'Shutdown due to ' + err_code['msg']
        # self.printer.invoke_shutdown(str(err_code))
        # while True:
        #     self.delay_s(1.)
        #     self.print_msg('RAISE_ERROR', str(err_code), True)
        # raise self.printer.command_error(str(err_code))
        self.printer.command_error(str(err_code))
        gcode = self.printer.lookup_object("gcode")
        gcode.respond_info(str(err_code))
        pass
    def cmd_SOFTX_G28_CHECK_ERROR(self, gcmd):
        self.check_home_falg = gcmd.get_int('FLAG', 0)
        gcmd.respond_info('self.check_home_falg:%d\n' %(self.check_home_falg))
        pass
    def cmd_SOFT_G28_X(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead', None)
        gcode = self.printer.lookup_object('gcode')
        virtual_sdcard = self.printer.lookup_object('virtual_sdcard')
        kin = toolhead.get_kinematics()
        steppers = kin.get_steppers()
        gcode.run_script_from_command('G28 X')
        mcu_pos = " ".join(["%s:%d" % (s.get_name(), s.get_mcu_position())
                            for s in steppers])
        step_sum = []
        break_flag = False
        pre_step = 0
        i =0
        pos = steppers[0].get_mcu_position()
        step_sum.append(pos)
        pre_step = pos
        gcmd.respond_info('step:%d\n' %(pos))
        gcmd.respond_info('steppers[0].get_mcu_position():%d\n' %(steppers[0].get_mcu_position()))
        
        status_print = virtual_sdcard.get_status(1)
        p_active = status_print['is_active']
        p_pos = status_print['file_position']
        gcmd.respond_info('p_active:%d p_pos:%d\n' %(p_active, p_pos))
        if p_active > 0 and p_pos > 10:
            check_flag = True
        else :
            check_flag = False
        if self.check_home_falg == 1:
            check_flag = False
        if self.check_home_falg == 2:
            check_flag = True
        gcmd.respond_info('check_flag = %d' %(check_flag))
        while(1):
            i = i + 1
            gcode.run_script_from_command('G91')
            gcode.run_script_from_command('G0 X%f' %(self.zeroHomeLen))
            gcode.run_script_from_command('G90')
            gcode.run_script_from_command('G4 P%d'%(self.waitingTime_ms))
            gcode.run_script_from_command('G28 X')
            pos = steppers[0].get_mcu_position()
            gcmd.respond_info('step:%d\n' %(pos))

           
            if self.home_mode == 0 : #和上一次回零对比
                if ((pre_step - pos) < self.diff_step) & ((pre_step - pos) > -self.diff_step):
                    break_flag = True
                    break
            else: #和之前回零对比
                for temp in step_sum :
                    if ((temp - pos) < self.diff_step) & ((temp - pos) > -self.diff_step):
                        break_flag = True
                        break
            step_sum.append(pos)
            pre_step = pos
            if(break_flag == True):
                break 
            if(i >= self.home_cnt_max):
                if check_flag == True :
                    self.ck_and_raise_error(SOFT_HOMING_X_ERR_CODE)
                break 
        gcmd.respond_info("mcu: %s\n \n"
                          % (mcu_pos))
        
    
        
    def cmd_SOFT_G28_Y(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead', None)
        gcode = self.printer.lookup_object('gcode')
        virtual_sdcard = self.printer.lookup_object('virtual_sdcard')
        kin = toolhead.get_kinematics()
        steppers = kin.get_steppers()
        gcode.run_script_from_command('G28 Y')
        mcu_pos = " ".join(["%s:%d" % (s.get_name(), s.get_mcu_position())
                            for s in steppers])
        step_sum = []
        pre_step = 0
        break_flag = False
        i =0
        pos = steppers[1].get_mcu_position()
        step_sum.append(pos)
        pre_step = pos
        gcmd.respond_info('%d\n' %(pos))

        status_print = virtual_sdcard.get_status(1)
        p_active = status_print['is_active']
        p_pos = status_print['file_position']
        gcmd.respond_info('p_active:%d p_pos:%d\n' %(p_active, p_pos))
        if p_active > 0 and p_pos > 10:
            check_flag = True
        else :
            check_flag = False
        if self.check_home_falg == 1:
            check_flag = False
        if self.check_home_falg == 2:
            check_flag = True
        gcmd.respond_info('check_flag = %d' %(check_flag))
        while(1):
            i = i + 1
            gcode.run_script_from_command('G91')
            gcode.run_script_from_command('G0 Y%f' %(self.zeroHomeLen))
            gcode.run_script_from_command('G90')
            gcode.run_script_from_command('G4 P%d'%(self.waitingTime_ms))
            gcode.run_script_from_command('G28 Y')
            pos = steppers[1].get_mcu_position()
            gcmd.respond_info('%d\n' %(pos))
            if self.home_mode == 0 :
                if ((pre_step - pos) < self.diff_step) & ((pre_step - pos) > -self.diff_step):
                    break_flag = True
                    break
            else: 
                for temp in step_sum :
                    if ((temp - pos) < self.diff_step) & ((temp - pos) > -self.diff_step):
                        break_flag = True
                        break
            step_sum.append(pos)
            pre_step = pos
            if(break_flag == True): 
                break 
            if(i >= self.home_cnt_max):
                if check_flag == True:
                    self.ck_and_raise_error(SOFT_HOMING_Y_ERR_CODE)
                break 
        gcmd.respond_info("mcu: %s\n \n"
                          % (mcu_pos))  


def load_config(config):

    return SoftHomingInit(config)