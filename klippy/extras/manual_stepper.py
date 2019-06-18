# Support for a manual controlled stepper
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import stepper, homing, force_move, chelper

ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 4

class ManualStepper:
    def __init__(self, config):
        self.printer = config.get_printer()
        if config.get('endstop_pin', None) is not None:
            self.can_home = True
            self.stepper = stepper.PrinterRail(
                config, need_position_minmax=False, default_position_endstop=0.)
        else:
            self.can_home = False
            self.stepper = stepper.PrinterStepper(config)
        self.velocity = config.getfloat('velocity', 5., above=0.)
        self.accel = config.getfloat('accel', 0., minval=0.)
        self.next_cmd_time = 0.
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.cmove = ffi_main.gc(ffi_lib.move_alloc(), ffi_lib.free)
        self.move_fill = ffi_lib.move_fill
        self.stepper.setup_itersolve('cartesian_stepper_alloc', 'x')
        self.stepper.set_max_jerk(9999999.9, 9999999.9)
        # Register commands
        stepper_name = config.get_name().split()[1]
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command('MANUAL_STEPPER', "STEPPER",
                                        stepper_name, self.cmd_MANUAL_STEPPER,
                                        desc=self.cmd_MANUAL_STEPPER_help)
        self.printer.register_event_handler(
            "toolhead:motor_off", self.handle_motor_off)
    def sync_print_time(self):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        if self.next_cmd_time > print_time:
            toolhead.dwell(self.next_cmd_time - print_time)
        else:
            self.next_cmd_time = print_time
    def do_enable(self, enable):
        self.sync_print_time()
        self.stepper.motor_enable(self.next_cmd_time, enable)
        self.sync_print_time()
    def do_set_position(self, setpos):
        self.stepper.set_position([setpos, 0., 0.])
    def do_move(self, movepos, speed, accel):
        self.sync_print_time()
        cp = self.stepper.get_commanded_position()
        dist = movepos - cp
        accel_t, cruise_t, cruise_v = force_move.calc_move_time(
            dist, speed, accel)
        self.move_fill(self.cmove, self.next_cmd_time,
                       accel_t, cruise_t, accel_t,
                       cp, 0., 0., dist, 0., 0.,
                       0., cruise_v, accel)
        self.stepper.step_itersolve(self.cmove)
        self.next_cmd_time += accel_t + cruise_t + accel_t
        self.sync_print_time()
    def do_homing_move(self, movepos, speed, accel, triggered):
        if not self.can_home:
            raise self.gcode.error("No endstop for this manual stepper")
        # Notify endstops of upcoming home
        endstops = self.stepper.get_endstops()
        for mcu_endstop, name in endstops:
            mcu_endstop.home_prepare()
        # Start endstop checking
        self.sync_print_time()
        for mcu_endstop, name in endstops:
            min_step_dist = min([s.get_step_dist()
                                 for s in mcu_endstop.get_steppers()])
            mcu_endstop.home_start(
                self.next_cmd_time, ENDSTOP_SAMPLE_TIME, ENDSTOP_SAMPLE_COUNT,
                min_step_dist / speed, triggered=triggered)
        # Issue move
        self.do_move(movepos, speed, accel)
        # Wait for endstops to trigger
        error = None
        for mcu_endstop, name in endstops:
            try:
                mcu_endstop.home_wait(self.next_cmd_time)
            except mcu_endstop.TimeoutError as e:
                if error is None:
                    error = "Failed to home %s: %s" % (name, str(e))
        for mcu_endstop, name in endstops:
            try:
                mcu_endstop.home_finalize()
            except homing.CommandError as e:
                if error is None:
                    error = str(e)
        self.sync_print_time()
        if error is not None:
            raise homing.CommandError(error)
    cmd_MANUAL_STEPPER_help = "Command a manually configured stepper"
    def cmd_MANUAL_STEPPER(self, params):
        if 'ENABLE' in params:
            self.do_enable(self.gcode.get_int('ENABLE', params))
        if 'SET_POSITION' in params:
            setpos = self.gcode.get_float('SET_POSITION', params)
            self.do_set_position(setpos)
        homing_move = self.gcode.get_int('STOP_ON_ENDSTOP', params, 0)
        speed = self.gcode.get_float('SPEED', params, self.velocity, above=0.)
        accel = self.gcode.get_float('ACCEL', params, self.accel, minval=0.)
        if homing_move:
            movepos = self.gcode.get_float('MOVE', params)
            if 'ENABLE' not in params and not self.stepper.is_motor_enabled():
                self.do_enable(True)
            self.do_homing_move(movepos, speed, accel, homing_move > 0)
        elif 'MOVE' in params:
            movepos = self.gcode.get_float('MOVE', params)
            if 'ENABLE' not in params and not self.stepper.is_motor_enabled():
                self.do_enable(True)
            self.do_move(movepos, speed, accel)
    def handle_motor_off(self, print_time):
        self.do_enable(0)

def load_config_prefix(config):
    return ManualStepper(config)
