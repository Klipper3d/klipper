# Support for I2C based ADS1013, ADS1014, ADS1015, ADS1113
#   , ADS1114, ADS1115 sensors
#
# Copyright (C) 2022  Kevin Peck <kevindpeck@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
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
    'MUX_DIFF_0_1': 0x0000,  # Differential P = AIN0, N = AIN1 0
    'MUX_DIFF_0_3': 0x1000,  # Differential P = AIN0, N = AIN3 4096
    'MUX_DIFF_1_3': 0x2000,  # Differential P = AIN1, N = AIN3 8192
    'MUX_DIFF_2_3': 0x3000,  # Differential P = AIN2, N = AIN3 12288
    'MUX_SINGLE_0': 0x4000,  # Single-ended (ADS1015: AIN0 16384)
    'MUX_SINGLE_1': 0x5000,  # Single-ended (ADS1015: AIN1 20480)
    'MUX_SINGLE_2': 0x6000,  # Single-ended (ADS1015: AIN2 24576)
    'MUX_SINGLE_3': 0x7000   # Single-ended (ADS1015: AIN3 28672)
}

ADS1X1X_PGA = {
    'PGA_6144': 0x0000,  # +/-6.144V range = Gain 2/3
    'PGA_4096': 0x0200,  # +/-4.096V range = Gain 1
    'PGA_2048': 0x0400,  # +/-2.048V range = Gain 2
    'PGA_1024': 0x0600,  # +/-1.024V range = Gain 4
    'PGA_512': 0x0800,  # +/-0.512V range = Gain 8
    'PGA_256': 0x0A00  # +/-0.256V range = Gain 16
}
ADS1X1X_PGA_SCALAR16 = {
    0x0000: 6.144 / 32767.0,  # +/-6.144V range = Gain 2/3
    0x0200: 4.096 / 32767.0,  # +/-4.096V range = Gain 1
    0x0400: 2.048 / 32767.0,  # +/-2.048V range = Gain 2
    0x0600: 1.024 / 32767.0,  # +/-1.024V range = Gain 4
    0x0800: 0.512 / 32767.0,  # +/-0.512V range = Gain 8
    0x0A00: 0.256 / 32767.0  # +/-0.256V range = Gain 16
}
ADS1X1X_PGA_SCALAR12 = {
    0x0000: 6.144 / 2047.0,  # +/-6.144V range = Gain 2/3
    0x0200: 4.096 / 2047.0,  # +/-4.096V range = Gain 1
    0x0400: 2.048 / 2047.0,  # +/-2.048V range = Gain 2
    0x0600: 1.024 / 2047.0,  # +/-1.024V range = Gain 4
    0x0800: 0.512 / 2047.0,  # +/-0.512V range = Gain 8
    0x0A00: 0.256 / 2047.0  # +/-0.256V range = Gain 16
}
ADS1X1X_MODE = {
    'MODE_CONTINUOUS': 0x0000,  # Continuous conversion mode
    'MODE_SINGLE_SHOT': 0x0100  # Power-down single-shot mode
}

