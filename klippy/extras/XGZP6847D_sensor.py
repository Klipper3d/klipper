#################################################################
# Klippy Suction monitoring Module @ Albatross
# Description: This module provides klippy interface for suction
#              i2c-based sensor (XGZP6847D) with asynchronous 
#              read for pressure and temperature
# Date: 2025-06-26
# Version: 0.1
# Owner: Mo
#################################################################
from . import bus
import logging

# "I2C device factory setting slave address: 0X6D."
XGZP6847D_FACTORY_ADDRESS = 0x6D

REG_ADD_P_DATA_MSB = 0x06
REG_ADD_P_DATA_CSB = 0x07
REG_ADD_P_DATA_LSB = 0x08
REG_ADD_T_DATA_MSB = 0x09
REG_ADD_T_DATA_LSB = 0x0A
REG_ADD_CMD = 0x30
REG_DATA_CMD = 0x0A # start combined conversion pressure and temperature
# Sampling takes 20ms at most
SAMPLING_CYCLE_MS = 5.
MAX_SAMPLING_CYCLE = 6

READ_READY_STATE = 0

# K vlue for the datasheet
# Pressure Range(Absolute Value-KPa）K valueSensor Pressure Range Example
# 1000≤Pressure Range 4 0~1000kPa; 0~1500kPa; -100~1000kPa ...
# 500<Pressure Range＜1000 8 0~700kPa; -100~700kPa...
# 260<Pressure Range≤500 16 0~500kPa; -100~300kPa...
# 130<Pressure Range≤260 32 0~200kPa; -100~200kPa... <<< XGZP6847D100KPG
# 65<Pressure Range≤130 64 0~100kPa; -100~100kPa; -100~0kPa... 
# 32<Pressure Range≤65 128 0~40kPa; -40~40kPa; -40~0kPa... 
# 16<Pressure Range≤32 256 0~20kPa; -20~20kPa; -20~0kPa; -30~0kPa...
# 8<Pressure Range≤16 512 0~10kPa; -10~10kPa; -10~0kPa...
# 4<Pressure Range≤8 1024 0~5kPa; -5~5kPa; -1~1PSI...
# 2≤Pressure Range≤4 2048 0~2.5kPa; -2.5~2.5kPa...
# 1≤Pressure Range<2 4096 0~1kPa; -1~1kPa...
# Pressure Range<1 8192 -0.5~0.5kPa...

k_dict = {
    'XGZP6847D100KPG': 128. # 130<Pressure Range≤260 (absolute value)
}

class PumpPressureSensor:
    # XGZP6847D vacuum pressure sensor
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object('gcode')

        self.i2c = bus.MCU_I2C_from_config(config, default_speed=100000, default_addr=XGZP6847D_FACTORY_ADDRESS)

        self.active_read = False

        self.pressure,  self.temperature, self.read_cycle = 0, 0, 0
        self.timer = None

        self.gcode.register_command('DO_PT_MEASURE', self.cmd_do_PT_measure,
            desc="Start a pressure and temperature measurement cycle on the XGZP6847D sensor")
        self.gcode.register_command('TELL_PT_MEASURE', self.cmd_tell_readings,
            desc="Give a pressure and temperature measurement cycle on the XGZP6847D sensor")
        
        self.printer.register_event_handler("klippy:ready", self._handle_on_ready)

               
    def get_status(self, eventtime):
        # Read pressure and temperature from the sensor
        return {'Sampling Status': self.active_read, 'Pressure': self.pressure, 'Temperature': self.temperature}
    def _handle_on_ready(self):
        """Handle the 'klippy:ready' event to initialize the sensor."""
        # Check the sensor if conncected or not. If not this line will issue a shutdown
        self.i2c.i2c_read([REG_ADD_CMD], 1)
        
    def _do_read(self, eventtime):
        # Confirm if the sampling is done
        cmd_data = self.i2c.i2c_read([REG_ADD_CMD], 1)['response'][0]
        done_s = (cmd_data & 0x08) >> 3
        if done_s == READ_READY_STATE:
            [p_msb, p_csb, p_lsb] = self.i2c.i2c_read([REG_ADD_P_DATA_MSB], 3)['response']
            [t_msb, t_lsb] = self.i2c.i2c_read([REG_ADD_T_DATA_MSB], 2)['response']
            # Convert raw values to signed integers
            raw_pressure = (p_msb << 16) | (p_csb << 8) | p_lsb
            if raw_pressure & 0x800000:
                raw_pressure -= 0x1000000  # Convert to signed 24-bit int
            self.pressure = raw_pressure/ k_dict['XGZP6847D100KPG']  # Convert to Pa using the K value
            self.temperature = (t_msb << 8) | t_lsb
            self.temperature = self.temperature / 256.  # Convert to Celsius
            # Force to make this the last cycle
            self.read_cycle = MAX_SAMPLING_CYCLE
            logging.info(f"XGZP6847D sensor read completed: Pressure={self.pressure} Pa, Temperature={self.temperature} C")
        else:
            # If not done, increment the read cycle
            self.read_cycle += 1
        if self.read_cycle >= MAX_SAMPLING_CYCLE:
            self.active_read = False
            self.read_cycle = 0
            if  done_s != READ_READY_STATE:
                logging.warning("XGZP6847D sensor read timed out or failed.")
            return self.reactor.NEVER
        # If not done or not timed out, reschedule the read
        return eventtime + SAMPLING_CYCLE_MS/1000.

    def _read_sensor_data(self):
        # Start sampling
        self.i2c.i2c_write([REG_ADD_CMD, REG_DATA_CMD])
        waketime = self.reactor.monotonic() + SAMPLING_CYCLE_MS/1000.
        self.active_read = True
        self.timer = self.reactor.register_timer(self._do_read, waketime)


    def _stop_read(self):
        """Stop the current sensor read operation."""
        if self.active_read:
            self.reactor.unregister_timer(self.timer)
            self.active_read = False
            self.read_cycle = 0
            self.timer = None
            logging.info("XGZP6847D sensor read stopped.")
        else:
            logging.warning("No active XGZP6847D sensor read to stop.")
    # ---------------------------------------------------------------------
    # GCode Command Handlers
    # ---------------------------------------------------------------------
    def cmd_do_PT_measure(self, gcmd):
        """Start a pressure and temperature measurement cycle on the XGZP6847D sensor."""
        if self.active_read:
            logging.warning("cmd:XGZP6847D sensor read already in progress.")
            gcmd.respond_info("cmd:XGZP6847D sensor read already in progress.")
            return
        self._read_sensor_data()
        gcmd.respond_info("XGZP6847D sensor measurement started.")
    
    def cmd_tell_readings(self, gcmd):
        # Read pressure and temperature from the sensor
        if self.active_read:
            logging.warning("XGZP6847D sensor read already in progress.")
            gcmd.respond_info("XGZP6847D sensor read already in progress.")
            return
        gcmd.respond_info(f"XGZP6847D sensor readings: Pressure={self.pressure} Pa, Temperature={self.temperature} C")
def load_config(config):
    return PumpPressureSensor(config)