# Basic LCD display support
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Aleph Objects, Inc <marcio@alephobjects.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import pins

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
        self.text_framebuffer = [
            bytearray([' ']*80), bytearray([0x00]*80), 0x80]
        self.glyph_framebuffer = [
            bytearray([0x00]*40), bytearray([0xff]*40), 0x40]
        self.framebuffers = [self.text_framebuffer, self.glyph_framebuffer]
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
        for fb in self.framebuffers:
            new_data, old_data, fb_id = fb
            if new_data == old_data:
                continue
            diffs = [[i, 1] for i, (nd, od) in enumerate(zip(new_data, old_data))
                     if nd != od]
            for i in range(len(diffs)-2, -1, -1):
                if diffs[i][0] + 4 >= diffs[i+1][0] and diffs[i+1][1] < 16:
                    diffs[i][1] = diffs[i+1][1] + diffs[i+1][0] - diffs[i][0]
                    del diffs[i+1]
            for pos, count in diffs:
                chip_pos = pos
                if fb_id == 0x80 and pos >= 40:
                    chip_pos += 0x40 - 40
                self.send(self.send_cmds_cmd, [fb_id + chip_pos])
                self.send(self.send_data_cmd, new_data[pos:pos+count])
            fb[1] = bytearray(new_data)
    def init(self):
        reactor = self.printer.get_reactor()
        for cmd in [0x33, 0x33, 0x33, 0x22]:
            self.mcu.send(self.send_cmds_cmd.encode([cmd]), cq=self.cmd_queue)
            reactor.pause(reactor.monotonic() + .100)
        self.send(self.send_cmds_cmd, [0x08, 0x06, 0x10, 0x0c])
        self.flush()
    def load_glyph(self, glyph_id, data, alt_text):
        return alt_text
    def write_str(self, x, y, data):
        if x + len(data) > 20:
            data = data[:20 - min(x, 20)]
        pos = [0, 40, 20, 60][y] + x
        self.text_framebuffer[0][pos:pos+len(data)] = data
    def write_graphics(self, x, y, row, data):
        pass


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
        self.text_framebuffer = [
            bytearray([' ']*64), bytearray([0x00]*64), 0x80]
        self.glyph_framebuffer = [
            bytearray([0x00]*128), bytearray([0xff]*128), 0x40]
        self.graphics_framebuffers = [
            [bytearray([0x00]*32), bytearray([0xff]*32), i]
            for i in range(32)]
        self.framebuffers = ([self.text_framebuffer, self.glyph_framebuffer]
                             + self.graphics_framebuffers)
    def build_config(self):
        self.send_cmds_cmd = self.mcu.lookup_command("st7920_send_cmds cmds=%*s")
        self.send_data_cmd = self.mcu.lookup_command("st7920_send_data data=%*s")
    def send(self, cmd_type, data):
        self.mcu.send(cmd_type.encode(data),
                      reqclock=BACKGROUND_PRIORITY_CLOCK, cq=self.cmd_queue)
    def flush(self):
        # Find all differences in the framebuffers and send them to the chip
        for fb in self.framebuffers:
            new_data, old_data, fb_id = fb
            if new_data == old_data:
                continue
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
                if fb_id < 0x40:
                    # Graphics framebuffer update
                    self.send(self.send_cmds_cmd,
                              [0x26, 0x80 + fb_id, 0x80 + chip_pos, 0x22])
                else:
                    self.send(self.send_cmds_cmd, [fb_id + chip_pos])
                self.send(self.send_data_cmd, new_data[pos:pos+count])
            fb[1] = bytearray(new_data)
    def init(self):
        self.send(self.send_cmds_cmd, [0x06, 0x24, 0x02, 0x26, 0x22])
        self.flush()
    def load_glyph(self, glyph_id, data, alt_text):
        if len(data) > 32:
            data = data[:32]
        pos = min(glyph_id * 32, 96)
        self.glyph_framebuffer[0][pos:pos+len(data)] = data
        return (0x00, glyph_id * 2)
    def write_str(self, x, y, data):
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
        self.printer = config.get_printer()
        self.lcd_chip = config.getchoice('lcd_type', LCD_chips)(config)
        # work timer
        self.reactor = self.printer.get_reactor()
        self.work_timer = self.reactor.register_timer(self.work_event)
        self.init_timer = self.reactor.register_timer(self.init_event)
        # glyphs
        self.fan_glyphs = self.bed_glyphs = None
        # printer objects
        self.gcode = self.toolhead = self.sdcard = None
        self.fan = self.extruder0 = self.extruder1 = self.heater_bed = None
    # Initialization
    def printer_state(self, state):
        if state == 'ready':
            self.reactor.update_timer(self.init_timer, self.reactor.NOW)
    def init_event(self, eventtime):
        self.reactor.unregister_timer(self.init_timer)
        self.init_timer = None
        self.lcd_chip.init()
        # Load printer objects
        self.gcode = self.printer.lookup_object('gcode')
        self.toolhead = self.printer.lookup_object('toolhead')
        self.sdcard = self.printer.lookup_object('virtual_sdcard', None)
        self.fan = self.printer.lookup_object('fan', None)
        self.extruder0 = self.printer.lookup_object('extruder0', None)
        self.extruder1 = self.printer.lookup_object('extruder1', None)
        self.heater_bed = self.printer.lookup_object('heater_bed', None)
        # Load the icons
        self.fan_glyphs = [self.load_glyph(0, fan1_icon, "f*"),
                           self.load_glyph(1, fan2_icon, "f+")]
        self.bed_glyphs = [self.load_glyph(2, bed1_icon, "b_"),
                           self.load_glyph(3, bed2_icon, "b-")]
        self.draw_icon(0, 0, nozzle_icon)
        if self.extruder1 is not None:
            self.draw_icon(0, 1, nozzle_icon)
        self.draw_icon(10, 1, feedrate_icon)

        self.reactor.update_timer(self.work_timer, self.reactor.NOW)
        return self.reactor.NEVER
    # Screen updating
    def animate_glyphs(self, eventtime, x, y, glyphs, do_animate):
        frame = do_animate and int(eventtime) & 1
        self.lcd_chip.write_str(x, y, glyphs[frame])
    def draw_progress_bar(self, x, y, width, value):
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
    def work_event(self, eventtime):
        write_str = self.lcd_chip.write_str
        gcode_info = self.gcode.get_status(eventtime)
        if self.extruder0 is not None:
            info = self.extruder0.get_heater().get_status(eventtime)
            write_str(2, 0, "%3d/%-3d" % (info['temperature'], info['target']))
        extruder_count = 1
        if self.extruder1 is not None:
            info = self.extruder1.get_heater().get_status(eventtime)
            write_str(2, 1, "%3d/%-3d" % (info['temperature'], info['target']))
            extruder_count = 2
        else:
            # Display g-code speed override
            write_str(12, 1, "%3d%%" % (gcode_info['speed_factor'] * 100.,))
        if self.heater_bed is not None:
            info = self.heater_bed.get_status(eventtime)
            write_str(2, extruder_count, "%3d/%-3d" % (
                info['temperature'], info['target']))
            self.animate_glyphs(eventtime, 0, extruder_count, self.bed_glyphs,
                                info['target'] != 0.)
        if self.fan is not None:
            info = self.fan.get_status(eventtime)
            write_str(12, 0, "%3d%%" % (info['speed'] * 100.,))
            self.animate_glyphs(eventtime, 10, 0, self.fan_glyphs,
                                info['speed'] != 0.)
        if self.sdcard is not None:
            info = self.sdcard.get_status(eventtime)
            progress = int(info['progress'] * 100.)
            if extruder_count == 1:
                write_str(0, 2, " {:^9}".format(str(progress)+'%'))
                self.draw_progress_bar(0, 2, 10, progress)
            else:
                write_str(10, 1, " {:^5}".format(str(progress)+'%'))
                self.draw_progress_bar(10, 1, 6, progress)
        info = self.toolhead.get_status(eventtime)
        printing_time = int(info['printing_time'])
        write_str(10, 2, " %02d:%02d" % (
            printing_time // (60 * 60), (printing_time // 60) % 60))
        status = info['status']
        if status == 'Printing' or gcode_info['busy']:
            pos = self.toolhead.get_position()
            write_str(0, 3, "X%-4dY%-4dZ%-5.2f    " % (pos[0], pos[1], pos[2]))
        else:
            write_str(0, 3, "%-20s" % (status,))
        self.lcd_chip.flush()
        return eventtime + .500
    # Icon handling
    def draw_icon(self, x, y, data):
        for i, bits in enumerate(data):
            self.lcd_chip.write_graphics(
                x, y, i, [(bits >> 8) & 0xff, bits & 0xff])
    def load_glyph(self, glyph_id, data, alt_text):
        glyph = [0x00] * (len(data) * 2)
        for i, bits in enumerate(data):
            glyph[i*2] = (bits >> 8) & 0xff
            glyph[i*2 + 1] = bits & 0xff
        return self.lcd_chip.load_glyph(glyph_id, glyph, alt_text)

def load_config(config):
    return PrinterLCD(config)
