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

def add_early_printer_objects(printer):
    printer.add_object('macro_list',MACRO_LIST(printer))
