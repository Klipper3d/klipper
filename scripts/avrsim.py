#!/usr/bin/env python
# Script to interact with simulavr by simulating a serial port.
#
# Copyright (C) 2015  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import sys, optparse, os, pty, select, fcntl, termios, traceback, errno
import pysimulavr

SERIALBITS = 10 # 8N1 = 1 start, 8 data, 1 stop

# Class to read serial data from AVR serial transmit pin.
class SerialRxPin(pysimulavr.PySimulationMember, pysimulavr.Pin):
    def __init__(self, baud):
        pysimulavr.Pin.__init__(self)
        pysimulavr.PySimulationMember.__init__(self)
        self.sc = pysimulavr.SystemClock.Instance()
        self.delay = 10**9 / baud
        self.current = 0
        self.pos = -1
        self.queue = ""
    def SetInState(self, pin):
        pysimulavr.Pin.SetInState(self, pin)
        self.state = pin.outState
        if self.pos < 0 and pin.outState == pin.LOW:
            self.pos = 0
            self.sc.Add(self)
    def DoStep(self, trueHwStep):
        ishigh = self.state == self.HIGH
        self.current |= ishigh << self.pos
        self.pos += 1
        if self.pos == 1:
            return int(self.delay * 1.5)
        if self.pos >= SERIALBITS:
            self.queue += chr((self.current >> 1) & 0xff)
            self.pos = -1
            self.current = 0
            return -1
        return self.delay
    def popChars(self):
        d = self.queue
        self.queue = ""
        return d

# Class to send serial data to AVR serial receive pin.
class SerialTxPin(pysimulavr.PySimulationMember, pysimulavr.Pin):
    MAX_QUEUE = 64
    def __init__(self, baud):
        pysimulavr.Pin.__init__(self)
        pysimulavr.PySimulationMember.__init__(self)
        self.SetPin('H')
        self.sc = pysimulavr.SystemClock.Instance()
        self.delay = 10**9 / baud
        self.current = 0
        self.pos = 0
        self.queue = ""
    def DoStep(self, trueHwStep):
        if not self.pos:
            if not self.queue:
                return -1
            self.current = (ord(self.queue[0]) << 1) | 0x200
            self.queue = self.queue[1:]
        newstate = 'L'
        if self.current & (1 << self.pos):
            newstate = 'H'
        self.SetPin(newstate)
        self.pos += 1
        if self.pos >= SERIALBITS:
            self.pos = 0
        return self.delay
    def needChars(self):
        if len(self.queue) > self.MAX_QUEUE / 2:
            return 0
        return self.MAX_QUEUE - len(self.queue)
    def pushChars(self, c):
        queueEmpty = not self.queue
        self.queue += c
        if queueEmpty:
            self.sc.Add(self)

# Support for creating VCD trace files
class Tracing:
    def __init__(self, filename, signals):
        self.filename = filename
        self.signals = signals
        if not signals:
            self.dman = None
            return
        self.dman = pysimulavr.DumpManager.Instance()
        self.dman.SetSingleDeviceApp()
    def show_help(self):
        ostr = pysimulavr.ostringstream()
        self.dman.save(ostr)
        sys.stdout.write(ostr.str())
        sys.exit(1)
    def load_options(self):
        if self.dman is None:
            return
        if self.signals.strip() == '?':
            self.show_help()
        sigs = "\n".join(["+ " + s for s in self.signals.split(',')])
        self.dman.addDumpVCD(self.filename, sigs, "ns", False, False)
    def start(self):
        if self.dman is not None:
            self.dman.start()
    def finish(self):
        if self.dman is not None:
            self.dman.stopApplication()

# Support for creating a pseudo-tty for emulating a serial port
def create_pty(ptyname):
    mfd, sfd = pty.openpty()
    try:
        os.unlink(ptyname)
    except os.error:
        pass
    os.symlink(os.ttyname(sfd), ptyname)
    fcntl.fcntl(mfd, fcntl.F_SETFL
                , fcntl.fcntl(mfd, fcntl.F_GETFL) | os.O_NONBLOCK)
    old = termios.tcgetattr(mfd)
    old[3] = old[3] & ~termios.ECHO
    termios.tcsetattr(mfd, termios.TCSADRAIN, old)
    return mfd

def main():
    usage = "%prog [options] <program.elf>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-m", "--machine", type="string", dest="machine",
                    default="atmega644", help="type of AVR machine to simulate")
    opts.add_option("-s", "--speed", type="int", dest="speed", default=8000000,
                    help="machine speed")
    opts.add_option("-b", "--baud", type="int", dest="baud", default=38400,
                    help="baud rate of the emulated serial port")
    opts.add_option("-t", "--trace", type="string", dest="trace",
                    help="signals to trace (? for help)")
    opts.add_option("-p", "--port", type="string", dest="port",
                    default="/tmp/pseudoserial",
                    help="pseudo-tty device to create for serial port")
    deffile = os.path.splitext(os.path.basename(sys.argv[0]))[0] + ".vcd"
    opts.add_option("-f", "--tracefile", type="string", dest="tracefile",
                    default=deffile, help="filename to write signal trace to")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    elffile = args[0]
    proc = options.machine
    ptyname = options.port
    speed = options.speed
    baud = options.baud

    # launch simulator
    sc = pysimulavr.SystemClock.Instance()
    trace = Tracing(options.tracefile, options.trace)
    dev = pysimulavr.AvrFactory.instance().makeDevice(proc)
    dev.Load(elffile)
    dev.SetClockFreq(10**9 / speed)
    sc.Add(dev)
    trace.load_options()

    # Setup rx pin
    rxpin = SerialRxPin(baud)
    net = pysimulavr.Net()
    net.Add(rxpin)
    net.Add(dev.GetPin("D1"))

    # Setup tx pin
    txpin = SerialTxPin(baud)
    net2 = pysimulavr.Net()
    net2.Add(dev.GetPin("D0"))
    net2.Add(txpin)

    # Display start banner
    msg = "Starting AVR simulation: machine=%s speed=%d\n" % (proc, speed)
    msg += "Serial: port=%s baud=%d\n" % (ptyname, baud)
    if options.trace:
        msg += "Trace file: %s\n" % (options.tracefile,)
    sys.stdout.write(msg)
    sys.stdout.flush()

    # Create terminal device
    fd = create_pty(ptyname)

    # Run loop
    try:
        trace.start()
        while 1:
            starttime = sc.GetCurrentTime()
            r = sc.RunTimeRange(speed/1000)
            endtime = sc.GetCurrentTime()
            if starttime == endtime:
                break
            d = rxpin.popChars()
            if d:
                os.write(fd, d)
            txsize = txpin.needChars()
            if txsize:
                res = select.select([fd], [], [], 0)
                if res[0]:
                    try:
                        d = os.read(fd, txsize)
                    except os.error, e:
                        if e.errno in (errno.EAGAIN, errno.EWOULDBLOCK):
                            continue
                        break
                    txpin.pushChars(d)
        trace.finish()
    finally:
        os.unlink(ptyname)

if __name__ == '__main__':
    main()
