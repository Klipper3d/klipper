# Support for the PCA9533 LED driver ic
#
# Copyright (C) 2021  Marc-Andre Denis <marcadenis@msn.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus, led

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

PCA9533_PWM0=0b010
PCA9533_PWM1=0b100
PCA9533_PLS0=0b101

class PCA9533:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=98)
        self.led_helper = led.LEDHelper(config, self.update_leds, 1)
        self.i2c.i2c_write([PCA9533_PWM0, 85])
        self.i2c.i2c_write([PCA9533_PWM1, 170])
        self.update_leds(self.led_helper.get_status()['color_data'], None)
    def update_leds(self, led_state, print_time):
        rmap = [0, 2, 3, 1, 1]
        red, green, blue, white = [rmap[int(v * 4.)] for v in led_state[0]]
        ls0 = (white<<6) | (blue<<4) | (green<<2) | red
        minclock = 0
        if print_time is not None:
            minclock = self.i2c.get_mcu().print_time_to_clock(print_time)
        self.i2c.i2c_write([PCA9533_PLS0, ls0], minclock=minclock,
                           reqclock=BACKGROUND_PRIORITY_CLOCK)
    def get_status(self, eventtime):
        return self.led_helper.get_status(eventtime)

def load_config_prefix(config):
    return PCA9533(config)
