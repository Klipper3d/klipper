# Helper script for tuning pressure advance
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class TestPressureAdvance:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.normal_transform = None
        self.last_position = [0., 0., 0., 0.]
        self.last_pa = self.last_z = self.pa_start = self.pa_increment = 0.
        # Register command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("TEST_PRESSURE_ADVANCE",
                                   self.cmd_TEST_PRESSURE_ADVANCE,
                                   desc=self.cmd_TEST_PRESSURE_ADVANCE_help)
    cmd_TEST_PRESSURE_ADVANCE_help = "Test extruder pressure advance"
    def cmd_TEST_PRESSURE_ADVANCE(self, params):
        if self.normal_transform is not None:
            self.end_test()
        # Get parameters
        start = self.gcode.get_float('START', params, 0.)
        increment = self.gcode.get_float('INCREMENT', params, above=0.)
        # Enable test mode
        self.normal_transform = self.gcode.set_move_transform(self, force=True)
        self.last_z = 0.
        self.pa_start = start
        self.last_pa = max(self.pa_start, 0.)
        self.pa_increment = increment
        # Setup low cornering speeds
        logging.info("Starting TEST_PRESSURE_ADVANCE (start=%.6f inc=%.6f)" % (
            start, increment))
        self.gcode.run_script_from_command(
            "SET_VELOCITY_LIMIT ACCEL=500 SQUARE_CORNER_VELOCITY=1\n"
            "SET_PRESSURE_ADVANCE ADVANCE=%.6f ADVANCE_LOOKAHEAD_TIME=0" % (
                self.last_pa,))
        self.gcode.reset_last_position()
    def get_position(self):
        pos = self.normal_transform.get_position()
        self.last_postition = list(pos)
        return pos
    def move(self, newpos, speed):
        if (newpos[3] > self.last_position[3] and newpos[2] != self.last_z
            and newpos[:3] != self.last_position[:3]):
            # Extrusion move at new z height
            self.last_z = z_pos = newpos[2]
            pa = max(0., self.pa_start + z_pos * self.pa_increment)
            if pa < self.last_pa:
                self.end_test()
            elif pa > self.last_pa:
                # Update pressure advance
                logging.info("TEST_PRESSURE_ADVANCE z=%.3f pa=%.6f", z_pos, pa)
                self.last_pa = pa
                self.gcode.run_script_from_command(
                    "SET_PRESSURE_ADVANCE ADVANCE=%.6f" % (pa,))
        # Forward move to actual handler
        self.last_position[:] = newpos
        self.normal_transform.move(newpos, speed)
    def end_test(self):
        msg = "Ending TEST_PRESSURE_ADVANCE mode"
        logging.info(msg)
        self.gcode.respond_info(msg)
        self.gcode.set_move_transform(self.normal_transform, force=True)
        self.normal_transform = None

def load_config(config):
    return TestPressureAdvance(config)
