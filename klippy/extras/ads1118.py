# Support for multiple type k thermocouples on an ADS1118 adc
#
# Copyright (C) 2022  Jacob Dockter <dockterj@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
import pins
from . import bus

REPORT_TIME = 0.300
MAX_INVALID_COUNT = 3

######################################################################
# ADS1118
# This class implements the primary device that is reponsible
# for registering a chip with two pins (channels) and the
# loop that queries the cold junction temperatre and up to two
# thermocouple voltages
######################################################################

class ADS1118(object):
    def __init__(self, config):
        self._printer = config.get_printer()
        self._ppins = self._printer.lookup_object("pins")
        self._ppins.register_chip("ads1118", self)
        self._spi = bus.MCU_SPI_from_config(
            config, 1, pin_option="sensor_pin", default_speed=4000000)
        self._mcu = self._spi.get_mcu()
        self._oid = self._mcu.create_oid()
        self._mcu.register_config_callback(self._build_config)
    def _build_config(self):
        clock = self._mcu.get_query_slot(self._oid)
        self._report_clock = self._mcu.seconds_to_clock(REPORT_TIME)
        self._mcu.add_config_cmd(
            "config_ads1118 oid=%u spi_oid=%u clock=%u rest_ticks=%u " \
            "max_invalid_count=%u" % (self._oid, self._spi.get_oid(),
            clock, self._report_clock, MAX_INVALID_COUNT))
    def get_mcu(self):
        return self._mcu
    def config_channel(self, oid, pin_number, min_sample_value,
        max_sample_value):
        self._mcu.add_config_cmd(
            "config_ads1118_channel oid=%u pin_number=%u " \
                "min_sample_value=%hi max_sample_value=%hi parent_oid=%u"
                % (oid, int(pin_number), min_sample_value, max_sample_value,
                self._oid))

def load_config(config):
    return ADS1118(config)
