# MCP4451 digipot code
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import mcu

WiperRegisters = [0x00, 0x01, 0x06, 0x07]

class mcp4451:
    def __init__(self, config):
        # Configure i2c bus
        printer = config.get_printer()
        self.mcu = mcu.get_printer_mcu(printer, config.get('mcu', 'mcu'))
        self.mcu.register_config_callback(self.build_config)
        self.i2c_addr = config.getint('i2c_address')
        self.i2c_send_cmd = None
        # Configure output values
        scale = config.getfloat('scale', 1., above=0.)
        self.wipers = [None]*4
        self.printing_values = list(self.wipers)
        for i in range(len(self.wipers)):
            val = config.getfloat('wiper_%d' % (i,), None,
                                  minval=0., maxval=scale)
            if val is None:
                continue
            self.wipers[i] = int(val * 255. / scale + .5)
            val = config.getfloat('wiper_%d_while_printing' % (i+1,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                self.printing_values[i] = int(val * 255. / scale + .5)
        if self.printing_values != [None]*4:
            printer.register_event_handler("idle_timeout:printing",
                                           self.handle_printing)
            printer.register_event_handler("idle_timeout:ready",
                                           self.handle_ready)
        # Setup initial configuration
        self.set_register(None, 0x04, 0xff)
        self.set_register(None, 0x0a, 0xff)
        for reg, val in zip(WiperRegisters, self.wipers):
            if val is not None:
                self.set_register(None, reg, val)
    def build_config(self):
        cmd_queue = self.mcu.alloc_command_queue()
        self.i2c_send_cmd = self.mcu.lookup_command(
            "i2c_send data=%*s", cq=cmd_queue)
    def set_register(self, print_time, reg, val):
        if self.i2c_send_cmd is None:
            self.mcu.add_config_cmd("i2c_send data=%02x%02x%02x" % (
                self.i2c_addr, (reg << 4) | ((val >> 8) & 0x03), val),
                                    is_init=True)
            return
        clock = self.mcu.print_time_to_clock(print_time)
        self.i2c_send_cmd.send([[self.i2c_addr, reg, val]],
                               minclock=clock, reqclock=clock)
    def handle_printing(self, print_time):
        for reg, pv in zip(WiperRegisters, self.printing_values):
            if pv is not None:
                self.set_register(print_time, reg, pv)
    def handle_ready(self, print_time):
        for reg, val, pv in zip(WiperRegisters, self.wipers,
                                self.printing_values):
            if pv is not None:
                self.set_register(print_time, reg, val)

def load_config_prefix(config):
    return mcp4451(config)
