# Multi-processor safe interface to micro-controller
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, zlib, logging, math
import serialhdl, pins, chelper

class error(Exception):
    pass

def parse_pin_extras(pin, can_pullup=False):
    pullup = invert = 0
    if can_pullup and pin.startswith('^'):
        pullup = 1
        pin = pin[1:].strip()
    if pin.startswith('!'):
        invert = 1
        pin = pin[1:].strip()
    return pin, pullup, invert

STEPCOMPRESS_ERROR_RET = -989898989

class MCU_stepper:
    def __init__(self, mcu, step_pin, dir_pin):
        self._mcu = mcu
        self._oid = mcu.create_oid(self)
        self._step_pin, pullup, self._invert_step = parse_pin_extras(step_pin)
        self._dir_pin, pullup, self._invert_dir = parse_pin_extras(dir_pin)
        self.commanded_position = 0
        self._mcu_position_offset = 0
        self._mcu_freq = self._min_stop_interval = 0.
        self._reset_cmd = self._get_position_cmd = None
        self.ffi_lib = self._stepqueue = None
        self.print_to_mcu_time = mcu.print_to_mcu_time
    def set_min_stop_interval(self, min_stop_interval):
        self._min_stop_interval = min_stop_interval
    def build_config(self):
        self._mcu_freq = self._mcu.get_mcu_freq()
        max_error = self._mcu.get_max_stepper_error()
        min_stop_interval = max(0., self._min_stop_interval - max_error)
        self._mcu.add_config_cmd(
            "config_stepper oid=%d step_pin=%s dir_pin=%s"
            " min_stop_interval=TICKS(%.9f) invert_step=%d" % (
                self._oid, self._step_pin, self._dir_pin,
                min_stop_interval, self._invert_step))
        self._mcu.register_stepper(self)
        step_cmd = self._mcu.lookup_command(
            "queue_step oid=%c interval=%u count=%hu add=%hi")
        dir_cmd = self._mcu.lookup_command(
            "set_next_step_dir oid=%c dir=%c")
        self._reset_cmd = self._mcu.lookup_command(
            "reset_step_clock oid=%c clock=%u")
        self._get_position_cmd = self._mcu.lookup_command(
            "stepper_get_position oid=%c")
        ffi_main, self.ffi_lib = chelper.get_ffi()
        max_error = int(max_error * self._mcu_freq)
        self._stepqueue = ffi_main.gc(self.ffi_lib.stepcompress_alloc(
            max_error, step_cmd.msgid, dir_cmd.msgid,
            self._invert_dir, self._oid),
                                      self.ffi_lib.stepcompress_free)
    def get_oid(self):
        return self._oid
    def set_position(self, pos):
        self._mcu_position_offset += self.commanded_position - pos
        self.commanded_position = pos
    def get_mcu_position(self):
        return self.commanded_position + self._mcu_position_offset
    def note_homing_start(self, homing_clock):
        ret = self.ffi_lib.stepcompress_set_homing(self._stepqueue, homing_clock)
        if ret:
            raise error("Internal error in stepcompress")
    def note_homing_finalized(self):
        ret = self.ffi_lib.stepcompress_set_homing(self._stepqueue, 0)
        if ret:
            raise error("Internal error in stepcompress")
        ret = self.ffi_lib.stepcompress_reset(self._stepqueue, 0)
        if ret:
            raise error("Internal error in stepcompress")
    def note_homing_triggered(self):
        params = self._mcu.serial.send_with_response(
            self._get_position_cmd.encode(self._oid),
            'stepper_position', self._oid)
        pos = params['pos']
        if self._invert_dir:
            pos = -pos
        self._mcu_position_offset = pos - self.commanded_position
    def reset_step_clock(self, mcu_time):
        clock = int(mcu_time * self._mcu_freq)
        ret = self.ffi_lib.stepcompress_reset(self._stepqueue, clock)
        if ret:
            raise error("Internal error in stepcompress")
        data = (self._reset_cmd.msgid, self._oid, clock & 0xffffffff)
        ret = self.ffi_lib.stepcompress_queue_msg(
            self._stepqueue, data, len(data))
        if ret:
            raise error("Internal error in stepcompress")
    def step(self, mcu_time, sdir):
        clock = mcu_time * self._mcu_freq
        ret = self.ffi_lib.stepcompress_push(self._stepqueue, clock, sdir)
        if ret:
            raise error("Internal error in stepcompress")
        if sdir:
            self.commanded_position += 1
        else:
            self.commanded_position -= 1
    def step_sqrt(self, mcu_time, steps, step_offset, sqrt_offset, factor):
        clock = mcu_time * self._mcu_freq
        mcu_freq2 = self._mcu_freq**2
        count = self.ffi_lib.stepcompress_push_sqrt(
            self._stepqueue, steps, step_offset, clock
            , sqrt_offset * mcu_freq2, factor * mcu_freq2)
        if count == STEPCOMPRESS_ERROR_RET:
            raise error("Internal error in stepcompress")
        self.commanded_position += count
        return count
    def step_factor(self, mcu_time, steps, step_offset, factor):
        clock = mcu_time * self._mcu_freq
        count = self.ffi_lib.stepcompress_push_factor(
            self._stepqueue, steps, step_offset, clock, factor * self._mcu_freq)
        if count == STEPCOMPRESS_ERROR_RET:
            raise error("Internal error in stepcompress")
        self.commanded_position += count
        return count
    def step_delta_const(self, mcu_time, dist, start_pos
                         , inv_velocity, step_dist
                         , height, closestxy_d, closest_height2, movez_r):
        clock = mcu_time * self._mcu_freq
        count = self.ffi_lib.stepcompress_push_delta_const(
            self._stepqueue, clock, dist, start_pos
            , inv_velocity * self._mcu_freq, step_dist
            , height, closestxy_d, closest_height2, movez_r)
        if count == STEPCOMPRESS_ERROR_RET:
            raise error("Internal error in stepcompress")
        self.commanded_position += count
        return count
    def step_delta_accel(self, mcu_time, dist, start_pos
                         , accel_multiplier, step_dist
                         , height, closestxy_d, closest_height2, movez_r):
        clock = mcu_time * self._mcu_freq
        mcu_freq2 = self._mcu_freq**2
        count = self.ffi_lib.stepcompress_push_delta_accel(
            self._stepqueue, clock, dist, start_pos
            , accel_multiplier * mcu_freq2, step_dist
            , height, closestxy_d, closest_height2, movez_r)
        if count == STEPCOMPRESS_ERROR_RET:
            raise error("Internal error in stepcompress")
        self.commanded_position += count
        return count

