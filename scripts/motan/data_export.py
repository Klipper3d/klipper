#!/usr/bin/env python
# Script to export motion analysis data to CSV
#
# Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2026  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, csv, optparse, ast
import readlog, analyzers


######################################################################
# CSV Export
######################################################################

def export_csv(amanager, columns):
    for dataset in columns:
        amanager.setup_dataset(dataset)
    amanager.generate_datasets()
    datasets = amanager.get_datasets()
    times = amanager.get_dataset_times()

    header = ["Time (s)"]
    for dataset in columns:
        label = amanager.get_label(dataset)
        unit = label['units'].split('\n')[-1]
        if unit == "Unknown":
            header.append(label['label'])
        else:
            header.append("%s %s" % (label['label'], unit))

    rows = []
    for i, t in enumerate(times):
        row = [t]
        for dataset in columns:
            row.append(datasets[dataset][i])
        rows.append(row)

    return header, rows


######################################################################
# Startup
######################################################################

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
                    default=None, help="filename of output csv")
    opts.add_option("-s", "--skip", type="float", default=0.,
                    help="Set the start time to export")
    opts.add_option("-d", "--duration", type="float", default=5.,
                    help="Number of seconds to export")
    opts.add_option("--segment-time", type="float", default=0.000100,
                    help="Analysis segment time (default 0.000100 seconds)")
    opts.add_option("-c", "--columns",
                    help="Columns to export (python literal)")
    opts.add_option("-l", "--list-datasets", action="store_true",
                    help="List available datasets")
    options, args = opts.parse_args()
    if options.list_datasets:
        list_datasets()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    if options.columns is None:
        opts.error("Option --columns is required")
    log_prefix = args[0]

    columns = ast.literal_eval(options.columns)

    lmanager = readlog.LogManager(log_prefix)
    lmanager.setup_index()
    lmanager.seek_time(options.skip)
    amanager = analyzers.AnalyzerManager(lmanager, options.segment_time)
    amanager.set_duration(options.duration)

    header, rows = export_csv(amanager, columns)

    if options.output is None:
        writer = csv.writer(sys.stdout)
    else:
        outfile = open(options.output, "w")
        writer = csv.writer(outfile)

    writer.writerow(header)
    for row in rows:
        writer.writerow(row)

    if options.output is not None:
        outfile.close()

if __name__ == '__main__':
    main()
