# Z-Probe support
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins, homing

HINT_TIMEOUT = """
Make sure to home the printer before probing. If the probe
did not move far enough to trigger, then consider reducing
the Z axis minimum position so the probe can travel further
(the Z minimum position can be negative).
"""

class PrinterProbe:
    def __init__(self, config, mcu_probe):
        self.printer = config.get_printer()
        self.mcu_probe = mcu_probe
        self.speed = config.getfloat('speed', 5.0)
        self.x_offset = config.getfloat('x_offset', 0.)
        self.y_offset = config.getfloat('y_offset', 0.)
        self.z_offset = config.getfloat('z_offset')
        # Infer Z position to move to during a probe
        if config.has_section('stepper_z'):
            zconfig = config.getsection('stepper_z')
            self.z_position = zconfig.getfloat('position_min', 0.)
        else:
            pconfig = config.getsection('printer')
            self.z_position = pconfig.getfloat('minimum_z_position', 0.)
        # Register z_virtual_endstop pin
        self.printer.lookup_object('pins').register_chip('probe', self)
        # Register PROBE/QUERY_PROBE commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'PROBE', self.cmd_PROBE, desc=self.cmd_PROBE_help)
        self.gcode.register_command(
            'QUERY_PROBE', self.cmd_QUERY_PROBE, desc=self.cmd_QUERY_PROBE_help)
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'z_virtual_endstop':
            raise pins.error("Probe virtual endstop only useful as endstop pin")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert probe virtual endstop")
        return self.mcu_probe
    def get_offsets(self):
        return self.x_offset, self.y_offset, self.z_offset
    cmd_PROBE_help = "Probe Z-height at current XY position"
    def cmd_PROBE(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        homing_state = homing.Homing(self.printer)
        pos = toolhead.get_position()
        pos[2] = self.z_position
        endstops = [(self.mcu_probe, "probe")]
        try:
            homing_state.homing_move(pos, endstops, self.speed,
                                     probe_pos=True, verify_movement=True)
        except homing.EndstopError as e:
            reason = str(e)
            if "Timeout during endstop homing" in reason:
                reason += HINT_TIMEOUT
            raise self.gcode.error(reason)
        pos = toolhead.get_position()
        self.gcode.respond_info("probe at %.3f,%.3f is z=%.6f" % (
            pos[0], pos[1], pos[2]))
        self.gcode.reset_last_position()
    cmd_QUERY_PROBE_help = "Return the status of the z-probe"
    def cmd_QUERY_PROBE(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        self.mcu_probe.query_endstop(print_time)
        res = self.mcu_probe.query_endstop_wait()
        self.gcode.respond_info(
            "probe: %s" % (["open", "TRIGGERED"][not not res],))

# Endstop wrapper that enables probe specific features
class ProbeEndstopWrapper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.position_endstop = config.getfloat('z_offset')
        self.activate_gcode = config.get('activate_gcode', None)
        self.deactivate_gcode = config.get('deactivate_gcode', None)
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
        self.query_endstop_wait = self.mcu_endstop.query_endstop_wait
        self.TimeoutError = self.mcu_endstop.TimeoutError
    def _build_config(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers('Z'):
            stepper.add_to_endstop(self)
    def home_prepare(self):
        if self.activate_gcode is not None:
            gcode = self.printer.lookup_object('gcode')
            gcode.run_script_from_command(self.activate_gcode)
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        if self.deactivate_gcode is not None:
            gcode = self.printer.lookup_object('gcode')
            gcode.run_script_from_command(self.deactivate_gcode)
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
        # Read config settings
        if default_points is None or config.get('points', None) is not None:
            points = config.get('points').split('\n')
            try:
                points = [line.split(',', 1) for line in points if line.strip()]
                self.probe_points = [(float(p[0].strip()), float(p[1].strip()))
                                     for p in points]
            except:
                raise config.error("Unable to parse probe points in %s" % (
                    config.get_name()))
        if len(self.probe_points) < 3:
            raise config.error("Need at least 3 probe points for %s" % (
                config.get_name()))
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 5.)
        self.speed = self.lift_speed = config.getfloat('speed', 50., above=0.)
        self.probe_offsets = (0., 0., 0.)
        self.samples = config.getint('samples', 1, minval=1)
        self.sample_retract_dist = config.getfloat(
            'sample_retract_dist', 2., above=0.)
        # Internal probing state
        self.results = []
        self.busy = False
        self.gcode = self.toolhead = None
    def get_lift_speed(self):
        return self.lift_speed
    def _lift_z(self, z_pos, add=False, speed=None):
        # Lift toolhead
        curpos = self.toolhead.get_position()
        if add:
            curpos[2] += z_pos
        else:
            curpos[2] = z_pos
        if speed is None:
            speed = self.lift_speed
        try:
            self.toolhead.move(curpos, speed)
        except homing.EndstopError as e:
            self._finalize(False)
            raise self.gcode.error(str(e))
    def _move_next(self):
        # Lift toolhead
        self._lift_z(self.horizontal_move_z)
        # Check if done probing
        if len(self.results) >= len(self.probe_points):
            self.toolhead.get_last_move_time()
            self._finalize(True)
            return
        # Move to next XY probe point
        x, y = self.probe_points[len(self.results)]
        curpos = self.toolhead.get_position()
        curpos[0] = x
        curpos[1] = y
        curpos[2] = self.horizontal_move_z
        try:
            self.toolhead.move(curpos, self.speed)
        except homing.EndstopError as e:
            self._finalize(False)
            raise self.gcode.error(str(e))
        self.gcode.reset_last_position()
    def _automatic_probe_point(self):
        positions = []
        for i in range(self.samples):
            try:
                self.gcode.run_script_from_command("PROBE")
            except self.gcode.error as e:
                self._finalize(False)
                raise
            positions.append(self.toolhead.get_position())
            if i < self.samples - 1:
                # retract
                self._lift_z(self.sample_retract_dist, add=True)
        avg_pos = [sum([pos[i] for pos in positions]) / self.samples
                   for i in range(3)]
        self.results.append(avg_pos)
    def start_probe(self, params):
        # Lookup objects
        self.toolhead = self.printer.lookup_object('toolhead')
        self.gcode = self.printer.lookup_object('gcode')
        probe = self.printer.lookup_object('probe', None)
        method = self.gcode.get_str('METHOD', params, 'automatic').lower()
        if probe is not None and method == 'automatic':
            self.lift_speed = min(self.speed, probe.speed)
            self.probe_offsets = probe.get_offsets()
            if self.horizontal_move_z < self.probe_offsets[2]:
                raise self.gcode.error("horizontal_move_z can't be less than"
                                       " probe's z_offset")
        else:
            probe = None
            self.lift_speed = self.speed
            self.probe_offsets = (0., 0., 0.)
        # Start probe
        self.results = []
        self.busy = True
        self._lift_z(self.horizontal_move_z, speed=self.speed)
        self._move_next()
        if probe is None:
            # Setup for manual probing
            self.gcode.register_command('NEXT', None)
            self.gcode.register_command('NEXT', self.cmd_NEXT,
                                        desc=self.cmd_NEXT_help)
        else:
            # Perform automatic probing
            while self.busy:
                self._automatic_probe_point()
                self._move_next()
    cmd_NEXT_help = "Move to the next XY position to probe"
    def cmd_NEXT(self, params):
        # Record current position for manual probe
        self.toolhead.get_last_move_time()
        self.results.append(self.toolhead.get_kinematics().calc_position())
        # Move to next position
        self._move_next()
    def _finalize(self, success):
        self.busy = False
        self.gcode.reset_last_position()
        self.gcode.register_command('NEXT', None)
        if success:
            self.finalize_callback(self.probe_offsets, self.results)

def load_config(config):
    return PrinterProbe(config, ProbeEndstopWrapper(config))
