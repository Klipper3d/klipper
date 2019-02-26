# Helper script to adjust bed screws
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import math

def parse_coord(config, param):
    pair = config.get(param).strip().split(',', 1)
    try:
        return (float(pair[0]), float(pair[1]))
    except:
        raise config.error("%s:%s needs to be an x,y coordinate" % (
            config.get_name(), param))

class BedScrews:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.state = None
        self.current_screw = 0
        self.adjust_again = False
        # Read config
        self.screws = []
        fine_adjust = []
        for i in range(99):
            prefix = "screw%d" % (i + 1,)
            if config.get(prefix, None) is None:
                break
            screw_coord = parse_coord(config, prefix)
            screw_name = "screw at %.3f,%.3f" % screw_coord
            screw_name = config.get(prefix + "_name", screw_name)
            self.screws.append((screw_coord, screw_name))
            if config.get(prefix + "_fine_adjust", None) is not None:
                fine_coord = parse_coord(config, prefix + "_fine_adjust")
                fine_adjust.append((fine_coord, screw_name))
        if len(self.screws) < 3:
            raise config.error("bed_screws: Must have at least three screws")
        self.states = {'adjust': self.screws, 'fine': fine_adjust}
        self.speed = config.getfloat('speed', 50., above=0.)
        self.lift_speed = config.getfloat('probe_speed', 5., above=0.)
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 5.)
        self.probe_z = config.getfloat('probe_height', 0.)
        self.screw_thread = config.getfloat('screw_thread', default=3, minval=3, maxval=5)
        self.knob_direction = config.getfloat('knob_direction', default=1, minval=0, maxval=1)
        # Register command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("BED_SCREWS_ADJUST",
                                    self.cmd_BED_SCREWS_ADJUST,
                                    desc=self.cmd_BED_SCREWS_ADJUST_help)
        self.gcode.register_command("BED_SCREWS_CALCULATE",
                                    self.cmd_BED_SCREWS_CALCULATE,
                                    desc=self.cmd_BED_SCREWS_CALCULATE_help)
    def move(self, coord, speed):
        toolhead = self.printer.lookup_object('toolhead')
        curpos = toolhead.get_position()
        for i in range(len(coord)):
            if coord[i] is not None:
                curpos[i] = coord[i]
        try:
            toolhead.move(curpos, speed)
        except homing.EndstopError as e:
            raise self.gcode.error(str(e))
    def move_to_screw(self, state, screw):
        # Move up, over, and then down
        self.move((None, None, self.horizontal_move_z), self.lift_speed)
        coord, name = self.states[state][screw]
        self.move((coord[0], coord[1], self.horizontal_move_z), self.speed)
        self.move((coord[0], coord[1], self.probe_z), self.lift_speed)
        # Update state
        self.state = state
        self.current_screw = screw
        # Register commands
        self.gcode.respond_info(
            "Adjust %s. Then run ACCEPT, ADJUSTED, or ABORT\n"
            "Use ADJUSTED if a significant screw adjustment is made" % (name,))
        self.gcode.register_command('ACCEPT', self.cmd_ACCEPT,
                                    desc=self.cmd_ACCEPT_help)
        self.gcode.register_command('ADJUSTED', self.cmd_ADJUSTED,
                                    desc=self.cmd_ADJUSTED_help)
        self.gcode.register_command('ABORT', self.cmd_ABORT,
                                    desc=self.cmd_ABORT_help)
    def unregister_commands(self):
        self.gcode.register_command('ACCEPT', None)
        self.gcode.register_command('ADJUSTED', None)
        self.gcode.register_command('ABORT', None)
    cmd_BED_SCREWS_ADJUST_help = "Tool to help adjust bed leveling screws"
    def cmd_BED_SCREWS_ADJUST(self, params):
        if self.state is not None:
            raise self.gcode.error(
                "Already in bed_screws helper; use ABORT to exit")
        self.adjust_again = False
        self.move((None, None, self.horizontal_move_z), self.speed)
        self.move_to_screw('adjust', 0)
    cmd_ACCEPT_help = "Accept bed screw position"
    def cmd_ACCEPT(self, params):
        self.unregister_commands()
        if self.current_screw + 1 < len(self.states[self.state]):
            # Continue with next screw
            self.move_to_screw(self.state, self.current_screw + 1)
            return
        if self.adjust_again:
            # Retry coarse adjustments
            self.adjust_again = False
            self.move_to_screw('adjust', 0)
            return
        if self.state == 'adjust' and self.states['fine']:
            # Perform fine screw adjustments
            self.move_to_screw('fine', 0)
            return
        # Done
        self.state = None
        self.move((None, None, self.horizontal_move_z), self.lift_speed)
        self.gcode.respond_info("Bed screws tool completed successfully")
    cmd_ADJUSTED_help = "Accept bed screw position after notable adjustment"
    def cmd_ADJUSTED(self, params):
        self.unregister_commands()
        self.adjust_again = True
        self.cmd_ACCEPT(params)
    cmd_ABORT_help = "Abort bed screws tool"
    def cmd_ABORT(self, params):
        self.unregister_commands()
        self.state = None
    cmd_BED_SCREWS_CALCULATE_help = "Tool to help adjust bed leveling screws by " \
                                    "calculating the number of turns to level it."
    def cmd_BED_SCREWS_CALCULATE(self, params):
        if self.state is not None:
            raise self.gcode.error(
                "Already in bed_screws helper; use ABORT to exit before using this command")
        threads_factor = {3: 0.5, 4: 0.7, 5: 0.8}
        toolhead = self.printer.lookup_object('toolhead')
        screws_info = []
        self.gcode.run_script_from_command("G28")
        for coord, name in self.screws:
            pos = toolhead.get_position()
            self.move((pos[0], pos[1], self.horizontal_move_z), self.speed)
            self.move((coord[0], coord[1], self.horizontal_move_z), self.speed)
            self.gcode.run_script_from_command("PROBE")
            pos = toolhead.get_position()
            screws_info.append((pos[2], coord, name))

        pos = toolhead.get_position()
        self.move((pos[0], pos[1], self.horizontal_move_z), self.speed)

        for i, screw_info in enumerate(screws_info):
            if i == 0:
                z_base, coord_base, name_base = screw_info
                self.gcode.respond_info("%s (Base): X %.1f, Y %.1f, Z %.5f" %
                                        (name_base, coord_base[0], coord_base[1], z_base))
            else:
                z, coord, name = screw_info

                diff = z_base - z
                if abs(diff) < 0.001:
                    adjust = 0
                else:
                    adjust = diff / threads_factor.get(self.screw_thread, 0.5)
                if self.knob_direction == 1:
                    sign = "O>" if adjust < 0 else "<O"
                else:
                    sign = "<O" if adjust < 0 else "O>"
                full_turns = math.trunc(adjust)
                decimal_part = adjust - full_turns
                minutes = round(decimal_part * 60, 0)

                self.gcode.respond_info("%s : X %.1f, Y %.1f, Z %.5f : Adjust -> %s %02d:%02d" % (
                      name, coord[0], coord[1], z, sign, abs(full_turns), abs(minutes)))

def load_config(config):
    return BedScrews(config)
