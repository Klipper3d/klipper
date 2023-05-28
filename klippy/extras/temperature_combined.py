# Support generic temperature sensors
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

KELVIN_TO_CELSIUS = -273.15

class PrinterSensorCombined:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        # get sensor names
        self.sensor_names = config.getlist("sensors", None)
        # get type of algorithm to handle the different sensor values with
        algos = {'min': min, 'max': max, 'mean': mean}
        self.mode = config.getchoice('type', algos)
        # get heater
        pheaters = self.printer.lookup_object('heaters')
        # set min / max temp
        self.min_temp = config.getfloat('min_temp', KELVIN_TO_CELSIUS,
                                        minval=KELVIN_TO_CELSIUS)
        self.max_temp = config.getfloat('max_temp', 99999999.9,
                                        above=self.min_temp)
        # register sensor
        pheaters.register_sensor(config, self)
        # set default values
        self.last_temp = 0.
        self.measured_min = 99999999.
        self.measured_max = 0.
        # update original sensor callbacks
        for sensor_name in self.sensor_names:
            sensor = self.printer.lookup_object(sensor_name)
            def new_temperature_callback(read_time, temp):
                sensor.temperature_callback(read_time, temp)
                self.temperature_callback(read_time, temp)
            sensor.setup_callback(new_temperature_callback)
    def temperature_callback(self, read_time, temp):
        values = []
        for sensor_name in self.sensor_names:
            sensor = self.printer.lookup_object(sensor_name)
            temp_sensor, _ = sensor.get_temp(read_time)
            values.append(temp_sensor)
        temp = self.mode(values)
        if temp:
            self.last_temp = temp
            self.measured_min = min(self.measured_min, temp)
            self.measured_max = max(self.measured_max, temp)
    def get_temp(self, eventtime):
        return self.last_temp, 0.
    def stats(self, eventtime):
        return False, '%s: temp=%.1f' % (self.name, self.last_temp)
    def get_status(self, eventtime):
        return {
            'temperature': round(self.last_temp, 2),
            'measured_min_temp': round(self.measured_min, 2),
            'measured_max_temp': round(self.measured_max, 2)
        }

def load_config_prefix(config):
    return PrinterSensorCombined(config)

def mean(values):
    if not values:
        return
    return sum(values) / len(values)
