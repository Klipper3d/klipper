# Minimal TJC3224 / DWIN T5L drawing protocol
#
# Copyright (C) 2026 Bernardo Costa
# Based on GPLv3 display work by Joao Pedro Curti, contributors, and odwdinc.
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import struct
import unicodedata


try:
    text_type = unicode
except NameError:
    text_type = str


FRAME_HEAD = 0xaa
FRAME_TAIL = b"\xcc\x33\xc3\x3c"
MAX_TEXT_BYTES = 40


def _as_text(value):
    if isinstance(value, text_type):
        return value
    if isinstance(value, bytes):
        return value.decode("utf-8", "replace")
    return text_type(value)


def utf8_truncate(value, max_bytes=MAX_TEXT_BYTES):
    encoded = _as_text(value).encode("utf-8")
    if len(encoded) <= max_bytes:
        return encoded.decode("utf-8")
    return encoded[:max_bytes].decode("utf-8", "ignore")


def utf8_scroll(value, max_bytes, offset):
    text = _as_text(value)
    if len(text.encode("utf-8")) <= max_bytes:
        return text
    if not text:
        return text
    offset %= len(text)
    rotated = text[offset:] + u"   " + text[:offset]
    return utf8_truncate(rotated, max_bytes)


def safe_text_bytes(value, max_bytes=MAX_TEXT_BYTES):
    # The stock font cannot reliably draw accented Latin glyphs. Normalize
    # only the wire label, leaving filenames, translations and commands intact.
    chars = []
    latin_base = False
    for char in unicodedata.normalize("NFD", _as_text(value)):
        combining = unicodedata.combining(char)
        if combining and latin_base:
            continue
        chars.append(char)
        if not combining:
            latin_base = (u"a" <= char <= u"z" or u"A" <= char <= u"Z")
    text = unicodedata.normalize("NFC", u"".join(chars))
    text = u"".join(ch if ord(ch) >= 32 else u" " for ch in text)
    encoded = utf8_truncate(text, max_bytes).encode("utf-8")
    return encoded.replace(FRAME_TAIL, b"?")


class TJC3224:
    WIDTH = 240
    HEIGHT = 320

    CMD_DRAW_VALUE = 0x14
    CMD_SET_PALETTE = 0x40
    CMD_DRAW_LINE = 0x51
    CMD_CLEAR = 0x52
    CMD_RECTANGLE = 0x59
    CMD_FILL_RECTANGLE = 0x5b
    CMD_BACKLIGHT = 0x5f
    CMD_DRAW_TEXT = 0x98

    FONT_8X16 = 0x02
    FONT_12X24 = 0x03
    FONT_16X32 = 0x04

    def __init__(self, transport):
        self.transport = transport

    def _send(self, payload):
        frame = bytearray([FRAME_HEAD])
        frame.extend(payload)
        frame.extend(FRAME_TAIL)
        self.transport.send(bytes(frame))
        return bytes(frame)

    @staticmethod
    def _byte(value):
        return struct.pack(">B", max(0, min(0xff, int(value))))

    @staticmethod
    def _word(value):
        return struct.pack(">H", max(0, min(0xffff, int(value))))

    @classmethod
    def _x(cls, value):
        return max(0, min(cls.WIDTH - 1, int(value)))

    @classmethod
    def _y(cls, value):
        return max(0, min(cls.HEIGHT - 1, int(value)))

    def set_palette(self, foreground, background):
        payload = bytearray([self.CMD_SET_PALETTE])
        payload.extend(self._word(foreground))
        payload.extend(self._word(background))
        return self._send(payload)

    def set_backlight(self, brightness):
        return self._send(bytearray([self.CMD_BACKLIGHT,
                                     max(0, min(0x40, int(brightness)))]))

    def clear(self, color):
        self.set_palette(0xffff, color)
        return self._send(bytearray([self.CMD_CLEAR]))

    def line(self, color, x0, y0, x1, y1):
        self.set_palette(color, 0)
        payload = bytearray([self.CMD_DRAW_LINE])
        for value in (self._x(x0), self._y(y0), self._x(x1), self._y(y1)):
            payload.extend(self._word(value))
        return self._send(payload)

    def rectangle(self, color, x0, y0, x1, y1, filled=False):
        self.set_palette(color, 0)
        command = self.CMD_FILL_RECTANGLE if filled else self.CMD_RECTANGLE
        payload = bytearray([command])
        for value in (self._x(x0), self._y(y0), self._x(x1), self._y(y1)):
            payload.extend(self._word(value))
        return self._send(payload)

    def text(self, x, y, value, color=0xffff, background=0x0000,
             font=FONT_8X16, show_background=True, max_bytes=MAX_TEXT_BYTES):
        payload = bytearray([self.CMD_DRAW_TEXT])
        payload.extend(self._word(self._x(x)))
        payload.extend(self._word(self._y(y)))
        payload.extend(b"\x00")
        payload.extend(self._byte(0x02 | (0x40 if show_background else 0)))
        payload.extend(self._byte(font))
        payload.extend(self._word(color))
        payload.extend(self._word(background))
        payload.extend(safe_text_bytes(value, max_bytes))
        return self._send(payload)

    def value(self, x, y, value, color=0xffff, background=0x0000,
              font=FONT_8X16, digits=4, decimals=0, signed=True):
        mode = 0x80 | (0x40 if signed else 0) | int(font)
        payload = bytearray([self.CMD_DRAW_VALUE, mode])
        payload.extend(self._word(color))
        payload.extend(self._word(background))
        payload.extend(self._byte(digits))
        payload.extend(self._byte(decimals))
        payload.extend(self._word(self._x(x)))
        payload.extend(self._word(self._y(y)))
        payload.extend(struct.pack(">q", int(value)))
        return self._send(payload)
