#!/usr/bin/env python2
# Script to parse a serial port data dump
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, sys, logging
import msgproto

def read_dictionary(filename):
    dfile = open(filename, 'rb')
    dictionary = dfile.read()
    dfile.close()
    return dictionary

# bytes to string differ between python 2 & 3
# http://python3porting.com/problems.html#binary-data-in-python-2-and-python-3
if sys.version_info[0] < 3:
    def decode(x):
        return x
    def encode(x):
        return x
else:
    def decode(x):
        return x.decode('ISO-8859-1')
    def encode(x):
        return x.encode('ISO-8859-1')


def main():
    dict_filename, data_filename = sys.argv[1:]

    dictionary = read_dictionary(dict_filename)

    mp = msgproto.MessageParser()
    mp.process_identify(dictionary, decompress=False)

    f = open(data_filename, 'rb')
    fd = f.fileno()
    data = ""
    while 1:
        newdata = os.read(fd, 4096)
        if not newdata:
            break
        data += decode(newdata)
        while 1:
            l = mp.check_packet(data)
            if l == 0:
                break
            if l < 0:
                logging.error("Invalid data")
                data = data[-l:]
                continue
            ba =bytearray(encode(data[:l]))
            msgs = mp.dump(ba)
            sys.stdout.write('\n'.join(msgs[1:]) + '\n')
            data = data[l:]

if __name__ == '__main__':
    main()