class MCU_endstop:
    error = error
    RETRY_QUERY = 1.000
    def __init__(self, mcu, pin, stepper):
        self._mcu = mcu
        self._oid = mcu.create_oid(self)
        self._stepper = stepper
        self._pin, self._pullup, self._invert = parse_pin_extras(
            pin, can_pullup=True)
        self._cmd_queue = mcu.alloc_command_queue()
        self._home_cmd = self._query_cmd = None
        self._homing = False
        self._min_query_time = self._mcu_freq = 0.
        self._next_query_clock = self._home_timeout_clock = 0
        self._retry_query_ticks = 0
        self._last_state = {}
        self.print_to_mcu_time = mcu.print_to_mcu_time
    def build_config(self):
        self._mcu_freq = self._mcu.get_mcu_freq()
        self._mcu.add_config_cmd(
            "config_end_stop oid=%d pin=%s pull_up=%d stepper_oid=%d" % (
                self._oid, self._pin, self._pullup, self._stepper.get_oid()))
        self._retry_query_ticks = int(self._mcu_freq * self.RETRY_QUERY)
        self._home_cmd = self._mcu.lookup_command(
            "end_stop_home oid=%c clock=%u rest_ticks=%u pin_value=%c")
        self._query_cmd = self._mcu.lookup_command("end_stop_query oid=%c")
        self._mcu.register_msg(self._handle_end_stop_state, "end_stop_state"
                               , self._oid)
    def home_start(self, mcu_time, rest_time):
        clock = int(mcu_time * self._mcu_freq)
        rest_ticks = int(rest_time * self._mcu_freq)
        self._homing = True
        self._min_query_time = self._mcu.monotonic()
        self._next_query_clock = clock + self._retry_query_ticks
        msg = self._home_cmd.encode(
            self._oid, clock, rest_ticks, 1 ^ self._invert)
        self._mcu.send(msg, reqclock=clock, cq=self._cmd_queue)
        self._stepper.note_homing_start(clock)
    def home_finalize(self, mcu_time):
        self._stepper.note_homing_finalized()
        self._home_timeout_clock = int(mcu_time * self._mcu_freq)
    def home_wait(self):
        eventtime = self._mcu.monotonic()
        while self._check_busy(eventtime):
            eventtime = self._mcu.pause(eventtime + 0.1)
    def _handle_end_stop_state(self, params):
        logging.debug("end_stop_state %s" % (params,))
        self._last_state = params
    def _check_busy(self, eventtime):
        # Check if need to send an end_stop_query command
        if self._mcu.is_fileoutput():
            return False
        last_sent_time = self._last_state.get('#sent_time', -1.)
        if last_sent_time >= self._min_query_time:
            if not self._homing:
                return False
            if not self._last_state.get('homing', 0):
                self._stepper.note_homing_triggered()
                self._homing = False
                return False
            if (self._mcu.serial.get_clock(last_sent_time)
                > self._home_timeout_clock):
                # Timeout - disable endstop checking
                msg = self._home_cmd.encode(self._oid, 0, 0, 0)
                self._mcu.send(msg, reqclock=0, cq=self._cmd_queue)
                raise error("Timeout during endstop homing")
        if self._mcu.is_shutdown:
            raise error("MCU is shutdown")
        last_clock, last_clock_time = self._mcu.get_last_clock()
        if last_clock >= self._next_query_clock:
            self._next_query_clock = last_clock + self._retry_query_ticks
            msg = self._query_cmd.encode(self._oid)
            self._mcu.send(msg, cq=self._cmd_queue)
        return True
    def query_endstop(self, mcu_time):
        clock = int(mcu_time * self._mcu_freq)
        self._homing = False
        self._min_query_time = self._mcu.monotonic()
        self._next_query_clock = clock
    def query_endstop_wait(self):
        eventtime = self._mcu.monotonic()
        while self._check_busy(eventtime):
            eventtime = self._mcu.pause(eventtime + 0.1)
        return self._last_state.get('pin', self._invert) ^ self._invert

