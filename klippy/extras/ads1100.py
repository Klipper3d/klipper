# Support for ADS1100 ADC chip connected via I2C
#
# Copyright (C) 2020 Martin Hierholzer <martin@hierholzer.info>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging, struct
from . import bus

ADS1100_CHIP_ADDR=0x49
ADS1100_I2C_SPEED=3000000

ADS1100_SAMPLE_RATE_TABLE={ 8:3, 16:2, 32:1, 128:0 }
ADS1100_GAIN_TABLE={ 1:0, 2:1, 4:2, 8:3 }

class ADS1100Error(Exception):
    pass

class MCU_ADS1100:

    def __init__(self, main):
        self._main = main
        self.reactor = main.printer.get_reactor()
        self.i2c = main.i2c
        self.mcu = main.mcu
        self.report_time = main.report_time
        self.rate = main.rate
        self.gain = main.gain
        self._last_value = 0.
        self._last_time = 0
        self.sample_timer = None

        query_adc = main.printer.lookup_object('query_adc')
        qname = main.name
        query_adc.register_adc(qname, self)
        self._callback = None

        main.printer.register_event_handler("klippy:ready", self._handle_ready)


    def _handle_ready(self):
        self._write_configuration()
        self.sample_timer = self.reactor.register_timer(self._sample_timer,
            self.reactor.NOW)


    def setup_adc_callback(self, report_time, callback):
        if self._callback is not None:
          logging.exception("ADS1100: ADC callback already configured")
          raise ADS1100Error
        if report_time is not None:
          self.report_time = report_time
        self._callback = callback


    def setup_minmax(self, sample_time, sample_count, minval, maxval,
                     range_check_count):
        pass


    def get_last_value(self):
        return self._last_value, self._last_time


    def read_single_value(self):

        # wait until conversion is ready and the timer callback has been called
        self._wait_conversion_ready()

        # extract and return result
        return self._last_value


    def _wait_conversion_ready(self):
        # wait shortly after the timer has called _sample_timer
        self._last_time = self.reactor.pause(self._last_time + self.report_time
            + 0.0001)


    def _read_response(self):
        while True :
          # read with error handling, spurious errors are possible
          result = self.i2c.i2c_read([], 2)

          response = bytearray(result['response'])

          # retry if response too short
          if len(response) < 2:
            logging.info("ADS1100: conversion failed, trying again...")
            continue

          # return response
          self._conversion_started = False
          return response


    def _sample_timer(self, eventtime):
        response = self._read_response()
        self._last_value = struct.unpack('>h', response[0:2])[0]
        self._last_time = eventtime
        self._call_callback()
        return self._last_time + self.report_time


    def _call_callback(self):
        if self._callback is not None :
          self._callback(self.mcu.estimated_print_time(self._last_time),
              self._last_value)


    def _write_configuration(self):
        # configuration byte: continuous conversion (SC bit not set), selected
        # gain and SPS
        config = ADS1100_SAMPLE_RATE_TABLE[self.rate] << 2 \
            | ADS1100_GAIN_TABLE[self.gain]

        # write the 8 bit configuration register
        self.i2c.i2c_write([config])


class PrinterADS1100:

    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        self.i2c = bus.MCU_I2C_from_config(config,
            default_addr=ADS1100_CHIP_ADDR, default_speed=ADS1100_I2C_SPEED)
        self.mcu = self.i2c.get_mcu()
        self.rate = config.getint('rate', 8)
        if self.rate not in ADS1100_SAMPLE_RATE_TABLE :
          raise self.printer.config_error("ADS1100 does not support the "
              "selected sampling rate: %d" % self.rate)
        self.report_time = 1./self.rate
        self.gain = config.getint('gain', 1, minval=1)
        if self.gain not in ADS1100_GAIN_TABLE :
          raise self.printer.config_error("ADS1100 does not support the "
              "selected gain: %d" % self.gain)
        # Register setup_pin
        ppins = self.printer.lookup_object('pins')
        ppins.register_chip(self.name, self)

    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'adc':
            raise self.printer.config_error("ADS1100 only supports adc pins")
        return MCU_ADS1100(self)


def load_config_prefix(config):
    return PrinterADS1100(config)
