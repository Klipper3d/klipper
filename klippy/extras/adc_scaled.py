# Support for scaling ADC values based on measured VREF and VSSA
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

SAMPLE_TIME = 0.001
SAMPLE_COUNT = 8
REPORT_TIME = 0.300

class MCU_scaled_adc:
    def __init__(self, main, pin_params):
        self._main = main
        self._last_state = (0., 0.)
        self._mcu_adc = main.mcu.setup_pin('adc', pin_params)
        query_adc = main.printer.lookup_object('query_adc')
        qname = main.name + ":" + pin_params['pin']
        query_adc.register_adc(qname, self._mcu_adc)
        self._callback = None
        self.setup_adc_sample = self._mcu_adc.setup_adc_sample
        self.get_mcu = self._mcu_adc.get_mcu
    def _handle_callback(self, read_time, read_value):
        max_adc = self._main.last_vref[1]
        min_adc = self._main.last_vssa[1]
        scaled_val = (read_value - min_adc) / (max_adc - min_adc)
        self._last_state = (scaled_val, read_time)
        self._callback(read_time, scaled_val)
    def setup_adc_callback(self, report_time, callback):
        self._callback = callback
        self._mcu_adc.setup_adc_callback(report_time, self._handle_callback)
    def get_last_value(self):
        return self._last_state

class PrinterADCScaled:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        self.last_vref = (0., 0.)
        self.last_vssa = (0., 0.)
        # Configure vref and vssa pins
        self.mcu_vref = self._config_pin(config, 'vref', self.vref_callback)
        self.mcu_vssa = self._config_pin(config, 'vssa', self.vssa_callback)
        smooth_time = config.getfloat('smooth_time', 2., above=0.)
        self.inv_smooth_time = 1. / smooth_time
        self.mcu = self.mcu_vref.get_mcu()
        if self.mcu is not self.mcu_vssa.get_mcu():
            raise config.error("vref and vssa must be on same mcu")
        # Register setup_pin
        ppins = self.printer.lookup_object('pins')
        ppins.register_chip(self.name, self)
    def _config_pin(self, config, name, callback):
        pin_name = config.get(name + '_pin')
        ppins = self.printer.lookup_object('pins')
        mcu_adc = ppins.setup_pin('adc', pin_name)
        mcu_adc.setup_adc_callback(REPORT_TIME, callback)
        mcu_adc.setup_adc_sample(SAMPLE_TIME, SAMPLE_COUNT)
        query_adc = config.get_printer().load_object(config, 'query_adc')
        query_adc.register_adc(self.name + ":" + name, mcu_adc)
        return mcu_adc
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'adc':
            raise self.printer.config_error("adc_scaled only supports adc pins")
        return MCU_scaled_adc(self, pin_params)
    def calc_smooth(self, read_time, read_value, last):
        last_time, last_value = last
        time_diff = read_time - last_time
        value_diff = read_value - last_value
        adj_time = min(time_diff * self.inv_smooth_time, 1.)
        smoothed_value = last_value + value_diff * adj_time
        return (read_time, smoothed_value)
    def vref_callback(self, read_time, read_value):
        self.last_vref = self.calc_smooth(read_time, read_value, self.last_vref)
    def vssa_callback(self, read_time, read_value):
        self.last_vssa = self.calc_smooth(read_time, read_value, self.last_vssa)

def load_config_prefix(config):
    return PrinterADCScaled(config)
