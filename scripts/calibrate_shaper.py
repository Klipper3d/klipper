#!/usr/bin/env python3
# Shaper auto-calibration script
#
# Copyright (C) 2020-2024  Dmitry Butyugin <dmbutyugin@google.com>
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from __future__ import print_function
import importlib, optparse, os, sys
from textwrap import wrap
import numpy as np, matplotlib
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),
                             '..', 'klippy'))
shaper_calibrate = importlib.import_module('.shaper_calibrate', 'extras')

MAX_TITLE_LENGTH=65

def parse_log(logname):
    with open(logname) as f:
        for header in f:
            if not header.startswith('#'):
                break
        if not header.startswith('freq,psd_x,psd_y,psd_z,psd_xyz'):
            # Raw accelerometer data
            return np.loadtxt(logname, comments='#', delimiter=',')
    # Parse power spectral density data
    data = np.loadtxt(logname, skiprows=1, comments='#', delimiter=',')
    calibration_data = shaper_calibrate.CalibrationData(
            freq_bins=data[:,0], psd_sum=data[:,4],
            psd_x=data[:,1], psd_y=data[:,2], psd_z=data[:,3])
    calibration_data.set_numpy(np)
    # If input shapers are present in the CSV file, the frequency
    # response is already normalized to input frequencies
    if 'mzv' not in header:
        calibration_data.normalize_to_frequencies()
    return calibration_data

######################################################################
# Shaper calibration
######################################################################

# Find the best shaper parameters
def calibrate_shaper(datas, csv_output, *, shapers, damping_ratio, scv,
                     shaper_freqs, max_smoothing, test_damping_ratios,
                     max_freq):
    helper = shaper_calibrate.ShaperCalibrate(printer=None)
    if isinstance(datas[0], shaper_calibrate.CalibrationData):
        calibration_data = datas[0]
        for data in datas[1:]:
            calibration_data.add_data(data)
    else:
        # Process accelerometer data
        calibration_data = helper.process_accelerometer_data(datas[0])
        for data in datas[1:]:
            calibration_data.add_data(helper.process_accelerometer_data(data))
        calibration_data.normalize_to_frequencies()


    shaper, all_shapers = helper.find_best_shaper(
            calibration_data, shapers=shapers, damping_ratio=damping_ratio,
            scv=scv, shaper_freqs=shaper_freqs, max_smoothing=max_smoothing,
            test_damping_ratios=test_damping_ratios, max_freq=max_freq,
            logger=print)
    if not shaper:
        print("No recommended shaper, possibly invalid value for --shapers=%s" %
              (','.join(shapers)))
        return None, None, None
    print("Recommended shaper is %s @ %.1f Hz" % (shaper.name, shaper.freq))
    if csv_output is not None:
        helper.save_calibration_data(
                csv_output, calibration_data, all_shapers)
    return shaper.name, all_shapers, calibration_data

######################################################################
# Plot frequency response and suggested input shapers
######################################################################

def plot_freq_response(lognames, calibration_data, shapers,
                       selected_shaper, max_freq):
    freqs = calibration_data.freq_bins
    psd = calibration_data.psd_sum[freqs <= max_freq]
    px = calibration_data.psd_x[freqs <= max_freq]
    py = calibration_data.psd_y[freqs <= max_freq]
    pz = calibration_data.psd_z[freqs <= max_freq]
    freqs = freqs[freqs <= max_freq]

    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')

    fig, ax = matplotlib.pyplot.subplots()
    ax.set_xlabel('Frequency, Hz')
    ax.set_xlim([0, max_freq])
    ax.set_ylabel('Power spectral density')

    ax.plot(freqs, psd, label='X+Y+Z', color='purple')
    ax.plot(freqs, px, label='X', color='red')
    ax.plot(freqs, py, label='Y', color='green')
    ax.plot(freqs, pz, label='Z', color='blue')

    title = "Frequency response and shapers (%s)" % (', '.join(lognames))
    ax.set_title("\n".join(wrap(title, MAX_TITLE_LENGTH)))
    ax.xaxis.set_minor_locator(matplotlib.ticker.MultipleLocator(5))
    ax.yaxis.set_minor_locator(matplotlib.ticker.AutoMinorLocator())
    ax.ticklabel_format(axis='y', style='scientific', scilimits=(0,0))
    ax.grid(which='major', color='grey')
    ax.grid(which='minor', color='lightgrey')

    ax2 = ax.twinx()
    ax2.set_ylabel('Shaper vibration reduction (ratio)')
    best_shaper_vals = None
    for shaper in shapers:
        label = "%s (%.1f Hz, vibr=%.1f%%, sm~=%.2f, accel<=%.f)" % (
                shaper.name.upper(), shaper.freq,
                shaper.vibrs * 100., shaper.smoothing,
                round(shaper.max_accel / 100.) * 100.)
        linestyle = 'dotted'
        if shaper.name == selected_shaper:
            linestyle = 'dashdot'
            best_shaper_vals = shaper.vals
        ax2.plot(freqs, shaper.vals, label=label, linestyle=linestyle)
    ax.plot(freqs, psd * best_shaper_vals,
            label='After\nshaper', color='cyan')
    # A hack to add a human-readable shaper recommendation to legend
    ax2.plot([], [], ' ',
             label="Recommended shaper: %s" % (selected_shaper.upper()))

    ax.legend(loc='upper left', prop=fontP)
    ax2.legend(loc='upper right', prop=fontP)

    fig.tight_layout()
    return fig

