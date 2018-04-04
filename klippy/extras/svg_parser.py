# SVG support
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import pins, homing
import os, logging
from svgpathtools import *
import numpy as np

class SvgParser:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.z_lift_height = config.getfloat('z_lift_height', 2.0)
        self.z_draw_height = config.getfloat('z_draw_height', 0.0)
        sp = config.get('storage_path')
        self.storage_path_dirname = os.path.normpath(os.path.expanduser(sp))
        self.paths = None
        self.file_size = 0
        self.current_file = None
        self.current_path = self.current_segment = self.path_count = 0
        # Work timer
        self.reactor = self.printer.get_reactor()
        self.must_pause_work = False
        self.work_timer = None
        # Register command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('SVG_LIST', self.cmd_list_files)
        self.gcode.register_command('M78', self.cmd_open)
        self.gcode.register_command('SVG_START', self.cmd_start)
        self.gcode.register_command('SVG_PAUSE', self.cmd_pause)
        self.gcode.register_command('SVG_STATUS', self.cmd_status)
        self.gcode.register_command('SVG_CANCEL', self.cmd_cancel)
    cmd_svg_help = "SVG processor commands: \n"
    "- SVG_SELECT\n"
    "- SVG_START\n"
    "- SVG_PAUSE\n"
    "- SVG_STATUS\n"
    "- SVG_CANCEL"
    def printer_state(self, state):
        if state == 'shutdown' and self.work_timer is not None:
            self.must_pause_work = True
    def get_file_list(self):
        dname = self.storage_path_dirname
        try:
            filtered = [f for f in os.listdir(dname) if (f.find('.svg') > -1)]
            return [(fname, os.path.getsize(os.path.join(dname, fname)))
                    for fname in filtered]
        except:
            logging.exception("svg parser get_file_list")
            raise self.gcode.error("Unable to get file list")
    def get_status(self, eventtime):
        progress = 0.
        if self.work_timer is not None and self.path_count:
            progress = float(self.current_path) / self.path_count
        return {'progress': progress}
    def pixels_to_mm(self, value, dpi):
        return value * 25.4 / dpi
    def mm_to_pixels(self, value, dpi):
        return value / 25.4 * dpi
    def lift_pen(self, head):
        pos = head.get_position()
        pos[2] = self.z_lift_height
        head.move(pos, 5)
    def engage_pen(self, head):
        pos = head.get_position()
        pos[2] = self.z_draw_height
        head.move(pos, 5)
    def cmd_list_files(self, params):
        # List SD card
        files = self.get_file_list()
        self.gcode.respond("+ Available files ---------------")
        for fname, fsize in files:
            self.gcode.respond(" %s %d" % (fname, fsize))
        self.gcode.respond("- -------------------------------")
    def cmd_open(self, params):
        # Select SVG file
        if self.work_timer is not None:
            raise self.gcode.error("SVG processor busy")
        if self.current_file is not None:
            self.current_file.close()
            self.current_file = None
            self.current_path = self.current_segment = self.file_size = 0
        try:
            orig = params['#original']
            filename = orig[orig.find("M78") + 4:].split()[0].strip()
            if '*' in filename:
                filename = filename[:filename.find('*')].strip()
        except:
            raise self.gcode.error("Unable to extract filename")
        if filename.startswith('/'):
            filename = filename[1:]
        files = self.get_file_list()
        files_by_lower = { fname.lower(): fname for fname, fsize in files }
        try:
            fname = files_by_lower[filename.lower()]
            fname = os.path.join(self.storage_path_dirname, fname)
            # Load SVG and convert all elements to SVG paths
            self.paths, attributes = svg2paths(fname)
        except:
            logging.exception("SVG file already in use")
            raise self.gcode.error("Unable to open file")
        self.current_path = self.current_segment = 0
        self.path_count = len(self.paths)
        self.gcode.respond("File opened:%s Number of paths:%d" % (filename, self.path_count))
        self.gcode.respond("File opened:%s Size:%d" % (filename, self.path_count))
        self.gcode.respond("File selected")
    def cmd_start(self, params):
        # Start/resume SVG print
        if self.work_timer is not None:
            raise self.gcode.error("SVG processor busy")
        self.must_pause_work = False
        toolhead = self.printer.lookup_object('toolhead')
        self.lift_pen(toolhead)
        self.work_timer = self.reactor.register_timer(
            self.work_handler, self.reactor.NOW)
    def cmd_pause(self, params):
        # Pause SVG print
        if self.work_timer is not None:
            self.must_pause_work = True
    def cmd_status(self, params):
        # Report SVG print status
        if self.current_file is None or self.work_timer is None:
            self.gcode.respond("Not currently SVG printing.")
            return
        self.gcode.respond("SVG printing path %d/%d" % (
            self.current_path, self.path_count))
    def cmd_cancel(self, params):
        # Cancel SVG print
        pass
    # Background work timer
    def work_handler(self, eventtime):
        self.reactor.unregister_timer(self.work_timer)
        toolhead = self.printer.lookup_object('toolhead')
        points = []
        stepover = self.mm_to_pixels(2.0, 72)
        speed = 8
        while not self.must_pause_work:
            logging.info("path {0}/{1} segment {2}".format(self.current_path, self.path_count, self.current_segment))
            if self.current_path < self.path_count:
                if self.current_segment < len(self.paths[self.current_path]):
                    logging.info("  - segment {0}".format(self.current_segment))
                    
                    segment = self.paths[self.current_path][self.current_segment]
                    
                    if type(segment) is Line:
                        points.append([self.pixels_to_mm(real(segment.start), 72), self.pixels_to_mm(imag(segment.start), 72), self.z_draw_height, 0])
                        points.append([self.pixels_to_mm(real(segment.end), 72), self.pixels_to_mm(imag(segment.end), 72), self.z_draw_height, 0])
                    else:                    
                        steps = int(segment.length() / stepover)
                        for i in range(0, steps):
                            point = segment.point(i / (steps * 1.0))
                            points.append([self.pixels_to_mm(real(point), 72), self.pixels_to_mm(imag(point), 72), self.z_draw_height, 0])
                    p = points.pop(0)
                    toolhead.move(p, speed)
                    self.engage_pen(toolhead)
                    while points:
                        p = points.pop(0)
                        toolhead.move(p, speed)
                        
                    self.current_segment += 1
                else:
                    self.current_path += 1
                    self.current_segment = 0
                    self.lift_pen(toolhead)
            else:
                break
        self.work_timer = None
        return self.reactor.NEVER
def load_config(config):
    return SvgParser(config)

# NOTES
# accessing toolhead.move directly does not honor the bedtilt compensation
# In gcode.py this is handled by using the "tranform "function that redirects
# move commands to the bedtilt class if compensation is used