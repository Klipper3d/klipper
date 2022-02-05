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
        self.last_command_value = None
        self.command_fmt = ""
        self.gcode_move = self.printer.load_object(config, "gcode_move")
        # Register command
        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_command("TUNING_TOWER", self.cmd_TUNING_TOWER,
                                    desc=self.cmd_TUNING_TOWER_help)
    cmd_TUNING_TOWER_help = "Tool to adjust a parameter at each Z height"
    def cmd_TUNING_TOWER(self, gcmd):
        if self.normal_transform is not None:
            self.end_test()
        # Get parameters
        command = gcmd.get('COMMAND')
        parameter = gcmd.get('PARAMETER')
        self.start = gcmd.get_float('START', 0.)
        self.factor = gcmd.get_float('FACTOR', 0.)
        self.band = gcmd.get_float('BAND', 0., minval=0.)
        self.step_delta = gcmd.get_float('STEP_DELTA', 0.)
        self.step_height = gcmd.get_float('STEP_HEIGHT', 0., minval=0.)
        self.skip = gcmd.get_float('SKIP', 0., minval=0.)
        if self.factor and (self.step_height or self.step_delta):
            raise gcmd.error(
                "Cannot specify both FACTOR and STEP_DELTA/STEP_HEIGHT")
        if (self.step_delta != 0.) != (self.step_height != 0.):
            raise gcmd.error("Must specify both STEP_DELTA and STEP_HEIGHT")
        # Enable test mode
        if self.gcode.is_traditional_gcode(command):
            self.command_fmt = "%s %s%%.9f" % (command, parameter)
        else:
            self.command_fmt = "%s %s=%%.9f" % (command, parameter)
        nt = self.gcode_move.set_move_transform(self, force=True)
        self.normal_transform = nt
        self.last_z = -99999999.9
        self.last_command_value = None
        self.get_position()
        message_parts = []
        message_parts.append("start=%.6f" % (self.start,))
        if self.factor:
            message_parts.append("factor=%.6f" % (self.factor,))
            if self.band:
                message_parts.append("band=%.6f" % (self.band,))
        else:
            message_parts.append("step_delta=%.6f" % (self.step_delta,))
            message_parts.append("step_height=%.6f" % (self.step_height,))
        if self.skip:
            message_parts.append("skip=%.6f" % (self.skip,))
        gcmd.respond_info(
            "Starting tuning test (" + " ".join(message_parts) + ")")
    def get_position(self):
        pos = self.normal_transform.get_position()
        self.last_position = list(pos)
        return pos
    def calc_value(self, z):
        if self.skip:
            z = max(0., z - self.skip)
        if self.step_height:
            return self.start + \
                   self.step_delta * math.floor(z / self.step_height)
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
                gcode_z = self.gcode_move.get_status()['gcode_position'].z
                newval = self.calc_value(gcode_z)
                self.last_z = z
                if newval != self.last_command_value:
                    self.last_command_value = newval
                    self.gcode.run_script_from_command(self.command_fmt
                                                       % (newval,))
        # Forward move to actual handler
        self.last_position[:] = newpos
        normal_transform.move(newpos, speed)
    def end_test(self):
        self.gcode.respond_info("Ending tuning test mode")
        self.gcode_move.set_move_transform(self.normal_transform, force=True)
        self.normal_transform = None

def load_config(config):
    return TuningTower(config)
