# Support for reading acceleration data from an adxl345 chip
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, time, collections
from . import bus

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

SCALE = 0.004 * 9.80665 * 1000. # 4mg/LSB * Earth gravity in mm/s**2

Accel_Measurement = collections.namedtuple(
    'Accel_Measurement', ('time', 'accel_x', 'accel_y', 'accel_z'))

# Sample results
class ADXL345Results:
    def __init__(self):
        self.samples = []
        self.drops = self.overflows = 0
        self.time_per_sample = self.start_range = self.end_range = 0.
    def get_samples(self):
        return self.samples
    def get_stats(self):
        return ("drops=%d,overflows=%d"
                ",time_per_sample=%.9f,start_range=%.6f,end_range=%.6f"
                % (self.drops, self.overflows,
                   self.time_per_sample, self.start_range, self.end_range))
    def setup_data(self, axes_map, raw_samples, end_sequence, overflows,
                   start1_time, start2_time, end1_time, end2_time):
        if not raw_samples or not end_sequence:
            return
        (x_pos, x_scale), (y_pos, y_scale), (z_pos, z_scale) = axes_map
        self.overflows = overflows
        self.start_range = start2_time - start1_time
        self.end_range = end2_time - end1_time
        total_count = (end_sequence - 1) * 8 + len(raw_samples[-1][1]) // 6
        total_time = end2_time - start2_time
        self.time_per_sample = time_per_sample = total_time / total_count
        seq_to_time = time_per_sample * 8.
        self.samples = samples = [None] * total_count
        actual_count = 0
        for seq, data in raw_samples:
            d = bytearray(data)
            count = len(data)
            sdata = [(d[i] | (d[i+1] << 8)) - ((d[i+1] & 0x80) << 9)
                     for i in range(0, count-1, 2)]
            seq_time = start2_time + seq * seq_to_time
            for i in range(count//6):
                samp_time = seq_time + i * time_per_sample
                x = sdata[i*3 + x_pos] * x_scale
                y = sdata[i*3 + y_pos] * y_scale
                z = sdata[i*3 + z_pos] * z_scale
                samples[actual_count] = Accel_Measurement(samp_time, x, y, z)
                actual_count += 1
        del samples[actual_count:]
        self.drops = total_count - actual_count

# Printer class that controls measurments
class ADXL345:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.query_rate = 0
        self.last_tx_time = 0.
        am = {'x': (0, SCALE), 'y': (1, SCALE), 'z': (2, SCALE),
              '-x': (0, -SCALE), '-y': (1, -SCALE), '-z': (2, -SCALE)}
        axes_map = config.get('axes_map', 'x,y,z').split(',')
        if len(axes_map) != 3 or any([a.strip() not in am for a in axes_map]):
            raise config.error("Invalid adxl345 axes_map parameter")
        self.axes_map = [am[a.strip()] for a in axes_map]
        # Measurement storage (accessed from background thread)
        self.raw_samples = []
        self.last_sequence = 0
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
        # Register commands
        name = "default"
        if len(config.get_name().split()) > 1:
            name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("ACCELEROMETER_MEASURE", "CHIP", name,
                                   self.cmd_ACCELEROMETER_MEASURE,
                                   desc=self.cmd_ACCELEROMETER_MEASURE_help)
        if name == "default":
            gcode.register_mux_command("ACCELEROMETER_MEASURE", "CHIP", None,
                                       self.cmd_ACCELEROMETER_MEASURE)
    def _build_config(self):
        self.query_adxl345_cmd = self.mcu.lookup_command(
            "query_adxl345 oid=%c clock=%u rest_ticks=%u",
            cq=self.spi.get_command_queue())
        self.query_adxl345_end_cmd = self.mcu.lookup_query_command(
            "query_adxl345 oid=%c clock=%u rest_ticks=%u",
            "adxl345_end oid=%c end1_time=%u end2_time=%u"
            " limit_count=%hu sequence=%hu",
            oid=self.oid, cq=self.spi.get_command_queue())
    def _clock_to_print_time(self, clock):
        return self.mcu.clock_to_print_time(self.mcu.clock32_to_clock64(clock))
    def _handle_adxl345_start(self, params):
        self.samples_start1 = self._clock_to_print_time(params['start1_time'])
        self.samples_start2 = self._clock_to_print_time(params['start2_time'])
    def _handle_adxl345_data(self, params):
        last_sequence = self.last_sequence
        sequence = (last_sequence & ~0xffff) | params['sequence']
        if sequence < last_sequence:
            sequence += 0x10000
        self.last_sequence = sequence
        raw_samples = self.raw_samples
        if len(raw_samples) >= 200000:
            # Avoid filling up memory with too many samples
            return
        raw_samples.append((sequence, params['data']))
    def _convert_sequence(self, sequence):
        sequence = (self.last_sequence & ~0xffff) | sequence
        if sequence < self.last_sequence:
            sequence += 0x10000
        return sequence
    def start_measurements(self, rate=None):
        if rate is None:
            rate = 3200
        # Verify chip connectivity
        params = self.spi.spi_transfer([REG_DEVID | REG_MOD_READ, 0x00])
        response = bytearray(params['response'])
        if response[1] != 0xe5:
            raise self.printer.command_error("Invalid adxl345 id (got %x vs %x)"
                                             % (response[1], 0xe5))
        # Setup chip in requested query rate
        clock = 0
        if self.last_tx_time:
            clock = self.mcu.print_time_to_clock(self.last_tx_time)
        self.spi.spi_send([REG_POWER_CTL, 0x00], minclock=clock)
        self.spi.spi_send([REG_FIFO_CTL, 0x00])
        self.spi.spi_send([REG_DATA_FORMAT, 0x0B])
        self.spi.spi_send([REG_BW_RATE, QUERY_RATES[rate]])
        self.spi.spi_send([REG_FIFO_CTL, 0x80])
        # Setup samples
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.raw_samples = []
        self.last_sequence = 0
        self.samples_start1 = self.samples_start2 = print_time
        # Start bulk reading
        reqclock = self.mcu.print_time_to_clock(print_time)
        rest_ticks = self.mcu.seconds_to_clock(4. / rate)
        self.last_tx_time = print_time
        self.query_rate = rate
        self.query_adxl345_cmd.send([self.oid, reqclock, rest_ticks],
                                    reqclock=reqclock)
    def finish_measurements(self):
        query_rate = self.query_rate
        if not query_rate:
            return ADXL345Results()
        # Halt bulk reading
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        clock = self.mcu.print_time_to_clock(print_time)
        params = self.query_adxl345_end_cmd.send([self.oid, 0, 0],
                                                 minclock=clock)
        self.last_tx_time = print_time
        self.query_rate = 0
        raw_samples = self.raw_samples
        self.raw_samples = []
        # Generate results
        end1_time = self._clock_to_print_time(params['end1_time'])
        end2_time = self._clock_to_print_time(params['end2_time'])
        end_sequence = self._convert_sequence(params['sequence'])
        overflows = params['limit_count']
        res = ADXL345Results()
        res.setup_data(self.axes_map, raw_samples, end_sequence, overflows,
                       self.samples_start1, self.samples_start2,
                       end1_time, end2_time)
        logging.info("ADXL345 finished %d measurements: %s",
                     len(res.get_samples()), res.get_stats())
        return res
    def end_query(self, name):
        if not self.query_rate:
            return
        res = self.finish_measurements()
        # Write data to file
        f = open("/tmp/adxl345-%s.csv" % (name,), "w")
        f.write("##%s\n#time,accel_x,accel_y,accel_z\n" % (res.get_stats(),))
        for t, accel_x, accel_y, accel_z in res.get_samples():
            f.write("%.6f,%.6f,%.6f,%.6f\n" % (t, accel_x, accel_y, accel_z))
        f.close()
    cmd_ACCELEROMETER_MEASURE_help = "Start/stop accelerometer"
    def cmd_ACCELEROMETER_MEASURE(self, gcmd):
        rate = gcmd.get_int("RATE", 0)
        if not rate:
            name = gcmd.get("NAME", time.strftime("%Y%m%d_%H%M%S"))
            if not name.replace('-', '').replace('_', '').isalnum():
                raise gcmd.error("Invalid adxl345 NAME parameter")
            self.end_query(name)
            gcmd.respond_info("adxl345 measurements stopped")
        elif self.query_rate:
            raise gcmd.error("adxl345 already running")
        elif rate not in QUERY_RATES:
            raise gcmd.error("Not a valid adxl345 query rate")
        else:
            self.start_measurements(rate)

def load_config(config):
    return ADXL345(config)

def load_config_prefix(config):
    return ADXL345(config)
