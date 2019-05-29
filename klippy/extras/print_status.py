# Add 'RESPOND' and 'M118' commands for sending messages to the host
#
# Copyright (C) 2018  Alec Plumb <alec@etherwalker.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrintStatus:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.cmd_PRINT_STATUS_help = "Print the status of all printer objects."
        self.gcode.register_command('PRINT_STATUS', self.cmd_PRINT_STATUS, True, self.cmd_PRINT_STATUS_help)
    def cmd_PRINT_STATUS(self, params):
        reactor = self.printer.get_reactor()
        eventtime = reactor.monotonic()
        for cfg_name, obj in self.printer.lookup_objects():
            get_status = getattr(obj, "get_status", None)
            if callable(get_status):
                self.gcode.respond("'%s' : %s" %(cfg_name, str(get_status(eventtime))))
def load_config(config):
    return PrintStatus(config)
