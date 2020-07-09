#!/usr/bin/env python3
# This file may be distributed under the terms of the GNU GPLv3 license.
import mmap
import argparse
import sys

SRAM_A2_BASE    = 0x00050000
SRAM_A2_LIMIT   = 0x00054000
SRAM_A2_SIZE    = SRAM_A2_LIMIT - SRAM_A2_BASE

R_CPU_CFG_PAGE_BASE  = 0x01F01000
R_CPU_CFG_PAGE_LIMIT = 0x01F02000
R_CPU_CFG_SIZE       = R_CPU_CFG_PAGE_LIMIT - R_CPU_CFG_PAGE_BASE
R_CPU_CFG_OFFSET     = 0xC00

parser = argparse.ArgumentParser(description='Write AR100 binary file to SRAM of A64')
parser.add_argument('filename', nargs='?', help='binary file to write to memory')
parser.add_argument('--reset', action='store_true', help='reset the AR100 cpu and exit')
args = parser.parse_args()


#firmware_file = "/home/klipper/klipper/out/ar100.bin"

# TODO: write exception vectors

def assert_deassert_reset(ass):
    with open("/dev/mem", "w+b") as f:
        r_cpucfg = mmap.mmap(f.fileno(),
            length=R_CPU_CFG_SIZE,
            offset=R_CPU_CFG_PAGE_BASE)
        if ass:
            r_cpucfg[R_CPU_CFG_OFFSET] &= ~0x01
        else:
            r_cpucfg[R_CPU_CFG_OFFSET] |= 0x01
        r_cpucfg.close()

def write_file(filename):
    with open(filename, "r+b") as fw:
        data = fw.read()
        if len(data) > SRAM_A2_SIZE:
            print("File does not fit in memory")
            sys.exit(1)
        assert_deassert_reset(1)
        print("Writing file to SRAM A2")
        with open("/dev/mem", "w+b") as f:
            sram_a2 = mmap.mmap(f.fileno(), length=SRAM_A2_SIZE, offset=SRAM_A2_BASE)
            sram_a2[0:len(data)] = data
            sram_a2.close()
        assert_deassert_reset(0)


if args.reset:
    print("Resetting AR100")
    assert_deassert_reset(1)
    assert_deassert_reset(0)
    sys.exit(0)

if args.filename:
    write_file(args.filename)