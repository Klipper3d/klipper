# Support for PWM driven LEDs
#
# Copyright (C) 2019-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import output_pin

# Helper code for common LED initialization and control
class LEDHelper:
    def __init__(self, config, update_func, led_count=1):
        self.printer = config.get_printer()
        self.update_func = update_func
        self.led_count = led_count
        self.need_transmit = False
        # Initial color
        red = config.getfloat('initial_RED', 0., minval=0., maxval=1.)
        green = config.getfloat('initial_GREEN', 0., minval=0., maxval=1.)
        blue = config.getfloat('initial_BLUE', 0., minval=0., maxval=1.)
        white = config.getfloat('initial_WHITE', 0., minval=0., maxval=1.)
        self.led_state = [(red, green, blue, white)] * led_count
        # Support setting an led template
        self.template_eval = output_pin.lookup_template_eval(config)
        self.tcallbacks = [(lambda text, s=self, index=i+1:
                            s._template_update(index, text))
                           for i in range(led_count)]
        # Register commands
        name = config.get_name().split()[-1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_LED", "LED", name, self.cmd_SET_LED,
                                   desc=self.cmd_SET_LED_help)
        gcode.register_mux_command("SET_LED_TEMPLATE", "LED", name,
                                   self.cmd_SET_LED_TEMPLATE,
                                   desc=self.cmd_SET_LED_TEMPLATE_help)
    def get_status(self, eventtime=None):
        return {'color_data': self.led_state}
    def _set_color(self, index, color):
        if index is None:
            new_led_state = [color] * self.led_count
            if self.led_state == new_led_state:
                return
        else:
            if self.led_state[index - 1] == color:
                return
            new_led_state = list(self.led_state)
            new_led_state[index - 1] = color
        self.led_state = new_led_state
        self.need_transmit = True
    def _template_update(self, index, text):
        try:
            parts = [max(0., min(1., float(f)))
                     for f in text.split(',', 4)]
        except ValueError as e:
            logging.exception("led template render error")
            parts = []
        if len(parts) < 4:
            parts += [0.] * (4 - len(parts))
        self._set_color(index, tuple(parts))
    def _check_transmit(self, print_time=None):
        if not self.need_transmit:
            return
        self.need_transmit = False
        try:
            self.update_func(self.led_state, print_time)
        except self.printer.command_error as e:
            logging.exception("led update transmit error")
    cmd_SET_LED_help = "Set the color of an LED"
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        white = gcmd.get_float('WHITE', 0., minval=0., maxval=1.)
        index = gcmd.get_int('INDEX', None, minval=1, maxval=self.led_count)
        transmit = gcmd.get_int('TRANSMIT', 1)
        sync = gcmd.get_int('SYNC', 1)
        color = (red, green, blue, white)
        # Update and transmit data
        def lookahead_bgfunc(print_time):
            self._set_color(index, color)
            if transmit:
                self._check_transmit(print_time)
        if sync:
            #Sync LED Update with print time and send
            toolhead = self.printer.lookup_object('toolhead')
            toolhead.register_lookahead_callback(lookahead_bgfunc)
        else:
            #Send update now (so as not to wake toolhead and reset idle_timeout)
            lookahead_bgfunc(None)
    cmd_SET_LED_TEMPLATE_help = "Assign a display_template to an LED"
    def cmd_SET_LED_TEMPLATE(self, gcmd):
        index = gcmd.get_int("INDEX", None, minval=1, maxval=self.led_count)
        set_template = self.template_eval.set_template
        if index is not None:
            set_template(gcmd, self.tcallbacks[index-1], self._check_transmit)
        else:
            for i in range(self.led_count):
                set_template(gcmd, self.tcallbacks[i], self._check_transmit)

# Handler for PWM controlled LEDs
class PrinterPWMLED:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        # Configure pwm pins
        ppins = printer.lookup_object('pins')
        max_duration = printer.lookup_object('mcu').max_nominal_duration()
        cycle_time = config.getfloat('cycle_time', 0.010, above=0.,
                                     maxval=max_duration)
        hardware_pwm = config.getboolean('hardware_pwm', False)
        self.pins = []
        for i, name in enumerate(("red", "green", "blue", "white")):
            pin_name = config.get(name + '_pin', None)
            if pin_name is None:
                continue
            mcu_pin = ppins.setup_pin('pwm', pin_name)
            mcu_pin.setup_max_duration(0.)
            mcu_pin.setup_cycle_time(cycle_time, hardware_pwm)
            self.pins.append((i, mcu_pin))
        if not self.pins:
            raise config.error("No LED pin definitions found in '%s'"
                               % (config.get_name(),))
        self.last_print_time = 0.
        # Initialize color data
        self.led_helper = LEDHelper(config, self.update_leds, 1)
        self.prev_color = color = self.led_helper.get_status()['color_data'][0]
        for idx, mcu_pin in self.pins:
            mcu_pin.setup_start_value(color[idx], 0.)
    def update_leds(self, led_state, print_time):
        mcu = self.pins[0][1].get_mcu()
        min_sched_time = mcu.min_schedule_time()
        if print_time is None:
            eventtime = self.printer.get_reactor().monotonic()
            print_time = mcu.estimated_print_time(eventtime) + min_sched_time
        print_time = max(print_time, self.last_print_time + min_sched_time)
        color = led_state[0]
        for idx, mcu_pin in self.pins:
            if self.prev_color[idx] != color[idx]:
                mcu_pin.set_pwm(print_time, color[idx])
                self.last_print_time = print_time
        self.prev_color = color
    def get_status(self, eventtime=None):
        return self.led_helper.get_status(eventtime)

def load_config_prefix(config):
    return PrinterPWMLED(config)
