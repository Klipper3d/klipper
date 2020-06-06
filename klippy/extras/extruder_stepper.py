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
        stepper_name = config.get_name().split()[1]
        self.extruder_name = config.get('extruder', 'extruder')
        self.stepper = stepper.PrinterStepper(config)
        self.stepper.set_max_jerk(9999999.9, 9999999.9)
        self.stepper.setup_itersolve('extruder_stepper_alloc')
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SYNC_STEPPER_TO_EXTRUDER", "STEPPER",
                                   stepper_name,
                                   self.cmd_SYNC_STEPPER_TO_EXTRUDER,
                                   desc=self.cmd_SYNC_STEPPER_TO_EXTRUDER_help)
    def handle_connect(self):
        extruder = self.printer.lookup_object(self.extruder_name)
        self.stepper.set_trapq(extruder.get_trapq())
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_step_generator(self.stepper.generate_steps)
    cmd_SYNC_STEPPER_TO_EXTRUDER_help = "Set extruder stepper"
    def cmd_SYNC_STEPPER_TO_EXTRUDER(self, gcmd):
        gcode = self.printer.lookup_object('gcode')
        self.extruder_name = gcmd.get('EXTRUDER', None)
        extruder = self.printer.lookup_object(self.extruder_name, None)
        if extruder is not None:
            epos = extruder.stepper.get_commanded_position()
            self.stepper.set_position([epos, 0., 0.])
            self.stepper.set_trapq(extruder.get_trapq())
            gcode.respond_info("Extruder stepper now syncing with '%s'"
                                       % (self.extruder_name))
        else:
            raise gcmd.error("'%s' is not a valid extruder."
                                       % (self.extruder_name))

def load_config_prefix(config):
    return ExtruderStepper(config)
