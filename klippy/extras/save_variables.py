# Save arbitrary variables so that values can be kept across restarts.
#
# Copyright (C) 2020 Dushyant Ahuja <dusht.ahuja@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import sys,os,logging, ConfigParser

class SaveVariables:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.name = config.get_name()
        self.cfilename = config.get('filename')
        self.filename = os.path.expanduser(self.cfilename)
        self.allVariables = {}
        self.variablefile = ConfigParser.ConfigParser()
        if os.path.isfile(self.filename):
            self.loadVariables()
        logging.info("save_variable state: %s", self.allVariables)
        self.gcode.register_command(
            'SAVE_VARIABLE', self.cmd_SAVE_VARIABLE,
            desc=self.cmd_SAVE_VARIABLE_help)
    cmd_SAVE_VARIABLE_help = "Save arbitrary variables to disk"
    def loadVariables(self):
        self.allVariables = {}
        try:
            #self.variablefile = ConfigParser.ConfigParser()
            self.variablefile.read(self.filename)
            self.allVariables = dict(self.variablefile.items('Variables'))
        except:
            msg = "\nUnable to parse existing variable file"
            logging.exception(msg)
            raise self.gcode.error(msg)
    def cmd_SAVE_VARIABLE(self, params):
        self.variable_name = self.gcode.get_str('VARIABLE', params)
        self.variable_value = self.gcode.get_str('VALUE', params)
        if os.path.isfile(self.filename):
            self.loadVariables()
        try:
            if not self.variablefile.has_section('Variables'):
                self.variablefile.add_section('Variables')
            self.variablefile.set('Variables',self.variable_name,
                              self.variable_value)
            self.variablefile.write(open(self.filename, "w+"))
        except:
            msg = "\nUnable to save variable"
            logging.exception(msg)
            raise self.gcode.error(msg)
        self.gcode.respond_info("Variable Saved")
        self.loadVariables()
        logging.info(str(self.allVariables))
    def get_status(self, eventtime):
        return self.allVariables

def load_config(config):
    return SaveVariables(config)
