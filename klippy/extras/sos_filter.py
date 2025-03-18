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

Q1_INT_BITS = 1
Q1_FRAC_BITS = (32 - (1 + Q1_INT_BITS))
def as_fixed_q1(val):
    fixed_val = int(val * (2 ** Q1_FRAC_BITS))
    return assert_is_int32(fixed_val, "Fixed point Q1 overflow")

Q16_INT_BITS = 16
Q16_FRAC_BITS = (32 - (1 + Q16_INT_BITS))
def as_fixed_q16(val):
    fixed_val = int(val * (2 ** Q16_FRAC_BITS))
    return assert_is_int32(fixed_val, "Fixed point Q16 overflow")


#########################
## Config Parsing Helpers

# support for validating individual options in a config list
def getfloatlist(config, option, above=None, below=None, max_len=None):
    values = config.getfloatlist(option, default=[])
    if max_len is not None and len(values) > max_len:
        raise config.error("Option '%s' in section '%s' must have maximum"
                           " length %s" % (option, config.get_name(), max_len))
    for value in values:
        validatefloat(config, option, value, above, below)
    return values

def validatefloat(config, option, value, above, below):
    if above is not None and value <= above:
        raise config.error("Option '%s' in section '%s' must be above %s"
                    % (option, config.get_name(), above))
    if below is not None and value >= below:
        raise config.error("Option '%s' in section '%s' must be below %s"
                     % (option, config.get_name(), below))

# Digital filter designer and container
class DigitalFilter:
    def __init__(self, sps, cfg_error, highpass=None, lowpass=None,
                    notches=None, notch_quality=2.0):
        self.filter_sections = []
        self.sample_frequency = sps
        # an empty filter can be created without SciPi/numpy
        if not (highpass or lowpass or notches):
            return
        try:
            import scipy.signal as signal
        except:
            raise cfg_error("DigitalFilter require the SciPy module")
        if highpass:
            self.filter_sections.append(self._butter(highpass, "highpass"))
        for notch_freq in notches:
            self.filter_sections.append(self._notch(notch_freq, notch_quality))
        if lowpass:
            self.filter_sections.append(self._butter(lowpass, "lowpass"))

    def _butter(self, frequency, btype):
        import scipy.signal as signal
        return signal.butter(2, Wn=frequency, btype=btype,
                             fs=self.sample_frequency, output='sos')[0]

    def _notch(self, freq, quality):
        import scipy.signal as signal
        b, a = signal.iirnotch(freq, Q=quality, fs=self.sample_frequency)
        return signal.tf2sos(b, a)[0]

    def get_filter_sections(self):
        return self.filter_sections

# Control an sos_filter object on the MCU
class SosFilter:
    def __init__(self, mcu):
        self._mcu = mcu
        self.oid = self._mcu.create_oid()

    # create an uninitialized filter object
    def create_filter(self):
        self._mcu.add_config_cmd("config_sos_filter oid=%d"
                                 % (self.oid,))

    # convert the SciPi sos filters to fixed point Q1 format
    def _convert_filters_to_q1(self, filter_sections):
        num_sections = len(filter_sections)
        if num_sections > 4:
            raise ValueError("Too many filter sections: %i, The max is 4"
                             % (num_sections,))
        sos_fixed = []
        for section in filter_sections:
            nun_coeff = len(section)
            if nun_coeff != 6:
                raise ValueError("The number of filter coefficients %i"
                                 ", must be 6" % (nun_coeff,))
            fixed_section = []
            for col, coeff in enumerate(section):
                if col != 3:  # omit column 3
                    fixed_section.append(as_fixed_q1(coeff))
                elif coeff != 1.0: # double check colum 3 is always 1.0
                    raise ValueError("Coefficient 3 is expected to be 1.0"
                                     " but was %f" % (coeff,))

            sos_fixed.append(fixed_section)
        return sos_fixed

    # configure the filter sections on the mcu
    # filters should be an array of filter sections in SciPi SOS format
    def configure_filter(self, sos_filters):
        config_section = ("config_sos_filter_section oid=%d n_sections=%d"
               " section_idx=%d sos0=%i sos1=%i sos2=%i sos3=%i sos4=%i")
        if not sos_filters:
            # configure the empty filter
            args = (self.oid, 0, 0, 0, 0, 0, 0, 0)
            self._mcu.add_config_cmd(config_section % args)
        else:
            q1_filter_sections = self._convert_filters_to_q1(sos_filters)
            n_sections = len(q1_filter_sections)
            for i, section in enumerate(q1_filter_sections):
                args = (self.oid, n_sections, i, section[0], section[1],
                        section[2], section[3], section[4])
                self._mcu.add_config_cmd(config_section % args)
