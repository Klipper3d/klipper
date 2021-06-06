# Sdcard file looping support
#
# Copyright (C) 2021  Jason S. McMullan <jason.mcmullan@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class SDCardLoop:
    def __init__(self, config):
        printer = config.get_printer()
        self.sdcard = printer.load_object(config, "virtual_sdcard")
        self.gcode = printer.lookup_object('gcode')
        self.gcode.register_command(
            "SDCARD_LOOP_BEGIN", self.cmd_SDCARD_LOOP_BEGIN,
            desc=self.cmd_SDCARD_LOOP_BEGIN_help)
        self.gcode.register_command(
            "SDCARD_LOOP_END", self.cmd_SDCARD_LOOP_END,
            desc=self.cmd_SDCARD_LOOP_END_help)
        self.gcode.register_command(
            "SDCARD_LOOP_DESIST", self.cmd_SDCARD_LOOP_DESIST,
            desc=self.cmd_SDCARD_LOOP_DESIST_help)
        self.loop_stack = []
    cmd_SDCARD_LOOP_BEGIN_help = "Begins a looped section in the SD file."
    def cmd_SDCARD_LOOP_BEGIN(self, gcmd):
        count = gcmd.get_int("COUNT", minval=0)
        if not self.loop_begin(count):
            raise gcmd.error("Only permitted in SD file.")
    cmd_SDCARD_LOOP_END_help = "Ends a looped section in the SD file."
    def cmd_SDCARD_LOOP_END(self, gcmd):
        if not self.loop_end():
            raise gcmd.error("Only permitted in SD file.")
    cmd_SDCARD_LOOP_DESIST_help = "Stops iterating the current loop stack."
    def cmd_SDCARD_LOOP_DESIST(self, gcmd):
        if not self.loop_desist():
            raise gcmd.error("Only permitted outside of a SD file.")
    def loop_begin(self, count):
        if not self.sdcard.is_cmd_from_sd():
            # Can only run inside of an SD file
            return False
        self.loop_stack.append((count, self.sdcard.get_file_position()))
        return True
    def loop_end(self):
        if not self.sdcard.is_cmd_from_sd():
            # Can only run inside of an SD file
            return False
        # If the stack is empty, no need to skip back
        if len(self.loop_stack) == 0:
            return True
        # Get iteration count and return position
        count, position = self.loop_stack.pop()
        if count == 0: # Infinite loop
            self.sdcard.set_file_position(position)
            self.loop_stack.append((0, position))
        elif count == 1: # Last repeat
            # Nothing to do
            pass
        else:
            # At the next opportunity, seek back to the start of the loop
            self.sdcard.set_file_position(position)
            # Decrement the count by 1, and add the position back to the stack
            self.loop_stack.append((count - 1, position))
        return True
    def loop_desist(self):
        if self.sdcard.is_cmd_from_sd():
            # Can only run outside of an SD file
            return False
        logging.info("Desisting existing SD loops")
        self.loop_stack = []
        return True

def load_config(config):
    return SDCardLoop(config)
