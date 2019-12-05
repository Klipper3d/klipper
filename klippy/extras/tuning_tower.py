# Helper script to adjust parameters based on Z level
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging

CANCEL_Z_DELTA=2.0

class TuningTower:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.normal_transform = None
        self.last_position = [0., 0., 0., 0.]
        self.last_z = self.start = self.factor = self.band = 0.
        self.command = self.parameter = ""
        # Register command
        gcode = self.printer.lookup_object("gcode")
        gcode.register_command("TUNING_TOWER", self.cmd_TUNING_TOWER,
                               desc=self.cmd_TUNING_TOWER_help)
    cmd_TUNING_TOWER_help = "Tool to adjust a parameter at each Z height"
    def cmd_TUNING_TOWER(self, params):
        if self.normal_transform is not None:
            self.end_test()
        # Get parameters
        gcode = self.printer.lookup_object("gcode")
        self.command = gcode.get_str('COMMAND', params)
        self.parameter = gcode.get_str('PARAMETER', params)
        self.start = gcode.get_float('START', params, 0.)
        self.factor = gcode.get_float('FACTOR', params)
        self.band = gcode.get_float('BAND', params, 0., minval=0.)
        # Enable test mode
        self.normal_transform = gcode.set_move_transform(self, force=True)
        self.last_z = -99999999.9
        gcode.reset_last_position()
        self.get_position()
        gcode.respond_info("Starting tuning test (start=%.6f factor=%.6f)"
                           % (self.start, self.factor))
    def get_position(self):
        pos = self.normal_transform.get_position()
        self.last_position = list(pos)
        return pos
    def calc_value(self, z):
        if self.band:
            z = (math.floor(z / self.band) + .5) * self.band
        return self.start + z * self.factor
    def move(self, newpos, speed):
        normal_transform = self.normal_transform
        if (newpos[3] > self.last_position[3] and newpos[2] != self.last_z
            and newpos[:3] != self.last_position[:3]):
            # Extrusion move at new z height
            z = newpos[2]
            if z < self.last_z - CANCEL_Z_DELTA:
                # Extrude at a lower z height - probably start of new print
                self.end_test()
            else:
                # Process update
                oldval = self.calc_value(self.last_z)
                newval = self.calc_value(z)
                self.last_z = z
                if newval != oldval:
                    gcode = self.printer.lookup_object("gcode")
                    gcode.run_script_from_command("%s %s=%.9f" % (
                        self.command, self.parameter, newval))
        # Forward move to actual handler
        self.last_position[:] = newpos
        normal_transform.move(newpos, speed)
    def end_test(self):
        gcode = self.printer.lookup_object("gcode")
        gcode.respond_info("Ending tuning test mode")
        gcode.set_move_transform(self.normal_transform, force=True)
        self.normal_transform = None

def load_config(config):
    return TuningTower(config)
