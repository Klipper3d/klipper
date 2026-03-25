# klippy/extras/mlx90614.py
#
# MLX90614 (GY-906) IR temperature sensor over SMBus/I2C
#
# Registers:
#   RAM 0x06 = Ta (die/ambient)
#   RAM 0x07 = To1 (object 1)
#   RAM 0x08 = To2 (object 2, dual-zone parts)
#
# Conversion (Melexis):
#   T[K] = raw * 0.02
#   T[C] = T[K] - 273.15
#
# This module registers a Klipper temperature sensor factory:
#   sensor_type: MLX90614
#
# License: GPLv3 (to match Klipper)

import logging
from . import bus

_REG_TA = 0x06
_REG_TO1 = 0x07
_REG_TO2 = 0x08

_LSB_KELVIN = 0.02
_KELVIN_TO_C = -273.15


def _crc8_smbus(data):
    """SMBus PEC CRC-8: poly 0x07, init 0x00, MSB-first."""
    crc = 0x00
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 0x80:
                crc = ((crc << 1) ^ 0x07) & 0xFF
            else:
                crc = (crc << 1) & 0xFF
    return crc


class MLX90614:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]

        # Create MCU-side I2C device from config (supports hw or sw I2C)
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=0x5A, default_speed=100000
        )

        # Which MLX90614 RAM register to report as "temperature"
        channel = config.get("mlx90614_channel", "object").strip().lower()
        if channel in ("object", "obj", "to", "to1"):
            self._reg = _REG_TO1
        elif channel in ("object2", "obj2", "to2"):
            self._reg = _REG_TO2
        elif channel in ("ambient", "ta", "die"):
            self._reg = _REG_TA
        else:
            raise config.error(
                "mlx90614_channel must be one of: object, object2, ambient"
            )

        # Polling interval (seconds)
        self.report_time = config.getfloat(
            "mlx90614_report_time", 0.5, minval=0.05)

        # Validate SMBus PEC byte returned by the device
        self.verify_pec = config.getboolean("mlx90614_verify_pec", True)

        self.temp = 0.0
        self.min_temp = 0.0
        self.max_temp = 0.0
        self._callback = None

        self._sample_timer = self.reactor.register_timer(self._sample)

        # Skip hardware access during debugoutput
        if self.printer.get_start_args().get("debugoutput") is not None:
            return

        self.printer.register_event_handler(
            "klippy:connect", self._handle_connect)

    # --- Klipper temperature-sensor interface ---
    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = float(min_temp)
        self.max_temp = float(max_temp)

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    # --- internals ---
    def _handle_connect(self):
        self.reactor.update_timer(self._sample_timer, self.reactor.NOW)

    def _read_word_with_pec(self, reg):
        # SMBus "Read Word" style: command=reg, response=LSB,MSB,PEC
        params = self.i2c.i2c_read([reg & 0xFF], 3)
        resp = bytearray(params["response"])
        if len(resp) != 3:
            raise self.printer.command_error(
                "mlx90614: short read (len=%d)" % (len(resp),)
            )

        lsb, msb, pec = resp[0], resp[1], resp[2]

        if self.verify_pec:
            addr7 = self.i2c.get_i2c_address() & 0x7F
            sa_w = ((addr7 << 1) | 0) & 0xFF
            sa_r = ((addr7 << 1) | 1) & 0xFF
            expect = _crc8_smbus(
                bytearray([sa_w, reg & 0xFF, sa_r, lsb, msb]))
            if pec != expect:
                raise self.printer.command_error(
                    "mlx90614: PEC mismatch (got=0x%02X, expect=0x%02X)"
                    % (pec, expect))

        return (msb << 8) | lsb

    @staticmethod
    def _raw_to_c(raw):
        # Datasheet: MSB may be an error flag
        if raw & 0x8000:
            raise ValueError("mlx90614: error flag set in reading")
        temp_k = (raw & 0x7FFF) * _LSB_KELVIN
        return temp_k + _KELVIN_TO_C

    def _sample(self, eventtime):
        next_time = self.reactor.monotonic() + self.report_time
        measured_time = self.reactor.monotonic()

        try:
            raw = self._read_word_with_pec(self._reg)
            temp_c = self._raw_to_c(raw)
        except Exception:
            logging.exception("mlx90614: error reading '%s'", self.name)
            return next_time

        self.temp = temp_c

        if temp_c < self.min_temp or temp_c > self.max_temp:
            self.printer.invoke_shutdown(
                "MLX90614 temperature %.1f outside range of %.1f:%.1f"
                % (temp_c, self.min_temp, self.max_temp)
            )
            return self.reactor.NEVER

        if self._callback is not None:
            print_time = self.i2c.get_mcu().estimated_print_time(measured_time)
            self._callback(print_time, temp_c)

        return next_time


def load_config(config):
    # Register sensor factory: sensor_type: MLX90614
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("MLX90614", MLX90614)
