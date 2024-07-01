# Support for I2C based ADS1013, ADS1014, ADS1015, ADS1113, ADS1114 and ADS1115
#
# Copyright (C) 2024 Konstantin Koch <korsarnek@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins
from . import bus
from . import adc_temperature

# Supported chip types
ADS1X1X_CHIP_TYPE = {
    'ADS1013': 3,
    'ADS1014': 4,
    'ADS1015': 5,
    'ADS1113': 13,
    'ADS1114': 14,
    'ADS1115': 15
}

def isADS101X(chip):
    return (chip == ADS1X1X_CHIP_TYPE['ADS1013'] \
        or chip == ADS1X1X_CHIP_TYPE['ADS1014'] \
        or chip == ADS1X1X_CHIP_TYPE['ADS1015'])

def isADS111X(chip):
    return (chip == ADS1X1X_CHIP_TYPE['ADS1113'] \
        or chip == ADS1X1X_CHIP_TYPE['ADS1114'] \
        or chip == ADS1X1X_CHIP_TYPE['ADS1115'])

# Address is defined by how the address pin is wired
ADS1X1X_CHIP_ADDR = {
    'GND': 0x48,
    'VCC': 0x49,
    'SDA': 0x4a,
    'SCL': 0x4b
}

# Chip "pointer" registers
ADS1X1X_REG_POINTER_MASK = 0x03
ADS1X1X_REG_POINTER = {
    'CONVERSION': 0x00,
    'CONFIG': 0x01,
    'LO_THRESH': 0x02,
    'HI_THRESH': 0x03
}

# Config register masks
ADS1X1X_REG_CONFIG = {
    'OS_MASK': 0x8000,
    'MULTIPLEXER_MASK': 0x7000,
    'PGA_MASK': 0x0E00,
    'MODE_MASK': 0x0100,
    'DATA_RATE_MASK': 0x00E0,
    'COMPARATOR_MODE_MASK': 0x0010,
    'COMPARATOR_POLARITY_MASK': 0x0008,
    # Determines if ALERT/RDY pin latches once asserted
    'COMPARATOR_LATCHING_MASK': 0x0004,
    'COMPARATOR_QUEUE_MASK': 0x0003
}

# Config register bits
#   Read: bit = 1 when device is not performing a conversion
ADS1X1X_REG_CONFIG_OS_IDLE=0x8000

#
# The following enums are to be used with the configuration functions.
#
ADS1X1X_OS = {
    'OS_SINGLE': 0x8000 # Single-conversion
}

ADS1X1X_MUX = {
    'DIFF01': 0x0000,  # Differential P = AIN0, N = AIN1 0
    'DIFF03': 0x1000,  # Differential P = AIN0, N = AIN3 4096
    'DIFF13': 0x2000,  # Differential P = AIN1, N = AIN3 8192
    'DIFF23': 0x3000,  # Differential P = AIN2, N = AIN3 12288
    'AIN0': 0x4000,  # Single-ended (ADS1015: AIN0 16384)
    'AIN1': 0x5000,  # Single-ended (ADS1015: AIN1 20480)
    'AIN2': 0x6000,  # Single-ended (ADS1015: AIN2 24576)
    'AIN3': 0x7000   # Single-ended (ADS1015: AIN3 28672)
}

ADS1X1X_PGA = {
    '6.144V': 0x0000,  # +/-6.144V range = Gain 2/3
    '4.096V': 0x0200,  # +/-4.096V range = Gain 1
    '2.048V': 0x0400,  # +/-2.048V range = Gain 2
    '1.024V': 0x0600,  # +/-1.024V range = Gain 4
    '0.512V': 0x0800,  # +/-0.512V range = Gain 8
    '0.256V': 0x0A00  # +/-0.256V range = Gain 16
}
ADS111X_PGA_SCALAR = {
    0x0000: 6.144 / 32767.0,  # +/-6.144V range = Gain 2/3
    0x0200: 4.096 / 32767.0,  # +/-4.096V range = Gain 1
    0x0400: 2.048 / 32767.0,  # +/-2.048V range = Gain 2
    0x0600: 1.024 / 32767.0,  # +/-1.024V range = Gain 4
    0x0800: 0.512 / 32767.0,  # +/-0.512V range = Gain 8
    0x0A00: 0.256 / 32767.0  # +/-0.256V range = Gain 16
}
ADS101X_PGA_SCALAR = {
    0x0000: 6.144 / 2047.0,  # +/-6.144V range = Gain 2/3
    0x0200: 4.096 / 2047.0,  # +/-4.096V range = Gain 1
    0x0400: 2.048 / 2047.0,  # +/-2.048V range = Gain 2
    0x0600: 1.024 / 2047.0,  # +/-1.024V range = Gain 4
    0x0800: 0.512 / 2047.0,  # +/-0.512V range = Gain 8
    0x0A00: 0.256 / 2047.0  # +/-0.256V range = Gain 16
}
ADS1X1X_MODE = {
    'continous': 0x0000,  # Continuous conversion mode
    'single': 0x0100  # Power-down single-shot mode
}

