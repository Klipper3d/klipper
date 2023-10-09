# Support fans that are enabled when a heater is on
#
# Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import fan

PIN_MIN_TIME = 0.100
SAFETY_CHECK_INIT_TIME = 3.

class PrinterHeaterFan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.load_object(config, 'heaters')
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.heater_names = config.getlist("heater", ("extruder",))
        self.heater_temp = config.getfloat("heater_temp", 50.0)
        self.heaters = []
        self.fan = fan.Fan(config, default_shutdown_speed=1.)
        self.fan_speed = config.getfloat("fan_speed", 1., minval=0., maxval=1.)
        self.last_speed = 0.
        self.name = config.get_name().split()[-1]
        self.num_err = 0
        self.max_err = 3
        self.min_rpm = config.getint("min_rpm", 0, minval=0)
        if self.min_rpm > 0 and self.fan.tachometer._freq_counter is None:
            raise config.error(
                "'tachometer_pin' must be specified before enabling `min_rpm`")
    def handle_ready(self):
        pheaters = self.printer.lookup_object('heaters')
        self.heaters = [pheaters.lookup_heater(n) for n in self.heater_names]
        reactor = self.printer.get_reactor()
        reactor.register_timer(self.callback, reactor.monotonic()+PIN_MIN_TIME)
        if self.min_rpm > 0:
            reactor.register_timer(
                self.fan_check, reactor.monotonic()+SAFETY_CHECK_INIT_TIME)
    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)
    def callback(self, eventtime):
        speed = 0.
        for heater in self.heaters:
            current_temp, target_temp = heater.get_temp(eventtime)
            if target_temp or current_temp > self.heater_temp:
                speed = self.fan_speed
        if speed != self.last_speed:
            self.last_speed = speed
            curtime = self.printer.get_reactor().monotonic()
            print_time = self.fan.get_mcu().estimated_print_time(curtime)
            self.fan.set_speed(print_time + PIN_MIN_TIME, speed)
        return eventtime + 1.
    def fan_check(self, eventtime):
        rpm = self.fan.tachometer.get_status(eventtime)['rpm']
        has_err = False
        if rpm is not None:
            for heater in self.heaters:
                current_temp, target_temp = heater.get_temp(eventtime)
                if current_temp > self.heater_temp and rpm < self.min_rpm:
                    has_err = True
        if has_err:
            self.num_err += 1
            if self.num_err > self.max_err:
                msg = "'%s' spinning below minimum safe speed of %d rev/min" % (
                    self.name, self.min_rpm)
                logging.error(msg)
                self.printer.invoke_shutdown(msg)
                return self.printer.get_reactor().NEVER
        else:
            self.num_err = 0
        return eventtime + 1.5

def load_config_prefix(config):
    return PrinterHeaterFan(config)
