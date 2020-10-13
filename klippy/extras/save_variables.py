# Save arbitrary variables so that values can be kept across restarts.
#
# Copyright (C) 2020 Dushyant Ahuja <dusht.ahuja@gmail.com>
# Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import ast,os,logging,ConfigParser

class SaveVariables:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.filename = os.path.expanduser(config.get('filename'))
        self.gcode = self.printer.lookup_object('gcode')
        self.allVariables = {}
        self.variablefile = ConfigParser.ConfigParser()
        if os.path.isfile(self.filename):
            self.loadVariables()
        else:
            f = open(self.filename, "w+")
            self.variablefile.add_section('Variables')
            self.variablefile.write(f)
            f.close()
            logging.info("Variable File Initialised")
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command(
            'SAVE_VARIABLE', self.cmd_SAVE_VARIABLE,
            desc=self.cmd_SAVE_VARIABLE_help)
    cmd_SAVE_VARIABLE_help = "Save arbitrary variables to disk"
    def loadVariables(self):
        allvars = {}
        try:
            self.variablefile.read(self.filename)
            for name, val in self.variablefile.items('Variables'):
                allvars[name] = ast.literal_eval(val)
        except:
            msg = "\nUnable to parse existing variable file"
            logging.exception(msg)
            raise self.printer.command_error(msg)
        self.allVariables = allvars
    def cmd_SAVE_VARIABLE(self, gcmd):
        variable_name = gcmd.get('VARIABLE')
        variable_value = gcmd.get('VALUE')
        try:
            if os.path.isfile(self.filename):
                self.loadVariables()
        except self.printer.command_error, e:
            raise gcmd.error(str(e))
        try:
            if not self.variablefile.has_section('Variables'):
                self.variablefile.add_section('Variables')
            self.variablefile.set('Variables',variable_name,
                              variable_value)
            f = open(self.filename, "w+")
            self.variablefile.write(f)
            f.close()
        except:
            msg = "\nUnable to save variable"
            logging.exception(msg)
            raise gcmd.error(msg)
        self.gcode.respond_info("Variable Saved")
        try:
            if os.path.isfile(self.filename):
                self.loadVariables()
        except self.printer.command_error, e:
            raise gcmd.error(str(e))
        logging.info("Variables saved using save_variable: %s",
            self.allVariables)
    def get_status(self, eventtime):
        return {'variables': dict(self.allVariables)}

def load_config(config):
    return SaveVariables(config)
