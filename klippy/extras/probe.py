# Z-Probe support
#
# Copyright (C) 2017-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins, homing, manual_probe

HINT_TIMEOUT = """
Make sure to home the printer before probing. If the probe
did not move far enough to trigger, then consider reducing
the Z axis minimum position so the probe can travel further
(the Z minimum position can be negative).
"""

class PrinterProbe:
    def __init__(self, config, mcu_probe):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.mcu_probe = mcu_probe
        self.speed = config.getfloat('speed', 5.0)
        self.x_offset = config.getfloat('x_offset', 0.)
        self.y_offset = config.getfloat('y_offset', 0.)
        self.z_offset = config.getfloat('z_offset')
        self.probe_calibrate_z = 0.
        # Infer Z position to move to during a probe
        if config.has_section('stepper_z'):
            zconfig = config.getsection('stepper_z')
            self.z_position = zconfig.getfloat('position_min', 0.)
        else:
            pconfig = config.getsection('printer')
            self.z_position = pconfig.getfloat('minimum_z_position', 0.)
        # Multi-sample support (for improved accuracy)
        self.sample_count = config.getint('samples', 1, minval=1)
        self.sample_retract_dist = config.getfloat('sample_retract_dist', 2.,
                                                   above=0.)
        atypes = {'median': 'median', 'average': 'average'}
        self.samples_result = config.getchoice('samples_result', atypes,
                                               'average')
        self.samples_tolerance = config.getfloat('samples_tolerance', 0.100,
                                                 minval=0.)
        self.samples_retries = config.getint('samples_tolerance_retries', 0,
                                             minval=0)
        # Register z_virtual_endstop pin
        self.printer.lookup_object('pins').register_chip('probe', self)
        # Register PROBE/QUERY_PROBE commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('PROBE', self.cmd_PROBE,
                                    desc=self.cmd_PROBE_help)
        self.gcode.register_command('QUERY_PROBE', self.cmd_QUERY_PROBE,
                                    desc=self.cmd_QUERY_PROBE_help)
        self.gcode.register_command('PROBE_CALIBRATE', self.cmd_PROBE_CALIBRATE,
                                    desc=self.cmd_PROBE_CALIBRATE_help)
        self.gcode.register_command('PROBE_ACCURACY', self.cmd_PROBE_ACCURACY,
                                    desc=self.cmd_PROBE_ACCURACY_help)
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'z_virtual_endstop':
            raise pins.error("Probe virtual endstop only useful as endstop pin")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert probe virtual endstop")
        return self.mcu_probe
    def get_offsets(self):
        return self.x_offset, self.y_offset, self.z_offset
    def _probe(self, speed):
        toolhead = self.printer.lookup_object('toolhead')
        homing_state = homing.Homing(self.printer)
        pos = toolhead.get_position()
        pos[2] = self.z_position
        endstops = [(self.mcu_probe, "probe")]
        verify = self.printer.get_start_args().get('debugoutput') is None
        try:
            homing_state.homing_move(pos, endstops, speed,
                                     probe_pos=True, verify_movement=verify)
        except homing.CommandError as e:
            reason = str(e)
            if "Timeout during endstop homing" in reason:
                reason += HINT_TIMEOUT
            raise homing.CommandError(reason)
        pos = toolhead.get_position()
        self.gcode.respond_info("probe at %.3f,%.3f is z=%.6f" % (
            pos[0], pos[1], pos[2]))
        self.gcode.reset_last_position()
        return pos[:3]
    def _move(self, coord, speed):
        toolhead = self.printer.lookup_object('toolhead')
        curpos = toolhead.get_position()
        for i in range(len(coord)):
            if coord[i] is not None:
                curpos[i] = coord[i]
        toolhead.move(curpos, speed)
        self.gcode.reset_last_position()
    def _calc_mean(self, positions):
        count = float(len(positions))
        return [sum([pos[i] for pos in positions]) / count
                for i in range(3)]
    def _calc_median(self, positions):
        z_sorted = sorted(positions, key=(lambda p: p[2]))
        middle = len(positions) // 2
        if (len(positions) & 1) == 1:
            # odd number of samples
            return z_sorted[middle]
        # even number of samples
        return self._calc_mean(z_sorted[middle-1:middle+1])
    def run_probe(self, params={}):
        speed = self.gcode.get_float(
            "PROBE_SPEED", params, self.speed, above=0.)
        sample_count = self.gcode.get_int(
            "SAMPLES", params, self.sample_count, minval=1)
        sample_retract_dist = self.gcode.get_float(
            "SAMPLE_RETRACT_DIST", params, self.sample_retract_dist, above=0.)
        samples_tolerance = self.gcode.get_float(
            "SAMPLES_TOLERANCE", params, self.samples_tolerance, minval=0.)
        samples_retries = self.gcode.get_int(
            "SAMPLES_TOLERANCE_RETRIES", params, self.samples_retries, minval=0)
        samples_result = self.gcode.get_str(
            "SAMPLES_RESULT", params, self.samples_result)
        retries = 0
        positions = []
        while len(positions) < sample_count:
            # Probe position
            pos = self._probe(speed)
            positions.append(pos)
            # Check samples tolerance
            z_positions = [p[2] for p in positions]
            if max(z_positions) - min(z_positions) > samples_tolerance:
                if retries >= samples_retries:
                    raise homing.CommandError(
                        "Probe samples exceed samples_tolerance")
                self.gcode.respond_info(
                    "Probe samples exceed tolerance. Retrying...")
                retries += 1
                positions = []
            # Retract
            if len(positions) < sample_count:
                liftpos = [None, None, pos[2] + sample_retract_dist]
                self._move(liftpos, speed)
        # Calculate and return result
        if samples_result == 'median':
            return self._calc_median(positions)
        return self._calc_mean(positions)
    cmd_PROBE_help = "Probe Z-height at current XY position"
    def cmd_PROBE(self, params):
        pos = self.run_probe(params)
        self.gcode.respond_info("Result is z=%.6f" % (pos[2],))
    cmd_QUERY_PROBE_help = "Return the status of the z-probe"
    def cmd_QUERY_PROBE(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        res = self.mcu_probe.query_endstop(print_time)
        self.gcode.respond_info(
            "probe: %s" % (["open", "TRIGGERED"][not not res],))
    cmd_PROBE_ACCURACY_help = "Probe Z-height accuracy at current XY position"
    def cmd_PROBE_ACCURACY(self, params):
        speed = self.gcode.get_float(
            "PROBE_SPEED", params, self.speed, above=0.)
        sample_count = self.gcode.get_int("SAMPLES", params, 10, minval=1)
        sample_retract_dist = self.gcode.get_float(
            "SAMPLE_RETRACT_DIST", params, self.sample_retract_dist, above=0.)
        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        self.gcode.respond_info("PROBE_ACCURACY at X:%.3f Y:%.3f Z:%.3f"
                                " (samples=%d retract=%.3f speed=%.1f\n"
                                % (pos[0], pos[1], pos[2],
                                   sample_count, sample_retract_dist, speed))
        # Probe bed sample_count times
        positions = []
        while len(positions) < sample_count:
            # Probe position
            pos = self._probe(speed)
            positions.append(pos)
            # Retract
            liftpos = [None, None, pos[2] + sample_retract_dist]
            self._move(liftpos, speed)
        # Calculate maximum, minimum and average values
        max_value = max([p[2] for p in positions])
        min_value = min([p[2] for p in positions])
        range_value = max_value - min_value
        avg_value = self._calc_mean(positions)[2]
        median = self._calc_median(positions)[2]
        # calculate the standard deviation
        deviation_sum = 0
        for i in range(len(positions)):
            deviation_sum += pow(positions[i][2] - avg_value, 2.)
        sigma = (deviation_sum / len(positions)) ** 0.5
        # Show information
        self.gcode.respond_info(
            "probe accuracy results: maximum %.6f, minimum %.6f, range %.6f, "
            "average %.6f, median %.6f, standard deviation %.6f" % (
            max_value, min_value, range_value, avg_value, median, sigma))
    def probe_calibrate_finalize(self, kin_pos):
        if kin_pos is None:
            return
        z_offset = self.probe_calibrate_z - kin_pos[2]
        self.gcode.respond_info(
            "%s: z_offset: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer." % (self.name, z_offset))
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.name, 'z_offset', "%.3f" % (z_offset,))
    cmd_PROBE_CALIBRATE_help = "Calibrate the probe's z_offset"
    def cmd_PROBE_CALIBRATE(self, params):
        manual_probe.verify_no_manual_probe(self.printer)
        # Perform initial probe
        curpos = self.run_probe(params)
        # Move away from the bed
        self.probe_calibrate_z = curpos[2]
        curpos[2] += 5.
        self._move(curpos, self.speed)
        # Move the nozzle over the probe point
        curpos[0] += self.x_offset
        curpos[1] += self.y_offset
        self._move(curpos, self.speed)
        # Start manual probe
        manual_probe.ManualProbeHelper(self.printer, params,
                                       self.probe_calibrate_finalize)

