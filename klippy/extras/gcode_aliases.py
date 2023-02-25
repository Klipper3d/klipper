# Add ability to define custom g-code aliases
#
# Copyright (C) 2023  Andreas Sandberg <andreas@sandberg.uk>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class GCodeAliases:
    def __init__(self, config):
        printer = config.get_printer()
        gcode = printer.lookup_object('gcode')
        options = ["aliases"] + config.get_prefix_options("aliases_")
        for opt in options:
            aliases = config.getlists(opt, seps=('=', ','), count=2)
            for name, value in aliases:
                gcode.register_alias(name.upper(), value)

def load_config_prefix(config):
    return GCodeAliases(config)

def load_config(config):
    return GCodeAliases(config)
