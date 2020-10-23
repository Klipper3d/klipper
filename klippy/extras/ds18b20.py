# Support for 1-wire based temperature sensors
#
# Copyright (C) 2020 Alan Lord <alanslists@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

REPORT_FREQ = 5
W1_PATH = '/sys/bus/w1/devices/'
W1_SLAVE = '/w1_slave'

class DS18B20:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.w1_sensor_fd = W1_PATH + config.get("serial_no") + W1_SLAVE
        self.min_temp = config.getfloat('min_temp')
        self.max_temp = config.getfloat('max_temp')
        self.setup_minmax(self.min_temp, self.max_temp)
        self.reactor = self.printer.get_reactor()
        self.sample_timer = self.reactor.register_timer(self._sample_ds18b20)
        self.printer.add_object("ds18b20 " + self.name, self)
        self.printer.register_event_handler("klippy:ready", self.handle_ready)

    def handle_ready(self):
        self._init_ds18b20()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def get_report_time_delta(self):
        return REPORT_FREQ

    def setup_minmax(self, min_temp, max_temp):
        pass

    def setup_callback(self, cb):
        self._callback = cb

    def _init_ds18b20(self):
        try:
            self.fd = open(self.w1_sensor_fd, 'r')
        except IOError:
            # Not really following the exception handling.
            # from the docs seems like this should be handled in 
            # load_config()???
            raise self.printer.config_error(
                self.w1_sensor_fd + ' does not exist')

    def _sample_ds18b20(self, eventtime):
        self.reactor.pause(
            self.reactor.monotonic() + REPORT_FREQ)
        self.fd.seek(0)
        self.lines = self.fd.readlines()

        temp_output = self.lines[1].find('t=')
        if temp_output != -1:
            temp_string = self.lines[1].strip()[temp_output+2:]
            temp_c = float(temp_string) / 1000.0
            self.temp = temp_c
            logging.info(self.temp)
            self._callback(eventtime, self.temp)

    def get_status(self, eventtime):
        return {
            'temperature': self.temp,
        }

def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("DS18B20", DS18B20)
