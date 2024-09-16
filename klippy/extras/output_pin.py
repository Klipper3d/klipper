# PWM and digital output pin handling
#
# Copyright (C) 2017-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, ast
from .display import display


######################################################################
# G-Code request queuing helper
######################################################################

PIN_MIN_TIME = 0.100

# Helper code to queue g-code requests
class GCodeRequestQueue:
    def __init__(self, config, mcu, callback):
        self.printer = printer = config.get_printer()
        self.mcu = mcu
        self.callback = callback
        self.rqueue = []
        self.next_min_flush_time = 0.
        self.toolhead = None
        mcu.register_flush_callback(self._flush_notification)
        printer.register_event_handler("klippy:connect", self._handle_connect)
    def _handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')
    def _flush_notification(self, print_time, clock):
        rqueue = self.rqueue
        while rqueue:
            next_time = max(rqueue[0][0], self.next_min_flush_time)
            if next_time > print_time:
                return
            # Skip requests that have been overridden with a following request
            pos = 0
            while pos + 1 < len(rqueue) and rqueue[pos + 1][0] <= next_time:
                pos += 1
            req_pt, req_val = rqueue[pos]
            # Invoke callback for the request
            min_wait = 0.
            ret = self.callback(next_time, req_val)
            if ret is not None:
                # Handle special cases
                action, min_wait = ret
                if action == "discard":
                    del rqueue[:pos+1]
                    continue
                if action == "delay":
                    pos -= 1
            del rqueue[:pos+1]
            self.next_min_flush_time = next_time + max(min_wait, PIN_MIN_TIME)
            # Ensure following queue items are flushed
            self.toolhead.note_mcu_movequeue_activity(self.next_min_flush_time)
    def _queue_request(self, print_time, value):
        self.rqueue.append((print_time, value))
        self.toolhead.note_mcu_movequeue_activity(print_time)
    def queue_gcode_request(self, value):
        self.toolhead.register_lookahead_callback(
            (lambda pt: self._queue_request(pt, value)))
    def send_async_request(self, print_time, value):
        while 1:
            next_time = max(print_time, self.next_min_flush_time)
            # Invoke callback for the request
            action, min_wait = "normal", 0.
            ret = self.callback(next_time, value)
            if ret is not None:
                # Handle special cases
                action, min_wait = ret
                if action == "discard":
                    break
            self.next_min_flush_time = next_time + max(min_wait, PIN_MIN_TIME)
            if action != "delay":
                break


######################################################################
# Template evaluation helper
######################################################################

# Time between each template update
RENDER_TIME = 0.500

# Main template evaluation code
class PrinterTemplateEvaluator:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.active_templates = {}
        self.render_timer = None
        # Load templates
        dtemplates = display.lookup_display_templates(config)
        self.templates = dtemplates.get_display_templates()
        gcode_macro = self.printer.load_object(config, "gcode_macro")
        self.create_template_context = gcode_macro.create_template_context
    def _activate_timer(self):
        if self.render_timer is not None or not self.active_templates:
            return
        reactor = self.printer.get_reactor()
        self.render_timer = reactor.register_timer(self._render, reactor.NOW)
    def _activate_template(self, callback, template, lparams, flush_callback):
        if template is not None:
            uid = (template,) + tuple(sorted(lparams.items()))
            self.active_templates[callback] = (
                uid, template, lparams, flush_callback)
            return
        if callback in self.active_templates:
            del self.active_templates[callback]
    def _render(self, eventtime):
        if not self.active_templates:
            # Nothing to do - unregister timer
            reactor = self.printer.get_reactor()
            reactor.unregister_timer(self.render_timer)
            self.render_timer = None
            return reactor.NEVER
        # Setup gcode_macro template context
        context = self.create_template_context(eventtime)
        def render(name, **kwargs):
            return self.templates[name].render(context, **kwargs)
        context['render'] = render
        # Render all templates
        flush_callbacks = {}
        rendered = {}
        template_info = self.active_templates.items()
        for callback, (uid, template, lparams, flush_callback) in template_info:
            text = rendered.get(uid)
            if text is None:
                try:
                    text = template.render(context, **lparams)
                except Exception as e:
                    logging.exception("display template render error")
                    text = ""
                rendered[uid] = text
            if flush_callback is not None:
                flush_callbacks[flush_callback] = 1
            callback(text)
        context.clear() # Remove circular references for better gc
        # Invoke optional flush callbacks
        for flush_callback in flush_callbacks.keys():
            flush_callback()
        return eventtime + RENDER_TIME
    def set_template(self, gcmd, callback, flush_callback=None):
        template = None
        lparams = {}
        tpl_name = gcmd.get("TEMPLATE")
        if tpl_name:
            template = self.templates.get(tpl_name)
            if template is None:
                raise gcmd.error("Unknown display_template '%s'" % (tpl_name,))
            tparams = template.get_params()
            for p, v in gcmd.get_command_parameters().items():
                if not p.startswith("PARAM_"):
                    continue
                p = p.lower()
                if p not in tparams:
                    raise gcmd.error("Invalid display_template parameter: %s"
                                     % (p,))
                try:
                    lparams[p] = ast.literal_eval(v)
                except ValueError as e:
                    raise gcmd.error("Unable to parse '%s' as a literal" % (v,))
        self._activate_template(callback, template, lparams, flush_callback)
        self._activate_timer()

