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
        self.stealthchop_comp = config.getfloat('stealthchop_comp', 0.0,
                                                minval=0.0, maxval=0.1)
        self.velocity_comp = config.getfloat('velocity_comp', 0.0,
                                             minval=0.0, maxval=0.1)
        self.printer.register_event_handler("klippy:connect", self._connect)
        self.printer.register_event_handler("klippy:mcu_identify",
                                            self._handle_mcu_identify)
        if not config.has_section(self.stepper_name):
            raise config.error("Incorrect stepper name '%s' specified"
                               % self.stepper_name)
        sconfig = config.getsection(self.stepper_name)
        self.microsteps = sconfig.getint('microsteps', note_valid=False)
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_STEPPER_SHAPER",
                                   "STEPPER", self.stepper_name,
                                   self.cmd_SET_STEPPER_SHAPER,
                                   desc=self.cmd_SET_STEPPER_SHAPER_help)
    def _connect(self):
        input_shaper = self.printer.lookup_object("input_shaper", None)
        if input_shaper is not None:
            # Make sure to initialize input shaper stepper kinematics
            # before initializing stepper shaper.
            input_shaper.init_for_steppers([self.stepper])
        if not self.stepper.set_lag_correction(
                self.calc_rad_per_mm(), self.stealthchop_comp,
                self.velocity_comp, self.velocity_smooth_time):
            raise self.printer.config_error(
                    "Invalid stepper shaper configuration for stepper '%s'"
                    % self.stepper_name)
    def calc_rad_per_mm(self):
        return .5 * math.pi / (self.microsteps * self.stepper.get_step_dist())
    def _handle_mcu_identify(self):
        # Lookup stepper object
        force_move = self.printer.lookup_object("force_move")
        self.stepper = force_move.lookup_stepper(self.stepper_name)
    cmd_SET_STEPPER_SHAPER_help = "Set stepper motion compensation parameters"
    def cmd_SET_STEPPER_SHAPER(self, gcmd):
        stealthchop_comp = gcmd.get_float('STEALTHCHOP_COMP', None,
                                          minval=0.0, maxval=0.1)
        velocity_comp = gcmd.get_float('VELOCITY_COMP', None,
                                       minval=0.0, maxval=0.1)
        if stealthchop_comp is None and velocity_comp is None:
            gcmd.respond_info('STEALTHCHOP_COMP=%.6f, VELOCITY_COMP=%.6f'
                              % (self.stealthchop_comp, self.velocity_comp))
            return
        if stealthchop_comp is None:
            stealthchop_comp = self.stealthchop_comp
        if velocity_comp is None:
            velocity_comp = self.velocity_comp
        self.printer.lookup_object("toolhead").flush_step_generation()
        if not self.stepper.set_lag_correction(
                self.calc_rad_per_mm(), stealthchop_comp, velocity_comp,
                self.velocity_smooth_time):
            raise gcmd.error(
                    "Unable to set STEALTHCHOP_COMP=%.6f VELOCITY_COMP=%.6f "
                    + "for stepper '%s'" % (stealthchop_comp, velocity_comp,
                                            self.stepper_name))
        motion_queuing = self.printer.lookup_object("motion_queuing")
        motion_queuing.check_step_generation_scan_windows()
        self.stealthchop_comp = stealthchop_comp
        self.velocity_comp = velocity_comp

def load_config_prefix(config):
    return StepperShaper(config)
