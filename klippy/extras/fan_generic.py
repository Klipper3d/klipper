# Support fans that are controlled by gcode
#
# Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import fan

def load_config_prefix(config):
    instance = fan.FanControllable(config)
    # Support for suffix only name. The full name is already registered in init.
    instance.register_cmds_for_name(instance.name[len('fan_generic '):])
    return instance
