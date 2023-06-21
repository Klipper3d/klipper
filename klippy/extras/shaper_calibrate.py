# Automatic calibration of input shapers
#
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections, importlib, logging, math, multiprocessing, traceback
shaper_defs = importlib.import_module('.shaper_defs', 'extras')

MIN_FREQ = 5.
MAX_FREQ = 200.
WINDOW_T_SEC = 0.5
MAX_SHAPER_FREQ = 150.

TEST_DAMPING_RATIOS=[0.075, 0.1, 0.15]

AUTOTUNE_SHAPERS = ['smooth_zv', 'smooth_mzv', 'smooth_ei', 'smooth_2hump_ei',
                    'smooth_zvd_ei', 'smooth_si', 'mzv', 'ei', '2hump_ei']

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
    def add_data(self, other):
        np = self.numpy
        for psd, other_psd in zip(self._psd_list, other._psd_list):
            # `other` data may be defined at different frequency bins,
            # interpolating to fix that.
            other_normalized = np.interp(
                    self.freq_bins, other.freq_bins, other_psd)
            psd[:] = np.maximum(psd, other_normalized)
    def set_numpy(self, numpy):
        self.numpy = numpy
    def normalize_to_frequencies(self):
        freq_bins = self.freq_bins
        for psd in self._psd_list:
            # Avoid division by zero errors and remove low-frequency noise
            psd *= self.numpy.tanh(.5 / MIN_FREQ * freq_bins) / (freq_bins + .1)
    def get_psd(self, axis='all'):
        return self._psd_map[axis]


CalibrationResult = collections.namedtuple(
        'CalibrationResult',
        ('name', 'freq', 'vals', 'vibrs', 'smoothing', 'score', 'max_accel'))

def step_response(np, t, omega, damping_ratio):
    t = np.maximum(t, 0.)
    omega = np.swapaxes(np.array(omega, ndmin=2), 0, 1)
    damping = damping_ratio * omega
    omega_d = omega * math.sqrt(1. - damping_ratio**2)
    phase = math.acos(damping_ratio)
    return (1. - np.exp((-damping * t))
               * np.sin((omega_d * t) + phase) * (1. / math.sin(phase)))

def step_response_min_velocity(damping_ratio):
    d2 = damping_ratio * damping_ratio
    d_r = damping_ratio / math.sqrt(1. - d2)
    # Analytical formula for the minimum was obtained using Maxima system
    t = .5 * math.atan2(2. * d2, (2. * d2 - 1.) * d_r) + math.pi
    phase = math.acos(damping_ratio)
    v = math.exp(-d_r * t) * (d_r * math.sin(t + phase) - math.cos(t + phase))
    return v

def estimate_shaper_old(np, shaper, test_damping_ratio, test_freqs):
    A, T = np.asarray(shaper[0]), np.asarray(shaper[1])
    inv_D = 1. / A.sum()

    omega = 2. * math.pi * np.asarray(test_freqs)
    damping = test_damping_ratio * omega
    omega_d = omega * math.sqrt(1. - test_damping_ratio**2)
    W = A * np.exp(np.outer(-damping, (T[-1] - T)))
    S = W * np.sin(np.outer(omega_d, T))
    C = W * np.cos(np.outer(omega_d, T))
    return np.sqrt(S.sum(axis=1)**2 + C.sum(axis=1)**2) * inv_D

def estimate_shaper(np, shaper, test_damping_ratio, test_freqs):
    A, T = np.asarray(shaper[0]), np.asarray(shaper[1])
    inv_D = 1. / A.sum()
    n = len(T)
    t_s = T[-1] - T[0]

    test_freqs = np.asarray(test_freqs)
    t_start = T[0]
    t_end = T[-1] + 2.0 * np.maximum(1. / test_freqs[test_freqs > 0.], t_s)
    n_t = 1000
    unity_range = np.linspace(0., 1., n_t)
    time = (t_end[:, np.newaxis] - t_start) * unity_range + t_start
    dt = (time[:,-1] - time[:,0]) / n_t

    min_v = -step_response_min_velocity(test_damping_ratio)

    omega = 2. * math.pi * test_freqs[test_freqs > 0.]

    response = np.zeros(shape=(omega.shape[0], time.shape[-1]))
    for i in range(n):
        s_r = step_response(np, time - T[i], omega, test_damping_ratio)
        response += A[i] * s_r
    response *= inv_D
    velocity = (response[:,1:] - response[:,:-1]) / (omega * dt)[:, np.newaxis]
    res = np.zeros(shape=test_freqs.shape)
    res[test_freqs > 0.] = -velocity.min(axis=-1) / min_v
    res[test_freqs <= 0.] = 1.
    return res

