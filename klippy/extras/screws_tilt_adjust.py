# Helper script to adjust bed screws tilt using Z probe
#
# Copyright (C) 2019  Rui Caridade <rui.mcbc@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import homing
import math

def parse_coord(config, param):
    pair = config.get(param).strip().split(',', 1)
    try:
        return (float(pair[0]), float(pair[1]))
    except:
        raise config.error("%s:%s needs to be an x,y coordinate" % (
            config.get_name(), param))

class ScrewsTiltAdjust:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.screws = []
        # Verify that a probe exists
        try:
            self.printer.lookup_object("probe")
        except:
            raise self.gcode.error("Error: you must have a probe on "
                                   "your config file.")
        # Read config
        for i in range(99):
            prefix = "screw%d" % (i + 1,)
            if config.get(prefix, None) is None:
                break
            screw_coord = parse_coord(config, prefix)
            screw_name = "screw at %.3f,%.3f" % screw_coord
            screw_name = config.get(prefix + "_name", screw_name)
            self.screws.append((screw_coord, screw_name))
        if len(self.screws) < 2:
            raise config.error("screws_tilt_adjust: Must have "
                               "at least two screws")
        self.move_speed = config.getfloat('move_speed', 50., above=0.)
        self.probe_z_start = config.getfloat('probe_z_start', 10.)
        self.probes = config.getint('num_probes', 3, minval=1)
        # 'screw_thread' is optional
        self.threads = {'CW-M3': 0, 'CCW-M3': 1, 'CW-M4': 2, 'CCW-M4': 3,
                        'CW-M5': 4, 'CCW-M5': 5}
        self.calculations = {'median': 0, 'average': 1}
        self.thread = config.getchoice('screw_thread', self.threads,
                                       default='CW-M3')
        self.calculation = config.getchoice('calculate', self.calculations,
                                            default='median')
        # Register command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("SCREWS_TILT_CALCULATE",
                                    self.cmd_SCREWS_TILT_CALCULATE,
                                    desc=self.cmd_SCREWS_TILT_CALCULATE_help)
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
    cmd_SCREWS_TILT_CALCULATE_help = "Tool to help adjust bed leveling " \
                                     "screws by calculating the number " \
                                     "of turns to level it."
    def cmd_SCREWS_TILT_CALCULATE(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        # Give feedback of parameters
        self.gcode.respond_info("Screw thread: %s, calculate: %s " %
                                (self.threads.keys()[
                                     self.threads.values().index(
                                         self.thread)],
                                 self.calculations.keys()[
                                     self.calculations.values().index(
                                         self.calculation)]))
        # Factors used for CW-M3, CCW-M3, CW-M4, CCW-M4, CW-M5 and CCW-M5
        threads_factor = {0: 0.5, 1: 0.5, 2: 0.7, 3: 0.7, 4: 0.8, 5: 0.8}
        # Prepare a place to save the Z read values
        screws_info = []
        # Probe the bed in the screw define points
        for coord, name in self.screws:
            self.gcode.respond_info("Screw: %s, probes: %d" %
                                    (name, self.probes))
            probes = []
            for i in range(self.probes):
                pos = toolhead.get_position()
                # First move Z up
                self.move((pos[0], pos[1], self.probe_z_start),
                          self.move_speed)
                # Move to probe position
                self.move((coord[0], coord[1], self.probe_z_start),
                          self.move_speed)
                # Probe the bed
                self.gcode.run_script_from_command("PROBE")
                pos = toolhead.get_position()
                probes.append(pos[2])

            if self.probes > 1:
                # More than one probe
                if self.calculation == 0:
                    # Calculate Median
                    sorted_probes = sorted(probes)
                    middle = self.probes // 2
                    if (self.probes & 1) == 1:
                        # odd number of reads
                        value = sorted_probes[middle]
                    else:
                        # even number of reads
                        value = (sorted_probes[middle] +
                                 sorted_probes[middle - 1]) / 2
                else:
                    # Calculate Average
                    value = sum(probes) / self.probes
            else:
                # Only one probe
                value = probes[0]
            # Save read Z value
            screws_info.append((value, coord, name))
        # When finish move Z up
        pos = toolhead.get_position()
        self.move((pos[0], pos[1], self.probe_z_start), self.move_speed)
        # Process the read Z values and
        for i, screw_info in enumerate(screws_info):
            if i == 0:
                # First screw is the base position used for comparison
                z_base, coord_base, name_base = screw_info
                # Show the results
                self.gcode.respond_info("%s (Base): X %.1f, Y %.1f, Z %.5f" %
                                        (name_base, coord_base[0],
                                         coord_base[1], z_base))
            else:
                # Calculate how knob must me adjusted for other positions
                z, coord, name = screw_info
                diff = z_base - z
                if abs(diff) < 0.001:
                    adjust = 0
                else:
                    adjust = diff / threads_factor.get(self.thread, 0.5)
                if (self.thread & 1) == 1:
                    sign = "CW" if adjust < 0 else "CCW"
                else:
                    sign = "CCW" if adjust < 0 else "CW"
                full_turns = math.trunc(adjust)
                decimal_part = adjust - full_turns
                minutes = round(decimal_part * 60, 0)
                # Show the results
                self.gcode.respond_info("%s : X %.1f, Y %.1f, Z %.5f : "
                                         "Adjust -> %s %02d:%02d" % (
                                         name, coord[0], coord[1], z, sign,
                                         abs(full_turns), abs(minutes)))

def load_config(config):
    return ScrewsTiltAdjust(config)
