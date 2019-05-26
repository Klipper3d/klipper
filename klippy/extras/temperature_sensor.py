# Support generic temperature sensors
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

KELVIN_TO_CELCIUS = -273.15

class PrinterSensorGeneric:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.sensor = self.printer.lookup_object('heater').setup_sensor(config)
        self.min_temp = config.getfloat('min_temp', KELVIN_TO_CELCIUS,
                                        minval=KELVIN_TO_CELCIUS)
        self.max_temp = config.getfloat('max_temp', 99999999.9,
                                        above=self.min_temp)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self.temperature_callback)
        self.printer.lookup_object('heater').register_sensor(config, self)
        self.last_temp = 0.
    def temperature_callback(self, read_time, temp):
        self.last_temp = temp
    def get_temp(self, eventtime):
        return self.last_temp, 0.

def load_config_prefix(config):
    return PrinterSensorGeneric(config)
