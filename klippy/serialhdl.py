# Serial port management for firmware communication
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, threading
import serial

import msgproto, chelper, util

class error(Exception):
    pass

class SerialReader:
    BITS_PER_BYTE = 10.
    def __init__(self, reactor, serialport, baud):
        self.reactor = reactor
        self.serialport = serialport
        self.baud = baud
        # Serial port
        self.ser = None
        self.msgparser = msgproto.MessageParser()
        # C interface
        self.ffi_main, self.ffi_lib = chelper.get_ffi()
        self.serialqueue = None
        self.default_cmd_queue = self.alloc_command_queue()
        self.stats_buf = self.ffi_main.new('char[4096]')
        # MCU time/clock tracking
        self.last_ack_time = self.last_ack_rtt_time = 0.
        self.last_ack_clock = self.last_ack_rtt_clock = 0
        self.est_clock = 0.
        # Threading
        self.lock = threading.Lock()
        self.background_thread = None
        # Message handlers
        self.status_timer = self.reactor.register_timer(self._status_event)
        self.status_cmd = None
        handlers = {
            '#unknown': self.handle_unknown,
            '#output': self.handle_output, 'status': self.handle_status,
            'shutdown': self.handle_output, 'is_shutdown': self.handle_output
        }
        self.handlers = dict(((k, None), v) for k, v in handlers.items())
    def _bg_thread(self):
        response = self.ffi_main.new('struct pull_queue_message *')
        while 1:
            self.ffi_lib.serialqueue_pull(self.serialqueue, response)
            count = response.len
            if count <= 0:
                break
            params = self.msgparser.parse(response.msg[0:count])
            params['#sent_time'] = response.sent_time
            params['#receive_time'] = response.receive_time
            with self.lock:
                hdl = (params['#name'], params.get('oid'))
                hdl = self.handlers.get(hdl, self.handle_default)
            try:
                hdl(params)
            except:
                logging.exception("Exception in serial callback")
    def connect(self):
        # Initial connection
        logging.info("Starting serial connect")
        while 1:
            starttime = self.reactor.monotonic()
            try:
                if self.baud:
                    self.ser = serial.Serial(
                        self.serialport, self.baud, timeout=0)
                else:
                    self.ser = open(self.serialport, 'rb+')
            except (OSError, serial.SerialException), e:
                logging.warn("Unable to open port: %s" % (e,))
                self.reactor.pause(starttime + 5.)
                continue
            if self.baud:
                stk500v2_leave(self.ser, self.reactor)
            self.serialqueue = self.ffi_lib.serialqueue_alloc(
                self.ser.fileno(), 0)
            self.background_thread = threading.Thread(target=self._bg_thread)
            self.background_thread.start()
            # Obtain and load the data dictionary from the firmware
            sbs = SerialBootStrap(self)
            identify_data = sbs.get_identify_data(starttime + 5.)
            if identify_data is None:
                logging.warn("Timeout on serial connect")
                self.disconnect()
                continue
            break
        msgparser = msgproto.MessageParser()
        msgparser.process_identify(identify_data)
        self.msgparser = msgparser
        self.register_callback(self.handle_unknown, '#unknown')
        logging.info("Loaded %d commands (%s)" % (
            len(msgparser.messages_by_id), msgparser.version))
        logging.info("MCU config: %s" % (" ".join(
            ["%s=%s" % (k, v) for k, v in msgparser.config.items()])))
        # Setup baud adjust
        mcu_baud = float(msgparser.config.get('SERIAL_BAUD', 0.))
        if mcu_baud:
            baud_adjust = self.BITS_PER_BYTE / mcu_baud
            self.ffi_lib.serialqueue_set_baud_adjust(
                self.serialqueue, baud_adjust)
        # Enable periodic get_status timer
        get_status = msgparser.lookup_command('get_status')
        self.status_cmd = get_status.encode()
        self.reactor.update_timer(self.status_timer, self.reactor.NOW)
        # Load initial last_ack_clock/last_ack_time
        uptime_msg = msgparser.create_command('get_uptime')
        params = self.send_with_response(uptime_msg, 'uptime')
        self.last_ack_clock = (params['high'] << 32) | params['clock']
        self.last_ack_time = params['#receive_time']
        # Make sure est_clock is calculated
        starttime = eventtime = self.reactor.monotonic()
        while not self.est_clock:
            if eventtime > starttime + 5.:
                raise error("timeout on est_clock calculation")
            eventtime = self.reactor.pause(eventtime + 0.010)
    def connect_file(self, debugoutput, dictionary, pace=False):
        self.ser = debugoutput
        self.msgparser.process_identify(dictionary, decompress=False)
        est_clock = 1000000000000.
        if pace:
            est_clock = float(self.msgparser.config['CLOCK_FREQ'])
        self.serialqueue = self.ffi_lib.serialqueue_alloc(self.ser.fileno(), 1)
        self.est_clock = est_clock
        self.last_ack_time = self.reactor.monotonic()
        self.last_ack_clock = 0
        self.ffi_lib.serialqueue_set_clock_est(
            self.serialqueue, self.est_clock, self.last_ack_time
            , self.last_ack_clock)
    def disconnect(self):
        if self.serialqueue is not None:
            self.ffi_lib.serialqueue_exit(self.serialqueue)
            if self.background_thread is not None:
                self.background_thread.join()
            self.ffi_lib.serialqueue_free(self.serialqueue)
            self.background_thread = self.serialqueue = None
        if self.ser is not None:
            self.ser.close()
            self.ser = None
    def stats(self, eventtime):
        if self.serialqueue is None:
            return ""
        sqstats = self.ffi_lib.serialqueue_get_stats(
            self.serialqueue, self.stats_buf, len(self.stats_buf))
        sqstats = self.ffi_main.string(self.stats_buf)
        tstats = " est_clock=%.3f last_ack_time=%.3f last_ack_clock=%d" % (
            self.est_clock, self.last_ack_time, self.last_ack_clock)
        return sqstats + tstats
    def _status_event(self, eventtime):
        self.send(self.status_cmd)
        return eventtime + 1.0
    # Serial response callbacks
    def register_callback(self, callback, name, oid=None):
        with self.lock:
            self.handlers[name, oid] = callback
    def unregister_callback(self, name, oid=None):
        with self.lock:
            del self.handlers[name, oid]
    # Clock tracking
    def get_clock(self, eventtime):
        with self.lock:
            return int(self.last_ack_clock
                       + (eventtime - self.last_ack_time) * self.est_clock)
    def translate_clock(self, raw_clock):
        with self.lock:
            last_ack_clock = self.last_ack_clock
        clock_diff = (last_ack_clock - raw_clock) & 0xffffffff
        if clock_diff & 0x80000000:
            return last_ack_clock + 0x100000000 - clock_diff
        return last_ack_clock - clock_diff
    def get_last_clock(self):
        with self.lock:
            return self.last_ack_clock, self.last_ack_time
    # Command sending
    def send(self, cmd, minclock=0, reqclock=0, cq=None):
        if cq is None:
            cq = self.default_cmd_queue
        self.ffi_lib.serialqueue_send(
            self.serialqueue, cq, cmd, len(cmd), minclock, reqclock)
    def encode_and_send(self, data, minclock, reqclock, cq):
        self.ffi_lib.serialqueue_encode_and_send(
            self.serialqueue, cq, data, len(data), minclock, reqclock)
    def send_with_response(self, cmd, name, oid=None):
        src = SerialRetryCommand(self, cmd, name, oid)
        return src.get_response()
    def alloc_command_queue(self):
        return self.ffi_main.gc(self.ffi_lib.serialqueue_alloc_commandqueue(),
                                self.ffi_lib.serialqueue_free_commandqueue)
    # Dumping debug lists
    def dump_debug(self):
        sdata = self.ffi_main.new('struct pull_queue_message[1024]')
        rdata = self.ffi_main.new('struct pull_queue_message[1024]')
        scount = self.ffi_lib.serialqueue_extract_old(
            self.serialqueue, 1, sdata, len(sdata))
        rcount = self.ffi_lib.serialqueue_extract_old(
            self.serialqueue, 0, rdata, len(rdata))
        logging.info("Dumping send queue %d messages" % (scount,))
        for i in range(scount):
            msg = sdata[i]
            cmds = self.msgparser.dump(msg.msg[0:msg.len])
            logging.info("Sent %d %f %f %d: %s" % (
                i, msg.receive_time, msg.sent_time, msg.len, ', '.join(cmds)))
        logging.info("Dumping receive queue %d messages" % (rcount,))
        for i in range(rcount):
            msg = rdata[i]
            cmds = self.msgparser.dump(msg.msg[0:msg.len])
            logging.info("Receive: %d %f %f %d: %s" % (
                i, msg.receive_time, msg.sent_time, msg.len, ', '.join(cmds)))
    # Default message handlers
    def handle_status(self, params):
        with self.lock:
            # Update last_ack_time / last_ack_clock
            ack_clock = (self.last_ack_clock & ~0xffffffff) | params['clock']
            if ack_clock < self.last_ack_clock:
                ack_clock += 0x100000000
            sent_time = params['#sent_time']
            self.last_ack_time = receive_time = params['#receive_time']
            self.last_ack_clock = ack_clock
            # Update est_clock (if applicable)
            if receive_time > self.last_ack_rtt_time + 1. and sent_time:
                if self.last_ack_rtt_time:
                    timedelta = receive_time - self.last_ack_rtt_time
                    clockdelta = ack_clock - self.last_ack_rtt_clock
                    estclock = clockdelta / timedelta
                    if estclock > self.est_clock and self.est_clock:
                        self.est_clock = (self.est_clock * 63. + estclock) / 64.
                    else:
                        self.est_clock = estclock
                self.last_ack_rtt_time = sent_time
                self.last_ack_rtt_clock = ack_clock
            self.ffi_lib.serialqueue_set_clock_est(
                self.serialqueue, self.est_clock, receive_time, ack_clock)
    def handle_unknown(self, params):
        logging.warn("Unknown message type %d: %s" % (
            params['#msgid'], repr(params['#msg'])))
    def handle_output(self, params):
        logging.info("%s: %s" % (params['#name'], params['#msg']))
    def handle_default(self, params):
        logging.warn("got %s" % (params,))
    def __del__(self):
        self.disconnect()

