# Support for LED groups
#
# Copyright (C) 2022 Julian Schill <j.schill@web.de>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
class PrinterLEDGroup:
    def __init__(self, config):
        self.config = config
        self.printer = config.get_printer()
        self.config_leds = config.get('leds')
        self.printer_led = self.printer.lookup_object('led')
        self.config_chains = self.config_leds.split('\n')
        self.leds=[]
        self.led_helpers=[]
        self.printer.register_event_handler('klippy:connect',
                                                self._handle_connect)

    def _handle_connect(self):
        for chain in self.config_chains:
            chain = chain.strip()
            parms = [parameter.strip() for parameter in chain.split()
                        if parameter.strip()]
            if parms:
                led_helper = self.printer_led.lookup_led_helper(parms[0])
                led_count = led_helper.get_led_count()
                led_indices = ''.join(parms[1:]).strip('()').split(',')
                for led in led_indices:
                    if led:
                        if '-' in led:
                            start, stop = map(int,led.split('-'))
                            if start > led_count or stop > led_count or \
                                start < 1 or stop < 1:
                                raise self.config.error(
                                    "LED index out of range for '%s' in '%s'"
                                        % (parms[0],self.config.get_name(),))
                            if stop == start:
                                led_list = [start-1]
                            elif stop > start:
                                led_list = list(range(start-1, stop))
                            else:
                                led_list = list(reversed(range(stop-1, start)))
                            for i in led_list:
                                self.leds.append((led_helper, int(i)))
                        else:
                            i = int(led)
                            if i > led_count or i < 1:
                                raise self.config.error(
                                    "LED index out of range for '%s' in '%s'"
                                        % (parms[0],self.config.get_name(),))
                            self.leds.append((led_helper, (int(i)-1)))
                    else:
                        for i in range(led_count):
                            self.leds.append((led_helper, int(i)))
                if led_helper not in self.led_helpers:
                    self.led_helpers.append(led_helper)
        self.ledCount = len(self.leds)

        pled = self.printer.load_object(self.config, "led")
        self.led_helper = pled.setup_helper(self.config, self.update_leds,
                                                self.ledCount)

    def update_leds(self, led_state, print_time):
        for i, (led_helper, index) in enumerate(self.leds):
            led_helper.set_color(index+1, led_state[i])
        for led_helper in self.led_helpers:
            led_helper.check_transmit(print_time)

    def get_status(self, eventtime=None):
        return self.led_helper.get_status(eventtime)

def load_config_prefix(config):
    return PrinterLEDGroup(config)