ADS1X1X_I2C_SPEED = {
    # ADS101x
    'ADS101x_128': 0x0000,  # 128 samples per second
    'ADS101x_250': 0x0020,  # 250 samples per second
    'ADS101x_490': 0x0040,  # 490 samples per second
    'ADS101x_920': 0x0060,  # 920 samples per second
    'ADS101x_1600': 0x0080,  # 1600 samples per second
    'ADS101x_2400': 0x00a0,  # 2400 samples per second
    'ADS101x_3300': 0x00c0,  # 3300 samples per second
    # ADS111x
    'ADS111x_8': 0x0000,  # 8 samples per second
    'ADS111x_16': 0x0020,  # 16 samples per second
    'ADS111x_32': 0x0040,  # 32 samples per second
    'ADS111x_64': 0x0060,  # 64 samples per second
    'ADS111x_128': 0x0080,  # 128 samples per second
    'ADS111x_250': 0x00a0,  # 250 samples per second
    'ADS111x_475': 0x00c0,  # 475 samples per second
    'ADS111x_860': 0x00e0  # 860 samples per second
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

class Node:
    def __init__(self,data):
        self.data = data
        self.next = None

class CircularLinkedList:
    def __init__(self):
        self.last = None
    def add(self,data):
        newNode = Node(data)
        if self.last == None:
            # First item in new list
            self.last = newNode
            # create link to iteself
            self.last.next = self.last
            return self.last
        else:
            # store the address of the current first node in the newNode
            newNode.next = self.last.next
        # make newNode as last
        self.last.next = newNode


ADS1X1X_REPORT_TIME = 800E-3
# Temperature can be sampled at any time. Conversion is
# very fast but each port shares the same conversion
# register.  Select port, call for conversion, get result.
ADS1X1X_MIN_REPORT_TIME = 4E-3
I2C_SPEED = 400E3
class ADS1X1X:
    # Class variables
    reactor = None
    i2c = {} # {self.chip_addr: bus.MCU_I2C_from_config(), ... up to 4 devices}
    sensor_instances = CircularLinkedList()
    sample_timer = None
    report_time = None
    current_sensor = None
    current_operation = None

    @classmethod
    def _sample_ads1x1(cls, eventtime):
        # Initialize
        measured_time = None
        if(cls.current_sensor is None):
            cls.current_sensor = cls.sensor_instances.last
        if(cls.current_operation is None):
            cls.current_operation = ADS1X1_OPERATIONS['SET_MUX']
        # If sensor is allocated
        if(cls.current_sensor is not None):
            # State machine
            if(cls.current_operation == ADS1X1_OPERATIONS['SET_MUX']):
                # Set channel on current sensor, initiate conversion
                try:
                    cls.write_register(cls.current_sensor.data.chip_addr\
                        ,ADS1X1X_REG_POINTER['CONFIG']\
                        ,cls.current_sensor.data.config)
                except Exception:
                    logging.exception("ads1x1x: Error triggering mux")
                    cls.current_sensor.data.temp = 0.0
                    return cls.reactor.NEVER
                # Set next operation
                cls.current_operation = ADS1X1_OPERATIONS['READ_CONVERSION']
            else:
                if(cls.current_operation == \
                    ADS1X1_OPERATIONS['READ_CONVERSION']):
                    # Read result
                    try:
                        sample = \
                            cls.read_register( \
                                cls.current_sensor.data.chip_addr \
                                ,ADS1X1X_REG_POINTER['CONVERSION'], 2)
                    except Exception:
                        logging.exception("ads1x1x: Error reading data")
                        cls.current_sensor.data.temp = 0.0
                        return cls.reactor.NEVER
                    # Calc temp
                    cls.current_sensor.data.temp = \
                        cls.current_sensor.data.degrees_from_sample(sample)
                    # Publish result
                    measured_time = cls.reactor.monotonic()
                    cls.current_sensor.data._callback( \
                        cls.current_sensor.data.mcu.estimated_print_time( \
                            measured_time), \
                            cls.current_sensor.data.temp)
                    # Move to next sensor
                    cls.current_sensor = cls.current_sensor.next
                    # Set next operation
                    cls.current_operation = ADS1X1_OPERATIONS['SET_MUX']

        # Set next execution time
        if(measured_time is None):
            measured_time = cls.reactor.monotonic()
        return measured_time + cls.report_time / len(ADS1X1_OPERATIONS)

    @classmethod
    def read_register(cls, chip_addr, reg, read_len):
        # read a single register
        params = ADS1X1X.i2c[chip_addr].i2c_read([reg], read_len)
        buff = bytearray(params['response'])
        return (buff[0]<<8 | buff[1])

    @classmethod
    def write_register(cls, chip_addr, reg, data):
        data = [
            (reg & 0xFF), # Control register
            ((data>>8) & 0xFF), # High byte
            (data & 0xFF), # Lo byte
        ]
        ADS1X1X.i2c[chip_addr].i2c_write(data)

    def __init__(self, config):
        self.i2c_broadcast = bus.MCU_I2C_from_config(config, 0x00, I2C_SPEED)
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.chip = config.getint('ads1x1_chip'
            , minval=ADS1X1X_CHIP_TYPE['ADS1013']
            , maxval=ADS1X1X_CHIP_TYPE['ADS1015'])
        self.chip_addr = config.getint('ads1x1_addr'
            , ADS1X1X_CHIP_ADDR['GND']
            , minval=ADS1X1X_CHIP_ADDR['GND']
            , maxval=ADS1X1X_CHIP_ADDR['SCL'])
        self.speed = config.getint('ads1x1_i2c_speed'
            , ADS1X1X_I2C_SPEED['ADS101x_1600']
            , minval=ADS1X1X_I2C_SPEED['ADS101x_128']
            , maxval=ADS1X1X_I2C_SPEED['ADS111x_860'])
        self.mux = config.getint('ads1x1_mux'
            , ADS1X1X_MUX['MUX_SINGLE_0']
            , minval=ADS1X1X_MUX['MUX_DIFF_0_1']
            , maxval=ADS1X1X_MUX['MUX_SINGLE_3'])
        self.pga = config.getint('ads1x1_pga'
            , ADS1X1X_PGA['PGA_4096']
            , minval=ADS1X1X_PGA['PGA_6144']
            , maxval=ADS1X1X_PGA['PGA_256'])
        self.mode = config.getint('ads1x1_mode'
            , ADS1X1X_MODE['MODE_SINGLE_SHOT']
            , minval=ADS1X1X_MODE['MODE_CONTINUOUS']
            , maxval=ADS1X1X_MODE['MODE_SINGLE_SHOT'])
        self.comp_mode = config.getint('ads1x1_comp_mode'
            , ADS1X1X_COMPARATOR_MODE['TRADITIONAL']
            , minval=ADS1X1X_COMPARATOR_MODE['TRADITIONAL']
            , maxval=ADS1X1X_COMPARATOR_MODE['WINDOW'])
        self.comp_polarity = config.getint('ads1x1_comp_polarity'
            , ADS1X1X_COMPARATOR_POLARITY['ACTIVE_LO']
            , minval=ADS1X1X_COMPARATOR_POLARITY['ACTIVE_LO']
            , maxval=ADS1X1X_COMPARATOR_POLARITY['ACTIVE_HI'])
        self.comp_latching = config.getint('ads1x1_comp_latching'
            , ADS1X1X_COMPARATOR_LATCHING['NON_LATCHING']
            , minval=ADS1X1X_COMPARATOR_LATCHING['NON_LATCHING']
            , maxval=ADS1X1X_COMPARATOR_LATCHING['LATCHING'])
        self.comp_queue = config.getint('ads1x1_comp_queue'
            , ADS1X1X_COMPARATOR_QUEUE['QUEUE_NONE']
            , minval=ADS1X1X_COMPARATOR_QUEUE['QUEUE_1']
            , maxval=ADS1X1X_COMPARATOR_QUEUE['QUEUE_NONE'])
        if( self.chip_addr not in ADS1X1X.i2c):
            ADS1X1X.i2c[self.chip_addr] = bus.MCU_I2C_from_config(config
                , self.chip_addr, I2C_SPEED)
        else:
            # Dummy read of parameters to avoid config error
            config.get('i2c_mcu', 'mcu')
            config.getint('i2c_speed', 100000)
            config.get('i2c_bus', None)
            config.getint('i2c_address', 0)

        # Configure probe on sensor to use
        self.adc_voltage = config.getfloat('adc_voltage', 5., above=0.)
        self.voltage_offset = config.getfloat('voltage_offset', 0.0)
        self.pullup_resistor = config.getfloat('pullup_resistor', 4700.,
            above=0.)
        # Find probe for voltage to temp conversion function/table
        for probe_type, params in adc_temperature.DefaultVoltageSensors:
            if(probe_type == config.get('probe_type')):
                self.converter = adc_temperature.ConvertADCtoTemperature( \
                    adc_temperature.LinearVoltage(config, params))
                break
        # Find probe for resistance to temp conversion function/table
        if(probe_type != config.get('probe_type')):
            for probe_type, params in \
                adc_temperature.DefaultResistanceSensors:
                if(probe_type == config.get('probe_type')):
                    self.converter = \
                        adc_temperature.ConvertADCtoTemperature( \
                        adc_temperature.LinearResistance(config, params))
                    break
        # Attempt to match against existing thermistors, generate conversion
        # table
        if(probe_type != config.get('probe_type')):
            pheaters = config.get_printer().load_object(config, "heaters")
            probe_config = \
                pheaters.sensor_factories[config.get('probe_type')](config) \
                    .adc_convert
            all_temps = [float(i) for i in range(1, 351)]
            adcs = [(t, probe_config.calc_adc(t)) for t in all_temps]
            rs = [(t,self.pullup_resistor * adc / (1.0 - adc)) \
                for t, adc in adcs]
            if( probe_config is not None ):
                self.converter = adc_temperature.ConvertADCtoTemperature( \
                        adc_temperature.LinearResistance(config, rs))
                probe_type = config.get('probe_type')

        if(probe_type != config.get('probe_type')):
            logging.error("Probe '%s' not found in adc_temperature for %s" \
                % (config.get('probe_type'), self.name));
        self.mcu = ADS1X1X.i2c[self.chip_addr].get_mcu()
        # Initialize and keep the smallest time
        if(ADS1X1X.report_time is None):
            ADS1X1X.report_time = config.getfloat('ads1x1_report_time'
                , ADS1X1X_REPORT_TIME
                , minval=ADS1X1X_MIN_REPORT_TIME)
        else:
            report_time = config.getfloat('ads1x1_report_time'
                , ADS1X1X_REPORT_TIME
                , minval=ADS1X1X_MIN_REPORT_TIME)
            ADS1X1X.report_time = min(ADS1X1X.report_time, report_time)

        # Set up 2-byte configuration that will be used with each request
        self.config = 0
        self.config |= (ADS1X1X_OS['OS_SINGLE'] \
            & ADS1X1X_REG_CONFIG['OS_MASK'])
        self.config |= (self.mux & ADS1X1X_REG_CONFIG['MULTIPLEXER_MASK'])
        self.config |= (self.pga & ADS1X1X_REG_CONFIG['PGA_MASK'])
        self.config |= (self.mode & ADS1X1X_REG_CONFIG['MODE_MASK'])
        self.config |= (self.speed & ADS1X1X_REG_CONFIG['DATA_RATE_MASK'])
        self.config |= (self.comp_mode \
            & ADS1X1X_REG_CONFIG['COMPARATOR_MODE_MASK'])
        self.config |= (self.comp_polarity \
            & ADS1X1X_REG_CONFIG['COMPARATOR_POLARITY_MASK'])
        self.config |= (self.comp_latching \
            & ADS1X1X_REG_CONFIG['COMPARATOR_LATCHING_MASK'])
        self.config |= (self.comp_queue \
            & ADS1X1X_REG_CONFIG['COMPARATOR_QUEUE_MASK'])
        logging.info("ADS1x1: Chip Type %#x Chip Addr %#x Pin ID %#x for %s" \
            % (self.chip, self.chip_addr, self.mux, self.name))

        self.temp = self.min_temp = self.max_temp = 0.0

        ADS1X1X.sensor_instances.add(self)
        self.printer.add_object("ads1x1 " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.printer.register_event_handler("klippy:shutdown",
            self.reset_all_devices)

    def handle_connect(self):
        # Only once
        if(ADS1X1X.sample_timer is None):
            # Init all devices on bus for this kind of device
            self.i2c_broadcast.i2c_write([0x06, 0x00, 0x00])
            # Register timer
            ADS1X1X.reactor = self.printer.get_reactor()
            ADS1X1X.sample_timer = \
                ADS1X1X.reactor.register_timer(ADS1X1X._sample_ads1x1)
            ADS1X1X.reactor.update_timer(self.sample_timer, ADS1X1X.reactor.NOW)

    def reset_all_devices(self, print_time=0.):
        # Init all devices on bus for this kind of device
        self.i2c_broadcast.i2c_write([0x06, 0x00, 0x00])

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def degrees_from_sample(self, result):
        # The sample is encoded in the top 12 or full 16 bits
        # Value's meaning is defined by ADS1X1X_REG_CONFIG['PGA_MASK']
        if self.chip == ADS1X1X_CHIP_TYPE['ADS1013'] \
            or self.chip == ADS1X1X_CHIP_TYPE['ADS1014'] \
            or self.chip == ADS1X1X_CHIP_TYPE['ADS1015'] :
            result >>= 4
            volts = result * ADS1X1X_PGA_SCALAR12[self.pga]
        else:
            volts = result * ADS1X1X_PGA_SCALAR16[self.pga]
        temp = self.converter.calcTemp( (volts - self.voltage_offset) \
            / self.adc_voltage )
        # Debugging statement
        #logging.info(\
        #    "ADS1x1 (%02x,%02x): Read %#x Volts %.3f Scale %.4f Temp %.3f"
        #    % (self.chip_addr, self.mux, result, volts - self.voltage_offset
        #        , (volts - self.voltage_offset) / self.adc_voltage, temp) )
        # Check if result is within limits
        if temp < self.min_temp or temp > self.max_temp:
            self.printer.invoke_shutdown(
                "ADS1X1X temperature %0.1f outside range of %0.1f:%.01f"
                % (temp, self.min_temp, self.max_temp))
        return temp

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
        }

def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("ADS1X1X", ADS1X1X)
