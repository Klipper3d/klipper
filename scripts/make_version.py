#!/usr/bin/env python2
# Get the version number for klippy
#
# Copyright (C) 2018  Lucas Fink <software@lfcode.ca>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from __future__ import print_function

import argparse
import os
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), '../klippy'))

import util


def main(argv):
    p = argparse.ArgumentParser()
    p.add_argument(
        'distroname',
        help='Name of distro this package is intended for'
    )
    args = p.parse_args()
    print(util.get_git_version(from_file=False)["version"],
          args.distroname.replace(' ', ''), sep='-')


if __name__ == '__main__':
    main(sys.argv[1:])
