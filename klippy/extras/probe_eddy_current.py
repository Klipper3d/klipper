# Support for eddy current based Z probes
#
# Copyright (C) 2021-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math, bisect
import mcu
from . import ldc1612, probe, manual_probe
from mathutil import Polynomial2d

OUT_OF_RANGE = 99.9

# Tool for calibrating the sensor Z detection and applying that calibration
class EddyCalibration:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.drift_comp = EddyDriftCompensation(config)
        # Current calibration data
        self.cal_freqs = []
        self.cal_zpos = []
        self.cal_temp = config.getfloat('calibration_temp', 0)
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
    def is_calibrated(self):
        return len(self.cal_freqs) > 2
    def load_calibration(self, cal):
        cal = sorted([(c[1], c[0]) for c in cal])
        self.cal_freqs = [c[0] for c in cal]
        self.cal_zpos = [c[1] for c in cal]
    def apply_calibration(self, samples):
        for i, (samp_time, freq, dummy_z) in enumerate(samples):
            adj_freq = self.drift_comp.adjust_freq(freq, self.cal_temp)
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
        return self.drift_comp.lookup_freq(freq, self.cal_temp)
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
        temp = self.drift_comp.get_temperature()
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
        temp = (temp + self.drift_comp.get_temperature()) / 2.
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
        return cal, temp
    def calc_freqs(self, meas):
        total_count = total_variance = 0
        positions = {}
        for pos, freqs in meas.items():
            count = len(freqs)
            freq_avg = float(sum(freqs)) / count
            positions[pos] = freq_avg
            total_count += count
            total_variance += sum([(f - freq_avg)**2 for f in freqs])
        return positions, math.sqrt(total_variance / total_count), total_count
    def post_manual_probe(self, kin_pos):
        if kin_pos is None:
            # Manual Probe was aborted
            return
        curpos = list(kin_pos)
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
        cal, temp = self.do_calibration_moves(self.probe_speed)
        # Calculate each sample position average and variance
        positions, std, total = self.calc_freqs(cal)
        last_freq = 0.
        for pos, freq in reversed(sorted(positions.items())):
            if freq <= last_freq:
                raise self.printer.command_error(
                    "Failed calibration - frequency not increasing each step")
            last_freq = freq
        gcode = self.printer.lookup_object("gcode")
        gcode.respond_info(
            "probe_eddy_current: stddev=%.3f in %d queries\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "and restart the printer." % (std, total))
        # Save results
        cal_contents = []
        for i, (pos, freq) in enumerate(sorted(positions.items())):
            if not i % 3:
                cal_contents.append('\n')
            cal_contents.append("%.6f:%.3f" % (pos - probe_calibrate_z, freq))
            cal_contents.append(',')
        cal_contents.pop()
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.name, 'calibrate', ''.join(cal_contents))
        configfile.set(self.name, 'calibration_temp', "%.6f" % (temp,))
    cmd_EDDY_CALIBRATE_help = "Calibrate eddy current probe"
    def cmd_EDDY_CALIBRATE(self, gcmd):
        self.probe_speed = gcmd.get_float("PROBE_SPEED", 5., above=0.)
        # Start manual probe
        manual_probe.ManualProbeHelper(self.printer, gcmd,
                                       self.post_manual_probe)

