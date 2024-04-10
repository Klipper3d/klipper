# Support for a generic heater
#
# Copyright (C) 2019  John Jardine <john@gprime.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

def load_config_prefix(config):
    return config.get_printer().lookup_object('heater').setup_heater(config)
