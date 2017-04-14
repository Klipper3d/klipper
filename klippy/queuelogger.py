# Code to implement asynchronous logging from a background thread
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, logging.handlers, threading, Queue

# Class to forward all messages through a queue to a background thread
class QueueHandler(logging.Handler):
    def __init__(self, queue):
        logging.Handler.__init__(self)
        self.queue = queue
    def emit(self, record):
        try:
            self.format(record)
            record.msg = record.message
            record.args = None
            record.exc_info = None
            self.queue.put_nowait(record)
        except Exception:
            self.handleError(record)

# Class to poll a queue in a background thread and log each message
class QueueListener(object):
    def __init__(self, handler):
        self.handler = handler
        self.queue = Queue.Queue()
        self.thread = threading.Thread(target=self._bg_thread)
        self.thread.start()
    def _bg_thread(self):
        while 1:
            record = self.queue.get(True)
            if record is None:
                break
            self.handler.handle(record)
    def stop(self):
        self.queue.put_nowait(None)
        self.thread.join()

def setup_bg_logging(filename, debuglevel):
    handler = logging.handlers.TimedRotatingFileHandler(
        filename, when='midnight', backupCount=5)
    ql = QueueListener(handler)
    qh = QueueHandler(ql.queue)
    root = logging.getLogger()
    root.addHandler(qh)
    root.setLevel(debuglevel)
    return ql
