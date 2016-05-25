# File descriptor and timer event helper
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import select, time, math

class ReactorTimer:
    def __init__(self, callback, waketime):
        self.callback = callback
        self.waketime = waketime

class ReactorFileHandler:
    def __init__(self, fd, callback):
        self.fd = fd
        self.callback = callback
    def fileno(self):
        return self.fd

class SelectReactor:
    NOW = 0.
    NEVER = 9999999999999999.
    def __init__(self):
        self._fds = []
        self._timers = []
        self._next_timer = self.NEVER
        self._process = True
    # Timers
    def _note_time(self, t):
        nexttime = t.waketime
        if nexttime < self._next_timer:
            self._next_timer = nexttime
    def update_timer(self, t, nexttime):
        t.waketime = nexttime
        self._note_time(t)
    def register_timer(self, callback, waketime = NEVER):
        handler = ReactorTimer(callback, waketime)
        timers = list(self._timers)
        timers.append(handler)
        self._timers = timers
        self._note_time(handler)
        return handler
    def unregister_timer(self, handler):
        timers = list(self._timers)
        timers.pop(timers.index(handler))
        self._timers = timers
    def _check_timers(self, eventtime):
        if eventtime < self._next_timer:
            return min(1., max(.001, self._next_timer - eventtime))
        self._next_timer = self.NEVER
        for t in self._timers:
            if eventtime >= t.waketime:
                t.waketime = t.callback(eventtime)
            self._note_time(t)
        if eventtime >= self._next_timer:
            return 0.
        return min(1., max(.001, self._next_timer - time.time()))
    # File descriptors
    def register_fd(self, fd, callback):
        handler = ReactorFileHandler(fd, callback)
        self._fds.append(handler)
        return handler
    def unregister_fd(self, handler):
        self._fds.pop(self._fds.index(handler))
    # Main loop
    def run(self):
        self._process = True
        eventtime = time.time()
        while self._process:
            timeout = self._check_timers(eventtime)
            res = select.select(self._fds, [], [], timeout)
            eventtime = time.time()
            for fd in res[0]:
                fd.callback(eventtime)
    def end(self):
        self._process = False

class PollReactor(SelectReactor):
    def __init__(self):
        SelectReactor.__init__(self)
        self._poll = select.poll()
        self._fds = {}
    # File descriptors
    def register_fd(self, fd, callback):
        handler = ReactorFileHandler(fd, callback)
        fds = self._fds.copy()
        fds[fd] = callback
        self._fds = fds
        self._poll.register(handler, select.POLLIN | select.POLLHUP)
        return handler
    def unregister_fd(self, handler):
        self._poll.unregister(handler)
        fds = self._fds.copy()
        del fds[handler.fd]
        self._fds = fds
    # Main loop
    def run(self):
        self._process = True
        eventtime = time.time()
        while self._process:
            timeout = int(math.ceil(self._check_timers(eventtime) * 1000.))
            res = self._poll.poll(timeout)
            eventtime = time.time()
            for fd, event in res:
                self._fds[fd](eventtime)

class EPollReactor(SelectReactor):
    def __init__(self):
        SelectReactor.__init__(self)
        self._epoll = select.epoll()
        self._fds = {}
    # File descriptors
    def register_fd(self, fd, callback):
        handler = ReactorFileHandler(fd, callback)
        fds = self._fds.copy()
        fds[fd] = callback
        self._fds = fds
        self._epoll.register(fd, select.EPOLLIN | select.EPOLLHUP)
        return handler
    def unregister_fd(self, handler):
        self._epoll.unregister(handler.fd)
        fds = self._fds.copy()
        del fds[handler.fd]
        self._fds = fds
    # Main loop
    def run(self):
        self._process = True
        eventtime = time.time()
        while self._process:
            timeout = self._check_timers(eventtime)
            res = self._epoll.poll(timeout)
            eventtime = time.time()
            for fd, event in res:
                self._fds[fd](eventtime)

# Use the poll based reactor if it is available
try:
    select.poll
    Reactor = PollReactor
except:
    Reactor = SelectReactor