# Class to retry sending of a query command until a given response is received
class SerialRetryCommand:
    TIMEOUT_TIME = 5.0
    RETRY_TIME = 0.500
    def __init__(self, serial, cmd, name, oid=None):
        self.serial = serial
        self.cmd = cmd
        self.name = name
        self.oid = oid
        self.response = None
        self.min_query_time = self.serial.reactor.monotonic()
        self.serial.register_callback(self.handle_callback, self.name, self.oid)
        self.send_timer = self.serial.reactor.register_timer(
            self.send_event, self.serial.reactor.NOW)
    def unregister(self):
        self.serial.unregister_callback(self.name, self.oid)
        self.serial.reactor.unregister_timer(self.send_timer)
    def send_event(self, eventtime):
        if self.response is not None:
            return self.serial.reactor.NEVER
        self.serial.send(self.cmd)
        return eventtime + self.RETRY_TIME
    def handle_callback(self, params):
        last_sent_time = params['#sent_time']
        if last_sent_time >= self.min_query_time:
            self.response = params
    def get_response(self):
        eventtime = self.serial.reactor.monotonic()
        while self.response is None:
            eventtime = self.serial.reactor.pause(eventtime + 0.05)
            if eventtime > self.min_query_time + self.TIMEOUT_TIME:
                self.unregister()
                raise error("Timeout on wait for '%s' response" % (self.name,))
        self.unregister()
        return self.response

