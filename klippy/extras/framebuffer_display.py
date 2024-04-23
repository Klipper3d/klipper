# mSLA and DLP display properties
#
# Copyright (C) 2024  Tiago Conceição <tiago_caza@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
import io
import logging
import math
import re
import os
import mmap

from PIL import Image
import threading


class FramebufferDisplay():
    def __init__(self, config):
        self.printer = config.get_printer()

        self.model = config.get('model', None)
        atypes = {'Mono': 'Mono', 'RGB': 'RGB', 'RGBA': 'RGBA'}
        self.type = config.getchoice('type', atypes, default='Mono')
        self.framebuffer_index = config.getint('framebuffer_index', minval=0)
        if not os.path.exists(self.get_framebuffer_path()):
            msg = f"The frame buffer device {self.get_framebuffer_path()} does not exists."
            logging.exception(msg)
            raise config.error(msg)

        self.resolution_x = config.getint('resolution_x', minval=1)  # In pixels
        self.resolution_y = config.getint('resolution_y', minval=1)  # In pixels
        self.display_width = config.getfloat('display_width', 0., above=0.)  # In millimeters
        self.display_height = config.getfloat('display_height', 0., above=0.)  # In millimeters
        self.pixel_width = config.getfloat('pixel_width', 0., above=0.)  # In millimeters
        self.pixel_height = config.getfloat('pixel_height', 0., above=0.)  # In millimeters

        if self.display_width == 0. and self.pixel_width == 0.:
            msg = "Either display_width or pixel_width must be provided."
            logging.exception(msg)
            raise config.error(msg)

        if self.display_height == 0. and self.pixel_height == 0.:
            msg = "Either display_height or pixel_height must be provided."
            logging.exception(msg)
            raise config.error(msg)

        if self.display_width == 0.:
            self.display_width = round(self.resolution_x * self.pixel_width, 4)
        elif self.pixel_width == 0.:
            self.pixel_width = round(self.display_width / self.resolution_x, 5)

        if self.display_height == 0.:
            self.display_height = round(self.resolution_y * self.pixel_height, 4)
        elif self.pixel_height == 0.:
            self.pixel_height = round(self.display_height / self.resolution_y, 5)

        # Calculated values
        self.bit_depth = 8 if self.type == 'Mono' else 24
        self.is_landscape = self.resolution_x >= self.resolution_y
        self.is_portrait = not self.is_landscape
        self.diagonal_inch = round(math.sqrt(self.display_width ** 2 + self.display_height ** 2) / 25.4, 2)

        # get modes width and height
        with open(f"/sys/class/graphics/fb{self.framebuffer_index}/modes", "r") as f:
            modes = f.read()
            match = re.search(r"(\d+)x(\d+)", modes)
            if match:
                self.fb_modes_width = int(match.group(1))
                self.fb_modes_height = int(match.group(2))
            else:
                msg = f"Unable to extract \"modes\" information from fame buffer device fb{self.framebuffer_index}."
                logging.exception(msg)
                raise config.error(msg)

        # get virtual width and height
        with open(f"/sys/class/graphics/fb{self.framebuffer_index}/virtual_size", "r") as f:
            virtual_size = f.read()
            width_string, height_string = virtual_size.split(',')
            self.fb_virtual_width = int(width_string)  # width
            self.fb_virtual_height = int(height_string)  # height

        # get stride
        with open(f"/sys/class/graphics/fb{self.framebuffer_index}/stride", "r") as f:
            self.fb_stride = int(f.read())

        # get bits per pixel
        with open(f"/sys/class/graphics/fb{self.framebuffer_index}/bits_per_pixel", "r") as f:
            self.fb_bits_per_pixel = int(f.read())

        # Check if configured resolutions match framebuffer information
        if self.resolution_x not in (self.fb_stride, self.fb_modes_width):
            msg = f"The configured resolution_x of {self.resolution_x} does not match any of the framebuffer stride of ({self.fb_stride}) nor the modes of ({self.fb_modes_width})."
            logging.exception(msg)
            raise config.error(msg)
        if self.resolution_y != self.fb_modes_height:
            msg = f"The configured resolution_y of {self.resolution_y} does not match the framebuffer modes of ({self.fb_modes_height})."
            logging.exception(msg)
            raise config.error(msg)

        self.fb_maxsize = self.fb_stride * self.fb_modes_height

        # Defines the current thread that hold the display session to the framebuffer
        self._framebuffer_thread = None
        self.is_busy = False

        self.clear_framebuffer_threaded()

    def get_framebuffer_path(self) -> str:
        """
        Gets the framebuffer device path associated with this object.
        @return: /dev/fb[i]
        """
        return f"/dev/fb{self.framebuffer_index}"

    def wait_framebuffer_thread(self, timeout=None):
        """
        Wait for the framebuffer thread to terminate (Finish writes).
        @param timeout: Timeout time to wait for framebuffer thread to terminate.
        """
        if self._framebuffer_thread is not None and self._framebuffer_thread.is_alive():
            self._framebuffer_thread.join(timeout)  # Wait for other render completion

    def get_image_buffer(self, path) -> bytes:
        """
        Reads an image from disk and return it buffer, ready to send.
        Note that it does not do any file check, it will always try to open an image file.
        @param path: Image path
        @return: bytes buffer
        """
        with Image.open(path) as img:
            return img.tobytes()

    def clear_framebuffer(self):
        """
        Clears the display with zeros (black background).
        """
        self.is_busy = True
        with open(self.get_framebuffer_path(), mode='r+b') as framebuffer_device:  # open R/W
            with mmap.mmap(framebuffer_device.fileno(), self.fb_stride * self.fb_modes_height, mmap.MAP_SHARED,
                           mmap.PROT_WRITE | mmap.PROT_READ) as framebuffer_memory_map:
                color = 0
                framebuffer_memory_map.write(color.to_bytes(1, byteorder='little') * self.fb_maxsize)
        self.is_busy = False

    def clear_framebuffer_threaded(self, wait_thread=False):
        """
        Wait for the framebuffer thread to terminate (Finish writes).
        @param wait_thread: Wait for the framebuffer thread to terminate.
        """
        self.wait_framebuffer_thread()
        self._framebuffer_thread = threading.Thread(target=self.clear_framebuffer,
                                                    name=f"Clears the fb{self.framebuffer_index}")
        self._framebuffer_thread.start()
        if wait_thread:
            self._framebuffer_thread.join()

    def send_buffer(self, buffer, clear_first=False, offset=0):
        """
        Send a byte array to the framebuffer device.
        @param buffer: A 1D byte array with data to send
        @param clear_first: If true first clears the display and then show the image.
        @param offset: Sets a positive offset from start position of the buffer.
        """
        if buffer is None or len(buffer) == 0:
            return

        if not isinstance(buffer, (list, bytes)):
            msg = f"The buffer must be a 1D byte array. {type(buffer)} was passed"
            logging.exception(msg)
            raise Exception(msg)

        if offset < 0:
            msg = f"The offset {offset} can not be negative value."
            logging.exception(msg)
            raise Exception(msg)

        # open framebuffer and map it onto a python bytearray
        self.is_busy = True
        with open(self.get_framebuffer_path(), mode='r+b') as framebuffer_device:  # open R/W
            with mmap.mmap(framebuffer_device.fileno(), self.fb_stride * self.fb_modes_height, mmap.MAP_SHARED,
                           mmap.PROT_WRITE | mmap.PROT_READ) as framebuffer_memory_map:
                if clear_first:
                    color = 0
                    framebuffer_memory_map.write(color.to_bytes(1, byteorder='little') * self.fb_maxsize)

                buffer_size = len(buffer)
                if buffer_size + offset > self.fb_maxsize:
                    msg = f"The buffer size of {buffer_size} + {offset} is larger than actual framebuffer size of {self.fb_maxsize}."
                    logging.exception(msg)
                    raise Exception(msg)

                if offset > 0:
                    framebuffer_memory_map.seek(offset)

                framebuffer_memory_map.write(buffer)
        self.is_busy = False

    def send_buffer_threaded(self, buffer, clear_first=False, offset=0, wait_thread=False):
        """
        Send a byte array to the framebuffer device in a separate thread.
        @param buffer: A 1D byte array with data to send
        @param clear_first: If true first clears the display and then show the image.
        @param offset: Sets a positive offset from start position of the buffer.
        @param wait_thread: If true wait for the framebuffer thread to terminate
        """
        self.wait_framebuffer_thread()
        self._framebuffer_thread = threading.Thread(target=lambda: self.send_buffer(buffer, clear_first, offset),
                                                    name=f"Sends an buffer to fb{self.framebuffer_index}")
        self._framebuffer_thread.start()
        if wait_thread:
            self._framebuffer_thread.join()

    def send_image(self, path, clear_first=False, offset=0):
        """
        Reads an image from a path and sends the bitmap to the framebuffer device.
        @param path: Image file path to be read.
        @param clear_first: If true first clears the display and then show the image.
        @param offset: Sets a positive offset from start position of the buffer.
        """
        if offset < 0:
            msg = f"The offset {offset} can not be negative value."
            logging.exception(msg)
            raise Exception(msg)

        if not isinstance(path, str):
            msg = f"Path must be a string."
            logging.exception(msg)
            raise Exception(msg)

        if not os.path.isfile(path):
            msg = f"The file '{path}' does not exists."
            logging.exception(msg)
            raise Exception(msg)

        self.is_busy = True
        buffer = self.get_image_buffer(path)
        self.send_buffer(buffer, clear_first, offset)

    def send_image_threaded(self, path, clear_first=False, offset=0, wait_thread=False):
        """
        Reads an image from a path and sends the bitmap to the framebuffer device.
        @param path: Image file path to be read.
        @param clear_first: If true first clears the display and then show the image.
        @param offset: Sets a positive offset from start position of the buffer.
        @param wait_thread: If true wait for the framebuffer thread to terminate.
        """
        self.wait_framebuffer_thread()
        self._framebuffer_thread = threading.Thread(target=lambda: self.send_image(path, clear_first, offset),
                                                    name=f"Render an image to fb{self.framebuffer_index}")
        self._framebuffer_thread.start()
        if wait_thread:
            self._framebuffer_thread.join()


