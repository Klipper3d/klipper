# Serial port management for firmware communication
#
# Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
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
        # Threading
        self.lock = threading.Lock()
        self.background_thread = None
        # Message handlers
        self.handlers = {}
        self.register_response(self._handle_unknown_init, '#unknown')
        self.register_response(self.handle_output, '#output')
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
            hdl = (params['#name'], params.get('oid'))
            try:
                with self.lock:
                    hdl = self.handlers.get(hdl, self.handle_default)
                    hdl(params)
            except:
                logging.exception("Exception in serial callback")
    def _get_identify_data(self, timeout):
        # Query the "data dictionary" from the micro-controller
        identify_data = ""
        while 1:
            msg = "identify offset=%d count=%d" % (len(identify_data), 40)
            params = self.send_with_response(msg, 'identify_response')
            if params['offset'] == len(identify_data):
                msgdata = params['data']
                if not msgdata:
                    # Done
                    return identify_data
                identify_data += msgdata
            if self.reactor.monotonic() > timeout:
                raise error("Timeout during identify")
    def connect(self):
        # Initial connection
        logging.info("Starting serial connect")
        start_time = self.reactor.monotonic()
        while 1:
            connect_time = self.reactor.monotonic()
            if connect_time > start_time + 150.:
                raise error("Unable to connect")
            try:
                if self.baud:
                    self.ser = serial.Serial(
                        self.serialport, self.baud, timeout=0, exclusive=True)
                else:
                    self.ser = open(self.serialport, 'rb+')
            except (OSError, IOError, serial.SerialException) as e:
                logging.warn("Unable to open port: %s", e)
                self.reactor.pause(connect_time + 5.)
                continue
            if self.baud:
                stk500v2_leave(self.ser, self.reactor)
            self.serialqueue = self.ffi_lib.serialqueue_alloc(
                self.ser.fileno(), 0)
            self.background_thread = threading.Thread(target=self._bg_thread)
            self.background_thread.start()
            # Obtain and load the data dictionary from the firmware
            try:
                identify_data = self._get_identify_data(connect_time + 5.)
            except error as e:
                logging.exception("Timeout on serial connect")
                self.disconnect()
                continue
            break
        msgparser = msgproto.MessageParser()
        msgparser.process_identify(identify_data)
        self.msgparser = msgparser
        self.register_response(self.handle_unknown, '#unknown')
        # Setup baud adjust
        mcu_baud = msgparser.get_constant_float('SERIAL_BAUD', None)
        if mcu_baud is not None:
            baud_adjust = self.BITS_PER_BYTE / mcu_baud
            self.ffi_lib.serialqueue_set_baud_adjust(
                self.serialqueue, baud_adjust)
        receive_window = msgparser.get_constant_int('RECEIVE_WINDOW', None)
        if receive_window is not None:
            self.ffi_lib.serialqueue_set_receive_window(
                self.serialqueue, receive_window)
    def connect_file(self, debugoutput, dictionary, pace=False):
        self.ser = debugoutput
        self.msgparser.process_identify(dictionary, decompress=False)
        self.serialqueue = self.ffi_lib.serialqueue_alloc(self.ser.fileno(), 1)
    def set_clock_est(self, freq, last_time, last_clock):
        self.ffi_lib.serialqueue_set_clock_est(
            self.serialqueue, freq, last_time, last_clock)
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
        self.ffi_lib.serialqueue_get_stats(
            self.serialqueue, self.stats_buf, len(self.stats_buf))
        return self.ffi_main.string(self.stats_buf)
    def get_msgparser(self):
        return self.msgparser
    def get_default_command_queue(self):
        return self.default_cmd_queue
    # Serial response callbacks
    def register_response(self, callback, name, oid=None):
        with self.lock:
            if callback is None:
                del self.handlers[name, oid]
            else:
                self.handlers[name, oid] = callback
    # Command sending
    def raw_send(self, cmd, minclock, reqclock, cmd_queue):
        self.ffi_lib.serialqueue_send(
            self.serialqueue, cmd_queue, cmd, len(cmd), minclock, reqclock)
    def send(self, msg, minclock=0, reqclock=0):
        cmd = self.msgparser.create_command(msg)
        self.raw_send(cmd, minclock, reqclock, self.default_cmd_queue)
    def send_with_response(self, msg, response):
        cmd = self.msgparser.create_command(msg)
        src = SerialRetryCommand(self, response)
        return src.get_response([cmd], self.default_cmd_queue)
    def alloc_command_queue(self):
        return self.ffi_main.gc(self.ffi_lib.serialqueue_alloc_commandqueue(),
                                self.ffi_lib.serialqueue_free_commandqueue)
    # Dumping debug lists
    def dump_debug(self):
        out = []
        out.append("Dumping serial stats: %s" % (
            self.stats(self.reactor.monotonic()),))
        sdata = self.ffi_main.new('struct pull_queue_message[1024]')
        rdata = self.ffi_main.new('struct pull_queue_message[1024]')
        scount = self.ffi_lib.serialqueue_extract_old(
            self.serialqueue, 1, sdata, len(sdata))
        rcount = self.ffi_lib.serialqueue_extract_old(
            self.serialqueue, 0, rdata, len(rdata))
        out.append("Dumping send queue %d messages" % (scount,))
        for i in range(scount):
            msg = sdata[i]
            cmds = self.msgparser.dump(msg.msg[0:msg.len])
            out.append("Sent %d %f %f %d: %s" % (
                i, msg.receive_time, msg.sent_time, msg.len, ', '.join(cmds)))
        out.append("Dumping receive queue %d messages" % (rcount,))
        for i in range(rcount):
            msg = rdata[i]
            cmds = self.msgparser.dump(msg.msg[0:msg.len])
            out.append("Receive: %d %f %f %d: %s" % (
                i, msg.receive_time, msg.sent_time, msg.len, ', '.join(cmds)))
        return '\n'.join(out)
    # Default message handlers
    def _handle_unknown_init(self, params):
        logging.debug("Unknown message %d (len %d) while identifying",
                      params['#msgid'], len(params['#msg']))
    def handle_unknown(self, params):
        logging.warn("Unknown message type %d: %s",
                     params['#msgid'], repr(params['#msg']))
    def handle_output(self, params):
        logging.info("%s: %s", params['#name'], params['#msg'])
    def handle_default(self, params):
        logging.warn("got %s", params)
    def __del__(self):
        self.disconnect()

