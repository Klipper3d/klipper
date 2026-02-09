# Wrapper around mcu trigger_analog objects
#
# Copyright (C) 2025 Gareth Farrington <gareth@waves.ky>
# Copyright (C) 2026  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import mcu


######################################################################
# SOS filters (Second Order Sectional)
######################################################################

MAX_INT32 = (2 ** 31)
MIN_INT32 = -(2 ** 31) - 1
def assert_is_int32(value, frac_bits):
    if value > MAX_INT32 or value < MIN_INT32:
        raise OverflowError("Fixed point Q%d.%d overflow"
                            % (31-frac_bits, frac_bits))
    return value

# convert a floating point value to a 32 bit fixed point representation
# checks for overflow
def to_fixed_32(value, frac_bits=0):
    fixed_val = int(value * (2**frac_bits))
    return assert_is_int32(fixed_val, frac_bits)


# Digital filter designer and container
class DigitalFilter:
    def __init__(self, sps, cfg_error, highpass=None, highpass_order=1,
            lowpass=None, lowpass_order=1, notches=None, notch_quality=2.0):
        self.filter_sections = []
        self.initial_state = []
        self.sample_frequency = sps
        # an empty filter can be created without SciPi/numpy
        if not (highpass or lowpass or notches):
            return
        try:
            import scipy.signal as signal
            import numpy
        except:
            raise cfg_error("DigitalFilter require the SciPy module")
        if highpass:
            self.filter_sections.extend(
                self._butter(highpass, "highpass", highpass_order))
        if lowpass:
            self.filter_sections.extend(
                self._butter(lowpass, "lowpass", lowpass_order))
        if notches is None:
            notches = []
        for notch_freq in notches:
            self.filter_sections.append(self._notch(notch_freq, notch_quality))
        if len(self.filter_sections) > 0:
            self.initial_state = signal.sosfilt_zi(self.filter_sections)

    def _butter(self, frequency, btype, order):
        import scipy.signal as signal
        return signal.butter(order, Wn=frequency, btype=btype,
            fs=self.sample_frequency, output='sos')

    def _notch(self, freq, quality):
        import scipy.signal as signal
        b, a = signal.iirnotch(freq, Q=quality, fs=self.sample_frequency)
        return signal.tf2sos(b, a)[0]

    def get_filter_sections(self):
        return self.filter_sections

    def get_initial_state(self):
        return self.initial_state

    def filtfilt(self, data):
        import scipy.signal as signal
        import numpy
        data = numpy.array(data)
        return signal.sosfiltfilt(self.filter_sections, data)

# Produce sample to sample difference (derivative) of a DigitalFilter
class DerivativeFilter:
    def __init__(self, main_filter):
        self._main_filter = main_filter

    def get_main_filter(self):
        return self._main_filter

    def get_filter_sections(self):
        s = list(self._main_filter.get_filter_sections())
        return s + [(1., -1., 0., 1., 0., 0.)]

    def get_initial_state(self):
        s = list(self._main_filter.get_initial_state())
        return s + [(-1., 0.)]

