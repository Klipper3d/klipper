# Support for "neopixel" leds
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

BIT_MAX_TIME=.000004
RESET_MIN_TIME=.000050

MAX_MCU_SIZE = 500  # Sanity check on LED chain length

class PrinterNeoPixel:
    def __init__(self, config):
        self.printer = config.get_printer()
        name = config.get_name().split()[1]
        self.mutex = self.printer.get_reactor().mutex()
        # Configure neopixel
        ppins = self.printer.lookup_object('pins')
        pin_params = ppins.lookup_pin(config.get('pin'))
        self.mcu = pin_params['chip']
        self.oid = self.mcu.create_oid()
        self.pin = pin_params['pin']
        self.mcu.register_config_callback(self.build_config)
        formats = {v: v for v in ["RGB", "GRB", "RGBW", "GRBW"]}
        self.color_order = config.getchoice("color_order", formats, "GRB")
        elem_size = len(self.color_order)
        self.chain_count = config.getint('chain_count', 1, minval=1,
                                         maxval=MAX_MCU_SIZE//elem_size)
        self.neopixel_update_cmd = self.neopixel_send_cmd = None
        # Initial color
        self.color_data = bytearray(self.chain_count * elem_size)
        red = config.getfloat('initial_RED', 0., minval=0., maxval=1.)
        green = config.getfloat('initial_GREEN', 0., minval=0., maxval=1.)
        blue = config.getfloat('initial_BLUE', 0., minval=0., maxval=1.)
        white = 0
        if elem_size == 4:
            white = config.getfloat('initial_WHITE', 0., minval=0., maxval=1.)
        self.update_color_data(red, green, blue, white)
        self.old_color_data = bytearray([d ^ 1 for d in self.color_data])
        # Register commands
        self.printer.register_event_handler("klippy:connect", self.send_data)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_LED", "LED", name, self.cmd_SET_LED,
                                   desc=self.cmd_SET_LED_help)
    def build_config(self):
        bmt = self.mcu.seconds_to_clock(BIT_MAX_TIME)
        rmt = self.mcu.seconds_to_clock(RESET_MIN_TIME)
        self.mcu.add_config_cmd("config_neopixel oid=%d pin=%s data_size=%d"
                                " bit_max_ticks=%d reset_min_ticks=%d"
                                % (self.oid, self.pin, len(self.color_data),
                                   bmt, rmt))
        cmd_queue = self.mcu.alloc_command_queue()
        self.neopixel_update_cmd = self.mcu.lookup_command(
            "neopixel_update oid=%c pos=%hu data=%*s", cq=cmd_queue)
        self.neopixel_send_cmd = self.mcu.lookup_query_command(
            "neopixel_send oid=%c", "neopixel_result oid=%c success=%c",
            oid=self.oid, cq=cmd_queue)
    def update_color_data(self, red, green, blue, white, index=None):
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        white = int(white * 255. + .5)
        if self.color_order == "GRB":
            color_data = [green, red, blue]
        elif self.color_order == "RGB":
            color_data = [red, green, blue]
        elif self.color_order == "GRBW":
            color_data = [green, red, blue, white]
        else:
            color_data = [red, green, blue, white]
        if index is None:
            self.color_data[:] = color_data * self.chain_count
        else:
            elem_size = len(color_data)
            self.color_data[(index-1)*elem_size:index*elem_size] = color_data
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
        ucmd = self.neopixel_update_cmd.send
        for pos, count in diffs:
            ucmd([self.oid, pos, new_data[pos:pos+count]],
                 reqclock=BACKGROUND_PRIORITY_CLOCK)
        old_data[:] = new_data
        # Instruct mcu to update the LEDs
        minclock = 0
        if print_time is not None:
            minclock = self.mcu.print_time_to_clock(print_time)
        scmd = self.neopixel_send_cmd.send
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        for i in range(8):
            params = scmd([self.oid], minclock=minclock,
                          reqclock=BACKGROUND_PRIORITY_CLOCK)
            if params['success']:
                break
        else:
            logging.info("Neopixel update did not succeed")
    cmd_SET_LED_help = "Set the color of an LED"
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        white = gcmd.get_float('WHITE', 0., minval=0., maxval=1.)
        index = gcmd.get_int('INDEX', None, minval=1, maxval=self.chain_count)
        transmit = gcmd.get_int('TRANSMIT', 1)
        sync = gcmd.get_int('SYNC', 1)
        # Update and transmit data
        def reactor_bgfunc(print_time):
            with self.mutex:
                self.update_color_data(red, green, blue, white, index)
                if transmit:
                    self.send_data(print_time)
        def lookahead_bgfunc(print_time):
            reactor = self.printer.get_reactor()
            reactor.register_callback(lambda et: reactor_bgfunc(print_time))
        if sync:
            #Sync LED Update with print time and send
            toolhead = self.printer.lookup_object('toolhead')
            toolhead.register_lookahead_callback(lookahead_bgfunc)
        else:
            #Send update now (so as not to wake toolhead and reset idle_timeout)
            lookahead_bgfunc(None)

def load_config_prefix(config):
    return PrinterNeoPixel(config)
