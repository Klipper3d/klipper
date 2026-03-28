# Support for SSD1363 256x128 OLED display
# Based off uc1701 klipper implementation and
# U8G2 orginal implementation for ssd1363
#
# Copyright holders are preserved as i consider this a derivative
# work of both implementations.
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Eric Callahan  <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from .. import bus
from . import font8x14

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

TextGlyphs = { 'right_arrow': b'\x1a', 'degrees': b'\xf8' }

class DisplayBase:
    def __init__(self, io, columns=256, x_offset=0):
        self.send = io.send
        # framebuffers
        self.columns = columns
        self.x_offset = x_offset
        self.vram = [bytearray(self.columns) for i in range(16)]
        self.all_framebuffers = [(self.vram[i], bytearray(b'~'*self.columns), i)
                                 for i in range(16)]
        # Cache fonts and icons in display byte order
        self.font = [self._swizzle_bits(bytearray(c))
                     for c in font8x14.VGA_FONT]
        self.icons = {}
        # Large font mode (2x width and height): 16px wide x 28px tall,
        # giving a 16-column x 4-row grid.  Font is precomputed at init time.
        self._large_font = False
        self.font_large = self._build_font_large()
        self.icons_large = {}
    @staticmethod
    def _double_bits(b):
        # Expand 8 input bits to 16 output bits: bit i -> bits 2i and 2i+1.
        # Uses a sequence of spread-and-mask steps (no loop needed).
        x = b & 0xFF
        x = (x | (x << 4)) & 0x0F0F
        x = (x | (x << 2)) & 0x3333
        x = (x | (x << 1)) & 0x5555
        return (x | (x << 1)) & 0xFFFF
    def _build_font_large(self):
        # Pre-compute 2x scaled glyphs (16px wide x 32px tall in 4 VRAM pages).
        # Source glyph: (top, bot) each 8 bytes = 8 source columns.
        # Output: (p0, p1, p2, p3) each 16 bytes = 16 output columns.
        font_large = []
        for top, bot in self.font:
            p0 = bytearray(16)
            p1 = bytearray(16)
            p2 = bytearray(16)
            p3 = bytearray(16)
            for k in range(8):
                dt = self._double_bits(top[k])   # rows 0-7 -> rows 0-15
                db = self._double_bits(bot[k])   # rows 8-15 -> rows 16-31
                p0[k*2] = p0[k*2+1] = dt & 0xFF
                p1[k*2] = p1[k*2+1] = (dt >> 8) & 0xFF
                p2[k*2] = p2[k*2+1] = db & 0xFF
                p3[k*2] = p3[k*2+1] = (db >> 8) & 0xFF
            font_large.append((p0, p1, p2, p3))
        return font_large
    def _build_icon_large(self, icon_top, icon_bot):
        # Pre-compute 2x scaled icon from 16-byte (top, bot) column arrays.
        # Output: (p0, p1, p2, p3) each 32 bytes = 32 output columns.
        p0 = bytearray(32)
        p1 = bytearray(32)
        p2 = bytearray(32)
        p3 = bytearray(32)
        for k in range(16):
            dt = self._double_bits(icon_top[k])
            db = self._double_bits(icon_bot[k])
            p0[k*2] = p0[k*2+1] = dt & 0xFF
            p1[k*2] = p1[k*2+1] = (dt >> 8) & 0xFF
            p2[k*2] = p2[k*2+1] = db & 0xFF
            p3[k*2] = p3[k*2+1] = (db >> 8) & 0xFF
        return (p0, p1, p2, p3)
    def set_large_font(self, enabled):
        self._large_font = bool(enabled)
    def _swizzle_bits(self, data):
        # Convert from "rows of pixels" format to "columns of pixels"
        top = bot = 0
        for row in range(8):
            spaced = (data[row] * 0x8040201008040201) & 0x8080808080808080
            top |= spaced >> (7 - row)
            spaced = (data[row + 8] * 0x8040201008040201) & 0x8080808080808080
            bot |= spaced >> (7 - row)
        bits_top = [(top >> s) & 0xff for s in range(0, 64, 8)]
        bits_bot = [(bot >> s) & 0xff for s in range(0, 64, 8)]
        return (bytearray(bits_top), bytearray(bits_bot))
    def set_glyphs(self, glyphs):
        for glyph_name, glyph_data in glyphs.items():
            icon = glyph_data.get('icon16x16')
            if icon is not None:
                top1, bot1 = self._swizzle_bits(icon[0])
                top2, bot2 = self._swizzle_bits(icon[1])
                self.icons[glyph_name] = (top1 + top2, bot1 + bot2)
                self.icons_large[glyph_name] = self._build_icon_large(
                    top1 + top2, bot1 + bot2)
    def write_text(self, x, y, data):
        if self._large_font:
            self._write_text_large(x, y, data)
            return
        if x + len(data) > 32:
            data = data[:32 - min(x, 32)]
        pix_x = x * 8
        pix_x += self.x_offset
        page_top = self.vram[y * 2]
        page_bot = self.vram[y * 2 + 1]
        for c in bytearray(data):
            bits_top, bits_bot = self.font[c]
            page_top[pix_x:pix_x+8] = bits_top
            page_bot[pix_x:pix_x+8] = bits_bot
            pix_x += 8
    def _write_text_large(self, x, y, data):
        # Large font path: 16px wide x 28px tall chars, 16 cols x 4 rows grid.
        if x + len(data) > 16:
            data = data[:16 - min(x, 16)]
        pix_x = x * 16 + self.x_offset
        p0 = self.vram[y * 4]
        p1 = self.vram[y * 4 + 1]
        p2 = self.vram[y * 4 + 2]
        p3 = self.vram[y * 4 + 3]
        for c in bytearray(data):
            lp0, lp1, lp2, lp3 = self.font_large[c]
            p0[pix_x:pix_x+16] = lp0
            p1[pix_x:pix_x+16] = lp1
            p2[pix_x:pix_x+16] = lp2
            p3[pix_x:pix_x+16] = lp3
            pix_x += 16
    def write_graphics(self, x, y, data):
        if self._large_font:
            self._write_graphics_large(x, y, data)
            return
        if x >= 32 or y >= 8 or len(data) != 16:
            return
        bits_top, bits_bot = self._swizzle_bits(data)
        pix_x = x * 8
        pix_x += self.x_offset
        page_top = self.vram[y * 2]
        page_bot = self.vram[y * 2 + 1]
        for i in range(8):
            page_top[pix_x + i] ^= bits_top[i]
            page_bot[pix_x + i] ^= bits_bot[i]
    def _write_graphics_large(self, x, y, data):
        # Large font path: each graphic column is 16px wide, 4 VRAM pages tall.
        if x >= 16 or y >= 4 or len(data) != 16:
            return
        top, bot = self._swizzle_bits(data)
        pix_x = x * 16 + self.x_offset
        p0 = self.vram[y * 4]
        p1 = self.vram[y * 4 + 1]
        p2 = self.vram[y * 4 + 2]
        p3 = self.vram[y * 4 + 3]
        for k in range(8):
            dt = self._double_bits(top[k])
            db = self._double_bits(bot[k])
            c0 = dt & 0xFF
            c1 = (dt >> 8) & 0xFF
            c2 = db & 0xFF
            c3 = (db >> 8) & 0xFF
            for j in (k * 2, k * 2 + 1):
                p0[pix_x + j] ^= c0
                p1[pix_x + j] ^= c1
                p2[pix_x + j] ^= c2
                p3[pix_x + j] ^= c3
    def write_glyph(self, x, y, glyph_name):
        if self._large_font:
            return self._write_glyph_large(x, y, glyph_name)
        icon = self.icons.get(glyph_name)
        if icon is not None and x < 31:
            # Draw icon in graphics mode
            pix_x = x * 8
            pix_x += self.x_offset
            page_idx = y * 2
            self.vram[page_idx][pix_x:pix_x+16] = icon[0]
            self.vram[page_idx + 1][pix_x:pix_x+16] = icon[1]
            return 2
        char = TextGlyphs.get(glyph_name)
        if char is not None:
            # Draw character
            self.write_text(x, y, char)
            return 1
        return 0
    def _write_glyph_large(self, x, y, glyph_name):
        # Large font path: Icons are double in size
        icon = self.icons_large.get(glyph_name)
        if icon is not None and x < 15:
            pix_x = x * 16 + self.x_offset
            page_idx = y * 4
            self.vram[page_idx][pix_x:pix_x+32] = icon[0]
            self.vram[page_idx + 1][pix_x:pix_x+32] = icon[1]
            self.vram[page_idx + 2][pix_x:pix_x+32] = icon[2]
            self.vram[page_idx + 3][pix_x:pix_x+32] = icon[3]
            return 2
        char = TextGlyphs.get(glyph_name)
        if char is not None:
            self._write_text_large(x, y, char)
            return 1
        return 0
    def clear(self):
        zeros = bytearray(self.columns)
        for page in self.vram:
            page[:] = zeros
    def get_dimensions(self):
        return (32, 8)

