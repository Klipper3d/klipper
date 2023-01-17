# Package definition for the extras directory
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import math, logging
from . import bus, tmc, tmc2240, tmc_uart, tmc2130

REPORT_TIME = 0.300
TEMP_MAX = 140
TEMP_MIN = 5

class PrinterTemperatureTMC2240_Z:
    def __init__(self, config):
        self.fields = tmc.FieldHelper(tmc2240.Fields, tmc2240.SignedFields, tmc2240.FieldFormatters)
        self.mcu_tmc = tmc2130.MCU_TMC_SPI(config, tmc2240.Registers, self.fields)
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()  
        self.name = config.get_name().split()[-1]  
        self._callback = None 
        self.temp = self.min_temp = self.max_temp = 0.0  
        self.setup_minmax(TEMP_MIN, TEMP_MAX)   
        self.fields = self.mcu_tmc.get_fields()
        self.printer.add_object("temperature_tmc2240_z " + self.name, self)          
        self.sample_timer = self.reactor.register_timer(
            self._sample_tmc2240_z_temperature)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def _calc_temp_from_field(self):
        adc = self.mcu_tmc.get_register("ADC_TEMP")
        temp = (float)(adc- 2038)/7.7  
        return temp  
    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp 
    def setup_callback(self, cb):
        self._callback = cb  
    def handle_connect(self):
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)
    def _sample_tmc2240_z_temperature(self, eventtime):
        self.tmc2240_temp = self._calc_temp_from_field()
        if self.temp < self.min_temp:
            self.printer.invoke_shutdown(
                "TMC2240_z temperature %0.1f below minimum temperature of %0.1f."
                % (self.temp, self.min_temp,))
        if self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "TMC2240_z temperature %0.1f above maximum temperature of %0.1f."
                % (self.temp, self.max_temp,))
        mcu = self.printer.lookup_object('mcu')
        measured_time = self.reactor.monotonic()
        self._callback(mcu.estimated_print_time(measured_time), self.tmc2240_temp)    
        return measured_time + REPORT_TIME 
                
def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("temperature_tmc2240_z", PrinterTemperatureTMC2240_Z)