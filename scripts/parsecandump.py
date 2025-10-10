#!/usr/bin/python3
# Check for out of order timestamps in the output of candump
#
# Copyright (C) 2023  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, optparse

def import_msgproto():
    global msgproto
    # Load msgproto.py module
    kdir = os.path.join(os.path.dirname(__file__), '..', 'klippy')
    sys.path.append(kdir)
    import msgproto

def read_dictionary(filename):
    dfile = open(filename, 'rb')
    dictionary = dfile.read()
    dfile.close()
    return dictionary

def report(msg, line_info, name="", is_err=False):
    line_number, line_time = line_info
    warn = ""
    if is_err:
        warn = " WARN"
    sys.stdout.write("%04d:%010.6f:%s%s %s\n"
                     % (line_number, line_time, name, warn, msg))

class canscan:
    def __init__(self, name, mp):
        self.name = name
        self.mp = mp
        self.data = bytearray()
        self.need_scan = False
    def handle_data(self, line_info, line, newdata):
        data = self.data
        data += bytearray(newdata)
        while 1:
            if self.need_scan:
                drop = len(data)
                syncpos = data.find(msgproto.MESSAGE_SYNC)
                if syncpos >= 0:
                    drop = syncpos + 1
                    self.need_scan = False
                disc = ["%02X" % (d,) for d in data[:drop]]
                report("Discarding %d (%s)" % (drop, " ".join(disc)),
                       line_info, self.name, is_err=True)
                data[:drop] = []
                if not data:
                    break
            l = self.mp.check_packet(data)
            if l == 0:
                break
            if l < 0:
                report("Invalid data: %s" % (line.strip(),),
                       line_info, self.name, is_err=True)
                self.need_scan = True
                continue
            if l == 5:
                report("Ack %02x" % (data[1],), line_info, self.name)
            else:
                msgs = self.mp.dump(data[:l])
                report("%d: %s" % (l, ', '.join(msgs)), line_info, self.name)
            data[:l] = []

def read_candump(canfile, canid, dictionary):
    mp = msgproto.MessageParser()
    mp.process_identify(dictionary, decompress=False)
    rxid = "%03X" % (canid | 1,)
    txid = "%03X" % (canid & ~1,)
    handlers = {rxid: canscan("RX", mp), txid: canscan("TX", mp)}

    last_time = -1.
    line_number = 0
    must_scan = False
    data = bytearray()
    for line in canfile:
        line_number += 1
        parts = line.split()
        if len(parts) < 7:
            if parts:
                report("Ignoring line: %s" % (line.strip(),),
                       (line_number, 0.), is_err=True)
            continue
        p_ts = parts[0]
        p_canid = parts[5]
        p_len = parts[6]
        p_data = parts[7:]
        if (not p_ts.startswith('(') or not p_ts.endswith(')')
            or not p_len.startswith('[') or not p_len.endswith(']')):
            report("Ignoring line: %s" % (line.strip(),),
                   (line_number, 0.), is_err=True)
            continue
        new_time = float(p_ts[1:-1])
        line_info = (line_number, new_time)
        if new_time < last_time:
            report("Backwards time %.6f vs %.6f: %s"
                   % (new_time, last_time, line.strip()),
                   line_info, is_err=True)
        last_time = new_time

        hdlr = handlers.get(p_canid)
        if hdlr is not None:
            newdata = [int(i, 16) for i in p_data]
            hdlr.handle_data(line_info, line, newdata)

def main():
    usage = "%prog <candump.log> <canid> <mcu.dict>"
    opts = optparse.OptionParser(usage)
    options, args = opts.parse_args()
    if len(args) != 3:
        opts.error("Incorrect number of arguments")
    canfilename, canid, dictfilename = args
    canid = int(canid, 16)

    import_msgproto()
    dictionary = read_dictionary(dictfilename)

    canfile = open(canfilename, "r")
    read_candump(canfile, canid, dictionary)
    canfile.close()

if __name__ == '__main__':
    main()
