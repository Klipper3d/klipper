# Support for load cells connected to the host
#
# Copyright (C) 2026  Markus Reiter <me@reitermark.us>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

HOST_REPORT_TIME = 1.0

class LoadCell_HOST:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]
        self.path = config.get("sensor_path")
        self.min_count = config.getint("min_count")
        self.max_count = config.getint("max_count")

        self.client_callbacks = []

        self.printer.add_object("load_cell_host " + self.name, self)
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        self.sample_timer = self.reactor.register_timer(
            self._sample_load_cell)
        aio = self.printer.load_object(config, 'aio_executor')
        self.executor = aio.allocate_executor("load_cell_host")

        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def add_client(self, callback):
        self.client_callbacks.append(callback)

    def get_mcu(self):
        return self.printer.lookup_object('mcu')

    def get_range(self):
        return self.min_count, self.max_count

    def get_samples_per_second(self):
        return 1.0 / HOST_REPORT_TIME

    def _sample_load_cell(self, eventtime):
        def _get_sample():
          with open(self.path, "r") as f:
            return f.read()

        measured_time = self.reactor.monotonic()

        try:
            raw_value = self.executor.submit(_get_sample)
            force_g = float(raw_value)
        except Exception:
            logging.exception("load_cell_host: Error reading data")
            return measured_time + HOST_REPORT_TIME

        ptime = self.get_mcu().estimated_print_time(measured_time)
        sample = {
            "data": [(ptime, force_g)],
            "errors": 0,
            "overflows": 0,
        }

        for client_callback in self.client_callbacks:
            client_callback(sample)

        return measured_time + HOST_REPORT_TIME

    def get_status(self, eventtime):
        return {
            'errors': 0,
            'overflows': 0,
            'sample_rate': self.get_samples_per_second(),
        }

LOAD_CELL_HOST_SENSOR_TYPE = { "load_cell_host": LoadCell_HOST }
