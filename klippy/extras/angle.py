# Support for reading SPI magnetic angle sensors
#
# Copyright (C) 2021,2022  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, threading
from . import bus, motion_report

MIN_MSG_TIME = 0.100
TCODE_ERROR = 0xff

class HelperA1333:
    SPI_MODE = 3
    SPI_SPEED = 10000000
    def __init__(self, config, spi, oid):
        self.spi = spi
    def get_static_delay(self):
        return .000001
    def start(self):
        # Setup for angle query
        self.spi.spi_transfer([0x32, 0x00])

class HelperAS5047D:
    SPI_MODE = 1
    SPI_SPEED = int(1. / .000000350)
    def __init__(self, config, spi, oid):
        self.spi = spi
    def get_static_delay(self):
        return .000100
    def start(self):
        # Clear any errors from device
        self.spi.spi_transfer([0xff, 0xfc]) # Read DIAAGC
        self.spi.spi_transfer([0x40, 0x01]) # Read ERRFL
        self.spi.spi_transfer([0xc0, 0x00]) # Read NOP

class HelperTLE5012B:
    SPI_MODE = 1
    SPI_SPEED = 4000000
    def __init__(self, config, spi, oid):
        self.spi = spi
    def get_static_delay(self):
        return .000042700 * 2.5
    def start(self):
        # Clear any errors from device
        self.spi.spi_transfer([0x80, 0x01, 0x00, 0x00, 0x00, 0x00]) # Read STAT

SAMPLE_PERIOD = 0.000400

class Angle:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.sample_period = config.getfloat('sample_period', SAMPLE_PERIOD,
                                             above=0.)
        # Measurement conversion
        self.start_clock = self.time_shift = self.sample_ticks = 0
        self.last_sequence = self.last_angle = 0
        # Measurement storage (accessed from background thread)
        self.lock = threading.Lock()
        self.raw_samples = []
        # Sensor type
        sensors = { "a1333": HelperA1333, "as5047d": HelperAS5047D,
                    "tle5012b": HelperTLE5012B }
        sensor_type = config.getchoice('sensor_type', {s: s for s in sensors})
        sensor_class = sensors[sensor_type]
        self.spi = bus.MCU_SPI_from_config(config, sensor_class.SPI_MODE,
                                           default_speed=sensor_class.SPI_SPEED)
        self.mcu = mcu = self.spi.get_mcu()
        self.oid = oid = mcu.create_oid()
        self.sensor_helper = sensor_class(config, self.spi, oid)
        # Setup mcu sensor_spi_angle bulk query code
        self.query_spi_angle_cmd = self.query_spi_angle_end_cmd = None
        mcu.add_config_cmd(
            "config_spi_angle oid=%d spi_oid=%d spi_angle_type=%s"
            % (oid, self.spi.get_oid(), sensor_type))
        mcu.add_config_cmd(
            "query_spi_angle oid=%d clock=0 rest_ticks=0 time_shift=0"
            % (oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        mcu.register_response(self._handle_spi_angle_data,
                              "spi_angle_data", oid)
        # API server endpoints
        self.api_dump = motion_report.APIDumpHelper(
            self.printer, self._api_update, self._api_startstop, 0.100)
        self.name = config.get_name().split()[1]
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint("angle/dump_angle", "sensor", self.name,
                                 self._handle_dump_angle)
    def _build_config(self):
        freq = self.mcu.seconds_to_clock(1.)
        while float(TCODE_ERROR << self.time_shift) / freq < 0.002:
            self.time_shift += 1
        cmdqueue = self.spi.get_command_queue()
        self.query_spi_angle_cmd = self.mcu.lookup_command(
            "query_spi_angle oid=%c clock=%u rest_ticks=%u time_shift=%c",
            cq=cmdqueue)
        self.query_spi_angle_end_cmd = self.mcu.lookup_query_command(
            "query_spi_angle oid=%c clock=%u rest_ticks=%u time_shift=%c",
            "spi_angle_end oid=%c sequence=%hu", oid=self.oid, cq=cmdqueue)
    # Measurement collection
    def is_measuring(self):
        return self.start_clock != 0
    def _handle_spi_angle_data(self, params):
        with self.lock:
            self.raw_samples.append(params)
    def _extract_samples(self, raw_samples):
        # Load variables to optimize inner loop below
        sample_ticks = self.sample_ticks
        start_clock = self.start_clock
        clock_to_print_time = self.mcu.clock_to_print_time
        last_sequence = self.last_sequence
        last_angle = self.last_angle
        time_shift = self.time_shift
        static_delay = self.sensor_helper.get_static_delay()
        # Process every message in raw_samples
        count = error_count = 0
        samples = [None] * (len(raw_samples) * 16)
        for params in raw_samples:
            seq = (last_sequence & ~0xffff) | params['sequence']
            if seq < last_sequence:
                seq += 0x10000
            last_sequence = seq
            d = bytearray(params['data'])
            msg_mclock = start_clock + seq*16*sample_ticks
            for i in range(len(d) // 3):
                tcode = d[i*3]
                if tcode == TCODE_ERROR:
                    error_count += 1
                    continue
                raw_angle = d[i*3 + 1] | (d[i*3 + 2] << 8)
                angle_diff = (last_angle - raw_angle) & 0xffff
                angle_diff -= (angle_diff & 0x8000) << 1
                last_angle -= angle_diff
                mclock = msg_mclock + i*sample_ticks + (tcode<<time_shift)
                ptime = round(clock_to_print_time(mclock) - static_delay, 6)
                samples[count] = (ptime, last_angle)
                count += 1
        self.last_sequence = last_sequence
        self.last_angle = last_angle
        del samples[count:]
        return samples, error_count
    # API interface
    def _api_update(self, eventtime):
        with self.lock:
            raw_samples = self.raw_samples
            self.raw_samples = []
        if not raw_samples:
            return {}
        samples, error_count = self._extract_samples(raw_samples)
        if not samples:
            return {}
        return {'data': samples, 'errors': error_count}
    def _start_measurements(self):
        if self.is_measuring():
            return
        logging.info("Starting angle '%s' measurements", self.name)
        self.sensor_helper.start()
        # Start bulk reading
        with self.lock:
            self.raw_samples = []
        self.last_sequence = 0
        systime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(systime) + MIN_MSG_TIME
        self.start_clock = reqclock = self.mcu.print_time_to_clock(print_time)
        rest_ticks = self.mcu.seconds_to_clock(self.sample_period)
        self.sample_ticks = rest_ticks
        self.query_spi_angle_cmd.send([self.oid, reqclock, rest_ticks,
                                       self.time_shift], reqclock=reqclock)
    def _finish_measurements(self):
        if not self.is_measuring():
            return
        # Halt bulk reading
        params = self.query_spi_angle_end_cmd.send([self.oid, 0, 0, 0])
        self.start_clock = 0
        with self.lock:
            self.raw_samples = []
        logging.info("Stopped angle '%s' measurements", self.name)
    def _api_startstop(self, is_start):
        if is_start:
            self._start_measurements()
        else:
            self._finish_measurements()
    def _handle_dump_angle(self, web_request):
        self.api_dump.add_client(web_request)
        hdr = ('time', 'angle')
        web_request.send({'header': hdr})

def load_config_prefix(config):
    return Angle(config)
