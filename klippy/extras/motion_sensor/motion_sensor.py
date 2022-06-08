# Support for reading and analyzing motion data from an external chip
#
# Copyright (C) 2022 Harry Beyel <harry3b9@gmail.com>
# Copyright (C) 2020-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from abc import ABCMeta, abstractmethod
import importlib, time, threading, logging, multiprocessing, os, collections
from .clocksync import ClockSyncRegression
from .. import motion_report

# info used to determine which class to load in load_config
sensor_chip_infos = {
        "adxl345": { "module" : ".adxl345",     "class" : "ADXL345" },
        "mpu6050": { "module" : ".invensense",  "class" : "MPU6050" },
        "mpu9250": { "module" : ".invensense",  "class" : "MPU9250" },
    }

# General constants
MIN_MSG_TIME = 0.100
FREEFALL_ACCEL = 9.80665 * 1000. # mm/s**2

def load_config(config):
    sensor_chip_info = config.getchoice('chip', sensor_chip_infos)
    mod = importlib.import_module(name=sensor_chip_info["module"],
        package='.'.join(__name__.split('.')[:-1]))
    sensor_class = getattr(mod, sensor_chip_info["class"])
    return sensor_class(config)

Accel_Measurement = collections.namedtuple(
    'Accel_Measurement', ('time', 'accel_x', 'accel_y', 'accel_z'))

# Helper class to obtain measurements
class MotionSensorQueryHelper:
    def __init__(self, printer, cconn):
        self.printer = printer
        self.cconn = cconn
        print_time = printer.lookup_object('toolhead').get_last_move_time()
        self.request_start_time = self.request_end_time = print_time
        self.samples = self.raw_samples = []
    def finish_measurements(self):
        toolhead = self.printer.lookup_object('toolhead')
        self.request_end_time = toolhead.get_last_move_time()
        toolhead.wait_moves()
        self.cconn.finalize()
    def _get_raw_samples(self):
        raw_samples = self.cconn.get_messages()
        if raw_samples:
            self.raw_samples = raw_samples
        return self.raw_samples
    def has_valid_samples(self):
        raw_samples = self._get_raw_samples()
        for msg in raw_samples:
            data = msg['params']['data']
            first_sample_time = data[0][0]
            last_sample_time = data[-1][0]
            if (first_sample_time > self.request_end_time
                    or last_sample_time < self.request_start_time):
                continue
            # The time intervals [first_sample_time, last_sample_time]
            # and [request_start_time, request_end_time] have non-zero
            # intersection. It is still theoretically possible that none
            # of the samples from raw_samples fall into the time interval
            # [request_start_time, request_end_time] if it is too narrow
            # or on very heavy data losses. In practice, that interval
            # is at least 1 second, so this possibility is negligible.
            return True
        return False
    def get_samples(self):
        raw_samples = self._get_raw_samples()
        if not raw_samples:
            return self.samples
        total = sum([len(m['params']['data']) for m in raw_samples])
        count = 0
        self.samples = samples = [None] * total
        for msg in raw_samples:
            for samp_time, x, y, z in msg['params']['data']:
                if samp_time < self.request_start_time:
                    continue
                if samp_time > self.request_end_time:
                    break
                samples[count] = Accel_Measurement(samp_time, x, y, z)
                count += 1
        del samples[count:]
        return self.samples
    def write_to_file(self, filename):
        def write_impl():
            try:
                # Try to re-nice writing process
                os.nice(20)
            except:
                pass
            f = open(filename, "w")
            f.write("#time,accel_x,accel_y,accel_z\n")
            samples = self.samples or self.get_samples()
            for t, accel_x, accel_y, accel_z in samples:
                f.write("%.6f,%.6f,%.6f,%.6f\n" % (
                    t, accel_x, accel_y, accel_z))
            f.close()
        write_proc = multiprocessing.Process(target=write_impl)
        write_proc.daemon = True
        write_proc.start()

