# Printer heater support
#
# Copyright (C) 2018  Petri Honkala <cruwaller@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
from sensorbase import SensorBase

MAX31865_CONFIG_REG            = 0x00
MAX31865_RTDMSB_REG            = 0x01
MAX31865_RTDLSB_REG            = 0x02
MAX31865_HFAULTMSB_REG         = 0x03
MAX31865_HFAULTLSB_REG         = 0x04
MAX31865_LFAULTMSB_REG         = 0x05
MAX31865_LFAULTLSB_REG         = 0x06
MAX31865_FAULTSTAT_REG         = 0x07

MAX31865_CONFIG_BIAS           = 0x80
MAX31865_CONFIG_MODEAUTO       = 0x40
MAX31865_CONFIG_1SHOT          = 0x20
MAX31865_CONFIG_3WIRE          = 0x10
MAX31865_CONFIG_FAULTCLEAR     = 0x02
MAX31865_CONFIG_FILT50HZ       = 0x01

MAX31865_FAULT_HIGHTHRESH      = 0x80
MAX31865_FAULT_LOWTHRESH       = 0x40
MAX31865_FAULT_REFINLOW        = 0x20
MAX31865_FAULT_REFINHIGH       = 0x10
MAX31865_FAULT_RTDINLOW        = 0x08
MAX31865_FAULT_OVUV            = 0x04

VAL_A = 0.00390830
VAL_B = 0.0000005775
VAL_C = -0.00000000000418301
VAL_ADC_MAX = 32768.0 # 2^15

class RTD(SensorBase):
    def __init__(self, config, params):
        self.chip_type = config.get('sensor_type')
        self.rtd_nominal_r = config.getint('rtd_nominal_r', 100)
        self.reference_r = config.getfloat('rtd_reference_r', 430., above=0.)
        self.num_wires  = config.getint('rtd_num_of_wires', 2)
        self.use_50Hz_filter = config.getboolean('rtd_use_50Hz_filter', False)
        SensorBase.__init__(self, config, is_spi = True, sample_count = 1)
    def check_faults(self, fault):
        if fault & 0x80:
            raise self.error("Max31865 RTD input is disconnected")
        if fault & 0x40:
            raise self.error("Max31865 RTD input is shorted")
        if fault & 0x20:
            raise self.error("Max31865 VREF- is greater than 0.85 * VBIAS, FORCE- open")
        if fault & 0x10:
            raise self.error("Max31865 VREF- is less than 0.85 * VBIAS, FORCE- open")
        if fault & 0x08:
            raise self.error("Max31865 VRTD- is less than 0.85 * VBIAS, FORCE- open")
        if fault & 0x04:
            raise self.error("Max31865 Overvoltage or undervoltage fault")
        if fault & 0x03:
            raise self.error("Max31865 Unspecified error")
    def calc_temp(self, adc):
        adc = adc >> 1 # remove fault bit
        R_rtd = (self.reference_r * adc) / VAL_ADC_MAX
        temp = (
            (( ( -1 * self.rtd_nominal_r ) * VAL_A ) +
             math.sqrt( ( self.rtd_nominal_r * self.rtd_nominal_r * VAL_A * VAL_A ) -
                        ( 4 * self.rtd_nominal_r * VAL_B * ( self.rtd_nominal_r - R_rtd ) )))
            / (2 * self.rtd_nominal_r * VAL_B))
        return temp
    def calc_adc(self, temp):
        R_rtd = temp * ( 2 * self.rtd_nominal_r * VAL_B )
        R_rtd = math.pow( ( R_rtd + ( self.rtd_nominal_r * VAL_A ) ), 2)
        R_rtd = -1 * ( R_rtd - ( self.rtd_nominal_r * self.rtd_nominal_r * VAL_A * VAL_A ) )
        R_rtd = R_rtd / ( 4 * self.rtd_nominal_r * VAL_B )
        R_rtd = ( -1 * R_rtd ) + self.rtd_nominal_r
        adc = int ( ( ( R_rtd * VAL_ADC_MAX ) / self.reference_r) + 0.5 )
        adc = adc << 1 # Add fault bit
        return adc
    def get_configs(self):
        value = (MAX31865_CONFIG_BIAS |
                 MAX31865_CONFIG_MODEAUTO |
                 MAX31865_CONFIG_FAULTCLEAR)
        if self.use_50Hz_filter:
            value |= MAX31865_CONFIG_FILT50HZ
        if self.num_wires == 3:
            value |= MAX31865_CONFIG_3WIRE
        cmd = 0x80 + MAX31865_CONFIG_REG
        return [cmd, value]
