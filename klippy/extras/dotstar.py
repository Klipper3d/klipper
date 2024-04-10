# Support for "dotstar" leds
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import bus

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

class PrinterDotstar:
    def __init__(self, config):
        self.printer = config.get_printer()
        name = config.get_name().split()[1]
        # Configure a software spi bus
        ppins = self.printer.lookup_object('pins')
        data_pin_params = ppins.lookup_pin(config.get('data_pin'))
        clock_pin_params = ppins.lookup_pin(config.get('clock_pin'))
        mcu = data_pin_params['chip']
        if mcu is not clock_pin_params['chip']:
            raise config.error("Dotstar pins must be on same mcu")
        sw_spi_pins = (data_pin_params['pin'], data_pin_params['pin'],
                       clock_pin_params['pin'])
        self.spi = bus.MCU_SPI(mcu, None, None, 0, 500000, sw_spi_pins)
        # Initial color
        self.chain_count = config.getint('chain_count', 1, minval=1)
        red = config.getfloat('initial_RED', 0., minval=0., maxval=1.)
        green = config.getfloat('initial_GREEN', 0., minval=0., maxval=1.)
        blue = config.getfloat('initial_BLUE', 0., minval=0., maxval=1.)
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        color_data = [0xff, blue, green, red] * self.chain_count
        self.color_data = [0, 0, 0, 0] + color_data + [0xff, 0xff, 0xff, 0xff]
        self.printer.register_event_handler("klippy:connect", self.send_data)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("SET_LED", "LED", name,
                                        self.cmd_SET_LED,
                                        desc=self.cmd_SET_LED_help)
    def send_data(self, minclock=0):
        data = self.color_data
        for d in [data[i:i+20] for i in range(0, len(data), 20)]:
            self.spi.spi_send(d, minclock=minclock,
                              reqclock=BACKGROUND_PRIORITY_CLOCK)
    cmd_SET_LED_help = "Set the color of an LED"
    def cmd_SET_LED(self, params):
        # Parse parameters
        red = self.gcode.get_float('RED', params, 0., minval=0., maxval=1.)
        green = self.gcode.get_float('GREEN', params, 0., minval=0., maxval=1.)
        blue = self.gcode.get_float('BLUE', params, 0., minval=0., maxval=1.)
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        color_data = [0xff, blue, green, red]
        if 'INDEX' in params:
            index = self.gcode.get_int('INDEX', params,
                                       minval=1, maxval=self.chain_count)
            self.color_data[index*4:(index+1)*4] = color_data
        else:
            self.color_data[4:-4] = color_data * self.chain_count
        # Send command
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.send_data(self.spi.get_mcu().print_time_to_clock(print_time))

def load_config_prefix(config):
    return PrinterDotstar(config)
