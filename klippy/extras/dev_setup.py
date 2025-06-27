#################################################################
# Klipper debug/development setup 
#################################################################

import logging
from . import bus

# -------------------------------------------------------------------------
# Dev Setup Class
# -------------------------------------------------------------------------
class dev_setup:

    def __init__(self, config):

        # Printer and pin setup
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.fakeHome = config.getboolean('fake_home', False, False)
  
    # ---------------------------------------------------------------------
    # Event Handlers
    # ---------------------------------------------------------------------
    def _handle_on_ready(self):
        if self.fakeHome:
            logging.info("Fake home enabled, running FAKE_HOME script.")
            self.gcode.run_script("SET_KINEMATIC_POSITION X=0 Y=0 Z=199")

  

# -------------------------------------------------------------------------
# Module Load Function
# -------------------------------------------------------------------------
def load_config(config):
    return dev_setup(config)