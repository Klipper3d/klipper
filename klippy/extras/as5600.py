# klippy/extras/as5600.py
#
# AS5600 I2C Magnetic Rotary Position Sensor
#
# This module implements support for the AS5600 magnetic rotary sensor over I2C.
# It can be accessed via regular polling.
#
# Configuration:
# [as5600 my_sensor]
# i2c_mcu: mcu
# i2c_software_scl_pin: PA8
# i2c_software_sda_pin: PC9
# i2c_address: 54
# report_time: 0.1
# stepper: stepper_x

import logging
from . import bus

try:
    from .angle import AngleCalibration
except ImportError:
    AngleCalibration = None

# AS5600 Registers
_REG_RAW_ANGLE_H = 0x0C
_REG_RAW_ANGLE_L = 0x0D
_REG_ANGLE_H = 0x0E
_REG_ANGLE_L = 0x0F
_REG_STATUS = 0x0B


class AS5600:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()

        # Create MCU-side I2C device from config (supports hw or sw I2C)
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=0x36, default_speed=100000
        )

        # Allow user to specify how frequently we poll
        self.report_time = config.getfloat("report_time", 0.1, minval=0.01)

        self.raw_angle = 0
        self.angle_deg = 0.0
        self.status = 0
        self._clients = []

        self.calibration = None
        if config.get('stepper', None) is not None:
            if AngleCalibration is not None:
                self.calibration = AngleCalibration(config)
            else:
                raise config.error(
                    "AngleCalibration not available. Cannot link stepper.")

        self._sample_timer = self.reactor.register_timer(self._sample)

        # Skip hardware access during runtime if simulating
        if self.printer.get_start_args().get("debugoutput") is not None:
            return

        self.printer.register_event_handler(
            "klippy:connect", self._handle_connect)

        # Register a GCode command for querying the current angle
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command(
            "QUERY_AS5600", "SENSOR", self.name,
            self.cmd_QUERY_AS5600,
            desc="Query the current angle from the AS5600 sensor"
        )

    def _handle_connect(self):
        self.reactor.update_timer(self._sample_timer, self.reactor.NOW)

    def _read_registers(self, reg, count):
        """Read `count` bytes starting from `reg`."""
        params = self.i2c.i2c_read([reg & 0xFF], count)
        resp = bytearray(params["response"])
        if len(resp) != count:
            raise self.printer.command_error(
                "as5600: short read (expected %d, got %d)" % (count, len(resp))
            )
        return resp

    def _sample(self, eventtime):
        next_time = self.reactor.monotonic() + self.report_time
        measured_time = self.reactor.monotonic()

        try:
            # Read STATUS (0x0B) to verify magnet presence (optional)
            # and RAW_ANGLE (0x0C-0x0D).
            # Actually, to keep it simple, we just read RAW_ANGLE
            resp = self._read_registers(_REG_RAW_ANGLE_H, 2)

            # Combine 12-bit value: bits 8-11 in high byte, bits 0-7 in low byte
            self.raw_angle = ((resp[0] & 0x0F) << 8) | resp[1]

            # Calculate angle in degrees (0 to 360)
            self.angle_deg = (self.raw_angle / 4096.0) * 360.0

            if self._clients:
                print_time = self.i2c.get_mcu().estimated_print_time(
                    measured_time)

                # AS5600 is 12-bit (0-4095). Klipper AngleCalibration
                # expects 16-bit angles.
                # Shift by 4 to map 0x0FFF to 0xFFF0.
                klipper_angle = self.raw_angle << 4
                samples = [(print_time, klipper_angle)]

                offset = None
                if self.calibration is not None:
                    offset = self.calibration.apply_calibration(samples)

                msg = {
                    'data': samples,
                    'errors': 0,
                    'position_offset': offset
                }
                for client in self._clients:
                    client(msg)

        except Exception:
            logging.exception(
                "as5600: error reading from sensor '%s'",
                self.name)
            return next_time

        return next_time

    def get_status(self, eventtime):
        """Return status dictionary for frontends."""
        return {
            'raw_angle': self.raw_angle,
            'angle': self.angle_deg
        }

    def cmd_QUERY_AS5600(self, gcmd):
        """Handle the QUERY_AS5600 G-Code command."""
        gcmd.respond_info("AS5600 %s: raw_angle=%d angle=%.2f deg" % (
            self.name, self.raw_angle, self.angle_deg))

    # --- Client interface (optional but useful for streaming/analysis
    # plugins) ---
    def add_client(self, callback):
        self._clients.append(callback)


def load_config(config):
    return AS5600(config)


def load_config_prefix(config):
    return AS5600(config)
