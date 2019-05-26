# Mesh Bed Leveling
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import math
import json
import probe
import collections

class BedMeshError(Exception):
    pass

# PEP 485 isclose()
def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

# Constrain value between min and max
def constrain(val, min_val, max_val):
    return min(max_val, max(min_val, val))

# Linear interpolation between two values
def lerp(t, v0, v1):
    return (1. - t) * v0 + t * v1

# retreive commma separated pair from config
def parse_pair(config, param, check=True, cast=float,
               minval=None, maxval=None):
    val = config.get(*param).strip().split(',', 1)
    pair = tuple(cast(p.strip()) for p in val)
    if check and len(pair) != 2:
        raise config.error(
            "bed_mesh: malformed '%s' value: %s"
            % (param[0], config.get(*param)))
    elif len(pair) == 1:
        pair = (pair[0], pair[0])
    if minval is not None:
        if pair[0] < minval or pair[1] < minval:
            raise config.error(
                "Option '%s' in section bed_mesh must have a minimum of %s"
                % (param[0]), minval)
    if maxval is not None:
        if pair[0] > maxval or pair[1] > maxval:
            raise config.error(
                "Option '%s' in section bed_mesh must have a maximum of %s"
                % (param[0]), str(minval))
    return pair


class BedMesh:
    FADE_DISABLE = 0x7FFFFFFF
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.last_position = [0., 0., 0., 0.]
        self.calibrate = BedMeshCalibrate(config, self)
        self.z_mesh = None
        self.toolhead = None
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 5.)
        self.fade_start = config.getfloat('fade_start', 1.)
        self.fade_end = config.getfloat('fade_end', 0.)
        self.fade_dist = self.fade_end - self.fade_start
        if self.fade_dist <= 0.:
            self.fade_start = self.fade_end = self.FADE_DISABLE
        self.log_fade_complete = False
        self.base_fade_target = config.getfloat('fade_target', None)
        self.fade_target = 0.
        self.gcode = self.printer.lookup_object('gcode')
        self.splitter = MoveSplitter(config, self.gcode)
        self.gcode.register_command(
            'BED_MESH_OUTPUT', self.cmd_BED_MESH_OUTPUT,
            desc=self.cmd_BED_MESH_OUTPUT_help)
        self.gcode.register_command(
            'BED_MESH_CLEAR', self.cmd_BED_MESH_CLEAR,
            desc=self.cmd_BED_MESH_CLEAR_help)
        self.gcode.set_move_transform(self)
    def handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        self.calibrate.load_default_profile()
    def set_mesh(self, mesh):
        if mesh is not None and self.fade_end != self.FADE_DISABLE:
            self.log_fade_complete = True
            if self.base_fade_target is None:
                self.fade_target = mesh.avg_z
            else:
                self.fade_target = self.base_fade_target
                mesh_min, mesh_max = mesh.get_z_range()
                if (not mesh_min <= self.fade_target <= mesh_max and
                        self.fade_target != 0.):
                    # fade target is non-zero, out of mesh range
                    err_target = self.fade_target
                    self.z_mesh = None
                    self.fade_target = 0.
                    raise self.gcode.error(
                        "bed_mesh: ERROR, fade_target lies outside of mesh z "
                        "range\nmin: %.4f, max: %.4f, fade_target: %.4f"
                        % (mesh_min, mesh_max, err_target))
            if self.fade_target:
                mesh.offset_mesh(self.fade_target)
            mesh_min, mesh_max = mesh.get_z_range()
            if self.fade_dist <= max(abs(mesh_min), abs(mesh_max)):
                self.z_mesh = None
                self.fade_target = 0.
                raise self.gcode.error(
                    "bed_mesh:  Mesh extends outside of the fade range, "
                    "please see the fade_start and fade_end options in"
                    "example-extras.cfg. fade distance: %.2f mesh min: %.4f"
                    "mesh max: %.4f" % (self.fade_dist, mesh_min, mesh_max))
        else:
            self.fade_target = 0.
        self.z_mesh = mesh
        self.splitter.initialize(mesh)
        # cache the current position before a transform takes place
        self.gcode.reset_last_position()
    def get_z_factor(self, z_pos):
        if z_pos >= self.fade_end:
            return 0.
        elif z_pos >= self.fade_start:
            return (self.fade_end - z_pos) / self.fade_dist
        else:
            return 1.
    def get_position(self):
        # Return last, non-transformed position
        if self.z_mesh is None:
            # No mesh calibrated, so send toolhead position
            self.last_position[:] = self.toolhead.get_position()
            self.last_position[2] -= self.fade_target
        else:
            # return current position minus the current z-adjustment
            x, y, z, e = self.toolhead.get_position()
            z_adj = self.z_mesh.calc_z(x, y)
            factor = 1.
            max_adj = z_adj + self.fade_target
            if min(z, (z - max_adj)) >= self.fade_end:
                # Fade out is complete, no factor
                factor = 0.
            elif max(z, (z - max_adj)) >= self.fade_start:
                # Likely in the process of fading out adjustment.
                # Because we don't yet know the gcode z position, use
                # algebra to calculate the factor from the toolhead pos
                factor = ((self.fade_end + self.fade_target - z) /
                          (self.fade_dist - z_adj))
                factor = constrain(factor, 0., 1.)
            final_z_adj = factor * z_adj + self.fade_target
            self.last_position[:] = [x, y, z - final_z_adj, e]
        return list(self.last_position)
    def move(self, newpos, speed):
        factor = self.get_z_factor(newpos[2])
        if self.z_mesh is None or not factor:
            # No mesh calibrated, or mesh leveling phased out.
            x, y, z, e = newpos
            if self.log_fade_complete:
                self.log_fade_complete = False
                logging.info(
                    "bed_mesh fade complete: Current Z: %.4f fade_target: %.4f "
                    % (z, self.fade_target))
            self.toolhead.move([x, y, z + self.fade_target, e], speed)
        else:
            self.splitter.build_move(self.last_position, newpos, factor)
            while not self.splitter.traverse_complete:
                split_move = self.splitter.split()
                if split_move:
                    self.toolhead.move(split_move, speed)
                else:
                    raise self.gcode.error(
                        "Mesh Leveling: Error splitting move ")
        self.last_position[:] = newpos
    cmd_BED_MESH_OUTPUT_help = "Retrieve interpolated grid of probed z-points"
    def cmd_BED_MESH_OUTPUT(self, params):
        if self.z_mesh is None:
            self.gcode.respond_info("Bed has not been probed")
        else:
            self.calibrate.print_probed_positions(self.gcode.respond_info)
            self.z_mesh.print_mesh(self.gcode.respond, self.horizontal_move_z)
    cmd_BED_MESH_CLEAR_help = "Clear the Mesh so no z-adjusment is made"
    def cmd_BED_MESH_CLEAR(self, params):
        self.set_mesh(None)


