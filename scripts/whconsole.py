#!/usr/bin/env python2
# Test console for webhooks interface
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, optparse, socket, fcntl, select, json, errno, time

# Set a file-descriptor as non-blocking
def set_nonblock(fd):
    fcntl.fcntl(fd, fcntl.F_SETFL
                , fcntl.fcntl(fd, fcntl.F_GETFL) | os.O_NONBLOCK)

def webhook_socket_create(uds_filename):
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.setblocking(0)
    sys.stderr.write("Waiting for connect to %s\n" % (uds_filename,))
    while 1:
        try:
            sock.connect(uds_filename)
        except socket.error as e:
            if e.errno == errno.ECONNREFUSED:
                time.sleep(0.1)
                continue
            sys.stderr.write("Unable to connect socket %s [%d,%s]\n"
                             % (uds_filename, e.errno,
                                errno.errorcode[e.errno]))
            sys.exit(-1)
        break
    sys.stderr.write("Connection.\n")
    return sock

class KeyboardReader:
    def __init__(self, uds_filename):
        self.kbd_fd = sys.stdin.fileno()
        set_nonblock(self.kbd_fd)
        self.webhook_socket = webhook_socket_create(uds_filename)
        self.poll = select.poll()
        self.poll.register(sys.stdin, select.POLLIN | select.POLLHUP)
        self.poll.register(self.webhook_socket, select.POLLIN | select.POLLHUP)
        self.kbd_data = self.socket_data = b""
    def process_socket(self):
        data = self.webhook_socket.recv(4096)
        if not data:
            sys.stderr.write("Socket closed\n")
            sys.exit(0)
        parts = data.split(b'\x03')
        parts[0] = self.socket_data + parts[0]
        self.socket_data = parts.pop()
        for line in parts:
            sys.stdout.write("GOT: %s\n" % (line,))
    def process_kbd(self):
        data = os.read(self.kbd_fd, 4096)
        parts = data.split(b'\n')
        parts[0] = self.kbd_data + parts[0]
        self.kbd_data = parts.pop()
        for line in parts:
            line = line.strip()
            if not line or line.startswith(b'#'):
                continue
            try:
                m = json.loads(line)
            except:
                sys.stderr.write("ERROR: Unable to parse line\n")
                continue
            cm = json.dumps(m, separators=(',', ':'))
            sys.stdout.write("SEND: %s\n" % (cm,))
            self.webhook_socket.send(cm.encode() + b"\x03")
    def run(self):
        while 1:
            res = self.poll.poll(1000.)
            for fd, event in res:
                if fd == self.kbd_fd:
                    self.process_kbd()
                else:
                    self.process_socket()

def main():
    usage = "%prog [options] <socket filename>"
    opts = optparse.OptionParser(usage)
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")

    ml = KeyboardReader(args[0])
    ml.run()

if __name__ == '__main__':
    main()
