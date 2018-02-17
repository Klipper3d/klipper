# Basic LCD display support
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Aleph Objects, Inc <marcio@alephobjects.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, pins, math

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000


######################################################################
# HD44780 (20x4 text) lcd chip
######################################################################

class HD44780:
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
                raise pins.error("hd44780 can not invert pin")
        self.mcu = mcu
        self.mcu.add_config_cmd(
            "config_hd44780 oid=%d rs_pin=%s e_pin=%s"
            " d4_pin=%s d5_pin=%s d6_pin=%s d7_pin=%s" % (
                self.mcu.create_oid(), pins[0]['pin'], pins[1]['pin'],
                pins[2]['pin'], pins[3]['pin'], pins[4]['pin'], pins[5]['pin']))
        self.mcu.add_config_object(self)
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.send_data_cmd = self.send_cmds_cmd = None
        # framebuffers
        self.text_framebuffers = [
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 0],
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 0x40],
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 20],
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 0x40 + 20]]
        self.glyph_framebuffer = [
            [bytearray([0x00]*40), bytearray([0xff]*40), 0x40, 0]]
        self.graphics_framebuffer = [
            [bytearray([0x00]*16), bytearray([0xff]*16), 0x00, i]
            for i in range(64)]
        self.pending_framebuffers = (
            self.glyph_framebuffer + self.text_framebuffers)
    def build_config(self):
        self.send_cmds_cmd = self.mcu.lookup_command(
            "hd44780_send_cmds cmds=%*s")
        self.send_data_cmd = self.mcu.lookup_command(
            "hd44780_send_data data=%*s")
    def send(self, cmd_type, data):
        self.mcu.send(cmd_type.encode(data),
                      reqclock=BACKGROUND_PRIORITY_CLOCK, cq=self.cmd_queue)
    def flush(self):
        # Find all differences in the framebuffers and send them to the chip
        for fb in self.pending_framebuffers:
            new_data, old_data, cmd, offset = fb
            diffs = [[i, 1] for i, (nd, od) in enumerate(zip(new_data, old_data))
                     if nd != od]
            for i in range(len(diffs)-2, -1, -1):
                if diffs[i][0] + 4 >= diffs[i+1][0] and diffs[i+1][1] < 16:
                    diffs[i][1] = diffs[i+1][1] + diffs[i+1][0] - diffs[i][0]
                    del diffs[i+1]
            for pos, count in diffs:
                self.send(self.send_cmds_cmd, [cmd + offset + pos])
                self.send(self.send_data_cmd, new_data[pos:pos+count])
            fb[1] = new_data
        del self.pending_framebuffers[:]
    def init(self):
        reactor = self.printer.get_reactor()
        for cmd in [0x33, 0x33, 0x33, 0x22]:
            self.mcu.send(self.send_cmds_cmd.encode([cmd]), cq=self.cmd_queue)
            reactor.pause(reactor.monotonic() + .100)
        self.send(self.send_cmds_cmd, [0x08, 0x06, 0x10, 0x0c])
        self.flush()
    def load_glyph(self, glyph_id, data, alt_text):
        return alt_text


######################################################################
# ST7920 (128x64 graphics) lcd chip
######################################################################

