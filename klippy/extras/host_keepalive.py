# -*- coding: utf-8 -*-
# Support for host keepalive messages
#
# Copyright (C) 2018  Janar Sööt <janar.soot@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.


class HostKeepAlive:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.toolhead = None
        self.gcode = None
        self._counter = 0
        self._interval = config.getint('interval', 5, minval=1)

    def printer_state(self, state):
        if state == 'ready':
            self.toolhead = self.printer.lookup_object('toolhead')
            self.gcode = self.printer.lookup_object('gcode')
            reactor = self.printer.get_reactor()
            reactor.register_timer(self.keepalive_handler, reactor.NOW)

    def keepalive_handler(self, eventtime):
        toolhead_info = self.toolhead.get_status(eventtime)
        gcode_info = self.gcode.get_status(eventtime)

        if (self._counter >= self._interval):
            self.gcode.respond("busy: processing")
            self._counter = 0

        if (toolhead_info['status'] == 'Printing'
                and gcode_info['busy'] is True):
            self._counter += 1
        else:
            self._counter = 0

        return eventtime + 1.


def load_config(config):
    return HostKeepAlive(config)
