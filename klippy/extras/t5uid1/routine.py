# Routine class
#
# Copyright (C) 2020  Desuuuu <contact@desuuuu.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

TRIGGERS = [
    "enter_pre",
    "enter",
    "leave",
    "manual"
]

class T5UID1_Routine:
    def __init__(self, gcode_macro, context, page_names, config):
        self.printer = config.get_printer()

        self.gcode = self.printer.lookup_object('gcode')
        self.reactor = self.printer.get_reactor()

        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        self.name = name_parts[1]

        self.trigger = config.get('trigger')
        if self.trigger not in TRIGGERS:
            raise config.error("Invalid trigger '%s' in section '%s'"
                               % (self.trigger, config.get_name()))

        if self.trigger != "manual":
            self.page = config.get('page')
            if self.page not in page_names:
                raise config.error("Invalid page '%s' in section '%s'"
                                   % (self.page, config.get_name()))
        else:
            self.page = None

        if self.trigger != "enter_pre":
            self.delay = config.getint('delay', None, minval=0, maxval=60)
        else:
            self.delay = None

        if self.trigger in ["enter", "manual"]:
            self.interval = config.getint('interval', 0, minval=0, maxval=60)
        else:
            self.interval = 0

        self._template = gcode_macro.load_template(config, 'script')
        self._context = context
        self.run_as_gcode = config.getboolean('run_as_gcode', False)

        self._should_stop = False
        self._timer = self.reactor.register_timer(self._timer_run)

    def _timer_run(self, eventtime):
        if self._should_stop:
            return self.reactor.NEVER
        self.run(is_timer=True)
        if self._should_stop or self.interval <= 0:
            return self.reactor.NEVER
        return eventtime + self.interval

    def run(self, repeat=True, is_timer=False):
        if not is_timer:
            self._should_stop = False
            if self.delay is not None:
                if self.delay > 0:
                    self.reactor.update_timer(self._timer,
                                              self.reactor.monotonic()
                                                  + self.delay)
                else:
                    self.reactor.update_timer(self._timer, self.reactor.NOW)
                return

        context = self._template.create_template_context()
        context['is_timer'] = is_timer
        context.update(self._context)

        result = self._template.render(context).strip()
        if self.run_as_gcode and len(result) > 0:
            self.gcode.run_script_from_command(result)

        if not is_timer and self.interval > 0:
            if repeat:
                next_run = self.reactor.monotonic() + self.interval
            else:
                next_run = self.reactor.NEVER
            self.reactor.update_timer(self._timer, next_run)
            return

        if self.trigger == "enter_pre":
            if len(result) == 0:
                return True
            commands = [ c.strip() for c in result.split('\n') ]
            return not "DGUS_ABORT_PAGE_SWITCH" in commands

    def stop(self):
        self._should_stop = True
        self.reactor.update_timer(self._timer, self.reactor.NEVER)
