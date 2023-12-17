# Tools for reading bulk sensor data from the mcu
#
# Copyright (C) 2020-2023  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import threading

# This "bulk sensor" module facilitates the processing of sensor chip
# measurements that do not require the host to respond with low
# latency.  This module helps collect these measurements into batches
# that are then processed periodically by the host code (as specified
# by BatchBulkHelper.batch_interval).  It supports the collection of
# thousands of sensor measurements per second.
#
# Processing measurements in batches reduces load on the mcu, reduces
# bandwidth to/from the mcu, and reduces load on the host.  It also
# makes it easier to export the raw measurements via the webhooks
# system (aka API Server).

BATCH_INTERVAL = 0.500

# Helper to process accumulated messages in periodic batches
class BatchBulkHelper:
    def __init__(self, printer, batch_cb, start_cb=None, stop_cb=None,
                 batch_interval=BATCH_INTERVAL):
        self.printer = printer
        self.batch_cb = batch_cb
        if start_cb is None:
            start_cb = (lambda: None)
        self.start_cb = start_cb
        if stop_cb is None:
            stop_cb = (lambda: None)
        self.stop_cb = stop_cb
        self.is_started = False
        self.batch_interval = batch_interval
        self.batch_timer = None
        self.client_cbs = []
        self.webhooks_start_resp = {}
    # Periodic batch processing
    def _start(self):
        if self.is_started:
            return
        self.is_started = True
        try:
            self.start_cb()
        except self.printer.command_error as e:
            logging.exception("BatchBulkHelper start callback error")
            self.is_started = False
            del self.client_cbs[:]
            raise
        reactor = self.printer.get_reactor()
        systime = reactor.monotonic()
        waketime = systime + self.batch_interval
        self.batch_timer = reactor.register_timer(self._proc_batch, waketime)
    def _stop(self):
        del self.client_cbs[:]
        self.printer.get_reactor().unregister_timer(self.batch_timer)
        self.batch_timer = None
        if not self.is_started:
            return
        try:
            self.stop_cb()
        except self.printer.command_error as e:
            logging.exception("BatchBulkHelper stop callback error")
            del self.client_cbs[:]
        self.is_started = False
        if self.client_cbs:
            # New client started while in process of stopping
            self._start()
    def _proc_batch(self, eventtime):
        try:
            msg = self.batch_cb(eventtime)
        except self.printer.command_error as e:
            logging.exception("BatchBulkHelper batch callback error")
            self._stop()
            return self.printer.get_reactor().NEVER
        if not msg:
            return eventtime + self.batch_interval
        for client_cb in list(self.client_cbs):
            res = client_cb(msg)
            if not res:
                # This client no longer needs updates - unregister it
                self.client_cbs.remove(client_cb)
                if not self.client_cbs:
                    self._stop()
                    return self.printer.get_reactor().NEVER
        return eventtime + self.batch_interval
    # Client registration
    def add_client(self, client_cb):
        self.client_cbs.append(client_cb)
        self._start()
    # Webhooks registration
    def _add_api_client(self, web_request):
        whbatch = BatchWebhooksClient(web_request)
        self.add_client(whbatch.handle_batch)
        web_request.send(self.webhooks_start_resp)
    def add_mux_endpoint(self, path, key, value, webhooks_start_resp):
        self.webhooks_start_resp = webhooks_start_resp
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint(path, key, value, self._add_api_client)

# A webhooks wrapper for use by BatchBulkHelper
class BatchWebhooksClient:
    def __init__(self, web_request):
        self.cconn = web_request.get_client_connection()
        self.template = web_request.get_dict('response_template', {})
    def handle_batch(self, msg):
        if self.cconn.is_closed():
            return False
        tmp = dict(self.template)
        tmp['params'] = msg
        self.cconn.send(tmp)
        return True

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


######################################################################
# Clock synchronization
######################################################################

# It is common for sensors to produce measurements at a fixed
# frequency.  If the mcu can reliably obtain all of these
# measurements, then the code here can calculate a precision timestamp
# for them.  That is, it can determine the actual sensor measurement
# frequency, the time of the first measurement, and thus a precise
# time for all measurements.
#
# This system works by having the mcu periodically report a precision
# timestamp along with the total number of measurements the sensor has
# taken as of that time.  In brief, knowing the total number of
# measurements taken over an extended period provides an accurate
# estimate of measurement frequency, which can then also be utilized
# to determine the time of the first measurement.

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
