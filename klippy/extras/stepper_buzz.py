# Utility for moving a stepper back and forth to help identify it
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class StepperBuzz:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = {}
    def register_stepper(self, stepper, name):
        self.steppers[name] = stepper
        # Register STEPPER_BUZZ command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command(
            'STEPPER_BUZZ', 'STEPPER', name, self.cmd_STEPPER_BUZZ,
            desc=self.cmd_STEPPER_BUZZ_help)
    cmd_STEPPER_BUZZ_help = "Oscillate a given stepper to help id it"
    def cmd_STEPPER_BUZZ(self, params):
        name = self.gcode.get_str('STEPPER', params)
        logging.info("Stepper buzz %s", name)
        stepper = self.steppers[name]
        need_motor_enable = stepper.need_motor_enable
        # Move stepper
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.wait_moves()
        pos = stepper.mcu_stepper.get_commanded_position()
        print_time = toolhead.get_last_move_time()
        if need_motor_enable:
            stepper.motor_enable(print_time, 1)
            print_time += .1
        was_ignore = stepper.mcu_stepper.set_ignore_move(False)
        for i in range(10):
            stepper.step_const(print_time, pos, 1., 4., 0.)
            print_time += .3
            stepper.step_const(print_time, pos + 1., -1., 4., 0.)
            toolhead.reset_print_time(print_time + .7)
            print_time = toolhead.get_last_move_time()
        stepper.mcu_stepper.set_ignore_move(was_ignore)
        if need_motor_enable:
            print_time += .1
            stepper.motor_enable(print_time, 0)
            toolhead.reset_print_time(print_time)

def load_config(config):
    return StepperBuzz(config)
