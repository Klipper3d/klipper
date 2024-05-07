# Interface to Klipper micro-controller code
#
# Copyright (C) 2016-2023  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, zlib, logging, math
import serialhdl, msgproto, pins, chelper, clocksync

class error(Exception):
    pass


######################################################################
# Command transmit helper classes
######################################################################

# Class to retry sending of a query command until a given response is received
class RetryAsyncCommand:
    TIMEOUT_TIME = 5.0
    RETRY_TIME = 0.500
    def __init__(self, serial, name, oid=None):
        self.serial = serial
        self.name = name
        self.oid = oid
        self.reactor = serial.get_reactor()
        self.completion = self.reactor.completion()
        self.min_query_time = self.reactor.monotonic()
        self.need_response = True
        self.serial.register_response(self.handle_callback, name, oid)
    def handle_callback(self, params):
        if self.need_response and params['#sent_time'] >= self.min_query_time:
            self.need_response = False
            self.reactor.async_complete(self.completion, params)
    def get_response(self, cmds, cmd_queue, minclock=0, reqclock=0):
        cmd, = cmds
        self.serial.raw_send_wait_ack(cmd, minclock, reqclock, cmd_queue)
        self.min_query_time = 0.
        first_query_time = query_time = self.reactor.monotonic()
        while 1:
            params = self.completion.wait(query_time + self.RETRY_TIME)
            if params is not None:
                self.serial.register_response(None, self.name, self.oid)
                return params
            query_time = self.reactor.monotonic()
            if query_time > first_query_time + self.TIMEOUT_TIME:
                self.serial.register_response(None, self.name, self.oid)
                raise serialhdl.error("Timeout on wait for '%s' response"
                                      % (self.name,))
            self.serial.raw_send(cmd, minclock, minclock, cmd_queue)

# Wrapper around query commands
class CommandQueryWrapper:
    def __init__(self, serial, msgformat, respformat, oid=None,
                 cmd_queue=None, is_async=False, error=serialhdl.error):
        self._serial = serial
        self._cmd = serial.get_msgparser().lookup_command(msgformat)
        serial.get_msgparser().lookup_command(respformat)
        self._response = respformat.split()[0]
        self._oid = oid
        self._error = error
        self._xmit_helper = serialhdl.SerialRetryCommand
        if is_async:
            self._xmit_helper = RetryAsyncCommand
        if cmd_queue is None:
            cmd_queue = serial.get_default_command_queue()
        self._cmd_queue = cmd_queue
    def _do_send(self, cmds, minclock, reqclock):
        xh = self._xmit_helper(self._serial, self._response, self._oid)
        reqclock = max(minclock, reqclock)
        try:
            return xh.get_response(cmds, self._cmd_queue, minclock, reqclock)
        except serialhdl.error as e:
            raise self._error(str(e))
    def send(self, data=(), minclock=0, reqclock=0):
        return self._do_send([self._cmd.encode(data)], minclock, reqclock)
    def send_with_preface(self, preface_cmd, preface_data=(), data=(),
                          minclock=0, reqclock=0):
        cmds = [preface_cmd._cmd.encode(preface_data), self._cmd.encode(data)]
        return self._do_send(cmds, minclock, reqclock)

# Wrapper around command sending
class CommandWrapper:
    def __init__(self, serial, msgformat, cmd_queue=None):
        self._serial = serial
        msgparser = serial.get_msgparser()
        self._cmd = msgparser.lookup_command(msgformat)
        if cmd_queue is None:
            cmd_queue = serial.get_default_command_queue()
        self._cmd_queue = cmd_queue
        self._msgtag = msgparser.lookup_msgtag(msgformat) & 0xffffffff
    def send(self, data=(), minclock=0, reqclock=0):
        cmd = self._cmd.encode(data)
        self._serial.raw_send(cmd, minclock, reqclock, self._cmd_queue)
    def send_wait_ack(self, data=(), minclock=0, reqclock=0):
        cmd = self._cmd.encode(data)
        self._serial.raw_send_wait_ack(cmd, minclock, reqclock, self._cmd_queue)
    def get_command_tag(self):
        return self._msgtag


######################################################################
# Wrapper classes for MCU pins
######################################################################

