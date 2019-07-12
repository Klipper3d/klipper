# Interface to Klipper micro-controller code
#
# Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, zlib, logging, math
import serialhdl, pins, chelper, clocksync

class error(Exception):
    pass

class MCU_stepper:
    def __init__(self, mcu, pin_params):
        self._mcu = mcu
        self._oid = oid = self._mcu.create_oid()
        self._mcu.register_config_callback(self._build_config)
        self._step_pin = pin_params['pin']
        self._invert_step = pin_params['invert']
        self._dir_pin = self._invert_dir = None
        self._mcu_position_offset = 0.
        self._step_dist = 0.
        self._min_stop_interval = 0.
        self._reset_cmd_id = self._get_position_cmd = None
        ffi_main, self._ffi_lib = chelper.get_ffi()
        self._stepqueue = ffi_main.gc(self._ffi_lib.stepcompress_alloc(oid),
                                      self._ffi_lib.stepcompress_free)
        self._mcu.register_stepqueue(self._stepqueue)
        self._stepper_kinematics = self._itersolve_gen_steps = None
        self.set_ignore_move(False)
    def get_mcu(self):
        return self._mcu
    def setup_dir_pin(self, pin_params):
        if pin_params['chip'] is not self._mcu:
            raise pins.error("Stepper dir pin must be on same mcu as step pin")
        self._dir_pin = pin_params['pin']
        self._invert_dir = pin_params['invert']
    def setup_min_stop_interval(self, min_stop_interval):
        self._min_stop_interval = min_stop_interval
    def setup_step_distance(self, step_dist):
        self._step_dist = step_dist
    def setup_itersolve(self, alloc_func, *params):
        ffi_main, ffi_lib = chelper.get_ffi()
        sk = ffi_main.gc(getattr(ffi_lib, alloc_func)(*params), ffi_lib.free)
        self.set_stepper_kinematics(sk)
    def _build_config(self):
        max_error = self._mcu.get_max_stepper_error()
        min_stop_interval = max(0., self._min_stop_interval - max_error)
        self._mcu.add_config_cmd(
            "config_stepper oid=%d step_pin=%s dir_pin=%s"
            " min_stop_interval=%d invert_step=%d" % (
                self._oid, self._step_pin, self._dir_pin,
                self._mcu.seconds_to_clock(min_stop_interval),
                self._invert_step))
        self._mcu.add_config_cmd(
            "reset_step_clock oid=%d clock=0" % (self._oid,), is_init=True)
        step_cmd_id = self._mcu.lookup_command_id(
            "queue_step oid=%c interval=%u count=%hu add=%hi")
        dir_cmd_id = self._mcu.lookup_command_id(
            "set_next_step_dir oid=%c dir=%c")
        self._reset_cmd_id = self._mcu.lookup_command_id(
            "reset_step_clock oid=%c clock=%u")
        self._get_position_cmd = self._mcu.lookup_command(
            "stepper_get_position oid=%c")
        self._ffi_lib.stepcompress_fill(
            self._stepqueue, self._mcu.seconds_to_clock(max_error),
            self._invert_dir, step_cmd_id, dir_cmd_id)
    def get_oid(self):
        return self._oid
    def get_step_dist(self):
        return self._step_dist
    def is_dir_inverted(self):
        return self._invert_dir
    def calc_position_from_coord(self, coord):
        return self._ffi_lib.itersolve_calc_position_from_coord(
            self._stepper_kinematics, coord[0], coord[1], coord[2])
    def set_position(self, coord):
        self.set_commanded_position(self.calc_position_from_coord(coord))
    def get_commanded_position(self):
        return self._ffi_lib.itersolve_get_commanded_pos(
            self._stepper_kinematics)
    def set_commanded_position(self, pos):
        self._mcu_position_offset += self.get_commanded_position() - pos
        self._ffi_lib.itersolve_set_commanded_pos(self._stepper_kinematics, pos)
    def get_mcu_position(self):
        mcu_pos_dist = self.get_commanded_position() + self._mcu_position_offset
        mcu_pos = mcu_pos_dist / self._step_dist
        if mcu_pos >= 0.:
            return int(mcu_pos + 0.5)
        return int(mcu_pos - 0.5)
    def set_stepper_kinematics(self, sk):
        old_sk = self._stepper_kinematics
        self._stepper_kinematics = sk
        if sk is not None:
            self._ffi_lib.itersolve_set_stepcompress(
                sk, self._stepqueue, self._step_dist)
        return old_sk
    def set_ignore_move(self, ignore_move):
        was_ignore = (self._itersolve_gen_steps
                      is not self._ffi_lib.itersolve_gen_steps)
        if ignore_move:
            self._itersolve_gen_steps = (lambda *args: 0)
        else:
            self._itersolve_gen_steps = self._ffi_lib.itersolve_gen_steps
        return was_ignore
    def note_homing_end(self, did_trigger=False):
        ret = self._ffi_lib.stepcompress_reset(self._stepqueue, 0)
        if ret:
            raise error("Internal error in stepcompress")
        data = (self._reset_cmd_id, self._oid, 0)
        ret = self._ffi_lib.stepcompress_queue_msg(
            self._stepqueue, data, len(data))
        if ret:
            raise error("Internal error in stepcompress")
        if not did_trigger or self._mcu.is_fileoutput():
            return
        params = self._get_position_cmd.send_with_response(
            [self._oid], response='stepper_position', response_oid=self._oid)
        mcu_pos_dist = params['pos'] * self._step_dist
        if self._invert_dir:
            mcu_pos_dist = -mcu_pos_dist
        self._ffi_lib.itersolve_set_commanded_pos(
            self._stepper_kinematics, mcu_pos_dist - self._mcu_position_offset)
    def step_itersolve(self, cmove):
        ret = self._itersolve_gen_steps(self._stepper_kinematics, cmove)
        if ret:
            raise error("Internal error in stepcompress")

