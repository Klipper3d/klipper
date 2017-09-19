# Micro-controller clock synchronization
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, threading

MAX_CLOCK_DRIFT = 0.000100

class ClockSync:
    def __init__(self, reactor):
        self.reactor = reactor
        self.serial = None
        self.lock = threading.Lock()
        self.status_timer = self.reactor.register_timer(self._status_event)
        self.status_cmd = None
        self.last_clock = 0
        self.last_clock_time = self.last_clock_time_min = 0.
        self.min_freq = self.max_freq = 0.
    def connect(self, serial):
        self.serial = serial
        # Load initial last_clock/last_clock_time
        msgparser = serial.msgparser
        uptime_msg = msgparser.create_command('get_uptime')
        params = serial.send_with_response(uptime_msg, 'uptime')
        self.last_clock = (params['high'] << 32) | params['clock']
        self.last_clock_time = params['#receive_time']
        self.last_clock_time_min = params['#sent_time']
        clock_freq = msgparser.get_constant_float('CLOCK_FREQ')
        self.min_freq = clock_freq * (1. - MAX_CLOCK_DRIFT)
        self.max_freq = clock_freq * (1. + MAX_CLOCK_DRIFT)
        # Enable periodic get_status timer
        serial.register_callback(self._handle_status, 'status')
        self.status_cmd = msgparser.create_command('get_status')
        self.reactor.update_timer(self.status_timer, self.reactor.NOW)
    def connect_file(self, serial, pace=False):
        self.serial = serial
        est_freq = 1000000000000.
        if pace:
            est_freq = float(self.msgparser.config['CLOCK_FREQ'])
        self.min_freq = self.max_freq = est_freq
        self.last_clock = 0
        self.last_clock_time = self.reactor.monotonic()
        serial.set_clock_est(
            self.min_freq, self.last_clock_time, self.last_clock)
    def stats(self, eventtime):
        return "last_clock=%d last_clock_time=%.3f" % (
            self.last_clock, self.last_clock_time)
    def get_clock(self, eventtime):
        with self.lock:
            last_clock = self.last_clock
            last_clock_time = self.last_clock_time
            min_freq = self.min_freq
        return int(last_clock + (eventtime - last_clock_time) * min_freq)
    def translate_clock(self, raw_clock):
        with self.lock:
            last_clock = self.last_clock
        clock_diff = (last_clock - raw_clock) & 0xffffffff
        if clock_diff & 0x80000000:
            return last_clock + 0x100000000 - clock_diff
        return last_clock - clock_diff
    def get_last_clock(self):
        with self.lock:
            return self.last_clock, self.last_clock_time
    def _status_event(self, eventtime):
        self.serial.send(self.status_cmd)
        return eventtime + 1.0
    def _handle_status(self, params):
        sent_time = params['#sent_time']
        if not sent_time:
            return
        receive_time = params['#receive_time']
        clock = params['clock']
        with self.lock:
            # Extend clock to 64bit
            clock = (self.last_clock & ~0xffffffff) | clock
            if clock < self.last_clock:
                clock += 0x100000000
            # Calculate expected send time from clock and previous estimates
            clock_delta = clock - self.last_clock
            min_send_time = (self.last_clock_time_min
                             + clock_delta / self.max_freq)
            max_send_time = self.last_clock_time + clock_delta / self.min_freq
            # Calculate intersection of times
            min_time = max(min_send_time, sent_time)
            max_time = min(max_send_time, receive_time)
            if min_time > max_time:
                # No intersection - clock drift must be greater than expected
                new_min_freq, new_max_freq = self.min_freq, self.max_freq
                if min_send_time > receive_time:
                    new_max_freq = (
                        clock_delta / (receive_time - self.last_clock_time_min))
                else:
                    new_min_freq = (
                        clock_delta / (sent_time - self.last_clock_time))
                logging.warning(
                    "High clock drift! Now %.0f:%.0f was %.0f:%.0f" % (
                        new_min_freq, new_max_freq,
                        self.min_freq, self.max_freq))
                self.min_freq, self.max_freq = new_min_freq, new_max_freq
                min_time, max_time = sent_time, receive_time
            # Update variables
            self.last_clock = clock
            self.last_clock_time = max_time
            self.last_clock_time_min = min_time
            self.serial.set_clock_est(self.min_freq, max_time + 0.001, clock)
