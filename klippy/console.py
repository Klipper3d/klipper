#!/usr/bin/env python
# Script to implement a test console with firmware over serial port
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, os, re, logging

import reactor, serialhdl, pins, util, msgproto

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
        self.local_commands = { "PINS": self.set_pin_map, "SET": self.set_var }
        self.eval_globals = {}
    def connect(self, eventtime):
        self.ser.connect()
        self.mcu_freq = int(self.ser.msgparser.get_constant_float('CLOCK_FREQ'))
        mcu = self.ser.msgparser.get_constant('MCU')
        self.pins = pins.get_pin_map(mcu)
        self.reactor.unregister_timer(self.connect_timer)
        return self.reactor.NEVER
    def update_evals(self, eventtime):
        self.eval_globals['freq'] = self.mcu_freq
        self.eval_globals['clock'] = int(self.ser.get_clock(eventtime))
    def set_pin_map(self, parts):
        mcu = self.ser.msgparser.get_constant('MCU')
        self.pins = pins.get_pin_map(mcu, parts[1])
    def set_var(self, parts):
        val = parts[2]
        try:
            val = int(val)
        except ValueError:
            try:
                val = float(val)
            except ValueError:
                pass
        self.eval_globals[parts[1]] = val
    def translate(self, line, eventtime):
        evalparts = re_eval.split(line)
        if len(evalparts) > 1:
            self.update_evals(eventtime)
            try:
                for i in range(1, len(evalparts), 2):
                    evalparts[i] = str(eval(evalparts[i], self.eval_globals))
            except:
                print "Unable to evaluate: ", line
                return None
            line = ''.join(evalparts)
            print "Eval:", line
        if self.pins is not None:
            try:
                line = pins.update_command(
                    line, self.mcu_freq, self.pins).strip()
            except:
                print "Unable to map pin: ", line
                return None
        if line:
            parts = line.split()
            if parts[0] in self.local_commands:
                self.local_commands[parts[0]](parts)
                return None
        try:
            msg = self.ser.msgparser.create_command(line)
        except msgproto.error, e:
            print "Error:", e
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
