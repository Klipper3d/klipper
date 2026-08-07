# Support for custom board pin aliases
#
# Copyright (C) 2019-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterBoardAliases:
    def __init__(self, config):
        ppins = config.get_printer().lookup_object('pins')
        mcu_names = config.getlist('mcu', ('mcu',))
        pin_resolvers = [ppins.get_pin_resolver(n) for n in mcu_names]
        options = ["aliases"] + config.get_prefix_options("aliases_")
        for opt in options:
            aliases = config.getlists(opt, seps=('=', ','), count=2)
            for name, value in aliases:
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
