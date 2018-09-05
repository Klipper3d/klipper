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

# Return a stable position from a cartesian coordinate
def calc_stable_position(coord, delta_params):
    dp = delta_params
    steppos = [
        math.sqrt(a**2 - (t[0]-coord[0])**2 - (t[1]-coord[1])**2) + coord[2]
        for t, a in zip(dp.towers, dp.arms) ]
    return [(ep - sp) / sd
            for sd, ep, sp in zip(dp.stepdists, dp.abs_endstops, steppos)]


######################################################################
# Delta calibration object
######################################################################

# The angles and distances of the calibration object found in
# docs/prints/calibrate_size.stl
MeasureAngles = [210., 270., 330., 30., 90., 150.]
MeasureOuterRadius = 65
MeasureRidgeRadius = 5. - .5

# How much to prefer a distance measurement over a height measurement
MEASURE_WEIGHT = 0.5

# Convert distance measurements made on the calibration object to
# 3-tuples of (actual_distance, stable_position1, stable_position2)
def measurements_to_distances(measured_params, delta_params):
    # Extract params
    mp = measured_params
    dp = delta_params
    scale = mp['SCALE']
    cpw = mp['CENTER_PILLAR_WIDTHS']
    center_widths = [cpw[0], cpw[2], cpw[1], cpw[0], cpw[2], cpw[1]]
    center_dists = [od - cw
                    for od, cw in zip(mp['CENTER_DISTS'], center_widths)]
    outer_dists = [
        od - opw
        for od, opw in zip(mp['OUTER_DISTS'], mp['OUTER_PILLAR_WIDTHS']) ]
    # Convert angles in degrees to an XY multiplier
    obj_angles = map(math.radians, MeasureAngles)
    xy_angles = zip(map(math.cos, obj_angles), map(math.sin, obj_angles))
    # Calculate stable positions for center measurements
    inner_ridge = MeasureRidgeRadius * scale
    inner_pos = [(ax * inner_ridge, ay * inner_ridge, 0.)
                 for ax, ay in xy_angles]
    outer_ridge = (MeasureOuterRadius + MeasureRidgeRadius) * scale
    outer_pos = [(ax * outer_ridge, ay * outer_ridge, 0.)
                 for ax, ay in xy_angles]
    center_positions = [
        (cd, calc_stable_position(ip, dp), calc_stable_position(op, dp))
        for cd, ip, op in zip(center_dists, inner_pos, outer_pos)]
    # Calculate positions of outer measurements
    outer_center = MeasureOuterRadius * scale
    start_pos = [(ax * outer_center, ay * outer_center) for ax, ay in xy_angles]
    shifted_angles = xy_angles[2:] + xy_angles[:2]
    first_pos = [(ax * inner_ridge + spx, ay * inner_ridge + spy, 0.)
                 for (ax, ay), (spx, spy) in zip(shifted_angles, start_pos)]
    second_pos = [(ax * outer_ridge + spx, ay * outer_ridge + spy, 0.)
                  for (ax, ay), (spx, spy) in zip(shifted_angles, start_pos)]
    outer_positions = [
        (od, calc_stable_position(fp, dp), calc_stable_position(sp, dp))
        for od, fp, sp in zip(outer_dists, first_pos, second_pos)]
    return center_positions + outer_positions


######################################################################
# Delta Calibrate class
######################################################################

class DeltaCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        if config.getsection('printer').get('kinematics') != 'delta':
            raise config.error("Delta calibrate is only for delta printers")
        self.printer.try_load_module(config, 'save_config')
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
        # Storage for extended calibration
        self.last_probe_positions = []
        self.last_distances = []
        self.delta_analyze_entry = {'SCALE': 1.}
        # Register gcode commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('DELTA_CALIBRATE', self.cmd_DELTA_CALIBRATE,
                                    desc=self.cmd_DELTA_CALIBRATE_help)
        self.gcode.register_command('DELTA_ANALYZE', self.cmd_DELTA_ANALYZE,
                                    desc=self.cmd_DELTA_ANALYZE_help)
    def restore_state(self):
        section = 'delta_calibrate'
        save_config = self.printer.lookup_object('save_config')
        autosave = save_config.getsection(section)
        radius = autosave.getfloat('radius', None)
        if radius is None:
            # Nothing to restore
            return
        logging.info("Restoring previously saved DELTA_CALIBRATE state")
        # Restore main delta parameters
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        dp = kin.get_delta_params()
        new_delta_params = kin.build_delta_params(
            radius, [autosave.getfloat('angle_'+a, dp.angles[i])
                     for i, a in enumerate('abc')],
            [autosave.getfloat('arm_'+a, dp.arms[i])
                     for i, a in enumerate('abc')],
            [autosave.getfloat('endstop_'+a, dp.endstops[i])
                     for i, a in enumerate('abc')],
            dp.stepdists)
        kin.set_delta_params(new_delta_params)
        # Restore probe stable positions
        self.last_probe_positions = []
        for i in range(999):
            height = autosave.getfloat("height%d" % (i,), None)
            if height is None:
                break
            sa, sb, sc = map(int, autosave.get(
                "height%d_pos" % (i,)).split(','))
            self.last_probe_positions.append((height, (sa, sb, sc)))
        # Restore distance measurements
        self.delta_analyze_entry = {'SCALE': 1.}
        self.last_distances = []
        for i in range(999):
            dist = autosave.getfloat("distance%d" % (i,), None)
            if dist is None:
                break
            sa1, sb1, sc1 = map(float, autosave.get(
                "distance%d_pos1" % (i,)).split(','))
            sa2, sb2, sc2 = map(float, autosave.get(
                "distance%d_pos1" % (i,)).split(','))
            self.last_probe_positions.append(
                (height, (sa1, sb1, sc1), (sa2, sb2, sc2)))
    def save_state(self, probe_positions, distances, params):
        # Save main delta parameters
        section = 'delta_calibrate'
        save_config = self.printer.lookup_object('save_config')
        save_config.remove_section(section)
        save_params = ('radius', 'angle_a', 'angle_b', 'angle_c',
                       'arm_a', 'arm_b', 'arm_c',
                       'endstop_a', 'endstop_b', 'endstop_c')
        for name in save_params:
            save_config.set(section, name, "%.6f" % (params[name],))
        # Save probe stable positions
        for i, (z_offset, spos) in enumerate(probe_positions):
            save_config.set(section, "height%d" % (i,), z_offset)
            save_config.set(section, "height%d_pos" % (i,),
                            "%d,%d,%d" % tuple(spos))
        # Save distance measurements
        for i, (dist, spos1, spos2) in enumerate(distances):
            save_config.set(section, "distance%d" % (i,), dist)
            save_config.set(section, "distance%d_pos1" % (i,),
                            "%.3f,%.3f,%.3f" % tuple(spos1))
            save_config.set(section, "distance%d_pos2" % (i,),
                            "%.3f,%.3f,%.3f" % tuple(spos2))
        # Apply these settings
        self.restore_state()
    def printer_state(self, state):
        if state == 'connect':
            self.restore_state()
    def get_probed_position(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        return [s.get_commanded_position() for s in kin.get_steppers()]
    def finalize(self, offsets, positions):
        # Convert positions into (z_offset, stable_position) pairs
        z_offset = offsets[2]
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        delta_params = kin.get_delta_params()
        probe_positions = [(z_offset, get_stable_position(p, delta_params))
                           for p in positions]
        # Perform analysis
        self.calculate_params(probe_positions, self.last_distances)
    def calculate_params(self, probe_positions, distances):
        # Setup for coordinate descent analysis
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        orig_delta_params = kin.get_delta_params()
        params = {'radius': orig_delta_params.radius}
        for i, a in enumerate('abc'):
            params['angle_' + a] = orig_delta_params.angles[i]
            params['arm_' + a] = orig_delta_params.arms[i]
            params['endstop_' + a] = orig_delta_params.endstops[i]
        logging.info("Calculating delta_calibrate with:\n%s\n%s\n"
                     "Initial delta_calibrate parameters: %s",
                     probe_positions, distances, params)
        adj_params = ('radius', 'angle_a', 'angle_b',
                      'endstop_a', 'endstop_b', 'endstop_c')
        z_weight = 1.
        if distances:
            adj_params += ('arm_a', 'arm_b', 'arm_c')
            z_weight = len(distances) / (MEASURE_WEIGHT * len(probe_positions))
        # Perform coordinate descent
        call_count = [0]
        def delta_errorfunc(params):
            call_count[0] += 1
            if not call_count[0] % 1000:
                self.gcode.respond_info("Working on calibration...")
                self.printer.get_reactor().pause(0.)
            # Build new delta_params for params under test
            delta_params = kin.build_delta_params(
                params['radius'], [params['angle_'+a] for a in 'abc'],
                [params['arm_'+a] for a in 'abc'],
                [params['endstop_'+a] for a in 'abc'],
                orig_delta_params.stepdists)
            # Calculate z height errors
            total_error = 0.
            for z_offset, stable_pos in probe_positions:
                x, y, z = get_position_from_stable(stable_pos, delta_params)
                total_error += (z - z_offset)**2
            total_error *= z_weight
            # Calculate distance errors
            for dist, stable_pos1, stable_pos2 in distances:
                x1, y1, z1 = get_position_from_stable(stable_pos1, delta_params)
                x2, y2, z2 = get_position_from_stable(stable_pos2, delta_params)
                d = math.sqrt((x1-x2)**2 + (y1-y2)**2 + (z1-z2)**2)
                total_error += (d - dist)**2
            return total_error
        new_params = mathutil.coordinate_descent(
            adj_params, params, delta_errorfunc)
        # Log and report results
        logging.info("Calculated delta_calibrate parameters: %s", new_params)
        new_delta_params = kin.build_delta_params(
            new_params['radius'], [new_params['angle_'+a] for a in 'abc'],
            [new_params['arm_'+a] for a in 'abc'],
            [new_params['endstop_'+a] for a in 'abc'],
            orig_delta_params.stepdists)
        for z_offset, spos in probe_positions:
            logging.info("height orig: %.6f new: %.6f goal: %.6f",
                         get_position_from_stable(spos, orig_delta_params)[2],
                         get_position_from_stable(spos, new_delta_params)[2],
                         z_offset)
        for dist, spos1, spos2 in distances:
            x1, y1, z1 = get_position_from_stable(spos1, orig_delta_params)
            x2, y2, z2 = get_position_from_stable(spos2, orig_delta_params)
            orig_dist = math.sqrt((x1-x2)**2 + (y1-y2)**2 + (z1-z2)**2)
            x1, y1, z1 = get_position_from_stable(spos1, new_delta_params)
            x2, y2, z2 = get_position_from_stable(spos2, new_delta_params)
            new_dist = math.sqrt((x1-x2)**2 + (y1-y2)**2 + (z1-z2)**2)
            logging.info("distance orig: %.6f new: %.6f goal: %.6f",
                         orig_dist, new_dist, dist)
        self.gcode.respond_info(
            "stepper_a: position_endstop: %.6f angle: %.6f arm: %.6f\n"
            "stepper_b: position_endstop: %.6f angle: %.6f arm: %.6f\n"
            "stepper_c: position_endstop: %.6f angle: %.6f arm: %.6f\n"
            "delta_radius: %.6f\n"
            "The above parameters have been applied to the current\n"
            "session. Use SAVE_CONFIG to save them for future sessions." % (
                new_params['endstop_a'], new_params['angle_a'],
                new_params['arm_a'],
                new_params['endstop_b'], new_params['angle_b'],
                new_params['arm_b'],
                new_params['endstop_c'], new_params['angle_c'],
                new_params['arm_c'],
                new_params['radius']))
        # Save and apply results
        self.save_state(probe_positions, distances, new_params)
    cmd_DELTA_CALIBRATE_help = "Delta calibration script"
    def cmd_DELTA_CALIBRATE(self, params):
        self.gcode.run_script_from_command("G28")
        self.probe_helper.start_probe()
    def do_extended_calibration(self):
        # Extract distance positions
        if len(self.delta_analyze_entry) <= 1:
            distances = self.last_distances
        elif len(self.delta_analyze_entry) < 5:
            raise self.gcode.error("Not all measurements provided")
        else:
            kin = self.printer.lookup_object('toolhead').get_kinematics()
            delta_params = kin.get_delta_params()
            distances = measurements_to_distances(
                self.delta_analyze_entry, delta_params)
        if not self.last_probe_positions:
            raise self.gcode.error(
                "Must run basic calibration with DELTA_CALIBRATE first")
        # Perform analysis
        self.calculate_params(self.last_probe_positions, distances)
    cmd_DELTA_ANALYZE_help = "Extended delta calibration tool"
    def cmd_DELTA_ANALYZE(self, params):
        # Parse distance measurements
        args = {'CENTER_DISTS': 6, 'CENTER_PILLAR_WIDTHS': 3,
                'OUTER_DISTS': 6, 'OUTER_PILLAR_WIDTHS': 6, 'SCALE': 1}
        for name, count in args.items():
            if name not in params:
                continue
            data = self.gcode.get_str(name, params)
            try:
                parts = map(float, data.split(','))
            except:
                raise self.gcode.error("Unable to parse parameter '%s'" % (
                    name,))
            if len(parts) != count:
                raise self.gcode.error("Parameter '%s' must have %d values" % (
                    name, count))
            self.delta_analyze_entry[name] = parts
            logging.info("DELTA_ANALYZE %s = %s", name, parts)
        # Perform analysis if requested
        if 'CALIBRATE' in params:
            action = self.gcode.get_str('CALIBRATE', params)
            actions = {'extended': 1}
            if action not in actions:
                raise self.gcode.error("Unknown calibrate action")
            self.do_extended_calibration()

def load_config(config):
    return DeltaCalibrate(config)
