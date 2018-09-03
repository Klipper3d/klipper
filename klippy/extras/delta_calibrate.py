# Delta calibration support
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import probe, mathutil


######################################################################
# Delta "stable position" coordinates
######################################################################

# A "stable position" is a 3-tuple containing the number of steps
# taken since hitting the endstop on each delta tower.  Delta
# calibration uses this coordinate system because it allows a position
# to be described independent of the software parameters.

# Return cartesian coordinates for the given stable_positions when the
# given delta_params are used.
def get_position_from_stable(stable_position, delta_params):
    dp = delta_params
    sphere_coords = [
        (t[0], t[1], es - sp * sd)
        for sd, t, es, sp in zip(
                dp.stepdists, dp.towers, dp.abs_endstops, stable_position) ]
    return mathutil.trilateration(sphere_coords, [a**2 for a in dp.arms])

# Return a stable position from the nominal delta tower positions
def get_stable_position(stepper_position, delta_params):
    dp = delta_params
    return [int((ep - sp) / sd + .5)
            for sd, ep, sp in zip(
                    dp.stepdists, dp.abs_endstops, stepper_position)]


######################################################################
# Delta Calibrate class
######################################################################

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
        return [s.get_commanded_position() for s in kin.get_steppers()]
    def finalize(self, offsets, positions):
        z_offset = offsets[2]
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        orig_delta_params = kin.get_delta_params()
        stable_positions = [get_stable_position(p, orig_delta_params)
                            for p in positions]
        params = {'radius': orig_delta_params.radius}
        for i, a in enumerate('abc'):
            params['angle_' + a] = orig_delta_params.angles[i]
            params['arm_' + a] = orig_delta_params.arms[i]
            params['endstop_' + a] = orig_delta_params.endstops[i]
        logging.info("Calculating delta_calibrate with: %s\n"
                     "Initial delta_calibrate parameters: %s",
                     stable_positions, params)
        adj_params = ('radius', 'angle_a', 'angle_b',
                      'endstop_a', 'endstop_b', 'endstop_c')
        def delta_errorfunc(params):
            delta_params = kin.build_delta_params(
                params['radius'], [params['angle_'+a] for a in 'abc'],
                [params['arm_'+a] for a in 'abc'],
                [params['endstop_'+a] for a in 'abc'],
                orig_delta_params.stepdists)
            total_error = 0.
            for stable_pos in stable_positions:
                x, y, z = get_position_from_stable(stable_pos, delta_params)
                total_error += (z - z_offset)**2
            return total_error
        new_params = mathutil.coordinate_descent(
            adj_params, params, delta_errorfunc)
        new_delta_params = kin.build_delta_params(
            new_params['radius'], [new_params['angle_'+a] for a in 'abc'],
            [new_params['arm_'+a] for a in 'abc'],
            [new_params['endstop_'+a] for a in 'abc'],
            orig_delta_params.stepdists)
        logging.info("Calculated delta_calibrate parameters: %s", new_params)
        for spos in stable_positions:
            logging.info("orig: %s new: %s",
                         get_position_from_stable(spos, orig_delta_params),
                         get_position_from_stable(spos, new_delta_params))
        kin.set_delta_params(new_delta_params)
        self.gcode.respond_info(
            "stepper_a: position_endstop: %.6f angle: %.6f\n"
            "stepper_b: position_endstop: %.6f angle: %.6f\n"
            "stepper_c: position_endstop: %.6f angle: %.6f\n"
            "delta_radius: %.6f\n"
            "The above parameters have been applied to the current\n"
            "session. Update the printer config file with the above to\n"
            "use these settings in future sessions." % (
                new_params['endstop_a'], new_params['angle_a'],
                new_params['endstop_b'], new_params['angle_b'],
                new_params['endstop_c'], new_params['angle_c'],
                new_params['radius']))

def load_config(config):
    return DeltaCalibrate(config)
