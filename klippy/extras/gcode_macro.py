# Add ability to define custom g-code macros
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import traceback, logging, ast
import jinja2


######################################################################
# Template handling
######################################################################

# Wrapper for access to printer object get_status() methods
class GetStatusWrapper:
    def __init__(self, printer, eventtime=None):
        self.printer = printer
        self.eventtime = eventtime
        self.cache = {}
    def __getitem__(self, val):
        sval = str(val).strip()
        if sval in self.cache:
            return self.cache[sval]
        po = self.printer.lookup_object(sval, None)
        if po is None or not hasattr(po, 'get_status'):
            raise KeyError(val)
        if self.eventtime is None:
            self.eventtime = self.printer.get_reactor().monotonic()
        self.cache[sval] = res = dict(po.get_status(self.eventtime))
        return res
    def __contains__(self, val):
        try:
            self.__getitem__(val)
        except KeyError as e:
            return False
        return True
    def __iter__(self):
        for name, obj in self.printer.lookup_objects():
            if self.__contains__(name):
                yield name

# Wrapper around a Jinja2 template
class TemplateWrapper:
    def __init__(self, printer, env, name, script):
        self.printer = printer
        self.name = name
        self.gcode = self.printer.lookup_object('gcode')
        try:
            self.template = env.from_string(script)
        except Exception as e:
            msg = "Error loading template '%s': %s" % (
                 name, traceback.format_exception_only(type(e), e)[-1])
            logging.exception(msg)
            raise printer.config_error(msg)
    def create_status_wrapper(self, eventtime=None):
        return GetStatusWrapper(self.printer, eventtime)
    def render(self, context=None):
        if context is None:
            context = {'printer': self.create_status_wrapper()}
        try:
            return str(self.template.render(context))
        except Exception as e:
            msg = "Error evaluating '%s': %s" % (
                self.name, traceback.format_exception_only(type(e), e)[-1])
            logging.exception(msg)
            raise self.gcode.error(msg)
    def run_gcode_from_command(self, context=None):
        self.gcode.run_script_from_command(self.render(context))

# Main gcode macro template tracking
class PrinterGCodeMacro:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.env = jinja2.Environment('{%', '%}', '{', '}')
    def load_template(self, config, option, default=None):
        name = "%s:%s" % (config.get_name(), option)
        if default is None:
            script = config.get(option)
        else:
            script = config.get(option, default)
        return TemplateWrapper(self.printer, self.env, name, script)

def load_config(config):
    return PrinterGCodeMacro(config)


######################################################################
# GCode macro
######################################################################

class GCodeMacro:
    def __init__(self, config):
        name = config.get_name().split()[1]
        self.alias = name.upper()
        printer = config.get_printer()
        gcode_macro = printer.try_load_module(config, 'gcode_macro')
        self.template = gcode_macro.load_template(config, 'gcode')
        self.gcode = printer.lookup_object('gcode')
        self.gcode.register_command(self.alias, self.cmd, desc=self.cmd_desc)
        self.gcode.register_mux_command("SET_GCODE_VARIABLE", "MACRO",
                                        name, self.cmd_SET_GCODE_VARIABLE,
                                        desc=self.cmd_SET_GCODE_VARIABLE_help)
        self.in_script = False
        prefix = 'default_parameter_'
        self.kwparams = { o[len(prefix):].upper(): config.get(o)
                          for o in config.get_prefix_options(prefix) }
        self.variables = {}
        prefix = 'variable_'
        for option in config.get_prefix_options(prefix):
            try:
                self.variables[option[len(prefix):]] = ast.literal_eval(
                    config.get(option))
            except ValueError as e:
                raise config.error(
                    "Option '%s' in section '%s' is not a valid literal" % (
                        option, config.get_name()))
    def get_status(self, eventtime):
        return dict(self.variables)
    cmd_SET_GCODE_VARIABLE_help = "Set the value of a G-Code macro variable"
    def cmd_SET_GCODE_VARIABLE(self, params):
        variable = self.gcode.get_str('VARIABLE', params)
        value = self.gcode.get_str('VALUE', params)
        if variable not in self.variables:
            if variable in self.kwparams:
                self.kwparams[variable] = value
                return
            raise self.gcode.error("Unknown gcode_macro variable '%s'" % (
                variable,))
        try:
            literal = ast.literal_eval(value)
        except ValueError as e:
            raise self.gcode.error("Unable to parse '%s' as a literal" % (
                value,))
        self.variables[variable] = literal
    cmd_desc = "G-Code macro"
    def cmd(self, params):
        if self.in_script:
            raise self.gcode.error(
                "Macro %s called recursively" % (self.alias,))
        kwparams = dict(self.kwparams)
        kwparams.update(params)
        kwparams.update(self.variables)
        kwparams['printer'] = self.template.create_status_wrapper()
        kwparams['params'] = params
        self.in_script = True
        try:
            self.template.run_gcode_from_command(kwparams)
        finally:
            self.in_script = False

def load_config_prefix(config):
    return GCodeMacro(config)
