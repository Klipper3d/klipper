# MCP4451 digipot code
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import mcu

WiperRegisters = [0x00, 0x01, 0x06, 0x07]

class mcp4451:
    def __init__(self, config):
        printer = config.get_printer()
        # Read config parameters
        self.mcu = mcu.get_printer_mcu(printer, config.get('mcu', 'mcu'))
        i2c_addr = config.getint('i2c_address')
        scale = config.getfloat('scale', 1., above=0.)
        wipers = [None]*4
        for i in range(len(wipers)):
            val = config.getfloat('wiper_%d' % (i,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                wipers[i] = int(val * 255. / scale + .5)
        # Setup i2c
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd(
            "config_i2c oid=%d bus=0 rate=100000 addr=%d" % (
                self.oid, i2c_addr))
        # Configure registers
        self.add_config_cmd(0x04, 0xff)
        self.add_config_cmd(0x0a, 0xff)
        for reg, val in zip(WiperRegisters, wipers):
            if val is not None:
                self.add_config_cmd(reg, val)
    def add_config_cmd(self, reg, val):
        self.mcu.add_config_cmd("i2c_write oid=%d data=%02x%02x" % (
            self.oid, (reg << 4) | ((val >> 8) & 0x03), val), is_init=True)

def load_config_prefix(config):
    return mcp4451(config)
