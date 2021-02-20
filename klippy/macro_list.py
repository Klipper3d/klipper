# Add ability to define custom macros mapped to functions
#
# Copyright (C) 2021  Stefan Verse <verse@amotronics.de>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class MACRO_LIST:
    def __init__(self, printer):
        self.printer = printer
        self.gcode = self.printer.lookup_object('gcode')
        self.scripts = []
        self.names = []
        self.names_possible = []
        self.gcode.register_command("FUNCTION_MACRO_DEFINED", 
             self.cmd_FUNCTION_MACRO_DEFINED,
             desc=self.cmd_FUNCTION_MACRO_DEFINED_help)
        self.gcode.register_command("FUNCTION_MACRO_AVAILABLE", 
             self.cmd_FUNCTION_MACRO_AVAILABLE,
             desc=self.cmd_FUNCTION_MACRO_AVAILABLE_help)
    def register_function(self, function_name):
        self.names_possible.append(function_name.upper())
	return function_name.upper()
    def add_script(self, script, function_name):
        self.scripts.append(script)
        self.names.append(function_name.upper())
    def get_index_from_name(self, function_name):
        try:
            return self.names.index(function_name.upper())
        except:
            return None
    def run_macro_from_index(self, index):
        if index != None:
            try:
                self.gcode.run_script_from_command(self.scripts[index].strip())
            except:
                return
    def run_macro_from_name(self, function_name):
        index = self.get_index_from_name(function_name)
        self.run_macro_from_index(index)
    cmd_FUNCTION_MACRO_DEFINED_help = "Show list of registered function_macro names"
    def cmd_FUNCTION_MACRO_DEFINED(self, gcmd):
        for name in self.names:
#            self.gcode.respond_info("function_macro '%s'" % (name))
            self.gcode.respond_info(name)
    cmd_FUNCTION_MACRO_AVAILABLE_help = "Show list of available function_macro names"
    def cmd_FUNCTION_MACRO_AVAILABLE(self, gcmd):
        for name in self.names_possible :
#            self.gcode.respond_info("function_macro '%s'" % (name))
            self.gcode.respond_info(name)

def add_early_printer_objects(printer):
    printer.add_object('macro_list',MACRO_LIST(printer))