class MCU_digital_out:
    def __init__(self, mcu, pin, max_duration):
        self._mcu = mcu
        self._oid = mcu.create_oid(self)
        pin, pullup, self._invert = parse_pin_extras(pin)
        self._last_clock = 0
        self._last_value = None
        self._mcu_freq = 0.
        self._cmd_queue = mcu.alloc_command_queue()
        mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s default_value=%d"
            " max_duration=TICKS(%f)" % (
                self._oid, pin, self._invert, max_duration))
        self._set_cmd = None
        self.print_to_mcu_time = mcu.print_to_mcu_time
    def build_config(self):
        self._mcu_freq = self._mcu.get_mcu_freq()
        self._set_cmd = self._mcu.lookup_command(
            "schedule_digital_out oid=%c clock=%u value=%c")
    def set_digital(self, mcu_time, value):
        clock = int(mcu_time * self._mcu_freq)
        msg = self._set_cmd.encode(self._oid, clock, value ^ self._invert)
        self._mcu.send(msg, minclock=self._last_clock, reqclock=clock
                      , cq=self._cmd_queue)
        self._last_clock = clock
        self._last_value = value
    def get_last_setting(self):
        return self._last_value
    def set_pwm(self, mcu_time, value):
        dval = 0
        if value >= 0.5:
            dval = 1
        self.set_digital(mcu_time, dval)

class MCU_pwm:
    PWM_MAX = 255.
    def __init__(self, mcu, pin, cycle_time, hard_cycle_ticks, max_duration):
        self._mcu = mcu
        self._hard_cycle_ticks = hard_cycle_ticks
        self._oid = mcu.create_oid(self)
        pin, pullup, self._invert = parse_pin_extras(pin)
        self._last_clock = 0
        self._mcu_freq = 0.
        self._cmd_queue = mcu.alloc_command_queue()
        if hard_cycle_ticks:
            mcu.add_config_cmd(
                "config_pwm_out oid=%d pin=%s cycle_ticks=%d default_value=%d"
                " max_duration=TICKS(%f)" % (
                    self._oid, pin, hard_cycle_ticks, self._invert,
                    max_duration))
        else:
            mcu.add_config_cmd(
                "config_soft_pwm_out oid=%d pin=%s cycle_ticks=TICKS(%f)"
                " default_value=%d max_duration=TICKS(%f)" % (
                    self._oid, pin, cycle_time, self._invert, max_duration))
        self._set_cmd = None
        self.print_to_mcu_time = mcu.print_to_mcu_time
    def build_config(self):
        self._mcu_freq = self._mcu.get_mcu_freq()
        if self._hard_cycle_ticks:
            self._set_cmd = self._mcu.lookup_command(
                "schedule_pwm_out oid=%c clock=%u value=%c")
        else:
            self._set_cmd = self._mcu.lookup_command(
                "schedule_soft_pwm_out oid=%c clock=%u value=%c")
    def set_pwm(self, mcu_time, value):
        clock = int(mcu_time * self._mcu_freq)
        if self._invert:
            value = 1. - value
        value = int(value * self.PWM_MAX + 0.5)
        msg = self._set_cmd.encode(self._oid, clock, value)
        self._mcu.send(msg, minclock=self._last_clock, reqclock=clock
                      , cq=self._cmd_queue)
        self._last_clock = clock

