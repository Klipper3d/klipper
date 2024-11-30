# Support for micro-controller chip based temperature sensors
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import mcu

SAMPLE_TIME = 0.1
REPORT_TIME = 0.1

class PrinterTemperatureHCU:
    def __init__(self, config):
        self.printer = config.get_printer()
        # Read config
        mcu_name = config.get('sensor_mcu', 'mcu')
        # Setup register query
        _mcu = config.get_printer().lookup_object("mcu hcu")
        _mcu_temp_register = _mcu.setup_register(0x200E)
        _mcu_temp_register.setup_register_read_callback(REPORT_TIME, self.hcu_temp_callback)
        _mcu_pwm_register = _mcu.setup_register(0x2020)
        _mcu_pwm_register.setup_register_read_callback(REPORT_TIME, self.hcu_pwm_callback)

    def setup_callback(self, temperature_callback):
        self.temperature_callback = temperature_callback
    def setup_pwm_callback(self, pwm_callback):
        self.pwm_callback = pwm_callback
    def get_report_time_delta(self):
        return REPORT_TIME
    def hcu_temp_callback(self, read_time, read_value):
        temp = read_value/10.0
        self.temperature_callback(read_time + SAMPLE_TIME, temp)
    def hcu_pwm_callback(self, read_time, read_value):
        duty_cycle = read_value/65535
        self.pwm_callback(read_time + SAMPLE_TIME, duty_cycle)
    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("temperature_hcu", PrinterTemperatureHCU)
