# Support for a heated bed
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

def load_config(config):
    pheater = config.get_printer().lookup_object('heater')
    return pheater.setup_heater(config, 'B')
