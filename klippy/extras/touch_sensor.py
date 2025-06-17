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
        self.probe_interrupt_pin = ppins.lookup_pin(config.get('probe_interrupt_pin'))
        self.adc_int_pin = ppins.lookup_pin(config.get('adc_int_pin')) 

        self.gcode.register_command('INIT_SPI_TS', self.cmd_INIT_TS,
            desc="Initialize the touch sensor")
        self.gcode.register_command('RESET_TS', self.cmd_RESET_TS,
            desc="Reset the touch sensor")
        self.gcode.register_command('GET_TS_VAL', self.cmd_GET_TS_VALUE,
            desc="Klippy read for touch sensor value")

        self.cfg_hw_cmd = self.mcu.lookup_command(
            "cfg_ts_adc oid=%c spi_oid=%c adc_int_pin=%u trigger_out_pin=%u"
        )
        self.start_ts_session_cmd = self.mcu.lookup_command(
            "start_ts_session oid=%c timeout_cycles=%u rest_ticks=%u sensitivity=%u"
        )


        self.configured = False

        self.printer.register_event_handler("klippy:ready", self._handle_on_ready)
        # Hook a handler to probing event, so once it is triggered, we tell the mcu/adc that we are looking for your input now.
        self.printer.register_event_handler("probe:PROBE", self._handle_probing_event)
        
    def _do_initialization_commands(self):
        self.spi.spi_send(
            CONFIG0_WRITE + CONFIG0_DATA + CONFIG1_DATA +
            CONFIG2_FORCE_DATA + CONFIG3_DATA + IRQ_DATA
            )
        # Config the HW connections, send a command
        self.cfg_hw_cmd.send([
            self.oid, self.spi_oid, self.adc_int_pin['pin'], self.probe_interrupt_pin['pin']
        ])
        # For future validation
        return True

    def _handle_on_ready(self):
        # Startup configuration for the touch sensor (Initialization)
        self.configured = self._do_initialization_commands()

        #TEMPORARY: For testing purposes, I will run a fake home command
        self.gcode.run_script("FAKE_HOME")

    def _handle_probing_event(self, gcmd):
        if not self.configured:
            raise gcmd.error("Touch sensor is not configured. Please initialize it first.")
        self.mcu.register_response(self._handle_ts_session_response, "Ts_session_result", self.oid)
        self.start_ts_session_cmd.send([
            self.oid, 1000, 100, 10
        ])
        # # Later, send the command with the appropriate data
        # self._set_cmd.send([oid, clock, on_ticks])

    def _handle_ts_session_response(self, params):
        """Handle the response from the touch sensor session."""
        if params['oid'] != self.oid:
            logging.warning("Received response for unknown OID: %s", params['oid'])
            return
        status = params['status']
        data = params['lstValue']

        if status == 'done':
            logging.info("Touched with value: %d, hex: %s", data, data.hex())
        elif status == 'timeout':
            logging.warning("Touch sensor session timed out without sensing.")

    def cmd_INIT_TS(self, gcmd):
        """Initialize the touch sensor-SPI."""
        if not self.configured:
            # Perform any necessary initialization commands here
            self.configured = self._do_initialization_commands()
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