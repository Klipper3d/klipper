# Support for YHCB2004 (20x4 text) LCD displays based on AiP31068 controller
#
# Copyright (C) 2024  Alexander Bazarov <oss@bazarov.dev>
#
# based on : hd44780_spi.py
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Eric Callahan <arksine.code@gmail.com>
# Copyright (C) 2021  Marc-Andre Denis <marcadenis@msn.com>
#
# inspired by https://github.com/red-scorp/LiquidCrystal_AIP31068
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from enum import IntEnum, IntFlag
from .. import bus

LINE_LENGTH_DEFAULT=20
LINE_LENGTH_OPTIONS={16:16, 20:20}

TextGlyphs = { 'right_arrow': b'\x7e' }

class CMND(IntEnum):
    CLR = 1         # Clear display
    HOME = 2        # Return home
    ENTERY_MODE = 2**2     # Entry mode set
    DISPLAY = 2**3  # Display on/off control
    SHIFT = 2**4    # Cursor or display shift
    FUNCTION = 2**5 # Function set
    CGRAM = 2**6    # Character Generator RAM
    DDRAM = 2**7    # Display Data RAM
    WRITE_RAM = 2**8    # Write to RAM

# Define flags for all commands
class flg_ENTERY_MODE(IntFlag):
    INC = 2**1  # Increment
    SHIFT = 2**0    # Shift display

class flg_DISPLAY(IntFlag):
    ON = 2**2   # Display ON
    CURSOR = 2**1  # Cursor ON
    BLINK = 2**0   # Blink ON

class flg_SHIFT(IntFlag):
    WHOLE_DISPLAY = 2**3  # Shift whole display
    RIGHT = 2**2   # Shift right

class flg_FUNCTION(IntFlag):
    TWO_LINES = 2**3    # 2-line display mode
    FIVE_BY_ELEVEN = 2**2   # 5x11 dot character font

class flg_CGRAM(IntFlag):
    MASK = 0b00111111   # CGRAM address mask

class flg_DDRAM(IntFlag):
    MASK = 0b01111111   # DDRAM address mask

class flg_WRITE_RAM(IntFlag):
    MASK = 0b11111111   # Write RAM mask


DISPLAY_INIT_CMNDS= [
    # CMND.CLR - no need as framebuffer will rewrite all
    CMND.HOME, # move cursor to home (0x00)
    CMND.ENTERY_MODE | flg_ENTERY_MODE.INC, # increment cursor and no shift
    CMND.DISPLAY | flg_DISPLAY.ON, # keep cursor and blinking off
    CMND.SHIFT | flg_SHIFT.RIGHT, # shift right curson only
    CMND.FUNCTION | flg_FUNCTION.TWO_LINES, # 2-line display mode, 5x8 dots
]

class aip31068_spi9bit:
    def __init__(self, config):
        self.printer = config.get_printer()
        # spi config
        self.spi = bus.MCU_SPI_from_config(
            config, 0x00, pin_option="latch_pin") # (config, mode, cs_name)
        self.mcu = self.spi.get_mcu()
        self.icons = {}
        self.line_length = config.getchoice('line_length', LINE_LENGTH_OPTIONS,
                                            LINE_LENGTH_DEFAULT)
        # each controller's line is 2 lines on the display and hence twice
        # line length
        self.text_framebuffers = [bytearray(b' '*2*self.line_length),
                                  bytearray(b' '*2*self.line_length)]
        self.glyph_framebuffer = bytearray(64)
        # all_framebuffers - list of tuples per buffer type.
        # Each tuple contains:
        #   1. the updated framebuffer
        #   2. a copy of the old framebuffer == data on the display
        #   3. the command to send to write to this buffer
        # Then flush() will compare new data with data on the display
        # and send only the differences to the display controller
        # and update the old framebuffer with the new data
        # (immutable tuple is allowed to store mutable bytearray)
        self.all_framebuffers = [
            # Text framebuffers
            (self.text_framebuffers[0], bytearray(b'~'*2*self.line_length),
                CMND.DDRAM | (flg_DDRAM.MASK & 0x00) ),
            (self.text_framebuffers[1], bytearray(b'~'*2*self.line_length),
                CMND.DDRAM | (flg_DDRAM.MASK & 0x40) ),
            # Glyph framebuffer
            (self.glyph_framebuffer, bytearray(b'~'*64),
                CMND.CGRAM | (flg_CGRAM.MASK & 0x00) ) ]
    def send(self, data, minclock=0):
        # for elem in data:
        #     self.spi.spi_send(elem, minclock)
        self.spi.spi_send(data, minclock)
    def flush(self):
        # Find all differences in the framebuffers and send them to the chip
        for new_data, old_data, fb_cmnd in self.all_framebuffers:
            if new_data == old_data:
                continue
            # Find the position of all changed bytes in this framebuffer
            diffs = [[i, 1] for i, (n, o) in enumerate(zip(new_data, old_data))
                     if n != o]
            # Batch together changes that are close to each other
            for i in range(len(diffs)-2, -1, -1):
                pos, count = diffs[i]
                nextpos, nextcount = diffs[i+1]
                if pos + 4 >= nextpos and nextcount < 16:
                    diffs[i][1] = nextcount + (nextpos - pos)
                    del diffs[i+1]
            # Transmit changes
            for pos, count in diffs:
                chip_pos = pos
                self.send([fb_cmnd + chip_pos])
                self.send([CMND.WRITE_RAM | byte for byte in
                           new_data[pos:pos+count]])
            old_data[:] = new_data
    def init(self):
        curtime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(curtime)
        for i, cmds in enumerate(DISPLAY_INIT_CMNDS):
            minclock = self.mcu.print_time_to_clock(print_time + i * .100)
            self.send([cmds], minclock=minclock)
        self.flush()
    def write_text(self, x, y, data):
        if x + len(data) > self.line_length:
            data = data[:self.line_length - min(x, self.line_length)]
        pos = x + ((y & 0x02) >> 1) * self.line_length
        self.text_framebuffers[y & 1][pos:pos+len(data)] = data
    def set_glyphs(self, glyphs):
        for glyph_name, glyph_data in glyphs.items():
            data = glyph_data.get('icon5x8')
            if data is not None:
                self.icons[glyph_name] = data
    def write_glyph(self, x, y, glyph_name):
        data = self.icons.get(glyph_name)
        if data is not None:
            slot, bits = data
            self.write_text(x, y, [slot])
            self.glyph_framebuffer[slot * 8:(slot + 1) * 8] = bits
            return 1
        char = TextGlyphs.get(glyph_name)
        if char is not None:
            # Draw character
            self.write_text(x, y, char)
            return 1
        return 0
    def write_graphics(self, x, y, data):
        pass # this display supports only hardcoded or 8 user defined glyphs
    def clear(self):
        spaces = b' ' * 2*self.line_length
        self.text_framebuffers[0][:] = spaces
        self.text_framebuffers[1][:] = spaces
    def get_dimensions(self):
        return (self.line_length, 4)
