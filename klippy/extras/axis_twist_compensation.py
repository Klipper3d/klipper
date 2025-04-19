# Axis Twist Compensation
#
# Copyright (C) 2022  Jeremy Tan <jeremytkw98@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import math
from . import manual_probe, bed_mesh, probe


DEFAULT_SAMPLE_COUNT = 3
DEFAULT_SPEED = 50.
DEFAULT_HORIZONTAL_MOVE_Z = 5.


class AxisTwistCompensation:
    def __init__(self, config):
        # get printer
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')

        # get values from [axis_twist_compensation] section in printer .cfg
        self.horizontal_move_z = config.getfloat('horizontal_move_z',
                                                 DEFAULT_HORIZONTAL_MOVE_Z)
        self.speed = config.getfloat('speed', DEFAULT_SPEED)
        self.calibrate_start_x = config.getfloat('calibrate_start_x',
                                                default=None)
        self.calibrate_end_x = config.getfloat('calibrate_end_x', default=None)
        self.calibrate_y = config.getfloat('calibrate_y', default=None)
        self.z_compensations = config.getlists('z_compensations',
                                               default=[], parser=float)
        self.compensation_start_x = config.getfloat('compensation_start_x',
                                                    default=None)
        self.compensation_end_x = config.getfloat('compensation_end_x',
                                                  default=None)

        self.calibrate_start_y = config.getfloat('calibrate_start_y',
                                                  default=None)
        self.calibrate_end_y = config.getfloat('calibrate_end_y', default=None)
        self.calibrate_x = config.getfloat('calibrate_x', default=None)
        self.compensation_start_y = config.getfloat('compensation_start_y',
                                                    default=None)
        self.compensation_end_y = config.getfloat('compensation_end_y',
                                                  default=None)
        self.zy_compensations = config.getlists('zy_compensations',
                                                default=[], parser=float)

        # setup calibrater
        self.calibrater = Calibrater(self, config)
        # register events
        self.printer.register_event_handler("probe:update_results",
                                            self._update_z_compensation_value)

    def _update_z_compensation_value(self, pos):
        if self.z_compensations:
            pos[2] += self._get_interpolated_z_compensation(
                pos[0], self.z_compensations,
                self.compensation_start_x,
                self.compensation_end_x
                )

        if self.zy_compensations:
            pos[2] += self._get_interpolated_z_compensation(
                pos[1], self.zy_compensations,
                self.compensation_start_y,
                self.compensation_end_y
                )

    def _get_interpolated_z_compensation(
            self, coord, z_compensations,
            comp_start,
            comp_end
            ):

        sample_count = len(z_compensations)
        spacing = ((comp_end - comp_start)
                   / (sample_count - 1))
        interpolate_t = (coord - comp_start) / spacing
        interpolate_i = int(math.floor(interpolate_t))
        interpolate_i = bed_mesh.constrain(interpolate_i, 0, sample_count - 2)
        interpolate_t -= interpolate_i
        interpolated_z_compensation = bed_mesh.lerp(
            interpolate_t, z_compensations[interpolate_i],
            z_compensations[interpolate_i + 1])
        return interpolated_z_compensation

    def clear_compensations(self, axis=None):
        if axis is None:
            self.z_compensations = []
            self.zy_compensations = []
        elif axis == 'X':
            self.z_compensations = []
        elif axis == 'Y':
            self.zy_compensations = []

