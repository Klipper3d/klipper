#!/usr/bin/env python2
# Generate extruder pressure advance motion graphs
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, optparse, datetime
import matplotlib

SEG_TIME = .000100
INV_SEG_TIME = 1. / SEG_TIME


######################################################################
# Basic trapezoid motion
######################################################################

# List of moves: [(start_v, end_v, move_t), ...]
Moves = [
    (0., 0., .200),
    (0., 100., None), (100., 100., .200), (100., 60., None),
    (60., 100., None), (100., 100., .200), (100., 0., None),
    (0., 0., .300)
]
EXTRUDE_R = (.4 * .4 * .75) / (math.pi * (1.75 / 2.)**2)
ACCEL = 3000. * EXTRUDE_R

def gen_positions():
    out = []
    start_d = start_t = t = 0.
    for start_v, end_v, move_t in Moves:
        start_v *= EXTRUDE_R
        end_v *= EXTRUDE_R
        if move_t is None:
            move_t = abs(end_v - start_v) / ACCEL
        half_accel = 0.
        if end_v > start_v:
            half_accel = .5 * ACCEL
        elif start_v > end_v:
            half_accel = -.5 * ACCEL
        end_t = start_t + move_t
        while t <= end_t:
            rel_t = t - start_t
            out.append(start_d + (start_v + half_accel * rel_t) * rel_t)
            t += SEG_TIME
        start_d += (start_v + half_accel * move_t) * move_t
        start_t = end_t
    return out

def gen_deriv(data):
    return [0.] + [(data[i+1] - data[i]) * INV_SEG_TIME
                   for i in range(len(data)-1)]

def time_to_index(t):
    return int(t * INV_SEG_TIME + .5)


######################################################################
# Pressure advance
######################################################################

PA_HALF_SMOOTH_T = .040 / 2.
PRESSURE_ADVANCE = .045

# Calculate raw pressure advance positions
def calc_pa_raw(t, positions):
    pa = PRESSURE_ADVANCE * INV_SEG_TIME
    i = time_to_index(t)
    return positions[i] + pa * (positions[i+1] - positions[i])

# Pressure advance smoothed using average velocity (for reference only)
def calc_pa_average(t, positions):
    pa_factor = PRESSURE_ADVANCE / (2. * PA_HALF_SMOOTH_T)
    base_pos = positions[time_to_index(t)]
    start_pos = positions[time_to_index(t - PA_HALF_SMOOTH_T)]
    end_pos = positions[time_to_index(t + PA_HALF_SMOOTH_T)]
    return base_pos + (end_pos - start_pos) * pa_factor

# Pressure advance with simple time smoothing (for reference only)
def calc_pa_smooth(t, positions):
    start_index = time_to_index(t - PA_HALF_SMOOTH_T) + 1
    end_index = time_to_index(t + PA_HALF_SMOOTH_T)
    pa = PRESSURE_ADVANCE * INV_SEG_TIME
    pa_data = [positions[i] + pa * (positions[i+1] - positions[i])
               for i in range(start_index, end_index)]
    return sum(pa_data) / (end_index - start_index)

# Calculate pressure advance smoothed using a "weighted average"
def calc_pa_weighted(t, positions):
    base_index = time_to_index(t)
    start_index = time_to_index(t - PA_HALF_SMOOTH_T) + 1
    end_index = time_to_index(t + PA_HALF_SMOOTH_T)
    diff = .5 * (end_index - start_index)
    pa = PRESSURE_ADVANCE * INV_SEG_TIME
    pa_data = [(positions[i] + pa * (positions[i+1] - positions[i]))
               * (diff - abs(i-base_index))
               for i in range(start_index, end_index)]
    return sum(pa_data) / diff**2


######################################################################
# Plotting and startup
######################################################################

MARGIN_TIME = 0.100

def plot_motion():
    # Nominal motion
    positions = gen_positions()
    drop = int(MARGIN_TIME * INV_SEG_TIME)
    times = [SEG_TIME * t for t in range(len(positions))][drop:-drop]
    velocities = gen_deriv(positions[drop:-drop])
    # Motion with pressure advance
    pa_positions = [calc_pa_raw(t, positions) for t in times]
    pa_velocities = gen_deriv(pa_positions)
    # Smoothed motion
    sm_positions = [calc_pa_weighted(t, positions) for t in times]
    sm_velocities = gen_deriv(sm_positions)
    # Build plot
    shift_times = [t - MARGIN_TIME for t in times]
    fig, ax1 = matplotlib.pyplot.subplots(nrows=1, sharex=True)
    ax1.set_title("Extruder Velocity")
    ax1.set_ylabel('Velocity (mm/s)')
    pa_plot, = ax1.plot(shift_times, pa_velocities, 'r',
                        label='Pressure Advance', alpha=0.3)
    nom_plot, = ax1.plot(shift_times, velocities, 'black', label='Nominal')
    sm_plot, = ax1.plot(shift_times, sm_velocities, 'g', label='Smooth PA',
                        alpha=0.9)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax1.legend(handles=[nom_plot, pa_plot, sm_plot], loc='best', prop=fontP)
    ax1.set_xlabel('Time (s)')
    ax1.grid(True)
    fig.tight_layout()
    return fig

def setup_matplotlib(output_to_file):
    global matplotlib
    if output_to_file:
        matplotlib.rcParams.update({'figure.autolayout': True})
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
    fig = plot_motion()

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(6, 2.5)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
