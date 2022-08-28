# SPI DAC DAC084S085 implementation
#
# Copyright (C) 2021  Lorenzo Franco <lorenzo.franco@lorenzing.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from . import bus

class dac084S085:
    def __init__(self, config):
        self.spi = bus.MCU_SPI_from_config(
            config, 1, pin_option="enable_pin", default_speed=10000000)
        scale = config.getfloat('scale', 1., above=0.)
        for chan, name in enumerate("ABCD"):
            val = config.getfloat('channel_%s' % (name,), None,
                                   minval=0., maxval=scale)
            if val is not None:
                self.set_register(chan, int(val * 255. / scale))
    def set_register(self, chan, value):
        b1 = (chan << 6) | (1 << 4) | ((value >> 4) & 0x0f)
        b2 = (value << 4) & 0xf0
        self.spi.spi_send([b1, b2])

def load_config_prefix(config):
    return dac084S085(config)
