# Axis Twist Compensation

# Copyright (C) 2022  Jeremy Tan <jeremytkw98@gmail.com>
# This file may be distributed under the terms of the GNU GPLv3 license.

"""
[axis_twist_compensation]
horizontal_move_z: 5
speed: 50
start_x: 0 ; nozzle's x coordinate at the start of the calibration ! required
end_x: 200 ; nozzle's x coordinate at the end of the calibration ! required
y: 100 ; nozzle's y coordinate during the calibration ! required
type: multilinear
"""

import logging
import math
from . import manual_probe as ManualProbe, bed_mesh as BedMesh


DEFAULT_N_POINTS = 3


class Config:
    DEFAULT_SPEED = 50.
    DEFAULT_HORIZONTAL_MOVE_Z = 5.
    REQUIRED = True
    OPTIONAL = False
    CONFIG_OPTIONS = {
        'horizontal_move_z': (float, OPTIONAL, DEFAULT_HORIZONTAL_MOVE_Z),
        'speed': (float, OPTIONAL, DEFAULT_SPEED),
        'start_x': (float, REQUIRED, None),
        'end_x': (float, REQUIRED, None),
        'y': (float, REQUIRED, None),
        'type': (str, OPTIONAL, 'multilinear'),
        'z_compensations': (str, OPTIONAL, None),
    }

    @staticmethod
    def save_to_config(name, z_compensations, configfile):
        values_as_str = ', '.join([Helpers.format_float_to_n_decimals(x)
                                   for x in z_compensations])
        configfile.set(name, 'z_compensations', values_as_str)


class AxisTwistCompensation:
    def __init__(self, config):
        # get printer
        self.config = config
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')

        # get values from [axis_twist_compensation] section in printer .cfg
        for config_key, \
            (config_type, required, default) in Config.CONFIG_OPTIONS.items():
            value = None
            if config_type == float:
                value = config.getfloat(config_key, default)
            elif config_key == 'z_compensations':
                value = Helpers.parse_comma_separated_floats(
                    config.get('z_compensations', default=""))
            else:
                value = config.get(config_key, default)
            if required and value is None:
                raise config.error(
                    "Missing required config option for section [{}]: {}"
                    .format(config.get_name(), config_key))
            setattr(self, config_key, value)

        if self.type not in ['linear', 'multilinear']:
                raise config.error(
                    "Invalid axis_twist_compensation.type value: {}"
                    .format(self.type))

        self.m = None
        self.b = None

        # setup calibrater
        self.calibrater = Calibrater(self)

        self._register_gcode_handlers()

    def get_z_compensation_value(self, x_coord):
        if not self.z_compensations:
            return 0
        self._ensure_init()

        if self.type == 'linear':
            return self._get_z_compensation_value_linear(x_coord)
        elif self.type == 'multilinear':
            return self._get_z_compensation_value_multilinear(x_coord)

    def _get_z_compensation_value_linear(self, x_coord):
        return self.m * x_coord + self.b


    def _get_z_compensation_value_multilinear(self, x_coord):
        z_compensations = self.z_compensations
        n_points = len(z_compensations)
        spacing = (self.end_x - self.start_x) / (n_points - 1)
        interpolate_t = (x_coord - self.start_x) / spacing
        interpolate_i = int(math.floor(interpolate_t))
        interpolate_i = BedMesh.constrain(interpolate_i, 0, n_points - 2)
        interpolate_t -= interpolate_i
        interpolated_z_compensation = BedMesh.lerp(
            interpolate_t, z_compensations[interpolate_i],
            z_compensations[interpolate_i + 1])
        return interpolated_z_compensation

    def _ensure_init(self):
        if self.type == 'linear' and self.m is None:
            z_compensations = self.z_compensations
            n_points = len(z_compensations)
            interval_dist = \
                (self.end_x - self.start_x) / (n_points - 1)
            indexes = [self.start_x + i*interval_dist
                       for i in range(0, n_points)]

            # Calculate the mean of x and y values
            mean_indexes = sum(indexes) / n_points
            mean_z_compensations = sum(z_compensations) / n_points

            # Calculate the covariance and variance
            covar = sum((indexes[i] - mean_indexes) *
                        (z_compensations[i] - mean_z_compensations)
                        for i in range(n_points))
            var = sum((indexes[i] - mean_indexes)**2 for i in range(n_points))

            # Compute the slope (m) and intercept (b) of the best-fit line
            self.m = covar / var
            self.b = mean_z_compensations - self.m * mean_indexes

    def clear_compensations(self):
        self.z_compensations = []
        self.m = None
        self.b = None

    def _register_gcode_handlers(self):
        self.gcode.register_command(
            'AXIS_TWIST_COMPENSATION_CLEAR',
            self.cmd_AXIS_TWIST_COMPENSATION_CLEAR,
            desc=self.cmd_AXIS_TWIST_COMPENSATION_CLEAR_help)

    cmd_AXIS_TWIST_COMPENSATION_CLEAR_help = \
        "Clears the active axis twist compensation"

    def cmd_AXIS_TWIST_COMPENSATION_CLEAR(self, gcmd):
        self.clear_compensations()


