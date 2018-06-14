# Protocol definitions for firmware communication
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import json, zlib, logging

DefaultMessages = {
    0: "identify_response offset=%u data=%.*s",
    1: "identify offset=%u count=%c",
}

MESSAGE_MIN = 5
MESSAGE_MAX = 64
MESSAGE_HEADER_SIZE  = 2
MESSAGE_TRAILER_SIZE = 3
MESSAGE_POS_LEN = 0
MESSAGE_POS_SEQ = 1
MESSAGE_TRAILER_CRC  = 3
MESSAGE_TRAILER_SYNC = 1
MESSAGE_PAYLOAD_MAX = MESSAGE_MAX - MESSAGE_MIN
MESSAGE_SEQ_MASK = 0x0f
MESSAGE_DEST = 0x10
MESSAGE_SYNC = '\x7E'

class error(Exception):
    pass

def crc16_ccitt(buf):
    crc = 0xffff
    for data in buf:
        data = ord(data)
        data ^= crc & 0xff
        data ^= (data & 0x0f) << 4
        crc = ((data << 8) | (crc >> 8)) ^ (data >> 4) ^ (data << 3)
    crc = chr(crc >> 8) + chr(crc & 0xff)
    return crc

class PT_uint32:
    is_int = 1
    max_length = 5
    signed = 0
    def encode(self, out, v):
        if v >= 0xc000000 or v < -0x4000000: out.append((v>>28) & 0x7f | 0x80)
        if v >= 0x180000 or v < -0x80000:    out.append((v>>21) & 0x7f | 0x80)
        if v >= 0x3000 or v < -0x1000:       out.append((v>>14) & 0x7f | 0x80)
        if v >= 0x60 or v < -0x20:           out.append((v>>7)  & 0x7f | 0x80)
        out.append(v & 0x7f)
    def parse(self, s, pos):
        c = s[pos]
        pos += 1
        v = c & 0x7f
        if (c & 0x60) == 0x60:
            v |= -0x20
        while c & 0x80:
            c = s[pos]
            pos += 1
            v = (v<<7) | (c & 0x7f)
        if not self.signed:
            v = int(v & 0xffffffff)
        return v, pos

class PT_int32(PT_uint32):
    signed = 1
class PT_uint16(PT_uint32):
    max_length = 3
class PT_int16(PT_int32):
    signed = 1
    max_length = 3
class PT_byte(PT_uint32):
    max_length = 2

class PT_string:
    is_int = 0
    max_length = 64
    def encode(self, out, v):
        out.append(len(v))
        out.extend(bytearray(v))
    def parse(self, s, pos):
        l = s[pos]
        return str(bytearray(s[pos+1:pos+l+1])), pos+l+1
class PT_progmem_buffer(PT_string):
    pass
class PT_buffer(PT_string):
    pass

MessageTypes = {
    '%u': PT_uint32(), '%i': PT_int32(),
    '%hu': PT_uint16(), '%hi': PT_int16(),
    '%c': PT_byte(),
    '%s': PT_string(), '%.*s': PT_progmem_buffer(), '%*s': PT_buffer(),
}

# Update the message format to be compatible with python's % operator
def convert_msg_format(msgformat):
    mf = msgformat.replace('%c', '%u')
    mf = mf.replace('%.*s', '%s').replace('%*s', '%s')
    return mf

class MessageFormat:
    def __init__(self, msgid, msgformat):
        self.msgid = msgid
        self.msgformat = msgformat
        self.debugformat = convert_msg_format(msgformat)
        parts = msgformat.split()
        self.name = parts[0]
        argparts = [arg.split('=') for arg in parts[1:]]
        self.param_types = [MessageTypes[fmt] for name, fmt in argparts]
        self.param_names = [(name, MessageTypes[fmt]) for name, fmt in argparts]
        self.name_to_type = dict(self.param_names)
    def encode(self, params):
        out = []
        out.append(self.msgid)
        for i, t in enumerate(self.param_types):
            t.encode(out, params[i])
        return out
    def encode_by_name(self, **params):
        out = []
        out.append(self.msgid)
        for name, t in self.param_names:
            t.encode(out, params[name])
        return out
    def parse(self, s, pos):
        pos += 1
        out = {}
        for name, t in self.param_names:
            v, pos = t.parse(s, pos)
            out[name] = v
        return out, pos
    def format_params(self, params):
        out = []
        for name, t in self.param_names:
            v = params[name]
            if not t.is_int:
                v = repr(v)
            out.append(v)
        return self.debugformat % tuple(out)

