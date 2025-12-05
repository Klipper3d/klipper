# Support for GC9A01 (240x240 circular TFT) displays
#
# Copyright (C) 2025  Your Name <your@email.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from .. import bus
from . import font8x14

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

# GC9A01 timing
GC9A01_CMD_DELAY = .000020  # 20us between commands

# Display dimensions
DISPLAY_WIDTH = 240
DISPLAY_HEIGHT = 240

class GC9A01:
    def __init__(self, config):
        self.printer = config.get_printer()

        # SPI setup
        self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=4000000)
        self.mcu = self.spi.get_mcu()

        # Pin setup
        dc_pin = config.get('dc_pin')
        reset_pin = config.get('reset_pin')
        ppins = self.printer.lookup_object('pins')

        # Parse pins
        self.dc_pin_params = ppins.lookup_pin(dc_pin, can_invert=False)
        self.reset_pin_params = ppins.lookup_pin(reset_pin, can_invert=False)

        # Verify pins are on the same MCU as SPI
        if self.dc_pin_params['chip'] != self.mcu:
            raise ppins.error("GC9A01 dc_pin must be on same mcu as spi")
        if self.reset_pin_params['chip'] != self.mcu:
            raise ppins.error("GC9A01 reset_pin must be on same mcu as spi")

        # Display configuration
        base_width = config.getint('width', DISPLAY_WIDTH)
        base_height = config.getint('height', DISPLAY_HEIGHT)

        # Rotation (0, 90, 180, 270)
        self.rotation = config.getint('rotation', 0)
        if self.rotation not in [0, 90, 180, 270]:
            raise config.error("GC9A01 rotation must be 0, 90, 180, or 270")

        # Swap dimensions for 90/270 degree rotations
        if self.rotation in [90, 270]:
            self.width = base_height
            self.height = base_width
        else:
            self.width = base_width
            self.height = base_height

        # Display colors (palette indices 0-15)
        self.fg_color = config.getint('fg_color', 15,
                                      minval=0, maxval=15)
        self.bg_color = config.getint('bg_color', 0,
                                      minval=0, maxval=15)

        # Framebuffer for 4-bit color (2 pixels per byte)
        fb_size = (self.width * self.height + 1) // 2
        self.framebuffer = bytearray(fb_size)
        self.glyphs = {}

        self.window_x0 = -1
        self.window_y0 = -1
        self.window_x1 = -1
        self.window_y1 = -1

        # Dirty region tracking
        self.dirty_x0 = self.width
        self.dirty_y0 = self.height
        self.dirty_x1 = -1
        self.dirty_y1 = -1

        # Setup MCU communication
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self.build_config)
        self.set_window_cmd = None
        self.start_write_cmd = None
        self.send_data_cmd = None
        self.fill_rect_cmd = None
        self.set_palette_cmd = None

        # Register with printer
        self.printer.register_event_handler("klippy:ready", self._handle_ready)

        logging.debug("GC9A01 display initialized")

    def build_config(self):
        """Configure the GC9A01 device on the microcontroller"""
        try:
            # Get pin strings
            dc_pin = self.dc_pin_params['pin']
            reset_pin = self.reset_pin_params['pin']

            # Configure the GC9A01 device
            cmd_delay_ticks = self.mcu.seconds_to_clock(GC9A01_CMD_DELAY)
            self.mcu.add_config_cmd(
                "config_gc9a01 oid=%d spi_oid=%d dc_pin=%s reset_pin=%s"
                " rotation=%d delay_ticks=%d" % (
                    self.oid, self.spi.get_oid(),
                    dc_pin, reset_pin, self.rotation, cmd_delay_ticks))

            # Setup command queue and lookup commands
            cmd_queue = self.spi.get_command_queue()
            self.set_window_cmd = self.mcu.lookup_command(
                "gc9a01_set_window oid=%c x0=%hu y0=%hu x1=%hu y1=%hu",
                cq=cmd_queue)
            self.start_write_cmd = self.mcu.lookup_command(
                "gc9a01_start_write oid=%c", cq=cmd_queue)
            self.send_data_cmd = self.mcu.lookup_command(
                "gc9a01_send_data oid=%c data=%*s", cq=cmd_queue)
            self.fill_rect_cmd = self.mcu.lookup_command(
                "gc9a01_fill_rect oid=%c x0=%hu y0=%hu x1=%hu y1=%hu color=%c",
                cq=cmd_queue)
            self.set_palette_cmd = self.mcu.lookup_command(
                "gc9a01_set_palette oid=%c c1=%hu c2=%hu c3=%hu c4=%hu "
                "c5=%hu c6=%hu c7=%hu c8=%hu c9=%hu c10=%hu c11=%hu "
                "c12=%hu c13=%hu c14=%hu", cq=cmd_queue)

            logging.debug("GC9A01 MCU commands configured")
        except Exception as e:
            logging.error("GC9A01 build_config error: %s", e)
            raise

    def _handle_ready(self):
        """Called when Klipper is ready"""
        try:
            self.clear()
            self.fill_rectangle(0, 0, self.width - 1, self.height - 1,
                                self.bg_color)
            logging.debug("GC9A01 display ready")
        except Exception as e:
            logging.error("GC9A01 ready handler error: %s", e)

    def set_window(self, x0, y0, x1, y1):
        """Set the drawing window on the display"""
        if self.set_window_cmd is None:
            return
        if (self.window_x0 == x0 and self.window_y0 == y0 and
            self.window_x1 == x1 and self.window_y1 == y1):
            return  # No change
        self.window_x0 = x0
        self.window_y0 = y0
        self.window_x1 = x1
        self.window_y1 = y1
        logging.debug("GC9A01 set_window %d,%d to %d,%d", x0, y0, x1, y1)
        self.set_window_cmd.send([self.oid, x0, y0, x1, y1],
                                reqclock=BACKGROUND_PRIORITY_CLOCK)

    def fill_rectangle(self, x0, y0, x1, y1, color_idx):
        """Fill a rectangle on the display with a palette color index"""
        if self.fill_rect_cmd is None:
            return
        color_idx &= 0x0F
        logging.debug("GC9A01 fill_rect %d,%d to %d,%d color_idx=%d",
                     x0, y0, x1, y1, color_idx)
        self.fill_rect_cmd.send([self.oid, x0, y0, x1, y1, color_idx],
                            reqclock=BACKGROUND_PRIORITY_CLOCK)
        self.window_x0 = x0
        self.window_y0 = y0
        self.window_x1 = x1
        self.window_y1 = y1

    def set_palette(self, colors):
        """Upload custom palette colors (indices 1-14)
        colors: list of 14 RGB565 values
        """
        if self.set_palette_cmd is None:
            return
        if len(colors) != 14:
            logging.error("GC9A01 set_palette: need exactly 14 colors")
            return
        self.set_palette_cmd.send([self.oid] + colors,
                                   reqclock=BACKGROUND_PRIORITY_CLOCK)

    def init(self):
        """Initialize the display"""
        self.clear()
        logging.debug("GC9A01 init called")

    def clear(self):
        """Clear the framebuffer (fill with background color index)"""
        fill_byte = (self.bg_color << 4) | self.bg_color
        for i in range(len(self.framebuffer)):
            self.framebuffer[i] = fill_byte
        self.dirty_x0 = self.width
        self.dirty_y0 = self.height
        self.dirty_x1 = -1
        self.dirty_y1 = -1

    def flush(self):
        """Send updated framebuffer regions to the display"""
        if self.set_window_cmd is None or self.send_data_cmd is None:
            return

        # Check if there are any dirty regions
        if self.dirty_x1 < 0 or self.dirty_y1 < 0:
            logging.debug("GC9A01 flush: no dirty regions, skipping")
            return

        # Align dirty region to even pixel boundaries (2 pixels per byte)
        x0 = self.dirty_x0 & ~1  # Round down to even
        y0 = self.dirty_y0
        x1 = self.dirty_x1 | 1   # Round up to odd
        y1 = self.dirty_y1

        self.set_window(x0, y0, x1, y1)

        # Start write operation (send RAMWR once)
        self.start_write_cmd.send([self.oid],
                                   reqclock=BACKGROUND_PRIORITY_CLOCK)

        # Send framebuffer data for dirty region
        max_chunk = 32  # Max bytes per send
        dirty_width = x1 - x0 + 1

        for y in range(y0, y1 + 1):
            # Calculate byte positions for this row
            row_start_pixel = y * self.width + x0
            row_end_pixel = row_start_pixel + dirty_width

            byte_start = row_start_pixel // 2
            byte_end = (row_end_pixel + 1) // 2

            row_data = self.framebuffer[byte_start:byte_end]

            # Send row in chunks
            for i in range(0, len(row_data), max_chunk):
                chunk = row_data[i:i+max_chunk]
                self.send_data_cmd.send([self.oid, chunk],
                                        reqclock=BACKGROUND_PRIORITY_CLOCK)

        # Reset dirty region
        self.dirty_x0 = self.width
        self.dirty_y0 = self.height
        self.dirty_x1 = -1
        self.dirty_y1 = -1

    def draw_pixel(self, x, y, color_idx):
        """Draw a single pixel using 4-bit color index (0-15)"""
        if x >= self.width or y >= self.height or x < 0 or y < 0:
            return

        color_idx &= 0x0F
        pixel_num = y * self.width + x
        byte_pos = pixel_num // 2
        is_high = (pixel_num % 2) == 0

        old_byte = self.framebuffer[byte_pos]
        if is_high:
            new_byte = (color_idx << 4) | (old_byte & 0x0F)
        else:
            new_byte = (old_byte & 0xF0) | color_idx

        if old_byte == new_byte:
            return

        self.framebuffer[byte_pos] = new_byte

        # Update dirty region
        self.dirty_x0 = min(self.dirty_x0, x)
        self.dirty_y0 = min(self.dirty_y0, y)
        self.dirty_x1 = max(self.dirty_x1, x)
        self.dirty_y1 = max(self.dirty_y1, y)

    def fill_rect(self, x, y, w, h, color_idx):
        """Fill a rectangle with a palette color index"""
        self.fill_rectangle(x, y, x + w - 1, y + h - 1, color_idx)

    # Display interface compatibility methods
    # For testing purposes only
    def get_dimensions(self):
        """Return display dimensions"""
        return (16, 4) # Use only central area (128x32) for compatibility

    def write_text(self, x, y, data):
        """Text display in central area (128x32) using 8x14 font"""
        if not data:
            return

        char_width = 8
        char_height = 14
        centered_x = int((240 - 128)/2)
        centered_y = int((240 - 32)/2)

        for i, char_code in enumerate(data):
            if char_code >= len(font8x14.VGA_FONT):
                char_code = 0

            glyph = font8x14.VGA_FONT[char_code]
            char_x = (x + i) * char_width + centered_x

            for row in range(char_height):
                byte = glyph[row]
                for col in range(char_width):
                    pixel_x = char_x + col
                    pixel_y = y * char_height + row + centered_y

                    if byte & (0x80 >> col):
                        self.draw_pixel(pixel_x, pixel_y, self.fg_color)

    def write_graphics(self, x, y, data):
        """Compatibility method for graphics display"""
        pass

    def write_glyph(self, x, y, glyph_name):
        """Draw a custom glyph at position"""
        if glyph_name not in self.glyphs:
            return 0

        glyph_entry = self.glyphs[glyph_name]
        centered_x = int((240 - 128)/2)
        centered_y = int((240 - 32)/2)

        # Handle icon14x16 format
        if 'icon14x16' in glyph_entry:
            glyph_data = glyph_entry['icon14x16']
            width = 14
            height = 16
            char_x = x * 8 + centered_x

            for row in range(height):
                byte1 = glyph_data[row * 2]
                byte2 = glyph_data[row * 2 + 1]
                if isinstance(byte1, (list, tuple)):
                    byte1 = byte1[0]
                if isinstance(byte2, (list, tuple)):
                    byte2 = byte2[0]
                bits = (byte1 << 8) | byte2

                for col in range(width):
                    pixel_x = char_x + col
                    pixel_y = y * height + row + centered_y
                    color_idx = (self.fg_color if (bits & (0x8000 >> col))
                                 else self.bg_color)
                    self.draw_pixel(pixel_x, pixel_y, color_idx)
            return 2

        # Handle icon16x16 format
        elif 'icon16x16' in glyph_entry:
            glyph_data1 = glyph_entry['icon16x16'][0]
            glyph_data2 = glyph_entry['icon16x16'][1]
            width = 8
            height = 16
            char_x = x * 8 + centered_x

            for row in range(height):
                byte1 = glyph_data1[row]
                byte2 = glyph_data2[row]
                if isinstance(byte1, (list, tuple)):
                    byte1 = byte1[0]
                if isinstance(byte2, (list, tuple)):
                    byte2 = byte2[0]

                for col in range(width):
                    pixel_x = char_x + col
                    pixel_y = y * height + row + centered_y
                    color_idx = (self.fg_color if (byte1 & (0x80 >> col))
                                 else self.bg_color)
                    self.draw_pixel(pixel_x, pixel_y, color_idx)

                    pixel_x = char_x + col + width
                    color_idx = (self.fg_color if (byte2 & (0x80 >> col))
                                 else self.bg_color)
                    self.draw_pixel(pixel_x, pixel_y, color_idx)
            return 2

        return 0

    def set_glyphs(self, glyphs):
        """Store custom glyphs for display"""
        self.glyphs = glyphs

def load_config(config):
    """Klipper config loader - this is the entry point"""
    return GC9A01(config)
