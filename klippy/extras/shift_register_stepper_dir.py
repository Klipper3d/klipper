# Support for stepper direction control via shift register
#
# Copyright (C) 2026
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from .shift_register import ShiftRegisterStepperDir

def load_config_prefix(config):
    return ShiftRegisterStepperDir(config)
