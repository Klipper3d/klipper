# Code for stepper position corrections
#
# Copyright (C) 2025  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import math, logging

class StepperShaper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.stepper_name = ' '.join(config.get_name().split()[1:])
        self.stepper = None
        self.velocity_smooth_time = config.getfloat('velocity_smooth_time',
                                                    0.001, above=0., maxval=0.1)
        self.printer.register_event_handler("klippy:mcu_identify",
                                            self._handle_mcu_identify)
        sconfig = config.getsection(self.stepper_name)
        self.microsteps = sconfig.getint('microsteps', note_valid=False)
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_STEPPER_SHAPER",
                                   "STEPPER", self.stepper_name,
                                   self.cmd_SET_STEPPER_SHAPER,
                                   desc=self.cmd_SET_STEPPER_SHAPER_help)
    def _handle_mcu_identify(self):
        # Lookup stepper object
        force_move = self.printer.lookup_object("force_move")
        self.stepper = force_move.lookup_stepper(self.stepper_name)
    cmd_SET_STEPPER_SHAPER_help = "Set stepper motion compensation parameters"
    def cmd_SET_STEPPER_SHAPER(self, gcmd):
        lag_coeff = gcmd.get_float('STEALTHCHOP_COMP', None,
                                   minval=0.0, maxval=0.1)
        toolhead = self.printer.lookup_object("toolhead")
        if lag_coeff is not None:
            toolhead.flush_step_generation()
            sk = self.stepper.get_stepper_kinematics()
            rad_per_mm = .5 * math.pi / (
                    self.microsteps * self.stepper.get_step_dist())
            if not self.stepper.set_lag_correction(rad_per_mm, lag_coeff,
                                                   self.velocity_smooth_time):
                raise gcmd.error(
                        "Unable to set STEALTHCHOP_COMP=%.6f for stepper '%s'" %
                        (lag_coeff, self.stepper_name))
            motion_queuing = self.printer.lookup_object("motion_queuing")
            motion_queuing.check_step_generation_scan_windows()

def load_config_prefix(config):
    return StepperShaper(config)
