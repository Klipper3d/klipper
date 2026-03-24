# Font building, swizzling and caching strategies
#
# Copyright (C) 2036  Christian Hofbauer <chof@gmx.at>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from dataclasses import dataclass
from typing import Dict, List, TypeAlias

class FontStrategyError(Exception):
    pass

class SwizzleStrategyError(Exception):
    pass

#Class representing a font data object
@dataclass
class FontDataObject:
    width: int
    height: int
    glyph_rows: Dict[int, List[int]]
    fallback_codepoint: int
@dataclass
class FontObject:
    width: int
    height: int
    pages: int
    glyphs: Dict[int, List[bytearray]]
    fallback: List[bytearray]
    #return a character or fallback
    def __call__(self, c: int):
        return self.glyphs.get(c, self.fallback)



class FontSourceStrategy(object):
    # Load the font and return font parameters
    def load(self) -> FontDataObject:
        raise NotImplementedError

# Swizzle strategy to convert font files into format needed for the display
class SwizzleStrategy(object):
    # Convert from "rows of pixels" format to display native format
    def swizzle_glyph(self, font_data: FontDataObject) -> Dict[int, List[bytearray]]:
        raise NotImplementedError
    
# Font Cache builder class combining logic for Source and Swizzle strategies to build the font cache
class FontCacheBuilder(object):

    def __init__(self, source_strategy: FontSourceStrategy, swizzle_strategy: SwizzleStrategy):
        self.source_strategy = source_strategy
        self.swizzle_strategy = swizzle_strategy
    # building routine sourcing the font and swizzling it into the format for the display    
    def build(self) ->FontObject:
        font_src = self.source_strategy.load()
        glyphs, pages = self.swizzle_strategy.swizzle_glyph(font_src)
        return FontObject(
            width = font_src.width,
            height = font_src.height,
            pages = pages,
            glyphs = glyphs,
            fallback=glyphs[font_src.fallback_codepoint]    
        )