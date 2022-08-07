# A simple timer for executing gcode templates
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
from gcode_macro import GetStatusWrapper

class DelayedGcode:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[1]
        self.gcode = self.printer.lookup_object('gcode')
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.timer_gcode = gcode_macro.load_template(config, 'gcode')
        self.duration = config.getfloat('initial_duration', 0., minval=0.)
        self.timer_handler = None
        self.inside_timer = self.repeat = False
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        self.gcode.register_mux_command(
            "UPDATE_DELAYED_GCODE", "ID", self.name,
            self.cmd_UPDATE_DELAYED_GCODE,
            desc=self.cmd_UPDATE_DELAYED_GCODE_help)
        self.gcode.register_mux_command(
            "QUERY_DELAYED_GCODE", "ID", self.name,
            self.cmd_QUERY_DELAYED_GCODE,
            desc=self.cmd_QUERY_DELAYED_GCODE_help)
    def get_status(self, eventtime):
        remain_time = 0
        if self.duration > 0:
          calc_time = self.waketime - self.printer.get_reactor().monotonic()
          if calc_time > 0:
            remain_time = calc_time
        return {'remaining': remain_time}
    def _handle_ready(self):
        self.waketime = self.reactor.NEVER
        if self.duration:
            self.waketime = self.reactor.monotonic() + self.duration
        self.timer_handler = self.reactor.register_timer(
            self._gcode_timer_event, self.waketime)
    def _gcode_timer_event(self, eventtime):
        self.inside_timer = True
        try:
            self.gcode.run_script(self.timer_gcode.render())
        except Exception:
            logging.exception("Script running error")
        nextwake = self.reactor.NEVER
        if self.repeat:
            nextwake = eventtime + self.duration
        self.inside_timer = self.repeat = False
        return nextwake
    cmd_UPDATE_DELAYED_GCODE_help = "Update the duration of a delayed_gcode"
    def cmd_UPDATE_DELAYED_GCODE(self, gcmd):
        self.duration = gcmd.get_float('DURATION', minval=0.)
        if self.inside_timer:
            self.repeat = (self.duration != 0.)
        else:
            self.waketime = self.reactor.NEVER
            if self.duration:
                self.waketime = self.reactor.monotonic() + self.duration
            self.reactor.update_timer(self.timer_handler, self.waketime)
    cmd_QUERY_DELAYED_GCODE_help = "Returns the status of a delayed_gcode"
    def cmd_QUERY_DELAYED_GCODE(self, gcmd):
        remain_time = 0
        if self.duration > 0 and self.waketime:
            remain_time = self.waketime - self.printer.get_reactor().monotonic()
            if remain_time > 0:
              msg = "%s running, %.1f secs remaining" % (self.name, remain_time)
            else:
              msg = "%s not running" % (self.name)
        gcmd.respond_info(msg)

def load_config_prefix(config):
    return DelayedGcode(config)