class BedMeshCalibrate:
    ALGOS = ['lagrange', 'bicubic']
    def __init__(self, config, bedmesh):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.radius = None
        self.relative_reference_index = None
        self.bedmesh = bedmesh
        self.probed_z_table = None
        self.build_map = False
        self.probe_params = collections.OrderedDict()
        points = self._generate_points(config)
        self._init_probe_params(config, points)
        self.probe_helper = probe.ProbePointsHelper(
            config, self.probe_finalize, points)
        self.probe_helper.minimum_points(3)
        # setup persistent storage
        self.profiles = {}
        self._load_storage(config)
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'BED_MESH_CALIBRATE', self.cmd_BED_MESH_CALIBRATE,
            desc=self.cmd_BED_MESH_CALIBRATE_help)
        self.gcode.register_command(
            'BED_MESH_MAP', self.cmd_BED_MESH_MAP,
            desc=self.cmd_BED_MESH_MAP_help)
        self.gcode.register_command(
            'BED_MESH_PROFILE', self.cmd_BED_MESH_PROFILE,
            desc=self.cmd_BED_MESH_PROFILE_help)
    def _generate_points(self, config):
        self.radius = config.getfloat('bed_radius', None, above=0.)
        if self.radius is not None:
            x_cnt = y_cnt = config.getint('round_probe_count', 5, minval=3)
            # round beds must have an odd number of points along each axis
            if not x_cnt & 1:
                raise config.error(
                    "bed_mesh: probe_count must be odd for round beds")
            # radius may have precision to .1mm
            self.radius = math.floor(self.radius * 10) / 10
            min_x = min_y = -self.radius
            max_x = max_y = self.radius
        else:
            # rectangular
            x_cnt, y_cnt = parse_pair(
                config, ('probe_count', '3'), check=False, cast=int, minval=3)
            min_x, min_y = parse_pair(config, ('min_point',))
            max_x, max_y = parse_pair(config, ('max_point',))
            if max_x <= min_x or max_y <= min_y:
                raise config.error('bed_mesh: invalid min/max points')

        self.probe_params['x_count'] = x_cnt
        self.probe_params['y_count'] = y_cnt
        x_dist = (max_x - min_x) / (x_cnt - 1)
        y_dist = (max_y - min_y) / (y_cnt - 1)
        # floor distances down to next hundredth
        x_dist = math.floor(x_dist * 100) / 100
        y_dist = math.floor(y_dist * 100) / 100
        if x_dist <= 1. or y_dist <= 1.:
            raise config.error("bed_mesh: min/max points too close together")

        if self.radius is not None:
            # round bed, min/max needs to be recalculated
            y_dist = x_dist
            new_r = (x_cnt / 2) * x_dist
            min_x = min_y = -new_r
            max_x = max_y = new_r
        else:
            # rectangular bed, only re-calc max_x
            max_x = min_x + x_dist * (x_cnt - 1)
        pos_y = min_y
        points = []
        for i in range(y_cnt):
            for j in range(x_cnt):
                if not i % 2:
                    # move in positive directon
                    pos_x = min_x + j * x_dist
                else:
                    # move in negative direction
                    pos_x = max_x - j * x_dist
                if self.radius is None:
                    # rectangular bed, append
                    points.append((pos_x, pos_y))
                else:
                    # round bed, check distance from origin
                    dist_from_origin = math.sqrt(pos_x*pos_x + pos_y*pos_y)
                    if dist_from_origin <= self.radius:
                        points.append((pos_x, pos_y))
            pos_y += y_dist
        logging.info('bed_mesh: generated points')
        for i, p in enumerate(points):
            logging.info("%d: (%.1f, %.1f)" % (i, p[0], p[1]))
        rref_index = config.get('relative_reference_index', None)
        if rref_index is not None:
            rref_index = int(rref_index)
            if rref_index < 0 or rref_index >= len(points):
                raise config.error("bed_mesh: relative reference index %d "
                    "is out of bounds" % (rref_index))
            logging.info("bed_mesh: relative_reference_index %d is (%.2f, %.2f)"
                % (rref_index, points[rref_index][0], points[rref_index][1]))
            self.relative_reference_index = rref_index
        return points
    def _init_probe_params(self, config, points):
        self.probe_params['min_x'] = min(points, key=lambda p: p[0])[0]
        self.probe_params['max_x'] = max(points, key=lambda p: p[0])[0]
        self.probe_params['min_y'] = min(points, key=lambda p: p[1])[1]
        self.probe_params['max_y'] = max(points, key=lambda p: p[1])[1]
        self.probe_params['x_offset'] = 0.
        self.probe_params['y_offset'] = 0.
        pps = parse_pair(config, ('mesh_pps', '2'), check=False,
                         cast=int, minval=0)
        self.probe_params['mesh_x_pps'] = pps[0]
        self.probe_params['mesh_y_pps'] = pps[1]
        self.probe_params['algo'] = config.get('algorithm', 'lagrange') \
                                          .strip().lower()
        if self.probe_params['algo'] not in self.ALGOS:
            raise config.error(
                "bed_mesh: Unknown algorithm <%s>"
                % (self.probe_params['algo']))
        self.probe_params['tension'] = config.getfloat(
            'bicubic_tension', .2, minval=0., maxval=2.)
    def _load_storage(self, config):
        stored_profs = config.get_prefix_sections(self.name)
        # Remove primary bed_mesh section, as it is not a stored profile
        stored_profs = [s for s in stored_profs
                        if s.get_name() != self.name]
        for profile in stored_profs:
            name = profile.get_name().split(' ', 1)[1]
            self.profiles[name] = {}
            z_values = profile.get('points').split('\n')
            self.profiles[name]['points'] = \
                [[float(pt.strip()) for pt in line.split(',')]
                    for line in z_values if line.strip()]
            self.profiles[name]['probe_params'] = params = \
                collections.OrderedDict()
            for key, value in self.probe_params.iteritems():
                if type(value) is int:
                    params[key] = profile.getint(key)
                elif type(value) is float:
                    params[key] = profile.getfloat(key)
                elif type(value) is str:
                    params[key] = profile.get(key)
    def save_profile(self, prof_name):
        if self.probed_z_table is None:
            self.gcode.respond_info(
                "Unable to save to profile [%s], the bed has not been probed"
                % (prof_name))
            return
        configfile = self.printer.lookup_object('configfile')
        cfg_name = self.name + " " + prof_name
        # set params
        z_values = ""
        for line in self.probed_z_table:
            z_values += "\n  "
            for p in line:
                z_values += "%.6f, " % p
            z_values = z_values[:-2]
        configfile.set(cfg_name, 'points', z_values)
        for key, value in self.probe_params.iteritems():
            configfile.set(cfg_name, key, value)
        # save copy in local storage
        self.profiles[prof_name] = profile = {}
        profile['points'] = list(self.probed_z_table)
        profile['probe_params'] = collections.OrderedDict(self.probe_params)
        self.gcode.respond_info(
            "Bed Mesh state has been saved to profile [%s]\n"
            "for the current session.  The SAVE_CONFIG command will\n"
            "update the printer config file and restart the printer."
            % (prof_name))
    def load_profile(self, prof_name):
        profile = self.profiles.get(prof_name, None)
        if profile is None:
            raise self.gcode.error(
                "bed_mesh: Unknown profile [%s]" % prof_name)
        self.probed_z_table = profile['points']
        zmesh = ZMesh(profile['probe_params'])
        try:
            zmesh.build_mesh(self.probed_z_table)
        except BedMeshError as e:
            raise self.gcode.error(e.message)
        self.bedmesh.set_mesh(zmesh)
    def remove_profile(self, prof_name):
        if prof_name in self.profiles:
            configfile = self.printer.lookup_object('configfile')
            configfile.remove_section('bed_mesh ' + prof_name)
            del self.profiles[prof_name]
            self.gcode.respond_info(
                "Profile [%s] removed from storage for this session.\n"
                "The SAVE_CONFIG command will update the printer\n"
                "configuration and restart the printer" % (prof_name))
        else:
            self.gcode.respond_info(
                "No profile named [%s] to remove" % (prof_name))
    def load_default_profile(self):
        if "default" in self.profiles:
            self.load_profile("default")
    cmd_BED_MESH_PROFILE_help = "Bed Mesh Persistent Storage management"
    def cmd_BED_MESH_PROFILE(self, params):
        options = collections.OrderedDict({
            'LOAD': self.load_profile,
            'SAVE': self.save_profile,
            'REMOVE': self.remove_profile
        })
        for key in options:
            name = self.gcode.get_str(key, params, None)
            if name is not None:
                if name == "default" and key != 'LOAD':
                    self.gcode.respond_info(
                        "Profile 'default' is reserved, please chose"
                        " another profile name.")
                else:
                    options[key](name)
                return
        self.gcode.respond_info(
            "Invalid syntax '%s'" % (params['#original']))
    cmd_BED_MESH_MAP_help = "Probe the bed and serialize output"
    def cmd_BED_MESH_MAP(self, params):
        self.build_map = True
        self.start_calibration(params)
    cmd_BED_MESH_CALIBRATE_help = "Perform Mesh Bed Leveling"
    def cmd_BED_MESH_CALIBRATE(self, params):
        self.build_map = False
        self.start_calibration(params)
    def start_calibration(self, params):
        self.bedmesh.set_mesh(None)
        self.probe_helper.start_probe(params)
    def print_probed_positions(self, print_func):
        if self.probed_z_table is not None:
            msg = "Mesh Leveling Probed Z positions:\n"
            for line in self.probed_z_table:
                for x in line:
                    msg += " %f" % x
                msg += "\n"
            print_func(msg)
        else:
            print_func("bed_mesh: bed has not been probed")
    def probe_finalize(self, offsets, positions):
        self.probe_params['x_offset'] = offsets[0]
        self.probe_params['y_offset'] = offsets[1]
        z_offset = offsets[2]
        x_cnt = self.probe_params['x_count']
        y_cnt = self.probe_params['y_count']

        if self.relative_reference_index is not None:
            # zero out probe z offset and
            # set offset relative to reference index
            z_offset = positions[self.relative_reference_index][2]

        self.probed_z_table = []
        row = []
        prev_pos = positions[0]
        for pos in positions:
            if not isclose(pos[1], prev_pos[1], abs_tol=.1):
                # y has changed, append row and start new
                self.probed_z_table.append(row)
                row = []
            if pos[0] > prev_pos[0]:
                # probed in the positive direction
                row.append(pos[2] - z_offset)
            else:
                # probed in the negative direction
                row.insert(0, pos[2] - z_offset)
            prev_pos = pos
        # append last row
        self.probed_z_table.append(row)

        # make sure the y-axis is the correct length
        if len(self.probed_z_table) != y_cnt:
            raise self.gcode.error(
                ("bed_mesh: Invalid y-axis table length\n"
                 "Probed table length: %d Probed Table:\n%s") %
                (len(self.probed_z_table), str(self.probed_z_table)))

        if self.radius is not None:
            # round bed, extrapolate probed values to create a square mesh
            for row in self.probed_z_table:
                row_size = len(row)
                if not row_size & 1:
                    # an even number of points in a row shouldn't be possible
                    msg = "bed_mesh: incorrect number of points sampled on X\n"
                    msg += "Probed Table:\n"
                    msg += str(self.probed_z_table)
                    raise self.gcode.error(msg)
                buf_cnt = (x_cnt - row_size) / 2
                if buf_cnt == 0:
                    continue
                left_buffer = [row[0]] * buf_cnt
                right_buffer = [row[row_size-1]] * buf_cnt
                row[0:0] = left_buffer
                row.extend(right_buffer)

        #  make sure that the x-axis is the correct length
        for row in self.probed_z_table:
            if len(row) != x_cnt:
                raise self.gcode.error(
                    ("bed_mesh: invalid x-axis table length\n"
                        "Probed table length: %d Probed Table:\n%s") %
                    (len(self.probed_z_table), str(self.probed_z_table)))

        if self.build_map:
            params = self.probe_params
            outdict = {
                'min_point': (params['min_x'], params['min_y']),
                'max_point': (params['max_x'], params['max_y']),
                'xy_offset': offsets[:2],
                'z_positions': self.probed_z_table}
            self.gcode.respond(
                "mesh_map_output " + json.dumps(outdict))
        else:
            mesh = ZMesh(self.probe_params)
            try:
                mesh.build_mesh(self.probed_z_table)
            except BedMeshError as e:
                raise self.gcode.error(e.message)
            self.bedmesh.set_mesh(mesh)
            self.gcode.respond_info("Mesh Bed Leveling Complete")
            self.save_profile("default")


