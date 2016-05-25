# Printer fan support
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

FAN_MIN_TIME = 0.1

class PrinterFan:
    def __init__(self, printer, config):
        self.printer = printer
        self.config = config
        self.mcu_fan = None
        self.last_fan_clock = self.last_fan_value = 0
        self.min_fan_clock = 0
        self.kick_start_clock = 0
    def build_config(self):
        pin = self.config.get('pin')
        hard_pwm = self.config.getint('hard_pwm', 128)
        mcu_freq = self.printer.mcu.get_mcu_freq()
        self.min_fan_clock = int(FAN_MIN_TIME * mcu_freq)
        kst = self.config.getfloat('kick_start_time', 0.1)
        self.kick_start_clock = int(kst * mcu_freq)
        self.mcu_fan = self.printer.mcu.create_pwm(pin, hard_pwm, 0)
    # External commands
    def set_speed(self, print_time, value):
        value = max(0, min(255, int(value*255. + 0.5)))
        if value == self.last_fan_value:
            return
        pc = int(self.mcu_fan.get_print_clock(print_time))
        pc = max(self.last_fan_clock + self.min_fan_clock, pc)
        if (value and value < 255
            and not self.last_fan_value and self.kick_start_clock):
            # Run fan at full speed for specified kick_start_time
            self.mcu_fan.set_pwm(pc, 255)
            pc += self.kick_start_clock
        self.mcu_fan.set_pwm(pc, value)
        self.last_fan_clock = pc
        self.last_fan_value = value
