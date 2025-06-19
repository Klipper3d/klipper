# Second Order Sections Filter
#
# Copyright (C) 2025 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

MAX_INT32 = (2 ** 31)
MIN_INT32 = -(2 ** 31) - 1
def assert_is_int32(value, error):
    if value > MAX_INT32 or value < MIN_INT32:
        raise OverflowError(error)
    return value

# convert a floating point value to a 32 bit fixed point representation
# checks for overflow
def to_fixed_32(value, int_bits):
    fractional_bits = (32 - (1 + int_bits))
    fixed_val = int(value * (2 ** fractional_bits))
    return assert_is_int32(fixed_val, "Fixed point Q%i overflow"
                           % (int_bits,))


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
        except:
            raise cfg_error("DigitalFilter require the SciPy module")
        if highpass:
            self.filter_sections.append(
                self._butter(highpass, "highpass", highpass_order))
        if lowpass:
            self.filter_sections.append(
                self._butter(lowpass, "lowpass", lowpass_order))
        for notch_freq in notches:
            self.filter_sections.append(self._notch(notch_freq, notch_quality))
        if len(self.filter_sections) > 0:
            self.initial_state = signal.sosfilt_zi(self.filter_sections)

    def _butter(self, frequency, btype, order):
        import scipy.signal as signal
        return signal.butter(order, Wn=frequency, btype=btype,
            fs=self.sample_frequency, output='sos')[0]

    def _notch(self, freq, quality):
        import scipy.signal as signal
        b, a = signal.iirnotch(freq, Q=quality, fs=self.sample_frequency)
        return signal.tf2sos(b, a)[0]

    def get_filter_sections(self):
        return self.filter_sections

    def get_initial_state(self):
        return self.initial_state

# container that accepts SciPy formatted SOS filter data and converts it to a
# selected fixed point representation. This data could come from DigitalFilter,
# static data, config etc.
class FixedPointSosFilter:
    # filter_sections is an array of SciPy formatted SOS filter sections (sos)
    # initial_state is an array of SciPy formatted SOS state sections (zi)
    def __init__(self, filter_sections=None, initial_state=None,
                        coeff_int_bits=2, value_int_bits=15):
        filter_sections = [] if filter_sections is None else filter_sections
        initial_state = [] if initial_state is None else initial_state
        num_sections = len(filter_sections)
        num_state = len(initial_state)
        if num_state != num_sections:
            raise ValueError("The number of filter sections (%i) and state "
                             "sections (%i) must be equal" % (
                                 num_sections, num_state))
        self._coeff_int_bits = self._validate_int_bits(coeff_int_bits)
        self._value_int_bits = self._validate_int_bits(value_int_bits)
        self._filter = self._convert_filter(filter_sections)
        self._state = self._convert_state(initial_state)

    def get_filter_sections(self):
        return self._filter

    def get_initial_state(self):
        return self._state

    def get_coeff_int_bits(self):
        return self._coeff_int_bits

    def get_value_int_bits(self):
        return self._value_int_bits

    def get_num_sections(self):
        return len(self._filter)

    def _validate_int_bits(self, int_bits):
        if int_bits < 1 or int_bits > 30:
            raise ValueError("The number of integer bits (%i) must be a"
                             " value between 1 and 30" % (int_bits,))
        return int_bits

    # convert the SciPi SOS filters to fixed point format
    def _convert_filter(self, filter_sections):
        sos_fixed = []
        for section in filter_sections:
            nun_coeff = len(section)
            if nun_coeff != 6:
                raise ValueError("The number of filter coefficients is %i"
                                 ", must be 6" % (nun_coeff,))
            fixed_section = []
            for col, coeff in enumerate(section):
                if col != 3:  # omit column 3
                    fixed_coeff = to_fixed_32(coeff, self._coeff_int_bits)
                    fixed_section.append(fixed_coeff)
                elif coeff != 1.0:  # double check colum 3 is always 1.0
                    raise ValueError("Coefficient 3 is expected to be 1.0"
                                     " but was %f" % (coeff,))
            sos_fixed.append(fixed_section)
        return sos_fixed

    # convert the SOS filter state matrix (zi) to fixed point format
    def _convert_state(self, filter_state):
        sos_state = []
        for section in filter_state:
            nun_states = len(section)
            if nun_states != 2:
                raise ValueError(
                    "The number of state elements is %i, must be 2"
                    % (nun_states,))
            fixed_state = []
            for col, value in enumerate(section):
                fixed_state.append(to_fixed_32(value, self._value_int_bits))
            sos_state.append(fixed_state)
        return sos_state


