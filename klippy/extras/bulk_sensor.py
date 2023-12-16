# Tools for reading bulk sensor data from the mcu
#
# Copyright (C) 2020-2023  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import threading

API_UPDATE_INTERVAL = 0.500

# Helper to periodically transmit data to a set of API clients
class APIDumpHelper:
    def __init__(self, printer, data_cb, startstop_cb=None,
                 update_interval=API_UPDATE_INTERVAL):
        self.printer = printer
        self.data_cb = data_cb
        if startstop_cb is None:
            startstop_cb = (lambda is_start: None)
        self.startstop_cb = startstop_cb
        self.is_started = False
        self.update_interval = update_interval
        self.update_timer = None
        self.clients = {}
    def _stop(self):
        self.clients.clear()
        reactor = self.printer.get_reactor()
        reactor.unregister_timer(self.update_timer)
        self.update_timer = None
        if not self.is_started:
            return reactor.NEVER
        try:
            self.startstop_cb(False)
        except self.printer.command_error as e:
            logging.exception("API Dump Helper stop callback error")
            self.clients.clear()
        self.is_started = False
        if self.clients:
            # New client started while in process of stopping
            self._start()
        return reactor.NEVER
    def _start(self):
        if self.is_started:
            return
        self.is_started = True
        try:
            self.startstop_cb(True)
        except self.printer.command_error as e:
            logging.exception("API Dump Helper start callback error")
            self.is_started = False
            self.clients.clear()
            raise
        reactor = self.printer.get_reactor()
        systime = reactor.monotonic()
        waketime = systime + self.update_interval
        self.update_timer = reactor.register_timer(self._update, waketime)
    def add_client(self, web_request):
        cconn = web_request.get_client_connection()
        template = web_request.get_dict('response_template', {})
        self.clients[cconn] = template
        self._start()
    def add_internal_client(self):
        cconn = InternalDumpClient()
        self.clients[cconn] = {}
        self._start()
        return cconn
    def _update(self, eventtime):
        try:
            msg = self.data_cb(eventtime)
        except self.printer.command_error as e:
            logging.exception("API Dump Helper data callback error")
            return self._stop()
        if not msg:
            return eventtime + self.update_interval
        for cconn, template in list(self.clients.items()):
            if cconn.is_closed():
                del self.clients[cconn]
                if not self.clients:
                    return self._stop()
                continue
            tmp = dict(template)
            tmp['params'] = msg
            cconn.send(tmp)
        return eventtime + self.update_interval

# An "internal webhooks" wrapper for using APIDumpHelper internally
class InternalDumpClient:
    def __init__(self):
        self.msgs = []
        self.is_done = False
    def get_messages(self):
        return self.msgs
    def finalize(self):
        self.is_done = True
    def is_closed(self):
        return self.is_done
    def send(self, msg):
        self.msgs.append(msg)
        if len(self.msgs) >= 10000:
            # Avoid filling up memory with too many samples
            self.finalize()

# Helper class to store incoming messages in a queue
class BulkDataQueue:
    def __init__(self, mcu, msg_name, oid):
        # Measurement storage (accessed from background thread)
        self.lock = threading.Lock()
        self.raw_samples = []
        # Register callback with mcu
        mcu.register_response(self._handle_data, msg_name, oid)
    def _handle_data(self, params):
        with self.lock:
            self.raw_samples.append(params)
    def pull_samples(self):
        with self.lock:
            raw_samples = self.raw_samples
            self.raw_samples = []
        return raw_samples
    def clear_samples(self):
        self.pull_samples()

