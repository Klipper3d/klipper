#!/usr/bin/env python
# Tool to query CAN bus uuids
#
# Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import json, optparse, os, select, socket, struct, sys, time, zlib

CANBUS_ID_ADMIN = 0x3f0
CMD_QUERY_UNASSIGNED = 0x00
RESP_NEED_NODEID = 0x20
CMD_SET_KLIPPER_NODEID = 0x01
CMD_SET_CANBOOT_NODEID = 0x11

MESSAGE_MIN = 5
MESSAGE_MAX = 64
MESSAGE_HEADER_SIZE = 2
MESSAGE_TRAILER_SIZE = 3
MESSAGE_SEQ_MASK = 0x0f
MESSAGE_DEST = 0x10
MESSAGE_SYNC = 0x7e

CAN_FRAME_FMT = "=IB3x8s"
CAN_FRAME_SIZE = struct.calcsize(CAN_FRAME_FMT)
SOL_CAN_RAW = getattr(socket, "SOL_CAN_RAW", 101)
CAN_RAW_FILTER = getattr(socket, "CAN_RAW_FILTER", 1)

class error(Exception):
    pass

def output_line(msg):
    sys.stdout.write("%s\n" % (msg,))
    sys.stdout.flush()

def crc16_ccitt(buf):
    crc = 0xffff
    for data in bytearray(buf):
        data ^= crc & 0xff
        data ^= (data & 0x0f) << 4
        crc = ((data << 8) | (crc >> 8)) ^ (data >> 4) ^ (data << 3)
    return bytearray([(crc >> 8) & 0xff, crc & 0xff])

def encode_int(v):
    out = bytearray()
    if v >= 0xc000000 or v < -0x4000000:
        out.append(((v >> 28) & 0x7f) | 0x80)
    if v >= 0x180000 or v < -0x80000:
        out.append(((v >> 21) & 0x7f) | 0x80)
    if v >= 0x3000 or v < -0x1000:
        out.append(((v >> 14) & 0x7f) | 0x80)
    if v >= 0x60 or v < -0x20:
        out.append(((v >> 7) & 0x7f) | 0x80)
    out.append(v & 0x7f)
    return out

def parse_int(buf, pos):
    c = buf[pos]
    pos += 1
    v = c & 0x7f
    if (c & 0x60) == 0x60:
        v |= -0x20
    while c & 0x80:
        c = buf[pos]
        pos += 1
        v = (v << 7) | (c & 0x7f)
    return int(v & 0xffffffff), pos

def parse_string(buf, pos):
    count = buf[pos]
    pos += 1
    return bytes(buf[pos:pos + count]), pos + count

def check_packet(buf):
    if len(buf) < MESSAGE_MIN:
        return 0
    msglen = buf[0]
    if msglen < MESSAGE_MIN or msglen > MESSAGE_MAX:
        return -1
    msgseq = buf[1]
    if (msgseq & ~MESSAGE_SEQ_MASK) != MESSAGE_DEST:
        return -1
    if len(buf) < msglen:
        return 0
    if buf[msglen - 1] != MESSAGE_SYNC:
        return -1
    msgcrc = buf[msglen - 3:msglen - 1]
    if msgcrc != crc16_ccitt(buf[:msglen - MESSAGE_TRAILER_SIZE]):
        return -1
    return msglen

def format_mcu_name(mcu):
    mcu = str(mcu).upper()
    if mcu.endswith("XX"):
        mcu = mcu[:-2]
    return mcu

class SocketCan:
    def __init__(self, canbus_iface, can_filter=None):
        self.sock = socket.socket(socket.PF_CAN, socket.SOCK_RAW,
                                  socket.CAN_RAW)
        if can_filter is not None:
            packed_filter = struct.pack("=II", can_filter, 0x7ff)
            self.sock.setsockopt(SOL_CAN_RAW, CAN_RAW_FILTER, packed_filter)
        self.sock.bind((canbus_iface,))

    def close(self):
        self.sock.close()

    def send(self, can_id, data):
        data = bytes(bytearray(data))
        frame = struct.pack(CAN_FRAME_FMT, can_id, len(data),
                            data + b"\x00" * (8 - len(data)))
        self.sock.send(frame)

    def recv(self, timeout):
        rlist, _wlist, _xlist = select.select([self.sock], [], [], timeout)
        if not rlist:
            return None
        frame = self.sock.recv(CAN_FRAME_SIZE)
        can_id, can_dlc, data = struct.unpack(CAN_FRAME_FMT, frame)
        return can_id & 0x7ff, bytearray(data[:can_dlc])

