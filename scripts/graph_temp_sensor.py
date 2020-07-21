#!/usr/bin/env python2
# Tool to graph temperature sensor ADC resolution
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, optparse
import matplotlib


######################################################################
# Dummy config / printer / etc. class emulation
######################################################################

class DummyConfig:
    def __init__(self, config_settings):
        self.config_settings = config_settings
        self.sensor_factories = {}
    # Emulate config class
    def getfloat(self, option, default, **kw):
        return self.config_settings.get(option, default)
    def get(self, option, default=None):
        return default
    def get_printer(self):
        return self
    def get_name(self):
        return "dummy"
    # Emulate printer class
    def load_object(self, config, name):
        return self
    def lookup_object(self, name):
        return self
    # Emulate heaters class
    def add_sensor_factory(self, name, factory):
        self.sensor_factories[name] = factory
    def do_create_sensor(self, sensor_type):
        return self.sensor_factories[sensor_type](self).adc_convert
    # Emulate query_adc class
    def register_adc(self, name, klass):
        pass
    # Emulate pins class
    def setup_pin(self, pin_type, pin_name):
        return self
    # Emulate mcu_adc class
    def setup_adc_callback(self, time, callback):
        pass


######################################################################
# Plotting
######################################################################

def plot_adc_resolution(config, sensors):
    # Temperature list
    all_temps = [float(i) for i in range(1, 351)]
    temps = all_temps[:-1]
    # Build plot
    fig, (ax1, ax2) = matplotlib.pyplot.subplots(nrows=2, sharex=True)
    pullup = config.getfloat('pullup_resistor', 0.)
    adc_voltage = config.getfloat('adc_voltage', 0.)
    ax1.set_title("Temperature Sensor (pullup=%.0f, adc_voltage=%.3f)"
                  % (pullup, adc_voltage))
    ax1.set_ylabel('ADC')
    ax2.set_ylabel('ADC change per 1C')
    for sensor in sensors:
        sc = config.do_create_sensor(sensor)
        adcs = [sc.calc_adc(t) for t in all_temps]
        ax1.plot(temps, adcs[:-1], label=sensor, alpha=0.6)
        adc_deltas = [abs(adcs[i+1] - adcs[i]) for i in range(len(temps))]
        ax2.plot(temps, adc_deltas, alpha=0.6)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax1.legend(loc='best', prop=fontP)
    ax2.set_xlabel('Temperature (C)')
    ax1.grid(True)
    ax2.grid(True)
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

def import_sensors(config):
    global extras
    # Load adc_temperature.py and thermistor.py modules
    kdir = os.path.join(os.path.dirname(__file__), '..', 'klippy')
    sys.path.append(kdir)
    import extras.adc_temperature, extras.thermistor
    extras.thermistor.load_config(config)
    extras.adc_temperature.load_config(config)

def main():
    # Parse command-line arguments
    usage = "%prog [options]"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--output", type="string", dest="output",
                    default=None, help="filename of output graph")
    opts.add_option("-p", "--pullup", type="float", dest="pullup",
                    default=4700., help="pullup resistor")
    opts.add_option("-v", "--voltage", type="float", dest="voltage",
                    default=5., help="pullup resistor")
    opts.add_option("-s", "--sensors", type="string", dest="sensors",
                    default="", help="list of sensors (comma separated)")
    options, args = opts.parse_args()
    if len(args) != 0:
        opts.error("Incorrect number of arguments")

    # Import sensors
    config_settings = {'pullup_resistor': options.pullup,
                       'adc_voltage': options.voltage}
    config = DummyConfig(config_settings)
    import_sensors(config)

    # Determine sensors to graph
    if options.sensors:
        sensors = [s.strip() for s in options.sensors.split(',')]
    else:
        sensors = sorted(config.sensor_factories.keys())

    # Draw graph
    setup_matplotlib(options.output is not None)
    fig = plot_adc_resolution(config, sensors)

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
