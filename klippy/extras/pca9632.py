# Support for the PCA9632 LED driver ic
#
# Copyright (C) 2022  Ricardo Alcantara <ricardo@vulcanolabs.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from .mcp4018 import SoftwareI2C

# Register addresses
PCA9632_MODE1 = 0x00
PCA9632_MODE2 = 0x01
PCA9632_PWM0 = 0x02
PCA9632_PWM1 = 0x03
PCA9632_PWM2 = 0x04
PCA9632_PWM3 = 0x05
PCA9632_LEDOUT = 0x08

LED_PWM = 0x02
PCA9632_RED = 0x00
PCA9632_GRN = 0x04
PCA9632_BLU = 0x02
PCA9632_WHT = 0x06

class PCA9632:
    def __init__(self, config):
        self.printer = config.get_printer()

        self.i2c = SoftwareI2C(config, 98)
        #i2c_addr = self.i2c.get_i2c_address()

        name = config.get_name().split()[1]
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("SET_LED","LED",name,self.set_led,
            desc="Set the color of an LED")

        self.led_red = config.getfloat("initial_RED",0.,0.,1.)
        self.led_green = config.getfloat("initial_GREEN",0.,0.,1.)
        self.led_blue = config.getfloat("initial_BLUE",0.,0.,1.)
        self.led_white = config.getfloat("initial_WHITE",0.,0.,1.)

        config.get_printer().register_event_handler("klippy:connect",
                                                    self.handle_connect)
    def handle_connect(self):
        self._init_PCA9632()
    def _init_PCA9632(self):
        #Configure MODE1
        self.i2c.i2c_write([PCA9632_MODE1,0x00])

        #Configure MODE2 (DIMMING, INVERT, CHANGE ON STOP,TOTEM)
        self.i2c.i2c_write([PCA9632_MODE2,0x15])

        self.led_red = int(self.led_red * 255.)
        self.led_blue = int(self.led_blue * 255.)
        self.led_green = int(self.led_green * 255.)
        self.led_white = int(self.led_white * 255.)

        self.i2c.i2c_write([PCA9632_PWM0,self.led_red])
        self.i2c.i2c_write([PCA9632_PWM1,self.led_blue])
        self.i2c.i2c_write([PCA9632_PWM2,self.led_green])
        self.i2c.i2c_write([PCA9632_PWM3,self.led_white])

        LEDOUT = (LED_PWM << PCA9632_RED if self.led_red else 0)
        LEDOUT |= (LED_PWM << PCA9632_GRN if self.led_green else 0)
        LEDOUT |= (LED_PWM << PCA9632_BLU if self.led_blue else 0)
        LEDOUT |= (LED_PWM << PCA9632_WHT if self.led_white else 0)

        self.i2c.i2c_write([PCA9632_LEDOUT,LEDOUT])
    def set_led(self, gcmd):
        self.led_red = gcmd.get_float("RED",self.led_red,0.,1.)
        self.led_green = gcmd.get_float("GREEN",self.led_green,0.,1.)
        self.led_blue = gcmd.get_float("BLUE",self.led_blue,0.,1.)
        self.led_white = gcmd.get_float("WHITE",self.led_white,0.,1.)

        self.led_red = int(self.led_red * 255.)
        self.led_blue = int(self.led_blue * 255.)
        self.led_green = int(self.led_green * 255.)
        self.led_white = int(self.led_white * 255.)

        self.i2c.i2c_write([PCA9632_PWM0,self.led_red])
        self.i2c.i2c_write([PCA9632_PWM1,self.led_blue])
        self.i2c.i2c_write([PCA9632_PWM2,self.led_green])
        self.i2c.i2c_write([PCA9632_PWM3,self.led_white])

        LEDOUT = (LED_PWM << PCA9632_RED if self.led_red else 0)
        LEDOUT |= (LED_PWM << PCA9632_GRN if self.led_green else 0)
        LEDOUT |= (LED_PWM << PCA9632_BLU if self.led_blue else 0)
        LEDOUT |= (LED_PWM << PCA9632_WHT if self.led_white else 0)

        self.i2c.i2c_write([PCA9632_LEDOUT,LEDOUT])

def load_config_prefix(config):
    return PCA9632(config)
