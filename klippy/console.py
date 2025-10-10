#!/usr/bin/env python2
# Script to implement a test console with firmware over serial port
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, os, re, logging
import util, reactor, serialhdl, msgproto, clocksync

help_txt = """
  This is a debugging console for the Klipper micro-controller.
  In addition to mcu commands, the following artificial commands are
  available:
    DELAY : Send a command at a clock time (eg, "DELAY 9999 get_uptime")
    FLOOD : Send a command many times (eg, "FLOOD 22 .01 get_uptime")
    SUPPRESS : Suppress a response message (eg, "SUPPRESS analog_in_state 4")
    SET   : Create a local variable (eg, "SET myvar 123.4")
    DUMP  : Dump memory (eg, "DUMP 0x12345678 100 32")
    FILEDUMP : Dump to file (eg, "FILEDUMP data.bin 0x12345678 100 32")
    STATS : Report serial statistics
    LIST  : List available mcu commands, local commands, and local variables
    HELP  : Show this text
  All commands also support evaluation by enclosing an expression in { }.
  For example, "reset_step_clock oid=4 clock={clock + freq}".  In addition
  to user defined variables (via the SET command) the following builtin
  variables may be used in expressions:
    clock : The current mcu clock time (as estimated by the host)
    freq  : The mcu clock frequency
"""

re_eval = re.compile(r'\{(?P<eval>[^}]*)\}')

