# General threaded AIO executor
#
# Copyright (C) 2026  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os
import chelper
import logging
from queue import Queue
from threading import Thread

class FileAIO:
    def __init__(self, completion, pool, fn, *args, **kwargs):
        self.completion = completion
        self.pool = pool
        self.set_thread_name = chelper.get_ffi()[1].set_thread_name
        self.pair = self.pool.pop()
        self.queue, _ = self.pair
        self.fd = None
        try:
            self.fd = self.submit(self._open, fn, *args, **kwargs)
        except:
            self.close()
            raise
    def _open(self, fn, *args, **kwargs):
        fd = fn(*args, **kwargs)
        try:
            name = os.path.basename(fd.name)
            name_short = ("IO %s" % (name))[:15]
            self.set_thread_name(name_short.encode('utf-8'))
        except:
            logging.exception("AIO name update error")
        return fd
    # Single execution flow is assumed
    # Any race condition should lead to an exception in the queue
    # Public to allow offload function which expects fd as an argument
    def submit(self, fn, *args, **kwargs):
        completion = self.completion()
        self.queue.put_nowait((fn, args, kwargs, completion))
        result, exc = completion.wait()
        if exc:
            raise exc
        return result
    def get_fd(self):
        return self.fd
    def _free_fd_thread(self):
        if self.fd is not None:
            self.fd.close()
        self.set_thread_name("IO pool".encode('utf-8'))
    def close(self):
        self.submit(self._free_fd_thread)
        self.pool.append(self.pair)
    # Call any not-implemented function from the wrapped object
    # Assume it is blocking, so pass to the thread
    def __getattr__(self, name):
        attr = getattr(self.fd, name)
        if not callable(attr):
            return attr
        def proxy(*args, **kwargs):
            return self.submit(attr, *args, **kwargs)
        return proxy
    def __enter__(self):
        return self
    def __exit__(self, *args):
        self.close()

class AIODispatcher:
    class sentinel: pass
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self._all = []
        self.pool = []
        self.pool_timer = None
        self.printer.register_event_handler("klippy:disconnect",
                                            self._handle_disconnect)
    def _handle_disconnect(self):
        for queue, thread in self._all:
            queue.put(self.sentinel)
            thread.join()
    # Eventually free unused threads
    def _recycle(self, eventtime):
        if self.pool:
            pair = self.pool.pop()
            i = self._all.index(pair)
            self._all.pop(i)
            queue, thread = pair
            queue.put(self.sentinel)
            thread.join()
        if not self._all:
            self.reactor.unregister_timer(self.pool_timer)
            self.pool_timer = None
        return eventtime + 30.0
    def _run(self, queue):
        while True:
            item = queue.get()
            if item is self.sentinel:
                break
            fn, args, kwargs, completion = item
            try:
                res = fn(*args, **kwargs)
                self.reactor.async_complete(completion, (res, None))
            except Exception as e:
                self.reactor.async_complete(completion, (None, e))
    def _ensure_one_available(self):
        if not self._all:
            eventtime = self.reactor.monotonic()
            self.pool_timer = self.reactor.register_timer(self._recycle,
                                                          eventtime + 30.0)
        if not self.pool:
            queue = Queue(1)
            thread = Thread(target=self._run, args=([queue]))
            thread.start()
            pair = (queue, thread)
            self.pool.append(pair)
            self._all.append(pair)
    def get_wrapper(self, fn, *args, **kwargs):
        self._ensure_one_available()
        completion = self.reactor.completion
        return FileAIO(completion, self.pool, fn, *args, **kwargs)

def load_config(config):
    return AIODispatcher(config)