# Control an `sos_filter` object on the MCU
class MCU_SosFilter:
    # max_sections should be the largest number of sections you expect
    # to use at runtime.
    def __init__(self, mcu, cmd_queue, max_sections):
        self._mcu = mcu
        self._cmd_queue = cmd_queue
        self._max_sections = max_sections
        # SOS filter "design"
        self._design = None
        self._coeff_frac_bits = 0
        self._start_value = 0.
        # Offset and scaling
        self._offset = 0
        self._scale = 1.
        self._scale_frac_bits = 0
        self._auto_offset = False
        # MCU commands
        self._oid = self._mcu.create_oid()
        self._set_section_cmd = self._set_state_cmd = None
        self._set_active_cmd = self._set_offset_scale_cmd = None
        self._last_sent_coeffs = [None] * self._max_sections
        self._last_sent_offset_scale = None
        self._mcu.add_config_cmd("config_sos_filter oid=%d max_sections=%d"
                                 % (self._oid, self._max_sections))
        self._mcu.register_config_callback(self._build_config)

    def _build_config(self):
        self._set_section_cmd = self._mcu.lookup_command(
            "sos_filter_set_section oid=%c section_idx=%c"
            " sos0=%i sos1=%i sos2=%i sos3=%i sos4=%i", cq=self._cmd_queue)
        self._set_state_cmd = self._mcu.lookup_command(
            "sos_filter_set_state oid=%c section_idx=%c state0=%i state1=%i",
            cq=self._cmd_queue)
        self._set_offset_scale_cmd = self._mcu.lookup_command(
            "sos_filter_set_offset_scale oid=%c offset=%i"
            " scale=%i scale_frac_bits=%c auto_offset=%c", cq=self._cmd_queue)
        self._set_active_cmd = self._mcu.lookup_command(
            "sos_filter_set_active oid=%c n_sections=%c coeff_frac_bits=%c",
            cq=self._cmd_queue)

    def get_oid(self):
        return self._oid

    # convert the SciPi SOS filters to fixed point format
    def _convert_filter(self):
        if self._design is None:
            return []
        filter_sections = self._design.get_filter_sections()
        sos_fixed = []
        for section in filter_sections:
            nun_coeff = len(section)
            if nun_coeff != 6:
                raise ValueError("The number of filter coefficients is %i"
                                 ", must be 6" % (nun_coeff,))
            fixed_section = []
            for col, coeff in enumerate(section):
                if col != 3:  # omit column 3
                    fixed_coeff = to_fixed_32(coeff, self._coeff_frac_bits)
                    fixed_section.append(fixed_coeff)
                elif coeff != 1.0:  # double check column 3 is always 1.0
                    raise ValueError("Coefficient 3 is expected to be 1.0"
                                     " but was %f" % (coeff,))
            sos_fixed.append(fixed_section)
        return sos_fixed

    # convert the SOS filter state matrix (zi) to fixed point format
    def _convert_state(self):
        if self._design is None:
            return []
        filter_state = self._design.get_initial_state()
        sos_state = []
        for section in filter_state:
            nun_states = len(section)
            if nun_states != 2:
                raise ValueError(
                    "The number of state elements is %i, must be 2"
                    % (nun_states,))
            fixed_state = []
            for col, value in enumerate(section):
                adjval = value * self._start_value
                fixed_state.append(to_fixed_32(adjval))
            sos_state.append(fixed_state)
        return sos_state

    # Set expected state when filter first starts (avoids filter
    # "ringing" if sensor data has a known static offset)
    def set_start_state(self, start_value):
        self._start_value = start_value

    # Set conversion of a raw value 1 to a 1.0 value processed by sos filter
    def set_offset_scale(self, offset=0, scale=1., scale_frac_bits=0,
                         auto_offset=False):
        self._offset = offset
        self._scale = scale
        self._scale_frac_bits = scale_frac_bits
        self._auto_offset = auto_offset

    # Change the filter coefficients and state at runtime
    def set_filter_design(self, design, coeff_frac_bits=29):
        self._design = design
        self._coeff_frac_bits = coeff_frac_bits

    # Resets the filter state back to initial conditions at runtime
    def reset_filter(self):
        # Generate filter parameters
        sos_fixed = self._convert_filter()
        sos_state = self._convert_state()
        num_sections = len(sos_fixed)
        if num_sections > self._max_sections:
            raise ValueError("Too many filter sections: %i, The max is %i"
                             % (num_sections, self._max_sections,))
        if len(sos_state) != num_sections:
            raise ValueError("The number of filter sections (%i) and state "
                             "sections (%i) must be equal"
                             % (num_sections, len(sos_state)))
        # Send section coefficients (if they have changed)
        for i, section in enumerate(sos_fixed):
            args = (self._oid, i, section[0], section[1], section[2],
                    section[3], section[4])
            if args == self._last_sent_coeffs[i]:
                continue
            self._set_section_cmd.send(args)
            self._last_sent_coeffs[i] = args
        # Send section initial states
        for i, state in enumerate(sos_state):
            self._set_state_cmd.send([self._oid, i, state[0], state[1]])
        # Send offset/scale (if they have changed)
        su = to_fixed_32(self._scale, self._scale_frac_bits)
        args = (self._oid, self._offset, su, self._scale_frac_bits,
                self._auto_offset)
        if args != self._last_sent_offset_scale or self._auto_offset:
            self._set_offset_scale_cmd.send(args)
            self._last_sent_offset_scale = args
        # Activate filter
        if self._max_sections:
            self._set_active_cmd.send([self._oid, num_sections,
                                       self._coeff_frac_bits])


######################################################################
# Trigger Analog
######################################################################

