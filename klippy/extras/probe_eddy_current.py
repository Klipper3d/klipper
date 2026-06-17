# Support for eddy current based Z probes
#
# Copyright (C) 2021-2026  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, logging, math, bisect
import mcu, mathutil
from . import ldc1612, trigger_analog, probe, manual_probe


######################################################################
# Calibration
######################################################################

OUT_OF_RANGE = 99.9

# Dummy temperature adjustments when "[temperature_probe]" not utilized
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

# Storage for frequency to height calibration
class EddyCalibration:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.drift_comp = DummyDriftCompensation()
        # Current calibration data
        self.cal_freqs = []
        self.cal_zpos = []
        cal = config.get('calibrate', None)
        if cal is not None:
            cal = [list(map(float, d.strip().split(':', 1)))
                   for d in cal.split(',')]
            self._load_calibration(cal)
    def _load_calibration(self, cal):
        cal = sorted([(c[1], c[0]) for c in cal])
        self.cal_freqs = [c[0] for c in cal]
        self.cal_zpos = [c[1] for c in cal]
    def get_printer(self):
        return self.printer
    def note_z_calibration_start(self):
        self.drift_comp.note_z_calibration_start()
    def note_z_calibration_finish(self):
        self.drift_comp.note_z_calibration_finish()
    def register_drift_compensation(self, comp):
        self.drift_comp = comp
    def verify_calibrated(self):
        if len(self.cal_freqs) <= 2:
            raise self.printer.command_error(
                "Must calibrate probe_eddy_current first")
    def get_calibration(self):
        return list(self.cal_freqs), list(self.cal_zpos)
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