class MotionSensorCommandHelper:
    # Helper class for G-Code commands
    def __init__(self, config, chip):
        self.printer = config.get_printer()
        self.chip = chip
        self.bg_client = None
        self.name = config.get_name().split()[-1]
        self.register_commands(self.name)
        if self.name == "motion_sensor":
            self.register_commands(None)
    def register_commands(self, name):
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("ACCELEROMETER_MEASURE", "CHIP", name,
                                   self.cmd_ACCELEROMETER_MEASURE,
                                   desc=self.cmd_ACCELEROMETER_MEASURE_help)
        gcode.register_mux_command("ACCELEROMETER_QUERY", "CHIP", name,
                                   self.cmd_ACCELEROMETER_QUERY,
                                   desc=self.cmd_ACCELEROMETER_QUERY_help)
        gcode.register_mux_command("ACCELEROMETER_DEBUG_READ", "CHIP", name,
                                   self.cmd_ACCELEROMETER_DEBUG_READ,
                                   desc=self.cmd_ACCELEROMETER_DEBUG_READ_help)
        gcode.register_mux_command("ACCELEROMETER_DEBUG_WRITE", "CHIP", name,
                                   self.cmd_ACCELEROMETER_DEBUG_WRITE,
                                   desc=self.cmd_ACCELEROMETER_DEBUG_WRITE_help)
    cmd_ACCELEROMETER_MEASURE_help = "Start/stop accelerometer"
    def cmd_ACCELEROMETER_MEASURE(self, gcmd):
        if self.bg_client is None:
            # Start measurements
            self.bg_client = self.chip.start_internal_client()
            gcmd.respond_info(self.name + " measurements started")
            return
        # End measurements
        name = gcmd.get("NAME", time.strftime("%Y%m%d_%H%M%S"))
        if not name.replace('-', '').replace('_', '').isalnum():
            raise gcmd.error("Invalid motion_sensor NAME parameter")
        bg_client = self.bg_client
        self.bg_client = None
        bg_client.finish_measurements()
        # Write data to file
        if self.name == "motion_sensor":
            filename = "/tmp/motion_sensor-%s.csv" % (name,)
        else:
            filename = "/tmp/motion_sensor-%s-%s.csv" % (self.name, name,)
        bg_client.write_to_file(filename)
        gcmd.respond_info("Writing raw accelerometer data to %s"
                          % (filename,))
    cmd_ACCELEROMETER_QUERY_help = "Query accelerometer for the current values"
    def cmd_ACCELEROMETER_QUERY(self, gcmd):
        aclient = self.chip.start_internal_client()
        self.printer.lookup_object('toolhead').dwell(1.)
        aclient.finish_measurements()
        values = aclient.get_samples()
        if not values:
            raise gcmd.error("No motion_sensor measurements found")
        _, accel_x, accel_y, accel_z = values[-1]
        gcmd.respond_info("motion_sensor values (x, y, z): %.6f, %.6f, %.6f"
                          % (accel_x, accel_y, accel_z))
    cmd_ACCELEROMETER_DEBUG_READ_help = "Query motion_sensor register " \
        "(for debugging)"
    def cmd_ACCELEROMETER_DEBUG_READ(self, gcmd):
        reg = gcmd.get("REG", minval=0, maxval=255, parser=lambda x: int(x, 0))
        val = self.chip.read_reg(reg)
        gcmd.respond_info("motion_sensor REG[0x%x] = 0x%x" % (reg, val))
    cmd_ACCELEROMETER_DEBUG_WRITE_help = "Set motion_sensor register " \
        "(for debugging)"
    def cmd_ACCELEROMETER_DEBUG_WRITE(self, gcmd):
        reg = gcmd.get("REG", minval=0, maxval=255, parser=lambda x: int(x, 0))
        val = gcmd.get("VAL", minval=0, maxval=255, parser=lambda x: int(x, 0))
        self.chip.set_reg(reg, val)

