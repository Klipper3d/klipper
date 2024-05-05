# Write to framebuffer devices (HDMI and DSI) display
#
# Copyright (C) 2024  Tiago Conceicao <tiago_caza@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
import logging
import math
import re
import os
import mmap
import struct

from PIL import Image
import threading

CLEAR_NO_FLAG = 0
CLEAR_YES_FLAG = 1
CLEAR_AUTO_FLAG = 2

class FramebufferDisplay:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')

        self.model = config.get('model', None)
        atypes = {'Mono': 'Mono',
                  'RGB': 'RGB',
                  'BGR': 'BGR',
                  }
        self.pixel_format = config.getchoice('pixel_format', atypes,
                                             default='RGB')
        self.framebuffer_index = config.getint('framebuffer_index', minval=0)
        if not os.path.exists(self.get_framebuffer_path()):
            msg = ("The frame buffer device %s does not exists."
                   % self.get_framebuffer_path())
            logging.exception(msg)
            raise config.error(msg)

        self.resolution_x = config.getint('resolution_x', minval=1)  # In pixels
        self.resolution_y = config.getint('resolution_y', minval=1)  # In pixels
        self.pixel_width = config.getfloat('pixel_width', 0.,
                                           above=0.)  # In millimeters
        self.pixel_height = config.getfloat('pixel_height', 0.,
                                            above=0.)  # In millimeters

        self.display_width = round(self.resolution_x * self.pixel_width, 4)
        self.display_height = round(self.resolution_y * self.pixel_height, 4)

        if self.pixel_format == 'Mono':
            self.channels = 1
            self.bit_depth = 8
        if self.pixel_format == 'RGB' or self.pixel_format == 'BGR':
            self.channels = 3
            self.bit_depth = 24
        else:
            self.channels = len(self.pixel_format)
            self.bit_depth = self.channels * 8

        self.is_landscape = self.resolution_x >= self.resolution_y
        self.is_portrait = not self.is_landscape
        self.diagonal_inch = round(math.sqrt(
            self.display_width ** 2 + self.display_height ** 2) / 25.4, 2)

        # get modes width and height
        with open("/sys/class/graphics/fb%d/modes" % self.framebuffer_index,
                  "r") as f:
            modes = f.read()
            match = re.search(r"(\d+)x(\d+)", modes)
            if match:
                self.fb_modes_width = int(match.group(1))
                self.fb_modes_height = int(match.group(2))
            else:
                msg = ('Unable to extract "modes" information from famebuffer '
                       "device fb%d." % self.framebuffer_index)
                logging.exception(msg)
                raise config.error(msg)

        # get virtual width and height
        with open(
                "/sys/class/graphics/fb%d/virtual_size"
                % self.framebuffer_index, "r") as f:
            virtual_size = f.read()
            width_string, height_string = virtual_size.split(',')
            self.fb_virtual_width = int(width_string)  # width
            self.fb_virtual_height = int(height_string)  # height

        # get stride
        with open("/sys/class/graphics/fb%d/stride" % self.framebuffer_index,
                  "r") as f:
            self.fb_stride = int(f.read())

        # get bits per pixel
        with open(
                "/sys/class/graphics/fb%d/bits_per_pixel"
                % self.framebuffer_index, "r") as f:
            self.fb_bits_per_pixel = int(f.read())

        # Check if configured resolutions match framebuffer information
        if self.resolution_x not in (self.fb_stride, self.fb_modes_width):
            msg = ("The configured resolution_x of %d does not match any of "
                   "the framebuffer stride of (%d) nor the modes of (%d)."
                   % (self.resolution_x, self.fb_stride, self.fb_modes_width))
            logging.exception(msg)
            raise config.error(msg)
        if self.resolution_y != self.fb_modes_height:
            msg = ("The configured resolution_y of %d does not match the "
                   "framebuffer modes of (%d)."
                   % (self.resolution_y, self.fb_modes_height))
            logging.exception(msg)
            raise config.error(msg)

        self.fb_maxsize = self.fb_stride * self.fb_modes_height

        # Defines the current thread that hold the display session to the fb
        self._framebuffer_thread = None
        self._framebuffer_lock = threading.Lock()

        # Keeps device open to spare transfer times
        self._fb_device = open(self.get_framebuffer_path(), mode='r+b')
        self.fb_memory_map = mmap.mmap(self._fb_device.fileno(),
                                       self.fb_stride * self.fb_modes_height,
                                       mmap.MAP_SHARED,
                                       mmap.PROT_WRITE | mmap.PROT_READ)

        self.clear_buffer_threaded()

    def is_busy(self):
        """
        Checks if the device is busy.
        @return: True if the device is busy, false otherwise.
        """
        return (self._framebuffer_lock.locked() or
                (self._framebuffer_thread is not None
                and self._framebuffer_thread.is_alive()))

    def wait_framebuffer_thread(self, timeout=None):
        """
        Wait for the framebuffer thread to terminate (Finish writes).
        @param timeout: Timeout time to wait for framebuffer thread to terminate
        @return: True if waited for framebuffer thread, otherwise False.
        """
        if (self._framebuffer_thread is not None
                and self._framebuffer_thread.is_alive()):
            # Wait for other render completion
            self._framebuffer_thread.join(timeout)
            return True
        return False

    def get_framebuffer_path(self):
        """
        Gets the framebuffer device path associated with this object.
        @return: /dev/fb[i]
        """
        return "/dev/fb%d" % self.framebuffer_index

    def get_max_offset(self, buffer_size, stride, height=-1):
        """
        Gets the maximum possible offset of the framebuffer device given a
        buffer and it stride.
        @param buffer_size: The total buffer size
        @param stride: The buffer stride, how many bytes per row
        @param height: The buffer height
        @return:
        """
        if height < 0:
            height = buffer_size // stride
        return max(self.fb_maxsize - self.fb_stride * (height - 1) - stride, 0)

    def get_position_from_xy(self, x, y):
        """
        Gets the starting point in the buffer given an x and y coordinate
        @param x: X coordinate
        @param y: Y coordinate
        @return: Buffer position
        """
        if x < 0:
            msg = "The x value can not be negative."
            logging.exception(msg)
            raise self.gcode.error(msg)

        if x >= self.resolution_x:
            msg = "The x value must be less than %d" % self.resolution_x
            logging.exception(msg)
            raise self.gcode.error(msg)

        if y < 0:
            msg = "The y value can not be negative."
            logging.exception(msg)
            raise self.gcode.error(msg)

        if y >= self.resolution_y:
            msg = "The y value must be less than %d" % self.resolution_y
            logging.exception(msg)
            raise self.gcode.error(msg)

        return y * self.fb_stride + x * self.channels

    def get_image_buffer(self, path, strip_alpha=True):
        """
        Reads an image from disk and return it buffer, ready to send to fb.
        Note that it does not do any file check,
        it will always try to open an image file.
        @param path: Image path
        @param strip_alpha: Strip the alpha channel of the image if present
        @return: dict with image data
        """
        with Image.open(path) as img:
            depth = 3
            if img.mode in ("L", "P"):
                depth = 1
            elif img.mode in ("RGBA", "CMYK"):
                depth = 4

            if img.mode == "RGBA" and strip_alpha:
                depth = 3
                # Strip alpha
                with Image.new('RGB', img.size) as img2:
                    img2.paste(img, (0, 0), img)
                    buffer = img2.tobytes()
                    width = img.width
                    height = img.height
            else:
                buffer = img.tobytes()
                width = img.width
                height = img.height

            buffer_size = len(buffer)
            #logging.exception(f"buffer: {buffer_size}, "
            #                  f"stride: {width * depth}, "
            #                  f"width: {width}, "
            #                  f"height: {height}, "
            #                  f"depth: {depth}, "
            #                  f"bpp: {depth * 8}")
            return {'buffer': buffer,
                    'stride': img.width * depth,
                    'width': width,
                    'height': height,
                    'depth': depth,
                    'bpp': depth * 8,
                    'size': buffer_size}

    def clear_buffer(self):
        """
        Clears the display with zeros (black background).
        """
        self.fill_buffer(0)

    def clear_buffer_threaded(self, wait_thread=False):
        """
        Clears the display with zeros (black background).
        @param wait_thread: Wait for the framebuffer thread to terminate.
        """
        self.wait_framebuffer_thread()
        self._framebuffer_thread = threading.Thread(
            target=self.clear_buffer,
            name="Clears the fb%d" % self.framebuffer_index)
        self._framebuffer_thread.start()
        if wait_thread:
            self._framebuffer_thread.join()

    def write_buffer(self, buffer, buffer_stride=0, offset=0,
                     clear_flag=CLEAR_NO_FLAG):
        """
        Write a byte array into the framebuffer device.
        @param buffer: A 1D byte array with data to send
        @param offset: Sets a positive offset from start position of the buffer.
        @param buffer_stride: The stride/row of the buffer, set if buffer is a
                              cropped region.
        @param clear_flag: Clear the remaining buffer, 0=No, 1=Yes, 2=Auto
        """
        if buffer is None or len(buffer) == 0:
            return

        if not isinstance(buffer, (list, bytes)):
            msg = ("The buffer must be a 1D byte array. %s was passed"
                   % type(buffer))
            logging.exception(msg)
            raise self.gcode.error(msg)

        if offset < 0:
            msg = "The offset %d can not be negative value." % offset
            logging.exception(msg)
            raise self.gcode.error(msg)

        if clear_flag < 0 or clear_flag > CLEAR_AUTO_FLAG:
            msg = "The clear flag must be between 0 and %d." % CLEAR_AUTO_FLAG
            raise self.gcode.error(msg)

        clear = clear_flag

        buffer_size = len(buffer)
        if buffer_size + offset > self.fb_maxsize:
            msg = ("The buffer size of %d + %d is greater "
                   "than actual framebuffer size of %d."
                   % (buffer_size, offset, self.fb_maxsize))
            logging.exception(msg)
            raise self.gcode.error(msg)

        # Auto clear, clears if buffer is smaller than framebuffer device
        if clear_flag == CLEAR_AUTO_FLAG and buffer_size != self.fb_maxsize:
            clear = CLEAR_YES_FLAG

        is_region = False
        if buffer_stride > 0 and buffer_stride < self.fb_stride:
            is_region = True
            if buffer_size % buffer_stride != 0:
                msg = ("The buffer stride of %d must be an exact"
                       " multiple of the buffer size %d."
                       % (buffer_stride, buffer_size))
                logging.exception(msg)
                raise self.gcode.error(msg)

            max_offset = self.get_max_offset(buffer_size, buffer_stride)
            if offset > max_offset:
                msg = ("The offset of %d can not be greater than %d in order "
                       "to fit the a buffer of %d." %
                       (offset, max_offset, buffer_size))
                logging.exception(msg)
                raise self.gcode.error(msg)

        with (self._framebuffer_lock):
            if offset > 0:
                if clear:
                    self.fb_memory_map.write(
                        struct.pack('<B', 0) * offset)
                else:
                    self.fb_memory_map.seek(offset)

            if is_region:
                stride_offset = self.fb_stride - buffer_stride
                if clear:
                    stride_cleaner = struct.pack('<B', 0) * stride_offset
                for i in range(0, buffer_size, buffer_stride):
                    if i > 0:
                        if clear:
                            self.fb_memory_map.write(stride_cleaner)
                        else:
                            self.fb_memory_map.seek(stride_offset, os.SEEK_CUR)
                    self.fb_memory_map.write(buffer[i:i + buffer_stride])
            else:
                self.fb_memory_map.write(buffer)

            pos = self.fb_memory_map.tell()
            if clear and pos < self.fb_maxsize:
                self.fb_memory_map.write(
                    struct.pack('<B', 0) * (self.fb_maxsize - pos))

            self.fb_memory_map.seek(0)

    def write_buffer_threaded(self, buffer, buffer_stride=0, offset=0,
                              clear_flag=CLEAR_NO_FLAG, wait_thread=False):
        """
        Write a byte array into the framebuffer device in a separate thread.
        @param buffer: A 1D byte array with data to send
        @param buffer_stride: The stride/row of the buffer, set if buffer is a
                              cropped region.
        @param offset: Sets a positive offset from start position of the buffer.
        @param clear_flag: Clear the remaining buffer, 0=No, 1=Yes, 2=Auto
        @param wait_thread: If true wait for the framebuffer thread to terminate

        """
        self.wait_framebuffer_thread()
        self._framebuffer_thread = threading.Thread(
            target=lambda: self.write_buffer(buffer, buffer_stride, offset,
                                             clear_flag),
            name="Sends an buffer to fb%d" % self.framebuffer_index)
        self._framebuffer_thread.start()
        if wait_thread:
            self._framebuffer_thread.join()

    def fill_buffer(self, color):
        """
        Fills the framebuffer with the given color.
        @param color: From 0 to 255 or a tuple/list of RGB
        """
        if isinstance(color, int):
            if color < 0 or color > 255:
                msg = "The color must be an int number from 0 to 255."
                logging.exception(msg)
                raise self.gcode.error(msg)

            buffer = struct.pack('<B', color)
        elif isinstance(color, (tuple, list)):
            for x in color:
                if x < 0 or x > 255:
                    msg = "The color must be an int number from 0 to 255."
                    logging.exception(msg)
                    raise self.gcode.error(msg)
            buffer = bytes(color)
        else:
            msg = ("The color must be an int number from 0 to 255 "
                   "or a tuple/list of RGB.")
            logging.exception(msg)
            raise self.gcode.error(msg)

        buffer_size = len(buffer)
        if self.fb_maxsize % buffer_size != 0:
            msg = (
                "The buffer size of %d must be an exact"
                " multiple of the framebuffer size %d."
                % (buffer_size, self.fb_maxsize))
            logging.exception(msg)
            raise self.gcode.error(msg)
        self.write_buffer(buffer * (self.fb_maxsize // buffer_size))

    def fill_buffer_threaded(self, color, wait_thread=False):
        """
        Fills the framebuffer with the given color.
        @param color: From 0 to 255 or a tuple/list of RGB
        @param wait_thread: Wait for the framebuffer thread to terminate.
        """
        self.wait_framebuffer_thread()
        self._framebuffer_thread = threading.Thread(
            target=self.fill_buffer, args=(color,),
            name="Fill the fb%d with %d" % (self.framebuffer_index, color))
        self._framebuffer_thread.start()
        if wait_thread:
            self._framebuffer_thread.join()

    def draw_image(self, path, offset=0, clear_flag=CLEAR_NO_FLAG):
        """
        Reads an image from a path and draw the bitmap to the fb device.
        @param path: Image file path to be read.
        @param offset: Sets a positive offset from start position of the buffer.
        @param clear_flag: Clear the remaining buffer, 0=No, 1=Yes, 2=Auto
        """
        gcode = self.printer.lookup_object('gcode')
        if offset < 0:
            msg = "The offset %d can not be negative value." % offset
            logging.exception(msg)
            raise gcode.error(msg)

        if not isinstance(path, str):
            msg = "Path must be a string."
            logging.exception(msg)
            raise gcode.error(msg)

        if not os.path.isfile(path):
            msg = "The file '%s' does not exists." % path
            logging.exception(msg)
            raise gcode.error(msg)

        di = self.get_image_buffer(path)
        self.write_buffer(di['buffer'], di['stride'], offset, clear_flag)

    def draw_image_threaded(self, path, offset=0, clear_flag=CLEAR_NO_FLAG,
                            wait_thread=False):
        """
        Reads an image from a path and draw the bitmap to the fb device.
        @param path: Image file path to be read.
        @param offset: Sets a positive offset from start position of the buffer.
        @param clear_flag: Clear the remaining buffer, 0=No, 1=Yes, 2=Auto
        @param wait_thread: If true wait for the framebuffer thread to terminate
        """
        self.wait_framebuffer_thread()
        self._framebuffer_thread = threading.Thread(
            target=lambda: self.draw_image(path, offset, clear_flag),
            name="Render an image to fb%d" % self.framebuffer_index)
        self._framebuffer_thread.start()
        if wait_thread:
            self._framebuffer_thread.join()


class FramebufferDisplayWrapper(FramebufferDisplay):
    def __init__(self, config):
        super(FramebufferDisplayWrapper, self).__init__(config)

        device_name = config.get_name().split()[1]
        self.gcode.register_mux_command("FRAMEBUFFER_CLEAR", "DEVICE",
                                        device_name, self.cmd_FRAMEBUFFER_CLEAR,
                                        desc=self.cmd_FRAMEBUFFER_CLEAR_help)
        self.gcode.register_mux_command("FRAMEBUFFER_SEND_IMAGE", "DEVICE",
                                   device_name, self.cmd_FRAMEBUFFER_DRAW_IMAGE,
                                   desc=self.cmd_FRAMEBUFFER_DRAW_IMAGE_help)

    def get_status(self, eventtime):
        return {'is_busy': self.is_busy()}

    cmd_FRAMEBUFFER_CLEAR_help = ("Clears the display with zeros "
                                  "(black background)")

    def cmd_FRAMEBUFFER_CLEAR(self, gcmd):
        wait_thread = gcmd.get_int('WAIT', 0, 0, 1)

        self.clear_buffer_threaded(wait_thread)

    cmd_FRAMEBUFFER_DRAW_IMAGE_help = ("Reads a image from a path "
                                       "and render it on the framebuffer.")

    def cmd_FRAMEBUFFER_DRAW_IMAGE(self, gcmd):
        path = gcmd.get('PATH')
        offset = gcmd.get_int('OFFSET', 0, 0)
        clear_flag = gcmd.get_int('CLEAR', CLEAR_AUTO_FLAG, 0, CLEAR_AUTO_FLAG)
        wait_thread = gcmd.get_int('WAIT', 0, 0, 1)

        if not os.path.isfile(path):
            raise gcmd.error("The file '%s' does not exists." % path)

        self.draw_image_threaded(path, offset, clear_flag, wait_thread)


def load_config_prefix(config):
    return FramebufferDisplayWrapper(config)