# Implement PROBE_EDDY_CURRENT_CALIBRATE (and similar)
class EddyCalibrationTool:
    def __init__(self, config, calibration):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.calibration = calibration
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
    # PROBE_EDDY_CURRENT_CALIBRATE
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
        self.calibration.note_z_calibration_start()
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
        self.calibration.note_z_calibration_finish()
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
                msg = "z: %.3f # noise %.6fmm, MAD_Hz=%.3f\n" % (
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
    cmd_EDDY_CALIBRATE_help = "Calibrate eddy current probe"
    def cmd_EDDY_CALIBRATE(self, gcmd):
        self.probe_speed = gcmd.get_float("PROBE_SPEED", 5., above=0.)
        # Start manual probe
        manual_probe.ManualProbeHelper(self.printer, gcmd,
                                       self.post_manual_probe)
    # Z_OFFSET_APPLY_PROBE
    def _save_tap_z_offset(self, gcmd, homing_z):
        eventtime = self.printer.get_reactor().monotonic()
        configfile = self.printer.lookup_object('configfile')
        cstatus = configfile.get_status(eventtime)
        csettings = cstatus.get('settings', {}).get(self.name, {})
        tap_z_offset = csettings.get('tap_z_offset', 0.)
        new_calibrate = tap_z_offset - homing_z
        gcmd.respond_info(
            "%s: tap_z_offset: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer."
            % (self.name, new_calibrate))
        configfile.set(self.name, 'tap_z_offset', "%.3f" % (new_calibrate,))
    cmd_Z_OFFSET_APPLY_PROBE_help = "Adjust the probe's z_offset"
    def cmd_Z_OFFSET_APPLY_PROBE(self, gcmd):
        gcode_move = self.printer.lookup_object("gcode_move")
        offset = gcode_move.get_status()['homing_origin'].z
        if offset == 0:
            gcmd.respond_info("Nothing to do: Z Offset is 0")
            return
        if gcmd.get("METHOD", "").lower() == "tap":
            self._save_tap_z_offset(gcmd, offset)
            return
        cal_freqs, cal_zpos = self.calibration.get_calibration()
        cal_zpos = [z - offset for z in cal_zpos]
        z_freq_pairs = zip(cal_zpos, cal_freqs)
        z_freq_pairs = sorted(z_freq_pairs)
        self._save_calibration(z_freq_pairs)

# Tool for calibrating tap_threshold
class EddyTapCalibration:
    def __init__(self, config, calibration, eddy_tap):
        self._printer = config.get_printer()
        self._name = config.get_name()
        self._calibration = calibration
        self._eddy_tap = eddy_tap
        self._refine_tap_threshold = None
        gcode = self._printer.lookup_object("gcode")
        gcode.register_command("PROBE_EDDY_CURRENT_TAP_CALIBRATE",
                               self.cmd_TAP_CALIBRATE,
                               desc=self.cmd_TAP_CALIBRATE_help)
    def _analyze_main_calibration(self):
        freqs, zpos = self._calibration.get_calibration()
        if len(freqs) < 2:
            return None
        # Find best fit for: freq = c0 + c1*z + c2*z*z
        eqs = []
        ans = []
        for freq, z in zip(freqs, zpos):
            if z <= 0.750:
                ans.append([freq])
                eqs.append([1., z, z*z])
        return mathutil.solve_linear_equations(eqs, ans)
    def _describe_main_calibration(self, coeffs):
        if coeffs is None:
            return ["Main calibration data not available.", ""]
        msg = ("Calibration: f=%.3f s=%.3f q=%.3f"
               % (coeffs[0][0], coeffs[1][0], coeffs[2][0]))
        return [msg, ""]
    def _describe_last_tap(self, last_tap):
        if last_tap is None:
            return ["Run tap probe for last tap analysis."]
        status, depress_dist, coeffs = last_tap
        z_contact, freq_contact, depress_slope, slope, slope2 = coeffs
        contact_slope_delta = depress_slope - slope
        m1 = ("Last tap: z=%.6f f=%.3f s=%.3f q=%.3f"
              % (z_contact, freq_contact, slope, slope2))
        m2 = ("  depress_dist=%.6f depress_slope=%.3f"
              % (depress_dist, depress_slope))
        m3 = ("  contact_slope_delta=%.3f" % (contact_slope_delta,))
        msgs = [m1, m2, m3]
        if status != "success":
            msgs.extend(["", "Warning! Last tap did not succeed."])
        return msgs
    def _try_tap(self, gcmd, tap_threshold, samples=1):
        # Create dummy gcmd with SAMPLES=1
        fo_params = dict(gcmd.get_command_parameters())
        fo_params['METHOD'] = "tap"
        fo_params['TAP_THRESHOLD'] = "%.3f" % (tap_threshold,)
        fo_params['SAMPLES'] = str(samples)
        gcode = self._printer.lookup_object('gcode')
        fo_gcmd = gcode.create_gcode_command("", "", fo_params)
        gcmd.respond_info("Tap probing with TAP_THRESHOLD=%s SAMPLES=%s"
                          % (fo_params['TAP_THRESHOLD'], fo_params['SAMPLES']))
        # Run "tap" probe
        probe = self._printer.lookup_object('probe')
        probe_session = probe.start_probe_session(fo_gcmd)
        probe_session.run_probe(fo_gcmd)
        positions = probe_session.pull_probed_results()
        probe_session.end_probe_session()
        gcmd.respond_info("Tap probing reports z=%.6f" % (positions[0][2],))
    def _save_tap_threshold(self, gcmd, tap_threshold):
        configfile = self._printer.lookup_object('configfile')
        gcmd.respond_info(
            "%s: tap_threshold: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer."
            % (self._name, tap_threshold))
        configfile.set(self._name, 'tap_threshold', "%.3f" % (tap_threshold,))
    cmd_TAP_CALIBRATE_help = "Calibrate tap_threshold for 'tap' probing"
    def cmd_TAP_CALIBRATE(self, gcmd):
        mc_coeffs = self._analyze_main_calibration()
        last_tap = self._eddy_tap.get_last_tap_info()
        tap_test = gcmd.get("TAP", None)
        if tap_test is None:
            # Provide technical information
            mc_msgs = self._describe_main_calibration(mc_coeffs)
            lt_msgs = self._describe_last_tap(last_tap)
            gcmd.respond_info('\n'.join(mc_msgs + lt_msgs))
        elif tap_test == 'guess':
            # Attempt tap based on main calibration
            self._refine_tap_threshold = None
            if mc_coeffs is None:
                raise gcmd.error(
                    "Must complete PROBE_EDDY_CURRENT_CALIBRATE first")
            self._try_tap(gcmd, mc_coeffs[1][0] * -0.10)
        elif tap_test == 'refine':
            # Attempt tap based on change in slope observed during last tap
            self._refine_tap_threshold = None
            if last_tap is None or last_tap[0] != "success":
                raise gcmd.error("Must complete valid 'tap' probe first")
            status, depress_dist, coeffs = last_tap
            z_contact, freq_contact, depress_slope, slope, slope2 = coeffs
            contact_slope_delta = depress_slope - slope
            try_tap_threshold = contact_slope_delta * 0.20
            self._try_tap(gcmd, try_tap_threshold)
            self._refine_tap_threshold = try_tap_threshold
        elif tap_test == 'verify':
            # Retry tap several times to verify it is stable
            if self._refine_tap_threshold is None:
                raise gcmd.error("Must complete valid 'refine' step first")
            self._try_tap(gcmd, self._refine_tap_threshold, 5)
            self._save_tap_threshold(gcmd, self._refine_tap_threshold)
        else:
            raise gcmd.error("Please provide a valid TAP parameter")


######################################################################
# Measurement collection
######################################################################

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
                dummy_pr = manual_probe.create_probe_result((0., 0., 0.,))
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
    return manual_probe.create_probe_result(toolhead_pos,
                                            (offsets[0], offsets[1], sensor_z))


######################################################################
# Data fitting for "tap"
######################################################################

# Given a list of (frequency, z) pairs, find the coefficients
# z_contact, freq_contact, depress_slope, slope, and slope2 that best
# fit the data to the formulas `frequency = freq_contact +
# depress_slope*(z-z_contact)` when z<=z_contact and `frequency =
# freq_contact + slope*(z-z_contact) + slope2*(z-z_contact)*(z-z_contact)`
# when z>=z_contact.  This implements a form of non-linear least
# squares.
class TapBestFit:
    def __init__(self):
        self._least_squares_cache = {}
    def _build_ls_matrix(self, samples, est_z_contact):
        # The function here is only a reference for the optimized version below
        len_samples = len(samples)
        eqs = [[0.] * 4 for i in range(len_samples)]
        ans = [[0.] for i in range(len_samples)]
        for i, (step_z, sensor_freq) in enumerate(samples):
            ans[i][0] = sensor_freq
            eq = eqs[i]
            eq[0] = 1.
            if step_z <= est_z_contact:
                # 1*c0 + (z-ezc)*c1 + ezc*c2 + ezc*ezc*c3 = freq
                eq[1] = step_z - est_z_contact
                eq[2] = est_z_contact
                eq[3] = est_z_contact * est_z_contact
            else:
                # 1*c0 + 0*c1 + z*c2 + z*z*c3 = freq
                eq[1] = 0.
                eq[2] = step_z
                eq[3] = step_z * step_z
        eqst = mathutil.mat_transp(eqs)
        eqst_eqs = mathutil.mat_mat_mul(eqst, eqs)
        eqst_ans = mathutil.mat_mat_mul(eqst, ans)
        return eqst_eqs, eqst_ans
    def _build_sums(self, samples, num_le):
        sum_le_z = sum_le_z2 = sum_le_freq = sum_le_freq_z = 0.
        for z, freq in samples[:num_le]:
            sum_le_z += z
            sum_le_z2 += z**2
            sum_le_freq += freq
            sum_le_freq_z += freq*z
        sum_gt_z = sum_gt_z2 = sum_gt_z3 = sum_gt_z4 = 0.
        sum_gt_freq = sum_gt_freq_z = sum_gt_freq_z2 = 0.
        for z, freq in samples[num_le:]:
            sum_gt_z += z
            sum_gt_z2 += z**2
            sum_gt_z3 += z**3
            sum_gt_z4 += z**4
            sum_gt_freq += freq
            sum_gt_freq_z += freq*z
            sum_gt_freq_z2 += freq * z**2
        return (sum_le_z, sum_le_z2, sum_le_freq, sum_le_freq_z,
                sum_gt_z, sum_gt_z2, sum_gt_z3, sum_gt_z4,
                sum_gt_freq, sum_gt_freq_z, sum_gt_freq_z2)
    def _build_ls_matrix_opt(self, samples, est_z_contact):
        # This function is an optimized version of _build_ls_matrix()
        num_le = bisect.bisect(samples, (est_z_contact, sys.float_info.max))
        # Check for previously calculated raw freq/z counters
        sums = self._least_squares_cache.get(num_le)
        if sums is None:
            sums = self._build_sums(samples, num_le)
            self._least_squares_cache[num_le] = sums
        (sum_le_z, sum_le_z2, sum_le_freq, sum_le_freq_z,
         sum_gt_z, sum_gt_z2, sum_gt_z3, sum_gt_z4,
         sum_gt_freq, sum_gt_freq_z, sum_gt_freq_z2) = sums
        num_samples = len(samples)
        ezc = est_z_contact
        ezc2 = ezc**2
        ezc3 = ezc**3
        ezc4 = ezc**4
        # Build matrices for least squares evaluation
        eqst_eqs = [[0.] * 4 for i in range(4)]
        eqst_eqs[0][0] = num_samples
        eqst_eqs[1][1] = sum_le_z2 - 2*ezc*sum_le_z + num_le*ezc2
        eqst_eqs[2][2] = sum_gt_z2 + num_le*ezc2
        eqst_eqs[3][3] = sum_gt_z4 + num_le*ezc4
        eqst_eqs[0][1] = eqst_eqs[1][0] = sum_le_z - num_le*ezc
        eqst_eqs[0][2] = eqst_eqs[2][0] = sum_gt_z + num_le*ezc
        eqst_eqs[0][3] = eqst_eqs[3][0] = sum_gt_z2 + num_le*ezc2
        eqst_eqs[2][3] = eqst_eqs[3][2] = sum_gt_z3 + num_le*ezc3
        eqst_eqs[2][1] = eqst_eqs[1][2] = ezc * eqst_eqs[0][1]
        eqst_eqs[3][1] = eqst_eqs[1][3] = ezc2 * eqst_eqs[0][1]
        eqst_ans = [[0.] for i in range(4)]
        eqst_ans[0][0] = sum_le_freq + sum_gt_freq
        eqst_ans[1][0] = sum_le_freq_z - ezc*sum_le_freq
        eqst_ans[2][0] = sum_gt_freq_z + ezc*sum_le_freq
        eqst_ans[3][0] = sum_gt_freq_z2 + ezc2 * sum_le_freq
        return eqst_eqs, eqst_ans
    def _calc_least_squares(self, samples, est_z_contact):
        eqst_eqs, eqst_ans = self._build_ls_matrix_opt(samples, est_z_contact)
        coeffs = mathutil.gaussian_solve(eqst_eqs, eqst_ans)
        if coeffs is not None and coeffs[3][0] < 0.:
            # z**2 factor can't be negative - retry using only linear
            alt_eqst_eqs = [ee[:3] for ee in eqst_eqs[:3]]
            alt_eqst_ans = eqst_ans[:3]
            coeffs = mathutil.gaussian_solve(alt_eqst_eqs, alt_eqst_ans)
            if coeffs is not None:
                coeffs = coeffs + [[0.]]
        if coeffs is None:
            return sys.float_info.max, [[0.]] * 4
        rel_err = -sum([c[0]*a[0] for c, a in zip(coeffs, eqst_ans)])
        return rel_err, coeffs
    def find_best_fit(self, data):
        #for d in data:
        #    logging.info("sample: freq=%.3f z=%.6f", d[0], d[1][2])
        self._least_squares_cache.clear()
        # Change base of freq/z measurements to improve numerical stability
        base_z = .5 * (data[0][1][2] + data[-1][1][2])
        base_freq = .5 * (data[0][0] + data[-1][0])
        samples = [(d[1][2] - base_z, d[0] - base_freq) for d in data]
        # Run least squares with various z values to reduce residual error
        min_z = best_z = samples[0][0]
        max_z = samples[-1][0]
        best_err = sys.float_info.max
        best_coeffs = [0., 0., 0., 0.]
        while max_z - min_z > 0.000050:
            # Select z value to check
            mid_z = (min_z + max_z) * .5
            if best_z < mid_z:
                guess_z = (best_z + max_z) * .5
            else:
                guess_z = (min_z + best_z) * .5
            # Calculate least squares error for given z
            guess_err, coeffs = self._calc_least_squares(samples, guess_z)
            # Update search bounds
            if guess_err < best_err:
                if guess_z > best_z:
                    min_z = best_z
                else:
                    max_z = best_z
                best_z = guess_z
                best_err = guess_err
                best_coeffs = coeffs
            else:
                if guess_z > best_z:
                    max_z = guess_z
                else:
                    min_z = guess_z
        self._least_squares_cache.clear()
        # Return to original freq/z measurement base
        bc = [v[0] for v in best_coeffs]
        z_contact = base_z + best_z
        freq_contact = base_freq + bc[0] + best_z*bc[2] + best_z*best_z*bc[3]
        depress_slope = bc[1]
        slope = bc[2] + 2.*best_z*bc[3]
        slope2 = bc[3]
        #logging.info("probe_analysis: coeffs=%s",
        #             (z_contact, freq_contact, depress_slope, slope, slope2))
        return z_contact, freq_contact, depress_slope, slope, slope2


######################################################################
# Probe sessions
######################################################################

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
        if (config.get('z_offset', None, note_valid=False) is not None
            and config.get('descend_z', None, note_valid=False) is None):
            config.deprecate('z_offset')
            self._descend_z = config.getfloat('z_offset', above=0.)
        else:
            self._descend_z = config.getfloat('descend_z', above=0.)
        self._z_min_position = probe.lookup_minimum_z(config)
        self._gather = None
        probe.HomingViaProbeHelper(config, self._descend_z)
    def _prep_trigger_analog(self):
        sos_filter = self._trigger_analog.get_sos_filter()
        sos_filter.set_filter_design(None)
        sos_filter.set_offset_scale(0, 1.)
        self._trigger_analog.set_raw_range(0, MAX_VALID_RAW_VALUE)
        trigger_freq = self._calibration.height_to_freq(self._descend_z)
        conv_freq = self._sensor_helper.convert_frequency_to_raw(trigger_freq)
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
        check_movement = not phoming.check_probe_first_home(gcmd)
        trig_pos = phoming.probing_move(self._trigger_analog, pos, speed,
                                        check_movement=check_movement)
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
        self._tap_z_offset = config.getfloat('tap_z_offset', 0.)
        self._tap_threshold = config.getfloat('tap_threshold', 0., above=0.)
        self._current_tap_threshold = 0.
        self._setup_tap()
        self._last_tap = None
    # Setup for "tap" probe request
    def _setup_tap(self):
        # Create sos filter "design"
        cfg_error = self._printer.config_error
        sps = self._sensor_helper.get_samples_per_second()
        design = trigger_analog.DigitalFilter(sps, cfg_error)
        design.add_lowpass(25.0, 4)
        design.add_derivative()
        self._filter_design = design
        # Create SOS filter
        cmd_queue = self._trigger_analog.get_dispatch().get_command_queue()
        mcu = self._sensor_helper.get_mcu()
        filter_size = design.get_size()
        sos_filter = trigger_analog.MCU_SosFilter(mcu, cmd_queue, filter_size)
        self._trigger_analog.setup_sos_filter(sos_filter)
    def _prep_trigger_analog_tap(self, gcmd):
        tap_threshold = gcmd.get_float("TAP_THRESHOLD",
                                       self._tap_threshold, above=0.)
        if not tap_threshold:
            raise self._printer.command_error("Tap not configured")
        params = self._param_helper.get_probe_params(gcmd)
        # Setup mcu filter (scale internal values to milli-hz)
        sos_filter = self._trigger_analog.get_sos_filter()
        sos_filter.set_filter_design(self._filter_design)
        FRAC_HZ = 1000.
        s = FRAC_HZ * self._sensor_helper.convert_raw_to_frequency(1)
        sos_filter.set_offset_scale(0, s, auto_offset=True)
        self._trigger_analog.set_raw_range(0, MAX_VALID_RAW_VALUE)
        # Set mcu trigger to tap_threshold
        sps = self._sensor_helper.get_samples_per_second()
        adj_thresh = tap_threshold * params['probe_speed'] / sps
        samp_thresh = int(FRAC_HZ * adj_thresh + 0.5)
        self._trigger_analog.set_trigger('diff_peak_gt', samp_thresh)
        self._current_tap_threshold = tap_threshold
    # Measurement analysis to determine "tap" position
    def _validate_samples_time(self, measures, start_time, end_time):
        cmderr = self._printer.command_error
        if end_time - start_time < 0.100:
            raise cmderr("Tap detected too close to start of move")
        timestamps = [m[0] for m in measures]
        if len(timestamps) < 2:
            raise cmderr("Unable to obtain probe_eddy_current sensor readings")
        ts = [start_time] + timestamps + [end_time]
        tdiffs = [ts[i] - ts[i-1] for i in range(1, len(ts))]
        tmax = max(tdiffs)
        tmin = min(tdiffs[1:-1])
        cycle_time = 1.0 / self._sensor_helper.get_samples_per_second()
        if tmax > cycle_time * 1.25:
            raise cmderr("Eddy: Gaps in the data: %.3f > %.3f"
                         % (tmax, cycle_time * 1.25))
        if tmin < cycle_time * 0.75:
            raise cmderr("Eddy: CLKIN frequency too low: %.3f < %.3f"
                         % (tmin, cycle_time * 0.75))
    def _lookup_toolhead_pos(self, pos_time):
        toolhead = self._printer.lookup_object('toolhead')
        kin = toolhead.get_kinematics()
        kin_spos = {s.get_name(): s.mcu_to_commanded_position(
                                      s.get_past_mcu_position(pos_time))
                    for s in kin.get_steppers()}
        return kin.calc_position(kin_spos)
    def _error_detect(self, msg):
        raise self._printer.command_error("Unable to detect tap: %s" % (msg,))
    def _analyze_pullback(self, measures, start_time, end_time):
        self._last_tap = None
        reactor = self._printer.get_reactor()
        self._validate_samples_time(measures, start_time, end_time)
        # Correlate measurements to toolhead position at time of measurement
        data = [(sensor_freq, self._lookup_toolhead_pos(samp_time))
                for samp_time, sensor_freq, sensor_z in measures]
        reactor.pause(0.)
        min_z = data[0][1][2]
        max_z = data[-1][1][2]
        if max_z - min_z < 0.350:
            self._error_detect("insufficient lift (%.6f vs %.6f)"
                               % (max_z - min_z, 0.350))
        # Find best fit for extracted measurements
        tap_fit = TapBestFit()
        coeffs = tap_fit.find_best_fit(data)
        z_contact, freq_contact, depress_slope, slope, slope2 = coeffs
        self._last_tap = ("fail", z_contact - min_z, coeffs)
        reactor.pause(0.)
        sps = self._sensor_helper.get_samples_per_second()
        contact_slope_delta = depress_slope - slope
        if contact_slope_delta < self._current_tap_threshold:
            self._error_detect("insufficient slope delta (%.6f vs %.6f)"
                               % (contact_slope_delta,
                                  self._current_tap_threshold))
        if slope >= 0. or slope2 < 0.:
            self._error_detect("invalid free air slope (s=%.6f s2=%.6f)"
                               % (slope, slope2))
        if z_contact - min_z < 0.030 or z_contact - min_z > 0.250:
            self._error_detect("invalid depress distance (%.6f vs %.6f:%.6f)"
                               % (z_contact - min_z, 0.030, 0.250))
        self._last_tap = ("success", z_contact - min_z, coeffs)
        # Report probe position
        trig_idx = len(data)-1
        while trig_idx > 0 and data[trig_idx-1][1][2] > z_contact:
            trig_idx -= 1
        trig_pos = data[trig_idx][1]
        adj_z_contact = z_contact - self._tap_z_offset
        return manual_probe.ProbeResult(trig_pos[0], trig_pos[1], adj_z_contact,
                                        trig_pos[0], trig_pos[1], trig_pos[2])
    def get_last_tap_info(self):
        return self._last_tap
    # Probe session interface
    def start_probe_session(self, gcmd):
        self._prep_trigger_analog_tap(gcmd)
        self._gather = EddyGatherSamples(self._printer, self._sensor_helper)
        return self
    def run_probe(self, gcmd):
        toolhead = self._printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        pos[2] = self._z_min_position
        params = self._param_helper.get_probe_params(gcmd)
        speed = params['probe_speed']
        lift_speed = params['lift_speed']
        lift_dist = gcmd.get_float('SAMPLE_RETRACT_DIST', 4., above=0.)
        # Perform probing move
        phoming = self._printer.lookup_object('homing')
        trig_pos = phoming.probing_move(self._trigger_analog, pos, speed)
        # Perform lifting move
        haltpos = toolhead.get_position()
        haltpos[2] += lift_dist
        retract_start_time = toolhead.get_last_move_time()
        toolhead.manual_move(haltpos, lift_speed)
        # Extract retract samples
        start_time = retract_start_time - 0.010
        end_time = retract_start_time + 0.150
        self._gather.add_probe_request(self._analyze_pullback, start_time,
                                       end_time, start_time, end_time)
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
        self._printer.register_event_handler("gcode:command_error",
                                             self._handle_command_error)
    def _handle_command_error(self):
        if self._gather is not None:
            self.end_probe_session()
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
        self._is_rapid = gcmd.get("METHOD", "scan").lower() == 'rapid_scan'
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


######################################################################
# Main probe interface
######################################################################

# Eddy specific ProbeOffsets class (does not store z_offset)
class EddyProbeOffsets:
    def __init__(self, config):
        self.x_offset = config.getfloat('x_offset', 0.)
        self.y_offset = config.getfloat('y_offset', 0.)
    def get_offsets(self, gcmd=None):
        return self.x_offset, self.y_offset, 0.

# Wrapper around ProbeParameterHelper
class EddyParameterHelper:
    def __init__(self, config):
        self._param_helper = probe.ProbeParameterHelper(config)
    def get_probe_params(self, gcmd=None):
        method = None
        if gcmd is not None:
            method = gcmd.get('METHOD', '').lower()
        if method not in ['scan', 'rapid_scan', 'tap']:
            return self._param_helper.get_probe_params(gcmd)
        probe_speed = gcmd.get_float("PROBE_SPEED", 5.0, above=0.)
        lift_speed = gcmd.get_float("LIFT_SPEED", 5.0, above=0.)
        samples = gcmd.get_int("SAMPLES", 1, minval=1)
        samp_retract_dist = 0.
        samp_tolerance = gcmd.get_float("SAMPLES_TOLERANCE", 0.100, minval=0.)
        samp_retries = gcmd.get_int("SAMPLES_TOLERANCE_RETRIES", 0, minval=0)
        samples_result = gcmd.get("SAMPLES_RESULT", 'average')
        return {'probe_speed': probe_speed,
                'lift_speed': lift_speed,
                'samples': samples,
                'sample_retract_dist': samp_retract_dist,
                'samples_tolerance': samp_tolerance,
                'samples_tolerance_retries': samp_retries,
                'samples_result': samples_result}

# Main "printer object"
class PrinterEddyProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.calibration = EddyCalibration(config)
        EddyCalibrationTool(config, self.calibration)
        # Sensor type
        sensors = { "ldc1612": ldc1612.LDC1612 }
        sensor_type = config.getchoice('sensor_type', {s: s for s in sensors})
        self.sensor_helper = sensors[sensor_type](config, self.calibration)
        # Create trigger_analog interface
        trig_analog = trigger_analog.MCU_trigger_analog(self.sensor_helper)
        probe.LookupZSteppers(config, trig_analog.get_dispatch().add_stepper)
        # Basic probe requests
        self.probe_offsets = EddyProbeOffsets(config)
        self.param_helper = EddyParameterHelper(config)
        eddy_descend = EddyDescend(
            config, self.sensor_helper, self.calibration, self.probe_offsets,
            self.param_helper, trig_analog)
        self.eddy_descend_session = probe.SampleAveragingHelper(
            config, self.param_helper, eddy_descend.start_probe_session)
        # Probing via "tap" interface
        eddy_tap = EddyTap(config, self.sensor_helper,
                           self.param_helper, trig_analog)
        EddyTapCalibration(config, self.calibration, eddy_tap)
        self.eddy_tap_session = probe.SampleAveragingHelper(
            config, self.param_helper, eddy_tap.start_probe_session)
        # Probing via "scan" and "rapid_scan" requests
        self.eddy_scan = EddyScanningProbe(config, self.sensor_helper,
                                           self.calibration, self.probe_offsets)
        # Register with main probe interface
        self.cmd_helper = probe.ProbeCommandHelper(config, self,
                                                   can_set_z_offset=False)
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
    def start_probe_session(self, gcmd):
        method = gcmd.get('METHOD', 'automatic').lower()
        if method in ('scan', 'rapid_scan'):
            return self.eddy_scan.start_probe_session(gcmd)
        elif method == 'tap':
            return self.eddy_tap_session.start_probe_session(gcmd)
        return self.eddy_descend_session.start_probe_session(gcmd)
    def register_drift_compensation(self, comp):
        self.calibration.register_drift_compensation(comp)

def load_config_prefix(config):
    return PrinterEddyProbe(config)
