# Printer heater support
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
from sensorbase import SensorBase

KELVIN_TO_CELCIUS = -273.15

# Thermistor calibrated with three temp measurements
class Thermistor(SensorBase):
    def __init__(self, config, params):
        self.pullup = config.getfloat('pullup_resistor', 4700., above=0.)
        # Calculate Steinhart-Hart coefficents from temp measurements
        inv_t1 = 1. / (params['t1'] - KELVIN_TO_CELCIUS)
        inv_t2 = 1. / (params['t2'] - KELVIN_TO_CELCIUS)
        inv_t3 = 1. / (params['t3'] - KELVIN_TO_CELCIUS)
        ln_r1 = math.log(params['r1'])
        ln_r2 = math.log(params['r2'])
        ln_r3 = math.log(params['r3'])
        ln3_r1, ln3_r2, ln3_r3 = ln_r1**3, ln_r2**3, ln_r3**3

        inv_t12, inv_t13 = inv_t1 - inv_t2, inv_t1 - inv_t3
        ln_r12, ln_r13 = ln_r1 - ln_r2, ln_r1 - ln_r3
        ln3_r12, ln3_r13 = ln3_r1 - ln3_r2, ln3_r1 - ln3_r3

        self.c3 = ((inv_t12 - inv_t13 * ln_r12 / ln_r13)
                   / (ln3_r12 - ln3_r13 * ln_r12 / ln_r13))
        self.c2 = (inv_t12 - self.c3 * ln3_r12) / ln_r12
        self.c1 = inv_t1 - self.c2 * ln_r1 - self.c3 * ln3_r1
        SensorBase.__init__(self, config)
    def calc_temp(self, adc):
        adc = max(.00001, min(.99999, adc))
        r = self.pullup * adc / (1.0 - adc)
        ln_r = math.log(r)
        inv_t = self.c1 + self.c2 * ln_r + self.c3 * ln_r**3
        return 1.0/inv_t + KELVIN_TO_CELCIUS
    def calc_adc(self, temp):
        inv_t = 1. / (temp - KELVIN_TO_CELCIUS)
        if self.c3:
            y = (self.c1 - inv_t) / (2. * self.c3)
            x = math.sqrt((self.c2 / (3. * self.c3))**3 + y**2)
            ln_r = math.pow(x - y, 1./3.) - math.pow(x + y, 1./3.)
        else:
            ln_r = (inv_t - self.c1) / self.c2
        r = math.exp(ln_r)
        return r / (self.pullup + r)
    def check_faults(self, fault):
        pass

# Thermistor calibrated from one temp measurement and its beta
class ThermistorBeta(Thermistor):
    def __init__(self, config, params):
        self.pullup = config.getfloat('pullup_resistor', 4700., above=0.)
        # Calculate Steinhart-Hart coefficents from beta
        inv_t1 = 1. / (params['t1'] - KELVIN_TO_CELCIUS)
        ln_r1 = math.log(params['r1'])
        self.c3 = 0.
        self.c2 = 1. / params['beta']
        self.c1 = inv_t1 - self.c2 * ln_r1
        SensorBase.__init__(self, config)
    def check_faults(self, fault):
        pass

# Linear style conversion chips calibrated with two temp measurements
class Linear(SensorBase):
    def __init__(self, config, params):
        adc_voltage = config.getfloat('adc_voltage', 5., above=0.)
        slope = (params['t2'] - params['t1']) / (params['v2'] - params['v1'])
        self.gain = adc_voltage * slope
        self.offset = params['t1'] - params['v1'] * slope
        SensorBase.__init__(self, config)
    def calc_temp(self, adc):
        return adc * self.gain + self.offset
    def calc_adc(self, temp):
        return (temp - self.offset) / self.gain
    def check_faults(self, fault):
        pass
