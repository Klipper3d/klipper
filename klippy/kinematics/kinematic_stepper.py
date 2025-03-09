# Kinematic stepper class for generic cartesian kinematics
#
# Copyright (C) 2024  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging, re
import stepper, chelper

def parse_carriages_string(carriages_str, printer_carriages, parse_error):
    nxt = 0
    pat = re.compile('[+-]')
    coeffs = [0.] * 3
    ref_carriages = []
    while nxt < len(carriages_str):
        match = pat.search(carriages_str, nxt+1)
        end = len(carriages_str) if match is None else match.start()
        term = carriages_str[nxt:end].strip()
        term_lst = term.split('*')
        if len(term_lst) not in [1, 2]:
            raise parse_error(
                    "Invalid term '%s' in '%s'" % (term, carriages_str))
        if len(term_lst) == 2:
            try:
                coeff = float(term_lst[0])
            except ValueError:
                raise error("Invalid float '%s'" % term_lst[0])
        else:
            coeff = -1. if term_lst[0].startswith('-') else 1.
            if term_lst[0].startswith('-') or term_lst[0].startswith('+'):
                term_lst[0] = term_lst[0][1:]
        c = term_lst[-1]
        if c not in printer_carriages:
            raise parse_error("Invalid '%s' carriage referenced in '%s'" %
                              (c, carriages_str))
        carriage = printer_carriages[c]
        j = carriage.get_axis()
        if coeffs[j]:
            raise error("Carriage '%s' was referenced multiple times in '%s'" %
                        (c, carriages_str))
        coeffs[j] = coeff
        ref_carriages.append(carriage)
        nxt = end
    return coeffs, ref_carriages

class KinematicStepper:
    def __init__(self, config, printer_carriages):
        self.printer = config.get_printer()
        self.stepper = stepper.PrinterStepper(config)
        self.kin_coeffs, self.carriages = parse_carriages_string(
                config.get('carriages'), printer_carriages, config.error)
        if not any(self.kin_coeffs):
            raise config.error(
                    "'%s' must provide a valid 'carriages' configuration" %
                    self.stepper.get_name())
        self.stepper.setup_itersolve(
                'generic_cartesian_stepper_alloc',
                self.kin_coeffs[0], self.kin_coeffs[1], self.kin_coeffs[2])
        self.stepper_sk = self.stepper.get_stepper_kinematics()
        # Add stepper methods
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
        # Add stepper to the carriages it references
        for sc in self.carriages:
            sc.add_stepper(self)
    def get_name(self, short=False):
        name = self.stepper.get_name(short)
        if short and name.startswith('stepper '):
            return name[8:]
        return name
    def get_kin_coeffs(self):
        return tuple(self.kin_coeffs)
    def get_active_axes(self):
        return [i for i, c in enumerate(self.kin_coeffs) if c]
    def get_carriages(self):
        return self.carriages
    def update_carriages(self, carriages_str, printer_carriages, report_error):
        kin_coeffs, carriages = parse_carriages_string(
                carriages_str, printer_carriages,
                report_error or self.printer.command_error)
        if report_error is not None and not any(kin_coeffs):
            raise report_error(
                    "A valid string that references at least one carriage"
                    " must be provided for '%s'" % self.get_name())
        self.kin_coeffs = kin_coeffs
        self.carriages = carriages
        ffi_main, ffi_lib = chelper.get_ffi()
        ffi_lib.generic_cartesian_stepper_set_coeffs(
                self.stepper_sk, kin_coeffs[0], kin_coeffs[1], kin_coeffs[2])
