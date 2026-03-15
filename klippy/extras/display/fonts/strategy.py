# Font building, swizzling and caching strategies
#
# Copyright (C) 2036  Christian Hofbauer <chof@gmx.at>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from dataclasses import dataclass
from typing import Dict, List, TypeAlias

class FontStrategyError(Exception):
    pass

#Class representing a font data object
@dataclass
class FontDataObject:
    width: int
    height: int
    glyph_rows: Dict[int, List[int]]
    fallback_codepoint: int

class FontSourceStrategy(object):
    # Load the font and return font parameters
    def load(self) -> FontDataObject:
        raise NotImplementedError

# Swizzle strategy to convert font files into format needed for the display
class SwizzleStrategy(object):
    # Convert from "rows of pixels" format to display native format
    def swizzle_glyph(self, rows: List[int], width: int, height: int):
        raise NotImplementedError
    
# Font Cache builder class combining logic for Source and Swizzle strategies to build the font cache
class FontCacheBuilder(object):

    def __init__(self, source_strategy: FontSourceStrategy, swizzle_strategy: SwizzleStrategy):
        self.source_strategy = source_strategy
        self.swizzle_strategy = swizzle_strategy
    # building routine sourcing the font and swizzling it into the format for the display    
    def build(self):
        glyphs = {}
        font_src = self.source_strategy.load()
        for cp, rows in font_src.glyph_rows.items():
            glyphs[cp] = self.swizzle_strategy.swizzle_glyph(rows, font_src.width, font_src.height)
  
        return {
            'width': font_src.width,
            'height': font_src.height,
            'glyphs': glyphs,
            'fallback_codepoint': glyphs[font_src.fallback_codepoint]    
        }