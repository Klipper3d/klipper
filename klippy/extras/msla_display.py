# mSLA and DLP display properties
#
# Copyright (C) 2024  Tiago Conceicao <tiago_caza@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#

import logging
import os
import threading
import re
import time
import tempfile
from PIL import Image
from . import framebuffer_display, pwm_tool, pwm_cycle_time, output_pin

CACHE_MIN_RAM = 256  # 256 MB


class mSLADisplay(framebuffer_display.FramebufferDisplay):
    """
    Represents an mSLA display with an associated UV LED
    """

    def __init__(self, config):
        super(mSLADisplay, self).__init__(config)

        # CACHE CONFIG
        self.buffer_cache_count = config.getint('cache', 0, 0, 100)

        # RAM guard
        if self.buffer_cache_count > 0 and os.path.isfile('/proc/meminfo'):
            with open('/proc/meminfo', 'r') as mem:
                for i in mem:
                    sline = i.split()
                    if str(sline[0]) == 'MemFree:':
                        free_memory_mb = int(sline[1]) / 1024.
                        buffer_max_size = (self.buffer_cache_count + 1
                                           ) * self.fb_maxsize / 1024. / 1024.
                        if free_memory_mb - buffer_max_size < CACHE_MIN_RAM:
                            msg = ("The current cache count of %d requires %.2f"
                                   " MB of RAM, currently have %.2f MB of free "
                                   "memory. It requires at least a margin of "
                                   "%.2f MB of free RAM.\n"
                                   "Please reduce the cache count.") % (
                                      self.buffer_cache_count, buffer_max_size,
                                      free_memory_mb, CACHE_MIN_RAM)
                            logging.exception(msg)
                            raise config.error(msg)
                        break

        self._cache = []
        self._cache_thread = None

        # UVLED CONFIG
        self.uvled_output_pin_name = config.get('uvled_output_pin_name',
                                                'msla_uvled')
        self.uvled_response_delay = config.getfloat('uvled_response_delay', 0,
                                                    minval=0., maxval=1000.)

        self.uvled = self.printer.lookup_object("output_pin %s"
                                                % self.uvled_output_pin_name,
                                                None)
        if self.uvled is None:
            self.uvled = self.printer.lookup_object("pwm_tool %s" %
                                                    self.uvled_output_pin_name,
                                                    None)
        if self.uvled is None:
            msg = "The [output_pin %s] or [pwm_tool %s] was not found." % (
                    self.uvled_output_pin_name, self.uvled_output_pin_name)
            logging.exception(msg)
            raise config.error(msg)

        # Variables
        self._sd = self.printer.lookup_object("virtual_sdcard")
        self._M1451_file_regex = re.compile(r'F(.+[.](png|jpg|jpeg|bmp|gif))')

        # Events
        self.printer.register_event_handler("virtual_sdcard:reset_file",
                                            self.clear_cache)

        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("MSLA_DISPLAY_VALIDATE",
                               self.cmd_MSLA_DISPLAY_VALIDATE,
                               desc=self.cmd_MSLA_DISPLAY_VALIDATE_help)
        self.gcode.register_command("MSLA_DISPLAY_TEST",
                               self.cmd_MSLA_DISPLAY_TEST,
                               desc=self.cmd_MSLA_DISPLAY_TEST_help)
        self.gcode.register_command("MSLA_DISPLAY_RESPONSE_TIME",
                               self.cmd_MSLA_DISPLAY_RESPONSE_TIME,
                               desc=self.cmd_MSLA_DISPLAY_RESPONSE_TIME_help)
        self.gcode.register_command('M1450', self.cmd_M1450,  # Display clear
                               desc=self.cmd_M1451_help)
        self.gcode.register_command('M1451', self.cmd_M1451,  # Display image
                               desc=self.cmd_M1451_help)

        self.gcode.register_command("MSLA_UVLED_RESPONSE_TIME",
                               self.cmd_MSLA_UVLED_RESPONSE_TIME,
                               desc=self.cmd_MSLA_UVLED_RESPONSE_TIME_help)
        self.gcode.register_command('M1400', self.cmd_M1400,  # UVLED SET
                               desc=self.cmd_M1400_help)
        self.gcode.register_command('M1401', self.cmd_M1401,  # UVLED OFF
                               desc=self.cmd_M1401_help)

    def get_status(self, eventtime):
        return {'display_busy': self.is_busy(),
                'is_exposure': self.is_exposure(),
                'uvled_is_pwm': self.is_uvled_pwm(),
                'uvled_value_raw': self.uvled.last_value,
                'uvled_value': self.uvled.last_value * 255.}

    def clear_cache(self):
        """
        Clear all cached buffers
        """
        self._cache = []

    def _can_cache(self):
        """
        Returns true if is ready to cache
        @rtype: bool
        """
        return (self.buffer_cache_count > 0
                and (self._cache_thread is None
                     or not self._cache_thread.is_alive()))

    def _wait_cache_thread(self):
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
                di = self.get_image_buffer(new_path)
                self._cache.append(BufferCache(new_path, di))

    def _get_cache_index(self, path):
        """
        Gets the index of the cache position on the list given a path
        @param path: Path to seek
        @return: Index of the cache on the list, otherwise -1
        """
        for i, cache in enumerate(self._cache):
            if cache.path == path:
                return i
        return -1

    def _get_image_buffercache(self, gcmd, path):
        """
        Gets an image buffer from cache if available, otherwise it creates the
        buffer from the path image
        @param gcmd:
        @param path: Image path
        @return: Image buffer
        """
        # If currently printing, use print directory to select the file
        current_filepath = self._sd.file_path()
        if current_filepath is not None and path[0] != '/':
            path = os.path.join(os.path.dirname(current_filepath), path)
            if not os.path.isfile(path):
                raise (gcmd.error("%s: The file '%s' does not exists.") %
                       (gcmd.get_command(), path))

            if self.buffer_cache_count > 0:
                self._wait_cache_thread()  # May be worth to wait if streaming
                index = self._get_cache_index(path)
                if index >= 0:
                    cache = self._cache[index]
                    # RTrim up to cache
                    self._cache = self._cache[index+1:]

                    if self._can_cache():
                        self._cache_thread = threading.Thread(
                            target=self._process_cache, args=(cache,))
                        self._cache_thread.start()

                    return cache.data

        if not os.path.isfile(path):
            raise (gcmd.error("%s: The file '%s' does not exists.") %
                   (gcmd.get_command(), path))

        di = self.get_image_buffer(path)
        if current_filepath and self._can_cache():
            cache = BufferCache(path, di)
            self._cache_thread = threading.Thread(target=self._process_cache,
                                                  args=(cache,))
            self._cache_thread.start()

        return di

    cmd_MSLA_DISPLAY_VALIDATE_help = ("Validate the display against resolution "
                                      "and pixel parameters. Throw error if out"
                                      " of parameters.")

    def cmd_MSLA_DISPLAY_VALIDATE(self, gcmd):
        """
        Layer images are universal within same pixel pitch, but it also must fit
        within the LCD area.
        Other printers with same or higher resolution and same pixel pitch can
        print same file.
        This command ensure print only continue if requirements are meet.

        Syntax: MSLA_DISPLAY_VALIDATE RESOLUTION=<x,y> PIXEL=<width,height>
                                      STRICT=[0/1]

        RESOLUTION: Machine LCD resolution
        PIXEL: Machine LCD pixel pitch
        STRICT: 0 = Prints if same or lower resolutions
                1 = Prints if same resolutions
        @param gcmd:
        @return:
        """
        resolution = gcmd.get('RESOLUTION')
        pixel_size = gcmd.get('PIXEL')
        strict = gcmd.get_int('STRICT', 0, 0, 1)

        resolution_split = resolution.split(',', 1)
        if len(resolution_split) < 2:
            raise gcmd.error("The resolution of %d is malformed. "
                             "Format: RESOLUTION_X,RESOLUTION_Y." % resolution)

        try:
            resolution_x = int(resolution_split[0])
        except:
            raise gcmd.error("The resolution x must be an integer number.")

        try:
            resolution_y = int(resolution_split[1])
        except:
            raise gcmd.error("The resolution y must be an integer.")

        if strict:
            if resolution_x > self.resolution_x:
                raise gcmd.error(
                    "The resolution X of %d is invalid. "
                    "Should be equal to %d."
                    % (resolution_x, self.resolution_x))

            if resolution_y > self.resolution_y:
                raise gcmd.error(
                    "The resolution Y of %d is invalid. "
                    "Should be equal to %d."
                    % (resolution_y, self.resolution_y))
        else:
            if resolution_x > self.resolution_x:
                raise gcmd.error("The resolution X of %d is "
                                 "invalid. Should be less or equal to %d."
                                 % (resolution_x, self.resolution_x))

            if resolution_y > self.resolution_y:
                raise gcmd.error("The resolution Y of %d is "
                                 "invalid. Should be less or equal to %d."
                                 % (resolution_y, self.resolution_y))

        pixel_size_split = pixel_size.split(',', 1)
        if len(pixel_size_split) < 2:
            raise gcmd.error("The pixel size of %f is malformed. "
                             "Format: PIXEL_WIDTH,PIXEL_HEIGHT." % pixel_size)

        try:
            pixel_width = float(pixel_size_split[0])
        except:
            raise gcmd.error("The pixel width must be an floating point "
                             "number.")

        try:
            pixel_height = float(pixel_size_split[1])
        except:
            raise gcmd.error("The pixel height must be an floating point "
                             "number.")

        if pixel_width != self.pixel_width:
            raise gcmd.error("The pixel width of %f is invalid. "
                             "Should be %f."
                             % (pixel_width, self.pixel_width))

        if pixel_height != self.pixel_height:
            raise gcmd.error("The pixel height of %f is invalid. "
                             "Should be %f."
                             % (pixel_height, self.pixel_height))

    cmd_MSLA_DISPLAY_TEST_help = ("Test the display by showing full white image"
                                  " and grey shades. Use a white paper on top "
                                  "of display and confirm if the pixels are "
                                  "healthy.")

    def cmd_MSLA_DISPLAY_TEST(self, gcmd):
        """
        Test the display by showing full white and grey shades.
        Use a white paper on top of display and confirm if the pixels
        are healthy.

        Syntax: MSLA_DISPLAY_TEST DELAY=[3000]
        DELAY: Time in milliseconds between tests (int)
        @param gcmd:
        @return:
        """
        if self._sd.current_file is not None:
            gcmd.respond_raw("MSLA_DISPLAY_TEST: Can not run this command while"
                             " printing.")
            return

        delay = gcmd.get_int('DELAY', 3000, 1000, 5000) / 1000.

        toolhead = self.printer.lookup_object('toolhead')
        color = 255
        decrement = 45

        self.set_uvled_on()
        while color > 0:
            gcmd.respond_raw("Fill color: %d" % color)
            self.fill_buffer(color)
            color -= decrement
            toolhead.dwell(delay)
        self.set_uvled_off()
        self.clear_buffer()
        gcmd.respond_raw("Test finished.")

    cmd_MSLA_DISPLAY_RESPONSE_TIME_help = ("Sends a buffer to display and test "
                                           "it response time to complete the "
                                           "render.")

    def cmd_MSLA_DISPLAY_RESPONSE_TIME(self, gcmd):
        """
        Sends a buffer to display and test it response time
        to complete the render.

        Syntax: MSLA_DISPLAY_RESPONSE_TIME AVG=[x]
        AVG: Number of samples to average (int)
        @param gcmd:
        @return:
        """
        if self._sd.current_file is not None:
            gcmd.respond_raw("MSLA_DISPLAY_RESPONSE_TIME: Can not run this "
                             "command while printing.")

        avg = gcmd.get_int('AVG', 1, 1, 20)

        fb_maxsize_mb = round(self.fb_maxsize / 1024.0 / 1024.0, 2)
        gcmd.respond_raw('Buffer size: %d bytes (%.2f MB)'
                         % (self.fb_maxsize, fb_maxsize_mb))

        time_sum = 0
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, 'buffer.png')
            with Image.new('L',
                           (self.resolution_x, self.resolution_y),
                           color='black') as image:
                image.save(path, "PNG")
            for _ in range(avg):
                timems = time.time()
                self.draw_image(path)
                time_sum += time.time() - timems
        gcmd.respond_raw('Read image from disk + send time: %fms (%d samples)'
                         % (round(time_sum * 1000 / avg, 6), avg))

        time_sum = 0
        buffer = bytes(os.urandom(self.fb_maxsize))
        for _ in range(avg):
            timems = time.time()
            self.write_buffer(buffer)
            time_sum += time.time() - timems
        gcmd.respond_raw('Send cached buffer: %fms (%d samples)'
                         % (round(time_sum * 1000 / avg, 6), avg))

        time_sum = 0
        for _ in range(avg):
            timems = time.time()
            self.clear_buffer()
            time_sum += time.time() - timems
        gcmd.respond_raw('Clear time: %fms (%d samples)' %
                         (round(time_sum * 1000 / avg, 6), avg))

    cmd_M1450_help = ("Clears the display with all black pixels "
                      "(Fill with zeros)")

    def cmd_M1450(self, gcmd):
        """
        Clears the display with all black pixels (Fill with zeros)

        Syntax: M1450 W[0/1]
        W: Wait for buffer to complete the transfer. (int)
        @param gcmd:
        """
        wait_thread = gcmd.get_int('W', 0, 0, 1)

        toolhead = self.printer.lookup_object('toolhead')
        if self.is_exposure():
            toolhead.wait_moves()
            self.clear_buffer_threaded(True)
        else:
            self.clear_buffer_threaded(wait_thread)

    cmd_M1451_help = ("Reads an image from a path and display it on "
                      "the display.")

    def cmd_M1451(self, gcmd):
        """
        Display an image from a path and display it on the main display.

        Syntax: M1451 F<"file.png"> O[x] C[0/1/3] W[0/1]
        F: File path eg: "1.png". quotes are optional. (str)
        O: Sets a positive offset from start position of the buffer. (int)
        C: Clear the remaining buffer, 0=No, 1=Yes, 2=Auto. (int) Default: 2
        W: Wait for buffer to complete the transfer. (int)
        @param gcmd:
        @return:
        """
        clear_flag = gcmd.get_int('C', framebuffer_display.CLEAR_AUTO_FLAG, 0,
                                  framebuffer_display.CLEAR_AUTO_FLAG)
        wait_thread = gcmd.get_int('W', 0, 0, 1)

        params = gcmd.get_raw_command_parameters().strip()

        if 'F' not in params:
            raise gcmd.error("Error on '%s': missing F" % gcmd.get_command())

        toolhead = self.printer.lookup_object('toolhead')
        match = self._M1451_file_regex.search(params)
        if not match:
            raise gcmd.error("Error on '%s': The F parameter"
                             " is malformed. Use a proper image file."
                             % gcmd.get_command())

        path = match.group(1).strip(' "\'')
        path = os.path.normpath(os.path.expanduser(path))
        di = self._get_image_buffercache(gcmd, path)

        max_offset = self.get_max_offset(di['size'], di['stride'], di['height'])
        offset = gcmd.get_int('O', 0, 0, max_offset)
        if self.is_exposure():
            # LED is ON, sync to prevent image change while exposure
            toolhead.wait_moves()
            self.write_buffer_threaded(di['buffer'], di['stride'], offset,
                                       clear_flag, True)
        else:
            self.write_buffer_threaded(di['buffer'], di['stride'], offset,
                                       clear_flag, wait_thread)

    def is_exposure(self):
        """
        True if the UV LED is on, exposure a layer, otherwise False
        @return:
        """
        return self.uvled.last_value > 0

    def is_uvled_pwm(self):
        """
        True if UV LED is configured as PWM pin, otherwise False
        """
        return (isinstance(self.uvled, (pwm_tool.PrinterOutputPin,
                                        pwm_cycle_time.PrinterOutputPWMCycle))
                or (isinstance(self.uvled, output_pin.PrinterOutputPin)
                    and self.uvled.is_pwm))

    def set_uvled(self, value, delay=0):
        """
        Turns the UV LED on or off.
        @param value: PWM value from 0 to 255
        @param delay: If set, it will switch off from on state after specified
                      delay. This also ensures that the UV LED is in sync with
                      the display.
        @return:
        """
        if self.is_uvled_pwm():
            value /= 255.0
        else:
            value = 1 if value else 0

        if value > 0:
            # Sync display render
            self.wait_framebuffer_thread()

        # Do not continue if same value
        if self.uvled.last_value == value:
            return

        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda print_time: self.uvled._set_pin(print_time, value))

        if value > 0 and delay > 0:
            delay -= self.uvled_response_delay / 1000.
            if delay > 0:
                toolhead.dwell(delay)
            toolhead.register_lookahead_callback(
                lambda print_time: self.uvled._set_pin(print_time, 0))
        toolhead.wait_moves()

    def set_uvled_on(self, delay=0):
        """
        Turns the UV LED on with max power
        @param delay: If set, it will switch off from on state after specified
                      delay. This also ensures that the UV LED is in sync with
                      the display.
        @return:
        """
        self.set_uvled(255, delay)

    def set_uvled_off(self):
        """
        Turns the UV LED off
        """
        self.set_uvled(0)

    cmd_MSLA_UVLED_RESPONSE_TIME_help = ("Tests the response time for the "
                                         "UV LED shutter.")

    def cmd_MSLA_UVLED_RESPONSE_TIME(self, gcmd):
        """
        Tests the response time for the UV LED to turn on and off.

        Syntax: MSLA_UVLED_RESPONSE_TIME TIME=<ms> OFFSET=[ms]
        TIME: Exposure time in milliseconds. (int)
        OFFSET: Offset time from exposure time. (int)
        TYPE: <0> when using M1400 Sx and Px. (Faster response time)
              <1> when using M1400 Sx, G4 Px, M1400 S0 (Slower response time)
        @param gcmd:
        @return:
        """
        if self._sd.current_file is not None:
            gcmd.respond_raw("MSLA_UVLED_RESPONSE_TIME: Can not run this "
                             "command while printing.")

        self.set_uvled(0)
        self.clear_buffer()

        delay = gcmd.get_int('TIME', 3000, minval=500, maxval=5000) / 1000.
        offset = gcmd.get_int('OFFSET', 0, minval=-1000, maxval=0) / 1000.
        etype = gcmd.get_int('TYPE', 0, minval=0, maxval=1)

        if etype == 0:
            offset -= 0.2

        toolhead = self.printer.lookup_object('toolhead')

        toolhead.wait_moves()
        time_total = enable_time = time.time()
        toolhead.register_lookahead_callback(
            lambda print_time: self.uvled._set_pin(print_time, 1))
        toolhead.wait_moves()
        enable_time = round(time.time() - enable_time, 4)

        exposure_time = time.time()
        toolhead.dwell(delay + offset)

        toolhead.register_lookahead_callback(
            lambda print_time: self.uvled._set_pin(print_time, 0))
        toolhead.wait_moves()

        final_time = time.time()
        exposure_time = round(final_time - exposure_time, 4)
        time_total = round(final_time - time_total, 4)

        if etype == 0:
            offset += 0.2
        calculated_offset = max(0,
                                round((exposure_time - delay + offset) * 1000))

        exposure_factor = round(exposure_time / delay, 2)

        gcmd.respond_raw("UV LED RESPONSE TIME TEST (%fs):\n"
                         "Switch time: %d ms\n"
                         "Exposure time: %f s\n"
                         "Total time: %f s\n"
                         "Exposure time factor: %f x\n"
                         "Calculated delay offset: %d ms\n"
                         % (delay, round(enable_time * 1000), exposure_time,
                            time_total, exposure_factor, calculated_offset))

    cmd_M1400_help = "Turn the main UV LED to cure the pixels."

    def cmd_M1400(self, gcmd):
        """
        Turn the main UV LED to cure the pixels.
        M1400 comes from the wavelength of UV radiation (UVR)
        lies in the range of 100-400 nm, and is further subdivided into
        UVA (315-400 nm), UVB (280-315 nm), and UVC (100-280 nm).

        Syntax: M1400 S[0-255] P[ms]
        S: LED Power (Non PWM LEDs will turn on from 1 to 255). (float)
        P: Time to wait in milliseconds when (S>0) before turn off. (int)
        @param gcmd:
        """
        value = gcmd.get_float('S', 255., minval=0., maxval=255.)
        delay = gcmd.get_float('P', 0, above=0.) / 1000.
        self.set_uvled(value, delay)

    cmd_M1401_help = "Turn the main UV LED off."

    def cmd_M1401(self, gcmd):
        """
        Turn the main UV LED off

        Syntax: M1401
        @param gcmd:
        """
        self.set_uvled(0)


class BufferCache:
    def __init__(self, path, data):
        self.path = path
        self.data = data

    def new_path_layerindex(self, n):
        """
        Return a new path with same base directory but new layer index
        @param n:
        @return:
        """
        extension = self.path[self.path.rfind('.'):]
        return re.sub(r"\d+%s$" % re.escape(extension),
                      "%d%s" % (n, extension),
                      self.path)

    def get_layer_index(self):
        """
        Gets the layer index from a path
        @return: New layer index. -1 if not founded.
        """
        extension = self.path[self.path.rfind('.'):]
        match = re.search(r"(\d+)%s$" % re.escape(extension), self.path)
        if match is None:
            return -1

        return int(match.group(1))


def load_config(config):
    return mSLADisplay(config)
