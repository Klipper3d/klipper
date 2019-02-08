# Support for a heated bed
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

def load_config(config):
    return config.get_printer().lookup_object('heater').setup_heater(config, 'B')
