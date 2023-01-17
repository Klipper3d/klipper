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

class PrinterTemperatureTMC2240_E1:
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
        self.printer.add_object("temperature_tmc2240_e1 " + self.name, self)          
        self.sample_timer = self.reactor.register_timer(
            self._sample_tmc2240_e1_temperature)
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
    def _sample_tmc2240_e1_temperature(self, eventtime):
        self.tmc2240_temp = self._calc_temp_from_field()
        if self.temp < self.min_temp:
            self.printer.invoke_shutdown(
                "TMC2240_e1 temperature %0.1f below minimum temperature of %0.1f."
                % (self.temp, self.min_temp,))
        if self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "TMC2240_e1 temperature %0.1f above maximum temperature of %0.1f."
                % (self.temp, self.max_temp,))
        mcu = self.printer.lookup_object('mcu')
        measured_time = self.reactor.monotonic()
        self._callback(mcu.estimated_print_time(measured_time), self.tmc2240_temp)    
        return measured_time + REPORT_TIME 


MAX_INVALID_COUNT = 3

# class TMC2240TEMPERATURE:
#     def __init__(self, config):
#         self.fields = tmc.FieldHelper(tmc2240.Fields, tmc2240.SignedFields, tmc2240.FieldFormatters)
#         #self.mcu_tmc = tmc2130.MCU_TMC_SPI(config, tmc2240.Registers, self.fields)
#         share = None
#         self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=4000000,
#                                            share_type=share)
#         self.printer = config.get_printer()
#         self._callback = None 
#         self.min_sample_value = self.max_sample_value = 0
#         self.mcu = mcu = self.spi.get_mcu()
#         self.oid = oid = mcu.create_oid()   
#         mcu.register_response(self._handle_spi_response,
#                               "tmc2240temperature_result", oid)
#         mcu.register_config_callback(self._build_config)
#     def calc_temp_from_field(self, adc):
#         adc = adc
#         temp = (float)(adc-2038)/7.7
#         return temp  
#     def setup_callback(self, cb):
#         self._callback = cb
#     def get_report_time_delta(self):
#         return REPORT_TIME 
#     def setup_minmax(self, min_temp, max_temp):
#         min_temp = 0
#         max_temp = 0xC79
#         self.min_sample_value = min_temp
#         self.max_sample_value = max_temp
#     def _build_config(self):
#         self.mcu.add_config_cmd(
#             "config_tmc2240 oid=%u spi_oid=%u" % (
#                 self.oid, self.spi.get_oid()))
#         clock = self.mcu.get_query_slot(self.oid)
#         self._report_clock = self.mcu.seconds_to_clock(REPORT_TIME)
#         self.mcu.add_config_cmd(
#             "query_tmc2240 oid=%u clock=%u rest_ticks=%u"
#             " min_value=%u max_value=%u " % (
#                 self.oid, clock, self._report_clock,
#                 self.min_sample_value, self.max_sample_value), is_init=True)
#     def _handle_spi_response(self, params):
#         logging.info("adc_value=%#x", params['value'])      
#         temp = self.calc_temp_from_field(params['value'])
#         next_clock      = self.mcu.clock32_to_clock64(params['next_clock'])
#         last_read_clock = next_clock - self._report_clock
#         last_read_time  = self.mcu.clock_to_print_time(last_read_clock)
#         self._callback(last_read_time, temp)


def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("temperature_tmc2240_e1", PrinterTemperatureTMC2240_E1)
