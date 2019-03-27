# Support a fan for cooling the MCU whenever a stepper or heater is on
#
# Copyright (C) 2019  Nils Friedchen <nils.friedchen@googlemail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import fan

PIN_MIN_TIME = 0.100

class ControllerFan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.steppers = []
        self.fan = fan.PrinterFan(config)
        self.mcu = self.fan.mcu_fan.get_mcu()
        self.max_power = config.getfloat(
            'max_power', 1., minval=0., maxval=1.)
        self.idle_speed = config.getfloat(
            'idle_speed', self.max_power, minval=0., maxval=self.max_power)
        self.idle_timeout = config.getint("idle_timeout", 30, minval=0)
        self.heater_name = config.get("heater", "extruder")
        self.last_on = self.idle_timeout
    def handle_ready(self):
        pheater = self.printer.lookup_object('heater')
        self.heaters = [pheater.lookup_heater(n.strip())
                        for n in self.heater_name.split(',')]
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        self.steppers = kin.get_steppers()
        reactor = self.printer.get_reactor()
        reactor.register_timer(self.callback, reactor.NOW)
    def callback(self, eventtime):
        power = 0.
        active = False
        for stepper in self.steppers:
            active |= stepper.is_motor_enabled()
        for heater in self.heaters:
            currentTemp, targetTemp = heater.get_temp(eventtime)
            if targetTemp:
                active = True
        if active:
            self.last_on = 0
            power = self.max_power
        else:
            if self.last_on < self.idle_timeout:
                power = self.idle_speed
                self.last_on += 1
        print_time = self.mcu.estimated_print_time(eventtime) + PIN_MIN_TIME
        self.fan.set_speed(print_time, power)
        return eventtime + 1.

def load_config_prefix(config):
    return ControllerFan(config)
