#################################################################
# Klippy Touch Sensor Module @ Alpatros
# Description: This module provides klippy interface for spi-based touch sensor (MCP3462R)
# Date: 2025-06-20
# Version: 0.1
# Owner: Mo
#################################################################
import logging
from . import bus

class Touch_sensor_MCP3462R:
    def __init__(self, config):
        self.spi = bus.MCU_SPI_from_config(
            config, 0, pin_option="cs_pin")

        self.printer = config.get_printer()

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('RESET_TS', self.cmd_reset_touch_sensor,
            desc="Reset the touch sensor")
        self.gcode.register_command('INIT_SPI_TS', self.cmd_init_spi_touch_sensor,
            desc="Initialize the touch sensor SPI")
        self.gcode.register_command('GET_TS_VAL', self.cmd_get_touch_sensor_reading,
            desc="Klippy read for touch sensor value")

        # ppins = self.printer.lookup_object('pins')
        # self.adc_int_pin = ppins.setup_pin('endstop',config.get('adc_int_pin'))

        self.FULL_RST = bytes([0b01111000])
        self.ADCDATA_READ = bytes([0b01000011])
        self.CONFIG0_WRITE = bytes([0b01000110])
        self.CONFIG0_DATA = bytes([0b01100011])
        self.CONFIG1_DATA = bytes([0b00001100]) # This is the default
        self.CONFIG2_FORCE_DATA = bytes([0b10110011]) # Gain is x32 (x16 analog, x2 digital) 
        self.CONFIG3_DATA = bytes([0b11000000])
        self.IRQ_DATA = bytes([0b00000110])
        self.MUX_FORCE_DATA = bytes([0b00110010]) # This is for ch2&3. The defualt is Ch0&1, which is not what we want.
        self.configured = self._do_config_commands()

        # need to hook a handler to probing envent, so once it is triggered, we tell the mcu/adc that we are looking for you inpu now.
        self.printer.register_event_handler("probe:PROBE", self._handle_probing_event)
        # self.gcode.register_command('TOGGLE_CS', self.cmd_toggle_cs_pin,
        #     desc="Toggle CS pin manually for debugging")
        self.printer.register_event_handler("klippy:ready", self._on_ready)

    def cmd_toggle_cs_pin(self, gcmd):
        for i in range(10000):
            resp = self.spi.spi_transfer([0xff])
            data = resp['response']  # <- FIXED: extract the actual bytes

            if not isinstance(data, (bytes, bytearray)) or len(data) != 1:
                raise gcmd.error("Invalid SPI response: expected 1 byte, got %s" % repr(data))

            value = data[0]
            hex_v = data.hex()
            gcmd.respond_info("Touch sensor value: %d, hex: %s" % (value , hex_v))
    def _on_ready(self):
        self.gcode.run_script("GET_TS_VAL")
        
    def _do_config_commands(self):
        self.spi.spi_send(
            self.CONFIG0_WRITE + self.CONFIG0_DATA + self.CONFIG1_DATA +
            self.CONFIG2_FORCE_DATA + self.CONFIG3_DATA + self.IRQ_DATA
            )
        return True

    def cmd_init_spi_touch_sensor(self, gcmd):
        """Initialize the touch sensor SPI."""
        if not self.configured:
            self.printer.get_printer().logger.info("Initializing touch sensor SPI")
            # Perform any necessary initialization commands here
            self.configured = self._do_config_commands()
            if not self.configured:
                raise gcmd.error("Failed to configure touch sensor SPI.")
            gcmd.respond_info("Touch sensor SPI initialized successfully.")
        else:
            gcmd.respond_info("Touch sensor SPI already initialized. Reset first if you attached a new head.")
        
    def cmd_reset_touch_sensor(self, gcmd):
        """Reset the touch sensor."""
        self.spi.spi_send(self.FULL_RST)
        self.configured = False
        # do whatever commands are needed to reset the touch sensor
        self.printer.get_printer().logger.info("Touch sensor reset. Please reinitialize SPI.")
        gcmd.respond_info("Touch sensor reset. Please reinitialize SPI once attachement is connected.")

    def _handle_probing_event(self, event):
        if not self.configured:
            self.printer.get_printer().logger.error("Touch sensor not configured. Please initialize it first.")
            return
        # send a cmd to the mcu to start reading the touch sensor adc
        # It should watch out and do some logic to give an interrupt singnal to the Z-probe pin
    def cmd_get_touch_sensor_reading(self, gcmd):
        """Get the touch sensor data."""
        if not self.configured:
            raise gcmd.error("Touch sensor not configured. Please initialize it first.")

        self.spi.spi_send(self.ADCDATA_READ)
        resp = self.spi.spi_transfer([0xff, 0xff])
        data = resp['response']  # <- FIXED: extract the actual bytes

        if not isinstance(data, (bytes, bytearray)) or len(data) != 2:
            raise gcmd.error("Invalid SPI response: expected 2 bytes, got %s" % repr(data))

        value = (data[0] << 8) | data[1]
        hex_v = data.hex()
        gcmd.respond_info("Touch sensor value: %d, hex: %s" % (value , hex_v))

def load_config(config):
    return Touch_sensor_MCP3462R(config)