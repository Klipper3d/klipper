# Load Cell Implementation
#
# Copyright (C) 2024 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import hx71x
from . import ads1220

# Printer class that controls a load cell
class LoadCell:
    def __init__(self, config, sensor):
        self.printer = printer = config.get_printer()
        self.sensor = sensor   # must implement BulkAdcSensor

    def _on_sample(self, msg):
        return True

    def get_sensor(self):
        return self.sensor

def load_config(config):
    # Sensor types
    sensors = {}
    sensors.update(hx71x.HX71X_SENSOR_TYPES)
    sensors.update(ads1220.ADS1220_SENSOR_TYPE)
    sensor_class = config.getchoice('sensor_type', sensors)
    return LoadCell(config, sensor_class(config))

def load_config_prefix(config):
    return load_config(config)
