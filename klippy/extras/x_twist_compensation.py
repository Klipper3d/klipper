# X Twist Compensation
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections
import logging
from math import asin, sin, sqrt
from . import manual_probe
from . import bed_mesh

BED_MESH_PROFILE_VERSION = bed_mesh.PROFILE_VERSION
BED_MESH_CONFIG_NAME = "bed_mesh"

def simple_linear_regression(train_values):
    # train values is a list of tuples of (x, y) values
    # returns the slope and intercept of the line of best fit
    x_sum = 0
    y_sum = 0
    x2_sum = 0
    xy_sum = 0
    for train_value in train_values:
        x = train_value[0]
        y = train_value[1]
        x_sum += x
        y_sum += y
        x2_sum += x**2
        xy_sum += x*y
    n = len(train_values)
    slope = (n*xy_sum - x_sum*y_sum) / (n*x2_sum - x_sum**2)
    intercept = (y_sum - slope*x_sum) / n
    return (slope, intercept)

def create_x_twist_angle_regression(measured_probe_offsets, probe_nozzle_euclid_distance):
    # create the twist angle regression from measured probe offsets
    # measured_probe_offsets is a list of tuples of (x, measured_z_probe_offset) values
    # probe_nozzle_euclid_distance is the probe nozzle euclid distance from PROBE_CALIBRATION
    # for each tuple, calculate the twist angle (arcsin of (measured_z_probe_offset/probe_nozzle_euclid_distance))
    train_values = []
    for measured_probe_offset in measured_probe_offsets:
        x = measured_probe_offset[0]
        measured_z_probe_offset = measured_probe_offset[1]
        twist_angle = asin(measured_z_probe_offset/probe_nozzle_euclid_distance)
        train_values.append((x, twist_angle))
    # create a linear regression using the twist angles
    simple_linear_regression_values = simple_linear_regression(train_values)
    return XTwistRegression(simple_linear_regression_values[0], simple_linear_regression_values[1], probe_nozzle_euclid_distance)

def calculate_probe_nozzle_euclid_distance(probe_y_offset, probe_z_offset):
    # calculate the euclidian distance from the probe to the nozzle (in the y plane)
    return sqrt(probe_y_offset**2 + probe_z_offset**2)

def calculate_compensated_z_probe_offset_at_x(x, twist_angle_regression):
    # calculate the z probe offset at x
    # x is the x probe offset at center of bed
    # twist_angle_regression is a tuple of (slope, intercept) values
    # probe_nozzle_euclid_distance is the probe nozzle euclid distance from PROBE_CALIBRATION
    # returns the corrected z probe offset at given x
    twist_angle = twist_angle_regression.slope * x + twist_angle_regression.intercept
    return sin(twist_angle) * twist_angle_regression.probe_nozzle_euclid_distance
# regression class
class XTwistRegression:
    def __init__(self, slope, intercept, probe_nozzle_euclid_distance):
        self.slope = slope
        self.intercept = intercept
        self.probe_nozzle_euclid_distance = probe_nozzle_euclid_distance

