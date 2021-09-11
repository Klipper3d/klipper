#!/usr/bin/env python
# Tool to subscribe to motion data and log it to a disk file
#
# Copyright (C) 2020-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, optparse, socket, select, json, errno, time, zlib

INDEX_UPDATE_TIME = 5.0
ClientInfo = {'program': 'motan_data_logger', 'version': 'v0.1'}

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

class LogWriter:
    def __init__(self, filename):
        self.file = open(filename, "wb")
        self.comp = zlib.compressobj(zlib.Z_DEFAULT_COMPRESSION,
                                     zlib.DEFLATED, 31)
        self.raw_pos = self.file_pos = 0
    def add_data(self, data):
        d = self.comp.compress(data + b"\x03")
        self.file.write(d)
        self.file_pos += len(d)
        self.raw_pos += len(data) + 1
    def flush(self, flag=zlib.Z_FULL_FLUSH):
        if not self.raw_pos:
            return self.file_pos
        d = self.comp.flush(flag)
        self.file.write(d)
        self.file_pos += len(d)
        return self.file_pos
    def close(self):
        self.flush(zlib.Z_FINISH)
        self.file.close()
        self.file = None
        self.comp = None

class DataLogger:
    def __init__(self, uds_filename, log_prefix):
        # IO
        self.webhook_socket = webhook_socket_create(uds_filename)
        self.poll = select.poll()
        self.poll.register(self.webhook_socket, select.POLLIN | select.POLLHUP)
        self.socket_data = b""
        # Data log
        self.logger = LogWriter(log_prefix + ".json.gz")
        self.index = LogWriter(log_prefix + ".index.gz")
        # Handlers
        self.query_handlers = {}
        self.async_handlers = {}
        # get_status databasing
        self.db = {}
        self.next_index_time = 0.
        # Start login process
        self.send_query("info", "info", {"client_info": ClientInfo},
                        self.handle_info)
    def error(self, msg):
        sys.stderr.write(msg + "\n")
    def finish(self, msg):
        self.error(msg)
        self.logger.close()
        self.index.close()
        sys.exit(0)
    # Unix Domain Socket IO
    def send_query(self, msg_id, method, params, cb):
        self.query_handlers[msg_id] = cb
        msg = {"id": msg_id, "method": method, "params": params}
        cm = json.dumps(msg, separators=(',', ':')).encode()
        self.webhook_socket.send(cm + b"\x03")
    def process_socket(self):
        data = self.webhook_socket.recv(4096)
        if not data:
            self.finish("Socket closed")
        parts = data.split(b"\x03")
        parts[0] = self.socket_data + parts[0]
        self.socket_data = parts.pop()
        for part in parts:
            try:
                msg = json.loads(part)
            except:
                self.error("ERROR: Unable to parse line")
                continue
            self.logger.add_data(part)
            msg_q = msg.get("q")
            if msg_q is not None:
                hdl = self.async_handlers.get(msg_q)
                if hdl is not None:
                    hdl(msg, part)
                continue
            msg_id = msg.get("id")
            hdl = self.query_handlers.get(msg_id)
            if hdl is not None:
                del self.query_handlers[msg_id]
                hdl(msg, part)
                if not self.query_handlers:
                    self.flush_index()
                continue
            self.error("ERROR: Message with unknown id")
    def run(self):
        try:
            while 1:
                res = self.poll.poll(1000.)
                for fd, event in res:
                    if fd == self.webhook_socket.fileno():
                        self.process_socket()
        except KeyboardInterrupt as e:
            self.finish("Keyboard Interrupt")
    # Query response handlers
    def send_subscribe(self, msg_id, method, params, cb=None, async_cb=None):
        if cb is None:
            cb = self.handle_dump
        if async_cb is not None:
            self.async_handlers[msg_id] = async_cb
        params["response_template"] = {"q": msg_id}
        self.send_query(msg_id, method, params, cb)
    def handle_info(self, msg, raw_msg):
        if msg["result"]["state"] != "ready":
            self.finish("Klipper not in ready state")
        self.send_query("list", "objects/list", {}, self.handle_list)
    def handle_list(self, msg, raw_msg):
        subreq = {o: None for o in msg["result"]["objects"]}
        self.send_subscribe("status", "objects/subscribe", {"objects": subreq},
                            self.handle_subscribe, self.handle_async_db)
    def handle_subscribe(self, msg, raw_msg):
        result = msg["result"]
        self.next_index_time = result["eventtime"] + INDEX_UPDATE_TIME
        self.db["status"] = status = result["status"]
        # Subscribe to trapq and stepper queue updates
        motion_report = status.get("motion_report", {})
        for trapq in motion_report.get("trapq", []):
            self.send_subscribe("trapq:" + trapq, "motion_report/dump_trapq",
                                {"name": trapq})
        for stepper in motion_report.get("steppers", []):
            self.send_subscribe("stepq:" + stepper,
                                "motion_report/dump_stepper", {"name": stepper})
        # Subscribe to additional sensor data
        config = status["configfile"]["settings"]
        for cfgname in config.keys():
            if cfgname == "adxl345" or cfgname.startswith("adxl345 "):
                aname = cfgname.split()[-1]
                self.send_subscribe("adxl345:" + aname, "adxl345/dump_adxl345",
                                    {"sensor": aname})
    def handle_dump(self, msg, raw_msg):
        msg_id = msg["id"]
        self.db.setdefault("subscriptions", {})[msg_id] = msg["result"]
    def flush_index(self):
        self.db['file_position'] = self.logger.flush()
        self.index.add_data(json.dumps(self.db, separators=(',', ':')).encode())
        self.db = {"status": {}}
    def handle_async_db(self, msg, raw_msg):
        params = msg["params"]
        db_status = self.db['status']
        for k, v in params.get("status", {}).items():
            db_status.setdefault(k, {}).update(v)
        eventtime = params['eventtime']
        if eventtime >= self.next_index_time:
            self.next_index_time = eventtime + INDEX_UPDATE_TIME
            self.flush_index()

def nice():
    try:
        # Try to re-nice writing process
        os.nice(10)
    except:
        pass

def main():
    usage = "%prog [options] <socket filename> <log name>"
    opts = optparse.OptionParser(usage)
    options, args = opts.parse_args()
    if len(args) != 2:
        opts.error("Incorrect number of arguments")

    nice()
    dl = DataLogger(args[0], args[1])
    dl.run()

if __name__ == '__main__':
    main()
