# SHT4x i2c temperature sensors
#
# Copyright (C) 2025 Milzo <overchecking@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

# SHT4x I2C Commands
DEFAULT_ADDR = 0x44
RESET = 0x94

# Measurement command (high precision)
MEASUREMENT_CMD = 0xFD

# Measurement timing constant
MEASUREMENT_TIME = 0.009  # 9ms
RESET_TIME = 0.001  # 1ms

class SHT4X:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()

        # I2C setup
        addr = config.getint('i2c_address', DEFAULT_ADDR)
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=addr)
        self.mcu = self.i2c.get_mcu()

        # Sensor state
        self.temp = 0
        self.humidity = 0
        self._error_count = 0
        self.sensor_ready = False

        # Register with Klipper
        self.printer.add_object("sht4x " + self.name, self)
        self.printer.register_event_handler(
            "klippy:connect", self.handle_connect)

    def handle_connect(self):
        """Initialize sensor"""
        try:
            self.reset()
            if self._test_measurement():
                self.sensor_ready = True
                logging.info("SHT4X: Sensor ready")
                self.sample_timer = self.reactor.register_timer(
                    self.sample_sensor)
                self.reactor.update_timer(self.sample_timer, self.reactor.NOW)
            else:
                logging.error("SHT4X: Sensor test failed")
        except Exception as e:
            logging.error("SHT4X: Init failed: %s" % str(e))

    def _test_measurement(self):
        """Quick measurement test"""
        try:
            recv = self.get_measurements()
            return self._validate_crc(recv)
        except Exception:
            return False

    def _validate_crc(self, data):
        """Validate CRC for both temp and humidity"""
        temp_crc = data[2]
        humidity_crc = data[5]
        return (temp_crc == self._crc8(data[0:2]) and 
                humidity_crc == self._crc8(data[3:5]))

    def setup_minmax(self, min_temp, max_temp):
        pass  # Required by Klipper interface

    def setup_callback(self, cb):
        self._callback = cb

    def sample_sensor(self, eventtime):
        """Main sensor sampling method"""
        if not self.sensor_ready:
            return eventtime + 1.0

        try:
            # Get measurements
            recv = self.get_measurements()

            if not self._validate_crc(recv):
                raise Exception("CRC validation failed")

            # Process temperature
            raw_temp = (recv[0] << 8) | recv[1]
            self.temp = -45.0 + 175.0 * raw_temp / 65535.0

            # Process humidity
            raw_humidity = (recv[3] << 8) | recv[4]
            humidity_percent = -6.0 + 125.0 * raw_humidity / 65535.0
            self.humidity = max(min(humidity_percent, 100.0), 0.0)

            self._error_count = 0

            # Report to Klipper
            if hasattr(self, '_callback'):
                self._callback(self.mcu.estimated_print_time(
                    eventtime), self.temp)

        except Exception as e:
            self._error_count += 1
            logging.warning("SHT4X: Error %d: %s" % (self._error_count, str(e)))

            if self._error_count > 5:
                logging.error("SHT4X: Too many errors, sensor failed")
                self.sensor_ready = False

        return eventtime + 1.0

    def get_measurements(self):
        """Get temperature and humidity measurements with proper timing"""
        # Send measurement command and wait for acknowledgment
        params = self.i2c.i2c_write_wait_ack([MEASUREMENT_CMD])
        completion_time = params['completion_time']

        # Wait for measurement to complete
        self.reactor.pause(completion_time + MEASUREMENT_TIME)

        # Read response after proper delay
        params = self.i2c.i2c_read([], 6)
        return bytearray(params['response'])

    def reset(self):
        """Reset sensor with proper timing"""
        params = self.i2c.i2c_write_wait_ack([RESET])
        self.reactor.pause(params['completion_time'] + RESET_TIME)

    def get_status(self, eventtime):
        """Return sensor status for Mainsail"""
        return {
            'temperature': round(self.temp, 2),
            'humidity': round(self.humidity, 1)
        }

    def _crc8(self, buffer):
        """CRC8 checksum for SHT4x sensors"""
        crc = 0xFF
        for byte in buffer:
            crc ^= byte
            for _ in range(8):
                if crc & 0x80:
                    crc = (crc << 1) ^ 0x31
                else:
                    crc = crc << 1
        return crc & 0xFF

def load_config(config):
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("SHT4X", SHT4X)

def load_config_prefix(config):
    return SHT4X(config)
