# Support for "neopixel" leds
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

BIT_MAX_TIME=.000004
RESET_MIN_TIME=.000050

class PrinterNeoPixel:
    def __init__(self, config):
        self.printer = config.get_printer()
        name = config.get_name().split()[1]
        # Configure neopixel
        ppins = self.printer.lookup_object('pins')
        pin_params = ppins.lookup_pin(config.get('pin'))
        self.mcu = pin_params['chip']
        self.oid = self.mcu.create_oid()
        self.pin = pin_params['pin']
        self.mcu.register_config_callback(self.build_config)
        self.color_order_GRB = config.getboolean("color_order_GRB", True)
        self.chain_count = config.getint('chain_count', 1, minval=1, maxval=18)
        self.neopixel_send_cmd = None
        # Initial color
        red = config.getfloat('initial_RED', 0., minval=0., maxval=1.)
        green = config.getfloat('initial_GREEN', 0., minval=0., maxval=1.)
        blue = config.getfloat('initial_BLUE', 0., minval=0., maxval=1.)
        self.update_color_data(red, green, blue)
        self.printer.register_event_handler("klippy:connect", self.send_data)
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_LED", "LED", name, self.cmd_SET_LED,
                                   desc=self.cmd_SET_LED_help)
    def build_config(self):
        bmt = self.mcu.seconds_to_clock(BIT_MAX_TIME)
        rmt = self.mcu.seconds_to_clock(RESET_MIN_TIME)
        self.mcu.add_config_cmd("config_neopixel oid=%d pin=%s"
                                " bit_max_ticks=%d reset_min_ticks=%d"
                                % (self.oid, self.pin, bmt, rmt))
        cmd_queue = self.mcu.alloc_command_queue()
        self.neopixel_send_cmd = self.mcu.lookup_command(
            "neopixel_send oid=%c data=%*s", cq=cmd_queue)
    def update_color_data(self, red, green, blue, index=None):
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        if self.color_order_GRB:
            color_data = [green, red, blue]
        else:
            color_data = [red, green, blue]
        if index is None:
            self.color_data = color_data * self.chain_count
        else:
            self.color_data[(index-1)*3:index*3] = color_data
    def send_data(self, minclock=0):
        self.neopixel_send_cmd.send([self.oid, self.color_data],
                                    minclock=minclock,
                                    reqclock=BACKGROUND_PRIORITY_CLOCK)
    cmd_SET_LED_help = "Set the color of an LED"
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        index = gcmd.get_int('INDEX', None, minval=1, maxval=self.chain_count)
        transmit = gcmd.get_int('TRANSMIT', 1)
        self.update_color_data(red, green, blue, index)
        # Send command
        if not transmit:
            return
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.send_data(self.mcu.print_time_to_clock(print_time))

def load_config_prefix(config):
    return PrinterNeoPixel(config)
