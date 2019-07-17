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
        self.neopixel_send_cmd = None
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
    cmd_SET_NEOPIXEL_help = "Set the color of a neopixel led"
    def cmd_SET_NEOPIXEL(self, params):
        # Parse parameters
        red = self.gcode.get_float('RED', params, 0., minval=0., maxval=1.)
        green = self.gcode.get_float('GREEN', params, 0., minval=0., maxval=1.)
        blue = self.gcode.get_float('BLUE', params, 0., minval=0., maxval=1.)
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        # Send command
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        minclock = self.mcu.print_time_to_clock(print_time)
        self.neopixel_send_cmd.send([self.oid, [green, red, blue]],
                                    minclock=minclock)

def load_config_prefix(config):
    return PrinterNeoPixel(config)
