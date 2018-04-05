#!/usr/bin/env python2
# Graphing script for first order plus delay time (fopdt) heater calibration
#
# Copyright (C) 2018-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, optparse
import matplotlib

# This script is intended to graph the results of a HEATER_CALIBRATE
# command.  Run the HEATER_CALIBRATE with a WRITE_FILE=1 parameter.
# This will cause a /tmp/heattest.txt file to be generated.  That file
# can be read by this script and graphed.

# Helper code that implements coordinate descent
def coordinate_descent(adj_params, params, error_func):
    # Define potential changes
    params = dict(params)
    dp = {param_name: 1. for param_name in adj_params}
    # Calculate the error
    best_err = error_func(params)

    threshold = 0.00001
    rounds = 0

    while sum(dp.values()) > threshold and rounds < 10000:
        rounds += 1
        for param_name in adj_params:
            orig = params[param_name]
            params[param_name] = orig + dp[param_name]
            err = error_func(params)
            if err < best_err:
                # There was some improvement
                best_err = err
                dp[param_name] *= 1.1
                continue
            params[param_name] = orig - dp[param_name]
            err = error_func(params)
            if err < best_err:
                # There was some improvement
                best_err = err
                dp[param_name] *= 1.1
                continue
            params[param_name] = orig
            dp[param_name] *= 0.9
    logging.info("Coordinate descent best_err: %s  rounds: %d",
                 best_err, rounds)
    return params


######################################################################
# FOPDT calculation
######################################################################

class GraphBumpTest:
    def __init__(self):
        self.pwm_samples = []
        self.temp_samples = []
        self.ambient_temp = 0.
    def model_fopdt(self, gain, time_constant, delay):
        heater_on_time = self.pwm_samples[0][0] + delay
        heater_off_time = self.pwm_samples[1][0] + delay
        gain *= self.pwm_samples[0][1]
        ambient_temp = self.ambient_temp
        inv_time_constant = 1. / time_constant
        heat_time = heater_off_time - heater_on_time
        peak_temp = gain * (1. - math.exp(-heat_time * inv_time_constant))
        out = []
        for time, measured_temp in self.temp_samples:
            rel_temp = 0.
            if time > heater_off_time:
                cool_time = time - heater_off_time
                rel_temp = peak_temp * math.exp(-cool_time * inv_time_constant)
            elif time > heater_on_time:
                heat_time = time - heater_on_time
                rel_temp = gain * (1. - math.exp(-heat_time
                                                 * inv_time_constant))
            out.append(ambient_temp + rel_temp)
        return out
    def model_smoothed_fopdt(self, gain, time_constant, delay):
        heater_on_time = self.pwm_samples[0][0]
        heater_off_time = self.pwm_samples[1][0]
        gain *= self.pwm_samples[0][1]
        ambient_temp = self.ambient_temp
        inv_time_constant = 1. / time_constant
        inv_delay = 1. / delay
        heat_time = heater_off_time - heater_on_time
        peak_temp = gain * (1. - math.exp(-heat_time * inv_time_constant))
        smooth_temp = last_time = 0.
        out = []
        for time, measured_temp in self.temp_samples:
            rel_temp = 0.
            if time > heater_off_time:
                cool_time = time - heater_off_time
                rel_temp = peak_temp * math.exp(-cool_time * inv_time_constant)
            elif time > heater_on_time:
                heat_time = time - heater_on_time
                rel_temp = gain * (1. - math.exp(-heat_time
                                                 * inv_time_constant))
            time_diff = time - last_time
            last_time = time
            smooth_factor = 1. - math.exp(-time_diff * inv_delay)
            smooth_temp += (rel_temp - smooth_temp) * smooth_factor
            out.append(ambient_temp + smooth_temp)
        return out
    def least_squares_error(self, params):
        gain = params['gain']
        time_constant = params['time_constant']
        delay = params['delay']
        if gain <= 0. or time_constant <= 0. or delay <= 0.:
            return 9.9e99
        model = self.model_smoothed_fopdt(gain, time_constant, delay)
        err = 0.
        for (time, measured_temp), model_temp in zip(self.temp_samples, model):
            err += (measured_temp-model_temp)**2
        return err
    def calc_fopdt(self):
        # Determine the ambient temperature
        heater_on_time, max_power = self.pwm_samples[0]
        pre_heat = [temp for time, temp in self.temp_samples
                    if time <= heater_on_time]
        self.ambient_temp = sum(pre_heat) / len(pre_heat)
        # Initial fopdt guesses
        params = {}
        maxtemp, maxtemptime = max([(temp, time)
                                    for time, temp in self.temp_samples])
        params['gain'] = maxtemp * 2.
        params['time_constant'] = self.temp_samples[-1][0] - maxtemptime
        params['delay'] = 10.
        # Fit smoothed fopdt model to measured temperatures
        new_params = coordinate_descent(
            ('gain', 'time_constant', 'delay'), params,
            self.least_squares_error)
        gain = new_params['gain']
        time_constant = new_params['time_constant']
        delay = new_params['delay']
        logging.info("calc_fopdt: ambient_temp=%.3f gain=%.3f"
                     " time_constant=%.3f delay_time=%.3f",
                     self.ambient_temp, gain, time_constant, delay)
        return gain, time_constant, delay
    def read_file(self, filename):
        f = open(filename, "rb")
        data = f.read()
        f.close()
        self.pwm_samples = []
        self.temp_samples = []
        for line in data.split('\n'):
            parts = line.split()
            if not parts:
                continue
            if parts[0] == 'pwm:':
                self.pwm_samples.append((float(parts[1]), float(parts[2])))
            else:
                self.temp_samples.append((float(parts[0]), float(parts[1])))


######################################################################
# Plotting
######################################################################

def plot_data(gbt):
    # Calculate data
    gain, time_constant, delay = gbt.calc_fopdt()
    fopdt_temps = gbt.model_fopdt(gain, time_constant, delay)
    smoothed_temps = gbt.model_smoothed_fopdt(gain, time_constant, delay)
    measured_temps = [i[1] for i in gbt.temp_samples]
    initial_time = gbt.temp_samples[0][0]
    times = [i[0] - initial_time for i in gbt.temp_samples]
    # Draw plot
    fig, ax1 = matplotlib.pyplot.subplots()
    ax1.set_title("FOPDT graph (gain=%.3f time_constant=%.3f delay=%.3f" % (
        gain, time_constant, delay))
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Temperature (c)')
    for time, pwm in gbt.pwm_samples:
        ax1.axvline(time - initial_time, color='y', alpha=0.8)
    #ax1.plot(times, gbt.model_fopdt(gain, time_constant, 0.), '-',
    #         color='purple', label='fo', alpha=0.3)
    ax1.plot(times, fopdt_temps, '-', color='blue', label='fopdt', alpha=0.3)
    ax1.plot(times, smoothed_temps, '-', color='red',
             label='smoothed fopdt', alpha=0.8)
    ax1.plot(times, measured_temps, '-', color='green',
             label='measured', alpha=0.8)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax1.legend(loc='best', prop=fontP)
    ax1.grid(True)
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
    usage = "%prog [options] <logfile>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--output", type="string", dest="output",
                    default=None, help="filename of output graph")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    logname = args[0]

    gbt = GraphBumpTest()
    gbt.read_file(logname)

    # Draw graph
    setup_matplotlib(options.output is not None)
    fig = plot_data(gbt)

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
