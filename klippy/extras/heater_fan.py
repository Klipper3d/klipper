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
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.heater_name = config.get("heater", "extruder0")
        self.heater_temp = config.getfloat("heater_temp", 50.0)
        self.heaters = []
        self.fan = fan.PrinterFan(config, default_shutdown_speed=1.)
        self.mcu = self.fan.mcu_fan.get_mcu()
        self.fan_speed = config.getfloat("fan_speed", 1., minval=0., maxval=1.)
    def handle_ready(self):
        pheater = self.printer.lookup_object('heater')
        self.heaters = [pheater.lookup_heater(n.strip())
                        for n in self.heater_name.split(',')]
        reactor = self.printer.get_reactor()
        reactor.register_timer(self.callback, reactor.NOW)
    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)
    def callback(self, eventtime):
        power = 0.
        for heater in self.heaters:
            current_temp, target_temp = heater.get_temp(eventtime)
            if target_temp or current_temp > self.heater_temp:
                power = self.fan_speed
        print_time = self.mcu.estimated_print_time(eventtime) + PIN_MIN_TIME
        self.fan.set_speed(print_time, power)
        return eventtime + 1.

def load_config_prefix(config):
    return PrinterHeaterFan(config)
