# Load Cell Implementation
#
# Copyright (C) 2024 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from . import hx71x
from . import ads1220
from .bulk_sensor import BatchWebhooksClient
import collections, itertools
# We want either Python 3's zip() or Python 2's izip() but NOT 2's zip():
zip_impl = zip
try:
    from itertools import izip as zip_impl # python 2.x izip
except ImportError: # will be Python 3.x
    pass

# alternative to numpy's column selection:
def select_column(data, column_idx):
    return list(zip_impl(*data))[column_idx]

def avg(data):
    return sum(data) / len(data)

# Helper for event driven webhooks and subscription based API clients
class ApiClientHelper(object):
    def __init__(self, printer):
        self.printer = printer
        self.client_cbs = []
        self.webhooks_start_resp = {}

    # send data to clients
    def send(self, msg):
        for client_cb in list(self.client_cbs):
            res = client_cb(msg)
            if not res:
                # This client no longer needs updates - unregister it
                self.client_cbs.remove(client_cb)

    # Add a client that gets data callbacks
    def add_client(self, client_cb):
        self.client_cbs.append(client_cb)

    # Add Webhooks client and send header
    def _add_webhooks_client(self, web_request):
        whbatch = BatchWebhooksClient(web_request)
        self.add_client(whbatch.handle_batch)
        web_request.send(self.webhooks_start_resp)

    # Set up a webhooks endpoint with a static header
    def add_mux_endpoint(self, path, key, value, webhooks_start_resp):
        self.webhooks_start_resp = webhooks_start_resp
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint(path, key, value, self._add_webhooks_client)

# Class for handling commands related ot load cells
class LoadCellCommandHelper:
    def __init__(self, config, load_cell):
        self.printer = config.get_printer()
        self.load_cell = load_cell
        name_parts = config.get_name().split()
        self.name = name_parts[-1]
        self.register_commands(self.name)
        if len(name_parts) == 1:
            self.register_commands(None)

    def register_commands(self, name):
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("LOAD_CELL_TARE", "LOAD_CELL", name,
                                   self.cmd_LOAD_CELL_TARE,
                                   desc=self.cmd_LOAD_CELL_TARE_help)
        gcode.register_mux_command("LOAD_CELL_CALIBRATE", "LOAD_CELL", name,
                                   self.cmd_LOAD_CELL_CALIBRATE,
                                   desc=self.cmd_CALIBRATE_LOAD_CELL_help)
        gcode.register_mux_command("LOAD_CELL_READ", "LOAD_CELL", name,
                                   self.cmd_LOAD_CELL_READ,
                                   desc=self.cmd_LOAD_CELL_READ_help)
        gcode.register_mux_command("LOAD_CELL_DIAGNOSTIC", "LOAD_CELL", name,
                                   self.cmd_LOAD_CELL_DIAGNOSTIC,
                                   desc=self.cmd_LOAD_CELL_DIAGNOSTIC_help)

    cmd_LOAD_CELL_TARE_help = "Set the Zero point of the load cell"
    def cmd_LOAD_CELL_TARE(self, gcmd):
        tare_counts = self.load_cell.avg_counts()
        self.load_cell.tare(tare_counts)
        tare_percent = self.load_cell.counts_to_percent(tare_counts)
        gcmd.respond_info("Load cell tare value: %.2f%% (%i)"
                          % (tare_percent, tare_counts))

    cmd_CALIBRATE_LOAD_CELL_help = "Start interactive calibration tool"
    def cmd_LOAD_CELL_CALIBRATE(self, gcmd):
        LoadCellGuidedCalibrationHelper(self.printer, self.load_cell)

    cmd_LOAD_CELL_READ_help = "Take a reading from the load cell"
    def cmd_LOAD_CELL_READ(self, gcmd):
        counts = self.load_cell.avg_counts()
        percent = self.load_cell.counts_to_percent(counts)
        force = self.load_cell.counts_to_grams(counts)
        if percent >= 100 or percent <= -100:
            gcmd.respond_info("Err (%.2f%%)" % (percent,))
        if force is None:
            gcmd.respond_info("---.-g (%.2f%%)" % (percent,))
        else:
            gcmd.respond_info("%.1fg (%.2f%%)" % (force, percent))

    cmd_LOAD_CELL_DIAGNOSTIC_help = "Check the health of the load cell"
    def cmd_LOAD_CELL_DIAGNOSTIC(self, gcmd):
        gcmd.respond_info("Collecting load cell data for 10 seconds...")
        collector = self.load_cell.get_collector()
        reactor = self.printer.get_reactor()
        collector.start_collecting()
        reactor.pause(reactor.monotonic() + 10.)
        samples, errors = collector.stop_collecting()
        if errors:
            gcmd.respond_info("Sensor reported errors: %i errors,"
                              " %i overflows" % (errors[0], errors[1]))
        else:
            gcmd.respond_info("Sensor reported no errors")
        if not samples:
            raise gcmd.error("No samples returned from sensor!")
        counts = select_column(samples, 2)
        range_min, range_max = self.load_cell.saturation_range()
        good_count = 0
        saturation_count = 0
        for sample in counts:
            if sample >= range_max or sample <= range_min:
                saturation_count += 1
            else:
                good_count += 1
        gcmd.respond_info("Samples Collected: %i" % (len(samples)))
        if len(samples) > 2:
            sensor_sps = self.load_cell.sensor.get_samples_per_second()
            sps = float(len(samples)) / (samples[-1][0] - samples[0][0])
            gcmd.respond_info("Measured samples per second: %.1f, "
                              "configured: %.1f" % (sps, sensor_sps))
        gcmd.respond_info("Good samples: %i, Saturated samples: %i, Unique"
                          " values: %i" % (good_count, saturation_count,
                          len(set(counts))))
        max_pct = self.load_cell.counts_to_percent(max(counts))
        min_pct = self.load_cell.counts_to_percent(min(counts))
        gcmd.respond_info("Sample range: [%.2f%% to %.2f%%]"
                          % (min_pct, max_pct))
        gcmd.respond_info("Sample range / sensor capacity: %.5f%%"
                          % ((max_pct - min_pct) / 2.))