class MCU_endstop:
    class TimeoutError(Exception):
        pass
    RETRY_QUERY = 1.000
    def __init__(self, mcu, pin_params):
        self._mcu = mcu
        self._steppers = []
        self._pin = pin_params['pin']
        self._pullup = pin_params['pullup']
        self._invert = pin_params['invert']
        self._reactor = mcu.get_printer().get_reactor()
        self._oid = self._home_cmd = self._query_cmd = None
        self._mcu.register_config_callback(self._build_config)
        self._min_query_time = self._last_sent_time = 0.
        self._next_query_print_time = self._end_home_time = 0.
        self._trigger_completion = self._home_completion = None
        self._trigger_notify = None
    def get_mcu(self):
        return self._mcu
    def add_stepper(self, stepper):
        if stepper.get_mcu() is not self._mcu:
            raise pins.error("Endstop and stepper must be on the same mcu")
        if stepper in self._steppers:
            return
        self._steppers.append(stepper)
    def get_steppers(self):
        return list(self._steppers)
    def _build_config(self):
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_endstop oid=%d pin=%s pull_up=%d stepper_count=%d" % (
                self._oid, self._pin, self._pullup, len(self._steppers)))
        self._mcu.add_config_cmd(
            "endstop_home oid=%d clock=0 sample_ticks=0 sample_count=0"
            " rest_ticks=0 pin_value=0" % (self._oid,), is_init=True)
        for i, s in enumerate(self._steppers):
            self._mcu.add_config_cmd(
                "endstop_set_stepper oid=%d pos=%d stepper_oid=%d" % (
                    self._oid, i, s.get_oid()), is_init=True)
        cmd_queue = self._mcu.alloc_command_queue()
        self._home_cmd = self._mcu.lookup_command(
            "endstop_home oid=%c clock=%u sample_ticks=%u sample_count=%c"
            " rest_ticks=%u pin_value=%c", cq=cmd_queue)
        self._query_cmd = self._mcu.lookup_command(
            "endstop_query_state oid=%c", cq=cmd_queue)
    def home_prepare(self):
        pass
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True, notify=None):
        clock = self._mcu.print_time_to_clock(print_time)
        rest_ticks = int(rest_time * self._mcu.get_adjusted_freq())
        self._trigger_notify = notify
        self._next_query_print_time = print_time + self.RETRY_QUERY
        self._min_query_time = self._reactor.monotonic()
        self._last_sent_time = 0.
        self._home_end_time = self._reactor.NEVER
        self._trigger_completion = self._reactor.completion()
        self._home_completion = self._reactor.completion()
        self._mcu.register_response(self._handle_endstop_state,
                                    "endstop_state", self._oid)
        self._home_cmd.send(
            [self._oid, clock, self._mcu.seconds_to_clock(sample_time),
             sample_count, rest_ticks, triggered ^ self._invert],
            reqclock=clock)
        self._home_completion = self._reactor.register_callback(
            self._home_retry)
    def _handle_endstop_state(self, params):
        logging.debug("endstop_state %s", params)
        if params['#sent_time'] >= self._min_query_time:
            if params['homing']:
                self._last_sent_time = params['#sent_time']
            else:
                self._min_query_time = self._reactor.NEVER
                self._reactor.async_complete(self._trigger_completion, params)
    def _home_retry(self, eventtime):
        if self._mcu.is_fileoutput():
            return True
        while 1:
            params = self._trigger_completion.wait(eventtime + 0.100)
            if params is not None:
                # Homing completed successfully
                if self._trigger_notify is not None:
                    self._trigger_notify()
                return True
            # Check for timeout
            last = self._mcu.estimated_print_time(self._last_sent_time)
            if last > self._home_end_time or self._mcu.is_shutdown():
                return False
            # Check for resend
            eventtime = self._reactor.monotonic()
            est_print_time = self._mcu.estimated_print_time(eventtime)
            if est_print_time >= self._next_query_print_time:
                self._next_query_print_time = est_print_time + self.RETRY_QUERY
                self._query_cmd.send([self._oid])
    def home_wait(self, home_end_time):
        self._home_end_time = home_end_time
        did_trigger = self._home_completion.wait()
        self._mcu.register_response(None, "endstop_state", self._oid)
        self._home_cmd.send([self._oid, 0, 0, 0, 0, 0])
        for s in self._steppers:
            s.note_homing_end(did_trigger=did_trigger)
        if not did_trigger:
            raise self.TimeoutError("Timeout during endstop homing")
    def home_finalize(self):
        pass
    def query_endstop(self, print_time):
        clock = self._mcu.print_time_to_clock(print_time)
        if self._mcu.is_fileoutput():
            return 0
        params = self._query_cmd.send_with_response(
            [self._oid], "endstop_state", self._oid, minclock=clock)
        return params['pin_value'] ^ self._invert

