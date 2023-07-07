#!/usr/bin/env python
# Script to plot input shapers
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020-2023  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse, importlib, math, os, sys
import numpy as np, matplotlib
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),
                             '..', 'klippy'))
shaper_defs = importlib.import_module('.shaper_defs', 'extras')
shaper_calibrate = importlib.import_module('.shaper_calibrate', 'extras')

# A set of damping ratios to calculate shaper response for
DAMPING_RATIOS=[0.075, 0.1, 0.15]

# Parameters of the input shaper
SHAPER_FREQ=50.0
SHAPER_DAMPING_RATIO=0.1

# Simulate input shaping of step function for these true resonance frequency
# and damping ratio
STEP_SIMULATION_RESONANCE_FREQ=50.
STEP_SIMULATION_DAMPING_RATIO=0.1

# If set, defines which range of frequencies to plot shaper frequency response
PLOT_FREQ_RANGE = []  # If empty, will be automatically determined
#PLOT_FREQ_RANGE = [10., 100.]

PLOT_FREQ_STEP = .05

######################################################################
# Plotting and startup
######################################################################

def bisect(func, left, right):
    lhs_sign = math.copysign(1., func(left))
    while right-left > 1e-8:
        mid = .5 * (left + right)
        val = func(mid)
        if math.copysign(1., val) == lhs_sign:
            left = mid
        else:
            right = mid
    return .5 * (left + right)

def find_shaper_plot_range(estimate_shaper, shaper_freq, vib_tol):
    def eval_shaper(freq):
        return estimate_shaper(freq) - vib_tol
    if not PLOT_FREQ_RANGE:
        left = bisect(eval_shaper, 0.1 * shaper_freq, shaper_freq)
        right = bisect(eval_shaper, shaper_freq, 3 * shaper_freq)
    else:
        left, right = PLOT_FREQ_RANGE
    return (left, right)

def gen_shaper_response(shaper, shaper_freq, estimate_shaper):
    # Calculate shaper vibration response on a range of requencies
    freq_start, freq_end = find_shaper_plot_range(
            lambda freq: estimate_shaper(
                np, shaper, DAMPING_RATIOS[0], [freq]),
            shaper_freq, vib_tol=0.25)
    freqs = np.arange(freq_start, freq_end, PLOT_FREQ_STEP)
    response = np.zeros(shape=(freqs.shape[0], len(DAMPING_RATIOS)))
    n_dr = len(DAMPING_RATIOS)
    response = np.zeros(shape=(freqs.shape[0], n_dr))
    for i in range(n_dr):
        response[:, i] = estimate_shaper(
                np, shaper, DAMPING_RATIOS[i], freqs)
    legend = ['damping ratio = %.3f' % d_r for d_r in DAMPING_RATIOS]
    return freqs, response, legend

def gen_shaped_step_function(shaper, freq, damping_ratio):
    # Calculate shaping of a step function
    A, T, _ = shaper
    inv_D = 1. / sum(A)
    n = len(T)
    t_s = T[-1] - T[0]
    A = np.asarray(A)

    omega = 2. * math.pi * freq

    t_start = T[0] - .5 * t_s
    t_end = T[-1] + 1.5 * max(1. / freq, t_s)
    dt = .01 * min(t_s, 1. / freq)
    time = np.arange(t_start, t_end, dt)
    time_t = time[np.newaxis].T

    step = np.zeros(time.shape)
    step[time >= 0.] = 1.
    commanded = np.sum(A * np.where(time_t - T >= 0, 1., 0.), axis=-1) * inv_D
    response = np.zeros(shape=(time.shape))
    for i in range(n):
        response += A[i] * shaper_calibrate.step_response(
                np, time - T[i], omega, damping_ratio)[0,:]
    response *= inv_D
    velocity = np.insert(
            (response[1:] - response[:-1]) / (2 * math.pi * freq * dt), 0, 0.)
    legend = ['step', 'shaper commanded', 'system response',
              'system response velocity']
    result = np.zeros(shape=(time.shape[0], 4))
    result[:,0] = step
    result[:,1] = commanded
    result[:,2] = response
    result[:,3] = velocity
    return time, result, legend

