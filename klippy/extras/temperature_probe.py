# Probe temperature sensor and drift calibration
#
# Copyright (C) 2024 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import manual_probe

KELVIN_TO_CELSIUS = -273.15

######################################################################
# Polynomial Helper Classes and Functions
######################################################################

def calc_determinant(matrix):
    m = matrix
    aei = m[0][0] * m[1][1] * m[2][2]
    bfg = m[1][0] * m[2][1] * m[0][2]
    cdh = m[2][0] * m[0][1] * m[1][2]
    ceg = m[2][0] * m[1][1] * m[0][2]
    bdi = m[1][0] * m[0][1] * m[2][2]
    afh = m[0][0] * m[2][1] * m[1][2]
    return aei + bfg + cdh - ceg - bdi - afh

class Polynomial2d:
    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c

    def __call__(self, xval):
        return self.c * xval * xval + self.b * xval + self.a

    def get_coefs(self):
        return (self.a, self.b, self.c)

    def __str__(self):
        return "%f, %f, %f" % (self.a, self.b, self.c)

    def __repr__(self):
        parts = ["y(x) ="]
        deg = 2
        for i, coef in enumerate((self.c, self.b, self.a)):
            if round(coef, 8) == int(coef):
                coef = int(coef)
            if abs(coef) < 1e-10:
                continue
            cur_deg = deg - i
            x_str = "x^%d" % (cur_deg,) if cur_deg > 1 else "x" * cur_deg
            if len(parts) == 1:
                parts.append("%f%s" % (coef, x_str))
            else:
                sym = "-" if coef < 0 else "+"
                parts.append("%s %f%s" % (sym, abs(coef), x_str))
        return " ".join(parts)

    @classmethod
    def fit(cls, coords):
        xlist = [c[0] for c in coords]
        ylist = [c[1] for c in coords]
        count = len(coords)
        sum_x = sum(xlist)
        sum_y = sum(ylist)
        sum_x2 = sum([x**2 for x in xlist])
        sum_x3 = sum([x**3 for x in xlist])
        sum_x4 = sum([x**4 for x in xlist])
        sum_xy = sum([x * y for x, y in coords])
        sum_x2y = sum([y*x**2 for x, y in coords])
        vector_b = [sum_y, sum_xy, sum_x2y]
        m = [
            [count, sum_x, sum_x2],
            [sum_x, sum_x2, sum_x3],
            [sum_x2, sum_x3, sum_x4]
        ]
        m0 = [vector_b, m[1], m[2]]
        m1 = [m[0], vector_b, m[2]]
        m2 = [m[0], m[1], vector_b]
        det_m = calc_determinant(m)
        a0 = calc_determinant(m0) / det_m
        a1 = calc_determinant(m1) / det_m
        a2 = calc_determinant(m2) / det_m
        return cls(a0, a1, a2)

