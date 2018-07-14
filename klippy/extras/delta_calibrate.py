# Delta calibration support
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import probe, mathutil

class DeltaCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        if config.getsection('printer').get('kinematics') != 'delta':
            raise config.error("Delta calibrate is only for delta printers")
        # Calculate default probing points
        radius = config.getfloat('radius', above=0.)
        points = [(0., 0.)]
        scatter = [.95, .90, .85, .70, .75, .80]
        for i in range(6):
            r = math.radians(90. + 60. * i)
            dist = radius * scatter[i]
            points.append((math.cos(r) * dist, math.sin(r) * dist))
        self.probe_helper = probe.ProbePointsHelper(
            config, self, default_points=points)
        # Register DELTA_CALIBRATE command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'DELTA_CALIBRATE', self.cmd_DELTA_CALIBRATE,
            desc=self.cmd_DELTA_CALIBRATE_help)
    cmd_DELTA_CALIBRATE_help = "Delta calibration script"
    def cmd_DELTA_CALIBRATE(self, params):
        self.gcode.run_script_from_command("G28")
        self.probe_helper.start_probe()
    def get_probed_position(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        return kin.get_stable_position()
    def finalize(self, z_offset, positions):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        logging.info("Calculating delta_calibrate with: %s", positions)
        params = kin.get_calibrate_params()
        logging.info("Initial delta_calibrate parameters: %s", params)
        adj_params = ('endstop_a', 'endstop_b', 'endstop_c', 'radius',
                      'angle_a', 'angle_b')
        def delta_errorfunc(params):
            total_error = 0.
            for x, y, z in kin.get_positions_from_stable(positions, params):
                total_error += (z - z_offset)**2
            return total_error
        new_params = mathutil.coordinate_descent(
            adj_params, params, delta_errorfunc)
        logging.info("Calculated delta_calibrate parameters: %s", new_params)
        old_positions = kin.get_positions_from_stable(positions, params)
        new_positions = kin.get_positions_from_stable(positions, new_params)
        for oldpos, newpos in zip(old_positions, new_positions):
            logging.info("orig: %s new: %s", oldpos, newpos)
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
    return DeltaCalibrate(config)
