# Support for enable pins on stepper motor drivers
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

DISABLE_STALL_TIME = 0.100

class StepperEnable:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = {}
        self.printer.register_event_handler("gcode:request_restart",
                                            self._handle_request_restart)
        # Register M18/M84 commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("M18", self.cmd_M18)
        gcode.register_command("M84", self.cmd_M18)
    def register_stepper(self, stepper):
        self.steppers[stepper.get_name()] = stepper
    def motor_off(self):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.dwell(DISABLE_STALL_TIME)
        print_time = toolhead.get_last_move_time()
        for s in self.steppers.values():
            s.motor_enable(print_time, 0)
        self.printer.send_event("stepper_enable:motor_off", print_time)
        toolhead.dwell(DISABLE_STALL_TIME)
        logging.debug('; Max time of %f', print_time)
    def _handle_request_restart(self, print_time):
        self.motor_off()
    def cmd_M18(self, params):
        # Turn off motors
        self.motor_off()

def load_config(config):
    return StepperEnable(config)