######################################################################
# Startup
######################################################################

def setup_matplotlib(output_to_file):
    global matplotlib
    if output_to_file:
        matplotlib.rcParams.update({'figure.autolayout': True})
        matplotlib.use('Agg')
    import matplotlib.pyplot, matplotlib.dates, matplotlib.font_manager
    import matplotlib.ticker

def main():
    # Parse command-line arguments
    usage = "%prog [options] <logs>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--output", type="string", dest="output",
                    default=None, help="filename of output graph")
    opts.add_option("-c", "--csv", type="string", dest="csv",
                    default=None, help="filename of output csv file")
    opts.add_option("-f", "--max_freq", type="float", default=200.,
                    help="maximum frequency to plot")
    opts.add_option("-s", "--max_smoothing", type="float", dest="max_smoothing",
                    default=None, help="maximum shaper smoothing to allow")
    opts.add_option("--scv", "--square_corner_velocity", type="float",
                    dest="scv", default=5., help="square corner velocity")
    opts.add_option("--shaper_freq", type="string", dest="shaper_freq",
                    default=None, help="shaper frequency(-ies) to test, " +
                    "either a comma-separated list of floats, or a range in " +
                    "the format [start]:end[:step]")
    opts.add_option("--shapers", type="string", dest="shapers", default=None,
                    help="a comma-separated list of shapers to test")
    opts.add_option("--damping_ratio", type="float", dest="damping_ratio",
                    default=None, help="shaper damping_ratio parameter")
    opts.add_option("--test_damping_ratios", type="string",
                    dest="test_damping_ratios", default=None,
                    help="a comma-separated liat of damping ratios to test " +
                    "input shaper for")
    options, args = opts.parse_args()
    if len(args) < 1:
        opts.error("Incorrect number of arguments")
    if options.max_smoothing is not None and options.max_smoothing < 0.05:
        opts.error("Too small max_smoothing specified (must be at least 0.05)")

    max_freq = options.max_freq
    if options.shaper_freq is None:
        shaper_freqs = []
    elif options.shaper_freq.find(':') >= 0:
        freq_start = None
        freq_end = None
        freq_step = None
        try:
            freqs_parsed = options.shaper_freq.partition(':')
            if freqs_parsed[0]:
                freq_start = float(freqs_parsed[0])
            freqs_parsed = freqs_parsed[-1].partition(':')
            freq_end = float(freqs_parsed[0])
            if freq_start and freq_start > freq_end:
                opts.error("Invalid --shaper_freq param: start range larger " +
                           "than its end")
            if freqs_parsed[-1].find(':') >= 0:
                opts.error("Invalid --shaper_freq param format")
            if freqs_parsed[-1]:
                freq_step = float(freqs_parsed[-1])
        except ValueError:
            opts.error("--shaper_freq param does not specify correct range " +
                       "in the format [start]:end[:step]")
        shaper_freqs = (freq_start, freq_end, freq_step)
        max_freq = max(max_freq, freq_end * 4./3.)
    else:
        try:
            shaper_freqs = [float(s) for s in options.shaper_freq.split(',')]
        except ValueError:
            opts.error("invalid floating point value in --shaper_freq param")
        max_freq = max(max_freq, max(shaper_freqs) * 4./3.)
    if options.test_damping_ratios:
        try:
            test_damping_ratios = [float(s) for s in
                                   options.test_damping_ratios.split(',')]
        except ValueError:
            opts.error("invalid floating point value in " +
                       "--test_damping_ratios param")
    else:
        test_damping_ratios = None
    if options.shapers is None:
        shapers = None
    else:
        shapers = options.shapers.lower().split(',')

    # Parse data
    datas = [parse_log(fn) for fn in args]

    # Calibrate shaper and generate outputs
    selected_shaper, shapers, calibration_data = calibrate_shaper(
            datas, options.csv, shapers=shapers,
            damping_ratio=options.damping_ratio,
            scv=options.scv, shaper_freqs=shaper_freqs,
            max_smoothing=options.max_smoothing,
            test_damping_ratios=test_damping_ratios,
            max_freq=max_freq)
    if selected_shaper is None:
        return

    if not options.csv or options.output:
        # Draw graph
        setup_matplotlib(options.output is not None)

        fig = plot_freq_response(args, calibration_data, shapers,
                                 selected_shaper, max_freq)

        # Show graph
        if options.output is None:
            matplotlib.pyplot.show()
        else:
            fig.set_size_inches(8, 6)
            fig.savefig(options.output)

if __name__ == '__main__':
    main()
