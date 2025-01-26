# X Twist Compensation
# Copyright (C) 2022  Jeremy Tan <jeremytkw98@gmail.com>
# This file may be distributed under the terms of the GNU GPLv3 license.

"""
[x_twist_compensation]
horizontal_move_z: 10
speed: 50
start_x: 0 ; nozzle's x coordinate at the start of the calibration ! required
end_x: 200 ; nozzle's x coordinate at the end of the calibration ! required
y: 100 ; nozzle's y coordinate during the calibration ! required
"""

import logging
import math
from . import manual_probe as ManualProbe, bed_mesh as BedMesh

DEFAULT_N_POINTS = 3
DEFAULT_PROFILE_NAME = 'default'
BED_MESH_CONFIG_NAME = 'bed_mesh'


class Config:
    # values stored in printer.cfg when a profile is saved
    DEFAULT_SPEED = 50.
    DEFAULT_HORIZONTAL_MOVE_Z = 10.
    REQUIRED = True
    OPTIONAL = False
    CONFIG_OPTIONS = {
        'horizontal_move_z': (float, OPTIONAL, DEFAULT_HORIZONTAL_MOVE_Z),
        'speed': (float, OPTIONAL, DEFAULT_SPEED),
        'start_x': (float, REQUIRED, None),
        'end_x': (float, REQUIRED, None),
        'y': (float, REQUIRED, None)
    }


