#!/usr/bin/env python2
# Test console for webhooks interface
#
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, optparse, socket, fcntl, select, json, errno, time
try:
    from urllib.parse import urlparse
except ImportError:
    from urlparse import urlparse

# Set a file-descriptor as non-blocking
def set_nonblock(fd):
    fcntl.fcntl(fd, fcntl.F_SETFL
                , fcntl.fcntl(fd, fcntl.F_GETFL) | os.O_NONBLOCK)

def webhook_socket_create(server_address):
    server_url = urlparse(server_address, allow_fragments=False)
    if server_url.scheme == 'unix' or server_url.scheme == '':
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        sock.setblocking(0)
        server_address = server_url.path
        sys.stderr.write("Waiting for connect to %s\n" % (server_url.path,))
    elif server_url.scheme == 'tcp':
        if server_url.netloc.startswith('['):
            # IPv6 address
            sock = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
        else:
            # IPv4 address
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (server_url.hostname,
                          server_url.port if server_url.port else 7120)
        sys.stderr.write("Waiting for connect to %s:%d\n" % (server_address[0],
                                                             server_address[1]))
    else:
        sys.stderr.write("Unknown scheme %s\n" % (server_url.scheme,))
        sys.exit(-1)
    while 1:
        try:
            sock.connect(server_address)
        except socket.error as e:
            if e.errno == errno.ECONNREFUSED:
                time.sleep(0.1)
                continue
            sys.stderr.write("Unable to connect socket %s [%d,%s]\n"
                             % (server_address, e.errno,
                                errno.errorcode[e.errno]))
            sys.exit(-1)
        break
    sys.stderr.write("Connection.\n")
    return sock

class KeyboardReader:
    def __init__(self, server_address):
        self.kbd_fd = sys.stdin.fileno()
        set_nonblock(self.kbd_fd)
        self.webhook_socket = webhook_socket_create(server_address)
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
    usage = "%prog [options] <server address>"
    opts = optparse.OptionParser(usage)
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")

    ml = KeyboardReader(args[0])
    ml.run()

if __name__ == '__main__':
    main()
