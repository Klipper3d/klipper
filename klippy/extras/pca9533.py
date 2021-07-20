# Support for the PCA9533 LED driver ic
#
# Copyright (C) 2021  Marc-Andre Denis <marcadenis@msn.com>
#               2021  Dean Cording <dean@cording.id.au>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

PCA9533_PSC0=0b001
PCA9533_PWM0=0b010
PCA9533_PSC1=0b011
PCA9533_PWM1=0b100
PCA9533_PLS0=0b101

class PCA9533:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=98)
        #i2c_addr = self.i2c.get_i2c_address()

        name = config.get_name().split()[1]
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("SET_LED","LED",name,self.set_led,
            desc="Set the color of an LED")

        self.led0 = config.getint("initial_RED",0,0,1)
        self.led1 = config.getint("initial_GREEN",0,0,1)
        self.led2 = config.getint("initial_BLUE",0,0,1)
        self.led3 = config.getint("initial_WHITE",0,0,1)

        self.i2c.i2c_write([PCA9533_PSC0,0])
        self.i2c.i2c_write([PCA9533_PSC1,0])
        self.set_leds(self.led0,self.led1,self.led2,self.led3)
    def set_led(self, gcmd):
        self.led0 = gcmd.get_float("RED",self.led0,0,1)
        self.led1 = gcmd.get_float("GREEN",self.led1,0,1)
        self.led2 = gcmd.get_float("BLUE",self.led2,0,1)
        self.led3 = gcmd.get_float("WHITE",self.led3,0,1)

        self.set_leds(self.led0,self.led1,self.led2,self.led3)

    def set_leds(self,*leds):

        ls0 = 0
        pwm0 = 0
        pwm1 = 0

        for index, led in enumerate(leds):
            if led == 0:
                continue
            if led == 1:
                ls0 |=  0b01<<(2*index)
            elif pwm0 == 0:
                pwm0 = led
                ls0 |= 0b10<<(2*index)
            elif pwm1 == 0:
                pwm1 = led
                ls0 |= 0b11<<(2*index)
            elif abs(pwm0 - led) < abs(pwm1 - led):
                pwm0 = (pwm0 + led) / 2
                ls0 |= 0b10<<(2*index)
            else:
                pwm1 = (pwm1 + led) / 2
                ls0 |= 0b11<<(2*index)

        self.i2c.i2c_write([PCA9533_PWM0,int(255*pwm0)])
        self.i2c.i2c_write([PCA9533_PWM1,int(255*pwm1)])
        self.i2c.i2c_write([PCA9533_PLS0,ls0])

def load_config_prefix(config):
    return PCA9533(config)
