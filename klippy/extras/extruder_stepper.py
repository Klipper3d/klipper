# Code for supporting multiple steppers in single filament extruder.
#
# Copyright (C) 2019-2020  Simo Apell <simo.apell@live.fi>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper, extruder

class ExtruderStepper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.extruder_name = config.get('add_to_extruder')
        self.stepper = stepper.PrinterStepper(config)
        self.printer.register_event_handler("klippy:connect", \
                                            self.add_stepper_to_extruder)
    def add_stepper_to_extruder(self):
        extruder_list = extruder.get_printer_extruders(self.printer)
        for e in extruder_list:
            if e.name == self.extruder_name:
                e.add_stepper(self.stepper)
                break

def load_config_prefix(config):
    return ExtruderStepper(config)
