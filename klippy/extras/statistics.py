# Support for logging periodic statistics
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class PrinterStats:
    def __init__(self, config):
        self.printer = config.get_printer()
        reactor = self.printer.get_reactor()
        self.stats_timer = reactor.register_timer(self.generate_stats)
        self.stats_cb = []
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
    def handle_ready(self):
        self.stats_cb = [o.stats for n, o in self.printer.lookup_objects()
                         if hasattr(o, 'stats')]
        if self.printer.get_start_args().get('debugoutput') is None:
            reactor = self.printer.get_reactor()
            reactor.update_timer(self.stats_timer, reactor.NOW)
    def generate_stats(self, eventtime):
        stats = [cb(eventtime) for cb in self.stats_cb]
        if max([s[0] for s in stats]):
            logging.info("Stats %.1f: %s", eventtime,
                         ' '.join([s[1] for s in stats]))
        return eventtime + 1.

def load_config(config):
    return PrinterStats(config)
