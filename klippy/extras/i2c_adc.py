# External ADC via I2C module
#
# Copyright (C) 2021 Dawid Murawski <dawid.m@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# Version Date: 11.09.2021
#
#############################################################
# Compatible ADCs:
#       MCP3421 - MCP3428
#       ADS1013 - ADS1015
#       Tested MCP3421 on Linux MCU
#       Tested ADS1015 on Linux MCU
#############################################################

#add to printer.cfg:
#
#[i2c_adc external_adc_name]
#i2c_address: 104
##  currently, only rpi works as mcu
#i2c_mcu: rpi
##  The i2c address that the chip is using on the i2c bus. This
##  parameter must be provided.
#i2c_bus: i2c.1
#sensor_ID: e.g. ADS1015
## (Optional config: see device manual)
#resolution: (default = 12)
#gain: (default = 1)
#channel: (default = 1 (channel 0))
#rate: (default = 1600 for ADS101x, none for MCP342x)
#report_time: (default 2 readings/second)

# a virtual adc chip is created. 
# The adc pin can be accessed as "external_adc_name:"
# e.g. pin: external_adc_name:

#Standard Adresses are:
# MCP3421: 104 (hex: 0x68)
# ADS1015: 72 (hex: 0x48)

## ADS10XX: ##
# Second Byte: Address pointer
# 0 0 0 0 0 0 P1 P2
#             0  0: Conversion register
#             0  1: Config register
#             1  0: Lo_threshold
#             1  1: Hi_threshold

# Config register
# 0  0  0  0  0  0  0  0
# 15 14 13 12 11 10 9  8
# OS [  MUX ] [ PGA  ] Mode
# ----------------------
# 0  0  0  0  0  0  0  0
# 7  6  5  4  3  2  1  0
# [  DR ] CM  CP CL [CQ]
#
# OS: Operational Status (write 1 for single shot)
# MUX:                  000: AIN0+AIN1 (def)
# Input Multiplexer     001: AIN0+AIN3
# (ADS1015 only)        010: AIN1+AIN3
#                       011: AIN2+AIN3
#                       100: AIN0+GND
#                       101: AIN1+GND
#                       110: AIN2+GND
#                       111: AIN3+GND
# PGA:                  000: FSR = +- 6.144 V (Gain 0.25)
# No function on        001: FSR = +- 4.096 V (Gain 0.5)
# ADS1013               010: FSR = +- 2.048 V (def)
#                       011: FSR = +- 1.024 V (Gain 2)
#                       100: FSR = +- 0.512 V (Gain 4)
#                       101: FSR = +- 0.256 V (Gain 8)
#                       110: FSR = +- 0.256 V
#                       111: FSR = +- 0.256 V
# Mode:                 0: continuous conversion
#                       1: single shot
# DR:                    000: 128 SPS
# Data rate             001: 250 SPS
#                       010: 490 SPS
#                       011: 920 SPS
#                       100: 1600 SPS (def)
#                       101: 2400 SPS
#                       110: 3300 SPS
#                       111: 3300 SPS
## MCP34XX: ##
# 7th bit: OS (1 start new conversion, no function in continuous)
# 6+5th bit: address bits (not used in mcp3425)
# 4th bit: mode (0: one shot mode, 1: continuous)
# 3rd+2nd bit: sample rate (10: 15 ms, 16 bit)
# 1st+0th bit: gain selection (00: gain = 1)


from . import bus
import pins
import mcu
import logging

SUPP_DEV_18 = ['MCP3421', 'MCP3422', 'MCP3423', 'MCP3424']
SUPP_DEV_16 = ['MCP3425', 'MCP3426', 'MCP3427', 'MCP3428']
SUPP_DEV_12 = ['ADS1013', 'ADS1014', 'ADS1015']

VREF = 2.048

N_CHANNELS = {
    'MCP3421': 1,
    'MCP3422': 2,
    'MCP3423': 3,
    'MCP3424': 4,
    'MCP3425': 1,
    'MCP3426': 2,
    'MCP3427': 3,
    'MCP3428': 4,
    'ADS1013': 1,
    'ADS1014': 1,
    'ADS1015': 8,
}

# ADS1013 does not have PGA
ADS_GAIN = {
    0.25: 0,
    0.5: 1,
    1: 2,
    2: 3,
    4: 4,
    8: 5
}

MCP_GAIN = {
    1: 0,
    2: 1,
    4: 2,
    8: 3
}

MCP_RATE = {
    240: (12,0),
    60: (14,1),
    15: (16,2),
    3.75: (18,3)
}

MCP_RES = {
    12: (240,0),
    14: (60,1),
    16: (15,2),
    18: (3.75,3)
}

MCP_CHANNEL = {
    1: 0b00000000,
    2: 0b00100000,
    3: 0b01000000,
    4: 0b01100000
}

ADS_RATE = {
    128: 0,
    250: 1,
    490: 2,
    920: 3,
    1600: 4,
    2400: 5,
    3300: 6,
    3300: 7
}

DEFAULT_RATE = {
    'MCP': 240,
    'ADS': 1600,
}

