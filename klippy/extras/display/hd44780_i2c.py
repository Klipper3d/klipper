# Support for HD44780 I2C (PF8574) (16x2 and 20x4) LCD displays
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Eric Callahan <arksine.code@gmail.com>
# Copyright (C) 2021  Marc-Andre Denis <marcadenis@msn.com>
# Copyright (C) 2023  JR Lomas <lomas.jr@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import time
from .. import bus

BACKGROUND_PRIORITY_CLOCK = 0x7FFFFFFF00000000

LINE_LENGTH_DEFAULT = 20
LINE_LENGTH_OPTIONS = {16: 16, 20: 20}
LINE_NUMBER_OPTIONS = {2: 2, 4: 4}
LINE_NUMBER_DEFAULT = 4

TextGlyphs = {"right_arrow": b"\x7e"}


class I2C:
    def __init__(self, config, default_addr):
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=default_addr, default_speed=400000
        )

    def send(self, cmds, is_data=False, minclock=0):
        cmds = bytearray(cmds)
        self.i2c.i2c_write(
            cmds, minclock=minclock, reqclock=BACKGROUND_PRIORITY_CLOCK
        )


class hd44780_i2c:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.bus = I2C(config, 0x27)
        self.i2c = self.bus.i2c
        self.mcu = self.i2c.get_mcu()
        self.data_mask = 1
        self.command_mask = 0
        self.enable_mask = 0b00000100
        self.backlight_mask = 0x08
        self.flush_count = 0
        self.upper = True

        self.icons = {}
        self.line_length = config.getchoice(
            "line_length", LINE_LENGTH_OPTIONS, LINE_LENGTH_DEFAULT
        )

        self.number_of_lines = config.getchoice(
            "number_of_lines", LINE_NUMBER_OPTIONS, LINE_NUMBER_DEFAULT
        )

        # framebuffers
        self.text_framebuffers = [
            bytearray(b" " * 2 * self.line_length),
            bytearray(b" " * 2 * self.line_length),
        ]
        self.glyph_framebuffer = bytearray(64)
        self.all_framebuffers = [
            # Text framebuffers
            (
                self.text_framebuffers[0],
                bytearray(b"~" * 2 * self.line_length),
                0x80,
            ),
            (
                self.text_framebuffers[1],
                bytearray(b"~" * 2 * self.line_length),
                0xC0,
            ),
            # Glyph framebuffer
            (self.glyph_framebuffer, bytearray(b"~" * 64), 0x40),
        ]

    def send_4_bits(self, cmd, is_data, minclock):
        if is_data:
            mask = self.data_mask
        else:
            mask = self.command_mask
        self.bus.send([(cmd & 0xF0) | mask | self.backlight_mask], minclock)
        self.bus.send(
            [(cmd & 0xF0) | mask | self.backlight_mask | self.enable_mask],
            minclock,
        )
        self.bus.send([(cmd & 0xF0) | mask | self.backlight_mask], minclock)

    def send(self, cmds, is_data=False, minclock=0):
        for data in cmds:
            self.send_4_bits(data, is_data, minclock)
            self.send_4_bits(data << 4, is_data, minclock)

    def redraw_screen(self):
        if self.number_of_lines > 2 or self.upper:
            for idx, c in enumerate(self.text_framebuffers[0]):
                if idx < self.line_length:
                    self.send([0x80 + idx], False)
                    self.send([c], True)
            for idx, c in enumerate(self.text_framebuffers[1]):
                if idx < self.line_length:
                    self.send([0xC0 + idx], False)
                    self.send([c], True)
        else:
            for idx, c in enumerate(self.text_framebuffers[0]):
                if idx >= self.line_length:
                    self.send([0x80 + idx - self.line_length], False)
                    self.send([c], True)
            for idx, c in enumerate(self.text_framebuffers[1]):
                if idx >= self.line_length:
                    self.send([0xC0 + idx - self.line_length], False)
                    self.send([c], True)

    def flush(self, is_menu=False):
        if self.number_of_lines == 2:
            if not is_menu:
                self.flush_count += 1
                if self.flush_count % 10 == 0:
                    if self.upper:
                        self.upper = False
                        self.redraw_screen()
                    else:
                        self.upper = True
                        self.redraw_screen()
            elif not self.upper:
                self.upper = True
                self.redraw_screen()

        # Find all differences in the framebuffers and send them to the chip
        for new_data, old_data, fb_id in self.all_framebuffers:
            if new_data == old_data:
                continue
            # Find the position of all changed bytes in this framebuffer
            diffs = [
                [i, 1]
                for i, (n, o) in enumerate(zip(new_data, old_data))
                if n != o
            ]
            # Batch together changes that are close to each other
            for i in range(len(diffs) - 2, -1, -1):
                pos, count = diffs[i]
                nextpos, nextcount = diffs[i + 1]
                if pos + 4 >= nextpos and nextcount < self.number_of_lines:
                    diffs[i][1] = nextcount + (nextpos - pos)
                    del diffs[i + 1]
            # Transmit changes
            for pos, count in diffs:
                chip_pos = pos
                if self.number_of_lines == 2:
                    if fb_id != 0x40 and (
                        (self.upper and chip_pos >= self.line_length)
                        or ((not self.upper) and chip_pos < self.line_length)
                    ):
                        continue
                    elif (fb_id == 0x80 or fb_id == 0xC0) and not self.upper:
                        chip_pos = chip_pos - self.line_length
                self.send([fb_id + chip_pos])
                self.send(new_data[pos : pos + count], is_data=True)
            old_data[:] = new_data

    def init(self):
        curtime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(curtime)
        # Program 4bit / 1-line mode and then issue 0x02 "Home" command
        init = [[0x33], [0x32], [0x06], [0x0C], [0x28], [0x01]]
        for i, cmds in enumerate(init):
            minclock = self.mcu.print_time_to_clock(print_time + i * 0.100)
            self.send(cmds, minclock=minclock)
        curtime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(curtime)
        time.sleep(1)
        self.flush()

    def write_text(self, x, y, data):
        if x + len(data) > self.line_length:
            data = data[: self.line_length - min(x, self.line_length)]
        pos = x + ((y & 0x02) >> 1) * self.line_length
        self.text_framebuffers[y & 1][pos : pos + len(data)] = data

    def set_glyphs(self, glyphs):
        for glyph_name, glyph_data in glyphs.items():
            data = glyph_data.get("icon5x8")
            if data is not None:
                self.icons[glyph_name] = data

    def write_glyph(self, x, y, glyph_name):
        data = self.icons.get(glyph_name)
        if data is not None:
            slot, bits = data
            self.write_text(x, y, [slot])
            self.glyph_framebuffer[slot * 8 : (slot + 1) * 8] = bits
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
        spaces = b" " * 2 * self.line_length
        self.text_framebuffers[0][:] = spaces
        self.text_framebuffers[1][:] = spaces

    def get_dimensions(self):
        return (self.line_length, self.number_of_lines)
