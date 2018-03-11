# Heater/sensor verification code
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import extruder

class HeaterCheck:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.heater_name = config.get_name().split()[1]
        self.heater = None
        self.hysteresis = config.getfloat('hysteresis', 10., above=0.)
        self.check_time = config.getfloat('check_time', 15., minval=1.)
        self.heating_gain = config.getfloat('heating_gain', 2., above=0.)
        default_gain_time = 20.
        if self.heater_name == 'heater_bed':
            default_gain_time = 60.
        self.check_gain_time = config.getfloat(
            'check_gain_time', default_gain_time, minval=1.)
        self.met_target = False
        self.last_target = self.goal_temp = 0.
        self.fault_systime = self.printer.get_reactor().NEVER
    def printer_state(self, state):
        if state == 'connect':
            self.heater = extruder.get_printer_heater(
                self.printer, self.heater_name)
            logging.info("Starting heater checks for %s", self.heater_name)
            reactor = self.printer.get_reactor()
            reactor.register_timer(self.check_event, reactor.NOW)
    def check_event(self, eventtime):
        temp, target = self.heater.get_temp(eventtime)
        if temp >= target - self.hysteresis:
            # Temperature near target - reset checks
            if not self.met_target:
                logging.info("Heater %s within range of %.3f",
                             self.heater_name, target)
            self.met_target = True
            self.fault_systime = eventtime + self.check_time
        elif self.met_target:
            if target != self.last_target:
                # Target changed - reset checks
                logging.info("Heater %s approaching new target of %.3f",
                             self.heater_name, target)
                self.met_target = False
                self.goal_temp = temp + self.heating_gain
                self.fault_systime = eventtime + self.check_gain_time
            elif eventtime >= self.fault_systime:
                # Failure due to inability to maintain target temperature
                return self.heater_fault()
        elif temp >= self.goal_temp:
            # Temperature approaching target - reset checks
            self.goal_temp = temp + self.heating_gain
            self.fault_systime = eventtime + self.check_gain_time
        elif eventtime >= self.fault_systime:
            # Failure due to inability to approach target temperature
            return self.heater_fault()
        self.last_target = target
        return eventtime + 1.
    def heater_fault(self):
        logging.error("Heater %s not heating at expected rate", self.heater_name)
        self.printer.invoke_shutdown("Heater %s failsafe" % (self.heater_name,))
        return self.printer.get_reactor().NEVER

def load_config_prefix(config):
    return HeaterCheck(config)