# Lesser samples per second means it takes and averages more samples before
# returning a result.
ADS101X_SAMPLES_PER_SECOND = {
    '128': 0x0000,  # 128 samples per second
    '250': 0x0020,  # 250 samples per second
    '490': 0x0040,  # 490 samples per second
    '920': 0x0060,  # 920 samples per second
    '1600': 0x0080,  # 1600 samples per second
    '2400': 0x00a0,  # 2400 samples per second
    '3300': 0x00c0,  # 3300 samples per second
}

ADS111X_SAMPLES_PER_SECOND = {
    '8': 0x0000,  # 8 samples per second
    '16': 0x0020,  # 16 samples per second
    '32': 0x0040,  # 32 samples per second
    '64': 0x0060,  # 64 samples per second
    '128': 0x0080,  # 128 samples per second
    '250': 0x00a0,  # 250 samples per second
    '475': 0x00c0,  # 475 samples per second
    '860': 0x00e0  # 860 samples per second
}

ADS1X1X_COMPARATOR_MODE = {
    'TRADITIONAL': 0x0000,  # Traditional comparator with hysteresis
    'WINDOW': 0x0010  # Window comparator
}

ADS1X1X_COMPARATOR_POLARITY = {
    'ACTIVE_LO': 0x0000,  # ALERT/RDY pin is low when active
    'ACTIVE_HI': 0x0008  # ALERT/RDY pin is high when active
}

ADS1X1X_COMPARATOR_LATCHING = {
    'NON_LATCHING': 0x0000,  # Non-latching comparator
    'LATCHING': 0x0004  # Latching comparator
}

ADS1X1X_COMPARATOR_QUEUE = {
    'QUEUE_1': 0x0000,  # Assert ALERT/RDY after one conversions
    'QUEUE_2': 0x0001,  # Assert ALERT/RDY after two conversions
    'QUEUE_4': 0x0002,  # Assert ALERT/RDY after four conversions
    'QUEUE_NONE': 0x0003  # Disable the comparator and put ALERT/RDY
                        # in high state
}

ADS1X1_OPERATIONS = {
    'SET_MUX': 0,
    'READ_CONVERSION': 1
}

ADS1X1_MAX_COMMAND_QUEUE_SIZE = 1000

