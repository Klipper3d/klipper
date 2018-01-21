# Bed tilt compensation
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import probe

class BedTilt:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.x_adjust = config.getfloat('x_adjust', 0.)
        self.y_adjust = config.getfloat('y_adjust', 0.)
        if config.get('points', None) is not None:
            BedTiltCalibrate(config, self)
        self.toolhead = None
        gcode = self.printer.lookup_object('gcode')
        gcode.set_move_transform(self)
    def printer_state(self, state):
        if state == 'connect':
            self.toolhead = self.printer.lookup_object('toolhead')
    def get_position(self):
        x, y, z, e = self.toolhead.get_position()
        return [x, y, z - x*self.x_adjust - y*self.y_adjust, e]
    def move(self, newpos, speed):
        x, y, z, e = newpos
        self.toolhead.move([x, y, z + x*self.x_adjust + y*self.y_adjust, e],
                           speed)

# Helper script to calibrate the bed tilt
class BedTiltCalibrate:
    def __init__(self, config, bedtilt):
        self.bedtilt = bedtilt
        self.printer = config.get_printer()
        points = config.get('points').split('\n')
        try:
            points = [line.split(',', 1) for line in points if line.strip()]
            self.points = [(float(p[0].strip()), float(p[1].strip()))
                           for p in points]
        except:
            raise config.error("Unable to parse bed tilt points")
        self.speed = config.getfloat('speed', 50., above=0.)
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 5.)
        self.probe_z_offset = config.getfloat('probe_z_offset', 0.)
        self.manual_probe = config.getboolean('manual_probe', None)
        if self.manual_probe is None:
            self.manual_probe = not config.has_section('probe')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'BED_TILT_CALIBRATE', self.cmd_BED_TILT_CALIBRATE,
            desc=self.cmd_BED_TILT_CALIBRATE_help)
    cmd_BED_TILT_CALIBRATE_help = "Bed tilt calibration script"
    def cmd_BED_TILT_CALIBRATE(self, params):
        self.gcode.run_script("G28")
        probe.ProbePointsHelper(
            self.printer, self.points, self.horizontal_move_z,
            self.speed, self.manual_probe, self)
    def get_position(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        return kin.get_position()
    def finalize(self, positions):
        logging.debug("Got: %s", positions)
        params = { 'x_adjust': self.bedtilt.x_adjust,
                   'y_adjust': self.bedtilt.y_adjust,
                   'z_adjust': self.probe_z_offset }
        logging.debug("Params: %s", params)
        def adjusted_height(pos, params):
            x, y, z = pos
            return (z + x*params['x_adjust'] + y*params['y_adjust']
                    - params['z_adjust'])
        def errorfunc(params):
            total_error = 0.
            for pos in positions:
                total_error += adjusted_height(pos, params)**2
            return total_error
        new_params = probe.coordinate_descent(params.keys(), params, errorfunc)
        logging.debug("Got2: %s", new_params)
        for pos in positions:
            logging.debug("orig: %s new: %s",
                          adjusted_height(pos, params),
                          adjusted_height(pos, new_params))
        z_warn = ""
        z_diff = new_params['z_adjust'] - self.probe_z_offset
        if abs(z_diff) > .010:
            z_warn = "Note: Z offset was %.6f\n" % (z_diff,)
        self.gcode.respond_info(
            "%sx_adjust: %.6f y_adjust: %.6f\n"
            "To use these parameters, update the printer config file with\n"
            "the above and then issue a RESTART command" % (
                z_warn, new_params['x_adjust'], new_params['y_adjust']))

def load_config(config):
    if config.get_name() != 'bed_tilt':
        raise config.error("Invalid bed_tilt config name")
    return BedTilt(config)