class MCU_trsync:
    REASON_ENDSTOP_HIT = 1
    REASON_COMMS_TIMEOUT = 2
    REASON_HOST_REQUEST = 3
    REASON_PAST_END_TIME = 4
    def __init__(self, mcu, trdispatch):
        self._mcu = mcu
        self._trdispatch = trdispatch
        self._reactor = mcu.get_printer().get_reactor()
        self._steppers = []
        self._trdispatch_mcu = None
        self._oid = mcu.create_oid()
        self._cmd_queue = mcu.alloc_command_queue()
        self._trsync_start_cmd = self._trsync_set_timeout_cmd = None
        self._trsync_trigger_cmd = self._trsync_query_cmd = None
        self._stepper_stop_cmd = None
        self._trigger_completion = None
        self._home_end_clock = None
        mcu.register_config_callback(self._build_config)
        printer = mcu.get_printer()
        printer.register_event_handler("klippy:shutdown", self._shutdown)
    def get_mcu(self):
        return self._mcu
    def get_oid(self):
        return self._oid
    def get_command_queue(self):
        return self._cmd_queue
    def add_stepper(self, stepper):
        if stepper in self._steppers:
            return
        self._steppers.append(stepper)
    def get_steppers(self):
        return list(self._steppers)
    def _build_config(self):
        mcu = self._mcu
        # Setup config
        mcu.add_config_cmd("config_trsync oid=%d" % (self._oid,))
        mcu.add_config_cmd(
            "trsync_start oid=%d report_clock=0 report_ticks=0 expire_reason=0"
            % (self._oid,), on_restart=True)
        # Lookup commands
        self._trsync_start_cmd = mcu.lookup_command(
            "trsync_start oid=%c report_clock=%u report_ticks=%u"
            " expire_reason=%c", cq=self._cmd_queue)
        self._trsync_set_timeout_cmd = mcu.lookup_command(
            "trsync_set_timeout oid=%c clock=%u", cq=self._cmd_queue)
        self._trsync_trigger_cmd = mcu.lookup_command(
            "trsync_trigger oid=%c reason=%c", cq=self._cmd_queue)
        self._trsync_query_cmd = mcu.lookup_query_command(
            "trsync_trigger oid=%c reason=%c",
            "trsync_state oid=%c can_trigger=%c trigger_reason=%c clock=%u",
            oid=self._oid, cq=self._cmd_queue)
        self._stepper_stop_cmd = mcu.lookup_command(
            "stepper_stop_on_trigger oid=%c trsync_oid=%c", cq=self._cmd_queue)
        # Create trdispatch_mcu object
        set_timeout_tag = mcu.lookup_command(
            "trsync_set_timeout oid=%c clock=%u").get_command_tag()
        trigger_cmd = mcu.lookup_command("trsync_trigger oid=%c reason=%c")
        trigger_tag = trigger_cmd.get_command_tag()
        state_cmd = mcu.lookup_command(
            "trsync_state oid=%c can_trigger=%c trigger_reason=%c clock=%u")
        state_tag = state_cmd.get_command_tag()
        ffi_main, ffi_lib = chelper.get_ffi()
        self._trdispatch_mcu = ffi_main.gc(ffi_lib.trdispatch_mcu_alloc(
            self._trdispatch, mcu._serial.get_serialqueue(), # XXX
            self._cmd_queue, self._oid, set_timeout_tag, trigger_tag,
            state_tag), ffi_lib.free)
    def _shutdown(self):
        tc = self._trigger_completion
        if tc is not None:
            self._trigger_completion = None
            tc.complete(False)
    def _handle_trsync_state(self, params):
        if not params['can_trigger']:
            tc = self._trigger_completion
            if tc is not None:
                self._trigger_completion = None
                reason = params['trigger_reason']
                is_failure = (reason == self.REASON_COMMS_TIMEOUT)
                self._reactor.async_complete(tc, is_failure)
        elif self._home_end_clock is not None:
            clock = self._mcu.clock32_to_clock64(params['clock'])
            if clock >= self._home_end_clock:
                self._home_end_clock = None
                self._trsync_trigger_cmd.send([self._oid,
                                               self.REASON_PAST_END_TIME])
    def start(self, print_time, report_offset,
              trigger_completion, expire_timeout):
        self._trigger_completion = trigger_completion
        self._home_end_clock = None
        clock = self._mcu.print_time_to_clock(print_time)
        expire_ticks = self._mcu.seconds_to_clock(expire_timeout)
        expire_clock = clock + expire_ticks
        report_ticks = self._mcu.seconds_to_clock(expire_timeout * .3)
        report_clock = clock + int(report_ticks * report_offset + .5)
        min_extend_ticks = int(report_ticks * .8 + .5)
        ffi_main, ffi_lib = chelper.get_ffi()
        ffi_lib.trdispatch_mcu_setup(self._trdispatch_mcu, clock, expire_clock,
                                     expire_ticks, min_extend_ticks)
        self._mcu.register_response(self._handle_trsync_state,
                                    "trsync_state", self._oid)
        self._trsync_start_cmd.send([self._oid, report_clock, report_ticks,
                                     self.REASON_COMMS_TIMEOUT],
                                    reqclock=report_clock)
        for s in self._steppers:
            self._stepper_stop_cmd.send([s.get_oid(), self._oid])
        self._trsync_set_timeout_cmd.send([self._oid, expire_clock],
                                          reqclock=expire_clock)
    def set_home_end_time(self, home_end_time):
        self._home_end_clock = self._mcu.print_time_to_clock(home_end_time)
    def stop(self):
        self._mcu.register_response(None, "trsync_state", self._oid)
        self._trigger_completion = None
        if self._mcu.is_fileoutput():
            return self.REASON_ENDSTOP_HIT
        params = self._trsync_query_cmd.send([self._oid,
                                              self.REASON_HOST_REQUEST])
        for s in self._steppers:
            s.note_homing_end()
        return params['trigger_reason']

TRSYNC_TIMEOUT = 0.025
TRSYNC_SINGLE_MCU_TIMEOUT = 0.250

