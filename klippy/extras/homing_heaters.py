# Heater handling on homing moves
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class HomingHeaters:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("homing:move_begin",
                                            self.handle_homing_move_begin)
        self.printer.register_event_handler("homing:move_end",
                                            self.handle_homing_move_end)
        self.heaters_to_disable = config.get("heaters", "")
        self.steppers_needing_quiet = config.get("steppers", "")
        self.last_target = {}
        self.pheater = self.printer.lookup_object('heater')
    def check_eligible(self, endstops):
        flaky_steppers = [n.strip()
                         for n in self.steppers_needing_quiet.split(',')]
        if flaky_steppers == [""]:
            return True
        steppers_being_homed = [s.get_name()
                                for es, name in endstops
                                for s in es.get_steppers()]
        return any(x in flaky_steppers for x in steppers_being_homed)
    def bld_heater_list(self):
        heaters = [n.strip() for n in self.heaters_to_disable.split(',')]
        if heaters == [""]:
            heaters = self.pheater.available_heaters
        return heaters
    def handle_homing_move_begin(self, endstops):
        if not self.check_eligible(endstops):
            return
        for heater_name in self.bld_heater_list():
            heater = self.pheater.lookup_heater(heater_name)
            self.last_target[heater_name] = heater.get_temp(0)[1]
            heater.set_temp(0.)
    def handle_homing_move_end(self, endstops):
        if not self.check_eligible(endstops):
            return
        for heater_name in self.bld_heater_list():
            heater = self.pheater.lookup_heater(heater_name)
            heater.set_temp(self.last_target[heater_name])

def load_config(config):
    return HomingHeaters(config)
