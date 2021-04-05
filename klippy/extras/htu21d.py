# HTU21D(F)/Si7013/Si7020/Si7021/SHT21 i2c based temperature sensors support
#
# Copyright (C) 2020  Lucio Tarantino <lucio.tarantino@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

######################################################################
# NOTE: The implementation requires write support of length 0
#       before reading on the i2c bus of the mcu.
#
# Compatible Sensors:
#       HTU21D - Tested on Linux MCU.
#       Si7013 - Untested
#       Si7020 - Untested
#       Si7021 - Untested
#       SHT21  - Untested
#
######################################################################

HTU21D_I2C_ADDR= 0x40

HTU21D_COMMANDS = {
    'HTU21D_TEMP'       :0xE3,
    'HTU21D_HUMID'      :0xE5,
    'HTU21D_TEMP_NH'    :0xF3,
    'HTU21D_HUMID_NH'   :0xF5,
    'WRITE'             :0xE6,
    'READ'              :0xE7,
    'RESET'             :0xFE,
    'SERIAL'            :[0xFA,0x0F,0xFC,0xC9],
    'FIRMWARE_READ'     :[0x84,0xB8]

}

HTU21D_RESOLUTION_MASK = 0x7E;
HTU21D_RESOLUTIONS = {
    'TEMP14_HUM12':int('00000000',2),
    'TEMP13_HUM10':int('10000000',2),
    'TEMP12_HUM08':int('00000001',2),
    'TEMP11_HUM11':int('10000001',2)
}

# Device with conversion time for tmp/resolution bit
# The format is:
#  <CHIPNAME>:{id:<ID>, ..<RESOlUTION>:[<temp time>,<humidity time>].. }
HTU21D_DEVICES = {
    'SI7013':{'id':0x0D,
        'TEMP14_HUM12':[.11,.12],
        'TEMP13_HUM10':[ .7, .5],
        'TEMP12_HUM08':[ .4, .4],
        'TEMP11_HUM11':[ .3, .7]},
    'SI7020':{'id':0x14,
        'TEMP14_HUM12':[.11,.12],
        'TEMP13_HUM10':[ .7, .5],
        'TEMP12_HUM08':[ .4, .4],
        'TEMP11_HUM11':[ .3, .7]},
    'SI7021':{'id':0x14,
        'TEMP14_HUM12':[.11,.12],
        'TEMP13_HUM10':[ .7, .5],
        'TEMP12_HUM08':[ .4, .4],
        'TEMP11_HUM11':[ .3, .7]},
    'SHT21': {'id':0x31,
        'TEMP14_HUM12':[.85,.29],
        'TEMP13_HUM10':[.43, .9],
        'TEMP12_HUM08':[.22, .4],
        'TEMP11_HUM11':[.11,.15]},
    'HTU21D':{'id':0x32,
        'TEMP14_HUM12':[.50,.16],
        'TEMP13_HUM10':[.25, .5],
        'TEMP12_HUM08':[.13, .3],
        'TEMP11_HUM11':[.12, .8]}
}
#temperature coefficient for RH compensation at range 0C..80C,
#  for HTU21D & SHT21 only
HTU21D_TEMP_COEFFICIENT= -0.15
#crc8 polynomial for 16bit value, CRC8 -> x^8 + x^5 + x^4 + 1
HTU21D_CRC8_POLYNOMINAL= 0x13100

