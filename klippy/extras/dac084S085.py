# SPI DAC DAC084S085 implementation
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus
import time

class dac084S085:
    def __init__(self, config):
        self.spi = bus.MCU_SPI_from_config(
            config, 1, pin_option="enable_pin", default_speed=10000000)

channel = [0,3,2,1,0 ]
        for i in range(1,5):
            vref = config.getint('motor%d' % (i,), None,
                                  minval=0., maxval=255)

            if vref is not None:
                buff = 0x01 << 12
                buff |= (channel[i]) << 14
                buff |= vref << 4
                self.spi.spi_send([buff])
                time.sleep(0.05)

def load_config_prefix(config):
    return dac084S085(config)
