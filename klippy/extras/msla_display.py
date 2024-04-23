# mSLA and DLP display properties
#
# Copyright (C) 2024  Tiago Conceição <tiago_caza@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#

import logging
import os
import re
import threading
import time
import tempfile
from pathlib import Path
from PIL import Image
from . import framebuffer_display

CACHE_MIN_FREE_RAM = 256  # 256 MB


class mSLADisplay(framebuffer_display.FramebufferDisplay):
    """
    Represents an mSLA display with an associated UV LED
    """

    def __init__(self, config):
        super().__init__(config)
        self.buffer_cache_count = config.getint('cache', 0, 0, 100)

        # RAM guard
        if self.buffer_cache_count > 0 and os.path.isfile('/proc/meminfo'):
            with open('/proc/meminfo', 'r') as mem:
                for i in mem:
                    sline = i.split()
                    if str(sline[0]) == 'MemFree:':
                        free_memory_mb = int(sline[1]) / 1024.
                        buffer_max_size = (self.buffer_cache_count + 1) * self.fb_maxsize / 1024. / 1024.
                        if free_memory_mb - buffer_max_size < CACHE_MIN_FREE_RAM:
                            msg = ("The current cache count of %d requires %.2f MB of RAM, currently have %.2f MB of "
                                   "free memory. It requires at least a margin of %.2f MB of free RAM.\n"
                                   "Please reduce the cache count.") % (
                                      self.buffer_cache_count, buffer_max_size, free_memory_mb, CACHE_MIN_FREE_RAM)
                            logging.exception(msg)
                            raise config.error(msg)
                        break

        self._cache = []
        self._cache_thread = None

        self.uvled_output_pin_name = config.get('uvled_output_pin_name', 'msla_uvled')
        self.uvled = self.printer.lookup_object(f"output_pin {self.uvled_output_pin_name}")

        self._sd = self.printer.lookup_object("virtual_sdcard")

        # Events
        self.printer.register_event_handler("virtual_sdcard:reset_file", self.clear_cache)

        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("MSLA_DISPLAY_VALIDATE", self.cmd_MSLA_DISPLAY_VALIDATE,
                               desc=self.cmd_MSLA_DISPLAY_VALIDATE_help)
        gcode.register_command("MSLA_DISPLAY_RESPONSE_TIME", self.cmd_MSLA_DISPLAY_RESPONSE_TIME,
                               desc=self.cmd_MSLA_DISPLAY_RESPONSE_TIME_help)

        #gcode.register_command("MSLA_DISPLAY_CLEAR", self.cmd_MSLA_DISPLAY_CLEAR,
        #                       desc=self.cmd_MSLA_DISPLAY_CLEAR_help)
        #gcode.register_command("MSLA_DISPLAY_IMAGE", self.cmd_MSLA_DISPLAY_IMAGE,
        #                       desc=self.cmd_MSLA_DISPLAY_IMAGE_help)

        gcode.register_command('M6054', self.cmd_M6054, desc=self.cmd_M6054_help)
        gcode.register_command('M1400', self.cmd_M1400, desc=self.cmd_M1400_help)

    def get_status(self, eventtime):
        return {'display_busy': self.is_busy,
                'uvled_value': self.uvled.last_value * 255.}

    def clear_cache(self):
        """
        Clear all cached buffers
        """
        self._cache.clear()

    def _can_cache(self) -> bool:
        """
        Returns true if is ready to cache
        @rtype: bool
        """
        return self.buffer_cache_count > 0 and (self._cache_thread is None or not self._cache_thread.is_alive())

    def _wait_cache_thread(self) -> bool:
        """
        Waits for cache thread completion
        @rtype: bool
        @return: True if waited, otherwise false
        """
        if self._cache_thread is not None and self._cache_thread.is_alive():
            self._cache_thread.join()
            return True
        return False

    def _process_cache(self, last_cache):
        """
        Cache next items from last cache position.
        @param last_cache:
        """
        # Dequeue items
        while len(self._cache) >= self.buffer_cache_count:
            self._cache.pop(0)

        cache_len = len(self._cache)
        if cache_len:
            last_cache = self._cache[-1]

        # Cache new items
        item_count = self.buffer_cache_count - cache_len
        layer_index = last_cache.get_layer_index()
        for i in range(item_count):
            layer_index += 1
            new_path = last_cache.new_path_layerindex(layer_index)
            if os.path.isfile(new_path):
                buffer = self.get_image_buffer(new_path)
                self._cache.append(BufferCache(new_path, buffer))

    def _get_cache_index(self, path) -> int:
        """
        Gets the index of the cache position on the list given a path
        @param path: Path to seek
        @return: Index of the cache on the list, otherwise -1
        """
        for i, cache in enumerate(self._cache):
            if cache.path == path:
                return i
        return -1

    def _get_image_buffercache(self, gcmd, path) -> bytes:
        """
        Gets a image buffer from cache if available, otherwise it creates the buffer from the path image
        @param gcmd:
        @param path: Image path
        @return: Image buffer
        """
        # If currently printing, use print directory to select the file
        current_filepath = self._sd.file_path()
        if current_filepath is not None and path[0] != '/':
            path = os.path.join(os.path.dirname(current_filepath), path)
            if not os.path.isfile(path):
                raise gcmd.error(f"M6054: The file '{path}' does not exists.")

            if self.buffer_cache_count > 0:
                self._wait_cache_thread()  # May be worth to wait if streaming images
                i = self._get_cache_index(path)
                #gcmd.respond_raw(f"File in cache: {i}, cache size: {len(self._cache)}, Can cache: {self._can_cache()}")
                if i >= 0:
                    cache = self._cache[i]
                    # RTrim up to cache
                    self._cache = self._cache[i + 1:]

                    if self._can_cache():
                        self._cache_thread = threading.Thread(target=self._process_cache, args=(cache,))
                        self._cache_thread.start()

                    return cache.buffer

        if not os.path.isfile(path):
            raise gcmd.error(f"M6054: The file '{path}' does not exists.")

        buffer = self.get_image_buffer(path)
        if self._can_cache():
            cache = BufferCache(path, buffer)
            self._cache_thread = threading.Thread(target=self._process_cache, args=(cache,))
            self._cache_thread.start()

        return buffer

    cmd_MSLA_DISPLAY_VALIDATE_help = "Validate the display against resolution and pixel parameters. Throw error if out of parameters."

    def cmd_MSLA_DISPLAY_VALIDATE(self, gcmd):
        """
        Layer images are universal within same resolution and pixel pitch. Other printers with same resolution and pixel
        pitch can print same file. This command ensure print only continue if requirements are meet.

        Syntax: MSLA_DISPLAY_VALIDATE RESOLUTION=<x,y> PIXEL=<x,y>

        RESOLUTION: Machine LCD resolution
        PIXEL: Machine LCD pixel pitch
        @param gcmd:
        @return:
        """
        resolution = gcmd.get('RESOLUTION')
        pixel_size = gcmd.get('PIXEL')

        resolution_split = resolution.split(',', 1)
        if len(resolution_split) < 2:
            raise gcmd.error(f"The resolution of {resolution} is malformed, format: RESOLUTION_X,RESOLUTION_Y.")

        try:
            resolution_x = int(resolution_split[0])
        except:
            raise gcmd.error(f"The resolution x must be an integer number.")

        try:
            resolution_y = int(resolution_split[1])
        except:
            raise gcmd.error(f"The resolution y must be an integer.")

        if resolution_x != self.resolution_x:
            raise gcmd.error(f"The resolution X of {resolution_x} is invalid. Should be {self.resolution_x}.")

        if resolution_y != self.resolution_y:
            raise gcmd.error(f"The resolution Y of {resolution_y} is invalid. Should be {self.resolution_y}.")

        pixel_size_split = pixel_size.split(',', 1)
        if len(pixel_size_split) < 2:
            raise gcmd.error(f"The pixel size of {pixel_size} is malformed, format: PIXEL_WIDTH,PIXEL_HEIGHT.")

        try:
            pixel_width = float(pixel_size_split[0])
        except:
            raise gcmd.error(f"The pixel width must be an floating point number.")

        try:
            pixel_height = float(pixel_size_split[1])
        except:
            raise gcmd.error(f"The pixel height must be an floating point number.")

        if pixel_width != self.pixel_width:
            raise gcmd.error(f"The pixel width of {pixel_width} is invalid. Should be {self.pixel_width}.")

        if pixel_height != self.pixel_height:
            raise gcmd.error(f"The pixel height of {pixel_height} is invalid. Should be {self.pixel_height}.")

    cmd_MSLA_DISPLAY_RESPONSE_TIME_help = "Send a buffer to display and test it response time to fill that buffer."

    def cmd_MSLA_DISPLAY_RESPONSE_TIME(self, gcmd):
        """
        Tests the display response time

        Syntax: MSLA_DISPLAY_RESPONSE_TIME AVG=[x]

        AVG: Number of samples to average (int)
        @param gcmd:
        @return:
        """
        avg = gcmd.get_int('AVG', 1, 1, 20)

        gcmd.respond_raw(
            'Buffer size: %d bytes (%.2f MB)' % (self.fb_maxsize, round(self.fb_maxsize / 1024.0 / 1024.0, 2)))

        time_sum = 0
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, 'buffer.png')
            with Image.new('L', (self.resolution_x, self.resolution_y), color='black') as image:
                image.save(path, "PNG")
            for _ in range(avg):
                timems = time.time()
                self.send_image(path)
                time_sum += time.time() - timems
        gcmd.respond_raw('Read image from disk + send time: %fms (%d samples)' % (round(time_sum * 1000 / avg, 6), avg))

        time_sum = 0
        buffer = bytes(os.urandom(self.fb_maxsize))
        for _ in range(avg):
            timems = time.time()
            self.send_buffer(buffer)
            time_sum += time.time() - timems
        gcmd.respond_raw('Send cached buffer: %fms (%d samples)' % (round(time_sum * 1000 / avg, 6), avg))

        time_sum = 0
        for _ in range(avg):
            timems = time.time()
            self.clear_framebuffer()
            time_sum += time.time() - timems
        gcmd.respond_raw('Clear time: %fms (%d samples)' % (round(time_sum * 1000 / avg, 6), avg))

    # Merged both into M6054
    #    cmd_MSLA_DISPLAY_CLEAR_help = "Clears the display by send a full buffer of zeros."
    #
    #    def cmd_MSLA_DISPLAY_CLEAR(self, gcmd):
    #        wait_thread = gcmd.get_int('WAIT', 0, 0, 1)
    #        self.clear_framebuffer_threaded(wait_thread)
    #
    #    cmd_MSLA_DISPLAY_IMAGE_help = "Sends a image from a path and display it on the main display."
    #
    #    def cmd_MSLA_DISPLAY_IMAGE(self, gcmd):
    #        path = gcmd.get('PATH')
    #        clear_first = gcmd.get_int('CLEAR', 0, 0, 1)
    #        offset = gcmd.get_int('OFFSET', 0, 0)
    #        wait_thread = gcmd.get_int('WAIT', 0, 0, 1)
    #        path = os.path.normpath(os.path.expanduser(path))
    #
    #        #times = time.time()
    #        buffer = self._get_image_buffercache(gcmd, path)
    #        self.send_buffer_threaded(buffer, clear_first, offset, wait_thread)
    #        #gcmd.respond_raw("Get/cache buffer: %.2f ms" % ((time.time() - times) * 1000.,))

    cmd_M6054_help = "Sends a image from a path and display it on the main display. If no parameter it will clear the display"

    def cmd_M6054(self, gcmd):
        """
        Display an image from a path and display it on the main display. If no parameter it will clear the display

        Syntax: M6054 F<"file.png"> C[0/1] O[x] W[0/1]
        F: File path eg: "1.png". quotes are optional. (str)
        C: Clear the image before display the new image. (int)
        W: Wait for buffer to complete the transfer. (int)
        @param gcmd:
        @return:
        """
        clear_first = gcmd.get_int('C', 0, 0, 1)
        offset = gcmd.get_int('O', 0, 0)
        wait_thread = gcmd.get_int('W', 0, 0, 1)

        params = gcmd.get_raw_command_parameters().strip()

        match = re.search(r'F?(.+[.](png|jpg|jpeg|bmp|gif))', params)
        if match:
            path = match.group(1).strip(' "\'')
            path = os.path.normpath(os.path.expanduser(path))
            buffer = self._get_image_buffercache(gcmd, path)
            self.send_buffer_threaded(buffer, clear_first, offset, wait_thread)
        else:
            if 'F' in params:
                raise gcmd.error(f"M6054: The F parameter is malformed. Use a proper image file.")
            self.clear_framebuffer_threaded(wait_thread)

    cmd_M1400_help = "Turn the main UV LED to cure the pixels."

    def cmd_M1400(self, gcmd):
        """
        Turn the main UV LED to cure the pixels.
        M1400 comes from the wavelength of UV radiation (UVR) lies in the range of 100–400 nm,
        and is further subdivided into UVA (315–400 nm), UVB (280–315 nm), and UVC (100–280 nm).

        Syntax: M1400 S<0-255> P[milliseconds]
        S: LED Power (Non PWM LEDs will turn on from 1 to 255). (int)
        P: Time to wait in milliseconds when (S>0) to turn off. (int)
        @param gcmd:
        """
        value = gcmd.get_float('S', minval=0., maxval=255.)
        delay = gcmd.get_float('P', 0, above=0.) / 1000.

        if self.uvled.is_pwm:
            value /= 255.0
        else:
            value = 1 if value else 0

        if value > 0:
            # Sync display
            self.wait_framebuffer_thread()

        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(lambda print_time: self.uvled._set_pin(print_time, value))

        if value > 0 and delay > 0:
            toolhead.dwell(delay)
            toolhead.register_lookahead_callback(lambda print_time: self.uvled._set_pin(print_time, 0))


class BufferCache():
    def __init__(self, path, buffer):
        self.path = path
        self.buffer = buffer
        self.pathinfo = Path(path)

    def new_path_layerindex(self, n):
        return re.sub(rf"\d+\{self.pathinfo.suffix}$", f"{n}{self.pathinfo.suffix}", self.path)

    def get_layer_index(self):
        match = re.search(rf"(\d+)\{self.pathinfo.suffix}$", self.path)
        if match is None:
            return None

        return int(match.group(1))


def load_config(config):
    return mSLADisplay(config)
