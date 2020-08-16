# Package definition for the extras/t5uid1 directory
#
# Copyright (C) 2020  Desuuuu <contact@desuuuu.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import t5uid1

def load_config(config):
    return t5uid1.load_config(config)
