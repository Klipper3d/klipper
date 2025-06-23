#################################################################
# Klippy Touch Sensor Module @ Albatross
# Description: This module provides klippy interface for spi-based touch sensor (MCP3462R)
# Date: 2025-05-20
# Version: 0.1
# Owner: Mo
#################################################################

import logging
from . import bus

# -------------------------------------------------------------------------
# MCP3462R SPI Command Constants
# -------------------------------------------------------------------------
FULL_RST         = bytes([0b01111000])
ADCDATA_READ     = bytes([0b01000011])
CONFIG0_WRITE    = bytes([0b01000110])
CONFIG0_DATA     = bytes([0b01100011])
CONFIG1_DATA     = bytes([0b00001100])  # Default
CONFIG2_FORCE_DATA = bytes([0b10110011])  # Gain is x32 (x16 analog, x2 digital)
CONFIG3_DATA     = bytes([0b11000000])
IRQ_DATA         = bytes([0b00000110])
MUX_FORCE_DATA   = bytes([0b00110010])  # For ch2&3. Default is Ch0&1.

# -------------------------------------------------------------------------
# Touch Sensor Class
# -------------------------------------------------------------------------
class Touch_sensor_MCP3462R:
    """Klipper interface for MCP3462R SPI-based touch sensor."""

    def __init__(self, config):
        # SPI and MCU setup
        self.spi = bus.MCU_SPI_from_config(config, 0, pin_option="cs_pin")
        self.spi_oid = self.spi.get_oid()
        self.mcu = self.spi.get_mcu()
        self.oid = self.mcu.create_oid()

        # Printer and pin setup
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        ppins = self.printer.lookup_object('pins')
        self.probe_interrupt_pin = ppins.lookup_pin(config.get('probe_interrupt_pin'))
        self.adc_int_pin = ppins.lookup_pin(config.get('adc_int_pin'))

        # Register MCU configuration
        self.mcu.add_config_cmd(
            "cfg_ts_adc oid=%d spi_oid=%d adc_int_pin=%s trigger_out_pin=%s" %
            (self.oid, self.spi_oid, self.adc_int_pin['pin'], self.probe_interrupt_pin['pin'])
        )
        self.mcu.register_config_callback(self._build_config)

        # Register GCode commands
        self.gcode.register_command('INIT_SPI_TS', self.cmd_INIT_TS,
            desc="Initialize the touch sensor")
        self.gcode.register_command('RESET_TS', self.cmd_RESET_TS,
            desc="Reset the touch sensor")
        self.gcode.register_command('GET_TS_VAL', self.cmd_GET_TS_VALUE,
            desc="Klippy read for touch sensor value")

        self.configured = False

        # Register event handlers
        self.printer.register_event_handler("klippy:ready", self._handle_on_ready)
        self.printer.register_event_handler("probe:PROBE", self._handle_probing_event)

    # ---------------------------------------------------------------------
    # MCU/Session Configuration
    # ---------------------------------------------------------------------
    def _build_config(self):
        self.start_ts_session_cmd = self.mcu.lookup_command(
            "start_ts_session oid=%c timeout_cycles=%u rest_ticks=%u sensitivity=%u"
        )
        self.mcu.register_response(self._handle_ts_session_response, "Ts_session_result", self.oid)

    def _do_initialization_commands(self):
        """Send initialization commands to the touch sensor."""
        self.spi.spi_send(
            CONFIG0_WRITE + CONFIG0_DATA + CONFIG1_DATA +
            CONFIG2_FORCE_DATA + CONFIG3_DATA + IRQ_DATA
        )
        logging.info("Touch sensor configuration commands sent successfully.")
        return True

    # ---------------------------------------------------------------------
    # Event Handlers
    # ---------------------------------------------------------------------
    def _handle_on_ready(self):
        """Handle printer ready event and initialize sensor if needed."""
        if not self.configured:
            self.configured = self._do_initialization_commands()
        # TEMPORARY: For testing purposes, run a fake home command
        self.gcode.run_script("FAKE_HOME")
        # self.gcode.run_script("PROBE")

    def _handle_probing_event(self, gcmd):
        """Handle probing event and start touch sensor session."""
        if not self.configured:
            raise gcmd.error("Touch sensor is not configured. Please initialize it first.")
        self._pending_gcmd = gcmd  # Store for later response
        logging.info("Probing event detected, starting touch sensor session.")
        logging.info("Touch sensor OID: %s", self.oid)
        logging.info("Touch sensor SPI OID: %s", self.spi_oid)
        logging.info("Touch sensor configured: %s", self.configured)
        self.start_ts_session_cmd.send([
            self.oid, 100000, 10000, 500
        ])

    def _handle_ts_session_response(self, params):
        """Handle the response from the touch sensor session."""
        if params['oid'] != self.oid:
            logging.warning("Received response for unknown OID: %s", params['oid'])
            return
        status = params['status']
        data = params['lstValue']

        gcmd = getattr(self, '_pending_gcmd', None)
        if gcmd is not None and status == 0:
            self._pending_gcmd = None  # Clear after use
            raise gcmd.error("Touch sensor session timed out without sensing.")
        else:
            # Fallback to logging if no gcmd is available
            if status == 1:
                logging.info("Touched with value: %d", data)
            elif status == 0:
                logging.warning("Touch sensor session timed out without sensing.")

    # ---------------------------------------------------------------------
    # GCode Command Handlers
    # ---------------------------------------------------------------------
    def cmd_INIT_TS(self, gcmd):
        """Initialize the touch sensor-SPI."""
        if not self.configured:
            self.configured = self._do_initialization_commands()
            gcmd.respond_info("Touch sensor-SPI initialized successfully.")
        else:
            gcmd.respond_info("Touch sensor-SPI already initialized. Reset first, if you attached a new head.")

    def cmd_RESET_TS(self, gcmd):
        """Reset the touch sensor."""
        self.spi.spi_send(FULL_RST)
        self.configured = False
        gcmd.respond_info("Touch sensor reset. Please reinitialize SPI once attachment is connected.")

    def cmd_GET_TS_VALUE(self, gcmd):
        """Get the touch sensor data."""
        if not self.configured:
            raise gcmd.error("Touch sensor not configured. Please initialize it first.")

        self.spi.spi_send(ADCDATA_READ)
        resp = self.spi.spi_transfer([0xff, 0xff])
        data = resp['response']

        if not isinstance(data, (bytes, bytearray)) or len(data) != 2:
            raise gcmd.error("Invalid SPI response: expected 2 bytes, got %s" % repr(data))

        value = (data[0] << 8) | data[1]
        hex_v = data.hex()
        gcmd.respond_info("Touch sensor value: %d, hex: %s" % (value, hex_v))

# -------------------------------------------------------------------------
# Module Load Function
# -------------------------------------------------------------------------
def load_config(config):
    return Touch_sensor_MCP3462R(config)