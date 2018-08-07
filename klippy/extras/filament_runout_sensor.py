# Support for common SPI based thermocouple and RTD temperature sensors
#
# Copyright (C) 2018  Miguel Moitinho <miguel@moitinho.net>
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class FilamentRunoutSensor:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')

        buttons = self.printer.try_load_module(config, "buttons")

        buttons.register_button_push(config.get('pin'), self.runnout_callback)

    def runnout_callback(self, eventtime):
        self.gcode.respond_info("Filament Sensor Triggered")


def load_config_prefix(config):
    return FilamentRunoutSensor(config)