class Calibrater:
    def __init__(self, compensation):
        # setup self attributes
        self.compensation = compensation
        self.printer = compensation.printer
        self.config = compensation.config
        self.gcode = self.printer.lookup_object('gcode')
        self.probe = None
        # probe settings are set to none, until they are available
        self.lift_speed, self.probe_x_offset, self.probe_y_offset, _ = \
            None, None, None, None
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect())
        self.speed = compensation.speed
        self.horizontal_move_z = compensation.horizontal_move_z
        self.start_point = (compensation.start_x, compensation.y)
        self.end_point = (compensation.end_x, compensation.y)
        self.results = None
        self.current_point_index = None
        self.gcmd = None

        # register gcode handlers
        self._register_gcode_handlers()

    def _handle_connect(self):
        # gets probe settings when they are available
        def callback():
            self.probe = self.printer.lookup_object('probe', None)
            if (self.probe is None):
                raise self.config.error(
                    "AXIS_TWIST_COMPENSATION requires [probe] to be defined")
            self.lift_speed = self.probe.get_lift_speed()
            self.probe_x_offset, self.probe_y_offset, _ = \
                self.probe.get_offsets()
        return callback

    def _register_gcode_handlers(self):
        # register gcode handlers
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'AXIS_TWIST_COMPENSATION_CALIBRATE',
            self.cmd_AXIS_TWIST_COMPENSATION_CALIBRATE,
            desc=self.cmd_AXIS_TWIST_COMPENSATION_CALIBRATE_help)

    cmd_AXIS_TWIST_COMPENSATION_CALIBRATE_help = """
    Performs the x twist calibration wizard
    Measure z probe offset at n points along the x axis,
    and calculate x twist compensation
    """

    def cmd_AXIS_TWIST_COMPENSATION_CALIBRATE(self, gcmd):
        self.gcmd = gcmd
        n_points = gcmd.get_int('N_POINTS', DEFAULT_N_POINTS)

        # check for valid n_points
        if n_points is None or n_points < 2:
            raise self.gcmd.error(
                "N_POINTS to probe must be at least 2")

        # clear the current config
        self.compensation.clear_compensations()

        # calculate some values
        x_range = self.end_point[0] - self.start_point[0]
        interval_dist = x_range / (n_points - 1)
        nozzle_points = self._calculate_nozzle_points(n_points, interval_dist)
        probe_points = self._calculate_probe_points(
            nozzle_points, self.probe_x_offset, self.probe_y_offset)

        # verify no other manual probe is in progress
        ManualProbe.verify_no_manual_probe(self.printer)

        # begin calibration
        self.current_point_index = 0
        self.results = []
        self._calibration(probe_points, nozzle_points, interval_dist)

    def _calculate_nozzle_points(self, n_points, interval_dist):
        # calculate the points to put the probe at, returned as a list of tuples
        nozzle_points = []
        for i in range(n_points):
            x = self.start_point[0] + i * interval_dist
            y = self.start_point[1]
            nozzle_points.append((x, y))
        return nozzle_points

    def _calculate_probe_points(self, nozzle_points,
        probe_x_offset, probe_y_offset):
        # calculate the points to put the nozzle at
        # returned as a list of tuples
        probe_points = []
        for point in nozzle_points:
            x = point[0] - probe_x_offset
            y = point[1] - probe_y_offset
            probe_points.append((x, y))
        return probe_points

    def _move_helper(self, target_coordinates, override_speed=None):
        # pad target coordinates
        target_coordinates = \
            (target_coordinates[0], target_coordinates[1], None) \
            if len(target_coordinates) == 2 else target_coordinates
        toolhead = self.printer.lookup_object('toolhead')
        speed = self.speed if target_coordinates[2] == None else self.lift_speed
        speed = override_speed if override_speed is not None else speed
        toolhead.manual_move(target_coordinates, speed)

    def _calibration(self, probe_points, nozzle_points, interval):
        # begin the calibration process
        self.gcmd.respond_info("AXIS_TWIST_COMPENSATION_CALIBRATE: "
                               "Probing point %d of %d" % (
                                   self.current_point_index + 1,
                                   len(probe_points)))

        # horizontal_move_z (to prevent probe trigger or hitting bed)
        self._move_helper((None, None, self.horizontal_move_z))

        # move to point to probe
        self._move_helper((probe_points[self.current_point_index][0],
                           probe_points[self.current_point_index][1], None))

        # probe the point
        self.current_measured_z = self.probe.run_probe(self.gcmd)[2]

        # horizontal_move_z (to prevent probe trigger or hitting bed)
        self._move_helper((None, None, self.horizontal_move_z))

        # move the nozzle over the probe point
        self._move_helper((nozzle_points[self.current_point_index]))

        # start the manual (nozzle) probe
        ManualProbe.ManualProbeHelper(
            self.printer, self.gcmd,
            self._manual_probe_callback_factory(
                probe_points, nozzle_points, interval))

    def _manual_probe_callback_factory(self, probe_points,
        nozzle_points, interval):
        # returns a callback function for the manual probe
        is_end = self.current_point_index == len(probe_points) - 1

        def callback(kin_pos):
            if kin_pos is None:
                # probe was cancelled
                self.gcmd.respond_info(
                    "AXIS_TWIST_COMPENSATION_CALIBRATE: Probe cancelled, "
                    "calibration aborted")
                return
            z_offset = self.current_measured_z - kin_pos[2]
            self.results.append(z_offset)
            if is_end:
                # end of calibration
                self._finalize_calibration()
            else:
                # move to next point
                self.current_point_index += 1
                self._calibration(probe_points, nozzle_points, interval)
        return callback

    def _finalize_calibration(self):
        # finalize the calibration process
        # calculate average of results
        avg = sum(self.results) / len(self.results)
        # subtract average from each result
        # so that they are independent of z_offset
        self.results = [avg - x for x in self.results]
        # save the config
        configfile = self.printer.lookup_object('configfile')
        Config.save_to_config(self.config.get_name(), self.results, configfile)
        self.gcode.respond_info(
            "AXIS_TWIST_COMPENSATION state has been saved "
            "for the current session.  The SAVE_CONFIG command will "
            "update the printer config file and restart the printer.")
        # output result
        self.gcmd.respond_info(
            "AXIS_TWIST_COMPENSATION_CALIBRATE: Calibration complete, "
            "offsets: %s, mean z_offset: %f"
            % (self.results, avg))


class Helpers:
    @staticmethod
    def format_float_to_n_decimals(raw_float, n=6):
        # format float to n decimals, defaults to 6
        return "{:.{}f}".format(raw_float, n)

    @staticmethod
    def parse_comma_separated_floats(comma_separated_floats):
        if not comma_separated_floats:
            return []
        # parse comma separated floats into list of floats
        return [float(value) for value in comma_separated_floats.split(', ')]

# klipper's entry point using [axis_twist_compensation] section in printer.cfg

def load_config(config):
    return AxisTwistCompensation(config)
