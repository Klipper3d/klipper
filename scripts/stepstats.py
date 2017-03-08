#!/usr/bin/env python2
# Script to calculate stats for each stepper from a log of messages
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse

def main():
    usage = "%prog [options] <comms file>"
    opts = optparse.OptionParser(usage)
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    filename = args[0]

    steppers = {}
    f = open(filename, 'rb')
    for line in f:
        parts = line.split()
        if not parts:
            continue
        args = dict([p.split('=', 1) for p in parts[1:]])
        if parts[0] == 'config_stepper':
            # steppers[oid] = [dir_cmds, dir, queue_cmds, pos steps, neg steps]
            steppers[args['oid']] = [0, 0, 0, 0, 0]
        elif parts[0] == 'set_next_step_dir':
            so = steppers[args['oid']]
            so[0] += 1
            so[1] = args['dir']
        elif parts[0] == 'queue_step':
            so = steppers[args['oid']]
            so[2] += 1
            so[{'0': 3, '1': 4}[so[1]]] += int(args['count'])
    for oid, so in sorted([(int(i[0]), i[1]) for i in steppers.items()]):
        print "oid:%3d dir_cmds:%6d queue_cmds:%7d (%8d -%8d = %8d)" % (
            oid, so[0], so[2], so[4], so[3], so[4]-so[3])

if __name__ == '__main__':
    main()
