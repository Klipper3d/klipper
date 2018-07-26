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
    def __init__(self, config):
        self.printer = config.get_printer()
        self.speed = config.getfloat('speed', 5.0)
        self.z_offset = config.getfloat('z_offset')
        # Infer Z position to move to during a probe
        if config.has_section('stepper_z'):
            zconfig = config.getsection('stepper_z')
            self.z_position = zconfig.getfloat('position_min', 0.)
        else:
            pconfig = config.getsection('printer')
            self.z_position = pconfig.getfloat('minimum_z_position', 0.)
        # Create an "endstop" object to handle the probe pin
        ppins = self.printer.lookup_object('pins')
        pin = config.get('pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        mcu = pin_params['chip']
        mcu.add_config_object(self)
        self.mcu_probe = mcu.setup_pin('endstop', pin_params)
        if (config.get('activate_gcode', None) is not None or
            config.get('deactivate_gcode', None) is not None):
            self.mcu_probe = ProbeEndstopWrapper(config, self.mcu_probe)
        # Create z_virtual_endstop pin
        ppins.register_chip('probe', self)
        self.z_virtual_endstop = None
        # Register PROBE/QUERY_PROBE commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'PROBE', self.cmd_PROBE, desc=self.cmd_PROBE_help)
        self.gcode.register_command(
            'QUERY_PROBE', self.cmd_QUERY_PROBE, desc=self.cmd_QUERY_PROBE_help)
    def build_config(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers('Z'):
            stepper.add_to_endstop(self.mcu_probe)
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'z_virtual_endstop':
            raise pins.error("Probe virtual endstop only useful as endstop pin")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert probe virtual endstop")
        self.z_virtual_endstop = ProbeVirtualEndstop(
            self.printer, self.mcu_probe)
        return self.z_virtual_endstop
    def last_home_position(self):
        if self.z_virtual_endstop is None:
            return None
        return self.z_virtual_endstop.position
    cmd_PROBE_help = "Probe Z-height at current XY position"
    def cmd_PROBE(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        homing_state = homing.Homing(toolhead)
        pos = toolhead.get_position()
        pos[2] = self.z_position
        try:
            homing_state.homing_move(
                pos, [(self.mcu_probe, "probe")], self.speed, probe_pos=True)
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

# Endstop wrapper that enables running g-code scripts on setup
class ProbeEndstopWrapper:
    def __init__(self, config, mcu_endstop):
        self.mcu_endstop = mcu_endstop
        self.gcode = config.get_printer().lookup_object('gcode')
        self.activate_gcode = config.get('activate_gcode', "")
        self.deactivate_gcode = config.get('deactivate_gcode', "")
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        self.query_endstop_wait = self.mcu_endstop.query_endstop_wait
        self.TimeoutError = self.mcu_endstop.TimeoutError
    def home_prepare(self):
        self.gcode.run_script_from_command(self.activate_gcode)
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        self.gcode.run_script_from_command(self.deactivate_gcode)
        self.mcu_endstop.home_finalize()

# Wrapper that records the last XY position of a virtual endstop probe
class ProbeVirtualEndstop:
    def __init__(self, printer, mcu_endstop):
        self.printer = printer
        self.mcu_endstop = mcu_endstop
        self.position = None
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        self.query_endstop_wait = self.mcu_endstop.query_endstop_wait
        self.home_prepare = self.mcu_endstop.home_prepare
        self.TimeoutError = self.mcu_endstop.TimeoutError
    def home_finalize(self):
        self.position = self.printer.lookup_object('toolhead').get_position()
        self.mcu_endstop.home_finalize()

# Helper code that can probe a series of points and report the
# position at each point.
class ProbePointsHelper:
    def __init__(self, config, callback, default_points=None):
        self.printer = config.get_printer()
        self.callback = callback
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
        # Lookup probe object
        self.probe = None
        self.probe_z_offset = 0.
        manual_probe = config.getboolean('manual_probe', None)
        if manual_probe is None:
            manual_probe = not config.has_section('probe')
        if not manual_probe:
            self.printer.try_load_module(config, 'probe')
            self.probe = self.printer.lookup_object('probe')
            self.lift_speed = min(self.speed, self.probe.speed)
            self.probe_z_offset = self.probe.z_offset
            if self.horizontal_move_z < self.probe_z_offset:
                raise config.error("horizontal_move_z can't be less than probe's"
                                   " z_offset in %s" % (config.get_name()))
        # Internal probing state
        self.results = []
        self.busy = False
        self.gcode = self.toolhead = None
    def get_lift_speed(self):
        return self.lift_speed
    def start_probe(self):
        # Begin probing
        self.toolhead = self.printer.lookup_object('toolhead')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'NEXT', self.cmd_NEXT, desc=self.cmd_NEXT_help)
        self.results = []
        self.busy = True
        self.move_next()
        if self.probe is not None:
            try:
                while self.busy:
                    self.gcode.run_script_from_command("PROBE")
                    self.cmd_NEXT({})
            except:
                self.finalize(False)
                raise
    def move_next(self):
        x, y = self.probe_points[len(self.results)]
        curpos = self.toolhead.get_position()
        curpos[0] = x
        curpos[1] = y
        curpos[2] = self.horizontal_move_z
        try:
            self.toolhead.move(curpos, self.speed)
        except homing.EndstopError as e:
            self.finalize(False)
            raise self.gcode.error(str(e))
        self.gcode.reset_last_position()
    cmd_NEXT_help = "Move to the next XY position to probe"
    def cmd_NEXT(self, params):
        # Record current position
        self.toolhead.wait_moves()
        self.results.append(self.callback.get_probed_position())
        # Lift toolhead
        curpos = self.toolhead.get_position()
        curpos[2] = self.horizontal_move_z
        try:
            self.toolhead.move(curpos, self.lift_speed)
        except homing.EndstopError as e:
            self.finalize(False)
            raise self.gcode.error(str(e))
        # Move to next position
        if len(self.results) == len(self.probe_points):
            self.toolhead.get_last_move_time()
            self.finalize(True)
            return
        self.move_next()
    def finalize(self, success):
        self.busy = False
        self.gcode.reset_last_position()
        self.gcode.register_command('NEXT', None)
        if success:
            self.callback.finalize(self.probe_z_offset, self.results)

def load_config(config):
    return PrinterProbe(config)
