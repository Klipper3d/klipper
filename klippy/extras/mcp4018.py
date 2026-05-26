# MCP4018 digipot support
#
# Copyright (C) 2019-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus

class mcp4018:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=0x2f)
        self.scale = config.getfloat('scale', 1., above=0.)
        self.start_value = config.getfloat('wiper',
                                           minval=0., maxval=self.scale)
        config.get_printer().register_event_handler("klippy:connect",
                                                    self.handle_connect)
        # Register commands
        self.name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_DIGIPOT", "DIGIPOT", self.name,
                                   self.cmd_SET_DIGIPOT,
                                   desc=self.cmd_SET_DIGIPOT_help)
    def handle_connect(self):
        self.set_dac(self.start_value)
    def set_dac(self, value):
        val = int(value * 127. / self.scale + .5)
        self.i2c.i2c_write([val])
    cmd_SET_DIGIPOT_help = "Set digipot value"
    def cmd_SET_DIGIPOT(self, gcmd):
        wiper = gcmd.get_float('WIPER', minval=0., maxval=self.scale)
        if wiper is not None:
            self.set_dac(wiper)
            gcmd.respond_info("New value for DIGIPOT = %s, wiper = %.2f"
                               % (self.name, wiper))
def load_config_prefix(config):
    return mcp4018(config)