class MessageParser:
    def __init__(self):
        self.commands = {"identify offset=%u count=%c": 1}
        self.responses = {"identify_response offset=%u data=%.*s": 0}
        self.messages_by_id = {
            0: "identify_response offset=%u data=%.*s"
        }
        self.version = ""
        self.config = {}

    def process_identify(self, data):
        data = zlib.decompress(data)
        if not isinstance(data, str):
            data = data.decode()
        dictionary = json.loads(data)
        self.commands = dictionary.get("commands", {})
        self.responses = dictionary.get("responses", {})
        self.messages_by_id = dict((v, k) for k, v in self.responses.items())
        self.version = dictionary.get("version", "")
        self.config = dictionary.get("config", {})

    def lookup_command(self, name):
        for msgformat, msgid in self.commands.items():
            if msgformat.split()[0] == name:
                return msgformat, msgid
        raise error("Unknown command: %s" % (name,))

    def create_command(self, name, params=None):
        if params is None:
            params = {}
        msgformat, msgid = self.lookup_command(name)
        out = encode_int(msgid)
        for arg in msgformat.split()[1:]:
            argname, argtype = arg.split("=")
            val = params[argname]
            if argtype in ("%u", "%i", "%hu", "%hi", "%c"):
                out += encode_int(int(val))
            elif argtype in ("%s", "%.*s", "%*s"):
                val = bytes(bytearray(val))
                out.append(len(val))
                out += val
            else:
                raise error("Unknown parameter type: %s" % (argtype,))
        return out

    def parse_response(self, block):
        pos = MESSAGE_HEADER_SIZE
        msgid, pos = parse_int(block, pos)
        msgformat = self.messages_by_id.get(msgid)
        if msgformat is None:
            return None
        params = {"#name": msgformat.split()[0]}
        for arg in msgformat.split()[1:]:
            argname, argtype = arg.split("=")
            if argtype in ("%u", "%i", "%hu", "%hi", "%c"):
                params[argname], pos = parse_int(block, pos)
            elif argtype in ("%s", "%.*s", "%*s"):
                params[argname], pos = parse_string(block, pos)
            else:
                raise error("Unknown parameter type: %s" % (argtype,))
        return params

class CanbusQueryConnection:
    def __init__(self, canbus_iface, uuid, canbus_nodeid, connect_timeout):
        self.canbus_iface = canbus_iface
        self.uuid = uuid
        self.canbus_nodeid = canbus_nodeid
        self.connect_timeout = connect_timeout
        self.txid = canbus_nodeid * 2 + 0x100
        self.bus = SocketCan(canbus_iface, self.txid + 1)
        self.msgparser = MessageParser()
        self.send_seq = 1
        self.input_buf = bytearray()

    def close(self):
        self.bus.close()

    def _send_set_nodeid(self):
        uuid = [(self.uuid >> (40 - i * 8)) & 0xff for i in range(6)]
        self.bus.send(CANBUS_ID_ADMIN,
                      [CMD_SET_KLIPPER_NODEID] + uuid + [self.canbus_nodeid])

    def _send_block(self, payload):
        msglen = MESSAGE_MIN + len(payload)
        seq = MESSAGE_DEST | (self.send_seq & MESSAGE_SEQ_MASK)
        block = bytearray([msglen, seq]) + bytearray(payload)
        block += crc16_ccitt(block)
        block.append(MESSAGE_SYNC)
        for pos in range(0, len(block), 8):
            self.bus.send(self.txid, block[pos:pos + 8])
        self.send_seq += 1

    def _read_block(self, deadline):
        while time.time() < deadline:
            msg = self.bus.recv(deadline - time.time())
            if msg is None:
                break
            can_id, data = msg
            if can_id != self.txid + 1:
                continue
            self.input_buf += data
            while self.input_buf:
                msglen = check_packet(self.input_buf)
                if msglen == 0:
                    break
                if msglen < 0:
                    sync_pos = self.input_buf.find(bytearray([MESSAGE_SYNC]))
                    if sync_pos < 0:
                        del self.input_buf[:]
                    else:
                        del self.input_buf[:sync_pos + 1]
                    continue
                block = self.input_buf[:msglen]
                del self.input_buf[:msglen]
                if msglen == MESSAGE_MIN:
                    continue
                return block
        return None

    def send_with_response(self, name, params, response, deadline):
        retry_delay = .010
        while time.time() < deadline:
            self._send_block(self.msgparser.create_command(name, params))
            retry_deadline = min(deadline, time.time() + retry_delay)
            while time.time() < retry_deadline:
                block = self._read_block(retry_deadline)
                if block is None:
                    break
                msg = self.msgparser.parse_response(block)
                if msg is not None and msg["#name"] == response:
                    return msg
            retry_delay = min(retry_delay * 2., .500)
        raise error("Unable to connect")

    def identify(self):
        self._send_set_nodeid()
        deadline = time.time() + self.connect_timeout
        identify_data = bytearray()
        while 1:
            params = {
                "offset": len(identify_data),
                "count": 40
            }
            msg = self.send_with_response("identify", params,
                                          "identify_response", deadline)
            if msg["offset"] != len(identify_data):
                continue
            data = msg["data"]
            if not data:
                break
            identify_data += data
        self.msgparser.process_identify(bytes(identify_data))
        try:
            msg = self.send_with_response("get_canbus_id", {}, "canbus_id",
                                          deadline)
        except error as e:
            if "Unknown command: get_canbus_id" not in str(e):
                raise
            return
        got_uuid = sum([v << ((5 - i) * 8)
                        for i, v in enumerate(bytearray(msg["canbus_uuid"]))])
        if got_uuid != self.uuid:
            raise error("Failed to match canbus_uuid")

    def reset(self):
        try:
            self._send_block(self.msgparser.create_command("reset"))
        except error:
            pass

