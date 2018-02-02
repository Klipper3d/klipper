# Delta calibration support
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import probe, delta

class DeltaCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        if config.getsection('printer').get('kinematics') != 'delta':
            raise config.error("Delta calibrate is only for delta printers")
        self.radius = config.getfloat('radius', above=0.)
        self.speed = config.getfloat('speed', 50., above=0.)
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 5.)
        self.probe_z_offset = config.getfloat('probe_z_offset', 0.)
        self.manual_probe = config.getboolean('manual_probe', None)
        if self.manual_probe is None:
            self.manual_probe = not config.has_section('probe')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'DELTA_CALIBRATE', self.cmd_DELTA_CALIBRATE,
            desc=self.cmd_DELTA_CALIBRATE_help)
    cmd_DELTA_CALIBRATE_help = "Delta calibration script"
    def cmd_DELTA_CALIBRATE(self, params):
        # Setup probe points
        points = [(0., 0.)]
        scatter = [.95, .90, .85, .70, .75, .80]
        for i in range(6):
            r = math.radians(90. + 60. * i)
            dist = self.radius * scatter[i]
            points.append((math.cos(r) * dist, math.sin(r) * dist))
        # Probe them
        self.gcode.run_script("G28")
        probe.ProbePointsHelper(self.printer, points, self.horizontal_move_z,
                                self.speed, self.manual_probe, self)
    def get_position(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        return kin.get_stable_position()
    def finalize(self, positions):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        logging.debug("Got: %s", positions)
        params = kin.get_calibrate_params()
        logging.debug("Params: %s", params)
        adj_params = ('endstop_a', 'endstop_b', 'endstop_c', 'radius',
                      'angle_a', 'angle_b')
        def delta_errorfunc(params):
            total_error = 0.
            for spos in positions:
                x, y, z = delta.get_position_from_stable(spos, params)
                total_error += (z - self.probe_z_offset)**2
            return total_error
        new_params = probe.coordinate_descent(
            adj_params, params, delta_errorfunc)
        logging.debug("Got2: %s", new_params)
        for spos in positions:
            logging.debug("orig: %s new: %s",
                          delta.get_position_from_stable(spos, params),
                          delta.get_position_from_stable(spos, new_params))
        self.gcode.respond_info(
            "stepper_a: position_endstop: %.6f angle: %.6f\n"
            "stepper_b: position_endstop: %.6f angle: %.6f\n"
            "stepper_c: position_endstop: %.6f angle: %.6f\n"
            "radius: %.6f\n"
            "To use these parameters, update the printer config file with\n"
            "the above and then issue a RESTART command" % (
                new_params['endstop_a'], new_params['angle_a'],
                new_params['endstop_b'], new_params['angle_b'],
                new_params['endstop_c'], new_params['angle_c'],
                new_params['radius']))

def load_config(config):
    if config.get_name() != 'delta_calibrate':
        raise config.error("Invalid delta_calibrate config name")
    return DeltaCalibrate(config)
