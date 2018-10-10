# Endstop accuracy improvement via stepper phase tracking
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import homing

TRINAMIC_DRIVERS = ["tmc2130", "tmc2208"]

class EndstopPhase:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        # Determine number of stepper phases
        for driver in TRINAMIC_DRIVERS:
            driver_name = "%s %s" % (driver, self.name)
            if config.has_section(driver_name):
                module = self.printer.try_load_module(config, driver_name)
                self.get_phase = module.get_phase
                self.phases = module.get_microsteps() * 4
                break
        else:
            self.get_phase = None
            self.phases = config.getint('phases', minval=1)
        # Determine endstop phase position
        self.endstop_phase = config.getint('endstop_phase', None,
                                           minval=0, maxval=self.phases-1)
        self.endstop_align_zero = config.getboolean('endstop_align_zero', False)
        # Determine endstop accuracy
        stepper_config = config.getsection(self.name)
        self.step_dist = step_dist = stepper_config.getfloat('step_distance')
        endstop_accuracy = config.getfloat('endstop_accuracy', None, above=0.)
        if endstop_accuracy is None:
            self.endstop_accuracy = self.phases//2 - 1
        elif self.endstop_phase is not None:
            self.endstop_accuracy = int(
                math.ceil(endstop_accuracy * .5 / step_dist))
        else:
            self.endstop_accuracy = int(math.ceil(endstop_accuracy / step_dist))
        if self.endstop_accuracy >= self.phases // 2:
            raise config.error("Endstop for %s is not accurate enough for"
                               " stepper phase adjustment" % (self.name,))
        if self.printer.get_start_args().get('debugoutput') is not None:
            self.endstop_accuracy = self.phases
        # Register event handler
        self.printer.register_event_handler(
            "homing:homed_rails", self.handle_homed_rails)
    def align_endstop(self, pos):
        if not self.endstop_align_zero or self.endstop_phase is None:
            return pos
        # Adjust the endstop position so 0.0 is always at a full step
        microsteps = self.phases // 4
        half_microsteps = microsteps // 2
        phase_offset = (((self.endstop_phase + half_microsteps) % microsteps)
                        - half_microsteps) * self.step_dist
        full_step = microsteps * self.step_dist
        return int(pos / full_step + .5) * full_step + phase_offset
    def get_homed_offset(self, stepper):
        if self.get_phase is not None:
            try:
                phase = self.get_phase()
            except Exception as e:
                msg = "Unable to get stepper %s phase: %s" % (self.name, str(e))
                logging.exception(msg)
                raise homing.EndstopError(msg)
        else:
            phase = stepper.get_mcu_position() % self.phases
        if self.endstop_phase is None:
            logging.info("Setting %s endstop phase to %d", self.name, phase)
            self.endstop_phase = phase
            return 0.
        delta = (phase - self.endstop_phase) % self.phases
        if delta >= self.phases - self.endstop_accuracy:
            delta -= self.phases
        elif delta > self.endstop_accuracy:
            raise homing.EndstopError(
                "Endstop %s incorrect phase (got %d vs %d)" % (
                    self.name, phase, self.endstop_phase))
        return delta * self.step_dist
    def handle_homed_rails(self, homing_state, rails):
        for rail in rails:
            stepper = rail.get_steppers()[0]
            if stepper.get_name() != self.name:
                continue
            orig_pos = rail.get_commanded_position()
            offset = self.get_homed_offset(stepper)
            pos = self.align_endstop(orig_pos) + offset
            if pos == orig_pos:
                return False
            rail.set_commanded_position(pos)
            return True

def load_config_prefix(config):
    return EndstopPhase(config)
