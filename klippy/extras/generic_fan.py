# Support for a gcode controlled fan
#
# Copyright (C) 2020  Pontus Borg <liquidpontus@yahoo.se>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import fan

class PrinterGenericFan:
    cmd_SET_FAN_SPEED_help = "Sets a generic fan speed"
    
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        
        self.fan = fan.PrinterFan(config, default_shutdown_speed=1.)
        self.mcu = self.fan.mcu_fan.get_mcu()
        self.fan_speed = config.getfloat("fan_speed", 1., minval=0., maxval=1.)
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("SET_FAN_SPEED", "FAN",
                                   self.name, self.cmd_SET_FAN_SPEED,
                                   desc=self.cmd_SET_FAN_SPEED_help)

    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)

    def set_speed(self, print_time, value):
        self.fan.set_speed(print_time, value)

    def _delayed_set_speed(self, value):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback((lambda pt:
                                              self.set_speed(pt, value)))

    def cmd_SET_FAN_SPEED(self, gcmd):
        speed = gcmd.get_float('SPEED', 0.)
        self._delayed_set_speed(speed)

def load_config_prefix(config):
    return PrinterGenericFan(config)
