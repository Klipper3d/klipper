# Klipper Heater Predictional Control
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class HeaterPredictControl:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.name = config.get_name().split()[-1]
        gcode_macro = self.printer.load_object(self.config, 'gcode_macro')
        self.template = gcode_macro.load_template(self.config, 'func')
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

def load_config_prefix(config):
    return HeaterPredictControl(config)
