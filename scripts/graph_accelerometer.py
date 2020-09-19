#!/usr/bin/env python2
# Generate adxl345 accelerometer graphs
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse
import matplotlib

def parse_log(logname):
    f = open(logname, 'r')
    out = []
    for line in f:
        if line.startswith('#'):
            continue
        parts = line.split(',')
        if len(parts) != 4:
            continue
        try:
            fparts = [float(p) for p in parts]
        except ValueError:
            continue
        out.append(fparts)
    return out

def plot_accel(data, logname):
    half_smooth_samples = 15
    first_time = data[0][0]
    times = [d[0] - first_time for d in data]
    fig, axes = matplotlib.pyplot.subplots(nrows=3, sharex=True)
    axes[0].set_title("Accelerometer data (%s)" % (logname,))
    axis_names = ['x', 'y', 'z']
    for i in range(len(axis_names)):
        adata = [d[i+1] for d in data]
        avg = sum(adata) / len(adata)
        adata = [ad - avg for ad in adata]
        ax = axes[i]
        ax.plot(times, adata, alpha=0.8)
        ax.grid(True)
        ax.set_ylabel('%s accel (%+.3f)\n(mm/s^2)' % (axis_names[i], -avg))
    axes[-1].set_xlabel('Time (%+.3f)\n(s)' % (-first_time,))
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
    usage = "%prog [options] <log>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--output", type="string", dest="output",
                    default=None, help="filename of output graph")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")

    # Parse data
    data = parse_log(args[0])

    # Draw graph
    setup_matplotlib(options.output is not None)
    fig = plot_accel(data, args[0])

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