class TemperatureProbe:
    def __init__(self, config):
        self.name = config.get_name()
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object("gcode")
        self.speed = config.getfloat("speed", None, above=0.)
        self.horizontal_move_z = config.getfloat(
            "horizontal_move_z", 2., above=0.
        )
        self.resting_z = config.getfloat("resting_z", .4, above=0.)
        self.cal_pos = config.getfloatlist(
            "calibration_position", None, count=3
        )
        self.cal_bed_temp = config.getfloat(
            "calibration_bed_temp", None, above=50.
        )
        self.cal_extruder_temp = config.getfloat(
            "calibration_extruder_temp", None, above=50.
        )
        self.cal_extruder_z = config.getfloat(
            "extruder_heating_z", 50., above=0.
        )
        # Setup temperature sensor
        smooth_time = config.getfloat("smooth_time", 2., above=0.)
        self.inv_smooth_time = 1. / smooth_time
        self.min_temp = config.getfloat(
            "min_temp", KELVIN_TO_CELSIUS, minval=KELVIN_TO_CELSIUS
        )
        self.max_temp = config.getfloat(
            "max_temp", 99999999.9, above=self.min_temp
        )
        pheaters = self.printer.load_object(config, "heaters")
        self.sensor = pheaters.setup_sensor(config)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self._temp_callback)
        pheaters.register_sensor(config, self)
        self.last_temp_read_time = 0.
        self.last_measurement = (0., 99999999., 0.,)
        # Calibration State
        self.cal_helper = None
        self.next_auto_temp = 99999999.
        self.target_temp = 0
        self.expected_count = 0
        self.sample_count = 0
        self.in_calibration = False
        self.step = 2.
        self.last_zero_pos = None
        self.total_expansion = 0
        self.start_pos = []

        # Register GCode Commands
        pname = self.name.split(maxsplit=1)[-1]
        self.gcode.register_mux_command(
            "TEMPERATURE_PROBE_CALIBRATE", "PROBE", pname,
            self.cmd_TEMPERATURE_PROBE_CALIBRATE,
            desc=self.cmd_TEMPERATURE_PROBE_CALIBRATE_help
        )

        self.gcode.register_mux_command(
            "TEMPERATURE_PROBE_ENABLE", "PROBE", pname,
            self.cmd_TEMPERATURE_PROBE_ENABLE,
            desc=self.cmd_TEMPERATURE_PROBE_ENABLE_help
        )

        # Register Drift Compensation Helper with probe
        full_probe_name = "probe_eddy_current %s" % (pname,)
        if config.has_section(full_probe_name):
            pprobe = self.printer.load_object(config, full_probe_name)
            self.cal_helper = EddyDriftCompensation(config, self)
            pprobe.register_drift_compensation(self.cal_helper)
            logging.info(
                "%s: registered drift compensation with probe [%s]"
                % (self.name, full_probe_name)
            )
        else:
            logging.info(
                "%s: No probe named %s configured, thermal drift compensation "
                "disabled." % (self.name, pname)
            )

    def _temp_callback(self, read_time, temp):
        smoothed_temp, measured_min, measured_max = self.last_measurement
        time_diff = read_time - self.last_temp_read_time
        self.last_temp_read_time = read_time
        temp_diff = temp - smoothed_temp
        adj_time = min(time_diff * self.inv_smooth_time, 1.)
        smoothed_temp += temp_diff * adj_time
        measured_min = min(measured_min, smoothed_temp)
        measured_max = max(measured_max, smoothed_temp)
        self.last_measurement = (smoothed_temp, measured_min, measured_max)
        if self.in_calibration and smoothed_temp >= self.next_auto_temp:
            self.printer.get_reactor().register_async_callback(
                self._check_kick_next
            )

    def _check_kick_next(self, eventtime):
        smoothed_temp = self.last_measurement[0]
        if self.in_calibration and smoothed_temp >= self.next_auto_temp:
            self.next_auto_temp = 99999999.
            self.gcode.run_script("TEMPERATURE_PROBE_NEXT")

    def get_temp(self, eventtime=None):
        return self.last_measurement[0], self.target_temp

    def _collect_sample(self, kin_pos, tool_zero_z):
        probe = self._get_probe()
        x_offset, y_offset, _ = probe.get_offsets()
        speeds = self._get_speeds()
        lift_speed, _, move_speed = speeds
        toolhead = self.printer.lookup_object("toolhead")
        cur_pos = toolhead.get_position()
        # Move to probe to sample collection position
        cur_pos[2] += self.horizontal_move_z
        toolhead.manual_move(cur_pos, lift_speed)
        cur_pos[0] -= x_offset
        cur_pos[1] -= y_offset
        toolhead.manual_move(cur_pos, move_speed)
        return self.cal_helper.collect_sample(kin_pos, tool_zero_z, speeds)

    def _prepare_next_sample(self, last_temp, tool_zero_z):
        # Register our own abort command now that the manual
        # probe has finished and unregistered
        self.gcode.register_command(
            "ABORT", self.cmd_TEMPERATURE_PROBE_ABORT,
            desc=self.cmd_TEMPERATURE_PROBE_ABORT_help
        )
        probe_speed = self._get_speeds()[1]
        # Move tool down to the resting position
        toolhead = self.printer.lookup_object("toolhead")
        cur_pos = toolhead.get_position()
        cur_pos[2] = tool_zero_z + self.resting_z
        toolhead.manual_move(cur_pos, probe_speed)
        cnt, exp_cnt = self.sample_count, self.expected_count
        self.next_auto_temp = last_temp + self.step
        self.gcode.respond_info(
            "%s: collected sample %d/%d at temp %.2fC, next sample scheduled "
            "at temp %.2fC"
            % (self.name, cnt, exp_cnt, last_temp, self.next_auto_temp)
        )

    def _manual_probe_finalize(self, kin_pos):
        if kin_pos is None:
            # Calibration aborted
            self._finalize_drift_cal(False)
            return
        if self.last_zero_pos is not None:
            z_diff = self.last_zero_pos[2] - kin_pos[2]
            self.total_expansion += z_diff
            logging.info(
                "Estimated Total Thermal Expansion: %.6f"
                % (self.total_expansion,)
            )
        self.last_zero_pos = kin_pos
        toolhead = self.printer.lookup_object("toolhead")
        tool_zero_z = toolhead.get_position()[2]
        try:
            last_temp = self._collect_sample(kin_pos, tool_zero_z)
        except Exception:
            self._finalize_drift_cal(False)
            raise
        self.sample_count += 1
        if last_temp >= self.target_temp:
            # Calibration Done
            self._finalize_drift_cal(True)
        else:
            try:
                self._prepare_next_sample(last_temp, tool_zero_z)
                if self.sample_count == 1:
                    self._set_bed_temp(self.cal_bed_temp)
            except Exception:
                self._finalize_drift_cal(False)
                raise

    def _finalize_drift_cal(self, success, msg=None):
        self.next_auto_temp = 99999999.
        self.target_temp = 0
        self.expected_count = 0
        self.sample_count = 0
        self.step = 2.
        self.in_calibration = False
        self.last_zero_pos = None
        self.total_expansion = 0
        self.start_pos = []
        # Unregister Temporary Commands
        self.gcode.register_command("ABORT", None)
        self.gcode.register_command("TEMPERATURE_PROBE_NEXT", None)
        self.gcode.register_command("TEMPERATURE_PROBE_COMPLETE", None)
        # Turn off heaters
        self._set_extruder_temp(0)
        self._set_bed_temp(0)
        try:
            self.cal_helper.finish_calibration(success)
        except self.gcode.error as e:
            success = False
            msg = str(e)
        if not success:
            msg = msg or "%s: calibration aborted" % (self.name,)
            self.gcode.respond_info(msg)

    def _get_probe(self):
        probe = self.printer.lookup_object("probe")
        if probe is None:
            raise self.gcode.error("No probe configured")
        return probe

    def _set_extruder_temp(self, temp, wait=False):
        if self.cal_extruder_temp is None:
            # Extruder temperature not configured
            return
        toolhead = self.printer.lookup_object("toolhead")
        extr_name = toolhead.get_extruder().get_name()
        self.gcode.run_script_from_command(
            "SET_HEATER_TEMPERATURE HEATER=%s TARGET=%f"
            % (extr_name, temp)
        )
        if wait:
            self.gcode.run_script_from_command(
                "TEMPERATURE_WAIT SENSOR=%s MINIMUM=%f"
                % (extr_name, temp)
            )
    def _set_bed_temp(self, temp):
        if self.cal_bed_temp is None:
            # Bed temperature not configured
            return
        self.gcode.run_script_from_command(
            "SET_HEATER_TEMPERATURE HEATER=heater_bed TARGET=%f"
            % (temp,)
        )

    def _check_homed(self):
        toolhead = self.printer.lookup_object("toolhead")
        reactor = self.printer.get_reactor()
        status = toolhead.get_status(reactor.monotonic())
        h_axes = status["homed_axes"]
        for axis in "xyz":
            if axis not in h_axes:
                raise self.gcode.error(
                    "Printer must be homed before calibration"
                )

    def _move_to_start(self):
        toolhead = self.printer.lookup_object("toolhead")
        cur_pos = toolhead.get_position()
        move_speed = self._get_speeds()[2]
        if self.cal_pos is not None:
            if self.cal_extruder_temp is not None:
                # Move to extruder heating z position
                cur_pos[2] = self.cal_extruder_z
                toolhead.manual_move(cur_pos, move_speed)
            toolhead.manual_move(self.cal_pos[:2], move_speed)
            self._set_extruder_temp(self.cal_extruder_temp, True)
            toolhead.manual_move(self.cal_pos, move_speed)
        elif self.cal_extruder_temp is not None:
            cur_pos[2] = self.cal_extruder_z
            toolhead.manual_move(cur_pos, move_speed)
            self._set_extruder_temp(self.cal_extruder_temp, True)

    def _get_speeds(self):
        pparams = self._get_probe().get_probe_params()
        probe_speed = pparams["probe_speed"]
        lift_speed = pparams["lift_speed"]
        move_speed = self.speed or max(probe_speed, lift_speed)
        return lift_speed, probe_speed, move_speed

    cmd_TEMPERATURE_PROBE_CALIBRATE_help = (
        "Calibrate probe temperature drift compensation"
    )
    def cmd_TEMPERATURE_PROBE_CALIBRATE(self, gcmd):
        if self.cal_helper is None:
            raise gcmd.error(
                "No calibration helper registered for [%s]"
                % (self.name,)
            )
        self._check_homed()
        probe = self._get_probe()
        probe_name = probe.get_status(None)["name"]
        short_name = probe_name.split(maxsplit=1)[-1]
        if short_name != self.name.split(maxsplit=1)[-1]:
            raise self.gcode.error(
                "[%s] not linked to registered probe [%s]."
                % (self.name, probe_name)
            )
        manual_probe.verify_no_manual_probe(self.printer)
        if self.in_calibration:
            raise gcmd.error(
                "Already in probe drift calibration. Use "
                "TEMPERATURE_PROBE_COMPLETE or ABORT to exit."
            )
        cur_temp = self.last_measurement[0]
        target_temp = gcmd.get_float("TARGET", above=cur_temp)
        step = gcmd.get_float("STEP", 2., minval=1.0)
        expected_count = int(
            (target_temp - cur_temp) / step + .5
        )
        if expected_count < 3:
            raise gcmd.error(
                "Invalid STEP and/or TARGET parameters resulted "
                "in too few expected samples: %d"
                % (expected_count,)
            )
        try:
            self.gcode.register_command(
                "TEMPERATURE_PROBE_NEXT", self.cmd_TEMPERATURE_PROBE_NEXT,
                desc=self.cmd_TEMPERATURE_PROBE_NEXT_help
            )
            self.gcode.register_command(
                "TEMPERATURE_PROBE_COMPLETE",
                self.cmd_TEMPERATURE_PROBE_COMPLETE,
                desc=self.cmd_TEMPERATURE_PROBE_NEXT_help
            )
        except self.printer.config_error:
            raise gcmd.error(
                "Auxiliary Probe Drift Commands already registered. Use "
                "TEMPERATURE_PROBE_COMPLETE or ABORT to exit."
            )
        self.in_calibration = True
        self.cal_helper.start_calibration()
        self.target_temp = target_temp
        self.step = step
        self.sample_count = 0
        self.expected_count = expected_count
        # If configured move to heating position and turn on extruder
        try:
            self._move_to_start()
        except self.printer.command_error:
            self._finalize_drift_cal(False, "Error during initial move")
            raise
        # Caputure start position and begin initial probe
        toolhead = self.printer.lookup_object("toolhead")
        self.start_pos = toolhead.get_position()[:2]
        manual_probe.ManualProbeHelper(
            self.printer, gcmd, self._manual_probe_finalize
        )

    cmd_TEMPERATURE_PROBE_NEXT_help = "Sample next probe drift temperature"
    def cmd_TEMPERATURE_PROBE_NEXT(self, gcmd):
        manual_probe.verify_no_manual_probe(self.printer)
        self.next_auto_temp = 99999999.
        toolhead = self.printer.lookup_object("toolhead")
        # Lift and Move to nozzle back to start position
        curpos = toolhead.get_position()
        start_z = curpos[2]
        lift_speed, probe_speed, move_speed = self._get_speeds()
        # Move nozzle to the manual probing position
        curpos[2] += self.horizontal_move_z
        toolhead.manual_move(curpos, lift_speed)
        curpos[0] = self.start_pos[0]
        curpos[1] = self.start_pos[1]
        toolhead.manual_move(curpos, move_speed)
        curpos[2] = start_z
        toolhead.manual_move(curpos, probe_speed)
        self.gcode.register_command("ABORT", None)
        manual_probe.ManualProbeHelper(
            self.printer, gcmd, self._manual_probe_finalize
        )

    cmd_TEMPERATURE_PROBE_COMPLETE_help = "Finish Probe Drift Calibration"
    def cmd_TEMPERATURE_PROBE_COMPLETE(self, gcmd):
        manual_probe.verify_no_manual_probe(self.printer)
        self._finalize_drift_cal(self.sample_count >= 3)

    cmd_TEMPERATURE_PROBE_ABORT_help = "Abort Probe Drift Calibration"
    def cmd_TEMPERATURE_PROBE_ABORT(self, gcmd):
        self._finalize_drift_cal(False)

    cmd_TEMPERATURE_PROBE_ENABLE_help = (
        "Set adjustment factor applied to drift correction"
    )
    def cmd_TEMPERATURE_PROBE_ENABLE(self, gcmd):
        if self.cal_helper is not None:
            self.cal_helper.set_enabled(gcmd)

    def is_in_calibration(self):
        return self.in_calibration

    def get_status(self, eventtime=None):
        smoothed_temp, measured_min, measured_max = self.last_measurement
        dcomp_enabled = False
        if self.cal_helper is not None:
            dcomp_enabled = self.cal_helper.is_enabled()
        return {
            "temperature": smoothed_temp,
            "measured_min_temp": round(measured_min, 2),
            "measured_max_temp": round(measured_max, 2),
            "in_calibration": self.in_calibration,
            "estimated_expansion": self.total_expansion,
            "compensation_enabled": dcomp_enabled
        }

    def stats(self, eventtime):
        return False, '%s: temp=%.1f' % (self.name, self.last_measurement[0])


