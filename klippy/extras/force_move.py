# Utility for manually moving a stepper for diagnostic purposes
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import chelper

BUZZ_VELOCITY = 4.
STALL_TIME = 0.100

# Calculate a move's accel_t, cruise_t, and cruise_v
def calc_move_time(dist, speed, accel):
    dist = abs(dist)
    if not accel or not dist:
        return 0., dist / speed, speed
    max_cruise_v2 = dist * accel
    if max_cruise_v2 < speed**2:
        speed = math.sqrt(max_cruise_v2)
    accel_t = speed / accel
    accel_decel_d = accel_t * speed
    cruise_t = (dist - accel_decel_d) / speed
    return accel_t, cruise_t, speed

class ForceMove:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = {}
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.cmove = ffi_main.gc(ffi_lib.move_alloc(), ffi_lib.free)
        self.move_fill = ffi_lib.move_fill
        self.stepper_kinematics = ffi_main.gc(
            ffi_lib.cartesian_stepper_alloc('x'), ffi_lib.free)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('STEPPER_BUZZ', self.cmd_STEPPER_BUZZ,
                                    desc=self.cmd_STEPPER_BUZZ_help)
        if config.getboolean("enable_force_move", False):
            self.gcode.register_command('FORCE_MOVE', self.cmd_FORCE_MOVE,
                                        desc=self.cmd_FORCE_MOVE_help)
            self.gcode.register_command(
                'SET_KINEMATIC_POSITION', self.cmd_SET_KINEMATIC_POSITION,
                desc=self.cmd_SET_KINEMATIC_POSITION_help)
    def register_stepper(self, stepper):
        name = stepper.get_name()
        self.steppers[name] = stepper
    def force_enable(self, stepper):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        was_enable = stepper.is_motor_enabled()
        if not was_enable:
            stepper.motor_enable(print_time, 1)
            toolhead.dwell(STALL_TIME)
        was_ignore = stepper.set_ignore_move(False)
        return was_enable, was_ignore
    def restore_enable(self, stepper, was_enable, was_ignore):
        stepper.set_ignore_move(was_ignore)
        if not was_enable:
            toolhead = self.printer.lookup_object('toolhead')
            toolhead.dwell(STALL_TIME)
            print_time = toolhead.get_last_move_time()
            stepper.motor_enable(print_time, 0)
            toolhead.dwell(STALL_TIME)
    def manual_move(self, stepper, dist, speed, accel=0.):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        prev_sk = stepper.set_stepper_kinematics(self.stepper_kinematics)
        stepper.set_position((0., 0., 0.))
        accel_t, cruise_t, cruise_v = calc_move_time(dist, speed, accel)
        self.move_fill(self.cmove, print_time, accel_t, cruise_t, accel_t,
                       0., 0., 0., dist, 0., 0., 0., cruise_v, accel)
        stepper.step_itersolve(self.cmove)
        stepper.set_stepper_kinematics(prev_sk)
        toolhead.dwell(accel_t + cruise_t + accel_t)
    def _lookup_stepper(self, params):
        name = self.gcode.get_str('STEPPER', params)
        if name not in self.steppers:
            raise self.gcode.error("Unknown stepper %s" % (name,))
        return self.steppers[name]
    cmd_STEPPER_BUZZ_help = "Oscillate a given stepper to help id it"
    def cmd_STEPPER_BUZZ(self, params):
        stepper = self._lookup_stepper(params)
        logging.info("Stepper buzz %s", stepper.get_name())
        was_enable, was_ignore = self.force_enable(stepper)
        toolhead = self.printer.lookup_object('toolhead')
        for i in range(10):
            self.manual_move(stepper, 1., BUZZ_VELOCITY)
            toolhead.dwell(.050)
            self.manual_move(stepper, -1., BUZZ_VELOCITY)
            toolhead.dwell(.450)
        self.restore_enable(stepper, was_enable, was_ignore)
    cmd_FORCE_MOVE_help = "Manually move a stepper; invalidates kinematics"
    def cmd_FORCE_MOVE(self, params):
        stepper = self._lookup_stepper(params)
        distance = self.gcode.get_float('DISTANCE', params)
        speed = self.gcode.get_float('VELOCITY', params, above=0.)
        accel = self.gcode.get_float('ACCEL', params, 0., minval=0.)
        logging.info("FORCE_MOVE %s distance=%.3f velocity=%.3f accel=%.3f",
                     stepper.get_name(), distance, speed, accel)
        was_enable, was_ignore = self.force_enable(stepper)
        self.manual_move(stepper, distance, speed, accel)
        self.restore_enable(stepper, True, was_ignore)
    cmd_SET_KINEMATIC_POSITION_help = "Force a low-level kinematic position"
    def cmd_SET_KINEMATIC_POSITION(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.get_last_move_time()
        curpos = toolhead.get_position()
        x = self.gcode.get_float('X', params, curpos[0])
        y = self.gcode.get_float('Y', params, curpos[1])
        z = self.gcode.get_float('Z', params, curpos[2])
        logging.info("SET_KINEMATIC_POSITION pos=%.3f,%.3f,%.3f", x, y, z)
        toolhead.set_position([x, y, z, curpos[3]], homing_axes=(0, 1, 2))
        self.gcode.reset_last_position()

def load_config(config):
    return ForceMove(config)