class MoveSplitter:
    def __init__(self, config, gcode):
        self.split_delta_z = config.getfloat(
            'split_delta_z', .025, minval=0.01)
        self.move_check_distance = config.getfloat(
            'move_check_distance', 5., minval=3.)
        self.z_mesh = None
        self.gcode = gcode
    def initialize(self, mesh):
        self.z_mesh = mesh
    def build_move(self, prev_pos, next_pos, factor):
        self.prev_pos = tuple(prev_pos)
        self.next_pos = tuple(next_pos)
        self.current_pos = list(prev_pos)
        self.z_factor = factor
        self.z_offset = self._calc_z_offset(prev_pos)
        self.traverse_complete = False
        self.distance_checked = 0.
        axes_d = [self.next_pos[i] - self.prev_pos[i] for i in range(4)]
        self.total_move_length = math.sqrt(sum([d*d for d in axes_d[:3]]))
        self.axis_move = [not isclose(d, 0., abs_tol=1e-10) for d in axes_d]
    def _calc_z_offset(self, pos):
        z = self.z_mesh.calc_z(pos[0], pos[1])
        return self.z_factor * z + self.z_mesh.mesh_offset
    def _set_next_move(self, distance_from_prev):
        t = distance_from_prev / self.total_move_length
        if t > 1. or t < 0.:
            raise self.gcode.error(
                "bed_mesh: Slice distance is negative "
                "or greater than entire move length")
        for i in range(4):
            if self.axis_move[i]:
                self.current_pos[i] = lerp(
                    t, self.prev_pos[i], self.next_pos[i])
    def split(self):
        if not self.traverse_complete:
            if self.axis_move[0] or self.axis_move[1]:
                # X and/or Y axis move, traverse if necessary
                while self.distance_checked + self.move_check_distance \
                        < self.total_move_length:
                    self.distance_checked += self.move_check_distance
                    self._set_next_move(self.distance_checked)
                    next_z = self._calc_z_offset(self.current_pos)
                    if abs(next_z - self.z_offset) >= self.split_delta_z:
                        self.z_offset = next_z
                        return self.current_pos[0], self.current_pos[1], \
                            self.current_pos[2] + self.z_offset, \
                            self.current_pos[3]
            # end of move reached
            self.current_pos[:] = self.next_pos
            self.z_offset = self._calc_z_offset(self.current_pos)
            # Its okay to add Z-Offset to the final move, since it will not be
            # used again.
            self.current_pos[2] += self.z_offset
            self.traverse_complete = True
            return self.current_pos
        else:
            # Traverse complete
            return None


