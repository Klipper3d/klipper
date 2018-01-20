# Printer fan support
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import extruder, pins

FAN_MIN_TIME = 0.1
PWM_CYCLE_TIME = 0.010

class PrinterFan:
    def __init__(self, printer, config):
        self.last_fan_value = 0.
        self.last_fan_time = 0.
        self.max_power = config.getfloat('max_power', 1., above=0., maxval=1.)
        self.kick_start_time = config.getfloat('kick_start_time', 0.1, minval=0.)
        self.mcu_fan = pins.setup_pin(printer, 'pwm', config.get('pin'))
        self.mcu_fan.setup_max_duration(0.)
        self.mcu_fan.setup_cycle_time(PWM_CYCLE_TIME)
        self.mcu_fan.setup_hard_pwm(config.getint('hard_pwm', 0))
    def set_speed(self, print_time, value):
        value = max(0., min(self.max_power, value))
        if value == self.last_fan_value:
            return
        print_time = max(self.last_fan_time + FAN_MIN_TIME, print_time)
        if (value and value < self.max_power
            and not self.last_fan_value and self.kick_start_time):
            # Run fan at full speed for specified kick_start_time
            self.mcu_fan.set_pwm(print_time, self.max_power)
            print_time += self.kick_start_time
        self.mcu_fan.set_pwm(print_time, value)
        self.last_fan_time = print_time
        self.last_fan_value = value

class PrinterHeaterFan:
    def __init__(self, printer, config):
        self.fan = PrinterFan(printer, config)
        self.mcu = printer.lookup_object('mcu')
        heater = config.get("heater", "extruder0")
        self.heater = extruder.get_printer_heater(printer, heater)
        self.heater_temp = config.getfloat("heater_temp", 50.0)
        max_power = self.fan.max_power
        self.fan_speed = config.getfloat(
            "fan_speed", max_power, minval=0., maxval=max_power)
        self.fan.mcu_fan.setup_start_value(0., max_power)
        reactor = printer.get_reactor()
        reactor.register_timer(self.callback, reactor.NOW)
    def callback(self, eventtime):
        current_temp, target_temp = self.heater.get_temp(eventtime)
        if not current_temp and not target_temp and not self.fan.last_fan_time:
            # Printer still starting
            return eventtime + 1.
        power = 0.
        if target_temp or current_temp > self.heater_temp:
            power = self.fan_speed
        print_time = self.mcu.estimated_print_time(eventtime) + FAN_MIN_TIME
        self.fan.set_speed(print_time, power)
        return eventtime + 1.

def add_printer_objects(printer, config):
    if config.has_section('fan'):
        printer.add_object('fan', PrinterFan(printer, config.getsection('fan')))
    for s in config.get_prefix_sections('heater_fan '):
        printer.add_object(s.section, PrinterHeaterFan(printer, s))
