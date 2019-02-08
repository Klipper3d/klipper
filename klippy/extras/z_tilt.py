# Mechanical bed tilt calibration with multiple Z steppers
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import probe, mathutil

class ZTilt:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        z_positions = config.get('z_positions').split('\n')
        try:
            z_positions = [line.split(',', 1)
                           for line in z_positions if line.strip()]
            self.z_positions = [(float(zp[0].strip()), float(zp[1].strip()))
                                for zp in z_positions]
        except:
            raise config.error("Unable to parse z_positions in %s" % (
                config.get_name()))
        if len(z_positions) < 2:
            raise config.error("z_tilt requires at least two z_positions")
        self.probe_helper = probe.ProbePointsHelper(config, self.probe_finalize)
        self.z_steppers = []
        # Register Z_TILT_ADJUST command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'Z_TILT_ADJUST', self.cmd_Z_TILT_ADJUST,
            desc=self.cmd_Z_TILT_ADJUST_help)
    def handle_connect(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        z_steppers = kin.get_steppers('Z')
        if len(z_steppers) != len(self.z_positions):
            raise self.printer.config_error(
                "z_tilt z_positions needs exactly %d items" % (
                    len(z_steppers),))
        self.z_steppers = z_steppers
    cmd_Z_TILT_ADJUST_help = "Adjust the Z tilt"
    def cmd_Z_TILT_ADJUST(self, params):
        self.probe_helper.start_probe(params)
    def probe_finalize(self, offsets, positions):
        # Setup for coordinate descent analysis
        z_offset = offsets[2]
        logging.info("Calculating bed tilt with: %s", positions)
        params = { 'x_adjust': 0., 'y_adjust': 0., 'z_adjust': z_offset }
        # Perform coordinate descent
        def adjusted_height(pos, params):
            x, y, z = pos
            return (z - x*params['x_adjust'] - y*params['y_adjust']
                    - params['z_adjust'])
        def errorfunc(params):
            total_error = 0.
            for pos in positions:
                total_error += adjusted_height(pos, params)**2
            return total_error
        new_params = mathutil.coordinate_descent(
            params.keys(), params, errorfunc)
        # Apply results
        logging.info("Calculated bed tilt parameters: %s", new_params)
        x_adjust = new_params['x_adjust']
        y_adjust = new_params['y_adjust']
        z_adjust = (new_params['z_adjust'] - z_offset
                    - x_adjust * offsets[0] - y_adjust * offsets[1])
        try:
            self.adjust_steppers(x_adjust, y_adjust, z_adjust)
        except:
            logging.exception("z_tilt adjust_steppers")
            for s in self.z_steppers:
                s.set_ignore_move(False)
            raise
    def adjust_steppers(self, x_adjust, y_adjust, z_adjust):
        toolhead = self.printer.lookup_object('toolhead')
        curpos = toolhead.get_position()
        speed = self.probe_helper.get_lift_speed()
        # Find each stepper adjustment and disable all stepper movements
        positions = []
        for s, (x, y) in zip(self.z_steppers, self.z_positions):
            s.set_ignore_move(True)
            stepper_offset = -(x*x_adjust + y*y_adjust)
            positions.append((stepper_offset, s))
        # Report on movements
        stepstrs = ["%s = %.6f" % (s.get_name(), so) for so, s in positions]
        msg = "Making the following Z adjustments:\n%s\nz_adjust = %.6f" % (
            "\n".join(stepstrs), z_adjust)
        logging.info(msg)
        self.gcode.respond_info(msg)
        # Move each z stepper (sorted from lowest to highest) until they match
        positions.sort()
        first_stepper_offset, first_stepper = positions[0]
        z_low = curpos[2] - first_stepper_offset
        for i in range(len(positions)-1):
            stepper_offset, stepper = positions[i]
            next_stepper_offset, next_stepper = positions[i+1]
            stepper.set_ignore_move(False)
            curpos[2] = z_low + next_stepper_offset
            toolhead.move(curpos, speed)
            toolhead.set_position(curpos)
        # Z should now be level - do final cleanup
        last_stepper_offset, last_stepper = positions[-1]
        last_stepper.set_ignore_move(False)
        curpos[2] -= z_adjust
        toolhead.set_position(curpos)
        self.gcode.reset_last_position()

def load_config(config):
    return ZTilt(config)