# Tool to gather samples and convert them to probe positions
class EddyGatherSamples:
    def __init__(self, printer, sensor_helper, calibration, z_offset):
        self._printer = printer
        self._sensor_helper = sensor_helper
        self._calibration = calibration
        self._z_offset = z_offset
        # Results storage
        self._samples = []
        self._probe_times = []
        self._probe_results = []
        self._need_stop = False
        # Start samples
        if not self._calibration.is_calibrated():
            raise self._printer.command_error(
                "Must calibrate probe_eddy_current first")
        sensor_helper.add_client(self._add_measurement)
    def _add_measurement(self, msg):
        if self._need_stop:
            del self._samples[:]
            return False
        self._samples.append(msg)
        self._check_samples()
        return True
    def finish(self):
        self._need_stop = True
    def _await_samples(self):
        # Make sure enough samples have been collected
        reactor = self._printer.get_reactor()
        mcu = self._sensor_helper.get_mcu()
        while self._probe_times:
            start_time, end_time, pos_time, toolhead_pos = self._probe_times[0]
            systime = reactor.monotonic()
            est_print_time = mcu.estimated_print_time(systime)
            if est_print_time > end_time + 1.0:
                raise self._printer.command_error(
                    "probe_eddy_current sensor outage")
            reactor.pause(systime + 0.010)
    def _pull_freq(self, start_time, end_time):
        # Find average sensor frequency between time range
        msg_num = discard_msgs = 0
        samp_sum = 0.
        samp_count = 0
        while msg_num < len(self._samples):
            msg = self._samples[msg_num]
            msg_num += 1
            data = msg['data']
            if data[0][0] > end_time:
                break
            if data[-1][0] < start_time:
                discard_msgs = msg_num
                continue
            for time, freq, z in data:
                if time >= start_time and time <= end_time:
                    samp_sum += freq
                    samp_count += 1
        del self._samples[:discard_msgs]
        if not samp_count:
            # No sensor readings - raise error in pull_probed()
            return 0.
        return samp_sum / samp_count
    def _lookup_toolhead_pos(self, pos_time):
        toolhead = self._printer.lookup_object('toolhead')
        kin = toolhead.get_kinematics()
        kin_spos = {s.get_name(): s.mcu_to_commanded_position(
                                      s.get_past_mcu_position(pos_time))
                    for s in kin.get_steppers()}
        return kin.calc_position(kin_spos)
    def _check_samples(self):
        while self._samples and self._probe_times:
            start_time, end_time, pos_time, toolhead_pos = self._probe_times[0]
            if self._samples[-1]['data'][-1][0] < end_time:
                break
            freq = self._pull_freq(start_time, end_time)
            if pos_time is not None:
                toolhead_pos = self._lookup_toolhead_pos(pos_time)
            self._probe_results.append((freq, toolhead_pos))
            self._probe_times.pop(0)
    def pull_probed(self):
        self._await_samples()
        results = []
        for freq, toolhead_pos in self._probe_results:
            if not freq:
                raise self._printer.command_error(
                    "Unable to obtain probe_eddy_current sensor readings")
            sensor_z = self._calibration.freq_to_height(freq)
            if sensor_z <= -OUT_OF_RANGE or sensor_z >= OUT_OF_RANGE:
                raise self._printer.command_error(
                    "probe_eddy_current sensor not in valid range")
            # Callers expect position relative to z_offset, so recalculate
            bed_deviation = toolhead_pos[2] - sensor_z
            toolhead_pos[2] = self._z_offset + bed_deviation
            results.append(toolhead_pos)
        del self._probe_results[:]
        return results
    def note_probe(self, start_time, end_time, toolhead_pos):
        self._probe_times.append((start_time, end_time, None, toolhead_pos))
        self._check_samples()
    def note_probe_and_position(self, start_time, end_time, pos_time):
        self._probe_times.append((start_time, end_time, pos_time, None))
        self._check_samples()

# Helper for implementing PROBE style commands (descend until trigger)
class EddyEndstopWrapper:
    REASON_SENSOR_ERROR = mcu.MCU_trsync.REASON_COMMS_TIMEOUT + 1
    def __init__(self, config, sensor_helper, calibration):
        self._printer = config.get_printer()
        self._sensor_helper = sensor_helper
        self._mcu = sensor_helper.get_mcu()
        self._calibration = calibration
        self._z_offset = config.getfloat('z_offset', minval=0.)
        self._dispatch = mcu.TriggerDispatch(self._mcu)
        self._trigger_time = 0.
        self._gather = None
    # Interface for MCU_endstop
    def get_mcu(self):
        return self._mcu
    def add_stepper(self, stepper):
        self._dispatch.add_stepper(stepper)
    def get_steppers(self):
        return self._dispatch.get_steppers()
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        self._trigger_time = 0.
        trigger_freq = self._calibration.height_to_freq(self._z_offset)
        trigger_completion = self._dispatch.start(print_time)
        self._sensor_helper.setup_home(
            print_time, trigger_freq, self._dispatch.get_oid(),
            mcu.MCU_trsync.REASON_ENDSTOP_HIT, self.REASON_SENSOR_ERROR)
        return trigger_completion
    def home_wait(self, home_end_time):
        self._dispatch.wait_end(home_end_time)
        trigger_time = self._sensor_helper.clear_home()
        res = self._dispatch.stop()
        if res >= mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
            if res == mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
                raise self._printer.command_error(
                    "Communication timeout during homing")
            raise self._printer.command_error("Eddy current sensor error")
        if res != mcu.MCU_trsync.REASON_ENDSTOP_HIT:
            return 0.
        if self._mcu.is_fileoutput():
            return home_end_time
        self._trigger_time = trigger_time
        return trigger_time
    def query_endstop(self, print_time):
        return False # XXX
    # Interface for ProbeEndstopWrapper
    def probing_move(self, pos, speed):
        # Perform probing move
        phoming = self._printer.lookup_object('homing')
        trig_pos = phoming.probing_move(self, pos, speed)
        if not self._trigger_time:
            return trig_pos
        # Extract samples
        start_time = self._trigger_time + 0.050
        end_time = start_time + 0.100
        toolhead = self._printer.lookup_object("toolhead")
        toolhead_pos = toolhead.get_position()
        self._gather.note_probe(start_time, end_time, toolhead_pos)
        return self._gather.pull_probed()[0]
    def multi_probe_begin(self):
        self._gather = EddyGatherSamples(self._printer, self._sensor_helper,
                                         self._calibration, self._z_offset)
    def multi_probe_end(self):
        self._gather.finish()
        self._gather = None
    def probe_prepare(self, hmove):
        pass
    def probe_finish(self, hmove):
        pass
    def get_position_endstop(self):
        return self._z_offset

