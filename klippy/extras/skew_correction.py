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

def get_skew_from_plane(plane, config):
    ac = config.getfloat(plane + "_ac_length", None, above=0.)
    if ac is None:
        return 0.
    bd = config.getfloat(plane + "_bd_length", above=0.)
    ad = config.getfloat(plane + "_ad_length", above=0.)
    return calc_skew_factor(ac, bd, ad)

class PrinterSkew:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.toolhead = None
        self.xy_factor = get_skew_from_plane("xy", config)
        self.xz_factor = get_skew_from_plane("xz", config)
        self.yz_factor = get_skew_from_plane("yz", config)
        self.printer.register_event_handler("klippy:ready",
                                            self._handle_ready)
        self.next_transform = None
        self.gcode.register_command(
            'GET_CURRENT_SKEW', self.cmd_GET_CURRENT_SKEW,
            desc=self.cmd_GET_CURRENT_SKEW_help)
        self.gcode.register_command(
            'CALC_MEASURED_SKEW', self.cmd_CALC_MEASURED_SKEW,
            desc=self.cmd_CALC_MEASURED_SKEW_help)
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

def load_config(config):
    return PrinterSkew(config)