class XTwistCompensation:
    def __init__(self, config):
        # get printer
        self.printer = config.get_printer()

        # get values from [x_twist_compensation] section in printer .cfg
        for config_key, \
            (config_type, required, default) in Config.CONFIG_OPTIONS.items():
            value = None
            if config_type == float:
                value = config.getfloat(config_key, default)
            else:
                value = config.get(config_key, default)
            if required and value is None:
                raise config.error(
                    "Missing required config option for section [{}]: {}"
                    .format(config.get_name(), config_key))
            setattr(self, config_key, value)

        # setup persistent storage
        self.pmgr = ProfileManager(config, self)

        # setup calibrater
        calibrater_config = {
            'horizontal_move_z': self.horizontal_move_z
                if hasattr(self, 'horizontal_move_z') else None,
            'speed': self.speed if hasattr(self, 'speed') else None,
            'start_x': self.start_x if hasattr(self, 'start_x') else None,
            'end_x': self.end_x if hasattr(self, 'end_x') else None,
            'y': self.y if hasattr(self, 'y') else None
        }
        self.calibrater = Calibrater(
            config, self.pmgr, calibrater_config)

        self.enabled = False

        # register gcode handlers
        self._register_gcode_handlers()

    def _register_gcode_handlers(self):
        # register gcode handlers
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'X_TWIST_COMPENSATE_MESH',
            self.cmd_X_TWIST_COMPENSATE_MESH,
            desc=self.cmd_X_TWIST_COMPENSATE_MESH_help)
        self.gcode.register_command(
            'X_TWIST_COMPENSATE_STATUS',
            self.cmd_X_TWIST_COMPENSATE_STATUS,
            desc=self.cmd_X_TWIST_COMPENSATE_STATUS_help)

    def get_z_compensation_value(self, x_coord, optional_profile_name=None):
        # returns the (lineraly interpolated) z compensation value
        # for the given x coordinate
        # uses the current profile if optional_profile_name is not specified
        enabled = self.pmgr.get_is_enabled()
        if enabled or optional_profile_name is not None:
            current_profile = \
                self.pmgr.get_current_profile() \
                if optional_profile_name is None \
                else self.pmgr.get_profile(optional_profile_name)
            z_compensations = current_profile.z_compensations
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
        else:
            return 0

    cmd_X_TWIST_COMPENSATE_MESH_help = \
    "Compensate a mesh by applying the x" \
    "twist compensation to the given raw mesh"

    def cmd_X_TWIST_COMPENSATE_MESH(self, gcmd):
        # get the mesh name from the gcode command
        raw_mesh_name = gcmd.get('MESH_NAME', None)
        Helpers.check_non_empty_param(raw_mesh_name, gcmd, 'MESH_NAME')

        # get the compensation profile name from the gcode command
        compensation_name = gcmd.get('COMPENSATION_NAME', None)
        Helpers.check_non_empty_param(
            compensation_name, gcmd, 'COMPENSATION_NAME')

        # get the bed_mesh object, then the bed_mesh profile manager
        bed_mesh = self.printer.lookup_object('bed_mesh', None)
        if not bed_mesh:
            raise gcmd.error(
                "[bed_mesh] is not specified in your printer configuration")
        bed_mesh_pmgr = bed_mesh.pmgr
        # load specified bed mesh as active bed mesh
        bed_mesh_pmgr.load_profile(raw_mesh_name)
        # get the active bed mesh
        active_bed_mesh = bed_mesh.get_mesh()
        # modify the probed matrix by applying the x twist compensation
        modified_probed_matrix = self._modify_probed_matrix(
            active_bed_mesh, compensation_name)

        # update active mesh with modified probed matrix, save under new name
        compensated_mesh_name = \
            raw_mesh_name + '_compensated_' + compensation_name
        active_bed_mesh.build_mesh(modified_probed_matrix)
        bed_mesh_pmgr.save_profile(compensated_mesh_name)

    def _modify_probed_matrix(self, bed_mesh, compensation_profile_name):
        # do compensating, by modifying z values in probed matrix
        # probed matrix is a list of rows of probed z values
        # eg. probed_matrix[0][0] = bottom left corner of mesh, z value
        probed_matrix = bed_mesh.get_probed_matrix()
        compensated_matrix = []
        for row_index in range(len(probed_matrix)):
            compensated_row = []
            row = probed_matrix[row_index]
            for col_index in range(len(row)):
                z = row[col_index]
                x_coord = self._get_mesh_point_x_coord(col_index, bed_mesh)
                compensated_z = z + \
                    self.get_z_compensation_value(
                        x_coord, compensation_profile_name)
                compensated_row.append(compensated_z)
            compensated_matrix.append(compensated_row)
        # compensated_matrix is a list of list
        # bed_mesh expects tuple of tuple hence convert
        compensated_matrix = tuple(tuple(row) for row in compensated_matrix)
        return compensated_matrix

    def _get_mesh_point_x_coord(self, col_index, mesh):
        # returns the x coordinate of the given column index
        # in the probed matrix
        x_min = mesh.mesh_x_min
        x_range = mesh.mesh_x_max - mesh.mesh_x_min
        x_step = x_range / (len(mesh.probed_matrix[0]) - 1)
        return x_min + col_index * x_step

    cmd_X_TWIST_COMPENSATE_STATUS_help = \
        "Get the status of the x twist compensation"

    def cmd_X_TWIST_COMPENSATE_STATUS(self, gcmd):
        if (self.pmgr.get_is_enabled()):
            profile = self.pmgr.get_current_profile()
            profile_name = profile.name
            profile_z_compensations = profile.z_compensations
            profile_recommended_z_offset = profile.recommended_z_offset
            gcmd.respond_info(
                """
                X twist compensation is enabled
                Profile name: {}
                Profile z compensations: {}
                Profile recommended z offset: {}
                """.format(profile_name,
                    profile_z_compensations, profile_recommended_z_offset))
        else:
            gcmd.respond_info(
                "X twist compensation is disabled, "\
                "load a profile using X_TWIST_PROFILE_LOAD"
            )


