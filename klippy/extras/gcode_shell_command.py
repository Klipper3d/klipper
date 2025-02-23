# Execute shell commands from gcode
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import os
import shlex
import subprocess
import logging

class ShellCommand:
    def __init__(self, config):
        self.name = config.get_name().split()[-1]
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        cmd = config.get('command')
        self.command = shlex.split(cmd)
        self.timeout = config.getfloat('timeout', 2., above=0.)
        self.verbose = config.getboolean('verbose', True)
        self.proc_fd = None
        self.gcode.register_mux_command(
            "RUN_SHELL_COMMAND", "CMD", self.name,
            self.cmd_RUN_SHELL_COMMAND,
            desc=self.cmd_RUN_SHELL_COMMAND_help)

    def cmd_RUN_SHELL_COMMAND_help(self):
        return "Run a linux shell command"

    def cmd_RUN_SHELL_COMMAND(self, gcmd):
        try:
            proc = subprocess.Popen(
                self.command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            if self.verbose:
                self.proc_fd = proc.stdout.fileno()
            timeout = gcmd.get_float('TIMEOUT', self.timeout, above=0.)
            proc.wait(timeout=timeout)
            if proc.returncode != 0:
                raise gcmd.error(
                    "Shell command '%s' returned error\n"
                    % (' '.join(self.command)))
        except Exception:
            logging.exception("Shell command error")
            raise

def load_config_prefix(config):
    return ShellCommand(config) 