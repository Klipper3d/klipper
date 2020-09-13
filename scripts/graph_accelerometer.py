#!/usr/bin/env python2
# Generate adxl345 accelerometer graphs
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse
import numpy as np, matplotlib

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


######################################################################
# Raw accelerometer graphing
######################################################################

def plot_accel(data, logname):
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


######################################################################
# Frequency graphing
######################################################################

WINDOW_T_SEC = 0.5

class CalibrationData:
    def __init__(self, freq_bins, px, py, pz):
        self.freq_bins = freq_bins
        self.px, self.py, self.pz = px, py, pz
        self.pall = px + py + pz
        avgs = [np.average(d, axis=1) for d in [self.pall, px, py, pz]]
        self.psd_sum, self.psd_x, self.psd_y, self.psd_z = avgs
    def get_psd_axis(self, axis):
        a = {'all': self.pall, 'x': self.px, 'y': self.py, 'z': self.pz}
        return a[axis]

# Calculate estimated "power spectral density"
def calc_freq_response(data, max_freq):
    data = np.array(data)
    N = data.shape[0]
    T = data[-1,0] - data[0,0]
    SAMPLING_FREQ = N / T
    # Round down to a power of 2 for faster FFT
    M = 1 << int(SAMPLING_FREQ * WINDOW_T_SEC - 1).bit_length()

    # Calculate PSD (power spectral density) of vibrations per window per
    # frequency bins (the same bins for X, Y, and Z)
    specgram = matplotlib.mlab.specgram
    px, fx, _ = specgram(data[:,1], Fs=SAMPLING_FREQ, NFFT=M, noverlap=M//2,
                         window=np.blackman(M), detrend='mean', mode='psd')
    py, fy, _ = specgram(data[:,2], Fs=SAMPLING_FREQ, NFFT=M, noverlap=M//2,
                         window=np.blackman(M), detrend='mean', mode='psd')
    pz, fz, _ = specgram(data[:,3], Fs=SAMPLING_FREQ, NFFT=M, noverlap=M//2,
                         window=np.blackman(M), detrend='mean', mode='psd')
    return CalibrationData(fx, px, py, pz)

def plot_frequency(data, logname, max_freq):
    calibration_data = calc_freq_response(data, max_freq)
    freqs = calibration_data.freq_bins
    psd = calibration_data.psd_sum[freqs <= max_freq]
    px = calibration_data.psd_x[freqs <= max_freq]
    py = calibration_data.psd_y[freqs <= max_freq]
    pz = calibration_data.psd_z[freqs <= max_freq]
    freqs = freqs[freqs <= max_freq]

    fig, ax = matplotlib.pyplot.subplots()
    ax.set_title("Accelerometer data (%s)" % (logname,))
    ax.set_xlabel('Frequency (Hz)')
    ax.set_ylabel('Power spectral density')

    ax.plot(freqs, psd, label='X+Y+Z', alpha=0.6)
    ax.plot(freqs, px, label='X', alpha=0.6)
    ax.plot(freqs, py, label='Y', alpha=0.6)
    ax.plot(freqs, pz, label='Z', alpha=0.6)

    ax.grid(True)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax.legend(loc='best', prop=fontP)
    fig.tight_layout()
    return fig

def plot_compare_frequency(datas, lognames, max_freq):
    fig, ax = matplotlib.pyplot.subplots()
    ax.set_title("Accelerometer data")
    ax.set_xlabel('Frequency (Hz)')
    ax.set_ylabel('Power spectral density')

    for data, logname in zip(datas, lognames):
        calibration_data = calc_freq_response(data, max_freq)
        freqs = calibration_data.freq_bins
        psd = calibration_data.psd_sum[freqs <= max_freq]
        px = calibration_data.psd_x[freqs <= max_freq]
        py = calibration_data.psd_y[freqs <= max_freq]
        pz = calibration_data.psd_z[freqs <= max_freq]
        freqs = freqs[freqs <= max_freq]
        ax.plot(freqs, psd, label=logname, alpha=0.6)

    ax.grid(True)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax.legend(loc='best', prop=fontP)
    fig.tight_layout()
    return fig

# Plot data in a "spectogram colormap"
def plot_specgram(data, logname, max_freq, axis):
    calibration_data = calc_freq_response(data, max_freq)
    pdata = calibration_data.get_psd_axis(axis)

    fig, ax = matplotlib.pyplot.subplots()
    ax.set_title("Spectogram %s (%s)" % (axis, logname))
    ax.pcolormesh(pdata, norm=matplotlib.colors.LogNorm())
    ax.set_ylim([0., max_freq])
    ax.set_ylabel('frequency (hz)')
    ax.set_xlabel('Time')
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
    opts.add_option("-f", "--max_freq", type="float", default=200.,
                    help="maximum frequency to graph")
    opts.add_option("-r", "--raw", action="store_true",
                    help="graph raw accelerometer data")
    opts.add_option("-s", "--specgram", action="store_true",
                    help="graph spectogram of accelerometer data")
    opts.add_option("-a", type="string", dest="axis", default="all",
                    help="axis to graph (one of 'all', 'x', 'y', or 'z')")
    options, args = opts.parse_args()
    if len(args) < 1:
        opts.error("Incorrect number of arguments")

    # Parse data
    datas = [parse_log(fn) for fn in args]

    # Draw graph
    setup_matplotlib(options.output is not None)
    if options.raw:
        fig = plot_accel(datas[0], args[0])
    elif options.specgram:
        fig = plot_specgram(datas[0], args[0], options.max_freq, options.axis)
    elif len(args) > 1:
        fig = plot_compare_frequency(datas, args, options.max_freq)
    else:
        fig = plot_frequency(datas[0], args[0], options.max_freq)

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
