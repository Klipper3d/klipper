# -*- coding: utf-8 -*-
# Support for combination of temperature sensors
#
# Copyright (C) 2023  Michael Jäger <michael@mjaeger.eu>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

REPORT_TIME = 0.300


class PrinterSensorCombined:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]
        # get sensor names
        self.sensor_names = config.getlist('sensor_list')
        # get maximum_deviation parameter from config
        self.max_deviation = config.getfloat('maximum_deviation', above=0.)
        # ensure compatibility with itself
        self.sensor = self
        # get empty list for sensors, could be any sensor class or a heater
        self.sensors = []
        # get combination method to handle the different sensor values with
        algos = {'min': min, 'max': max, 'mean': mean}
        self.apply_mode = config.getchoice('combination_method', algos)
        # set default values
        self.last_temp = self.min_temp = self.max_temp = 0.0
        # add object
        self.printer.add_object("temperature_combined " + self.name, self)
        # time-controlled sensor update
        self.temperature_update_timer = self.reactor.register_timer(
                self._temperature_update_event)
        self.printer.register_event_handler('klippy:connect',
                                            self._handle_connect)
        self.printer.register_event_handler('klippy:ready',
                                            self._handle_ready)

    def _handle_connect(self):
        for sensor_name in self.sensor_names:
            sensor = self.printer.lookup_object(sensor_name)
            # check if sensor has get_status function and
            # get_status has a 'temperature' value
            if not hasattr(sensor, 'get_status'):
                raise self.printer.config_error(
                        "'%s' does not have a status."
                        % (sensor_name,))
            status = sensor.get_status(self.reactor.monotonic())
            if 'temperature' not in status:
                raise self.printer.config_error(
                        "'%s' does not report a temperature."
                        % (sensor_name,))
            # Handle temperature monitors
            if status["temperature"] is None:
                raise self.printer.config_error(
                        "Temperature monitor '%s' is not supported"
                        % (sensor_name,))

            self.sensors.append(sensor)

    def _handle_ready(self):
        # Start temperature update timer
        # There is a race condition with sensors where they can be not ready,
        # and return 0 or None - initialize a little bit later.
        self.reactor.update_timer(self.temperature_update_timer,
                                  self.reactor.monotonic() + 1.)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, temperature_callback):
        self.temperature_callback = temperature_callback

    def get_report_time_delta(self):
        return REPORT_TIME

    def update_temp(self, eventtime):
        values = []
        for sensor in self.sensors:
            sensor_status = sensor.get_status(eventtime)
            sensor_temperature = sensor_status['temperature']
            values.append(sensor_temperature)

        # check if values are out of max_deviation range
        if (max(values) - min(values)) > self.max_deviation:
            self.printer.invoke_shutdown(
                "COMBINED SENSOR maximum deviation exceeded limit of %0.1f, "
                "max sensor value %0.1f, min sensor value %0.1f."
                % (self.max_deviation, max(values), min(values),))

        temp = self.apply_mode(values)
        if temp:
            self.last_temp = temp

    def get_temp(self, eventtime):
        return self.last_temp, 0.

    def get_status(self, eventtime):
        return {'temperature': round(self.last_temp, 2),
                }

    def _temperature_update_event(self, eventtime):
        # update sensor value
        self.update_temp(eventtime)

        # check min / max temp values
        if self.last_temp < self.min_temp:
            self.printer.invoke_shutdown(
                "COMBINED SENSOR temperature %0.1f "
                "below minimum temperature of %0.1f."
                % (self.last_temp, self.min_temp,))
        if self.last_temp > self.max_temp:
            self.printer.invoke_shutdown(
                "COMBINED SENSOR temperature %0.1f "
                "above maximum temperature of %0.1f."
                % (self.last_temp, self.max_temp,))

        # this is copied from temperature_host to enable time triggered updates
        # get mcu and measured / current(?) time
        mcu = self.printer.lookup_object('mcu')
        measured_time = self.reactor.monotonic()
        # convert to print time?! for the callback???
        self.temperature_callback(mcu.estimated_print_time(measured_time),
                                  self.last_temp)
        # set next update time
        return measured_time + REPORT_TIME


def mean(values):
    if not values:
        return
    return sum(values) / len(values)


def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("temperature_combined", PrinterSensorCombined)
