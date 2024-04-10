# Support for executing gcode when a hardware button is pressed or released.
#
# Copyright (C) 2019 Alec Plumb <alec@etherwalker.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class GCodeButton:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split(' ')[-1]
        self.pin = config.get('pin')
        self.last_state = 0
        self.template_queue = []
        buttons = self.printer.try_load_module(config, "buttons")
        buttons.register_buttons([self.pin], self.button_callback)
        gcode_macro = self.printer.try_load_module(config, 'gcode_macro')
        self.press_template = gcode_macro.load_template(config, 'press_gcode')
        self.release_template = gcode_macro.load_template(
                                        config, 'release_gcode')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("QUERY_BUTTON", "BUTTON", self.name,
                                        self.cmd_QUERY_BUTTON,
                                        desc=self.cmd_QUERY_BUTTON_help)

    cmd_QUERY_BUTTON_help = "Report on the state of a button"

    def cmd_QUERY_BUTTON(self, params):
        self.gcode.respond(self.name + ": " + self.get_status()['state'])

    def button_callback(self, eventtime, state):
        self.last_state = state
        if state and bool(self.press_template):
           self.queue_template(self.press_template)
        if (not state) and bool(self.release_template):
           self.queue_template(self.release_template)

    def get_status(self, eventtime=None):
        if self.last_state:
            return {'state': "PRESSED"}
        return {'state': "RELEASED"}

    def queue_template(self, template):
        if template is None:
            return
        if not self.template_queue:
            reactor = self.printer.get_reactor()
            reactor.register_callback(self.dispatch_templates)
        self.template_queue.append(template)

    def dispatch_templates(self, eventtime):
        while self.template_queue:
            template = self.template_queue[0]
            try:
                self.gcode.run_script(template.render())
            except Exception:
                logging.exception("Script running error")
            self.template_queue.pop(0)

def load_config_prefix(config):
    return GCodeButton(config)
