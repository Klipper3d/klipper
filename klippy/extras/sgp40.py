# SGP40 I2C-based humiditure sensor support
#
# Copyright (C) 2024 Stefan Dej <meteyou@gmail.com>
# Based on the Nevermore sgp40.py code:
# https://github.com/nevermore3d/Nevermore_Max/blob/master/Software/Klipper/sgp40.py
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import math
from . import bus
from struct import unpack_from
from sensirion_gas_index_algorithm.voc_algorithm import VocAlgorithm

SGP40_I2C_ADDR = 0x59
SGP40_WORD_LEN = 2

SGP40_CMD = {
    "GET_SERIAL": [0x36, 0x82],
    "SOFT_RESET": [0x00, 0x06],
    "SELF_TEST": [0x28, 0x0E],
    "MEASURE_RAW_NO_COMP": [0x26, 0x0F, 0x80, 0x00, 0xA2, 0x66, 0x66,
                             0x93],
    "MEASURE_RAW": [0x26, 0x0F]
}

class SGP40:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=SGP40_I2C_ADDR, default_speed=100000)
        self.report_time = config.getint('sgp40_report_time', 1, minval=1)
        self.temp_sensor = config.get('ref_temp_sensor', None)
        self.humidity_sensor = config.get('ref_humidity_sensor', None)

        self.temp = 25
        self.humidity = 50
        self.sample_timer = self.reactor.register_timer(self._sample_sgp40)

        self.voc_algorithm = VocAlgorithm()
        self.raw = self.voc = 0.
        self.printer.add_object("sgp40 " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self._init_sgp40()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def _init_sgp40(self):
        logging.info(self._log_message("Initializing sensor"))

        # Perform a soft reset with increased delay
        logging.info(self._log_message("Performing soft reset"))
        # Data sheet specifies a maximum of 0.6ms for the soft reset
        self._write_command(SGP40_CMD["SOFT_RESET"], delay=.001)

        # Get the serial number to verify communication
        serial = self._write_command(SGP40_CMD["GET_SERIAL"], read_len=6,
                                     delay=.001)
        if serial is None:
            logging.error(self._log_message("Failed to read serial number, "
                                            "communication error"))
        else:
            serial_str = ''.join([f'{x:02x}' for x in serial])
            logging.info(self._log_message(f"Serial number: {serial_str}"))

        # Perform a self-test with increased delay
        logging.info(self._log_message("Performing self-test"))
        # Data sheet specifies a maximum of 320ms for the self-test
        self_test_result = self._write_command(SGP40_CMD["SELF_TEST"],
                                               read_len=3, delay=.32)
        if self_test_result is None or self_test_result[0] != 0xD400:
            logging.error(self._log_message("Self-test failed"))
        else:
            logging.info(self._log_message("Self-test passed successfully"))

    def _sample_sgp40(self, eventtime):
        if self.temp_sensor is not None:
            self.temp = self.printer.lookup_object(
                "{}".format(self.temp_sensor)).get_status(eventtime)[
                    "temperature"]

        if self.humidity_sensor is not None:
            self.humidity = self.printer.lookup_object(
                "{}".format(self.humidity_sensor)).get_status(eventtime)[
                    "humidity"]
        else:
            # calculate humidity from temperature
            self.humidity = self._calculate_humidity(self.temp)


        try:
            cmd = (SGP40_CMD["MEASURE_RAW"]
                   + self._humidity_to_ticks(self.humidity)
                   + self._temperature_to_ticks(self.temp))
            value = self._write_command(cmd, read_len=1, delay=.03)
            self.raw = value[0]
            self.voc = self.voc_algorithm.process(self.raw)
        except Exception as e:
            logging.error(self._log_message(f"Error processing sample: {e}"))

        measured_time = self.reactor.monotonic()
        print_time = self.i2c.get_mcu().estimated_print_time(measured_time)
        self._callback(print_time, self.temp)
        return measured_time + self.report_time

    def _write_command(self, command, delay=0, read_len=0):
        self.i2c.i2c_write(command)

        if read_len == 0:
            return None

        # Delay for the command to be processed
        if delay > 0:
            self.reactor.pause(self.reactor.monotonic() + delay)

        reply_len = read_len * SGP40_WORD_LEN
        params = self.i2c.i2c_read([], reply_len)

        if 'response' not in params or len(params['response']) != reply_len:
            logging.error(self._log_message("Invalid response length from "
                                            "sensor"))
            return None

        response = bytearray(params['response'])
        data = [unpack_from(">H", response, i)[0] for i in range(0, reply_len,
                SGP40_WORD_LEN)]

        return data

    def _calculate_humidity(self, temp):
        # Magnus formula for estimating the saturation vapor pressure curve
        a = 17.62
        b = 243.12
        saturation_vapor_pressure = 6.112 * math.exp((a * temp) / (b + temp))
        actual_vapor_pressure = 6.112 * math.exp( (a * 25) / (b + 25))
        relative_humidity = ((actual_vapor_pressure / saturation_vapor_pressure)
                             * 50)
        return max(0, min(100, relative_humidity))

    def _temperature_to_ticks(self, temperature):
        ticks = int(round(((temperature + 45) * 65535) / 175)) & 0xFFFF
        data = [(ticks >> 8) & 0xFF, ticks & 0xFF]
        crc = self._generate_crc(data)

        return data + [crc]

    def _humidity_to_ticks(self, humidity):
        ticks = int(round((humidity * 65535) / 100)) & 0xFFFF
        data = [(ticks >> 8) & 0xFF, ticks & 0xFF]
        crc = self._generate_crc(data)

        return data + [crc]

    def _generate_crc(self, data):
        # From SGP40 data sheet
        crc = 0xFF
        for byte in data:
            crc ^= byte
            for _ in range(8):
                if crc & 0x80:
                    crc = (crc << 1) ^ 0x31
                else:
                    crc = crc << 1
        return crc & 0xFF

    def _log_message(self, message):
        return f"SGP40 {self.name}: {message}"

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
            'humidity': round(self.humidity, 1),
            'gas_row': self.raw,
            'gas': self.voc,
        }

def load_config(config):
    # Register sensor
    pheater = config.get_printer().lookup_object("heaters")
    pheater.add_sensor_factory("SGP40", SGP40)
