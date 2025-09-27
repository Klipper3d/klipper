# Helper script to adjust parameters based on Z level
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging

CANCEL_Z_DELTA=5.0

class extrusionTower:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.normal_transform = None
        self.last_position = [0., 0., 0., 0., 0.]
        self.last_z = self.start = self.factor = self.band = 0.
        self.last_command_value = None
        self.command_fmt = ""
        self.gcode_move = self.printer.load_object(config, "gcode_move")
        # Register command
        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_command("EXTRUSION_TOWER", self.cmd_EXTRUSION_TOWER,
                                    desc=self.cmd_CTC_EXTRUSION_help)
    cmd_CTC_EXTRUSION_help = "Tool to adjust a parameter at each Z height"
    def cmd_EXTRUSION_TOWER(self, gcmd):
        if self.normal_transform is not None:
            self.end_test()
        # Get parameters
        command = "M221"
        # parameter = gcmd.get('PARAMETER')
        self.start = gcmd.get_float('START', 0.)
        self.height_delta = gcmd.get_float('HEIGHT_DELTA', 0.)
        self.start_percent = gcmd.get_float('START_PERCENT', 100., minval=10.)
        self.stop_percent = gcmd.get_float('STOP_PERCENT', 100., minval=10.)
        self.step = gcmd.get_float('STEP', 10.)

        # Enable test mode
        self.command_fmt = "%s S%%.9f" % (command)

        nt = self.gcode_move.set_move_transform(self, force=True)
        self.normal_transform = nt
        self.last_z = -99999999.9
        self.current_value = self.start_percent
        self.last_command_value = None
        self.get_position()
        message_parts = []
        message_parts.append("start=%.6f" % (self.start,))
        message_parts.append("height_delta=%.6f" % (self.height_delta,))
        message_parts.append("start_percent=%.6f" % (self.start_percent,))
        message_parts.append("stop_percent=%.6f" % (self.stop_percent,))
        message_parts.append("step_size=%.6f" % (self.step,))
        # if self.skip:
        #     message_parts.append("skip=%.6f" % (self.skip,))
        gcmd.respond_info(
            "Starting tuning test (" + " ".join(message_parts) + ")")
    def get_position(self):
        pos = self.normal_transform.get_position()
        self.last_position = list(pos)
        return pos
    def calc_value(self):
        new_value = self.current_value + self.step

        if(new_value < self.stop_percent):
            new_value = self.stop_percent

        return new_value
    def move(self, newpos, speed):
        normal_transform = self.normal_transform
        if (newpos[4] > self.last_position[4] and newpos[2] >= (self.last_z + self.height_delta)
            and newpos[:4] != self.last_position[:4]):
            # Extruder move at new z height
            z = newpos[2]
            if z < self.last_z - CANCEL_Z_DELTA:
                # lower z height - probably start of new print
                self.end_test()
            else:
                # Process update
                newval = self.calc_value()
                self.current_value = newval
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
    def is_active(self):
        return self.normal_transform is not None

def load_config(config):
    return extrusionTower(config)
