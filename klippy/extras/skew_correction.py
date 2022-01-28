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
        self.name = config.get_name()
        self.toolhead = None
        self.xy_factor = 0.
        self.xz_factor = 0.
        self.yz_factor = 0.
        self.skew_profiles = {}
        self._load_storage(config)
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        self.next_transform = None
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('GET_CURRENT_SKEW', self.cmd_GET_CURRENT_SKEW,
                               desc=self.cmd_GET_CURRENT_SKEW_help)
        gcode.register_command('CALC_MEASURED_SKEW',
                               self.cmd_CALC_MEASURED_SKEW,
                               desc=self.cmd_CALC_MEASURED_SKEW_help)
        gcode.register_command('SET_SKEW', self.cmd_SET_SKEW,
                               desc=self.cmd_SET_SKEW_help)
        gcode.register_command('SKEW_PROFILE', self.cmd_SKEW_PROFILE,
                               desc=self.cmd_SKEW_PROFILE_help)
    def _handle_connect(self):
        gcode_move = self.printer.lookup_object('gcode_move')
        self.next_transform = gcode_move.set_move_transform(self, force=True)
    def _load_storage(self, config):
        stored_profs = config.get_prefix_sections(self.name)
        # Remove primary skew_correction section, as it is not a stored profile
        stored_profs = [s for s in stored_profs
                        if s.get_name() != self.name]
        for profile in stored_profs:
            name = profile.get_name().split(' ', 1)[1]
            self.skew_profiles[name] = {
                'xy_skew': profile.getfloat("xy_skew"),
                'xz_skew': profile.getfloat("xz_skew"),
                'yz_skew': profile.getfloat("yz_skew"),
            }
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
    def _update_skew(self, xy_factor, xz_factor, yz_factor):
        self.xy_factor = xy_factor
        self.xz_factor = xz_factor
        self.yz_factor = yz_factor
        gcode_move = self.printer.lookup_object('gcode_move')
        gcode_move.reset_last_position()
    cmd_GET_CURRENT_SKEW_help = "Report current printer skew"
    def cmd_GET_CURRENT_SKEW(self, gcmd):
        out = "Current Printer Skew:"
        planes = ["XY", "XZ", "YZ"]
        factors = [self.xy_factor, self.xz_factor, self.yz_factor]
        for plane, fac in zip(planes, factors):
            out += '\n' + plane
            out += " Skew: %.6f radians, %.2f degrees" % (
                fac, math.degrees(fac))
        gcmd.respond_info(out)
    cmd_CALC_MEASURED_SKEW_help = "Calculate skew from measured print"
    def cmd_CALC_MEASURED_SKEW(self, gcmd):
        ac = gcmd.get_float("AC", above=0.)
        bd = gcmd.get_float("BD", above=0.)
        ad = gcmd.get_float("AD", above=0.)
        factor = calc_skew_factor(ac, bd, ad)
        gcmd.respond_info("Calculated Skew: %.6f radians, %.2f degrees"
                          % (factor, math.degrees(factor)))
    cmd_SET_SKEW_help = "Set skew based on lengths of measured object"
    def cmd_SET_SKEW(self, gcmd):
        if gcmd.get_int("CLEAR", 0):
            self._update_skew(0., 0., 0.)
            return
        planes = ["XY", "XZ", "YZ"]
        for plane in planes:
            lengths = gcmd.get(plane, None)
            if lengths is not None:
                try:
                    lengths = lengths.strip().split(",", 2)
                    lengths = [float(l.strip()) for l in lengths]
                    if len(lengths) != 3:
                        raise Exception
                except Exception:
                    raise gcmd.error(
                        "skew_correction: improperly formatted entry for "
                        "plane [%s]\n%s" % (plane, gcmd.get_commandline()))
                factor = plane.lower() + '_factor'
                setattr(self, factor, calc_skew_factor(*lengths))
    cmd_SKEW_PROFILE_help = "Profile management for skew_correction"
    def cmd_SKEW_PROFILE(self, gcmd):
        if gcmd.get('LOAD', None) is not None:
            name = gcmd.get('LOAD')
            prof = self.skew_profiles.get(name)
            if prof is None:
                gcmd.respond_info(
                    "skew_correction:  Load failed, unknown profile [%s]"
                    % (name))
                return
            self._update_skew(prof['xy_skew'], prof['xz_skew'], prof['yz_skew'])
        elif gcmd.get('SAVE', None) is not None:
            name = gcmd.get('SAVE')
            configfile = self.printer.lookup_object('configfile')
            cfg_name = self.name + " " + name
            configfile.set(cfg_name, 'xy_skew', self.xy_factor)
            configfile.set(cfg_name, 'xz_skew', self.xz_factor)
            configfile.set(cfg_name, 'yz_skew', self.yz_factor)
            # Copy to local storage
            self.skew_profiles[name] = {
                'xy_skew': self.xy_factor,
                'xz_skew': self.xz_factor,
                'yz_skew': self.yz_factor
            }
            gcmd.respond_info(
                "Skew Correction state has been saved to profile [%s]\n"
                "for the current session.  The SAVE_CONFIG command will\n"
                "update the printer config file and restart the printer."
                % (name))
        elif gcmd.get('REMOVE', None) is not None:
            name = gmcd.get('REMOVE')
            if name in self.skew_profiles:
                configfile = self.printer.lookup_object('configfile')
                configfile.remove_section('skew_correction ' + name)
                del self.skew_profiles[name]
                gcmd.respond_info(
                    "Profile [%s] removed from storage for this session.\n"
                    "The SAVE_CONFIG command will update the printer\n"
                    "configuration and restart the printer" % (name))
            else:
                gcmd.respond_info(
                    "skew_correction: No profile named [%s] to remove"
                    % (name))


def load_config(config):
    return PrinterSkew(config)
