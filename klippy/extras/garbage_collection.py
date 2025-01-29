# Garbage collection optimizations
#
# Copyright (C) 2025  Branden Cash <ammmze@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import gc
import logging

class GarbageCollection:
    def __init__(self, config):
        self.printer = config.get_printer()
        # feature check ... freeze/unfreeze is only available in python 3.7+
        can_freeze = hasattr(gc, 'freeze') and hasattr(gc, 'unfreeze')
        if can_freeze:
            self.printer.register_event_handler("klippy:ready",
                                                self._handle_ready)
            self.printer.register_event_handler("klippy:disconnect",
                                                self._handle_disconnect)

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
