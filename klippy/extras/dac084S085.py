# SPI DAC DAC084S085 implementation for Alligator Board rev.2
#
# Copyright (C) 2021  Lorenzo Franco <lorenzo.franco@lorenzing.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from . import bus

class dac084S085:
    def __init__(self, config):
        self.spi = bus.MCU_SPI_from_config(
            config, 1, pin_option="enable_pin", default_speed=10000000)
        channel = [0,3,2,1,0 ]
        for i in range(5):
            vref = config.getint('channel%d' % (i,), None,
                                 minval=0., maxval=255)
            if vref is not None:
                buff = 0x01 << 12
                buff |= (channel[i]) << 14
                buff |= vref << 4
                self.spi.spi_send([buff])

def load_config_prefix(config):
    return dac084S085(config)
