# Kinematic stepper class for generic cartesian kinematics
#
# Copyright (C) 2024  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import re
import stepper, chelper

def parse_kinematic_string(s, error):
    nxt = 0
    pat = re.compile('[+-]')
    res = [0.] * 3
    while nxt < len(s):
        match = pat.search(s, nxt+1)
        end = len(s) if match is None else match.start()
        term = s[nxt:end].strip()
        term_lst = term.split("*")
        if len(term_lst) not in [1, 2]:
            raise error("Invalid term '%s' in kinematics '%s'" % (term, s))
        if len(term_lst) == 2:
            try:
                coeff = float(term_lst[0])
            except ValueError:
                raise error("Invalid float '%s'" % term_lst[0])
        else:
            coeff = -1. if term_lst[0].startswith('-') else 1.
        jj = [i for i, a in enumerate("xyz")
              if term_lst[-1] in ['-' + a, '+' + a, a]]
        if len(jj) != 1:
            raise error("Invalid term '%s' in kinematics '%s'" % (term, s))
        if res[jj[0]]:
            raise error("Axis '%s' was specified multiple times in "
                        "kinematics '%s'" % ("xyz"[i], s))
        res[jj[0]] = coeff
        nxt = end
    return res

class KinematicStepper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.stepper = stepper.PrinterStepper(config)
        kinematics_str = config.get('kinematics')
        self.kin_coeffs = parse_kinematic_string(kinematics_str, config.error)
        if not any(self.kin_coeffs):
            raise config.error(
                    "'%s' must provide a valid 'kinematics' configuration" %
                    self.stepper.get_name())
        self.carriage = self.config.get('carriage', None)
        self.endstop_pin = self.config.get('endstop_pin', None)
        if self.endstop_pin is not None and self.carriage is None and \
                len(self.get_active_axes()) > 1:
                    raise config.error(
                            "kinematics '%s' for '%s' requires that 'carriage' "
                            "is set in order to use 'endstop_pin'" % (
                                kinematics_str, self.stepper.get_name()))
        # TODO: Add shortcuts for some typical optimized kinematics
        self.stepper.setup_itersolve(
                'generic_cartesian_stepper_alloc',
                self.kin_coeffs[0], self.kin_coeffs[1], self.kin_coeffs[2])
        self.get_name = self.stepper.get_name
        self.get_step_dist = self.stepper.get_step_dist
        self.units_in_radians = self.stepper.units_in_radians
        self.get_pulse_duration = self.stepper.get_pulse_duration
        self.setup_default_pulse_duration = \
                self.stepper.setup_default_pulse_duration
        self.get_mcu = self.stepper.get_mcu
        self.get_oid = self.stepper.get_oid
        self.get_trapq = self.stepper.get_trapq
        self.set_trapq = self.stepper.set_trapq
        self.get_dir_inverted = self.stepper.get_dir_inverted
        self.set_dir_inverted = self.stepper.set_dir_inverted
        self.set_position = self.stepper.set_position
        self.get_commanded_position = self.stepper.get_commanded_position
        self.get_mcu_position = self.stepper.get_mcu_position
        self.get_past_mcu_position = self.stepper.get_past_mcu_position
        self.mcu_to_commanded_position = self.stepper.mcu_to_commanded_position
        self.dump_steps = self.stepper.dump_steps
        self.get_stepper_kinematics = self.stepper.get_stepper_kinematics
        self.set_stepper_kinematics = self.stepper.set_stepper_kinematics
        self.note_homing_end = self.stepper.note_homing_end
        self.calc_position_from_coord = self.stepper.calc_position_from_coord
        self.generate_steps = self.stepper.generate_steps
        self.add_active_callback = self.stepper.add_active_callback
        self.is_active_axis = self.stepper.is_active_axis
    def get_kin_coeffs(self):
        return tuple(self.kin_coeffs)
    def get_active_axes(self):
        return [i for i, c in enumerate(self.kin_coeffs) if c]
    def get_carriage(self):
        return self.carriage
    def add_to_rail(self, i, rail):
        active_axes = self.get_active_axes()
        endstop_pin = None
        if rail.get_name() == self.get_carriage() or active_axes == [i]:
            endstop_pin = self.config.get('endstop_pin', None)
        rail.add_stepper(self, endstop_pin)


def LoadKinematicSteppers(config):
    return [KinematicStepper(c)
            for c in config.get_prefix_sections('kinematic_stepper')]