#####################################################################
#
#  Eddy Current Probe Drift Compensation Helper
#
#####################################################################

DRIFT_SAMPLE_COUNT = 9

class EddyDriftCompensation:
    def __init__(self, config, sensor):
        self.printer = config.get_printer()
        self.temp_sensor = sensor
        self.name = config.get_name()
        self.cal_temp = config.getfloat("calibration_temp", 0.)
        self.drift_calibration = None
        self.calibration_samples = None
        self.max_valid_temp = config.getfloat("max_validation_temp", 60.)
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
            start_temp, end_temp = self.dc_min_temp, self.max_valid_temp
            self._check_calibration(cal, start_temp, end_temp, config.error)
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
                "drift compensation"
                % (self.name,)
            )
        self.enabled = has_dc = self.drift_calibration is not None
        if self.cal_temp < 1e-6 and has_dc:
            self.enabled = False
            logging.info(
                "%s: No temperature saved for eddy probe calibration, "
                "disabling temperature drift compensation."
                % (self.name,)
            )

    def is_enabled(self):
        return self.enabled

    def set_enabled(self, gcmd):
        enabled = gcmd.get_int("ENABLE")
        if enabled:
            if self.drift_calibration is None:
                raise gcmd.error(
                    "No drift calibration configured, cannot enable "
                    "temperature drift compensation"
                )
            if self.cal_temp < 1e-6:
                raise gcmd.error(
                    "Z Calibration temperature not configured, cannot enable "
                    "temperature drift compensation"
                )
        self.enabled = enabled

    def note_z_calibration_start(self):
        self.cal_temp = self.get_temperature()

    def note_z_calibration_finish(self):
        self.cal_temp = (self.cal_temp + self.get_temperature()) / 2.0
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.name, "calibration_temp", "%.6f " % (self.cal_temp))
        gcode = self.printer.lookup_object("gcode")
        gcode.respond_info(
            "%s: Z Calibration Temperature set to %.2f. "
            "The SAVE_CONFIG command will update the printer config "
            "file and restart the printer."
            % (self.name, self.cal_temp)
        )

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
        sect_name = "probe_eddy_current " + self.name.split(maxsplit=1)[-1]
        self.printer.lookup_object(sect_name).add_client(_on_bulk_data_recd)
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
        max_temp, _ = cal_samples[-1][0]
        polynomials = []
        for i, coords in enumerate(cal_samples):
            height = .05 + i * .5
            poly = Polynomial2d.fit(coords)
            polynomials.append(poly)
            logging.info("Polynomial at Z=%.2f: %s" % (height, repr(poly)))
        end_vld_temp = max(self.max_valid_temp, max_temp)
        self._check_calibration(polynomials, min_temp, end_vld_temp)
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

    def _check_calibration(self, calibration, start_temp, end_temp, error=None):
        error = error or self.printer.command_error
        start = int(start_temp)
        end = int(end_temp) + 1
        for temp in range(start, end, 1):
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

    def adjust_freq(self, freq, origin_temp=None):
        # Adjusts frequency from current temperature toward
        # destination temperature
        if not self.enabled or freq < self.min_freq:
            return freq
        if origin_temp is None:
            origin_temp = self.get_temperature()
        return self._calc_freq(freq, origin_temp, self.cal_temp)

    def unadjust_freq(self, freq, dest_temp=None):
        # Given a frequency and its orignal sampled temp, find the
        # offset frequency based on the current temp
        if not self.enabled or freq < self.min_freq:
            return freq
        if dest_temp is None:
            dest_temp = self.get_temperature()
        return self._calc_freq(freq, self.cal_temp, dest_temp)

    def _calc_freq(self, freq, origin_temp, dest_temp):
        high_freq = low_freq = None
        dc = self.drift_calibration
        for pos, poly in enumerate(dc):
            high_freq = low_freq
            low_freq = poly(origin_temp)
            if freq >= low_freq:
                if high_freq is None:
                    # Freqency above max calibration value
                    err = poly(dest_temp) - low_freq
                    return freq + err
                t = min(1., max(0., (freq - low_freq) / (high_freq - low_freq)))
                low_tgt_freq = poly(dest_temp)
                high_tgt_freq = dc[pos-1](dest_temp)
                return (1 - t) * low_tgt_freq + t * high_tgt_freq
        # Frequency below minimum, no correction
        return freq

    def get_temperature(self):
        return self.temp_sensor.get_temp()[0]


def load_config_prefix(config):
    return TemperatureProbe(config)