# Implementing probing with "METHOD=scan"
class EddyScanningProbe:
    def __init__(self, printer, sensor_helper, calibration, z_offset, gcmd):
        self._printer = printer
        self._sensor_helper = sensor_helper
        self._calibration = calibration
        self._z_offset = z_offset
        self._gather = EddyGatherSamples(printer, sensor_helper,
                                         calibration, z_offset)
        self._sample_time_delay = 0.050
        self._sample_time = gcmd.get_float("SAMPLE_TIME", 0.100, above=0.0)
        self._is_rapid = gcmd.get("METHOD", "scan") == 'rapid_scan'
    def _rapid_lookahead_cb(self, printtime):
        start_time = printtime - self._sample_time / 2
        self._gather.note_probe_and_position(
            start_time, start_time + self._sample_time, printtime)
    def run_probe(self, gcmd):
        toolhead = self._printer.lookup_object("toolhead")
        if self._is_rapid:
            toolhead.register_lookahead_callback(self._rapid_lookahead_cb)
            return
        printtime = toolhead.get_last_move_time()
        toolhead.dwell(self._sample_time_delay + self._sample_time)
        start_time = printtime + self._sample_time_delay
        self._gather.note_probe_and_position(
            start_time, start_time + self._sample_time, start_time)
    def pull_probed_results(self):
        if self._is_rapid:
            # Flush lookahead (so all lookahead callbacks are invoked)
            toolhead = self._printer.lookup_object("toolhead")
            toolhead.get_last_move_time()
        results = self._gather.pull_probed()
        # Allow axis_twist_compensation to update results
        for epos in results:
            self._printer.send_event("probe:update_results", epos)
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
        # Probe interface
        self.mcu_probe = EddyEndstopWrapper(config, self.sensor_helper,
                                            self.calibration)
        self.cmd_helper = probe.ProbeCommandHelper(
            config, self, self.mcu_probe.query_endstop)
        self.probe_offsets = probe.ProbeOffsetsHelper(config)
        self.probe_session = probe.ProbeSessionHelper(config, self.mcu_probe)
        self.printer.add_object('probe', self)
    def add_client(self, cb):
        self.sensor_helper.add_client(cb)
    def get_probe_params(self, gcmd=None):
        return self.probe_session.get_probe_params(gcmd)
    def get_offsets(self):
        return self.probe_offsets.get_offsets()
    def get_status(self, eventtime):
        return self.cmd_helper.get_status(eventtime)
    def start_probe_session(self, gcmd):
        method = gcmd.get('METHOD', 'automatic').lower()
        if method in ('scan', 'rapid_scan'):
            z_offset = self.get_offsets()[2]
            return EddyScanningProbe(self.printer, self.sensor_helper,
                                     self.calibration, z_offset, gcmd)
        return self.probe_session.start_probe_session(gcmd)


DRIFT_SAMPLE_COUNT = 9

