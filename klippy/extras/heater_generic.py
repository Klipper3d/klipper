# Support for a generic heater
#
# Copyright (C) 2019  John Jardine <john@gprime.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class PrinterHeaterGeneric:
    def __init__(self, config):
        self.printer = config.get_printer()
        gcode_id = config.get("gcode_id")
        self.heater = self.printer.lookup_object('heater').setup_heater(config, gcode_id)

def load_config_prefix(config):
    return PrinterHeaterGeneric(config)
