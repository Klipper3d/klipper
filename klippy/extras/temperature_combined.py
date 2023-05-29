# Support for combination of temperature sensors
#
# Copyright (C) 2023  Michael Jäger <michael@mjaeger.eu>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

UPDATE_TEMPERATURE_INTERVALL = 1.0


class PrinterSensorCombined:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]
        # get sensor names
        self.sensor_names = config.getlist('sensors')
        # get empty list for sensors
        self.sensors = []
        # get type of algorithm to handle the different sensor values with
        algos = {'min': min, 'max': max, 'mean': mean}
        self.apply_mode = config.getchoice('type', algos)
        # set default values
        self.last_temp = self.min_temp = self.max_temp = 0.0
        # add object
        self.printer.add_object("temperature_combined " + self.name, self)
        # update sensor
        self.temperature_update_timer = self.reactor.register_timer(
            self._temperature_update_event)
        # Register commands and event handlers
        self.printer.register_event_handler('klippy:connect',
                self._handle_connect)
        self.printer.register_event_handler('klippy:ready',
                self._handle_ready)

    def _handle_connect(self):
        for sensor_name in self.sensor_names:
            sensor = self.printer.lookup_object(sensor_name)
            # if temperature sensor
            if hasattr(sensor, 'get_temp'):
                self.sensors.append(sensor)
            # if heater
            elif hasattr(sensor, 'heater'):
                self.sensors.append(sensor.heater)

    def _handle_ready(self):
        # Start temperature update timer
        self.reactor.update_timer(self.temperature_update_timer,
                                  self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, temperature_callback):
        self.temperature_callback = temperature_callback

    def get_report_time_delta(self):
        return UPDATE_TEMPERATURE_INTERVALL

    def update_temp(self, eventtime):
        values = []
        for sensor in self.sensors:
            temp_sensor, _ = sensor.get_temp(eventtime)
            values.append(temp_sensor)
        temp = self.apply_mode(values)
        if temp:
            self.last_temp = temp

    def get_temp(self, eventtime):
        return self.last_temp, 0.

    def stats(self, eventtime):
        return False, '%s: temp=%.1f' % (self.name, self.last_temp)

    def get_status(self, eventtime):
        return {
            'temperature': round(self.last_temp, 2),
        }

    def _temperature_update_event(self, eventtime):
        # update sensor value
        self.update_temp(eventtime)
        self.temperature_callback(eventtime, self.last_temp)
        return eventtime + UPDATE_TEMPERATURE_INTERVALL


def mean(values):
    if not values:
        return
    return sum(values) / len(values)


def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("temperature_combined", PrinterSensorCombined)