class Calibrater:
    def __init__(self, config, pmgr, calibrater_config):
        # setup self attributes
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.pmgr = pmgr
        self.probe = None
        # probe settings are set to none, until they are available
        self.lift_speed, self.probe_x_offset, self.probe_y_offset, \
            self.stored_probe_z_offset = None, None, None, None
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect(config))
        self.speed = calibrater_config['speed']
        self.horizontal_move_z = calibrater_config['horizontal_move_z']
        self.start_point = (
            calibrater_config['start_x'], calibrater_config['y'])
        self.end_point = (calibrater_config['end_x'], calibrater_config['y'])
        self.results = []
        self.current_point_index = None
        self.gcmd = None

        # register gcode handlers
        self._register_gcode_handlers()

    def _handle_connect(self, config):
        # gets probe settings when they are available
        def callback():
            self.probe = self.printer.lookup_object('probe', None)
            if (self.probe is None):
                raise config.error(
                    "X_TWIST_COMPENSATION requires [probe] to be defined")
            self.lift_speed = self.probe.get_lift_speed()
            self.probe_x_offset, self.probe_y_offset, \
                self.stored_probe_z_offset = self.probe.get_offsets()
        return callback

    def _register_gcode_handlers(self):
        # register gcode handlers
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'X_TWIST_CALIBRATE', self.cmd_X_TWIST_CALIBRATE,
            desc=self.cmd_X_TWIST_CALIBRATE_help)

    cmd_X_TWIST_CALIBRATE_help = """
    Performs the x twist calibration wizard
    Measure z probe offset at n points along the x axis,
    and calculate x twist compensation
    Specify PROFILE_NAME=<PROFILE_NAME> - optional, default is 'default'
    """

    def cmd_X_TWIST_CALIBRATE(self, gcmd):
        self.gcmd = gcmd
        # performs the x twist calibration wizard
        # get params from command
        profile_name = gcmd.get('PROFILE_NAME', DEFAULT_PROFILE_NAME)
        n_points = gcmd.get_int('N_POINTS', DEFAULT_N_POINTS)

        # check for valid profile_name
        Helpers.check_non_empty_param(profile_name, self.gcmd, 'PROFILE_NAME')
        # check for valid n_points
        if n_points is None or n_points < 3:
            raise self.gcmd.error(
                "N_POINTS to probe must be at least 3")

        # clear the current profile
        self.pmgr.clear_profile()

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
        self._calibration(
            profile_name, probe_points, nozzle_points, interval_dist)

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

    def _calibration(self, profile_name, probe_points, nozzle_points, interval):
        # begin the calibration process
        self.gcmd.respond_info("X_TWIST_CALIBRATE: Probing point %d of %d" % (
            self.current_point_index + 1, len(probe_points)))

        # horizontal_move_z (to prevent probe trigger or hitting bed)
        self._move_helper((None, None, self.horizontal_move_z))

        # move to point to probe
        self._move_helper((probe_points[self.current_point_index]
                          [0], probe_points[self.current_point_index][1], None))

        # probe the point
        self.probe.run_probe(self.gcmd)

        # horizontal_move_z (to prevent probe trigger or hitting bed)
        self._move_helper((None, None, self.horizontal_move_z))

        # move the nozzle over the probe point
        self._move_helper((nozzle_points[self.current_point_index]))

        # start the manual (nozzle) probe
        ManualProbe.ManualProbeHelper(
            self.printer, self.gcmd,
            self._manual_probe_callback_factory(profile_name,
            probe_points, nozzle_points, interval))

    def _manual_probe_callback_factory(self, profile_name, probe_points,
        nozzle_points, interval):
        # returns a callback function for the manual probe
        is_end = self.current_point_index == len(probe_points) - 1

        def callback(kin_pos):
            if kin_pos is None:
                # probe was cancelled
                self.gcmd.respond_info(
                    "X_TWIST_CALIBRATE: Probe cancelled, calibration aborted")
                return
            z_offset = self.stored_probe_z_offset - kin_pos[2]
            self.results.append(z_offset)
            if is_end:
                # end of calibration
                self._finalize_calibration(profile_name)
            else:
                # move to next point
                self.current_point_index += 1
                self._calibration(
                    profile_name, probe_points, nozzle_points, interval)
        return callback

    def _finalize_calibration(self, profile_name):
        # finalize the calibration process
        # calculate average of results
        avg = sum(self.results) / len(self.results)
        # subtract average from each result
        # so that they are independent of z_offset
        self.results = [avg - x for x in self.results]
        # create a new profile using profile manager
        self.pmgr.create_profile(profile_name, self.results, avg)
        # recommend z offset to user
        self.gcmd.respond_info(
            "X_TWIST_CALIBRATE: Calibration complete, reccomended z_offset: %f"
            % (avg))


class Profile:
    PROFILE_OPTIONS = {
        'z_compensations': str, 'recommended_z_offset': float
    }

    def __init__(self, name, z_compensations, recommended_z_offset):
        self.name = name
        self.z_compensations = z_compensations
        self.recommended_z_offset = recommended_z_offset


