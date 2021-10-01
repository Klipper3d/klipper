# Helper script to control WLED via JSON API
#
# Copyright (C) 2019-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import requests

class WLED:
    def __init__(self, config):
        self.printer = config.get_printer()
        name = config.get_name().split()[1]
        # Read config
        self.verbose = config.get('verbose', False)
        self.url = config.get('url')
        self.json_on = config.get('on', {'on': True})
        self.json_off = config.get('off', {'on': False})
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command('WLED_JSON', 'WLED', name,
                            self.cmd_WLED_JSON, desc=self.cmd_WLED_JSON_help)
        self.gcode.register_mux_command('WLED_ON', 'WLED', name,
                                    self.cmd_WLED_ON, desc=self.cmd_WLED_ON_help)
        self.gcode.register_mux_command('WLED_OFF', 'WLED', name,
                            self.cmd_WLED_OFF, desc=self.cmd_WLED_OFF_help)
                            
    cmd_WLED_JSON_help = 'Send json data to WLED'
    def cmd_WLED_JSON(self, gcmd):
        json = gcmd.get('JSON')
        
        try:
            if self.verbose:
                self.gcode.respond_info("WLED: Sending - " + json)
            requests.post(self.url, json=eval(json))
        except:
            raise self.gcode.error('Incorrect json syntax')
        
    cmd_WLED_ON_help = 'Turn on WLED'
    def cmd_WLED_ON(self, gcmd):
        if self.verbose:
            self.gcode.respond_info("WLED: Turning on - " + self.json_on)
        requests.post(self.url, json=eval(self.json_on))

    cmd_WLED_OFF_help = 'Turn off WLED'
    def cmd_WLED_OFF(self, gcmd):
        if self.verbose:
            self.gcode.respond_info("WLED: Turning off " + self.json_off)
        requests.post(self.url, json=eval(self.json_off))

def load_config_prefix(config):
    return WLED(config)
