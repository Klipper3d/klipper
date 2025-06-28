# Queued PWM gpio output
#
# Copyright (C) 2017-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import chelper

class error(Exception):
    pass

class MCU_queued_pwm:
    def __init__(self, pin_params):
        self._mcu = pin_params['chip']
        self._hardware_pwm = False
        self._cycle_time = 0.100
        self._max_duration = 2.
        self._oid = self._mcu.create_oid()
        ffi_main, ffi_lib = chelper.get_ffi()
        self._stepqueue = ffi_main.gc(ffi_lib.stepcompress_alloc(self._oid),
                                      ffi_lib.stepcompress_free)
        self._mcu.register_stepqueue(self._stepqueue)
        self._stepcompress_queue_mq_msg = ffi_lib.stepcompress_queue_mq_msg
        self._mcu.register_config_callback(self._build_config)
        self._pin = pin_params['pin']
        self._invert = pin_params['invert']
        self._start_value = self._shutdown_value = float(self._invert)
        self._last_clock = self._last_value = self._default_value = 0
        self._duration_ticks = 0
        self._pwm_max = 0.
        self._set_cmd_tag = None
        self._toolhead = None
        printer = self._mcu.get_printer()
        printer.register_event_handler("klippy:connect", self._handle_connect)
    def _handle_connect(self):
        self._toolhead = self._mcu.get_printer().lookup_object("toolhead")
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_cycle_time(self, cycle_time, hardware_pwm=False):
        self._cycle_time = cycle_time
        self._hardware_pwm = hardware_pwm
    def setup_start_value(self, start_value, shutdown_value):
        if self._invert:
            start_value = 1. - start_value
            shutdown_value = 1. - shutdown_value
        self._start_value = max(0., min(1., start_value))
        self._shutdown_value = max(0., min(1., shutdown_value))
    def _build_config(self):
        config_error = self._mcu.get_printer().config_error
        if self._max_duration and self._start_value != self._shutdown_value:
            raise config_error("Pin with max duration must have start"
                               " value equal to shutdown value")
        cmd_queue = self._mcu.alloc_command_queue()
        curtime = self._mcu.get_printer().get_reactor().monotonic()
        printtime = self._mcu.estimated_print_time(curtime)
        self._last_clock = self._mcu.print_time_to_clock(printtime + 0.200)
        cycle_ticks = self._mcu.seconds_to_clock(self._cycle_time)
        if cycle_ticks >= 1<<31:
            raise config_error("PWM pin cycle time too large")
        self._duration_ticks = self._mcu.seconds_to_clock(self._max_duration)
        if self._duration_ticks >= 1<<31:
            raise config_error("PWM pin max duration too large")
        if self._duration_ticks:
            self._mcu.register_flush_callback(self._flush_notification)
        if self._hardware_pwm:
            self._pwm_max = self._mcu.get_constant_float("PWM_MAX")
            self._default_value = self._shutdown_value * self._pwm_max
            self._mcu.add_config_cmd(
                "config_pwm_out oid=%d pin=%s cycle_ticks=%d value=%d"
                " default_value=%d max_duration=%d"
                % (self._oid, self._pin, cycle_ticks,
                   self._start_value * self._pwm_max,
                   self._default_value, self._duration_ticks))
            self._last_value = int(self._start_value * self._pwm_max + 0.5)
            self._mcu.add_config_cmd("queue_pwm_out oid=%d clock=%d value=%d"
                                     % (self._oid, self._last_clock,
                                        self._last_value),
                                     on_restart=True)
            self._set_cmd_tag = self._mcu.lookup_command(
                "queue_pwm_out oid=%c clock=%u value=%hu",
                cq=cmd_queue).get_command_tag()
            return
        # Software PWM
        if self._shutdown_value not in [0., 1.]:
            raise config_error("shutdown value must be 0.0 or 1.0 on soft pwm")
        self._mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s value=%d"
            " default_value=%d max_duration=%d"
            % (self._oid, self._pin, self._start_value >= 1.0,
               self._shutdown_value >= 0.5, self._duration_ticks))
        self._default_value = int(self._shutdown_value >= 0.5) * cycle_ticks
        self._mcu.add_config_cmd(
            "set_digital_out_pwm_cycle oid=%d cycle_ticks=%d"
            % (self._oid, cycle_ticks))
        self._pwm_max = float(cycle_ticks)
        self._last_value = int(self._start_value * self._pwm_max + 0.5)
        self._mcu.add_config_cmd(
            "queue_digital_out oid=%d clock=%d on_ticks=%d"
            % (self._oid, self._last_clock, self._last_value), is_init=True)
        self._set_cmd_tag = self._mcu.lookup_command(
            "queue_digital_out oid=%c clock=%u on_ticks=%u",
            cq=cmd_queue).get_command_tag()
    def _send_update(self, clock, val):
        self._last_clock = clock = max(self._last_clock, clock)
        self._last_value = val
        data = (self._set_cmd_tag, self._oid, clock & 0xffffffff, val)
        ret = self._stepcompress_queue_mq_msg(self._stepqueue, clock,
                                              data, len(data))
        if ret:
            raise error("Internal error in stepcompress")
        # Notify toolhead so that it will flush this update
        wakeclock = clock
        if self._last_value != self._default_value:
            # Continue flushing to resend time
            wakeclock += self._duration_ticks
        wake_print_time = self._mcu.clock_to_print_time(wakeclock)
        self._toolhead.note_mcu_movequeue_activity(wake_print_time)
    def set_pwm(self, print_time, value):
        clock = self._mcu.print_time_to_clock(print_time)
        if self._invert:
            value = 1. - value
        v = int(max(0., min(1., value)) * self._pwm_max + 0.5)
        self._send_update(clock, v)
    def _flush_notification(self, print_time, clock):
        if self._last_value != self._default_value:
            while clock >= self._last_clock + self._duration_ticks:
                self._send_update(self._last_clock + self._duration_ticks,
                                  self._last_value)

