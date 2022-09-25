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
        self.config_chains = self.config_leds.split('\n')
        self.leds=[]
        self.led_chains=[]
        self.printer.register_event_handler('klippy:connect',
                                                self._handle_connect)

    def _handle_connect(self):
        for chain in self.config_chains:
            chain = chain.strip()
            parms = [parameter.strip() for parameter in chain.split()
                        if parameter.strip()]
            if parms:
                if parms[0].startswith("led_group"):
                    raise self.config.error(
                        "Error in '%s': LED group of LED groups is not allowed"
                            % (self.config.get_name(),))
                led_chain = self.printer.lookup_object(parms[0]\
                                                .replace(':',' '))
                chain_len = led_chain.led_helper.get_led_count()
                led_indices = ''.join(parms[1:]).strip('()').split(',')
                for led in led_indices:
                    if led:
                        if '-' in led:
                            start, stop = map(int,led.split('-'))
                            if start > chain_len or stop > chain_len or \
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
                                self.leds.append((led_chain, int(i)))
                        else:
                            i = int(led)
                            if i > chain_len or i < 1:
                                raise self.config.error(
                                    "LED index out of range for '%s' in '%s'"
                                        % (parms[0],self.config.get_name(),))
                            self.leds.append((led_chain, (int(i)-1)))
                    else:
                        for i in range(chain_len):
                            self.leds.append((led_chain, int(i)))
                if led_chain not in self.led_chains:
                    self.led_chains.append(led_chain)
        self.ledCount = len(self.leds)

        pled = self.printer.load_object(self.config, "led")
        self.led_helper = pled.setup_helper(self.config, self.update_leds,
                                                self.ledCount)

    def update_leds(self, led_state, print_time):
        for i, (chain, index) in enumerate(self.leds):
            chain.led_helper.set_color(index+1, led_state[i])
        for chain in self.led_chains:
            chain.led_helper.check_transmit(print_time)

    def get_status(self, eventtime=None):
        return self.led_helper.get_status(eventtime)

def load_config_prefix(config):
    return PrinterLEDGroup(config)
