# Temperature measurements with thermistors
#
# Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2021  Elias Bakken <elias@iagent.no>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import adc_temperature
from . import thermistor

KELVIN_TO_CELSIUS = -273.15

class RecoreThermistor(thermistor.Thermistor):
    def __init__(self, pullup, inline_resistor, adc_ref, pullup_ref, vo_ref):
         thermistor.Thermistor.__init__(self, pullup, inline_resistor)
         self.adc_ref = adc_ref
         self.pullup_ref = pullup_ref
         self.vo_ref = vo_ref

    def calc_temp(self, adc):
        # Calculate temperature from adc
        adc = max(.00001, min(.99999, adc))
        Vo = self.adc_ref*adc
        R2 = 100000
        R1 = 1000

        I1 = (self.vo_ref-Vo)/R2
        VR1 = I1*R1
        V3 = Vo-VR1
        I5 = (self.pullup_ref-V3)/self.pullup
        I3 = (I5+I1)
        r = V3/I3

        ln_r = math.log(r - self.inline_resistor)
        inv_t = self.c1 + self.c2 * ln_r + self.c3 * ln_r**3
        return 1.0/inv_t + KELVIN_TO_CELSIUS

# Create an ADC converter with a thermistor
def RecorePrinterThermistor(config, params):
    pullup = config.getfloat('pullup_resistor', 4700., above=0.)
    inline_resistor = config.getfloat('inline_resistor', 0., minval=0.)
    adc_ref = config.getfloat('adc_ref', 3.3)
    pullup_ref = config.getfloat('pullup_ref', 3.2)
    vo_ref = config.getfloat('vo_ref', 0)
    thermistor = RecoreThermistor(pullup, inline_resistor, adc_ref,
                                  pullup_ref, vo_ref)

    if 'beta' in params:
        thermistor.setup_coefficients_beta(
            params['t1'], params['r1'], params['beta'])
    else:
        thermistor.setup_coefficients(
            params['t1'], params['r1'], params['t2'], params['r2'],
            params['t3'], params['r3'], name=config.get_name())
    return adc_temperature.PrinterADCtoTemperature(config, thermistor)

# Custom defined thermistors from the config file
class RecoreCustomThermistor:
    def __init__(self, config):
        self.name = " ".join(config.get_name().split()[1:])
        t1 = config.getfloat("temperature1", minval=KELVIN_TO_CELSIUS)
        r1 = config.getfloat("resistance1", minval=0.)
        beta = config.getfloat("beta", None, above=0.)
        if beta is not None:
            self.params = {'t1': t1, 'r1': r1, 'beta': beta}
            return
        t2 = config.getfloat("temperature2", minval=KELVIN_TO_CELSIUS)
        r2 = config.getfloat("resistance2", minval=0.)
        t3 = config.getfloat("temperature3", minval=KELVIN_TO_CELSIUS)
        r3 = config.getfloat("resistance3", minval=0.)
        (t1, r1), (t2, r2), (t3, r3) = sorted([(t1, r1), (t2, r2), (t3, r3)])
        self.params = {'t1': t1, 'r1': r1, 't2': t2, 'r2': r2,
                       't3': t3, 'r3': r3}
    def create(self, config):
        return RecorePrinterThermistor(config, self.params)

def load_config_prefix(config):
    thermistor = RecoreCustomThermistor(config)
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory(thermistor.name, thermistor.create)
