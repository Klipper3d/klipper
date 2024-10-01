# Support fans that are controlled by gcode
#
# Copyright (C) 2016-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import fan, output_pin

class PrinterFanGeneric:
    cmd_SET_FAN_SPEED_help = "Sets the speed of a fan"
    def __init__(self, config):
        self.printer = config.get_printer()
        self.fan = fan.Fan(config, default_shutdown_speed=0.)
        self.fan_name = config.get_name().split()[-1]

        # Template handling
        self.template_eval = output_pin.lookup_template_eval(config)

        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("SET_FAN_SPEED", "FAN",
                                   self.fan_name,
                                   self.cmd_SET_FAN_SPEED,
                                   desc=self.cmd_SET_FAN_SPEED_help)

    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)
    def _template_update(self, text):
        try:
            value = float(text)
        except ValueError as e:
            logging.exception("fan_generic template render error")
        self.fan.set_speed(value)
    def cmd_SET_FAN_SPEED(self, gcmd):
        speed = gcmd.get_float('SPEED', None, 0.)
        template = gcmd.get('TEMPLATE', None)
        if (speed is None) == (template is None):
            raise gcmd.error("SET_FAN_SPEED must specify SPEED or TEMPLATE")
        # Check for template setting
        if template is not None:
            self.template_eval.set_template(gcmd, self._template_update)
            return
        self.fan.set_speed_from_command(speed)

def load_config_prefix(config):
    return PrinterFanGeneric(config)
