# Page class
#
# Copyright (C) 2020  Desuuuu <contact@desuuuu.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class T5UID1_Page:
    def __init__(self, var_names, config):
        self.printer = config.get_printer()
        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        self.name = name_parts[1]

        self.id = config.getint('id', minval=0, maxval=255)
        self.is_boot = config.getboolean('boot', False)
        self.is_timeout = config.getboolean('timeout', False)
        self.is_shutdown = config.getboolean('shutdown', False)
        self.var_auto = []
        self.var = []

        for var in config.get('var_auto', '').split(','):
            var = var.strip()
            if len(var) > 0 and var not in self.var_auto:
                if var not in var_names:
                    raise config.error("Invalid var '%s' in section '%s'"
                                       % (var, config.get_name()))
                self.var_auto.append(var)

        for var in config.get('var', '').split(','):
            var = var.strip()
            if (len(var) > 0
                and var not in self.var_auto and var not in self.var):
                if var not in var_names:
                    raise config.error("Invalid var '%s' in section '%s'"
                                       % (var, config.get_name()))
                self.var.append(var)
