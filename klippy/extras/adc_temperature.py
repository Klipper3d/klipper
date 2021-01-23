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
        query_adc = config.get_printer().load_object(config, 'query_adc')
        query_adc.register_adc(config.get_name(), self.mcu_adc)
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
        voltage_offset = config.getfloat('voltage_offset', 0.0)
        samples = []
        for temp, volt in params:
            adc = (volt - voltage_offset) / adc_voltage
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
            self.li = LinearInterpolate([(r, t) for t, r in samples])
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
            self.samples.append((t, r))
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

AD597 = [
    (0., 0.), (10., .097), (20., .196), (25., .245), (30., .295),
    (40., 0.395), (50., 0.496), (60., 0.598), (80., 0.802), (100., 1.005),
    (120., 1.207), (140., 1.407), (160., 1.605), (180., 1.801), (200., 1.997),
    (220., 2.194), (240., 2.392), (260., 2.592), (280., 2.794), (300., 2.996),
    (320., 3.201), (340., 3.406), (360., 3.611), (380., 3.817), (400., 4.024),
    (420., 4.232), (440., 4.440), (460., 4.649), (480., 4.857), (500., 5.066)
]

AD8494 = [
    (-180, -0.714), (-160, -0.658), (-140, -0.594), (-120, -0.523),
    (-100, -0.446), (-80, -0.365), (-60, -0.278), (-40, -0.188),
    (-20, -0.095), (0, 0.002), (20, 0.1), (25, 0.125), (40, 0.201),
    (60, 0.303), (80, 0.406), (100, 0.511), (120, 0.617), (140, 0.723),
    (160, 0.829), (180, 0.937), (200, 1.044), (220, 1.151), (240, 1.259),
    (260, 1.366), (280, 1.473), (300, 1.58), (320, 1.687), (340, 1.794),
    (360, 1.901), (380, 2.008), (400, 2.114), (420, 2.221), (440, 2.328),
    (460, 2.435), (480, 2.542), (500, 2.65), (520, 2.759), (540, 2.868),
    (560, 2.979), (580, 3.09), (600, 3.203), (620, 3.316), (640, 3.431),
    (660, 3.548), (680, 3.666), (700, 3.786), (720, 3.906), (740, 4.029),
    (760, 4.152), (780, 4.276), (800, 4.401), (820, 4.526), (840, 4.65),
    (860, 4.774), (880, 4.897), (900, 5.018), (920, 5.138), (940, 5.257),
    (960, 5.374), (980, 5.49), (1000, 5.606), (1020, 5.72), (1040, 5.833),
    (1060, 5.946), (1080, 6.058), (1100, 6.17), (1120, 6.282), (1140, 6.394),
    (1160, 6.505), (1180, 6.616), (1200, 6.727)
]

AD8495 = [
    (-260, -0.786), (-240, -0.774), (-220, -0.751), (-200, -0.719),
    (-180, -0.677), (-160, -0.627), (-140, -0.569), (-120, -0.504),
    (-100, -0.432), (-80, -0.355), (-60, -0.272), (-40, -0.184), (-20, -0.093),
    (0, 0.003), (20, 0.1), (25, 0.125), (40, 0.2), (60, 0.301), (80, 0.402),
    (100, 0.504), (120, 0.605), (140, 0.705), (160, 0.803), (180, 0.901),
    (200, 0.999), (220, 1.097), (240, 1.196), (260, 1.295), (280, 1.396),
    (300, 1.497), (320, 1.599), (340, 1.701), (360, 1.803), (380, 1.906),
    (400, 2.01), (420, 2.113), (440, 2.217), (460, 2.321), (480, 2.425),
    (500, 2.529), (520, 2.634), (540, 2.738), (560, 2.843), (580, 2.947),
    (600, 3.051), (620, 3.155), (640, 3.259), (660, 3.362), (680, 3.465),
    (700, 3.568), (720, 3.67), (740, 3.772), (760, 3.874), (780, 3.975),
    (800, 4.076), (820, 4.176), (840, 4.275), (860, 4.374), (880, 4.473),
    (900, 4.571), (920, 4.669), (940, 4.766), (960, 4.863), (980, 4.959),
    (1000, 5.055), (1020, 5.15), (1040, 5.245), (1060, 5.339), (1080, 5.432),
    (1100, 5.525), (1120, 5.617), (1140, 5.709), (1160, 5.8), (1180, 5.891),
    (1200, 5.98), (1220, 6.069), (1240, 6.158), (1260, 6.245), (1280, 6.332),
    (1300, 6.418), (1320, 6.503), (1340, 6.587), (1360, 6.671), (1380, 6.754)
]

