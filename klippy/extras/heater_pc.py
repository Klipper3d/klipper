# Klipper Heater Predictional Control
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from .gcode_macro import PrinterGCodeMacro
from .display import display

class HeaterPredictControl:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        template_name = config.get("macro_template")
        templates = display.lookup_display_templates(config)
        display_templates = templates.get_display_templates()
        self.create_context = PrinterGCodeMacro(config).create_template_context
        self.template = display_templates.get(template_name)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def handle_connect(self):
        pheaters = self.printer.load_object(self.config, 'heaters')
        heater_name = self.config.get_name().split()[-1]
        heater = pheaters.heaters.get(heater_name)
        if heater is None:
            self.config.error("Heater %s is not registered" % (heater_name))
        def callback():
            context = self.create_context()
            res = self.template.render(context)
            return float(res)
        heater.set_pc_callback(callback)

def load_config_prefix(config):
    return HeaterPredictControl(config)