# MCU_trigger_analog is the interface to `trigger_analog` on the MCU
class MCU_trigger_analog:
    MONITOR_MAX = 3
    REASON_TRIGGER_ANALOG = mcu.MCU_trsync.REASON_COMMS_TIMEOUT + 1
    def __init__(self, sensor_inst):
        self._printer = sensor_inst.get_mcu().get_printer()
        self._sensor = sensor_inst
        self._mcu = self._sensor.get_mcu()
        self._sos_filter = None
        self._dispatch = mcu.TriggerDispatch(self._mcu)
        self._last_trigger_time = 0.
        # Raw range checking
        self._raw_min = self._raw_max = 0
        self._last_range_args = None
        # Trigger type
        self._trigger_type = "unspecified"
        self._trigger_value = 0.
        self._last_trigger_args = None
        # Error codes from MCU
        self._error_map = {}
        self._sensor_specific_error = 0
        # Configure MCU objects
        self._oid = self._mcu.create_oid()
        self._home_cmd = self._query_state_cmd = None
        self._set_raw_range_cmd = self._set_trigger_cmd = None
        self._mcu.register_config_callback(self._build_config)

    def setup_sos_filter(self, sos_filter):
        self._sos_filter = sos_filter

    def _build_config(self):
        self._sensor.setup_trigger_analog(self._oid)
        cmd_queue = self._dispatch.get_command_queue()
        if self._sos_filter is None:
            self.setup_sos_filter(MCU_SosFilter(self._mcu, cmd_queue, 0))
        self._mcu.add_config_cmd(
            "config_trigger_analog oid=%d sos_filter_oid=%d" % (
                self._oid, self._sos_filter.get_oid()))
        # Lookup commands
        self._query_state_cmd = self._mcu.lookup_query_command(
            "trigger_analog_query_state oid=%c",
            "trigger_analog_state oid=%c homing=%c homing_clock=%u",
            oid=self._oid, cq=cmd_queue)
        self._set_raw_range_cmd = self._mcu.lookup_command(
            "trigger_analog_set_raw_range oid=%c raw_min=%i raw_max=%i",
            cq=cmd_queue)
        self._set_trigger_cmd = self._mcu.lookup_command(
            "trigger_analog_set_trigger oid=%c trigger_analog_type=%c"
            " trigger_value=%i", cq=cmd_queue)
        self._home_cmd = self._mcu.lookup_command(
            "trigger_analog_home oid=%c trsync_oid=%c trigger_reason=%c"
            " error_reason=%c clock=%u monitor_ticks=%u monitor_max=%u",
            cq=cmd_queue)
        # Load errors from mcu
        errors = self._mcu.get_enumerations().get("trigger_analog_error:", {})
        self._error_map = {v: k for k, v in errors.items()}
        self._sensor_specific_error = errors.get("SENSOR_SPECIFIC", 0)

    def get_oid(self):
        return self._oid

    def get_mcu(self):
        return self._mcu

    def get_sos_filter(self):
        return self._sos_filter

    def get_dispatch(self):
        return self._dispatch

    def get_last_trigger_time(self):
        return self._last_trigger_time

    def set_trigger(self, trigger_type, trigger_value):
        self._trigger_type = trigger_type
        self._trigger_value = trigger_value

    def set_raw_range(self, raw_min, raw_max):
        self._raw_min = raw_min
        self._raw_max = raw_max

    def _reset_filter(self):
        # Update raw range parameters in mcu (if they have changed)
        args = [self._oid, self._raw_min, self._raw_max]
        if args != self._last_range_args:
            self._set_raw_range_cmd.send(args)
            self._last_range_args = args
        # Update trigger in mcu (if it has changed)
        args = [self._oid, self._trigger_type, to_fixed_32(self._trigger_value)]
        if args != self._last_trigger_args:
            self._set_trigger_cmd.send(args)
            self._last_trigger_args = args
        # Update sos filter in mcu
        self._sos_filter.reset_filter()

    def _clear_home(self):
        self._home_cmd.send([self._oid, 0, 0, 0, 0, 0, 0, 0])
        params = self._query_state_cmd.send([self._oid])
        trigger_ticks = self._mcu.clock32_to_clock64(params['homing_clock'])
        return self._mcu.clock_to_print_time(trigger_ticks)

    def get_steppers(self):
        return self._dispatch.get_steppers()

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        self._last_trigger_time = 0.
        self._reset_filter()
        trigger_completion = self._dispatch.start(print_time)
        clock = self._mcu.print_time_to_clock(print_time)
        sensor_update = 1. / self._sensor.get_samples_per_second()
        sm_ticks = self._mcu.seconds_to_clock(sensor_update)
        self._home_cmd.send([self._oid, self._dispatch.get_oid(),
            mcu.MCU_trsync.REASON_ENDSTOP_HIT, self.REASON_TRIGGER_ANALOG,
            clock, sm_ticks, self.MONITOR_MAX], reqclock=clock)
        return trigger_completion

    def home_wait(self, home_end_time):
        self._dispatch.wait_end(home_end_time)
        # trigger has happened, now to find out why...
        res = self._dispatch.stop()
        # clear the homing state so it stops processing samples
        trigger_time = self._clear_home()
        if res >= mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
            if res == mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
                raise self._printer.command_error(
                    "Communication timeout during homing")
            error_code = res - self.REASON_TRIGGER_ANALOG
            if error_code >= self._sensor_specific_error:
                sensor_err = error_code - self._sensor_specific_error
                error_msg = self._sensor.lookup_sensor_error(sensor_err)
            else:
                defmsg = "Unknown code %i" % (error_code,)
                error_msg = self._error_map.get(error_code, defmsg)
            raise self._printer.command_error("Trigger analog error: %s"
                                              % (error_msg,))
        if res != mcu.MCU_trsync.REASON_ENDSTOP_HIT:
            return 0.
        if self._mcu.is_fileoutput():
            trigger_time = home_end_time
        self._last_trigger_time = trigger_time
        return trigger_time
