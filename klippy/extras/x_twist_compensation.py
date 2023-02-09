# X Twist Compensation
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections
import logging
from math import asin, sin, sqrt
from . import manual_probe

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

def create_x_twist_angle_regression(measured_probe_offsets, probe_nozzle_eucluid_distance):
    # create the twist angle regression from measured probe offsets
    # measured_probe_offsets is a list of tuples of (x, measured_z_probe_offset) values
    # probe_nozzle_eucluid_distance is the probe nozzle eucluid distance from PROBE_CALIBRATION
    # for each tuple, calculate the twist angle (arcsin of (measured_z_probe_offset/probe_nozzle_eucluid_distance))
    train_values = []
    for measured_probe_offset in measured_probe_offsets:
        x = measured_probe_offset[0]
        measured_z_probe_offset = measured_probe_offset[1]
        twist_angle = asin(measured_z_probe_offset/probe_nozzle_eucluid_distance)
        train_values.append((x, twist_angle))
    # create a linear regression using the twist angles
    return simple_linear_regression(train_values)

def calculate_probe_nozzle_euclid_distance(probe_y_offset, probe_z_offset):
    # calculate the euclidian distance from the probe to the nozzle (in the y plane)
    return sqrt(probe_y_offset**2 + probe_z_offset**2)

def calculate_compensated_z_probe_offset_at_x(x, twist_angle_regression, probe_nozzle_eucluid_distance):
    # calculate the z probe offset at x
    # x is the x probe offset at center of bed
    # twist_angle_regression is a tuple of (slope, intercept) values
    # probe_nozzle_eucluid_distance is the probe nozzle eucluid distance from PROBE_CALIBRATION
    # returns the corrected z probe offset at given x
    twist_angle = twist_angle_regression[0] * x + twist_angle_regression[1]
    return sin(twist_angle) * probe_nozzle_eucluid_distance

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
            "profiles": self.pmgr.get_profiles(),
        }
        if self.regression is not None:
            self.status["profile_name"] = self.pmgr.get_current_profile()
            self.status["slope"] = self.regression[0]
            self.status["intercept"] = self.regression[1]

    def set_regression(self, regression):
        self.regression = regression

    def get_regression(self):
        return self.regression

    cmd_COMPENSATE_MESH_help = "Compensate a mesh by applying the x twist compensation to the given raw mesh"
    def cmd_COMPENSATE_MESH(self, raw_mesh):
        # creates a compensated mesh by applying the x twist compensation to the raw mesh
        return

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
        # calculate the probe nozzle eucluid distance from PROBE_CALIBRATION
         # get current probe config
        probe = self.printer.lookup_object('probe', None)
        # if no probe, throw error
        logging.info('X_TWIST_INFO: Recorded z offsets {}'.format(measured_probe_offsets))
        probe_x_offset ,probe_y_offset, probe_z_offset = probe.get_offsets()[:3]
        probe_nozzle_eucluid_distance = calculate_probe_nozzle_euclid_distance(probe_y_offset, probe_z_offset)
        # create the twist angle regression from measured probe offsets
        twist_angle_regression = create_x_twist_angle_regression(measured_probe_offsets, probe_nozzle_eucluid_distance)
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
        configfile.set(cfg_name, 'slope', current_regression[0])
        configfile.set(cfg_name, 'intercept', current_regression[1])
        # save to local storage
        profiles = dict(self.profiles)
        new_profile = {}
        profiles[prof_name] = new_profile
        new_profile['slope'] = current_regression[0]
        new_profile['intercept'] = current_regression[1]
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
        # might want to do some checking to see if its a valid regression?
        self.current_profile = prof_name
        self.compensation.set_regression((slope, intercept))

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