# Endstop wrapper that enables probe specific features
class ProbeEndstopWrapper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.position_endstop = config.getfloat('z_offset')
        gcode_macro = self.printer.try_load_module(config, 'gcode_macro')
        self.activate_gcode = gcode_macro.load_template(
            config, 'activate_gcode', '')
        self.deactivate_gcode = gcode_macro.load_template(
            config, 'deactivate_gcode', '')
        # Create an "endstop" object to handle the probe pin
        ppins = self.printer.lookup_object('pins')
        pin = config.get('pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        mcu = pin_params['chip']
        mcu.register_config_callback(self._build_config)
        self.mcu_endstop = mcu.setup_pin('endstop', pin_params)
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        self.TimeoutError = self.mcu_endstop.TimeoutError
    def _build_config(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers('Z'):
            stepper.add_to_endstop(self)
    def home_prepare(self):
        toolhead = self.printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self.activate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise homing.CommandError(
                "Toolhead moved during probe activate_gcode script")
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        toolhead = self.printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self.deactivate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise homing.CommandError(
                "Toolhead moved during probe deactivate_gcode script")
        self.mcu_endstop.home_finalize()
    def get_position_endstop(self):
        return self.position_endstop

# Helper code that can probe a series of points and report the
# position at each point.
class ProbePointsHelper:
    def __init__(self, config, finalize_callback, default_points=None):
        self.printer = config.get_printer()
        self.finalize_callback = finalize_callback
        self.probe_points = default_points
        self.name = config.get_name()
        self.gcode = self.printer.lookup_object('gcode')
        # Read config settings
        if default_points is None or config.get('points', None) is not None:
            points = config.get('points').split('\n')
            try:
                points = [line.split(',', 1) for line in points if line.strip()]
                self.probe_points = [(float(p[0].strip()), float(p[1].strip()))
                                     for p in points]
            except:
                raise config.error("Unable to parse probe points in %s" % (
                    self.name))
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 5.)
        self.speed = config.getfloat('speed', 50., above=0.)
        # Internal probing state
        self.lift_speed = self.speed
        self.probe_offsets = (0., 0., 0.)
        self.results = []
    def minimum_points(self,n):
        if len(self.probe_points) < n:
            raise self.printer.config_error(
                "Need at least %d probe points for %s" % (n, self.name))
    def get_lift_speed(self):
        return self.lift_speed
    def _move_next(self):
        toolhead = self.printer.lookup_object('toolhead')
        # Lift toolhead
        speed = self.lift_speed
        if not self.results:
            # Use full speed to first probe position
            speed = self.speed
        curpos = toolhead.get_position()
        curpos[2] = self.horizontal_move_z
        toolhead.move(curpos, speed)
        # Check if done probing
        if len(self.results) >= len(self.probe_points):
            self.gcode.reset_last_position()
            toolhead.get_last_move_time()
            res = self.finalize_callback(self.probe_offsets, self.results)
            if res != "retry":
                return True
            self.results = []
        # Move to next XY probe point
        curpos[:2] = self.probe_points[len(self.results)]
        toolhead.move(curpos, self.speed)
        self.gcode.reset_last_position()
        return False
    def start_probe(self, params):
        manual_probe.verify_no_manual_probe(self.printer)
        # Lookup objects
        probe = self.printer.lookup_object('probe', None)
        method = self.gcode.get_str('METHOD', params, 'automatic').lower()
        self.results = []
        if probe is None or method != 'automatic':
            # Manual probe
            self.lift_speed = self.speed
            self.probe_offsets = (0., 0., 0.)
            self._manual_probe_start()
            return
        # Perform automatic probing
        self.lift_speed = min(self.speed, probe.speed)
        self.probe_offsets = probe.get_offsets()
        if self.horizontal_move_z < self.probe_offsets[2]:
            raise self.gcode.error("horizontal_move_z can't be less than"
                                   " probe's z_offset")
        while 1:
            done = self._move_next()
            if done:
                break
            pos = probe.run_probe(params)
            self.results.append(pos)
    def _manual_probe_start(self):
        done = self._move_next()
        if not done:
            manual_probe.ManualProbeHelper(self.printer, {},
                                           self._manual_probe_finalize)
    def _manual_probe_finalize(self, kin_pos):
        if kin_pos is None:
            return
        self.results.append(kin_pos)
        self._manual_probe_start()

def load_config(config):
    return PrinterProbe(config, ProbeEndstopWrapper(config))
