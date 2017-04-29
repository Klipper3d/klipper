# Printer fan support
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

FAN_MIN_TIME = 0.1
PWM_CYCLE_TIME = 0.010

class PrinterFan:
    def __init__(self, printer, config):
        self.last_fan_value = 0.
        self.last_fan_time = 0.
        self.kick_start_time = config.getfloat('kick_start_time', 0.1, minval=0.)
        pin = config.get('pin')
        hard_pwm = config.getint('hard_pwm', 0)
        self.mcu_fan = printer.mcu.create_pwm(pin, PWM_CYCLE_TIME, hard_pwm, 0.)
    # External commands
    def set_speed(self, print_time, value):
        value = max(0., min(1., value))
        if value == self.last_fan_value:
            return
        mcu_time = self.mcu_fan.print_to_mcu_time(print_time)
        mcu_time = max(self.last_fan_time + FAN_MIN_TIME, mcu_time)
        if (value and value < 1.
            and not self.last_fan_value and self.kick_start_time):
            # Run fan at full speed for specified kick_start_time
            self.mcu_fan.set_pwm(mcu_time, 1.)
            mcu_time += self.kick_start_time
        self.mcu_fan.set_pwm(mcu_time, value)
        self.last_fan_time = mcu_time
        self.last_fan_value = value

def add_printer_objects(printer, config):
    if config.has_section('fan'):
        printer.add_object('fan', PrinterFan(printer, config.getsection('fan')))
