# AD5206 digipot code
#
# Copyright (C) 2017,2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus

class ad5206:
    def __init__(self, config):
        self.spi = bus.MCU_SPI_from_config(
            config, 0, pin_option="enable_pin", default_speed=25000000)
        scale = config.getfloat('scale', 1., above=0.)
        for i in range(6):
            val = config.getfloat('channel_%d' % (i+1,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                self.set_register(i, int(val * 256. / scale + .5))
    def set_register(self, reg, value):
        self.spi.spi_send([reg, value])

def load_config_prefix(config):
    return ad5206(config)