class Calibrater:
    def __init__(self, compensation, config):
        # setup self attributes
        self.compensation = compensation
        self.printer = compensation.printer
        self.gcode = self.printer.lookup_object('gcode')
        self.probe = None
        # probe settings are set to none, until they are available
        self.lift_speed, self.probe_x_offset, self.probe_y_offset, _ = \
            None, None, None, None
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        self.speed = compensation.speed
        self.horizontal_move_z = compensation.horizontal_move_z
        self.x_start_point = (compensation.calibrate_start_x,
                            compensation.calibrate_y)
        self.x_end_point = (compensation.calibrate_end_x,
                          compensation.calibrate_y)
        self.y_start_point = (compensation.calibrate_x,
                            compensation.calibrate_start_y)
        self.y_end_point = (compensation.calibrate_x,
                            compensation.calibrate_end_y)
        self.results = None
        self.current_point_index = None
        self.gcmd = None
        self.configname = config.get_name()

        # register gcode handlers
        self._register_gcode_handlers()

    def _handle_connect(self):
        self.probe = self.printer.lookup_object('probe', None)
        if self.probe is None:
            raise self.printer.config_error(
                "AXIS_TWIST_COMPENSATION requires [probe] to be defined")
        self.lift_speed = self.probe.get_probe_params()['lift_speed']
        self.probe_x_offset, self.probe_y_offset, _ = \
            self.probe.get_offsets()

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
        sample_count = gcmd.get_int('SAMPLE_COUNT', DEFAULT_SAMPLE_COUNT)
        axis = gcmd.get('AXIS', 'X')

        # check for valid sample_count
        if sample_count < 2:
            raise self.gcmd.error(
                "SAMPLE_COUNT to probe must be at least 2")

        # calculate the points to put the probe at, returned as a list of tuples
        nozzle_points = []

        if axis == 'X':

            self.compensation.clear_compensations('X')

            if not all([
                self.x_start_point[0],
                self.x_end_point[0],
                self.x_start_point[1]
                ]):
                raise self.gcmd.error(
                    """AXIS_TWIST_COMPENSATION for X axis requires
                    calibrate_start_x, calibrate_end_x and calibrate_y
                    to be defined
                    """
                    )

            start_point = self.x_start_point
            end_point = self.x_end_point

            x_axis_range = end_point[0] - start_point[0]
            interval_dist = x_axis_range / (sample_count - 1)

            for i in range(sample_count):
                x = start_point[0] + i * interval_dist
                y = start_point[1]
                nozzle_points.append((x, y))

        elif axis == 'Y':

            self.compensation.clear_compensations('Y')

            if not all([
                self.y_start_point[0],
                self.y_end_point[0],
                self.y_start_point[1]
                ]):
                raise self.gcmd.error(
                    """AXIS_TWIST_COMPENSATION for Y axis requires
                    calibrate_start_y, calibrate_end_y and calibrate_x
                    to be defined
                    """
                    )

            start_point = self.y_start_point
            end_point = self.y_end_point

            y_axis_range = end_point[1] - start_point[1]
            interval_dist = y_axis_range / (sample_count - 1)

            for i in range(sample_count):
                x = start_point[0]
                y = start_point[1] + i * interval_dist
                nozzle_points.append((x, y))

        else:
            raise self.gcmd.error(
                "AXIS_TWIST_COMPENSATION_CALIBRATE: "
                "Invalid axis.")

        probe_points = self._calculate_probe_points(
            nozzle_points, self.probe_x_offset, self.probe_y_offset)

        # verify no other manual probe is in progress
        manual_probe.verify_no_manual_probe(self.printer)

        # begin calibration
        self.current_point_index = 0
        self.results = []
        self.current_axis = axis
        self._calibration(probe_points, nozzle_points, interval_dist)

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
        pos = probe.run_single_probe(self.probe, self.gcmd)
        self.current_measured_z = pos[2]

        # horizontal_move_z (to prevent probe trigger or hitting bed)
        self._move_helper((None, None, self.horizontal_move_z))

        # move the nozzle over the probe point
        self._move_helper((nozzle_points[self.current_point_index]))

        # start the manual (nozzle) probe
        manual_probe.ManualProbeHelper(
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
        values_as_str = ', '.join(["{:.6f}".format(x)
                                   for x in self.results])

        if(self.current_axis == 'X'):

            configfile.set(self.configname, 'z_compensations', values_as_str)
            configfile.set(self.configname, 'compensation_start_x',
                        self.x_start_point[0])
            configfile.set(self.configname, 'compensation_end_x',
                        self.x_end_point[0])

            self.compensation.z_compensations = self.results
            self.compensation.compensation_start_x = self.x_start_point[0]
            self.compensation.compensation_end_x = self.x_end_point[0]

        elif(self.current_axis == 'Y'):

            configfile.set(self.configname, 'zy_compensations', values_as_str)
            configfile.set(self.configname, 'compensation_start_y',
                        self.y_start_point[1])
            configfile.set(self.configname, 'compensation_end_y',
                        self.y_end_point[1])

            self.compensation.zy_compensations = self.results
            self.compensation.compensation_start_y = self.y_start_point[1]
            self.compensation.compensation_end_y = self.y_end_point[1]

        self.gcode.respond_info(
            "AXIS_TWIST_COMPENSATION state has been saved "
            "for the current session.  The SAVE_CONFIG command will "
            "update the printer config file and restart the printer.")
        # output result
        self.gcmd.respond_info(
            "AXIS_TWIST_COMPENSATION_CALIBRATE: Calibration complete, "
            "offsets: %s, mean z_offset: %f"
            % (self.results, avg))


# klipper's entry point using [axis_twist_compensation] section in printer.cfg
def load_config(config):
    return AxisTwistCompensation(config)
