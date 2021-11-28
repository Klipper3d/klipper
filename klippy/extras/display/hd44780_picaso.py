# Support for HD44780 (20x4 text) LCD displays connected through i2c.
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Eric Callahan <arksine.code@gmail.com>
# Copyright (C) 2021  Marc-Andre Denis <marcadenis@msn.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from .. import bus

LINE_LENGTH_DEFAULT=20
LINE_LENGTH_OPTIONS={16:16, 20:20}

TextGlyphs = { 'right_arrow': b'\x7e' }

class hd44780_picaso:
    def __init__(self, config):
        # i2c config
        self.i2c1 = bus.MCU_I2C_from_config(config, default_addr=56, default_speed=400000)
        self.i2c2 = bus.MCU_I2C_from_config(config, default_addr=57, default_speed=400000)
        self.mcu = self.i2c1.get_mcu()

        self.data_mask    = 0b01000000
        self.command_mask = 0b00000000
        self.enable_mask  = 0b00010000

        self.icons = {}
        self.line_length = config.getchoice('line_length', LINE_LENGTH_OPTIONS,
                                            LINE_LENGTH_DEFAULT)

        # framebuffers
        self.text_framebuffers = [bytearray(b'1'*2*self.line_length),
                                  bytearray(b'1'*2*self.line_length)]
        self.glyph_framebuffer = bytearray(64)
        self.all_framebuffers = [
            # Text framebuffers
            (self.text_framebuffers[0], bytearray(b'~'*2*self.line_length),
             0x80),
            (self.text_framebuffers[1], bytearray(b'~'*2*self.line_length),
             0xc0),
            # Glyph framebuffer
            (self.glyph_framebuffer, bytearray(b'~'*64), 0x40) ]

        # Clocks between sending update to display.
        # Values lower than 0.008 result to artifacts on display
        self.clock_step = 0.008
        self.printer = config.get_printer()
        # One update on display requires 2 packets. This is delay between them
        self.enable_command_delay = self.mcu.print_time_to_clock(self.clock_step/2)
        curtime = self.printer.get_reactor().monotonic()
        self.last_send_time = self.mcu.estimated_print_time(curtime)
    def send_8_bits(self, cmd, is_data, minclock):
        if is_data:
            mask = self.data_mask
        else:
            mask = self.command_mask
        # There are 2 i2c chips. We need manually sync them
        self.i2c2.i2c_write([cmd], minclock=minclock)
        self.i2c1.i2c_write([mask | self.enable_mask], minclock)
        self.i2c1.i2c_write([mask], minclock=minclock+self.enable_command_delay)
    def send(self, cmds, is_data=False):
        curtime = self.printer.get_reactor().monotonic()
        new_last_send_time = self.mcu.estimated_print_time(curtime)
        if(new_last_send_time > self.last_send_time):
            self.last_send_time = new_last_send_time

        for data in cmds:
            minclock = self.mcu.print_time_to_clock(self.last_send_time)
            self.send_8_bits(data, is_data, minclock)
            self.last_send_time = self.last_send_time + self.clock_step
    def flush(self):
        # Find all differences in the framebuffers and send them to the chip
        for new_data, old_data, fb_id in self.all_framebuffers:
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
                self.send([fb_id + chip_pos])
                self.send(new_data[pos:pos+count], is_data=True)
            old_data[:] = new_data
    def init(self):
        # Program 8bit / 2-line mode 
        # "Home" command
        # Reset (set positive direction ; enable display and hide cursor)
        self.send([0x38, 0x02, 0x06, 0x0c])
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
        pass
    def clear(self):
        spaces = b' ' * 2*self.line_length
        self.text_framebuffers[0][:] = spaces
        self.text_framebuffers[1][:] = spaces
    def get_dimensions(self):
        return (self.line_length, 4)
