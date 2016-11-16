# Multi-processor safe interface to micro-controller
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, zlib, logging, time, math
import serialhdl, pins, chelper

def parse_pin_extras(pin, can_pullup=False):
    pullup = invert = 0
    if can_pullup and pin.startswith('^'):
        pullup = 1
        pin = pin[1:].strip()
    if pin.startswith('!'):
        invert = 1
        pin = pin[1:].strip()
    return pin, pullup, invert

class MCU_stepper:
    def __init__(self, mcu, step_pin, dir_pin, min_stop_interval, max_error):
        self._mcu = mcu
        self._oid = mcu.create_oid()
        step_pin, pullup, invert_step = parse_pin_extras(step_pin)
        dir_pin, pullup, self._invert_dir = parse_pin_extras(dir_pin)
        self._mcu_freq = mcu.get_mcu_freq()
        min_stop_interval = int(min_stop_interval * self._mcu_freq)
        max_error = int(max_error * self._mcu_freq)
        mcu.add_config_cmd(
            "config_stepper oid=%d step_pin=%s dir_pin=%s"
            " min_stop_interval=%d invert_step=%d" % (
                self._oid, step_pin, dir_pin, min_stop_interval, invert_step))
        mcu.register_stepper(self)
        self._step_cmd = mcu.lookup_command(
            "queue_step oid=%c interval=%u count=%hu add=%hi")
        self._dir_cmd = mcu.lookup_command(
            "set_next_step_dir oid=%c dir=%c")
        self._reset_cmd = mcu.lookup_command(
            "reset_step_clock oid=%c clock=%u")
        ffi_main, self.ffi_lib = chelper.get_ffi()
        self._stepqueue = self.ffi_lib.stepcompress_alloc(
            max_error, self._step_cmd.msgid
            , self._dir_cmd.msgid, self._invert_dir, self._oid)
        self.print_to_mcu_time = mcu.print_to_mcu_time
    def get_oid(self):
        return self._oid
    def get_invert_dir(self):
        return self._invert_dir
    def note_stepper_stop(self):
        self.ffi_lib.stepcompress_reset(self._stepqueue, 0)
    def reset_step_clock(self, mcu_time):
        clock = int(mcu_time * self._mcu_freq)
        self.ffi_lib.stepcompress_reset(self._stepqueue, clock)
        data = (self._reset_cmd.msgid, self._oid, clock & 0xffffffff)
        self.ffi_lib.stepcompress_queue_msg(self._stepqueue, data, len(data))
    def step(self, mcu_time, sdir):
        clock = mcu_time * self._mcu_freq
        self.ffi_lib.stepcompress_push(self._stepqueue, clock, sdir)
    def step_sqrt(self, mcu_time, steps, step_offset, sqrt_offset, factor):
        clock = mcu_time * self._mcu_freq
        mcu_freq2 = self._mcu_freq**2
        return self.ffi_lib.stepcompress_push_sqrt(
            self._stepqueue, steps, step_offset, clock
            , sqrt_offset * mcu_freq2, factor * mcu_freq2)
    def step_factor(self, mcu_time, steps, step_offset, factor):
        clock = mcu_time * self._mcu_freq
        return self.ffi_lib.stepcompress_push_factor(
            self._stepqueue, steps, step_offset, clock, factor * self._mcu_freq)
    def step_delta_const(self, mcu_time, dist, step_dist, start_pos
                         , closest_height2, height, movez_r, inv_velocity):
        clock = mcu_time * self._mcu_freq
        return self.ffi_lib.stepcompress_push_delta_const(
            self._stepqueue, clock, dist, step_dist, start_pos
            , closest_height2, height, movez_r, inv_velocity * self._mcu_freq)
    def step_delta_accel(self, mcu_time, dist, step_dist, start_pos
                         , closest_height2, height, movez_r, accel_multiplier):
        clock = mcu_time * self._mcu_freq
        mcu_freq2 = self._mcu_freq**2
        return self.ffi_lib.stepcompress_push_delta_accel(
            self._stepqueue, clock, dist, step_dist, start_pos
            , closest_height2, height, movez_r, accel_multiplier * mcu_freq2)
    def get_errors(self):
        return self.ffi_lib.stepcompress_get_errors(self._stepqueue)

