#!/usr/bin/env python2
# Script to graph motion results
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse, datetime, math
import matplotlib

SEG_TIME = .000100
INV_SEG_TIME = 1. / SEG_TIME


######################################################################
# Basic trapezoid motion
######################################################################

# List of moves: [(start_v, end_v, move_t), ...]
Moves = [
    # X velocities from: 0,0 -> 0,20 -> 40,40 -> 80,40 -> 80,80
    (0., 0., .200),
    (6.869, 89.443, None), (89.443, 89.443, .200), (89.443, 17.361, None),
    (19.410, 100., None), (100., 100., .200), (100., 5., None),
    (0., 0., .300)
]
ACCEL = 3000.

# Standard constant acceleration generator
def get_acc_pos_ao2(rel_t, start_v, accel, move_t):
    return (start_v + 0.5 * accel * rel_t) * rel_t

# Bezier curve "accel_order=4" generator
def get_acc_pos_ao4(rel_t, start_v, accel, move_t):
    inv_accel_t = 1. / move_t
    accel_div_accel_t = accel * inv_accel_t
    accel_div_accel_t2 = accel_div_accel_t * inv_accel_t

    c4 = -.5 * accel_div_accel_t2;
    c3 = accel_div_accel_t;
    c1 = start_v
    return ((c4 * rel_t + c3) * rel_t * rel_t + c1) * rel_t

# Bezier curve "accel_order=6" generator
def get_acc_pos_ao6(rel_t, start_v, accel, move_t):
    inv_accel_t = 1. / move_t
    accel_div_accel_t = accel * inv_accel_t
    accel_div_accel_t2 = accel_div_accel_t * inv_accel_t
    accel_div_accel_t3 = accel_div_accel_t2 * inv_accel_t
    accel_div_accel_t4 = accel_div_accel_t3 * inv_accel_t

    c6 = accel_div_accel_t4;
    c5 = -3. * accel_div_accel_t3;
    c4 = 2.5 * accel_div_accel_t2;
    c1 = start_v;
    return (((c6 * rel_t + c5) * rel_t + c4)
            * rel_t * rel_t * rel_t + c1) * rel_t

get_acc_pos = get_acc_pos_ao2

# Calculate positions based on 'Moves' list
def gen_positions():
    out = []
    start_d = start_t = t = 0.
    for start_v, end_v, move_t in Moves:
        if move_t is None:
            move_t = abs(end_v - start_v) / ACCEL
        accel = 0.
        if end_v > start_v:
            accel = ACCEL
        elif start_v > end_v:
            accel = -ACCEL
        end_t = start_t + move_t
        while t <= end_t:
            rel_t = t - start_t
            out.append(start_d + get_acc_pos(rel_t, start_v, accel, move_t))
            t += SEG_TIME
        start_d += get_acc_pos(move_t, start_v, accel, move_t)
        start_t = end_t
    return out

def gen_deriv(data):
    return [0.] + [(data[i+1] - data[i]) * INV_SEG_TIME
                   for i in range(len(data)-1)]

def time_to_index(t):
    return int(t * INV_SEG_TIME + .5)


######################################################################
# Estimated motion with belt as spring
######################################################################

SPRING_FREQ = 35.0
DAMPING = 30.

def estimate_spring(positions):
    ang_freq2 = (SPRING_FREQ * 2. * math.pi)**2
    head_pos = head_v = 0.
    out = []
    for stepper_pos in positions:
        head_pos += head_v * SEG_TIME
        head_a = (stepper_pos - head_pos) * ang_freq2
        head_v += head_a * SEG_TIME
        head_v -= head_v * DAMPING * SEG_TIME
        out.append(head_pos)
    return out


######################################################################
# Motion functions
######################################################################

HALF_SMOOTH_T = .040 / 2.

def calc_position_average(t, positions):
    start_pos = positions[time_to_index(t - HALF_SMOOTH_T)]
    end_pos = positions[time_to_index(t + HALF_SMOOTH_T)]
    return .5 * (start_pos + end_pos)

def calc_position_smooth(t, positions):
    start_index = time_to_index(t - HALF_SMOOTH_T) + 1
    end_index = time_to_index(t + HALF_SMOOTH_T)
    return sum(positions[start_index:end_index]) / (end_index - start_index)

def calc_position_weighted(t, positions):
    base_index = time_to_index(t)
    start_index = time_to_index(t - HALF_SMOOTH_T) + 1
    end_index = time_to_index(t + HALF_SMOOTH_T)
    diff = .5 * (end_index - start_index)
    weighted_data = [positions[i] * (diff - abs(i-base_index))
                     for i in range(start_index, end_index)]
    return sum(weighted_data) / diff**2

SPRING_ADVANCE = .000020
RESISTANCE_ADVANCE = 0.

