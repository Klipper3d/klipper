# Support for multiple type k thermocouples on an ADS1118 adc
#
# Copyright (C) 2022  Jacob Dockter <dockterj@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import math
from . import bus


######################################################################
# SensorBase
######################################################################

REPORT_TIME = 0.300

class SensorBase:
    def __init__(self, config, chip_type):
        self.printer = config.get_printer()
        self.chip_type = chip_type
        self._callback = None
        self.min_sample_value = self.max_sample_value = 0
        self._report_clock = 0
    def setup_minmax(self, min_temp, max_temp):
        adc_range = [self.calc_adc(min_temp), self.calc_adc(max_temp)]
        self.min_sample_value = min(adc_range)
        self.max_sample_value = max(adc_range)
        logging.debug("min_sample_value  %u", min(adc_range))
    def setup_callback(self, cb):
        self._callback = cb
    def get_report_time_delta(self):
        return REPORT_TIME
    def _build_config(self):
        self.mcu.add_config_cmd(
            "config_ads1118 oid=%u spi_oid=%u thermocouple_type=%s" % (
                self.oid, self.spi.get_oid(), self.chip_type))
        clock = self.mcu.get_query_slot(self.oid)
        self._report_clock = self.mcu.seconds_to_clock(REPORT_TIME)
        self.mcu.add_config_cmd(
            "query_ads1118 oid=%u clock=%u rest_ticks=%u"
            " min_value=%u max_value=%u" % (
                self.oid, clock, self._report_clock,
                self.min_sample_value, self.max_sample_value), is_init=True)
    def _handle_spi_response(self, params):
        temp = self.calc_temp_ads1118(params['value'], params['value2'],
                                      params['fault'])
        next_clock      = self.mcu.clock32_to_clock64(params['next_clock'])
        last_read_clock = next_clock - self._report_clock
        last_read_time  = self.mcu.clock_to_print_time(last_read_clock)
        self._callback(last_read_time, temp)
    def fault(self, msg):
        self.printer.invoke_async_shutdown(msg)


######################################################################
# ADS1118A
######################################################################

ADS1118_MULT = .0078125

class ADS1118(SensorBase):
    def __init__(self, config, chip_type):
        SensorBase.__init__(self, config, chip_type)
    def calc_temp_ads1118(self, adc, cold_junction, fault):
        #logging.debug("calc temp called")
        #logging.debug("adc %u", adc)
        #logging.debug("cold_junction %u", cold_junction)
        # Fix sign bit:
        if adc & 0x8000:
            adc = adc - (1 << 16)
        #logging.debug("adc1 after fixing sign bit %u", adc)
        # Convert to mv
        adc = adc * ADS1118_MULT
        #logging.debug("adc in mv %.3f", adc)
        temp = mv_to_typek( typek_to_mv(cold_junction * .03125) + adc)
        #logging.debug("temp = %.2f", temp)
        return temp
    def calc_adc(self, temp):
        # assume cold junction is at room temperature
        temp = typek_to_mv(temp) - typek_to_mv(20)
        return temp

class ADS1118A(ADS1118):
    def __init__(self, config):
        ADS1118.__init__(self, config, "ADS1118A")
        self.spi = bus.MCU_SPI_from_config(
            config, 1, pin_option="sensor_pin", default_speed=4000000)
        self.mcu = mcu = self.spi.get_mcu()
        # Reader chip configuration
        self.oid = oid = mcu.create_oid()
        mcu.register_response(self._handle_spi_response,
                              "ads1118_result", oid)
        mcu.register_config_callback(self._build_config)

class ADS1118B(ADS1118):
    def __init__(self, config):
        ADS1118.__init__(self, config, "ADS1118B")
        pins = config.get_printer().lookup_object("pins").lookup_pin(
                                              config.get("sensor_pin"))
        self.mcu = mcu = pins['chip']
        # Reader chip configuration
        self.oid = oid = mcu.create_oid()
        mcu.register_response(self._handle_spi_response,
                              "ads1118_result", oid)
        mcu.register_config_callback(self._build_config)
    def _build_config(self):
        self.mcu.add_config_cmd(
            "config_ads1118 oid=%u spi_oid=%u thermocouple_type=%s" % (
                self.oid, 0, self.chip_type))
        clock = self.mcu.get_query_slot(self.oid)
        self._report_clock = self.mcu.seconds_to_clock(REPORT_TIME)
        self.mcu.add_config_cmd(
            "query_ads1118 oid=%u clock=%u rest_ticks=%u"
            " min_value=%u max_value=%u" % (
                self.oid, clock, self._report_clock,
                self.min_sample_value, self.max_sample_value), is_init=True)

######################################################################
# Sensor registration
######################################################################

Sensors = {
    "ADS1118A": ADS1118A,
    "ADS1118B": ADS1118B,
}

def load_config(config):
    # Register sensors
    pheaters = config.get_printer().load_object(config, "heaters")
    for name, klass in Sensors.items():
        pheaters.add_sensor_factory(name, klass)

def typek_to_mv(degc):
    """
    """
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
    """
    """
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
