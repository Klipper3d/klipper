# Obtain temperature using linear interpolation of ADC values, custom to Recore
#
# Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2021  Elias Bakken <elias@iagent.no>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, bisect
import math
from . import adc_temperature
######################################################################
# Interface between MCU adc and heater temperature callbacks
######################################################################

SAMPLE_TIME = 0.001
SAMPLE_COUNT = 8
REPORT_TIME = 0.300
RANGE_CHECK_COUNT = 4

class RecoreLinearResistance(adc_temperature.LinearResistance):
    def __init__(self, config, samples):
        adc_temperature.LinearResistance.__init__(self, config, samples)
        self.adc_ref = config.getfloat('adc_voltage', 3.3)
        self.pullup_ref = config.getfloat('pullup_voltage', 3.3)
        self.vo_ref = config.getfloat('offset_voltage', 0.050)
        self.a = 1.84633422e-07
        self.b = -3.15418589e-07
        self.c = 3.16706799e-07
        self.d = -5.08889891e-08

    def smf5a_iv_curve(self, v):
        return self.a*v**3 + self.b*v**2 + self.c*v + self.d

    def calc_temp(self, adc):
        # Calculate temperature from adc
        adc = max(.00001, min(.99999, adc))
        Vo = self.adc_ref*adc
        R2 = 100000
        R1 = 1000

        I1 = (Vo-self.vo_ref)/R2 # current through offset resistor
        VR1 = I1*R1
        V3 = Vo+VR1 # Voltage at the divider.

        ID = self.smf5a_iv_curve(V3) # TVS diode current
        I5 = (self.pullup_ref-V3)/self.pullup
        I3 = (I5-I1-ID)
        r = V3/(I3)
        return self.li.interpolate(r)

class RecoreCustomLinearResistance:
    def __init__(self, config):
        self.name = " ".join(config.get_name().split()[1:])
        self.samples = []
        for i in range(1, 1000):
            t = config.getfloat("temperature%d" % (i,), None)
            if t is None:
                break
            r = config.getfloat("resistance%d" % (i,))
            self.samples.append((t, r))
    def create(self, config):
        lr = RecoreLinearResistance(config, self.samples)
        return adc_temperature.PrinterADCtoTemperature(config, lr)

DefaultResistanceSensors = [
    ("RECORE PT1000", adc_temperature.calc_pt100(1000.))
]

def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    for sensor_type, params in DefaultResistanceSensors:
        func = (lambda config, params=params:
                adc_temperature.PrinterADCtoTemperature(config,
                RecoreLinearResistance(config, params)))
        pheaters.add_sensor_factory(sensor_type, func)

def load_config_prefix(config):
    custom_sensor = RecoreCustomLinearResistance(config)
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory(custom_sensor.name, custom_sensor.create)