# IO wrapper for "4 wire" spi bus (spi bus with an extra data/control line)
class SPI4wire:
    def __init__(self, config, data_pin_name):
        self.spi = bus.MCU_SPI_from_config(config, 0, default_speed=10000000)
        dc_pin = config.get(data_pin_name)
        self.mcu_dc = bus.MCU_bus_digital_out(self.spi.get_mcu(), dc_pin,
                                              self.spi.get_command_queue())
    def send(self, cmds, is_data=False):
        self.mcu_dc.update_digital_out(is_data,
                                       reqclock=BACKGROUND_PRIORITY_CLOCK)
        self.spi.spi_send(cmds, reqclock=BACKGROUND_PRIORITY_CLOCK)
    def send_cmd(self, cmd):
        self.mcu_dc.update_digital_out(0, reqclock=BACKGROUND_PRIORITY_CLOCK)
        self.spi.spi_send([cmd], reqclock=BACKGROUND_PRIORITY_CLOCK)
    def send_arg(self, arg):
        self.mcu_dc.update_digital_out(1, reqclock=BACKGROUND_PRIORITY_CLOCK)
        self.spi.spi_send([arg], reqclock=BACKGROUND_PRIORITY_CLOCK)
    def send_data(self, data):
        self.mcu_dc.update_digital_out(1, reqclock=BACKGROUND_PRIORITY_CLOCK)
        self.spi.spi_send(data, reqclock=BACKGROUND_PRIORITY_CLOCK)

