# File descriptor and timer event helper
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, select, math, time, Queue
import greenlet
import chelper, util

class ReactorTimer:
    def __init__(self, callback, waketime):
        self.callback = callback
        self.waketime = waketime

class ReactorCallback:
    def __init__(self, reactor, callback, waketime):
        self.reactor = reactor
        self.timer = reactor.register_timer(self.invoke, waketime)
        self.callback = callback
    def invoke(self, eventtime):
        self.reactor.unregister_timer(self.timer)
        self.callback(eventtime)
        return self.reactor.NEVER

class ReactorFileHandler:
    def __init__(self, fd, callback):
        self.fd = fd
        self.callback = callback
    def fileno(self):
        return self.fd

class ReactorGreenlet(greenlet.greenlet):
    def __init__(self, run):
        greenlet.greenlet.__init__(self, run=run)
        self.timer = None

class SelectReactor:
    NOW = 0.
    NEVER = 9999999999999999.
    def __init__(self):
        # Main code
        self._process = False
        self.monotonic = chelper.get_ffi()[1].get_monotonic
        # Timers
        self._timers = []
        self._next_timer = self.NEVER
        # Callbacks
        self._pipe_fds = None
        self._async_queue = Queue.Queue()
        # File descriptors
        self._fds = []
        # Greenlets
        self._g_dispatch = None
        self._greenlets = []
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
        g_dispatch = self._g_dispatch
        for t in self._timers:
            if eventtime >= t.waketime:
                t.waketime = self.NEVER
                t.waketime = t.callback(eventtime)
                if g_dispatch is not self._g_dispatch:
                    self._end_greenlet(g_dispatch)
                    return 0.
            self._note_time(t)
        if eventtime >= self._next_timer:
            return 0.
        return min(1., max(.001, self._next_timer - self.monotonic()))
    # Callbacks
    def register_callback(self, callback, waketime = NOW):
        ReactorCallback(self, callback, waketime)
    def register_async_callback(self, callback):
        self._async_queue.put_nowait(callback)
        try:
            os.write(self._pipe_fds[1], '.')
        except os.error:
            pass
    def _got_pipe_signal(self, eventtime):
        try:
            os.read(self._pipe_fds[0], 4096)
        except os.error:
            pass
        while 1:
            try:
                callback = self._async_queue.get_nowait()
            except Queue.Empty:
                break
            ReactorCallback(self, callback, self.NOW)
    def _setup_async_callbacks(self):
        self._pipe_fds = os.pipe()
        util.set_nonblock(self._pipe_fds[0])
        util.set_nonblock(self._pipe_fds[1])
        self.register_fd(self._pipe_fds[0], self._got_pipe_signal)
    def __del__(self):
        if self._pipe_fds is not None:
            os.close(self._pipe_fds[0])
            os.close(self._pipe_fds[1])
            self._pipe_fds = None
    # Greenlets
    def _sys_pause(self, waketime):
        # Pause using system sleep for when reactor not running
        delay = waketime - self.monotonic()
        if delay > 0.:
            time.sleep(delay)
        return self.monotonic()
    def pause(self, waketime):
        g = greenlet.getcurrent()
        if g is not self._g_dispatch:
            if self._g_dispatch is None:
                return self._sys_pause(waketime)
            return self._g_dispatch.switch(waketime)
        if self._greenlets:
            g_next = self._greenlets.pop()
        else:
            g_next = ReactorGreenlet(run=self._dispatch_loop)
        g_next.parent = g.parent
        g.timer = self.register_timer(g.switch, waketime)
        return g_next.switch()
    def _end_greenlet(self, g_old):
        # Cache this greenlet for later use
        self._greenlets.append(g_old)
        self.unregister_timer(g_old.timer)
        g_old.timer = None
        # Switch to existing dispatch
        self._g_dispatch.switch(self.NEVER)
        # This greenlet was reactivated - prepare for main processing loop
        self._g_dispatch = g_old
    # File descriptors
    def register_fd(self, fd, callback):
        handler = ReactorFileHandler(fd, callback)
        self._fds.append(handler)
        return handler
    def unregister_fd(self, handler):
        self._fds.pop(self._fds.index(handler))
    # Main loop
    def _dispatch_loop(self):
        self._g_dispatch = g_dispatch = greenlet.getcurrent()
        eventtime = self.monotonic()
        while self._process:
            timeout = self._check_timers(eventtime)
            res = select.select(self._fds, [], [], timeout)
            eventtime = self.monotonic()
            for fd in res[0]:
                fd.callback(eventtime)
                if g_dispatch is not self._g_dispatch:
                    self._end_greenlet(g_dispatch)
                    eventtime = self.monotonic()
                    break
        self._g_dispatch = None
    def run(self):
        if self._pipe_fds is None:
            self._setup_async_callbacks()
        self._process = True
        g_next = ReactorGreenlet(run=self._dispatch_loop)
        g_next.switch()
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
    def _dispatch_loop(self):
        self._g_dispatch = g_dispatch = greenlet.getcurrent()
        eventtime = self.monotonic()
        while self._process:
            timeout = self._check_timers(eventtime)
            res = self._poll.poll(int(math.ceil(timeout * 1000.)))
            eventtime = self.monotonic()
            for fd, event in res:
                self._fds[fd](eventtime)
                if g_dispatch is not self._g_dispatch:
                    self._end_greenlet(g_dispatch)
                    eventtime = self.monotonic()
                    break
        self._g_dispatch = None

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
    def _dispatch_loop(self):
        self._g_dispatch = g_dispatch = greenlet.getcurrent()
        eventtime = self.monotonic()
        while self._process:
            timeout = self._check_timers(eventtime)
            res = self._epoll.poll(timeout)
            eventtime = self.monotonic()
            for fd, event in res:
                self._fds[fd](eventtime)
                if g_dispatch is not self._g_dispatch:
                    self._end_greenlet(g_dispatch)
                    eventtime = self.monotonic()
                    break
        self._g_dispatch = None

# Use the poll based reactor if it is available
try:
    select.poll
    Reactor = PollReactor
except:
    Reactor = SelectReactor
