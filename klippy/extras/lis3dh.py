# Support for reading acceleration data from an LIS3DH chip
#
# Copyright (C) 2024 Luke Vuksta <wulfstawulfsta@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import lis2dw

def load_config(config):
    return lis2dw.LIS2DW(config, lis2dw.LIS3DH_TYPE)

def load_config_prefix(config):
    return lis2dw.LIS2DW(config, lis2dw.LIS3DH_TYPE)
