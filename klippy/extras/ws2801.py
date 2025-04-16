# Support for "ws2801" leds
# coding=utf-8
# Copyright (C) 2024  Anders Törnberg <anders.tornberg@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

MAX_MCU_SIZE = 500  # Sanity check on LED chain length

class WS2801:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        self.mutex = printer.get_reactor().mutex()
        # Configure ws2801
        self.spi = bus.MCU_SPI_from_config(config, 0, default_speed=1000000)
        self.mcu = self.spi.get_mcu()
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self.build_config)
        self.ws8201_update_cmd = self.ws8201_send_cmd = None
        # Build color map
        chain_count = config.getint('chain_count', 1, minval=1)
        color_order = config.getlist("color_order", ["RGB"])
        if len(color_order) == 1:
            color_order = [color_order[0]] * chain_count
        if len(color_order) != chain_count:
            raise config.error("color_order does not match chain_count")
        color_indexes = []
        for lidx, co in enumerate(color_order):
            if sorted(co) not in (sorted("RGB"), sorted("RGB")):
                raise config.error("Invalid color_order '%s'" % (co,))
            color_indexes.extend([(lidx, "RGB".index(c)) for c in co])
        self.color_map = list(enumerate(color_indexes))
        if len(self.color_map) > MAX_MCU_SIZE:
            raise config.error("WS2801 chain too long")
        # Initialize color data
        pled = printer.load_object(config, "led")
        self.led_helper = pled.setup_helper(config, self.update_leds,
                                            chain_count)
        self.color_data = bytearray(len(self.color_map))
        self.update_color_data(self.led_helper.get_status()['color_data'])
        self.old_color_data = bytearray([d ^ 1 for d in self.color_data])
        # Register callbacks
        printer.register_event_handler("klippy:connect", self.send_data)
    def build_config(self):
        print(self.spi.get_oid())
        self.mcu.add_config_cmd("config_ws2801 oid=%d spi_oid=%d data_size=%d"
                        % (self.oid, self.spi.get_oid(), len(self.color_data)))
        cmd_queue = self.mcu.alloc_command_queue()
        self.ws2801_update_cmd = self.mcu.lookup_command(
            "ws2801_update oid=%c pos=%hu data=%*s", cq=cmd_queue)
        self.ws2801_send_cmd = self.mcu.lookup_query_command(
            "ws2801_send oid=%c", "ws2801_result oid=%c success=%c",
            oid=self.oid, cq=cmd_queue)
    def update_color_data(self, led_state):
        color_data = self.color_data
        for cdidx, (lidx, cidx) in self.color_map:
            color_data[cdidx] = int(led_state[lidx][cidx] * 255. + .5)
    def send_data(self, print_time=None):
        old_data, new_data = self.old_color_data, self.color_data
        if new_data == old_data:
            return
        # Find the position of all changed bytes in this framebuffer
        diffs = [[i, 1] for i, (n, o) in enumerate(zip(new_data, old_data))
                 if n != o]
        # Batch together changes that are close to each other
        for i in range(len(diffs)-2, -1, -1):
            pos, count = diffs[i]
            nextpos, nextcount = diffs[i+1]
            if pos + 5 >= nextpos and nextcount < 16:
                diffs[i][1] = nextcount + (nextpos - pos)
                del diffs[i+1]
        # Transmit changes
        for pos, count in diffs:
            print(new_data[pos:pos+count])
        ucmd = self.ws2801_update_cmd.send
        for pos, count in diffs:
            ucmd([self.oid, pos, new_data[pos:pos+count]],
                 reqclock=BACKGROUND_PRIORITY_CLOCK)
        old_data[:] = new_data
        #Instruct mcu to update the LEDs
        minclock = 0
        if print_time is not None:
            minclock = self.mcu.print_time_to_clock(print_time)
        scmd = self.ws2801_send_cmd.send
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        for i in range(8):
            params = scmd([self.oid], minclock=minclock,
                          reqclock=BACKGROUND_PRIORITY_CLOCK)
            if params['success']:
                break
        else:
            logging.info("WS2801 update did not succeed")
    def update_leds(self, led_state, print_time):
        def reactor_bgfunc(eventtime):
            with self.mutex:
                self.update_color_data(led_state)
                self.send_data(print_time)
        self.printer.get_reactor().register_callback(reactor_bgfunc)
    def get_status(self, eventtime=None):
        return self.led_helper.get_status(eventtime)

def load_config_prefix(config):
    return WS2801(config)
