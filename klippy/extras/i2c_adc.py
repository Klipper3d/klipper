# External ADC via I2C module
#
# Copyright (C) 2021 Dawid Murawski <dawid.m@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# Version Date: 02.10.2021
#
#############################################################
# Compatible ADCs:
#       MCP3421 - MCP3428
#       ADS1013 - ADS1015
#       Tested MCP3421 on Linux MCU
#       Tested ADS1015 on Linux MCU
#############################################################

from . import bus
from collections import deque
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
        if self.gain not in MCP_GAIN and \
            self.gain not in ADC_GAIN:
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
        #Configure ADC
        self.conf = (self.channel, self.gain, self.resolution,
                    self.rate, self.devicePrefix, self.name)

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
        self.report_time = 0
        self.read = 0
        self.minval = self.maxval = self.range_check_count = 0
        self.range_counter = 0
        self.sample_timer = self.sample_count = 0
        self._last_time = 0
        self._callback = None
        self.rValue = None
        self.stack = None
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

    def setup_minmax(self, sample_time, sample_count=1,
                     minval=-100, maxval=100, range_check_count=5):
        self.sample_count = sample_count
        self.range_check_count = range_check_count
        self.minval = minval
        self.maxval = maxval

    def setup_adc_callback(self, report_time, callback):
        if report_time != None:
            self.report_time = report_time
        self._callback = callback

    def get_last_value(self):
    # via Query ADC tempereature_sensor
        return (self.rValue[1], self.rValue[0])

    def write_config(self):
        # Setup ADC and write ADC configuration
        if self.devicePrefix == "MCP":
            _gain = MCP_GAIN[self.gain]
            _rate = MCP_RES[self.resolution]
            self.rate = _rate[0]
            conf = 0b10010000 | self.channel << 5 | _rate[1] << 2 | _gain
            self.i2c.i2c_write([conf])
            self.read = []
        else:
            _gain = ADS_GAIN[self.gain]
            _rate = ADS_RATE[self.rate]
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
        # Check safety condition and raise/reset counter
        if rVolt < self.minval or rVolt > self.maxval:
            self.range_counter += 1
        else:
            self.range_counter = 0
        if self.range_counter == self.range_check_count:
            self.printer.invoke_shutdown(
                "i2c_adc {} voltage {} outside range of {}:{}" \
                .format(self.name, rVolt, self.minval, self.maxval))
        # Calculate running average
        if self.stack == None:
            self.stack = deque([])
            for i in range(self.sample_count):
                self.stack.append(rVolt)
        else:
            self.stack.popleft()
            self.stack.append(rVolt)
        rVolt = sum(self.stack)/self.sample_count
        return (rTime, rVolt)

def load_config_prefix(config):
    return i2c_adc(config)
