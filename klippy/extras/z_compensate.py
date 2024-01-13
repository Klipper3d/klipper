# prtouch support
#
# Copyright (C) 2018-2021  Creality <wangyulong878@sina.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import z_compensate_wrapper

def load_config(config):
    vrt = z_compensate_wrapper.ZCompensateInitWrapper(config)
    # config.get_printer().add_object('probe', probe.PrinterProbe(config, vrt))
    return vrt
