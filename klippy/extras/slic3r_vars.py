# Slic3r vars
#
# Copyright (C) 2021  Tom Whitwell <tom@whi.tw>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import re


class Slic3rGcodeVars:
    config = {}
    config_line_re = re.compile(r'; (.*?)\s=\s(.*)')
    file_path = None

    def __init__(self, file_path):
        self.file_path = file_path
        self.extract_config_lines()

    def get_status(self):
        return self.config

    def extract_config_lines(self):
        with open(self.file_path, "r") as f:
            lines = self.config_line_re.findall(f.read())
            if lines:
                self.config = {}
                for k, v in lines:
                    self.config[k] = v


class Slic3rVars:
    def __init__(self, config):
        self.current_job = None
        printer = config.get_printer()
        self.print_stats = printer.lookup_object("print_stats")
        self.virtual_sdcard = printer.lookup_object("virtual_sdcard")
        printer.register_event_handler("print_stats:load",
                                       self._handle_load)
        printer.register_event_handler("print_stats:reset",
                                       self._handle_finish)
        printer.register_event_handler("print_stats:finish",
                                       self._handle_finish)

    def _handle_load(self):
        current_file = self.virtual_sdcard.file_path()
        self.current_job = Slic3rGcodeVars(current_file)

    def _handle_finish(self):
        if self.current_job:
            del self.current_job
            self.current_job = None

    def get_status(self, _):
        try:
            return self.current_job.get_status()
        except AttributeError:
            return {}


def load_config(config):
    return Slic3rVars(config)
