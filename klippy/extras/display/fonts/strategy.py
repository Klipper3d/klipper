# Font building, swizzling and caching strategies
#
# Copyright (C) 2036  Christian Hofbauer <chof@gmx.at>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class FontStrategyError(Exception):
    pass

class SwizzleStrategyError(Exception):
    pass

#Class representing a font data object
class FontDataObject(object):
    __slots__ = ('width', 'height', 'glyph_rows', 'fallback_codepoint')

    def __init__(self, width, height, glyph_rows, fallback_codepoint):
        self.width = width
        self.height = height
        self.glyph_rows = glyph_rows
        self.fallback_codepoint = fallback_codepoint

class FontObject (object):
    __slots__ = ('width', 'height', 'pages', 'glyphs', 'fallback')

    def __init__(self, width, height, pages, glyphs, fallback):
        self.width = width
        self.height = height
        self.pages = pages
        self.glyphs = glyphs
        self.fallback = fallback

    def __call__(self, c):
        return self.glyphs.get(c, self.fallback)



class FontSourceStrategy(object):
    # Load the font and return font parameters
    def load(self):
        raise NotImplementedError

# Swizzle strategy to convert font files into format needed for the display
class SwizzleStrategy(object):
    # Convert from "rows of pixels" format to display native format
    def swizzle_glyph(self, font_data):
        raise NotImplementedError

# Font Cache builder class combining logic for Source and Swizzle strategies
# to build the font cache
class FontCacheBuilder(object):

    def __init__(self, source_strategy, swizzle_strategy):
        self.source_strategy = source_strategy
        self.swizzle_strategy = swizzle_strategy
    # sourcing the font and swizzling it into the format for the display
    def build(self):
        font_src = self.source_strategy.load()
        glyphs, pages = self.swizzle_strategy.swizzle_glyph(font_src)
        return FontObject(
            width = font_src.width,
            height = font_src.height,
            pages = pages,
            glyphs = glyphs,
            fallback=glyphs[font_src.fallback_codepoint]
        )
