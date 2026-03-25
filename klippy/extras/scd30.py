# klippy/extras/scd30.py
#
# Sensirion SCD30 (6251021_v5) CO2, Temperature, and Humidity Sensor
#
# This module implements support for the SCD30 sensor over I2C.

import logging
import struct
from . import bus


def sensirion_crc(data):
    crc = 0xFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x80:
                crc = ((crc << 1) ^ 0x31) & 0xFF
            else:
                crc = (crc << 1) & 0xFF
    return crc


class SCD30:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()

        # default SCD30 addr is 0x61 (97)
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=0x61, default_speed=100000)
        self.report_time = config.getfloat('report_time', 2.0, minval=0.5)

        self.co2 = 0.0
        self.temp = 0.0
        self.humidity = 0.0
        self.is_ready = False
        self.temp_callback = None
        self.min_temp = 0.
        self.max_temp = 0.

        self.state = "start_measurement"
        self.sample_timer = self.reactor.register_timer(self._sample_sensor)

        # Register as own printer object so CO2/humidity are accessible
        # via REST API: GET /printer/objects/query?scd30 <name>
        self.printer.add_object("scd30 " + self.name, self)

        self.printer.register_event_handler(
            "klippy:connect", self.handle_connect)

        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command(
            "QUERY_SCD30", "SENSOR", self.name,
            self.cmd_QUERY_SCD30,
            desc="Query the current values from the SCD30 sensor"
        )

    def handle_connect(self):
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW + 2.0)

    def _i2c_read_cmd(self, cmd_bytes, read_len):
        # SCD30 requires a full STOP after the command write before reading.
        # Using i2c_read([cmd], n) sends a repeated-START which the
        # SCD30 ignores.
        # Firmware reference (CO2_SENSORS_TEMP.py):
        #   i2c.writeto(ADDR, cmd)   <- STOP
        #   i2c.readfrom(ADDR, n)    <- new START
        self.i2c.i2c_write(cmd_bytes)
        return self.i2c.i2c_read([], read_len)

    def _sample_sensor(self, eventtime):
        try:
            if self.state == "start_measurement":
                # cmd=0x0010, arg=0x0000, CRC([0x00,0x00])=0x81
                self.i2c.i2c_write([0x00, 0x10, 0x00, 0x00, 0x81])
                self.state = "request_ready"
                return eventtime + self.report_time

            elif self.state == "request_ready":
                params = self._i2c_read_cmd([0x02, 0x02], 3)
                resp = bytearray(params["response"])
                if len(resp) == 3 and sensirion_crc(
                        resp[0:2]) == resp[2] and resp[1] == 1:
                    self.state = "read_data"
                    return eventtime + 0.010
                return eventtime + self.report_time

            elif self.state == "read_data":
                params = self._i2c_read_cmd([0x03, 0x00], 18)
                resp = bytearray(params["response"])
                if len(resp) == 18:
                    data = bytearray()
                    for i in range(6):
                        word = resp[i * 3: i * 3 + 2]
                        crc = resp[i * 3 + 2]
                        if sensirion_crc(word) == crc:
                            data.extend(word)
                        else:
                            logging.warning("scd30: CRC mismatch on word %d", i)
                            self.state = "request_ready"
                            return eventtime + self.report_time
                    if len(data) == 12:
                        self.co2 = struct.unpack(">f", data[0:4])[0]
                        self.temp = struct.unpack(">f", data[4:8])[0]
                        self.humidity = struct.unpack(">f", data[8:12])[0]
                        self.is_ready = True
                        if self.temp_callback is not None:
                            self.temp_callback(eventtime, self.temp)
                self.state = "request_ready"
                return eventtime + self.report_time

        except Exception:
            logging.exception("scd30: Error polling sensor '%s'", self.name)
            self.state = "start_measurement"
            return eventtime + self.report_time

        return eventtime + self.report_time

    # --- Klipper temperature sensor interface ---
    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self.temp_callback = cb

    def get_report_time_delta(self):
        return self.report_time

    # --- Printer object status (REST API + G-code) ---
    def get_status(self, eventtime):
        return {
            'co2': round(self.co2, 1),
            'temperature': round(self.temp, 2),
            'humidity': round(self.humidity, 1),
        }

    def cmd_QUERY_SCD30(self, gcmd):
        if self.is_ready:
            gcmd.respond_info(
                "SCD30 %s: CO2=%.1f ppm, Temp=%.2f C, Humidity=%.1f %%" %
                (self.name, self.co2, self.temp, self.humidity)
            )
        else:
            gcmd.respond_info(
                "SCD30 %s: Not ready or error reading data" %
                self.name)


def load_config(config):
    pheaters = config.get_printer().load_object(config, 'heaters')
    pheaters.add_sensor_factory("SCD30", SCD30)


def load_config_prefix(config):
    return SCD30(config)
