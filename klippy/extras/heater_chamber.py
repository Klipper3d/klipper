# Support for a heated chamber
#
# Copyright (C) 2024  Chad Condon
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterHeaterChamber:
    def __init__(self, config):
        self.printer = config.get_printer()
        pheaters = self.printer.load_object(config, 'heaters')
        self.heater = pheaters.setup_header(config, 'C')
        self.get_status = self.heater.get_status
        self.stats = self.heater.stats
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("M141", self.cmd_M141, desc=self.cmd_M141_help)
        gcode.register_command("M191", self.cmd_M191, desc=self.cmd_M191_help)

    cmd_M141_help = "Set chamber temperature"
    def cmd_M141(self, gcmd, wait=False):
        temp = gcmd.get_float('S', 0.)
        pheaters = self.printer.lookup_object('heaters')
        pheaters.set_temperature(self.heater, temp, wait)

    cmd_M191_help = "Wait for chamber temperature"
    def cmd_M191(self, gcmd):
        self.cmd_M141(gcmd, wait=True)

def load_object(config):
    return PrinterHeaterChamber(config)
