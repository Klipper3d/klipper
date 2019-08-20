# Support for custom board pin aliases
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterBoardAliases:
    def __init__(self, config, chip_name):
        ppins = config.get_printer().lookup_object('pins')
        pin_resolver = ppins.get_pin_resolver(chip_name)
        aliases = config.get("aliases").strip()
        if aliases.endswith(','):
            aliases = aliases[:-1]
        parts = [a.split('=', 1) for a in aliases.split(',')]
        for pair in parts:
            if len(pair) != 2:
                raise ppins.error("Unable to parse aliases in %s"
                                  % (config.get_name(),))
            name, value = [s.strip() for s in pair]
            if value.startswith('<') and value.endswith('>'):
                pin_resolver.reserve_pin(name, value)
            else:
                pin_resolver.alias_pin(name, value)

def load_config(config):
    return PrinterBoardAliases(config, "mcu")

def load_config_prefix(config):
    return PrinterBoardAliases(config, config.get_name().split()[1])
