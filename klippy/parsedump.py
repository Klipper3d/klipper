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
        data += newdata
        while 1:
            l = mp.check_packet(data)
            if l == 0:
                break
            if l < 0:
                logging.error("Invalid data")
                data = data[-l:]
                continue
            msgs = mp.dump(bytearray(data[:l]))
            sys.stdout.write('\n'.join(msgs[1:]) + '\n')
            data = data[l:]

if __name__ == '__main__':
    main()
