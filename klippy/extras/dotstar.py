# Support for "dotstar" leds
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

class PrinterDotstar:
    def __init__(self, config):
        self.printer = config.get_printer()
        name = config.get_name().split()[1]
        self.mutex = self.printer.get_reactor().mutex()
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
        color_data = [0xff, blue, green, red] * self.chain_count
        self.color_data = [0, 0, 0, 0] + color_data + [0xff, 0xff, 0xff, 0xff]
        self.update_color_data(red, green, blue)
        self.old_color_data = bytearray([d ^ 1 for d in self.color_data])
        # Register commands
        self.printer.register_event_handler("klippy:connect", self.send_data)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_LED", "LED", name, self.cmd_SET_LED,
                                   desc=self.cmd_SET_LED_help)
    def update_color_data(self, red, green, blue, white=None, index=None):
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        color_data = [0xff, blue, green, red]
        if index is not None:
            self.color_data[index*4:(index+1)*4] = color_data
        else:
            self.color_data[4:-4] = color_data * self.chain_count
    def send_data(self, print_time=None):
        old_data, new_data = self.old_color_data, self.color_data
        if new_data == old_data:
            return

        minclock = 0
        if print_time is not None:
            minclock = self.spi.get_mcu().print_time_to_clock(print_time)
        data = self.color_data
        for d in [data[i:i+20] for i in range(0, len(data), 20)]:
            self.spi.spi_send(d, minclock=minclock,
                              reqclock=BACKGROUND_PRIORITY_CLOCK)
    cmd_SET_LED_help = "Set the color of an LED"
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        white = 0.0 #dotstar's dont have white yet
        index = gcmd.get_int('INDEX', None, minval=1, maxval=self.chain_count)
        transmit = gcmd.get_int('TRANSMIT', 1)
        sync = gcmd.get_int('SYNC', 1)
        def reactor_bgfunc(print_time):
            with self.mutex:
                self.update_color_data(red, green, blue, index=index)
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
    return PrinterDotstar(config)
