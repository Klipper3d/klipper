# Helper script to adjust bed screws
#
# Copyright (C) 2019-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class BedScrews:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reset()
        self.number_of_screws = 0
        # Read config
        screws = []
        fine_adjust = []
        for i in range(99):
            prefix = "screw%d" % (i + 1,)
            if config.get(prefix, None) is None:
                break
            screw_coord = config.getfloatlist(prefix, count=2)
            screw_name = "screw at %.3f,%.3f" % screw_coord
            screw_name = config.get(prefix + "_name", screw_name)
            screws.append((screw_coord, screw_name))
            pfa = prefix + "_fine_adjust"
            if config.get(pfa, None) is not None:
                fine_coord = config.getfloatlist(pfa, count=2)
                fine_adjust.append((fine_coord, screw_name))
        if len(screws) < 3:
            raise config.error("bed_screws: Must have at least three screws")
        self.number_of_screws = len(screws)
        self.states = {'adjust': screws, 'fine': fine_adjust}
        self.speed = config.getfloat('speed', 50., above=0.)
        self.lift_speed = config.getfloat('probe_speed', 5., above=0.)
        self.horizontal_move_z = config.getfloat('horizontal_move_z', 5.)
        self.probe_z = config.getfloat('probe_height', 0.)
        # Register command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("BED_SCREWS_ADJUST",
                                    self.cmd_BED_SCREWS_ADJUST,
                                    desc=self.cmd_BED_SCREWS_ADJUST_help)
    def reset(self):
        self.state = None
        self.current_screw = 0
        self.accepted_screws = 0
    def move(self, coord, speed):
        self.printer.lookup_object('toolhead').manual_move(coord, speed)
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
    def get_status(self, eventtime):
        return {
            'is_active': self.state is not None,
            'state': self.state,
            'current_screw': self.current_screw,
            'accepted_screws': self.accepted_screws
        }
    cmd_BED_SCREWS_ADJUST_help = "Tool to help adjust bed leveling screws"
    def cmd_BED_SCREWS_ADJUST(self, gcmd):
        if self.state is not None:
            raise gcmd.error("Already in bed_screws helper; use ABORT to exit")
        # reset accepted screws
        self.accepted_screws = 0
        self.move((None, None, self.horizontal_move_z), self.speed)
        self.move_to_screw('adjust', 0)
    cmd_ACCEPT_help = "Accept bed screw position"
    def cmd_ACCEPT(self, gcmd):
        self.unregister_commands()
        self.accepted_screws = self.accepted_screws + 1
        if self.current_screw + 1 < len(self.states[self.state]) \
                and self.accepted_screws < self.number_of_screws:
            # Continue with next screw
            self.move_to_screw(self.state, self.current_screw + 1)
            return
        if self.accepted_screws < self.number_of_screws:
            # Retry coarse adjustments
            self.move_to_screw('adjust', 0)
            return
        if self.state == 'adjust' and self.states['fine']:
            # Reset accepted screws for fine adjustment
            self.accepted_screws = 0
            # Perform fine screw adjustments
            self.move_to_screw('fine', 0)
            return
        # Done
        self.reset()
        self.move((None, None, self.horizontal_move_z), self.lift_speed)
        gcmd.respond_info("Bed screws tool completed successfully")
    cmd_ADJUSTED_help = "Accept bed screw position after notable adjustment"
    def cmd_ADJUSTED(self, gcmd):
        self.unregister_commands()
        self.accepted_screws = -1
        self.cmd_ACCEPT(gcmd)
    cmd_ABORT_help = "Abort bed screws tool"
    def cmd_ABORT(self, gcmd):
        self.unregister_commands()
        self.reset()

def load_config(config):
    return BedScrews(config)
