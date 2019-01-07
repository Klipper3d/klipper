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
        self.name = config.get_name().split(' ')[-1]
        self.pin = config.get('pin')
        self.last_state = 0
        buttons = self.printer.try_load_module(config, "buttons")
        buttons.register_buttons([self.pin], self.runout_callback)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("QUERY_FILAMENT_SENSOR", "SENSOR", self.name,
                                        self.cmd_QUERY_FILAMENT_SENSOR,
                                        desc=self.cmd_QUERY_FILAMENT_SENSOR_help)
    cmd_QUERY_FILAMENT_SENSOR_help = "Report on the status of filament runout sensor"
    def cmd_QUERY_FILAMENT_SENSOR(self, params):
        gcode = self.printer.lookup_object('gcode')
        response = self.name+": "
        if self.last_state:
            response += "Filament present"
        else:
            response += "Filament NOT present"
        gcode.respond(response)
    def runout_callback(self,eventtime, state):
        toolhead = self.printer.lookup_object('toolhead')
        gcode = self.printer.lookup_object('gcode')
        self.last_state = state
        if state:
            return
        if toolhead.get_status(eventtime)['status'] == "Printing":
            gcode.respond_info("Filament runout sensor triggered! Pausing print job.")
            gcode.respond_info("action:pause")
def load_config_prefix(config):
    return FilamentRunoutSensor(config)