# Helper class for chip clock synchronization via linear regression
class ClockSyncRegression:
    def __init__(self, mcu, chip_clock_smooth, decay = 1. / 20.):
        self.mcu = mcu
        self.chip_clock_smooth = chip_clock_smooth
        self.decay = decay
        self.last_chip_clock = self.last_exp_mcu_clock = 0.
        self.mcu_clock_avg = self.mcu_clock_variance = 0.
        self.chip_clock_avg = self.chip_clock_covariance = 0.
    def reset(self, mcu_clock, chip_clock):
        self.mcu_clock_avg = self.last_mcu_clock = mcu_clock
        self.chip_clock_avg = chip_clock
        self.mcu_clock_variance = self.chip_clock_covariance = 0.
        self.last_chip_clock = self.last_exp_mcu_clock = 0.
    def update(self, mcu_clock, chip_clock):
        # Update linear regression
        decay = self.decay
        diff_mcu_clock = mcu_clock - self.mcu_clock_avg
        self.mcu_clock_avg += decay * diff_mcu_clock
        self.mcu_clock_variance = (1. - decay) * (
            self.mcu_clock_variance + diff_mcu_clock**2 * decay)
        diff_chip_clock = chip_clock - self.chip_clock_avg
        self.chip_clock_avg += decay * diff_chip_clock
        self.chip_clock_covariance = (1. - decay) * (
            self.chip_clock_covariance + diff_mcu_clock*diff_chip_clock*decay)
    def set_last_chip_clock(self, chip_clock):
        base_mcu, base_chip, inv_cfreq = self.get_clock_translation()
        self.last_chip_clock = chip_clock
        self.last_exp_mcu_clock = base_mcu + (chip_clock-base_chip) * inv_cfreq
    def get_clock_translation(self):
        inv_chip_freq = self.mcu_clock_variance / self.chip_clock_covariance
        if not self.last_chip_clock:
            return self.mcu_clock_avg, self.chip_clock_avg, inv_chip_freq
        # Find mcu clock associated with future chip_clock
        s_chip_clock = self.last_chip_clock + self.chip_clock_smooth
        scdiff = s_chip_clock - self.chip_clock_avg
        s_mcu_clock = self.mcu_clock_avg + scdiff * inv_chip_freq
        # Calculate frequency to converge at future point
        mdiff = s_mcu_clock - self.last_exp_mcu_clock
        s_inv_chip_freq = mdiff / self.chip_clock_smooth
        return self.last_exp_mcu_clock, self.last_chip_clock, s_inv_chip_freq
    def get_time_translation(self):
        base_mcu, base_chip, inv_cfreq = self.get_clock_translation()
        clock_to_print_time = self.mcu.clock_to_print_time
        base_time = clock_to_print_time(base_mcu)
        inv_freq = clock_to_print_time(base_mcu + inv_cfreq) - base_time
        return base_time, base_chip, inv_freq

MAX_BULK_MSG_SIZE = 52

# Handle common periodic chip status query responses
class ChipClockUpdater:
    def __init__(self, clock_sync, bytes_per_sample):
        self.clock_sync = clock_sync
        self.bytes_per_sample = bytes_per_sample
        self.samples_per_block = MAX_BULK_MSG_SIZE // bytes_per_sample
        self.mcu = clock_sync.mcu
        self.last_sequence = self.max_query_duration = 0
        self.last_limit_count = 0
    def get_last_sequence(self):
        return self.last_sequence
    def get_last_limit_count(self):
        return self.last_limit_count
    def clear_duration_filter(self):
        self.max_query_duration = 1 << 31
    def note_start(self, reqclock):
        self.last_sequence = 0
        self.last_limit_count = 0
        self.clock_sync.reset(reqclock, 0)
        self.clear_duration_filter()
    def update_clock(self, params):
        # Handle a status response message of the form:
        #   adxl345_status oid=x clock=x query_ticks=x next_sequence=x
        #     buffered=x fifo=x limit_count=x
        fifo = params['fifo']
        mcu_clock = self.mcu.clock32_to_clock64(params['clock'])
        seq_diff = (params['next_sequence'] - self.last_sequence) & 0xffff
        self.last_sequence += seq_diff
        buffered = params['buffered']
        lc_diff = (params['limit_count'] - self.last_limit_count) & 0xffff
        self.last_limit_count += lc_diff
        duration = params['query_ticks']
        if duration > self.max_query_duration:
            # Skip measurement as a high query time could skew clock tracking
            self.max_query_duration = max(2 * self.max_query_duration,
                                          self.mcu.seconds_to_clock(.000005))
            return
        self.max_query_duration = 2 * duration
        msg_count = (self.last_sequence * self.samples_per_block
                     + buffered // self.bytes_per_sample + fifo)
        # The "chip clock" is the message counter plus .5 for average
        # inaccuracy of query responses and plus .5 for assumed offset
        # of hardware processing time.
        chip_clock = msg_count + 1
        self.clock_sync.update(mcu_clock + duration // 2, chip_clock)
