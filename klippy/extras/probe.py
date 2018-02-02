# Z-Probe support
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins, homing

class PrinterProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.speed = config.getfloat('speed', 5.0)
        self.z_position = config.getfloat('z_position', 0.)
        # Create an "endstop" object to handle the probe pin
        ppins = self.printer.lookup_object('pins')
        pin_params = ppins.lookup_pin('endstop', config.get('pin'))
        mcu = pin_params['chip']
        mcu.add_config_object(self)
        self.mcu_probe = mcu.setup_pin(pin_params)
        if (config.get('activate_gcode', None) is not None or
            config.get('deactivate_gcode', None) is not None):
            self.mcu_probe = ProbeEndstopWrapper(config, self.mcu_probe)
        # Create z_virtual_endstop pin
        ppins.register_chip('probe', self)
        # Register PROBE/QUERY_PROBE commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'PROBE', self.cmd_PROBE, desc=self.cmd_PROBE_help)
        self.gcode.register_command(
            'QUERY_PROBE', self.cmd_QUERY_PROBE, desc=self.cmd_QUERY_PROBE_help)
    def build_config(self):
        toolhead = self.printer.lookup_object('toolhead')
        z_steppers = toolhead.get_kinematics().get_steppers("Z")
        for s in z_steppers:
            for mcu_endstop, name in s.get_endstops():
                for mcu_stepper in mcu_endstop.get_steppers():
                    self.mcu_probe.add_stepper(mcu_stepper)
    def setup_pin(self, pin_params):
        if (pin_params['pin'] != 'z_virtual_endstop'
            or pin_params['type'] != 'endstop'):
            raise pins.error("Probe virtual endstop only useful as endstop pin")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert probe virtual endstop")
        return self.mcu_probe
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
            raise self.gcode.error(str(e))
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
        self.gcode.run_script(self.activate_gcode)
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        self.gcode.run_script(self.deactivate_gcode)
        self.mcu_endstop.home_finalize()

# Helper code that can probe a series of points and report the
# position at each point.
class ProbePointsHelper:
    def __init__(self, printer, probe_points, horizontal_move_z, speed,
                 manual_probe, callback):
        self.printer = printer
        self.probe_points = probe_points
        self.horizontal_move_z = horizontal_move_z
        self.speed = speed
        self.callback = callback
        self.toolhead = self.printer.lookup_object('toolhead')
        self.results = []
        self.busy = True
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'NEXT', self.cmd_NEXT, desc=self.cmd_NEXT_help)
        # Begin probing
        self.move_next()
        if not manual_probe:
            while self.busy:
                self.gcode.run_script("PROBE")
                self.cmd_NEXT({})
    def move_next(self):
        x, y = self.probe_points[len(self.results)]
        curpos = self.toolhead.get_position()
        curpos[0] = x
        curpos[1] = y
        curpos[2] = self.horizontal_move_z
        self.toolhead.move(curpos, self.speed)
        self.gcode.reset_last_position()
    cmd_NEXT_help = "Move to the next XY position to probe"
    def cmd_NEXT(self, params):
        # Record current position
        self.toolhead.wait_moves()
        self.results.append(self.callback.get_position())
        # Move to next position
        curpos = self.toolhead.get_position()
        curpos[2] = self.horizontal_move_z
        self.toolhead.move(curpos, self.speed)
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
            self.callback.finalize(self.results)

# Helper code that implements coordinate descent
def coordinate_descent(adj_params, params, error_func):
    # Define potential changes
    params = dict(params)
    dp = {param_name: 1. for param_name in adj_params}
    # Calculate the error
    best_err = error_func(params)

    threshold = 0.00001
    rounds = 0

    while sum(dp.values()) > threshold and rounds < 10000:
        rounds += 1
        for param_name in adj_params:
            orig = params[param_name]
            params[param_name] = orig + dp[param_name]
            err = error_func(params)
            if err < best_err:
                # There was some improvement
                best_err = err
                dp[param_name] *= 1.1
                continue
            params[param_name] = orig - dp[param_name]
            err = error_func(params)
            if err < best_err:
                # There was some improvement
                best_err = err
                dp[param_name] *= 1.1
                continue
            params[param_name] = orig
            dp[param_name] *= 0.9
    logging.debug("best_err: %s  rounds: %d", best_err, rounds)
    return params

def load_config(config):
    if config.get_name() != 'probe':
        raise config.error("Invalid probe config name")
    return PrinterProbe(config)
