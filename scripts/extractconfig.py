#!/usr/bin/env python2
# Script to extract a config file from a log
#
# Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys

def format_comment(line_num, line):
    return "# %6d: %s" % (line_num, line)

def main():
    logname = sys.argv[1]
    f = open(logname, 'rb')
    last_git = last_start = ''
    config_count = config_line_num = 0
    configs = {}
    config_lines = []
    info = None
    # Parse log file
    for line_num, line in enumerate(f):
        line = line.strip()
        line_num += 1
        if config_line_num:
            if line == '=======================':
                lines = tuple(config_lines)
                info = configs.get(lines)
                if info is None:
                    config_count += 1
                    configs[lines] = info = (config_count, [])
                info[1].append(format_comment(config_line_num, "config file"))
                if last_git:
                    info[1].append(last_git)
                if last_start:
                    info[1].append(last_start)
                config_line_num = 0
                config_lines = []
            else:
                config_lines.append(line)
        elif line.startswith('Git version'):
            last_git = format_comment(line_num, line)
        elif line.startswith('Start printer at'):
            last_start = format_comment(line_num, line)
        elif line == '===== Config file =====':
            config_line_num = line_num
        elif info is not None and (
                ' shutdown: ' in line or line.startswith('Dumping ')):
            info[1].append(format_comment(line_num, line))
    # Write found config files
    for lines, (count, comments) in configs.items():
        f = open("%s.config%04d.cfg" % (logname, count), 'wb')
        f.write('\n'.join(tuple(comments) + lines))
        f.close()

if __name__ == '__main__':
    main()
