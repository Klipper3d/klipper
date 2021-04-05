# Automatic calibration of input shapers
#
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections, importlib, logging, math, multiprocessing

MIN_FREQ = 5.
MAX_FREQ = 200.
WINDOW_T_SEC = 0.5
MAX_SHAPER_FREQ = 150.

SHAPER_VIBRATION_REDUCTION=20.
TEST_DAMPING_RATIOS=[0.075, 0.1, 0.15]
SHAPER_DAMPING_RATIO = 0.1

######################################################################
# Input shapers
######################################################################

InputShaperCfg = collections.namedtuple(
        'InputShaperCfg', ('name', 'init_func', 'min_freq'))

def get_zv_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)
    A = [1., K]
    T = [0., .5*t_d]
    return (A, T)

def get_zvd_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)
    A = [1., 2.*K, K**2]
    T = [0., .5*t_d, t_d]
    return (A, T)

def get_mzv_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-.75 * damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    a1 = 1. - 1. / math.sqrt(2.)
    a2 = (math.sqrt(2.) - 1.) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0., .375*t_d, .75*t_d]
    return (A, T)

def get_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1. / SHAPER_VIBRATION_REDUCTION # vibration tolerance
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    a1 = .25 * (1. + v_tol)
    a2 = .5 * (1. - v_tol) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0., .5*t_d, t_d]
    return (A, T)

def get_2hump_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1. / SHAPER_VIBRATION_REDUCTION # vibration tolerance
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    V2 = v_tol**2
    X = pow(V2 * (math.sqrt(1. - V2) + 1.), 1./3.)
    a1 = (3.*X*X + 2.*X + 3.*V2) / (16.*X)
    a2 = (.5 - a1) * K
    a3 = a2 * K
    a4 = a1 * K * K * K

    A = [a1, a2, a3, a4]
    T = [0., .5*t_d, t_d, 1.5*t_d]
    return (A, T)

def get_3hump_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1. / SHAPER_VIBRATION_REDUCTION # vibration tolerance
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    K2 = K*K
    a1 = 0.0625 * (1. + 3. * v_tol + 2. * math.sqrt(2. * (v_tol + 1.) * v_tol))
    a2 = 0.25 * (1. - v_tol) * K
    a3 = (0.5 * (1. + v_tol) - 2. * a1) * K2
    a4 = a2 * K2
    a5 = a1 * K2 * K2

    A = [a1, a2, a3, a4, a5]
    T = [0., .5*t_d, t_d, 1.5*t_d, 2.*t_d]
    return (A, T)

def get_shaper_smoothing(shaper, accel=5000, scv=5.):
    half_accel = accel * .5

    A, T = shaper
    inv_D = 1. / sum(A)
    n = len(T)
    # Calculate input shaper shift
    ts = sum([A[i] * T[i] for i in range(n)]) * inv_D

    # Calculate offset for 90 and 180 degrees turn
    offset_90 = offset_180 = 0.
    for i in range(n):
        if T[i] >= ts:
            # Calculate offset for one of the axes
            offset_90 += A[i] * (scv + half_accel * (T[i]-ts)) * (T[i]-ts)
        offset_180 += A[i] * half_accel * (T[i]-ts)**2
    offset_90 *= inv_D * math.sqrt(2.)
    offset_180 *= inv_D
    return max(offset_90, offset_180)

# min_freq for each shaper is chosen to have projected max_accel ~= 1500
INPUT_SHAPERS = [
    InputShaperCfg('zv', get_zv_shaper, min_freq=21.),
    InputShaperCfg('mzv', get_mzv_shaper, min_freq=23.),
    InputShaperCfg('ei', get_ei_shaper, min_freq=29.),
    InputShaperCfg('2hump_ei', get_2hump_ei_shaper, min_freq=39.),
    InputShaperCfg('3hump_ei', get_3hump_ei_shaper, min_freq=48.),
]

######################################################################
# Frequency response calculation and shaper auto-tuning
######################################################################

