# Utility for manually moving a stepper for diagnostic purposes
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import chelper

BUZZ_VELOCITY = 4.
STALL_TIME = 0.100

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
        # Register STEPPER_BUZZ command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('STEPPER_BUZZ', self.cmd_STEPPER_BUZZ,
                                    desc=self.cmd_STEPPER_BUZZ_help)
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
    def manual_move(self, stepper, dist, speed):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        move_t = abs(dist / speed)
        prev_sk = stepper.set_stepper_kinematics(self.stepper_kinematics)
        stepper.set_position((0., 0., 0.))
        self.move_fill(self.cmove, print_time, 0., move_t, 0.,
                       0., 0., 0., dist, 0., 0., 0., speed, 0.)
        stepper.step_itersolve(self.cmove)
        stepper.set_stepper_kinematics(prev_sk)
        toolhead.dwell(move_t)
    cmd_STEPPER_BUZZ_help = "Oscillate a given stepper to help id it"
    def cmd_STEPPER_BUZZ(self, params):
        name = self.gcode.get_str('STEPPER', params)
        if name not in self.steppers:
            raise self.gcode.error("Unknown stepper %s" % (name,))
        stepper = self.steppers[name]
        logging.info("Stepper buzz %s", name)
        was_enable, was_ignore = self.force_enable(stepper)
        toolhead = self.printer.lookup_object('toolhead')
        for i in range(10):
            self.manual_move(stepper, 1., BUZZ_VELOCITY)
            toolhead.dwell(.050)
            self.manual_move(stepper, -1., BUZZ_VELOCITY)
            toolhead.dwell(.450)
        self.restore_enable(stepper, was_enable, was_ignore)

def load_config(config):
    return ForceMove(config)