# Class to retry sending of a query command until a given response is received
class SerialRetryCommand:
    TIMEOUT_TIME = 5.0
    RETRY_TIME = 0.500
    def __init__(self, serial, name, oid=None):
        self.serial = serial
        self.name = name
        self.oid = oid
        self.completion = serial.reactor.completion()
        self.min_query_time = serial.reactor.monotonic()
        self.serial.register_response(self.handle_callback, name, oid)
    def handle_callback(self, params):
        if params['#sent_time'] >= self.min_query_time:
            self.min_query_time = self.serial.reactor.NEVER
            self.serial.reactor.async_complete(self.completion, params)
    def get_response(self, cmds, cmd_queue, minclock=0, minsystime=0.):
        first_query_time = query_time = max(self.min_query_time, minsystime)
        while 1:
            for cmd in cmds:
                self.serial.raw_send(cmd, minclock, minclock, cmd_queue)
            params = self.completion.wait(query_time + self.RETRY_TIME)
            if params is not None:
                self.serial.register_response(None, self.name, self.oid)
                return params
            query_time = self.serial.reactor.monotonic()
            if query_time > first_query_time + self.TIMEOUT_TIME:
                self.serial.register_response(None, self.name, self.oid)
                raise error("Timeout on wait for '%s' response" % (self.name,))

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
    logging.debug("Got %s from stk500v2", repr(res))
    ser.baudrate = origbaud

# Attempt an arduino style reset on a serial port
def arduino_reset(serialport, reactor):
    # First try opening the port at a different baud
    ser = serial.Serial(serialport, 2400, timeout=0, exclusive=True)
    ser.read(1)
    reactor.pause(reactor.monotonic() + 0.100)
    # Then toggle DTR
    ser.dtr = True
    reactor.pause(reactor.monotonic() + 0.100)
    ser.dtr = False
    reactor.pause(reactor.monotonic() + 0.100)
    ser.close()
