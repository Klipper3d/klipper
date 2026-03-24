# Common swizzle strategy for row-major bit organized font data
#
# Copyright (C) 2036  Christian Hofbauer <chof@gmx.at>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from .strategy import FontDataObject, SwizzleStrategy, SwizzleStrategyError
from typing import Dict, List, Tuple
import logging

class PageSwizzleStrategy(SwizzleStrategy):
  #swizzling font data from row major bits to pages for display
  def swizzle_glyph(self, font_data: FontDataObject)-> Tuple[Dict[int, List[bytearray]], int]:
      if (font_data.height % 8 != 0):
          raise SwizzleStrategyError('Invalid font glyph height: %d, expect multiples of 8' % (font_data.height))
      pages = font_data.height // 8
      max_row_mask = (1 << font_data.width) - 1
      #swizzle from horizontal rows of pixels into vetical 8-byte  column of bytes per page
      swizzled = {}
      for c, rows in font_data.glyph_rows.items():
          if (len(rows) != font_data.height):
              raise ValueError("The bitmap for character '%s' has a wrong number of rows: expected %d, got %d."
                              % (chr(c), font_data.height, len(rows)))
          for i, r in enumerate(rows):
              if (r < 0 or r > max_row_mask):
                  raise ValueError("For character '%s'  row %d is out of range: %d"
                                % (chr(c), i, r))
          out = [bytearray(font_data.width) for _ in range(pages)]
          #transform the glyph data from row-major to column-major in the page size
          for p in range(pages):
              y_start = p * 8
              for x in range(font_data.width):
                  b = 0
                  for bit in range(8):
                      y = y_start + bit
                      if ((rows[y] >> (font_data.width - 1 - x)) & 1):
                          b |= (1 << bit)
                  out[p][x] = b
          #assemble result
          swizzled[c] = out

      return (swizzled, pages)

