# A utility class to test resonances of the printer
#
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math, os, time
from . import shaper_calibrate

def _parse_probe_points(config):
    points = config.get('probe_points').split('\n')
    try:
        points = [line.split(',', 2) for line in points if line.strip()]
        return [[float(coord.strip()) for coord in p] for p in points]
    except:
        raise config.error("Unable to parse probe_points in %s" % (
            config.get_name()))

class VibrationPulseTest:
    def __init__(self, config):
        printer = config.get_printer()
        self.gcode = printer.lookup_object('gcode')
        self.min_freq = config.getfloat('min_freq', 5., minval=1.)
        self.max_freq = config.getfloat('max_freq', 120.,
                                        minval=self.min_freq, maxval=200.)
        self.accel_per_hz = config.getfloat('accel_per_hz', 75.0, above=0.)
        self.hz_per_sec = config.getfloat('hz_per_sec', 1.,
                                          minval=0.1, maxval=2.)

        self.probe_points = _parse_probe_points(config)
    def get_supported_axes(self):
        return ['x', 'y']
    def get_start_test_points(self):
        return self.probe_points
    def prepare_test(self, toolhead, gcmd):
        self.freq_start = gcmd.get_float("FREQ_START", self.min_freq, minval=1.)
        self.freq_end = gcmd.get_float("FREQ_END", self.max_freq,
                                       minval=self.freq_start, maxval=200.)
        self.hz_per_sec = gcmd.get_float("HZ_PER_SEC", self.hz_per_sec,
                                         above=0., maxval=2.)
        # Attempt to adjust maximum acceleration and acceleration to
        # deceleration based on the maximum test frequency.
        max_accel = self.freq_end * self.accel_per_hz
        toolhead.cmd_SET_VELOCITY_LIMIT(self.gcode.create_gcode_command(
            "SET_VELOCITY_LIMIT", "SET_VELOCITY_LIMIT",
            {"ACCEL": max_accel, "ACCEL_TO_DECEL": max_accel}))
    def run_test(self, toolhead, axis, gcmd):
        X, Y, Z, E = toolhead.get_position()
        if axis not in self.get_supported_axes():
            raise gcmd.error("Test axis '%s' is not supported", axis)
        vib_dir = (1, 0) if axis == 'x' else (0., 1.)
        sign = 1.
        freq = self.freq_start
        gcmd.respond_info("Testing frequency %.0f Hz" % (freq,))
        _, max_accel = toolhead.get_max_velocity()
        while freq <= self.freq_end + 0.000001:
            t_seg = .25 / freq
            accel = min(self.accel_per_hz * freq, max_accel)
            V = accel * t_seg
            toolhead.cmd_M204(self.gcode.create_gcode_command(
                "M204", "M204", {"S": accel}))
            L = .5 * accel * t_seg**2
            nX = X + sign * vib_dir[0] * L
            nY = Y + sign * vib_dir[1] * L
            toolhead.move([nX, nY, Z, E], V)
            toolhead.move([X, Y, Z, E], V)
            sign = -sign
            old_freq = freq
            freq += 2. * t_seg * self.hz_per_sec
            if math.floor(freq) > math.floor(old_freq):
                gcmd.respond_info("Testing frequency %.0f Hz" % (freq,))

