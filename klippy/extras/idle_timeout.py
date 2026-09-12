# Support for disabling the printer on an idle timeout
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

DEFAULT_IDLE_GCODE = (
     """
{% if 'heaters' in printer %}
    TURN_OFF_HEATERS
{% endif %}
M84
"""
)

PIN_MIN_TIME = 0.100
READY_TIMEOUT = .500

class IdleTimeout:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object('gcode')
        self.toolhead = self.timeout_timer = None
        self.printer.register_event_handler(
            "klippy:ready",
            self.handle_ready,
        )
        self.idle_timeout = config.getfloat(
            'timeout',
            600.,
            above=0.,
        )
        # New: verbose debug and robust delayed_gcode-agnostic mode
        self.verbose_debug = config.getboolean(
            'verbose_debug',
            False,
        )
        self.robust_delayed_gcode = config.getboolean(
            'robust_delayed_gcode',
            False,
        )
        gcode_macro = self.printer.load_object(
            config,
            'gcode_macro',
        )
        self.idle_gcode = gcode_macro.load_template(
            config,
            'gcode',
            DEFAULT_IDLE_GCODE,
        )
        self.gcode.register_command(
            'SET_IDLE_TIMEOUT',
            self.cmd_SET_IDLE_TIMEOUT,
            desc=self.cmd_SET_IDLE_TIMEOUT_help,
        )
        self.state = "Idle"
        self.last_print_start_systime = 0.
        # Track when Ready state entered
        self.last_idle_start_time = None
    def get_status(self, eventtime):
        printing_time = 0.
        idle_time_remaining = 0.0
        debug_vals = {}
        if self.state == "Printing":
            printing_time = eventtime - self.last_print_start_systime
        if self.state == "Ready":
            # Use wall-clock idle start time for robust countdown
            if self.last_idle_start_time is not None:
                idle_time = eventtime - self.last_idle_start_time
                idle_time_remaining = max(0.0, self.idle_timeout - idle_time)
                try:
                    print_time, est_print_time, lookahead_empty = (
                        self.toolhead.check_busy(eventtime)
                    )
                except Exception as e:
                    print_time = est_print_time = 0.0
                    lookahead_empty = True
                    if self.verbose_debug:
                        logging.error(
                            "[IdleTimeout] Exception in get_status "
                            "toolhead.check_busy: {}".format(e))
                debug_vals = {
                    'print_time': print_time,
                    'est_print_time': est_print_time,
                    'lookahead_empty': lookahead_empty,
                    'idle_time': idle_time,
                    'idle_time_remaining': idle_time_remaining,
                    'last_idle_start_time': self.last_idle_start_time,
                }
            else:
                idle_time_remaining = self.idle_timeout
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] get_status called: "
                "state={}, ".format(self.state) +
                "printing_time={}, ".format(printing_time) +
                "idle_timeout={}, ".format(self.idle_timeout) +
                "idle_time_remaining={}, ".format(idle_time_remaining) +
                "robust_delayed_gcode={}, ".format(self.robust_delayed_gcode) +
                "debug_vals={}".format(debug_vals)
            )
        status = {
            "state": self.state,
            "printing_time": printing_time,
            "idle_timeout": self.idle_timeout,
            "idle_time_remaining": idle_time_remaining,
            "robust_delayed_gcode": self.robust_delayed_gcode,
            "verbose_debug": self.verbose_debug,
        }
        if self.verbose_debug:
            status["debug_vals"] = debug_vals
        return status
    def handle_ready(self):
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] handle_ready called"
            )
        self.toolhead = self.printer.lookup_object('toolhead')
        self.timeout_timer = self.reactor.register_timer(
            self.timeout_handler,
        )
        self.printer.register_event_handler(
            "toolhead:sync_print_time",
            self.handle_sync_print_time,
        )
    def transition_idle_state(self, eventtime):
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] transition_idle_state called at {}, "
                "current state={}".format(eventtime, self.state)
            )
        self.state = "Printing"
        try:
            script = self.idle_gcode.render()
            self.gcode.run_script(script)
        except:
            logging.exception("idle timeout gcode execution")
            self.state = "Ready"
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Exception in idle_gcode, "
                    "state set to Ready"
                )
            return eventtime + 1.
        print_time = self.toolhead.get_last_move_time()
        self.state = "Idle"
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] State transitioned to Idle, "
                "print_time={}".format(print_time)
            )
        self.printer.send_event(
            "idle_timeout:idle",
            print_time,
        )
        return self.reactor.NEVER
    def check_idle_timeout(self, eventtime):
        print_time, est_print_time, lookahead_empty = (
            self.toolhead.check_busy(
                eventtime,
            )
        )
        idle_time = est_print_time - print_time
        if self.verbose_debug:
            logging.debug(
                ("[IdleTimeout] check_idle_timeout: "
                 "eventtime={}, ".format(eventtime)
                 + "print_time={}, ".format(print_time)
                 + "est_print_time={}, ".format(est_print_time)
                 + "lookahead_empty={}, ".format(lookahead_empty)
                 + "idle_time={}, ".format(idle_time)
                 + "state={}".format(self.state))
            )
        if not lookahead_empty or idle_time < 1.:
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Toolhead busy, "
                    "delaying idle timeout check"
                )
            return eventtime + self.idle_timeout
        if idle_time < self.idle_timeout:
            if self.verbose_debug:
                logging.debug(
                    ("[IdleTimeout] Idle time {} < idle_timeout {} , waiting"
                     .format(idle_time, self.idle_timeout))
                )
            return eventtime + self.idle_timeout - idle_time
        if self.gcode.get_mutex().test():
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Gcode class busy, "
                    "delaying idle timeout check"
                )
            return eventtime + 1.
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] Idle timeout elapsed, "
                "transitioning idle state"
            )
        return self.transition_idle_state(eventtime)
    def timeout_handler(self, eventtime):
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] timeout_handler called: eventtime={}, "
                "state={}".format(eventtime, self.state)
            )
        if self.printer.is_shutdown():
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Printer is shutdown, "
                    "returning NEVER"
                )
            return self.reactor.NEVER
        if self.state == "Ready":
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] State is Ready, "
                    "checking idle timeout"
                )
            # Set last_idle_start_time if just entered Ready
            if self.last_idle_start_time is None:
                self.last_idle_start_time = eventtime
            return self.check_idle_timeout(eventtime)
        print_time, est_print_time, lookahead_empty = (
            self.toolhead.check_busy(
                eventtime,
            )
        )
        buffer_time = min(2., print_time - est_print_time)
        if self.verbose_debug:
            logging.debug(
                ("[IdleTimeout] timeout_handler: "
                 "print_time={},\n".format(print_time)
                 + "  est_print_time={},\n".format(est_print_time)
                 + "  lookahead_empty={},\n".format(lookahead_empty)
                 + "  buffer_time={}".format(buffer_time))
            )
        if not lookahead_empty:
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Toolhead busy, "
                    "delaying ready state transition"
                )
            return eventtime + READY_TIMEOUT + max(0., buffer_time)
        if buffer_time > -READY_TIMEOUT:
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Buffer time > -READY_TIMEOUT, "
                    "waiting"
                )
            return eventtime + READY_TIMEOUT + buffer_time
        if self.gcode.get_mutex().test():
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Gcode class busy, "
                    "delaying ready state transition"
                )
            return eventtime + READY_TIMEOUT
        self.state = "Ready"
        self.last_idle_start_time = eventtime  # Set idle start time
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] State transitioned to Ready, "
                "sending event idle_timeout:ready"
            )
        self.printer.send_event(
            "idle_timeout:ready",
            est_print_time + PIN_MIN_TIME,
        )
        return eventtime + self.idle_timeout
    def handle_sync_print_time(self, curtime, print_time, est_print_time):
        # Only set state to Printing if toolhead is actually busy
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] handle_sync_print_time called: "
                "curtime={}, ".format(curtime) +
                "print_time={}, ".format(print_time) +
                "est_print_time={}, ".format(est_print_time) +
                "state={}, ".format(self.state) +
                "robust_delayed_gcode={}".format(self.robust_delayed_gcode)
            )
        # Check if toolhead is busy
        try:
            th_print_time, th_est_print_time, th_lookahead_empty = (
                self.toolhead.check_busy(curtime)
            )
        except Exception as e:
            if self.verbose_debug:
                logging.error(
                    "[IdleTimeout] Exception in check_busy: {}".format(e))
            th_lookahead_empty = True
        # If robust_delayed_gcode is enabled, only transition to Printing
        # if toolhead is busy
        if self.robust_delayed_gcode and th_lookahead_empty:
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Toolhead not busy (lookahead empty), "
                    "not transitioning to Printing state."
                )
            return
        if self.state == "Printing":
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] Already in Printing state, "
                    "returning"
                )
            return
        self.state = "Printing"
        # Reset idle timer when printing resumes
        self.last_idle_start_time = None
        self.last_print_start_systime = curtime
        check_time = READY_TIMEOUT + print_time - est_print_time
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] State transitioned to Printing, "
                "updating timer, check_time={}".format(check_time)
            )
        self.reactor.update_timer(
            self.timeout_timer,
            curtime + check_time,
        )
        self.printer.send_event(
            "idle_timeout:printing",
            est_print_time + PIN_MIN_TIME,
        )
    cmd_SET_IDLE_TIMEOUT_help = "Set the idle timeout in seconds"
    def cmd_SET_IDLE_TIMEOUT(self, gcmd):
        timeout = gcmd.get_float('TIMEOUT', self.idle_timeout, above=0.)
        self.idle_timeout = timeout
        if self.verbose_debug:
            logging.debug(
                "[IdleTimeout] cmd_SET_IDLE_TIMEOUT called: "
                "timeout set to {}".format(timeout)
            )
        gcmd.respond_info(
            "idle_timeout: Timeout set to %.2f s" % (timeout,),
        )
        if self.state == "Ready":
            checktime = self.reactor.monotonic() + timeout
            if self.verbose_debug:
                logging.debug(
                    "[IdleTimeout] State is Ready, "
                    "updating timer to {}".format(checktime)
                )
            self.reactor.update_timer(
                self.timeout_timer,
                checktime,
            )

def load_config(config):
    return IdleTimeout(config)