class MCU_endstop:
    RETRY_QUERY = 1.000
    def __init__(self, mcu, pin, stepper):
        self._mcu = mcu
        self._oid = mcu.create_oid()
        self._stepper = stepper
        stepper_oid = stepper.get_oid()
        pin, pullup, self._invert = parse_pin_extras(pin, can_pullup=True)
        self._cmd_queue = mcu.alloc_command_queue()
        mcu.add_config_cmd(
            "config_end_stop oid=%d pin=%s pull_up=%d stepper_oid=%d" % (
                self._oid, pin, pullup, stepper_oid))
        self._home_cmd = mcu.lookup_command(
            "end_stop_home oid=%c clock=%u rest_ticks=%u pin_value=%c")
        mcu.register_msg(self._handle_end_stop_state, "end_stop_state"
                         , self._oid)
        self._query_cmd = mcu.lookup_command("end_stop_query oid=%c")
        self._homing = False
        self._min_query_time = 0.
        self._next_query_clock = 0
        self._mcu_freq = mcu.get_mcu_freq()
        self._retry_query_ticks = int(self._mcu_freq * self.RETRY_QUERY)
        self._last_state = {}
        self.print_to_mcu_time = mcu.print_to_mcu_time
    def home(self, mcu_time, rest_time):
        clock = int(mcu_time * self._mcu_freq)
        rest_ticks = int(rest_time * self._mcu_freq)
        self._homing = True
        self._min_query_time = time.time()
        self._next_query_clock = clock + self._retry_query_ticks
        msg = self._home_cmd.encode(
            self._oid, clock, rest_ticks, 1 ^ self._invert)
        self._mcu.send(msg, reqclock=clock, cq=self._cmd_queue)
    def home_finalize(self):
        # XXX - this flushes the serial port of messages ready to be
        # sent, but doesn't flush messages if they had an unmet minclock
        self._mcu.serial.send_flush()
        self._stepper.note_stepper_stop()
    def _handle_end_stop_state(self, params):
        logging.debug("end_stop_state %s" % (params,))
        self._last_state = params
    def check_busy(self, eventtime):
        # Check if need to send an end_stop_query command
        if self._mcu.output_file_mode:
            return False
        if self._last_state.get('#sent_time', -1.) >= self._min_query_time:
            if not self._homing or not self._last_state.get('homing', 0):
                return False
        last_clock = self._mcu.get_last_clock()
        if last_clock >= self._next_query_clock:
            self._next_query_clock = last_clock + self._retry_query_ticks
            msg = self._query_cmd.encode(self._oid)
            self._mcu.send(msg, cq=self._cmd_queue)
        return True
    def get_last_position(self):
        pos = self._last_state.get('pos', 0)
        if self._stepper.get_invert_dir():
            return -pos
        return pos
    def query_endstop(self):
        self._homing = False
        self._min_query_time = time.time()
        self._next_query_clock = 0
    def get_last_triggered(self):
        return self._last_state.get('pin', self._invert) ^ self._invert

class MCU_digital_out:
    def __init__(self, mcu, pin, max_duration):
        self._mcu = mcu
        self._oid = mcu.create_oid()
        pin, pullup, self._invert = parse_pin_extras(pin)
        self._last_clock = 0
        self._last_value = None
        self._mcu_freq = mcu.get_mcu_freq()
        self._cmd_queue = mcu.alloc_command_queue()
        mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s default_value=%d"
            " max_duration=%d" % (self._oid, pin, self._invert, max_duration))
        self._set_cmd = mcu.lookup_command(
            "schedule_digital_out oid=%c clock=%u value=%c")
        self.print_to_mcu_time = mcu.print_to_mcu_time
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
        if value > 127:
            dval = 1
        self.set_digital(mcu_time, dval)

