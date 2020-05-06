# Get and Save arbitrary variables in the config file so that values can be kept across
# restarts.
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import sys,os,logging, ConfigParser

error = ConfigParser.Error

class SaveVariables:
    error = ConfigParser.Error
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.name = config.get_name()
        self.cfilename = config.get('filename')
        self.filename = os.path.join(os.path.expanduser("~"), self.cfilename)
        self.allVariables = {}
        self.loadVariables()
        logging.info(str(self.allVariables))
        self.gcode.register_command(
            'SAVE_VARIABLE', self.cmd_SAVE_VARIABLE,
            desc=self.cmd_SAVE_VARIABLE_help)
    cmd_SAVE_VARIABLE_help = "Save arbitrary variables to disk"
    def loadVariables(self):
        self.allVariables = {}
        try:
            self.variablefile = ConfigParser.ConfigParser()
            self.variablefile.read(self.filename)
            self.allVariables = dict(self.variablefile.items('Variables'))
        except error as e:
            msg = e.message + "\nUnable to parse existing variable file"
            logging.exception(msg)
            raise self.gcode.error(msg)
    def cmd_SAVE_VARIABLE(self, params):
        self.variable_name = self.gcode.get_str('VARIABLE', params)
        self.variable_value = self.gcode.get_str('VALUE', params)
        try:
            self.variablefile = ConfigParser.ConfigParser()
            self.variablefile.read(self.filename)
        except error as e:
            msg = "Unable to parse existing variable file"
            logging.exception(msg)
            raise self.gcode.error(msg)
        try:
            self.variablefile.set('Variables',self.variable_name,
                self.variable_value)
            self.variablefile.write(open(self.filename, "w"))
        except error as e:
            msg = e.message + "\nUnable to save variable"
            logging.exception(msg)
            raise self.gcode.error(msg)
        self.gcode.respond_info("Variable Saved")
        self.loadVariables()
        logging.info(str(self.allVariables))
    def get_status(self, eventtime):
        #return {'temp1':self.filename}
        #logging.info("allVariables='%s'", repr(self.allVariables))
        return self.allVariables

def load_config(config):
    return SaveVariables(config)
