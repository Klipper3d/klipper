# Second Order Sections Filter
#
# Copyright (C) 2025 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

MAX_INT32 = (2 ** 31)
MIN_INT32 = -(2 ** 31) - 1
def assert_is_int32(value, frac_bits):
    if value > MAX_INT32 or value < MIN_INT32:
        raise OverflowError("Fixed point Q%d.%d overflow"
                            % (31-frac_bits, frac_bits))
    return value

# convert a floating point value to a 32 bit fixed point representation
# checks for overflow
def to_fixed_32(value, frac_bits):
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


# Control an `sos_filter` object on the MCU
class MCU_SosFilter:
    # max_sections should be the largest number of sections you expect
    # to use at runtime.
    def __init__(self, mcu, cmd_queue, max_sections, coeff_frac_bits=29):
        self._mcu = mcu
        self._cmd_queue = cmd_queue
        self._oid = self._mcu.create_oid()
        self._max_sections = max_sections
        self._coeff_frac_bits = coeff_frac_bits
        self._value_frac_bits = self._scale_frac_bits = 0
        self._design = None
        self._offset = 0
        self._scale = 1
        self._set_section_cmd = self._set_state_cmd = None
        self._set_active_cmd = self._set_offset_scale_cmd = None
        self._last_sent_coeffs = [None] * self._max_sections
        self._last_sent_offset_scale = None
        self._mcu.add_config_cmd("config_sos_filter oid=%d max_sections=%d"
                                 % (self._oid, self._max_sections))
        self._mcu.register_config_callback(self._build_config)

    def _validate_frac_bits(self, frac_bits):
        if frac_bits < 0 or frac_bits > 31:
            raise ValueError("The number of fractional bits (%i) must be a"
                             " value between 0 and 31" % (frac_bits,))
        return frac_bits

    def _build_config(self):
        self._set_section_cmd = self._mcu.lookup_command(
            "sos_filter_set_section oid=%c section_idx=%c"
            " sos0=%i sos1=%i sos2=%i sos3=%i sos4=%i", cq=self._cmd_queue)
        self._set_state_cmd = self._mcu.lookup_command(
            "sos_filter_set_state oid=%c section_idx=%c state0=%i state1=%i",
            cq=self._cmd_queue)
        self._set_offset_scale_cmd = self._mcu.lookup_command(
            "sos_filter_set_offset_scale oid=%c offset=%i"
            " scale=%i scale_frac_bits=%c", cq=self._cmd_queue)
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
                fixed_state.append(to_fixed_32(value, self._value_frac_bits))
            sos_state.append(fixed_state)
        return sos_state

    # Set conversion of a raw value 1 to a 1.0 value processed by sos filter
    def set_offset_scale(self, offset, scale, scale_frac_bits=0,
                         value_frac_bits=0):
        self._offset = offset
        self._value_frac_bits = value_frac_bits
        scale_mult = scale * float(1 << value_frac_bits)
        self._scale = to_fixed_32(scale_mult, scale_frac_bits)
        self._scale_frac_bits = scale_frac_bits

    # Change the filter coefficients and state at runtime
    def set_filter_design(self, design):
        self._design = design

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
        args = (self._oid, self._offset, self._scale, self._scale_frac_bits)
        if args != self._last_sent_offset_scale:
            self._set_offset_scale_cmd.send(args)
            self._last_sent_offset_scale = args
        # Activate filter
        self._set_active_cmd.send([self._oid, num_sections,
                                   self._coeff_frac_bits])
