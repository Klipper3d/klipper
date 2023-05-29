# Support generic temperature sensors
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

KELVIN_TO_CELSIUS = -273.15
UPDATE_TEMPERATURE_INTERVALL = 1

class PrinterSensorGeneric:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        pheaters = self.printer.load_object(config, 'heaters')
        self.sensor = pheaters.setup_sensor(config)
        self.min_temp = config.getfloat('min_temp', KELVIN_TO_CELSIUS,
                                        minval=KELVIN_TO_CELSIUS)
        self.max_temp = config.getfloat('max_temp', 99999999.9,
                                        above=self.min_temp)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self.temperature_callback)
        pheaters.register_sensor(config, self)
        self.last_temp = 0.
        self.measured_min = 99999999.
        self.measured_max = 0.
    def temperature_callback(self, read_time, temp):
        self.last_temp = temp
        if temp:
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

class PrinterSensorCombined:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]
        # get sensor names
        self.sensor_names = config.getlist('sensors', None)
        # get type of algorithm to handle the different sensor values with
        algos = {'min': min, 'max': max, 'mean': mean}
        self.mode = config.getchoice('type', algos)
        # get heater
        pheaters = self.printer.lookup_object('heaters')
        raise config.error(self.printer.lookup_objects())
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
        # Register commands and event handlers
        self.printer.register_event_handler('klippy:ready',
                self._handle_ready)
        # update sensor
        self.temperature_update_timer = self.reactor.register_timer(
            self._temperature_update_event)
    def update_temp(self, eventtime):
        values = []
        for sensor_name in self.sensor_names:
            sensor = self.printer.lookup_object(sensor_name)
            temp_sensor, _ = sensor.get_temp(eventtime)
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
    def _handle_ready(self):
        # self.pheaters = self.printer.lookup_object('heaters')
        # Start temperature update timer
        self.reactor.update_timer(self.temperature_update_timer,
                                  self.reactor.NOW)
    def _temperature_update_event(self, eventtime):
        # update sensor value
        self.update_temp(eventtime)
        return eventtime + UPDATE_TEMPERATURE_INTERVALL

def mean(values):
    if not values:
        return
    return sum(values) / len(values)

def load_config_prefix(config):
    if config.get('sensor_type') == 'combined':
        return PrinterSensorCombined(config)
    else:
        return PrinterSensorGeneric(config)
