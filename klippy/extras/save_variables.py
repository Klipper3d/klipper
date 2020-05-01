# Get and Save arbitrary variables in the config file so that values can be kept across restarts. 
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import math, logging

class SaveVariables:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'SAVE_VARIABLE', self.cmd_SAVE_VARIABLE,
            desc=self.cmd_SAVE_VARIABLE_help)
    cmd_SAVE_VARIABLE_help = "Save arbitrary variables in the config file"
    def cmd_SAVE_VARIABLE(self, params):
        variable_name = self.gcode.get_str('VARIABLE', params)
        variable_value = self.gcode.get_str('VALUE', params)
        self.gcode.respond_info(
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the new variable")
        # Store results for SAVE_CONFIG
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.name, variable_name, variable_value)

def load_config(config):
    return SaveVariables(config)
