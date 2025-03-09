# Klipper Heater Predictional Control
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import threading
from .gcode_macro import PrinterGCodeMacro
from .display import display

class HeaterPredictControl:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.config = config
        self.eval_time = 0.3
        self.min_pwm = -1.0
        self.max_pwm = 1.0
        self.render_timer = None
        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        # Use lock to pass data to/from heater code
        self.lock = threading.Lock()
        self.output = .0
        self.pwm_event_time = self.reactor.monotonic()
        # Link template
        template_name = config.get("macro_template")
        templates = display.lookup_display_templates(config)
        display_templates = templates.get_display_templates()
        self.create_context = PrinterGCodeMacro(config).create_template_context
        self.template = display_templates.get(template_name)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def handle_connect(self):
        pheaters = self.printer.load_object(self.config, 'heaters')
        heater_name = self.config.get_name().split()[-1]
        heater = pheaters.heaters.get(heater_name)
        if heater is None:
            self.config.error("Heater %s is not registered" % (heater_name))
        self.eval_time = heater.get_pwm_delay()
        self.min_pwm = -1.0 * heater.get_max_power()
        self.max_pwm = heater.get_max_power()
        reactor = self.reactor
        self.render_timer = reactor.register_timer(self._render, reactor.NOW)
        def callback():
            output = .0
            with self.lock:
                self.pwm_event_time = self.reactor.monotonic()
                output = self.output
            return output
        heater.set_pc_callback(callback)
    def _render(self, eventtime):
        context = self.create_context()
        output = self.template.render(context)
        # Normalize output to PWM limits
        output_f = float(output) * self.max_pwm
        output_f = max(self.min_pwm, min(self.max_pwm, output_f))
        with self.lock:
            self.output = output_f
            last_pwm = self.pwm_event_time
        # if we lag behind - reschedule
        if eventtime < last_pwm + self.eval_time * 3 / 4:
            return last_pwm + self.eval_time * 3 / 4
        return eventtime + self.eval_time

def load_config_prefix(config):
    return HeaterPredictControl(config)