AD8496 = [
    (-180, -0.642), (-160, -0.59), (-140, -0.53), (-120, -0.464),
    (-100, -0.392), (-80, -0.315), (-60, -0.235), (-40, -0.15), (-20, -0.063),
    (0, 0.027), (20, 0.119), (25, 0.142), (40, 0.213), (60, 0.308),
    (80, 0.405), (100, 0.503), (120, 0.601), (140, 0.701), (160, 0.8),
    (180, 0.9), (200, 1.001), (220, 1.101), (240, 1.201), (260, 1.302),
    (280, 1.402), (300, 1.502), (320, 1.602), (340, 1.702), (360, 1.801),
    (380, 1.901), (400, 2.001), (420, 2.1), (440, 2.2), (460, 2.3),
    (480, 2.401), (500, 2.502), (520, 2.603), (540, 2.705), (560, 2.808),
    (580, 2.912), (600, 3.017), (620, 3.124), (640, 3.231), (660, 3.34),
    (680, 3.451), (700, 3.562), (720, 3.675), (740, 3.789), (760, 3.904),
    (780, 4.02), (800, 4.137), (820, 4.254), (840, 4.37), (860, 4.486),
    (880, 4.6), (900, 4.714), (920, 4.826), (940, 4.937), (960, 5.047),
    (980, 5.155), (1000, 5.263), (1020, 5.369), (1040, 5.475), (1060, 5.581),
    (1080, 5.686), (1100, 5.79), (1120, 5.895), (1140, 5.999), (1160, 6.103),
    (1180, 6.207), (1200, 6.311)
]

AD8497 = [
    (-260, -0.785), (-240, -0.773), (-220, -0.751), (-200, -0.718),
    (-180, -0.676), (-160, -0.626), (-140, -0.568), (-120, -0.503),
    (-100, -0.432), (-80, -0.354), (-60, -0.271), (-40, -0.184),
    (-20, -0.092), (0, 0.003), (20, 0.101), (25, 0.126), (40, 0.2),
    (60, 0.301), (80, 0.403), (100, 0.505), (120, 0.605), (140, 0.705),
    (160, 0.804), (180, 0.902), (200, 0.999), (220, 1.097), (240, 1.196),
    (260, 1.296), (280, 1.396), (300, 1.498), (320, 1.599), (340, 1.701),
    (360, 1.804), (380, 1.907), (400, 2.01), (420, 2.114), (440, 2.218),
    (460, 2.322), (480, 2.426), (500, 2.53), (520, 2.634), (540, 2.739),
    (560, 2.843), (580, 2.948), (600, 3.052), (620, 3.156), (640, 3.259),
    (660, 3.363), (680, 3.466), (700, 3.569), (720, 3.671), (740, 3.773),
    (760, 3.874), (780, 3.976), (800, 4.076), (820, 4.176), (840, 4.276),
    (860, 4.375), (880, 4.474), (900, 4.572), (920, 4.67), (940, 4.767),
    (960, 4.863), (980, 4.96), (1000, 5.055), (1020, 5.151), (1040, 5.245),
    (1060, 5.339), (1080, 5.433), (1100, 5.526), (1120, 5.618), (1140, 5.71),
    (1160, 5.801), (1180, 5.891), (1200, 5.981), (1220, 6.07), (1240, 6.158),
    (1260, 6.246), (1280, 6.332), (1300, 6.418), (1320, 6.503), (1340, 6.588),
    (1360, 6.671), (1380, 6.754)
]

def calc_pt100(base=100.):
    # Calc PT100/PT1000 resistances using Callendar-Van Dusen formula
    A, B = (3.9083e-3, -5.775e-7)
    return [(float(t), base * (1. + A*t + B*t*t)) for t in range(0, 500, 10)]

def calc_ina826_pt100():
    # Standard circuit is 4400ohm pullup with 10x gain to 5V
    return [(t, 10. * 5. * r / (4400. + r)) for t, r in calc_pt100()]

DefaultVoltageSensors = [
    ("AD595", AD595), ("AD597", AD597), ("AD8494", AD8494), ("AD8495", AD8495),
    ("AD8496", AD8496), ("AD8497", AD8497),
    ("PT100 INA826", calc_ina826_pt100())
]

DefaultResistanceSensors = [
    ("PT1000", calc_pt100(1000.))
]

def load_config(config):
    # Register default sensors
    pheaters = config.get_printer().load_object(config, "heaters")
    for sensor_type, params in DefaultVoltageSensors:
        func = (lambda config, params=params:
                PrinterADCtoTemperature(config, LinearVoltage(config, params)))
        pheaters.add_sensor_factory(sensor_type, func)
    for sensor_type, params in DefaultResistanceSensors:
        func = (lambda config, params=params:
                PrinterADCtoTemperature(config,
                                        LinearResistance(config, params)))
        pheaters.add_sensor_factory(sensor_type, func)

def load_config_prefix(config):
    if config.get("resistance1", None) is None:
        custom_sensor = CustomLinearVoltage(config)
    else:
        custom_sensor = CustomLinearResistance(config)
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory(custom_sensor.name, custom_sensor.create)
