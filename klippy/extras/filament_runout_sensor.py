# Support for multiple filament run out sensors
#
# Copyright (C) 2018  Miguel Moitinho <miguel@moitinho.net>
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class FilamentRunoutSensor:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')

        self.name = config.get_name().split(' ')[-1]

        buttons = self.printer.try_load_module(config, "buttons")

        buttons.register_buttons([config.get('pin')], self.runout_callback)

    def runout_callback(self, eventtime, state):
        self.gcode.respond_info("Sensor {name} state {state}".format(
            name=self.name,
            state=state
        ))


def load_config_prefix(config):
    return FilamentRunoutSensor(config)