# Code to start communication and download message type dictionary
class SerialBootStrap:
    RETRY_TIME = 0.500
    def __init__(self, serial):
        self.serial = serial
        self.identify_data = ""
        self.identify_cmd = self.serial.msgparser.lookup_command(
            "identify offset=%u count=%c")
        self.is_done = False
        self.serial.register_callback(self.handle_identify, 'identify_response')
        self.serial.register_callback(self.handle_unknown, '#unknown')
        self.send_timer = self.serial.reactor.register_timer(
            self.send_event, self.serial.reactor.NOW)
    def get_identify_data(self, timeout):
        eventtime = self.serial.reactor.monotonic()
        while not self.is_done and eventtime <= timeout:
            eventtime = self.serial.reactor.pause(eventtime + 0.05)
        self.serial.unregister_callback('identify_response')
        self.serial.reactor.unregister_timer(self.send_timer)
        if not self.is_done:
            return None
        return self.identify_data
    def handle_identify(self, params):
        if self.is_done or params['offset'] != len(self.identify_data):
            return
        msgdata = params['data']
        if not msgdata:
            self.is_done = True
            return
        self.identify_data += msgdata
        imsg = self.identify_cmd.encode(len(self.identify_data), 40)
        self.serial.send(imsg)
    def send_event(self, eventtime):
        if self.is_done:
            return self.serial.reactor.NEVER
        imsg = self.identify_cmd.encode(len(self.identify_data), 40)
        self.serial.send(imsg)
        return eventtime + self.RETRY_TIME
    def handle_unknown(self, params):
        logging.debug("Unknown message %d (len %d) while identifying" % (
            params['#msgid'], len(params['#msg'])))

# Attempt to place an AVR stk500v2 style programmer into normal mode
def stk500v2_leave(ser, reactor):
    logging.debug("Starting stk500v2 leave programmer sequence")
    util.clear_hupcl(ser.fileno())
    origbaud = ser.baudrate
    # Request a dummy speed first as this seems to help reset the port
    ser.baudrate = 2400
    ser.read(1)
    # Send stk500v2 leave programmer sequence
    ser.baudrate = 115200
    reactor.pause(reactor.monotonic() + 0.100)
    ser.read(4096)
    ser.write('\x1b\x01\x00\x01\x0e\x11\x04')
    reactor.pause(reactor.monotonic() + 0.050)
    res = ser.read(4096)
    logging.debug("Got %s from stk500v2" % (repr(res),))
    ser.baudrate = origbaud

# Attempt an arduino style reset on a serial port
def arduino_reset(serialport, reactor):
    # First try opening the port at 1200 baud
    ser = serial.Serial(serialport, 1200, timeout=0)
    ser.read(1)
    reactor.pause(reactor.monotonic() + 0.100)
    # Then try toggling DTR
    ser.dtr = True
    reactor.pause(reactor.monotonic() + 0.100)
    ser.dtr = False
    reactor.pause(reactor.monotonic() + 0.100)
    ser.close()
