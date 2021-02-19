# Add ability to define custom macros mapped to functions
#
# Copyright (C) 2021  Stefan Verse <verse@amotronics.de>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class PrinterFunctionMacro:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.macro = self.printer.load_object(config, 'macro_list')
    def load_config(self, config, option, default=None, function_name=None):
        if default is None:
            script = config.get(option)
        else:
            script = config.get(option, default)
        self.macro.add_script(script, function_name)

def load_config(config):
    return PrinterFunctionMacro(config)

######################################################################
# FUNCTION_MACRO printer object
######################################################################

class FUNCTION_MACRO:
    def __init__(self, config):
        name = config.get_name().split()[1]
        self.alias = name.upper()
        self.printer = printer = config.get_printer()
        function_macro = printer.load_object(config, 'function_macro')
        function_macro.load_config(config, 'macro', function_name=name.upper())

def load_config_prefix(config):
    return FUNCTION_MACRO(config)

