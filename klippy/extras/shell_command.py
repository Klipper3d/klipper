# linux shell command execution utility
#
# Copyright (C) 2020  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os
import shlex
import subprocess
import logging

class ShellCommand:
    def __init__(self, printer, cmd):
        self.printer = printer
        self.name = cmd
        self.gcode = self.printer.lookup_object('gcode')
        self.output_cb = self.gcode.respond_info
        cmd = os.path.expanduser(cmd)
        self.command = shlex.split(cmd)
        self.proc_fd = None
        self.partial_output = ""

    def _process_output(self, eventime):
        if self.proc_fd is None:
            return
        try:
            data = os.read(self.proc_fd, 4096)
        except Exception:
            pass
        data = self.partial_output + data
        if '\n' not in data:
            self.partial_output = data
            return
        elif data[-1] != '\n':
            split = data.rfind('\n') + 1
            self.partial_output = data[split:]
            data = data[:split]
        try:
            self.output_cb(data)
        except Exception:
            logging.exception("Error writing command output")

    def set_output_callback(self, cb=None):
        if cb is None:
            self.output_cb = self.gcode.respond_info
        else:
            self.output_cb = cb

    def run(self, timeout=2., verbose=True):
        if not timeout:
            # Fire and forget commands cannot be verbose as we can't
            # clean up after the process terminates
            verbose = False
        reactor = self.printer.get_reactor()
        try:
            proc = subprocess.Popen(
                self.command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        except Exception:
            logging.exception(
                "shell_command: Command {%s} failed" % (self.name))
            raise self.gcode.error("Error running command {%s}" % (self.name))
        if verbose:
            self.proc_fd = proc.stdout.fileno()
            self.gcode.respond_info("Running Command {%s}...:" % (self.name))
            hdl = reactor.register_fd(self.proc_fd, self._process_output)
        elif not timeout:
            # fire and forget, return from execution
            return
        eventtime = reactor.monotonic()
        endtime = eventtime + timeout
        complete = False
        while eventtime < endtime:
            eventtime = reactor.pause(eventtime + .05)
            if proc.poll() is not None:
                complete = True
                break
        if not complete:
            proc.terminate()
        if verbose:
            if self.partial_output:
                self.output_cb(self.partial_output)
                self.partial_output = ""
            if complete:
                msg = "Command {%s} finished\n" % (self.name)
            else:
                msg = "Command {%s} timed out" % (self.name)
            self.gcode.respond_info(msg)
            reactor.unregister_fd(hdl)
            self.proc_fd = None

class PrinterShellCommand:
    def __init__(self, config):
        self.printer = config.get_printer()

    def load_shell_command(self, cmd):
        return ShellCommand(self.printer, cmd)

def load_config(config):
    return PrinterShellCommand(config)