class TriggerDispatch:
    def __init__(self, mcu):
        self._mcu = mcu
        self._trigger_completion = None
        ffi_main, ffi_lib = chelper.get_ffi()
        self._trdispatch = ffi_main.gc(ffi_lib.trdispatch_alloc(), ffi_lib.free)
        self._trsyncs = [MCU_trsync(mcu, self._trdispatch)]
    def get_oid(self):
        return self._trsyncs[0].get_oid()
    def get_command_queue(self):
        return self._trsyncs[0].get_command_queue()
    def add_stepper(self, stepper):
        trsyncs = {trsync.get_mcu(): trsync for trsync in self._trsyncs}
        trsync = trsyncs.get(stepper.get_mcu())
        if trsync is None:
            trsync = MCU_trsync(stepper.get_mcu(), self._trdispatch)
            self._trsyncs.append(trsync)
        trsync.add_stepper(stepper)
        # Check for unsupported multi-mcu shared stepper rails
        sname = stepper.get_name()
        if sname.startswith('stepper_'):
            for ot in self._trsyncs:
                for s in ot.get_steppers():
                    if ot is not trsync and s.get_name().startswith(sname[:9]):
                        cerror = self._mcu.get_printer().config_error
                        raise cerror("Multi-mcu homing not supported on"
                                     " multi-mcu shared axis")
    def get_steppers(self):
        return [s for trsync in self._trsyncs for s in trsync.get_steppers()]
    def start(self, print_time):
        reactor = self._mcu.get_printer().get_reactor()
        self._trigger_completion = reactor.completion()
        expire_timeout = TRSYNC_TIMEOUT
        if len(self._trsyncs) == 1:
            expire_timeout = TRSYNC_SINGLE_MCU_TIMEOUT
        for i, trsync in enumerate(self._trsyncs):
            report_offset = float(i) / len(self._trsyncs)
            trsync.start(print_time, report_offset,
                         self._trigger_completion, expire_timeout)
        etrsync = self._trsyncs[0]
        ffi_main, ffi_lib = chelper.get_ffi()
        ffi_lib.trdispatch_start(self._trdispatch, etrsync.REASON_HOST_REQUEST)
        return self._trigger_completion
    def wait_end(self, end_time):
        etrsync = self._trsyncs[0]
        etrsync.set_home_end_time(end_time)
        if self._mcu.is_fileoutput():
            self._trigger_completion.complete(True)
        self._trigger_completion.wait()
    def stop(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        ffi_lib.trdispatch_stop(self._trdispatch)
        res = [trsync.stop() for trsync in self._trsyncs]
        if any([r == MCU_trsync.REASON_COMMS_TIMEOUT for r in res]):
            return MCU_trsync.REASON_COMMS_TIMEOUT
        return res[0]

class MCU_endstop:
    def __init__(self, mcu, pin_params):
        self._mcu = mcu
        self._pin = pin_params['pin']
        self._pullup = pin_params['pullup']
        self._invert = pin_params['invert']
        self._oid = self._mcu.create_oid()
        self._home_cmd = self._query_cmd = None
        self._mcu.register_config_callback(self._build_config)
        self._rest_ticks = 0
        self._dispatch = TriggerDispatch(mcu)
    def get_mcu(self):
        return self._mcu
    def add_stepper(self, stepper):
        self._dispatch.add_stepper(stepper)
    def get_steppers(self):
        return self._dispatch.get_steppers()
    def _build_config(self):
        # Setup config
        self._mcu.add_config_cmd("config_endstop oid=%d pin=%s pull_up=%d"
                                 % (self._oid, self._pin, self._pullup))
        self._mcu.add_config_cmd(
            "endstop_home oid=%d clock=0 sample_ticks=0 sample_count=0"
            " rest_ticks=0 pin_value=0 trsync_oid=0 trigger_reason=0"
            % (self._oid,), on_restart=True)
        # Lookup commands
        cmd_queue = self._dispatch.get_command_queue()
        self._home_cmd = self._mcu.lookup_command(
            "endstop_home oid=%c clock=%u sample_ticks=%u sample_count=%c"
            " rest_ticks=%u pin_value=%c trsync_oid=%c trigger_reason=%c",
            cq=cmd_queue)
        self._query_cmd = self._mcu.lookup_query_command(
            "endstop_query_state oid=%c",
            "endstop_state oid=%c homing=%c next_clock=%u pin_value=%c",
            oid=self._oid, cq=cmd_queue)
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        clock = self._mcu.print_time_to_clock(print_time)
        rest_ticks = self._mcu.print_time_to_clock(print_time+rest_time) - clock
        self._rest_ticks = rest_ticks
        trigger_completion = self._dispatch.start(print_time)
        self._home_cmd.send(
            [self._oid, clock, self._mcu.seconds_to_clock(sample_time),
             sample_count, rest_ticks, triggered ^ self._invert,
             self._dispatch.get_oid(), MCU_trsync.REASON_ENDSTOP_HIT],
            reqclock=clock)
        return trigger_completion
    def home_wait(self, home_end_time):
        self._dispatch.wait_end(home_end_time)
        self._home_cmd.send([self._oid, 0, 0, 0, 0, 0, 0, 0])
        res = self._dispatch.stop()
        if res == MCU_trsync.REASON_COMMS_TIMEOUT:
            return -1.
        if res != MCU_trsync.REASON_ENDSTOP_HIT:
            return 0.
        if self._mcu.is_fileoutput():
            return home_end_time
        params = self._query_cmd.send([self._oid])
        next_clock = self._mcu.clock32_to_clock64(params['next_clock'])
        return self._mcu.clock_to_print_time(next_clock - self._rest_ticks)
    def query_endstop(self, print_time):
        clock = self._mcu.print_time_to_clock(print_time)
        if self._mcu.is_fileoutput():
            return 0
        params = self._query_cmd.send([self._oid], minclock=clock)
        return params['pin_value'] ^ self._invert

class MCU_digital_out:
    def __init__(self, mcu, pin_params):
        self._mcu = mcu
        self._oid = None
        self._mcu.register_config_callback(self._build_config)
        self._pin = pin_params['pin']
        self._invert = pin_params['invert']
        self._start_value = self._shutdown_value = self._invert
        self._max_duration = 2.
        self._last_clock = 0
        self._set_cmd = None
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_start_value(self, start_value, shutdown_value):
        self._start_value = (not not start_value) ^ self._invert
        self._shutdown_value = (not not shutdown_value) ^ self._invert
    def _build_config(self):
        if self._max_duration and self._start_value != self._shutdown_value:
            raise pins.error("Pin with max duration must have start"
                             " value equal to shutdown value")
        mdur_ticks = self._mcu.seconds_to_clock(self._max_duration)
        if mdur_ticks >= 1<<31:
            raise pins.error("Digital pin max duration too large")
        self._mcu.request_move_queue_slot()
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s value=%d default_value=%d"
            " max_duration=%d" % (self._oid, self._pin, self._start_value,
                                  self._shutdown_value, mdur_ticks))
        self._mcu.add_config_cmd("update_digital_out oid=%d value=%d"
                                 % (self._oid, self._start_value),
                                 on_restart=True)
        cmd_queue = self._mcu.alloc_command_queue()
        self._set_cmd = self._mcu.lookup_command(
            "queue_digital_out oid=%c clock=%u on_ticks=%u", cq=cmd_queue)
    def set_digital(self, print_time, value):
        clock = self._mcu.print_time_to_clock(print_time)
        self._set_cmd.send([self._oid, clock, (not not value) ^ self._invert],
                           minclock=self._last_clock, reqclock=clock)
        self._last_clock = clock

