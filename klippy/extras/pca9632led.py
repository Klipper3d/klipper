# PCA9632 LED controller on i2c
#
# Copyright (C) 2021 Jacob Dockter <dockterj@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus

#note that the address is actually 0b11000100 but the i2c library expects
#     7 bit addresses.  We can't read from the chip (requires 0b11000101)
PCA9632_I2C_ADDR = 0b1100010

LED_AUTO_INC_ALL = 0b10000000

LED_GROUP_BLINK = 0b00100000
LED_OUT_INVERTED = 0b00010000
LED_OUT_DRIVE = 0b00000100

#pca9632 registers:
# 0 mode 1
# 1 mode 2
# 2 PWM0 (blue)
# 3 PWM1 (green)
# 4 PWM2 (red)
# 5 PWM3 (n/c)
# 6 GRPPWM
# 7 GRPFREQ
# 8 LEDOUT

# Set up the control byte to start with reg 0 and autoincrement through all
PCA9632CONTROL = [0x00 | LED_AUTO_INC_ALL]
# Set up the default data to turn off blink and set PWM to 0 for all LEDs
# (blink duty cycle set to 50% - GRPPWM)
PCA9632DATA = [ 0b00000001, 
            0b00100000 | 0b00010000 | 0b00000100, 
            0, 0, 0, 0, 128, 0, 
            0b10101010]

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

class PCA9632LED:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=PCA9632_I2C_ADDR, default_speed=100000)
        self.blue = 0
        self.green = 0
        self.red = 0
        self.blink = 0
        self.printer.register_event_handler("klippy:connect",
                                            self.send_data)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_LED", "LED", self.name,
                                    self.cmd_SET_LED,
                                    desc=self.cmd_SET_LED_help)
    def send_data(self, print_time=None):
        minclock = 0
        if print_time is not None:
            minclock = self.i2c.get_mcu().print_time_to_clock(print_time)
        data = PCA9632DATA
        data[2] = self.blue
        data[3] = self.green
        data[4] = self.red
        data[7] = self.blink
        if self.blink > 0:
            # turn on blink for the LEDs but keep individual PWM control
            data[8] = 0b11111111
        self.i2c.i2c_write((PCA9632CONTROL + data), minclock=minclock,
                          reqclock=BACKGROUND_PRIORITY_CLOCK)
    cmd_SET_LED_help = "Set the color of an LED"
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        blink = gcmd.get_float('BLINK', 0., minval=0., maxval=1.)
        self.red = int(red * 255. + .5)
        self.green = int(green * 255. + .5)
        self.blue = int(blue * 255. + .5)
        self.blink = int(blink * 255. + .5)
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(self.send_data)
def load_config_prefix(config):
    return PCA9632LED(config)
