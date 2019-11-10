# Support for enable pins on stepper motor drivers
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class StepperEnable:
    def __init__(self, config):
        self.printer = config.get_printer()
        # Register M18/M84 commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("M18", self.cmd_M18)
        gcode.register_command("M84", self.cmd_M18)
    def cmd_M18(self, params):
        # Turn off motors
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.motor_off()

def load_config(config):
    return StepperEnable(config)
