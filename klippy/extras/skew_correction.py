# Printer Skew Correction
#
# This implementation is a port of Marlin's skew correction as
# implemented in planner.h, Copyright (C) Marlin Firmware
#
# https://github.com/MarlinFirmware/Marlin/tree/1.1.x/Marlin
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import math

def calc_skew_factor(ac, bd, ad):
    side = math.sqrt(2*ac*ac + 2*bd*bd - 4*ad*ad) / 2.
    return math.tan(math.pi/2 - math.acos(
        (ac*ac - side*side - ad*ad) / (2*side*ad)))

class PrinterSkew:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.toolhead = None
        self.xy_factor = 0.
        self.xz_factor = 0.
        self.yz_factor = 0.
        self.printer.register_event_handler("klippy:ready",
                                            self._handle_ready)
        self.next_transform = None
        self.gcode.register_command(
            'GET_CURRENT_SKEW', self.cmd_GET_CURRENT_SKEW,
            desc=self.cmd_GET_CURRENT_SKEW_help)
        self.gcode.register_command(
            'CALC_MEASURED_SKEW', self.cmd_CALC_MEASURED_SKEW,
            desc=self.cmd_CALC_MEASURED_SKEW_help)
        self.gcode.register_command(
            'SET_SKEW', self.cmd_SET_SKEW,
            desc=self.cmd_SET_SKEW_help)
    def _handle_ready(self):
        self.next_transform = self.gcode.set_move_transform(self, force=True)
    def calc_skew(self, pos):
        skewed_x = pos[0] - pos[1] * self.xy_factor \
            - pos[2] * (self.xz_factor - (self.xy_factor * self.yz_factor))
        skewed_y = pos[1] - pos[2] * self.yz_factor
        return [skewed_x, skewed_y, pos[2], pos[3]]
    def calc_unskew(self, pos):
        skewed_x = pos[0] + pos[1] * self.xy_factor \
            + pos[2] * self.xz_factor
        skewed_y = pos[1] + pos[2] * self.yz_factor
        return [skewed_x, skewed_y, pos[2], pos[3]]
    def get_position(self):
        return self.calc_unskew(self.next_transform.get_position())
    def move(self, newpos, speed):
        corrected_pos = self.calc_skew(newpos)
        self.next_transform.move(corrected_pos, speed)
    cmd_GET_CURRENT_SKEW_help = "Report current printer skew"
    def cmd_GET_CURRENT_SKEW(self, params):
        out = "Current Printer Skew:"
        planes = ["XY", "XZ", "YZ"]
        factors = [self.xy_factor, self.xz_factor, self.yz_factor]
        for plane, fac in zip(planes, factors):
            out += '\n' + plane
            out += " Skew: %.6f radians, %.2f degrees" % (
                fac, math.degrees(fac))
        self.gcode.respond_info(out)
    cmd_CALC_MEASURED_SKEW_help = "Calculate skew from measured print"
    def cmd_CALC_MEASURED_SKEW(self, params):
        ac = self.gcode.get_float("AC", params, above=0.)
        bd = self.gcode.get_float("BD", params, above=0.)
        ad = self.gcode.get_float("AD", params, above=0.)
        factor = calc_skew_factor(ac, bd, ad)
        self.gcode.respond_info(
            "Calculated Skew: %.6f radians, %.2f degrees" %
            (factor, math.degrees(factor)))
    cmd_SET_SKEW_help = "Set skew based on lengths of measured object"
    def cmd_SET_SKEW(self, params):
        if self.gcode.get_int("CLEAR", params, 0):
            self.xy_factor = 0.
            self.xz_factor = 0.
            self.yz_factor = 0.
            return
        planes = ["XY", "XZ", "YZ"]
        for plane in planes:
            lengths = self.gcode.get_str(plane, params, None)
            if lengths is not None:
                try:
                    lengths = lengths.strip().split(",", 2)
                    lengths = [float(l.strip()) for l in lengths]
                    if len(lengths) != 3:
                        raise Exception
                except Exception:
                    raise self.gcode.error(
                        "skew_correction: improperly formatted entry for "
                        "plane [%s]\n%s" % (plane, params['#original']))
                factor = plane.lower() + '_factor'
                setattr(self, factor, calc_skew_factor(*lengths))

def load_config(config):
    return PrinterSkew(config)