def lookup_template_eval(config):
    printer = config.get_printer()
    te = printer.lookup_object("template_evaluator", None)
    if te is None:
        te = PrinterTemplateEvaluator(config)
        printer.add_object("template_evaluator", te)
    return te


######################################################################
# Main output pin handling
######################################################################

MAX_SCHEDULE_TIME = 5.0

class PrinterOutputPin:
    def __init__(self, config):
        self.printer = config.get_printer()
        ppins = self.printer.lookup_object('pins')
        # Determine pin type
        self.is_pwm = config.getboolean('pwm', False)
        if self.is_pwm:
            self.mcu_pin = ppins.setup_pin('pwm', config.get('pin'))
            cycle_time = config.getfloat('cycle_time', 0.100, above=0.,
                                         maxval=MAX_SCHEDULE_TIME)
            hardware_pwm = config.getboolean('hardware_pwm', False)
            self.mcu_pin.setup_cycle_time(cycle_time, hardware_pwm)
            self.scale = config.getfloat('scale', 1., above=0.)
        else:
            self.mcu_pin = ppins.setup_pin('digital_out', config.get('pin'))
            self.scale = 1.
        self.mcu_pin.setup_max_duration(0.)
        # Determine start and shutdown values
        self.last_value = config.getfloat(
            'value', 0., minval=0., maxval=self.scale) / self.scale
        self.shutdown_value = config.getfloat(
            'shutdown_value', 0., minval=0., maxval=self.scale) / self.scale
        self.mcu_pin.setup_start_value(self.last_value, self.shutdown_value)
        # Create gcode request queue
        self.gcrq = GCodeRequestQueue(config, self.mcu_pin.get_mcu(),
                                      self._set_pin)
        # Register commands
        pin_name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_PIN", "PIN", pin_name,
                                   self.cmd_SET_PIN,
                                   desc=self.cmd_SET_PIN_help)
    def get_status(self, eventtime):
        return {'value': self.last_value}
    def _set_pin(self, print_time, value):
        if value == self.last_value:
            return "discard", 0.
        self.last_value = value
        if self.is_pwm:
            self.mcu_pin.set_pwm(print_time, value)
        else:
            self.mcu_pin.set_digital(print_time, value)
    cmd_SET_PIN_help = "Set the value of an output pin"
    def cmd_SET_PIN(self, gcmd):
        # Read requested value
        value = gcmd.get_float('VALUE', minval=0., maxval=self.scale)
        value /= self.scale
        if not self.is_pwm and value not in [0., 1.]:
            raise gcmd.error("Invalid pin value")
        # Queue requested value
        self.gcrq.queue_gcode_request(value)

def load_config_prefix(config):
    return PrinterOutputPin(config)