class MCU_pwm:
    def __init__(self, mcu, pin_params):
        self._mcu = mcu
        self._hardware_pwm = False
        self._cycle_time = 0.100
        self._max_duration = 2.
        self._oid = None
        self._mcu.register_config_callback(self._build_config)
        self._pin = pin_params['pin']
        self._invert = pin_params['invert']
        self._start_value = self._shutdown_value = float(self._invert)
        self._last_clock = 0
        self._pwm_max = 0.
        self._set_cmd = None
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
        if self._max_duration and self._start_value != self._shutdown_value:
            raise pins.error("Pin with max duration must have start"
                             " value equal to shutdown value")
        cmd_queue = self._mcu.alloc_command_queue()
        curtime = self._mcu.get_printer().get_reactor().monotonic()
        printtime = self._mcu.estimated_print_time(curtime)
        self._last_clock = self._mcu.print_time_to_clock(printtime + 0.200)
        cycle_ticks = self._mcu.seconds_to_clock(self._cycle_time)
        mdur_ticks = self._mcu.seconds_to_clock(self._max_duration)
        if mdur_ticks >= 1<<31:
            raise pins.error("PWM pin max duration too large")
        if self._hardware_pwm:
            self._pwm_max = self._mcu.get_constant_float("PWM_MAX")
            self._mcu.request_move_queue_slot()
            self._oid = self._mcu.create_oid()
            self._mcu.add_config_cmd(
                "config_pwm_out oid=%d pin=%s cycle_ticks=%d value=%d"
                " default_value=%d max_duration=%d"
                % (self._oid, self._pin, cycle_ticks,
                   self._start_value * self._pwm_max,
                   self._shutdown_value * self._pwm_max, mdur_ticks))
            svalue = int(self._start_value * self._pwm_max + 0.5)
            self._mcu.add_config_cmd("queue_pwm_out oid=%d clock=%d value=%d"
                                     % (self._oid, self._last_clock, svalue),
                                     on_restart=True)
            self._set_cmd = self._mcu.lookup_command(
                "queue_pwm_out oid=%c clock=%u value=%hu", cq=cmd_queue)
            return
        # Software PWM
        if self._shutdown_value not in [0., 1.]:
            raise pins.error("shutdown value must be 0.0 or 1.0 on soft pwm")
        if cycle_ticks >= 1<<31:
            raise pins.error("PWM pin cycle time too large")
        self._mcu.request_move_queue_slot()
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s value=%d"
            " default_value=%d max_duration=%d"
            % (self._oid, self._pin, self._start_value >= 1.0,
               self._shutdown_value >= 0.5, mdur_ticks))
        self._mcu.add_config_cmd(
            "set_digital_out_pwm_cycle oid=%d cycle_ticks=%d"
            % (self._oid, cycle_ticks))
        self._pwm_max = float(cycle_ticks)
        svalue = int(self._start_value * cycle_ticks + 0.5)
        self._mcu.add_config_cmd(
            "queue_digital_out oid=%d clock=%d on_ticks=%d"
            % (self._oid, self._last_clock, svalue), is_init=True)
        self._set_cmd = self._mcu.lookup_command(
            "queue_digital_out oid=%c clock=%u on_ticks=%u", cq=cmd_queue)
    def set_pwm(self, print_time, value):
        if self._invert:
            value = 1. - value
        v = int(max(0., min(1., value)) * self._pwm_max + 0.5)
        clock = self._mcu.print_time_to_clock(print_time)
        self._set_cmd.send([self._oid, clock, v],
                           minclock=self._last_clock, reqclock=clock)
        self._last_clock = clock

class MCU_adc:
    def __init__(self, mcu, pin_params):
        self._mcu = mcu
        self._pin = pin_params['pin']
        self._min_sample = self._max_sample = 0.
        self._sample_time = self._report_time = 0.
        self._sample_count = self._range_check_count = 0
        self._report_clock = 0
        self._last_state = (0., 0.)
        self._oid = self._callback = None
        self._mcu.register_config_callback(self._build_config)
        self._inv_max_adc = 0.
    def get_mcu(self):
        return self._mcu
    def setup_minmax(self, sample_time, sample_count,
                     minval=0., maxval=1., range_check_count=0):
        self._sample_time = sample_time
        self._sample_count = sample_count
        self._min_sample = minval
        self._max_sample = maxval
        self._range_check_count = range_check_count
    def setup_adc_callback(self, report_time, callback):
        self._report_time = report_time
        self._callback = callback
    def get_last_value(self):
        return self._last_state
    def _build_config(self):
        if not self._sample_count:
            return
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd("config_analog_in oid=%d pin=%s" % (
            self._oid, self._pin))
        clock = self._mcu.get_query_slot(self._oid)
        sample_ticks = self._mcu.seconds_to_clock(self._sample_time)
        mcu_adc_max = self._mcu.get_constant_float("ADC_MAX")
        max_adc = self._sample_count * mcu_adc_max
        self._inv_max_adc = 1.0 / max_adc
        self._report_clock = self._mcu.seconds_to_clock(self._report_time)
        min_sample = max(0, min(0xffff, int(self._min_sample * max_adc)))
        max_sample = max(0, min(0xffff, int(
            math.ceil(self._max_sample * max_adc))))
        self._mcu.add_config_cmd(
            "query_analog_in oid=%d clock=%d sample_ticks=%d sample_count=%d"
            " rest_ticks=%d min_value=%d max_value=%d range_check_count=%d" % (
                self._oid, clock, sample_ticks, self._sample_count,
                self._report_clock, min_sample, max_sample,
                self._range_check_count), is_init=True)
        self._mcu.register_response(self._handle_analog_in_state,
                                    "analog_in_state", self._oid)
    def _handle_analog_in_state(self, params):
        last_value = params['value'] * self._inv_max_adc
        next_clock = self._mcu.clock32_to_clock64(params['next_clock'])
        last_read_clock = next_clock - self._report_clock
        last_read_time = self._mcu.clock_to_print_time(last_read_clock)
        self._last_state = (last_value, last_read_time)
        if self._callback is not None:
            self._callback(last_read_time, last_value)