class HTU21D:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=HTU21D_I2C_ADDR, default_speed=100000)
        self.hold_master_mode = config.getboolean('htu21d_hold_master',False)
        self.resolution = config.get('htu21d_resolution','TEMP12_HUM08')
        self.report_time = config.getint('htu21d_report_time',30,minval=5)
        if self.resolution not in HTU21D_RESOLUTIONS:
            raise config.error("Invalid HTU21D Resolution. Valid are %s"
                % '|'.join(HTU21D_RESOLUTIONS.keys()))
        self.deviceId = config.get('sensor_type')
        self.temp = self.min_temp = self.max_temp = self.humidity = 0.
        self.sample_timer = self.reactor.register_timer(self._sample_htu21d)
        self.printer.add_object("htu21d " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self._init_htu21d()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def _init_htu21d(self):
        # Device Soft Reset
        self.i2c.i2c_write([HTU21D_COMMANDS['RESET']])
        # Wait 15ms after reset
        self.reactor.pause(self.reactor.monotonic() + .15)

        # Read ChipId
        params = self.i2c.i2c_read([HTU21D_COMMANDS['SERIAL'][2],
                                    HTU21D_COMMANDS['SERIAL'][3]], 3)
        response = bytearray(params['response'])
        rdevId = response[0] << 8
        rdevId |= response[1]
        checksum = response[2]
        if self._chekCRC8(rdevId) != checksum:
            logging.warn("htu21d: Reading deviceId !Checksum error!")
        rdevId = rdevId >> 8
        deviceId_list = list(
            filter(
              lambda elem: HTU21D_DEVICES[elem]['id'] == rdevId,HTU21D_DEVICES)
            )
        if len(deviceId_list) != 0:
            logging.info("htu21d: Found Device Type %s" % deviceId_list[0])
        else:
            logging.warn("htu21d: Unknown Device ID %#x " % rdevId)

        if(self.deviceId != deviceId_list[0]):
            logging.warn(
                "htu21d: Found device %s. Forcing to type %s as config.",
                 deviceId_list[0],self.deviceId)

        # Set Resolution
        params = self.i2c.i2c_read([HTU21D_COMMANDS['READ']], 1)
        response = bytearray(params['response'])
        registerData = response[0] & HTU21D_RESOLUTION_MASK
        registerData |= HTU21D_RESOLUTIONS[self.resolution]
        self.i2c.i2c_write([HTU21D_COMMANDS['WRITE']],registerData)
        logging.info("htu21d: Setting resolution to %s " % self.resolution)

    def _sample_htu21d(self, eventtime):
        try:
            # Read Temeprature
            if self.hold_master_mode:
                params = self.i2c.i2c_write([HTU21D_COMMANDS['HTU21D_TEMP']])
            else:
                params = self.i2c.i2c_write([HTU21D_COMMANDS['HTU21D_TEMP_NH']])

            # Wait
            self.reactor.pause(self.reactor.monotonic()
            + HTU21D_DEVICES[self.deviceId][self.resolution][0])


            params = self.i2c.i2c_read([],3)

            response = bytearray(params['response'])
            rtemp  = response[0] << 8
            rtemp |= response[1]
            if self._chekCRC8(rtemp) != response[2]:
                logging.warn("htu21d: Checksum error on Temperature reading!")
            else:
                self.temp = (0.002681 * float(rtemp) - 46.85)
                logging.debug("htu21d: Temperature %.2f " % self.temp)

            # Read Humidity
            if self.hold_master_mode:
                self.i2c.i2c_write([HTU21D_COMMANDS['HTU21D_HUMID']])
            else:
                self.i2c.i2c_write([HTU21D_COMMANDS['HTU21D_HUMID_NH']])

            # Wait
            self.reactor.pause(self.reactor.monotonic()
            + HTU21D_DEVICES[self.deviceId][self.resolution][1])

            params = self.i2c.i2c_read([],3)

            response = bytearray(params['response'])
            rhumid = response[0] << 8
            rhumid|= response[1]
            if self._chekCRC8(rhumid) != response[2]:
                logging.warn("htu21d: Checksum error on Humidity reading!")
            else:
                #clear status bits,
                # humidity always returns xxxxxx10 in the LSB field
                rhumid   ^= 0x02;
                self.humidity = (0.001907 * float(rhumid) - 6)
                if (self.humidity < 0):
                    #due to RH accuracy, measured value might be
                    # slightly less than 0 or more 100
                    self.temp = 0
                elif (self.humidity > 100):
                    self.humidity = 100
                # Only for HTU21D & SHT21.
                # Calculates temperature compensated Humidity, %RH
                if( self.deviceId in ['SHT21','HTU21D']
                    and self.temp > 0 and self.temp < 80):
                    logging.debug("htu21d: Do temp compensation..")
                    self.humidity = self.humidity
                    + (25.0 - self.temp) * HTU21D_TEMP_COEFFICIENT;
                logging.debug("htu21d: Humidity %.2f " % self.humidity)
        except Exception:
            logging.exception("htu21d: Error reading data")
            self.temp = self.humidity = .0
            return self.reactor.NEVER

        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "HTU21D temperature %0.1f outside range of %0.1f:%.01f"
                % (self.temp, self.min_temp, self.max_temp))

        measured_time = self.reactor.monotonic()
        print_time = self.i2c.get_mcu().estimated_print_time(measured_time)
        self._callback(print_time, self.temp)
        return measured_time + self.report_time

    def _chekCRC8(self,data):
        for bit in range(0,16):
            if (data & 0x8000):
                data = (data << 1) ^ HTU21D_CRC8_POLYNOMINAL;
            else:
                data <<= 1
        data = data >> 8
        return data

    def get_status(self, eventtime):
        return {
            'temperature': self.temp,
            'humidity': self.humidity,
        }


def load_config(config):
    # Register sensor
    pheater = config.get_printer().lookup_object("heaters")
    for stype in HTU21D_DEVICES:
        pheater.add_sensor_factory(stype, HTU21D)
