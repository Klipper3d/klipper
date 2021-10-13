# Temperature measurements with thermocouples. Only support for Type K for now.
# Cold junction compensation
#
# Copyright (C) 2021  Elias Bakken <elias@iagent.no>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import adc_temperature

TYPE_K_COEFFEICIENTS = [
     0.000000E+00,
     2.508355E+01,
     7.860106E-02,
    -2.503131E-01,
     8.315270E-02,
    -1.228034E-02,
     9.804036E-04,
    -4.413030E-05,
     1.057734E-06,
    -1.052755E-08
]

TYPE_K_INVERSE = [
 -0.176004136860E-01,
  0.389212049750E-01,
  0.185587700320E-04,
 -0.994575928740E-07,
  0.318409457190E-09,
 -0.560728448890E-12,
  0.560750590590E-15,
 -0.320207200030E-18,
  0.971511471520E-22,
 -0.121047212750E-25
]

TYPE_K_A = [
  0.118597600000E+00,
 -0.118343200000E-03,
  0.126968600000E+03
]

# Analog voltage to temperature converter for thermocouple
class RecoreThermocouple:
    def __init__(self, gain, offset, vin, cj_temp):
        self.gain = gain
        self.offset = offset
        self.vin = vin
        self.cj_temp = cj_temp

    def mv_to_temp(self, mv):
        return sum([coeff * mv**n  for n,coeff in enumerate(TYPE_K_COEFFEICIENTS)])

    def get_exp(self, temp):
        return TYPE_K_A[0] * math.exp(TYPE_K_A[1] * (temp - TYPE_K_A[2])**2)

    def temp_to_mv(self, temp):
        return sum([(coeff * temp**n) for n, coeff in enumerate(TYPE_K_INVERSE)]) + self.get_exp(temp)

    def calc_temp(self, adc):
        adc_val = max(.00001, min(.99999, adc))
        v_adc = self.vin*adc_val
        v_in = 1000.0*(v_adc-self.offset)/self.gain
        cj_temp = self.cj_temp.get_temp(0)[0]
        temp = cj_temp + self.mv_to_temp(v_in)
        return temp

    # We do not know the cj temp at the time of setting limits, setting to 35
    def calc_adc(self, temp):
        temp -= 35
        v_in = self.temp_to_mv(temp)
        v_adc = (v_in*self.gain/1000.0)+self.offset
        adc_val = v_adc/self.vin
        return adc_val

# Create an ADC converter with a thermocouple
def PrinterThermocouple(config):
    gain = config.getfloat('gain', 100)
    offset = config.getfloat('offset', 0.7)
    vin = config.getfloat('vin_adc', 3.3)
    cj_sensor = config.get('cj_sensor', 'temperature_sensor cold_junction')
    cj_temp = config.get_printer().load_object(config, cj_sensor)
    thermocouple = RecoreThermocouple(gain, offset, vin, cj_temp)
    return adc_temperature.PrinterADCtoTemperature(config, thermocouple)

# Default sensors
Sensors = ["Type K"]

def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    for sensor_type in Sensors:
        func = (lambda config: PrinterThermocouple(config))
        pheaters.add_sensor_factory(sensor_type, func)
