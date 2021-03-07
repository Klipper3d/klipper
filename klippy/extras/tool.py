# Printer cooling fan
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PWM_tool:
    def __init__(self, config, default_shutdown_value=0.):
        self.printer = config.get_printer()
        self.last_value = 0.
        self.last_time = 0.
        self.safety_timeout = config.getfloat('safety_timeout', 5,
                                                minval=0.)
        self.reactor = self.printer.get_reactor()
        self.resend_timer = self.reactor.register_timer(
            self._resend_current_val)
        # Read config
        self.max_value = config.getfloat('max_power', 1., above=0., maxval=1.)
        self.off_below = config.getfloat('off_below', default=0.,
                                         minval=0., maxval=1.)
        cycle_time = config.getfloat('cycle_time', 0.01, above=0.)
        hardware_pwm = config.getboolean('hardware_pwm', True)
        shutdown_value = config.getfloat(
            'shutdown_value', default_shutdown_value, minval=0., maxval=1.)
        # Setup pwm object
        ppins = self.printer.lookup_object('pins')
        self.mcu_pwm = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu_pwm.setup_max_duration(self.safety_timeout)
        self.mcu_pwm.setup_cycle_time(cycle_time, hardware_pwm)
        self.shutdown_value = max(0., min(self.max_value, shutdown_value))
        self.mcu_pwm.setup_start_value(0., shutdown_value)
        # Register callbacks
        self.printer.register_event_handler("gcode:request_restart",
                                            self._handle_request_restart)
    def get_mcu(self):
        return self.mcu_pwm.get_mcu()
    def set_value(self, print_time, value, resend=False):
        if value < self.off_below:
            value = 0.
        value = max(0., min(self.max_value, value * self.max_value))
        if value == self.last_value and not resend:
            return
        print_time = max(self.last_time, print_time)
        self.last_time = print_time
        self.last_value = value
        self.mcu_pwm.set_pwm(print_time, value)
        if self.safety_timeout != 0 and not resend:
            if value == self.shutdown_value:
                self.reactor.update_timer(
                    self.resend_timer, self.reactor.NEVER)
            else:
                self.reactor.update_timer(
                    self.resend_timer,
                    self.reactor.monotonic() + 0.75 * self.safety_timeout)
    def set_value_from_command(self, value):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback((lambda pt:
                                              self.set_value(pt, value)))
    def _handle_request_restart(self, print_time):
        self.set_value(print_time, 0.)
    def get_status(self, eventtime):
        return {'value': self.last_value}
    def _resend_current_val(self, eventtime):
        # TODO: Split moves into smaller segments to enforce resend
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback((lambda pt:
                              self.set_value(pt, self.last_value, True)))
        if self.last_value != self.shutdown_value:
            return eventtime + 0.75 * self.safety_timeout
        return self.reactor.NEVER

class PrinterSpindle:
    def __init__(self, config):
        self.tool = PWM_tool(config)
        # Register commands
        gcode = config.get_printer().lookup_object('gcode')
        gcode.register_command("M3", self.cmd_M3_M4)
        gcode.register_command("M4", self.cmd_M3_M4)
        gcode.register_command("M5", self.cmd_M5)
    def get_status(self, eventtime):
        return self.tool.get_status(eventtime)
    def cmd_M3_M4(self, gcmd):
        # Set spindle speed
        value = gcmd.get_float('S', 255., minval=0.) / 255.
        self.tool.set_value_from_command(value)
    def cmd_M5(self, gcmd):
        # Turn spindle off
        self.tool.set_value_from_command(0.)

def load_config(config):
    return PrinterSpindle(config)
