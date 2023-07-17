#!/usr/bin/env python3
# Script to extract config and shutdown information file a klippy.log file
#
# Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, re, collections, ast, itertools

def format_comment(line_num, line):
    return "# %6d: %s" % (line_num, line)


######################################################################
# Config file extraction
######################################################################

class GatherConfig:
    def __init__(self, configs, line_num, recent_lines, logname):
        self.configs = configs
        self.line_num = line_num
        self.config_num = len(configs) + 1
        self.filename = "%s.config%04d.cfg" % (logname, self.config_num)
        self.config_lines = []
        self.comments = []
    def add_line(self, line_num, line):
        if line != '=======================':
            self.config_lines.append(line)
            return True
        self.finalize()
        return False
    def finalize(self):
        lines = tuple(self.config_lines)
        ch = self.configs.get(lines)
        if ch is None:
            self.configs[lines] = ch = self
        else:
            ch.comments.extend(self.comments)
        ch.comments.append(format_comment(self.line_num, "config file"))
    def add_comment(self, comment):
        if comment is not None:
            self.comments.append(comment)
    def write_file(self):
        lines = itertools.chain(self.comments, self.config_lines)
        lines = ('%s\n' % l for l in lines)
        with open(self.filename, 'wt') as f:
            f.writelines(lines)


######################################################################
# TMC UART message parsing
######################################################################

uart_r = re.compile(r"tmcuart_(?:send|response) oid=[0-9]+ (?:read|write)=")

