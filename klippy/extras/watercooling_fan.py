# Support fans for watercooling loops that should run when selected
# steppers are active, a heater has a target temperature, or a heater
# remains above a configured threshold.
#
# Copyright (C) 2026 Steven Kretzschmar <privat.06.steven@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import fan

PIN_MIN_TIME = 0.100


class WatercoolingFan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.stepper_names = config.getlist("stepper", None)
        self.stepper_enable = self.printer.load_object(config, 'stepper_enable')
        self.printer.load_object(config, 'heaters')
        self.heaters = []
        self.heater_names = config.getlist("heater", ("extruder",))
        self.heater_temp = config.getfloat("heater_temp", 50.0)
        self.fan = fan.Fan(config, default_shutdown_speed=1.)
        self.fan_speed = config.getfloat('fan_speed', default=1.,
                                         minval=0., maxval=1.)
        self.single_active_speed = config.getfloat(
            'single_active_speed', default=self.fan_speed,
            minval=0., maxval=1.)
        self.idle_speed = config.getfloat(
            'idle_speed', default=0., minval=0., maxval=1.)
        self.idle_timeout = config.getint("idle_timeout", default=30, minval=0)
        self.last_on = self.idle_timeout
        self.last_speed = 0.

    def handle_connect(self):
        pheaters = self.printer.lookup_object('heaters')
        self.heaters = [pheaters.lookup_heater(n) for n in self.heater_names]
        all_steppers = self.stepper_enable.get_steppers()
        if self.stepper_names is None:
            self.stepper_names = all_steppers
            return
        if not all(x in all_steppers for x in self.stepper_names):
            raise self.printer.config_error(
                "One or more of these steppers are unknown: "
                "%s (valid steppers are: %s)"
                % (self.stepper_names, ", ".join(all_steppers)))

    def handle_ready(self):
        reactor = self.printer.get_reactor()
        reactor.register_timer(
            self.callback, reactor.monotonic() + PIN_MIN_TIME)

    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)

    def callback(self, eventtime):
        speed = 0.
        stepper_active = False
        heater_active = False
        for name in self.stepper_names:
            stepper_active |= self.stepper_enable.lookup_enable(
                name).is_motor_enabled()
        for heater in self.heaters:
            current_temp, target_temp = heater.get_temp(eventtime)
            if target_temp or current_temp > self.heater_temp:
                heater_active = True
        if stepper_active and heater_active:
            self.last_on = 0
            speed = self.fan_speed
        elif stepper_active or heater_active:
            self.last_on = 0
            speed = self.single_active_speed
        elif self.last_on < self.idle_timeout:
            speed = self.idle_speed
            self.last_on += 1
        if speed != self.last_speed:
            self.last_speed = speed
            self.fan.set_speed(speed)
        return eventtime + 1.


def load_config_prefix(config):
    return WatercoolingFan(config)
