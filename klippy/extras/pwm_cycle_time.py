# Handle pwm output pins with variable frequency
#
# Copyright (C) 2017-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class MCU_pwm_cycle:
    def __init__(self, pin_params, cycle_time, start_value, shutdown_value):
        self._mcu = pin_params['chip']
        self._cycle_time = cycle_time
        self._oid = None
        self._mcu.register_config_callback(self._build_config)
        self._pin = pin_params['pin']
        self._invert = pin_params['invert']
        if self._invert:
            start_value = 1. - start_value
            shutdown_value = 1. - shutdown_value
        self._start_value = max(0., min(1., start_value))
        self._shutdown_value = max(0., min(1., shutdown_value))
        self._last_clock = self._cycle_ticks = 0
        self._set_cmd = self._set_cycle_ticks = None
    def get_mcu(self):
        return self._mcu
    def _build_config(self):
        cmd_queue = self._mcu.alloc_command_queue()
        curtime = self._mcu.get_printer().get_reactor().monotonic()
        printtime = self._mcu.estimated_print_time(curtime)
        self._last_clock = self._mcu.print_time_to_clock(printtime + 0.200)
        cycle_ticks = self._mcu.seconds_to_clock(self._cycle_time)
        if self._shutdown_value not in [0., 1.]:
            raise self._mcu.get_printer().config_error(
                "shutdown value must be 0.0 or 1.0 on soft pwm")
        if cycle_ticks >= 1<<31:
            raise self._mcu.get_printer().config_error(
                "PWM pin cycle time too large")
        self._mcu.request_move_queue_slot()
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s value=%d"
            " default_value=%d max_duration=%d"
            % (self._oid, self._pin, self._start_value >= 1.0,
               self._shutdown_value >= 0.5, 0))
        self._mcu.add_config_cmd(
            "set_digital_out_pwm_cycle oid=%d cycle_ticks=%d"
            % (self._oid, cycle_ticks))
        self._cycle_ticks = cycle_ticks
        svalue = int(self._start_value * cycle_ticks + 0.5)
        self._mcu.add_config_cmd(
            "queue_digital_out oid=%d clock=%d on_ticks=%d"
            % (self._oid, self._last_clock, svalue), is_init=True)
        self._set_cmd = self._mcu.lookup_command(
            "queue_digital_out oid=%c clock=%u on_ticks=%u", cq=cmd_queue)
        self._set_cycle_ticks = self._mcu.lookup_command(
            "set_digital_out_pwm_cycle oid=%c cycle_ticks=%u", cq=cmd_queue)
    def set_pwm_cycle(self, print_time, value, cycle_time):
        clock = self._mcu.print_time_to_clock(print_time)
        minclock = self._last_clock
        # Send updated cycle_time if necessary
        cycle_ticks = self._mcu.seconds_to_clock(cycle_time)
        if cycle_ticks != self._cycle_ticks:
            if cycle_ticks >= 1<<31:
                raise self._mcu.get_printer().command_error(
                    "PWM cycle time too large")
            self._set_cycle_ticks.send([self._oid, cycle_ticks],
                                       minclock=minclock, reqclock=clock)
            self._cycle_ticks = cycle_ticks
        # Send pwm update
        if self._invert:
            value = 1. - value
        v = int(max(0., min(1., value)) * float(self._cycle_ticks) + 0.5)
        self._set_cmd.send([self._oid, clock, v],
                           minclock=self._last_clock, reqclock=clock)
        self._last_clock = clock

class PrinterOutputPWMCycle:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.last_print_time = 0.
        # Determine start and shutdown values
        self.scale = config.getfloat('scale', 1., above=0.)
        self.last_value = config.getfloat(
            'value', 0., minval=0., maxval=self.scale) / self.scale
        self.shutdown_value = config.getfloat(
            'shutdown_value', 0., minval=0., maxval=self.scale) / self.scale
        # Create pwm pin object
        ppins = self.printer.lookup_object('pins')
        pin_params = ppins.lookup_pin(config.get('pin'), can_invert=True)
        max_duration = pin_params['chip'].max_nominal_duration()
        cycle_time = config.getfloat('cycle_time', 0.100, above=0.,
                                     maxval=max_duration)
        self.mcu_pin = MCU_pwm_cycle(pin_params, cycle_time,
                                     self.last_value, self.shutdown_value)
        self.last_cycle_time = self.default_cycle_time = cycle_time
        # Register commands
        pin_name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_PIN", "PIN", pin_name,
                                   self.cmd_SET_PIN,
                                   desc=self.cmd_SET_PIN_help)
    def get_status(self, eventtime):
        return {'value': self.last_value}
    def _set_pin(self, print_time, value, cycle_time):
        if value == self.last_value and cycle_time == self.last_cycle_time:
            return
        min_sched_time = self.mcu_pin.get_mcu().min_schedule_time()
        print_time = max(print_time, self.last_print_time + min_sched_time)
        self.mcu_pin.set_pwm_cycle(print_time, value, cycle_time)
        self.last_value = value
        self.last_cycle_time = cycle_time
        self.last_print_time = print_time
    cmd_SET_PIN_help = "Set the value of an output pin"
    def cmd_SET_PIN(self, gcmd):
        # Read requested value
        value = gcmd.get_float('VALUE', minval=0., maxval=self.scale)
        value /= self.scale
        max_duration = self.mcu_pin.get_mcu().max_nominal_duration()
        cycle_time = gcmd.get_float('CYCLE_TIME', self.default_cycle_time,
                                    above=0., maxval=max_duration)
        # Obtain print_time and apply requested settings
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda print_time: self._set_pin(print_time, value, cycle_time))

def load_config_prefix(config):
    return PrinterOutputPWMCycle(config)
