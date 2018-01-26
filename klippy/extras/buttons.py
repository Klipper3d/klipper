# Test code for button reading
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

QUERY_TIME = .005

class PrinterButtons:
    def __init__(self, config):
        ppins = config.get_printer().lookup_object('pins')
        mcu = None
        self.pin_list = []
        for pin in config.get('pins').split(','):
            pin_params = ppins.lookup_pin('digital_in', pin.strip())
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("All buttons must be on same mcu")
            mcu = pin_params['chip']
            self.pin_list.append((pin_params['pin'], pin_params['pullup'],
                                  pin_params['invert']))
        self.mcu = mcu
        self.oid = mcu.create_oid()
        mcu.add_config_cmd("config_buttons oid=%d button_count=%d" % (
            self.oid, len(self.pin_list)))
        mcu.add_config_cmd("buttons_query clock=0 rest_ticks=0", is_init=True)
        for i, (pin, pull_up, invert) in enumerate(self.pin_list):
            mcu.add_config_cmd("buttons_add pos=%d pin=%s pull_up=%d" % (
                i, pin, pull_up), is_init=True)
        mcu.add_config_object(self)
        self.cmd_queue = mcu.alloc_command_queue()
        self.ack_cmd = None
        self.ack_count = 0
    def build_config(self):
        self.ack_cmd = self.mcu.lookup_command("buttons_ack count=%c")
        clock = self.mcu.get_query_slot(self.oid)
        rest_ticks = self.mcu.seconds_to_clock(QUERY_TIME)
        self.mcu.add_config_cmd("buttons_query clock=%d rest_ticks=%d" % (
            clock, rest_ticks), is_init=True)
        self.mcu.register_msg(self.handle_buttons_state, "buttons_state")
    def handle_buttons_state(self, params):
        # Expand the message ack_count from 8-bit
        ack_count = self.ack_count
        ack_diff = (ack_count - params['ack_count']) & 0xff
        if ack_diff & 0x80:
            ack_diff -= 0x100
        msg_ack_count = ack_count - ack_diff
        # Determine new buttons
        buttons = params['state']
        new_count = msg_ack_count + len(buttons) - self.ack_count
        if new_count > 0:
            new_buttons = buttons[-new_count:]
            self.mcu.send(self.ack_cmd.encode(new_count), cq=self.cmd_queue)
            self.ack_count += new_count
        else:
            new_buttons = ""
        # Report via log..
        logging.debug("state: %d: %s (%d %d: %s)", new_count, repr(new_buttons),
                      self.ack_count, params['ack_count'], repr(buttons))
        out = []
        for b in new_buttons:
            b = ord(b)
            out.append(','.join([
                pin for i, (pin, pull_up, invert) in enumerate(self.pin_list)
                if ((b>>i) & 1) ^ invert]))
        logging.info("buttons=%s", ' '.join(out))

def load_config(config):
    return PrinterButtons(config)
