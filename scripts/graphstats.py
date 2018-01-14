#!/usr/bin/env python2
# Script to parse a logging file, extract the stats, and graph them
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse, datetime
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot, matplotlib.dates, matplotlib.font_manager
import matplotlib.ticker

MAXBANDWIDTH=25000.
MAXBUFFER=2.
STATS_INTERVAL=5.
TASK_MAX=0.0025

def parse_log(logname):
    f = open(logname, 'rb')
    out = []
    for line in f:
        parts = line.split()
        if not parts or parts[0] not in ('Stats', 'INFO:root:Stats'):
            #if parts and parts[0] == 'INFO:root:shutdown:':
            #    break
            continue
        prefix = ""
        keyparts = {}
        for p in parts[2:]:
            if p.endswith(':'):
                prefix = p
                if prefix == 'mcu:':
                    prefix = ''
                continue
            name, val = p.split('=', 1)
            keyparts[prefix + name] = val
        if keyparts.get('bytes_write', '0') == '0':
            continue
        keyparts['#sampletime'] = float(parts[1][:-1])
        out.append(keyparts)
    f.close()
    return out

def find_print_restarts(data):
    runoff_samples = {}
    last_runoff_start = last_buffer_time = last_sampletime = 0.
    last_print_stall = 0
    for d in reversed(data):
        # Check for buffer runoff
        sampletime = d['#sampletime']
        buffer_time = float(d.get('buffer_time', 0.))
        if buffer_time < 1. or (buffer_time < MAXBUFFER
                                and buffer_time > last_buffer_time):
            if not last_runoff_start:
                last_runoff_start = last_sampletime
                runoff_samples[last_runoff_start] = [False, []]
            runoff_samples[last_runoff_start][1].append(sampletime)
        else:
            last_runoff_start = 0.
        last_buffer_time = buffer_time
        last_sampletime = sampletime
        # Check for print stall
        print_stall = int(d['print_stall'])
        if print_stall < last_print_stall:
            if last_runoff_start:
                runoff_samples[last_runoff_start][0] = True
        last_print_stall = print_stall
    sample_resets = {sampletime: 1 for stall, samples in runoff_samples.values()
                     for sampletime in samples if not stall}
    return sample_resets

def plot_mcu(data, maxbw, outname):
    # Generate data for plot
    basetime = lasttime = data[0]['#sampletime']
    lastbw = float(data[0]['bytes_write']) + float(data[0]['bytes_retransmit'])
    sample_resets = find_print_restarts(data)
    times = []
    bwdeltas = []
    loads = []
    awake = []
    hostbuffers = []
    for d in data:
        st = d['#sampletime']
        timedelta = st - lasttime
        if timedelta <= 0.:
            continue
        bw = float(d['bytes_write']) + float(d['bytes_retransmit'])
        if bw < lastbw:
            lastbw = bw
            continue
        load = float(d['mcu_task_avg']) + 3*float(d['mcu_task_stddev'])
        if st - basetime < 15.:
            load = 0.
        pt = float(d['print_time'])
        hb = float(d['buffer_time'])
        if hb >= MAXBUFFER or st in sample_resets:
            hb = 0.
        else:
            hb = 100. * (MAXBUFFER - hb) / MAXBUFFER
        hostbuffers.append(hb)
        times.append(datetime.datetime.utcfromtimestamp(st))
        bwdeltas.append(100. * (bw - lastbw) / (maxbw * timedelta))
        loads.append(100. * load / TASK_MAX)
        awake.append(100. * float(d.get('mcu_awake', 0.)) / STATS_INTERVAL)
        lasttime = st
        lastbw = bw

    # Build plot
    fig, ax1 = matplotlib.pyplot.subplots()
    ax1.set_title("MCU bandwidth and load utilization")
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Usage (%)')
    ax1.plot_date(times, bwdeltas, 'g', label='Bandwidth', alpha=0.8)
    ax1.plot_date(times, loads, 'r', label='MCU load', alpha=0.8)
    ax1.plot_date(times, hostbuffers, 'c', label='Host buffer', alpha=0.8)
    ax1.plot_date(times, awake, 'y', label='Awake time', alpha=0.6)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax1.legend(loc='best', prop=fontP)
    ax1.xaxis.set_major_formatter(matplotlib.dates.DateFormatter('%H:%M'))
    ax1.grid(True)
    fig.set_size_inches(8, 6)
    fig.savefig(outname)

def plot_frequency(data, outname):
    all_keys = {}
    for d in data:
        all_keys.update(d)
    graph_keys = {}
    for key in all_keys:
        if ((key in ("freq", "adj")
             or key.endswith(":freq") or key.endswith(":adj"))
            and key not in graph_keys):
            graph_keys[key] = ([], [])
    basetime = lasttime = data[0]['#sampletime']
    for d in data:
        st = datetime.datetime.utcfromtimestamp(d['#sampletime'])
        for key, (times, values) in graph_keys.items():
            val = d.get(key)
            if val not in (None, '0', '1'):
                times.append(st)
                values.append(float(val))

    # Build plot
    fig, ax1 = matplotlib.pyplot.subplots()
    ax1.set_title("MCU frequency")
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Frequency')
    for key in sorted(graph_keys):
        times, values = graph_keys[key]
        ax1.plot_date(times, values, '.', label=key)
    fontP = matplotlib.font_manager.FontProperties()
    fontP.set_size('x-small')
    ax1.legend(loc='best', prop=fontP)
    ax1.xaxis.set_major_formatter(matplotlib.dates.DateFormatter('%H:%M'))
    ax1.yaxis.set_major_formatter(matplotlib.ticker.FormatStrFormatter('%d'))
    ax1.grid(True)
    fig.savefig(outname)

def main():
    usage = "%prog [options] <logfile> <outname>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-f", "--frequency", action="store_true",
                    help="graph mcu frequency")
    options, args = opts.parse_args()
    if len(args) != 2:
        opts.error("Incorrect number of arguments")
    logname, outname = args
    data = parse_log(logname)
    if not data:
        return
    if options.frequency:
        plot_frequency(data, outname)
        return
    plot_mcu(data, MAXBANDWIDTH, outname)

if __name__ == '__main__':
    main()