# IO wrapper for i2c bus
class I2C:
    def __init__(self, config, default_addr):
        self.i2c = bus.MCU_I2C_from_config(config, default_addr=default_addr,
                                           default_speed=400000,
                                           async_write_only=True)
    def send(self, cmds, is_data=False):
        hdr = 0x40 if is_data else 0x00
        cmds = bytearray(cmds)
        cmds.insert(0, hdr)
        self.i2c.i2c_write_noack(cmds, reqclock=BACKGROUND_PRIORITY_CLOCK)
    def send_cmd(self, cmd):
        self.i2c.i2c_write_noack(bytearray([0x00, cmd]),
                                  reqclock=BACKGROUND_PRIORITY_CLOCK)
    def send_arg(self, arg):
        self.i2c.i2c_write_noack(bytearray([0x40, arg]),
                                  reqclock=BACKGROUND_PRIORITY_CLOCK)
    def send_data(self, data):
        data = bytearray(data)
        pos = 0
        while pos < len(data):
            chunk = data[pos:pos + 32]
            self.i2c.i2c_write_noack(bytearray([0x40]) + chunk,
                                      reqclock=BACKGROUND_PRIORITY_CLOCK)
            pos += 32

# Helper code for toggling a reset pin on startup
class ResetHelper:
    def __init__(self, pin_desc, io_bus):
        self.mcu_reset = None
        if pin_desc is None:
            return
        self.mcu_reset = bus.MCU_bus_digital_out(io_bus.get_mcu(), pin_desc,
                                                 io_bus.get_command_queue())
    def init(self):
        if self.mcu_reset is None:
            return
        mcu = self.mcu_reset.get_mcu()
        curtime = mcu.get_printer().get_reactor().monotonic()
        print_time = mcu.estimated_print_time(curtime)
        # Toggle reset
        minclock = mcu.print_time_to_clock(print_time + .100)
        self.mcu_reset.update_digital_out(0, minclock=minclock)
        minclock = mcu.print_time_to_clock(print_time + .200)
        self.mcu_reset.update_digital_out(1, minclock=minclock)
        # Force a delay to any subsequent commands on the command queue
        minclock = mcu.print_time_to_clock(print_time + .300)
        self.mcu_reset.update_digital_out(1, minclock=minclock)


