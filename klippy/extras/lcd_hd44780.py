# LCD demo code for the hd44780 display
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

class PrinterHD44780:
    def __init__(self, config):
        printer = config.get_printer()
        # pin config
        ppins = printer.lookup_object('pins')
        pins = [ppins.lookup_pin('digital_out', config.get(name + '_pin'))
                for name in ['rs', 'e', 'd4', 'd5', 'd6', 'd7']]
        mcu = None
        for pin_params in pins:
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("hd44780 all pins must be on same mcu")
            mcu = pin_params['chip']
            if pin_params['invert']:
                raise pins.error("hd44780 can not invert pin")
        self.mcu = mcu
        self.mcu.add_config_cmd(
            "config_hd44780 oid=%d rs_pin=%s e_pin=%s"
            " d4_pin=%s d5_pin=%s d6_pin=%s d7_pin=%s" % (
                self.mcu.create_oid(), pins[0]['pin'], pins[1]['pin'],
                pins[2]['pin'], pins[3]['pin'], pins[4]['pin'], pins[5]['pin']))
        self.mcu.add_config_object(self)
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.send_data_cmd = self.send_cmds_cmd = None
        # output buffers
        self.pending_data = []
        self.pending_type = None
        # work timer
        self.reactor = printer.get_reactor()
        self.work_timer = self.reactor.register_timer(self.work_event)
        self.init_timer = self.reactor.register_timer(self.init_event)
        self.position = 0
    def build_config(self):
        self.send_cmds_cmd = self.mcu.lookup_command(
            "hd44780_send_cmds cmds=%*s")
        self.send_data_cmd = self.mcu.lookup_command(
            "hd44780_send_data data=%*s")
    def printer_state(self, state):
        if state == 'ready':
            self.reactor.update_timer(self.init_timer, self.reactor.NOW)
    def flush(self):
        if not self.pending_data:
            return
        if self.pending_type == 'cmd':
            msg = self.send_cmds_cmd.encode(self.pending_data)
        else:
            msg = self.send_data_cmd.encode(self.pending_data)
        self.mcu.send(msg, reqclock=BACKGROUND_PRIORITY_CLOCK, cq=self.cmd_queue)
        del self.pending_data[:]
    def send_data(self, byte):
        if self.pending_type != 'data' or len(self.pending_data) >= 16:
            self.flush()
            self.pending_type = 'data'
        self.pending_data.append(byte)
    def send_command(self, cmd):
        if self.pending_type != 'cmd' or len(self.pending_data) >= 16:
            self.flush()
            self.pending_type = 'cmd'
        self.pending_data.append(cmd)
    def init_event(self, eventtime):
        self.reactor.unregister_timer(self.init_timer)
        self.init_timer = None
        for cmd in [0x33, 0x33, 0x33, 0x22]:
            self.mcu.send(self.send_cmds_cmd.encode([cmd]), cq=self.cmd_queue)
            self.reactor.pause(self.reactor.monotonic() + .100)
        self.send_command(0x08)
        self.send_command(0x06)
        self.send_command(0x10)
        self.send_command(0x01)
        self.flush()
        self.reactor.pause(self.reactor.monotonic() + .100)
        self.send_command(0x0c)
        self.flush()
        self.reactor.update_timer(self.work_timer, self.reactor.NOW)
        return self.reactor.NEVER
    def work_event(self, eventtime):
        if self.position < 40:
            self.send_command(0x80 | self.position)
        else:
            self.send_command(0xc0 | (self.position - 40))
        self.send_data(ord(' '))
        self.send_data(ord('#'))
        self.flush()
        self.position = (self.position + 1) % 80
        return eventtime + .100

def load_config(config):
    return PrinterHD44780(config)
