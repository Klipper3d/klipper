# Support for "neopixel" leds
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

class PrinterNeoPixel:
    def __init__(self, config):
        self.printer = config.get_printer()
        name = config.get_name().split()[1]
        # Configure neopixel
        ppins = self.printer.lookup_object('pins')
        pin_params = ppins.lookup_pin(config.get('pin'))
        self.mcu = pin_params['chip']
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd("config_neopixel oid=%d pin=%s"
                                % (self.oid, pin_params['pin']))
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
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("SET_LED", "LED", name,
                                        self.cmd_SET_LED,
                                        desc=self.cmd_SET_LED_help)
    def build_config(self):
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
    def cmd_SET_LED(self, params):
        # Parse parameters
        red = self.gcode.get_float('RED', params, 0., minval=0., maxval=1.)
        green = self.gcode.get_float('GREEN', params, 0., minval=0., maxval=1.)
        blue = self.gcode.get_float('BLUE', params, 0., minval=0., maxval=1.)
        index = self.gcode.get_int('INDEX', params, None,
                                   minval=1, maxval=self.chain_count)
        self.update_color_data(red, green, blue, index)
        # Send command
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.send_data(self.mcu.print_time_to_clock(print_time))

def load_config_prefix(config):
    return PrinterNeoPixel(config)