def estimate_smoother_old(np, smoother, test_damping_ratio, test_freqs):
    C, t_sm = smoother[0], smoother[1]
    hst = t_sm * 0.5

    test_freqs = np.asarray(test_freqs)
    omega = 2. * math.pi * test_freqs
    damping = test_damping_ratio * omega
    omega_d = omega * math.sqrt(1. - test_damping_ratio**2)

    n_t = max(100, 100 * round(t_sm * np.max(test_freqs)))
    t, dt = np.linspace(0., t_sm, n_t, retstep=True)
    w = np.zeros(shape=t.shape)
    for c in C[::-1]:
        w = w * (t-hst) + c

    E = w * np.exp(np.outer(damping, (t-t_sm)))
    C = np.cos(np.outer(omega_d, (t-t_sm)))
    S = np.sin(np.outer(omega_d, (t-t_sm)))
    return np.sqrt(np.trapz(E * C, dx=dt)**2 + np.trapz(E * S, dx=dt)**2)

def estimate_smoother(np, smoother, test_damping_ratio, test_freqs):
    C, t_sm = smoother[0], smoother[1]
    hst = t_sm * 0.5

    test_freqs = np.asarray(test_freqs)

    t_start = -hst
    t_end = hst + 1.5 * np.maximum(1. / test_freqs[test_freqs > 0.], t_sm)
    n_t = 1000
    unity_range = np.linspace(0., 1., n_t)
    time = (t_end[:, np.newaxis] - t_start) * unity_range + t_start
    dt = (time[:,-1] - time[:,0]) / n_t
    tau = np.copy(time)
    tau[time > hst] = 0.

    w = np.zeros(shape=tau.shape)
    for c in C[::-1]:
        w = w * tau + c
    w[time > hst] = 0.
    norms = (w * dt[:, np.newaxis]).sum(axis=-1)

    min_v = -step_response_min_velocity(test_damping_ratio)

    omega = 2. * math.pi * test_freqs[test_freqs > 0.]

    wl = np.count_nonzero(time <= hst, axis=-1).max()

    def get_windows(m, wl):
        nrows = m.shape[-1] - wl + 1
        n = m.strides[-1]
        return np.lib.stride_tricks.as_strided(m, shape=(m.shape[0], nrows, wl),
                                               strides=(m.strides[0], n, n))

    s_r = step_response(np, time, omega, test_damping_ratio)
    w_dt = w[:, :wl] * (np.reciprocal(norms) * dt)[:, np.newaxis]
    response = np.einsum("ijk,ik->ij", get_windows(s_r, wl), w_dt[:,::-1])
    velocity = (response[:,1:] - response[:,:-1]) / (omega * dt)[:, np.newaxis]
    res = np.zeros(shape=test_freqs.shape)
    res[test_freqs > 0.] = -velocity.min(axis=-1) / min_v
    res[test_freqs <= 0.] = 1.
    return res

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
            samples = raw_values.get_samples()
            if not samples:
                return None
            data = np.array(samples)

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

    def _estimate_remaining_vibrations(self, freq_bins, vals, psd):
        # Calculate the acceptable level of remaining vibrations.
        # Note that these are not true remaining vibrations, but rather
        # just a score to compare different shapers between each other.
        vibr_threshold = ((psd[freq_bins > 0] / freq_bins[freq_bins > 0]).max()
                          * (freq_bins + MIN_FREQ) * (1. / 33.3))
        remaining_vibrations = (self.numpy.maximum(
                vals * psd - vibr_threshold, 0) * freq_bins**2).sum()
        all_vibrations = (psd * freq_bins**2).sum()
        return remaining_vibrations / all_vibrations

    def _get_shaper_smoothing(self, shaper, accel=5000, scv=5.):
        half_accel = accel * .5

        A, T = shaper
        inv_D = 1. / sum(A)
        n = len(T)
        ts = shaper_defs.get_shaper_offset(A, T)

        # Calculate offset for 90 and 180 degrees turn
        offset_90_x = offset_90_y = offset_180 = 0.
        for i in range(n):
            if T[i] >= ts:
                # Calculate offset for one of the axes
                offset_90_x += A[i] * (scv + half_accel * (T[i]-ts)) * (T[i]-ts)
            else:
                offset_90_y += A[i] * (scv - half_accel * (T[i]-ts)) * (T[i]-ts)
            offset_180 += A[i] * half_accel * (T[i]-ts)**2
        offset_90 = inv_D * math.sqrt(offset_90_x**2 + offset_90_y**2)
        offset_180 *= inv_D
        return max(offset_90, abs(offset_180))

    def _get_smoother_smoothing(self, smoother, accel=5000, scv=5.):
        np = self.numpy
        half_accel = accel * .5

        C, t_sm = smoother
        hst = 0.5 * t_sm
        t, dt = np.linspace(-hst, hst, 100, retstep=True)
        w = np.zeros(shape=t.shape)
        for c in C[::-1]:
            w = w * (-t) + c
        inv_norm = 1. / np.trapz(w, dx=dt)
        w *= inv_norm
        t -= np.trapz(t * w, dx=dt)

        offset_180 = np.trapz(half_accel * t**2 * w, dx=dt)
        offset_90_x = np.trapz(((scv + half_accel * t) * t * w)[t >= 0], dx=dt)
        offset_90_y = np.trapz(((scv - half_accel * t) * t * w)[t <  0], dx=dt)
        offset_90 = math.sqrt(offset_90_x**2 + offset_90_y**2)
        return max(offset_90, abs(offset_180))

    def fit_shaper(self, shaper_cfg, calibration_data, max_smoothing,
                   estimate_shaper, get_shaper_smoothing):
        np = self.numpy

        shaper = shaper_cfg.init_func(1.0, shaper_defs.DEFAULT_DAMPING_RATIO)

        test_freq_bins = np.arange(0., 10., 0.01)
        test_shaper_vals = np.zeros(shape=test_freq_bins.shape)
        # Exact damping ratio of the printer is unknown, pessimizing
        # remaining vibrations over possible damping values
        for dr in TEST_DAMPING_RATIOS:
            vals = estimate_shaper(self.numpy, shaper, dr, test_freq_bins)
            test_shaper_vals = np.maximum(test_shaper_vals, vals)

        test_freqs = np.arange(shaper_cfg.min_freq, MAX_SHAPER_FREQ, .2)

        freq_bins = calibration_data.freq_bins
        psd = calibration_data.psd_sum[freq_bins <= MAX_FREQ]
        freq_bins = freq_bins[freq_bins <= MAX_FREQ]

        best_res = None
        results = []
        for test_freq in test_freqs[::-1]:
            shaper = shaper_cfg.init_func(
                    test_freq, shaper_defs.DEFAULT_DAMPING_RATIO)
            shaper_smoothing = get_shaper_smoothing(shaper)
            if max_smoothing and shaper_smoothing > max_smoothing and best_res:
                return best_res
            shaper_vals = np.interp(freq_bins, test_freq_bins * test_freq,
                                    test_shaper_vals)
            shaper_vibrations = self._estimate_remaining_vibrations(
                    freq_bins, shaper_vals, psd)
            max_accel = self.find_max_accel(shaper, get_shaper_smoothing)
            # The score trying to minimize vibrations, but also accounting
            # the growth of smoothing. The formula itself does not have any
            # special meaning, it simply shows good results on real user data
            shaper_score = shaper_smoothing * (2. * shaper_vibrations**1.5 +
                                               shaper_vibrations * .2 + .001 +
                                               shaper_smoothing * .002)
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
            if res.score < selected.score and (
                    res.vibrs < best_res.vibrs * 1.2 or
                    res.vibrs < best_res.vibrs + 0.0075):
                selected = res
        return selected

    def _bisect(self, func, eps = 1e-8):
        left = right = 1.
        while not func(left):
            right = left
            left *= .5
        if right == left:
            while func(right):
                right *= 2.
        while right - left > eps:
            middle = (left + right) * .5
            if func(middle):
                left = middle
            else:
                right = middle
        return left

    def find_max_accel(self, s, get_smoothing):
        # Just some empirically chosen value which produces good projections
        # for max_accel without much smoothing
        TARGET_SMOOTHING = 0.12
        max_accel = self._bisect(lambda test_accel: get_smoothing(
            s, test_accel) <= TARGET_SMOOTHING, 1e-2)
        return max_accel

    def find_best_shaper(self, calibration_data, max_smoothing, logger=None):
        best_shaper = None
        all_shapers = []
        for smoother_cfg in shaper_defs.INPUT_SMOOTHERS:
            if smoother_cfg.name not in AUTOTUNE_SHAPERS:
                continue
            smoother = self.background_process_exec(self.fit_shaper, (
                smoother_cfg, calibration_data, max_smoothing,
                estimate_smoother, self._get_smoother_smoothing))
            if logger is not None:
                logger("Fitted smoother '%s' frequency = %.1f Hz "
                       "(vibration score = %.2f%%, smoothing ~= %.3f,"
                       " combined score = %.3e)" % (
                           smoother.name, smoother.freq, smoother.vibrs * 100.,
                           smoother.smoothing, smoother.score))
                logger("To avoid too much smoothing with '%s', suggested "
                       "max_accel <= %.0f mm/sec^2" % (
                           smoother.name,
                           round(smoother.max_accel / 100.) * 100.))
            all_shapers.append(smoother)
            if (best_shaper is None or smoother.score * 1.2 < best_shaper.score
                or (smoother.score * 1.03 < best_shaper.score and
                    smoother.smoothing * 1.01 < best_shaper.smoothing)):
                # Either the smoother significantly improves the score (by 20%),
                # or it improves the score and smoothing (by 5% and 10% resp.)
                best_shaper = smoother
        for shaper_cfg in shaper_defs.INPUT_SHAPERS:
            if shaper_cfg.name not in AUTOTUNE_SHAPERS:
                continue
            shaper = self.background_process_exec(self.fit_shaper, (
                shaper_cfg, calibration_data, max_smoothing,
                estimate_shaper, self._get_shaper_smoothing))
            if logger is not None:
                logger("Fitted shaper '%s' frequency = %.1f Hz "
                       "(vibration score = %.2f%%, smoothing ~= %.3f,"
                       " combined score = %.3e)" % (
                           shaper.name, shaper.freq, shaper.vibrs * 100.,
                           shaper.smoothing, shaper.score))
                logger("To avoid too much smoothing with '%s', suggested "
                       "max_accel <= %.0f mm/sec^2" % (
                           shaper.name, round(shaper.max_accel / 100.) * 100.))
            all_shapers.append(shaper)
            if (best_shaper is None or shaper.score * 1.2 < best_shaper.score or
                    (shaper.score * 1.03 < best_shaper.score and
                        shaper.smoothing * 1.01 < best_shaper.smoothing)):
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

    def apply_params(self, input_shaper, axis, shaper_name, shaper_freq):
        if axis == 'xy':
            self.apply_params(input_shaper, 'x', shaper_name, shaper_freq)
            self.apply_params(input_shaper, 'y', shaper_name, shaper_freq)
            return
        gcode = self.printer.lookup_object("gcode")
        axis = axis.upper()
        input_shaper.cmd_SET_INPUT_SHAPER(gcode.create_gcode_command(
                "SET_INPUT_SHAPER", "SET_INPUT_SHAPER", {
                    "SHAPER_TYPE_" + axis: shaper_name,
                    "SHAPER_FREQ_" + axis: shaper_freq}))

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
