# Heater handling on homing moves
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class HomingHeaters:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.printer.register_event_handler("homing:homing_move_begin",
                                            self.handle_homing_move_begin)
        self.printer.register_event_handler("homing:homing_move_end",
                                            self.handle_homing_move_end)
        self.heaters_to_disable = config.get("heaters", "")
        self.disable_heaters = []
        self.steppers_needing_quiet = config.get("steppers", "")
        self.flaky_steppers = []
        self.pheaters = self.printer.load_object(config, 'heaters')
        self.target_save = {}

    def handle_connect(self):
        # heaters to disable
        all_heaters = self.pheaters.get_all_heaters()
        self.disable_heaters = [n.strip()
                          for n in self.heaters_to_disable.split(',')]
        if self.disable_heaters == [""]:
            self.disable_heaters = all_heaters
        else:
            if not all(x in all_heaters for x in self.disable_heaters):
                raise self.printer.config_error(
                    "One or more of these heaters are unknown: %s" % (
                        self.disable_heaters))
        # steppers valid?
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        all_steppers = [s.get_name() for s in kin.get_steppers()]
        self.flaky_steppers = [n.strip()
                         for n in self.steppers_needing_quiet.split(',')]
        if self.flaky_steppers == [""]:
            return
        if not all(x in all_steppers for x in self.flaky_steppers):
            raise self.printer.config_error(
                "One or more of these steppers are unknown: %s" % (
                    self.flaky_steppers))
    def check_eligible(self, endstops):
        if self.flaky_steppers == [""]:
            return True
        steppers_being_homed = [s.get_name()
                                for es in endstops
                                for s in es.get_steppers()]
        return any(x in self.flaky_steppers for x in steppers_being_homed)
    def handle_homing_move_begin(self, hmove):
        if not self.check_eligible(hmove.get_mcu_endstops()):
            return
        for heater_name in self.disable_heaters:
            heater = self.pheaters.lookup_heater(heater_name)
            self.target_save[heater_name] = heater.get_temp(0)[1]
            heater.set_temp(0.)
    def handle_homing_move_end(self, hmove):
        if not self.check_eligible(hmove.get_mcu_endstops()):
            return
        for heater_name in self.disable_heaters:
            heater = self.pheaters.lookup_heater(heater_name)
            heater.set_temp(self.target_save[heater_name])

def load_config(config):
    return HomingHeaters(config)