class ADS1X1X_chip:

    def __init__(self, config):
        self._printer = config.get_printer()
        self._reactor = self._printer.get_reactor()

        self.name = config.get_name().split()[-1]
        self.chip = config.getchoice('chip', ADS1X1X_CHIP_TYPE)
        address = ADS1X1X_CHIP_ADDR['GND']
        # If none is specified, i2c_address can be used for a specific address
        if config.get('address_pin', None) is not None:
            address = config.getchoice('address_pin', ADS1X1X_CHIP_ADDR)

        if isADS101X(self.chip):
            self.samples_per_second = config.getchoice('samples_per_second',
                ADS101X_SAMPLES_PER_SECOND, '128')
            self.samples_per_second_numeric = config.getint(
                'samples_per_second', 128)
        else:
            self.samples_per_second = config.getchoice('samples_per_second',
                ADS111X_SAMPLES_PER_SECOND, '128')
            self.samples_per_second_numeric = config.getint(
                'samples_per_second', 128)

        self._ppins = self._printer.lookup_object("pins")
        self._ppins.register_chip(self.name, self)

        self.pga = config.getchoice('pga', ADS1X1X_PGA, 'V4.096')
        self.mode = config.getchoice('mode', ADS1X1X_MODE, 'single')
        # Comparators are not implemented, they would only be useful if the
        # alert pin is used, which we haven't made configurable.
        # But that wouldn't be useful for a normal temperature sensor anyway.
        self.comp_mode = ADS1X1X_COMPARATOR_MODE['TRADITIONAL']
        self.comp_polarity = ADS1X1X_COMPARATOR_POLARITY['ACTIVE_LO']
        self.comp_latching = ADS1X1X_COMPARATOR_LATCHING['NON_LATCHING']
        self.comp_queue = ADS1X1X_COMPARATOR_QUEUE['QUEUE_NONE']
        self._i2c = bus.MCU_I2C_from_config(config, address)

        self.adc_voltage = config.getfloat('adc_voltage', 5., above=0.)

        self.mcu = self._i2c.get_mcu()

        self._printer.add_object("ads1x1x " + self.name, self)
        self._printer.register_event_handler("klippy:ready", \
                                            self._handle_ready)
        self._printer.register_event_handler("klippy:shutdown", \
                                            self.reset_all_devices)

        self._pins = {}
        self._command_queue = []

    def setup_pin(self, pin_type, pin_params):
        if pin_type == 'adc':
            pin = ADS1X1X_pin(self, pin_params)
            if pin.pin in self._pins:
                raise pins.error(
                    '%s pin %s for chip %s is used multiple times' \
                        % (self.chip, pin.pin, self.name))
            self._pins[pin.pin] = pin
            return pin
        raise pins.error('Wrong pin or incompatible type: %s with type %s! ' % (
            pin_params['pin'], pin_type))

    def _handle_ready(self):
        self.reset_all_devices()
        self.sample_timer = \
                self._reactor.register_timer(self._sample)
        self._reactor.update_timer(self.sample_timer, self._reactor.NOW)

    def request_sample(self, sensor, callback):
        if len(self._command_queue) <= ADS1X1_MAX_COMMAND_QUEUE_SIZE:
            self._command_queue.append({
                'sensor': sensor,
                'callback': callback
            })
        else:
            logging.warning(
                'ADS1X1X: command queue is overflowing, dropping requests')

    def _sample(self, eventtime):
        if self._command_queue:
            command = self._command_queue.pop()
            pin_object = self._pins[command['sensor'].pin]
            sample = 0
            try:
                self._write_register(ADS1X1X_REG_POINTER['CONFIG'],
                                     pin_object.config)

                self._reactor.pause(self._reactor.monotonic() + 1.0 /
                                    (self.samples_per_second_numeric + 1))

                sample = self._read_register(ADS1X1X_REG_POINTER['CONVERSION'])
            except Exception:
                logging.exception("ADS1X1X: error while sampling")
                return self._reactor.NEVER

            command['callback'](sample)

        return self._reactor.monotonic() + 1.0 / self.samples_per_second_numeric

    def _read_register(self, reg):
        # read a single register
        params = self._i2c.i2c_read([reg], 2)
        buff = bytearray(params['response'])
        return (buff[0]<<8 | buff[1])

    def _write_register(self, reg, data):
        data = [
            (reg & 0xFF), # Control register
            ((data>>8) & 0xFF), # High byte
            (data & 0xFF), # Lo byte
        ]
        self._i2c.i2c_write(data)

    def reset_all_devices(self):
        try:
            # Init all devices on bus for this kind of device
            self._i2c.i2c_write([0x06, 0x00, 0x00])
        except Exception:
            logging.exception("ADS1X1X: error while resetting device")

