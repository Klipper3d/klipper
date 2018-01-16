# LCD support for the st7920 chip
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, pins

DDRAM_ADDR = 1<<7

class PrinterST7920:
    def __init__(self, config):
        printer = config.get_printer()
        # pin config
        cs_pin = pins.setup_pin(printer, 'digital_out', config.get('cs_pin'))
        cs_pin.setup_start_value(1, 1, is_static=True)
        sclk_params = pins.get_printer_pins(printer).lookup_pin(
            'digital_out', config.get('sclk_pin'))
        sid_params = pins.get_printer_pins(printer).lookup_pin(
            'digital_out', config.get('sid_pin'))
        if sid_params['invert'] or sclk_params['invert']:
            raise pins.error("st7920 can not invert sclk or sid")
        if sid_params['chip'] is not sclk_params['chip']:
            raise pins.error("st7920 sclk must be on same mcu as sid")
        self.mcu = sclk_params['chip']
        self.mcu.add_config_cmd(
            "config_st7920 oid=%u sclk_pin=%s sid_pin=%s" % (
                self.mcu.create_oid(), sclk_params['pin'], sid_params['pin']))
        self.mcu.add_config_object(self)
        self.burst_queue = self.mcu.alloc_command_queue()
        self.burst_cmd = None
        # output buffers
        self.last_addr = -1
        self.pending_data = []
        # work timer
        self.reactor = printer.get_reactor()
        self.work_timer = self.reactor.register_timer(self.work_event)
        self.test_position = 0
    def build_config(self):
        self.burst_cmd = self.mcu.lookup_command(
            "st7920_burst cmd_then_data=%*s")
        self.mcu.register_msg(self.handle_position, "st7920_position")
        self.reactor.update_timer(self.work_timer, self.reactor.monotonic() + 5.)
    def handle_position(self, params):
        # XXX - release more data when queue space available
        pass
    def send(self, data):
        msg = self.burst_cmd.encode(data)
        # XXX - wait for space in mcu queue before transmitting!
        # XXX - send with low priority
        self.mcu.send(msg, cq=self.burst_queue)
    def flush_data(self):
        if self.pending_data:
            self.send([self.last_addr] + self.pending_data)
            self.last_addr = -1
            del self.pending_data[:]
    def send_data(self, addr, data):
        # XXX - support GDRAM and CGRAM addresses
        if (addr != self.last_addr + len(self.pending_data) // 2
            or len(self.pending_data) >= 16):
            self.flush_data()
            self.last_addr = addr
        self.pending_data.extend([(data >> 8) & 0xff, data & 0xff])
    def send_command(self, cmd):
        # XXX - support extended commands
        self.flush_data()
        self.send([cmd])
    def work_event(self, eventtime):
        # Test code
        self.send_data(DDRAM_ADDR | self.test_position, 0x2020)
        self.test_position = (self.test_position + 1) % 64
        self.send_data(DDRAM_ADDR | self.test_position, 0x2323)
        return eventtime + 0.100

def load_config(config):
    return PrinterST7920(config)
