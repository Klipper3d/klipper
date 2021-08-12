# Support for reading acceleration data from an adxl345 chip
#
# Copyright (C) 2020-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, time, collections, threading, multiprocessing, os
from . import bus, motion_report

# ADXL345 registers
REG_DEVID = 0x00
REG_BW_RATE = 0x2C
REG_POWER_CTL = 0x2D
REG_DATA_FORMAT = 0x31
REG_FIFO_CTL = 0x38
REG_MOD_READ = 0x80
REG_MOD_MULTI = 0x40

QUERY_RATES = {
    25: 0x8, 50: 0x9, 100: 0xa, 200: 0xb, 400: 0xc,
    800: 0xd, 1600: 0xe, 3200: 0xf,
}

ADXL345_DEV_ID = 0xe5

FREEFALL_ACCEL = 9.80665 * 1000.
SCALE = 0.0039 * FREEFALL_ACCEL # 3.9mg/LSB * Earth gravity in mm/s**2

Accel_Measurement = collections.namedtuple(
    'Accel_Measurement', ('time', 'accel_x', 'accel_y', 'accel_z'))

# Helper class to obtain measurements
class ADXL345QueryHelper:
    def __init__(self, printer, chip, cconn):
        self.printer = printer
        self.chip = chip
        self.cconn = cconn
        print_time = printer.lookup_object('toolhead').get_last_move_time()
        self.request_start_time = self.request_end_time = print_time
        self.raw_samples = None
        self.samples = []
        self.drops = self.overflows = 0
        self.start2_time = 0.
        self.time_per_sample = self.start_range = self.end_range = 0.
    def finish_measurements(self):
        toolhead = self.printer.lookup_object('toolhead')
        self.request_end_time = toolhead.get_last_move_time()
        toolhead.wait_moves()
        self.cconn.finalize()
        toolhead.dwell(0.200)
        toolhead.wait_moves()
        self._setup_data(*self.chip.final_results) # XXX
    def get_stats(self):
        return ("drops=%d,overflows=%d"
                ",time_per_sample=%.9f,start_range=%.6f,end_range=%.6f"
                % (self.drops, self.overflows,
                   self.time_per_sample, self.start_range, self.end_range))
    def _setup_data(self, end_sequence, overflows,
                    start1_time, start2_time, end1_time, end2_time):
        raw_samples = self.cconn.get_messages()
        if not raw_samples or not end_sequence:
            return
        self.raw_samples = raw_samples
        self.overflows = overflows
        self.start2_time = start2_time
        self.start_range = start2_time - start1_time
        self.end_range = end2_time - end1_time
        self.total_count = raw_samples[-1]['params']['data'][-1][0] + 1
        total_time = end2_time - start2_time
        self.time_per_sample = total_time / self.total_count
        actual_count = sum([len(m['params']['data']) for m in raw_samples])
        self.drops = self.total_count - actual_count
    def decode_samples(self):
        if not self.raw_samples:
            return self.samples
        count = 0
        self.samples = samples = [None] * self.total_count
        for msg in self.raw_samples:
            for seq, x, y, z in msg['params']['data']:
                samp_time = self.start2_time + seq * self.time_per_sample
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
            f.write("##%s\n#time,accel_x,accel_y,accel_z\n" % (
                self.get_stats(),))
            samples = self.samples or self.decode_samples()
            for t, accel_x, accel_y, accel_z in samples:
                f.write("%.6f,%.6f,%.6f,%.6f\n" % (
                    t, accel_x, accel_y, accel_z))
            f.close()
        write_proc = multiprocessing.Process(target=write_impl)
        write_proc.daemon = True
        write_proc.start()