class MCU_digital_out:
    def __init__(self, mcu, pin_params):
        self._mcu = mcu
        self._oid = None
        self._mcu.register_config_callback(self._build_config)
        self._pin = pin_params['pin']
        self._invert = pin_params['invert']
        self._start_value = self._shutdown_value = self._invert
        self._is_static = False
        self._max_duration = 2.
        self._last_clock = 0
        self._set_cmd = None
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_start_value(self, start_value, shutdown_value, is_static=False):
        if is_static and start_value != shutdown_value:
            raise pins.error("Static pin can not have shutdown value")
        self._start_value = (not not start_value) ^ self._invert
        self._shutdown_value = (not not shutdown_value) ^ self._invert
        self._is_static = is_static
    def _build_config(self):
        if self._is_static:
            self._mcu.add_config_cmd("set_digital_out pin=%s value=%d" % (
                self._pin, self._start_value))
            return
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s value=%d default_value=%d"
            " max_duration=%d" % (
                self._oid, self._pin, self._start_value, self._shutdown_value,
                self._mcu.seconds_to_clock(self._max_duration)))
        cmd_queue = self._mcu.alloc_command_queue()
        self._set_cmd = self._mcu.lookup_command(
            "schedule_digital_out oid=%c clock=%u value=%c", cq=cmd_queue)
    def set_digital(self, print_time, value):
        clock = self._mcu.print_time_to_clock(print_time)
        self._set_cmd.send([self._oid, clock, (not not value) ^ self._invert],
                           minclock=self._last_clock, reqclock=clock)
        self._last_clock = clock
    def set_pwm(self, print_time, value):
        self.set_digital(print_time, value >= 0.5)

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
        self._is_static = False
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
    def setup_start_value(self, start_value, shutdown_value, is_static=False):
        if is_static and start_value != shutdown_value:
            raise pins.error("Static pin can not have shutdown value")
        if self._invert:
            start_value = 1. - start_value
            shutdown_value = 1. - shutdown_value
        self._start_value = max(0., min(1., start_value))
        self._shutdown_value = max(0., min(1., shutdown_value))
        self._is_static = is_static
    def _build_config(self):
        cmd_queue = self._mcu.alloc_command_queue()
        cycle_ticks = self._mcu.seconds_to_clock(self._cycle_time)
        if self._hardware_pwm:
            self._pwm_max = self._mcu.get_constant_float("PWM_MAX")
            if self._is_static:
                self._mcu.add_config_cmd(
                    "set_pwm_out pin=%s cycle_ticks=%d value=%d" % (
                        self._pin, cycle_ticks,
                        self._start_value * self._pwm_max))
                return
            self._oid = self._mcu.create_oid()
            self._mcu.add_config_cmd(
                "config_pwm_out oid=%d pin=%s cycle_ticks=%d value=%d"
                " default_value=%d max_duration=%d" % (
                    self._oid, self._pin, cycle_ticks,
                    self._start_value * self._pwm_max,
                    self._shutdown_value * self._pwm_max,
                    self._mcu.seconds_to_clock(self._max_duration)))
            self._set_cmd = self._mcu.lookup_command(
                "schedule_pwm_out oid=%c clock=%u value=%hu", cq=cmd_queue)
        else:
            if self._shutdown_value not in [0., 1.]:
                raise pins.error(
                    "shutdown value must be 0.0 or 1.0 on soft pwm")
            self._pwm_max = float(cycle_ticks)
            if self._is_static:
                self._mcu.add_config_cmd("set_digital_out pin=%s value=%d" % (
                    self._pin, self._start_value >= 0.5))
                return
            self._oid = self._mcu.create_oid()
            self._mcu.add_config_cmd(
                "config_soft_pwm_out oid=%d pin=%s cycle_ticks=%d value=%d"
                " default_value=%d max_duration=%d" % (
                    self._oid, self._pin, cycle_ticks,
                    self._start_value >= 1.0, self._shutdown_value >= 0.5,
                    self._mcu.seconds_to_clock(self._max_duration)))
            if self._start_value not in [0., 1.]:
                clock = self._mcu.get_query_slot(self._oid)
                svalue = int(self._start_value * self._pwm_max + 0.5)
                self._mcu.add_config_cmd(
                    "schedule_soft_pwm_out oid=%d clock=%d on_ticks=%d" % (
                        self._oid, clock, svalue))
            self._set_cmd = self._mcu.lookup_command(
                "schedule_soft_pwm_out oid=%c clock=%u on_ticks=%u",
                cq=cmd_queue)
    def set_pwm(self, print_time, value):
        clock = self._mcu.print_time_to_clock(print_time)
        if self._invert:
            value = 1. - value
        value = int(max(0., min(1., value)) * self._pwm_max + 0.5)
        self._set_cmd.send([self._oid, clock, value],
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
        if self._callback is not None:
            self._callback(last_read_time, last_value)

# Wrapper around command sending
class CommandWrapper:
    def __init__(self, mcu, serial, clocksync, cmd, cmd_queue):
        self._mcu = mcu
        self._serial = serial
        self._clocksync = clocksync
        self._cmd = cmd
        self._cmd_queue = cmd_queue
    def send(self, data=(), minclock=0, reqclock=0):
        cmd = self._cmd.encode(data)
        self._serial.raw_send(cmd, minclock, reqclock, self._cmd_queue)
    def send_with_response(self, data=(), response=None, response_oid=None,
                           minclock=0):
        minsystime = 0.
        if minclock:
            minsystime = self._clocksync.estimate_clock_systime(minclock)
        cmd = self._cmd.encode(data)
        src = serialhdl.SerialRetryCommand(self._serial, response, response_oid)
        try:
            return src.get_response([cmd], self._cmd_queue,
                                    minclock=minclock, minsystime=minsystime)
        except serialhdl.error as e:
            raise error(str(e))

class MCU:
    error = error
    def __init__(self, config, clocksync):
        self._printer = config.get_printer()
        self._clocksync = clocksync
        self._reactor = self._printer.get_reactor()
        self._name = config.get_name()
        if self._name.startswith('mcu '):
            self._name = self._name[4:]
        self._printer.register_event_handler("klippy:connect", self._connect)
        self._printer.register_event_handler("klippy:shutdown", self._shutdown)
        self._printer.register_event_handler("klippy:disconnect",
                                             self._disconnect)
        # Serial port
        self._serialport = config.get('serial', '/dev/ttyS0')
        baud = 0
        if not (self._serialport.startswith("/dev/rpmsg_")
                or self._serialport.startswith("/tmp/klipper_host_")):
            baud = config.getint('baud', 250000, minval=2400)
        self._serial = serialhdl.SerialReader(
            self._reactor, self._serialport, baud)
        # Restarts
        self._restart_method = 'command'
        if baud:
            rmethods = {m: m for m in [None, 'arduino', 'command', 'rpi_usb']}
            self._restart_method = config.getchoice(
                'restart_method', rmethods, None)
        self._reset_cmd = self._config_reset_cmd = None
        self._emergency_stop_cmd = None
        self._is_shutdown = self._is_timeout = False
        self._shutdown_msg = ""
        # Config building
        self._printer.lookup_object('pins').register_chip(self._name, self)
        self._oid_count = 0
        self._config_callbacks = []
        self._init_cmds = []
        self._config_cmds = []
        self._pin_map = config.get('pin_map', None)
        self._custom = config.get('custom', '')
        self._mcu_freq = 0.
        # Move command queuing
        ffi_main, self._ffi_lib = chelper.get_ffi()
        self._max_stepper_error = config.getfloat(
            'max_stepper_error', 0.000025, minval=0.)
        self._move_count = 0
        self._stepqueues = []
        self._steppersync = None
        # Stats
        self._stats_sumsq_base = 0.
        self._mcu_tick_avg = 0.
        self._mcu_tick_stddev = 0.
        self._mcu_tick_awake = 0.
    # Serial callbacks
    def _handle_mcu_stats(self, params):
        count = params['count']
        tick_sum = params['sum']
        c = 1.0 / (count * self._mcu_freq)
        self._mcu_tick_avg = tick_sum * c
        tick_sumsq = params['sumsq'] * self._stats_sumsq_base
        self._mcu_tick_stddev = c * math.sqrt(count*tick_sumsq - tick_sum**2)
        self._mcu_tick_awake = tick_sum / self._mcu_freq
    def _handle_shutdown(self, params):
        if self._is_shutdown:
            return
        self._is_shutdown = True
        self._shutdown_msg = msg = params['static_string_id']
        logging.info("MCU '%s' %s: %s\n%s\n%s", self._name, params['#name'],
                     self._shutdown_msg, self._clocksync.dump_debug(),
                     self._serial.dump_debug())
        prefix = "MCU '%s' shutdown: " % (self._name,)
        if params['#name'] == 'is_shutdown':
            prefix = "Previous MCU '%s' shutdown: " % (self._name,)
        self._printer.invoke_async_shutdown(prefix + msg + error_help(msg))
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
    def _add_custom(self):
        for line in self._custom.split('\n'):
            line = line.strip()
            cpos = line.find('#')
            if cpos >= 0:
                line = line[:cpos].strip()
            if not line:
                continue
            self.add_config_cmd(line)
    def _send_config(self, prev_crc):
        # Build config commands
        for cb in self._config_callbacks:
            cb()
        self._add_custom()
        self._config_cmds.insert(0, "allocate_oids count=%d" % (
            self._oid_count,))
        # Resolve pin names
        mcu_type = self._serial.get_msgparser().get_constant('MCU')
        ppins = self._printer.lookup_object('pins')
        reserved_pins = ppins.get_reserved_pins(self._name)
        pin_resolver = pins.PinResolver(mcu_type, reserved_pins)
        if self._pin_map is not None:
            pin_resolver.update_aliases(self._pin_map)
        for i, cmd in enumerate(self._config_cmds):
            self._config_cmds[i] = pin_resolver.update_command(cmd)
        for i, cmd in enumerate(self._init_cmds):
            self._init_cmds[i] = pin_resolver.update_command(cmd)
        # Calculate config CRC
        config_crc = zlib.crc32('\n'.join(self._config_cmds)) & 0xffffffff
        self.add_config_cmd("finalize_config crc=%d" % (config_crc,))
        # Transmit config messages (if needed)
        if prev_crc is None:
            logging.info("Sending MCU '%s' printer configuration...",
                         self._name)
            for c in self._config_cmds:
                self._serial.send(c)
        elif config_crc != prev_crc:
            self._check_restart("CRC mismatch")
            raise error("MCU '%s' CRC does not match config" % (self._name,))
        # Transmit init messages
        for c in self._init_cmds:
            self._serial.send(c)
    def _send_get_config(self):
        get_config_cmd = self.lookup_command("get_config")
        if self.is_fileoutput():
            return { 'is_config': 0, 'move_count': 500, 'crc': 0 }
        config_params = get_config_cmd.send_with_response(response='config')
        if self._is_shutdown:
            raise error("MCU '%s' error during config: %s" % (
                self._name, self._shutdown_msg))
        if config_params['is_shutdown']:
            raise error("Can not update MCU '%s' config as it is shutdown" % (
                self._name,))
        return config_params
    def _check_config(self):
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
                raise error("Failed automated reset of MCU '%s'" % (
                    self._name,))
            # Already configured - send init commands
            self._send_config(config_params['crc'])
        # Setup steppersync with the move_count returned by get_config
        self._move_count = config_params['move_count']
        self._steppersync = self._ffi_lib.steppersync_alloc(
            self._serial.serialqueue, self._stepqueues, len(self._stepqueues),
            self._move_count)
        self._ffi_lib.steppersync_set_time(
            self._steppersync, 0., self._mcu_freq)
    def _connect(self):
        if self.is_fileoutput():
            self._connect_file()
        else:
            if (self._restart_method == 'rpi_usb'
                and not os.path.exists(self._serialport)):
                # Try toggling usb power
                self._check_restart("enable power")
            try:
                self._serial.connect()
                self._clocksync.connect(self._serial)
            except serialhdl.error as e:
                raise error(str(e))
        msgparser = self._serial.get_msgparser()
        name = self._name
        log_info = [
            "Loaded MCU '%s' %d commands (%s / %s)" % (
                name, len(msgparser.messages_by_id),
                msgparser.version, msgparser.build_versions),
            "MCU '%s' config: %s" % (name, " ".join(
                ["%s=%s" % (k, v) for k, v in self.get_constants().items()]))]
        logging.info("\n".join(log_info))
        ppins = self._printer.lookup_object('pins')
        for cname, value in self.get_constants().items():
            if cname.startswith("RESERVE_PINS_"):
                for pin in value.split(','):
                    ppins.reserve_pin(name, pin, cname[13:])
        self._mcu_freq = self.get_constant_float('CLOCK_FREQ')
        self._stats_sumsq_base = self.get_constant_float('STATS_SUMSQ_BASE')
        self._emergency_stop_cmd = self.lookup_command("emergency_stop")
        self._reset_cmd = self.try_lookup_command("reset")
        self._config_reset_cmd = self.try_lookup_command("config_reset")
        if (self._restart_method is None
            and (self._reset_cmd is not None
                 or self._config_reset_cmd is not None)
            and msgparser.get_constant('SERIAL_BAUD', None) is None):
            self._restart_method = 'command'
        self.register_response(self._handle_shutdown, 'shutdown')
        self.register_response(self._handle_shutdown, 'is_shutdown')
        self.register_response(self._handle_mcu_stats, 'stats')
        self._check_config()
        move_msg = "Configured MCU '%s' (%d moves)" % (name, self._move_count)
        logging.info(move_msg)
        log_info.append(move_msg)
        self._printer.set_rollover_info(name, "\n".join(log_info), log=False)
    # Config creation helpers
    def setup_pin(self, pin_type, pin_params):
        pcs = {'stepper': MCU_stepper, 'endstop': MCU_endstop,
               'digital_out': MCU_digital_out, 'pwm': MCU_pwm, 'adc': MCU_adc}
        if pin_type not in pcs:
            raise pins.error("pin type %s not supported on mcu" % (pin_type,))
        return pcs[pin_type](self, pin_params)
    def create_oid(self):
        self._oid_count += 1
        return self._oid_count - 1
    def register_config_callback(self, cb):
        self._config_callbacks.append(cb)
    def add_config_cmd(self, cmd, is_init=False):
        if is_init:
            self._init_cmds.append(cmd)
        else:
            self._config_cmds.append(cmd)
    def get_query_slot(self, oid):
        slot = self.seconds_to_clock(oid * .01)
        t = int(self.estimated_print_time(self._reactor.monotonic()) + 1.5)
        return self.print_time_to_clock(t) + slot
    def register_stepqueue(self, stepqueue):
        self._stepqueues.append(stepqueue)
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
        if cq is None:
            cq = self._serial.get_default_command_queue()
        cmd = self._serial.get_msgparser().lookup_command(msgformat)
        return CommandWrapper(self, self._serial, self._clocksync, cmd, cq)
    def try_lookup_command(self, msgformat):
        try:
            return self.lookup_command(msgformat)
        except self._serial.get_msgparser().error as e:
            return None
    def lookup_command_id(self, msgformat):
        return self._serial.get_msgparser().lookup_command(msgformat).msgid
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
    def get_adjusted_freq(self):
        return self._clocksync.get_adjusted_freq()
    def clock32_to_clock64(self, clock32):
        return self._clocksync.clock32_to_clock64(clock32)
    # Restarts
    def _disconnect(self):
        self._serial.disconnect()
        if self._steppersync is not None:
            self._ffi_lib.steppersync_free(self._steppersync)
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
    def microcontroller_restart(self):
        if self._restart_method == 'rpi_usb':
            self._restart_rpi_usb()
        elif self._restart_method == 'command':
            self._restart_via_command()
        else:
            self._restart_arduino()
    # Misc external commands
    def is_fileoutput(self):
        return self._printer.get_start_args().get('debugoutput') is not None
    def is_shutdown(self):
        return self._is_shutdown
    def flush_moves(self, print_time):
        if self._steppersync is None:
            return
        clock = self.print_time_to_clock(print_time)
        if clock < 0:
            return
        ret = self._ffi_lib.steppersync_flush(self._steppersync, clock)
        if ret:
            raise error("Internal error in MCU '%s' stepcompress" % (
                self._name,))
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
    def stats(self, eventtime):
        msg = "%s: mcu_awake=%.03f mcu_task_avg=%.06f mcu_task_stddev=%.06f" % (
            self._name, self._mcu_tick_awake, self._mcu_tick_avg,
            self._mcu_tick_stddev)
        return False, ' '.join([msg, self._serial.stats(eventtime),
                                self._clocksync.stats(eventtime)])
    def __del__(self):
        self._disconnect()

Common_MCU_errors = {
    ("Timer too close", "No next step", "Missed scheduling of next "): """
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
