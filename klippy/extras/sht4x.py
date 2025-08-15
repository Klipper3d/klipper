# SHT4X i2c based temperature sensors support
#
# Copyright (C) 2025 Milzo <overchecking@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus
######################################################################
# Compatible Sensors:
#       SHT40, SHT41, SHT45 - Sensirion SHT4X series
#
######################################################################
SHT4X_I2C_ADDR = 0x44

SHT4X_CMD = {
    'MEASURE': {
        'STRETCH_ENABLED': {
            'HIGH_REP': [0xFD],    # High precision, 8.2ms
            'MED_REP': [0xF6],     # Medium precision, 4.5ms
            'LOW_REP': [0xE0]      # Low precision, 1.7ms
        },
    },
    'OTHER': {
        'SOFTRESET': [0x94],       # Soft reset
    }
}

class SHT4X:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=SHT4X_I2C_ADDR, default_speed=100000)
        self._error = self.i2c.get_mcu().error
        self.report_time = config.getint('sht4x_report_time', 1, minval=1)
        self.deviceId = config.get('sensor_type')
        self.temp = self.min_temp = self.max_temp = self.humidity = 0.
        self.sample_timer = self.reactor.register_timer(self._sample_sht4x)
        self.printer.add_object("sht4x " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self._init_sht4x()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def _init_sht4x(self):
        try:
            # Soft reset the device
            if hasattr(self.i2c, 'i2c_write_wait_ack'):
                self.i2c.i2c_write_wait_ack(SHT4X_CMD['OTHER']['SOFTRESET'])
            else:
                self.i2c.i2c_write(SHT4X_CMD['OTHER']['SOFTRESET'])
            # Wait after reset
            self.reactor.pause(self.reactor.monotonic() + 0.001)

            logging.info("sht4x: initialized for single-shot measurements")

        except Exception:
            logging.exception("sht4x: initialization failed")
            raise

    def _sample_sht4x(self, eventtime):
        try:
            # Single-shot measurement with retries
            retries = 5
            params = None
            error = None

            while retries > 0 and params is None:
                try:
                    # Send measurement command
                    if hasattr(self.i2c, 'i2c_write_wait_ack'):
                        self.i2c.i2c_write_wait_ack(
                        SHT4X_CMD['MEASURE']['STRETCH_ENABLED']['HIGH_REP'])
                    else:
                        self.i2c.i2c_write(
                        SHT4X_CMD['MEASURE']['STRETCH_ENABLED']['HIGH_REP'])

                    # Wait for measurement to complete
                    self.reactor.pause(self.reactor.monotonic() + 0.009)

                    # Read 6 bytes
                    params = self.i2c.i2c_read([], 6, retry=False)

                except Exception as e:
                    logging.exception(
                    "sht4x: measurement attempt failed: %s", e)
                    error = e
                    self.reactor.pause(self.reactor.monotonic() + .5)
                    retries -= 1

            if params is None:
                raise error

            response = bytearray(params['response'])
            rtemp  = response[0] << 8
            rtemp |= response[1]
            if self._crc8(rtemp) != response[2]:
                logging.warning(
                    "sht4x: Checksum error on Temperature reading!"
                )
            else:
                self.temp = -45 + (175 * rtemp / 65535)
                logging.debug("sht4x: Temperature %.2f " % self.temp)

            rhumid  = response[3] << 8
            rhumid |= response[4]
            if self._crc8(rhumid) != response[5]:
                logging.warning("sht4x: Checksum error on Humidity reading!")
            else:
                self.humidity = 100 * rhumid / 65535
                logging.debug("sht4x: Humidity %.2f " % self.humidity)

        except Exception:
            logging.exception("sht4x: Error reading data")
            self.temp = self.humidity = .0
            return self.reactor.NEVER

        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "sht4x: temperature %0.1f outside range of %0.1f:%.01f"
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
        SHT4X_CRC8_POLYNOMINAL= 0x31
        crc = 0xFF
        data_bytes = self._split_bytes(data)
        for byte in data_bytes:
            crc ^= byte
            for _ in range(8):
                if crc & 0x80:
                    crc = (crc << 1) ^ SHT4X_CRC8_POLYNOMINAL
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
    pheater.add_sensor_factory("SHT4X", SHT4X)
