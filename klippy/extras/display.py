# Basic LCD display support
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Aleph Objects, Inc <marcio@alephobjects.com>
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
        self.flush()
    def load_glyph(self, glyph_id, data, alt_text):
        return alt_text
    def write_text(self, x, y, data):
        if x + len(data) > 20:
            data = data[:20 - min(x, 20)]
        pos = [0, 40, 20, 60][y] + x
        self.text_framebuffer[0][pos:pos+len(data)] = data
    def write_graphics(self, x, y, row, data):
        pass
    def clear(self):
        self.text_framebuffer[0][:] = ' '*80


######################################################################
# ST7920 (128x64 graphics) lcd chip
######################################################################

ST7920_DELAY = .000020 # Spec says 72us, but faster is possible in practice

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
            " delay_ticks=%d" % (
                self.oid, self.pins[0], self.pins[1], self.pins[2],
                self.mcu.seconds_to_clock(ST7920_DELAY)))
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
    def load_glyph(self, glyph_id, data, alt_text):
        if len(data) > 32:
            data = data[:32]
        pos = min(glyph_id * 32, 96)
        self.glyph_framebuffer[0][pos:pos+len(data)] = data
        return (0x00, glyph_id * 2)
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
];

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
];

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
        # glyphs
        self.fan_glyphs = self.heat_glyphs = None
        # printer objects
        self.gcode = self.toolhead = self.sdcard = None
        self.fan = self.extruder0 = self.extruder1 = self.heater_bed = None
    # Initialization
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
            # Load glyphs
            self.fan_glyphs = [self.load_glyph(0, fan1_icon, "f*"),
                               self.load_glyph(1, fan2_icon, "f+")]
            self.heat_glyphs = [self.load_glyph(2, heat1_icon, "b_"),
                                self.load_glyph(3, heat2_icon, "b-")]
            # Start screen update timer
            self.reactor.update_timer(self.work_timer, self.reactor.NOW)
    # Glyphs
    def load_glyph(self, glyph_id, data, alt_text):
        glyph = [0x00] * (len(data) * 2)
        for i, bits in enumerate(data):
            glyph[i*2] = (bits >> 8) & 0xff
            glyph[i*2 + 1] = bits & 0xff
        return self.lcd_chip.load_glyph(glyph_id, glyph, alt_text)
    def animate_glyphs(self, eventtime, x, y, glyphs, do_animate):
        frame = do_animate and int(eventtime) & 1
        self.lcd_chip.write_text(x, y, glyphs[frame])
    # Graphics drawing
    def draw_icon(self, x, y, data):
        for i, bits in enumerate(data):
            self.lcd_chip.write_graphics(
                x, y, i, [(bits >> 8) & 0xff, bits & 0xff])
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
    # Screen updating
    def format_temperature(self, info):
        temperature, target = info['temperature'], info['target']
        if target and abs(temperature - target) > 2.:
            return "%3d%s%-3d" % (temperature, self.lcd_chip.char_right_arrow, target)
        return "%3d" % (temperature)
    def work_event(self, eventtime):
        self.lcd_chip.clear()
        write_text = self.lcd_chip.write_text
        # Heaters
        if self.extruder0 is not None:
            info = self.extruder0.get_heater().get_status(eventtime)
            self.draw_icon(0, 0, nozzle_icon)
            write_text(2, 0, self.format_temperature(info))
        extruder_count = 1
        if self.extruder1 is not None:
            info = self.extruder1.get_heater().get_status(eventtime)
            self.draw_icon(0, 1, nozzle_icon)
            write_text(2, 1, self.format_temperature(info))
            extruder_count = 2
        if self.heater_bed is not None:
            info = self.heater_bed.get_status(eventtime)
            self.draw_icon(0, extruder_count, bed_icon)
            if info['target']:
                self.animate_glyphs(eventtime, 0, extruder_count,
                                    self.heat_glyphs, True)
            write_text(2, extruder_count, self.format_temperature(info))
        # Fan speed
        if self.fan is not None:
            info = self.fan.get_status(eventtime)
            self.animate_glyphs(eventtime, 10, 0, self.fan_glyphs,
                                info['speed'] != 0.)
            write_text(12, 0, "%3d%%" % (info['speed'] * 100.,))
        # SD card print progress
        if self.sdcard is not None:
            info = self.sdcard.get_status(eventtime)
            progress = int(info['progress'] * 100.)
            if extruder_count == 1:
                write_text(0, 2, " {:^9}".format(str(progress)+'%'))
                self.draw_progress_bar(0, 2, 10, progress)
            else:
                write_text(10, 1, " {:^5}".format(str(progress)+'%'))
                self.draw_progress_bar(10, 1, 6, progress)
        # G-Code speed factor
        gcode_info = self.gcode.get_status(eventtime)
        if extruder_count == 1:
            self.draw_icon(10, 1, feedrate_icon)
            write_text(12, 1, "%3d%%" % (gcode_info['speed_factor'] * 100.,))
        # Printing time
        toolhead_info = self.toolhead.get_status(eventtime)
        printing_time = int(toolhead_info['printing_time'])
        write_text(10, 2, " %02d:%02d" % (
            printing_time // (60 * 60), (printing_time // 60) % 60))
        # Printer status
        status = toolhead_info['status']
        if status == 'Printing' or gcode_info['busy']:
            pos = self.toolhead.get_position()
            status = "X%-4dY%-4dZ%-5.2f" % (pos[0], pos[1], pos[2])
        write_text(0, 3, status)
        self.lcd_chip.flush()
        return eventtime + .500

def load_config(config):
    return PrinterLCD(config)