# The SSD1363 supports both i2c and "4-wire" spi
class SSD1363(DisplayBase):
    def __init__(self, config, columns=256):
        cs_pin = config.get("cs_pin", None)
        if cs_pin is None:
            io = I2C(config, 60)
            io_bus = io.i2c
        else:
            io = SPI4wire(config, "dc_pin")
            io_bus = io.spi
        self.reset = ResetHelper(config.get("reset_pin", None), io_bus)
        self.io = io
        # chip_col_offset: SSD1363 chip supports 320px (80 col addrs) but the
        # panel is 256px wide.  Offset 8 centres the 256px panel: (80-64)/2=8.
        # Matches u8g2 default_x_offset=8.  Override via x_offset in [display].
        self.chip_col_offset = config.getint('x_offset', 8, minval=0, maxval=79)
        # effect: selects the greyscale 3D rendering mode.
        # 'emboss' -- raised look
        # 'none'   -- original behaviour
        self._effect = config.get('effect', 'emboss')
        if self._effect not in ('emboss', 'none'):
            raise config.error(
                "effect must be 'emboss' or 'none'")
        DisplayBase.__init__(self, io, columns, 0)
        self.contrast = config.getint('contrast', 239, minval=0, maxval=255)
        self.vcomh = config.getint('vcomh', 0, minval=0, maxval=7)
        self.invert = config.getboolean('invert', False)
    def init(self):
        self.reset.init()
        io = self.io
        # Init sequence synced with u8g2 u8x8_d_ssd1363_256x128_init_seq.
        # Reference: u8g2-src/clib/u8x8_d_ssd1363.c
        #
        # CAD=011 I2C protocol (u8x8_cad_011_ssd13xx_i2c):
        #   command -> own I2C transaction [0x00, cmd]
        #   arg     -> own I2C transaction [0x40, arg]   (one per byte)
        #   data    -> I2C transaction(s)  [0x40, ...data] (chunked at 32 bytes)
        # Unlock display
        io.send_cmd(0xFD); io.send_arg(0x12)
        # Display off
        io.send_cmd(0xAE)
        # Clock divide ratio / oscillator freq
        io.send_cmd(0xB3); io.send_arg(0x30)
        # Multiplex ratio (3..159)
        io.send_cmd(0xCA); io.send_arg(127)
        # Display offset
        io.send_cmd(0xA2); io.send_arg(0x20)
        # Display start line
        io.send_cmd(0xA1); io.send_arg(0x00)
        # Re-Map / Dual COM
        io.send_cmd(0xA0); io.send_arg(0x32); io.send_arg(0x00)
        # Display Enhancement A
        io.send_cmd(0xB4); io.send_arg(0x32); io.send_arg(0x0C)
        # Contrast
        io.send_cmd(0xC1); io.send_arg(self.contrast)
        # Voltage config (Vp cap)
        io.send_cmd(0xBA); io.send_arg(0x03)
        # Linear greyscale table
        io.send_cmd(0xB9)
        # Internal IREF (0x80 = external)
        io.send_cmd(0xAD); io.send_arg(0x90)
        # Phase 1/2 period adjustment
        io.send_cmd(0xB1); io.send_arg(0x74)
        # Pre-charge voltage
        io.send_cmd(0xBB); io.send_arg(0x0C)
        # 2nd pre-charge period
        io.send_cmd(0xB6); io.send_arg(0xC8)
        # VCOMH deselect level
        io.send_cmd(0xBE); io.send_arg(0x04)
        # Invert / normal display
        io.send_cmd(0xA7 if self.invert else 0xA6)
        # Exit partial display
        io.send_cmd(0xA9)
        # Display on
        io.send_cmd(0xAF)
        self.flush()

    def flush(self):
        # Find dirty tile runs and coalesce nearby ones to minimise I2C
        # transactions.  Each run shares a single column-window command and
        # one send_data() call instead of separate commands per tile.
        for new_data, old_data, page in self.all_framebuffers:
            if new_data == old_data:
                continue
            num_tiles = self.columns // 8
            # Collect indices of dirty tiles.
            dirty = [i for i in range(num_tiles)
                     if new_data[i*8:(i+1)*8] != old_data[i*8:(i+1)*8]]
            if not dirty:
                old_data[:] = new_data
                continue
            # Coalesce into runs: merge tile j into current run if the gap is
            # <= 3 tiles (re-sending <= 96 unchanged bytes saves 7 transactions).
            runs = []
            run_start = run_end = dirty[0]
            for j in dirty[1:]:
                if j <= run_end + 3:
                    run_end = j
                else:
                    runs.append((run_start, run_end))
                    run_start = run_end = j
            runs.append((run_start, run_end))
            for run_start, run_end in runs:
                tiles = [bytearray(new_data[i*8:(i+1)*8])
                         for i in range(run_start, run_end + 1)]
                lc = new_data[run_start * 8 - 1] if run_start > 0 else 0
                rc = (new_data[(run_end + 1) * 8]
                      if (run_end + 1) * 8 < self.columns else 0)
                self.draw_tile(run_start, page, tiles, lc, rc)
            old_data[:] = new_data

    def _8to32(self, ptr, left_col=0, right_col=0):
        """Convert 8 bytes of 1bpp tile data to 32 bytes of 4bpp SSD1363 format.

        effect='none'  -- every ON pixel at full brightness 0xF.
        effect='emboss'-- per-pixel brightness from top-left edge detection:
                         highlight(0xF) / interior(0xC) / shadow(0x8).
        left_col / right_col: VRAM column bytes bordering this tile (cross-tile
        edge detection for emboss).
        """
        dest = bytearray(32)
        if not any(ptr):
            return dest
        effect = getattr(self, '_effect', 'none')
        # Column index -> dest byte offset within each 4-byte row group.
        #   col:  0  1  2  3  4  5  6  7
        _bo = (1, 1, 0, 0, 3, 3, 2, 2)
        if effect == 'none':
            # Flat mode: every ON pixel at full brightness (0xF).
            a = 1
            for i in range(8):
                if ptr[0] & a: dest[i*4 + 1] |= 0x0f
                if ptr[1] & a: dest[i*4 + 1] |= 0xf0
                if ptr[2] & a: dest[i*4 + 0] |= 0x0f
                if ptr[3] & a: dest[i*4 + 0] |= 0xf0
                if ptr[4] & a: dest[i*4 + 3] |= 0x0f
                if ptr[5] & a: dest[i*4 + 3] |= 0xf0
                if ptr[6] & a: dest[i*4 + 2] |= 0x0f
                if ptr[7] & a: dest[i*4 + 2] |= 0xf0
                a <<= 1
            return dest
        if effect == 'emboss':
            # Raised look: brightness determined by which edges are exposed.
            for r in range(8):
                mask = 1 << r
                for c in range(8):
                    if not (ptr[c] & mask):
                        continue
                    above = (ptr[c] >> (r - 1)) & 1 if r > 0 else 0
                    below = (ptr[c] >> (r + 1)) & 1 if r < 7 else 0
                    left  = ((left_col  >> r) & 1 if c == 0
                             else (ptr[c - 1] >> r) & 1)
                    right = ((right_col >> r) & 1 if c == 7
                             else (ptr[c + 1] >> r) & 1)
                    if not above or not left:
                        grey = 0xF
                    elif not below or not right:
                        grey = 0x4
                    else:
                        grey = 0x9
                    bo = _bo[c]
                    if c & 1:
                        dest[r * 4 + bo] |= grey << 4
                    else:
                        dest[r * 4 + bo] |= grey
            return dest

        return dest

    def draw_tile(self, x_pos, y_pos, tiles, left_col=0, right_col=0):
        """Write tile data to display (mirrors U8X8_MSG_DISPLAY_DRAW_TILE).
        tiles: list of 8-byte bytearrays, one per tile column.
        left_col / right_col: VRAM column bytes bordering the first/last tile
        (cross-tile edge detection for emboss).
        """
        # each tile = 2 SSD1363 column addresses (8 pixels)
        x = x_pos * 2 + self.chip_col_offset
        # each tile = 8 rows
        y = y_pos * 8
        # Set row address once for all tiles in this row
        # (CAD=011: cmd + 2 args)
        self.io.send_cmd(0x75)
        self.io.send_arg(y)
        self.io.send_arg(y + 7)

        # Set column window to span all tiles, then write-to-RAM once so all
        # tile data streams as a single send_data() call.
        x_end = x + len(tiles) * 2 - 1
        self.io.send_cmd(0x15)              # set column address
        self.io.send_arg(x)                 # start
        self.io.send_arg(x_end)             # end
        self.io.send_cmd(0x5C)              # write to RAM

        converted = []
        for idx, tile in enumerate(tiles):
            lc = tiles[idx - 1][7] if idx > 0 else left_col
            rc = tiles[idx + 1][0] if idx < len(tiles) - 1 else right_col
            converted.append(self._8to32(tile, lc, rc))
        # SSD1363 uses row-major addressing:
        # for each row, all column bytes must be consecutive
        # across tiles before advancing to the next row.
        buf = bytearray()
        for r in range(8):
            for tile_data in converted:
                buf += tile_data[r*4:r*4+4]
        self.io.send_data(buf)