class CalibrationData:
    def __init__(self, freq_bins, psd_sum, psd_x, psd_y, psd_z):
        self.freq_bins = freq_bins
        self.psd_sum = psd_sum
        self.psd_x = psd_x
        self.psd_y = psd_y
        self.psd_z = psd_z
        self._psd_list = [self.psd_sum, self.psd_x, self.psd_y, self.psd_z]
        self._psd_map = {'x': self.psd_x, 'y': self.psd_y, 'z': self.psd_z,
                         'all': self.psd_sum}
        self.data_sets = 1
    def join(self, other):
        np = self.numpy
        joined_data_sets = self.data_sets + other.data_sets
        for psd, other_psd in zip(self._psd_list, other._psd_list):
            # `other` data may be defined at different frequency bins,
            # interpolating to fix that.
            other_normalized = other.data_sets * np.interp(
                    self.freq_bins, other.freq_bins, other_psd)
            psd *= self.data_sets
            psd[:] = (psd + other_normalized) * (1. / joined_data_sets)
        self.data_sets = joined_data_sets
    def set_numpy(self, numpy):
        self.numpy = numpy
    def normalize_to_frequencies(self):
        for psd in self._psd_list:
            # Avoid division by zero errors
            psd /= self.freq_bins + .1
            # Remove low-frequency noise
            psd[self.freq_bins < MIN_FREQ] = 0.
    def get_psd(self, axis='all'):
        return self._psd_map[axis]


CalibrationResult = collections.namedtuple(
        'CalibrationResult',
        ('name', 'freq', 'vals', 'vibrs', 'smoothing', 'score', 'max_accel'))

