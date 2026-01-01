# Shell command execution support for Klipper
#
# Copyright (C) 2024 Microlay
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import subprocess
import logging
import os


class ShellCommand:
    def __init__(self, config):
        self.name = config.get_name().split()[1]
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.command = config.get('command')
        self.timeout = config.getfloat('timeout', 2.0)
        self.verbose = config.getboolean('verbose', True)

        # Register the gcode command
        self.gcode.register_mux_command(
            "RUN_SHELL_COMMAND", "CMD", self.name,
            self.cmd_RUN_SHELL_COMMAND,
            desc="Run a predefined shell command"
        )

    def cmd_RUN_SHELL_COMMAND(self, gcmd):
        # Get optional parameters
        params = gcmd.get('PARAMS', '')

        # Build the full command
        if params:
            full_command = f"{self.command} {params}"
        else:
            full_command = self.command

        if self.verbose:
            self.gcode.respond_info(f"Running: {full_command}")

        try:
            # Execute the command
            result = subprocess.run(
                full_command,
                shell=True,
                capture_output=True,
                text=True,
                timeout=self.timeout
            )

            if self.verbose:
                if result.stdout:
                    self.gcode.respond_info(f"Output: {result.stdout.strip()}")
                if result.stderr:
                    self.gcode.respond_info(f"Stderr: {result.stderr.strip()}")
                if result.returncode != 0:
                    self.gcode.respond_info(f"Return code: {result.returncode}")

        except subprocess.TimeoutExpired:
            raise self.gcode.error(f"Shell command timed out after {self.timeout}s")
        except Exception as e:
            raise self.gcode.error(f"Shell command error: {str(e)}")


def load_config_prefix(config):
    return ShellCommand(config)