class MCU_adc:
    def __init__(self, mcu, pin):
        self._mcu = mcu
        self._oid = mcu.create_oid(self)
        self._min_sample = self._max_sample = 0.
        self._sample_time = self._report_time = 0.
        self._sample_count = 0
        self._report_clock = 0
        self._callback = None
        self._inv_max_adc = 0.
        self._mcu_freq = 0.
        self._cmd_queue = mcu.alloc_command_queue()
        mcu.add_config_cmd("config_analog_in oid=%d pin=%s" % (self._oid, pin))
        self._query_cmd = None
        mcu.add_init_callback(self._init_callback)
        self._query_cmd = None
    def build_config(self):
        self._mcu_freq = self._mcu.get_mcu_freq()
        self._query_cmd = self._mcu.lookup_command(
            "query_analog_in oid=%c clock=%u sample_ticks=%u sample_count=%c"
            " rest_ticks=%u min_value=%hu max_value=%hu")
    def set_minmax(self, sample_time, sample_count, minval=0., maxval=1.):
        self._sample_time = sample_time
        self._sample_count = sample_count
        self._min_sample = minval
        self._max_sample = maxval
    def _init_callback(self):
        if not self._sample_count:
            return
        last_clock, last_clock_time = self._mcu.get_last_clock()
        clock = last_clock + int(self._mcu_freq * (1.0 + self._oid * 0.01)) # XXX
        sample_ticks = int(self._sample_time * self._mcu_freq)
        mcu_adc_max = self._mcu.serial.msgparser.get_constant_float("ADC_MAX")
        max_adc = self._sample_count * mcu_adc_max
        self._inv_max_adc = 1.0 / max_adc
        self._report_clock = int(self._report_time * self._mcu_freq)
        self._mcu.register_msg(self._handle_analog_in_state, "analog_in_state"
                               , self._oid)
        min_sample = int(self._min_sample * max_adc)
        max_sample = min(0xffff, int(math.ceil(self._max_sample * max_adc)))
        msg = self._query_cmd.encode(
            self._oid, clock, sample_ticks, self._sample_count
            , self._report_clock, min_sample, max_sample)
        self._mcu.send(msg, reqclock=clock, cq=self._cmd_queue)
    def _handle_analog_in_state(self, params):
        last_value = params['value'] * self._inv_max_adc
        next_clock = self._mcu.serial.translate_clock(params['next_clock'])
        last_read_time = (next_clock - self._report_clock) / self._mcu_freq
        if self._callback is not None:
            self._callback(last_read_time, last_value)
    def set_adc_callback(self, report_time, callback):
        self._report_time = report_time
        self._callback = callback

