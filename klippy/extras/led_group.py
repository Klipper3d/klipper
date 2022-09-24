# Support for LED groups
#
# Copyright (C) 2022 Julian Schill <j.schill@web.de>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
class PrinterLEDGroup:
    def __init__(self, config):
        self.config = config
        self.printer = config.get_printer()
        self.configLeds = config.get('leds')
        self.configChains = self.configLeds.split('\n')
        self.leds=[]
        self.ledChains=[]
        self.printer.register_event_handler('klippy:ready', self._handle_ready)

    def _handle_ready(self):
        for chain in self.configChains:
            chain = chain.strip()
            parms = [parameter.strip() for parameter in chain.split()
                        if parameter.strip()]
            if parms:
                ledChain     = self.printer.lookup_object(parms[0]\
                                                .replace(':',' '))
                ledIndices   = ''.join(parms[1:]).strip('()').split(',')
                for led in ledIndices:
                    if led:
                        if '-' in led:
                            start, stop = map(int,led.split('-'))
                            if stop == start:
                                ledList = [start-1]
                            elif stop > start:
                                ledList = list(range(start-1, stop))
                            else:
                                ledList = list(reversed(range(stop-1, start)))
                            for i in ledList:
                                self.leds.append((ledChain, int(i)))
                        else:
                            for i in led.split(','):
                                self.leds.append((ledChain, \
                                                (int(i)-1)))
                    else:
                        for i in range(ledChain.led_helper.get_led_count()):
                            self.leds.append((ledChain, int(i)))
                if ledChain not in self.ledChains:
                    self.ledChains.append(ledChain)
        self.ledCount = len(self.leds)

        pled = self.printer.load_object(self.config, "led")
        self.led_helper = pled.setup_helper(self.config, self.update_leds,
                                                self.ledCount)

    def update_leds(self, led_state, print_time):
        for i, (chain, index) in enumerate(self.leds):
            chain.led_helper.set_color(index+1, led_state[i])
        for chain in self.ledChains:
            chain.led_helper.check_transmit(print_time)

    def get_status(self, eventtime=None):
        return self.led_helper.get_status(eventtime)

def load_config_prefix(config):
    return PrinterLEDGroup(config)
