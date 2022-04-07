# Module for reading generic ADC values from an Arduino over i2c
#
# Copyright (C) 2022 Chris Lombardi <clearchris@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, struct
from . import bus

AA_CHIP_ADDR=0x08
AA_I2C_SPEED=100000
AA_VALUE_LENGTH=9
AA_RETRIES=10
AA_ADC_BITS=24

class MCU_ArduinoAdc:

    def __init__(self, main, config, pin_params):
        self._main = main
        self.config = config
        self.reactor = main.printer.get_reactor()
        self.i2c = main.i2c
        self.mcu = main.mcu
        self.report_time = 1
        self._last_value = 0.
        self._last_time = 0
        self._timer = None
        self._mcu_adc = main.mcu.setup_pin('adc', pin_params)
        self.query_adc = None
        self._callback = None

        try:
            query_adc = main.printer.lookup_object('query_adc')
        except:
            query_adc = main.printer.load_object(config, 'query_adc')

        query_adc.register_adc(main.name, self)

        main.printer.register_event_handler("klippy:ready", self._handle_ready)


    def _handle_ready(self):
        self._timer = self.reactor.register_timer(self._run_timer,
            self.reactor.NOW)

    def get_mcu(self):
        return self.mcu

    def setup_adc_callback(self, report_time, callback):
        if report_time is not None:
          self.report_time = report_time
        self._callback = callback

    def setup_minmax(self, sample_time, sample_count, minval=0., maxval=1.,
                     range_check_count=0):
        pass

    def get_last_value(self):
        return self._last_value, self._last_time

    def _read_value(self):
        # adc value arrives as a byte array of ascii values
        success = False
        tries = 0
        response = None

        while not success and tries < AA_RETRIES:
            try:
                result = self.i2c.i2c_read([], AA_VALUE_LENGTH)
            except:
                tries += 1
                logging.warn("I2C exception in arduino_adc")
            else:
                success = True
                response = bytearray(result['response'])

        return response

    def _get_decoded_value(self):
        # adc value arrives as a byte array of ascii values
        response = self._read_value()
        reading = ""
        for val in response:
            reading = reading + chr(val)
        return float(reading)

    def _get_scaled_value(self):
        unscaled = self._get_decoded_value()
        return unscaled / pow(2, AA_ADC_BITS)

    def _run_timer(self, eventtime):
        self._last_value = self._get_scaled_value()
        self._last_time = eventtime
        if self._callback is not None:
          self._callback(self.mcu.estimated_print_time(self._last_time),
              self._last_value)
        return self._last_time + self.report_time


class PrinterArduinoAdc:

    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.name = config.get_name().split()[1]
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=AA_CHIP_ADDR,
            default_speed=AA_I2C_SPEED)
        self.mcu = self.i2c.get_mcu()
        ppins = self.printer.lookup_object('pins')
        ppins.register_chip(self.name, self)

    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'adc':
            raise self.printer.config_error("ArduinoAdc only supports adc pins")
        return MCU_ArduinoAdc(self, self.config, pin_params)


def load_config_prefix(config):
    return PrinterArduinoAdc(config)
