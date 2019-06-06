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
# Linear interpolation
######################################################################

# Helper code to perform linear interpolation
class LinearInterpolate:
    def __init__(self, samples):
        self.keys = []
        self.slopes = []
        last_key = last_value = None
        for key, value in sorted(samples):
            if last_key is None:
                last_key = key
                last_value = value
                continue
            if key <= last_key:
                raise ValueError("duplicate value")
            gain = (value - last_value) / (key - last_key)
            offset = last_value - last_key * gain
            if self.slopes and self.slopes[-1] == (gain, offset):
                continue
            last_value = value
            last_key = key
            self.keys.append(key)
            self.slopes.append((gain, offset))
        if not self.keys:
            raise ValueError("need at least two samples")
        self.keys.append(9999999999999.)
        self.slopes.append(self.slopes[-1])
    def interpolate(self, key):
        pos = bisect.bisect(self.keys, key)
        gain, offset = self.slopes[pos]
        return key * gain + offset
    def reverse_interpolate(self, value):
        values = [key * gain + offset for key, (gain, offset) in zip(
            self.keys, self.slopes)]
        if values[0] < values[-2]:
            valid = [i for i in range(len(values)) if values[i] >= value]
        else:
            valid = [i for i in range(len(values)) if values[i] <= value]
        gain, offset = self.slopes[min(valid + [len(values) - 1])]
        return (value - offset) / gain


######################################################################
# Linear voltage to temperature converter
######################################################################

# Linear style conversion chips calibrated from temperature measurements
class LinearVoltage:
    def __init__(self, config, params):
        adc_voltage = config.getfloat('adc_voltage', 5., above=0.)
        samples = []
        for temp, volt in params:
            adc = volt / adc_voltage
            if adc < 0. or adc > 1.:
                logging.warn("Ignoring adc sample %.3f/%.3f in heater %s",
                             temp, volt, config.get_name())
                continue
            samples.append((adc, temp))
        try:
            li = LinearInterpolate(samples)
        except ValueError as e:
            raise config.error("adc_temperature %s in heater %s" % (
                str(e), config.get_name()))
        self.calc_temp = li.interpolate
        self.calc_adc = li.reverse_interpolate

# Custom defined sensors from the config file
class CustomLinearVoltage:
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
        lv = LinearVoltage(config, self.params)
        return PrinterADCtoTemperature(config, lv)


######################################################################
# Linear resistance to temperature converter
######################################################################

# Linear resistance calibrated from temperature measurements
class LinearResistance:
    def __init__(self, config, samples):
        self.pullup = config.getfloat('pullup_resistor', 4700., above=0.)
        try:
            self.li = LinearInterpolate(samples)
        except ValueError as e:
            raise config.error("adc_temperature %s in heater %s" % (
                str(e), config.get_name()))
    def calc_temp(self, adc):
        # Calculate temperature from adc
        adc = max(.00001, min(.99999, adc))
        r = self.pullup * adc / (1.0 - adc)
        return self.li.interpolate(r)
    def calc_adc(self, temp):
        # Calculate adc reading from a temperature
        r = self.li.reverse_interpolate(temp)
        return r / (self.pullup + r)

# Custom defined sensors from the config file
class CustomLinearResistance:
    def __init__(self, config):
        self.name = " ".join(config.get_name().split()[1:])
        self.samples = []
        for i in range(1, 1000):
            t = config.getfloat("temperature%d" % (i,), None)
            if t is None:
                break
            r = config.getfloat("resistance%d" % (i,))
            self.samples.append((r, t))
    def create(self, config):
        lr = LinearResistance(config, self.samples)
        return PrinterADCtoTemperature(config, lr)


######################################################################
# Default sensors
######################################################################

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
                PrinterADCtoTemperature(config, LinearVoltage(config, params)))
        pheater.add_sensor_factory(sensor_type, func)

def load_config_prefix(config):
    if config.get("resistance1", None) is None:
        custom_sensor = CustomLinearVoltage(config)
    else:
        custom_sensor = CustomLinearResistance(config)
    pheater = config.get_printer().lookup_object("heater")
    pheater.add_sensor_factory(custom_sensor.name, custom_sensor.create)
