# Micro-controller clock synchronization
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math

RTT_AGE = .000010 / (60. * 60.)
DECAY = 1. / 30.
TRANSMIT_EXTRA = .001

class ClockSync:
    def __init__(self, reactor):
        self.reactor = reactor
        self.serial = None
        self.get_clock_timer = reactor.register_timer(self._get_clock_event)
        self.get_clock_cmd = self.cmd_queue = None
        self.queries_pending = 0
        self.mcu_freq = 1.
        self.last_clock = 0
        self.clock_est = (0., 0., 0.)
        # Minimum round-trip-time tracking
        self.min_half_rtt = 999999999.9
        self.min_rtt_time = 0.
        # Linear regression of mcu clock and system sent_time
        self.time_avg = self.time_variance = 0.
        self.clock_avg = self.clock_covariance = 0.
        self.prediction_variance = 0.
        self.last_prediction_time = 0.
    def connect(self, serial):
        self.serial = serial
        self.mcu_freq = serial.msgparser.get_constant_float('CLOCK_FREQ')
        # Load initial clock and frequency
        params = serial.send_with_response('get_uptime', 'uptime')
        self.last_clock = (params['high'] << 32) | params['clock']
        self.clock_avg = self.last_clock
        self.time_avg = params['#sent_time']
        self.clock_est = (self.time_avg, self.clock_avg, self.mcu_freq)
        self.prediction_variance = (.001 * self.mcu_freq)**2
        # Enable periodic get_clock timer
        for i in range(8):
            params = serial.send_with_response('get_clock', 'clock')
            self._handle_clock(params)
            self.reactor.pause(0.100)
        self.get_clock_cmd = serial.get_msgparser().create_command('get_clock')
        self.cmd_queue = serial.alloc_command_queue()
        serial.register_response(self._handle_clock, 'clock')
        self.reactor.update_timer(self.get_clock_timer, self.reactor.NOW)
    def connect_file(self, serial, pace=False):
        self.serial = serial
        self.mcu_freq = serial.msgparser.get_constant_float('CLOCK_FREQ')
        self.clock_est = (0., 0., self.mcu_freq)
        freq = 1000000000000.
        if pace:
            freq = self.mcu_freq
        serial.set_clock_est(freq, self.reactor.monotonic(), 0)
    # MCU clock querying (_handle_clock is invoked from background thread)
    def _get_clock_event(self, eventtime):
        self.serial.raw_send(self.get_clock_cmd, 0, 0, self.cmd_queue)
        self.queries_pending += 1
        # Use an unusual time for the next event so clock messages
        # don't resonate with other periodic events.
        return eventtime + .9839
    def _handle_clock(self, params):
        self.queries_pending = 0
        # Extend clock to 64bit
        last_clock = self.last_clock
        clock = (last_clock & ~0xffffffff) | params['clock']
        if clock < last_clock:
            clock += 0x100000000
        self.last_clock = clock
        # Check if this is the best round-trip-time seen so far
        sent_time = params['#sent_time']
        if not sent_time:
            return
        receive_time = params['#receive_time']
        half_rtt = .5 * (receive_time - sent_time)
        aged_rtt = (sent_time - self.min_rtt_time) * RTT_AGE
        if half_rtt < self.min_half_rtt + aged_rtt:
            self.min_half_rtt = half_rtt
            self.min_rtt_time = sent_time
            logging.debug("new minimum rtt %.3f: hrtt=%.6f freq=%d",
                          sent_time, half_rtt, self.clock_est[2])
        # Filter out samples that are extreme outliers
        exp_clock = ((sent_time - self.time_avg) * self.clock_est[2]
                     + self.clock_avg)
        clock_diff2 = (clock - exp_clock)**2
        if (clock_diff2 > 25. * self.prediction_variance
            and clock_diff2 > (.000500 * self.mcu_freq)**2):
            if clock > exp_clock and sent_time < self.last_prediction_time+10.:
                logging.debug("Ignoring clock sample %.3f:"
                              " freq=%d diff=%d stddev=%.3f",
                              sent_time, self.clock_est[2], clock - exp_clock,
                              math.sqrt(self.prediction_variance))
                return
            logging.info("Resetting prediction variance %.3f:"
                         " freq=%d diff=%d stddev=%.3f",
                         sent_time, self.clock_est[2], clock - exp_clock,
                         math.sqrt(self.prediction_variance))
            self.prediction_variance = (.001 * self.mcu_freq)**2
        else:
            self.last_prediction_time = sent_time
            self.prediction_variance = (
                (1. - DECAY) * (self.prediction_variance + clock_diff2 * DECAY))
        # Add clock and sent_time to linear regression
        diff_sent_time = sent_time - self.time_avg
        self.time_avg += DECAY * diff_sent_time
        self.time_variance = (1. - DECAY) * (
            self.time_variance + diff_sent_time**2 * DECAY)
        diff_clock = clock - self.clock_avg
        self.clock_avg += DECAY * diff_clock
        self.clock_covariance = (1. - DECAY) * (
            self.clock_covariance + diff_sent_time * diff_clock * DECAY)
        # Update prediction from linear regression
        new_freq = self.clock_covariance / self.time_variance
        pred_stddev = math.sqrt(self.prediction_variance)
        self.serial.set_clock_est(new_freq, self.time_avg + TRANSMIT_EXTRA,
                                  int(self.clock_avg - 3. * pred_stddev))
        self.clock_est = (self.time_avg + self.min_half_rtt,
                          self.clock_avg, new_freq)
        #logging.debug("regr %.3f: freq=%.3f d=%d(%.3f)",
        #              sent_time, new_freq, clock - exp_clock, pred_stddev)
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
    def estimate_clock_systime(self, reqclock):
        sample_time, clock, freq = self.clock_est
        return float(reqclock - clock)/freq + sample_time
    def estimated_print_time(self, eventtime):
        return self.clock_to_print_time(self.get_clock(eventtime))
    # misc commands
    def clock32_to_clock64(self, clock32):
        last_clock = self.last_clock
        clock_diff = (last_clock - clock32) & 0xffffffff
        if clock_diff & 0x80000000:
            return last_clock + 0x100000000 - clock_diff
        return last_clock - clock_diff
    def is_active(self):
        return self.queries_pending <= 4
    def dump_debug(self):
        sample_time, clock, freq = self.clock_est
        return ("clocksync state: mcu_freq=%d last_clock=%d"
                " clock_est=(%.3f %d %.3f) min_half_rtt=%.6f min_rtt_time=%.3f"
                " time_avg=%.3f(%.3f) clock_avg=%.3f(%.3f)"
                " pred_variance=%.3f" % (
                    self.mcu_freq, self.last_clock, sample_time, clock, freq,
                    self.min_half_rtt, self.min_rtt_time,
                    self.time_avg, self.time_variance,
                    self.clock_avg, self.clock_covariance,
                    self.prediction_variance))
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
        self.clock_adj = (0., 1.)
        self.last_sync_time = 0.
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
    def dump_debug(self):
        adjusted_offset, adjusted_freq = self.clock_adj
        return "%s clock_adj=(%.3f %.3f)" % (
            ClockSync.dump_debug(self), adjusted_offset, adjusted_freq)
    def stats(self, eventtime):
        adjusted_offset, adjusted_freq = self.clock_adj
        return "%s adj=%d" % (ClockSync.stats(self, eventtime), adjusted_freq)
    def calibrate_clock(self, print_time, eventtime):
        # Calculate: est_print_time = main_sync.estimatated_print_time()
        ser_time, ser_clock, ser_freq = self.main_sync.clock_est
        main_mcu_freq = self.main_sync.mcu_freq
        est_main_clock = (eventtime - ser_time) * ser_freq + ser_clock
        est_print_time = est_main_clock / main_mcu_freq
        # Determine sync1_print_time and sync2_print_time
        sync1_print_time = max(print_time, est_print_time)
        sync2_print_time = max(sync1_print_time + 4., self.last_sync_time,
                               print_time + 2.5 * (print_time - est_print_time))
        # Calc sync2_sys_time (inverse of main_sync.estimatated_print_time)
        sync2_main_clock = sync2_print_time * main_mcu_freq
        sync2_sys_time = ser_time + (sync2_main_clock - ser_clock) / ser_freq
        # Adjust freq so estimated print_time will match at sync2_print_time
        sync1_clock = self.print_time_to_clock(sync1_print_time)
        sync2_clock = self.get_clock(sync2_sys_time)
        adjusted_freq = ((sync2_clock - sync1_clock)
                         / (sync2_print_time - sync1_print_time))
        adjusted_offset = sync1_print_time - sync1_clock / adjusted_freq
        # Apply new values
        self.clock_adj = (adjusted_offset, adjusted_freq)
        self.last_sync_time = sync2_print_time
        return self.clock_adj
