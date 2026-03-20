# General threaded executor
#
# Copyright (C) 2026  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import chelper
from queue import Queue, Full
from threading import Thread

class Executor:
    class sentinel: pass
    def __init__(self, reactor, thread_name=""):
        self.reactor = reactor
        self._queue = Queue(1)
        self._qsize = 0
        self._thread_name = thread_name[:15]
        self._set_thread_name = chelper.get_ffi()[1].set_thread_name
        self._thread = Thread(target=self._main)
        self._wait_for_work = True
        self._thread.start()
    def _main(self):
        try:
            self._set_thread_name(self._thread_name.encode('utf-8'))
        except:
            pass
        while self._wait_for_work:
            item = self._queue.get()
            if item is self.sentinel:
                return
            completion, fn, args, kwargs = item
            try:
                res = fn(*args, **kwargs)
                self.reactor.async_complete(completion, (res, None))
            except Exception as e:
                self.reactor.async_complete(completion, (None, e))
    # Single execution flow is assumed
    # *Any* race condition should lead to an exception in the queue
    # Public to allow offload function which expects fd as an argument
    def submit(self, fn, *args, **kwargs):
        if self._qsize:
            raise Full
        completion = self.reactor.completion()
        self._qsize += 1
        self._queue.put_nowait((completion, fn, args, kwargs))
        result, exc = completion.wait()
        self._qsize -= 1
        if exc:
            raise exc
        return result
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
    def submit(self, fn, *args, **kwargs):
        return self.executor.submit(fn, *args, **kwargs)
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

class BareExecutor:
    def __init__(self, pool):
        self.pool = pool
        self.executor = self.pool.pop()
    def submit(self, fn, *args, **kwargs):
        return self.executor.submit(fn, *args, **kwargs)
    def finish(self):
        self.pool.append(self.executor)
        self.executor = None
    def __enter__(self):
        return self
    def __exit__(self, *args):
        self.finish()

class Dispatcher:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self._all_executors = []
        self._free_executors = []
        self._counter = 0
        self.printer.register_event_handler("klippy:disconnect",
                                            self._handle_disconnect)
    def _handle_disconnect(self):
        for executor in self._all_executors:
            executor.join()
    def _ensure_one_available(self):
        if not self._free_executors:
            name = "aio %d" % (self._counter)
            self._counter += 1
            executor = Executor(self.reactor, name)
            self._free_executors.append(executor)
            self._all_executors.append(executor)
    def get_wrapper(self, open_call, *args, **kwargs):
        self._ensure_one_available()
        return FileAIO(self._free_executors, open_call, *args, **kwargs)
    def get_executor(self):
        self._ensure_one_available()
        return BareExecutor(self._free_executors)

def load_config(config):
    return Dispatcher(config)
