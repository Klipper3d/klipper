# Support for stepper direction control via shift register
#
# Copyright (C) 2026
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import shift_register

def load_config_prefix(config):
    return shift_register.ShiftRegisterStepperDir(config)