######################################################################
# Main MCU class
######################################################################

class MCU:
    error = error
    def __init__(self, config, clocksync):
        self._printer = printer = config.get_printer()
        self._clocksync = clocksync
        self._reactor = printer.get_reactor()
        self._name = config.get_name()
        if self._name.startswith('mcu '):
            self._name = self._name[4:]
        # Serial port
        wp = "mcu '%s': " % (self._name)
        self._serial = serialhdl.SerialReader(self._reactor, warn_prefix=wp)
        self._baud = 0
        self._canbus_iface = None
        canbus_uuid = config.get('canbus_uuid', None)
        if canbus_uuid is not None:
            self._serialport = canbus_uuid
            self._canbus_iface = config.get('canbus_interface', 'can0')
            cbid = self._printer.load_object(config, 'canbus_ids')
            cbid.add_uuid(config, canbus_uuid, self._canbus_iface)
        else:
            self._serialport = config.get('serial')
            if not (self._serialport.startswith("/dev/rpmsg_")
                    or self._serialport.startswith("/tmp/klipper_host_")):
                self._baud = config.getint('baud', 250000, minval=2400)
        # Restarts
        restart_methods = [None, 'arduino', 'cheetah', 'command', 'rpi_usb']
        self._restart_method = 'command'
        if self._baud:
            rmethods = {m: m for m in restart_methods}
            self._restart_method = config.getchoice('restart_method',
                                                    rmethods, None)
        self._reset_cmd = self._config_reset_cmd = None
        self._is_mcu_bridge = False
        self._emergency_stop_cmd = None
        self._is_shutdown = self._is_timeout = False
        self._shutdown_clock = 0
        self._shutdown_msg = ""
        # Config building
        printer.lookup_object('pins').register_chip(self._name, self)
        self._oid_count = 0
        self._config_callbacks = []
        self._config_cmds = []
        self._restart_cmds = []
        self._init_cmds = []
        self._mcu_freq = 0.
        # Move command queuing
        ffi_main, self._ffi_lib = chelper.get_ffi()
        self._max_stepper_error = config.getfloat('max_stepper_error', 0.000025,
                                                  minval=0.)
        self._reserved_move_slots = 0
        self._stepqueues = []
        self._steppersync = None
        self._flush_callbacks = []
        # Stats
        self._get_status_info = {}
        self._stats_sumsq_base = 0.
        self._mcu_tick_avg = 0.
        self._mcu_tick_stddev = 0.
        self._mcu_tick_awake = 0.
        # Register handlers
        printer.register_event_handler("klippy:firmware_restart",
                                       self._firmware_restart)
        printer.register_event_handler("klippy:mcu_identify",
                                       self._mcu_identify)
        printer.register_event_handler("klippy:connect", self._connect)
        printer.register_event_handler("klippy:shutdown", self._shutdown)
        printer.register_event_handler("klippy:disconnect", self._disconnect)
        printer.register_event_handler("klippy:ready", self._ready)
    # Serial callbacks
    def _handle_mcu_stats(self, params):
        count = params['count']
        tick_sum = params['sum']
        c = 1.0 / (count * self._mcu_freq)
        self._mcu_tick_avg = tick_sum * c
        tick_sumsq = params['sumsq'] * self._stats_sumsq_base
        diff = count*tick_sumsq - tick_sum**2
        self._mcu_tick_stddev = c * math.sqrt(max(0., diff))
        self._mcu_tick_awake = tick_sum / self._mcu_freq
    def _handle_shutdown(self, params):
        if self._is_shutdown:
            return
        self._is_shutdown = True
        clock = params.get("clock")
        if clock is not None:
            self._shutdown_clock = self.clock32_to_clock64(clock)
        self._shutdown_msg = msg = params['static_string_id']
        logging.info("MCU '%s' %s: %s\n%s\n%s", self._name, params['#name'],
                     self._shutdown_msg, self._clocksync.dump_debug(),
                     self._serial.dump_debug())
        prefix = "MCU '%s' shutdown: " % (self._name,)
        if params['#name'] == 'is_shutdown':
            prefix = "Previous MCU '%s' shutdown: " % (self._name,)
        self._printer.invoke_async_shutdown(prefix + msg + error_help(msg))
    def _handle_starting(self, params):
        if not self._is_shutdown:
            self._printer.invoke_async_shutdown("MCU '%s' spontaneous restart"
                                                % (self._name,))
    # Connection phase
    def _check_restart(self, reason):
        start_reason = self._printer.get_start_args().get("start_reason")
        if start_reason == 'firmware_restart':
            return
        logging.info("Attempting automated MCU '%s' restart: %s",
                     self._name, reason)
        self._printer.request_exit('firmware_restart')
        self._reactor.pause(self._reactor.monotonic() + 2.000)
        raise error("Attempt MCU '%s' restart failed" % (self._name,))
    def _connect_file(self, pace=False):
        # In a debugging mode.  Open debug output file and read data dictionary
        start_args = self._printer.get_start_args()
        if self._name == 'mcu':
            out_fname = start_args.get('debugoutput')
            dict_fname = start_args.get('dictionary')
        else:
            out_fname = start_args.get('debugoutput') + "-" + self._name
            dict_fname = start_args.get('dictionary_' + self._name)
        outfile = open(out_fname, 'wb')
        dfile = open(dict_fname, 'rb')
        dict_data = dfile.read()
        dfile.close()
        self._serial.connect_file(outfile, dict_data)
        self._clocksync.connect_file(self._serial, pace)
        # Handle pacing
        if not pace:
            def dummy_estimated_print_time(eventtime):
                return 0.
            self.estimated_print_time = dummy_estimated_print_time
    def _send_config(self, prev_crc):
        # Build config commands
        for cb in self._config_callbacks:
            cb()
        self._config_cmds.insert(0, "allocate_oids count=%d"
                                 % (self._oid_count,))
        # Resolve pin names
        ppins = self._printer.lookup_object('pins')
        pin_resolver = ppins.get_pin_resolver(self._name)
        for cmdlist in (self._config_cmds, self._restart_cmds, self._init_cmds):
            for i, cmd in enumerate(cmdlist):
                cmdlist[i] = pin_resolver.update_command(cmd)
        # Calculate config CRC
        encoded_config = '\n'.join(self._config_cmds).encode()
        config_crc = zlib.crc32(encoded_config) & 0xffffffff
        self.add_config_cmd("finalize_config crc=%d" % (config_crc,))
        if prev_crc is not None and config_crc != prev_crc:
            self._check_restart("CRC mismatch")
            raise error("MCU '%s' CRC does not match config" % (self._name,))
        # Transmit config messages (if needed)
        self.register_response(self._handle_starting, 'starting')
        try:
            if prev_crc is None:
                logging.info("Sending MCU '%s' printer configuration...",
                             self._name)
                for c in self._config_cmds:
                    self._serial.send(c)
            else:
                for c in self._restart_cmds:
                    self._serial.send(c)
            # Transmit init messages
            for c in self._init_cmds:
                self._serial.send(c)
        except msgproto.enumeration_error as e:
            enum_name, enum_value = e.get_enum_params()
            if enum_name == 'pin':
                # Raise pin name errors as a config error (not a protocol error)
                raise self._printer.config_error(
                    "Pin '%s' is not a valid pin name on mcu '%s'"
                    % (enum_value, self._name))
            raise
    def _send_get_config(self):
        get_config_cmd = self.lookup_query_command(
            "get_config",
            "config is_config=%c crc=%u is_shutdown=%c move_count=%hu")
        if self.is_fileoutput():
            return { 'is_config': 0, 'move_count': 500, 'crc': 0 }
        config_params = get_config_cmd.send()
        if self._is_shutdown:
            raise error("MCU '%s' error during config: %s" % (
                self._name, self._shutdown_msg))
        if config_params['is_shutdown']:
            raise error("Can not update MCU '%s' config as it is shutdown" % (
                self._name,))
        return config_params
    def _log_info(self):
        msgparser = self._serial.get_msgparser()
        message_count = len(msgparser.get_messages())
        version, build_versions = msgparser.get_version_info()
        log_info = [
            "Loaded MCU '%s' %d commands (%s / %s)"
            % (self._name, message_count, version, build_versions),
            "MCU '%s' config: %s" % (self._name, " ".join(
                ["%s=%s" % (k, v) for k, v in self.get_constants().items()]))]
        return "\n".join(log_info)
    def _connect(self):
        config_params = self._send_get_config()
        if not config_params['is_config']:
            if self._restart_method == 'rpi_usb':
                # Only configure mcu after usb power reset
                self._check_restart("full reset before config")
            # Not configured - send config and issue get_config again
            self._send_config(None)
            config_params = self._send_get_config()
            if not config_params['is_config'] and not self.is_fileoutput():
                raise error("Unable to configure MCU '%s'" % (self._name,))
        else:
            start_reason = self._printer.get_start_args().get("start_reason")
            if start_reason == 'firmware_restart':
                raise error("Failed automated reset of MCU '%s'"
                            % (self._name,))
            # Already configured - send init commands
            self._send_config(config_params['crc'])
        # Setup steppersync with the move_count returned by get_config
        move_count = config_params['move_count']
        if move_count < self._reserved_move_slots:
            raise error("Too few moves available on MCU '%s'" % (self._name,))
        ffi_main, ffi_lib = chelper.get_ffi()
        self._steppersync = ffi_main.gc(
            ffi_lib.steppersync_alloc(self._serial.get_serialqueue(),
                                      self._stepqueues, len(self._stepqueues),
                                      move_count-self._reserved_move_slots),
            ffi_lib.steppersync_free)
        ffi_lib.steppersync_set_time(self._steppersync, 0., self._mcu_freq)
        # Log config information
        move_msg = "Configured MCU '%s' (%d moves)" % (self._name, move_count)
        logging.info(move_msg)
        log_info = self._log_info() + "\n" + move_msg
        self._printer.set_rollover_info(self._name, log_info, log=False)
    def _mcu_identify(self):
        if self.is_fileoutput():
            self._connect_file()
        else:
            resmeth = self._restart_method
            if resmeth == 'rpi_usb' and not os.path.exists(self._serialport):
                # Try toggling usb power
                self._check_restart("enable power")
            try:
                if self._canbus_iface is not None:
                    cbid = self._printer.lookup_object('canbus_ids')
                    nodeid = cbid.get_nodeid(self._serialport)
                    self._serial.connect_canbus(self._serialport, nodeid,
                                                self._canbus_iface)
                elif self._baud:
                    # Cheetah boards require RTS to be deasserted
                    # else a reset will trigger the built-in bootloader.
                    rts = (resmeth != "cheetah")
                    self._serial.connect_uart(self._serialport, self._baud, rts)
                else:
                    self._serial.connect_pipe(self._serialport)
                self._clocksync.connect(self._serial)
            except serialhdl.error as e:
                raise error(str(e))
        logging.info(self._log_info())
        ppins = self._printer.lookup_object('pins')
        pin_resolver = ppins.get_pin_resolver(self._name)
        for cname, value in self.get_constants().items():
            if cname.startswith("RESERVE_PINS_"):
                for pin in value.split(','):
                    pin_resolver.reserve_pin(pin, cname[13:])
        self._mcu_freq = self.get_constant_float('CLOCK_FREQ')
        self._stats_sumsq_base = self.get_constant_float('STATS_SUMSQ_BASE')
        self._emergency_stop_cmd = self.lookup_command("emergency_stop")
        self._reset_cmd = self.try_lookup_command("reset")
        self._config_reset_cmd = self.try_lookup_command("config_reset")
        ext_only = self._reset_cmd is None and self._config_reset_cmd is None
        msgparser = self._serial.get_msgparser()
        mbaud = msgparser.get_constant('SERIAL_BAUD', None)
        if self._restart_method is None and mbaud is None and not ext_only:
            self._restart_method = 'command'
        if msgparser.get_constant('CANBUS_BRIDGE', 0):
            self._is_mcu_bridge = True
            self._printer.register_event_handler("klippy:firmware_restart",
                                                 self._firmware_restart_bridge)
        version, build_versions = msgparser.get_version_info()
        self._get_status_info['mcu_version'] = version
        self._get_status_info['mcu_build_versions'] = build_versions
        self._get_status_info['mcu_constants'] = msgparser.get_constants()
        self.register_response(self._handle_shutdown, 'shutdown')
        self.register_response(self._handle_shutdown, 'is_shutdown')
        self.register_response(self._handle_mcu_stats, 'stats')
    def _ready(self):
        if self.is_fileoutput():
            return
        # Check that reported mcu frequency is in range
        mcu_freq = self._mcu_freq
        systime = self._reactor.monotonic()
        get_clock = self._clocksync.get_clock
        calc_freq = get_clock(systime + 1) - get_clock(systime)
        mcu_freq_mhz = int(mcu_freq / 1000000. + 0.5)
        calc_freq_mhz = int(calc_freq / 1000000. + 0.5)
        if mcu_freq_mhz != calc_freq_mhz:
            pconfig = self._printer.lookup_object('configfile')
            msg = ("MCU '%s' configured for %dMhz but running at %dMhz!"
                    % (self._name, mcu_freq_mhz, calc_freq_mhz))
            pconfig.runtime_warning(msg)
    # Config creation helpers
    def setup_pin(self, pin_type, pin_params):
        pcs = {'endstop': MCU_endstop,
               'digital_out': MCU_digital_out, 'pwm': MCU_pwm, 'adc': MCU_adc}
        if pin_type not in pcs:
            raise pins.error("pin type %s not supported on mcu" % (pin_type,))
        return pcs[pin_type](self, pin_params)
    def create_oid(self):
        self._oid_count += 1
        return self._oid_count - 1
    def register_config_callback(self, cb):
        self._config_callbacks.append(cb)
    def add_config_cmd(self, cmd, is_init=False, on_restart=False):
        if is_init:
            self._init_cmds.append(cmd)
        elif on_restart:
            self._restart_cmds.append(cmd)
        else:
            self._config_cmds.append(cmd)
    def get_query_slot(self, oid):
        slot = self.seconds_to_clock(oid * .01)
        t = int(self.estimated_print_time(self._reactor.monotonic()) + 1.5)
        return self.print_time_to_clock(t) + slot
    def seconds_to_clock(self, time):
        return int(time * self._mcu_freq)
    def get_max_stepper_error(self):
        return self._max_stepper_error
    # Wrapper functions
    def get_printer(self):
        return self._printer
    def get_name(self):
        return self._name
    def register_response(self, cb, msg, oid=None):
        self._serial.register_response(cb, msg, oid)
    def alloc_command_queue(self):
        return self._serial.alloc_command_queue()
    def lookup_command(self, msgformat, cq=None):
        return CommandWrapper(self._serial, msgformat, cq)
    def lookup_query_command(self, msgformat, respformat, oid=None,
                             cq=None, is_async=False):
        return CommandQueryWrapper(self._serial, msgformat, respformat, oid,
                                   cq, is_async, self._printer.command_error)
    def try_lookup_command(self, msgformat):
        try:
            return self.lookup_command(msgformat)
        except self._serial.get_msgparser().error as e:
            return None
    def get_enumerations(self):
        return self._serial.get_msgparser().get_enumerations()
    def get_constants(self):
        return self._serial.get_msgparser().get_constants()
    def get_constant_float(self, name):
        return self._serial.get_msgparser().get_constant_float(name)
    def print_time_to_clock(self, print_time):
        return self._clocksync.print_time_to_clock(print_time)
    def clock_to_print_time(self, clock):
        return self._clocksync.clock_to_print_time(clock)
    def estimated_print_time(self, eventtime):
        return self._clocksync.estimated_print_time(eventtime)
    def clock32_to_clock64(self, clock32):
        return self._clocksync.clock32_to_clock64(clock32)
    # Restarts
    def _disconnect(self):
        self._serial.disconnect()
        self._steppersync = None
    def _shutdown(self, force=False):
        if (self._emergency_stop_cmd is None
            or (self._is_shutdown and not force)):
            return
        self._emergency_stop_cmd.send()
    def _restart_arduino(self):
        logging.info("Attempting MCU '%s' reset", self._name)
        self._disconnect()
        serialhdl.arduino_reset(self._serialport, self._reactor)
    def _restart_cheetah(self):
        logging.info("Attempting MCU '%s' Cheetah-style reset", self._name)
        self._disconnect()
        serialhdl.cheetah_reset(self._serialport, self._reactor)
    def _restart_via_command(self):
        if ((self._reset_cmd is None and self._config_reset_cmd is None)
            or not self._clocksync.is_active()):
            logging.info("Unable to issue reset command on MCU '%s'",
                         self._name)
            return
        if self._reset_cmd is None:
            # Attempt reset via config_reset command
            logging.info("Attempting MCU '%s' config_reset command", self._name)
            self._is_shutdown = True
            self._shutdown(force=True)
            self._reactor.pause(self._reactor.monotonic() + 0.015)
            self._config_reset_cmd.send()
        else:
            # Attempt reset via reset command
            logging.info("Attempting MCU '%s' reset command", self._name)
            self._reset_cmd.send()
        self._reactor.pause(self._reactor.monotonic() + 0.015)
        self._disconnect()
    def _restart_rpi_usb(self):
        logging.info("Attempting MCU '%s' reset via rpi usb power", self._name)
        self._disconnect()
        chelper.run_hub_ctrl(0)
        self._reactor.pause(self._reactor.monotonic() + 2.)
        chelper.run_hub_ctrl(1)
    def _firmware_restart(self, force=False):
        if self._is_mcu_bridge and not force:
            return
        if self._restart_method == 'rpi_usb':
            self._restart_rpi_usb()
        elif self._restart_method == 'command':
            self._restart_via_command()
        elif self._restart_method == 'cheetah':
            self._restart_cheetah()
        else:
            self._restart_arduino()
    def _firmware_restart_bridge(self):
        self._firmware_restart(True)
    # Move queue tracking
    def register_stepqueue(self, stepqueue):
        self._stepqueues.append(stepqueue)
    def request_move_queue_slot(self):
        self._reserved_move_slots += 1
    def register_flush_callback(self, callback):
        self._flush_callbacks.append(callback)
    def flush_moves(self, print_time, clear_history_time):
        if self._steppersync is None:
            return
        clock = self.print_time_to_clock(print_time)
        if clock < 0:
            return
        for cb in self._flush_callbacks:
            cb(print_time, clock)
        clear_history_clock = \
            max(0, self.print_time_to_clock(clear_history_time))
        ret = self._ffi_lib.steppersync_flush(self._steppersync, clock,
                                              clear_history_clock)
        if ret:
            raise error("Internal error in MCU '%s' stepcompress"
                        % (self._name,))
    def check_active(self, print_time, eventtime):
        if self._steppersync is None:
            return
        offset, freq = self._clocksync.calibrate_clock(print_time, eventtime)
        self._ffi_lib.steppersync_set_time(self._steppersync, offset, freq)
        if (self._clocksync.is_active() or self.is_fileoutput()
            or self._is_timeout):
            return
        self._is_timeout = True
        logging.info("Timeout with MCU '%s' (eventtime=%f)",
                     self._name, eventtime)
        self._printer.invoke_shutdown("Lost communication with MCU '%s'" % (
            self._name,))
    # Misc external commands
    def is_fileoutput(self):
        return self._printer.get_start_args().get('debugoutput') is not None
    def is_shutdown(self):
        return self._is_shutdown
    def get_shutdown_clock(self):
        return self._shutdown_clock
    def get_status(self, eventtime=None):
        return dict(self._get_status_info)
    def stats(self, eventtime):
        load = "mcu_awake=%.03f mcu_task_avg=%.06f mcu_task_stddev=%.06f" % (
            self._mcu_tick_awake, self._mcu_tick_avg, self._mcu_tick_stddev)
        stats = ' '.join([load, self._serial.stats(eventtime),
                          self._clocksync.stats(eventtime)])
        parts = [s.split('=', 1) for s in stats.split()]
        last_stats = {k:(float(v) if '.' in v else int(v)) for k, v in parts}
        self._get_status_info['last_stats'] = last_stats
        return False, '%s: %s' % (self._name, stats)

