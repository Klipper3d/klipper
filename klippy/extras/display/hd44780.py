# Support for HD44780 (20x4 text) LCD displays
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

HD44780_DELAY = .000040

class HD44780:
    def __init__(self, config):
        self.printer = config.get_printer()
        # pin config
        ppins = self.printer.lookup_object('pins')
        pins = [ppins.lookup_pin(config.get(name + '_pin'))
                for name in ['rs', 'e', 'd4', 'd5', 'd6', 'd7']]
        mcu = None
        for pin_params in pins:
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("hd44780 all pins must be on same mcu")
            mcu = pin_params['chip']
        self.pins = [pin_params['pin'] for pin_params in pins]
        self.cmd_delay = config.getfloat('cmd_delay', HD44780_DELAY,
            minval=0, maxval=0.1)
        self.mcu = mcu
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self.build_config)
        self.send_data_cmd = self.send_cmds_cmd = None
        # framebuffers
        self.text_framebuffers = [bytearray(' '*40), bytearray(' '*40)]
        self.glyph_framebuffer = bytearray(64)
        self.all_framebuffers = [
            # Text framebuffers
            (self.text_framebuffers[0], bytearray('~'*40), 0x80),
            (self.text_framebuffers[1], bytearray('~'*40), 0xc0),
            # Glyph framebuffer
            (self.glyph_framebuffer, bytearray('~'*64), 0x40) ]
    def build_config(self):
        self.mcu.add_config_cmd(
            "config_hd44780 oid=%d rs_pin=%s e_pin=%s"
            " d4_pin=%s d5_pin=%s d6_pin=%s d7_pin=%s delay_ticks=%d" % (
                self.oid, self.pins[0], self.pins[1],
                self.pins[2], self.pins[3], self.pins[4], self.pins[5],
                self.mcu.seconds_to_clock(self.cmd_delay)))
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
        curtime = self.printer.get_reactor().monotonic()
        print_time = self.mcu.estimated_print_time(curtime)
        # Program 4bit / 2-line mode and then issue 0x02 "Home" command
        # 0x32 0x28 by nibles: 0x3 - set 8 bit, 0x2 - set 4bit, 0x2,0x8 - config
        init = [[0x33], [0x33], [0x32, 0x28, 0x02]]
        # Reset (set positive direction ; enable display and hide cursor)
        init.append([0x06, 0x0c])
        for i, cmds in enumerate(init):
            minclock = self.mcu.print_time_to_clock(print_time + i * .100)
            self.send_cmds_cmd.send([self.oid, cmds], minclock=minclock)
        # Add custom fonts
        self.glyph_framebuffer[:len(HD44780_chars)] = HD44780_chars
        for i in range(len(self.glyph_framebuffer)):
            self.all_framebuffers[2][1][i] = self.glyph_framebuffer[i] ^ 1
        self.flush()
    def write_text(self, x, y, data):
        if x + len(data) > 20:
            data = data[:20 - min(x, 20)]
        pos = x + ((y & 0x02) >> 1) * 20
        self.text_framebuffers[y & 1][pos:pos+len(data)] = data
    def write_glyph(self, x, y, glyph_name):
        char = TextGlyphs.get(glyph_name)
        if char is not None:
            # Draw character
            self.write_text(x, y, char)
            return 1
        return 0
    def write_graphics(self, x, y, pixel_row, pixel_col):
        pass
    def clear(self):
        spaces = ' ' * 40
        self.text_framebuffers[0][:] = spaces
        self.text_framebuffers[1][:] = spaces
    def get_dimensions(self):
        return (20, 4)

HD44780_chars = [
    # Extruder (a thermometer)
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
    # Feed rate
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
    # Fan
    0b00000,
    0b10011,
    0b11010,
    0b00100,
    0b01011,
    0b11001,
    0b00000,
    0b00000,
]

TextGlyphs = {
    'right_arrow': '\x7e',
    'extruder': '\x00',
    'bed': '\x01', 'bed_heat1': '\x01', 'bed_heat2': '\x01',
    'feedrate': '\x02',
    'clock': '\x03',
    'degrees': '\x04',
    'usb': '\x05',
    'sd': '\x06',
    'fan': '\x07', 'fan1': '\x07', 'fan2': '\x07',
}
