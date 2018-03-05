# Printer heater support
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, sys
from thermistor import Thermistor, ThermistorBeta, Linear

Sensors = {
    "EPCOS 100K B57560G104F": {
        'class': Thermistor, 't1': 25., 'r1': 100000.,
        't2': 150., 'r2': 1641.9, 't3': 250., 'r3': 226.15 },
    "ATC Semitec 104GT-2": {
        'class': Thermistor, 't1': 20., 'r1': 126800.,
        't2': 150., 'r2': 1360., 't3': 300., 'r3': 80.65 },
    "NTC 100K beta 3950": {
        'class': ThermistorBeta, 't1': 25., 'r1': 100000., 'beta': 3950. },
    "AD595": { 'class': Linear, 't1': 25., 'v1': .25, 't2': 300., 'v2': 3.022 },
}

def load_sensor(config):
    sensor_params = config.getchoice('sensor_type', Sensors)
    return sensor_params['class'](config, sensor_params)
