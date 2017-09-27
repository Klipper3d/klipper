# Micro-controller clock synchronization
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, threading

COMM_TIMEOUT = 3.5
RTT_AGE = .000010 / (60. * 60.)

class ClockSync:
    def __init__(self, reactor):
        self.reactor = reactor
        self.serial = None
        self.status_timer = self.reactor.register_timer(self._status_event)
        self.status_cmd = None
        self.mcu_freq = 0.
        self.last_clock = 0
        self.min_half_rtt = 999999999.9
        self.min_half_rtt_time = 0.
        self.clock_est = self.prev_est = (0., 0, 0.)
        self.min_freq = 0.
        self.last_clock_fast = False
    def connect(self, serial):
        self.serial = serial
        msgparser = serial.msgparser
        self.mcu_freq = msgparser.get_constant_float('CLOCK_FREQ')
        # Load initial clock and frequency
        uptime_msg = msgparser.create_command('get_uptime')
        params = serial.send_with_response(uptime_msg, 'uptime')
        self.last_clock = clock = (params['high'] << 32) | params['clock']
        new_time = .5 * (params['#sent_time'] + params['#receive_time'])
        self.clock_est = self.prev_est = (new_time, clock, self.mcu_freq)
        self.min_freq = self.mcu_freq
        # Enable periodic get_status timer
        self.status_cmd = msgparser.create_command('get_status')
        for i in range(8):
            params = serial.send_with_response(self.status_cmd, 'status')
            self._handle_status(params)
            self.reactor.pause(0.100)
        serial.register_callback(self._handle_status, 'status')
        self.reactor.update_timer(self.status_timer, self.reactor.NOW)
    def connect_file(self, serial, pace=False):
        self.serial = serial
        self.mcu_freq = serial.msgparser.get_constant_float('CLOCK_FREQ')
        freq = 1000000000000.
        if pace:
            freq = self.mcu_freq
        serial.set_clock_est(freq, self.reactor.monotonic(), 0)
    # mcu clock querying
    def _status_event(self, eventtime):
        self.serial.send(self.status_cmd)
        return eventtime + 1.0
    def _handle_status(self, params):
        # Extend clock to 64bit
        clock32 = params['clock']
        last_clock = self.last_clock
        clock = (last_clock & ~0xffffffff) | clock32
        if clock < last_clock:
            clock += 0x100000000
        self.last_clock = clock
        # Check if this is the best round-trip-time seen so far
        sent_time = params['#sent_time']
        if not sent_time:
            return
        receive_time = params['#receive_time']
        half_rtt = .5 * (receive_time - sent_time)
        aged_rtt = (sent_time - self.min_half_rtt_time) * RTT_AGE
        if half_rtt < self.min_half_rtt + aged_rtt:
            self.min_half_rtt = half_rtt
            self.min_half_rtt_time = sent_time
            logging.debug("new minimum rtt=%.6f (%d)", half_rtt, self.mcu_freq)
        # Calculate expected clock range from sent/receive time
        est_min_clock = self.get_clock(sent_time + self.min_half_rtt)
        est_max_clock = self.get_clock(receive_time - self.min_half_rtt)
        if clock >= est_min_clock and clock <= est_max_clock:
            # Sample inline with expectations
            return
        # Update estimated frequency based on latest sample
        if clock > est_max_clock:
            clock_fast = True
            new_time = receive_time - self.min_half_rtt
        else:
            clock_fast = False
            new_time = sent_time + self.min_half_rtt
        if clock_fast != self.last_clock_fast:
            self.prev_est = self.clock_est
            self.last_clock_fast = clock_fast
        new_freq = (self.prev_est[1] - clock) / (self.prev_est[0] - new_time)
        self.min_freq = min(self.min_freq, new_freq)
        self.serial.set_clock_est(
            self.min_freq, new_time + self.min_half_rtt + 0.001, clock)
        self.clock_est = (new_time, clock, new_freq)
    # clock frequency conversions
    def print_time_to_clock(self, print_time):
        return int(print_time * self.mcu_freq)
    def clock_to_print_time(self, clock):
        return clock / self.mcu_freq
    def get_adjusted_freq(self):
        return self.mcu_freq
    # system time conversions
    def get_clock(self, eventtime):
        sample_time, clock, freq = self.clock_est
        return int(clock + (eventtime - sample_time) * freq)
    def estimated_print_time(self, eventtime):
        return self.clock_to_print_time(self.get_clock(eventtime))
    # misc commands
    def clock32_to_clock64(self, clock32):
        last_clock = self.last_clock
        clock_diff = (last_clock - clock32) & 0xffffffff
        if clock_diff & 0x80000000:
            return last_clock + 0x100000000 - clock_diff
        return last_clock - clock_diff
    def is_active(self, eventtime):
        print_time = self.estimated_print_time(eventtime)
        last_clock_print_time = self.clock_to_print_time(self.last_clock)
        return print_time < last_clock_print_time + COMM_TIMEOUT
    def stats(self, eventtime):
        sample_time, clock, freq = self.clock_est
        return "freq=%d" % (freq,)
    def calibrate_clock(self, print_time, eventtime):
        return (0., self.mcu_freq)

# Clock syncing code for secondary MCUs (whose clocks are sync'ed to a
# primary MCU)
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
    # clock frequency conversions
    def print_time_to_clock(self, print_time):
        adjusted_offset, adjusted_freq = self.clock_adj
        return int((print_time - adjusted_offset) * adjusted_freq)
    def clock_to_print_time(self, clock):
        adjusted_offset, adjusted_freq = self.clock_adj
        return clock / adjusted_freq + adjusted_offset
    def get_adjusted_freq(self):
        adjusted_offset, adjusted_freq = self.clock_adj
        return adjusted_freq
    # misc commands
    def stats(self, eventtime):
        adjusted_offset, adjusted_freq = self.clock_adj
        return "%s adj=%d" % (ClockSync.stats(self, eventtime), adjusted_freq)
    def calibrate_clock(self, print_time, eventtime):
        ser_time, ser_clock, ser_freq = self.main_sync.clock_est
        main_mcu_freq = self.main_sync.mcu_freq

        main_clock = (eventtime - ser_time) * ser_freq + ser_clock
        print_time = max(print_time, main_clock / main_mcu_freq)
        main_sync_clock = (print_time + 4.) * main_mcu_freq
        sync_time = ser_time + (main_sync_clock - ser_clock) / ser_freq

        print_clock = self.print_time_to_clock(print_time)
        sync_clock = self.get_clock(sync_time)
        adjusted_freq = .25 * (sync_clock - print_clock)
        adjusted_offset = print_time - print_clock / adjusted_freq

        self.clock_adj = (adjusted_offset, adjusted_freq)
        return self.clock_adj
