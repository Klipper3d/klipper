#!/usr/bin/env python3
# Generate adxl345 accelerometer graphs
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import importlib, optparse, os, sys
from textwrap import wrap
import numpy as np, matplotlib
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),
                             '..', 'klippy'))
shaper_calibrate = importlib.import_module('.shaper_calibrate', 'extras')

MAX_TITLE_LENGTH=65

def parse_log(logname, opts):
    with open(logname) as f:
        for header in f:
            if header.startswith('#'):
                continue
            if header.startswith('freq,psd_x,psd_y,psd_z,psd_xyz'):
                # Processed power spectral density file
                break
            # Raw accelerometer data
            return np.loadtxt(logname, comments='#', delimiter=',')
    # Parse power spectral density data
    data = np.loadtxt(logname, skiprows=1, comments='#', delimiter=',')
    calibration_data = shaper_calibrate.CalibrationData(
            freq_bins=data[:,0], psd_sum=data[:,4],
            psd_x=data[:,1], psd_y=data[:,2], psd_z=data[:,3])
    calibration_data.set_numpy(np)
    return calibration_data

######################################################################
# Raw accelerometer graphing
######################################################################

def plot_accel(datas, lognames):
    fig, axes = matplotlib.pyplot.subplots(nrows=3, sharex=True)
    axes[0].set_title("\n".join(wrap(
        "Accelerometer data (%s)" % (', '.join(lognames)), MAX_TITLE_LENGTH)))
    axis_names = ['x', 'y', 'z']
    for data, logname in zip(datas, lognames):
        if isinstance(data, shaper_calibrate.CalibrationData):
            raise error("Cannot plot raw accelerometer data using the processed"
                        " resonances, raw_data input is required")
        first_time = data[0, 0]
        times = data[:,0] - first_time
        for i in range(len(axis_names)):
            avg = data[:,i+1].mean()
            adata = data[:,i+1] - data[:,i+1].mean()
            ax = axes[i]
            label = '\n'.join(wrap(logname, 60)) + ' (%+.3f mm/s^2)' % (-avg,)
            ax.plot(times, adata, alpha=0.8, label=label)
    axes[-1].set_xlabel('Time (s)')
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    for i in range(len(axis_names)):
        ax = axes[i]
        ax.grid(True)
        ax.legend(loc='best', prop=fontP)
        ax.set_ylabel('%s accel' % (axis_names[i],))
    fig.tight_layout()
    return fig


######################################################################
# Frequency graphing
######################################################################

# Calculate estimated "power spectral density"
def calc_freq_response(data, max_freq):
    if isinstance(data, shaper_calibrate.CalibrationData):
        return data
    helper = shaper_calibrate.ShaperCalibrate(printer=None)
    return helper.process_accelerometer_data(data)

def calc_specgram(data, axis):
    if isinstance(data, shaper_calibrate.CalibrationData):
        raise error("Cannot calculate the spectrogram using the processed"
                    " resonances, raw_data input is required")
    N = data.shape[0]
    Fs = N / (data[-1,0] - data[0,0])
    # Round up to a power of 2 for faster FFT
    M = 1 << int(.5 * Fs - 1).bit_length()
    window = np.kaiser(M, 6.)
    def _specgram(x):
        return matplotlib.mlab.specgram(
                x, Fs=Fs, NFFT=M, noverlap=M//2, window=window,
                mode='psd', detrend='mean', scale_by_freq=False)

    d = {'x': data[:,1], 'y': data[:,2], 'z': data[:,3]}
    if axis != 'all':
        pdata, bins, t = _specgram(d[axis])
    else:
        pdata, bins, t = _specgram(d['x'])
        for ax in 'yz':
            pdata += _specgram(d[ax])[0]
    return pdata, bins, t

def plot_frequency(datas, lognames, max_freq):
    calibration_data = calc_freq_response(datas[0], max_freq)
    for data in datas[1:]:
        calibration_data.add_data(calc_freq_response(data, max_freq))
    freqs = calibration_data.freq_bins
    psd = calibration_data.psd_sum[freqs <= max_freq]
    px = calibration_data.psd_x[freqs <= max_freq]
    py = calibration_data.psd_y[freqs <= max_freq]
    pz = calibration_data.psd_z[freqs <= max_freq]
    freqs = freqs[freqs <= max_freq]

    fig, ax = matplotlib.pyplot.subplots()
    ax.set_title("\n".join(wrap(
        "Frequency response (%s)" % (', '.join(lognames)), MAX_TITLE_LENGTH)))
    ax.set_xlabel('Frequency (Hz)')
    ax.set_ylabel('Power spectral density')

    ax.plot(freqs, psd, label='X+Y+Z', alpha=0.6)
    ax.plot(freqs, px, label='X', alpha=0.6)
    ax.plot(freqs, py, label='Y', alpha=0.6)
    ax.plot(freqs, pz, label='Z', alpha=0.6)

    ax.xaxis.set_minor_locator(matplotlib.ticker.AutoMinorLocator())
    ax.yaxis.set_minor_locator(matplotlib.ticker.AutoMinorLocator())
    ax.grid(which='major', color='grey')
    ax.grid(which='minor', color='lightgrey')
    ax.ticklabel_format(axis='y', style='scientific', scilimits=(0,0))

    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax.legend(loc='best', prop=fontP)
    fig.tight_layout()
    return fig

