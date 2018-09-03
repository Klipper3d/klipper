# Delta calibration support
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, collections
import probe, mathutil


######################################################################
# Delta "stable position" coordinates
######################################################################

# A "stable position" is a 3-tuple containing the number of steps
# taken since hitting the endstop on each delta tower.  Delta
# calibration uses this coordinate system because it allows a position
# to be described independent of the software parameters.

# Storage helper for delta parameters
DeltaParams = collections.namedtuple('DeltaParams', [
    'radius', 'angles', 'arms', 'endstops', 'stepdists',
    'towers', 'abs_endstops'])

# Generate delta_params from delta configuration parameters
def build_delta_params(params):
    radius = params['radius']
    angles = [params['angle_'+a] for a in 'abc']
    arms = [params['arm_'+a] for a in 'abc']
    endstops = [params['endstop_'+a] for a in 'abc']
    stepdists = [params['stepdist_'+a] for a in 'abc']
    # Calculate the XY cartesian coordinates of the delta towers
    radian_angles = [math.radians(a) for a in angles]
    towers = [(math.cos(a) * radius, math.sin(a) * radius)
              for a in radian_angles]
    # Calculate the absolute Z height of each tower endstop
    radius2 = radius**2
    abs_endstops = [e + math.sqrt(a**2 - radius2)
                    for e, a in zip(endstops, arms)]
    return DeltaParams(radius, angles, arms, endstops, stepdists,
                       towers, abs_endstops)

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
        params = kin.get_calibrate_params()
        orig_delta_params = build_delta_params(params)
        stable_positions = [get_stable_position(p, orig_delta_params)
                            for p in positions]
        logging.info("Calculating delta_calibrate with: %s\n"
                     "Initial delta_calibrate parameters: %s",
                     stable_positions, params)
        adj_params = ('radius', 'angle_a', 'angle_b',
                      'endstop_a', 'endstop_b', 'endstop_c')
        def delta_errorfunc(params):
            delta_params = build_delta_params(params)
            total_error = 0.
            for stable_pos in stable_positions:
                x, y, z = get_position_from_stable(stable_pos, delta_params)
                total_error += (z - z_offset)**2
            return total_error
        new_params = mathutil.coordinate_descent(
            adj_params, params, delta_errorfunc)
        logging.info("Calculated delta_calibrate parameters: %s", new_params)
        new_delta_params = build_delta_params(new_params)
        for spos in stable_positions:
            logging.info("orig: %s new: %s",
                         get_position_from_stable(spos, orig_delta_params),
                         get_position_from_stable(spos, new_delta_params))
        self.gcode.respond_info(
            "stepper_a: position_endstop: %.6f angle: %.6f\n"
            "stepper_b: position_endstop: %.6f angle: %.6f\n"
            "stepper_c: position_endstop: %.6f angle: %.6f\n"
            "delta_radius: %.6f\n"
            "To use these parameters, update the printer config file with\n"
            "the above and then issue a RESTART command" % (
                new_params['endstop_a'], new_params['angle_a'],
                new_params['endstop_b'], new_params['angle_b'],
                new_params['endstop_c'], new_params['angle_c'],
                new_params['radius']))

def load_config(config):
    return DeltaCalibrate(config)
