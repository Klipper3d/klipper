#!/usr/bin/env python
# Tool to subscribe to motion data and log it to a disk file
#
# Copyright (C) 2020-2026  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, optparse, socket, select, json, errno, time, zlib, fnmatch

INDEX_UPDATE_TIME = 5.0
ClientInfo = {'program': 'motan_data_logger', 'version': 'v0.1'}

# Available subscriptions when a given config type is found
ConfigSubscriptions = [
    # (cfgtype, capture_name, api_request, request_params)
    ('adxl345', 'accelerometer:{csn}', '{ct}/dump_{ct}', {'sensor': '{csn}'}),
    ('lis2dw', 'accelerometer:{csn}', '{ct}/dump_{ct}', {'sensor': '{csn}'}),
    ('mpu9250', 'accelerometer:{csn}', '{ct}/dump_{ct}', {'sensor': '{csn}'}),
    ('bmi160', 'accelerometer:{csn}', '{ct}/dump_{ct}', {'sensor': '{csn}'}),
    ('icm20948', 'accelerometer:{csn}', '{ct}/dump_{ct}', {'sensor': '{csn}'}),
    ('angle', '{ct}:{csn}', '{ct}/dump_{ct}', {'sensor': '{csn}'}),
    ('probe_eddy_current', 'ldc1612:{csn}', 'ldc1612/dump_ldc1612',
     {'sensor': '{csn}'}),
    ('tmc2130', 'stallguard:{csn}', 'tmc/stallguard_dump', {'name': '{csn}'}),
    ('tmc2209', 'stallguard:{csn}', 'tmc/stallguard_dump', {'name': '{csn}'}),
    ('tmc2260', 'stallguard:{csn}', 'tmc/stallguard_dump', {'name': '{csn}'}),
    ('tmc2240', 'stallguard:{csn}', 'tmc/stallguard_dump', {'name': '{csn}'}),
    ('tmc5160', 'stallguard:{csn}', 'tmc/stallguard_dump', {'name': '{csn}'}),
]

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
    def __init__(self, uds_filename, log_prefix, want_subscriptions):
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
        # Subscriptions
        self.want_subscriptions = want_subscriptions
        self.need_subscriptions = []
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
    # Subscription handling
    def build_subscriptions(self, status):
        avail = []
        # trapq and stepq subscriptions from motion_report
        motion_report = status.get("motion_report", {})
        for trapq in motion_report.get("trapq", []):
            avail.append(("trapq:" + trapq, "motion_report/dump_trapq",
                          {"name": trapq}))
        for stepper in motion_report.get("steppers", []):
            avail.append(("stepq:" + stepper, "motion_report/dump_stepper",
                          {"name": stepper}))
        # config based subsciriptions
        config = status["configfile"]["settings"]
        cfgtypes = {p[0]: p for p in ConfigSubscriptions}
        for cfgname in config.keys():
            ct = cfgname.split()[0]
            if ct not in cfgtypes:
                continue
            cfgtype, capture_name, request, params = cfgtypes[ct]
            cfgparts = cfgname.split()
            csn = cfgparts[-1]
            if len(cfgparts) > 1:
                csn = ' '.join(cfgparts[1:])
            fill = {'ct':ct, 'csn': csn}
            capture_name = capture_name.format(**fill)
            request = request.format(**fill)
            params = {k: v.format(**fill) for k, v in params.items()}
            avail.append((capture_name, request, params))
        # Report available subscriptions
        capture_names = [a[0] for a in avail]
        sys.stdout.write("Available subscriptions:\n  %s\n"
                         % ('\n  '.join(sorted(capture_names)).strip(),))
        # Limit subscriptions to those desired
        want_captures = {}
        for pattern in self.want_subscriptions:
            m = fnmatch.filter(capture_names, pattern)
            want_captures.update({cn: 1 for cn in m})
        want = sorted(list(want_captures.keys()))
        avail_by_name = {a[0]:a for a in avail}
        self.need_subscriptions = [avail_by_name[n] for n in want]
        sys.stdout.write("Subscribing to:\n  %s\n"
                         % ('\n  '.join(want).strip(),))
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
        # Determine available subscriptions
        self.build_subscriptions(status)
        # Subscribe
        for capture_name, request, params in self.need_subscriptions:
            self.send_subscribe(capture_name, request, params)
        sys.stdout.write("Starting capture...\n")
    def handle_dump(self, msg, raw_msg):
        msg_id = msg["id"]
        if "result" not in msg:
            self.error("Unable to subscribe to '%s': %s"
                       % (msg_id, msg.get("error", {}).get("message", "")))
            return
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
    opts.add_option("-s", "--subscribe", dest="subscribe", default="",
                    help="comma separated list of subscription patterns")
    opts.add_option("--no-default", action="store_true",
                    help="disable default subscriptions")
    options, args = opts.parse_args()
    if len(args) != 2:
        opts.error("Incorrect number of arguments")

    # Determine subscriptions
    want_subs = ['trapq:*', 'stepq:*']
    if options.no_default:
        want_subs = []
    want_subs.extend([s.strip() for s in options.subscribe.split(',')])

    # Connect and start capture
    nice()
    dl = DataLogger(args[0], args[1], want_subs)
    dl.run()

if __name__ == '__main__':
    main()
