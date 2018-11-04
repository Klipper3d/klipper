# Support for a heated chamber.
#
# Copyright (C) 2018  John Jardine <john@gprime.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

def load_config(config):
    return config.get_printer().lookup_object('heater').setup_heater(config)
