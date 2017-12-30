#!/usr/bin/env python2
# Script to parse a logging file, extract the stats, and graph them
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse, datetime
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot, matplotlib.dates

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
        keyparts = dict(p.split('=', 1)
                        for p in parts[2:] if not p.endswith(':'))
        if keyparts.get('bytes_write', '0') == '0':
            continue
        keyparts['#sampletime'] = float(parts[1][:-1])
        out.append(keyparts)
    f.close()
    return out

def find_print_restarts(data):
    last_print_time = 0.
    print_resets = []
    for d in data:
        print_time = float(d.get('print_time', last_print_time))
        buffer_time = float(d.get('buffer_time', 0.))
        if print_time == last_print_time and not buffer_time:
            print_resets.append(d['#sampletime'])
            last_print_time = 0.
        elif buffer_time:
            last_print_time = print_time
    sample_resets = {}
    for d in data:
        st = d['#sampletime']
        while print_resets and st > print_resets[0]:
            print_resets.pop(0)
        if not print_resets:
            break
        if st + 2. * MAXBUFFER > print_resets[0]:
            sample_resets[st] = 1
    return sample_resets

def plot_mcu(data, maxbw, outname, graph_awake=False):
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
        if not hb or hb >= MAXBUFFER or st in sample_resets:
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
    if graph_awake:
        ax1.plot_date(times, awake, 'b', label='Awake time')
    ax1.plot_date(times, bwdeltas, 'g', label='Bandwidth')
    ax1.plot_date(times, loads, 'r', label='MCU load')
    ax1.plot_date(times, hostbuffers, 'c', label='Host buffer')
    ax1.legend(loc='best')
    ax1.xaxis.set_major_formatter(matplotlib.dates.DateFormatter('%H:%M'))
    ax1.grid(True)
    fig.savefig(outname)

def main():
    usage = "%prog [options] <logfile> <outname>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-a", "--awake", action="store_true"
                    , help="graph mcu awake time")
    options, args = opts.parse_args()
    if len(args) != 2:
        opts.error("Incorrect number of arguments")
    logname, outname = args
    data = parse_log(logname)
    if not data:
        return
    plot_mcu(data, MAXBANDWIDTH, outname, graph_awake=options.awake)

if __name__ == '__main__':
    main()