# Control an `sos_filter` object on the MCU
class SosFilter:
    # fixed_point_filter should be an FixedPointSosFilter instance. A filter of
    # size 0 will create a passthrough filter.
    # max_sections should be the largest number of sections you expect
    # to use at runtime. The default is the size of the fixed_point_filter.
    def __init__(self, mcu, cmd_queue, fixed_point_filter, max_sections=None):
        self._mcu = mcu
        self._cmd_queue = cmd_queue
        self._oid = self._mcu.create_oid()
        self._filter = fixed_point_filter
        self._max_sections = max_sections
        if self._max_sections is None:
            self._max_sections = self._filter.get_num_sections()
        self._cmd_set_section = [
            "sos_filter_set_section oid=%d section_idx=%d"
                " sos0=%i sos1=%i sos2=%i sos3=%i sos4=%i",
            "sos_filter_set_section oid=%c section_idx=%c"
                " sos0=%i sos1=%i sos2=%i sos3=%i sos4=%i"]
        self._cmd_config_state = [
            "sos_filter_set_state oid=%d section_idx=%d state0=%i state1=%i",
            "sos_filter_set_state oid=%c section_idx=%c state0=%i state1=%i"]
        self._cmd_activate = [
            "sos_filter_set_active oid=%d n_sections=%d coeff_int_bits=%d",
            "sos_filter_set_active oid=%c n_sections=%c coeff_int_bits=%c"]
        self._mcu.register_config_callback(self._build_config)

    def _build_config(self):
        cmds = [self._cmd_set_section, self._cmd_config_state,
            self._cmd_activate]
        for cmd in cmds:
            cmd.append(self._mcu.lookup_command(cmd[1], cq=self._cmd_queue))

    def get_oid(self):
        return self._oid

    # create an uninitialized filter object
    def create_filter(self):
        self._mcu.add_config_cmd("config_sos_filter oid=%d max_sections=%d"
            % (self._oid, self._max_sections))
        self._configure_filter(is_init=True)

    # either setup an init command or send the command based on a flag
    def _cmd(self, command, args, is_init=False):
        if is_init:
            self._mcu.add_config_cmd(command[0] % args, is_init=True)
        else:
            command[2].send(args)

    def _set_filter_sections(self, is_init=False):
        for i, section in enumerate(self._filter.get_filter_sections()):
            args = (self._oid, i, section[0], section[1], section[2],
                    section[3], section[4])
            self._cmd(self._cmd_set_section, args, is_init)

    def _set_filter_state(self, is_init=False):
        for i, state in enumerate(self._filter.get_initial_state()):
            args = (self._oid, i, state[0], state[1])
            self._cmd(self._cmd_config_state, args, is_init)

    def _activate_filter(self, is_init=False):
        args = (self._oid, self._filter.get_num_sections(),
                self._filter.get_coeff_int_bits())
        self._cmd(self._cmd_activate, args, is_init)

    # configure the filter sections on the mcu
    # filters should be an array of filter sections in SciPi SOS format
    # sos_filter_state should be an array of zi filter state elements
    def _configure_filter(self, is_init=False):
        num_sections = self._filter.get_num_sections()
        if num_sections > self._max_sections:
            raise ValueError("Too many filter sections: %i, The max is %i"
                             % (num_sections, self._max_sections,))
        # convert to fixed point to find errors
        # no errors, state is accepted
        # configure MCU filter and activate
        self._set_filter_sections(is_init)
        self._set_filter_state(is_init,)
        self._activate_filter(is_init)

    # Change the filter coefficients and state at runtime
    # fixed_point_filter should be an FixedPointSosFilter instance
    # cq is an optional command queue to for command sequencing
    def change_filter(self, fixed_point_filter):
        self._filter = fixed_point_filter
        self._configure_filter(False)

    # Resets the filter state back to initial conditions at runtime
    # cq is an optional command queue to for command sequencing
    def reset_filter(self):
        self._set_filter_state(False)
        self._activate_filter(False)
