# Printer cooling fan
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins

FAN_MIN_TIME = 0.1
PWM_CYCLE_TIME = 0.010

class PrinterFan:
    def __init__(self, config):
        self.last_fan_value = 0.
        self.last_fan_time = 0.
        self.max_power = config.getfloat('max_power', 1., above=0., maxval=1.)
        self.kick_start_time = config.getfloat('kick_start_time', 0.1, minval=0.)
        printer = config.get_printer()
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

def load_config(config):
    if config.get_name() != 'fan':
        raise config.error("Invalid print cooling fan config name")
    return PrinterFan(config)