# Class to guide the user through calibrating a load cell
class LoadCellGuidedCalibrationHelper:
    def __init__(self, printer, load_cell):
        self.printer = printer
        self.gcode = printer.lookup_object('gcode')
        self.load_cell = load_cell
        self._tare_counts = self._counts_per_gram = None
        self.tare_percent = 0.
        self.register_commands()
        self.gcode.respond_info(
            "Starting load cell calibration. \n"
            "1.) Remove all load and run TARE. \n"
            "2.) Apply a known load, run CALIBRATE GRAMS=nnn. \n"
            "Complete calibration with the ACCEPT command.\n"
            "Use the ABORT command to quit.")

    def verify_no_active_calibration(self,):
        try:
            self.gcode.register_command('TARE', 'dummy')
        except self.printer.config_error as e:
            raise self.gcode.error(
                "Already Calibrating a Load Cell. Use ABORT to quit.")
        self.gcode.register_command('TARE', None)

    def register_commands(self):
        self.verify_no_active_calibration()
        register_command = self.gcode.register_command
        register_command("ABORT", self.cmd_ABORT, desc=self.cmd_ABORT_help)
        register_command("ACCEPT", self.cmd_ACCEPT, desc=self.cmd_ACCEPT_help)
        register_command("TARE", self.cmd_TARE, desc=self.cmd_TARE_help)
        register_command("CALIBRATE", self.cmd_CALIBRATE,
                         desc=self.cmd_CALIBRATE_help)

    # convert the delta of counts to a counts/gram metric
    def counts_per_gram(self, grams, cal_counts):
        return float(abs(int(self._tare_counts - cal_counts))) / grams

    # calculate max force that the load cell can register
    # given tare bias, at saturation in kilograms
    def capacity_kg(self, counts_per_gram):
        range_min, range_max = self.load_cell.saturation_range()
        return (int((range_max - abs(self._tare_counts)) / counts_per_gram)
                / 1000.)

    def finalize(self, save_results=False):
        for name in ['ABORT', 'ACCEPT', 'TARE', 'CALIBRATE']:
            self.gcode.register_command(name, None)
        if not save_results:
            self.gcode.respond_info("Load cell calibration aborted")
            return
        if self._counts_per_gram is None or self._tare_counts is None:
            self.gcode.respond_info("Calibration process is incomplete, "
                                    "aborting")
        self.load_cell.set_calibration(self._counts_per_gram, self._tare_counts)
        self.gcode.respond_info("Load cell calibration settings:\n\n"
            "counts_per_gram: %.6f\n"
            "reference_tare_counts: %i\n\n"
            "The SAVE_CONFIG command will update the printer config file"
            " with the above and restart the printer."
            % (self._counts_per_gram, self._tare_counts))
        self.load_cell.tare(self._tare_counts)

    cmd_ABORT_help = "Abort load cell calibration tool"
    def cmd_ABORT(self, gcmd):
        self.finalize(False)

    cmd_ACCEPT_help = "Accept calibration results and apply to load cell"
    def cmd_ACCEPT(self, gcmd):
        self.finalize(True)

    cmd_TARE_help = "Tare the load cell"
    def cmd_TARE(self, gcmd):
        self._tare_counts = self.load_cell.avg_counts()
        self._counts_per_gram = None  # require re-calibration on tare
        self.tare_percent = self.load_cell.counts_to_percent(self._tare_counts)
        gcmd.respond_info("Load cell tare value: %.2f%% (%i)"
                          % (self.tare_percent, self._tare_counts))
        if self.tare_percent > 2.:
            gcmd.respond_info(
                "WARNING: tare value is more than 2% away from 0!\n"
                "The load cell's range will be impacted.\n"
                "Check for external force on the load cell.")
        gcmd.respond_info("Now apply a known force to the load cell and enter \
                         the force value with:\n CALIBRATE GRAMS=nnn")

    cmd_CALIBRATE_help = "Enter the load cell value in grams"
    def cmd_CALIBRATE(self, gcmd):
        if self._tare_counts is None:
            gcmd.respond_info("You must use TARE first.")
            return
        grams = gcmd.get_float("GRAMS", minval=50., maxval=25000.)
        cal_counts = self.load_cell.avg_counts()
        cal_percent = self.load_cell.counts_to_percent(cal_counts)
        c_per_g = self.counts_per_gram(grams, cal_counts)
        cap_kg = self.capacity_kg(c_per_g)
        gcmd.respond_info("Calibration value: %.2f%% (%i), Counts/gram: %.5f, \
            Total capacity: +/- %0.2fKg"
                % (cal_percent, cal_counts, c_per_g, cap_kg))
        range_min, range_max = self.load_cell.saturation_range()
        if cal_counts >= range_max or cal_counts <= range_min:
            raise self.printer.command_error(
                "ERROR: Sensor is saturated with too much load!\n"
                "Use less force to calibrate the load cell.")
        if cal_counts == self._tare_counts:
            raise self.printer.command_error(
                "ERROR: Tare and Calibration readings are the same!\n"
                "Check wiring and validate sensor with READ_LOAD_CELL command.")
        if (abs(cal_percent - self.tare_percent)) < 1.:
            raise self.printer.command_error(
                "ERROR: Tare and Calibration readings are less than 1% "
                "different!\n"
                "Use more force when calibrating or a higher sensor gain.")
        # only set _counts_per_gram after all errors are raised
        self._counts_per_gram = c_per_g
        if cap_kg < 1.:
            gcmd.respond_info("WARNING: Load cell capacity is less than 1kg!\n"
                "Check wiring and consider using a lower sensor gain.")
        if cap_kg > 25.:
            gcmd.respond_info("WARNING: Load cell capacity is more than 25Kg!\n"
                "Check wiring and consider using a higher sensor gain.")
        gcmd.respond_info("Accept calibration with the ACCEPT command.")


