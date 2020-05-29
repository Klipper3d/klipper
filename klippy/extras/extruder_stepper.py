# Code for supporting multiple steppers in single filament extruder.
#
# Copyright (C) 2019 Simo Apell <simo.apell@live.fi>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper

class ExtruderStepper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.extruder_name = config.get('extruder', 'extruder')
        self.stepper = stepper.PrinterStepper(config)
        self.stepper.set_max_jerk(9999999.9, 9999999.9)
        self.stepper.setup_itersolve('extruder_stepper_alloc')
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def handle_connect(self):
        extruder = self.printer.lookup_object(self.extruder_name)
        self.stepper.set_trapq(extruder.get_trapq())
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_step_generator(self.stepper.generate_steps)

def load_config_prefix(config):
    return ExtruderStepper(config)