class TMCUartHelper:
    def _calc_crc8(self, data):
        # Generate a CRC8-ATM value for a bytearray
        crc = 0
        for b in data:
            for i in range(8):
                if (crc >> 7) ^ (b & 0x01):
                    crc = (crc << 1) ^ 0x07
                else:
                    crc = (crc << 1)
                crc &= 0xff
                b >>= 1
        return crc
    def _add_serial_bits(self, data):
        # Add serial start and stop bits to a message in a bytearray
        out = 0
        pos = 0
        for d in data:
            b = (d << 1) | 0x200
            out |= (b << pos)
            pos += 10
        res = bytearray()
        for i in range((pos+7)//8):
            res.append((out >> (i*8)) & 0xff)
        return res
    def _encode_read(self, sync, addr, reg):
        # Generate a uart read register message
        msg = bytearray([sync, addr, reg])
        msg.append(self._calc_crc8(msg))
        return self._add_serial_bits(msg)
    def _encode_write(self, sync, addr, reg, val):
        # Generate a uart write register message
        msg = bytearray([sync, addr, reg, (val >> 24) & 0xff,
                         (val >> 16) & 0xff, (val >> 8) & 0xff, val & 0xff])
        msg.append(self._calc_crc8(msg))
        return self._add_serial_bits(msg)
    def _decode_read(self, data):
        # Extract a uart read request message
        if len(data) != 5:
            return
        # Convert data into a long integer for easy manipulation
        mval = pos = 0
        for d in data:
            mval |= d << pos
            pos += 8
        # Extract register value
        addr = (mval >> 11) & 0xff
        reg = (mval >> 21) & 0xff
        # Verify start/stop bits and crc
        encoded_data = self._encode_read(0xf5, addr, reg)
        if data != encoded_data:
            return "Invalid: %s" % (self.pretty_print(addr, reg),)
        return self.pretty_print(addr, reg)
    def _decode_reg(self, data):
        # Extract a uart read response message
        if len(data) != 10:
            return
        # Convert data into a long integer for easy manipulation
        mval = pos = 0
        for d in data:
            mval |= d << pos
            pos += 8
        # Extract register value
        addr = (mval >> 11) & 0xff
        reg = (mval >> 21) & 0xff
        val = ((((mval >> 31) & 0xff) << 24) | (((mval >> 41) & 0xff) << 16)
               | (((mval >> 51) & 0xff) << 8) | ((mval >> 61) & 0xff))
        sync = 0xf5
        if addr == 0xff:
            sync = 0x05
        # Verify start/stop bits and crc
        encoded_data = self._encode_write(sync, addr, reg, val)
        if data != encoded_data:
            #print("Got %s vs %s" % (repr(data), repr(encoded_data)))
            return "Invalid:%s" % (self.pretty_print(addr, reg, val),)
        return self.pretty_print(addr, reg, val)
    def pretty_print(self, addr, reg, val=None):
        if val is None:
            return "(%x@%x)" % (reg, addr)
        if reg & 0x80:
            return "(%x@%x=%08x)" % (reg & ~0x80, addr, val)
        return "(%x@%x==%08x)" % (reg, addr, val)
    def parse_msg(self, msg):
        data = bytearray(msg)
        if len(data) == 10:
            return self._decode_reg(data)
        elif len(data) == 5:
            return self._decode_read(data)
        elif len(data) == 0:
            return ""
        return "(length?)"


######################################################################
# Shutdown extraction
######################################################################

def add_high_bits(val, ref, mask):
    half = (mask + 1) // 2
    return ref + ((val - (ref & mask) + half) & mask) - half

count_s = r"(?P<count>[0-9]+)"
time_s = r"(?P<time>[0-9]+[.][0-9]+)"
esttime_s = r"(?P<esttime>[0-9]+[.][0-9]+)"
shortseq_s = r"(?P<shortseq>[0-9a-f])"
sent_r = re.compile(r"^Sent " + count_s + " " + esttime_s + " " + time_s
                    + " [0-9]+: seq: 1" + shortseq_s + ",")

# MCU "Sent" shutdown message parsing
class MCUSentStream:
    def __init__(self, mcu, count):
        self.mcu = mcu
        self.sent_stream = []
        self.send_count = count
    def parse_line(self, line_num, line):
        m = sent_r.match(line)
        if m is not None:
            shortseq = int(m.group('shortseq'), 16)
            seq = (self.mcu.shutdown_seq + int(m.group('count'))
                   - self.send_count)
            seq = add_high_bits(shortseq, seq, 0xf)
            ts = float(m.group('time'))
            esttime = float(m.group('esttime'))
            self.mcu.sent_time_to_seq[(esttime, seq & 0xf)] = seq
            self.mcu.sent_seq_to_time[seq] = ts
            line = self.mcu.annotate(line, seq, ts)
            self.sent_stream.append((ts, line_num, line))
            return True, None
        return self.mcu.parse_line(line_num, line)
    def get_lines(self):
        return self.sent_stream

receive_r = re.compile(r"^Receive: " + count_s + " " + time_s + " " + esttime_s
                       + " [0-9]+: seq: 1" + shortseq_s + ",")

# MCU "Receive" shutdown message parsing
class MCUReceiveStream:
    def __init__(self, mcu):
        self.mcu = mcu
        self.receive_stream = []
    def parse_line(self, line_num, line):
        m = receive_r.match(line)
        if m is not None:
            shortseq = int(m.group('shortseq'), 16)
            ts = float(m.group('time'))
            esttime = float(m.group('esttime'))
            seq = self.mcu.sent_time_to_seq.get((esttime, (shortseq - 1) & 0xf))
            if seq is not None:
                self.mcu.receive_seq_to_time[seq + 1] = ts
            line = self.mcu.annotate(line, seq, ts)
            self.receive_stream.append((ts, line_num, line))
            return True, None
        return self.mcu.parse_line(line_num, line)
    def get_lines(self):
        return self.receive_stream

stats_seq_s = r" send_seq=(?P<sseq>[0-9]+) receive_seq=(?P<rseq>[0-9]+) "
serial_dump_r = re.compile(r"^Dumping serial stats: .*" + stats_seq_s)
send_dump_r = re.compile(r"^Dumping send queue " + count_s + " messages$")
receive_dump_r = re.compile(r"^Dumping receive queue " + count_s + " messages$")
clock_r = re.compile(r"^clocksync state: mcu_freq=(?P<freq>[0-9]+) .*"
                     + r" clock_est=\((?P<st>[^ ]+)"
                     + r" (?P<sc>[0-9]+) (?P<f>[^ ]+)\)")
repl_seq_r = re.compile(r": seq: 1" + shortseq_s)
clock_s = r"(?P<clock>[0-9]+)"
repl_clock_r = re.compile(r"clock=" + clock_s)
repl_uart_r = re.compile(r"tmcuart_(?:response|send) oid=[0-9]+"
                         + r" (?:read|write)=b?(?P<msg>(?:'[^']*'"
                         + r'|"[^"]*"))')

# MCU shutdown message parsing
class MCUStream:
    def __init__(self, name):
        self.name = name
        self.sent_time_to_seq = {}
        self.sent_seq_to_time = {}
        self.receive_seq_to_time = {}
        self.mcu_freq = 1
        self.clock_est = (0., 0., 1.)
        self.shutdown_seq = None
    def trans_clock(self, clock, ts):
        sample_time, sample_clock, freq = self.clock_est
        exp_clock = int(sample_clock + (ts - sample_time) * freq)
        ext_clock = add_high_bits(clock, exp_clock, 0xffffffff)
        return sample_time + (ext_clock - sample_clock) / freq
    def annotate(self, line, seq, ts):
        if seq is not None:
            line = repl_seq_r.sub(r"\g<0>(%d)" % (seq,), line)
        def clock_update(m):
            return m.group(0).rstrip() + "(%.6f)" % (
                self.trans_clock(int(m.group('clock')), ts),)
        line = repl_clock_r.sub(clock_update, line)
        def uart_update(m):
            msg = ast.literal_eval('b' + m.group('msg'))
            msg = TMCUartHelper().parse_msg(msg)
            return m.group(0).rstrip() + msg
        line = repl_uart_r.sub(uart_update, line)
        if self.name != 'mcu':
            line = "mcu '%s': %s" % (self.name, line)
        return line
    def parse_line(self, line_num, line):
        m = clock_r.match(line)
        if m is not None:
            self.mcu_freq = int(m.group('freq'))
            st = float(m.group('st'))
            sc = int(m.group('sc'))
            f = float(m.group('f'))
            self.clock_est = (st, sc, f)
        m = serial_dump_r.match(line)
        if m is not None:
            self.shutdown_seq = int(m.group('rseq'))
        m = send_dump_r.match(line)
        if m is not None:
            return True, MCUSentStream(self, int(m.group('count')))
        m = receive_dump_r.match(line)
        if m is not None:
            return True, MCUReceiveStream(self)
        return False, None
    def get_lines(self):
        return []

stepper_move_r = re.compile(r"^queue_step " + count_s + r": t=" + clock_s
                            + r" ")

# Kinematic "trapq" shutdown message parsing
class StepperStream:
    def __init__(self, name, mcu_name, mcus):
        self.name = name
        self.stepper_stream = []
        self.clock_est = (0., 0., 1.)
        mcu = mcus.get(mcu_name)
        if mcu is not None:
            self.clock_est = mcu.clock_est
    def parse_line(self, line_num, line):
        m = stepper_move_r.match(line)
        if m is not None:
            # Convert clock to systime
            clock = int(m.group('clock'))
            sample_time, sample_clock, freq = self.clock_est
            ts = sample_time + (clock - sample_clock) / freq
            # Add systime to log
            parts = line.split(' ', 4)
            parts[0] = "%s queue_step" % (self.name,)
            parts[2] += '(%.6f)' % (ts,)
            self.stepper_stream.append((ts, line_num, ' '.join(parts)))
            return True, None
        return False, None
    def get_lines(self):
        return self.stepper_stream

trapq_move_r = re.compile(r"^move " + count_s + r": pt=" + time_s)

# Kinematic "trapq" shutdown message parsing
class TrapQStream:
    def __init__(self, name, mcus):
        self.name = name
        self.trapq_stream = []
        self.mcu_freq = 1
        self.clock_est = (0., 0., 1.)
        mcu = mcus.get("mcu")
        if mcu is not None:
            self.mcu_freq = mcu.mcu_freq
            self.clock_est = mcu.clock_est
    def parse_line(self, line_num, line):
        m = trapq_move_r.match(line)
        if m is not None:
            # Convert print_time to systime
            pt = float(m.group('time'))
            clock = pt * self.mcu_freq
            sample_time, sample_clock, freq = self.clock_est
            ts = sample_time + (clock - sample_clock) / freq
            # Add systime to log
            parts = line.split(' ', 4)
            parts[0] = "%s move" % (self.name,)
            parts[2] += '(%.6f)' % (ts,)
            self.trapq_stream.append((ts, line_num, ' '.join(parts)))
            return True, None
        return False, None
    def get_lines(self):
        return self.trapq_stream

gcode_cmd_r = re.compile(r"^Read " + time_s + r": (?P<gcode>['\"].*)$")
varlist_split_r = re.compile(r"([^ ]+)=")

# G-Code shutdown message parsing
class GCodeStream:
    def __init__(self, shutdown_line_num, logname):
        self.gcode_stream = []
        self.gcode_commands = []
        self.gcode_state = ''
        self.gcode_filename = "%s.gcode%05d" % (logname, shutdown_line_num)
    def extract_params(self, line):
        parts = varlist_split_r.split(line)
        try:
            return { parts[i]: ast.literal_eval(parts[i+1].strip())
                     for i in range(1, len(parts), 2) }
        except:
            return {}
    def handle_gcode_state(self, line):
        kv = self.extract_params(line)
        out = ['; Start g-code state restore', 'G28']
        if not kv.get('absolute_coord', kv.get('absolutecoord')):
            out.append('G91')
        if not kv.get('absolute_extrude', kv.get('absoluteextrude')):
            out.append('M83')
        lp = kv['last_position']
        out.append('G1 X%f Y%f Z%f F%f' % (
            lp[0], lp[1], lp[2], kv['speed'] * 60.))
        bp = kv['base_position']
        if bp[:3] != [0., 0., 0.]:
            out.append('; Must manually set base position...')
        out.append('G92 E%f' % (lp[3] - bp[3],))
        hp = kv['homing_position']
        if hp != [0., 0., 0., 0.]:
            out.append('; Must manually set homing position...')
        if abs(kv['speed_factor'] - 1. / 60.) > .000001:
            out.append('M220 S%f' % (kv['speed_factor'] * 60. * 100.,))
        if kv['extrude_factor'] != 1.:
            out.append('M221 S%f' % (kv['extrude_factor'] * 100.,))
        out.extend(['; End of state restore', '', ''])
        self.gcode_state = '\n'.join(out)
    def parse_line(self, line_num, line):
        m = gcode_cmd_r.match(line)
        if m is not None:
            ts = float(m.group('time'))
            self.gcode_stream.append((ts, line_num, line))
            self.gcode_commands.append(m.group('gcode'))
            return True, None
        return False, None
    def get_lines(self):
        # Produce output gcode stream
        if self.gcode_stream:
            data = (ast.literal_eval(gc) for gc in self.gcode_commands)
            with open(self.gcode_filename, 'wt') as f:
                f.write(self.gcode_state + ''.join(data))
        return self.gcode_stream

api_cmd_r = re.compile(r"^Received " + time_s + r": \{.*\}$")

# API server shutdowm message parsing
class APIStream:
    def __init__(self):
        self.api_stream = []
    def parse_line(self, line_num, line):
        m = api_cmd_r.match(line)
        if m is not None:
            ts = float(m.group('time'))
            self.api_stream.append((ts, line_num, line))
            return True, None
        return False, None
    def get_lines(self):
        return self.api_stream

stats_r = re.compile(r"^Stats " + time_s + ": ")
mcu_r = re.compile(r"MCU '(?P<mcu>[^']+)' (is_)?shutdown: (?P<reason>.*)$")
stepper_r = re.compile(r"^Dumping stepper '(?P<name>[^']*)' \((?P<mcu>[^)]+)\) "
                       + count_s + r" queue_step:$")
trapq_r = re.compile(r"^Dumping trapq '(?P<name>[^']*)' " + count_s
                     + r" moves:$")
gcode_r = re.compile(r"Dumping gcode input " + count_s + r" blocks$")
gcode_state_r = re.compile(r"^gcode state: ")
api_r = re.compile(r"Dumping " + count_s + r" requests for client "
                   + r"(?P<client>[0-9]+)" + r"$")

# Stats message parsing and high-level message dispatch
class StatsStream:
    def __init__(self, shutdown_line_num, logname):
        self.shutdown_line_num = shutdown_line_num
        self.gcode_stream = GCodeStream(shutdown_line_num, logname)
        self.mcus = {}
        self.first_stat_time = self.last_stat_time = None
        self.stats_stream = []
    def reset_first_stat_time(self):
        self.first_stat_time = self.last_stat_time
    def get_stat_times(self):
        return self.first_stat_time, self.last_stat_time
    def check_stats_seq(self, ts, line):
        # Parse stats
        parts = line.split()
        mcu = ""
        keyparts = {}
        for p in parts[2:]:
            if '=' not in p:
                mcu = p
                continue
            name, val = p.split('=', 1)
            keyparts[mcu + name] = val
        min_ts = 0
        max_ts = 999999999999
        for mcu_name, mcu in self.mcus.items():
            sname = '%s:send_seq' % (mcu_name,)
            rname = '%s:receive_seq' % (mcu_name,)
            if sname not in keyparts:
                continue
            sseq = int(keyparts[sname])
            rseq = int(keyparts[rname])
            min_ts = max(min_ts, mcu.sent_seq_to_time.get(sseq-1, 0),
                         mcu.receive_seq_to_time.get(rseq, 0))
            max_ts = min(max_ts, mcu.sent_seq_to_time.get(sseq, 999999999999),
                         mcu.receive_seq_to_time.get(rseq+1, 999999999999))
        return min(max(ts, min_ts + 0.00000001), max_ts - 0.00000001)
    def parse_line(self, line_num, line):
        m = stats_r.match(line)
        if m is not None:
            ts = float(m.group('time'))
            self.last_stat_time = ts
            if self.first_stat_time is None:
                self.first_stat_time = ts
            self.stats_stream.append((ts, line_num, line))
            return True, None
        self.stats_stream.append((None, line_num, line))
        m = mcu_r.match(line)
        if m is not None:
            mcu_name = m.group('mcu')
            mcu_stream = MCUStream(mcu_name)
            self.mcus[mcu_name] = mcu_stream
            return True, mcu_stream
        m = stepper_r.match(line)
        if m is not None:
            return True, StepperStream(m.group('name'), m.group('mcu'),
                                       self.mcus)
        m = trapq_r.match(line)
        if m is not None:
            return True, TrapQStream(m.group('name'), self.mcus)
        m = gcode_r.match(line)
        if m is not None:
            return True, self.gcode_stream
        m = gcode_state_r.match(line)
        if m is not None:
            self.gcode_stream.handle_gcode_state(line)
            return True, None
        m = api_r.match(line)
        if m is not None:
            return True, APIStream()
        return False, None
    def get_lines(self):
        # Ignore old stats
        all_ts = []
        for mcu_name, mcu in self.mcus.items():
            all_ts.extend(mcu.sent_seq_to_time.values())
            all_ts.extend(mcu.receive_seq_to_time.values())
        if not all_ts:
            return []
        min_stream_ts = min(all_ts)
        max_stream_ts = max(all_ts)
        for i, info in enumerate(self.stats_stream):
            if info[0] is not None and info[0] >= min_stream_ts - 5.:
                del self.stats_stream[:i]
                break
        # Find the first stats timestamp
        last_ts = min_stream_ts
        for ts, line_num, line in self.stats_stream:
            if ts is not None:
                last_ts = ts
                break
        # Improve accuracy of stats timestamps
        for i, (ts, line_num, line) in enumerate(self.stats_stream):
            if ts is not None:
                last_ts = self.check_stats_seq(ts, line)
            elif (line_num >= self.shutdown_line_num
                  and last_ts <= max_stream_ts):
                last_ts = max_stream_ts + 0.00000001
            self.stats_stream[i] = (last_ts, line_num, line)
        return self.stats_stream

# Main handler for creating shutdown diagnostics file
class GatherShutdown:
    def __init__(self, configs, line_num, recent_lines, logname):
        self.filename = "%s.shutdown%05d" % (logname, line_num)
        self.comments = []
        if configs:
            configs_by_id = {c.config_num: c for c in configs.values()}
            config = configs_by_id[max(configs_by_id.keys())]
            config.add_comment(format_comment(line_num, recent_lines[-1][1]))
            self.comments.append("# config %s" % (config.filename,))
        self.stats_stream = StatsStream(line_num, logname)
        self.active_streams = [self.stats_stream]
        self.all_streams = list(self.active_streams)
        for line_num, line in recent_lines:
            self.parse_line(line_num, line)
        self.stats_stream.reset_first_stat_time()
    def add_comment(self, comment):
        if comment is not None:
            self.comments.append(comment)
    def add_line(self, line_num, line):
        self.parse_line(line_num, line)
        first, last = self.stats_stream.get_stat_times()
        if first is not None and last > first + 5.:
            self.finalize()
            return False
        if (line.startswith('Git version')
            or line.startswith('Start printer at')
            or line == '===== Config file ====='):
            self.finalize()
            return False
        return True
    def parse_line(self, line_num, line):
        for s in self.active_streams:
            did_parse, new_stream = s.parse_line(line_num, line)
            if did_parse:
                if new_stream is not None:
                    self.all_streams.append(new_stream)
                    self.active_streams = [new_stream, self.stats_stream]
                break
    def finalize(self):
        # Make sure no timestamp goes backwards
        streams = [p.get_lines() for p in self.all_streams]
        for s in streams:
            for i in range(1, len(s)):
                if s[i-1][0] > s[i][0]:
                    s[i] = (s[i-1][0], s[i][1], s[i][2])
        # Produce output sorted by timestamp
        out = [i for s in streams for i in s]
        out.sort()
        lines = itertools.chain(self.comments, (i[2] for i in out))
        lines = ('%s\n' % l for l in lines)
        with open(self.filename, 'wt') as f:
            f.writelines(lines)


######################################################################
# Startup
######################################################################

def main():
    logname = sys.argv[1]
    last_git = last_start = None
    configs = {}
    handler = None
    recent_lines = collections.deque([], 200)
    # Parse log file
    with open(logname, 'rt') as f:
        for line_num, line in enumerate(f):
            line = line.rstrip()
            line_num += 1
            recent_lines.append((line_num, line))
            if handler is not None:
                ret = handler.add_line(line_num, line)
                if ret:
                    continue
                recent_lines.clear()
                handler = None
            if line.startswith('Git version'):
                last_git = format_comment(line_num, line)
            elif line.startswith('Start printer at'):
                last_start = format_comment(line_num, line)
            elif line == '===== Config file =====':
                handler = GatherConfig(configs, line_num,
                                       recent_lines, logname)
                handler.add_comment(last_git)
                handler.add_comment(last_start)
            elif 'shutdown: ' in line or line.startswith('Dumping '):
                handler = GatherShutdown(configs, line_num,
                                         recent_lines, logname)
                handler.add_comment(last_git)
                handler.add_comment(last_start)
    if handler is not None:
        handler.finalize()
    # Write found config files
    for cfg in configs.values():
        cfg.write_file()

if __name__ == '__main__':
    main()
