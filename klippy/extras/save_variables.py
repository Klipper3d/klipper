# Get and Save arbitrary variables in the config file so that values can be kept across restarts. 
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import sys,os,logging, ConfigParser

error = ConfigParser.Error

class LoadSaveVariables:
    error = ConfigParser.Error
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.name = config.get_name()
        self.cfilename = config.get('filename')
        self.filename = os.path.join(os.path.expanduser("~"), self.cfilename)
        self.gcode.register_command(
            'SAVE_VARIABLE', self.cmd_SAVE_VARIABLE,
            desc=self.cmd_SAVE_VARIABLE_help)
        self.gcode.register_command(
            'LOAD_VARIABLE', self.cmd_LOAD_VARIABLE,
            desc=self.cmd_LOAD_VARIABLE_help)
    cmd_LOAD_VARIABLE_help = "Load arbitrary variables from disk"
    cmd_SAVE_VARIABLE_help = "Save arbitrary variables to disk"
    def cmd_LOAD_VARIABLE(self, params):
        try:
            self.variablefile = ConfigParser.ConfigParser()
            self.variablefile.read(self.filename)
        except error as e:
            msg = e.message + "\nUnable to parse existing variable file"
            logging.exception(msg)
            raise self.gcode.error(msg)
        self.variable_name = self.gcode.get_str('VARIABLE', params)
        try:
            self.variable_value = self.variablefile.get('Variables',self.variable_name)
        except error as e:
            msg = e.message + "\nUnable to parse variables"
            logging.exception(msg)
            raise self.gcode.error(msg)
        #self.gcode.respond_info(self.filename)
        self.gcode.respond_info(self.variable_name)
        self.gcode.respond_info(self.variable_value)

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
            self.variablefile.set('Variables',self.variable_name,self.variable_value)
            self.variablefile.write(open(self.filename, "w"))
        except error as e:
            msg = e.message + "\nUnable to save variable"
            logging.exception(msg)
            raise self.gcode.error(msg)
        self.gcode.respond_info("Variable Saved")

def load_config(config):
    return LoadSaveVariables(config)