# X twist compensation class
class XTwistCompensation:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.regression = None
        # setup x twist calibrater
        self.calibrater = XTwistCalibrate(config, self)
        # setup persistent storage
        self.pmgr = ProfileManager(config, self)
        self.save_profile = self.pmgr.save_profile
        # get regression from saved config (if it exists)
        # register command to perform calibration (measuring n points along the x axis)
        # register gcodes
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'X_TWIST_COMPENSATE_MESH', self.cmd_COMPENSATE_MESH,
            desc=self.cmd_COMPENSATE_MESH_help)
        self.speed = config.getfloat('speed', 50., above=0.)
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 10)

    def handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')

    def update_status(self):
        self.status = {
            "profile_name": "",
            "slope": 0.0,
            "intercept": 0.0,
            "probe_nozzle_euclid_distance": 0.0,
            "profiles": self.pmgr.get_profiles(),
        }
        if self.regression is not None:
            self.status["profile_name"] = self.pmgr.get_current_profile()
            self.status["slope"] = self.regression.slope
            self.status["intercept"] = self.regression.intercept
            self.status["probe_nozzle_euclid_distance"] = self.regression.probe_nozzle_euclid_distance

    def set_regression(self, regression):
        self.regression = regression

    def get_regression(self):
        return self.regression

    cmd_COMPENSATE_MESH_help = "Compensate a mesh by applying the x twist compensation to the given raw mesh"
    def cmd_COMPENSATE_MESH(self, gcmd):
        # creates a compensated mesh by applying the x twist compensation to the raw mesh
        # get the mesh name from the gcode command
        self.raw_mesh_name = gcmd.get('MESH', None)
        if not self.raw_mesh_name or not self.raw_mesh_name.strip():
            raise gcmd.error("Value for parameter 'MESH' must be specified (name of mesh profile for compensation to be applied to)")
        # load a saved mesh of name raw_mesh_name
        self.bed_mesh = self.printer.lookup_object('bed_mesh', None)
        if not self.bed_mesh:
            raise gcmd.error("[bed_mesh] is not specified in your printer configuration")
        self.bed_mesh_profile_manager = self.bed_mesh.pmgr
        self.all_mesh_profiles = self.bed_mesh_profile_manager.get_profiles()

        # get the regression name from the gcode command
        self.regression_name = gcmd.get('REGRESSION', None)
        if not self.regression_name or not self.regression_name.strip():
            raise gcmd.error("Value for parameter 'REGRESSION' must be specified (name of regression profile for compensation to be applied with)")
        # load a saved regression of name regression_name
        all_regression_profiles = self.pmgr.get_profiles()

        
        if self.raw_mesh_name in self.all_mesh_profiles and self.regression_name in all_regression_profiles:
            raw_mesh = self.all_mesh_profiles[self.raw_mesh_name]
            regression = all_regression_profiles[self.regression_name]
            slope = regression['slope']
            intercept = regression['intercept']
            probe_nozzle_euclid_distance = regression['probe_nozzle_euclid_distance']
            twist_angle_regression = XTwistRegression(slope, intercept, probe_nozzle_euclid_distance)
            # create copy of the raw mesh
            probed_matrix = raw_mesh['points']
            mesh_params = raw_mesh['mesh_params']
            version = BED_MESH_PROFILE_VERSION
            z_mesh = bed_mesh.ZMesh(mesh_params)
            try:
                z_mesh.build_mesh(probed_matrix)
            except bed_mesh.BedMeshError as e:
                raise self.gcode.error(str(e))
            # do compensating, by modifying z values in probed matrix
            # probed matrix is a list of rows of probed z values
            # eg. probed_matrix[0][0] = bottom left corner of mesh, z value
            compensated_matrix = []
            for row_index in range(len(probed_matrix)):
                compensated_row = []
                row = probed_matrix[row_index]
                for col_index in range(len(row)):
                    z = row[col_index]
                    x = self._get_probed_point_x_coordinate(col_index, z_mesh)
                    compensated_z = z + calculate_compensated_z_probe_offset_at_x(x, twist_angle_regression)
                    compensated_row.append(compensated_z)
                compensated_matrix.append(compensated_row)
            # compensated_matrix is a list of list, convert to tuple of tuples
            compensated_matrix = tuple(tuple(row) for row in compensated_matrix)
            logging.info("probed_matrix: {}".format(probed_matrix))
            logging.info("compensated_matrix: {}".format(compensated_matrix))
            
            # save compensated mesh
            configfile = self.printer.lookup_object('configfile')
            prof_name = self.raw_mesh_name + " " + self.regression_name
            cfg_name = BED_MESH_CONFIG_NAME + " " + prof_name

            # set params
            z_values = ""
            for line in compensated_matrix:
                z_values += "\n  "
                for p in line:
                    z_values += "%.6f, " % p
                z_values = z_values[:-2]
            configfile.set(cfg_name, 'version', version)
            configfile.set(cfg_name, 'points', z_values)
            for key, value in mesh_params.items():
                configfile.set(cfg_name, key, value)
            # save copy in local storage
            # ensure any self.profiles returned as status remains immutable
            profiles = dict(self.all_mesh_profiles)
            profiles[prof_name] = profile = {}
            profile['points'] = compensated_matrix
            profile['mesh_params'] = collections.OrderedDict(mesh_params)
            self.bed_mesh_profile_manager.profiles = profiles
            self.bed_mesh_profile_manager.current_profile = prof_name
            self.bed_mesh_profile_manager.bedmesh.update_status()
            self.gcode.respond_info(
                "Bed Mesh state has been saved to profile [%s]\n"
                "for the current session.  The SAVE_CONFIG command will\n"
                "update the printer config file and restart the printer."
                % (prof_name))
        else:
            if self.raw_mesh_name not in self.all_mesh_profiles:
                self.gcode.respond_info("No mesh profile named [%s] to modify" % (self.raw_mesh_name))
            if self.regression_name not in all_regression_profiles:
                self.gcode.respond_info("No regression profile named [%s] to apply" % (self.regression_name))
    def _get_probed_point_x_coordinate(self, x_index, mesh):
        x_min = mesh.mesh_x_min
        x_range = mesh.mesh_x_max - mesh.mesh_x_min
        x_step = x_range / (len(mesh.probed_matrix[0]) - 1)
        return x_min + x_index * x_step