# Helper class for G-Code commands
class ADXLCommandHelper:
    def __init__(self, config, chip):
        self.printer = config.get_printer()
        self.chip = chip
        self.bg_client = None
        self.name = "default"
        if len(config.get_name().split()) > 1:
            self.name = config.get_name().split()[1]
        self.register_commands(self.name)
        if self.name == "default":
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
        gcode.register_mux_command("ADXL345_DEBUG_READ", "CHIP", name,
                                   self.cmd_ADXL345_DEBUG_READ,
                                   desc=self.cmd_ADXL345_DEBUG_READ_help)
        gcode.register_mux_command("ADXL345_DEBUG_WRITE", "CHIP", name,
                                   self.cmd_ADXL345_DEBUG_WRITE,
                                   desc=self.cmd_ADXL345_DEBUG_WRITE_help)
    cmd_ACCELEROMETER_MEASURE_help = "Start/stop accelerometer"
    def cmd_ACCELEROMETER_MEASURE(self, gcmd):
        if not self.chip.is_measuring():
            # Start measurements
            self.bg_client = self.chip.start_internal_client()
            gcmd.respond_info("adxl345 measurements started")
            return
        if self.bg_client is None:
            raise gcmd.error("adxl345 measurements in progress")
        # End measurements
        name = gcmd.get("NAME", time.strftime("%Y%m%d_%H%M%S"))
        if not name.replace('-', '').replace('_', '').isalnum():
            raise gcmd.error("Invalid adxl345 NAME parameter")
        bg_client = self.bg_client
        self.bg_client = None
        bg_client.finish_measurements()
        # Write data to file
        if self.name == "default":
            filename = "/tmp/adxl345-%s.csv" % (name,)
        else:
            filename = "/tmp/adxl345-%s-%s.csv" % (self.name, name,)
        bg_client.write_to_file(filename)
        gcmd.respond_info("Writing raw accelerometer data to %s file"
                          % (filename,))
    cmd_ACCELEROMETER_QUERY_help = "Query accelerometer for the current values"
    def cmd_ACCELEROMETER_QUERY(self, gcmd):
        if self.chip.is_measuring():
            raise gcmd.error("adxl345 measurements in progress")
        aclient = self.chip.start_internal_client()
        self.printer.lookup_object('toolhead').dwell(1.)
        aclient.finish_measurements()
        values = aclient.decode_samples()
        if not values:
            raise gcmd.error("No adxl345 measurements found")
        _, accel_x, accel_y, accel_z = values[-1]
        gcmd.respond_info("adxl345 values (x, y, z): %.6f, %.6f, %.6f"
                          % (accel_x, accel_y, accel_z))
    cmd_ADXL345_DEBUG_READ_help = "Query accelerometer register (for debugging)"
    def cmd_ADXL345_DEBUG_READ(self, gcmd):
        if self.chip.is_measuring():
            raise gcmd.error("adxl345 measurements in progress")
        reg = gcmd.get("REG", minval=29, maxval=57, parser=lambda x: int(x, 0))
        val = self.chip.read_reg(reg)
        gcmd.respond_info("ADXL345 REG[0x%x] = 0x%x" % (reg, val))
    cmd_ADXL345_DEBUG_WRITE_help = "Set accelerometer register (for debugging)"
    def cmd_ADXL345_DEBUG_WRITE(self, gcmd):
        if self.chip.is_measuring():
            raise gcmd.error("adxl345 measurements in progress")
        reg = gcmd.get("REG", minval=29, maxval=57, parser=lambda x: int(x, 0))
        val = gcmd.get("VAL", minval=0, maxval=255, parser=lambda x: int(x, 0))
        self.chip.set_reg(reg, val)

MIN_MSG_TIME = 0.100