class ADS1X1X_sensor:
    def __init__(self, config):
        self._printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        sensor_pin = config.get('sensor_pin')

        self.pin = sensor_pin.split(':')[-1]
        self.chip = self._printer.lookup_object("ads1x1x " +
                                                sensor_pin.split(':')[0])

        self.voltage_offset = config.getfloat('voltage_offset', 0.0)
        self.pullup_resistor = config.getfloat('pullup_resistor', 4700.,
            above=0.)
        self.report_time = config.getfloat('report_time'
            , 1.0 / self.chip.samples_per_second_numeric * 4 # 4 possible inputs
            , minval= 1.0 / self.chip.samples_per_second_numeric)

        # Find probe for voltage to temp conversion function/table
        for probe_type, params in adc_temperature.DefaultVoltageSensors:
            if probe_type == config.get('probe_type'):
                self.converter = adc_temperature.LinearVoltage(config, params)
                break
        # Find probe for resistance to temp conversion function/table
        if probe_type != config.get('probe_type'):
            for probe_type, params in \
                adc_temperature.DefaultResistanceSensors:
                if (probe_type == config.get('probe_type')):
                    self.converter = adc_temperature.LinearResistance(config,
                                                                      params)
                    break
        # Attempt to match against existing thermistors, generate conversion
        # table
        if probe_type != config.get('probe_type'):
            pheaters = config.get_printer().load_object(config, "heaters")
            probe_config = \
                pheaters.sensor_factories[config.get('probe_type')](config) \
                    .adc_convert
            all_temps = [float(i) for i in range(1, 351)]
            adcs = [(t, probe_config.calc_adc(t)) for t in all_temps]
            rs = [(t,self.pullup_resistor * adc / (1.0 - adc)) \
                for t, adc in adcs]
            if probe_config is not None:
                self.converter = adc_temperature.LinearResistance(config, rs)
                probe_type = config.get('probe_type')

        if probe_type != config.get('probe_type'):
            logging.error("Probe '%s' not found in adc_temperature for %s" \
                % (config.get('probe_type'), self.name))

        self.temp = self.min_temp = self.max_temp = 0.0

        self._printer.register_event_handler("klippy:connect", \
                                            self._handle_connect)

    def _handle_connect(self):
        self._reactor = self._printer.get_reactor()
        self._sample_timer = \
                self._reactor.register_timer(self._timer)
        self._reactor.update_timer(self._sample_timer, self._reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def _timer(self, eventtime):
        self.chip.request_sample(self, self._process_sample)
        return self._reactor.monotonic() + self.report_time

    def _process_sample(self, sample):
        # The sample is encoded in the top 12 or full 16 bits
        # Value's meaning is defined by ADS1X1X_REG_CONFIG['PGA_MASK']
        if isADS101X(self.chip.chip):
            sample >>= 4
            volts = sample * ADS101X_PGA_SCALAR[self.chip.pga]
        else:
            volts = sample * ADS111X_PGA_SCALAR[self.chip.pga]
        self.temp = self.converter.calc_temp( (volts - self.voltage_offset) \
            / self.chip.adc_voltage )

        # Check if result is within limits
        if self.temp < self.min_temp or self.temp > self.max_temp:
            self._printer.invoke_shutdown(
                "ADS1X1X temperature %0.1f outside range of %0.1f:%.01f"
                % (self.temp, self.min_temp, self.max_temp))

        # Publish result
        measured_time = self._reactor.monotonic()
        self._callback(self.chip.mcu.estimated_print_time(measured_time),
                       self.temp)

        return measured_time + self.report_time

    def get_report_time_delta(self):
        return self.report_time

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
        }


class ADS1X1X_pin:
    def __init__(self, chip, pin_params):
        self.mcu = chip.mcu
        self.pin = pin_params['pin']

        if (self.pin not in ADS1X1X_MUX):
            raise pins.error('ADS1x1x pin %s is not valid' % self.pin)

        self.mux = ADS1X1X_MUX[self.pin]
        # Set up 2-byte configuration that will be used with each request
        self.config = 0
        self.config |= (ADS1X1X_OS['OS_SINGLE'] \
            & ADS1X1X_REG_CONFIG['OS_MASK'])
        self.config |= (self.mux & ADS1X1X_REG_CONFIG['MULTIPLEXER_MASK'])
        self.config |= (chip.pga & ADS1X1X_REG_CONFIG['PGA_MASK'])
        self.config |= (chip.mode & ADS1X1X_REG_CONFIG['MODE_MASK'])
        self.config |= (chip.samples_per_second & \
                        ADS1X1X_REG_CONFIG['DATA_RATE_MASK'])
        self.config |= (chip.comp_mode \
            & ADS1X1X_REG_CONFIG['COMPARATOR_MODE_MASK'])
        self.config |= (chip.comp_polarity \
            & ADS1X1X_REG_CONFIG['COMPARATOR_POLARITY_MASK'])
        self.config |= (chip.comp_latching \
            & ADS1X1X_REG_CONFIG['COMPARATOR_LATCHING_MASK'])
        self.config |= (chip.comp_queue \
            & ADS1X1X_REG_CONFIG['COMPARATOR_QUEUE_MASK'])

    def get_mcu(self):
        return self.mcu

    def setup_adc_callback(self, report_time, callback):
        self.report_time = report_time
        self.callback = callback

def load_config_prefix(config):
    return ADS1X1X_chip(config)

def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory('ads1x1x', ADS1X1X_sensor)