class ProfileManager:
    def __init__(self, config, x_twist_compensation):
        # setup self attributes
        self.name = config.get_name()
        self.printer = config.get_printer()
        self.x_twist_compensation = x_twist_compensation
        self.profiles = {}
        self.gcode = self.printer.lookup_object('gcode')
        self.current_profile = None

        # fetch the stored profiles
        self._fetch_stored_profiles(config)
        logging.info('stored profiles: %s', self.profiles)

        # register gcode handlers
        self._register_gcode_handlers()

    def get_is_enabled(self):
        # returns
        return self.current_profile is not None

    def get_current_profile(self):
        # return the current profile
        if self.current_profile is None:
            raise self.gcode.error(
                "No X_TWIST_PROFILE loaded")
        return self.current_profile

    def get_profiles(self):
        #  dictionary of profiles loaded from printer.cfg, key is profile name
        return self.profiles

    def get_profile(self, profile_name):
        # attempt to get profile from self.profiles
        # throws error if profile does not exist or is corrupt
        logging.info('attempting to get profile name %s', profile_name)
        logging.info('available profiles: %s', self.profiles)
        profile = self.profiles.get(profile_name, None)
        if profile is None:
            raise self.gcode.error(
                "X_TWIST_PROFILE %s does not exist" % (profile_name))
        z_compensations = profile.get('z_compensations', None)
        if z_compensations is None:
            raise self.gcode.error(
                "X_TWIST_PROFILE %s does not have z_compensations"
                % (profile_name))
        recommended_z_offset = profile.get('recommended_z_offset', None)
        if recommended_z_offset is None:
            raise self.gcode.error(
                "X_TWIST_PROFILE %s does not have recommended_z_offset"
                % (profile_name))
        return Profile(profile_name, z_compensations, recommended_z_offset)

    def _fetch_stored_profiles(self, config):
        # fetch stored profiles in printer.cfg
        # (using prefix of "x_twist_compensation"")
        stored_profiles = config.get_prefix_sections(self.name)
        stored_profiles = [
            stored_profile for stored_profile in stored_profiles
            if stored_profile.get_name() != self.name
            ]
        # add stored profiles to self.profiles
        for stored_profile in stored_profiles:
            prefixed_name = stored_profile.get_name()
            # remove prefix from name
            name = prefixed_name.split(' ', 1)[1]
            self.profiles[name] = {}
            for option, option_type in Profile.PROFILE_OPTIONS.items():
                if option_type == float:
                    self.profiles[name][option] = stored_profile.getfloat(
                        option)
                elif option_type == str:
                    value = stored_profile.get(option)
                    if option == 'z_compensations':
                        self.profiles[name][option] = \
                        Helpers.parse_comma_separated_floats(value)
                    else:
                        self.profiles[name][option] = stored_profile.get(
                            option)

    def _register_gcode_handlers(self):
        # register gcode handlers
        self.gcode.register_command(
            'X_TWIST_PROFILE', self.cmd_X_TWIST_PROFILE,
            desc=self.cmd_X_TWIST_PROFILE_help)
        self.gcode.register_command(
            'X_TWIST_PROFILE_LOAD', self.cmd_X_TWIST_PROFILE_LOAD,
            desc=self.cmd_X_TWIST_PROFILE_LOAD_help)
        self.gcode.register_command(
            'X_TWIST_PROFILE_SAVE', self.cmd_X_TWIST_PROFILE_SAVE,
            desc=self.cmd_X_TWIST_PROFILE_SAVE_help)
        self.gcode.register_command(
            'X_TWIST_PROFILE_DELETE', self.cmd_X_TWIST_PROFILE_DELETE,
            desc=self.cmd_X_TWIST_PROFILE_DELETE_help)
        self.gcode.register_command(
            'X_TWIST_PROFILE_CLEAR', self.cmd_X_TWIST_PROFILE_CLEAR,
            desc=self.cmd_X_TWIST_PROFILE_CLEAR_help)

    def create_profile(self, profile_name, z_compensations,
        recommended_z_offset):
        # create a new profile
        new_profile = Profile(
            profile_name, z_compensations, recommended_z_offset)
        # save the profile
        self._save_profile(new_profile)

    def load_profile(self, profile_name):
        # set the current profile
        self.current_profile = self.get_profile(profile_name)

    def clear_profile(self):
        # clear the current profile
        self.current_profile = None

    def delete_profile(self, profile_name):
        # try getting the profile to ensure it exists
        self.get_profile(profile_name)
        # remove the profile from config file
        configfile = self.printer.lookup_object('configfile')
        configfile.remove_section('%s %s' % (self.name, profile_name))
        # remove the profile from self.profiles
        profiles = dict(self.profiles)
        del profiles[profile_name]
        self.profiles = profiles
        # inform user to save deletion
        self.gcode.respond_info(
            "Profile [%s] removed from storage for this session.\n"
            "The SAVE_CONFIG command will update the printer\n"
            "configuration and restart the printer" % (profile_name))

    def _save_profile(self, profile):
        profile_name = profile.name
        config_name = '%s %s' % (self.name, profile_name)
        configfile = self.printer.lookup_object('configfile')
        # save the profile to config file
        # also save to self.profiles, make sure immutable by making a copy
        profiles = dict(self.profiles)
        profiles[profile_name] = new_profile = {}
        for option, option_type in Profile.PROFILE_OPTIONS.items():
            value = getattr(profile, option)
            if option_type == float:
                value = float(value)
                new_profile[option] = value  # save to self.profiles
                configfile.set(config_name, option,
                               Helpers.format_float_to_n_decimals(value))
            elif option_type == str:
                if option == 'z_compensations':
                    # convert to list of floats
                    value = [float(x) for x in value]
                    new_profile[option] = value  # save to self.profiles
                    value_as_str = [Helpers.format_float_to_n_decimals(
                        x) for x in value]  # convert to list of strs
                    configfile.set(config_name, option, ', '.join(
                        value_as_str))  # store as comma separated
                else:
                    new_profile[option] = value  # save to self.profiles
                    configfile.set(config_name, option, value)
        # inform user to save changes
        self.gcode.respond_info(
            "X_TWIST_COMPENSATION state has been saved to profile [%s]\n"
            "for the current session.  The SAVE_CONFIG command will\n"
            "update the printer config file and restart the printer."
            % (profile_name))

    def save_current_profile(self, profile_name):
        # get the current profile
        profile = self.get_current_profile()
        # set the name
        profile.name = profile_name
        # save the profile
        self._save_profile(profile)

    cmd_X_TWIST_PROFILE_LOAD_help = \
        "Loads a saved mesh as the active mesh"

    def cmd_X_TWIST_PROFILE_LOAD(self, gcmd):
        # loads a saved mesh as the active mesh
        profile_name = gcmd.get('NAME', None)
        Helpers.check_non_empty_param(profile_name, gcmd, 'NAME')
        self.load_profile(profile_name)

    cmd_X_TWIST_PROFILE_CLEAR_help = \
        "Clears the active mesh"

    def cmd_X_TWIST_PROFILE_CLEAR(self, gcmd):
        # clears the active mesh
        self.clear_profile()

    cmd_X_TWIST_PROFILE_SAVE_help = \
        "Saves the active mesh to the config file"

    def cmd_X_TWIST_PROFILE_SAVE(self, gcmd):
        # saves the active mesh to the config file
        profile_name = gcmd.get('NAME', None)
        Helpers.check_non_empty_param(profile_name, gcmd, 'NAME')
        self.save_current_profile(profile_name)

    cmd_X_TWIST_PROFILE_DELETE_help = \
        "Deletes a saved profile from the config file"

    def cmd_X_TWIST_PROFILE_DELETE(self, gcmd):
        # deletes a saved mesh from the config file
        profile_name = gcmd.get('NAME', None)
        Helpers.check_non_empty_param(profile_name, gcmd, 'NAME')
        self.delete_profile(profile_name)

    cmd_X_TWIST_PROFILE_help = \
        "Prints information on how to use the X_TWIST_PROFILE command"

    def cmd_X_TWIST_PROFILE(self, gcmd):
        raise self.gcode.error(
            """
            Please follow the following syntax:
            X_TWIST_PROFILE_LOAD NAME=<PROFILE_NAME>
            X_TWIST_PROFILE_SAVE NAME=<PROFILE_NAME>
            X_TWIST_PROFILE_DELETE NAME=<PROFILE_NAME>
            X_TWIST_PROFILE_CLEAR
            """
        )


class Helpers:
    @staticmethod
    def format_float_to_n_decimals(raw_float, n=6):
        # format float to n decimals, defaults to 6
        return "{:.{}f}".format(raw_float, n)

    @staticmethod
    def parse_comma_separated_floats(comma_separated_floats):
        # parse comma separated floats into list of floats
        return [float(value) for value in comma_separated_floats.split(', ')]

    @staticmethod
    def check_non_empty_param(param_str, gcmd, param_name=None):
        # throws gcmd error if parameter is None or just spaces
        if param_str is None or not param_str.strip():
            error = "Parameter [%s] is required" % (
                param_name) if param_name else "Parameter is required"
            raise gcmd.error(error)

# klipper's entry point using [x_twist_compensation] section in printer.cfg


def load_config(config):
    return XTwistCompensation(config)