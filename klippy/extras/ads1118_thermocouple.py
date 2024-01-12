# Support for multiple type k thermocouples on an ADS1118 adc
#
# Copyright (C) 2022  Jacob Dockter <dockterj@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import pins
from . import bus

REPORT_TIME = 0.300
MAX_INVALID_COUNT = 3

######################################################################
# ADS1118_Thermocouple
# This class implements the secondary devices that respond to
# messages from the mcu and provide temperature data to the
# heaters module.
######################################################################

ADS1118_MULT = .0078125
ADS1118_CJ_MULT = .03125

ADS1118_COLD_JUNCTION_HIGH_FAULT = 0x01
ADS1118_COLD_JUNCTION_LOW_FAULT = 0x02
ADS1118_CONFIG_READ_ERROR = 0x04

class ADS1118_Thermocouple(object):
    def __init__(self, config):
        self._printer = config.get_printer()
        self._callback = None
        self.min_sample_value = self.max_sample_value = 0
        self._report_clock = 0
        self._type = config.get("sensor_type").split("_")[1]
        self._pin = config.get("sensor_pin").split(":")[1][4:5]
        pins = config.get_printer().lookup_object("pins").lookup_pin(
                                              config.get("sensor_pin"))
        self._parent = pins['chip']
        self._mcu = self._parent.get_mcu()
        self._oid = self._mcu.create_oid()
        self._mcu.register_response(self._handle_spi_response,
                              "ads1118_result", self._oid)
        self._mcu.register_config_callback(self._build_config)
    def setup_minmax(self, min_temp, max_temp):
        adc_range = [self.calc_adc(min_temp), self.calc_adc(max_temp)]
        self.min_sample_value = min(adc_range)
        self.max_sample_value = max(adc_range)
    def setup_callback(self, cb):
        self._callback = cb
    def get_report_time_delta(self):
        return REPORT_TIME
    def _build_config(self):
        self._parent.config_channel(self._oid, self._pin, self.min_sample_value,
            self.max_sample_value)
    def _handle_spi_response(self, params):
        if params['fault']:
            self.handle_fault(params['adc_mv'], params['cj_temp'],
                params['fault'])
            return
        if self._type == "typek":
            temp = self.calc_temp(params['adc_mv'], params['cj_temp'],
                                  params['fault'])
        next_clock      = self._mcu.clock32_to_clock64(params['next_clock'])
        last_read_clock = next_clock - self._report_clock
        last_read_time  = self._mcu.clock_to_print_time(last_read_clock)
        self._callback(last_read_time, temp)
    def report_fault(self, msg):
        logging.warn(msg)
    def handle_fault(self, adc_mv, cj_temp, fault):
        if fault & ADS1118_COLD_JUNCTION_HIGH_FAULT:
            self.report_fault("ADS1118: Cold Junction High Fault")
        if fault & ADS1118_COLD_JUNCTION_LOW_FAULT:
            self.report_fault("ADS1118: Cold Junction Low Fault")
        if fault & ADS1118_CONFIG_READ_ERROR:
            self.report_fault("ADS1118: Config register readback is incorrect")
    def calc_temp(self, adc_mv, cj_temp, fault):
        try:
            adc_mv = adc_mv * ADS1118_MULT
            if self._type == "typek":
                temp = mv_to_typek( typek_to_mv(cj_temp * ADS1118_CJ_MULT)
                       + adc_mv)
            return temp
        except ValueError as err:
            self.report_fault(err)
    def calc_adc(self, temp):
        try:
            # determine the unscaled adc value for a particular temperature
            # to use for min/max checking.  We don't want to have to use
            # floating point on the mcu. Assume cold junction is at room
            # temperature.
            adc = (typek_to_mv(temp) - typek_to_mv(25)) / ADS1118_MULT
            return adc
        except ValueError as err:
            self.report_fault(err)

######################################################################
# Sensor registration
######################################################################

Sensors = {
    "ads1118_typek": ADS1118_Thermocouple,
}

def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    for name, klass in Sensors.items():
        pheaters.add_sensor_factory(name, klass)

######################################################################
# Type K thermocouple conversions
# based on GPLv3 code from:
# github.com/wjasper/Linux_Drivers/blob/master/USB/python/thermocouple.py
######################################################################

def typek_to_mv(degc):
    tab1 = [
        0.000000000000E+00,
        0.394501280250E-01,
        0.236223735980E-04,
        -0.328589067840E-06,
        -0.499048287770E-08,
        -0.675090591730E-10,
        -0.574103274280E-12,
        -0.310888728940E-14,
        -0.104516093650E-16,
        -0.198892668780E-19,
        -0.163226974860E-22,
    ]

    tab2 = [
        -0.176004136860E-01,
        0.389212049750E-01,
        0.185587700320E-04,
        -0.994575928740E-07,
        0.318409457190E-09,
        -0.560728448890E-12,
        0.560750590590E-15,
        -0.320207200030E-18,
        0.971511471520E-22,
        -0.121047212750E-25,
    ]

    a0 = 0.118597600000E+00
    a1 = -0.118343200000E-03
    a2 = 0.126968600000E+03

    if -270 <= degc <= 0:
        c = tab1
    elif 0 < degc <= 1372:
        c = tab2
    else:
        raise ValueError("Temperature specified is out of range " +
                          "for Type K thermocouple")

    e = 0
    for p in range(0, len(c)):
        e += c[p] * math.pow(degc, p)

    if degc > 0:
        e += a0 * math.exp(a1 * math.pow(degc - a2, 2))

    return e

def mv_to_typek(mv):
    tab1 = [
        0.0000000E+00,
        2.5173462E+01,
        -1.1662878E+00,
        -1.0833638E+00,
        -8.9773540E-01,
        -3.7342377E-01,
        -8.6632643E-02,
        -1.0450598E-02,
        -5.1920577E-04,
        0.0000000E+00,
    ]

    tab2 = [
        0.000000E+00,
        2.508355E+01,
        7.860106E-02,
        -2.503131E-01,
        8.315270E-02,
        -1.228034E-02,
        9.804036E-04,
        -4.413030E-05,
        1.057734E-06,
        -1.052755E-08,
    ]

    tab3 = [
        -1.318058E+02,
        4.830222E+01,
        -1.646031E+00,
        5.464731E-02,
        -9.650715E-04,
        8.802193E-06,
        -3.110810E-08,
        0.000000E+00,
        0.000000E+00,
        0.000000E+00,
    ]

    if -5.891 <= mv <= 0.0:
        c = tab1
    elif 0.0 < mv <= 20.644:
        c = tab2
    elif 20.644 < mv <= 54.886:
        c = tab3
    else:
        raise ValueError("Voltage specified is out of range for " +
                          "Type K thermocouple")

    t = 0.0
    for p in range(0, len(c)):
        t += c[p] * math.pow(mv, p)

    return t
