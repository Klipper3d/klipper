# Font sourcing strategy for bdf files
#
# Copyright (C) 2036  Christian Hofbauer <chof@gmx.at>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from .strategy import FontSourceStrategy, FontDataObject, FontStrategyError
from typing import Dict, List

class BdfFontSourcingStrategy(FontSourceStrategy):
  
    def __init__(self, font_file: str, cell_width: int, cell_height: int, charset: str = 'ascii', fallback_codepoint: int = ord('?')):
        self.font_file = font_file
        self.charset = charset
        self.font_src = FontDataObject(cell_width, cell_height, {}, fallback_codepoint)
    # provide a set of allowed codepoints
    def _allowed_codepoints(self):
        if self.charset == 'ascii':
            return set(range(0x20, 0x7F))
        raise FontStrategyError("Unsupported charset '%s'" % (self.charset))
    # parse the single BBX lines: BBX w h xoff yoff
    def _parse_bbx(self, line: str):
        parts = line.split()
        if len(parts) != 5:
          raise FontStrategyError('Invalid BBX line: %s', (line))
        return tuple(int(v) for v in parts[1:5])
    #decode the hex rows to widht bits: BDF row width is encoded by hex chars (multiple of 4 bits)
    def _hex_row_to_width_bits(self, hexrow):
        row_value = int(hexrow, 16)
        row_bits = len(hexrow) * 4
        if (row_bits < self.font_src.width):
            row_value <<= (self.font_src.width - row_bits)
        elif (row_bits > self.font_src.width): 
            row_value >>= (row_bits - self.font_src.width)
        row_bits = self.font_src.width
        return row_value & ((1 << row_bits) - 1)
    #Font Loading Routine
    def load(self)-> FontDataObject:
        # open the font file
        try:
            with open(self.font_file, "r", encoding='ascii', errors='strict') as ff:
                lines = [ ln.rstrip("\n") for ln in ff]
        except OSError as e:
            raise FontStrategyError("Could not open BDF file '%s', error was: %s" % (self.font_file, str(e)))        
        glyph_rows = self._read_lines(lines)
        # check for complete charset
        missing = sorted(c for c in self._allowed_codepoints() if c not in glyph_rows)
        if (missing):
            raise FontStrategyError("BDF file is missing %d of the required glyphs for %s: %s" % (len(missing), self.charset, ", ".join(missing)))
        self.font_src.glyph_rows = glyph_rows
        #define fallback character
        if self.font_src.fallback_codepoint not in glyph_rows:
            self.font_src.fallback_codepoint = ord('?') if ord(' ') in glyph_rows else None
        if self.font_src.fallback_codepoint is None:
            raise FontStrategyError("BDF font misses fallback glyph/character")
        
        return self.font_src
    #iterate through lines and build glyphs
    def _read_lines(self, lines)->Dict[int, List[int]]:
        allowed = self._allowed_codepoints()
        glyph_rows = {}
        i=0
        while (i < len(lines)):
            if not lines[i].startswith("STARTCHAR"):
                i += 1
                continue
            #initialize char block data 
            encoding = None
            bbx = None
            bitmap_rows = []
            in_bitmap = False
            #read main lines
            i += 1
            while (i < len(lines)):
                line = lines[i]
                if line == 'ENDCHAR':
                    #finished with current character
                    break
                if line.startswith('ENCODING'):
                    encoding = int(line.split()[1])
                elif line.startswith('BBX '):
                    bbx = self._parse_bbx(line)
                elif line.startswith('BITMAP'):
                    in_bitmap = True
                elif in_bitmap:
                    bitmap_rows.append(line.strip())
                i += 1
            #finalize font glyph
            if (encoding is not None and encoding in allowed and bbx is not None):
                bw, bh, bx, by = bbx
                if ((bw, bh, bx, by) != (self.font_src.width, self.font_src.height, 0, 0)):
                    raise FontStrategyError("Glyph %d bitmap mismatch of rows=%d expected was %d."
                                            % (encoding, len(bitmap_rows), self.font_src.height))
                glyph_rows[encoding] = [self._hex_row_to_width_bits(r) for r in bitmap_rows]
            i += 1

        return glyph_rows
                

            

