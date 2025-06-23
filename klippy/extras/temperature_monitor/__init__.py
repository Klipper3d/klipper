# Support generic temperature monitor
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from . import tmc

MONITORS = {
    'tmc': tmc.TMCMonitor
}

def load_config_prefix(config):
    printer = config.get_printer()
    monitor = config.getchoice('monitor', MONITORS)(config)

    pheater = printer.lookup_object("heaters")
    pheater.register_monitor(config)
    return monitor