class ResonanceTester:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.move_speed = config.getfloat('move_speed', 50., above=0.)
        self.test = VibrationPulseTest(config)
        if not config.get('accel_chip_x', None):
            self.accel_chip_names = [('xy', config.get('accel_chip').strip())]
        else:
            self.accel_chip_names = [
                ('x', config.get('accel_chip_x').strip()),
                ('y', config.get('accel_chip_y').strip())]
            if self.accel_chip_names[0][1] == self.accel_chip_names[1][1]:
                self.accel_chip_names = [('xy', self.accel_chip_names[0][1])]
        self.max_smoothing = config.getfloat('max_smoothing', None, minval=0.05)

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("MEASURE_AXES_NOISE",
                                    self.cmd_MEASURE_AXES_NOISE)
        self.gcode.register_command("TEST_RESONANCES",
                                    self.cmd_TEST_RESONANCES)
        self.gcode.register_command("SHAPER_CALIBRATE",
                                    self.cmd_SHAPER_CALIBRATE)
        self.printer.register_event_handler("klippy:connect", self.connect)

    def connect(self):
        self.accel_chips = [
                (axis, self.printer.lookup_object(chip_name))
                for axis, chip_name in self.accel_chip_names]

    def cmd_TEST_RESONANCES(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        # Parse parameters
        self.test.prepare_test(toolhead, gcmd)
        if len(self.test.get_supported_axes()) > 1:
            axis = gcmd.get("AXIS").lower()
        else:
            axis = gcmd.get("AXIS", self.test.get_supported_axes()[0]).lower()
        if axis not in self.test.get_supported_axes():
            raise gcmd.error("Unsupported axis '%s'" % (axis,))

        outputs = gcmd.get("OUTPUT", "resonances").lower().split(',')
        for output in outputs:
            if output not in ['resonances', 'raw_data']:
                raise gcmd.error("Unsupported output '%s', only 'resonances'"
                                 " and 'raw_data' are supported" % (output,))
        if not outputs:
            raise gcmd.error("No output specified, at least one of 'resonances'"
                             " or 'raw_data' must be set in OUTPUT parameter")
        name_suffix = gcmd.get("NAME", time.strftime("%Y%m%d_%H%M%S"))
        if not self.is_valid_name_suffix(name_suffix):
            raise gcmd.error("Invalid NAME parameter")
        csv_output = 'resonances' in outputs
        raw_output = 'raw_data' in outputs

        # Setup calculation of resonances
        if csv_output:
            helper = shaper_calibrate.ShaperCalibrate(self.printer)

        currentPos = toolhead.get_position()
        Z = currentPos[2]
        E = currentPos[3]

        calibration_points = self.test.get_start_test_points()
        data = None
        for point in calibration_points:
            toolhead.manual_move(point, self.move_speed)
            if len(calibration_points) > 1:
                gcmd.respond_info(
                        "Probing point (%.3f, %.3f, %.3f)" % tuple(point))
            toolhead.wait_moves()
            toolhead.dwell(0.500)
            gcmd.respond_info("Testing axis %s" % axis.upper())

            for chip_axis, chip in self.accel_chips:
                if axis in chip_axis or chip_axis in axis:
                    chip.start_measurements()
            # Generate moves
            self.test.run_test(toolhead, axis, gcmd)
            raw_values = []
            for chip_axis, chip in self.accel_chips:
                if axis in chip_axis or chip_axis in axis:
                    results = chip.finish_measurements()
                    if raw_output:
                        raw_name = self.get_filename(
                                'raw_data', name_suffix, axis,
                                point if len(calibration_points) > 1 else None)
                        results.write_to_file(raw_name)
                        gcmd.respond_info(
                                "Writing raw accelerometer data to %s file" % (
                                    raw_name,))
                    raw_values.append((chip_axis, results))
            if not csv_output:
                continue
            for chip_axis, chip_values in raw_values:
                gcmd.respond_info("%s-axis accelerometer stats: %s" % (
                    chip_axis, chip_values.get_stats(),))
                if not chip_values:
                    raise gcmd.error(
                            "%s-axis accelerometer measured no data" % (
                                chip_axis,))
                new_data = helper.process_accelerometer_data(chip_values)
                data = data.join(new_data) if data else new_data
        if csv_output:
            csv_name = self.save_calibration_data('resonances', name_suffix,
                                                  helper, axis, data)
            gcmd.respond_info(
                    "Resonances data written to %s file" % (csv_name,))

    def cmd_SHAPER_CALIBRATE(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        # Parse parameters
        self.test.prepare_test(toolhead, gcmd)
        axis = gcmd.get("AXIS", None)
        if not axis:
            calibrate_axes = self.test.get_supported_axes()
        elif axis.lower() not in self.test.get_supported_axes():
            raise gcmd.error("Unsupported axis '%s'" % (axis,))
        else:
            calibrate_axes = [axis.lower()]

        max_smoothing = gcmd.get_float(
                "MAX_SMOOTHING", self.max_smoothing, minval=0.05)

        name_suffix = gcmd.get("NAME", time.strftime("%Y%m%d_%H%M%S"))
        if not self.is_valid_name_suffix(name_suffix):
            raise gcmd.error("Invalid NAME parameter")

        # Setup shaper calibration
        helper = shaper_calibrate.ShaperCalibrate(self.printer)

        input_shaper = self.printer.lookup_object('input_shaper', None)
        if input_shaper is not None:
            input_shaper.disable_shaping()
            gcmd.respond_info("Disabled [input_shaper] for calibration")

        currentPos = toolhead.get_position()
        Z = currentPos[2]
        E = currentPos[3]
        calibration_data = {axis: None for axis in calibrate_axes}

        calibration_points = self.test.get_start_test_points()
        for point in calibration_points:
            toolhead.manual_move(point, self.move_speed)
            if len(calibration_points) > 1:
                gcmd.respond_info(
                        "Probing point (%.3f, %.3f, %.3f)" % tuple(point))
            for axis in calibrate_axes:
                toolhead.wait_moves()
                toolhead.dwell(0.500)
                gcmd.respond_info("Testing axis %s" % axis.upper())

                for chip_axis, chip in self.accel_chips:
                    if axis in chip_axis or chip_axis in axis:
                        chip.start_measurements()
                # Generate moves
                self.test.run_test(toolhead, axis, gcmd)
                raw_values = [(chip_axis, chip.finish_measurements())
                              for chip_axis, chip in self.accel_chips
                              if axis in chip_axis or chip_axis in axis]
                for chip_axis, chip_values in raw_values:
                    gcmd.respond_info("%s-axis accelerometer stats: %s" % (
                        chip_axis, chip_values.get_stats(),))
                    if not chip_values:
                        raise gcmd.error(
                                "%s-axis accelerometer measured no data" % (
                                    chip_axis,))
                    new_data = helper.process_accelerometer_data(chip_values)
                    if calibration_data[axis] is None:
                        calibration_data[axis] = new_data
                    else:
                        calibration_data[axis].join(new_data)

        configfile = self.printer.lookup_object('configfile')

        for axis in calibrate_axes:
            gcmd.respond_info(
                    "Calculating the best input shaper parameters for %s axis"
                    % (axis,))
            calibration_data[axis].normalize_to_frequencies()
            best_shaper, all_shapers = helper.find_best_shaper(
                    calibration_data[axis], max_smoothing, gcmd.respond_info)
            gcmd.respond_info(
                    "Recommended shaper_type_%s = %s, shaper_freq_%s = %.1f Hz"
                    % (axis, best_shaper.name, axis, best_shaper.freq))
            helper.save_params(configfile, axis,
                               best_shaper.name, best_shaper.freq)
            csv_name = self.save_calibration_data(
                    'calibration_data', name_suffix, helper, axis,
                    calibration_data[axis], all_shapers)
            gcmd.respond_info(
                    "Shaper calibration data written to %s file" % (csv_name,))

        gcmd.respond_info(
            "The SAVE_CONFIG command will update the printer config file\n"
            "with these parameters and restart the printer.")
        if input_shaper is not None:
            input_shaper.enable_shaping()
            gcmd.respond_info("Re-enabled [input_shaper] after calibration")

    def cmd_MEASURE_AXES_NOISE(self, gcmd):
        meas_time = gcmd.get_float("MEAS_TIME", 2.)
        for _, chip in self.accel_chips:
            chip.start_measurements()
        self.printer.lookup_object('toolhead').dwell(meas_time)
        raw_values = [(axis, chip.finish_measurements())
                      for axis, chip in self.accel_chips]
        helper = shaper_calibrate.ShaperCalibrate(self.printer)
        for axis, raw_data in raw_values:
            data = helper.process_accelerometer_data(raw_data)
            vx = data.psd_x.mean()
            vy = data.psd_y.mean()
            vz = data.psd_z.mean()
            gcmd.respond_info("Axes noise for %s-axis accelerometer: "
                              "%.6f (x), %.6f (y), %.6f (z)" % (
                                  axis, vx, vy, vz))

    def is_valid_name_suffix(self, name_suffix):
        return name_suffix.replace('-', '').replace('_', '').isalnum()

    def get_filename(self, base, name_suffix, axis=None, point=None):
        name = base
        if axis:
            name += '_' + axis
        if point:
            name += "_%.3f_%.3f_%.3f" % (point[0], point[1], point[2])
        name += '_' + name_suffix
        return os.path.join("/tmp", name + ".csv")

    def save_calibration_data(self, base_name, name_suffix, shaper_calibrate,
                              axis, calibration_data, all_shapers=None):
        output = self.get_filename(base_name, name_suffix, axis)
        shaper_calibrate.save_calibration_data(output, calibration_data,
                                               all_shapers)
        return output

def load_config(config):
    return ResonanceTester(config)