class KeyboardReader:
    def __init__(self, reactor, serialport, baud, canbus_iface, canbus_nodeid):
        self.serialport = serialport
        self.baud = baud
        self.canbus_iface = canbus_iface
        self.canbus_nodeid = canbus_nodeid
        self.ser = serialhdl.SerialReader(reactor)
        self.reactor = reactor
        self.start_time = reactor.monotonic()
        self.clocksync = clocksync.ClockSync(self.reactor)
        self.fd = sys.stdin.fileno()
        util.set_nonblock(self.fd)
        self.mcu_freq = 0
        self.data = ""
        reactor.register_fd(self.fd, self.process_kbd)
        reactor.register_callback(self.connect)
        self.local_commands = {
            "SET": self.command_SET,
            "DUMP": self.command_DUMP, "FILEDUMP": self.command_FILEDUMP,
            "DELAY": self.command_DELAY, "FLOOD": self.command_FLOOD,
            "SUPPRESS": self.command_SUPPRESS, "STATS": self.command_STATS,
            "LIST": self.command_LIST, "HELP": self.command_HELP,
        }
        self.eval_globals = {}
    def connect(self, eventtime):
        self.output(help_txt)
        self.output("="*20 + " attempting to connect " + "="*20)
        if self.canbus_iface is not None:
            self.ser.connect_canbus(self.serialport, self.canbus_nodeid,
                                    self.canbus_iface)
        elif self.baud:
            self.ser.connect_uart(self.serialport, self.baud)
        else:
            self.ser.connect_pipe(self.serialport)
        msgparser = self.ser.get_msgparser()
        message_count = len(msgparser.get_messages())
        version, build_versions = msgparser.get_version_info()
        self.output("Loaded %d commands (%s / %s)"
                    % (message_count, version, build_versions))
        self.output("MCU config: %s" % (" ".join(
            ["%s=%s" % (k, v) for k, v in msgparser.get_constants().items()])))
        self.clocksync.connect(self.ser)
        self.ser.handle_default = self.handle_default
        self.ser.register_response(self.handle_output, '#output')
        self.mcu_freq = msgparser.get_constant_float('CLOCK_FREQ')
        self.output("="*20 + "       connected       " + "="*20)
        return self.reactor.NEVER
    def output(self, msg):
        sys.stdout.write("%s\n" % (msg,))
        sys.stdout.flush()
    def handle_default(self, params):
        tdiff = params['#receive_time'] - self.start_time
        msg = self.ser.get_msgparser().format_params(params)
        self.output("%07.3f: %s" % (tdiff, msg))
    def handle_output(self, params):
        tdiff = params['#receive_time'] - self.start_time
        self.output("%07.3f: %s: %s" % (tdiff, params['#name'], params['#msg']))
    def handle_suppress(self, params):
        pass
    def update_evals(self, eventtime):
        self.eval_globals['freq'] = self.mcu_freq
        self.eval_globals['clock'] = self.clocksync.get_clock(eventtime)
    def command_SET(self, parts):
        val = parts[2]
        try:
            val = float(val)
        except ValueError:
            pass
        self.eval_globals[parts[1]] = val
    def command_DUMP(self, parts, filename=None):
        # Extract command args
        try:
            addr = int(parts[1], 0)
            count = int(parts[2], 0)
            order = [2, 0, 1, 0][(addr | count) & 3]
            if len(parts) > 3:
                order = {'32': 2, '16': 1, '8': 0}[parts[3]]
        except ValueError as e:
            self.output("Error: %s" % (str(e),))
            return
        bsize = 1 << order
        # Query data from mcu
        vals = []
        for i in range((count + bsize - 1) >> order):
            caddr = addr + (i << order)
            cmd = "debug_read order=%d addr=%d" % (order, caddr)
            params = self.ser.send_with_response(cmd, "debug_result")
            vals.append(params['val'])
        # Report data
        if filename is None and order == 2:
            # Common 32bit hex dump
            for i in range((len(vals) + 3) // 4):
                p = i * 4
                hexvals = " ".join(["%08x" % (v,) for v in vals[p:p+4]])
                self.output("%08x  %s" % (addr + p * 4, hexvals))
            return
        # Convert to byte format
        data = bytearray()
        for val in vals:
            for b in range(bsize):
                data.append((val >> (8 * b)) & 0xff)
        data = data[:count]
        if filename is not None:
            f = open(filename, 'wb')
            f.write(data)
            f.close()
            self.output("Wrote %d bytes to '%s'" % (len(data), filename))
            return
        for i in range((count + 15) // 16):
            p = i * 16
            paddr = addr + p
            d = data[p:p+16]
            hexbytes = " ".join(["%02x" % (v,) for v in d])
            pb = "".join([chr(v) if v >= 0x20 and v < 0x7f else '.' for v in d])
            o = "%08x  %-47s  |%s|" % (paddr, hexbytes, pb)
            self.output("%s %s" % (o[:34], o[34:]))
    def command_FILEDUMP(self, parts):
        self.command_DUMP(parts[1:], filename=parts[1])
    def command_DELAY(self, parts):
        try:
            val = int(parts[1])
        except ValueError as e:
            self.output("Error: %s" % (str(e),))
            return
        try:
            self.ser.send(' '.join(parts[2:]), minclock=val)
        except msgproto.error as e:
            self.output("Error: %s" % (str(e),))
            return
    def command_FLOOD(self, parts):
        try:
            count = int(parts[1])
            delay = float(parts[2])
        except ValueError as e:
            self.output("Error: %s" % (str(e),))
            return
        msg = ' '.join(parts[3:])
        delay_clock = int(delay * self.mcu_freq)
        msg_clock = int(self.clocksync.get_clock(self.reactor.monotonic())
                        + self.mcu_freq * .200)
        try:
            for i in range(count):
                next_clock = msg_clock + delay_clock
                self.ser.send(msg, minclock=msg_clock, reqclock=next_clock)
                msg_clock = next_clock
        except msgproto.error as e:
            self.output("Error: %s" % (str(e),))
            return
    def command_SUPPRESS(self, parts):
        oid = None
        try:
            name = parts[1]
            if len(parts) > 2:
                oid = int(parts[2])
        except ValueError as e:
            self.output("Error: %s" % (str(e),))
            return
        self.ser.register_response(self.handle_suppress, name, oid)
    def command_STATS(self, parts):
        curtime = self.reactor.monotonic()
        self.output(' '.join([self.ser.stats(curtime),
                              self.clocksync.stats(curtime)]))
    def command_LIST(self, parts):
        self.update_evals(self.reactor.monotonic())
        mp = self.ser.get_msgparser()
        cmds = [msgformat for msgtag, msgtype, msgformat in mp.get_messages()
                if msgtype == 'command']
        out = "Available mcu commands:"
        out += "\n  ".join([""] + sorted(cmds))
        out += "\nAvailable artificial commands:"
        out += "\n  ".join([""] + [n for n in sorted(self.local_commands)])
        out += "\nAvailable local variables:"
        lvars = sorted(self.eval_globals.items())
        out += "\n  ".join([""] + ["%s: %s" % (k, v) for k, v in lvars])
        self.output(out)
    def command_HELP(self, parts):
        self.output(help_txt)
    def translate(self, line, eventtime):
        evalparts = re_eval.split(line)
        if len(evalparts) > 1:
            self.update_evals(eventtime)
            try:
                for i in range(1, len(evalparts), 2):
                    e = eval(evalparts[i], dict(self.eval_globals))
                    if type(e) == type(0.):
                        e = int(e)
                    evalparts[i] = str(e)
            except:
                self.output("Unable to evaluate: %s" % (line,))
                return None
            line = ''.join(evalparts)
            self.output("Eval: %s" % (line,))
        line = line.strip()
        if line:
            parts = line.split()
            if parts[0] in self.local_commands:
                self.local_commands[parts[0]](parts)
                return None
        return line
    def process_kbd(self, eventtime):
        self.data += str(os.read(self.fd, 4096).decode())

        kbdlines = self.data.split('\n')
        for line in kbdlines[:-1]:
            line = line.strip()
            cpos = line.find('#')
            if cpos >= 0:
                line = line[:cpos]
                if not line:
                    continue
            msg = self.translate(line.strip(), eventtime)
            if msg is None:
                continue
            try:
                self.ser.send(msg)
            except msgproto.error as e:
                self.output("Error: %s" % (str(e),))
        self.data = kbdlines[-1]

def main():
    usage = "%prog [options] <serialdevice>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-v", action="store_true", dest="verbose",
                    help="enable debug messages")
    opts.add_option("-b", "--baud", type="int", dest="baud", help="baud rate")
    opts.add_option("-c", "--canbus_iface", dest="canbus_iface",
                    help="Use CAN bus interface; serialdevice is the chip UUID")
    opts.add_option("-i", "--canbus_nodeid", type="int", dest="canbus_nodeid",
                    default=64, help="The CAN nodeid to use (default 64)")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    serialport = args[0]

    baud = options.baud
    if baud is None and not (serialport.startswith("/dev/rpmsg_")
                             or serialport.startswith("/tmp/")):
        baud = 250000

    debuglevel = logging.INFO
    if options.verbose:
        debuglevel = logging.DEBUG
    logging.basicConfig(level=debuglevel)

    r = reactor.Reactor()
    kbd = KeyboardReader(r, serialport, baud, options.canbus_iface,
                         options.canbus_nodeid)
    try:
        r.run()
    except KeyboardInterrupt:
        sys.stdout.write("\n")

if __name__ == '__main__':
    main()