class MCU_pwm:
    def __init__(self, mcu, pin, cycle_ticks, max_duration, hard_pwm=True):
        self._mcu = mcu
        self._oid = mcu.create_oid()
        self._last_clock = 0
        self._mcu_freq = mcu.get_mcu_freq()
        self._cmd_queue = mcu.alloc_command_queue()
        if hard_pwm:
            mcu.add_config_cmd(
                "config_pwm_out oid=%d pin=%s cycle_ticks=%d default_value=0"
                " max_duration=%d" % (self._oid, pin, cycle_ticks, max_duration))
            self._set_cmd = mcu.lookup_command(
                "schedule_pwm_out oid=%c clock=%u value=%c")
        else:
            mcu.add_config_cmd(
                "config_soft_pwm_out oid=%d pin=%s cycle_ticks=%d"
                " default_value=0 max_duration=%d" % (
                    self._oid, pin, cycle_ticks, max_duration))
            self._set_cmd = mcu.lookup_command(
                "schedule_soft_pwm_out oid=%c clock=%u value=%c")
        self.print_to_mcu_time = mcu.print_to_mcu_time
    def set_pwm(self, mcu_time, value):
        clock = int(mcu_time * self._mcu_freq)
        msg = self._set_cmd.encode(self._oid, clock, value)
        self._mcu.send(msg, minclock=self._last_clock, reqclock=clock
                      , cq=self._cmd_queue)
        self._last_clock = clock

class MCU_adc:
    ADC_MAX = 1024 # 10bit adc
    def __init__(self, mcu, pin):
        self._mcu = mcu
        self._oid = mcu.create_oid()
        self._min_sample = 0
        self._max_sample = 0xffff
        self._sample_ticks = 0
        self._sample_count = 1
        self._report_clock = 0
        self._callback = None
        self._inv_max_adc = 0.
        self._mcu_freq = mcu.get_mcu_freq()
        self._cmd_queue = mcu.alloc_command_queue()
        mcu.add_config_cmd("config_analog_in oid=%d pin=%s" % (self._oid, pin))
        mcu.register_msg(self._handle_analog_in_state, "analog_in_state"
                         , self._oid)
        self._query_cmd = mcu.lookup_command(
            "query_analog_in oid=%c clock=%u sample_ticks=%u sample_count=%c"
            " rest_ticks=%u min_value=%hu max_value=%hu")
    def set_minmax(self, sample_time, sample_count, minval=None, maxval=None):
        self._sample_ticks = int(sample_time * self._mcu_freq)
        self._sample_count = sample_count
        if minval is None:
            minval = 0
        if maxval is None:
            maxval = 0xffff
        max_adc = sample_count * self.ADC_MAX
        self._min_sample = int(minval * max_adc)
        self._max_sample = min(0xffff, int(math.ceil(maxval * max_adc)))
        self._inv_max_adc = 1.0 / max_adc
    def query_analog_in(self, report_time):
        self._report_clock = int(report_time * self._mcu_freq)
        cur_clock = self._mcu.get_last_clock()
        clock = cur_clock + int(self._mcu_freq * (1.0 + self._oid * 0.01)) # XXX
        msg = self._query_cmd.encode(
            self._oid, clock, self._sample_ticks, self._sample_count
            , self._report_clock, self._min_sample, self._max_sample)
        self._mcu.send(msg, reqclock=clock, cq=self._cmd_queue)
    def _handle_analog_in_state(self, params):
        last_value = params['value'] * self._inv_max_adc
        next_clock = self._mcu.serial.translate_clock(params['next_clock'])
        last_read_time = (next_clock - self._report_clock) / self._mcu_freq
        if self._callback is not None:
            self._callback(last_read_time, last_value)
    def set_adc_callback(self, cb):
        self._callback = cb

