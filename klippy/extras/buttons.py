# Support for button detection and callbacks
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

QUERY_TIME = .002
RETRANSMIT_COUNT = 50


######################################################################
# Button state tracking
######################################################################

class MCU_buttons:
    def __init__(self, printer, mcu):
        self.reactor = printer.get_reactor()
        self.mcu = mcu
        mcu.add_config_object(self)
        self.pin_list = []
        self.callbacks = []
        self.invert = self.last_button = 0
        self.ack_cmd = None
        self.ack_count = 0
    def setup_buttons(self, pins, callback):
        mask = 0
        shift = len(self.pin_list)
        for pin_params in pins:
            if pin_params['invert']:
                self.invert |= 1 << len(self.pin_list)
            mask |= 1 << len(self.pin_list)
            self.pin_list.append((pin_params['pin'], pin_params['pullup']))
        self.callbacks.append((mask, shift, callback))
    def build_config(self):
        if not self.pin_list:
            return
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd("config_buttons oid=%d button_count=%d" % (
            self.oid, len(self.pin_list)))
        for i, (pin, pull_up) in enumerate(self.pin_list):
            self.mcu.add_config_cmd(
                "buttons_add oid=%d pos=%d pin=%s pull_up=%d" % (
                    self.oid, i, pin, pull_up), is_init=True)
        cmd_queue = self.mcu.alloc_command_queue()
        self.ack_cmd = self.mcu.lookup_command(
            "buttons_ack oid=%c count=%c", cq=cmd_queue)
        clock = self.mcu.get_query_slot(self.oid)
        rest_ticks = self.mcu.seconds_to_clock(QUERY_TIME)
        self.mcu.add_config_cmd(
            "buttons_query oid=%d clock=%d rest_ticks=%d retransmit_count=%d" % (
                self.oid, clock, rest_ticks, RETRANSMIT_COUNT), is_init=True)
        self.mcu.register_msg(
            self.handle_buttons_state, "buttons_state", self.oid)
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
        if new_count <= 0:
            return
        new_buttons = buttons[-new_count:]
        # Send ack to MCU
        self.ack_cmd.send([self.oid, new_count])
        self.ack_count += new_count
        # Call self.handle_button() with this event in main thread
        for b in new_buttons:
            self.reactor.register_async_callback(
                (lambda e, s=self, b=ord(b): s.handle_button(e, b)))
    def handle_button(self, eventtime, button):
        button ^= self.invert
        changed = button ^ self.last_button
        for mask, shift, callback in self.callbacks:
            if changed & mask:
                callback(eventtime, (button & mask) >> shift)
        self.last_button = button


######################################################################
# Rotary Encoders
######################################################################

class RotaryEncoder:
    def __init__(self, cw_callback, ccw_callback):
        self.cw_callback = cw_callback
        self.ccw_callback = ccw_callback
        self.next_callback = None
    def encoder_callback(self, eventtime, state):
        # XXX - do full encoder state tracking
        if state == 3:
            self.next_callback = None
        elif state == 2:
            self.next_callback = self.ccw_callback
        elif state == 1:
            self.next_callback = self.cw_callback
        elif self.next_callback is not None:
            self.next_callback(eventtime)
            self.next_callback = None


######################################################################
# Button registration code
######################################################################

class PrinterButtons:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.mcu_buttons = {}
    def register_buttons(self, pins, callback):
        # Parse pins
        ppins = self.printer.lookup_object('pins')
        mcu = mcu_name = None
        pin_params_list = []
        for pin in pins:
            pin_params = ppins.lookup_pin('digital_in', pin)
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("button pins must be on same mcu")
            mcu = pin_params['chip']
            mcu_name = pin_params['chip_name']
            pin_params_list.append(pin_params)
        # Register pins and callback with the appropriate MCU
        mcu_buttons = self.mcu_buttons.get(mcu_name)
        if (mcu_buttons is None
            or len(mcu_buttons.pin_list) + len(pin_params_list) > 8):
            self.mcu_buttons[mcu_name] = mcu_buttons = MCU_buttons(
                self.printer, mcu)
        mcu_buttons.setup_buttons(pin_params_list, callback)
    def register_rotary_encoder(self, pin1, pin2, cw_callback, ccw_callback):
        re = RotaryEncoder(cw_callback, ccw_callback)
        self.register_buttons([pin1, pin2], re.encoder_callback)
    def register_button_push(self, pin, callback):
        def helper(eventtime, state, callback=callback):
            if state:
                callback(eventtime)
        self.register_buttons([pin], helper)

def load_config(config):
    return PrinterButtons(config)
