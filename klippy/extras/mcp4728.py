# MCP4728 dac code
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import bus

class mcp4728:
    def __init__(self, config):
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=0)
        scale = config.getfloat('scale', 1., above=0.)
        # Configure registers
        for i, name in enumerate('abcd'):
            val = config.getfloat('channel_%s' % (name,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                self.set_dac(i, int(val * 4095. / scale + .5))
    def set_dac(self, dac, value):
        self.i2c.i2c_write([0x40 | (dac << 1),
                            (value >> 8) & 0x0f, value & 0xff])

def load_config_prefix(config):
    return mcp4728(config)