class ST7920:
    def __init__(self, config):
        printer = config.get_printer()
        # pin config
        cs_pin = pins.setup_pin(printer, 'digital_out', config.get('cs_pin'))
        cs_pin.setup_start_value(1, 1, is_static=True)
        sclk_params = pins.get_printer_pins(printer).lookup_pin(
            'digital_out', config.get('sclk_pin'))
        sid_params = pins.get_printer_pins(printer).lookup_pin(
            'digital_out', config.get('sid_pin'))
        if sid_params['invert'] or sclk_params['invert']:
            raise pins.error("st7920 can not invert sclk or sid")
        if sid_params['chip'] is not sclk_params['chip']:
            raise pins.error("st7920 sclk must be on same mcu as sid")
        self.mcu = sclk_params['chip']
        self.mcu.add_config_cmd(
            "config_st7920 oid=%u sclk_pin=%s sid_pin=%s" % (
                self.mcu.create_oid(), sclk_params['pin'], sid_params['pin']))
        self.mcu.add_config_object(self)
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.send_data_cmd = self.send_cmds_cmd = None
        # framebuffers
        self.text_framebuffers = [
            [bytearray([' ']*16), bytearray([0x00]*16), 0x80, 0x00],
            [bytearray([' ']*16), bytearray([0x00]*16), 0x80, 0x10],
            [bytearray([' ']*16), bytearray([0x00]*16), 0x80, 0x08],
            [bytearray([' ']*16), bytearray([0x00]*16), 0x80, 0x18]]
        self.glyph_framebuffer = [
            [bytearray([0x00]*128), bytearray([0xff]*128), 0x40, 0]]
        self.graphics_framebuffer = [
            [bytearray([0x00]*16), bytearray([0xff]*16),
             i & 0x1f, (i & 0x20) >> 2]
            for i in range(64)]
        self.pending_framebuffers = (
            self.glyph_framebuffer + self.text_framebuffers
            + self.graphics_framebuffer)
    def build_config(self):
        self.send_cmds_cmd = self.mcu.lookup_command("st7920_send_cmds cmds=%*s")
        self.send_data_cmd = self.mcu.lookup_command("st7920_send_data data=%*s")
    def send(self, cmd_type, data):
        self.mcu.send(cmd_type.encode(data),
                      reqclock=BACKGROUND_PRIORITY_CLOCK, cq=self.cmd_queue)
    def flush(self):
        # Find all differences in the framebuffers and send them to the chip
        for fb in self.pending_framebuffers:
            new_data, old_data, cmd, offset = fb
            diffs = [[i, 1] for i, (nd, od) in enumerate(zip(new_data, old_data))
                     if nd != od]
            for i in range(len(diffs)-2, -1, -1):
                if diffs[i][0] + 5 >= diffs[i+1][0] and diffs[i+1][1] < 16:
                    diffs[i][1] = diffs[i+1][1] + diffs[i+1][0] - diffs[i][0]
                    del diffs[i+1]
            for pos, count in diffs:
                count += pos & 0x01
                count += count & 0x01
                pos = pos & ~0x01
                chip_pos = pos >> 1
                if cmd < 0x40:
                    # Graphics framebuffer update
                    self.send(self.send_cmds_cmd,
                              [0x26, 0x80+cmd, 0x80+offset+chip_pos, 0x22])
                else:
                    self.send(self.send_cmds_cmd, [cmd + offset + chip_pos])
                self.send(self.send_data_cmd, new_data[pos:pos+count])
            fb[1] = new_data
        del self.pending_framebuffers[:]
    def init(self):
        self.send(self.send_cmds_cmd, [0x06, 0x24, 0x02, 0x26, 0x22])
        self.flush()
    def load_glyph(self, glyph_id, data, alt_text):
        gfb = self.glyph_framebuffer[0]
        if gfb[0] is gfb[1]:
            gfb[0] = bytearray(gfb[1])
            self.pending_framebuffers.append(gfb)
        pos = glyph_id * 32
        for i, d in enumerate(data):
            gfb[0][pos + i*2] = (d >> 8) & 0xff
            gfb[0][pos + i*2 + 1] = d & 0xff
        return (0x00, glyph_id * 2)


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
];

bed1_icon = [
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
    0b0111111111111110,
    0b0111111111111110,
    0b0000000000000000,
    0b0000000000000000
];

bed2_icon = [
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
    0b0111111111111110,
    0b0111111111111110,
    0b0000000000000000,
    0b0000000000000000
];

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
];

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
];

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
];


######################################################################
# LCD screen updates
######################################################################

LCD_chips = { 'st7920': ST7920, 'hd44780': HD44780 }