def plot_compare_frequency(datas, lognames, max_freq, axis):
    fig, ax = matplotlib.pyplot.subplots()
    ax.set_title('Frequency responses comparison')
    ax.set_xlabel('Frequency (Hz)')
    ax.set_ylabel('Power spectral density')

    for data, logname in zip(datas, lognames):
        calibration_data = calc_freq_response(data, max_freq)
        freqs = calibration_data.freq_bins
        psd = calibration_data.get_psd(axis)[freqs <= max_freq]
        freqs = freqs[freqs <= max_freq]
        ax.plot(freqs, psd, label="\n".join(wrap(logname, 60)), alpha=0.6)

    ax.xaxis.set_minor_locator(matplotlib.ticker.AutoMinorLocator())
    ax.yaxis.set_minor_locator(matplotlib.ticker.AutoMinorLocator())
    ax.grid(which='major', color='grey')
    ax.grid(which='minor', color='lightgrey')
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax.legend(loc='best', prop=fontP)
    fig.tight_layout()
    return fig

# Plot data in a "spectrogram colormap"
def plot_specgram(data, logname, max_freq, axis):
    pdata, bins, t = calc_specgram(data, axis)

    fig, ax = matplotlib.pyplot.subplots()
    ax.set_title("\n".join(wrap("Spectrogram %s (%s)" % (axis, logname),
                 MAX_TITLE_LENGTH)))
    ax.pcolormesh(t, bins, pdata, norm=matplotlib.colors.LogNorm())
    ax.set_ylim([0., max_freq])
    ax.set_ylabel('frequency (hz)')
    ax.set_xlabel('Time (s)')
    fig.tight_layout()
    return fig

######################################################################
# CSV output
######################################################################

def write_frequency_response(datas, output):
    helper = shaper_calibrate.ShaperCalibrate(printer=None)
    calibration_data = helper.process_accelerometer_data(datas[0])
    for data in datas[1:]:
        calibration_data.add_data(helper.process_accelerometer_data(data))
    helper.save_calibration_data(output, calibration_data)

def write_specgram(psd, freq_bins, time, output):
    M = freq_bins.shape[0]
    with open(output, "w") as csvfile:
        csvfile.write("freq\\t")
        for ts in time:
            csvfile.write(",%.6f" % (ts,))
        csvfile.write("\n")
        for i in range(M):
            csvfile.write("%.1f" % (freq_bins[i],))
            for value in psd[i,:]:
                csvfile.write(",%.6e" % (value,))
            csvfile.write("\n")

######################################################################
# Startup
######################################################################

def is_csv_output(output):
    return output and os.path.splitext(output)[1].lower() == '.csv'

def setup_matplotlib(output):
    global matplotlib
    if is_csv_output(output):
        # Only mlab may be necessary with CSV output
        import matplotlib.mlab
        return
    if output:
        matplotlib.rcParams.update({'figure.autolayout': True})
        matplotlib.use('Agg')
    import matplotlib.pyplot, matplotlib.dates, matplotlib.font_manager
    import matplotlib.ticker

def main():
    # Parse command-line arguments
    usage = "%prog [options] <raw logs>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--output", type="string", dest="output",
                    default=None, help="filename of output graph")
    opts.add_option("-f", "--max_freq", type="float", default=200.,
                    help="maximum frequency to graph")
    opts.add_option("-r", "--raw", action="store_true",
                    help="graph raw accelerometer data")
    opts.add_option("-c", "--compare", action="store_true",
                    help="graph comparison of power spectral density "
                         "between different accelerometer data files")
    opts.add_option("-s", "--specgram", action="store_true",
                    help="graph spectrogram of accelerometer data")
    opts.add_option("-a", type="string", dest="axis", default="all",
                    help="axis to graph (one of 'all', 'x', 'y', or 'z')")
    options, args = opts.parse_args()
    if len(args) < 1:
        opts.error("Incorrect number of arguments")

    # Parse data
    datas = [parse_log(fn, opts) for fn in args]

    setup_matplotlib(options.output)

    if is_csv_output(options.output):
        if options.raw:
            opts.error("raw mode is not supported with csv output")
        if options.compare:
            opts.error("comparison mode is not supported with csv output")
        if options.specgram:
            if len(args) > 1:
                opts.error("Only 1 input is supported in specgram mode")
            pdata, bins, t = calc_specgram(datas[0], options.axis)
            write_specgram(pdata, bins, t, options.output)
        else:
            write_frequency_response(datas, options.output)
        return

    # Draw graph
    if options.raw:
        fig = plot_accel(datas, args)
    elif options.specgram:
        if len(args) > 1:
            opts.error("Only 1 input is supported in specgram mode")
        fig = plot_specgram(datas[0], args[0], options.max_freq, options.axis)
    elif options.compare:
        fig = plot_compare_frequency(datas, args, options.max_freq,
                                     options.axis)
    else:
        fig = plot_frequency(datas, args, options.max_freq)

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