class FramebufferDisplayWrapper(FramebufferDisplay):
    def __init__(self, config):
        super().__init__(config)

        device_name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("FRAMEBUFFER_CLEAR", "DEVICE", device_name, self.cmd_FRAMEBUFFER_CLEAR,
                                   desc=self.cmd_FRAMEBUFFER_CLEAR_help)
        gcode.register_mux_command("FRAMEBUFFER_SEND_IMAGE", "DEVICE", device_name, self.cmd_FRAMEBUFFER_SEND_IMAGE,
                                   desc=self.cmd_FRAMEBUFFER_SEND_IMAGE_help)


    cmd_FRAMEBUFFER_CLEAR_help = "Clears the display with zeros (black background)"
    def cmd_FRAMEBUFFER_CLEAR(self, gcmd):
        wait_thread = gcmd.get_int('WAIT', 0, 0, 1)

        self.clear_framebuffer_threaded(wait_thread)

    cmd_FRAMEBUFFER_SEND_IMAGE_help = "Send a image from a path and display it on the framebuffer."
    def cmd_FRAMEBUFFER_SEND_IMAGE(self, gcmd):
        path = gcmd.get('PATH')
        offset = gcmd.get_int('OFFSET', 0, 0)
        clear_first = gcmd.get_int('CLEAR', 0, 0, 1)
        wait_thread = gcmd.get_int('WAIT', 0, 0, 1)

        if not os.path.isfile(path):
            raise gcmd.error(f"The file '{path}' does not exists.")

        self.send_image_threaded(path, clear_first, offset, wait_thread)


def load_config_prefix(config):
    return FramebufferDisplayWrapper(config)
