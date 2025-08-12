# Support for sht4x temperature sensors
#
# Copyright (C) 2025  Milzo <overchecking@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
from . import bus
import threading

# SHT4x I2C Commands
DEFAULT_ADDR = 0x44
RESET = 0x94
REQUEST_CHIPID = 0x89

# Measurement commands
HIGH_PRECISION_MODE = 0xFD    # ~8.3ms measurement time
MEDIUM_PRECISION_MODE = 0xF6  # ~4.5ms measurement time
LOW_PRECISION_MODE = 0xE0     # ~1.6ms measurement time

# Timing constants from datasheet
MEASUREMENT_TIMES = {
    HIGH_PRECISION_MODE: 0.0083,   # 8.3ms
    MEDIUM_PRECISION_MODE: 0.0045, # 4.5ms
    LOW_PRECISION_MODE: 0.0016     # 1.6ms
}

SOFT_RESET_TIME = 0.001  # 1ms max
REPORT_TIME = 0.8        # How often to report readings


class SHT4X:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()

        # I2C setup
        valid_addresses = [0x44, 0x45, 0x46]
        addr = config.getint('i2c_address', DEFAULT_ADDR)
        if addr not in valid_addresses:
            addr_list = ', '.join([f'{a:#x}' for a in valid_addresses])
            raise config.error(f"Invalid I2C address {addr:#x}. "
                              f"Valid addresses are: {addr_list}")

        speed = config.getint('i2c_speed', 100000)
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=addr, default_speed=speed)
        self.mcu = self.i2c.get_mcu()

        # Precision setup
        precision = config.get('precision', 'high').lower()
        precision_map = {
            'high': HIGH_PRECISION_MODE,
            'medium': MEDIUM_PRECISION_MODE,
            'low': LOW_PRECISION_MODE
        }
        if precision not in precision_map:
            raise config.error(f"Invalid precision value '{precision}'. "
                              "Valid options are: high, medium, low")
        self.precision_mode = precision_map[precision]

        # Core sensor state
        self.temp = 0
        self.humidity = 0
        self._error_count = 0
        self.sensor_ready = False
        self.measurement_lock = threading.Lock()
        self._callback = None
        self.serial_number = None

        # Register sensor
        self.printer.add_object("sht4x " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                           self.handle_connect)

    def handle_connect(self):
        """Initialize sensor"""
        try:
            self.reset()
            # Add a small delay after reset
            self.reactor.pause(self.reactor.monotonic() + 0.01)  # 10ms

            # Try to read serial number
            self.serial_number = self.get_serial_number()
            if self.serial_number:
                logging.info("SHT4X: Sensor serial number: 0x%08X",
                           self.serial_number)

            # Test measurement
            if self._test_measurement():
                self.sensor_ready = True
                logging.info("SHT4X: Sensor ready")
                self.sample_timer = self.reactor.register_timer(
                    self.sample_sensor)
                self.reactor.update_timer(self.sample_timer,
                                        self.reactor.NOW)
            else:
                logging.error("SHT4X: Sensor test failed")
        except Exception as e:
            logging.error("SHT4X: Init failed: %s" % str(e))

    def _test_measurement(self):
        """Quick measurement test"""
        try:
            recv = self.get_measurements()
            if len(recv) != 6:
                logging.error("SHT4X: Invalid data length: %d", len(recv))
                return False
            if not self._validate_crc(recv):
                logging.error("SHT4X: CRC validation failed")
                return False
            return True
        except Exception as e:
            logging.error("SHT4X: Test measurement failed: %s", str(e))
            return False

    def _validate_crc(self, data):
        """Validate CRC for both temp and humidity"""
        temp_crc = data[2]
        humidity_crc = data[5]
        return (temp_crc == self._crc8(data[0:2]) and
                humidity_crc == self._crc8(data[3:5]))

    def setup_minmax(self, min_temp, max_temp):
        pass

    def setup_callback(self, cb):
        self._callback = cb

    def sample_sensor(self, eventtime):
        """Main sampling loop"""
        if not self.sensor_ready:
            return eventtime + REPORT_TIME

        try:
            with self.measurement_lock:
                recv = self.get_measurements()

                if len(recv) != 6 or not self._validate_crc(recv):
                    raise Exception("Invalid data or CRC failed")

                # Convert temperature
                raw_temp = (recv[0] << 8) | recv[1]
                temp = -45.0 + 175.0 * raw_temp / 65535.0

                # Check if temperature is in reasonable range
                if -50 <= temp <= 150:
                    self.temp = temp
                else:
                    raise ValueError(f"Temperature out of reasonable range: "
                                   f"{temp}°C")

                # Convert humidity
                raw_humidity = (recv[3] << 8) | recv[4]
                humidity_percent = -6.0 + 125.0 * raw_humidity / 65535.0
                self.humidity = max(min(humidity_percent, 100.0), 0.0)

                self._error_count = 0

                # Report to Klipper
                if self._callback is not None:
                    measured_time = self.reactor.monotonic()
                    self._callback(
                        self.mcu.estimated_print_time(measured_time),
                        self.temp)

        except Exception as e:
            self._error_count += 1
            logging.warning("SHT4X: Error %d: %s" %
                          (self._error_count, str(e)))

            # Add retry logic
            if self._error_count <= 3:
                # Retry immediately for first few errors
                return eventtime + 0.5  # Short delay before retry
            elif self._error_count <= 10:
                return eventtime + 2.0  # Longer delay for persistent errors
            else:
                logging.error("SHT4X: Too many errors, sensor failed")
                self.sensor_ready = False

        return eventtime + REPORT_TIME

    def get_measurements(self):
        """Get sensor data with retry logic for NACK handling"""
        data = [self.precision_mode]
        try:
            self.i2c.i2c_write(data)

            measurement_time = MEASUREMENT_TIMES[self.precision_mode]
            self.reactor.pause(self.reactor.monotonic() + measurement_time)

            # Try up to 3 times to read the data
            for retry in range(3):
                try:
                    recv = self.i2c.i2c_read([], 6)
                    return bytearray(recv['response'])
                except Exception as e:
                    if "NACK" in str(e) and retry < 2:
                        # Sensor might still be busy, wait a bit more
                        self.reactor.pause(self.reactor.monotonic() +
                                         measurement_time)
                    else:
                        raise
        except Exception as e:
            raise Exception(f"Failed to get measurements: {e}")

    def get_serial_number(self):
        """Read the sensor's unique serial number"""
        try:
            self.i2c.i2c_write([REQUEST_CHIPID])
            self.reactor.pause(self.reactor.monotonic() + 0.001)  # Small delay
            recv = self.i2c.i2c_read([], 6)
            data = bytearray(recv['response'])
            if len(data) != 6 or not self._validate_crc(data):
                raise Exception("Invalid serial number data or CRC failed")

            # Extract the serial number
            serial_msb = (data[0] << 8) | data[1]
            serial_lsb = (data[3] << 8) | data[4]
            return (serial_msb << 16) | serial_lsb
        except Exception as e:
            logging.warning("SHT4X: Failed to read serial number: %s", str(e))
            return None

    def reset(self):
        """Reset sensor"""
        with self.measurement_lock:
            data = [RESET]
            self.i2c.i2c_write(data)
            self.reactor.pause(self.reactor.monotonic() + SOFT_RESET_TIME)

    def get_status(self, eventtime):
        """Return sensor status for Mainsail"""
        return {
            'temperature': round(self.temp, 2),
            'humidity': round(self.humidity, 1),
            'sensor_ready': self.sensor_ready,
            'serial_number': (f"0x{self.serial_number:08X}"
                            if self.serial_number else "unknown")
        }

    def _crc8(self, buffer):
        """CRC8 checksum implementation per datasheet"""
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