class EddyDriftCompensation:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.drift_calibration = None
        self.calibration_samples = None
        self.scale = config.getfloat("drift_adjust_factor", 1., minval=0.)
        self.dc_min_temp = config.getfloat("drift_calibration_min_temp", 0.)
        dc = config.getlists(
            "drift_calibration", None, seps=(',', '\n'), parser=float
        )
        self.min_freq = 999999999999.
        if dc is not None:
            for coefs in dc:
                if len(coefs) != 3:
                    raise config.error(
                        "Invalid polynomial in drift calibration"
                    )
            self.drift_calibration = [Polynomial2d(*coefs) for coefs in dc]
            cal = self.drift_calibration
            self._check_calibration(cal, self.dc_min_temp, config.error)
            low_poly = self.drift_calibration[-1]
            self.min_freq = min([low_poly(temp) for temp in range(121)])
            cal_str = "\n".join([repr(p) for p in cal])
            logging.info(
                "%s: loaded temperature drift calibration. Min Temp: %.2f,"
                " Min Freq: %.6f\n%s"
                % (self.name, self.dc_min_temp, self.min_freq, cal_str)
            )
        else:
            logging.info(
                "%s: No drift calibration configured, disabling temperature "
                "compensation"
                % (self.name,)
            )
        self.enabled = has_dc = self.drift_calibration is not None
        cal_temp = config.getfloat('calibration_temp', 0)
        if cal_temp < 1e-6 and has_dc:
            self.enabled = False
            logging.info(
                "%s: No temperature saved for eddy probe calibration, "
                "disabling temperature compensation."
                % (self.name,)
            )

        short_name = self.name.split(maxsplit=1)[-1]
        temp_section = "temperature_probe " + short_name
        self.temp_sensor = None
        if config.has_section(temp_section):
            self.temp_sensor = self.printer.load_object(
                config, temp_section
            )
            self.temp_sensor.register_calibration_helper(self)
        if self.temp_sensor is None and has_dc:
            self.enabled = False
            logging.info(
                "%s: Temperature Sensor [%s] not configured, "
                "disabling temperature compensation"
                % (self.name, temp_section)
            )

        # Register Gcode Command
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command(
            "SET_PROBE_DRIFT_ADJ_FACTOR", "PROBE",
            short_name, self.cmd_SET_DRIFT_FACTOR,
            desc=self.cmd_SET_DRIFT_FACTOR_help
        )

    def is_enabled(self):
        return self.enabled

    def collect_sample(self, kin_pos, tool_zero_z, speeds):
        if self.calibration_samples is None:
            self.calibration_samples = [[] for _ in range(DRIFT_SAMPLE_COUNT)]
        move_times = []
        temps = [0. for _ in range(DRIFT_SAMPLE_COUNT)]
        probe_samples = [[] for _ in range(DRIFT_SAMPLE_COUNT)]
        toolhead = self.printer.lookup_object("toolhead")
        cur_pos = toolhead.get_position()
        lift_speed, probe_speed, _ = speeds

        def _on_bulk_data_recd(msg):
            if move_times:
                idx, start_time, end_time = move_times[0]
                cur_temp = self.get_temperature()
                for sample in msg["data"]:
                    ptime = sample[0]
                    while ptime > end_time:
                        move_times.pop(0)
                        if not move_times:
                            return idx >= DRIFT_SAMPLE_COUNT - 1
                        idx, start_time, end_time = move_times[0]
                    if ptime < start_time:
                        continue
                    temps[idx] = cur_temp
                    probe_samples[idx].append(sample)
            return True
        self.printer.lookup_object(self.name).add_client(_on_bulk_data_recd)
        for i in range(DRIFT_SAMPLE_COUNT):
            if i == 0:
                # Move down to first sample location
                cur_pos[2] = tool_zero_z + .05
            else:
                # Sample each .5mm in z
                cur_pos[2] += 1.
                toolhead.manual_move(cur_pos, lift_speed)
                cur_pos[2] -= .5
            toolhead.manual_move(cur_pos, probe_speed)
            start = toolhead.get_last_move_time() + .05
            end = start + .1
            move_times.append((i, start, end))
            toolhead.dwell(.2)
        toolhead.wait_moves()
        # Wait for sample collection to finish
        reactor = self.printer.get_reactor()
        evttime = reactor.monotonic()
        while move_times:
            evttime = reactor.pause(evttime + .1)
        sample_temp = sum(temps) / len(temps)
        for i, data in enumerate(probe_samples):
            freqs = [d[1] for d in data]
            zvals = [d[2] for d in data]
            avg_freq = sum(freqs) / len(freqs)
            avg_z = sum(zvals) / len(zvals)
            kin_z = i * .5 + .05 + kin_pos[2]
            logging.info(
                "Probe Values at Temp %.2fC, Z %.4fmm: Avg Freq = %.6f, "
                "Avg Measured Z = %.6f"
                % (sample_temp, kin_z, avg_freq, avg_z)
            )
            self.calibration_samples[i].append((sample_temp, avg_freq))
        return sample_temp

    def start_calibration(self):
        self.enabled = False
        self.calibration_samples = [[] for _ in range(DRIFT_SAMPLE_COUNT)]

    def finish_calibration(self, success):
        cal_samples = self.calibration_samples
        self.calibration_samples = None
        if not success:
            return
        gcode = self.printer.lookup_object("gcode")
        if len(cal_samples) < 3:
            raise gcode.error(
                "calbration error, not enough samples"
            )
        min_temp, _ = cal_samples[0][0]
        polynomials = []
        for i, coords in enumerate(cal_samples):
            height = .05 + i * .5
            poly = Polynomial2d.fit(coords)
            polynomials.append(poly)
            logging.info("Polynomial at Z=%.2f: %s" % (height, repr(poly)))
        self._check_calibration(polynomials, min_temp)
        coef_cfg = "\n" + "\n".join([str(p) for p in polynomials])
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.name, "drift_calibration", coef_cfg)
        configfile.set(self.name, "drift_calibration_min_temp", min_temp)
        gcode.respond_info(
            "%s: generated %d 2D polynomials\n"
            "The SAVE_CONFIG command will update the printer config "
            "file and restart the printer."
            % (self.name, len(polynomials))
        )
        try:
            # Dump collected data to temporary file
            import json
            ctime = int(self.printer.get_reactor().monotonic())
            tmpfname = "/tmp/eddy-probe-drift-%d.json" % (ctime)
            out = {
                "polynomial_coefs": [c.get_coefs() for c in polynomials],
                "legend": ["temperature", "frequency"],
                "data": cal_samples,
                "start_z": .05,
                "sample_z_dist": .5
            }
            with open(tmpfname, "w") as f:
                f.write(json.dumps(out))
        except Exception:
            logging.exception("Failed to write %s" % (tmpfname))

    def _check_calibration(self, calibration, start_temp, error=None):
        error = error or self.printer.command_error
        start = int(start_temp)
        for temp in range(start, 121, 1):
            last_freq = calibration[0](temp)
            for i, poly in enumerate(calibration[1:]):
                next_freq = poly(temp)
                if next_freq >= last_freq:
                    # invalid polynomial
                    raise error(
                        "%s: invalid calibration detected, curve at index "
                        "%d overlaps previous curve at temp %dC."
                        % (self.name, i + 1, temp)
                    )
                last_freq = next_freq

    def adjust_freq(self, freq, dest_temp):
        # Adjusts frequency from current temperature toward
        # destination temperature
        if not self.enabled or freq < self.min_freq:
            return freq
        cur_temp = self.temp_sensor.get_temp()[0]
        return self._calc_freq(freq, cur_temp, dest_temp)

    def lookup_freq(self, freq, origin_temp):
        # Given a frequency and its orignal sampled temp, find the
        # offset frequency based on the current temp
        if not self.enabled or freq < self.min_freq:
            return freq
        cur_temp = self.temp_sensor.get_temp()[0]
        return self._calc_freq(freq, origin_temp, cur_temp)

    def _calc_freq(self, freq, origin_temp, dest_temp):
        high_freq = low_freq = None
        dc = self.drift_calibration
        for pos, poly in enumerate(dc):
            high_freq = low_freq
            low_freq = poly(origin_temp)
            if freq >= low_freq:
                if high_freq is None:
                    # Freqency above max calibration value
                    return freq + ((dc[0](dest_temp) - freq) * self.scale)
                t = min(1., max(0., (freq - low_freq) / (high_freq - low_freq)))
                low_tgt_freq = poly(dest_temp)
                high_tgt_freq = dc[pos-1](dest_temp)
                err = ((1 - t) * low_tgt_freq + t * high_tgt_freq) - freq
                return freq + err * self.scale
        # Frequency below minimum, no correction
        return freq

    def get_temperature(self):
        if self.temp_sensor is not None:
            return self.temp_sensor.get_temp()[0]
        return 0.

    cmd_SET_DRIFT_FACTOR_help = (
        "Set adjustment factor applied to drift correction"
    )
    def cmd_SET_DRIFT_FACTOR(self, gcmd):
        self.scale = gcmd.get_float("FACTOR", minval=0.)

def load_config_prefix(config):
    return PrinterEddyProbe(config)
