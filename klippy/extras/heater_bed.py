# Support for a heated bed
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterHeaterBed:
    def __init__(self, config):
        self.printer = config.get_printer()
        pheaters = self.printer.try_load_module(config, 'heaters')
        self.heater = pheaters.setup_heater(config, 'B')
        self.get_status = self.heater.get_status
        self.stats = self.heater.stats
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("M140", self.cmd_M140)
        gcode.register_command("M190", self.cmd_M190)
    def cmd_M140(self, params, wait=False):
        # Set Bed Temperature
        gcode = self.printer.lookup_object('gcode')
        temp = gcode.get_float('S', params, 0.)
        self.heater.set_temp(temp)
        if wait and temp:
            pheaters = self.printer.lookup_object('heaters')
            pheaters.wait_for_temperature(self.heater)
    def cmd_M190(self, params):
        # Set Bed Temperature and Wait
        self.cmd_M140(params, wait=True)

def load_config(config):
    return PrinterHeaterBed(config)
