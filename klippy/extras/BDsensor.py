# Bed leveling sensor BDsensor(Bed Distance sensor)
# https://github.com/markniu/Bed_Distance_sensor
# Copyright (C) 2023 Mark yue <niujl123@sina.com>
# This file may be distributed under the terms of the GNU GPLv3 license.
import sched, time
from threading import Timer

import chelper
import mcu,math
from . import probe
BD_TIMER = 0.500


# Calculate a move's accel_t, cruise_t, and cruise_v
def calc_move_time(dist, speed, accel):
    axis_r = 1.
    if dist < 0.:
        axis_r = -1.
        dist = -dist
    if not accel or not dist:
        return axis_r, 0., dist / speed, speed
    max_cruise_v2 = dist * accel
    if max_cruise_v2 < speed**2:
        speed = math.sqrt(max_cruise_v2)
    accel_t = speed / accel
    accel_decel_d = accel_t * speed
    cruise_t = (dist - accel_decel_d) / speed
    return axis_r, accel_t, cruise_t, speed


# I2C BD_SENSOR
# devices connected to an MCU via an virtual i2c bus(2 any gpio)

class MCU_I2C_BD:
    def __init__(self,mcu,   sda_pin,scl_pin, delay_t):
        self.mcu = mcu
        self.oid = self.mcu.create_oid()
        # Generate I2C bus config message
        self.config_fmt = (
            "config_I2C_BD oid=%d sda_pin=%s scl_pin=%s delay=%s"
            % (self.oid, sda_pin,scl_pin, delay_t))
        self.cmd_queue = mcu.alloc_command_queue()
        mcu.register_config_callback(self.build_config)
        self.I2C_BD_send_cmd = self.I2C_BD_receive_cmd = None
    def build_config(self):
        self.mcu.add_config_cmd(self.config_fmt)
        self.I2C_BD_send_cmd = self.mcu.lookup_command(
            "I2C_BD_send oid=%c data=%*s", cq=self.cmd_queue)
        self.I2C_BD_receive_cmd = self.mcu.lookup_query_command(
            "I2C_BD_receive oid=%c data=%*s",
            "I2C_BD_receive_response oid=%c response=%*s",
             oid=self.oid, cq=self.cmd_queue)

    def get_oid(self):
        return self.oid
    def get_mcu(self):
        return self.mcu
    def get_command_queue(self):
        return self.cmd_queue
    def I2C_BD_send(self, data):
        self.I2C_BD_send_cmd.send([self.oid, data.encode('utf-8')])
    def I2C_BD_receive(self,  data):
        return self.I2C_BD_receive_cmd.send([self.oid, data])

def MCU_BD_I2C_from_config(mcu,config):
    # Determine pin from config
    ppins = config.get_printer().lookup_object("pins")
    pin_sda=config.get('sda_pin')
    pin_scl=config.get('scl_pin')
    return MCU_I2C_BD(mcu,pin_sda,pin_scl,config.get('delay'))

