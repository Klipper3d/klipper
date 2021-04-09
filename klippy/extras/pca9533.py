# Support for the PCA9533 LED driver ic
#
# Copyright (C) 2021  Marc-Andre Denis <marcadenis@msn.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

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

        ls0 = (self.led3<<6) | (self.led2<<4) | (self.led1<<2) | (self.led0)

        self.i2c.i2c_write([PCA9533_PLS0,ls0])
    def set_led(self, gcmd):
        if gcmd.get_float("RED",self.led0,0,1) != 0:
            self.led0 = 1
        else:
            self.led0 = 0
        if gcmd.get_float("GREEN",self.led1,0,1) != 0:
            self.led1 = 1
        else:
            self.led1 = 0
        if gcmd.get_float("BLUE",self.led2,0,1) != 0:
            self.led2 = 1
        else:
            self.led2 = 0
        if gcmd.get_float("WHITE",self.led3,0,1) != 0:
            self.led3 = 1
        else:
            self.led3 = 0
        ls0 = (self.led3<<6) | (self.led2<<4) | (self.led1<<2) | (self.led0)

        self.i2c.i2c_write([PCA9533_PLS0,ls0])


def load_config_prefix(config):
    return PCA9533(config)
