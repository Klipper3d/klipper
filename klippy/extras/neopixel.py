# Support for "neopixel" leds
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

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
        self.chain_count = config.getint('chain_count', 1, minval=1, maxval=18)
        self.neopixel_send_cmd = None
        # Initial color
        red = config.getfloat('initial_RED', 0., minval=0., maxval=1.)
        green = config.getfloat('initial_GREEN', 0., minval=0., maxval=1.)
        blue = config.getfloat('initial_BLUE', 0., minval=0., maxval=1.)
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        self.color_data = [green, red, blue] * self.chain_count
        self.printer.register_event_handler("klippy:connect", self.send_data)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("SET_NEOPIXEL", "NEOPIXEL", name,
                                        self.cmd_SET_NEOPIXEL,
                                        desc=self.cmd_SET_NEOPIXEL_help)
    def build_config(self):
        if self.mcu.get_constant_float('CLOCK_FREQ') <= 20000000:
            raise self.printer.config_error(
                "Neopixel is not supported on AVR micro-controllers")
        cmd_queue = self.mcu.alloc_command_queue()
        self.neopixel_send_cmd = self.mcu.lookup_command(
            "neopixel_send oid=%c data=%*s", cq=cmd_queue)
    def send_data(self, minclock=0):
        self.neopixel_send_cmd.send([self.oid, self.color_data],
                                    minclock=minclock)
    cmd_SET_NEOPIXEL_help = "Set the color of a neopixel led"
    def cmd_SET_NEOPIXEL(self, params):
        # Parse parameters
        red = self.gcode.get_float('RED', params, 0., minval=0., maxval=1.)
        green = self.gcode.get_float('GREEN', params, 0., minval=0., maxval=1.)
        blue = self.gcode.get_float('BLUE', params, 0., minval=0., maxval=1.)
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        color_data = [green, red, blue]
        if 'INDEX' in params:
            index = self.gcode.get_int('INDEX', params,
                                       minval=1, maxval=self.chain_count)
            self.color_data[(index-1)*3:index*3] = color_data
        else:
            self.color_data = color_data * self.chain_count
        # Send command
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.send_data(self.mcu.print_time_to_clock(print_time))

def load_config_prefix(config):
    return PrinterNeoPixel(config)
