# AD5206 digipot code
#
# Copyright (C) 2017,2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class ad5206:
    def __init__(self, config):
        # Get enable pin config
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        enable_pin = config.get('enable_pin')
        enable_pin_params = ppins.lookup_pin(enable_pin)
        self.mcu = enable_pin_params['chip']
        pin = enable_pin_params['pin']
        # Get channel config
        self.spi_send_cmd = None
        scale = config.getfloat('scale', 1., above=0.)
        self.channels = [None]*6
        self.printing_values = list(self.channels)
        for i in range(len(self.channels)):
            val = config.getfloat('channel_%d' % (i+1,), None,
                                  minval=0., maxval=scale)
            if val is None:
                continue
            self.channels[i] = int(val * 256. / scale + .5)
            val = config.getfloat('channel_%d_while_printing' % (i+1,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                self.printing_values[i] = int(val * 256. / scale + .5)
        if self.printing_values != [None]*6:
            printer.register_event_handler("idle_timeout:printing",
                                           self.handle_printing)
            printer.register_event_handler("idle_timeout:ready",
                                           self.handle_ready)
        # Setup initial configuration
        self.mcu.register_config_callback(self.build_config)
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd(
            "config_spi oid=%d bus=%d pin=%s mode=%u rate=%u shutdown_msg=" % (
                self.oid, 0, pin, 0, 25000000))
        for i, val in enumerate(self.channels):
            if val is not None:
                self.set_channel(None, i, val)
    def build_config(self):
        cmd_queue = self.mcu.alloc_command_queue()
        self.spi_send_cmd = self.mcu.lookup_command(
            "spi_send oid=%c data=%*s", cq=cmd_queue)
    def set_channel(self, print_time, channel, val):
        if self.spi_send_cmd is None:
            self.mcu.add_config_cmd(
                "spi_send oid=%d data=%02x%02x" % (self.oid, channel, val),
                is_init=True)
            return
        clock = self.mcu.print_time_to_clock(print_time)
        self.spi_send_cmd.send([self.oid, [channel, val]],
                               minclock=clock, reqclock=clock)
    def handle_printing(self, print_time):
        for i, pv in enumerate(self.printing_values):
            if pv is not None:
                self.set_channel(print_time, i, pv)
    def handle_ready(self, print_time):
        for i, (val, pv) in enumerate(zip(self.channels, self.printing_values)):
            if pv is not None:
                self.set_channel(print_time, i, val)

def load_config_prefix(config):
    return ad5206(config)
