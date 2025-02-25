# Klipper Heater Predictional Control
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
from .gcode_macro import TemplateWrapper, PrinterGCodeMacro

class HeaterPredictControl:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.name = config.get_name().split()[-1]
        script = config.get("func")
        self.env = PrinterGCodeMacro(config).env
        self.env.globals.update({
            'log': self._log,
            'set_var': self._set_var,
            'get_var': self._get_var,
        })
        self.variables = {}
        self.load_variables()
        self.template = TemplateWrapper(self.printer, self.env,
                                        config.get_name(), script)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def handle_connect(self):
        pheaters = self.printer.load_object(self.config, 'heaters')
        heater = pheaters.heaters.get(self.name)
        if heater is None:
            self.config.error("Heater %s is not registered" % (self.name))
        def callback():
            return float(self.template.render())
        heater.set_pc_callback(callback)

    def load_variables(self):
        import ast
        import json
        prefix = 'variable_'
        for option in self.config.get_prefix_options(prefix):
            try:
                literal = ast.literal_eval(self.config.get(option))
                json.dumps(literal, separators=(',', ':'))
                self.variables[option[len(prefix):]] = literal
            except (SyntaxError, TypeError, ValueError) as e:
                raise self.config.error(
                    "Option '%s' in section '%s' is not a valid literal: %s" % (
                        option, self.config.get_name(), e))

    def _log(self, output):
        logging.info(output)
        # Make it callable in a block section
        return ""

    def _get_var(self, name):
        v = self.variables.get(name)
        if v is None:
            self.printer.invoke_shutdown(
                "variable_%s is not defined" % (name)
            )
        return v

    def _set_var(self, name, value):
        v = self._get_var(name)
        self.variables[name] = value
        return v

def load_config_prefix(config):
    return HeaterPredictControl(config)