class OutputFormat:
    name = '#output'
    def __init__(self, msgid, msgformat):
        self.msgid = msgid
        self.msgformat = msgformat
        self.debugformat = convert_msg_format(msgformat)
        self.param_types = []
        args = msgformat
        while 1:
            pos = args.find('%')
            if pos < 0:
                break
            if pos+1 >= len(args) or args[pos+1] != '%':
                for i in range(4):
                    t = MessageTypes.get(args[pos:pos+1+i])
                    if t is not None:
                        self.param_types.append(t)
                        break
                else:
                    raise error("Invalid output format for '%s'" % (msg,))
            args = args[pos+1:]
    def parse(self, s, pos):
        pos += 1
        out = []
        for t in self.param_types:
            v, pos = t.parse(s, pos)
            if not t.is_int:
                v = repr(v)
            out.append(v)
        outmsg = self.debugformat % tuple(out)
        return {'#msg': outmsg}, pos
    def format_params(self, params):
        return "#output %s" % (params['#msg'],)

class UnknownFormat:
    name = '#unknown'
    def parse(self, s, pos):
        msgid = s[pos]
        msg = str(bytearray(s))
        return {'#msgid': msgid, '#msg': msg}, len(s)-MESSAGE_TRAILER_SIZE
    def format_params(self, params):
        return "#unknown %s" % (repr(params['#msg']),)

