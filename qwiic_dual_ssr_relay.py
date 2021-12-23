# qwiic_dual_ssr_relay i2c based two Solid State Relay
#
# Copyright (C) 2021 Luke Parker <parsko@gmail.com>
#
#   Author(s): Gaston Williams
#   Luke stole much code from Gaston!  He get's all credit.
#   Please see:
#   https://www.sparkfun.com/products/15093
#   and
#   https://github.com/adafruit/circuitpython/releases
#   and
#   https://github.com/adafruit/Adafruit_CircuitPython_BusDevice
#
#   They get all the credit, I just translated  it  to work with
#   Klipper.
#

import time
import logging
from . import bus

'''
# This is  all from the sparkfun Library
# public constants
QWIIC_RELAY_ADDR = 0x0A

# private constants
_RELAY_OFF = 0x00
_RELAY_ON = 0x01
_RELAY_CHANGE_ADDRESS = 0x03
_RELAY_VERSION = 0x04
_RELAY_STATUS = 0x05
_RELAY_NOTHING_NEW = 0x99
_DEFAULT_NAME = "SparkFun Qwiic Relay"

# Define the register offsets of each relay
RELAY_ONE             = 1
RELAY_TWO             = 2
RELAY_THREE           = 3
RELAY_FOUR            = 4

#define register start positions
DUAL_QUAD_TOGGLE_BASE   = 0x00
STATUS_BASE             = 0x04
DUAL_QUAD_PWM_BASE      = 0x0F
_TURN_ALL_OFF        = 0x0A
_TURN_ALL_ON     = 0x0B
TOGGLE_ALL      = 0x0C
SINGLE_OFF = 0x00
SINGLE_ON = 0x01
STATUS_OFF      = 0'''

