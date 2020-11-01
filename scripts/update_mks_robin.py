#!/usr/bin/env python2
# Script to update firmware for MKS Robin bootloader
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import optparse

XOR_PATTERN = [
    0xA3, 0xBD, 0xAD, 0x0D, 0x41, 0x11, 0xBB, 0x8D, 0xDC, 0x80,
    0x2D, 0xD0, 0xD2, 0xC4, 0x9B, 0x1E, 0x26, 0xEB, 0xE3, 0x33,
    0x4A, 0x15, 0xE4, 0x0A, 0xB3, 0xB1, 0x3C, 0x93, 0xBB, 0xAF,
    0xF7, 0x3E
]

def main():
    # Parse command-line arguments
    usage = "%prog <input_file> <output_file>"
    opts = optparse.OptionParser(usage)
    options, args = opts.parse_args()
    if len(args) != 2:
        opts.error("Incorrect number of arguments")
    infilename, outfilename = args
    # Read input
    f = open(infilename, "rb")
    srcfirmware = f.read()
    f.close()
    # Update
    firmware = bytearray(srcfirmware)
    for pos in range(320, min(31040, len(firmware))):
        firmware[pos] ^= XOR_PATTERN[pos & 31]
    # Write output
    f = open(outfilename, "wb")
    f.write(firmware)
    f.close()

if __name__ == '__main__':
    main()