class i2c_adc:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]
        self.i2c = bus.MCU_I2C_from_config(config,
                    default_speed=100000)
        self.mcu = self.i2c.get_mcu()
        # Register Chip
        ppins = self.printer.lookup_object('pins')
        ppins.register_chip(self.name, self)
        self.reportTime = config.getfloat('report_time',2)
        self.deviceId = config.get('sensor_ID').upper()
        # Check if device is supported
        if self.deviceId not in SUPP_DEV_18 and \
                self.deviceId not in SUPP_DEV_16 and \
                self.deviceId not in SUPP_DEV_12:
            raise config.error(self.deviceId + " not supported")
        self.devicePrefix = self.deviceId[0:3]
        self.channel = config.getint('channel',minval=1,
                            maxval=N_CHANNELS[self.deviceId],
                            default=1)-1
        self.gain = config.get('gain',1)
        # Check if gain is supported
        if self.gain not in eval(self.devicePrefix + "_GAIN"):
            raise config.error("Invalid PGA setting")
        self.resolution = config.getint('resolution',12)
        #Check if selected resolution is correct/supported
        if self.resolution not in [12, 14, 16, 18]:
            raise config.error("Invalid resolution")
        elif self.resolution == 18 and \
                self.deviceId not in SUPP_DEV_18:
            raise config.error("18 bit sampling not supported by " +
                            self.deviceId)
        elif self.resolution != 12 and \
                self.deviceId in SUPP_DEV_12:
            raise config.error(self.deviceId +
                " only supports 12 bit sampling")
        self.rate = config.getint('rate',
                DEFAULT_RATE[self.devicePrefix])
        # Register ADC
        #self.qadc = self.printer.lookup_object('query_adc')
        #self.qadc = config.get_printer().load_object(config,
                             #'query_adc')
        # query_adc = config.get_printer().load_object(config,
                             # 'query_adc')
        # query_adc.register_adc(self.name, self)
        #Configure ADC
        self.conf = (self.channel, self.gain, self.resolution,
                    self.rate, self.devicePrefix, self.name,
                    self.reportTime)

    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'adc':
            raise self.printer.config_error(
                    "I2C_ADC only supports adc pins")
        return ADC_sample(self.printer, self.i2c, self.conf)

class ADC_sample:
    def __init__(self, printer, i2c, conf):
        self.printer = printer
        self.reactor = printer.get_reactor()
        self.i2c = i2c
        self.channel = conf[0]
        self.gain = conf[1]
        self.resolution = conf[2]
        self.rate = conf[3]
        self.devicePrefix = conf[4]
        self.name = conf[5]
        self.report_time = conf[6]
        self.read = 0
        self.sample_timer = 0
        self._last_time = 0
        self._callback = None
        self.rValue = None
        self.LSB = (VREF*2)/(2**self.resolution)
        ppins = self.printer.lookup_object('pins')
        self.adc = ppins.setup_pin('adc', self.name)
        self.printer.register_event_handler("klippy:connect",
                          self.handle_connect)

    def handle_connect(self):
        self.write_config()
        self.sample_timer = self.reactor.register_timer(
            self._sample_timer, self.reactor.NOW)
        logging.info('I2C_ADC {} connected. Channel {}'.format(
            self.name, self.channel + 1))

    def _sample_timer(self, eventtime):
        self.rValue = self.read_voltage()
        self._last_time = eventtime
        self.reactor.pause(self._last_time + self.report_time)
        self._callback(self.rValue[0], self.rValue[1])
        return self._last_time + self.report_time + 0.0001

    def setup_minmax(self, min_temp, max_temp,
                     minval=0., maxval=1000, range_check_count=0):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_adc_callback(self, report_time, callback):
        if report_time is not None:
            self.report_time = report_time
        self._callback = callback

    def get_last_value(self):
    # via Query ADC tempereature_sensor
        return (self.rValue[1], self.rValue[0])

    def write_config(self):
        _gain = eval(self.devicePrefix + "_GAIN")[self.gain]
        _rate = eval(self.devicePrefix + "_RATE")[self.rate]
        # Setup ADC and write ADC configuration
        if self.devicePrefix == "MCP":
            _rate = MCP_RES[resolution]
            self.rate = _rate[0]
            conf = 0b10010000 | self.channel << 5 | _rate[1] << 2 | _gain
            self.i2c.i2c_write([conf])
            self.read = []
        else:
            _conf1 = 0b00000000 | int(self.channel) << 4 | _gain << 1
            _conf2 = 0b00000011 | _rate << 5
            conf = [0b00000001, _conf1, _conf2]
            self.read = [0b00000000]
        self.i2c.i2c_write(bytearray(conf))

    def read_voltage(self):
        # Read 3 bytes of data
        params = self.i2c.i2c_read(self.read, 3)
        response = bytearray(params['response'])
        # Calculate response according to
        # 12, 14, 16 or 18 bit resolution
        if self.devicePrefix == 'MCP':
            if self.resolution == 16:
                value = response[0] << 8 | response[1]
            elif self.resolution < 16:
                value = response[0] << 8 | response[1]
                #Cut repeating leading bits
                value &= ((2 ** resolution)-1)
            else:
                value = response[2] << 16 | response[1] << 8 \
                        | response[2]
                value &= ((2 ** self.resolution)-1)
        else:
            if self.resolution < 18:
                value = response[0] << (self.resolution - 8) | response[1]
            else:
                value = response[0] << (self.resolution - 8) \
                    | response [1] << (self.resolution - 16) | response[2]
        # Check sign
        if (value & 1 << (self.resolution - 1)) != 0:
            value -= ((1 << (self.resolution - 1)))
        # calculate Voltage
        rVolt = value * self.LSB / self.gain
        rTime = params['#receive_time']
        return (rTime, rVolt)

def load_config_prefix(config):
    return i2c_adc(config)