# X twist calibrater class
class XTwistCalibrate:
    def __init__(self, config, compensation):
        self.compensation = compensation
        self._profile_name = None
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.config = config
        self.gcode.register_command(
            'X_TWIST_CALIBRATE', self.cmd_CALIBRATE,
            desc=self.cmd_CALIBRATE_help)

    cmd_CALIBRATE_help = "Measure z probe offset at n points along the x axis, and calculate x twist compensation"
    def cmd_CALIBRATE(self, gcmd):
        self._profile_name = gcmd.get('PROFILE', "default") # user can specify a profile name with PROFILE=, defaults to "default"
        if not self._profile_name.strip():
            raise gcmd.error("Value for parameter 'PROFILE' must be specified")
        self.compensation.set_regression(None) # clear the current regression
        MultipleZProbeOffsetHelper(self.config, gcmd, self.finalize)
        # prompt user to measure n points along the x axis
        # for each point, probe the point, then prompt user to fine tune the probe offset manually
        # get a list of tuples of (x, measured_z_probe_offset) values
        # calculate simple linear regression, and ask user if we should save it to config
        # if yes, save regression to config (slope, intercept)

    def finalize(self, measured_probe_offsets):
        # calculate the probe nozzle euclid distance from PROBE_CALIBRATION
         # get current probe config
        probe = self.printer.lookup_object('probe', None)
        # if no probe, throw error
        logging.info('X_TWIST_INFO: Recorded z offsets {}'.format(measured_probe_offsets))
        probe_x_offset ,probe_y_offset, probe_z_offset = probe.get_offsets()[:3]
        probe_nozzle_euclid_distance = calculate_probe_nozzle_euclid_distance(probe_y_offset, probe_z_offset)
        # create the twist angle regression from measured probe offsets
        twist_angle_regression = create_x_twist_angle_regression(measured_probe_offsets, probe_nozzle_euclid_distance)
        # set the current regression
        self.compensation.set_regression(twist_angle_regression)
        self.gcode.respond_info("X twist calibration complete")
        # save the regression to profile name
        self.compensation.save_profile(self._profile_name)

