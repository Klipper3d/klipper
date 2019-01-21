# Obtain temperature using linear interpolation of ADC values
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, bisect


######################################################################
# Interface between MCU adc and heater temperature callbacks
######################################################################

SAMPLE_TIME = 0.001
SAMPLE_COUNT = 8
REPORT_TIME = 0.300
RANGE_CHECK_COUNT = 4

# Interface between ADC and heater temperature callbacks
class PrinterADCtoTemperature:
    def __init__(self, config, adc_convert):
        self.adc_convert = adc_convert
        ppins = config.get_printer().lookup_object('pins')
        self.mcu_adc = ppins.setup_pin('adc', config.get('sensor_pin'))
        self.mcu_adc.setup_adc_callback(REPORT_TIME, self.adc_callback)
    def setup_callback(self, temperature_callback):
        self.temperature_callback = temperature_callback
    def get_report_time_delta(self):
        return REPORT_TIME
    def adc_callback(self, read_time, read_value):
        temp = self.adc_convert.calc_temp(read_value)
        self.temperature_callback(read_time + SAMPLE_COUNT * SAMPLE_TIME, temp)
    def setup_minmax(self, min_temp, max_temp):
        adc_range = [self.adc_convert.calc_adc(t) for t in [min_temp, max_temp]]
        self.mcu_adc.setup_minmax(SAMPLE_TIME, SAMPLE_COUNT,
                                  minval=min(adc_range), maxval=max(adc_range),
                                  range_check_count=RANGE_CHECK_COUNT)


######################################################################
# Linear voltage sensors
######################################################################

# Linear style conversion chips calibrated with two temp measurements
class Linear:
    def __init__(self, config, params):
        self.adc_samples = []
        self.slope_samples = []
        self.calc_coefficients(config, params)
    def calc_coefficients(self, config, params):
        adc_voltage = config.getfloat('adc_voltage', 5., above=0.)
        last_volt = last_temp = None
        for volt, temp in sorted([(v, t) for t, v in params]):
            adc = volt / adc_voltage
            if adc < 0. or adc > 1.:
                logging.warn("Ignoring adc sample %.3f/%.3f in heater %s",
                             temp, volt, config.get_name())
                continue
            if last_volt is None:
                last_volt = volt
                last_temp = temp
                continue
            if volt <= last_volt:
                raise config.error("adc_temperature duplicate voltage")
            slope = (temp - last_temp) / (volt - last_volt)
            gain = adc_voltage * slope
            offset = last_temp - last_volt * slope
            if self.slope_samples and self.slope_samples[-1] == (gain, offset):
                continue
            last_temp = temp
            last_volt = volt
            self.adc_samples.append(adc)
            self.slope_samples.append((gain, offset))
        if not self.adc_samples:
            raise config.error(
                "adc_temperature needs two volt and temperature measurements")
        self.adc_samples[-1] = 1.
    def calc_temp(self, adc):
        pos = bisect.bisect(self.adc_samples, adc)
        gain, offset = self.slope_samples[pos]
        return read_value * gain + offset
    def calc_adc(self, temp):
        temps = [adc * gain + offset for adc, (gain, offset) in zip(
            self.adc_samples, self.slope_samples)]
        if temps[0] < temps[-1]:
            pos = min([i for i in range(len(temps)) if temps[i] >= temp]
                      + [len(temps) - 1])
        else:
            pos = min([i for i in range(len(temps)) if temps[i] <= temp]
                      + [len(temps) - 1])
        gain, offset = self.slope_samples[pos]
        return (temp - offset) / gain

# Custom defined sensors from the config file
class CustomLinear:
    def __init__(self, config):
        self.name = " ".join(config.get_name().split()[1:])
        self.params = []
        for i in range(1, 1000):
            t = config.getfloat("temperature%d" % (i,), None)
            if t is None:
                break
            v = config.getfloat("voltage%d" % (i,))
            self.params.append((t, v))
    def create(self, config):
        lv = Linear(config, self.params)
        return PrinterADCtoTemperature(config, lv)

AD595 = [
    (0., .0027), (10., .101), (20., .200), (25., .250), (30., .300),
    (40., .401), (50., .503), (60., .605), (80., .810), (100., 1.015),
    (120., 1.219), (140., 1.420), (160., 1.620), (180., 1.817), (200., 2.015),
    (220., 2.213), (240., 2.413), (260., 2.614), (280., 2.817), (300., 3.022),
    (320., 3.227), (340., 3.434), (360., 3.641), (380., 3.849), (400., 4.057),
    (420., 4.266), (440., 4.476), (460., 4.686), (480., 4.896)
]

PT100 = [
    (0, 0.00), (1, 1.11), (10, 1.15), (20, 1.20), (30, 1.24), (40, 1.28),
    (50, 1.32), (60, 1.36), (70, 1.40), (80, 1.44), (90, 1.48), (100, 1.52),
    (110, 1.56), (120, 1.61), (130, 1.65), (140, 1.68), (150, 1.72),
    (160, 1.76), (170, 1.80), (180, 1.84), (190, 1.88), (200, 1.92),
    (210, 1.96), (220, 2.00), (230, 2.04), (240, 2.07), (250, 2.11),
    (260, 2.15), (270, 2.18), (280, 2.22), (290, 2.26), (300, 2.29),
    (310, 2.33), (320, 2.37), (330, 2.41), (340, 2.44), (350, 2.48),
    (360, 2.51), (370, 2.55), (380, 2.58), (390, 2.62), (400, 2.66),
    (500, 3.00), (600, 3.33), (700, 3.63), (800, 3.93), (900, 4.21),
    (1000, 4.48), (1100, 4.73)
]

def load_config(config):
    # Register default sensors
    pheater = config.get_printer().lookup_object("heater")
    for sensor_type, params in [("AD595", AD595), ("PT100 INA826", PT100)]:
        func = (lambda config, params=params:
                PrinterADCtoTemperature(config, Linear(config, params)))
        pheater.add_sensor(sensor_type, func)

def load_config_prefix(config):
    linear = CustomLinear(config)
    pheater = config.get_printer().lookup_object("heater")
    pheater.add_sensor(linear.name, linear.create)
