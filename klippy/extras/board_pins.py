# Support for custom board pin aliases
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterBoardAliases:
    def __init__(self, config):
        ppins = config.get_printer().lookup_object('pins')
        mcu_names = [n.strip() for n in config.get('mcu', 'mcu').split(',')]
        pin_resolvers = [ppins.get_pin_resolver(n) for n in mcu_names]
        options = ["aliases"] + config.get_prefix_options("aliases_")
        for opt in options:
            aliases = config.get(opt, "").strip()
            if not aliases:
                continue
            if aliases.endswith(','):
                aliases = aliases[:-1]
            parts = [a.split('=', 1) for a in aliases.split(',')]
            for pair in parts:
                if len(pair) != 2:
                    raise ppins.error("Unable to parse aliases in %s"
                                      % (config.get_name(),))
                name, value = [s.strip() for s in pair]
                if value.startswith('<') and value.endswith('>'):
                    for pin_resolver in pin_resolvers:
                        pin_resolver.reserve_pin(name, value)
                else:
                    for pin_resolver in pin_resolvers:
                        pin_resolver.alias_pin(name, value)

def load_config(config):
    return PrinterBoardAliases(config)

def load_config_prefix(config):
    return PrinterBoardAliases(config)
