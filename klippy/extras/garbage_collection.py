# Garbage collection optimizations
#
# Copyright (C) 2025  Branden Cash <ammmze@gmail.com>
# Copyright (C) 2016-2026  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import gc
import logging

class GarbageCollection:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        # Perform gc on reactor idle
        self._last_gc_times = [0., 0., 0.]
        reactor = printer.get_reactor()
        reactor.set_idle_notifier(self._handle_idle)
        printer.register_event_handler("klippy:analyze_shutdown",
                                       self._handle_analyze_shutdown)
        gc.disable()
        # feature check ... freeze/unfreeze is only available in python 3.7+
        can_freeze = hasattr(gc, 'freeze') and hasattr(gc, 'unfreeze')
        if can_freeze:
            printer.register_event_handler("klippy:ready", self._handle_ready)
            printer.register_event_handler("klippy:disconnect",
                                           self._handle_disconnect)

    def _handle_analyze_shutdown(self, msg, details):
        logging.info("Reactor garbage collection: %s", self._last_gc_times)

    def _handle_idle(self, eventtime, start_busy_time):
        gi = gc.get_count()
        if gi[0] < 700:
            return False
        # Reactor is idle and gc is due - run it
        gc_level = 0
        if gi[1] >= 10:
            gc_level = 1
            if gi[2] >= 10:
                gc_level = 2
        self._last_gc_times[gc_level] = eventtime
        gc.collect(gc_level)
        return True

    def _handle_ready(self):
        logging.debug("Running full garbage collection and freezing")
        for n in range(3):
            gc.collect(n)
        gc.freeze()

    def _handle_disconnect(self):
        logging.debug("Unfreezing garbage collection")
        gc.unfreeze()

def load_config(config):
    return GarbageCollection(config)
