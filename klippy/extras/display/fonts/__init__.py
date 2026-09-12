# Package definition for the fonts directory
#
# Copyright (C) 2036  Christian Hofbauer <chof@gmx.at>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os
from .strategy import (
    FontCacheBuilder,
    FontStrategyError,
    FontSourceStrategy,
    SwizzleStrategy)
from .sourcing_builtin_vga import BuiltinVGAFontSource
from .sourcing_bdf import BdfFontSource
from .swizzle import PageSwizzleStrategy
import logging

DISPLAY_FONT_SourcingStrategies = {
  'bdf' : BdfFontSource,
}

#Storage of [display_font my_font] sections (one instance per font name)
class DisplayFont:
    def __init__(self, config):
        self.printer = config.get_printer()
        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        self.name = name_parts[1]
        self.format = config.getchoice(
            'format',
            list(DISPLAY_FONT_SourcingStrategies.keys()), default='bdf')
        self.font_file = self._resolve_font_file(config.get('font_file'))
        self.cell_width = config.getint('cell_width', minval=1)
        self.cell_height = config.getint('cell_height', minval=1)
        self.baseline = config.getint('baseline', minval=0)
        self.charset = config.getchoice('charset', ['ascii'], default='ascii')
        self.rows_override = config.getint('rows_override', None, minval=1)

        if self.baseline >= self.cell_height:
            raise config.error(
                "Option 'baseline' in section '%s' must be less than "
                "'cell_height'"
                % (config.get_name()))
        if not os.path.exists(self.font_file):
            raise config.error(
                "Option 'font_file' in section '%s' does not exist: '%s'"
                % (config.get_name(), self.font_file))
        logging.debug("Loading font from: %s" % (self.font_file))

    def _resolve_font_file(self, font_file):
        path = os.path.expanduser(font_file)
        if os.path.isabs(path):
            return path
        cfg_file = self.printer.get_start_args()['config_file']
        cfg_dir = os.path.dirname(os.path.abspath(cfg_file))
        return os.path.normpath(os.path.join(cfg_dir, path))
