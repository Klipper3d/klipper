# Obtain temperature using linear interpolation of ADC values
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

SAMPLE_TIME = 0.001
SAMPLE_COUNT = 8
REPORT_TIME = 0.300

# Linear style conversion chips calibrated with two temp measurements
class Linear:
    def __init__(self, config, params):
        adc_voltage = config.getfloat('adc_voltage', 5., above=0.)
        ppins = config.get_printer().lookup_object('pins')
        self.mcu_adc = ppins.setup_pin('adc', config.get('sensor_pin'))
        self.mcu_adc.setup_adc_callback(REPORT_TIME, self.adc_callback)
        self.temperature_callback = None
        slope = (params['t2'] - params['t1']) / (params['v2'] - params['v1'])
        self.gain = adc_voltage * slope
        self.offset = params['t1'] - params['v1'] * slope
    def setup_minmax(self, min_temp, max_temp):
        adc_range = [self.calc_adc(min_temp), self.calc_adc(max_temp)]
        self.mcu_adc.setup_minmax(SAMPLE_TIME, SAMPLE_COUNT,
                                  minval=min(adc_range), maxval=max(adc_range))
    def setup_callback(self, temperature_callback):
        self.temperature_callback = temperature_callback
    def get_report_time_delta(self):
        return REPORT_TIME
    def adc_callback(self, read_time, read_value):
        temp = read_value * self.gain + self.offset
        self.temperature_callback(read_time + SAMPLE_COUNT * SAMPLE_TIME, temp)
    def calc_adc(self, temp):
        return (temp - self.offset) / self.gain

Sensors = {
    "AD595": { 't1': 25., 'v1': .25, 't2': 300., 'v2': 3.022 },
}

def load_config(config):
    # Register default sensors
    pheater = config.get_printer().lookup_object("heater")
    for sensor_type, params in Sensors.items():
        func = (lambda config, params=params: Linear(config, params))
        pheater.add_sensor(sensor_type, func)
