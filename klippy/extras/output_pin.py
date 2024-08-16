# PWM and digital output pin handling
#
# Copyright (C) 2017-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

PIN_MIN_TIME = 0.100
MAX_SCHEDULE_TIME = 5.0

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
        if not rqueue:
            return
        next_time = max(rqueue[0][0], self.next_min_flush_time)
        if next_time > print_time:
            return
        # Skip requests that have been overridden with a following request
        pos = 0
        while pos + 1 < len(rqueue) and rqueue[pos + 1][0] <= next_time:
            pos += 1
        req_pt, req_val = rqueue[pos]
        # Invoke callback for the request
        want_dequeue, min_wait_time = self.callback(next_time, req_val)
        self.next_min_flush_time = next_time + max(min_wait_time, PIN_MIN_TIME)
        if want_dequeue:
            pos += 1
        del rqueue[:pos]
        # Ensure following queue items are flushed
        self.toolhead.note_mcu_movequeue_activity(self.next_min_flush_time)
    def queue_request(self, print_time, value):
        self.rqueue.append((print_time, value))
        self.toolhead.note_mcu_movequeue_activity(print_time)
    def queue_gcode_request(self, value):
        self.toolhead.register_lookahead_callback(
            (lambda pt: self.queue_request(pt, value)))

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
        if value != self.last_value:
            self.last_value = value
            if self.is_pwm:
                self.mcu_pin.set_pwm(print_time, value)
            else:
                self.mcu_pin.set_digital(print_time, value)
        return (True, 0.)
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