# BDsensor wrapper that enables probe specific features
# set this type of sda_pin 2 as virtual endstop
# add new gcode command M102 for BDsensor
class BDsensorEndstopWrapper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.position_endstop = config.getfloat('z_offset')
        self.stow_on_each_sample = config.getboolean(
            'deactivate_on_each_sample', True)
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.activate_gcode = gcode_macro.load_template(
            config, 'activate_gcode', '')
        self.deactivate_gcode = gcode_macro.load_template(
            config, 'deactivate_gcode', '')
        # Create an "endstop" object to handle the probe pin
        ppins = self.printer.lookup_object('pins')
        pin = config.get('sda_pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        self.mcu = mcu.get_printer_mcu(self.printer, 'mcu')
        # set this type of sda_pin 2 as virtual endstop
        pin_params['pullup']=2
        self.mcu_endstop = self.mcu.setup_pin('endstop', pin_params)
        self.printer.register_event_handler('klippy:mcu_identify',
                                            self._handle_mcu_identify)
        self.oid = self.mcu.create_oid()
        self.cmd_queue = self.mcu.alloc_command_queue()
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.trapq = ffi_main.gc(ffi_lib.trapq_alloc(), ffi_lib.trapq_free)
        self.trapq_append = ffi_lib.trapq_append
        self.trapq_finalize_moves = ffi_lib.trapq_finalize_moves
        self.stepper_kinematics = ffi_main.gc(
            ffi_lib.cartesian_stepper_alloc(b'x'), ffi_lib.free)

        self.bd_sensor=MCU_BD_I2C_from_config(self.mcu,config)
        self.distance=5;
        # Register M102 commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('M102', self.cmd_M102)
        self.BD_display = config.get('BD_display', None)
        self.I2C_BD_receive_cmd2 = None
        self.gcode_move = self.printer.load_object(config, "gcode_move")
        self.gcode = self.printer.lookup_object('gcode')
        
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        # multi probes state
        self.multi = 'OFF'
        self.mcu.register_config_callback(self.build_config)
        self.adjust_range=0;
        #bd_scheduler = sched.scheduler(time.time, time.sleep)
        #bd_scheduler.enter(1, 1, self.BD_loop, (bd_scheduler,))
        #bd_scheduler.run()
        #Timer(2, self.BD_loop, ()).start()
        self.reactor = self.printer.get_reactor()
        self.bd_update_timer = self.reactor.register_timer(
            self.bd_update_event)
        self.reactor.update_timer(self.bd_update_timer, self.reactor.NOW)

    def z_live_adjust(self,bd_value):
        #self.toolhead = self.printer.lookup_object('toolhead')        
        self.toolhead = self.printer.lookup_object('toolhead')
        if self.adjust_range<=0 or self.adjust_range > 40:
            return
        cur_pos=self.toolhead.get_position()
        if cur_pos[:3]*10>self.adjust_range: 
            return
        if bd_value < 0:
            pr = self.I2C_BD_receive_cmd2.send([self.oid, "32".encode('utf-8')])
            bd_value=int(pr['response'])/100.0
        if bd_value>1010:
            return;
        if abs(cur_pos[:3]-bd_value)<=0.01:
            return;
        #self.toolhead.wait_moves() 
        if cur_pos[:3] > bd_value:
            #g1_params = {'X': coord[0], 'Y': coord[1], 'Z': coord[2]}
            g1_gcmd = self.gcode.self.gcode.create_gcode_command("G1", "G1", {'Z': cur_pos[:3]+(cur_pos[:3]-bd_value)})   
            self.gcode_move.cmd_G1(g1_gcmd)
        else:
            g1_gcmd = self.gcode.self.gcode.create_gcode_command("G1", "G1", {'Z': cur_pos[:3]-(cur_pos[:3]-bd_value)})
            self.gcode_move.cmd_G1(g1_gcmd)
        
        
    def bd_update_event(self, eventtime): 
        # schedule the next call first
        #scheduler.enter(1, 1, self.BD_loop, (scheduler,))
        #Timer(1, self.BD_loop, ()).start()
        bd_value = -1.0;
        if self.I2C_BD_receive_cmd2 is not None:
            #print("BD_loop.",self.I2C_BD_receive_cmd2)
            pr = self.I2C_BD_receive_cmd2.send([self.oid, "32".encode('utf-8')])
            bd_value=int(pr['response'])/100.0
            strd=str(bd_value)
            status_dis=self.printer.lookup_object('display_status')
            status_dis.set_message(strd+"mm")
            if bd_value == 10.24:
                status_dis.set_message("BDs:ConnectErr")
            if bd_value == 3.9:
                status_dis.set_message("BDs:Out Range")

        self.z_live_adjust(bd_value)
        
        return eventtime + BD_TIMER

    def build_config(self):
       self.I2C_BD_receive_cmd = self.mcu.lookup_query_command(
            "I2C_BD_receive oid=%c data=%*s",
            "I2C_BD_receive_response oid=%c response=%*s",
            oid=self.oid, cq=self.cmd_queue)
       
       self.I2C_BD_receive_cmd2 = self.mcu.lookup_query_command(
            "I2C_BD_receive2 oid=%c data=%*s",
            "I2C_BD_receive2_response oid=%c response=%*s",
            oid=self.oid, cq=self.cmd_queue)

    def _force_enable(self,stepper):
        self.toolhead = self.printer.lookup_object('toolhead')
        print_time = self.toolhead.get_last_move_time()
        stepper_enable = self.printer.lookup_object('stepper_enable')
        enable = stepper_enable.lookup_enable(stepper.get_name())
        was_enable = enable.is_motor_enabled()
        STALL_TIME = 0.100
        if not was_enable:
            enable.motor_enable(print_time)
            self.toolhead.dwell(STALL_TIME)
        return was_enable

    def manual_move(self, stepper, dist, speed, accel=0.):
         self.toolhead = self.printer.lookup_object('toolhead')
         self.toolhead.flush_step_generation()
         prev_sk = stepper.set_stepper_kinematics(self.stepper_kinematics)
         prev_trapq = stepper.set_trapq(self.trapq)
         stepper.set_position((0., 0., 0.))
         axis_r, accel_t,cruise_t,cruise_v=calc_move_time(dist, speed, accel)
         print_time = self.toolhead.get_last_move_time()
         self.trapq_append(self.trapq, print_time, accel_t, cruise_t, accel_t,
                           0., 0., 0., axis_r, 0., 0., 0., cruise_v, accel)
         print_time = print_time + accel_t + cruise_t + accel_t
         stepper.generate_steps(print_time)
         self.trapq_finalize_moves(self.trapq, print_time + 99999.9)
         stepper.set_trapq(prev_trapq)
         stepper.set_stepper_kinematics(prev_sk)
         self.toolhead.note_kinematic_activity(print_time)
         self.toolhead.dwell(accel_t + cruise_t + accel_t)



        
    
    def cmd_M102(self, gcmd, wait=False):
        CMD_BD = gcmd.get_int('S', None)
        self.toolhead = self.printer.lookup_object('toolhead')
        if CMD_BD == -6:
            kin = self.toolhead.get_kinematics()
            self.bd_sensor.I2C_BD_send("1019")
            distance = 0.5#gcmd.get_float('DISTANCE')
            speed = 10#gcmd.get_float('VELOCITY', above=0.)
            accel = 2000#gcmd.get_float('ACCEL', 0., minval=0.)
            self.distance=0.1
            for stepper in kin.get_steppers():
                if stepper.is_active_axis('z'):
                    self._force_enable(stepper)
                    self.toolhead.wait_moves()
            ncount=0
            while 1:
                self.toolhead.dwell(0.1)
                self.bd_sensor.I2C_BD_send(str(ncount))
                self.toolhead.dwell(0.1)
                #self.bd_sensor.I2C_BD_send(str(ncount))
                #toolhead.dwell(0.5)
                for stepper in kin.get_steppers():
                    if stepper.is_active_axis('z'):
                        self._force_enable(stepper)
                        self.manual_move(stepper, self.distance, speed)
                self.toolhead.wait_moves()
                ncount=ncount+1
                if ncount>=40:
                    self.bd_sensor.I2C_BD_send("1021")
                    break
        if  CMD_BD == -5:
            self.bd_sensor.I2C_BD_send("1017")#tart read raw calibrate data
            ncount1=0
            while 1:
                pr=self.I2C_BD_receive_cmd.send([self.oid,"3".encode('utf-8')])
                intd=int(pr['response'])
                strd=str(intd)
                gcmd.respond_raw(strd)
                self.toolhead.dwell(0.1)
                ncount1=ncount1+1
                if ncount1>=40:
                    break
        if  CMD_BD == -1:
            self.bd_sensor.I2C_BD_send("1016")#1016 // // read sensor version
            ncount1=0
            x=[]
            while 1:
                pr=self.I2C_BD_receive_cmd.send([self.oid,"3".encode('utf-8')])
              #  print"params:%s" % pr['response']
                intd=int(pr['response'])
                if intd>127:
                    intd=127
                if intd<0x20:
                    intd=0x20
                x.append(intd)
                self.toolhead.dwell(0.3)
                ncount1=ncount1+1
                if ncount1>=20:
                    self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
                    res = ''.join(map(chr, x))
                    gcmd.respond_raw(res)
                    break
        if  CMD_BD == -2:# gcode M102 S-2 read distance data
            self.bd_sensor.I2C_BD_send("1015")#1015   read distance data
            pr = self.I2C_BD_receive_cmd.send([self.oid, "32".encode('utf-8')])
            intd=int(pr['response'])
            strd=str(intd/100.0)+"mm"
            gcmd.respond_raw(strd)
        self.bd_sensor.I2C_BD_send("1018")
    def _handle_mcu_identify(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self.add_stepper(stepper)
    def raise_probe(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        start_pos = self.toolhead.get_position()
        self.deactivate_gcode.run_gcode_from_command()
        if self.toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe activate_gcode script")
    def lower_probe(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        start_pos = self.toolhead.get_position()
        self.activate_gcode.run_gcode_from_command()
        if self.toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe deactivate_gcode script")
    def multi_probe_begin(self):
        if self.stow_on_each_sample:
            return
        self.multi = 'FIRST'
    def multi_probe_end(self):
        if self.stow_on_each_sample:
            return
        self.raise_probe()
        self.multi = 'OFF'
    def probe_prepare(self, hmove):
       # self.bd_sensor.I2C_BD_send("890")
        if self.multi == 'OFF' or self.multi == 'FIRST':
            self.lower_probe()
            if self.multi == 'FIRST':
                self.multi = 'ON'
    def probe_finish(self, hmove):
        if self.multi == 'OFF':
            self.raise_probe()
    def get_position_endstop(self):
        return self.position_endstop

def load_config(config):
    bdl=BDsensorEndstopWrapper(config)
    config.get_printer().add_object('probe', probe.PrinterProbe(config, bdl))
    return bdl
