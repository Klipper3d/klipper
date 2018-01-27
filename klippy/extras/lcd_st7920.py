# LCD support for the st7920 chip
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Aleph Objects, Inc <marcio@alephobjects.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, pins, math

CGRAM_ICON_1_ADDR = 0x0000
CGRAM_ICON_2_ADDR = 0x0010
CGRAM_ICON_3_ADDR = 0x0020
CGRAM_ICON_4_ADDR = 0x0030

CGRAM_ICON_1_CHAR = "\x00\x00"
CGRAM_ICON_2_CHAR = "\x00\x02"
CGRAM_ICON_3_CHAR = "\x00\x04"
CGRAM_ICON_4_CHAR = "\x00\x06"

DDRAM_LINE_1 = 0x00
DDRAM_LINE_2 = 0x10
DDRAM_LINE_3 = 0x08
DDRAM_LINE_4 = 0x18

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

class PrinterST7920:
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
        # output buffers
        self.pending_data = []
        self.pending_type = None
        # work timer
        self.reactor = printer.get_reactor()
        self.work_timer = self.reactor.register_timer(self.work_event)
        # State bits for ST7920
        self.extended_bit = False
        self.graphics_bit = False
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

    def build_config(self):
        self.send_cmds_cmd = self.mcu.lookup_command("st7920_send_cmds cmds=%*s")
        self.send_data_cmd = self.mcu.lookup_command("st7920_send_data data=%*s")
    def printer_state(self, state):
        if state == 'ready':
            self.lcd_init_interface()
            self.reactor.update_timer(self.work_timer, self.reactor.NOW)
    def flush(self):
        if not self.pending_data:
            return
        if self.pending_type == 'cmd':
            msg = self.send_cmds_cmd.encode(self.pending_data)
        else:
            msg = self.send_data_cmd.encode(self.pending_data)
        # XXX - send with low priority
        self.mcu.send(msg, cq=self.cmd_queue)
        del self.pending_data[:]
    def send_data(self, byte):
        if self.pending_type != 'data' or len(self.pending_data) >= 16:
            self.flush()
            self.pending_type = 'data'
        self.pending_data.append(byte)
    def send_command(self, cmd):
        if self.pending_type != 'cmd' or len(self.pending_data) >= 16:
            self.flush()
            self.pending_type = 'cmd'
        self.pending_data.append(cmd)
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
        self.flush()
        self.demo_animation()
        return eventtime + 1

    def lcd_write_byte(self, byte):
        self.send_data(byte)

    def lcd_write_word(self, word):
        self.send_data((word >> 8) & 0xff)
        self.send_data((word >> 0) & 0xff)

    def lcd_write_str(self, str):
        for c in str:
            self.lcd_write_byte(c)

    # Loads an icon into one of the four CGRAM
    # slots. These are useful for animated icons
    def load_cgram_icon(self, addr, data):
        self.lcd_set_cgram_address(addr);
        for i in range(16):
            self.lcd_write_word(data[i])

    # Draws an icon in GDRAM. The x position
    # is [1-8], while the y position is [1-4]
    def draw_gdram_icon(self, x, y, data):
        if(y > 2):
            # Handle the display folding
            y -= 2
            x += 8
        x -= 1
        y -= 1
        for i in range(0,16):
            self.lcd_set_gdram_address(x,i+y*16)
            self.lcd_write_word(data[i])

    # We animate the fan by changing a single word
    # in DDRAM to draw one or the other custom icon.
    def animate_fan(self):
        # Animate the fan icon
        self.lcd_set_ddram_address(DDRAM_LINE_1+5)
        self.lcd_write_str(CGRAM_ICON_3_CHAR if self.odd_frame else CGRAM_ICON_4_CHAR)

    def animate_bed(self):
        self.odd_frame = not self.odd_frame
        # Animate the build plate icon
        self.lcd_set_ddram_address(DDRAM_LINE_2 if self.extruder_count == 1 else DDRAM_LINE_3)
        self.lcd_write_str(CGRAM_ICON_1_CHAR if self.odd_frame else CGRAM_ICON_2_CHAR)


    # Although this is undocumented, the ST7920 allows the character
    # data buffer (DDRAM) to be used in conjunction with the graphics
    # bitmap buffer (CGRAM). The contents of the graphics buffer is
    # XORed with the data from the character generator. This allows
    # us to make the progess bar out of graphical data (the bar) and
    # text data (the percentage).
    def draw_progress_bar(self, value):
        top        = 16 + 1    # Top in pixels
        bottom     = 16 + 13   # Bottom in pixels
        left       = 5         # Left edge, in 16-bit words
        width      = 3         # Width of progress bar, in 16-bit words
        char_pcnt  = int(100/width) # How many percent does each 16-bit word represent?

        # If we have only one extruder, draw a long progress bar on the third line
        if self.extruder_count == 1:
            top    -= 16
            bottom -= 16
            left    = 8
            width   = 5

        # Draw the percentage as text in DDRAM
        if self.extruder_count == 1:
            self.lcd_set_ddram_address(DDRAM_LINE_3)
            self.lcd_write_str(" {:^9}".format(str(value)+'%'))
        else:
            self.lcd_set_ddram_address(DDRAM_LINE_2 + left)
            self.lcd_write_str(" {:^5}".format(str(value)+'%'))

        # Draw the progress bar as a bitmap in CGRAM
        for y in range(top,bottom+1):
            self.lcd_set_gdram_address(left,y)
            for x in range(width):
                gfx_word = 0x0000
                if (x+1)*char_pcnt <= value:
                    # Draw completely filled bytes
                    gfx_word = 0xFFFF
                elif (x*char_pcnt) < value:
                    # Draw partially filled bytes
                    gfx_word = -1 << 16-((value % char_pcnt)*16/char_pcnt)
                # Draw the frame around the progress bar
                if y == top or y == bottom:
                    # Draw top/bottom border
                    gfx_word = 0xFFFF
                elif x == (width-1):
                    # Draw right border
                    gfx_word |= 0x0001
                elif x == 0:
                    # Draw left border
                    gfx_word |= 0x8000
                self.lcd_write_word(gfx_word)

    def draw_extruder_1_temp(self):
        self.lcd_set_ddram_address(DDRAM_LINE_1+1)
        self.lcd_write_str("{:3d}/{:3d}".format(
            int(self.extruder_1_temp),
            int(self.extruder_1_target)
        ))

    def draw_extruder_2_temp(self):
        self.lcd_set_ddram_address(DDRAM_LINE_2+1)
        self.lcd_write_str("{:3d}/{:3d}".format(
            int(self.extruder_2_temp),
            int(self.extruder_2_target)
        ))

    def draw_bed_temp(self):
        self.lcd_set_ddram_address((DDRAM_LINE_2 if self.extruder_count == 1 else DDRAM_LINE_3)+1)
        self.lcd_write_str("{:3d}/{:3d}".format(
            int(self.bed_temp),
            int(self.bed_target)
        ))
        self.animate_bed()

    def draw_print_time(self):
        self.lcd_set_ddram_address((DDRAM_LINE_3 if self.extruder_count == 1 else DDRAM_LINE_3)+5)
        self.lcd_write_str(" {:02d}:{:02d}".format(
            int(self.print_time_hrs),
            int(self.print_time_min)
        ))

    def draw_fan_speed(self):
        self.lcd_set_ddram_address(DDRAM_LINE_1+5)
        self.lcd_write_str("{}{:3d}%".format(
            CGRAM_ICON_3_CHAR,
            int(self.fan_percentage)
        ))
        if self.fan_percentage > 0:
            self.animate_fan()

    def draw_feedrate_percentage(self):
        # We only have enough room for the feedrate when
        # we have one extruder
        if self.extruder_count == 1:
            self.lcd_set_ddram_address(DDRAM_LINE_2+6)
            self.lcd_write_str("{:3d}%".format(self.feedrate_percentage))

    def draw_status_message(self):
        self.lcd_set_ddram_address(DDRAM_LINE_4)
        self.lcd_write_str(self.status_message)

    def draw_position(self):
        self.lcd_set_ddram_address(DDRAM_LINE_4)
        self.lcd_write_str("X{:<4d}Y{:<4d}Z{:05.2f}".format(
            int(self.position_x),
            int(self.position_y),
            self.position_z
        ))

    def lcd_init_interface(self):
        self.lcd_entry_mode_select(True, False)
        self.lcd_clear_gdram()
        # Loads the extruder, bed and fan icons into the
        # four custom character slots.
        self.load_cgram_icon(CGRAM_ICON_1_ADDR, bed1_icon)
        self.load_cgram_icon(CGRAM_ICON_2_ADDR, bed2_icon)
        self.load_cgram_icon(CGRAM_ICON_3_ADDR, fan1_icon)
        self.load_cgram_icon(CGRAM_ICON_4_ADDR, fan2_icon)
        # Draw the static icons in GDRAM
        self.draw_gdram_icon(1,1,nozzle_icon)
        if self.extruder_count == 2:
            self.draw_gdram_icon(1,2,nozzle_icon)
        self.draw_gdram_icon(6,2,feedrate_icon)
        self.lcd_enable_graphics(True)

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

    # Clears the graphics buffer
    def lcd_clear_gdram(self):
        for y in range(32):
            self.lcd_set_gdram_address(0,y)
            for x in range(16):
                self.lcd_write_word(0)

    # ST7920 commands
    #   Reference: https://www.lcd-module.de/eng/pdf/zubehoer/st7920_chinese.pdf
    def lcd_clear(self, display_on, cursor_on, blink_on):
        # TODO: Figure out how to insert a 1.6ms delay after this command
        self.send_command(0x00000001)
    def lcd_home(self):
        self.send_command(0x00000010)
    def lcd_display_status(self, display_on, cursor_on, blink_on):
        self.send_command(0b00001000 |
                         (0b00000100 if display_on  else 0) |
                         (0b00000010 if  cursor_on  else 0) |
                         (0b00000001 if   blink_on  else 0))
    def lcd_extended_function_set(self, extended):
        if extended != self.extended_bit:
            self._lcd_extended_function_set(extended, self.graphics_bit)
    def lcd_enable_graphics(self, graphics):
        if graphics != self.graphics_bit:
            self._lcd_extended_function_set(self.extended_bit, graphics)
    def _lcd_extended_function_set(self, extended, graphics):
        self.extended_bit = extended
        self.graphics_bit = graphics
        self.send_command(0b00100000 |
                        (0b00000100 if self.extended_bit else 0) |
                        (0b00000010 if self.graphics_bit else 0))
    def lcd_scroll_or_addr_select(self, sa):
        self.send_command(0b00100010 |
                         (0b00000001 if sa          else 0))
    def lcd_entry_mode_select(self, ac_increase, shift):
        self.send_command(0b00000100 |
                         (0b00000010 if ac_increase else 0) |
                         (0b00000001 if       shift else 0))
    def lcd_set_ddram_address(self, addr):
        self.send_command(0b10000000 | (addr & 0b00111111))
    def lcd_set_cgram_address(self, addr):
        self.lcd_extended_function_set(True);
        self.lcd_scroll_or_addr_select(False);
        self.lcd_extended_function_set(False);
        self.send_command(0b01000000 | (addr & 0b00111111))
    def lcd_set_gdram_address(self, x, y):
        self.lcd_extended_function_set(True);
        self.send_command(0b10000000 | (y    & 0b01111111))
        self.send_command(0b10000000 | (x    & 0b00001111))
        self.lcd_extended_function_set(False);

def load_config(config):
    return PrinterST7920(config)