# Utility to collect some samples from the LoadCell for later analysis
# Optionally blocks execution while collecting with reactor.pause()
# can collect a minimum n samples or collect until a specific print_time
# samples returned in [[time],[force],[counts]] arrays for easy processing
RETRY_DELAY = 0.05  # 20Hz
class LoadCellSampleCollector:
    def __init__(self, printer, load_cell):
        self._printer = printer
        self._load_cell = load_cell
        self._reactor = printer.get_reactor()
        self._mcu = load_cell.sensor.get_mcu()
        self.min_time = 0.
        self.max_time = float("inf")
        self.min_count = float("inf")  # In Python 3.5 math.inf is better
        self.is_started = False
        self._samples = []
        self._errors = 0
        self._overflows = 0

    def _on_samples(self, msg):
        if not self.is_started:
            return False  # already stopped, ignore
        self._errors += msg['errors']
        self._overflows += msg['overflows']
        samples = msg['data']
        for sample in samples:
            time = sample[0]
            if self.min_time <= time <= self.max_time:
                self._samples.append(sample)
            if time > self.max_time:
                self.is_started = False
        if len(self._samples) >= self.min_count:
            self.is_started = False
        return self.is_started

    def _finish_collecting(self):
        self.is_started = False
        self.min_time = 0.
        self.max_time = float("inf")
        self.min_count = float("inf")  # In Python 3.5 math.inf is better
        samples = self._samples
        self._samples = []
        errors = self._errors
        self._errors = 0
        overflows = self._overflows
        self._overflows = 0
        return samples, (errors, overflows) if errors or overflows else 0

    def _collect_until(self, timeout):
        self.start_collecting()
        while self.is_started:
            now = self._reactor.monotonic()
            if self._mcu.estimated_print_time(now) > timeout:
                self._finish_collecting()
                raise self._printer.command_error(
                    "LoadCellSampleCollector timed out! Errors: %i,"
                    " Overflows: %i" % (self._errors, self._overflows))
            self._reactor.pause(now + RETRY_DELAY)
        return self._finish_collecting()

    # start collecting with no automatic end to collection
    def start_collecting(self, min_time=None):
        if self.is_started:
            return
        self.min_time = min_time if min_time is not None else self.min_time
        self.is_started = True
        self._load_cell.add_client(self._on_samples)

    # stop collecting immediately and return results
    def stop_collecting(self):
        return self._finish_collecting()

    # block execution until at least min_count samples are collected
    # will return all samples collected, not just up to min_count
    def collect_min(self, min_count=1):
        self.min_count = min_count
        if len(self._samples) >= min_count:
            return self._finish_collecting()
        print_time = self._mcu.estimated_print_time(self._reactor.monotonic())
        start_time = max(print_time, self.min_time)
        sps = self._load_cell.sensor.get_samples_per_second()
        return self._collect_until(start_time + 1. + (min_count / sps))

    # returns when a sample is collected with a timestamp after print_time
    def collect_until(self, print_time=None):
        self.max_time = print_time
        if len(self._samples) and self._samples[-1][0] >= print_time:
            return self._finish_collecting()
        return self._collect_until(self.max_time + 1.)