# Printer class that controls ADXL345 chip
class ADXL345:
    def __init__(self, config):
        self.printer = config.get_printer()
        ADXLCommandHelper(config, self)
        self.query_rate = 0
        self.last_sequence = 0
        am = {'x': (0, SCALE), 'y': (1, SCALE), 'z': (2, SCALE),
              '-x': (0, -SCALE), '-y': (1, -SCALE), '-z': (2, -SCALE)}
        axes_map = config.getlist('axes_map', ('x','y','z'), count=3)
        if any([a not in am for a in axes_map]):
            raise config.error("Invalid adxl345 axes_map parameter")
        self.axes_map = [am[a.strip()] for a in axes_map]
        self.data_rate = config.getint('rate', 3200)
        if self.data_rate not in QUERY_RATES:
            raise config.error("Invalid rate parameter: %d" % (self.data_rate,))
        # Measurement storage (accessed from background thread)
        self.lock = threading.Lock()
        self.raw_samples = []
        self.samples_start1 = self.samples_start2 = 0.
        # Setup mcu sensor_adxl345 bulk query code
        self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=5000000)
        self.mcu = mcu = self.spi.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.query_adxl345_cmd = self.query_adxl345_end_cmd =None
        mcu.add_config_cmd("config_adxl345 oid=%d spi_oid=%d"
                           % (oid, self.spi.get_oid()))
        mcu.add_config_cmd("query_adxl345 oid=%d clock=0 rest_ticks=0"
                           % (oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        mcu.register_response(self._handle_adxl345_start, "adxl345_start", oid)
        mcu.register_response(self._handle_adxl345_data, "adxl345_data", oid)
        # API server endpoints
        self.api_dump = motion_report.APIDumpHelper(
            self.printer, self._api_update, self._api_startstop, 0.100)
        self.name = "default"
        if len(config.get_name().split()) > 1:
            self.name = config.get_name().split()[1]
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint("adxl345/dump_adxl345", "sensor", self.name,
                                 self._handle_dump_adxl345)
    def _build_config(self):
        self.query_adxl345_cmd = self.mcu.lookup_command(
            "query_adxl345 oid=%c clock=%u rest_ticks=%u",
            cq=self.spi.get_command_queue())
        self.query_adxl345_end_cmd = self.mcu.lookup_query_command(
            "query_adxl345 oid=%c clock=%u rest_ticks=%u",
            "adxl345_end oid=%c end1_clock=%u end2_clock=%u"
            " limit_count=%hu sequence=%hu",
            oid=self.oid, cq=self.spi.get_command_queue())
    def _clock_to_print_time(self, clock):
        return self.mcu.clock_to_print_time(self.mcu.clock32_to_clock64(clock))
    def _convert_sequence(self, sequence):
        sequence = (self.last_sequence & ~0xffff) | sequence
        if sequence < self.last_sequence:
            sequence += 0x10000
        return sequence
    def read_reg(self, reg):
        params = self.spi.spi_transfer([reg | REG_MOD_READ, 0x00])
        response = bytearray(params['response'])
        return response[1]
    def set_reg(self, reg, val, minclock=0):
        self.spi.spi_send([reg, val & 0xFF], minclock=minclock)
        stored_val = self.read_reg(reg)
        if stored_val != val:
            raise self.printer.command_error(
                    "Failed to set ADXL345 register [0x%x] to 0x%x: got 0x%x. "
                    "This is generally indicative of connection problems "
                    "(e.g. faulty wiring) or a faulty adxl345 chip." % (
                        reg, val, stored_val))
    # Measurement collection
    def is_measuring(self):
        return self.query_rate > 0
    def _handle_adxl345_start(self, params):
        self.samples_start1 = self._clock_to_print_time(params['start1_clock'])
        self.samples_start2 = self._clock_to_print_time(params['start2_clock'])
    def _handle_adxl345_data(self, params):
        with self.lock:
            self.raw_samples.append(params)
    def _extract_samples(self, raw_samples):
        # Load variables to optimize inner loop below
        (x_pos, x_scale), (y_pos, y_scale), (z_pos, z_scale) = self.axes_map
        last_sequence = self.last_sequence
        # Process every message in raw_samples
        count = 0
        samples = [None] * (len(raw_samples) * 8)
        for params in raw_samples:
            seq = (last_sequence & ~0xffff) | params['sequence']
            if seq < last_sequence:
                seq += 0x10000
            last_sequence = seq
            d = bytearray(params['data'])
            len_d = len(d)
            sdata = [(d[i] | (d[i+1] << 8)) - ((d[i+1] & 0x80) << 9)
                     for i in range(0, len_d-1, 2)]
            for i in range(len_d // 6):
                x = round(sdata[i*3 + x_pos] * x_scale, 6)
                y = round(sdata[i*3 + y_pos] * y_scale, 6)
                z = round(sdata[i*3 + z_pos] * z_scale, 6)
                samples[count] = (seq * 8 + i, x, y, z)
                count += 1
        self.last_sequence = last_sequence
        del samples[count:]
        return samples
    def _start_measurements(self):
        if self.is_measuring():
            return
        # In case of miswiring, testing ADXL345 device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        dev_id = self.read_reg(REG_DEVID)
        if dev_id != ADXL345_DEV_ID:
            raise self.printer.command_error("Invalid adxl345 id (got %x vs %x)"
                                             % (dev_id, ADXL345_DEV_ID))
        # Setup chip in requested query rate
        self.set_reg(REG_POWER_CTL, 0x00)
        self.set_reg(REG_DATA_FORMAT, 0x0B)
        self.set_reg(REG_FIFO_CTL, 0x00)
        self.set_reg(REG_BW_RATE, QUERY_RATES[self.data_rate])
        self.set_reg(REG_FIFO_CTL, 0x80)
        # Setup samples
        systime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(systime) + MIN_MSG_TIME
        self.samples_start1 = self.samples_start2 = print_time
        self.last_sequence = 0
        with self.lock:
            self.raw_samples = []
        # Start bulk reading
        reqclock = self.mcu.print_time_to_clock(print_time)
        rest_ticks = self.mcu.seconds_to_clock(4. / self.data_rate)
        self.query_rate = self.data_rate
        self.query_adxl345_cmd.send([self.oid, reqclock, rest_ticks],
                                    reqclock=reqclock)
        logging.info("ADXL345 starting '%s' measurements", self.name)
    def _finish_measurements(self):
        if not self.is_measuring():
            return
        # Halt bulk reading
        params = self.query_adxl345_end_cmd.send([self.oid, 0, 0])
        self.query_rate = 0
        with self.lock:
            self.raw_samples = []
        # Generate results
        end1_time = self._clock_to_print_time(params['end1_clock'])
        end2_time = self._clock_to_print_time(params['end2_clock'])
        end_sequence = self._convert_sequence(params['sequence'])
        overflows = params['limit_count']
        logging.info("ADXL345 finished '%s' measurements", self.name)
        self.final_results = (end_sequence, overflows,
                              self.samples_start1, self.samples_start2,
                              end1_time, end2_time) # XXX
    # API interface
    def _api_update(self, eventtime):
        with self.lock:
            raw_samples = self.raw_samples
            self.raw_samples = []
        if not raw_samples:
            return {}
        samples = self._extract_samples(raw_samples)
        if not samples:
            return {}
        return {'data': samples}
    def _api_startstop(self, is_start):
        if is_start:
            self._start_measurements()
        else:
            self._finish_measurements()
    def _handle_dump_adxl345(self, web_request):
        self.api_dump.add_client(web_request)
        hdr = ('sequence', 'x_acceleration', 'y_acceleration', 'z_acceleration')
        web_request.send({'header': hdr})
    def start_internal_client(self):
        if self.is_measuring():
            raise self.printer.command_error(
                "ADXL345 measurement already in progress")
        cconn = self.api_dump.add_internal_client()
        return ADXL345QueryHelper(self.printer, self, cconn)

def load_config(config):
    return ADXL345(config)

def load_config_prefix(config):
    return ADXL345(config)
