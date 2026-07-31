# General threaded executor
#
# Copyright (C) 2026  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import chelper
from queue import Queue
from threading import Thread

class WrapperAIO:
    def __init__(self, executor, object):
        self.object = object
        self.executor = executor
    # Call any not-implemented function from the wrapped object
    # Assume it is blocking, so pass to the thread
    def __getattr__(self, name):
        attr = getattr(self.object, name)
        if not callable(attr):
            return attr
        def proxy(*args, **kwargs):
            return self.executor.submit(attr, *args, **kwargs)
        return proxy

class Executor:
    class sentinel: pass
    def __init__(self, reactor, thread_name=""):
        self.reactor = reactor
        self._queue = Queue()
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
    def submit(self, fn, *args, **kwargs):
        if not self._wait_for_work:
            return fn(*args, **kwargs)
        completion = self.reactor.completion()
        self._queue.put_nowait((completion, fn, args, kwargs))
        result, exc = completion.wait()
        if exc:
            raise exc
        return result
    def wrap_obj(self, object):
        return WrapperAIO(self, object)
    def join(self):
        self._wait_for_work = False
        self._queue.put_nowait(self.sentinel)
        self._thread.join()

class Dispatcher:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self._all_executors = []
        self.printer.register_event_handler("klippy:disconnect",
                                            self._handle_disconnect)
    def _handle_disconnect(self):
        for executor in self._all_executors:
            executor.join()
    def allocate_executor(self, name=""):
        executor = Executor(self.reactor, name)
        self._all_executors.append(executor)
        return executor

def load_config(config):
    return Dispatcher(config)
