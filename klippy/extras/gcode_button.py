# Support for multiple filament run out sensors
#
# Inspired by pull request #545 from Miguel Moitinho <miguel@moitinho.net>
# Inspired by pull request #1098 from Paulo Drugos <paulodrugos@gmail.com>
#
# Copyright (C) 2019 Alec Plumb <alec@etherwalker.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.


class GCodeButton:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split(' ')[-1]
        self.pin = config.get('pin')
        self.last_state = 0
        buttons = self.printer.try_load_module(config, "buttons")
        buttons.register_buttons([self.pin], self.button_callback)
        gcode_macro = self.printer.try_load_module(config, 'gcode_macro')
        self.press_template = gcode_macro.load_template(config, 'press_gcode')
        self.release_template = gcode_macro.load_template(config, 'release_gcode')
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("QUERY_BUTTON", "BUTTON", self.name,
                                        self.cmd_QUERY_BUTTON,
                                        desc=self.cmd_QUERY_BUTTON_help)

    cmd_QUERY_BUTTON_help = "Report on the state of a button"

    def cmd_QUERY_BUTTON(self, params):
        gcode = self.printer.lookup_object('gcode')
        gcode.respond(self.name + ": " + self.get_status()['state'])

    def button_callback(self, eventtime, state):
        self.last_state = state
        if state and bool(self.press_template):
           self.press_template.run_gcode_from_command()
        if (not state) and bool(self.release_template):
           self.release_template.run_gcode_from_command()

    def get_status(self, eventtime=None):
        if self.last_state:
            return {'state': "PRESSED"}
        return {'state': "RELEASED"}

def load_config_prefix(config):
    return GCodeButton(config)
