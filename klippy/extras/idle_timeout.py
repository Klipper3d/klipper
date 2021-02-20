# Support for disabling the printer on an idle timeout
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

DEFAULT_IDLE_GCODE = """
TURN_OFF_HEATERS
M84
"""

PIN_MIN_TIME = 0.100
READY_TIMEOUT = .500

class IdleTimeout:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object('gcode')
        self.toolhead = self.timeout_timer = None
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.idle_timeout = config.getfloat('timeout', 600., above=0.)
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.idle_gcode = gcode_macro.load_template(config, 'gcode',
                                                    DEFAULT_IDLE_GCODE)
        self.gcode.register_command('SET_IDLE_TIMEOUT',
                                    self.cmd_SET_IDLE_TIMEOUT,
                                    desc=self.cmd_SET_IDLE_TIMEOUT_help)
        self.state = "Idle"
        self.last_print_start_systime = 0.
    def get_status(self, eventtime):
        printing_time = 0.
        if self.state == "Printing":
            printing_time = eventtime - self.last_print_start_systime
        return { "state": self.state, "printing_time": printing_time }
    def handle_ready(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        self.timeout_timer = self.reactor.register_timer(self.timeout_handler)
        self.printer.register_event_handler("toolhead:sync_print_time",
                                            self.handle_sync_print_time)
    def transition_idle_state(self, eventtime):
        self.state = "Printing"
        try:
            script = self.idle_gcode.render()
            res = self.gcode.run_script(script)
        except:
            logging.exception("idle timeout gcode execution")
            self.state = "Ready"
            return eventtime + 1.
        print_time = self.toolhead.get_last_move_time()
        self.state = "Idle"
        self.printer.send_event("idle_timeout:idle", print_time)
        return self.reactor.NEVER
    def check_idle_timeout(self, eventtime):
        # Make sure toolhead class isn't busy
        print_time, est_print_time, lookahead_empty = self.toolhead.check_busy(
            eventtime)
        idle_time = est_print_time - print_time
        if not lookahead_empty or idle_time < 1.:
            # Toolhead is busy
            return eventtime + self.idle_timeout
        if idle_time < self.idle_timeout:
            # Wait for idle timeout
            return eventtime + self.idle_timeout - idle_time
        if self.gcode.get_mutex().test():
            # Gcode class busy
            return eventtime + 1.
        # Idle timeout has elapsed
        return self.transition_idle_state(eventtime)
    def timeout_handler(self, eventtime):
        if self.state == "Ready":
            return self.check_idle_timeout(eventtime)
        # Check if need to transition to "ready" state
        print_time, est_print_time, lookahead_empty = self.toolhead.check_busy(
            eventtime)
        buffer_time = min(2., print_time - est_print_time)
        if not lookahead_empty:
            # Toolhead is busy
            return eventtime + READY_TIMEOUT + max(0., buffer_time)
        if buffer_time > -READY_TIMEOUT:
            # Wait for ready timeout
            return eventtime + READY_TIMEOUT + buffer_time
        if self.gcode.get_mutex().test():
            # Gcode class busy
            return eventtime + READY_TIMEOUT
        # Transition to "ready" state
        self.state = "Ready"
        self.printer.send_event("idle_timeout:ready",
                                est_print_time + PIN_MIN_TIME)
        return eventtime + self.idle_timeout
    def handle_sync_print_time(self, curtime, print_time, est_print_time):
        if self.state == "Printing":
            return
        # Transition to "printing" state
        self.state = "Printing"
        self.last_print_start_systime = curtime
        check_time = READY_TIMEOUT + print_time - est_print_time
        self.reactor.update_timer(self.timeout_timer, curtime + check_time)
        self.printer.send_event("idle_timeout:printing",
                                est_print_time + PIN_MIN_TIME)
    cmd_SET_IDLE_TIMEOUT_help = "Set the idle timeout in seconds"
    def cmd_SET_IDLE_TIMEOUT(self, gcmd):
        timeout = gcmd.get_float('TIMEOUT', self.idle_timeout, above=0.)
        self.idle_timeout = timeout
        gcmd.respond_info("idle_timeout: Timeout set to %.2f s" % (timeout,))
        if self.state == "Ready":
            checktime = self.reactor.monotonic() + timeout
            self.reactor.update_timer(self.timeout_timer, checktime)

def load_config(config):
    return IdleTimeout(config)
