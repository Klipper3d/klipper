#################################################################
# Klippy Touch Sensor Module @ Albatross
# Description: This module provides klippy interface for spi-based touch sensor (MCP3462R)
# Date: 2025-05-20
# Version: 0.1
# Owner: Mo
#################################################################
import logging
from . import bus

FULL_RST = bytes([0b01111000])
ADCDATA_READ = bytes([0b01000011])
CONFIG0_WRITE = bytes([0b01000110])
CONFIG0_DATA = bytes([0b01100011])
CONFIG1_DATA = bytes([0b00001100]) # This is the default
CONFIG2_FORCE_DATA = bytes([0b10110011]) # Gain is x32 (x16 analog, x2 digital) 
CONFIG3_DATA = bytes([0b11000000])
IRQ_DATA = bytes([0b00000110])
MUX_FORCE_DATA = bytes([0b00110010]) # This is for ch2&3. The default is Ch0&1, which is not what we want.
        

class Touch_sensor_MCP3462R:
    def __init__(self, config):
        self.spi = bus.MCU_SPI_from_config(
            config, 0, pin_option="cs_pin")
        self.spi_oid = self.spi.get_oid()  # Get the OID for the SPI bus
        self.mcu = self.spi.get_mcu()  
        self.oid = self.mcu.create_oid() # Get an OID for that sensor

        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')

        ppins = self.printer.lookup_object('pins')
        self.probe_interrupt_pin = ppins.setup_pin('digital_out',config.get('probe_interrupt_pin'))
        self.adc_int_pin = ppins.setup_pin('endstop',config.get('adc_int_pin')) # Future edit: register config cmd for the sensor with the oid

        self.gcode.register_command('INIT_SPI_TS', self.cmd_INIT_TS,
            desc="Initialize the touch sensor")
        self.gcode.register_command('RESET_TS', self.cmd_RESET_TS,
            desc="Reset the touch sensor")
        self.gcode.register_command('GET_TS_VAL', self.cmd_GET_TS_VALUE,
            desc="Klippy read for touch sensor value")

        self.mcu.register_response(self._handle_ts_session_response, "ts_session_result", self.oid)

        self.configured = False

        self.printer.register_event_handler("klippy:ready", self._handle_on_ready)
        # Hook a handler to probing event, so once it is triggered, we tell the mcu/adc that we are looking for your input now.
        self.printer.register_event_handler("probe:PROBE", self._handle_probing_event)
        
    def _do_config_commands(self):
        self.spi.spi_send(
            CONFIG0_WRITE + CONFIG0_DATA + CONFIG1_DATA +
            CONFIG2_FORCE_DATA + CONFIG3_DATA + IRQ_DATA
            )
        # For future validation
        return True

    def _handle_on_ready(self):
        # Startup configuration for the touch sensor (Initialization)
        self.configured = self._do_config_commands()

        #TEMPORARY: For testing purposes, I will run a fake home command
        self.gcode.run_script("FAKE_HOME")

    def _handle_probing_event(self, gcmd):
        if not self.configured:
            raise gcmd.error("Touch sensor is not configured. Please initialize it first.")

        # TODO: send a cmd to the mcu to start reading the touch sensor adc with a prspecified timout.
        # It should watch out and do some logic to give an interrupt signal to the Z-probe pin
        # # Get the command object (usually done once, e.g. in __init__ or _build_config)
        # self._set_cmd = self._mcu.lookup_command(
        #     "queue_digital_out oid=%c clock=%u on_ticks=%u"
        # )

        # # Later, send the command with the appropriate data
        # self._set_cmd.send([oid, clock, on_ticks])

    def _handle_ts_session_response(self, params):
        """Handle the response from the touch sensor session."""
        # if response['oid'] != self.oid:
        #     logging.warning("Received response for unknown OID: %s", response['oid'])
        #     return

        # # Process the response data
        # if 'response' in response:
        #     data = response['response']
        #     if isinstance(data, (bytes, bytearray)) and len(data) == 2:
        #         value = (data[0] << 8) | data[1]
        #         hex_v = data.hex()
        #         logging.info("Touch sensor value: %d, hex: %s", value, hex_v)
        #     else:
        #         logging.error("Invalid SPI response: expected 2 bytes, got %s", repr(data))
        # else:
        #     logging.error("No response data received.")

    def cmd_INIT_TS(self, gcmd):
        """Initialize the touch sensor-SPI."""
        if not self.configured:
            # Perform any necessary initialization commands here
            self.configured = self._do_config_commands()
            gcmd.respond_info("Touch sensor-SPI initialized successfully.")
        else:
            gcmd.respond_info("Touch sensor-SPI already initialized. Reset first, if you attached a new head.")

    def cmd_RESET_TS(self, gcmd):
        """Reset the touch sensor."""
        self.spi.spi_send(FULL_RST)
        self.configured = False
        # do whatever commands are needed to reset the touch sensor
        gcmd.respond_info("Touch sensor reset. Please reinitialize SPI once attachment is connected.")

    def cmd_GET_TS_VALUE(self, gcmd):
        """Get the touch sensor data."""
        if not self.configured:
            raise gcmd.error("Touch sensor not configured. Please initialize it first.")

        self.spi.spi_send(ADCDATA_READ)
        resp = self.spi.spi_transfer([0xff, 0xff])
        data = resp['response']  # <- FIXED: extract the actual bytes

        if not isinstance(data, (bytes, bytearray)) or len(data) != 2:
            raise gcmd.error("Invalid SPI response: expected 2 bytes, got %s" % repr(data))

        value = (data[0] << 8) | data[1]
        hex_v = data.hex()
        gcmd.respond_info("Touch sensor value: %d, hex: %s" % (value , hex_v))

def load_config(config):
    return Touch_sensor_MCP3462R(config)