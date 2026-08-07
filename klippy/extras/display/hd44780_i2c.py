# -*- coding: utf-8 -*-
# Support for HD44780/WS0010 OLED 2004 text displays with PCF8574 I2C Expander
#
# Copyright (C) 2026  Minicom365 <3387910@naver.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import time
from .. import bus

LINE_LENGTH_DEFAULT = 20
LINE_LENGTH_OPTIONS = [16, 20]

TextGlyphs = { 'right_arrow': b'\x7e', 'degrees': b'\xdf' }

class hd44780_i2c:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.hd44780_protocol_init = config.getboolean(
            'hd44780_protocol_init', True)

        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=0x27, async_write_only=True)
        self.mcu = self.i2c.get_mcu()
        self.line_length = config.getchoice(
            'line_length', LINE_LENGTH_OPTIONS, LINE_LENGTH_DEFAULT)

        self.backlight_mask = 0x08
        self.enable_mask = 0x04
        self.command_mask = 0x00
        self.data_mask = 0x01
        self.icons = {}

        # Precalculate 256-byte LUTs for I2C encoding (6 bytes per char)
        self.cmd_lut = bytearray(256 * 6)
        self.data_lut = bytearray(256 * 6)
        for val in range(256):
            high = val & 0xF0
            low = (val << 4) & 0xF0
            for is_data, lut in [(False, self.cmd_lut), (True, self.data_lut)]:
                mask = self.data_mask if is_data else self.command_mask
                mask |= self.backlight_mask
                h_mask = high | mask
                l_mask = low | mask
                offset = val * 6
                lut[offset] = h_mask
                lut[offset+1] = h_mask | self.enable_mask
                lut[offset+2] = h_mask
                lut[offset+3] = l_mask
                lut[offset+4] = l_mask | self.enable_mask
                lut[offset+5] = l_mask

        # 4 lines x line_length text framebuffers
        self.text_framebuffers = [
            bytearray(b' '*self.line_length) for _ in range(4)]
        self.glyph_framebuffer = bytearray(64)

        # Standard Arduino LiquidCrystal_I2C 2004 DDRAM offsets
        self.all_framebuffers = [
            (self.text_framebuffers[0], bytearray(b'~'*self.line_length), 0x80),
            (self.text_framebuffers[1], bytearray(b'~'*self.line_length), 0xc0),
            (self.text_framebuffers[2], bytearray(b'~'*self.line_length), 0x94),
            (self.text_framebuffers[3], bytearray(b'~'*self.line_length), 0xd4),
            (self.glyph_framebuffer, bytearray(b'~'*64), 0x40)
        ]

    def send_data(self, data_bytes):
        max_chunk = 6
        for offset in range(0, len(data_bytes), max_chunk):
            sub_data = data_bytes[offset:offset+max_chunk]
            packet = bytearray(len(sub_data) * 6)
            for i, b in enumerate(sub_data):
                lut_offset = b * 6
                packet[i*6:(i+1)*6] = self.data_lut[lut_offset:lut_offset+6]
            try:
                self.i2c.i2c_write_noack(bytes(packet))
            except Exception as e:
                logging.error("LCD I2C Data Write Error: %s" % (str(e),))

    def send_4_bits(self, cmd, is_data, minclock=0):
        lut = self.data_lut if is_data else self.cmd_lut
        offset = (cmd & 0xF0) * 6
        # _encode_nibble returns 3 bytes, which correspond
        # to the first 3 bytes of the LUT for high nibble
        data = lut[offset:offset+3]
        try:
            self.i2c.i2c_write_noack(bytes(data), minclock)
        except Exception as e:
            logging.error("LCD I2C Write Error: %s" % (str(e),))

    def send(self, cmds, is_data=False):
        if isinstance(cmds, (int, float)):
            cmds = [int(cmds)]
        packet = bytearray(len(cmds) * 6)
        lut = self.data_lut if is_data else self.cmd_lut
        for i, cmd in enumerate(cmds):
            offset = cmd * 6
            packet[i*6:(i+1)*6] = lut[offset:offset+6]
        try:
            self.i2c.i2c_write_noack(bytes(packet))
        except Exception as e:
            logging.error("LCD I2C Write Error: %s" % (str(e),))

    def init(self):
        # 1. Force invalidate all framebuffers so flush() rewrites
        # physical display
        for fb in self.all_framebuffers:
            fb[1][:] = b'\xff' * len(fb[1])

        # 2. Send dummy nibble pulses to clear any half-received
        # out-of-phase 4-bit nibble
        self.send_4_bits(0x00, False)
        time.sleep(0.020)
        self.send_4_bits(0x00, False)
        time.sleep(0.020)

        # 3. HD44780/WS0010 Hardware Reset: Force state machine
        # back to 8-bit mode
        self.send_4_bits(0x30, False)
        time.sleep(0.010)
        self.send_4_bits(0x30, False)
        time.sleep(0.010)
        self.send_4_bits(0x30, False)
        time.sleep(0.010)

        # 4. Set 4-bit interface mode with clean nibble alignment
        self.send_4_bits(0x20, False)
        time.sleep(0.010)

        # Function Set: 4-bit mode, 2/4-line display, 5x8 dots
        self.send(0x28, is_data=False)
        time.sleep(0.005)

        # Display OFF
        self.send(0x08, is_data=False)
        time.sleep(0.005)

        # Clear Display (physical hardware wipe)
        self.send(0x01, is_data=False)
        time.sleep(0.010)

        # Return Home & Unshift Display (resets display shift registers to zero)
        self.send(0x02, is_data=False)
        time.sleep(0.005)

        # Entry Mode Set (Cursor Increment, No Shift)
        self.send(0x06, is_data=False)
        time.sleep(0.005)

        # Display ON (Display ON, Cursor OFF, Blink OFF)
        self.send(0x0C, is_data=False)
        time.sleep(0.005)

        self.flush()

    def flush(self):
        # HD44780 2004 Datasheet Standard DDRAM Addresses
        row_offsets = [0x80, 0xC0, 0x94, 0xD4]
        for row in range(4):
            new_data = self.text_framebuffers[row]
            old_data = self.all_framebuffers[row][1]

            if new_data == old_data:
                continue

            # 1. Send DDRAM Address Command as separate I2C packet
            # (with execution delay)
            self.send(row_offsets[row], is_data=False)

            # 2. Send data bytes in sub-chunks (hardware auto-increments cursor)
            self.send_data(new_data)

            old_data[:] = new_data

        # Custom CGRAM Glyphs
        new_glyph = self.glyph_framebuffer
        old_glyph = self.all_framebuffers[4][1]
        if new_glyph != old_glyph:
            self.send(0x40, is_data=False)
            self.send_data(new_glyph)
            # Restore LCD address counter back to DDRAM Line 0
            self.send(0x80, is_data=False)
            old_glyph[:] = new_glyph

    def _write_test_pattern(self):
        test_glyphs = {
            'bed': {
                'icon5x8': (0, [0x00, 0x00, 0x1f, 0x0a, 0x0a, 0x1f, 0x00, 0x00])
            },
            'extruder': {
                'icon5x8': (1, [0x04, 0x0e, 0x1f, 0x04, 0x04, 0x0e, 0x00, 0x00])
            },
            'fan': {
                'icon5x8': (2, [0x0c, 0x0d, 0x06, 0x18, 0x16, 0x06, 0x00, 0x00])
            },
        }
        self.set_glyphs(test_glyphs)

        self._raw_write_text(0, 0, "[KLIPPER 2004 TEST ]")
        self._raw_write_text(0, 1, "Full Text & ")
        self._raw_write_glyph(12, 1, 'bed')
        self._raw_write_glyph(13, 1, 'extruder')
        self._raw_write_glyph(14, 1, 'fan')
        self._raw_write_text(17, 1, "OK!")
        self._raw_write_text(0, 2, "1234567890ABCDEFGHIJ")
        self._raw_write_text(0, 3, "####################")

    def _raw_write_text(self, x, y, data):
        if isinstance(data, (bytes, bytearray)):
            try:
                s = data.decode('utf-8')
                s = s.replace('°', '\xdf').replace('→', '\x7e')
                data = s.encode('latin1', errors='replace')
            except Exception:
                pass
        elif isinstance(data, str):
            s = data.replace('°', '\xdf').replace('→', '\x7e')
            data = s.encode('latin1', errors='replace')
        elif isinstance(data, list):
            data = bytes(data)

        if y < 0 or y > 3:
            return
        if x + len(data) > self.line_length:
            data = data[:self.line_length - min(x, self.line_length)]
        self.text_framebuffers[y][x:x+len(data)] = data

    def write_text(self, x, y, data):
        self._raw_write_text(x, y, data)

    def set_glyphs(self, glyphs):
        for glyph_name, glyph_data in glyphs.items():
            if isinstance(glyph_data, tuple):
                self.icons[glyph_name] = glyph_data
                slot, bits = glyph_data
                self.glyph_framebuffer[slot * 8:(slot + 1) * 8] = bits
                continue
            if isinstance(glyph_data, dict):
                data = glyph_data.get('icon5x8')
                if data is not None:
                    self.icons[glyph_name] = data
                    slot, bits = data
                    self.glyph_framebuffer[slot * 8:(slot + 1) * 8] = bits

    def _raw_write_glyph(self, x, y, glyph_name):
        data = self.icons.get(glyph_name)
        if data is not None:
            slot, bits = data
            self._raw_write_text(x, y, [slot])
            self.glyph_framebuffer[slot * 8:(slot + 1) * 8] = bits
            return 1
        char = TextGlyphs.get(glyph_name)
        if char is not None:
            self._raw_write_text(x, y, char)
            return 1
        return 0

    def write_glyph(self, x, y, glyph_name):
        return self._raw_write_glyph(x, y, glyph_name)

    def write_graphics(self, x, y, data):
        pass

    def clear(self):
        spaces = b' ' * self.line_length
        for i in range(4):
            self.text_framebuffers[i][:] = spaces

    def get_dimensions(self):
        return (self.line_length, 4)

def load_code(config):
    return hd44780_i2c(config)