class MessageParser:
    error = error
    def __init__(self):
        self.unknown = UnknownFormat()
        self.command_ids = []
        self.messages_by_id = {}
        self.messages_by_name = {}
        self.static_strings = {}
        self.config = {}
        self.version = self.build_versions = ""
        self.raw_identify_data = ""
        self._init_messages(DefaultMessages, DefaultMessages.keys())
    def check_packet(self, s):
        if len(s) < MESSAGE_MIN:
            return 0
        msglen = ord(s[MESSAGE_POS_LEN])
        if msglen < MESSAGE_MIN or msglen > MESSAGE_MAX:
            return -1
        msgseq = ord(s[MESSAGE_POS_SEQ])
        if (msgseq & ~MESSAGE_SEQ_MASK) != MESSAGE_DEST:
            return -1
        if len(s) < msglen:
            # Need more data
            return 0
        if s[msglen-MESSAGE_TRAILER_SYNC] != MESSAGE_SYNC:
            return -1
        msgcrc = s[msglen-MESSAGE_TRAILER_CRC:msglen-MESSAGE_TRAILER_CRC+2]
        crc = crc16_ccitt(s[:msglen-MESSAGE_TRAILER_SIZE])
        if crc != msgcrc:
            #logging.debug("got crc %s vs %s", repr(crc), repr(msgcrc))
            return -1
        return msglen
    def dump(self, s):
        msgseq = s[MESSAGE_POS_SEQ]
        out = ["seq: %02x" % (msgseq,)]
        pos = MESSAGE_HEADER_SIZE
        while 1:
            msgid = s[pos]
            mid = self.messages_by_id.get(msgid, self.unknown)
            params, pos = mid.parse(s, pos)
            out.append(mid.format_params(params))
            if pos >= len(s)-MESSAGE_TRAILER_SIZE:
                break
        return out
    def format_params(self, params):
        name = params.get('#name')
        mid = self.messages_by_name.get(name)
        if mid is not None:
            return mid.format_params(params)
        msg = params.get('#msg')
        if msg is not None:
            return "%s %s" % (name, msg)
        return str(params)
    def parse(self, s):
        msgid = s[MESSAGE_HEADER_SIZE]
        mid = self.messages_by_id.get(msgid, self.unknown)
        params, pos = mid.parse(s, MESSAGE_HEADER_SIZE)
        if pos != len(s)-MESSAGE_TRAILER_SIZE:
            raise error("Extra data at end of message")
        params['#name'] = mid.name
        static_string_id = params.get('static_string_id')
        if static_string_id is not None:
            params['#msg'] = self.static_strings.get(static_string_id, "?")
        return params
    def encode(self, seq, cmd):
        msglen = MESSAGE_MIN + len(cmd)
        seq = (seq & MESSAGE_SEQ_MASK) | MESSAGE_DEST
        out = [chr(msglen), chr(seq), cmd]
        out.append(crc16_ccitt(''.join(out)))
        out.append(MESSAGE_SYNC)
        return ''.join(out)
    def _parse_buffer(self, value):
        if not value:
            return []
        tval = int(value, 16)
        out = []
        for i in range(len(value) // 2):
            out.append(tval & 0xff)
            tval >>= 8
        out.reverse()
        return out
    def lookup_command(self, msgformat):
        parts = msgformat.strip().split()
        msgname = parts[0]
        mp = self.messages_by_name.get(msgname)
        if mp is None:
            raise error("Unknown command: %s" % (msgname,))
        if msgformat != mp.msgformat:
            raise error("Command format mismatch: %s vs %s" % (
                msgformat, mp.msgformat))
        return mp
    def create_command(self, msg):
        parts = msg.strip().split()
        if not parts:
            return ""
        msgname = parts[0]
        mp = self.messages_by_name.get(msgname)
        if mp is None:
            raise error("Unknown command: %s" % (msgname,))
        try:
            argparts = dict(arg.split('=', 1) for arg in parts[1:])
            for name, value in argparts.items():
                t = mp.name_to_type[name]
                if t.is_int:
                    tval = int(value, 0)
                else:
                    tval = self._parse_buffer(value)
                argparts[name] = tval
        except:
            #traceback.print_exc()
            raise error("Unable to extract params from: %s" % (msgname,))
        try:
            cmd = mp.encode_by_name(**argparts)
        except:
            #traceback.print_exc()
            raise error("Unable to encode: %s" % (msgname,))
        return cmd
    def _init_messages(self, messages, parsers):
        for msgid, msgformat in messages.items():
            msgid = int(msgid)
            if msgid not in parsers:
                self.messages_by_id[msgid] = OutputFormat(msgid, msgformat)
                continue
            msg = MessageFormat(msgid, msgformat)
            self.messages_by_id[msgid] = msg
            self.messages_by_name[msg.name] = msg
    def process_identify(self, data, decompress=True):
        try:
            if decompress:
                data = zlib.decompress(data)
            self.raw_identify_data = data
            data = json.loads(data)
            messages = data.get('messages')
            commands = data.get('commands')
            self.command_ids = commands
            responses = data.get('responses')
            self._init_messages(messages, commands+responses)
            static_strings = data.get('static_strings', {})
            self.static_strings = { int(k): v for k, v in static_strings.items() }
            self.config.update(data.get('config', {}))
            self.version = data.get('version', '')
            self.build_versions = data.get('build_versions', '')
        except error as e:
            raise
        except Exception as e:
            logging.exception("process_identify error")
            raise error("Error during identify: %s" % (str(e),))
    class sentinel: pass
    def get_constant(self, name, default=sentinel, parser=str):
        if name not in self.config:
            if default is not self.sentinel:
                return default
            raise error("Firmware constant '%s' not found" % (name,))
        try:
            value = parser(self.config[name])
        except:
            raise error("Unable to parse firmware constant %s: %s" % (
                name, self.config[name]))
        return value
    def get_constant_float(self, name, default=sentinel):
        return self.get_constant(name, default, parser=float)
    def get_constant_int(self, name, default=sentinel):
        return self.get_constant(name, default, parser=int)