class ShaperCalibrate:
    def __init__(self, printer):
        self.printer = printer
        self.error = printer.command_error if printer else Exception
        try:
            self.numpy = importlib.import_module('numpy')
        except ImportError:
            raise self.error(
                    "Failed to import `numpy` module, make sure it was "
                    "installed via `~/klippy-env/bin/pip install` (refer to "
                    "docs/Measuring_Resonances.md for more details).")

    def background_process_exec(self, method, args):
        if self.printer is None:
            return method(*args)
        import queuelogger
        parent_conn, child_conn = multiprocessing.Pipe()
        def wrapper():
            queuelogger.clear_bg_logging()
            try:
                res = method(*args)
            except:
                child_conn.send((True, traceback.format_exc()))
                child_conn.close()
                return
            child_conn.send((False, res))
            child_conn.close()
        # Start a process to perform the calculation
        calc_proc = multiprocessing.Process(target=wrapper)
        calc_proc.daemon = True
        calc_proc.start()
        # Wait for the process to finish
        reactor = self.printer.get_reactor()
        gcode = self.printer.lookup_object("gcode")
        eventtime = last_report_time = reactor.monotonic()
        while calc_proc.is_alive():
            if eventtime > last_report_time + 5.:
                last_report_time = eventtime
                gcode.respond_info("Wait for calculations..", log=False)
            eventtime = reactor.pause(eventtime + .1)
        # Return results
        is_err, res = parent_conn.recv()
        if is_err:
            raise self.error("Error in remote calculation: %s" % (res,))
        calc_proc.join()
        parent_conn.close()
        return res

    def _split_into_windows(self, x, window_size, overlap):
        # Memory-efficient algorithm to split an input 'x' into a series
        # of overlapping windows
        step_between_windows = window_size - overlap
        n_windows = (x.shape[-1] - overlap) // step_between_windows
        shape = (window_size, n_windows)
        strides = (x.strides[-1], step_between_windows * x.strides[-1])
        return self.numpy.lib.stride_tricks.as_strided(
                x, shape=shape, strides=strides, writeable=False)

    def _psd(self, x, fs, nfft):
        # Calculate power spectral density (PSD) using Welch's algorithm
        np = self.numpy
        window = np.kaiser(nfft, 6.)
        # Compensation for windowing loss
        scale = 1.0 / (window**2).sum()

        # Split into overlapping windows of size nfft
        overlap = nfft // 2
        x = self._split_into_windows(x, nfft, overlap)

        # First detrend, then apply windowing function
        x = window[:, None] * (x - np.mean(x, axis=0))

        # Calculate frequency response for each window using FFT
        result = np.fft.rfft(x, n=nfft, axis=0)
        result = np.conjugate(result) * result
        result *= scale / fs
        # For one-sided FFT output the response must be doubled, except
        # the last point for unpaired Nyquist frequency (assuming even nfft)
        # and the 'DC' term (0 Hz)
        result[1:-1,:] *= 2.

        # Welch's algorithm: average response over windows
        psd = result.real.mean(axis=-1)

        # Calculate the frequency bins
        freqs = np.fft.rfftfreq(nfft, 1. / fs)
        return freqs, psd

    def calc_freq_response(self, raw_values):
        np = self.numpy
        if raw_values is None:
            return None
        if isinstance(raw_values, np.ndarray):
            data = raw_values
        else:
            data = np.array(raw_values.decode_samples())

        N = data.shape[0]
        T = data[-1,0] - data[0,0]
        SAMPLING_FREQ = N / T
        # Round up to the nearest power of 2 for faster FFT
        M = 1 << int(SAMPLING_FREQ * WINDOW_T_SEC - 1).bit_length()
        if N <= M:
            return None

        # Calculate PSD (power spectral density) of vibrations per
        # frequency bins (the same bins for X, Y, and Z)
        fx, px = self._psd(data[:,1], SAMPLING_FREQ, M)
        fy, py = self._psd(data[:,2], SAMPLING_FREQ, M)
        fz, pz = self._psd(data[:,3], SAMPLING_FREQ, M)
        return CalibrationData(fx, px+py+pz, px, py, pz)

    def process_accelerometer_data(self, data):
        calibration_data = self.background_process_exec(
                self.calc_freq_response, (data,))
        if calibration_data is None:
            raise self.error(
                    "Internal error processing accelerometer data %s" % (data,))
        calibration_data.set_numpy(self.numpy)
        return calibration_data

    def _estimate_shaper(self, shaper, test_damping_ratio, test_freqs):
        np = self.numpy

        A, T = np.array(shaper[0]), np.array(shaper[1])
        inv_D = 1. / A.sum()

        omega = 2. * math.pi * test_freqs
        damping = test_damping_ratio * omega
        omega_d = omega * math.sqrt(1. - test_damping_ratio**2)
        W = A * np.exp(np.outer(-damping, (T[-1] - T)))
        S = W * np.sin(np.outer(omega_d, T))
        C = W * np.cos(np.outer(omega_d, T))
        return np.sqrt(S.sum(axis=1)**2 + C.sum(axis=1)**2) * inv_D

    def _estimate_remaining_vibrations(self, shaper, test_damping_ratio,
                                       freq_bins, psd):
        vals = self._estimate_shaper(shaper, test_damping_ratio, freq_bins)
        # The input shaper can only reduce the amplitude of vibrations by
        # SHAPER_VIBRATION_REDUCTION times, so all vibrations below that
        # threshold can be igonred
        vibrations_threshold = psd.max() / SHAPER_VIBRATION_REDUCTION
        remaining_vibrations = self.numpy.maximum(
                vals * psd - vibrations_threshold, 0).sum()
        all_vibrations = self.numpy.maximum(psd - vibrations_threshold, 0).sum()
        return (remaining_vibrations / all_vibrations, vals)

    def fit_shaper(self, shaper_cfg, calibration_data, max_smoothing):
        np = self.numpy

        test_freqs = np.arange(shaper_cfg.min_freq, MAX_SHAPER_FREQ, .2)

        freq_bins = calibration_data.freq_bins
        psd = calibration_data.psd_sum[freq_bins <= MAX_FREQ]
        freq_bins = freq_bins[freq_bins <= MAX_FREQ]

        best_res = None
        results = []
        for test_freq in test_freqs[::-1]:
            shaper_vibrations = 0.
            shaper_vals = np.zeros(shape=freq_bins.shape)
            shaper = shaper_cfg.init_func(test_freq, SHAPER_DAMPING_RATIO)
            shaper_smoothing = get_shaper_smoothing(shaper)
            if max_smoothing and shaper_smoothing > max_smoothing and best_res:
                return best_res
            # Exact damping ratio of the printer is unknown, pessimizing
            # remaining vibrations over possible damping values
            for dr in TEST_DAMPING_RATIOS:
                vibrations, vals = self._estimate_remaining_vibrations(
                        shaper, dr, freq_bins, psd)
                shaper_vals = np.maximum(shaper_vals, vals)
                if vibrations > shaper_vibrations:
                    shaper_vibrations = vibrations
            max_accel = self.find_shaper_max_accel(shaper)
            # The score trying to minimize vibrations, but also accounting
            # the growth of smoothing. The formula itself does not have any
            # special meaning, it simply shows good results on real user data
            shaper_score = shaper_smoothing * (shaper_vibrations**1.5 +
                                               shaper_vibrations * .2 + .01)
            results.append(
                    CalibrationResult(
                        name=shaper_cfg.name, freq=test_freq, vals=shaper_vals,
                        vibrs=shaper_vibrations, smoothing=shaper_smoothing,
                        score=shaper_score, max_accel=max_accel))
            if best_res is None or best_res.vibrs > results[-1].vibrs:
                # The current frequency is better for the shaper.
                best_res = results[-1]
        # Try to find an 'optimal' shapper configuration: the one that is not
        # much worse than the 'best' one, but gives much less smoothing
        selected = best_res
        for res in results[::-1]:
            if res.vibrs < best_res.vibrs * 1.1 and res.score < selected.score:
                selected = res
        return selected

    def _bisect(self, func):
        left = right = 1.
        while not func(left):
            right = left
            left *= .5
        if right == left:
            while func(right):
                right *= 2.
        while right - left > 1e-8:
            middle = (left + right) * .5
            if func(middle):
                left = middle
            else:
                right = middle
        return left

    def find_shaper_max_accel(self, shaper):
        # Just some empirically chosen value which produces good projections
        # for max_accel without much smoothing
        TARGET_SMOOTHING = 0.12
        max_accel = self._bisect(lambda test_accel: get_shaper_smoothing(
            shaper, test_accel) <= TARGET_SMOOTHING)
        return max_accel

    def find_best_shaper(self, calibration_data, max_smoothing, logger=None):
        best_shaper = None
        all_shapers = []
        for shaper_cfg in INPUT_SHAPERS:
            shaper = self.background_process_exec(self.fit_shaper, (
                shaper_cfg, calibration_data, max_smoothing))
            if logger is not None:
                logger("Fitted shaper '%s' frequency = %.1f Hz "
                       "(vibrations = %.1f%%, smoothing ~= %.3f)" % (
                           shaper.name, shaper.freq, shaper.vibrs * 100.,
                           shaper.smoothing))
                logger("To avoid too much smoothing with '%s', suggested "
                       "max_accel <= %.0f mm/sec^2" % (
                           shaper.name, round(shaper.max_accel / 100.) * 100.))
            all_shapers.append(shaper)
            if (best_shaper is None or shaper.score * 1.2 < best_shaper.score or
                    (shaper.score * 1.05 < best_shaper.score and
                        shaper.smoothing * 1.1 < best_shaper.smoothing)):
                # Either the shaper significantly improves the score (by 20%),
                # or it improves the score and smoothing (by 5% and 10% resp.)
                best_shaper = shaper
        return best_shaper, all_shapers

    def save_params(self, configfile, axis, shaper_name, shaper_freq):
        if axis == 'xy':
            self.save_params(configfile, 'x', shaper_name, shaper_freq)
            self.save_params(configfile, 'y', shaper_name, shaper_freq)
        else:
            configfile.set('input_shaper', 'shaper_type_'+axis, shaper_name)
            configfile.set('input_shaper', 'shaper_freq_'+axis,
                           '%.1f' % (shaper_freq,))

    def save_calibration_data(self, output, calibration_data, shapers=None):
        try:
            with open(output, "w") as csvfile:
                csvfile.write("freq,psd_x,psd_y,psd_z,psd_xyz")
                if shapers:
                    for shaper in shapers:
                        csvfile.write(",%s(%.1f)" % (shaper.name, shaper.freq))
                csvfile.write("\n")
                num_freqs = calibration_data.freq_bins.shape[0]
                for i in range(num_freqs):
                    if calibration_data.freq_bins[i] >= MAX_FREQ:
                        break
                    csvfile.write("%.1f,%.3e,%.3e,%.3e,%.3e" % (
                        calibration_data.freq_bins[i],
                        calibration_data.psd_x[i],
                        calibration_data.psd_y[i],
                        calibration_data.psd_z[i],
                        calibration_data.psd_sum[i]))
                    if shapers:
                        for shaper in shapers:
                            csvfile.write(",%.3f" % (shaper.vals[i],))
                    csvfile.write("\n")
        except IOError as e:
            raise self.error("Error writing to file '%s': %s", output, str(e))
