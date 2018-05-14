# Printer heater support
#
# Copyright (C) 2018  Petri Honkala <cruwaller@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from thermocouple import Thermocouple
from rtd import RTD

Sensors = {
    # Thermocouples readers (SPI)
    "MAX6675": {
        'class': Thermocouple, 'simple': True},
    "MAX31855": {
        'class': Thermocouple, 'simple': True},
    "MAX31856": {
        'class': Thermocouple, 'simple': False},
    # RTD readers (SPI)
    "MAX31865": {
        'class': RTD},
}

def load_sensor(config):
    sensor_params = config.getchoice('sensor_type', Sensors)
    return sensor_params['class'](config, sensor_params)
