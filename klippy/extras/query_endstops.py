# Utility for querying the current state of all endstops
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class QueryEndstops:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.endstops = []
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("QUERY_ENDSTOPS", self.cmd_QUERY_ENDSTOPS,
                               desc=self.cmd_QUERY_ENDSTOPS_help)
        gcode.register_command("M119", self.cmd_QUERY_ENDSTOPS)
    def register_endstop(self, mcu_endstop, name):
        self.endstops.append((mcu_endstop, name))
    cmd_QUERY_ENDSTOPS_help = "Report on the status of each endstop"
    def cmd_QUERY_ENDSTOPS(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        # Query the endstops
        for mcu_endstop, name in self.endstops:
            mcu_endstop.query_endstop(print_time)
        out = []
        for mcu_endstop, name in self.endstops:
            out.append((name, mcu_endstop.query_endstop_wait()))
        # Report results
        msg = " ".join(["%s:%s" % (name, ["open", "TRIGGERED"][not not t])
                        for name, t in out])
        gcode = self.printer.lookup_object('gcode')
        gcode.respond(msg)

def load_config(config):
    return QueryEndstops(config)