def get_firmware_info(canbus_iface, uuid, canbus_nodeid, connect_timeout):
    conn = CanbusQueryConnection(canbus_iface, uuid, canbus_nodeid,
                                 connect_timeout)
    try:
        conn.identify()
        processor = format_mcu_name(conn.msgparser.config.get("MCU",
                                                              "Unknown"))
        firmware = conn.msgparser.version or "Unknown"
        return {"processor": processor, "firmware": firmware}
    finally:
        try:
            conn.reset()
        except Exception:
            pass
        conn.close()

def wait_can_iface(canbus_iface, timeout):
    deadline = time.time() + timeout
    sys_path = "/sys/class/net/%s" % (canbus_iface,)
    while time.time() < deadline:
        if os.path.exists(sys_path):
            try:
                test_sock = socket.socket(socket.PF_CAN, socket.SOCK_RAW,
                                          socket.CAN_RAW)
                test_sock.bind((canbus_iface,))
                test_sock.close()
                return True
            except OSError:
                pass
        time.sleep(.100)
    return False

def get_firmware_info_with_retry(canbus_iface, uuid, canbus_nodeid,
                                 connect_timeout, iface_timeout):
    endtime = time.time() + iface_timeout
    last_error = None
    try:
        return get_firmware_info(canbus_iface, uuid, canbus_nodeid,
                                 connect_timeout)
    except OSError as e:
        last_error = e
    while time.time() < endtime:
        remaining = endtime - time.time()
        if not wait_can_iface(canbus_iface, remaining):
            break
        time.sleep(.250)
        try:
            return get_firmware_info(canbus_iface, uuid, canbus_nodeid,
                                     connect_timeout)
        except OSError as e:
            last_error = e
    raise last_error

def query_unassigned(canbus_iface, canbus_nodeid, connect_timeout,
                     iface_timeout):
    bus = SocketCan(canbus_iface, CANBUS_ID_ADMIN + 1)
    try:
        bus.send(CANBUS_ID_ADMIN, [CMD_QUERY_UNASSIGNED])
        found_ids = {}
        found_devices = []
        start_time = curtime = time.time()
        while 1:
            tdiff = start_time + 2. - curtime
            if tdiff <= 0.:
                break
            msg = bus.recv(tdiff)
            curtime = time.time()
            if msg is None:
                continue
            can_id, data = msg
            if (can_id != CANBUS_ID_ADMIN + 1 or len(data) < 7
                or data[0] != RESP_NEED_NODEID):
                continue
            uuid = sum([v << ((5 - i) * 8)
                        for i, v in enumerate(data[1:7])])
            if uuid in found_ids:
                continue
            app_names = {
                CMD_SET_KLIPPER_NODEID: "Klipper",
                CMD_SET_CANBOOT_NODEID: "CanBoot/Katapult"
            }
            app_id = CMD_SET_KLIPPER_NODEID
            if len(data) > 7:
                app_id = data[7]
            app_name = app_names.get(app_id, "Unknown")
            found_ids[uuid] = 1
            found_devices.append((uuid, app_id, app_name))
    finally:
        bus.close()

    for uuid, app_id, app_name in found_devices:
        if app_id != CMD_SET_KLIPPER_NODEID:
            output_line("Found canbus_uuid=%012x, Application: %s"
                        % (uuid, app_name))
            continue
        try:
            info = get_firmware_info_with_retry(canbus_iface, uuid,
                                                canbus_nodeid,
                                                connect_timeout,
                                                iface_timeout)
            output_line("Found canbus_uuid=%012x, Application: %s,"
                        " Processor: %s,"
                        " Firmware: %s"
                        % (uuid, app_name, info["processor"],
                           info["firmware"]))
        except (error, OSError) as e:
            output_line("Found canbus_uuid=%012x, Application: %s"
                        % (uuid, app_name))
            output_line("Unable to query firmware info: %s" % (str(e),))
    output_line("Total %d uuids found" % (len(found_ids,)))

def main():
    usage = "%prog [options] <can interface>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-i", "--canbus-nodeid", "--canbus_nodeid",
                    type="int", dest="canbus_nodeid",
                    default=64, help="Temporary CAN nodeid to use while"
                    " querying Klipper nodes (default 64)")
    opts.add_option("-t", "--connect-timeout", type="float",
                    dest="connect_timeout", default=10.,
                    help="CAN connect timeout per Klipper node (default 10)")
    opts.add_option("--iface-timeout", type="float", dest="iface_timeout",
                    default=5., help="CAN interface recovery timeout after"
                    " reset (default 5)")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    query_unassigned(args[0], options.canbus_nodeid, options.connect_timeout,
                     options.iface_timeout)

if __name__ == '__main__':
    main()
