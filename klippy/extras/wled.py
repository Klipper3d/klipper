# Helper script to adjust bed screws
#
# Copyright (C) 2019-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import requests

class BedScrews:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.state = None
        self.current_screw = 0
        self.accepted_screws = 0
        self.number_of_screws = 0
        # Read config
        self.url = config.get('url')
        # Register command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("WLED_ON",
                                    self.cmd_WLED_ON,
                                    desc=self.cmd_WLED_ON_help)
        self.gcode.register_command("WLED_OFF",
                            self.cmd_WLED_OFF,
                            desc=self.cmd_WLED_OFF_help)                            
    cmd_WLED_ON_help = "Turn on WLED"
    def cmd_WLED_ON(self, gcmd):
        requests.get(self.url + "&T=1")
    cmd_WLED_OFF_help = "Turn off WLED"
    def cmd_WLED_ON(self, gcmd):
        requests.get(self.url + "&T=0")

def load_config(config):
    return BedScrews(config)
