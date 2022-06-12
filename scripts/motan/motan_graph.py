#!/usr/bin/env python
# Script to perform motion analysis and graphing
#
# Copyright (C) 2019-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, ast
import matplotlib
import readlog, analyzers
try:
    import urlparse
except:
    import urllib.parse as urlparse


######################################################################
# Graphing
######################################################################

def plot_motion(amanager, graphs, log_prefix):
    # Generate data
    for graph in graphs:
        for dataset, plot_params in graph:
            amanager.setup_dataset(dataset)
    amanager.generate_datasets()
    datasets = amanager.get_datasets()
    times = amanager.get_dataset_times()
    # Build plot
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    fig, rows = matplotlib.pyplot.subplots(nrows=len(graphs), sharex=True)
    if len(graphs) == 1:
        rows = [rows]
    rows[0].set_title("Motion Analysis (%s)" % (log_prefix,))
    for graph, graph_ax in zip(graphs, rows):
        graph_units = graph_twin_units = twin_ax = None
        for dataset, plot_params in graph:
            label = amanager.get_label(dataset)
            ax = graph_ax
            if graph_units is None:
                graph_units = label['units']
                ax.set_ylabel(graph_units)
            elif label['units'] != graph_units:
                if graph_twin_units is None:
                    ax = twin_ax = graph_ax.twinx()
                    graph_twin_units = label['units']
                    ax.set_ylabel(graph_twin_units)
                elif label['units'] == graph_twin_units:
                    ax = twin_ax
                else:
                    graph_units = "Unknown"
                    ax.set_ylabel(graph_units)
            pparams = {'label': label['label'], 'alpha': 0.8}
            pparams.update(plot_params)
            ax.plot(times, datasets[dataset], **pparams)
        if twin_ax is not None:
            li1, la1 = graph_ax.get_legend_handles_labels()
            li2, la2 = twin_ax.get_legend_handles_labels()
            twin_ax.legend(li1 + li2, la1 + la2, loc='best', prop=fontP)
        else:
            graph_ax.legend(loc='best', prop=fontP)
        graph_ax.grid(True)
    rows[-1].set_xlabel('Time (s)')
    return fig


######################################################################
# Startup
######################################################################

def setup_matplotlib(output_to_file):
    global matplotlib
    if output_to_file:
        matplotlib.use('Agg')
    import matplotlib.pyplot, matplotlib.dates, matplotlib.font_manager
    import matplotlib.ticker

def parse_graph_description(desc):
    if '?' not in desc:
        return (desc, {})
    dataset, params = desc.split('?', 1)
    params = {k: v for k, v in urlparse.parse_qsl(params)}
    for fkey in ['alpha']:
        if fkey in params:
            params[fkey] = float(params[fkey])
    return (dataset, params)

def list_datasets():
    datasets = readlog.list_datasets() + analyzers.list_datasets()
    out = ["\nAvailable datasets:\n"]
    for dataset, desc in datasets:
        out.append("%-24s: %s\n" % (dataset, desc))
    out.append("\n")
    sys.stdout.write("".join(out))
    sys.exit(0)

def main():
    # Parse command-line arguments
    usage = "%prog [options] <logname>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--output", type="string", dest="output",
                    default=None, help="filename of output graph")
    opts.add_option("-s", "--skip", type="float", default=0.,
                    help="Set the start time to graph")
    opts.add_option("-d", "--duration", type="float", default=5.,
                    help="Number of seconds to graph")
    opts.add_option("--segment-time", type="float", default=0.000100,
                    help="Analysis segment time (default 0.000100 seconds)")
    opts.add_option("-g", "--graph", help="Graph to generate (python literal)")
    opts.add_option("-l", "--list-datasets", action="store_true",
                    help="List available datasets")
    options, args = opts.parse_args()
    if options.list_datasets:
        list_datasets()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    log_prefix = args[0]

    # Open data files
    lmanager = readlog.LogManager(log_prefix)
    lmanager.setup_index()
    lmanager.seek_time(options.skip)
    amanager = analyzers.AnalyzerManager(lmanager, options.segment_time)
    amanager.set_duration(options.duration)

    # Default graphs to draw
    graph_descs = [
        ["trapq(toolhead,velocity)?color=green"],
        ["trapq(toolhead,accel)?color=green"],
        ["deviation(stepq(stepper_x),kin(stepper_x))?color=blue"],
    ]
    if options.graph is not None:
        graph_descs = ast.literal_eval(options.graph)
    graphs = [[parse_graph_description(g) for g in graph_row]
              for graph_row in graph_descs]

    # Draw graph
    setup_matplotlib(options.output is not None)
    fig = plot_motion(amanager, graphs, log_prefix)

    # Show graph
    if options.output is None:
        matplotlib.pyplot.show()
    else:
        fig.set_size_inches(8, 6)
        fig.savefig(options.output)

if __name__ == '__main__':
    main()
