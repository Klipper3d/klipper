# Basic LCD display support
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Aleph Objects, Inc <marcio@alephobjects.com>
# Copyright (C) 2018  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000


######################################################################
# HD44780 (20x4 text) lcd chip
######################################################################

HD44780_DELAY = .000037

class HD44780:
    char_right_arrow = '\x7e'
    char_thermometer = '\x00'
    char_heater_bed = '\x01'
    char_speed_factor = '\x02'
    char_clock = '\x03'
    char_degrees = '\x04'
    char_usb = '\x05'
    char_sd = '\x06'
    def __init__(self, config):
        self.printer = config.get_printer()
        # pin config
        ppins = self.printer.lookup_object('pins')
        pins = [ppins.lookup_pin('digital_out', config.get(name + '_pin'))
                for name in ['rs', 'e', 'd4', 'd5', 'd6', 'd7']]
        mcu = None
        for pin_params in pins:
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("hd44780 all pins must be on same mcu")
            mcu = pin_params['chip']
            if pin_params['invert']:
                raise ppins.error("hd44780 can not invert pin")
        self.pins = [pin_params['pin'] for pin_params in pins]
        self.mcu = mcu
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_object(self)
        self.send_data_cmd = self.send_cmds_cmd = None
        # framebuffers
        self.text_framebuffer = (bytearray(' '*80), bytearray('~'*80), 0x80)
        self.glyph_framebuffer = (bytearray(64), bytearray('~'*64), 0x40)
        self.framebuffers = [self.text_framebuffer, self.glyph_framebuffer]
    def build_config(self):
        self.mcu.add_config_cmd(
            "config_hd44780 oid=%d rs_pin=%s e_pin=%s"
            " d4_pin=%s d5_pin=%s d6_pin=%s d7_pin=%s delay_ticks=%d" % (
                self.oid, self.pins[0], self.pins[1],
                self.pins[2], self.pins[3], self.pins[4], self.pins[5],
                self.mcu.seconds_to_clock(HD44780_DELAY)))
        cmd_queue = self.mcu.alloc_command_queue()
        self.send_cmds_cmd = self.mcu.lookup_command(
            "hd44780_send_cmds oid=%c cmds=%*s", cq=cmd_queue)
        self.send_data_cmd = self.mcu.lookup_command(
            "hd44780_send_data oid=%c data=%*s", cq=cmd_queue)
    def send(self, cmds, is_data=False):
        cmd_type = self.send_cmds_cmd
        if is_data:
            cmd_type = self.send_data_cmd
        cmd_type.send([self.oid, cmds], reqclock=BACKGROUND_PRIORITY_CLOCK)
        #logging.debug("hd44780 %d %s", is_data, repr(cmds))
    def flush(self):
        # Find all differences in the framebuffers and send them to the chip
        for new_data, old_data, fb_id in self.framebuffers:
            if new_data == old_data:
                continue
            # Find the position of all changed bytes in this framebuffer
            diffs = [[i, 1] for i, (nd, od) in enumerate(zip(new_data, old_data))
                     if nd != od]
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
                if fb_id == 0x80 and pos >= 40:
                    chip_pos += 0x40 - 40
                self.send([fb_id + chip_pos])
                self.send(new_data[pos:pos+count], is_data=True)
            old_data[:] = new_data
    def init(self):
        curtime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(curtime)
        # Program 4bit / 2-line mode and then issue 0x02 "Home" command
        init = [[0x33], [0x33], [0x33, 0x22, 0x28, 0x02]]
        # Reset (set positive direction ; enable display and hide cursor)
        init.append([0x06, 0x0c])
        for i, cmds in enumerate(init):
            minclock = self.mcu.print_time_to_clock(print_time + i * .100)
            self.send_cmds_cmd.send([self.oid, cmds], minclock=minclock)
        # Add custom fonts
        self.glyph_framebuffer[0][:len(HD44780_chars)] = HD44780_chars
        for i in range(len(self.glyph_framebuffer[0])):
            self.glyph_framebuffer[1][i] = self.glyph_framebuffer[0][i] ^ 1
        self.flush()
    def write_text(self, x, y, data):
        if x + len(data) > 20:
            data = data[:20 - min(x, 20)]
        pos = [0, 40, 20, 60][y] + x
        self.text_framebuffer[0][pos:pos+len(data)] = data
    def clear(self):
        self.text_framebuffer[0][:] = ' '*80

HD44780_chars = [
    # Thermometer
    0b00100,
    0b01010,
    0b01010,
    0b01010,
    0b01010,
    0b10001,
    0b10001,
    0b01110,
    # Heated bed
    0b00000,
    0b11111,
    0b10101,
    0b10001,
    0b10101,
    0b11111,
    0b00000,
    0b00000,
    # Speed factor
    0b11100,
    0b10000,
    0b11000,
    0b10111,
    0b00101,
    0b00110,
    0b00101,
    0b00000,
    # Clock
    0b00000,
    0b01110,
    0b10011,
    0b10101,
    0b10001,
    0b01110,
    0b00000,
    0b00000,
    # Degrees
    0b01100,
    0b10010,
    0b10010,
    0b01100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    # USB
    0b01110,
	0b01110,
	0b01110,
	0b11111,
	0b11111,
	0b11111,
	0b00100,
	0b00100,
    # SD
    0b00000,
	0b00111,
	0b01111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b00000,
]


######################################################################
# ST7920 (128x64 graphics) lcd chip
######################################################################

# Spec says 72us, but faster is possible in practice
ST7920_CMD_DELAY  = .000020
ST7920_SYNC_DELAY = .000045

class ST7920:
    char_right_arrow = '\x1a'
    def __init__(self, config):
        printer = config.get_printer()
        # pin config
        ppins = printer.lookup_object('pins')
        pins = [ppins.lookup_pin('digital_out', config.get(name + '_pin'))
                for name in ['cs', 'sclk', 'sid']]
        mcu = None
        for pin_params in pins:
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("st7920 all pins must be on same mcu")
            mcu = pin_params['chip']
            if pin_params['invert']:
                raise ppins.error("st7920 can not invert pin")
        self.pins = [pin_params['pin'] for pin_params in pins]
        self.mcu = mcu
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_object(self)
        self.send_data_cmd = self.send_cmds_cmd = None
        self.is_extended = False
        # framebuffers
        self.text_framebuffer = (bytearray(' '*64), bytearray('~'*64), 0x80)
        self.glyph_framebuffer = (bytearray(128), bytearray('~'*128), 0x40)
        self.graphics_framebuffers = [(bytearray(32), bytearray('~'*32), i)
                                      for i in range(32)]
        self.framebuffers = ([self.text_framebuffer, self.glyph_framebuffer]
                             + self.graphics_framebuffers)
    def build_config(self):
        self.mcu.add_config_cmd(
            "config_st7920 oid=%u cs_pin=%s sclk_pin=%s sid_pin=%s"
            " sync_delay_ticks=%d cmd_delay_ticks=%d" % (
                self.oid, self.pins[0], self.pins[1], self.pins[2],
                self.mcu.seconds_to_clock(ST7920_SYNC_DELAY),
                self.mcu.seconds_to_clock(ST7920_CMD_DELAY)))
        cmd_queue = self.mcu.alloc_command_queue()
        self.send_cmds_cmd = self.mcu.lookup_command(
            "st7920_send_cmds oid=%c cmds=%*s", cq=cmd_queue)
        self.send_data_cmd = self.mcu.lookup_command(
            "st7920_send_data oid=%c data=%*s", cq=cmd_queue)
    def send(self, cmds, is_data=False, is_extended=False):
        cmd_type = self.send_cmds_cmd
        if is_data:
            cmd_type = self.send_data_cmd
        elif self.is_extended != is_extended:
            add_cmd = 0x22
            if is_extended:
                add_cmd = 0x26
            cmds = [add_cmd] + cmds
            self.is_extended = is_extended
        cmd_type.send([self.oid, cmds], reqclock=BACKGROUND_PRIORITY_CLOCK)
        #logging.debug("st7920 %d %s", is_data, repr(cmds))
    def flush(self):
        # Find all differences in the framebuffers and send them to the chip
        for new_data, old_data, fb_id in self.framebuffers:
            if new_data == old_data:
                continue
            # Find the position of all changed bytes in this framebuffer
            diffs = [[i, 1] for i, (nd, od) in enumerate(zip(new_data, old_data))
                     if nd != od]
            # Batch together changes that are close to each other
            for i in range(len(diffs)-2, -1, -1):
                pos, count = diffs[i]
                nextpos, nextcount = diffs[i+1]
                if pos + 5 >= nextpos and nextcount < 16:
                    diffs[i][1] = nextcount + (nextpos - pos)
                    del diffs[i+1]
            # Transmit changes
            for pos, count in diffs:
                count += pos & 0x01
                count += count & 0x01
                pos = pos & ~0x01
                chip_pos = pos >> 1
                if fb_id < 0x40:
                    # Graphics framebuffer update
                    self.send([0x80 + fb_id, 0x80 + chip_pos], is_extended=True)
                else:
                    self.send([fb_id + chip_pos])
                self.send(new_data[pos:pos+count], is_data=True)
            old_data[:] = new_data
    def init(self):
        cmds = [0x24, # Enter extended mode
                0x40, # Clear vertical scroll address
                0x02, # Enable CGRAM access
                0x26, # Enable graphics
                0x22, # Leave extended mode
                0x02, # Home the display
                0x06, # Set positive update direction
                0x0c] # Enable display and hide cursor
        self.send(cmds)
        self.flush()
    def load_glyph(self, glyph_id, data):
        if len(data) > 32:
            data = data[:32]
        pos = min(glyph_id * 32, 96)
        self.glyph_framebuffer[0][pos:pos+len(data)] = data
    def write_text(self, x, y, data):
        if x + len(data) > 16:
            data = data[:16 - min(x, 16)]
        pos = [0, 32, 16, 48][y] + x
        self.text_framebuffer[0][pos:pos+len(data)] = data
    def write_graphics(self, x, y, row, data):
        if x + len(data) > 16:
            data = data[:16 - min(x, 16)]
        gfx_fb = y * 16 + row
        if gfx_fb >= 32:
            gfx_fb -= 32
            x += 16
        self.graphics_framebuffers[gfx_fb][0][x:x+len(data)] = data
    def clear(self):
        self.text_framebuffer[0][:] = ' '*64
        zeros = bytearray(32)
        for new_data, old_data, fb_id in self.graphics_framebuffers:
            new_data[:] = zeros

