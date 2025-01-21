# Support for button detection and callbacks
#
# Copyright (C) 2018-2023  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging


######################################################################
# Button state tracking
######################################################################

QUERY_TIME = .002
RETRANSMIT_COUNT = 50

class MCU_buttons:
    def __init__(self, printer, mcu):
        self.reactor = printer.get_reactor()
        self.mcu = mcu
        self.mcu.register_config_callback(self.build_config)
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
            "buttons_query oid=%d clock=%d"
            " rest_ticks=%d retransmit_count=%d invert=%d" % (
                self.oid, clock, rest_ticks, RETRANSMIT_COUNT,
                self.invert), is_init=True)
        self.mcu.register_response(self.handle_buttons_state,
                                   "buttons_state", self.oid)
    def handle_buttons_state(self, params):
        # Expand the message ack_count from 8-bit
        ack_count = self.ack_count
        ack_diff = (params['ack_count'] - ack_count) & 0xff
        ack_diff -= (ack_diff & 0x80) << 1
        msg_ack_count = ack_count + ack_diff
        # Determine new buttons
        buttons = bytearray(params['state'])
        new_count = msg_ack_count + len(buttons) - self.ack_count
        if new_count <= 0:
            return
        new_buttons = buttons[-new_count:]
        # Send ack to MCU
        self.ack_cmd.send([self.oid, new_count])
        self.ack_count += new_count
        # Invoke callbacks with this event in main thread
        btime = params['#receive_time']
        for button in new_buttons:
            button ^= self.invert
            changed = button ^ self.last_button
            self.last_button = button
            for mask, shift, callback in self.callbacks:
                if changed & mask:
                    state = (button & mask) >> shift
                    self.reactor.register_async_callback(
                        (lambda et, c=callback, bt=btime, s=state: c(bt, s)))


######################################################################
# ADC button tracking
######################################################################

ADC_REPORT_TIME = 0.015
ADC_DEBOUNCE_TIME = 0.025
ADC_SAMPLE_TIME = 0.001
ADC_SAMPLE_COUNT = 6

class MCU_ADC_buttons:
    def __init__(self, printer, pin, pullup):
        self.reactor = printer.get_reactor()
        self.buttons = []
        self.last_button = None
        self.last_pressed = None
        self.last_debouncetime = 0
        self.pullup = pullup
        self.pin = pin
        self.min_value = 999999999999.9
        self.max_value = 0.
        ppins = printer.lookup_object('pins')
        self.mcu_adc = ppins.setup_pin('adc', self.pin)
        self.mcu_adc.setup_adc_sample(ADC_SAMPLE_TIME, ADC_SAMPLE_COUNT)
        self.mcu_adc.setup_adc_callback(ADC_REPORT_TIME, self.adc_callback)
        query_adc = printer.lookup_object('query_adc')
        query_adc.register_adc('adc_button:' + pin.strip(), self.mcu_adc)

    def setup_button(self, min_value, max_value, callback):
        self.min_value = min(self.min_value, min_value)
        self.max_value = max(self.max_value, max_value)
        self.buttons.append((min_value, max_value, callback))

    def adc_callback(self, read_time, read_value):
        adc = max(.00001, min(.99999, read_value))
        value = self.pullup * adc / (1.0 - adc)

        # Determine button pressed
        btn = None
        if self.min_value <= value <= self.max_value:
            for i, (min_value, max_value, cb) in enumerate(self.buttons):
                if min_value < value < max_value:
                    btn = i
                    break

        # If the button changed, due to noise or pressing:
        if btn != self.last_button:
            # reset the debouncing timer
            self.last_debouncetime = read_time

        # button debounce check & new button pressed
        if ((read_time - self.last_debouncetime) >= ADC_DEBOUNCE_TIME
            and self.last_button == btn and self.last_pressed != btn):
            # release last_pressed
            if self.last_pressed is not None:
                self.call_button(self.last_pressed, False)
                self.last_pressed = None
            if btn is not None:
                self.call_button(btn, True)
                self.last_pressed = btn

        self.last_button = btn

    def call_button(self, button, state):
        minval, maxval, callback = self.buttons[button]
        self.reactor.register_async_callback(
            (lambda e, cb=callback, s=state: cb(e, s)))


######################################################################
# Rotary Encoders
######################################################################

# Rotary encoder handler https://github.com/brianlow/Rotary
# Copyright 2011 Ben Buxton (bb@cactii.net).
# Licenced under the GNU GPL Version 3.
class BaseRotaryEncoder:
    R_START     = 0x0
    R_DIR_CW    = 0x10
    R_DIR_CCW   = 0x20
    R_DIR_MSK   = 0x30

    def __init__(self, cw_callback, ccw_callback):
        self.cw_callback = cw_callback
        self.ccw_callback = ccw_callback
        self.encoder_state = self.R_START
    def encoder_callback(self, eventtime, state):
        es = self.ENCODER_STATES[self.encoder_state & 0xf][state & 0x3]
        self.encoder_state = es
        if es & self.R_DIR_MSK == self.R_DIR_CW:
            self.cw_callback(eventtime)
        elif es & self.R_DIR_MSK == self.R_DIR_CCW:
            self.ccw_callback(eventtime)

