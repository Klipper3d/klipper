#!/usr/bin/env python
# Script to plot input shapers
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse, math
import matplotlib

# A set of damping ratios to calculate shaper response for
DAMPING_RATIOS=[0.05, 0.1, 0.2]

# Parameters of the input shaper
SHAPER_FREQ=50.0
SHAPER_DAMPING_RATIO=0.1

# Simulate input shaping of step function for these true resonance frequency
# and damping ratio
STEP_SIMULATION_RESONANCE_FREQ=60.
STEP_SIMULATION_DAMPING_RATIO=0.15

# If set, defines which range of frequencies to plot shaper frequency responce
PLOT_FREQ_RANGE = []  # If empty, will be automatically determined
#PLOT_FREQ_RANGE = [10., 100.]

PLOT_FREQ_STEP = .01

######################################################################
# Input shapers
######################################################################

def get_zv_shaper():
    df = math.sqrt(1. - SHAPER_DAMPING_RATIO**2)
    K = math.exp(-SHAPER_DAMPING_RATIO * math.pi / df)
    t_d = 1. / (SHAPER_FREQ * df)
    A = [1., K]
    T = [0., .5*t_d]
    return (A, T, "ZV")

def get_zvd_shaper():
    df = math.sqrt(1. - SHAPER_DAMPING_RATIO**2)
    K = math.exp(-SHAPER_DAMPING_RATIO * math.pi / df)
    t_d = 1. / (SHAPER_FREQ * df)
    A = [1., 2.*K, K**2]
    T = [0., .5*t_d, t_d]
    return (A, T, "ZVD")

def get_mzv_shaper():
    df = math.sqrt(1. - SHAPER_DAMPING_RATIO**2)
    K = math.exp(-.75 * SHAPER_DAMPING_RATIO * math.pi / df)
    t_d = 1. / (SHAPER_FREQ * df)

    a1 = 1. - 1. / math.sqrt(2.)
    a2 = (math.sqrt(2.) - 1.) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0., .375*t_d, .75*t_d]
    return (A, T, "MZV")

def get_ei_shaper():
    v_tol = 0.05 # vibration tolerance
    df = math.sqrt(1. - SHAPER_DAMPING_RATIO**2)
    K = math.exp(-SHAPER_DAMPING_RATIO * math.pi / df)
    t_d = 1. / (SHAPER_FREQ * df)

    a1 = .25 * (1. + v_tol)
    a2 = .5 * (1. - v_tol) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0., .5*t_d, t_d]
    return (A, T, "EI")

def get_2hump_ei_shaper():
    v_tol = 0.05 # vibration tolerance
    df = math.sqrt(1. - SHAPER_DAMPING_RATIO**2)
    K = math.exp(-SHAPER_DAMPING_RATIO * math.pi / df)
    t_d = 1. / (SHAPER_FREQ * df)

    V2 = v_tol**2
    X = pow(V2 * (math.sqrt(1. - V2) + 1.), 1./3.)
    a1 = (3.*X*X + 2.*X + 3.*V2) / (16.*X)
    a2 = (.5 - a1) * K
    a3 = a2 * K
    a4 = a1 * K * K * K

    A = [a1, a2, a3, a4]
    T = [0., .5*t_d, t_d, 1.5*t_d]
    return (A, T, "2-hump EI")

def get_3hump_ei_shaper():
    v_tol = 0.05 # vibration tolerance
    df = math.sqrt(1. - SHAPER_DAMPING_RATIO**2)
    K = math.exp(-SHAPER_DAMPING_RATIO * math.pi / df)
    t_d = 1. / (SHAPER_FREQ * df)

    K2 = K*K
    a1 = 0.0625 * (1. + 3. * v_tol + 2. * math.sqrt(2. * (v_tol + 1.) * v_tol))
    a2 = 0.25 * (1. - v_tol) * K
    a3 = (0.5 * (1. + v_tol) - 2. * a1) * K2
    a4 = a2 * K2
    a5 = a1 * K2 * K2

    A = [a1, a2, a3, a4, a5]
    T = [0., .5*t_d, t_d, 1.5*t_d, 2.*t_d]
    return (A, T, "3-hump EI")


def estimate_shaper(shaper, freq, damping_ratio):
    A, T, _ = shaper
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
    A, T, name = shaper
    n = len(T)
    ts = sum([A[i] * T[i] for i in range(n)]) / sum(A)
    for i in range(n):
        T[i] -= ts

# Shaper selection
get_shaper = get_ei_shaper


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

def find_shaper_plot_range(shaper, vib_tol):
    def eval_shaper(freq):
        return estimate_shaper(shaper, freq, DAMPING_RATIOS[0]) - vib_tol
    if not PLOT_FREQ_RANGE:
        left = bisect(eval_shaper, 0., SHAPER_FREQ)
        right = bisect(eval_shaper, SHAPER_FREQ, 2.4 * SHAPER_FREQ)
    else:
        left, right = PLOT_FREQ_RANGE
    return (left, right)

def gen_shaper_response(shaper):
    # Calculate shaper vibration responce on a range of requencies
    response = []
    freqs = []
    freq, freq_end = find_shaper_plot_range(shaper, vib_tol=0.25)
    while freq <= freq_end:
        vals = []
        for damping_ratio in DAMPING_RATIOS:
            vals.append(estimate_shaper(shaper, freq, damping_ratio))
        response.append(vals)
        freqs.append(freq)
        freq += PLOT_FREQ_STEP
    legend = ['damping ratio = %.3f' % d_r for d_r in DAMPING_RATIOS]
    return freqs, response, legend

def gen_shaped_step_function(shaper):
    # Calculate shaping of a step function
    A, T, _ = shaper
    inv_D = 1. / sum(A)
    n = len(T)

    omega = 2. * math.pi * STEP_SIMULATION_RESONANCE_FREQ
    damping = STEP_SIMULATION_DAMPING_RATIO * omega
    omega_d = omega * math.sqrt(1. - STEP_SIMULATION_DAMPING_RATIO**2)
    phase = math.acos(STEP_SIMULATION_DAMPING_RATIO)

    t_start = T[0] - .5 / SHAPER_FREQ
    t_end = T[-1] + 1.5 / STEP_SIMULATION_RESONANCE_FREQ
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
        t += .01 / SHAPER_FREQ
    legend = ['step', 'shaper commanded', 'system response']
    return time, result, legend


def plot_shaper(shaper):
    shift_pulses(shaper)
    freqs, response, response_legend = gen_shaper_response(shaper)
    time, step_vals, step_legend = gen_shaped_step_function(shaper)

    fig, (ax1, ax2) = matplotlib.pyplot.subplots(nrows=2, figsize=(10,9))
    ax1.set_title("Vibration response simulation for shaper '%s',\n"
                  "shaper_freq=%.1f Hz, damping_ratio=%.3f"
                  % (shaper[-1], SHAPER_FREQ, SHAPER_DAMPING_RATIO))
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
                  "damping ratio=%.3f" % (STEP_SIMULATION_RESONANCE_FREQ,
                                          STEP_SIMULATION_DAMPING_RATIO))
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
    options, args = opts.parse_args()
    if len(args) != 0:
        opts.error("Incorrect number of arguments")

    # Draw graph
    setup_matplotlib(options.output is not None)
    fig = plot_shaper(get_shaper())

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