# Helper to get z probe offset at n points on the x axis
class MultipleZProbeOffsetHelper:
    def __init__(self, config, gcmd, finalize_callback):
        # start_point and end_point are tuples of (x, y) values
        # they represent the NOZZLE's location at the start and end points along the line to probe
        # user can specify the number of points to probe with POINTS=, defaults to 3, in gcmd
        # list of tuples of ((x, y), measured_z_probe_offset) values will be passed to finalize_callback
        self.printer = config.get_printer()
        self.speed = config.getfloat('speed', 50., above=0.)
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 10)
        self.finalize_callback = finalize_callback
        self.n_points = gcmd.get_int('POINTS', 3)
        if self.n_points < 3:
            raise self.printer.command_error("Must specify at least 3 points to probe")
        start_x = gcmd.get_float('START_X', None)
        end_x = gcmd.get_float('END_X', None)
        y = gcmd.get_float('Y', None)
        if start_x is None or end_x is None or y is None:
            raise self.printer.command_error("Must specify START_X, END_X, and Y")
        self.start_point = (start_x, y)
        self.end_point = (end_x, y)
        self.results = []
        # verify no other manual probe is in progress
        manual_probe.verify_no_manual_probe(self.printer)
        # get current probe config
        self.probe = self.printer.lookup_object('probe', None)
        # if no probe, throw error
        if self.probe is None:
            raise self.printer.command_error("No probe found, make sure to specify one under [probe] section in config")
        self.probe_x_offset, self.probe_y_offset, self.probe_z_offset = self.probe.get_offsets()
        self.lift_speed = self.probe.get_lift_speed()
        self.points_to_probe = self._get_points_to_probe()
        self.index_to_probe = 0
        logging.info('X_TWIST_INFO: User configured probe offsets: [X: {}, Y: {}, Z: {}]'.format(self.probe_x_offset, self.probe_y_offset, self.probe_z_offset))
        self.gcmd = gcmd
        self.probe_point()

    def probe_point(self):
        point = self.points_to_probe[self.index_to_probe]
        logging.info('X_TWIST_INFO: Probing point {} of {}'.format(self.index_to_probe + 1, self.n_points))
        # move toolhead up, else probe triggers (user can specify horizontal_move z)
        self.printer.lookup_object('toolhead').manual_move((None, None, self.horizontal_move_z), self.lift_speed)
        # move PROBE to point to be probed (point_to_probe describes nozzle location, hence must offset)
        probe_coordiantes = (point[0] - self.probe_x_offset, point[1] - self.probe_y_offset)
        self.printer.lookup_object('toolhead').manual_move(probe_coordiantes, self.speed)
        
        # probe point
        curpos = self.probe.run_probe(self.gcmd)
        self.current_probe_z = curpos[2]

        # move away from bed
        curpos[2] += self.horizontal_move_z
        self.printer.lookup_object('toolhead').manual_move(curpos, self.lift_speed)

        # move the nozzle over the probe point
        curpos[0] += self.probe_x_offset
        curpos[1] += self.probe_y_offset
        self.printer.lookup_object('toolhead').manual_move(curpos, self.speed)

        # start manual probe
        # callback should store the z probe offset for the point, and move to the next point
        manual_probe.ManualProbeHelper(self.printer, self.gcmd,
                                   self.manual_probe_callback_factory())

    def manual_probe_callback_factory(self):
        point = self.points_to_probe[self.index_to_probe]
        end = self.index_to_probe == len(self.points_to_probe) - 1
        logging.info('X_TWIST_INFO: Point {}/{} was probed, therefore end == {}'.format(self.index_to_probe + 1, self.n_points, end))
        def callback(kin_pos):
            logging.info('X_TWIST_INFO: Callback received kin_pos is {}'.format(kin_pos))
            if kin_pos is None:
                logging.info('X_TWIST_INFO: ManualProbeHelper did not return kinematic position after probe')
                # user cancelled
                return
            z_offset = self.current_probe_z - kin_pos[2]
            self.results.append((point[0], z_offset))
            if not end:
                # move to next point
                self.index_to_probe += 1
                logging.info('X_TWIST_INFO: Moving to next probe point: {}/{}'.format(self.index_to_probe + 1, self.n_points))
                self.probe_point()
            if end:
                # finalize
                logging.info('X_TWIST_INFO: Done probing all {} points! Storing regression.'.format(self.n_points))
                self.finalize_callback(self.results)
        return callback
            
    def _get_points_to_probe(self):
        # calculate points to probe
        # use self.n_points, self.start_point, self.end_point to generate a list of points to probe
        # NOTE: this returns coordinates where the nozzle should be at
        x_range = self.end_point[0] - self.start_point[0]
        x_points = [self.start_point[0] + (x_range / (self.n_points - 1)) * i for i in range(self.n_points)]
        probe_coordinates = [(x, self.start_point[1]) for x in x_points]
        return probe_coordinates