class MotionSensorBase:
    # Base class for motion sensing devices such as accelerometers and
    # gyroscopes.  Use caution when creating member variables (self.xxx)
    # in subclasses.  Unintentionally overriding an existing variable can
    # cause problems.

    __metaclass__ = ABCMeta
    @property
    @abstractmethod
    def SCALE(self):
        # Scale of the device in mg per least significant bit
        pass

    @property
    @abstractmethod
    def SAMPLE_RATES(self):
        # Dict of supported rates in Hz, mapped to any needed config data
        return {}

    @property
    @abstractmethod
    def SAMPLES_PER_BLOCK(self):
        # Number of samples returned while querying device
        pass

    @property
    @abstractmethod
    def BYTES_PER_SAMPLE(self):
        # Number of bytes returned by the device for each sample
        pass

    @property
    @abstractmethod
    def FIFO_SIZE(self):
        # FIFO size of the device in number of samples
        pass

    def _wake_sensor(self):
        # Wakes up sensor prior to usage
        pass

    def _sleep_sensor(self):
        # Puts sensor to sleep when not in use
        pass

    def _configure_sensor(self):
        # Performs any necessary configuration prior to using device
        pass

    def __init__(self, config):
        self.printer = config.get_printer()
        MotionSensorCommandHelper(config, self)

        # Set up axes mappings
        am = {'x':(0, self.SCALE),  'y':(1, self.SCALE),  'z':(2, self.SCALE),
             '-x':(0, -self.SCALE),'-y':(1, -self.SCALE),'-z':(2, -self.SCALE)}
        axes_map = config.getlist('axes_map', ('x','y','z'), count=3)
        if any([a not in am for a in axes_map]):
            raise config.error("Invalid motion_sensor axes_map parameter")
        self.axes_map = [am[a.strip()] for a in axes_map]

        # Measurement storage (accessed from background thread)
        self.lock = threading.Lock()
        self.raw_samples = []

        # Other core variables
        self.name = config.get_name().split()[-1]
        self._init_conn(config) # initialize serial connection
        self.data_rate = 0
        self.query_rate = 0
        self.mcu = mcu = self.conn.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.query_motion_sensor_cmd = None
        self.query_motion_sensor_end_cmd = None
        self.query_motion_sensor_status_cmd = None

        # Register event handlers
        mcu.register_config_callback(self._build_config)
        self.printer.register_event_handler("klippy:connect",
                                            self._sleep_sensor)

        # Clock tracking
        self.last_sequence = self.max_query_duration = 0
        self.last_limit_count = self.last_error_count = 0
        self.clock_sync = ClockSyncRegression(self.mcu, 640)

        # API server endpoints
        self.api_dump = motion_report.APIDumpHelper(
            self.printer, self._api_update, self._api_startstop, 0.100)
        self.name = config.get_name().split()[-1]
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint("motion_sensor/dump_motion_sensor", "sensor",
                                    self.name, self._handle_dump_motion_sensor)

    @abstractmethod
    def _init_conn(self, config):
        # Sets up device connection object (self.conn)
        pass

    @abstractmethod
    def _build_config(self):
        # Configures device connection by adding config commands and
        # getting the query command objects
        pass

    def _update_clock(self, minclock=0):
        # Query current state
        for retry in range(5):
            params = self.query_motion_sensor_status_cmd.send([self.oid],
                                                        minclock=minclock)
            fifo = params['fifo'] & 0x7f
            if fifo <= self.FIFO_SIZE:
                break
        else:
            raise self.printer.command_error("Unable to query '%s' fifo" %
                                                self.name)
        mcu_clock = self.mcu.clock32_to_clock64(params['clock'])
        sequence = (self.last_sequence & ~0xffff) | params['next_sequence']
        if sequence < self.last_sequence:
            sequence += 0x10000
        self.last_sequence = sequence
        buffered = params['buffered']
        limit_count = (self.last_limit_count & ~0xffff) | params['limit_count']
        if limit_count < self.last_limit_count:
            limit_count += 0x10000
        self.last_limit_count = limit_count
        duration = params['query_ticks']
        if duration > self.max_query_duration:
            # Skip measurement as a high query time could skew clock tracking
            self.max_query_duration = max(2 * self.max_query_duration,
                                          self.mcu.seconds_to_clock(.000005))
            return
        self.max_query_duration = 2 * duration
        msg_count = (sequence * self.SAMPLES_PER_BLOCK
                     + buffered // self.BYTES_PER_SAMPLE + fifo)
        # The "chip clock" is the message counter plus .5 for average
        # inaccuracy of query responses and plus .5 for assumed offset
        # of hw processing time.
        chip_clock = msg_count + 1
        self.clock_sync.update(mcu_clock + duration // 2, chip_clock)

    # Measurement collection
    def is_measuring(self):
        return self.query_rate > 0

    def _handle_motion_sensor_data(self, params):
        with self.lock:
            self.raw_samples.append(params)

    def _start_measurements(self):
        if self.is_measuring():
            return
        # Setup samples
        with self.lock:
            self.raw_samples = []
        # Start bulk reading
        systime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(systime) + MIN_MSG_TIME
        reqclock = self.mcu.print_time_to_clock(print_time)
        rest_ticks = self.mcu.seconds_to_clock(1. / self.data_rate)
        self.query_rate = self.data_rate
        self._wake_sensor()
        self._configure_sensor()
        self.query_motion_sensor_cmd.send([self.oid, reqclock, rest_ticks],
                                    reqclock=reqclock)
        logging.info("Motion sensor starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.last_sequence = 0
        self.last_limit_count = self.last_error_count = 0
        self.clock_sync.reset(reqclock, 0)
        self.max_query_duration = 1 << 31
        self._update_clock(minclock=reqclock)
        self.max_query_duration = 1 << 31

    def _finish_measurements(self):
        if not self.is_measuring():
            return
        # Halt bulk reading
        params = self.query_motion_sensor_end_cmd.send([self.oid, 0, 0])
        self.query_rate = 0
        with self.lock:
            self.raw_samples = []
        self._sleep_sensor()
        logging.info("Motion sensor finished '%s' measurements", self.name)

    # API interface
    def _api_update(self, eventtime):
        self._update_clock()
        with self.lock:
            raw_samples = self.raw_samples
            self.raw_samples = []
        if not raw_samples:
            return {}
        samples = self._extract_samples(raw_samples)
        if not samples:
            return {}
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.last_limit_count}

    def _api_startstop(self, is_start):
        if is_start:
            self._start_measurements()
        else:
            self._finish_measurements()

    def _handle_dump_motion_sensor(self, web_request):
        self.api_dump.add_client(web_request)
        hdr = ('time', 'x_acceleration', 'y_acceleration', 'z_acceleration')
        web_request.send({'header': hdr})

    def start_internal_client(self):
        cconn = self.api_dump.add_internal_client()
        return MotionSensorQueryHelper(self.printer, cconn)