class MCU:
    def __init__(self, printer, config):
        self._printer = printer
        self._config = config
        # Serial port
        baud = config.getint('baud', 115200)
        serialport = config.get('serial', '/dev/ttyS0')
        self.serial = serialhdl.SerialReader(printer.reactor, serialport, baud)
        self.is_shutdown = False
        self.output_file_mode = False
        # Config building
        self._emergency_stop_cmd = None
        self._num_oids = 0
        self._config_cmds = []
        self._config_crc = None
        # Move command queuing
        ffi_main, self.ffi_lib = chelper.get_ffi()
        self._steppers = []
        self._steppersync = None
        # Print time to clock epoch calculations
        self._print_start_time = 0.
        self._mcu_freq = 0.
        # Stats
        self._mcu_tick_avg = 0.
        self._mcu_tick_stddev = 0.
    def handle_mcu_stats(self, params):
        logging.debug("mcu stats: %s" % (params,))
        count = params['count']
        tick_sum = params['sum']
        c = 1.0 / (count * self._mcu_freq)
        self._mcu_tick_avg = tick_sum * c
        tick_sumsq = params['sumsq']
        tick_sumavgsq = ((tick_sum // (256*count)) * count)**2
        self._mcu_tick_stddev = c * 256. * math.sqrt(
            count * tick_sumsq - tick_sumavgsq)
    def handle_shutdown(self, params):
        if self.is_shutdown:
            return
        self.is_shutdown = True
        logging.info("%s: %s" % (params['#name'], params['#msg']))
        self.serial.dump_debug()
        self._printer.shutdown()
    # Connection phase
    def _init_steppersync(self, count):
        stepqueues = tuple(s._stepqueue for s in self._steppers)
        self._steppersync = self.ffi_lib.steppersync_alloc(
            self.serial.serialqueue, stepqueues, len(stepqueues), count)
    def connect(self):
        def handle_serial_state(params):
            if params['#state'] == 'connected':
                self._printer.reactor.end()
        self.serial.register_callback(handle_serial_state, '#state')
        self.serial.connect()
        self._printer.reactor.run()
        self.serial.unregister_callback('#state')
        logging.info("serial connected")
        self._mcu_freq = float(self.serial.msgparser.config['CLOCK_FREQ'])
        self.register_msg(self.handle_shutdown, 'shutdown')
        self.register_msg(self.handle_shutdown, 'is_shutdown')
        self.register_msg(self.handle_mcu_stats, 'stats')
    def connect_file(self, debugoutput, dictionary, pace=False):
        self.output_file_mode = True
        self.serial.connect_file(debugoutput, dictionary)
        self._mcu_freq = float(self.serial.msgparser.config['CLOCK_FREQ'])
        def dummy_send_config():
            for c in self._config_cmds:
                self.send(self.create_command(c))
            self._init_steppersync(500)
        self._send_config = dummy_send_config
        if not pace:
            def dummy_set_print_start_time(eventtime):
                pass
            def dummy_get_print_buffer_time(eventtime, last_move_end):
                return 0.250
            self.set_print_start_time = dummy_set_print_start_time
            self.get_print_buffer_time = dummy_get_print_buffer_time
    def disconnect(self):
        self.serial.disconnect()
    def stats(self, eventtime):
        stats = self.serial.stats(eventtime)
        stats += " mcu_task_avg=%.06f mcu_task_stddev=%.06f" % (
            self._mcu_tick_avg, self._mcu_tick_stddev)
        err = 0
        for s in self._steppers:
            err += s.get_errors()
        if err:
            stats += " step_errors=%d" % (err,)
        return stats
    def force_shutdown(self):
        self.send(self._emergency_stop_cmd.encode())
    # Configuration phase
    def _add_custom(self):
        data = self._config.get('custom', '')
        for line in data.split('\n'):
            line = line.strip()
            cpos = line.find('#')
            if cpos >= 0:
                line = line[:cpos].strip()
            if not line:
                continue
            self.add_config_cmd(line)
    def build_config(self):
        self._emergency_stop_cmd = self.lookup_command("emergency_stop")
        # Build config commands
        self._add_custom()
        self._config_cmds.insert(0, "allocate_oids count=%d" % (
            self._num_oids,))

        # Resolve pin names
        mcu = self.serial.msgparser.config['MCU']
        pin_map = self._config.get('pin_map')
        if pin_map is None:
            pnames = pins.mcu_to_pins(mcu)
        else:
            pnames = pins.map_pins(pin_map, mcu)
        self._config_cmds = [pins.update_command(c, pnames)
                             for c in self._config_cmds]

        # Calculate config CRC
        self._config_crc = zlib.crc32('\n'.join(self._config_cmds)) & 0xffffffff
        self.add_config_cmd("finalize_config crc=%d" % (self._config_crc,))

        self._send_config()
    def _send_config(self):
        msg = self.create_command("get_config")
        config_params = {}
        sent_config = False
        def handle_get_config(params):
            config_params.update(params)
            done = not sent_config or params['is_config']
            if done:
                self._printer.reactor.end()
            return done
        while 1:
            self.serial.send_with_response(msg, handle_get_config, 'config')
            self._printer.reactor.run()
            if not config_params['is_config']:
                # Send config commands
                for c in self._config_cmds:
                    self.send(self.create_command(c))
                config_params.clear()
                sent_config = True
                continue
            if self._config_crc != config_params['crc']:
                logging.error("Printer CRC does not match config")
                sys.exit(1)
            break
        logging.info("Configured")
        self._init_steppersync(config_params['move_count'])
    # Config creation helpers
    def create_oid(self):
        oid = self._num_oids
        self._num_oids += 1
        return oid
    def add_config_cmd(self, cmd):
        self._config_cmds.append(cmd)
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
    def create_stepper(self, step_pin, dir_pin, min_stop_interval, max_error):
        return MCU_stepper(self, step_pin, dir_pin, min_stop_interval, max_error)
    def create_endstop(self, pin, stepper):
        return MCU_endstop(self, pin, stepper)
    def create_digital_out(self, pin, max_duration=2.):
        max_duration = int(max_duration * self._mcu_freq)
        return MCU_digital_out(self, pin, max_duration)
    def create_pwm(self, pin, hard_cycle_ticks, max_duration=2.):
        max_duration = int(max_duration * self._mcu_freq)
        if hard_cycle_ticks:
            return MCU_pwm(self, pin, hard_cycle_ticks, max_duration)
        if hard_cycle_ticks < 0:
            return MCU_digital_out(self, pin, max_duration)
        cycle_ticks = int(self._mcu_freq / 10.)
        return MCU_pwm(self, pin, cycle_ticks, max_duration, hard_pwm=False)
    def create_adc(self, pin):
        return MCU_adc(self, pin)
    # Clock syncing
    def set_print_start_time(self, eventtime):
        est_mcu_time = self.serial.get_clock(eventtime) / self._mcu_freq
        self._print_start_time = est_mcu_time
    def get_print_buffer_time(self, eventtime, print_time):
        mcu_time = print_time + self._print_start_time
        est_mcu_time = self.serial.get_clock(eventtime) / self._mcu_freq
        return mcu_time - est_mcu_time
    def print_to_mcu_time(self, print_time):
        return print_time + self._print_start_time
    def get_mcu_freq(self):
        return self._mcu_freq
    def get_last_clock(self):
        return self.serial.get_last_clock()
    # Move command queuing
    def send(self, cmd, minclock=0, reqclock=0, cq=None):
        self.serial.send(cmd, minclock, reqclock, cq=cq)
    def flush_moves(self, print_time):
        mcu_time = print_time + self._print_start_time
        clock = int(mcu_time * self._mcu_freq)
        self.ffi_lib.steppersync_flush(self._steppersync, clock)
