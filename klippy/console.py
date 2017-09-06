#!/usr/bin/env python2
# Script to implement a test console with firmware over serial port
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, os, re, logging

import reactor, serialhdl, pins, util, msgproto

help_txt = """
  This is a debugging console for the Klipper micro-controller.
  In addition to mcu commands, the following artificial commands are
  available:
    PINS  : Load pin name aliases (eg, "PINS arduino")
    DELAY : Send a command at a clock time (eg, "DELAY 9999 get_uptime")
    FLOOD : Send a command many times (eg, "FLOOD 22 .01 get_uptime")
    SUPPRESS : Suppress a response message (eg, "SUPPRESS stats")
    SET   : Create a local variable (eg, "SET myvar 123.4")
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
    def __init__(self, ser, reactor):
        self.ser = ser
        self.reactor = reactor
        self.fd = sys.stdin.fileno()
        util.set_nonblock(self.fd)
        self.mcu_freq = 0
        self.pins = None
        self.data = ""
        reactor.register_fd(self.fd, self.process_kbd)
        self.connect_timer = reactor.register_timer(self.connect, reactor.NOW)
        self.local_commands = {
            "PINS": self.command_PINS, "SET": self.command_SET,
            "DELAY": self.command_DELAY, "FLOOD": self.command_FLOOD,
            "SUPPRESS": self.command_SUPPRESS, "STATS": self.command_STATS,
            "LIST": self.command_LIST, "HELP": self.command_HELP,
        }
        self.eval_globals = {}
    def connect(self, eventtime):
        self.output(help_txt)
        self.output("="*20 + " attempting to connect " + "="*20)
        self.ser.connect()
        self.ser.handle_default = self.handle_default
        self.mcu_freq = self.ser.msgparser.get_constant_float('CLOCK_FREQ')
        mcu = self.ser.msgparser.get_constant('MCU')
        self.pins = pins.get_pin_map(mcu)
        self.reactor.unregister_timer(self.connect_timer)
        self.output("="*20 + "       connected       " + "="*20)
        return self.reactor.NEVER
    def output(self, msg):
        sys.stdout.write("%s\n" % (msg,))
        sys.stdout.flush()
    def handle_default(self, params):
        self.output(self.ser.msgparser.format_params(params))
    def handle_suppress(self, params):
        pass
    def update_evals(self, eventtime):
        self.eval_globals['freq'] = self.mcu_freq
        self.eval_globals['clock'] = self.ser.get_clock(eventtime)
    def command_PINS(self, parts):
        mcu = self.ser.msgparser.get_constant('MCU')
        self.pins = pins.get_pin_map(mcu, parts[1])
    def command_SET(self, parts):
        val = parts[2]
        try:
            val = float(val)
        except ValueError:
            pass
        self.eval_globals[parts[1]] = val
    def command_DELAY(self, parts):
        try:
            val = int(parts[1])
        except ValueError as e:
            self.output("Error: %s" % (str(e),))
            return
        try:
            msg = self.ser.msgparser.create_command(' '.join(parts[2:]))
        except msgproto.error as e:
            self.output("Error: %s" % (str(e),))
            return
        self.ser.send(msg, minclock=val)
    def command_FLOOD(self, parts):
        try:
            count = int(parts[1])
            delay = float(parts[2])
        except ValueError as e:
            self.output("Error: %s" % (str(e),))
            return
        try:
            msg = self.ser.msgparser.create_command(' '.join(parts[3:]))
        except msgproto.error as e:
            self.output("Error: %s" % (str(e),))
            return
        delay_clock = int(delay * self.mcu_freq)
        msg_clock = int(self.ser.get_clock(self.reactor.monotonic())
                        + self.mcu_freq * .200)
        for i in range(count):
            next_clock = msg_clock + delay_clock
            self.ser.send(msg, minclock=msg_clock, reqclock=next_clock)
            msg_clock = next_clock
    def command_SUPPRESS(self, parts):
        oid = None
        try:
            name = parts[1]
            if len(parts) > 2:
                oid = int(parts[2])
        except ValueError as e:
            self.output("Error: %s" % (str(e),))
            return
        self.ser.register_callback(self.handle_suppress, name, oid)
    def command_STATS(self, parts):
        self.output(self.ser.stats(self.reactor.monotonic()))
    def command_LIST(self, parts):
        self.update_evals(self.reactor.monotonic())
        mp = self.ser.msgparser
        out = "Available mcu commands:"
        out += "\n  ".join([""] + sorted([
            mp.messages_by_id[i].msgformat for i in mp.command_ids]))
        out += "\nAvailable artificial commands:"
        out += "\n  ".join([""] + [n for n in sorted(self.local_commands)])
        out += "\nAvailable local variables:"
        out += "\n  ".join([""] + ["%s: %s" % (k, v)
                                   for k, v in sorted(self.eval_globals.items())])
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
        if self.pins is not None:
            try:
                line = pins.update_command(
                    line, self.mcu_freq, self.pins).strip()
            except:
                self.output("Unable to map pin: %s" % (line,))
                return None
        if line:
            parts = line.split()
            if parts[0] in self.local_commands:
                self.local_commands[parts[0]](parts)
                return None
        try:
            msg = self.ser.msgparser.create_command(line)
        except msgproto.error as e:
            self.output("Error: %s" % (str(e),))
            return None
        return msg
    def process_kbd(self, eventtime):
        self.data += os.read(self.fd, 4096)

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
            self.ser.send(msg)
        self.data = kbdlines[-1]

def main():
    usage = "%prog [options] <serialdevice> <baud>"
    opts = optparse.OptionParser(usage)
    options, args = opts.parse_args()
    serialport, baud = args
    baud = int(baud)

    logging.basicConfig(level=logging.DEBUG)
    r = reactor.Reactor()
    ser = serialhdl.SerialReader(r, serialport, baud)
    kbd = KeyboardReader(ser, r)
    try:
        r.run()
    except KeyboardInterrupt:
        sys.stdout.write("\n")

if __name__ == '__main__':
    main()
