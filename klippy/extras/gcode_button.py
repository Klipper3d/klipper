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
        gcode = self.printer.lookup_object('gcode')
        self.press_gcode = config.get('press_gcode', '')
        self.release_gcode = config.get('release_gcode', '')
        gcode.register_mux_command("QUERY_BUTTON", "BUTTON", self.name,
                                        self.cmd_QUERY_BUTTON,
                                        desc=self.cmd_QUERY_BUTTON_help)

    cmd_QUERY_BUTTON_help = "Report on the state of a button"

    def cmd_QUERY_BUTTON(self, params):
        gcode = self.printer.lookup_object('gcode')
        response = self.name+": "
        if self.last_state:
            response += "PRESSED"
        else:
            response += "RELEASED"
        gcode.respond(response)

    def button_callback(self, eventtime, state):
        toolhead = self.printer.lookup_object('toolhead')
        gcode = self.printer.lookup_object('gcode')
        self.last_state = state
        if state and bool(self.press_gcode):
           gcode.run_script_from_command(self.press_gcode)
        if (not state) and bool(self.release_gcode):
            gcode.run_script_from_command(self.release_gcode)

def load_config_prefix(config):
    return GCodeButton(config)