class FullStepRotaryEncoder(BaseRotaryEncoder):
    R_CW_FINAL  = 0x1
    R_CW_BEGIN  = 0x2
    R_CW_NEXT   = 0x3
    R_CCW_BEGIN = 0x4
    R_CCW_FINAL = 0x5
    R_CCW_NEXT  = 0x6

    # Use the full-step state table (emits a code at 00 only)
    ENCODER_STATES = (
        # R_START
        (BaseRotaryEncoder.R_START, R_CW_BEGIN, R_CCW_BEGIN,
         BaseRotaryEncoder.R_START),

        # R_CW_FINAL
        (R_CW_NEXT, BaseRotaryEncoder.R_START, R_CW_FINAL,
         BaseRotaryEncoder.R_START | BaseRotaryEncoder.R_DIR_CW),

        # R_CW_BEGIN
        (R_CW_NEXT, R_CW_BEGIN, BaseRotaryEncoder.R_START,
         BaseRotaryEncoder.R_START),

        # R_CW_NEXT
        (R_CW_NEXT, R_CW_BEGIN, R_CW_FINAL, BaseRotaryEncoder.R_START),

        # R_CCW_BEGIN
        (R_CCW_NEXT, BaseRotaryEncoder.R_START, R_CCW_BEGIN,
         BaseRotaryEncoder.R_START),

        # R_CCW_FINAL
        (R_CCW_NEXT, R_CCW_FINAL, BaseRotaryEncoder.R_START,
         BaseRotaryEncoder.R_START | BaseRotaryEncoder.R_DIR_CCW),

        # R_CCW_NEXT
        (R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, BaseRotaryEncoder.R_START)
    )

class HalfStepRotaryEncoder(BaseRotaryEncoder):
    # Use the half-step state table (emits a code at 00 and 11)
    R_CCW_BEGIN   = 0x1
    R_CW_BEGIN    = 0x2
    R_START_M     = 0x3
    R_CW_BEGIN_M  = 0x4
    R_CCW_BEGIN_M = 0x5

    ENCODER_STATES = (
        # R_START (00)
        (R_START_M, R_CW_BEGIN, R_CCW_BEGIN, BaseRotaryEncoder.R_START),

        # R_CCW_BEGIN
        (R_START_M | BaseRotaryEncoder.R_DIR_CCW, BaseRotaryEncoder.R_START,
         R_CCW_BEGIN, BaseRotaryEncoder.R_START),

        # R_CW_BEGIN
        (R_START_M | BaseRotaryEncoder.R_DIR_CW,  R_CW_BEGIN,
         BaseRotaryEncoder.R_START, BaseRotaryEncoder.R_START),

        # R_START_M (11)
        (R_START_M, R_CCW_BEGIN_M, R_CW_BEGIN_M,  BaseRotaryEncoder.R_START),

        # R_CW_BEGIN_M
        (R_START_M, R_START_M, R_CW_BEGIN_M,
         BaseRotaryEncoder.R_START | BaseRotaryEncoder.R_DIR_CW),

        # R_CCW_BEGIN_M
        (R_START_M, R_CCW_BEGIN_M, R_START_M,
         BaseRotaryEncoder.R_START | BaseRotaryEncoder.R_DIR_CCW),
    )


######################################################################
# Button registration code
######################################################################

class PrinterButtons:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.load_object(config, 'query_adc')
        self.mcu_buttons = {}
        self.adc_buttons = {}
    def register_adc_button(self, pin, min_val, max_val, pullup, callback):
        adc_buttons = self.adc_buttons.get(pin)
        if adc_buttons is None:
            self.adc_buttons[pin] = adc_buttons = MCU_ADC_buttons(
                self.printer, pin, pullup)
        adc_buttons.setup_button(min_val, max_val, callback)
    def register_adc_button_push(self, pin, min_val, max_val, pullup, callback):
        def helper(eventtime, state, callback=callback):
            if state:
                callback(eventtime)
        self.register_adc_button(pin, min_val, max_val, pullup, helper)
    def register_buttons(self, pins, callback):
        # Parse pins
        ppins = self.printer.lookup_object('pins')
        mcu = mcu_name = None
        pin_params_list = []
        for pin in pins:
            pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
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
    def register_rotary_encoder(self, pin1, pin2, cw_callback, ccw_callback,
                                steps_per_detent):
        if steps_per_detent == 2:
            re = HalfStepRotaryEncoder(cw_callback, ccw_callback)
        elif steps_per_detent == 4:
            re = FullStepRotaryEncoder(cw_callback, ccw_callback)
        else:
            raise self.printer.config_error(
                "%d steps per detent not supported" % steps_per_detent)
        self.register_buttons([pin1, pin2], re.encoder_callback)
    def register_button_push(self, pin, callback):
        def helper(eventtime, state, callback=callback):
            if state:
                callback(eventtime)
        self.register_buttons([pin], helper)

def load_config(config):
    return PrinterButtons(config)
