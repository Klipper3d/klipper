# SHT3X i2c based temperature sensors support
#
# Copyright (C) 2024  Timofey Titovets <nefelim4ag@gmail.com>
# Based on htu21d.py code
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

######################################################################
# Compatible Sensors:
#       SHT31  - Tested on octopus pro and Linux MCU
#
######################################################################
SHT3X_I2C_ADDR = 0x44

SHT3X_CMD = {
    'MEASURE': {
        'STRETCH_ENABLED': {
            'HIGH_REP': [0x2c, 0x06], # High   (15ms) repeatability measurement
            'MED_REP': [0x2c, 0x0D],  # Medium (6ms)  repeatability measurement
            'LOW_REP': [0x2c, 0x10],  # Low    (4ms)  repeatability measurement
        },
        'STRETCH_DISABLED' : {
            'HIGH_REP': [0x24, 0x00],
            'MED_REP': [0x24, 0x0B],
            'LOW_REP': [0x24, 0x16],
        },
    },
    'PERIODIC': {
        '2HZ': {
            'HIGH_REP': [0x22, 0x36],
            'MED_REP': [0x22, 0x20],
            'LOW_REP': [0x22, 0x2B],
        },
    },
    'OTHER': {
        'STATUS': {
            'READ': [0xF3, 0x2D],
            'CLEAN': [0x30, 0x41],
        },
        'SOFTRESET': [0x30, 0xA2], # Soft reset
        'HEATER': {
            "ENABLE": [0x30, 0x6D],
            "DISABLE": [0x30, 0x66],
        },
        'FETCH': [0xE0, 0x00],
        'BREAK': [0x30, 0x93],
    }
}

class SHT3X:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=SHT3X_I2C_ADDR, default_speed=100000)
        self.report_time = config.getint('sht3x_report_time', 1, minval=1)
        self.deviceId = config.get('sensor_type')
        self.temp = self.min_temp = self.max_temp = self.humidity = 0.
        self.sample_timer = self.reactor.register_timer(self._sample_sht3x)
        self.printer.add_object("sht3x " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def handle_connect(self):
        self._init_sht3x()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def _init_sht3x(self):
        # Device Soft Reset
        self.i2c.i2c_write_wait_ack(SHT3X_CMD['OTHER']['BREAK'])
        # Break takes ~ 1ms
        self.reactor.pause(self.reactor.monotonic() + .0015)
        self.i2c.i2c_write_wait_ack(SHT3X_CMD['OTHER']['SOFTRESET'])
        # Wait <=1.5ms after reset
        self.reactor.pause(self.reactor.monotonic() + .0015)

        status = self.i2c.i2c_read(SHT3X_CMD['OTHER']['STATUS']['READ'], 3)
        response = bytearray(status['response'])
        status = response[0] << 8
        status |= response[1]
        checksum = response[2]

        if self._crc8(status) != checksum:
            logging.warning("sht3x: Reading status - checksum error!")

        # Enable periodic mode
        self.i2c.i2c_write_wait_ack(
            SHT3X_CMD['PERIODIC']['2HZ']['HIGH_REP']
        )
        # Wait <=15.5ms for first measurment
        self.reactor.pause(self.reactor.monotonic() + .0155)

    def _sample_sht3x(self, eventtime):
        try:
            # Read measurment
            params = self.i2c.i2c_read(SHT3X_CMD['OTHER']['FETCH'], 6)

            response = bytearray(params['response'])
            rtemp  = response[0] << 8
            rtemp |= response[1]
            if self._crc8(rtemp) != response[2]:
                logging.warning(
                    "sht3x: Checksum error on Temperature reading!"
                )
            else:
                self.temp = -45 + (175 * rtemp / 65535)
                logging.debug("sht3x: Temperature %.2f " % self.temp)

            rhumid  = response[3] << 8
            rhumid |= response[4]
            if self._crc8(rhumid) != response[5]:
                logging.warning("sht3x: Checksum error on Humidity reading!")
            else:
                self.humidity = 100 * rhumid / 65535
                logging.debug("sht3x: Humidity %.2f " % self.humidity)

        except Exception:
            logging.exception("sht3x: Error reading data")
            self.temp = self.humidity = .0
            return self.reactor.NEVER

        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "sht3x: temperature %0.1f outside range of %0.1f:%.01f"
                % (self.temp, self.min_temp, self.max_temp))

        measured_time = self.reactor.monotonic()
        print_time = self.i2c.get_mcu().estimated_print_time(measured_time)
        self._callback(print_time, self.temp)
        return measured_time + self.report_time

    def _split_bytes(self, data):
        bytes = []
        for i in range((data.bit_length() + 7) // 8):
            bytes.append((data >> i*8) & 0xFF)
        bytes.reverse()
        return bytes

    def _crc8(self, data):
        #crc8 polynomial for 16bit value, CRC8 -> x^8 + x^5 + x^4 + 1
        SHT3X_CRC8_POLYNOMINAL= 0x31
        crc = 0xFF
        data_bytes = self._split_bytes(data)
        for byte in data_bytes:
            crc ^= byte
            for _ in range(8):
                if crc & 0x80:
                    crc = (crc << 1) ^ SHT3X_CRC8_POLYNOMINAL
                else:
                    crc <<= 1
        return crc & 0xFF

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
            'humidity': round(self.humidity, 1),
        }

def load_config(config):
    # Register sensor
    pheater = config.get_printer().lookup_object("heaters")
    pheater.add_sensor_factory("SHT3X", SHT3X)
