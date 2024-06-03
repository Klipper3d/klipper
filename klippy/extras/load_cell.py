# Load Cell Implementation
#
# Copyright (C) 2024 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import hx71x

# Printer class that controls a load cell
class LoadCell:
    def __init__(self, config, sensor):
        self.printer = printer = config.get_printer()
        self.sensor = sensor   # must implement BulkAdcSensor
        # startup, when klippy is ready, start capturing data
        printer.register_event_handler("klippy:ready", self._handle_ready)

    def _handle_ready(self):
        self.sensor.add_client(self._on_sample)

    def _on_sample(self, msg):
        return True

    def get_sensor(self):
        return self.sensor

    def get_status(self, eventtime):
        return {}

def load_config(config):
    # Sensor types
    sensors = {}
    sensors.update(hx71x.HX71X_SENSOR_TYPES)
    sensor_class = config.getchoice('sensor_type', sensors)
    return LoadCell(config, sensor_class(config))

def load_config_prefix(config):
    return load_config(config)
