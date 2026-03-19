# General threaded executor
#
# Copyright (C) 2026  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import chelper
from queue import Queue, Full
from threading import Thread, Condition, Lock

class Executor:
    class sentinel: pass
    def __init__(self, reactor):
        self.reactor = reactor
        self._completion = None
        self._queue = Queue(1)
        self._qsize = 0
        self._thread = Thread(target=self._main)
        self._wait_for_work = True
        self._thread.start()
    def _main(self):
        while self._wait_for_work:
            try:
                item = self._queue.get()
                if item is self.sentinel:
                    return
                fn, args, kwargs = item
                res = fn(*args, **kwargs)
                self.reactor.async_complete(self._completion, (res, None))
            except Exception as e:
                self.reactor.async_complete(self._completion, (None, e))
    # Single execution flow is assumed
    # *Any* race condition should lead to an exception in the queue
    # Public to allow offload function which expects fd as an argument
    def submit(self, fn, *args, **kwargs):
        if self._qsize:
            raise Full
        self._completion = self.reactor.completion()
        self._qsize += 1
        self._queue.put_nowait((fn, args, kwargs))
        result, exc = self._completion.wait()
        self._qsize -= 1
        self._completion = None
        if exc:
            raise exc
        return result
    def set_thread_name(self, name):
        name_short = name[:15]
        stn = chelper.get_ffi()[1].set_thread_name
        self.submit(stn, name_short.encode('utf-8'))
    def join(self):
        self._wait_for_work = False
        self._queue.put_nowait(self.sentinel)
        self._thread.join()

class FileAIO:
    def __init__(self, pool, open_call, *args, **kwargs):
        self.pool = pool
        self.executor = self.pool.pop()
        self.file_object = None
        try:
            f = self.executor.submit(open_call, *args, **kwargs)
            self.file_object = f
        except:
            self.close()
            raise
    def set_thread_name(self, name):
        self.executor.set_thread_name(name)
    def submit(self, fn, *args, **kwargs):
        self.executor.submit(fn, *args, **kwargs)
    def get_file_object(self):
        return self.file_object
    def close(self):
        if self.file_object is not None:
            self.executor.submit(self.file_object.close)
        self.pool.append(self.executor)
    # Call any not-implemented function from the wrapped object
    # Assume it is blocking, so pass to the thread
    def __getattr__(self, name):
        attr = getattr(self.file_object, name)
        if not callable(attr):
            return attr
        def proxy(*args, **kwargs):
            return self.executor.submit(attr, *args, **kwargs)
        return proxy
    def __enter__(self):
        return self
    def __exit__(self, *args):
        self.close()

class Dispatcher:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self._all_executors = []
        self._free_executors = []
        self.pool_timer = None
        self.printer.register_event_handler("klippy:disconnect",
                                            self._handle_disconnect)
    def _handle_disconnect(self):
        for executor in self._all_executors:
            executor.join()
    # Eventually free unused threads
    def _recycle(self, eventtime):
        if self._free_executors:
            executor = self._free_executors.pop()
            i = self._all_executors.index(executor)
            self._all_executors.pop(i)
            executor.join()
        if not self._all_executors:
            self.reactor.unregister_timer(self.pool_timer)
            self.pool_timer = None
        return eventtime + 30.0
    def _ensure_one_available(self):
        if not self._all_executors:
            eventtime = self.reactor.monotonic()
            t = self.reactor.register_timer(self._recycle, eventtime + 30.0)
            self.pool_timer = t
        if not self._free_executors:
            executor = Executor(self.reactor)
            self._free_executors.append(executor)
            self._all_executors.append(executor)
    def get_wrapper(self, open_call, *args, **kwargs):
        self._ensure_one_available()
        return FileAIO(self._free_executors, open_call, *args, **kwargs)

def load_config(config):
    return Dispatcher(config)
