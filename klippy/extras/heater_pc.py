# Klipper Heater Predictional Control
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import threading
from . import output_pin

class HeaterPredictControl:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.config = config
        self.eval_time = 0.3
        self.min_pwm = -1.0
        self.max_pwm = 1.0
        self.old_control = None
        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        # Use lock to pass data to/from heater code
        self.lock = threading.Lock()
        self.output = .0
        self.pwm_event_time = self.reactor.monotonic()
        # Link template
        self.template_name = config.get("macro_template")
        # Template handling
        self.template_eval = output_pin.lookup_template_eval(config)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def handle_connect(self):
        pheaters = self.printer.load_object(self.config, 'heaters')
        heater_name = self.config.get_name().split()[-1]
        heater = pheaters.heaters.get(heater_name)
        if heater is None:
            self.config.error("Heater %s is not registered" % (heater_name))
        self.eval_time = heater.get_pwm_delay()
        self.min_pwm = -heater.get_max_power()
        self.max_pwm = heater.get_max_power()
        self.old_control = heater.set_control(self)
        gcode = self.printer.lookup_object('gcode')
        params = {"TEMPLATE": self.template_name}
        fo_gcmd = gcode.create_gcode_command("", "", params)
        self.template_eval.set_template(fo_gcmd, self._adj_pwm)
    def _adj_pwm(self, text):
        try:
            output = float(text) * self.max_pwm
        except ValueError as e:
            logging.exception("heater_pc template render error")
            output = .0
        output = max(self.min_pwm, min(self.max_pwm, output))
        with self.lock:
            self.output = output
    def temperature_update(self, read_time, temp, target_temp):
        output = .0
        with self.lock:
            output = self.output
        # Returns +- max_power
        co = self.old_control.temperature_update(read_time, temp, target_temp)
        co += output
        return co
    def check_busy(self, eventtime, smoothed_temp, target_temp):
        res = self.old_control.check_busy(eventtime, smoothed_temp,
                                          target_temp)
        return res

def load_config_prefix(config):
    return HeaterPredictControl(config)
