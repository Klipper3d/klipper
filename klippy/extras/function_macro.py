# Add ability to define custom macros mapped to functions
#
# Copyright (C) 2021  Stefan Verse <verse@amotronics.de>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import traceback, logging, ast, copy
import jinja2

# Wrapper around a Jinja2 template
class TemplateWrapper:
    def __init__(self, printer, env, name, script):
        self.printer = printer
        self.name = name
#        function_macro = self.printer.lookup_object('function_macro')
        try:
            self.template = env.from_string(script)
        except Exception as e:
            msg = "Error loading template '%s': %s" % (
                 name, traceback.format_exception_only(type(e), e)[-1])
            logging.exception(msg)
            raise printer.config_error(msg)

# Main function macro template tracking
class PrinterFunctionMacro:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.env = jinja2.Environment('{%', '%}', '{', '}')
        self.scripts = []
        self.names = []
    def load_template(self, config, option, default=None, function_name=None):
        name = "%s:%s" % (config.get_name(), option)
        if default is None:
            script = config.get(option)
        else:
            script = config.get(option, default)
        self.scripts.append(script)
        self.names.append(function_name)
        return TemplateWrapper(self.printer, self.env, name, script)
    def run_macro_from_name(self, function_name):
        try:
            index=self.names.index(function_name.upper())
            self.gcode.run_script_from_command(self.scripts[index].strip())
        except:
            return

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
        self.template = function_macro.load_template(config, 'macro', function_name=name.upper())

def load_config_prefix(config):
    return FUNCTION_MACRO(config)
