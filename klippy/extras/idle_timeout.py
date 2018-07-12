# Support for disabling the printer on an idle timeout
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

DEFAULT_IDLE_GCODE = """
M84
"""

class IdleTimeout:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.toolhead = None
        self.last_timeout = 0.
        self.idle_timeout = config.getfloat('timeout', 600., above=0.)
        self.idle_script = config.get('gcode', DEFAULT_IDLE_GCODE)
    def printer_state(self, state):
        if state == 'ready':
            self.toolhead = self.printer.lookup_object('toolhead')
            reactor = self.printer.get_reactor()
            reactor.register_timer(self.timeout_handler, reactor.NOW)
    def run_idle_script(self):
        gcode = self.printer.lookup_object('gcode')
        for line in self.idle_script.split('\n'):
            try:
                res = gcode.process_batch(line)
            except:
                break
            if not res:
                # Raced with incoming g-code commands
                return
        self.last_timeout = self.toolhead.get_last_move_time()
    def timeout_handler(self, eventtime):
        info = self.toolhead.get_status(eventtime)
        print_time = info['print_time']
        status = info['status']
        if print_time <= self.last_timeout or status == 'Printing':
            return eventtime + self.idle_timeout
        estimated_print_time = info['estimated_print_time']
        elapsed_time = estimated_print_time - print_time
        if elapsed_time < self.idle_timeout:
            return eventtime + self.idle_timeout - elapsed_time
        self.run_idle_script()
        return eventtime + 1.

def load_config(config):
    return IdleTimeout(config)