# Manage X twist compensation profiles
class ProfileManager:
    def __init__(self, config, compensation):
        self.name = config.get_name() # gets config section name (in this case, x_twist_compensation)
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.compensation = compensation
        self.current_profile = None

        # load all profiles that have been stored/saved in the config
        self.profiles = {}
        stored_profiles = config.get_prefix_sections(self.name) # gets all config that start with x_twist_compensation
        stored_profiles = [profile for profile in stored_profiles if profile.get_name() != self.name] # remove the x_twist_compensation section, we only want profiles
        for profile in stored_profiles:
            name = profile.get_name().split(' ', 1)[1] # get the profile name, discard the x_twist_compensation prefix
            self.profiles[name] = {} # add this stored profile to current working profiles
            self.profiles[name]['slope'] = profile.getfloat('slope', 0.0) # get the slope from the stored profile
            self.profiles[name]['intercept'] = profile.getfloat('intercept', 0.0) # get the intercept from the stored profile
            self.profiles[name]['probe_nozzle_euclid_distance'] = profile.getfloat('probe_nozzle_euclid_distance', 0.0) # get the probe_nozzle_euclid_distance from the stored profile

        # Register GCode to manage profiles
        self.gcode.register_command(
            'X_TWIST_PROFILE', self.cmd_PROFILE,
            desc=self.cmd_PROFILE_help)

    cmd_PROFILE_help = "X Twist Compensation Persistent Storage Management"
    def cmd_PROFILE(self, gcmd):
        options = collections.OrderedDict({
            'LOAD': self.load_profile,
            'SAVE': self.save_profile,
            'REMOVE': self.remove_profile
        })
        # input validation and call appropriate command based on arguments passed
        for key in options:
            name = gcmd.get(key, None)
            if name is not None:
                if not name.strip():
                    raise gcmd.error(
                        "Value for parameter '%s' must be specified" % (key)
                    )
                if name == "default" and key == 'SAVE':
                    gcmd.respond_info(
                        "Profile 'default' is reserved, please choose"
                        " another profile name.")
                else:
                    options[key](name)
                return
        gcmd.respond_info("Invalid syntax '%s'" % (gcmd.get_commandline(),))

    def save_profile(self, prof_name):
        # save the current regression to the given profile name
        # if the profile name already exists, overwrite it
        current_regression = self.compensation.get_regression()
        if current_regression is None:
            self.gcode.respond_info(
                "Unable to save to profile [%s], the bed has not been probed"
                % (prof_name))
            return
        configfile = self.printer.lookup_object('configfile')
        cfg_name = "%s %s" % (self.name, prof_name)
        configfile.set(cfg_name, 'slope', current_regression.slope)
        configfile.set(cfg_name, 'intercept', current_regression.intercept)
        configfile.set(cfg_name, 'probe_nozzle_euclid_distance', current_regression.probe_nozzle_euclid_distance)
        # save to local storage
        profiles = dict(self.profiles)
        new_profile = {}
        profiles[prof_name] = new_profile
        new_profile['slope'] = current_regression.slope
        new_profile['intercept'] = current_regression.intercept
        new_profile['probe_nozzle_euclid_distance'] = current_regression.probe_nozzle_euclid_distance
        self.profiles = profiles
        self.current_profile = prof_name
        self.compensation.update_status()
        self.gcode.respond_info(
            "X twist compensation state has been saved to profile [%s]\n"
            "for the current session.  The SAVE_CONFIG command will\n"
            "update the printer config file and restart the printer."
            % (prof_name))
    
    def load_profile(self, prof_name):
        # load the given profile name
        profile = self.profiles.get(prof_name, None)
        # if the profile name does not exist, throw an error
        if profile is None:
            raise self.gcode.error(
                "bed_mesh: Unknown profile [%s]" % prof_name)
        slope = profile['slope']
        intercept = profile['intercept']
        probe_nozzle_euclid_distance = profile['probe_nozzle_euclid_distance']
        # might want to do some checking to see if its a valid regression?
        self.current_profile = prof_name
        self.compensation.set_regression(XTwistRegression(slope, intercept, probe_nozzle_euclid_distance))

    def remove_profile(self, prof_name):
        # check if the profile exists
        if prof_name in self.profiles:
            # remove the profile from the config
            configfile = self.printer.lookup_object('configfile')
            cfg_name = "%s %s" % (self.name, prof_name)
            configfile.remove_section(cfg_name)
            # remove the profile from the local storage
            profiles = dict(self.profiles)
            del profiles[prof_name]
            self.profiles = profiles
            self.compensation.update_status()
            self.gcode.respond_info(
                "Profile [%s] removed from storage for this session.\n"
                "The SAVE_CONFIG command will update the printer\n"
                "configuration and restart the printer" % (prof_name))
        else:
            self.gcode.respond_info(
                "No profile named [%s] to remove" % (prof_name))

    def get_profiles(self):
        return self.profiles
    
    def get_current_profile(self):
        return self.current_profile
    
# klippers entry point to this module
def load_config(config): 
    return XTwistCompensation(config)