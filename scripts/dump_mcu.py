#!/usr/bin/env python
# MCU flash dump assistant
#
# Copyright (C) 2022 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import sys
import argparse
import os
import traceback
import logging
KLIPPER_DIR = os.path.abspath(os.path.join(
    os.path.dirname(__file__), "../"))
sys.path.append(os.path.join(KLIPPER_DIR, "klippy"))
import reactor
import serialhdl
import clocksync

###########################################################
#
# Helper methods
#
###########################################################

def output_line(msg):
    sys.stdout.write("%s\n" % (msg,))
    sys.stdout.flush()

def output(msg):
    sys.stdout.write("%s" % (msg,))
    sys.stdout.flush()

DUMP_CMD="debug_read order=%d addr=%d"
DUMP_RESP="debug_result"

class MCUDumpError(Exception):
    pass

class MCUDump:
    def __init__(self, args):
        self.reactor = reactor.Reactor()
        self.device = args.device
        self.baud = args.baud
        self.canbus_iface = args.canbus_iface
        self.canbus_nodeid = args.canbus_nodeid
        self.output_file = os.path.expanduser(args.outfile)
        try:
            self.read_start = int(args.read_start, 0)
            self.read_length = int(args.read_length, 0)
        except ValueError as e:
            raise MCUDumpError(
                "Error converting flash address: %s " % (str(e),)
            )
        if self.read_length <= 0:
            raise MCUDumpError("Read count must be greater than 0")
        self._serial = serialhdl.SerialReader(self.reactor)
        self.clocksync = clocksync.ClockSync(self.reactor)
        self.connect_completion = None
        self.connected = False

    def connect(self):
        output("Connecting to MCU..")
        self.connect_completion = self.reactor.completion()
        self.connected = False
        self.reactor.register_callback(self._do_serial_connect)
        curtime = self.reactor.monotonic()
        while True:
            curtime = self.reactor.pause(curtime + 1.)
            output(".")
            if self.connect_completion.test():
                self.connected = self.connect_completion.wait()
                break
        self.connect_completion = None
        if not self.connected:
            output("\n")
            raise MCUDumpError("Unable to connect to MCU")
        output_line("Connected")
        msgparser = self._serial.get_msgparser()
        mcu_id = msgparser.get_constant("MCU")
        freq = msgparser.get_constant("CLOCK_FREQ")
        output_line("MCU type: %s" % (mcu_id,))
        output_line("Frequency: %s\n" % (freq,))

    def _do_serial_connect(self, eventtime):
        endtime = eventtime + 60.
        while True:
            try:
                if self.canbus_iface is not None:
                    self._serial.connect_canbus(
                        self.device, self.canbus_nodeid, self.canbus_iface
                    )
                elif (
                    self.device.startswith("/dev/rpmsg_") or
                    self.device.startswith("/tmp/")
                ):
                    self._serial.connect_pipe(self.device)
                else:
                    self._serial.connect_uart(self.device, self.baud)
                self.clocksync.connect(self._serial)
            except Exception:
                curtime = self.reactor.monotonic()
                if curtime > endtime:
                    self.connect_completion.complete(False)
                    return
                output("Connection Error, retrying..")
                self._serial.disconnect()
                self.reactor.pause(curtime + 2.)
            else:
                break
        self.connect_completion.complete(True)

    def disconnect(self):
        if not self.connected:
            return
        self._serial.disconnect()
        self.connected = False

    def _dump_flash(self):
        addr = self.read_start
        count = self.read_length
        order = [2, 0, 1, 0][(addr | count) & 3]
        bsize = 1 << order
        # Query data from mcu
        output_line(
            "Reading %d bytes from flash, start address 0x%x\n"
            % (count, addr)
        )
        output("[")
        bytes_read = last_reported_pct = 0
        vals = []
        for i in range((count + bsize - 1) >> order):
            caddr = addr + (i << order)
            cmd = DUMP_CMD % (order, caddr)
            params = self._serial.send_with_response(cmd, DUMP_RESP)
            vals.append(params['val'])
            bytes_read += bsize
            pct = int(bytes_read / float(count) * 100 + .5)
            diff = (pct - last_reported_pct) // 2
            if diff:
                last_reported_pct = pct
                output("#" * diff)
        output_line("]\n")
        # Convert to byte format
        data = bytearray()
        for val in vals:
            for b in range(bsize):
                data.append((val >> (8 * b)) & 0xff)
        data = data[:count]
        with open(self.output_file, "wb") as f:
            f.write(data)
        output_line("Wrote %d bytes to '%s'" % (len(data), self.output_file))

    def _run_dump_task(self, eventtime):
        self.connect()
        self._dump_flash()
        self.reactor.end()

    def run(self):
        self.reactor.register_callback(self._run_dump_task)
        try:
            self.reactor.run()
        finally:
            self.disconnect()
            self.reactor.finalize()

def main():
    parser = argparse.ArgumentParser(description="MCU Flash Dump Utility")

    parser.add_argument(
        "-b", "--baud", metavar="<baud rate>", type=int,
        default=250000, help="Baud Rate")
    parser.add_argument(
        "-c", "--canbus_iface", metavar="<canbus iface>", default=None,
         help="Use CAN bus interface; <device> is the chip UUID")
    parser.add_argument(
        "-i", "--canbus_nodeid", metavar="<canbus nodeid>", type=int,
        default=64, help="The CAN nodeid to use (default 64)")
    parser.add_argument(
        "-s", "--read_start", metavar="<read start>", default="0x0",
        help="Flash address to start reading")
    parser.add_argument(
        "-l", "--read_length", metavar="<read length>", default="0x400",
        help="Number of bytes to read")
    parser.add_argument(
        "device", metavar="<device>", help="Device Serial Port")
    parser.add_argument(
        "outfile", metavar="<outfile>",
        help="Path to output file")
    args = parser.parse_args()
    logging.basicConfig(level=logging.CRITICAL)
    try:
        mcudump = MCUDump(args)
        mcudump.run()
    except Exception as e:
        output_line("\nMCU Dump Error: %s" % (str(e),))
        traceback.print_exc(file=sys.stdout)
        sys.exit(-1)
    output_line("MCU Dump Complete")


if __name__ == "__main__":
    main()
