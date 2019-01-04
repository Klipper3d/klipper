# Support for multiple filament run out sensors
#
# Inspired by pull request #545 from Miguel Moitinho <miguel@moitinho.net>
#
# Copyright (C) 2019  Paulo Drugos <paulodrugos@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class FilamentRunoutSensor:
    def __init__(self, config):
        self.printer = config.get_printer()
        buttons = self.printer.try_load_module(config, "buttons")
        pins = config.get('pins').split(',')
        buttons.register_buttons(pins, self.runout_callback)

    def runout_callback(self, eventtime, state):
        if state:
            return

        toolhead = self.printer.lookup_object('toolhead')
        gcode = self.printer.lookup_object('gcode')
        
        if toolhead.get_status(eventtime)['status'] == "Printing":
            gcode.respond_info("Filament run out sensor triggered! Pausing print job.")
            gcode.respond_info("action:pause")

def load_config(config):
    return FilamentRunoutSensor(config)