# Support for "dotstar" leds
#
# Copyright (C) 2019-2022  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

class PrinterDotstar:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        # Configure a software spi bus
        ppins = printer.lookup_object('pins')
        data_pin_params = ppins.lookup_pin(config.get('data_pin'))
        clock_pin_params = ppins.lookup_pin(config.get('clock_pin'))
        mcu = data_pin_params['chip']
        if mcu is not clock_pin_params['chip']:
            raise config.error("Dotstar pins must be on same mcu")
        sw_spi_pins = (data_pin_params['pin'], data_pin_params['pin'],
                       clock_pin_params['pin'])
        self.spi = bus.MCU_SPI(mcu, None, None, 0, 500000, sw_spi_pins)
        # Initialize color data
        self.chain_count = config.getint('chain_count', 1, minval=1)
        pled = printer.load_object(config, "led")
        self.led_helper = pled.setup_helper(config, self.update_leds,
                                            self.chain_count)
        self.prev_data = None
        # Register commands
        printer.register_event_handler("klippy:connect", self.handle_connect)
    def handle_connect(self):
        self.update_leds(self.led_helper.get_status()['color_data'], None)
    def update_leds(self, led_state, print_time):
        if led_state == self.prev_data:
            return
        self.prev_data = led_state
        # Build data to send
        data = [0] * ((len(led_state) + 2) * 4)
        for i, (red, green, blue, white) in enumerate(led_state):
            idx = (i + 1) * 4
            data[idx] = 0xff
            data[idx+1] = int(blue * 255. + .5)
            data[idx+2] = int(green * 255. + .5)
            data[idx+3] = int(red * 255. + .5)
        data[-4] = data[-3] = data[-2] = data[-1] = 0xff
        # Transmit update
        minclock = 0
        if print_time is not None:
            minclock = self.spi.get_mcu().print_time_to_clock(print_time)
        for d in [data[i:i+20] for i in range(0, len(data), 20)]:
            self.spi.spi_send(d, minclock=minclock,
                              reqclock=BACKGROUND_PRIORITY_CLOCK)
    def get_status(self, eventtime):
        return self.led_helper.get_status(eventtime)

def load_config_prefix(config):
    return PrinterDotstar(config)
