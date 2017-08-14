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
        self.queries_pending = 0
        self.status_timer = self.reactor.register_timer(self._status_event)
        self.status_cmd = None
        self.mcu_freq = 0.
        self.lock = threading.Lock()
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
        self.mcu_freq = msgparser.get_constant_float('CLOCK_FREQ')
        self.min_freq = self.mcu_freq * (1. - MAX_CLOCK_DRIFT)
        self.max_freq = self.mcu_freq * (1. + MAX_CLOCK_DRIFT)
        # Enable periodic get_status timer
        serial.register_callback(self._handle_status, 'status')
        self.status_cmd = msgparser.create_command('get_status')
        self.reactor.update_timer(self.status_timer, self.reactor.NOW)
    def connect_file(self, serial, pace=False):
        self.serial = serial
        self.mcu_freq = serial.msgparser.get_constant_float('CLOCK_FREQ')
        est_freq = 1000000000000.
        if pace:
            est_freq = self.mcu_freq
        self.min_freq = self.max_freq = est_freq
        self.last_clock = 0
        self.last_clock_time = self.reactor.monotonic()
        serial.set_clock_est(
            self.min_freq, self.last_clock_time, self.last_clock)
    def stats(self, eventtime):
        return "last_clock=%d last_clock_time=%.3f" % (
            self.last_clock, self.last_clock_time)
    def is_active(self, eventtime):
        return self.queries_pending <= 4
    def calibrate_clock(self, print_time, eventtime):
        return (0., self.mcu_freq)
    def get_clock(self, eventtime):
        with self.lock:
            last_clock = self.last_clock
            last_clock_time = self.last_clock_time
            min_freq = self.min_freq
        return int(last_clock + (eventtime - last_clock_time) * min_freq)
    def clock32_to_clock64(self, clock32):
        with self.lock:
            last_clock = self.last_clock
        clock_diff = (last_clock - clock32) & 0xffffffff
        if clock_diff & 0x80000000:
            return last_clock + 0x100000000 - clock_diff
        return last_clock - clock_diff
    def print_time_to_clock(self, print_time):
        return int(print_time * self.mcu_freq)
    def clock_to_print_time(self, clock):
        return clock / self.mcu_freq
    def estimated_print_time(self, eventtime):
        return self.clock_to_print_time(self.get_clock(eventtime))
    def get_adjusted_freq(self):
        return self.mcu_freq
    def _status_event(self, eventtime):
        self.queries_pending += 1
        self.serial.send(self.status_cmd)
        return eventtime + 1.0
    def _handle_status(self, params):
        self.queries_pending = 0
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
                    "High clock drift! Now %.0f:%.0f was %.0f:%.0f",
                    new_min_freq, new_max_freq, self.min_freq, self.max_freq)
                self.min_freq, self.max_freq = new_min_freq, new_max_freq
                min_time, max_time = sent_time, receive_time
            # Update variables
            self.last_clock = clock
            self.last_clock_time = max_time
            self.last_clock_time_min = min_time
            self.serial.set_clock_est(self.min_freq, max_time + 0.001, clock)

# Clock synching code for secondary MCUs (whose clocks are sync'ed to
# a primary MCU)
class SecondarySync(ClockSync):
    def __init__(self, reactor, main_sync):
        ClockSync.__init__(self, reactor)
        self.main_sync = main_sync
        self.clock_adj = (0., 0.)
    def connect(self, serial):
        ClockSync.connect(self, serial)
        self.clock_adj = (0., self.mcu_freq)
        curtime = self.reactor.monotonic()
        main_print_time = self.main_sync.estimated_print_time(curtime)
        local_print_time = self.estimated_print_time(curtime)
        self.clock_adj = (main_print_time - local_print_time, self.mcu_freq)
        self.calibrate_clock(0., curtime)
    def connect_file(self, serial, pace=False):
        ClockSync.connect_file(self, serial, pace)
        self.clock_adj = (0., self.mcu_freq)
    def print_time_to_clock(self, print_time):
        adjusted_offset, adjusted_freq = self.clock_adj
        return int((print_time - adjusted_offset) * adjusted_freq)
    def clock_to_print_time(self, clock):
        adjusted_offset, adjusted_freq = self.clock_adj
        return clock / adjusted_freq + adjusted_offset
    def get_adjusted_freq(self):
        adjusted_offset, adjusted_freq = self.clock_adj
        return adjusted_freq
    def calibrate_clock(self, print_time, eventtime):
        #logging.debug("calibrate: %.3f: %.6f vs %.6f",
        #              eventtime,
        #              self.estimated_print_time(eventtime),
        #              self.main_sync.estimated_print_time(eventtime))
        with self.main_sync.lock:
            ser_clock = self.main_sync.last_clock
            ser_clock_time = self.main_sync.last_clock_time
            ser_freq = self.main_sync.min_freq
        main_mcu_freq = self.main_sync.mcu_freq

        main_clock = (eventtime - ser_clock_time) * ser_freq + ser_clock
        print_time = max(print_time, main_clock / main_mcu_freq)
        main_sync_clock = (print_time + 2.) * main_mcu_freq
        sync_time = ser_clock_time + (main_sync_clock - ser_clock) / ser_freq

        print_clock = self.print_time_to_clock(print_time)
        sync_clock = self.get_clock(sync_time)
        adjusted_freq = .5 * (sync_clock - print_clock)
        adjusted_offset = print_time - print_clock / adjusted_freq

        self.clock_adj = (adjusted_offset, adjusted_freq)
        return self.clock_adj
