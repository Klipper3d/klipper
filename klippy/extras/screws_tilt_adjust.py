# Helper script to adjust bed screws tilt using Z probe
#
# Copyright (C) 2019  Rui Caridade <rui.mcbc@gmail.com>
# Copyright (C) 2021  Matthew Lloyd <github@matthewlloyd.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
from . import probe

class ScrewsTiltAdjust:
    def __init__(self, config):
        self.config = config
        self.printer = config.get_printer()
        self.screws = []
        self.results = []
        self.max_diff = None
        self.max_diff_error = False
        self.end_z = config.getfloat("end_z", None)

        use_bed_screws = config.get("use_bed_screws", "no").lower()
        if use_bed_screws == "yes" or use_bed_screws == "true" or use_bed_screws == "1":
            self.need_probe_offsets = True
            self._load_screws_from_bed_screws()
        else:
            self.need_probe_offsets = False
            self._load_screws(self.config)
        self.threads = {'CW-M3': 0, 'CCW-M3': 1, 'CW-M4': 2, 'CCW-M4': 3,
                        'CW-M5': 4, 'CCW-M5': 5, 'CW-M6': 6, 'CCW-M6': 7}
        self.thread = config.getchoice('screw_thread', self.threads,
                                       default='CW-M3')
        # Initialize ProbePointsHelper
        self.probe_helper = probe.ProbePointsHelper(self.config,
                                                    self.probe_finalize,
                                                    default_points=[(0,0)])
        # Register command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("SCREWS_TILT_CALCULATE",
                                    self.cmd_SCREWS_TILT_CALCULATE,
                                    desc=self.cmd_SCREWS_TILT_CALCULATE_help)
    cmd_SCREWS_TILT_CALCULATE_help = "Tool to help adjust bed leveling " \
                                     "screws by calculating the number " \
                                     "of turns to level it."

    def _load_screws_from_bed_screws(self):
        config = self.config
        if not config.has_section('bed_screws'):
            raise config.error("screws_tilt_adjust: must have bed_screws "
                    "if use_bed_screws is set")
        bed_screws = config.getsection('bed_screws')
        self._load_screws(bed_screws)

    def _load_screws(self, config):
        # Read config
        for i in range(99):
            prefix = "screw%d" % (i + 1,)
            if config.get(prefix, None) is None:
                break
            screw_coord = config.getfloatlist(prefix, count=2)
            screw_name = "screw at %.3f,%.3f" % screw_coord
            screw_name = config.get(prefix + "_name", screw_name)
            self.screws.append((screw_coord, screw_name))
        if len(self.screws) < 3:
            raise config.error("screws_tilt_adjust: Must have "
                           "at least three screws")

    def _offset_screws(self, gcmd):
        if not self.need_probe_offsets:
            return
        th = self.printer.lookup_object('toolhead')
        xrange = th.get_kinematics().rails[0].get_range()
        yrange = th.get_kinematics().rails[1].get_range()
        probe = self.printer.lookup_object('probe')
        probe_x, probe_y, _ = probe.get_offsets()
        for i in range(len(self.screws)):
            screw_coord, screw_name = self.screws[i]
            sx, sy = screw_coord
            sx -= probe_x
            sy -= probe_y
            if xrange[0] >= sx or sx >= xrange[1]:
                sx = max(xrange[0]+1.0, min(sx, xrange[1]-1.0))
                gcmd.respond_info(f"screws_tilt_adjust: screw {screw_name} X"
                        f"out of range, adjusting to {sx}")
            if yrange[0] >= sy or sy >= yrange[1]:
                sy = max(yrange[0]+1.0, min(sy, yrange[1]-1.0))
                gcmd.respond_info(f"screws_tilt_adjust: screw {screw_name} Y"
                        f"out of range, adjusting to {sy}")
            self.screws[i] = ((sx, sy), screw_name)
        self.need_probe_offsets = False

    def cmd_SCREWS_TILT_CALCULATE(self, gcmd):
        self.max_diff = gcmd.get_float("MAX_DEVIATION", None)
        # Option to force all turns to be in the given direction (CW or CCW)
        direction = gcmd.get("DIRECTION", default=None)
        if direction is not None:
            direction = direction.upper()
            if direction not in ('CW', 'CCW'):
                raise gcmd.error(
                    "Error on '%s': DIRECTION must be either CW or CCW" % (
                        gcmd.get_commandline(),))
        self.direction = direction
        self._offset_screws(gcmd)
        points = [coord for coord, name in self.screws]
        self.probe_helper.update_probe_points(points, 3)
        self.probe_helper.start_probe(gcmd)

    def get_status(self, eventtime):
        return {'error': self.max_diff_error,
            'max_deviation': self.max_diff,
            'results': self.results}

    def probe_finalize(self, offsets, positions):
        if self.end_z is not None:
            th = self.printer.lookup_object('toolhead')
            th.manual_move([None, None, self.end_z], 10)
        self.results = {}
        self.max_diff_error = False
        # Factors used for CW-M3, CCW-M3, CW-M4, CCW-M4, CW-M5, CCW-M5, CW-M6
        #and CCW-M6
        threads_factor = {0: 0.5, 1: 0.5, 2: 0.7, 3: 0.7, 4: 0.8, 5: 0.8,
        6: 1.0, 7: 1.0}
        is_clockwise_thread = (self.thread & 1) == 0
        screw_diff = []
        # Process the read Z values
        if self.direction is not None:
            # Lowest or highest screw is the base position used for comparison
            use_max = ((is_clockwise_thread and self.direction == 'CW')
                    or (not is_clockwise_thread and self.direction == 'CCW'))
            min_or_max = max if use_max else min
            i_base, z_base = min_or_max(
                enumerate([pos[2] for pos in positions]), key=lambda v: v[1])
        else:
            # First screw is the base position used for comparison
            i_base, z_base = 0, positions[0][2]
        # Provide the user some information on how to read the results
        self.gcode.respond_info("01:20 means 1 full turn and 20 minutes, "
                                "CW=clockwise, CCW=counter-clockwise")
        for i, screw in enumerate(self.screws):
            z = positions[i][2]
            coord, name = screw
            if i == i_base:
                # Show the results
                self.gcode.respond_info(
                    "%s : x=%.1f, y=%.1f, z=%.5f" %
                    (name + ' (base)', coord[0], coord[1], z))
                sign = "CW" if is_clockwise_thread else "CCW"
                self.results["screw%d" % (i + 1,)] = {'z': z, 'sign': sign,
                    'adjust': '00:00', 'is_base': True}
            else:
                # Calculate how knob must be adjusted for other positions
                diff = z_base - z
                screw_diff.append(abs(diff))
                if abs(diff) < 0.001:
                    adjust = 0
                else:
                    adjust = diff / threads_factor.get(self.thread, 0.5)
                if is_clockwise_thread:
                    sign = "CW" if adjust >= 0 else "CCW"
                else:
                    sign = "CCW" if adjust >= 0 else "CW"
                adjust = abs(adjust)
                full_turns = math.trunc(adjust)
                decimal_part = adjust - full_turns
                minutes = round(decimal_part * 60, 0)
                # Show the results
                self.gcode.respond_info(
                    "%s : x=%.1f, y=%.1f, z=%.5f : adjust %s %02d:%02d" %
                    (name, coord[0], coord[1], z, sign, full_turns, minutes))
                self.results["screw%d" % (i + 1,)] = {'z': z, 'sign': sign,
                    'adjust':"%02d:%02d" % (full_turns, minutes),
                    'is_base': False}
        if self.max_diff and any((d > self.max_diff) for d in screw_diff):
            self.max_diff_error = True
            raise self.gcode.error(
                "bed level exceeds configured limits ({}mm)! " \
                "Adjust screws and restart print.".format(self.max_diff))

def load_config(config):
    return ScrewsTiltAdjust(config)