def gen_smoothed_step_function(smoother, freq, damping_ratio):
    # Calculate smoothing of a step function
    C, t_sm, t_offs, _ = smoother
    hst = 0.5 * t_sm

    n_t = max(1000, 1000 * round(t_sm * freq))
    tau, dt = np.linspace(-hst, hst, n_t, retstep=True)
    w = np.zeros(shape=tau.shape)
    for c in C[::-1]:
        w = w * tau + c

    omega = 2. * math.pi * freq
    damping = damping_ratio * omega
    omega_d = omega * math.sqrt(1. - damping_ratio**2)
    phase = math.acos(damping_ratio)

    t_start = -t_sm - t_offs
    t_end = hst - t_offs + 1.5 * max(1. / freq, t_sm)
    time = np.arange(t_start, t_end, dt)
    time_t = time[np.newaxis].T
    w_dt = w * dt
    step = np.zeros(time.shape)
    step[time >= 0.] = 1.
    commanded = np.sum(w_dt * np.where(time_t - tau + t_offs >= 0, 1., 0.),
                       axis=-1)
    s_r_n = time.size + tau.size - 1
    s_r_t = np.arange(t_start - hst, t_end + hst * 1.1, dt)[:s_r_n]
    s_r = shaper_calibrate.step_response(
            np, s_r_t + t_offs, omega, damping_ratio)
    response = np.convolve(s_r[0,:], w_dt, mode='valid')
    velocity = np.insert((response[1:] - response[:-1])
                         / (2 * math.pi * freq * dt), 0, 0.)
    legend = ['step', 'shaper commanded', 'system response',
              'system response velocity']
    result = np.zeros(shape=(time.shape[0], 4))
    result[:,0] = step
    result[:,1] = commanded
    result[:,2] = response
    result[:,3] = velocity
    return time, result, legend

def plot_shaper(shaper_name, shaper_freq, freq, damping_ratio):
    for s in shaper_defs.INPUT_SHAPERS:
        if s.name == shaper_name.lower():
            A, T = s.init_func(shaper_freq, SHAPER_DAMPING_RATIO)
            ts = shaper_defs.get_shaper_offset(A, T)
            T = [t - ts for t in T]
            shaper = A, T, s.name.upper()
            freqs, response, response_legend = gen_shaper_response(
                    shaper, shaper_freq, shaper_calibrate.estimate_shaper)
            time, step_vals, step_legend = gen_shaped_step_function(
                    shaper, freq, damping_ratio)
    for s in shaper_defs.INPUT_SMOOTHERS:
        if s.name == shaper_name.lower():
            C, t_sm = s.init_func(shaper_freq)
            t_offs = shaper_defs.get_smoother_offset(C, t_sm)
            shaper = C, t_sm, t_offs, s.name.upper()
            freqs, response, response_legend = gen_shaper_response(
                    shaper, shaper_freq, shaper_calibrate.estimate_smoother)
            time, step_vals, step_legend = gen_smoothed_step_function(
                    shaper, freq, damping_ratio)

    fig, (ax1, ax2) = matplotlib.pyplot.subplots(nrows=2, figsize=(10,9))
    ax1.set_title("Vibration response simulation for shaper '%s',\n"
                  "shaper_freq=%.1f Hz, damping_ratio=%.3f"
                  % (shaper[-1], shaper_freq, SHAPER_DAMPING_RATIO))
    ax1.plot(freqs, response)
    ax1.set_ylim(bottom=0.)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax1.legend(response_legend, loc='best', prop=fontP)
    ax1.set_xlabel('Resonance frequency, Hz')
    ax1.set_ylabel('Remaining vibrations, ratio')
    ax1.xaxis.set_minor_locator(matplotlib.ticker.AutoMinorLocator())
    ax1.yaxis.set_minor_locator(matplotlib.ticker.AutoMinorLocator())
    ax1.grid(which='major', color='grey')
    ax1.grid(which='minor', color='lightgrey')

    ax2.set_title("Unit step input, resonance frequency=%.1f Hz, "
                  "damping ratio=%.3f" % (freq, damping_ratio))
    ax2.plot(time, step_vals)
    ax2.legend(step_legend, loc='best', prop=fontP)
    ax2.set_xlabel('Time, sec')
    ax2.set_ylabel('Amplitude')
    ax2.grid()
    fig.tight_layout()
    return fig

def setup_matplotlib(output_to_file):
    global matplotlib
    if output_to_file:
        matplotlib.use('Agg')
    import matplotlib.pyplot, matplotlib.dates, matplotlib.font_manager
    import matplotlib.ticker

def main():
    # Parse command-line arguments
    usage = "%prog [options]"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--output", type="string", dest="output",
                    default=None, help="filename of output graph")
    opts.add_option("-s", "--shaper", type="string", dest="shaper",
                    default="ei", help="name of the shaper to plot")
    opts.add_option("-f", "--freq", type="float", dest="freq",
                    default=STEP_SIMULATION_RESONANCE_FREQ,
                    help="the frequency of the system")
    opts.add_option("--damping_ratio", type="float", dest="damping_ratio",
                    default=STEP_SIMULATION_DAMPING_RATIO,
                    help="the damping ratio of the system")
    opts.add_option("--shaper_freq", type="float", dest="shaper_freq",
                    default=SHAPER_FREQ,
                    help="the frequency of the shaper")
    options, args = opts.parse_args()
    if len(args) != 0:
        opts.error("Incorrect number of arguments")
    if (options.shaper.lower() not in
            [s.name for s in shaper_defs.INPUT_SHAPERS] and
        options.shaper.lower() not in
            [s.name for s in shaper_defs.INPUT_SMOOTHERS]):
        opts.error("Invalid shaper name '%s'" % (opts.shaper,))

    # Draw graph
    setup_matplotlib(options.output is not None)
    fig = plot_shaper(options.shaper, options.shaper_freq, options.freq,
                      options.damping_ratio)

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
