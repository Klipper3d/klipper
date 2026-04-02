# Sourcing strategy for built in VGA font (standard behaviour)
#
# Copyright (C) 2036  Christian Hofbauer <chof@gmx.at>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from .strategy import FontDataObject, FontSourceStrategy, FontStrategyError
from ..font8x14 import VGA_FONT

class BuiltinVGAFontSource(FontSourceStrategy):
    def __init__(self, fallback_codepoint=ord('?')):
        self.font_src = FontDataObject(8, 16, {}, fallback_codepoint)
    #load builtin vga font
    def load(self):
        glyph_rows = {}
        for c, glyph in enumerate(VGA_FONT):
            if (len(glyph) != self.font_src.height):
                raise FontStrategyError(
                    "Buildin font data (font8x14.py) is corrupted at entry %d:"
                    " %d instead of %d bytes!"
                    % (c, len(glyph), self.font_src.height))
            glyph_rows[c] = [b for b in bytearray(glyph)]
        if (self.font_src.fallback_codepoint not in glyph_rows):
            fallback = ord(' ') if ord(' ') in glyph_rows else None
        else:
            fallback = self.font_src.fallback_codepoint

        if (fallback is None):
            raise FontStrategyError(
                "No fallback character defined neither '%s' nor ' ' available"
                % (self.font_src.fallback_codepoint))
        else:
            self.font_src.fallback_codepoint = fallback
        self.font_src.glyph_rows = glyph_rows
        return self.font_src
