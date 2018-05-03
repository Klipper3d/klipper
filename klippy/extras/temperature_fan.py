# Support fans that are enabled when temperature exceeds a set threshold
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import fan

PIN_MIN_TIME = 0.100
KELVIN_TO_CELCIUS = -273.15

class TemperatureFan:
    def __init__(self, config):
        self.name = config.get_name()
        self.printer = config.get_printer()
        self.fan = fan.PrinterFan(config)
        self.mcu = self.fan.mcu_fan.get_mcu()
        self.fan_speed = config.getfloat('fan_speed', 1., minval=0., maxval=1.)
        min_temp = config.getfloat('min_temp', minval=KELVIN_TO_CELCIUS)
        max_temp = config.getfloat('max_temp', above=min_temp)
        self.temp = config.getfloat('temp', 40. if max_temp > 40. else max_temp,
            minval=min_temp, maxval=max_temp)
        self.fan.set_shutdown_speed(1.)
        self.last_temp = KELVIN_TO_CELCIUS
        self.sensor = self.printer.lookup_object('heater').setup_sensor(config)
        self.sensor.setup_minmax(min_temp, max_temp)
        self.sensor.setup_callback(self.temperature_callback)
    def printer_state(self, state):
        if state == 'ready':
            reactor = self.printer.get_reactor()
            reactor.register_timer(self.callback, reactor.NOW)
    def callback(self, eventtime):
        power = 0.
        if self.last_temp > self.temp:
            power = self.fan_speed
        print_time = self.mcu.estimated_print_time(eventtime) + PIN_MIN_TIME
        self.fan.set_speed(print_time, power)
        return eventtime + 1.
    def temperature_callback(self, read_time, temp):
        self.last_temp = temp
    def stats(self, eventtime):
        return True, '%s: temp=%.1f' % (self.name, self.last_temp)

def load_config_prefix(config):
    return TemperatureFan(config)