class MCU:
    error = error
    COMM_TIMEOUT = 3.5
    def __init__(self, printer, config):
        self._printer = printer
        self._config = config
        # Serial port
        baud = config.getint('baud', 250000)
        self._serialport = config.get('serial', '/dev/ttyS0')
        self.serial = serialhdl.SerialReader(
            printer.reactor, self._serialport, baud)
        self.is_shutdown = False
        self._shutdown_msg = ""
        self._is_fileoutput = False
        self._timeout_timer = printer.reactor.register_timer(
            self.timeout_handler)
        # Config building
        self._emergency_stop_cmd = self._clear_shutdown_cmd = None
        self._oids = []
        self._config_cmds = []
        self._config_crc = None
        self._init_callbacks = []
        self._pin_map = config.get('pin_map', None)
        self._custom = config.get('custom', '')
        # Move command queuing
        ffi_main, self.ffi_lib = chelper.get_ffi()
        self._max_stepper_error = config.getfloat('max_stepper_error', 0.000025)
        self._steppers = []
        self._steppersync = None
        # Print time to clock epoch calculations
        self._print_start_time = 0.
        self._mcu_freq = 0.
        # Stats
        self._stats_sumsq_base = 0.
        self._mcu_tick_avg = 0.
        self._mcu_tick_stddev = 0.
    def handle_mcu_stats(self, params):
        logging.debug("mcu stats: %s" % (params,))
        count = params['count']
        tick_sum = params['sum']
        c = 1.0 / (count * self._mcu_freq)
        self._mcu_tick_avg = tick_sum * c
        tick_sumsq = params['sumsq'] * self._stats_sumsq_base
        self._mcu_tick_stddev = c * math.sqrt(count*tick_sumsq - tick_sum**2)
    def handle_shutdown(self, params):
        if self.is_shutdown:
            return
        self.is_shutdown = True
        self._shutdown_msg = params['#msg']
        logging.info("%s: %s" % (params['#name'], self._shutdown_msg))
        self.serial.dump_debug()
        self._printer.note_shutdown(self._shutdown_msg)
    # Connection phase
    def connect(self):
        if not self._is_fileoutput:
            self.serial.connect()
            self._printer.reactor.update_timer(
                self._timeout_timer, self.monotonic() + self.COMM_TIMEOUT)
        self._mcu_freq = self.serial.msgparser.get_constant_float('CLOCK_FREQ')
        self._stats_sumsq_base = self.serial.msgparser.get_constant_float(
            'STATS_SUMSQ_BASE')
        self._emergency_stop_cmd = self.lookup_command("emergency_stop")
        self._clear_shutdown_cmd = self.lookup_command("clear_shutdown")
        self.register_msg(self.handle_shutdown, 'shutdown')
        self.register_msg(self.handle_shutdown, 'is_shutdown')
        self.register_msg(self.handle_mcu_stats, 'stats')
        self._build_config()
        self._send_config()
    def connect_file(self, debugoutput, dictionary, pace=False):
        self._is_fileoutput = True
        self.serial.connect_file(debugoutput, dictionary)
        if not pace:
            def dummy_set_print_start_time(eventtime):
                pass
            def dummy_get_print_buffer_time(eventtime, last_move_end):
                return 1.250
            self.set_print_start_time = dummy_set_print_start_time
            self.get_print_buffer_time = dummy_get_print_buffer_time
    def timeout_handler(self, eventtime):
        last_clock, last_clock_time = self.serial.get_last_clock()
        timeout = last_clock_time + self.COMM_TIMEOUT
        if eventtime < timeout:
            return timeout
        logging.info("Timeout with firmware (eventtime=%f last_status=%f)" % (
            eventtime, last_clock_time))
        self._printer.note_mcu_error("Lost communication with firmware")
        return self._printer.reactor.NEVER
    def disconnect(self):
        self.serial.disconnect()
        if self._steppersync is not None:
            self.ffi_lib.steppersync_free(self._steppersync)
            self._steppersync = None
    def stats(self, eventtime):
        return "%s mcu_task_avg=%.06f mcu_task_stddev=%.06f" % (
            self.serial.stats(eventtime),
            self._mcu_tick_avg, self._mcu_tick_stddev)
    def force_shutdown(self):
        self.send(self._emergency_stop_cmd.encode())
    def clear_shutdown(self):
        logging.info("Sending clear_shutdown command")
        self.send(self._clear_shutdown_cmd.encode())
    def microcontroller_restart(self):
        logging.info("Attempting a microcontroller reset")
        self.disconnect()
        serialhdl.arduino_reset(self._serialport, self._printer.reactor)
    def is_fileoutput(self):
        return self._is_fileoutput
    # Configuration phase
    def _add_custom(self):
        for line in self._custom.split('\n'):
            line = line.strip()
            cpos = line.find('#')
            if cpos >= 0:
                line = line[:cpos].strip()
            if not line:
                continue
            self.add_config_cmd(line)
    def _build_config(self):
        # Build config commands
        for oid in self._oids:
            oid.build_config()
        self._add_custom()
        self._config_cmds.insert(0, "allocate_oids count=%d" % (
            len(self._oids),))

        # Resolve pin names
        mcu = self.serial.msgparser.get_constant('MCU')
        pnames = pins.get_pin_map(mcu, self._pin_map)
        updated_cmds = []
        for cmd in self._config_cmds:
            try:
                updated_cmds.append(pins.update_command(
                    cmd, self._mcu_freq, pnames))
            except:
                raise self._config.error("Unable to translate pin name: %s" % (
                    cmd,))
        self._config_cmds = updated_cmds

        # Calculate config CRC
        self._config_crc = zlib.crc32('\n'.join(self._config_cmds)) & 0xffffffff
        self.add_config_cmd("finalize_config crc=%d" % (self._config_crc,))
    def _send_config(self):
        msg = self.create_command("get_config")
        if self._is_fileoutput:
            config_params = {
                'is_config': 0, 'move_count': 500, 'crc': self._config_crc}
        else:
            config_params = self.serial.send_with_response(msg, 'config')
        if not config_params['is_config']:
            # Send config commands
            logging.info("Sending printer configuration...")
            for c in self._config_cmds:
                self.send(self.create_command(c))
            if not self._is_fileoutput:
                config_params = self.serial.send_with_response(msg, 'config')
                if not config_params['is_config']:
                    if self.is_shutdown:
                        raise error("Firmware error during config: %s" % (
                            self._shutdown_msg,))
                    raise error("Unable to configure printer")
        if self._config_crc != config_params['crc']:
            if self._printer.get_startup_state() != 'firmware_restart':
                # Attempt a firmware restart to fix the CRC error
                logging.info(
                    "Printer CRC mismatch - attempting firmware restart")
                self._printer.request_exit('firmware_restart')
                self._printer.reactor.pause(0.100)
            raise error("Printer CRC does not match config")
        move_count = config_params['move_count']
        logging.info("Configured (%d moves)" % (move_count,))
        stepqueues = tuple(s._stepqueue for s in self._steppers)
        self._steppersync = self.ffi_lib.steppersync_alloc(
            self.serial.serialqueue, stepqueues, len(stepqueues), move_count)
        for cb in self._init_callbacks:
            cb()
    # Config creation helpers
    def create_oid(self, oid):
        self._oids.append(oid)
        return len(self._oids) - 1
    def add_config_cmd(self, cmd):
        self._config_cmds.append(cmd)
    def add_init_callback(self, callback):
        self._init_callbacks.append(callback)
    def register_msg(self, cb, msg, oid=None):
        self.serial.register_callback(cb, msg, oid)
    def register_stepper(self, stepper):
        self._steppers.append(stepper)
    def alloc_command_queue(self):
        return self.serial.alloc_command_queue()
    def lookup_command(self, msgformat):
        return self.serial.msgparser.lookup_command(msgformat)
    def create_command(self, msg):
        return self.serial.msgparser.create_command(msg)
    # Wrappers for mcu object creation
    def create_stepper(self, step_pin, dir_pin):
        return MCU_stepper(self, step_pin, dir_pin)
    def create_endstop(self, pin, stepper):
        return MCU_endstop(self, pin, stepper)
    def create_digital_out(self, pin, max_duration=2.):
        return MCU_digital_out(self, pin, max_duration)
    def create_pwm(self, pin, cycle_time, hard_cycle_ticks=0, max_duration=2.):
        if hard_cycle_ticks < 0:
            return MCU_digital_out(self, pin, max_duration)
        return MCU_pwm(self, pin, cycle_time, hard_cycle_ticks, max_duration)
    def create_adc(self, pin):
        return MCU_adc(self, pin)
    # Clock syncing
    def set_print_start_time(self, eventtime):
        clock = self.serial.get_clock(eventtime)
        logging.info("Synchronizing mcu clock at %.6f to %d" % (
            eventtime, clock))
        est_mcu_time = clock / self._mcu_freq
        self._print_start_time = est_mcu_time
    def get_print_buffer_time(self, eventtime, print_time):
        if self.is_shutdown:
            return 0.
        mcu_time = print_time + self._print_start_time
        est_mcu_time = self.serial.get_clock(eventtime) / self._mcu_freq
        return mcu_time - est_mcu_time
    def print_to_mcu_time(self, print_time):
        return print_time + self._print_start_time
    def get_mcu_freq(self):
        return self._mcu_freq
    def get_last_clock(self):
        return self.serial.get_last_clock()
    def get_max_stepper_error(self):
        return self._max_stepper_error
    # Move command queuing
    def send(self, cmd, minclock=0, reqclock=0, cq=None):
        self.serial.send(cmd, minclock, reqclock, cq=cq)
    def flush_moves(self, print_time):
        if self._steppersync is None:
            return
        mcu_time = print_time + self._print_start_time
        clock = int(mcu_time * self._mcu_freq)
        ret = self.ffi_lib.steppersync_flush(self._steppersync, clock)
        if ret:
            raise error("Internal error in stepcompress")
    def pause(self, waketime):
        return self._printer.reactor.pause(waketime)
    def monotonic(self):
        return self._printer.reactor.monotonic()
    def __del__(self):
        self.disconnect()
