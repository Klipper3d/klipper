# Minimal Hangprinter compatibility helpers for RepRapFirmware-style M569 gcodes.
#
# Copyright (C) 2026
#
# This file may be distributed under the terms of the GNU GPLv3 license.


class HangprinterM569:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('M569.3', self.cmd_M569_3,
                                    desc=self.cmd_M569_3_help)

    cmd_M569_3_help = "Read motor encoder positions (placeholder for terminal-side simulation bridge)"

    def cmd_M569_3(self, gcmd):
        if gcmd.get('P', None) is None:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        # The terminal bridge overrides this placeholder reply with simulated
        # encoder angles when hp-sim is attached over gcode_ws.
        gcmd.respond_raw("Error: M569.3: Message not received")


def load_config(config):
    return HangprinterM569(config)
