# adds support for coordinate system work offsets
#
# Copyright (C) 2022-3  Andrew Mirsky <andrew@mirsky.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
#
import os
try:
    import configparser
except ImportError:
    import ConfigParser as configparser
from enum import Enum
from position import Position

WorkOffsetCmds = Enum('WorkOffsetCmds',
                      ["G{}".format(c) for c in range(54, 60)])


class WorkOffsetNotInitializedError(Exception):
    pass


class WorkOffsetLoadError(Exception):
    pass


class WorkOffsetsSupport:

    work_offsets = None

    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.lookup_object('gcode_move')
        self.work_offset_idx = 0

        self.gcode.register_command("G53", self.cmd_G53)
        for w in WorkOffsetCmds:
            self.gcode.register_command(w.name, self.cmd_setWorkOffset)
        self.gcode.register_command("WORK_OFFSETS", self.cmd_showWorkOffsets)
        self.gcode.register_command("G10", self.cmd_G10)

        start_args = self.printer.get_start_args()
        self.work_offset_fn = start_args['work_offsets_file']
        self.load_work_offsets()

    def load_work_offsets(self):
        work_offset_parser = configparser.ConfigParser()
        if os.path.exists(self.work_offset_fn):
            try:
                work_offset_parser.read(self.work_offset_fn)
            except configparser.ParsingError as e:
                raise WorkOffsetLoadError(
                    "loading '{}' failed".format(self.work_offset_fn))
            self.work_offsets = {}
            for k in WorkOffsetCmds:
                pos = [work_offset_parser.getfloat('coordsys%s' % k.value, axis)
                       for axis in Position.axes()]
                self.work_offsets[k] = Position(*pos)

        else:
            self.work_offsets = {k.value: Position(0, 0, 0, 0)
                                 for k in WorkOffsetCmds}
            self.store_work_offsets()

    def store_work_offsets(self):
        work_offset_parser = configparser.ConfigParser()
        if not self.work_offsets:
            raise WorkOffsetNotInitializedError(
                "work offsets need to be initialized before storing")
        for k, position in self.work_offsets.items():
            work_offset_parser.add_section('coordsys%s' % k)
            for axis, val in position.items():
                work_offset_parser.set('coordsys%s' % k, axis, str(val))
        with open(self.work_offset_fn, 'w') as fh:
            work_offset_parser.write(fh)

    def cmd_G53(self, gcmd):
        if self.work_offset_idx != 0:
            old_offset = self.work_offsets[self.work_offset_idx]
            self.gcode_move.set_offset(old_offset * -1)
            self.work_offset_idx = 0

    _show_offsets = "[{}: {:03.3f},{:03.3f},{:03.3f},{:03.3f}]{}"
    def cmd_showWorkOffsets(self, gcmd):
        response = []
        for k, p in self.work_offsets.items():
            response.append(self._show_offsets.format(
                WorkOffsetCmds(k).name, p.x, p.y, p.z, p.e,
                "*" if k == self.work_offset_idx else ""))
        gcmd.respond_info("\n".join(response))

    def cmd_G10(self, gcmd):
        asL = gcmd.get_int("L", None)
        if not asL or asL != 2:
            # TODO : add support for L10 (?)
            raise gcmd.error("L2 much be specified")
        asP = gcmd.get_int("P", None)
        if asP is None or not 0 <= asP <= 6:
            raise gcmd.error("P0-6 must be specified")

        if asP == 0 and self.work_offset_idx == 0:
            raise gcmd.error("Offsets in machine space are not supported")

        asOffset = Position(*[gcmd.get_float(a, 0.) for a in Position.axes()])

        # if current space offset is being changed, remove old and set new
        if asP == self.work_offset_idx or asP == 0:
            old = self.work_offsets[self.work_offset_idx]
            self.gcode_move.clear_offset(old)
            self.gcode_move.set_offset(asOffset)

        if 0 < asP <= 6:
            self.work_offsets[asP] = asOffset
        else:
            self.work_offsets[self.work_offset_idx] = asOffset
        self.store_work_offsets()

    def cmd_setWorkOffset(self, gcmd):
        new_offset_idx = WorkOffsetCmds[gcmd.get_command()].value

        if self.work_offset_idx != 0:
            old_offset = self.work_offsets[self.work_offset_idx]
            self.gcode_move.clear_offset(old_offset)
        self.work_offset_idx = new_offset_idx
        self.gcode_move.set_offset(self.work_offsets[self.work_offset_idx])
        gcmd.respond_info(
            "coordinate system {} selected".format(self.work_offset_idx))

def load_config(config):
    return WorkOffsetsSupport(config)