class PrinterOutputPin:
    def __init__(self, config):
        self.printer = config.get_printer()
        ppins = self.printer.lookup_object('pins')
        # Determine pin type
        pin_params = ppins.lookup_pin(config.get('pin'), can_invert=True)
        self.mcu_pin = MCU_queued_pwm(pin_params)
        max_duration = self.mcu_pin.get_mcu().max_nominal_duration()
        cycle_time = config.getfloat('cycle_time', 0.100, above=0.,
                                     maxval=max_duration)
        hardware_pwm = config.getboolean('hardware_pwm', False)
        self.mcu_pin.setup_cycle_time(cycle_time, hardware_pwm)
        self.scale = config.getfloat('scale', 1., above=0.)
        self.last_print_time = 0.
        # Support mcu checking for maximum duration
        max_mcu_duration = config.getfloat('maximum_mcu_duration', 0.,
                                           minval=0.500, maxval=max_duration)
        self.mcu_pin.setup_max_duration(max_mcu_duration)
        # Determine start and shutdown values
        self.last_value = config.getfloat(
            'value', 0., minval=0., maxval=self.scale) / self.scale
        self.shutdown_value = config.getfloat(
            'shutdown_value', 0., minval=0., maxval=self.scale) / self.scale
        self.mcu_pin.setup_start_value(self.last_value, self.shutdown_value)
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
            return
        print_time = max(print_time, self.last_print_time)
        self.mcu_pin.set_pwm(print_time, value)
        self.last_value = value
        self.last_print_time = print_time
    cmd_SET_PIN_help = "Set the value of an output pin"
    def cmd_SET_PIN(self, gcmd):
        # Read requested value
        value = gcmd.get_float('VALUE', minval=0., maxval=self.scale)
        value /= self.scale
        # Obtain print_time and apply requested settings
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda print_time: self._set_pin(print_time, value))

def load_config_prefix(config):
    return PrinterOutputPin(config)