class PrinterLCD:
    def __init__(self, config):
        printer = config.get_printer()
        self.lcd_chip = config.getchoice('lcd_type', LCD_chips)(config)
        # work timer
        self.reactor = printer.get_reactor()
        self.work_timer = self.reactor.register_timer(self.work_event)
        self.init_timer = self.reactor.register_timer(self.init_event)
        # Glyphs
        self.fan_glyphs = self.bed_glyphs = None
        # For the fan animation
        self.odd_frame = False
        # Information which is printed on the LCD screen
        self.extruder_count      = 1   # Can be 1 or 2
        self.extruder_1_temp     = 100
        self.extruder_1_target   = 210
        self.extruder_2_temp     = 178
        self.extruder_2_target   = 205
        self.feedrate_percentage = 100
        self.bed_temp            = 0
        self.bed_target          = 110
        self.print_progress      = 0
        self.print_time_hrs      = 0
        self.print_time_min      = 0
        self.fan_percentage      = 90
        self.position_x          = 0
        self.position_y          = 0
        self.position_z          = 0
        self.status_message      = "Ready" # If blank, position will be shown
    def printer_state(self, state):
        if state == 'ready':
            self.reactor.update_timer(self.init_timer, self.reactor.NOW)
    def init_event(self, eventtime):
        self.reactor.unregister_timer(self.init_timer)
        self.init_timer = None
        self.lcd_chip.init()

        # Load the icons
        self.fan_glyphs = [self.lcd_chip.load_glyph(0, fan1_icon, "f+"),
                           self.lcd_chip.load_glyph(1, fan2_icon, "f*")]
        self.bed_glyphs = [self.lcd_chip.load_glyph(2, bed1_icon, "b-"),
                           self.lcd_chip.load_glyph(3, bed2_icon, "b_")]
        self.draw_icon(0, 0, nozzle_icon)
        if self.extruder_count == 2:
            self.draw_icon(0, 1, nozzle_icon)
        self.draw_icon(10, 1, feedrate_icon)

        self.reactor.update_timer(self.work_timer, self.reactor.NOW)
        return self.reactor.NEVER
    def update_framebuffer(self, fb, pos, data):
        new_fb_data, old_fb_data = fb[:2]
        if new_fb_data is not old_fb_data:
            # Some changes are already pending to this buffer
            new_fb_data[pos:pos+len(data)] = data
        else:
            new_fb_data = bytearray(old_fb_data)
            new_fb_data[pos:pos+len(data)] = data
            if new_fb_data == old_fb_data:
                # No update
                return
            fb[0] = new_fb_data
            self.lcd_chip.pending_framebuffers.append(fb)
        if len(new_fb_data) > len(old_fb_data):
            del new_fb_data[len(old_fb_data):]
    def write_str(self, x, y, data):
        self.update_framebuffer(self.lcd_chip.text_framebuffers[y], x, data)
    def write_graphics(self, x, y, row, data):
        self.update_framebuffer(
            self.lcd_chip.graphics_framebuffer[y*16 + row], x, data)

    def demo_animation(self):
        self.print_progress  = (self.print_progress + 1) % 100
        self.extruder_1_temp = self.extruder_1_temp*0.95 + self.extruder_1_target*0.05
        self.extruder_2_temp = self.extruder_2_temp*0.95 + self.extruder_1_target*0.05
        self.bed_temp        = self.bed_temp*0.95 + self.bed_target*0.05
        self.print_time_min  += 1
        if self.print_time_min < 10 and self.print_time_hrs == 0:
            self.status_message = "Heating..."
        else:
            self.status_message = ""
        if self.print_time_min == 60:
            self.print_time_min = 0
            self.print_time_hrs += 1
        self.position_x = 150 + 150 * math.cos(2*math.pi*self.print_time_min/60)
        self.position_y = 150 + 150 * math.sin(2*math.pi*self.print_time_min/60)
        self.position_z = self.print_time_hrs * 0.01

    def work_event(self, eventtime):
        self.lcd_draw_interface()
        self.lcd_chip.flush()
        self.demo_animation()
        return eventtime + 1.

    # Draws an icon to the graphics framebuffer
    def draw_icon(self, x, y, data):
        for i, bits in enumerate(data):
            self.write_graphics(x, y, i, [(bits >> 8) & 0xff, bits & 0xff])

    # We animate the fan / bed by changing the active glyph
    def animate_fan(self):
        self.write_str(10, 0, self.fan_glyphs[self.odd_frame])
    def animate_bed(self):
        self.odd_frame = not self.odd_frame
        self.write_str(0, self.extruder_count, self.bed_glyphs[self.odd_frame])

    def draw_progress_bar(self, value):
        # Draw the percentage as text
        if self.extruder_count == 1:
            x, y, width = 0, 2, 10
            self.write_str(x, y, " {:^9}".format(str(value)+'%'))
        else:
            x, y, width = 10, 1, 6
            self.write_str(x, y, " {:^5}".format(str(value)+'%'))

        # Draw bar
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
        self.write_graphics(x, y, 0, [0xff]*width)
        for i in range(1, 15):
            self.write_graphics(x, y, i, data)
        self.write_graphics(x, y, 15, [0xff]*width)

    def draw_extruder_1_temp(self):
        self.write_str(2, 0, "%3d/%3d" % (
            self.extruder_1_temp, self.extruder_1_target))

    def draw_extruder_2_temp(self):
        self.write_str(2, 1, "%3d/%3d" % (
            self.extruder_2_temp, self.extruder_2_target))

    def draw_bed_temp(self):
        self.write_str(2, self.extruder_count, "%3d/%3d" % (
            self.bed_temp, self.bed_target))
        self.animate_bed()

    def draw_print_time(self):
        self.write_str(10, 2, " %02d:%02d" % (
            self.print_time_hrs, self.print_time_min))

    def draw_fan_speed(self):
        self.write_str(10, 0, "%s%3d" % (
            self.fan_glyphs[0], self.fan_percentage))
        if self.fan_percentage > 0:
            self.animate_fan()

    def draw_feedrate_percentage(self):
        # We only have enough room for the feedrate when
        # we have one extruder
        if self.extruder_count == 1:
            self.write_str(12, 1, "%3d%%" % (self.feedrate_percentage))

    def draw_status_message(self):
        self.write_str(0, 3, self.status_message)

    def draw_position(self):
        self.write_str(0, 3, "X%-4dY%-4dZ%-5.2f" % (
            self.position_x, self.position_y, self.position_z))

    # Paints the entire LCD interface
    def lcd_draw_interface(self):
        self.draw_extruder_1_temp()
        if self.extruder_count == 2:
            self.draw_extruder_2_temp()
        self.draw_bed_temp()
        self.draw_fan_speed()
        self.draw_print_time()
        self.draw_feedrate_percentage()
        if self.status_message:
            self.draw_status_message()
        else:
            self.draw_position()
        self.draw_progress_bar(self.print_progress);

def load_config(config):
    return PrinterLCD(config)
