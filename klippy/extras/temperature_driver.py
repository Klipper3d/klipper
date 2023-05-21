# Support for micro-controller chip based temperature sensors
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
DRIVER_REPORT_TIME = 1.0

class PrinterTemperatureDriver:
    def __init__(self, config):
        self.printer = config.get_printer()

        driver_name = config.get('sensor_driver')
        self.driver = self.printer.lookup_object(driver_name)

        self.temp = self.min_temp = self.max_temp = 0.0

        self.reactor = self.printer.get_reactor()
        self.sample_timer = self.reactor.register_timer(
            self._sample_driver_temperature)

        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_callback(self, temperature_callback):
        self.temperature_callback = temperature_callback

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def get_report_time_delta(self):
        return DRIVER_REPORT_TIME

    def _sample_driver_temperature(self, eventtime):
        self.temp = self.driver.get_temperature()

        if self.temp is not None:
            if self.temp < self.min_temp or self.temp > self.max_temp:
                self.printer.invoke_shutdown(
                    "DRIVER temperature %0.1f outside range of %0.1f:%.01f"
                    % (self.temp, self.min_temp, self.max_temp))

        measured_time = self.reactor.monotonic()

        if self.temp is not None:
            mcu = self.driver.get_mcu()
            self.temperature_callback(
                mcu.estimated_print_time(measured_time),
                self.temp)

        return measured_time + DRIVER_REPORT_TIME

def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("temperature_driver", PrinterTemperatureDriver)
