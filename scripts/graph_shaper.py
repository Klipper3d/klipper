#!/usr/bin/env python
# Script to plot input shapers
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import importlib, math, optparse, os, sys
import matplotlib

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),
                             '..', 'klippy'))
shaper_defs = importlib.import_module('.shaper_defs', 'extras')

# A set of damping ratios to calculate shaper response for
DEFAULT_DAMPING_RATIOS=[0.075, 0.1, 0.15]

# If set, defines which range of frequencies to plot shaper frequency response
PLOT_FREQ_RANGE = []  # If empty, will be automatically determined
#PLOT_FREQ_RANGE = [10., 100.]

PLOT_FREQ_STEP = .01

######################################################################
# Input shapers
######################################################################

def estimate_shaper(shaper, freq, damping_ratio):
    A, T = shaper
    n = len(T)
    inv_D = 1. / sum(A)
    omega = 2. * math.pi * freq
    damping = damping_ratio * omega
    omega_d = omega * math.sqrt(1. - damping_ratio**2)
    S = C = 0
    for i in range(n):
        W = A[i] * math.exp(-damping * (T[-1] - T[i]))
        S += W * math.sin(omega_d * T[i])
        C += W * math.cos(omega_d * T[i])
    return math.sqrt(S*S + C*C) * inv_D

def shift_pulses(shaper):
    A, T = shaper
    n = len(T)
    ts = sum([A[i] * T[i] for i in range(n)]) / sum(A)
    for i in range(n):
        T[i] -= ts

# Shaper selection
def get_shaper(shaper_name, shaper_freq, damping_ratio):
    for s in shaper_defs.INPUT_SHAPERS:
        if shaper_name.lower() == s.name:
            return s.init_func(shaper_freq, damping_ratio)
    return shaper_defs.get_none_shaper()


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

def find_shaper_plot_range(shaper, shaper_freq, test_damping_ratios, vib_tol):
    def eval_shaper(freq):
        return estimate_shaper(shaper, freq, test_damping_ratios[0]) - vib_tol
    if not PLOT_FREQ_RANGE:
        left = bisect(eval_shaper, 0., shaper_freq)
        right = bisect(eval_shaper, shaper_freq, 2.4 * shaper_freq)
    else:
        left, right = PLOT_FREQ_RANGE
    return (left, right)

def gen_shaper_response(shaper, shaper_freq, test_damping_ratios):
    # Calculate shaper vibration response on a range of frequencies
    response = []
    freqs = []
    freq, freq_end = find_shaper_plot_range(shaper, shaper_freq,
                                            test_damping_ratios, vib_tol=0.25)
    while freq <= freq_end:
        vals = []
        for damping_ratio in test_damping_ratios:
            vals.append(estimate_shaper(shaper, freq, damping_ratio))
        response.append(vals)
        freqs.append(freq)
        freq += PLOT_FREQ_STEP
    legend = ['damping ratio = %.3f' % d_r for d_r in test_damping_ratios]
    return freqs, response, legend

def gen_shaped_step_function(shaper, shaper_freq,
                             system_freq, system_damping_ratio):
    # Calculate shaping of a step function
    A, T = shaper
    inv_D = 1. / sum(A)
    n = len(T)

    omega = 2. * math.pi * system_freq
    damping = system_damping_ratio * omega
    omega_d = omega * math.sqrt(1. - system_damping_ratio**2)
    phase = math.acos(system_damping_ratio)

    t_start = T[0] - .5 / shaper_freq
    t_end = T[-1] + 1.5 / system_freq
    result = []
    time = []
    t = t_start

    def step_response(t):
        if t < 0.:
            return 0.
        return 1. - math.exp(-damping * t) * math.sin(omega_d * t
                                                      + phase) / math.sin(phase)

    while t <= t_end:
        val = []
        val.append(1. if t >= 0. else 0.)
        #val.append(step_response(t))

        commanded = 0.
        response = 0.
        S = C = 0
        for i in range(n):
            if t < T[i]:
                continue
            commanded += A[i]
            response += A[i] * step_response(t - T[i])
        val.append(commanded * inv_D)
        val.append(response * inv_D)

        result.append(val)
        time.append(t)
        t += .01 / shaper_freq
    legend = ['step', 'shaper commanded', 'system response']
    return time, result, legend


def plot_shaper(shaper_name, shaper_freq, damping_ratio, test_damping_ratios,
                system_freq, system_damping_ratio):
    shaper = get_shaper(shaper_name, shaper_freq, damping_ratio)
    shift_pulses(shaper)
    freqs, response, response_legend = gen_shaper_response(
            shaper, shaper_freq, test_damping_ratios)
    time, step_vals, step_legend = gen_shaped_step_function(
            shaper, shaper_freq, system_freq, system_damping_ratio)

    fig, (ax1, ax2) = matplotlib.pyplot.subplots(nrows=2, figsize=(10,9))
    ax1.set_title("Vibration response simulation for shaper '%s',\n"
                  "shaper_freq=%.1f Hz, damping_ratio=%.3f"
                  % (shaper_name, shaper_freq, damping_ratio))
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
                  "damping ratio=%.3f" % (system_freq, system_damping_ratio))
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
    opts.add_option("--shaper", type="string", dest="shaper", default="mzv",
                    help="a shaper to plot")
    opts.add_option("--shaper_freq", type="float", dest="shaper_freq",
                    default=50.0, help="shaper frequency")
    opts.add_option("--damping_ratio", type="float", dest="damping_ratio",
                    default=shaper_defs.DEFAULT_DAMPING_RATIO,
                    help="shaper damping_ratio parameter")
    opts.add_option("--test_damping_ratios", type="string",
                    dest="test_damping_ratios",
                    default=",".join(["%.3f" % dr
                                      for dr in DEFAULT_DAMPING_RATIOS]),
                    help="a comma-separated list of damping ratios to test " +
                    "input shaper for")
    opts.add_option("--system_freq", type="float", dest="system_freq",
                    default=60.0,
                    help="natural frequency of a system for step simulation")
    opts.add_option("--system_damping_ratio", type="float",
                    dest="system_damping_ratio", default=0.15,
                    help="damping_ratio of a system for step simulation")
    options, args = opts.parse_args()
    if len(args) != 0:
        opts.error("Incorrect number of arguments")

    if options.shaper.lower() not in [
            s.name for s in shaper_defs.INPUT_SHAPERS]:
        opts.error("Invalid --shaper=%s specified" % options.shaper)

    if options.test_damping_ratios:
        try:
            test_damping_ratios = [float(s) for s in
                                   options.test_damping_ratios.split(',')]
        except ValueError:
            opts.error("invalid floating point value in " +
                       "--test_damping_ratios param")
    else:
        test_damping_ratios = None

    # Draw graph
    setup_matplotlib(options.output is not None)
    fig = plot_shaper(options.shaper, options.shaper_freq,
                      options.damping_ratio, test_damping_ratios,
                      options.system_freq, options.system_damping_ratio)

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