######################################################################
# UC1701 Character Set
# Font - Terminal 8x14, Row Major, MSB
# 
######################################################################

CHAR_SET = [
bytearray('\x00\x00\x00\x7E\xC3\x81\xA5\x81\xBD\x99\xC3\x7E\x00\x00'), # Code 1
bytearray('\x00\x00\x00\x7E\xFF\xFF\xDB\xFF\xC3\xE7\xFF\x7E\x00\x00'), # Code 2
bytearray('\x00\x00\x00\x00\x44\xEE\xFE\xFE\xFE\x7C\x38\x10\x00\x00'), # Code 3
bytearray('\x00\x00\x00\x10\x38\x7C\xFE\xFE\x7C\x38\x10\x00\x00\x00'), # Code 4
bytearray('\x00\x00\x00\x18\x3C\x3C\xFF\xE7\xE7\x18\x18\x7E\x00\x00'), # Code 5
bytearray('\x00\x00\x00\x18\x3C\x7E\xFF\xFF\x7E\x18\x18\x7E\x00\x00'), # Code 6
bytearray('\x00\x00\x00\x00\x00\x00\x3C\x7E\x7E\x3C\x00\x00\x00\x00'), # Code 7
bytearray('\x00\x00\xFF\xFF\xFF\xFF\xC3\x81\x81\xC3\xFF\xFF\xFF\xFF'), # Code 8
bytearray('\x00\x00\x00\x00\x3C\x7E\x66\x42\x42\x66\x7E\x3C\x00\x00'), # Code 9
bytearray('\x00\x00\xFF\xFF\xC3\x81\x99\xBD\xBD\x99\x81\xC3\xFF\xFF'), # Code 10
bytearray('\x00\x00\x00\x3E\x0E\x3A\x72\xF8\xCC\xCC\xCC\x78\x00\x00'), # Code 11
bytearray('\x00\x00\x00\x3C\x66\x66\x66\x3C\x18\x7E\x18\x18\x00\x00'), # Code 12
bytearray('\x00\x00\x00\x1F\x19\x19\x1F\x18\x18\x78\xF8\x70\x00\x00'), # Code 13
bytearray('\x00\x00\x00\x7F\x63\x7F\x63\x63\x63\x67\xE7\xE6\xC0\x00'), # Code 14
bytearray('\x00\x00\x00\x00\x18\xDB\x7E\xE7\xE7\x7E\xDB\x18\x00\x00'), # Code 15
bytearray('\x00\x00\x00\x80\xC0\xE0\xF8\xFE\xF8\xE0\xC0\x80\x00\x00'), # Code 16
bytearray('\x00\x00\x00\x02\x06\x0E\x3E\xFE\x3E\x0E\x06\x02\x00\x00'), # Code 17
bytearray('\x00\x00\x00\x18\x3C\x7E\x18\x18\x18\x7E\x3C\x18\x00\x00'), # Code 18
bytearray('\x00\x00\x00\x66\x66\x66\x66\x66\x00\x00\x66\x66\x00\x00'), # Code 19
bytearray('\x00\x00\x00\x7F\xDB\xDB\xDB\x7B\x1B\x1B\x1B\x1B\x00\x00'), # Code 20
bytearray('\x00\x00\x00\x7E\x63\x30\x3C\x66\x66\x3C\x0C\xC6\x7E\x00'), # Code 21
bytearray('\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFE\xFE\xFE\x00\x00'), # Code 22
bytearray('\x00\x00\x00\x18\x3C\x7E\x18\x18\x18\x7E\x3C\x18\x7E\x00'), # Code 23
bytearray('\x00\x00\x00\x18\x3C\x7E\x18\x18\x18\x18\x18\x18\x00\x00'), # Code 24
bytearray('\x00\x00\x00\x18\x18\x18\x18\x18\x18\x7E\x3C\x18\x00\x00'), # Code 25
bytearray('\x00\x00\x00\x00\x00\x18\x0C\xFE\x0C\x18\x00\x00\x00\x00'), # Code 26
bytearray('\x00\x00\x00\x00\x00\x30\x60\xFE\x60\x30\x00\x00\x00\x00'), # Code 27
bytearray('\x00\x00\x00\x00\x00\x00\xC0\xC0\xC0\xFE\x00\x00\x00\x00'), # Code 28
bytearray('\x00\x00\x00\x00\x00\x24\x66\xFF\x66\x24\x00\x00\x00\x00'), # Code 29
bytearray('\x00\x00\x00\x00\x10\x10\x38\x38\x7C\x7C\xFE\xFE\x00\x00'), # Code 30
bytearray('\x00\x00\x00\x00\xFE\xFE\x7C\x7C\x38\x38\x10\x10\x00\x00'), # Code 31
bytearray('\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'), # Code 32
bytearray('\x00\x00\x00\x30\x78\x78\x78\x30\x30\x00\x30\x30\x00\x00'), # Code 33
bytearray('\x00\x00\x00\x66\x66\x66\x24\x00\x00\x00\x00\x00\x00\x00'), # Code 34
bytearray('\x00\x00\x00\x6C\x6C\xFE\x6C\x6C\x6C\xFE\x6C\x6C\x00\x00'), # Code 35
bytearray('\x00\x00\x30\x30\x7C\xC0\xC0\x78\x0C\x0C\xF8\x30\x30\x00'), # Code 36
bytearray('\x00\x00\x00\x00\x00\xC4\xCC\x18\x30\x60\xCC\x8C\x00\x00'), # Code 37
bytearray('\x00\x00\x00\x70\xD8\xD8\x70\xFA\xDE\xCC\xDC\x76\x00\x00'), # Code 38
bytearray('\x00\x00\x00\x30\x30\x30\x60\x00\x00\x00\x00\x00\x00\x00'), # Code 39
bytearray('\x00\x00\x00\x0C\x18\x30\x60\x60\x60\x30\x18\x0C\x00\x00'), # Code 40
bytearray('\x00\x00\x00\x60\x30\x18\x0C\x0C\x0C\x18\x30\x60\x00\x00'), # Code 41
bytearray('\x00\x00\x00\x00\x00\x66\x3C\xFF\x3C\x66\x00\x00\x00\x00'), # Code 42
bytearray('\x00\x00\x00\x00\x00\x18\x18\x7E\x18\x18\x00\x00\x00\x00'), # Code 43
bytearray('\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x38\x38\x60\x00'), # Code 44
bytearray('\x00\x00\x00\x00\x00\x00\x00\xFE\x00\x00\x00\x00\x00\x00'), # Code 45
bytearray('\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x38\x38\x00\x00'), # Code 46
bytearray('\x00\x00\x00\x00\x02\x06\x0C\x18\x30\x60\xC0\x80\x00\x00'), # Code 47
bytearray('\x00\x00\x00\x7C\xC6\xCE\xDE\xD6\xF6\xE6\xC6\x7C\x00\x00'), # Code 48
bytearray('\x00\x00\x00\x10\x30\xF0\x30\x30\x30\x30\x30\xFC\x00\x00'), # Code 49
bytearray('\x00\x00\x00\x78\xCC\xCC\x0C\x18\x30\x60\xCC\xFC\x00\x00'), # Code 50
bytearray('\x00\x00\x00\x78\xCC\x0C\x0C\x38\x0C\x0C\xCC\x78\x00\x00'), # Code 51
bytearray('\x00\x00\x00\x0C\x1C\x3C\x6C\xCC\xFE\x0C\x0C\x1E\x00\x00'), # Code 52
bytearray('\x00\x00\x00\xFC\xC0\xC0\xC0\xF8\x0C\x0C\xCC\x78\x00\x00'), # Code 53
bytearray('\x00\x00\x00\x38\x60\xC0\xC0\xF8\xCC\xCC\xCC\x78\x00\x00'), # Code 54
bytearray('\x00\x00\x00\xFE\xC6\xC6\x06\x0C\x18\x30\x30\x30\x00\x00'), # Code 55
bytearray('\x00\x00\x00\x78\xCC\xCC\xEC\x78\xDC\xCC\xCC\x78\x00\x00'), # Code 56
bytearray('\x00\x00\x00\x78\xCC\xCC\xCC\x7C\x18\x18\x30\x70\x00\x00'), # Code 57
bytearray('\x00\x00\x00\x00\x00\x38\x38\x00\x00\x38\x38\x00\x00\x00'), # Code 58
bytearray('\x00\x00\x00\x00\x00\x38\x38\x00\x00\x38\x38\x18\x30\x00'), # Code 59
bytearray('\x00\x00\x00\x0C\x18\x30\x60\xC0\x60\x30\x18\x0C\x00\x00'), # Code 60
bytearray('\x00\x00\x00\x00\x00\x00\x7E\x00\x7E\x00\x00\x00\x00\x00'), # Code 61
bytearray('\x00\x00\x00\x60\x30\x18\x0C\x06\x0C\x18\x30\x60\x00\x00'), # Code 62
bytearray('\x00\x00\x00\x78\xCC\x0C\x18\x30\x30\x00\x30\x30\x00\x00'), # Code 63
bytearray('\x00\x00\x00\x7C\xC6\xC6\xDE\xDE\xDE\xC0\xC0\x7C\x00\x00'), # Code 64
bytearray('\x00\x00\x00\x30\x78\xCC\xCC\xCC\xFC\xCC\xCC\xCC\x00\x00'), # Code 65
bytearray('\x00\x00\x00\xFC\x66\x66\x66\x7C\x66\x66\x66\xFC\x00\x00'), # Code 66
bytearray('\x00\x00\x00\x3C\x66\xC6\xC0\xC0\xC0\xC6\x66\x3C\x00\x00'), # Code 67
bytearray('\x00\x00\x00\xF8\x6C\x66\x66\x66\x66\x66\x6C\xF8\x00\x00'), # Code 68
bytearray('\x00\x00\x00\xFE\x62\x60\x64\x7C\x64\x60\x62\xFE\x00\x00'), # Code 69
bytearray('\x00\x00\x00\xFE\x66\x62\x64\x7C\x64\x60\x60\xF0\x00\x00'), # Code 70
bytearray('\x00\x00\x00\x3C\x66\xC6\xC0\xC0\xCE\xC6\x66\x3E\x00\x00'), # Code 71
bytearray('\x00\x00\x00\xCC\xCC\xCC\xCC\xFC\xCC\xCC\xCC\xCC\x00\x00'), # Code 72
bytearray('\x00\x00\x00\x78\x30\x30\x30\x30\x30\x30\x30\x78\x00\x00'), # Code 73
bytearray('\x00\x00\x00\x1E\x0C\x0C\x0C\x0C\xCC\xCC\xCC\x78\x00\x00'), # Code 74
bytearray('\x00\x00\x00\xE6\x66\x6C\x6C\x78\x6C\x6C\x66\xE6\x00\x00'), # Code 75
bytearray('\x00\x00\x00\xF0\x60\x60\x60\x60\x62\x66\x66\xFE\x00\x00'), # Code 76
bytearray('\x00\x00\x00\xC6\xEE\xFE\xFE\xD6\xC6\xC6\xC6\xC6\x00\x00'), # Code 77
bytearray('\x00\x00\x00\xC6\xC6\xE6\xF6\xFE\xDE\xCE\xC6\xC6\x00\x00'), # Code 78
bytearray('\x00\x00\x00\x38\x6C\xC6\xC6\xC6\xC6\xC6\x6C\x38\x00\x00'), # Code 79
bytearray('\x00\x00\x00\xFC\x66\x66\x66\x7C\x60\x60\x60\xF0\x00\x00'), # Code 80
bytearray('\x00\x00\x00\x38\x6C\xC6\xC6\xC6\xCE\xDE\x7C\x0C\x1E\x00'), # Code 81
bytearray('\x00\x00\x00\xFC\x66\x66\x66\x7C\x6C\x66\x66\xE6\x00\x00'), # Code 82
bytearray('\x00\x00\x00\x78\xCC\xCC\xC0\x70\x18\xCC\xCC\x78\x00\x00'), # Code 83
bytearray('\x00\x00\x00\xFC\xB4\x30\x30\x30\x30\x30\x30\x78\x00\x00'), # Code 84
bytearray('\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 85
bytearray('\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x78\x30\x00\x00'), # Code 86
bytearray('\x00\x00\x00\xC6\xC6\xC6\xC6\xD6\xD6\x6C\x6C\x6C\x00\x00'), # Code 87
bytearray('\x00\x00\x00\xCC\xCC\xCC\x78\x30\x78\xCC\xCC\xCC\x00\x00'), # Code 88
bytearray('\x00\x00\x00\xCC\xCC\xCC\xCC\x78\x30\x30\x30\x78\x00\x00'), # Code 89
bytearray('\x00\x00\x00\xFE\xCE\x98\x18\x30\x60\x62\xC6\xFE\x00\x00'), # Code 90
bytearray('\x00\x00\x00\x3C\x30\x30\x30\x30\x30\x30\x30\x3C\x00\x00'), # Code 91
bytearray('\x00\x00\x00\x00\x80\xC0\x60\x30\x18\x0C\x06\x02\x00\x00'), # Code 92
bytearray('\x00\x00\x00\x3C\x0C\x0C\x0C\x0C\x0C\x0C\x0C\x3C\x00\x00'), # Code 93
bytearray('\x00\x00\x10\x38\x6C\xC6\x00\x00\x00\x00\x00\x00\x00\x00'), # Code 94
bytearray('\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x00'), # Code 95
bytearray('\x00\x00\x30\x30\x18\x00\x00\x00\x00\x00\x00\x00\x00\x00'), # Code 96
bytearray('\x00\x00\x00\x00\x00\x00\x78\x0C\x7C\xCC\xCC\x76\x00\x00'), # Code 97
bytearray('\x00\x00\x00\xE0\x60\x60\x7C\x66\x66\x66\x66\xDC\x00\x00'), # Code 98
bytearray('\x00\x00\x00\x00\x00\x00\x78\xCC\xC0\xC0\xCC\x78\x00\x00'), # Code 99
bytearray('\x00\x00\x00\x1C\x0C\x0C\x7C\xCC\xCC\xCC\xCC\x76\x00\x00'), # Code 100
bytearray('\x00\x00\x00\x00\x00\x00\x78\xCC\xFC\xC0\xCC\x78\x00\x00'), # Code 101
bytearray('\x00\x00\x00\x38\x6C\x60\x60\xF8\x60\x60\x60\xF0\x00\x00'), # Code 102
bytearray('\x00\x00\x00\x00\x00\x00\x76\xCC\xCC\xCC\x7C\x0C\xCC\x78'), # Code 103
bytearray('\x00\x00\x00\xE0\x60\x60\x6C\x76\x66\x66\x66\xE6\x00\x00'), # Code 104
bytearray('\x00\x00\x00\x18\x18\x00\x78\x18\x18\x18\x18\x7E\x00\x00'), # Code 105
bytearray('\x00\x00\x00\x0C\x0C\x00\x3C\x0C\x0C\x0C\x0C\xCC\xCC\x78'), # Code 106
bytearray('\x00\x00\x00\xE0\x60\x60\x66\x6C\x78\x6C\x66\xE6\x00\x00'), # Code 107
bytearray('\x00\x00\x00\x78\x18\x18\x18\x18\x18\x18\x18\x7E\x00\x00'), # Code 108
bytearray('\x00\x00\x00\x00\x00\x00\xFC\xD6\xD6\xD6\xD6\xC6\x00\x00'), # Code 109
bytearray('\x00\x00\x00\x00\x00\x00\xF8\xCC\xCC\xCC\xCC\xCC\x00\x00'), # Code 110
bytearray('\x00\x00\x00\x00\x00\x00\x78\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 111
bytearray('\x00\x00\x00\x00\x00\x00\xDC\x66\x66\x66\x66\x7C\x60\xF0'), # Code 112
bytearray('\x00\x00\x00\x00\x00\x00\x76\xCC\xCC\xCC\xCC\x7C\x0C\x1E'), # Code 113
bytearray('\x00\x00\x00\x00\x00\x00\xEC\x6E\x76\x60\x60\xF0\x00\x00'), # Code 114
bytearray('\x00\x00\x00\x00\x00\x00\x78\xCC\x60\x18\xCC\x78\x00\x00'), # Code 115
bytearray('\x00\x00\x00\x00\x20\x60\xFC\x60\x60\x60\x6C\x38\x00\x00'), # Code 116
bytearray('\x00\x00\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\x76\x00\x00'), # Code 117
bytearray('\x00\x00\x00\x00\x00\x00\xCC\xCC\xCC\xCC\x78\x30\x00\x00'), # Code 118
bytearray('\x00\x00\x00\x00\x00\x00\xC6\xC6\xD6\xD6\x6C\x6C\x00\x00'), # Code 119
bytearray('\x00\x00\x00\x00\x00\x00\xC6\x6C\x38\x38\x6C\xC6\x00\x00'), # Code 120
bytearray('\x00\x00\x00\x00\x00\x00\x66\x66\x66\x66\x3C\x0C\x18\xF0'), # Code 121
bytearray('\x00\x00\x00\x00\x00\x00\xFC\x8C\x18\x60\xC4\xFC\x00\x00'), # Code 122
bytearray('\x00\x00\x00\x1C\x30\x30\x60\xC0\x60\x30\x30\x1C\x00\x00'), # Code 123
bytearray('\x00\x00\x00\x18\x18\x18\x18\x00\x18\x18\x18\x18\x00\x00'), # Code 124
bytearray('\x00\x00\x00\xE0\x30\x30\x18\x0C\x18\x30\x30\xE0\x00\x00'), # Code 125
bytearray('\x00\x00\x00\x73\xDA\xCE\x00\x00\x00\x00\x00\x00\x00\x00'), # Code 126
bytearray('\x00\x00\x00\x00\x00\x10\x38\x6C\xC6\xC6\xFE\x00\x00\x00'), # Code 127
bytearray('\x00\x00\x00\x78\xCC\xCC\xC0\xC0\xC0\xCC\xCC\x78\x30\xF0'), # Code 128
bytearray('\x00\x00\x00\xCC\xCC\x00\xCC\xCC\xCC\xCC\xCC\x76\x00\x00'), # Code 129
bytearray('\x00\x00\x0C\x18\x30\x00\x78\xCC\xFC\xC0\xCC\x78\x00\x00'), # Code 130
bytearray('\x00\x00\x30\x78\xCC\x00\x78\x0C\x7C\xCC\xCC\x76\x00\x00'), # Code 131
bytearray('\x00\x00\x00\xCC\xCC\x00\x78\x0C\x7C\xCC\xCC\x76\x00\x00'), # Code 132
bytearray('\x00\x00\xC0\x60\x30\x00\x78\x0C\x7C\xCC\xCC\x76\x00\x00'), # Code 133
bytearray('\x00\x00\x38\x6C\x6C\x38\xF8\x0C\x7C\xCC\xCC\x76\x00\x00'), # Code 134
bytearray('\x00\x00\x00\x00\x00\x00\x78\xCC\xC0\xC0\xCC\x78\x30\xF0'), # Code 135
bytearray('\x00\x00\x30\x78\xCC\x00\x78\xCC\xFC\xC0\xC0\x7C\x00\x00'), # Code 136
bytearray('\x00\x00\x00\xCC\xCC\x00\x78\xCC\xFC\xC0\xC0\x7C\x00\x00'), # Code 137
bytearray('\x00\x00\xC0\x60\x30\x00\x78\xCC\xFC\xC0\xC0\x7C\x00\x00'), # Code 138
bytearray('\x00\x00\x00\x6C\x6C\x00\x78\x18\x18\x18\x18\x7E\x00\x00'), # Code 139
bytearray('\x00\x00\x10\x38\x6C\x00\x78\x18\x18\x18\x18\x7E\x00\x00'), # Code 140
bytearray('\x00\x00\x60\x30\x18\x00\x78\x18\x18\x18\x18\x7E\x00\x00'), # Code 141
bytearray('\x00\x00\x00\xCC\x00\x30\x78\xCC\xCC\xFC\xCC\xCC\x00\x00'), # Code 142
bytearray('\x00\x00\x78\xCC\xCC\x78\x78\xCC\xCC\xFC\xCC\xCC\x00\x00'), # Code 143
bytearray('\x00\x00\x0C\x18\x30\xFC\xC4\xC0\xF8\xC0\xC4\xFC\x00\x00'), # Code 144
bytearray('\x00\x00\x00\x00\x00\x00\xFE\x1B\x7F\xD8\xD8\xEF\x00\x00'), # Code 145
bytearray('\x00\x00\x00\x3E\x78\xD8\xD8\xFE\xD8\xD8\xD8\xDE\x00\x00'), # Code 146
bytearray('\x00\x00\x30\x78\xCC\x00\x78\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 147
bytearray('\x00\x00\x00\xCC\xCC\x00\x78\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 148
bytearray('\x00\x00\xC0\x60\x30\x00\x78\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 149
bytearray('\x00\x00\x30\x78\xCC\x00\xCC\xCC\xCC\xCC\xCC\x76\x00\x00'), # Code 150
bytearray('\x00\x00\xC0\x60\x30\x00\xCC\xCC\xCC\xCC\xCC\x76\x00\x00'), # Code 151
bytearray('\x00\x00\x00\x66\x66\x00\x66\x66\x66\x66\x3C\x0C\x18\xF0'), # Code 152
bytearray('\x00\x00\xCC\x00\x78\xCC\xCC\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 153
bytearray('\x00\x00\xCC\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 154
bytearray('\x00\x00\x00\x30\x30\x78\xCC\xC0\xC0\xCC\x78\x30\x30\x00'), # Code 155
bytearray('\x00\x00\x3C\x66\x60\x60\x60\xFC\x60\x60\xC0\xFE\x00\x00'), # Code 156
bytearray('\x00\x00\xCC\xCC\xCC\xCC\x78\xFC\x30\xFC\x30\x30\x00\x00'), # Code 157
bytearray('\x00\x00\xF0\x88\x88\x88\xF0\x88\x9E\x8C\x8D\x86\x00\x00'), # Code 158
bytearray('\x00\x00\x0E\x1B\x18\x18\x7E\x18\x18\x18\xD8\x70\x00\x00'), # Code 159
bytearray('\x00\x00\x0C\x18\x30\x00\x78\x0C\x7C\xCC\xCC\x76\x00\x00'), # Code 160
bytearray('\x00\x00\x0C\x18\x30\x00\x78\x18\x18\x18\x18\x7E\x00\x00'), # Code 161
bytearray('\x00\x00\x0C\x18\x30\x00\x78\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 162
bytearray('\x00\x00\x0C\x18\x30\x00\xCC\xCC\xCC\xCC\xCC\x76\x00\x00'), # Code 163
bytearray('\x00\x00\x00\x76\xDC\x00\xF8\xCC\xCC\xCC\xCC\xCC\x00\x00'), # Code 164
bytearray('\x00\x00\x76\xDC\x00\xC6\xE6\xF6\xDE\xCE\xC6\xC6\x00\x00'), # Code 165
bytearray('\x00\x00\x00\x78\xCC\xCC\x7E\x00\xFE\x00\x00\x00\x00\x00'), # Code 166
bytearray('\x00\x00\x00\x78\xCC\xCC\x78\x00\xFE\x00\x00\x00\x00\x00'), # Code 167
bytearray('\x00\x00\x00\x30\x30\x00\x30\x60\xC0\xC0\xCC\x78\x00\x00'), # Code 168
bytearray('\x00\x00\x00\x00\x00\x00\x00\xFC\xC0\xC0\xC0\x00\x00\x00'), # Code 169
bytearray('\x00\x00\x00\x00\x00\x00\x00\xFC\x0C\x0C\x0C\x00\x00\x00'), # Code 170
bytearray('\x00\x00\x00\x42\xC6\xCC\xD8\x30\x6E\xC3\x86\x0C\x1F\x00'), # Code 171
bytearray('\x00\x00\x00\x63\xE6\x6C\x78\x37\x6F\xDB\xB3\x3F\x03\x00'), # Code 172
bytearray('\x00\x00\x00\x30\x30\x00\x30\x30\x78\x78\x78\x30\x00\x00'), # Code 173
bytearray('\x00\x00\x00\x00\x00\x00\x33\x66\xCC\xCC\x66\x33\x00\x00'), # Code 174
bytearray('\x00\x00\x00\x00\x00\x00\xCC\x66\x33\x33\x66\xCC\x00\x00'), # Code 175
bytearray('\x00\x00\x24\x92\x49\x24\x92\x49\x24\x92\x49\x24\x92\x49'), # Code 176
bytearray('\x00\x00\x55\xAA\x55\xAA\x55\xAA\x55\xAA\x55\xAA\x55\xAA'), # Code 177
bytearray('\x00\x00\x6D\xDB\xB6\x6D\xDB\xB6\x6D\xDB\xB6\x6D\xDB\xB6'), # Code 178
bytearray('\x00\x00\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18'), # Code 179
bytearray('\x00\x00\x18\x18\x18\x18\x18\xF8\x18\x18\x18\x18\x18\x18'), # Code 180
bytearray('\x00\x00\x18\x18\x18\x18\xF8\x18\x18\xF8\x18\x18\x18\x18'), # Code 181
bytearray('\x00\x00\x66\x66\x66\x66\x66\xE6\x66\x66\x66\x66\x66\x66'), # Code 182
bytearray('\x00\x00\x00\x00\x00\x00\x00\xFE\x66\x66\x66\x66\x66\x66'), # Code 183
bytearray('\x00\x00\x00\x00\x00\x00\xF8\x18\x18\xF8\x18\x18\x18\x18'), # Code 184
bytearray('\x00\x00\x66\x66\x66\x66\xE6\x06\x06\xE6\x66\x66\x66\x66'), # Code 185
bytearray('\x00\x00\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66'), # Code 186
bytearray('\x00\x00\x00\x00\x00\x00\xFE\x06\x06\xE6\x66\x66\x66\x66'), # Code 187
bytearray('\x00\x00\x66\x66\x66\x66\xE6\x06\x06\xFE\x00\x00\x00\x00'), # Code 188
bytearray('\x00\x00\x66\x66\x66\x66\x66\xFE\x00\x00\x00\x00\x00\x00'), # Code 189
bytearray('\x00\x00\x18\x18\x18\x18\xF8\x18\x18\xF8\x00\x00\x00\x00'), # Code 190
bytearray('\x00\x00\x00\x00\x00\x00\x00\xF8\x18\x18\x18\x18\x18\x18'), # Code 191
bytearray('\x00\x00\x18\x18\x18\x18\x18\x1F\x00\x00\x00\x00\x00\x00'), # Code 192
bytearray('\x00\x00\x18\x18\x18\x18\x18\xFF\x00\x00\x00\x00\x00\x00'), # Code 193
bytearray('\x00\x00\x00\x00\x00\x00\x00\xFF\x18\x18\x18\x18\x18\x18'), # Code 194
bytearray('\x00\x00\x18\x18\x18\x18\x18\x1F\x18\x18\x18\x18\x18\x18'), # Code 195
bytearray('\x00\x00\x00\x00\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00'), # Code 196
bytearray('\x00\x00\x18\x18\x18\x18\x18\xFF\x18\x18\x18\x18\x18\x18'), # Code 197
bytearray('\x00\x00\x18\x18\x18\x18\x1F\x18\x18\x1F\x18\x18\x18\x18'), # Code 198
bytearray('\x00\x00\x66\x66\x66\x66\x66\x67\x66\x66\x66\x66\x66\x66'), # Code 199
bytearray('\x00\x00\x66\x66\x66\x66\x67\x60\x60\x7F\x00\x00\x00\x00'), # Code 200
bytearray('\x00\x00\x00\x00\x00\x00\x7F\x60\x60\x67\x66\x66\x66\x66'), # Code 201
bytearray('\x00\x00\x66\x66\x66\x66\xE7\x00\x00\xFF\x00\x00\x00\x00'), # Code 202
bytearray('\x00\x00\x00\x00\x00\x00\xFF\x00\x00\xE7\x66\x66\x66\x66'), # Code 203
bytearray('\x00\x00\x66\x66\x66\x66\x67\x60\x60\x67\x66\x66\x66\x66'), # Code 204
bytearray('\x00\x00\x00\x00\x00\x00\xFF\x00\x00\xFF\x00\x00\x00\x00'), # Code 205
bytearray('\x00\x00\x66\x66\x66\x66\xE7\x00\x00\xE7\x66\x66\x66\x66'), # Code 206
bytearray('\x00\x00\x18\x18\x18\x18\xFF\x00\x00\xFF\x00\x00\x00\x00'), # Code 207
bytearray('\x00\x00\x66\x66\x66\x66\x66\xFF\x00\x00\x00\x00\x00\x00'), # Code 208
bytearray('\x00\x00\x00\x00\x00\x00\xFF\x00\x00\xFF\x18\x18\x18\x18'), # Code 209
bytearray('\x00\x00\x00\x00\x00\x00\x00\xFF\x66\x66\x66\x66\x66\x66'), # Code 210
bytearray('\x00\x00\x66\x66\x66\x66\x66\x7F\x00\x00\x00\x00\x00\x00'), # Code 211
bytearray('\x00\x00\x18\x18\x18\x18\x1F\x18\x18\x1F\x00\x00\x00\x00'), # Code 212
bytearray('\x00\x00\x00\x00\x00\x00\x1F\x18\x18\x1F\x18\x18\x18\x18'), # Code 213
bytearray('\x00\x00\x00\x00\x00\x00\x00\x7F\x66\x66\x66\x66\x66\x66'), # Code 214
bytearray('\x00\x00\x66\x66\x66\x66\x66\xE7\x66\x66\x66\x66\x66\x66'), # Code 215
bytearray('\x00\x00\x18\x18\x18\x18\xFF\x00\x00\xFF\x18\x18\x18\x18'), # Code 216
bytearray('\x00\x00\x18\x18\x18\x18\x18\xF8\x00\x00\x00\x00\x00\x00'), # Code 217
bytearray('\x00\x00\x00\x00\x00\x00\x00\x1F\x18\x18\x18\x18\x18\x18'), # Code 218
bytearray('\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF'), # Code 219
bytearray('\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF'), # Code 220
bytearray('\x00\x00\xF0\xF0\xF0\xF0\xF0\xF0\xF0\xF0\xF0\xF0\xF0\xF0'), # Code 221
bytearray('\x00\x00\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F'), # Code 222
bytearray('\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00'), # Code 223
bytearray('\x00\x00\x00\x00\x00\x00\x76\xDE\xCC\xCC\xDE\x76\x00\x00'), # Code 224
bytearray('\x00\x00\x00\x78\xCC\xCC\xD8\xCC\xCC\xCC\xF8\xC0\x60\x00'), # Code 225
bytearray('\x00\x00\x00\xFC\xCC\xCC\xC0\xC0\xC0\xC0\xC0\xC0\x00\x00'), # Code 226
bytearray('\x00\x00\x00\xFE\x6C\x6C\x6C\x6C\x6C\x6C\x6C\x66\x00\x00'), # Code 227
bytearray('\x00\x00\x00\xFC\xC4\x64\x60\x30\x60\x64\xC4\xFC\x00\x00'), # Code 228
bytearray('\x00\x00\x00\x00\x00\x00\x7E\xC8\xCC\xCC\xCC\x78\x00\x00'), # Code 229
bytearray('\x00\x00\x00\x00\x00\x00\x66\x66\x66\x66\x66\x7B\x60\xC0'), # Code 230
bytearray('\x00\x00\x00\x00\x00\x76\xDC\x18\x18\x18\x18\x0E\x00\x00'), # Code 231
bytearray('\x00\x00\x00\xFC\x30\x78\xCC\xCC\xCC\x78\x30\xFC\x00\x00'), # Code 232
bytearray('\x00\x00\x00\x78\xCC\xCC\xCC\xFC\xCC\xCC\xCC\x78\x00\x00'), # Code 233
bytearray('\x00\x00\x00\x7C\xC6\xC6\xC6\xC6\x6C\x6C\x6C\xEE\x00\x00'), # Code 234
bytearray('\x00\x00\x00\x3C\x60\x30\x78\xCC\xCC\xCC\xCC\x78\x00\x00'), # Code 235
bytearray('\x00\x00\x00\x00\x00\x76\xDB\xDB\xDB\x6E\x00\x00\x00\x00'), # Code 236
bytearray('\x00\x00\x00\x00\x06\x7C\xDE\xD6\xF6\x7C\xC0\x00\x00\x00'), # Code 237
bytearray('\x00\x00\x00\x3C\x60\xC0\xC0\xFC\xC0\xC0\x60\x3C\x00\x00'), # Code 238
bytearray('\x00\x00\x00\x00\x78\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x00\x00'), # Code 239
bytearray('\x00\x00\x00\x00\xFC\x00\x00\xFC\x00\x00\xFC\x00\x00\x00'), # Code 240
bytearray('\x00\x00\x00\x00\x30\x30\xFC\x30\x30\x00\xFC\x00\x00\x00'), # Code 241
bytearray('\x00\x00\x00\x60\x30\x18\x18\x30\x60\x00\xFC\x00\x00\x00'), # Code 242
bytearray('\x00\x00\x00\x18\x30\x60\x60\x30\x18\x00\xFC\x00\x00\x00'), # Code 243
bytearray('\x00\x00\x00\x00\x0E\x1B\x1B\x18\x18\x18\x18\x18\x18\x18'), # Code 244
bytearray('\x00\x00\x18\x18\x18\x18\x18\x18\x18\xD8\xD8\x70\x00\x00'), # Code 245
bytearray('\x00\x00\x00\x00\x30\x30\x00\xFC\x00\x30\x30\x00\x00\x00'), # Code 246
bytearray('\x00\x00\x00\x00\x73\xDB\xCE\x00\x73\xDB\xCE\x00\x00\x00'), # Code 247
bytearray('\x00\x00\x00\x3C\x66\x66\x66\x3C\x00\x00\x00\x00\x00\x00'), # Code 248
bytearray('\x00\x00\x00\x00\x00\x00\x1C\x1C\x00\x00\x00\x00\x00\x00'), # Code 249
bytearray('\x00\x00\x00\x00\x00\x00\x00\x18\x00\x00\x00\x00\x00\x00'), # Code 250
bytearray('\x00\x00\x00\x07\x04\x04\x04\x44\x64\x34\x1C\x0C\x00\x00'), # Code 251
bytearray('\x00\x00\x00\xD8\x6C\x6C\x6C\x6C\x00\x00\x00\x00\x00\x00'), # Code 252
bytearray('\x00\x00\x00\x78\x0C\x18\x30\x7C\x00\x00\x00\x00\x00\x00'), # Code 253
bytearray('\x00\x00\x00\x00\x3C\x3C\x3C\x3C\x3C\x3C\x3C\x3C\x00\x00')  # Code 254
]


######################################################################
# UC1701 (128x64 graphics) lcd chip
######################################################################

UC1701_DELAY = .000020 # Seems to work okay, there is no mention in the spec to delay between
                       # bytes but probably required on faster hardware

class UC1701:
    char_right_arrow = '\x1a'
    CURRENT_BUF, OLD_BUF = 0, 1
    EMPTY_CHAR = (0, 32, 255)
    def __init__(self, config):
        printer = config.get_printer()
        # pin config
        ppins = printer.lookup_object('pins')
        pins = [ppins.lookup_pin('digital_out', config.get(name + '_pin'))
                for name in ['cs', 'sclk', 'sid', 'a0']]
        mcu = None
        for pin_params in pins:
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("uc1701 all pins must be on same mcu")
            mcu = pin_params['chip']
            if pin_params['invert']:
                raise ppins.error("uc1701 can not invert pin")
        self.pins = [pin_params['pin'] for pin_params in pins]
        self.mcu = mcu
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_object(self)
        self.glyph_buffer = []
        self.send_data_cmd = self.send_cmds_cmd = self.init_cmd = None
        self.vram = ([bytearray(128) for i in range(8)], 
                    [bytearray('~'*128) for i in range(8)])
    def build_config(self):
        self.mcu.add_config_cmd(
            "config_uc1701 oid=%u cs_pin=%s sclk_pin=%s sid_pin=%s"
            " a0_pin=%s delay_ticks=%d" % (
                self.oid, self.pins[0], self.pins[1], self.pins[2],
                self.pins[3], self.mcu.seconds_to_clock(UC1701_DELAY)))
        cmd_queue = self.mcu.alloc_command_queue()
        self.send_cmds_cmd = self.mcu.lookup_command(
            "uc1701_send_cmds oid=%c cmds=%*s", cq=cmd_queue)
        self.send_data_cmd = self.mcu.lookup_command(
            "uc1701_send_data oid=%c data=%*s", cq=cmd_queue)
        self.init_cmd = self.mcu.lookup_command(
            "uc1701_init oid=%c cmds=%*s", cq=cmd_queue)
    def send(self, cmds, is_data=False):
        cmd_type = self.send_cmds_cmd
        if is_data:
            cmd_type = self.send_data_cmd
        cmd_type.send([self.oid, cmds], reqclock=BACKGROUND_PRIORITY_CLOCK)        
    def init(self):
        init_cmds = [0xE2, # System reset
                     0x40, # Set display to start at line 0
                     0xA0, # Set SEG direction
                     0xC8, # Set COM Direction
                     0xA2, # Set Bias = 1/9
                     0x2C, # Boost ON
                     0x2E, # Voltage regulator on
                     0x2F, # Voltage follower on
                     0xF8, # Set booster ratio
                     0x00, # Booster ratio value (4x)
                     0x23, # Set resistor ratio (3)
                     0x81, # Set Electronic Volume
                     0x28, # Electronic volume value (40)
                     0xAC, # Set static indicator off
                     0x00, # NOP 
                     0xA6, # Disable Inverse
                     0xAF] # Set display enable
        self.init_cmd.send([self.oid, init_cmds], reqclock=BACKGROUND_PRIORITY_CLOCK)
        self.flush()
        logging.info("uc1701 initialized")
    def flush(self):
        new_data = self.vram[self.CURRENT_BUF]
        old_data = self.vram[self.OLD_BUF]
        for page in range(8):
            if new_data[page] == old_data[page]:
                continue
            # Find the position of all changed bytes in this framebuffer
            diffs = [[i, 1] for i, (nd, od) in enumerate(zip(new_data[page], old_data[page]))
                     if nd != od]
            # Batch together changes that are close to each other
            for i in range(len(diffs)-2, -1, -1):
                pos, count = diffs[i]
                nextpos, nextcount = diffs[i+1]
                if pos + 5 >= nextpos and nextcount < 16:
                    diffs[i][1] = nextcount + (nextpos - pos)
                    del diffs[i+1]
            # Transmit
            for col_pos, count in diffs:
                # Set Position registers
                ra = 0xb0 | (page & 0x0F)
                ca_msb = 0x10 | ((col_pos >> 4) & 0x0F)
                ca_lsb = col_pos & 0x0F
                self.send([ra, ca_msb, ca_lsb])
                # Send Data
                self.send(new_data[page][col_pos:col_pos+count], is_data=True)
            old_data[page][:] = new_data[page]
    def set_pixel(self, pix_x, pix_y, exclusive=True):
        page_idx = pix_y // 8
        page_byte = 0x01 << (pix_y % 8)
        if exclusive and self.vram[self.CURRENT_BUF][page_idx][pix_x] & page_byte:
            #invert pixel if it has alread been set
            self.vram[self.CURRENT_BUF][page_idx][pix_x] &= ~page_byte
        else:
            #set the correct pixel in the vram buffer to 1
            self.vram[self.CURRENT_BUF][page_idx][pix_x] |= page_byte
    def clear_pixel(self, pix_x, pix_y):
        page_idx = pix_y // 8
        page_byte = ~(0x01 << (pix_y % 8))
        #set the correct pixel in the vram buffer to 0
        self.vram[self.CURRENT_BUF][page_idx][pix_x] &= page_byte
    def load_glyph(self, glyph_id, data):
        if len(data) > 16:
            data = data[:16]
        self.glyph_buffer.append((glyph_id, data))
        self.glyph_buffer.sort(key=lambda x: x[0])
    def write_glyph(self, x, y, glyph_id):
        pix_x = x*8   
        pix_y = y*16
        data = self.glyph_buffer[glyph_id][1]
        for bits in data:
            if bits:
                bit_x = pix_x
                for i in range(15, -1, -1):
                    mask = 0x0001 << i
                    if bits & mask:
                        self.set_pixel(bit_x, pix_y)
                    bit_x += 1
            pix_y += 1
    def write_text(self, x, y, data):
        if x + len(data) > 16:
            data = data[:16 - min(x, 16)]
        pix_x = x*8
        pix_y = y*16
        for c in data:
            c_idx = ord(c)
            if c_idx in self.EMPTY_CHAR:
                # Blank Character, skip it
                pix_x += 8
                continue
            c_idx -= 1
            char = CHAR_SET[c_idx]
            bit_y = pix_y
            for bits in char:
                if bits:
                    bit_x = pix_x
                    for i in range(7, -1, -1):
                        mask = 0x01 << i
                        if bits & mask:
                            self.set_pixel(bit_x, bit_y)
                        bit_x += 1
                bit_y += 1
            pix_x += 8
    def write_graphics(self, x, y, row, data):
        if x + len(data) > 16:
            data = data[:16 - min(x, 16)]
        pix_x = x*8
        pix_y = y*16 + row
        for bits in data:
            for i in range(7, -1, -1):
                mask = 0x01 << i
                if bits & mask:
                    self.set_pixel(pix_x, pix_y)
                pix_x += 1
    def clear(self):
        zeros = bytearray(128)
        for page in self.vram[self.CURRENT_BUF]:
            page[:] = zeros


######################################################################
# Icons
######################################################################

nozzle_icon = [
    0b0000000000000000,
    0b0000000000000000,
    0b0000111111110000,
    0b0001111111111000,
    0b0001111111111000,
    0b0001111111111000,
    0b0000111111110000,
    0b0000111111110000,
    0b0001111111111000,
    0b0001111111111000,
    0b0001111111111000,
    0b0000011111100000,
    0b0000001111000000,
    0b0000000110000000,
    0b0000000000000000,
    0b0000000000000000
]

bed_icon = [
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0111111111111110,
    0b0111111111111110,
    0b0000000000000000,
    0b0000000000000000
]

heat1_icon = [
    0b0000000000000000,
    0b0000000000000000,
    0b0010001000100000,
    0b0001000100010000,
    0b0000100010001000,
    0b0000100010001000,
    0b0001000100010000,
    0b0010001000100000,
    0b0010001000100000,
    0b0001000100010000,
    0b0000100010001000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000
]

heat2_icon = [
    0b0000000000000000,
    0b0000000000000000,
    0b0000100010001000,
    0b0000100010001000,
    0b0001000100010000,
    0b0010001000100000,
    0b0010001000100000,
    0b0001000100010000,
    0b0000100010001000,
    0b0000100010001000,
    0b0001000100010000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000
]

fan1_icon = [
    0b0000000000000000,
    0b0111111111111110,
    0b0111000000001110,
    0b0110001111000110,
    0b0100001111000010,
    0b0100000110000010,
    0b0101100000011010,
    0b0101110110111010,
    0b0101100000011010,
    0b0100000110000010,
    0b0100001111000010,
    0b0110001111000110,
    0b0111000000001110,
    0b0111111111111110,
    0b0000000000000000,
    0b0000000000000000
]

fan2_icon = [
    0b0000000000000000,
    0b0111111111111110,
    0b0111000000001110,
    0b0110010000100110,
    0b0100111001110010,
    0b0101111001111010,
    0b0100110000110010,
    0b0100000110000010,
    0b0100110000110010,
    0b0101111001111010,
    0b0100111001110010,
    0b0110010000100110,
    0b0111000000001110,
    0b0111111111111110,
    0b0000000000000000,
    0b0000000000000000
]

feedrate_icon = [
    0b0000000000000000,
    0b0111111000000000,
    0b0100000000000000,
    0b0100000000000000,
    0b0100000000000000,
    0b0111111011111000,
    0b0100000010000100,
    0b0100000010000100,
    0b0100000010000100,
    0b0100000011111000,
    0b0000000010001000,
    0b0000000010000100,
    0b0000000010000100,
    0b0000000010000010,
    0b0000000000000000,
    0b0000000000000000
]


######################################################################
# LCD screen updates
######################################################################

LCD_chips = { 'st7920': ST7920, 'hd44780': HD44780, 'uc1701' : UC1701 }

class PrinterLCD:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.lcd_chip = config.getchoice('lcd_type', LCD_chips)(config)
        self.lcd_type = config.get('lcd_type')
        # printer objects
        self.gcode = self.toolhead = self.sdcard = None
        self.fan = self.extruder0 = self.extruder1 = self.heater_bed = None
        # screen updating
        self.screen_update_timer = self.reactor.register_timer(
            self.screen_update_event)
    # Initialization
    FAN1_GLYPH, FAN2_GLYPH, BED1_GLYPH, BED2_GLYPH = 0, 1, 2, 3
    def printer_state(self, state):
        if state == 'ready':
            self.lcd_chip.init()
            # Load printer objects
            self.gcode = self.printer.lookup_object('gcode')
            self.toolhead = self.printer.lookup_object('toolhead')
            self.sdcard = self.printer.lookup_object('virtual_sdcard', None)
            self.fan = self.printer.lookup_object('fan', None)
            self.extruder0 = self.printer.lookup_object('extruder0', None)
            self.extruder1 = self.printer.lookup_object('extruder1', None)
            self.heater_bed = self.printer.lookup_object('heater_bed', None)
            self.prg_time = .0
            self.progress = None
            self.msg_time = None
            self.message = None
            self.gcode.register_command('M73', self.cmd_M73)
            self.gcode.register_command('M117', self.cmd_M117, desc=self.cmd_M117_help)
            # Load glyphs
            self.load_glyph(self.BED1_GLYPH, heat1_icon)
            self.load_glyph(self.BED2_GLYPH, heat2_icon)
            self.load_glyph(self.FAN1_GLYPH, fan1_icon)
            self.load_glyph(self.FAN2_GLYPH, fan2_icon)
            # Start screen update timer
            self.reactor.update_timer(self.screen_update_timer, self.reactor.NOW)
    # ST7920/UC1701 Glyphs
    def load_glyph(self, glyph_id, data):
        if self.lcd_type == 'uc1701':
            self.lcd_chip.load_glyph(glyph_id, data)
        elif self.lcd_type == 'st7920':
            glyph = [0x00] * (len(data) * 2)
            for i, bits in enumerate(data):
                glyph[i*2] = (bits >> 8) & 0xff
                glyph[i*2 + 1] = bits & 0xff
            return self.lcd_chip.load_glyph(glyph_id, glyph)
    def animate_glyphs(self, eventtime, x, y, glyph_id, do_animate):
        frame = do_animate and int(eventtime) & 1
        if self.lcd_type == 'uc1701':
            self.lcd_chip.write_glyph(x, y, glyph_id + frame)
        elif self.lcd_type == 'st7920':
            self.lcd_chip.write_text(x, y, (0, (glyph_id + frame)*2))
    # Graphics drawing
    def draw_icon(self, x, y, data):
        for i, bits in enumerate(data):
            self.lcd_chip.write_graphics(
                x, y, i, [(bits >> 8) & 0xff, bits & 0xff])
    def draw_progress_bar(self, x, y, width, value):
        value = int(value * 100.)
        data = [0x00] * width
        char_pcnt = int(100/width)
        for i in range(width):
            if (i+1)*char_pcnt <= value:
                # Draw completely filled bytes
                data[i] |= 0xFF
            elif (i*char_pcnt) < value:
                # Draw partially filled bytes
                data[i] |= (-1 << 8-((value % char_pcnt)*8/char_pcnt)) & 0xff
        data[0] |= 0x80
        data[-1] |= 0x01
        self.lcd_chip.write_graphics(x, y, 0, [0xff]*width)
        for i in range(1, 15):
            self.lcd_chip.write_graphics(x, y, i, data)
        self.lcd_chip.write_graphics(x, y, 15, [0xff]*width)
    # Screen updating
    def screen_update_event(self, eventtime):
        self.lcd_chip.clear()
        if self.lcd_type == 'hd44780':
            self.screen_update_hd44780(eventtime)
        else:
            self.screen_update_128x64(eventtime)
        self.lcd_chip.flush()
        return eventtime + .500
    def screen_update_hd44780(self, eventtime):
        lcd_chip = self.lcd_chip
        # Heaters
        if self.extruder0 is not None:
            info = self.extruder0.get_heater().get_status(eventtime)
            lcd_chip.write_text(0, 0, lcd_chip.char_thermometer)
            self.draw_heater(1, 0, info)
        if self.extruder1 is not None:
            info = self.extruder1.get_heater().get_status(eventtime)
            lcd_chip.write_text(0, 1, lcd_chip.char_thermometer)
            self.draw_heater(1, 1, info)
        if self.heater_bed is not None:
            info = self.heater_bed.get_status(eventtime)
            lcd_chip.write_text(10, 0, lcd_chip.char_heater_bed)
            self.draw_heater(11, 0, info)
        # Fan speed
        if self.fan is not None:
            info = self.fan.get_status(eventtime)
            lcd_chip.write_text(10, 1, "Fan")
            self.draw_percent(14, 1, 4, info['speed'])
        # G-Code speed factor
        gcode_info = self.gcode.get_status(eventtime)
        lcd_chip.write_text(0, 2, lcd_chip.char_speed_factor)
        self.draw_percent(1, 2, 4, gcode_info['speed_factor'])
        # Print progress
        progress = None
        toolhead_info = self.toolhead.get_status(eventtime)
        if self.progress is not None:
            progress = self.progress / 100.
            lcd_chip.write_text(8, 2, lcd_chip.char_usb)
            if toolhead_info['status'] != "Printing":
                # 5 second timeout when not printing
                self.prg_time -= .5
                if self.prg_time <= 0.:
                    self.progress = None
        elif self.sdcard is not None:
            info = self.sdcard.get_status(eventtime)
            progress = info['progress']
            lcd_chip.write_text(8, 2, lcd_chip.char_sd)
        if progress is not None:
            self.draw_percent(9, 2, 4, progress)
        lcd_chip.write_text(14, 2, lcd_chip.char_clock)
        self.draw_time(15, 2, toolhead_info['printing_time'])
        # If there is a message set by M117, display it instead of toolhead info
        if self.message:
            lcd_chip.write_text(0, 3, self.message)
            if self.msg_time:
                # Screen updates every .5 seconds
                self.msg_time -= .5 
                if self.msg_time <= 0.:
                    self.message = None
                    self.msg_time = None
        else:
            self.draw_status(0, 3, gcode_info, toolhead_info)
    def screen_update_128x64(self, eventtime):
        # Heaters
        if self.extruder0 is not None:
            info = self.extruder0.get_heater().get_status(eventtime)
            self.draw_icon(0, 0, nozzle_icon)
            self.draw_heater(2, 0, info)
        extruder_count = 1
        if self.extruder1 is not None:
            info = self.extruder1.get_heater().get_status(eventtime)
            self.draw_icon(0, 1, nozzle_icon)
            self.draw_heater(2, 1, info)
            extruder_count = 2
        if self.heater_bed is not None:
            info = self.heater_bed.get_status(eventtime)
            self.draw_icon(0, extruder_count, bed_icon)
            if info['target']:
                self.animate_glyphs(eventtime, 0, extruder_count,
                                    self.BED1_GLYPH, True)
            self.draw_heater(2, extruder_count, info)
        # Fan speed
        if self.fan is not None:
            info = self.fan.get_status(eventtime)
            self.animate_glyphs(eventtime, 10, 0, self.FAN1_GLYPH,
                                info['speed'] != 0.)
            align = '>'if self.lcd_type == 'uc1701' else '^'
            self.draw_percent(12, 0, 4, info['speed'], align)
        # SD card print progress
        progress = None
        toolhead_info = self.toolhead.get_status(eventtime)
        if self.progress is not None:
            progress = self.progress / 100.
            if toolhead_info['status'] != "Printing":
                # 5 second timeout when not printing
                self.prg_time -= .5
                if self.prg_time <= 0.:
                    self.progress = None
        elif self.sdcard is not None:
            info = self.sdcard.get_status(eventtime)
            progress = info['progress']
        if progress is not None:
            if extruder_count == 1:
                x, y, width = 0, 2, 10
            else:
                x, y, width = 10, 1, 6
            self.draw_percent(x, y, width, progress)
            self.draw_progress_bar(x, y, width, progress)
        # G-Code speed factor
        gcode_info = self.gcode.get_status(eventtime)
        if extruder_count == 1:
            self.draw_icon(10, 1, feedrate_icon)
            align = '>'if self.lcd_type == 'uc1701' else '^'
            self.draw_percent(12, 1, 4, gcode_info['speed_factor'], align)
        # Printing time and status
        printing_time = toolhead_info['printing_time']
        remaining_time = None
        if progress is not None and progress > 0:
            remaining_time = int(printing_time / progress) - printing_time
        # switch mode every 6s
        if remaining_time is not None and int(eventtime) % 12 < 6:
            self.lcd_chip.write_text(10, 2, "-")
            self.draw_time(11, 2, remaining_time)
        else:
            offset = 1 if printing_time < 100 * 60 * 60 else 0
            self.draw_time(10 + offset, 2, printing_time)
        # if there is a message set by M117, display it instead of toolhaed info
        if self.message:
            self.lcd_chip.write_text(0, 3, self.message)
            if self.msg_time:
                # Screen updates every .5 seconds
                self.msg_time -= .5 
                if self.msg_time <= 0.:
                    self.message = None
                    self.msg_time = None
        else:
            self.draw_status(0, 3, gcode_info, toolhead_info)
    # Screen update helpers
    def draw_heater(self, x, y, info):
        temperature, target = info['temperature'], info['target']
        if target and abs(temperature - target) > 2.:
            s = "%3.0f%s%.0f" % (
                temperature, self.lcd_chip.char_right_arrow, target)
        else:
            s = "%3.0f" % (temperature,)
        if self.lcd_type == 'hd44780':
            s += self.lcd_chip.char_degrees
        self.lcd_chip.write_text(x, y, s)
    def draw_percent(self, x, y, width, value, align='^'):
        self.lcd_chip.write_text(x, y, '{:{}{}.0%}'.format(value, align, width))
    def draw_time(self, x, y, seconds):
        seconds = int(seconds)
        self.lcd_chip.write_text(x, y, "%02d:%02d" % (
            seconds // (60 * 60), (seconds // 60) % 60))
    def draw_status(self, x, y, gcode_info, toolhead_info):
        status = toolhead_info['status']
        if status == 'Printing' or gcode_info['busy']:
            pos = self.toolhead.get_position()
            status = "X%-4.0fY%-4.0fZ%-5.2f" % (pos[0], pos[1], pos[2])
        self.lcd_chip.write_text(x, y, status)
    def set_message(self, msg, msg_time=None):
        self.message = msg
        self.msg_time = msg_time
    # print progress: M73 P<percent>
    def cmd_M73(self, params):
        self.progress = self.gcode.get_int('P', params, minval=0, maxval=100)
        self.prg_time = 5.
    cmd_M117_help = "Show Message on Display"
    def cmd_M117(self, params):
        if '#original' in params:
            msg = params['#original']
            if not msg.startswith('M117'):
                # Parse out additional info if M117 recd during a print
                start = msg.find('M117')
                end = msg.rfind('*')
                msg = msg[start:end]
            if len(msg) > 5:
                msg = msg[5:]
                self.set_message(msg)
            else:
                self.set_message(None)

def load_config(config):
    return PrinterLCD(config)
