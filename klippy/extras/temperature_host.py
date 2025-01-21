# Support for Raspberry Pi temperature sensor
#
# Copyright (C) 2020  Al Crate <al3ph@users.noreply.github.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

HOST_REPORT_TIME = 1.0
RPI_PROC_TEMP_FILE = "/sys/class/thermal/thermal_zone0/temp"

class Temperature_HOST:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]
        self.path = config.get("sensor_path", RPI_PROC_TEMP_FILE)

        self.temp = self.min_temp = self.max_temp = 0.0

        self.printer.add_object("temperature_host " + self.name, self)
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        self.sample_timer = self.reactor.register_timer(
            self._sample_pi_temperature)
        try:
            self.file_handle = open(self.path, "r")
        except:
            raise config.error("Unable to open temperature file '%s'"
                               % (self.path,))

        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return HOST_REPORT_TIME

    def _sample_pi_temperature(self, eventtime):
        try:
            self.file_handle.seek(0)
            self.temp = float(self.file_handle.read())/1000.0
        except Exception:
            logging.exception("temperature_host: Error reading data")
            self.temp = 0.0
            return self.reactor.NEVER

        if self.temp < self.min_temp:
            self.printer.invoke_shutdown(
                "HOST temperature %0.1f below minimum temperature of %0.1f."
                % (self.temp, self.min_temp,))
        if self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "HOST temperature %0.1f above maximum temperature of %0.1f."
                % (self.temp, self.max_temp,))

        mcu = self.printer.lookup_object('mcu')
        measured_time = self.reactor.monotonic()
        self._callback(mcu.estimated_print_time(measured_time), self.temp)
        return measured_time + HOST_REPORT_TIME

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
        }


def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("temperature_host", Temperature_HOST)
