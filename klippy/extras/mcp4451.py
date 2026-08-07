# MCP4451 digipot code
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus

WiperRegisters = [0x00, 0x01, 0x06, 0x07]

class mcp4451:
    def __init__(self, config):
        self.i2c = bus.MCU_I2C_from_config(config)
        i2c_addr = self.i2c.get_i2c_address()
        if i2c_addr < 44 or i2c_addr > 47:
            raise config.error("mcp4451 address must be between 44 and 47")
        scale = config.getfloat('scale', 1., above=0.)
        # Configure registers
        self.set_register(0x04, 0xff)
        self.set_register(0x0a, 0xff)
        for i in range(4):
            val = config.getfloat('wiper_%d' % (i,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                val = int(val * 255. / scale + .5)
                self.set_register(WiperRegisters[i], val)
    def set_register(self, reg, value):
        self.i2c.i2c_write([(reg << 4) | ((value >> 8) & 0x03), value])

def load_config_prefix(config):
    return mcp4451(config)