Common_MCU_errors = {
    ("Timer too close",): """
This often indicates the host computer is overloaded. Check
for other processes consuming excessive CPU time, high swap
usage, disk errors, overheating, unstable voltage, or
similar system problems on the host computer.""",
    ("Missed scheduling of next ",): """
This is generally indicative of an intermittent
communication failure between micro-controller and host.""",
    ("ADC out of range",): """
This generally occurs when a heater temperature exceeds
its configured min_temp or max_temp.""",
    ("Rescheduled timer in the past", "Stepper too far in past"): """
This generally occurs when the micro-controller has been
requested to step at a rate higher than it is capable of
obtaining.""",
    ("Command request",): """
This generally occurs in response to an M112 G-Code command
or in response to an internal error in the host software.""",
}

def error_help(msg):
    for prefixes, help_msg in Common_MCU_errors.items():
        for prefix in prefixes:
            if msg.startswith(prefix):
                return help_msg
    return ""

def add_printer_objects(config):
    printer = config.get_printer()
    reactor = printer.get_reactor()
    mainsync = clocksync.ClockSync(reactor)
    printer.add_object('mcu', MCU(config.getsection('mcu'), mainsync))
    for s in config.get_prefix_sections('mcu '):
        printer.add_object(s.section, MCU(
            s, clocksync.SecondarySync(reactor, mainsync)))

def get_printer_mcu(printer, name):
    if name == 'mcu':
        return printer.lookup_object(name)
    return printer.lookup_object('mcu ' + name)