def calc_spring_weighted(t, positions):
    base_index = time_to_index(t)
    start_index = time_to_index(t - HALF_SMOOTH_T)
    end_index = time_to_index(t + HALF_SMOOTH_T)
    diff = .5 * (end_index - start_index)
    sa = SPRING_ADVANCE * INV_SEG_TIME * INV_SEG_TIME
    ra = RESISTANCE_ADVANCE * INV_SEG_TIME
    sa_data = [(positions[i]
                + sa * (positions[i-1] - 2.*positions[i] + positions[i+1])
                + ra * (positions[i+1] - positions[i]))
               * (diff - abs(i-base_index))
               for i in range(start_index, end_index)]
    return sum(sa_data) / diff**2

def calc_spring_double_weighted(t, positions):
    base_index = time_to_index(t)
    start_index = time_to_index(t - HALF_SMOOTH_T)
    end_index = time_to_index(t + HALF_SMOOTH_T)
    avg_v = base_index - start_index
    diff = .5 * (end_index - start_index)
    sa = SPRING_ADVANCE * (INV_SEG_TIME / avg_v)**2
    ra = RESISTANCE_ADVANCE * INV_SEG_TIME
    sa_data = [(positions[i]
                + sa * (positions[i-avg_v] - 2.*positions[i]
                        + positions[i+avg_v])
                + ra * (positions[i+1] - positions[i]))
               * (diff - abs(i-base_index))
               for i in range(start_index, end_index)]
    return sum(sa_data) / diff**2

# Ideal values
SPRING_ADVANCE = 1. / ((SPRING_FREQ * 2. * math.pi)**2)
RESISTANCE_ADVANCE = DAMPING * SPRING_ADVANCE
HALF_SMOOTH_T = (1./3.) * 2. * math.pi * math.sqrt(SPRING_ADVANCE) / 2.


######################################################################
# Plotting and startup
######################################################################

#gen_updated_position = calc_position_weighted
gen_updated_position = calc_spring_double_weighted

MARGIN_TIME = 0.100

def plot_motion():
    # Nominal motion
    positions = gen_positions()
    drop = int(MARGIN_TIME * INV_SEG_TIME)
    times = [SEG_TIME * t for t in range(len(positions))][drop:-drop]
    margin_positions = positions[drop:-drop]
    velocities = gen_deriv(margin_positions)
    accels = gen_deriv(velocities)
    # Updated motion
    upd_positions = [gen_updated_position(t, positions) for t in times]
    upd_velocities = gen_deriv(upd_positions)
    upd_accels = gen_deriv(upd_velocities)
    # Estimated position with model of belt as spring
    spring_orig = estimate_spring(margin_positions)
    spring_upd = estimate_spring(upd_positions)
    spring_diff_orig = [n-o for n, o in zip(spring_orig, margin_positions)]
    spring_diff_upd = [n-o for n, o in zip(spring_upd, margin_positions)]
    head_velocities = gen_deriv(spring_orig)
    head_accels = gen_deriv(head_velocities)
    head_upd_velocities = gen_deriv(spring_upd)
    head_upd_accels = gen_deriv(head_upd_velocities)
    # Build plot
    shift_times = [t - MARGIN_TIME for t in times]
    fig, (ax1, ax2, ax3) = matplotlib.pyplot.subplots(nrows=3, sharex=True)
    ax1.set_title("Simulation (belt frequency=%.3f, damping=%.3f)"
                  % (SPRING_FREQ, DAMPING))
    ax1.set_ylabel('Velocity (mm/s)')
    ax1.plot(shift_times, upd_velocities, 'r', label='New Velocity', alpha=0.8)
    ax1.plot(shift_times, velocities, 'g', label='Nominal Velocity', alpha=0.8)
    ax1.plot(shift_times, head_velocities, label='Head Velocity', alpha=0.4)
    ax1.plot(shift_times, head_upd_velocities, label='New Head Velocity',
             alpha=0.4)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax1.legend(loc='best', prop=fontP)
    ax1.grid(True)
    ax2.set_ylabel('Acceleration (mm/s^2)')
    ax2.plot(shift_times, upd_accels, 'r', label='New Accel', alpha=0.8)
    ax2.plot(shift_times, accels, 'g', label='Nominal Accel', alpha=0.8)
    ax2.plot(shift_times, head_accels, alpha=0.4)
    ax2.plot(shift_times, head_upd_accels, alpha=0.4)
    ax2.set_ylim([-5. * ACCEL, 5. * ACCEL])
    ax2.legend(loc='best', prop=fontP)
    ax2.grid(True)
    ax3.set_ylabel('Deviation (mm)')
    ax3.plot(shift_times, spring_diff_upd, 'r', label='New', alpha=0.8)
    ax3.plot(shift_times, spring_diff_orig, 'g', label='Nominal', alpha=0.8)
    ax3.grid(True)
    ax3.legend(loc='best', prop=fontP)
    ax3.set_xlabel('Time (s)')
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
    fig = plot_motion()

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
