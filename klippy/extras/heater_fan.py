# Support fans that are enabled when a heater is on
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import fan

PIN_MIN_TIME = 0.100

class PrinterHeaterFan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.heater_name = config.get("heater", "extruder0")
        self.heater_temp = config.getfloat("heater_temp", 50.0)
        self.fan = fan.PrinterFan(config)
        self.mcu = self.fan.mcu_fan.get_mcu()
        self.fan_speed = config.getfloat("fan_speed", 1., minval=0., maxval=1.)
        self.fan.set_shutdown_speed(1.)
    def printer_state(self, state):
        if state == 'ready':
            pheater = self.printer.lookup_object('heater')
            self.heater = pheater.lookup_heater(self.heater_name)
            reactor = self.printer.get_reactor()
            reactor.register_timer(self.callback, reactor.NOW)
    def callback(self, eventtime):
        current_temp, target_temp = self.heater.get_temp(eventtime)
        power = 0.
        if target_temp or current_temp > self.heater_temp:
            power = self.fan_speed
        print_time = self.mcu.estimated_print_time(eventtime) + PIN_MIN_TIME
        self.fan.set_speed(print_time, power)
        return eventtime + 1.

def load_config_prefix(config):
    return PrinterHeaterFan(config)
