# Support for UC1701 (128x64 graphics) LCD displays
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Eric Callahan  <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import icons, font8x14, extras.bus

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

TextGlyphs = { 'right_arrow': '\x1a', 'degrees': '\xf8' }

class UC1701:
    CURRENT_BUF, OLD_BUF = 0, 1
    EMPTY_CHAR = (0, 32, 255)
    def __init__(self, config):
        self.spi = extras.bus.MCU_SPI_from_config(config, 0,
                                                  default_speed=10000000)
        mcu = self.spi.get_mcu()
        # Create a0 pin
        ppins = config.get_printer().lookup_object('pins')
        a0_pin_params = ppins.lookup_pin(config.get('a0_pin'))
        if a0_pin_params['chip'] != mcu:
            raise ppins.error("uc1701 all pins must be on same mcu")
        self.a0_oid = mcu.create_oid()
        mcu.add_config_cmd("config_digital_out oid=%d pin=%s"
                           " value=%d default_value=%d max_duration=%d" % (
                               self.a0_oid, a0_pin_params['pin'], 0, 0, 0))
        mcu.register_config_callback(self.build_config)
        self.update_pin_cmd = None
        # framebuffers
        self.vram = ([bytearray(128) for i in range(8)],
                    [bytearray('~'*128) for i in range(8)])
    def build_config(self):
        self.update_pin_cmd = self.spi.get_mcu().lookup_command(
            "update_digital_out oid=%c value=%c",
            cq=self.spi.get_command_queue())
    def send(self, cmds, is_data=False):
        if is_data:
            self.update_pin_cmd.send([self.a0_oid, 1], reqclock=BACKGROUND_PRIORITY_CLOCK)
        else:
            self.update_pin_cmd.send([self.a0_oid, 0], reqclock=BACKGROUND_PRIORITY_CLOCK)
        self.spi.spi_send(cmds, reqclock=BACKGROUND_PRIORITY_CLOCK)
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
        self.send(init_cmds)
        self.send([0xA5]) # display all
        self.send([0xA4]) # normal display
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
    def write_text(self, x, y, data):
        if x + len(data) > 16:
            data = data[:16 - min(x, 16)]
        pix_x = x*8
        pix_y = y*16
        for c in data:
            c_idx = ord(c) & 0xFF
            if c_idx in self.EMPTY_CHAR:
                # Empty char
                pix_x += 8
                continue
            char = font8x14.VGA_FONT[c_idx]
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
    def write_glyph(self, x, y, glyph_name):
        icon = icons.Icons16x16.get(glyph_name)
        if icon is not None:
            # Draw icon in graphics mode
            for i, bits in enumerate(icon):
                self.write_graphics(x, y, i, [(bits >> 8) & 0xff, bits & 0xff])
            return 2
        char = TextGlyphs.get(glyph_name)
        if char is not None:
            # Draw character
            self.write_text(x, y, char)
            return 1
        return 0
    def clear(self):
        zeros = bytearray(128)
        for page in self.vram[self.CURRENT_BUF]:
            page[:] = zeros