# class
class qwiic_dual_ssr_relay:
    """Klipper Python class for the Sparkfun QwiicRelay"""
    
    def __init__(self, config):#i2c, address=QWIIC_RELAY_ADDR, debug=False):
        logging.info("Initializing the Qwiic Relay")
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=QWIIC_RELAY_ADDR, default_speed=100000)
        
        self._write_command( 0x0A ) # Turn off all relays!
        self.relay1 = 0
        self.relay2 = 0
        self.relay1_initial = config.getint("initial_RELAY1",0)
        self.relay2_initial = config.getint("initial_RELAY2",0)

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("SET_QRELAY_ONE_ON"     , "QRELAY" , self.name , self.cmd_SET_QRELAY_ONE_ON     , desc="Turn relay 1 on")
        self.gcode.register_mux_command("SET_QRELAY_ONE_OFF"    , "QRELAY" , self.name , self.cmd_SET_QRELAY_ONE_OFF    , desc="Turn relay 1 off")
        self.gcode.register_mux_command("SET_QRELAY_ONE_TOGGLE" , "QRELAY" , self.name , self.cmd_SET_QRELAY_ONE_TOGGLE , desc="Toggle relay 1")
        self.gcode.register_mux_command("SET_QRELAY_TWO_ON"     , "QRELAY" , self.name , self.cmd_SET_QRELAY_TWO_ON     , desc="Turn relay 2 on")
        self.gcode.register_mux_command("SET_QRELAY_TWO_OFF"    , "QRELAY" , self.name , self.cmd_SET_QRELAY_TWO_OFF    , desc="Turn relay 2 off")
        self.gcode.register_mux_command("SET_QRELAY_TWO_TOGGLE" , "QRELAY" , self.name , self.cmd_SET_QRELAY_TWO_TOGGLE , desc="Toggle relay 2")
        self.gcode.register_mux_command("SET_QRELAY_ALL_ON"   , "QRELAY" , self.name , self.cmd_SET_QRELAY_ALL_ON   , desc="Turn all relays on")
        self.gcode.register_mux_command("SET_QRELAY_ALL_OFF"  , "QRELAY" , self.name , self.cmd_SET_QRELAY_ALL_OFF  , desc="Turn all relays off")
        self.gcode.register_mux_command("SET_QRELAY_ALL_TOGGLE" , "QRELAY" , self.name , self.cmd_SET_QRELAY_ALL_TOGGLE , desc="Toggle all relays")
        
        templog = "Relay1 initial value: "+str(self.relay1)
        logging.info(templog)
        templog = "Relay2 initial value: "+str(self.relay2)
        logging.info(templog)

        self.printer.add_object("qwiic_dual_ssr_relay " + self.name, self)
        self.printer.register_event_handler("klippy:connect", self.handle_connect)

        logging.info("Qwiic Relay initialized")
        
    ##  Klipper stuff
    def handle_connect(self):
        return self._init_qwiic_dual_ssr_relay()
    
    def setup_callback(self, cb):
        self._callback = cb
        
    def _init_qwiic_dual_ssr_relay(self):
        gcmd = 0
        version = self._read_command( 0x04 , 1 )
        status_now = self._read_command( 0x05 , 2 )
        logging.info( "Status:" + str(status_now) )
        self.cmd_SET_QRELAY_ALL_OFF
        self.relay1 = 0
        self.relay2 = 0
        # Set initial values for relays
        if self.relay1_initial == 1:
            self.cmd_SET_QRELAY_ONE_ON(gcmd)
        else:
            self.cmd_SET_QRELAY_ONE_OFF(gcmd)
        if self.relay2_initial == 1:
            self.cmd_SET_QRELAY_TWO_ON(gcmd)
        else:
            self.cmd_SET_QRELAY_TWO_OFF(gcmd)
        return version
    
    ##  Callable routines
    def cmd_SET_QRELAY_ONE_ON(self, gcmd):
        if self.relay1 == 0:
            self._write_command( 0x01 )
            self.relay1 = 1

    def cmd_SET_QRELAY_ONE_OFF(self, gcmd):
        if self.relay1 == 1:
            self._write_command( 0x01 )
            self.relay1 = 0
        
    def cmd_SET_QRELAY_ONE_TOGGLE(self, gcmd):
        if self.relay1 == 0:
            self.relay1 = 1
        else:
            self.relay1 = 0
        self._write_command( 0x01 )

    def cmd_SET_QRELAY_TWO_ON(self, gcmd):
        if self.relay2 == 0:
            self._write_command( 0x02 )
            self.relay2 = 1

    def cmd_SET_QRELAY_TWO_OFF(self, gcmd):
        if self.relay2 == 1:
            self._write_command( 0x02 )
            self.relay2 = 0
        
    def cmd_SET_QRELAY_TWO_TOGGLE(self, gcmd):
        if self.relay2 == 0:
            self.relay2 = 1
        else:
            self.relay2 = 0
        self._write_command( 0x02 )

    def cmd_SET_QRELAY_ALL_ON(self, gcmd):
        self._write_command( 0x0B )
        self.relay1 = 1
        self.relay2 = 1

    def cmd_SET_QRELAY_ALL_OFF(self, gcmd):
        self._write_command( 0x0A )
        self.relay1 = 0
        self.relay2 = 0
        
    def cmd_SET_QRELAY_ALL_TOGGLE(self, gcmd):
        if self.relay1 == 0:
            self.relay1 = 1
            self._write_command( 0x01 )
        else:
            self.relay1 = 0
            self._write_command( 0x01 )
        if self.relay2 == 0:
            self.relay2 = 1
            self._write_command( 0x02 )
        else:
            self.relay2 = 0
            self._write_command( 0x02 )

    #  I2C stuff, comm stuff
    def _read_command(self, command, count):
        self.i2c.i2c_write([command])
        result = []
        self.i2c.i2c_read(result,count)
        return result

    def _write_command(self, command):
        self.i2c.i2c_write([command & 0xFF])

    def _write_register(self, addr, value):
        self.i2c.i2c_write([addr & 0xFF, value & 0xFF])

    def get_status(self, eventtime):
        data = {'relay1':self.relay1 , 'relay2':self.relay2}
        return data

def load_config(config):
    return qwiic_dual_ssr_relay(config)

def load_config_prefix(config):
    return qwiic_dual_ssr_relay(config)