class ZMesh:
    def __init__(self, params):
        self.mesh_z_table = None
        self.probe_params = params
        self.avg_z = 0.
        self.mesh_offset = 0.
        logging.debug('bed_mesh: probe/mesh parameters:')
        for key, value in self.probe_params.iteritems():
            logging.debug("%s :  %s" % (key, value))
        self.mesh_x_min = params['min_x'] + params['x_offset']
        self.mesh_x_max = params['max_x'] + params['x_offset']
        self.mesh_y_min = params['min_y'] + params['y_offset']
        self.mesh_y_max = params['max_y'] + params['y_offset']
        logging.debug(
            "bed_mesh: Mesh Min: (%.2f,%.2f) Mesh Max: (%.2f,%.2f)"
            % (self.mesh_x_min, self.mesh_y_min,
               self.mesh_x_max, self.mesh_y_max))
        if params['algo'] == 'bicubic':
            self._sample = self._sample_bicubic
        else:
            self._sample = self._sample_lagrange
        # Nummber of points to interpolate per segment
        mesh_x_pps = params['mesh_x_pps']
        mesh_y_pps = params['mesh_y_pps']
        px_cnt = params['x_count']
        py_cnt = params['y_count']
        if px_cnt == 3 or py_cnt == 3:
            # a mesh with 3 points on either axis defaults to legrange
            # upsampling
            self._sample = self._sample_lagrange
            self.probe_params['algo'] = 'lagrange'
        if mesh_x_pps == 0 and mesh_y_pps == 0:
            # No interpolation, sample the probed points directly
            self._sample = self._sample_direct
            self.probe_params['algo'] = 'direct'
        self.mesh_x_count = (px_cnt - 1) * mesh_x_pps + px_cnt
        self.mesh_y_count = (py_cnt - 1) * mesh_y_pps + py_cnt
        self.x_mult = mesh_x_pps + 1
        self.y_mult = mesh_y_pps + 1
        logging.debug("bed_mesh: Mesh grid size - X:%d, Y:%d"
                      % (self.mesh_x_count, self.mesh_y_count))
        self.mesh_x_dist = (self.mesh_x_max - self.mesh_x_min) / \
                           (self.mesh_x_count - 1)
        self.mesh_y_dist = (self.mesh_y_max - self.mesh_y_min) / \
                           (self.mesh_y_count - 1)
    def print_mesh(self, print_func, move_z=None):
        if self.mesh_z_table is not None:
            msg = "Mesh X,Y: %d,%d\n" % (self.mesh_x_count, self.mesh_y_count)
            if move_z is not None:
                msg += "Search Height: %d\n" % (move_z)
            msg += "Mesh Average: %.2f\n" % (self.avg_z)
            rng = self.get_z_range()
            msg += "Mesh Range: min=%.4f max=%.4f\n" % (rng[0], rng[1])
            msg += "Interpolation Algorithm: %s\n" \
                   % (self.probe_params['algo'])
            msg += "Measured points:\n"
            for y_line in range(self.mesh_y_count - 1, -1, -1):
                for z in self.mesh_z_table[y_line]:
                    msg += "  %f" % (z + self.mesh_offset)
                msg += "\n"
            print_func(msg)
        else:
            print_func("bed_mesh: Z Mesh not generated")
    def build_mesh(self, z_table):
        self._sample(z_table)
        self.avg_z = (sum([sum(x) for x in self.mesh_z_table]) /
                      sum([len(x) for x in self.mesh_z_table]))
        # Round average to the nearest 100th.  This
        # should produce an offset that is divisible by common
        # z step distances
        self.avg_z = round(self.avg_z, 2)
        self.print_mesh(logging.debug)
    def offset_mesh(self, offset):
        if self.mesh_z_table:
            self.mesh_offset = offset
            for y_line in self.mesh_z_table:
                for idx, z in enumerate(y_line):
                    y_line[idx] = z - self.mesh_offset
    def get_x_coordinate(self, index):
        return self.mesh_x_min + self.mesh_x_dist * index
    def get_y_coordinate(self, index):
        return self.mesh_y_min + self.mesh_y_dist * index
    def calc_z(self, x, y):
        if self.mesh_z_table is not None:
            tbl = self.mesh_z_table
            tx, xidx = self._get_linear_index(x, 0)
            ty, yidx = self._get_linear_index(y, 1)
            z0 = lerp(tx, tbl[yidx][xidx], tbl[yidx][xidx+1])
            z1 = lerp(tx, tbl[yidx+1][xidx], tbl[yidx+1][xidx+1])
            return lerp(ty, z0, z1)
        else:
            # No mesh table generated, no z-adjustment
            return 0.
    def get_z_range(self):
        if self.mesh_z_table is not None:
            mesh_min = min([min(x) for x in self.mesh_z_table])
            mesh_max = max([max(x) for x in self.mesh_z_table])
            return mesh_min, mesh_max
        else:
            return 0., 0.
    def _get_linear_index(self, coord, axis):
        if axis == 0:
            # X-axis
            mesh_min = self.mesh_x_min
            mesh_cnt = self.mesh_x_count
            mesh_dist = self.mesh_x_dist
            cfunc = self.get_x_coordinate
        else:
            # Y-axis
            mesh_min = self.mesh_y_min
            mesh_cnt = self.mesh_y_count
            mesh_dist = self.mesh_y_dist
            cfunc = self.get_y_coordinate
        t = 0.
        idx = int(math.floor((coord - mesh_min) / mesh_dist))
        idx = constrain(idx, 0, mesh_cnt - 2)
        t = (coord - cfunc(idx)) / mesh_dist
        return constrain(t, 0., 1.), idx
    def _sample_direct(self, z_table):
        self.mesh_z_table = z_table
    def _sample_lagrange(self, z_table):
        x_mult = self.x_mult
        y_mult = self.y_mult
        self.mesh_z_table = \
            [[0. if ((i % x_mult) or (j % y_mult))
             else z_table[j/y_mult][i/x_mult]
             for i in range(self.mesh_x_count)]
             for j in range(self.mesh_y_count)]
        xpts, ypts = self._get_lagrange_coords(z_table)
        # Interpolate X coordinates
        for i in range(self.mesh_y_count):
            # only interpolate X-rows that have probed coordinates
            if i % y_mult != 0:
                continue
            for j in range(self.mesh_x_count):
                if j % x_mult == 0:
                    continue
                x = self.get_x_coordinate(j)
                self.mesh_z_table[i][j] = self._calc_lagrange(xpts, x, i, 0)
        # Interpolate Y coordinates
        for i in range(self.mesh_x_count):
            for j in range(self.mesh_y_count):
                if j % y_mult == 0:
                    continue
                y = self.get_y_coordinate(j)
                self.mesh_z_table[j][i] = self._calc_lagrange(ypts, y, i, 1)
    def _get_lagrange_coords(self, z_table):
        xpts = []
        ypts = []
        for i in range(self.probe_params['x_count']):
            xpts.append(self.get_x_coordinate(i * self.x_mult))
        for j in range(self.probe_params['y_count']):
            ypts.append(self.get_y_coordinate(j * self.y_mult))
        return xpts, ypts
    def _calc_lagrange(self, lpts, c, vec, axis=0):
        pt_cnt = len(lpts)
        total = 0.
        for i in range(pt_cnt):
            n = 1.
            d = 1.
            for j in range(pt_cnt):
                if j == i:
                    continue
                n *= (c - lpts[j])
                d *= (lpts[i] - lpts[j])
            if axis == 0:
                # Calc X-Axis
                z = self.mesh_z_table[vec][i*self.x_mult]
            else:
                # Calc Y-Axis
                z = self.mesh_z_table[i*self.y_mult][vec]
            total += z * n / d
        return total
    def _sample_bicubic(self, z_table):
        # should work for any number of probe points above 3x3
        x_mult = self.x_mult
        y_mult = self.y_mult
        c = self.probe_params['tension']
        self.mesh_z_table = \
            [[0. if ((i % x_mult) or (j % y_mult))
             else z_table[j/y_mult][i/x_mult]
             for i in range(self.mesh_x_count)]
             for j in range(self.mesh_y_count)]
        # Interpolate X values
        for y in range(self.mesh_y_count):
            if y % y_mult != 0:
                continue
            for x in range(self.mesh_x_count):
                if x % x_mult == 0:
                    continue
                pts = self._get_x_ctl_pts(x, y)
                self.mesh_z_table[y][x] = self._cardinal_spline(pts, c)
        # Interpolate Y values
        for x in range(self.mesh_x_count):
            for y in range(self.mesh_y_count):
                if y % y_mult == 0:
                    continue
                pts = self._get_y_ctl_pts(x, y)
                self.mesh_z_table[y][x] = self._cardinal_spline(pts, c)
    def _get_x_ctl_pts(self, x, y):
        # Fetch control points and t for a X value in the mesh
        x_mult = self.x_mult
        x_row = self.mesh_z_table[y]
        last_pt = self.mesh_x_count - 1 - x_mult
        if x < x_mult:
            p0 = p1 = x_row[0]
            p2 = x_row[x_mult]
            p3 = x_row[2*x_mult]
            t = x / float(x_mult)
        elif x > last_pt:
            p0 = x_row[last_pt - x_mult]
            p1 = x_row[last_pt]
            p2 = p3 = x_row[last_pt + x_mult]
            t = (x - last_pt) / float(x_mult)
        else:
            found = False
            for i in range(x_mult, last_pt, x_mult):
                if x > i and x < (i + x_mult):
                    p0 = x_row[i - x_mult]
                    p1 = x_row[i]
                    p2 = x_row[i + x_mult]
                    p3 = x_row[i + 2*x_mult]
                    t = (x - i) / float(x_mult)
                    found = True
                    break
            if not found:
                raise BedMeshError(
                    "bed_mesh: Error finding x control points")
        return p0, p1, p2, p3, t
    def _get_y_ctl_pts(self, x, y):
        # Fetch control points and t for a Y value in the mesh
        y_mult = self.y_mult
        last_pt = self.mesh_y_count - 1 - y_mult
        y_col = self.mesh_z_table
        if y < y_mult:
            p0 = p1 = y_col[0][x]
            p2 = y_col[y_mult][x]
            p3 = y_col[2*y_mult][x]
            t = y / float(y_mult)
        elif y > last_pt:
            p0 = y_col[last_pt - y_mult][x]
            p1 = y_col[last_pt][x]
            p2 = p3 = y_col[last_pt + y_mult][x]
            t = (y - last_pt) / float(y_mult)
        else:
            found = False
            for i in range(y_mult, last_pt, y_mult):
                if y > i and y < (i + y_mult):
                    p0 = y_col[i - y_mult][x]
                    p1 = y_col[i][x]
                    p2 = y_col[i + y_mult][x]
                    p3 = y_col[i + 2*y_mult][x]
                    t = (y - i) / float(y_mult)
                    found = True
                    break
            if not found:
                raise BedMeshError(
                    "bed_mesh: Error finding y control points")
        return p0, p1, p2, p3, t
    def _cardinal_spline(self, p, tension):
        t = p[4]
        t2 = t*t
        t3 = t2*t
        m1 = tension * (p[2] - p[0])
        m2 = tension * (p[3] - p[1])
        a = p[1] * (2*t3 - 3*t2 + 1)
        b = p[2] * (-2*t3 + 3*t2)
        c = m1 * (t3 - 2*t2 + t)
        d = m2 * (t3 - t2)
        return a + b + c + d


def load_config(config):
    return BedMesh(config)
