# Support for eddy current based Z probes
#
# Copyright (C) 2021-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math, bisect
import mcu
from . import ldc1612, trigger_analog, probe, manual_probe

OUT_OF_RANGE = 99.9

# Tool for calibrating the sensor Z detection and applying that calibration
class EddyCalibration:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.drift_comp = DummyDriftCompensation()
        # Current calibration data
        self.cal_freqs = []
        self.cal_zpos = []
        cal = config.get('calibrate', None)
        if cal is not None:
            cal = [list(map(float, d.strip().split(':', 1)))
                   for d in cal.split(',')]
            self.load_calibration(cal)
        # Probe calibrate state
        self.probe_speed = 0.
        # Register commands
        cname = self.name.split()[-1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("PROBE_EDDY_CURRENT_CALIBRATE", "CHIP",
                                   cname, self.cmd_EDDY_CALIBRATE,
                                   desc=self.cmd_EDDY_CALIBRATE_help)
        gcode.register_command('Z_OFFSET_APPLY_PROBE',
                               self.cmd_Z_OFFSET_APPLY_PROBE,
                               desc=self.cmd_Z_OFFSET_APPLY_PROBE_help)
    def get_printer(self):
        return self.printer
    def verify_calibrated(self):
        if len(self.cal_freqs) <= 2:
            raise self.printer.command_error(
                "Must calibrate probe_eddy_current first")
    def load_calibration(self, cal):
        cal = sorted([(c[1], c[0]) for c in cal])
        self.cal_freqs = [c[0] for c in cal]
        self.cal_zpos = [c[1] for c in cal]
    def apply_calibration(self, samples):
        cur_temp = self.drift_comp.get_temperature()
        for i, (samp_time, freq, dummy_z) in enumerate(samples):
            adj_freq = self.drift_comp.adjust_freq(freq, cur_temp)
            pos = bisect.bisect(self.cal_freqs, adj_freq)
            if pos >= len(self.cal_zpos):
                zpos = -OUT_OF_RANGE
            elif pos == 0:
                zpos = OUT_OF_RANGE
            else:
                # XXX - could further optimize and avoid div by zero
                this_freq = self.cal_freqs[pos]
                prev_freq = self.cal_freqs[pos - 1]
                this_zpos = self.cal_zpos[pos]
                prev_zpos = self.cal_zpos[pos - 1]
                gain = (this_zpos - prev_zpos) / (this_freq - prev_freq)
                offset = prev_zpos - prev_freq * gain
                zpos = adj_freq * gain + offset
            samples[i] = (samp_time, freq, round(zpos, 6))
    def freq_to_height(self, freq):
        dummy_sample = [(0., freq, 0.)]
        self.apply_calibration(dummy_sample)
        return dummy_sample[0][2]
    def height_to_freq(self, height):
        # XXX - could optimize lookup
        rev_zpos = list(reversed(self.cal_zpos))
        rev_freqs = list(reversed(self.cal_freqs))
        pos = bisect.bisect(rev_zpos, height)
        if pos == 0 or pos >= len(rev_zpos):
            raise self.printer.command_error(
                "Invalid probe_eddy_current height")
        this_freq = rev_freqs[pos]
        prev_freq = rev_freqs[pos - 1]
        this_zpos = rev_zpos[pos]
        prev_zpos = rev_zpos[pos - 1]
        gain = (this_freq - prev_freq) / (this_zpos - prev_zpos)
        offset = prev_freq - prev_zpos * gain
        freq = height * gain + offset
        return self.drift_comp.unadjust_freq(freq)
    def do_calibration_moves(self, move_speed):
        toolhead = self.printer.lookup_object('toolhead')
        kin = toolhead.get_kinematics()
        move = toolhead.manual_move
        # Start data collection
        msgs = []
        is_finished = False
        def handle_batch(msg):
            if is_finished:
                return False
            msgs.append(msg)
            return True
        self.printer.lookup_object(self.name).add_client(handle_batch)
        toolhead.dwell(1.)
        self.drift_comp.note_z_calibration_start()
        # Move to each 40um position
        max_z = 4.0
        samp_dist = 0.040
        req_zpos = [i*samp_dist for i in range(int(max_z / samp_dist) + 1)]
        start_pos = toolhead.get_position()
        times = []
        for zpos in req_zpos:
            # Move to next position (always descending to reduce backlash)
            hop_pos = list(start_pos)
            hop_pos[2] += zpos + 0.500
            move(hop_pos, move_speed)
            next_pos = list(start_pos)
            next_pos[2] += zpos
            move(next_pos, move_speed)
            # Note sample timing
            start_query_time = toolhead.get_last_move_time() + 0.050
            end_query_time = start_query_time + 0.100
            toolhead.dwell(0.200)
            # Find Z position based on actual commanded stepper position
            toolhead.flush_step_generation()
            kin_spos = {s.get_name(): s.get_commanded_position()
                        for s in kin.get_steppers()}
            kin_pos = kin.calc_position(kin_spos)
            times.append((start_query_time, end_query_time, kin_pos[2]))
        toolhead.dwell(1.0)
        toolhead.wait_moves()
        self.drift_comp.note_z_calibration_finish()
        # Finish data collection
        is_finished = True
        # Correlate query responses
        cal = {}
        step = 0
        for msg in msgs:
            for query_time, freq, old_z in msg['data']:
                # Add to step tracking
                while step < len(times) and query_time > times[step][1]:
                    step += 1
                if step < len(times) and query_time >= times[step][0]:
                    cal.setdefault(times[step][2], []).append(freq)
        if len(cal) != len(times):
            raise self.printer.command_error(
                "Failed calibration - incomplete sensor data")
        return cal

    def _median(self, values):
        values = sorted(values)
        n = len(values)
        if n % 2 == 0:
            return (values[n//2 - 1] + values[n//2]) / 2.0
        return values[n // 2]
    def calc_freqs(self, meas):
        positions = {}
        for pos, freqs in meas.items():
            count = len(freqs)
            freq_avg = float(sum(freqs)) / count
            mads = [abs(f - freq_avg) for f in freqs]
            mad = self._median(mads)
            positions[pos] = (freq_avg, mad, count)
        return positions
    def validate_calibration_data(self, positions):
        last_freq = 40000000.
        last_pos = last_mad = .0
        gcode = self.printer.lookup_object("gcode")
        filtered = []
        mad_hz_total = .0
        mad_mm_total = .0
        samples_count = 0
        for pos, (freq_avg, mad_hz, count) in sorted(positions.items()):
            if freq_avg > last_freq:
                gcode.respond_info(
                    "Frequency stops decreasing at step %.3f" % (pos))
                break
            diff_mad = math.sqrt(last_mad**2 + mad_hz**2)
            # Calculate if samples have a significant difference
            freq_diff = last_freq - freq_avg
            last_freq = freq_avg
            if freq_diff < 2.5 * diff_mad:
                gcode.respond_info(
                    "Frequency too noisy at step %.3f -> %.3f" % (
                        last_pos, pos))
                gcode.respond_info(
                    "Frequency diff: %.3f, MAD_Hz: %.3f -> MAD_Hz: %.3f" % (
                        freq_diff, last_mad, mad_hz
                    ))
                break
            last_mad = mad_hz
            delta_dist = pos - last_pos
            last_pos = pos
            # MAD is Median Absolute Deviation to Frequency avg ~ delta_hz_1
            # Signal is delta_hz_2 / delta_dist
            # SNR ~= delta_hz_1 / (delta_hz_2 / delta_mm) = d_1 * d_mm / d_2
            mad_mm = mad_hz * delta_dist / freq_diff
            filtered.append((pos, freq_avg, mad_hz, mad_mm))
            mad_hz_total += mad_hz
            mad_mm_total += mad_mm
            samples_count += count
        avg_mad = mad_hz_total / len(filtered)
        avg_mad_mm = mad_mm_total / len(filtered)
        gcode.respond_info(
            "probe_eddy_current: noise %.6fmm, MAD_Hz=%.3f in %d queries\n" % (
                avg_mad_mm, avg_mad, samples_count))
        freq_list = [freq for _, freq, _, _ in filtered]
        freq_diff = max(freq_list) - min(freq_list)
        gcode.respond_info("Total frequency range: %.3f Hz\n" % (freq_diff))
        points = [0.25, 0.5, 1.0, 2.0, 3.0]
        for pos, _, mad_hz, mad_mm in filtered:
            if len(points) and points[0] <= pos:
                points.pop(0)
                msg = "z_offset: %.3f # noise %.6fmm, MAD_Hz=%.3f\n" % (
                    pos, mad_mm, mad_hz)
                gcode.respond_info(msg)
        return filtered
    def post_manual_probe(self, mpresult):
        if mpresult is None:
            # Manual Probe was aborted
            return
        curpos = [mpresult.bed_x, mpresult.bed_y, mpresult.bed_z]
        move = self.printer.lookup_object('toolhead').manual_move
        # Move away from the bed
        probe_calibrate_z = curpos[2]
        curpos[2] += 5.
        move(curpos, self.probe_speed)
        # Move sensor over nozzle position
        pprobe = self.printer.lookup_object("probe")
        x_offset, y_offset, z_offset = pprobe.get_offsets()
        curpos[0] -= x_offset
        curpos[1] -= y_offset
        move(curpos, self.probe_speed)
        # Descend back to bed
        curpos[2] -= 5. - 0.050
        move(curpos, self.probe_speed)
        # Perform calibration movement and capture
        cal = self.do_calibration_moves(self.probe_speed)
        # Calculate each sample position average and variance
        _positions = self.calc_freqs(cal)
        # Fix Z position offset
        positions = {}
        for k in _positions:
            v = _positions[k]
            k = k - probe_calibrate_z
            positions[k] = v
        filtered = self.validate_calibration_data(positions)
        if len(filtered) <= 8:
           raise self.printer.command_error(
              "Failed calibration - No usable data")
        z_freq_pairs = [(pos, freq) for pos, freq, _, _ in filtered]
        self._save_calibration(z_freq_pairs)
    def _save_calibration(self, z_freq_pairs):
        gcode = self.printer.lookup_object("gcode")
        gcode.respond_info(
            "The SAVE_CONFIG command will update the printer config file\n"
            "and restart the printer.")
        # Save results
        cal_contents = []
        for i, (pos, freq) in enumerate(z_freq_pairs):
            if not i % 3:
                cal_contents.append('\n')
            cal_contents.append("%.6f:%.3f" % (pos, freq))
            cal_contents.append(',')
        cal_contents.pop()
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.name, 'calibrate', ''.join(cal_contents))
    cmd_EDDY_CALIBRATE_help = "Calibrate eddy current probe"
    def cmd_EDDY_CALIBRATE(self, gcmd):
        self.probe_speed = gcmd.get_float("PROBE_SPEED", 5., above=0.)
        # Start manual probe
        manual_probe.ManualProbeHelper(self.printer, gcmd,
                                       self.post_manual_probe)
    cmd_Z_OFFSET_APPLY_PROBE_help = "Adjust the probe's z_offset"
    def cmd_Z_OFFSET_APPLY_PROBE(self, gcmd):
        gcode_move = self.printer.lookup_object("gcode_move")
        offset = gcode_move.get_status()['homing_origin'].z
        if offset == 0:
            gcmd.respond_info("Nothing to do: Z Offset is 0")
            return
        cal_zpos = [z - offset for z in self.cal_zpos]
        z_freq_pairs = zip(cal_zpos, self.cal_freqs)
        z_freq_pairs = sorted(z_freq_pairs)
        self._save_calibration(z_freq_pairs)
    def register_drift_compensation(self, comp):
        self.drift_comp = comp

# Tool to gather samples and convert them to probe positions
class EddyGatherSamples:
    def __init__(self, printer, sensor_helper):
        self._printer = printer
        self._sensor_helper = sensor_helper
        # Sensor reading
        self._sensor_messages = []
        self._need_stop = False
        # Probe request and results storage
        self._probe_requests = []
        self._analysis_results = []
        # Start samples
        sensor_helper.add_client(self._add_sensor_message)
    # Sensor reading and measurement extraction
    def _add_sensor_message(self, msg):
        if self._need_stop:
            del self._sensor_messages[:]
            return False
        self._sensor_messages.append(msg)
        self._check_sensor_messages()
        return True
    def finish(self):
        self._need_stop = True
    def _pull_measurements(self, start_time, end_time):
        # Extract measurements from sensor messages for given time range
        measures = []
        msg_num = discard_msgs = 0
        while msg_num < len(self._sensor_messages):
            msg = self._sensor_messages[msg_num]
            msg_num += 1
            data = msg['data']
            if data[0][0] > end_time:
                break
            if data[-1][0] < start_time:
                discard_msgs = msg_num
                continue
            for measure in data:
                time = measure[0]
                if time < start_time:
                    continue
                if time > end_time:
                    break
                measures.append(measure)
        del self._sensor_messages[:discard_msgs]
        return measures
    def _check_sensor_messages(self):
        while self._sensor_messages and self._probe_requests:
            cb, start_time, end_time, args = self._probe_requests[0]
            if self._sensor_messages[-1]['data'][-1][0] < end_time:
                break
            measures = self._pull_measurements(start_time, end_time)
            errmsg = res = None
            try:
                # Call analysis callback to process measurements
                res = cb(measures, *args)
            except self._printer.command_error as e:
                # Defer raising of errors to pull_probed()
                errmsg = str(e)
            self._analysis_results.append((res, errmsg))
            self._probe_requests.pop(0)
    def add_probe_request(self, cb, start_time, end_time, *args):
        self._probe_requests.append((cb, start_time, end_time, args))
        self._check_sensor_messages()
    # Extract probe results
    def _await_sensor_messages(self):
        # Make sure enough samples have been collected
        reactor = self._printer.get_reactor()
        mcu = self._sensor_helper.get_mcu()
        while self._probe_requests:
            cb, start_time, end_time, args = self._probe_requests[0]
            systime = reactor.monotonic()
            est_print_time = mcu.estimated_print_time(systime)
            if est_print_time > end_time + 1.0:
                raise self._printer.command_error(
                    "probe_eddy_current sensor outage")
            if mcu.is_fileoutput():
                # In debugging mode - just create dummy response
                dummy_pr = manual_probe.ProbeResult(0., 0., 0., 0., 0., 0.)
                self._analysis_results.append((dummy_pr, None))
                self._probe_requests.pop(0)
                continue
            reactor.pause(systime + 0.010)
    def pull_probed(self):
        self._await_sensor_messages()
        results = []
        for res, errmsg in self._analysis_results:
            if errmsg is not None:
                raise self._printer.command_error(errmsg)
            results.append(res)
        del self._analysis_results[:]
        return results

# Generate a ProbeResult from the average of a set of measurements
def probe_results_from_avg(measures, toolhead_pos, calibration, offsets):
    cmderr = calibration.get_printer().command_error
    if not measures:
        raise cmderr("Unable to obtain probe_eddy_current sensor readings")
    # Determine average of measurements
    freq_sum = sum([m[1] for m in measures])
    freq_avg = freq_sum / len(measures)
    # Determine height associated with frequency
    sensor_z = calibration.freq_to_height(freq_avg)
    if sensor_z <= -OUT_OF_RANGE or sensor_z >= OUT_OF_RANGE:
        raise cmderr("probe_eddy_current sensor not in valid range")
    return manual_probe.ProbeResult(
        toolhead_pos[0] + offsets[0], toolhead_pos[1] + offsets[1],
        toolhead_pos[2] - sensor_z,
        toolhead_pos[0], toolhead_pos[1], toolhead_pos[2])

MAX_VALID_RAW_VALUE=0x03ffffff

# Helper for implementing PROBE style commands (descend until trigger)
class EddyDescend:
    def __init__(self, config, sensor_helper, calibration,
                 probe_offsets, param_helper, trigger_analog):
        self._printer = config.get_printer()
        self._sensor_helper = sensor_helper
        self._calibration = calibration
        self._probe_offsets = probe_offsets
        self._param_helper = param_helper
        self._trigger_analog = trigger_analog
        self._z_min_position = probe.lookup_minimum_z(config)
        self._gather = None
    def _prep_trigger_analog(self):
        sos_filter = self._trigger_analog.get_sos_filter()
        sos_filter.set_filter_design(None)
        sos_filter.set_offset_scale(0, 1.)
        self._trigger_analog.set_raw_range(0, MAX_VALID_RAW_VALUE)
        z_offset = self._probe_offsets.get_offsets()[2]
        trigger_freq = self._calibration.height_to_freq(z_offset)
        conv_freq = self._sensor_helper.convert_frequency(trigger_freq)
        self._trigger_analog.set_trigger('gt', conv_freq)
    # Probe session interface
    def start_probe_session(self, gcmd):
        self._calibration.verify_calibrated()
        self._prep_trigger_analog()
        self._gather = EddyGatherSamples(self._printer, self._sensor_helper)
        return self
    def run_probe(self, gcmd):
        toolhead = self._printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        pos[2] = self._z_min_position
        speed = self._param_helper.get_probe_params(gcmd)['probe_speed']
        # Perform probing move
        phoming = self._printer.lookup_object('homing')
        trig_pos = phoming.probing_move(self._trigger_analog, pos, speed)
        # Extract samples
        start_time = self._trigger_analog.get_last_trigger_time() + 0.050
        end_time = start_time + 0.100
        toolhead_pos = toolhead.get_position()
        offsets = self._probe_offsets.get_offsets()
        self._gather.add_probe_request(probe_results_from_avg,
                                       start_time, end_time,
                                       toolhead_pos, self._calibration, offsets)
    def pull_probed_results(self):
        return self._gather.pull_probed()
    def end_probe_session(self):
        self._gather.finish()
        self._gather = None

# Wrapper to emulate mcu_endstop for probe:z_virtual_endstop
# Note that this does not provide accurate results
class EddyEndstopWrapper:
    def __init__(self, sensor_helper, eddy_descend):
        self._sensor_helper = sensor_helper
        self._eddy_descend = eddy_descend
        self._hw_probe_session = None
    # Interface for MCU_endstop
    def get_mcu(self):
        return self._sensor_helper.get_mcu()
    def add_stepper(self, stepper):
        pass
    def get_steppers(self):
        return self._eddy_descend._trigger_analog.get_steppers()
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        return self._eddy_descend._trigger_analog.home_start(
            print_time, sample_time, sample_count, rest_time, triggered)
    def home_wait(self, home_end_time):
        return self._eddy_descend._trigger_analog.home_wait(home_end_time)
    def query_endstop(self, print_time):
        return False # XXX
    # Interface for HomingViaProbeHelper
    def multi_probe_begin(self):
        self._hw_probe_session = self._eddy_descend.start_probe_session(None)
    def multi_probe_end(self):
        self._hw_probe_session.end_probe_session()
        self._hw_probe_session = None
    def probe_prepare(self, hmove):
        pass
    def probe_finish(self, hmove):
        pass
    def get_position_endstop(self):
        z_offset = self._eddy_descend._probe_offsets.get_offsets()[2]
        return z_offset

# Probing helper for "tap" requests
class EddyTap:
    def __init__(self, config, sensor_helper, param_helper, trigger_analog):
        self._printer = config.get_printer()
        self._sensor_helper = sensor_helper
        self._param_helper = param_helper
        self._trigger_analog = trigger_analog
        self._z_min_position = probe.lookup_minimum_z(config)
        self._gather = None
        self._filter_design = None
        self._tap_threshold = config.getfloat('tap_threshold', 0., minval=0.)
        if self._tap_threshold:
            self._setup_tap()
    # Setup for "tap" probe request
    def _setup_tap(self):
        # Create sos filter "design"
        cfg_error = self._printer.config_error
        sps = self._sensor_helper.get_samples_per_second()
        design = trigger_analog.DigitalFilter(sps, cfg_error,
                                              lowpass=25.0, lowpass_order=4)
        # Create the derivative (sample to sample difference) post filter
        self._filter_design = trigger_analog.DerivativeFilter(design)
        # Create SOS filter
        cmd_queue = self._trigger_analog.get_dispatch().get_command_queue()
        mcu = self._sensor_helper.get_mcu()
        sos_filter = trigger_analog.MCU_SosFilter(mcu, cmd_queue, 5)
        self._trigger_analog.setup_sos_filter(sos_filter)
    def _prep_trigger_analog_tap(self):
        if not self._tap_threshold:
            raise self._printer.command_error("Tap not configured")
        sos_filter = self._trigger_analog.get_sos_filter()
        sos_filter.set_filter_design(self._filter_design)
        sos_filter.set_offset_scale(0, 1., auto_offset=True)
        self._trigger_analog.set_raw_range(0, MAX_VALID_RAW_VALUE)
        convert_frequency = self._sensor_helper.convert_frequency
        raw_threshold = convert_frequency(self._tap_threshold)
        self._trigger_analog.set_trigger('diff_peak_gt', raw_threshold)
    # Measurement analysis to determine "tap" position
    def central_diff(self, times, values):
        velocity = [0.0] * len(values)
        for i in range(1, len(values) - 1):
            delta_v = (values[i+1] - values[i-1])
            delta_t = (times[i+1] - times[i-1])
            velocity[i] = delta_v / delta_t
        velocity[0] = (values[1] - values[0]) / (times[1] - times[0])
        velocity[-1] = (values[-1] - values[-2]) / (times[-1] - times[-2])
        return velocity
    def validate_samples_time(self, timestamps):
        sps = self._sensor_helper.get_samples_per_second()
        cycle_time = 1.0 / sps
        SYNC_SLACK = 0.001
        for i in range(1, len(timestamps)):
            tdiff = timestamps[i] - timestamps[i-1]
            if cycle_time + SYNC_SLACK < tdiff:
                logging.error("Eddy: Gaps in the data: %.3f < %.3f" % (
                    (cycle_time + SYNC_SLACK, tdiff)
                ))
                break
            if cycle_time - SYNC_SLACK > tdiff:
                logging.error(
                    "Eddy: CLKIN frequency too low: %.3f > %.3f" % (
                        (cycle_time - SYNC_SLACK, tdiff)
                    ))
                break
    def _pull_tap_time(self, measures):
        tap_time = []
        tap_value = []
        for time, freq, z in measures:
            tap_time.append(time)
            tap_value.append(freq)
        # If samples have gaps this will not produce adequate data
        self.validate_samples_time(tap_time)
        # Do the same filtering as on the MCU but without induced lag
        main_design = self._filter_design.get_main_filter()
        try:
            fvals = main_design.filtfilt(tap_value)
        except ValueError as e:
            raise self._printer.command_error(str(e))
        velocity = self.central_diff(tap_time, fvals)
        peak_velocity = max(velocity)
        i = velocity.index(peak_velocity)
        return tap_time[i]
    def _lookup_toolhead_pos(self, pos_time):
        toolhead = self._printer.lookup_object('toolhead')
        kin = toolhead.get_kinematics()
        kin_spos = {s.get_name(): s.mcu_to_commanded_position(
                                      s.get_past_mcu_position(pos_time))
                    for s in kin.get_steppers()}
        return kin.calc_position(kin_spos)
    def _analyze_tap(self, measures):
        pos_time = self._pull_tap_time(measures)
        trig_pos = self._lookup_toolhead_pos(pos_time)
        return manual_probe.ProbeResult(trig_pos[0], trig_pos[1], trig_pos[2],
                                        trig_pos[0], trig_pos[1], trig_pos[2])
    # Probe session interface
    def start_probe_session(self, gcmd):
        self._prep_trigger_analog_tap()
        self._gather = EddyGatherSamples(self._printer, self._sensor_helper)
        return self
    def run_probe(self, gcmd):
        toolhead = self._printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        pos[2] = self._z_min_position
        speed = self._param_helper.get_probe_params(gcmd)['probe_speed']
        move_start_time = toolhead.get_last_move_time()
        # Perform probing move
        phoming = self._printer.lookup_object('homing')
        trig_pos = phoming.probing_move(self._trigger_analog, pos, speed)
        # Extract samples
        trigger_time = self._trigger_analog.get_last_trigger_time()
        start_time = trigger_time - 0.250
        if start_time < move_start_time:
            # Filter short move
            start_time = move_start_time
        end_time = trigger_time
        self._gather.add_probe_request(self._analyze_tap, start_time, end_time)
    def pull_probed_results(self):
        return self._gather.pull_probed()
    def end_probe_session(self):
        self._gather.finish()
        self._gather = None

# Implementing probing with "METHOD=scan"
class EddyScanningProbe:
    def __init__(self, config, sensor_helper, calibration, probe_offsets):
        self._printer = config.get_printer()
        self._sensor_helper = sensor_helper
        self._calibration = calibration
        self._offsets = probe_offsets.get_offsets()
        self._gather = None
        self._sample_time_delay = 0.050
        self._sample_time = 0.
        self._is_rapid = False
    def _lookup_toolhead_pos(self, pos_time):
        toolhead = self._printer.lookup_object('toolhead')
        kin = toolhead.get_kinematics()
        kin_spos = {s.get_name(): s.mcu_to_commanded_position(
                                      s.get_past_mcu_position(pos_time))
                    for s in kin.get_steppers()}
        return kin.calc_position(kin_spos)
    def _analyze_scan(self, measures, pos_time):
        toolhead_pos = self._lookup_toolhead_pos(pos_time)
        return probe_results_from_avg(measures, toolhead_pos,
                                      self._calibration, self._offsets)
    def _rapid_lookahead_cb(self, printtime):
        start_time = printtime - self._sample_time / 2
        end_time = start_time + self._sample_time
        self._gather.add_probe_request(self._analyze_scan, start_time, end_time,
                                       printtime)
    # Probe session interface
    def start_probe_session(self, gcmd):
        self._calibration.verify_calibrated()
        self._gather = EddyGatherSamples(self._printer, self._sensor_helper)
        self._sample_time = gcmd.get_float("SAMPLE_TIME", 0.100, above=0.0)
        self._is_rapid = gcmd.get("METHOD", "scan") == 'rapid_scan'
        return self
    def run_probe(self, gcmd):
        toolhead = self._printer.lookup_object("toolhead")
        if self._is_rapid:
            toolhead.register_lookahead_callback(self._rapid_lookahead_cb)
            return
        printtime = toolhead.get_last_move_time()
        toolhead.dwell(self._sample_time_delay + self._sample_time)
        start_time = printtime + self._sample_time_delay
        end_time = start_time + self._sample_time
        self._gather.add_probe_request(self._analyze_scan, start_time, end_time,
                                       start_time)
    def pull_probed_results(self):
        if self._is_rapid:
            # Flush lookahead (so all lookahead callbacks are invoked)
            toolhead = self._printer.lookup_object("toolhead")
            toolhead.get_last_move_time()
        results = self._gather.pull_probed()
        # Allow axis_twist_compensation to update results
        self._printer.send_event("probe:update_results", results)
        return results
    def end_probe_session(self):
        self._gather.finish()
        self._gather = None

# Main "printer object"
class PrinterEddyProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.calibration = EddyCalibration(config)
        # Sensor type
        sensors = { "ldc1612": ldc1612.LDC1612 }
        sensor_type = config.getchoice('sensor_type', {s: s for s in sensors})
        self.sensor_helper = sensors[sensor_type](config, self.calibration)
        # Create trigger_analog interface
        trig_analog = trigger_analog.MCU_trigger_analog(self.sensor_helper)
        probe.LookupZSteppers(config, trig_analog.get_dispatch().add_stepper)
        # Basic probe requests
        self.probe_offsets = probe.ProbeOffsetsHelper(config)
        self.param_helper = probe.ProbeParameterHelper(config)
        self.eddy_descend = EddyDescend(
            config, self.sensor_helper, self.calibration, self.probe_offsets,
            self.param_helper, trig_analog)
        # Create wrapper to support Z homing with probe
        mcu_probe = EddyEndstopWrapper(self.sensor_helper, self.eddy_descend)
        probe.HomingViaProbeHelper(config, mcu_probe,
                                   self.probe_offsets, self.param_helper)
        # Probing via "tap" interface
        self.eddy_tap = EddyTap(config, self.sensor_helper,
                                self.param_helper, trig_analog)
        # Probing via "scan" and "rapid_scan" requests
        self.eddy_scan = EddyScanningProbe(config, self.sensor_helper,
                                           self.calibration, self.probe_offsets)
        # Register with main probe interface
        self.cmd_helper = probe.ProbeCommandHelper(config, self,
                                                   can_set_z_offset=False)
        self.probe_session = probe.ProbeSessionHelper(
            config, self.param_helper, self._start_descend_wrapper)
        self.printer.add_object('probe', self)
    def add_client(self, cb):
        self.sensor_helper.add_client(cb)
    def get_probe_params(self, gcmd=None):
        return self.param_helper.get_probe_params(gcmd)
    def get_offsets(self, gcmd=None):
        if gcmd is not None and gcmd.get('METHOD', '').lower() == "tap":
            return (0., 0., 0.)
        return self.probe_offsets.get_offsets(gcmd)
    def get_status(self, eventtime):
        return self.cmd_helper.get_status(eventtime)
    def _start_descend_wrapper(self, gcmd):
        method = gcmd.get('METHOD', 'automatic').lower()
        if method == "tap":
            return self.eddy_tap.start_probe_session(gcmd)
        return self.eddy_descend.start_probe_session(gcmd)
    def start_probe_session(self, gcmd):
        method = gcmd.get('METHOD', 'automatic').lower()
        if method in ('scan', 'rapid_scan'):
            return self.eddy_scan.start_probe_session(gcmd)
        # For "tap" and normal, probe_session can average multiple attempts
        return self.probe_session.start_probe_session(gcmd)
    def register_drift_compensation(self, comp):
        self.calibration.register_drift_compensation(comp)

class DummyDriftCompensation:
    def get_temperature(self):
        return 0.
    def note_z_calibration_start(self):
        pass
    def note_z_calibration_finish(self):
        pass
    def adjust_freq(self, freq, temp=None):
        return freq
    def unadjust_freq(self, freq, temp=None):
        return freq

def load_config_prefix(config):
    return PrinterEddyProbe(config)
