# Mechanical bed tilt calibration with multiple Z steppers
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import probe, mathutil

class ZAdjustHelper:
    def __init__(self, config, z_count):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.z_count = z_count
        self.z_steppers = []
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def handle_connect(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        z_steppers = kin.get_steppers('Z')
        if len(z_steppers) != self.z_count:
            raise self.printer.config_error(
                "%s z_positions needs exactly %d items" % (
                    self.name, len(z_steppers)))
        if len(z_steppers) < 2:
            raise self.printer.config_error(
                "%s requires multiple z steppers" % (self.name,))
        self.z_steppers = z_steppers
    def adjust_steppers(self, adjustments, speed):
        toolhead = self.printer.lookup_object('toolhead')
        gcode = self.printer.lookup_object('gcode')
        curpos = toolhead.get_position()
        # Report on movements
        stepstrs = ["%s = %.6f" % (s.get_name(), a)
                    for s, a in zip(self.z_steppers, adjustments)]
        msg = "Making the following Z adjustments:\n%s" % ("\n".join(stepstrs),)
        gcode.respond_info(msg)
        # Disable Z stepper movements
        for s in self.z_steppers:
            s.set_trapq(None)
        # Move each z stepper (sorted from lowest to highest) until they match
        positions = [(-a, s) for a, s in zip(adjustments, self.z_steppers)]
        positions.sort()
        first_stepper_offset, first_stepper = positions[0]
        z_low = curpos[2] - first_stepper_offset
        for i in range(len(positions)-1):
            stepper_offset, stepper = positions[i]
            next_stepper_offset, next_stepper = positions[i+1]
            stepper.set_trapq(toolhead.get_trapq())
            curpos[2] = z_low + next_stepper_offset
            try:
                toolhead.move(curpos, speed)
                toolhead.set_position(curpos)
            except:
                logging.exception("ZAdjustHelper adjust_steppers")
                for s in self.z_steppers:
                    s.set_trapq(toolhead.get_trapq())
                raise
        # Z should now be level - do final cleanup
        last_stepper_offset, last_stepper = positions[-1]
        last_stepper.set_trapq(toolhead.get_trapq())
        curpos[2] += first_stepper_offset
        toolhead.set_position(curpos)
        gcode.reset_last_position()

class RetryHelper:
    def __init__(self, config, error_msg_extra = ""):
        self.gcode = config.get_printer().lookup_object('gcode')
        self.default_max_retries = config.getint("retries", 0, minval=0)
        self.default_retry_tolerance = \
            config.getfloat("retry_tolerance", 0., above=0.)
        self.value_label = "Probed points range"
        self.error_msg_extra = error_msg_extra
    def start(self, params):
        self.max_retries = self.gcode.get_int('RETRIES', params,
            default=self.default_max_retries, minval=0, maxval=30)
        self.retry_tolerance = self.gcode.get_float('RETRY_TOLERANCE', params,
            default=self.default_retry_tolerance, minval=0, maxval=1.0)
        self.current_retry = 0
        self.previous = None
        self.increasing = 0
    def check_increase(self,error):
        if self.previous and error > self.previous + 0.0000001:
            self.increasing += 1
        elif self.increasing > 0:
            self.increasing -= 1
        self.previous = error
        return self.increasing > 1
    def check_retry(self,z_positions):
        if self.max_retries == 0:
            return
        error = max(z_positions) - min(z_positions)
        if self.check_increase(error):
            self.gcode.respond_error(
                "Retries aborting: %s is increasing. %s" % (
                    self.value_label, self.error_msg_extra))
            return
        self.gcode.respond_info(
            "Retries: %d/%d %s: %0.6f tolerance: %0.6f" % (
                self.current_retry, self.max_retries, self.value_label,
                error, self.retry_tolerance))
        if error <= self.retry_tolerance:
            return "done"
        self.current_retry += 1
        if self.current_retry > self.max_retries:
            self.gcode.respond_error("Too many retries")
            return
        return "retry"

class ZTilt:
    def __init__(self, config):
        self.printer = config.get_printer()
        z_positions = config.get('z_positions').split('\n')
        try:
            z_positions = [line.split(',', 1)
                           for line in z_positions if line.strip()]
            self.z_positions = [(float(zp[0].strip()), float(zp[1].strip()))
                                for zp in z_positions]
        except:
            raise config.error("Unable to parse z_positions in %s" % (
                config.get_name()))
        self.retry_helper = RetryHelper(config)
        self.probe_helper = probe.ProbePointsHelper(config, self.probe_finalize)
        self.probe_helper.minimum_points(2)
        self.z_helper = ZAdjustHelper(config, len(self.z_positions))
        # Register Z_TILT_ADJUST command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('Z_TILT_ADJUST', self.cmd_Z_TILT_ADJUST,
                                    desc=self.cmd_Z_TILT_ADJUST_help)
    cmd_Z_TILT_ADJUST_help = "Adjust the Z tilt"
    def cmd_Z_TILT_ADJUST(self, params):
        self.retry_helper.start(params)
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
        speed = self.probe_helper.get_lift_speed()
        logging.info("Calculated bed tilt parameters: %s", new_params)
        x_adjust = new_params['x_adjust']
        y_adjust = new_params['y_adjust']
        z_adjust = (new_params['z_adjust'] - z_offset
                    - x_adjust * offsets[0] - y_adjust * offsets[1])
        adjustments = [x*x_adjust + y*y_adjust + z_adjust
                       for x, y in self.z_positions]
        self.z_helper.adjust_steppers(adjustments, speed)
        return self.retry_helper.check_retry([p[2] for p in positions])

def load_config(config):
    return ZTilt(config)
