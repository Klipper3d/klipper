# Support generic TMC Temperature Monitor
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

TRINAMIC_DRIVERS = ["tmc2130", "tmc2208", "tmc2209", "tmc2240", "tmc2660",
    "tmc5160"]

class TMCMonitor:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.stepper_name = config.get('stepper')
        self.driver_name = ""
        self.adc_temp = None
        self.mcu_tmc = None
        self.sample_timer = None
        self.printer.register_event_handler("klippy:connect", self.connect)

    def lookup_tmc(self):
        for driver in TRINAMIC_DRIVERS:
            self.driver_name = "%s %s" % (driver, self.stepper_name)
            module = self.printer.lookup_object(self.driver_name, None)
            if module is not None:
                return module
        raise self.printer.config_error("Unable to find TMC driver for %s"
                                         % (self.stepper_name,))
    def connect(self):
        tmc_module = self.lookup_tmc()
        self.mcu_tmc = tmc_module.mcu_tmc
        stepper_enable = self.printer.lookup_object('stepper_enable')
        self.adc_temp_reg = self.mcu_tmc.fields.lookup_register("adc_temp")
        if self.adc_temp_reg is None:
            self.printer.config_error("%s can't report ADC temperature"
                                       % (self.driver_name))
        enable_line = stepper_enable.lookup_enable(self.stepper_name)
        enable_line.register_state_callback(self._handle_stepper_enable)

    def _handle_stepper_enable(self, print_time, is_enable):
        if is_enable:
            cb = (lambda ev: self._do_enable(print_time))
        else:
            cb = (lambda ev: self._do_disable(print_time))
        self.printer.get_reactor().register_callback(cb)

    def _do_enable(self, print_time):
        if self.sample_timer is not None:
            self._do_disable(print_time)

        reactor = self.printer.get_reactor()
        curtime = reactor.monotonic()
        self.sample_timer = reactor.register_timer(self._sample, curtime + 1.)

    def _do_disable(self, print_time):
        if self.sample_timer is None:
            return
        reactor = self.printer.get_reactor()
        reactor.unregister_timer(self.sample_timer)
        self.sample_timer = None
        self.adc_temp = None

    def _query_temperature(self):
        try:
            self.adc_temp = self.mcu_tmc.get_register(self.adc_temp_reg)
        except self.printer.command_error as e:
            # Ignore comms error for temperature
            self.adc_temp = None
            return

    def _sample(self, eventtime):
        self._query_temperature()
        return eventtime + 1.

    def get_status(self, eventtime):
        temp = None
        if self.adc_temp is not None:
            temp = round((self.adc_temp - 2038) / 7.7, 2)
        return {
            'temperature': temp
        }
