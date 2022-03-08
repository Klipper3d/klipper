# Utility for querying the current state of all endstops
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class QueryEndstops:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.endstops = []
        self.last_state = []
        # Register webhook if server is available
        webhooks = self.printer.lookup_object('webhooks')
        webhooks.register_endpoint(
            "query_endstops/status", self._handle_web_request)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("QUERY_ENDSTOPS", self.cmd_QUERY_ENDSTOPS,
                               desc=self.cmd_QUERY_ENDSTOPS_help)
        gcode.register_command("M119", self.cmd_QUERY_ENDSTOPS)
        gcode.register_command("MONITOR_ENDSTOPS", self.cmd_MONITOR_ENDSTOPS,
                                desc=self.cmd_MONITOR_ENDSTOPS_help)
    def register_endstop(self, mcu_endstop, name):
        self.endstops.append((mcu_endstop, name))
    def get_status(self, eventtime):
        return {'last_query': {name: value for name, value in self.last_state}}
    def _handle_web_request(self, web_request):
        gc_mutex = self.printer.lookup_object('gcode').get_mutex()
        toolhead = self.printer.lookup_object('toolhead')
        with gc_mutex:
            print_time = toolhead.get_last_move_time()
            self.last_state = [(name, mcu_endstop.query_endstop(print_time))
                               for mcu_endstop, name in self.endstops]
        web_request.send({name: ["open", "TRIGGERED"][not not t]
                          for name, t in self.last_state})
    cmd_QUERY_ENDSTOPS_help = "Report on the status of each endstop"
    def cmd_QUERY_ENDSTOPS(self, gcmd):
        # Query the endstops
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.last_state = [(name, mcu_endstop.query_endstop(print_time))
                           for mcu_endstop, name in self.endstops]
        # Report results
        msg = " ".join(["%s:%s" % (name, ["open", "TRIGGERED"][not not t])
                        for name, t in self.last_state])
        gcmd.respond_raw(msg)
    cmd_MONITOR_ENDSTOPS_help = "Continously monitor status of endstops"
    def cmd_MONITOR_ENDSTOPS(self, gcmd):
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        params = gcmd.get_command_parameters().setdefault('DELAY', "0.005")
        delay = gcmd.get_float('DELAY')
        params = [e for e in gcmd.get("ENDSTOPS", "").split(",") if e != ""]
        if len(params) > 0:
            for endstop, name in self.endstops:
                if name in params:
                    endstop.start_monitoring(print_time, delay)
        else:
            for endstop, name in self.endstops:
                endstop.start_monitoring(print_time, delay)

def load_config(config):
    return QueryEndstops(config)