# Printer class that controls the load cell
MIN_COUNTS_PER_GRAM = 1.
class LoadCell:
    def __init__(self, config, sensor):
        self.printer = printer = config.get_printer()
        self.config_name = config.get_name()
        self.name = config.get_name().split()[-1]
        self.sensor = sensor   # must implement BulkSensorAdc
        buffer_size = sensor.get_samples_per_second() // 2
        self._force_buffer = collections.deque(maxlen=buffer_size)
        self.reference_tare_counts = config.getint('reference_tare_counts',
                                                   default=None)
        self.tare_counts = self.reference_tare_counts
        self.counts_per_gram = config.getfloat('counts_per_gram',
                                   minval=MIN_COUNTS_PER_GRAM, default=None)
        self.invert = config.getchoice('sensor_orientation',
                        {'normal': 1., 'inverted': -1.}, default="normal")
        LoadCellCommandHelper(config, self)
        # Client support:
        self.clients = ApiClientHelper(printer)
        header = {"header": ["time", "force (g)", "counts", "tare_counts"]}
        self.clients.add_mux_endpoint("load_cell/dump_force",
                                      "load_cell", self.name, header)
        # startup, when klippy is ready, start capturing data
        printer.register_event_handler("klippy:ready", self._handle_ready)

    def _handle_ready(self):
        self.sensor.add_client(self._sensor_data_event)
        self.add_client(self._track_force)
        # announce calibration status on ready
        if self.is_calibrated():
            self.printer.send_event("load_cell:calibrate", self)
        if self.is_tared():
            self.printer.send_event("load_cell:tare", self)

    # convert raw counts to grams and broadcast to clients
    def _sensor_data_event(self, msg):
        data = msg.get("data")
        errors = msg.get("errors")
        overflows = msg.get("overflows")
        if data is None:
            return None
        samples = []
        for row in data:
            # [time, grams, counts, tare_counts]
            samples.append([row[0], self.counts_to_grams(row[1]), row[1],
                            self.tare_counts])
        msg = {'data': samples, 'errors': errors, 'overflows': overflows}
        self.clients.send(msg)
        return True

    # get internal events of force data
    def add_client(self, callback):
        self.clients.add_client(callback)

    def tare(self, tare_counts):
        self.tare_counts = int(tare_counts)
        self.printer.send_event("load_cell:tare", self)

    def set_calibration(self, counts_per_gram, tare_counts):
        if (counts_per_gram is None
                or abs(counts_per_gram) < MIN_COUNTS_PER_GRAM):
            raise self.printer.command_error("Invalid counts per gram value")
        if tare_counts is None:
            raise self.printer.command_error("Missing tare counts")
        self.counts_per_gram = counts_per_gram
        self.reference_tare_counts = int(tare_counts)
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.config_name, 'counts_per_gram',
                       "%.5f" % (self.counts_per_gram,))
        configfile.set(self.config_name, 'reference_tare_counts',
                       "%i" % (self.reference_tare_counts,))
        self.printer.send_event("load_cell:calibrate", self)

    def counts_to_grams(self, sample):
        if not self.is_calibrated() or not self.is_tared():
            return None
        sample_delta = float(sample - self.tare_counts)
        return self.invert * (sample_delta / self.counts_per_gram)

    # The maximum range of the sensor based on its bit width
    def saturation_range(self):
        return self.sensor.get_range()

    # convert raw counts to a +/- percentage of the sensors range
    def counts_to_percent(self, counts):
        range_min, range_max = self.saturation_range()
        return (float(counts) / float(range_max)) * 100.

    # read 1 second of load cell data and average it
    # performs safety checks for saturation
    def avg_counts(self, num_samples=None):
        if num_samples is None:
            num_samples = self.sensor.get_samples_per_second()
        samples, errors = self.get_collector().collect_min(num_samples)
        if errors:
            raise self.printer.command_error(
                "Sensor reported %i errors while sampling"
                    % (errors[0] + errors[1]))
        # check samples for saturated readings
        range_min, range_max = self.saturation_range()
        for sample in samples:
            if sample[2] >= range_max or sample[2] <= range_min:
                raise self.printer.command_error(
                    "Some samples are saturated (+/-100%)")
        return avg(select_column(samples, 2))

    # Provide ongoing force tracking/averaging for status updates
    def _track_force(self, msg):
        if not (self.is_calibrated() and self.is_tared()):
            return True
        samples = msg['data']
        # selectColumn unusable here because Python 2 lacks deque.extend
        for sample in samples:
            self._force_buffer.append(sample[1])
        return True

    def _force_g(self):
        if (self.is_calibrated() and self.is_tared()
                and len(self._force_buffer) > 0):
            return {"force_g": round(avg(self._force_buffer), 1),
                    "min_force_g": round(min(self._force_buffer), 1),
                    "max_force_g": round(max(self._force_buffer), 1)}
        return {}

    def is_tared(self):
        return self.tare_counts is not None

    def is_calibrated(self):
        return (self.counts_per_gram is not None
                and self.reference_tare_counts is not None)

    def get_sensor(self):
        return self.sensor

    def get_reference_tare_counts(self):
        return self.reference_tare_counts

    def get_tare_counts(self):
        return self.tare_counts

    def get_counts_per_gram(self):
        return self.counts_per_gram

    def get_collector(self):
        return LoadCellSampleCollector(self.printer, self)

    def get_status(self, eventtime):
        status = self._force_g()
        status.update({'is_calibrated': self.is_calibrated(),
                       'counts_per_gram': self.counts_per_gram,
                       'reference_tare_counts': self.reference_tare_counts,
                       'tare_counts': self.tare_counts})
        return status


def load_config(config):
    # Sensor types
    sensors = {}
    sensors.update(hx71x.HX71X_SENSOR_TYPES)
    sensors.update(ads1220.ADS1220_SENSOR_TYPE)
    sensor_class = config.getchoice('sensor_type', sensors)
    return LoadCell(config, sensor_class(config))

def load_config_prefix(config):
    return load_config(config)
