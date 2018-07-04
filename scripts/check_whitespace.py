#!/usr/bin/env python2
# Check files for whitespace problems
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os.path

HaveError = False

def report_error(filename, lineno, msg):
    global HaveError
    HaveError = True
    sys.stderr.write("Whitespace error in file %s on line %d: %s\n" % (
        filename, lineno + 1, msg))

def check_file(filename):
    # Open and read file
    try:
        f = open(filename, 'rb')
        data = f.read()
        f.close()
    except IOError:
        return
    if not data:
        # Empty files are okay
        return
    # Do checks
    lineno = 0
    for lineno, line in enumerate(data.split('\n')):
        # Check for control characters
        for c in line:
            oc = ord(c)
            if oc < 32:
                char_name = repr(c)
                if oc == 9:
                    if os.path.basename(filename).lower() == 'makefile':
                        continue
                    char_name = 'tab'
                report_error(filename, lineno, "Invalid %s character" % (
                    char_name,))
                break
        # Check for trailing space
        if line.endswith(' '):
            report_error(filename, lineno, "Trailing space")
    if not data.endswith('\n'):
        report_error(filename, lineno, "No newline at end of file")
    if data.endswith('\n\n'):
        report_error(filename, lineno, "Extra newlines at end of file")

def main():
    files = sys.argv[1:]
    for filename in files:
        check_file(filename)
    if HaveError:
        sys.exit(-1)

if __name__ == '__main__':
    main()
