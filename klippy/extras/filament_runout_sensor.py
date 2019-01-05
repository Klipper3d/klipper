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
        FilamentRunoutSensor.last_states = {}
        self.pins = config.get('pins').split(',')
        for pin in self.pins:
            FilamentRunoutSensor.last_states[pin] = 0
            f = self.create_callback(pin)
            buttons.register_buttons([pin], f)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("QUERY_FILAMENT_SENSOR", self.cmd_QUERY_FILAMENT_SENSOR,
                               desc=self.cmd_QUERY_FILAMENT_SENSOR_help)
    cmd_QUERY_FILAMENT_SENSOR_help = "Report on the status of each filament runout sensor"
    def cmd_QUERY_FILAMENT_SENSOR(self, params):
        gcode = self.printer.lookup_object('gcode')
        try:
            s=0
            for pin in FilamentRunoutSensor.last_states:
                response = "Sensor "+str(s)+": "
                if FilamentRunoutSensor.last_states[pin]:
                    response += "Filament present"
                else:
                    response += "Filament NOT present"
                gcode.respond(response)
                s+=1
        except Exception as e:
            gcode.respond(str(e))
    def create_callback(self,pin):
        def _f(eventtime, state):
            toolhead = self.printer.lookup_object('toolhead')
            gcode = self.printer.lookup_object('gcode')
            FilamentRunoutSensor.last_states[pin] = state
            if state:
                return
            if toolhead.get_status(eventtime)['status'] == "Printing":
                gcode.respond_info("Filament runout sensor triggered! Pausing print job.")
                gcode.respond_info("action:pause")
        return _f
def load_config(config):
    return FilamentRunoutSensor(config)
