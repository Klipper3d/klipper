#!/usr/bin/env python2
# Script to extract config and shutdown information file a klippy.log file
#
# Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, re, collections

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
        f = open(self.filename, 'wb')
        f.write('\n'.join(self.comments + self.config_lines))
        f.close()


######################################################################
# Shutdown extraction
######################################################################

stats_seq_s = r" send_seq=(?P<sseq>[0-9]+) receive_seq=(?P<rseq>[0-9]+) "
count_s = r"(?P<count>[0-9]+)"
time_s = r"(?P<time>[0-9]+[.][0-9]+)"
esttime_s = r"(?P<esttime>[0-9]+[.][0-9]+)"
shortseq_s = r"(?P<shortseq>[0-9a-f])"
stats_r = re.compile(r"^Stats " + time_s + ": .*" + stats_seq_s)
serial_dump_r = re.compile(r"^Dumping serial stats: .*" + stats_seq_s)
send_dump_r = re.compile(r"^Dumping send queue " + count_s + " messages$")
sent_r = re.compile(r"^Sent " + count_s + " " + esttime_s + " " + time_s
                    + " [0-9]+: seq: 1" + shortseq_s + ",")
receive_dump_r = re.compile(r"^Dumping receive queue " + count_s + " messages$")
receive_r = re.compile(r"^Receive: " + count_s + " " + time_s + " " + esttime_s
                    + " [0-9]+: seq: 1" + shortseq_s + ",")
gcode_r = re.compile(r"^Read " + time_s + ": ['\"]")
clock_r = re.compile(r"^clocksync state: .* clock_est=\((?P<st>[^ ]+)"
                     + r" (?P<sc>[0-9]+) (?P<f>[^ ]+)\)")
repl_seq_r = re.compile(r": seq: 1" + shortseq_s)
repl_clock_r = re.compile(r"clock=(?P<clock>[0-9]+) ")

def add_high_bits(val, ref, mask):
    half = (mask + 1) // 2
    return ref + ((val - (ref & mask) + half) & mask) - half

class GatherShutdown:
    def __init__(self, configs, line_num, recent_lines, logname):
        self.shutdown_line_num = line_num
        self.filename = "%s.shutdown%05d" % (logname, line_num)
        configs_by_id = {c.config_num: c for c in configs.values()}
        self.config = configs_by_id[max(configs_by_id.keys())]
        self.comments = ["# config %s" % (self.config.filename,)]
        self.config.add_comment(format_comment(line_num, recent_lines[-1][1]))
        self.stats_stream = []
        self.gcode_stream = []
        self.sent_stream = []
        self.receive_stream = []
        self.sent_time_to_seq = {}
        self.sent_seq_to_time = {}
        self.receive_seq_to_time = {}
        self.clock_est = 0., 0., 1.
        self.shutdown_seq = None
        self.first_stat_time = self.last_stat_time = None
        self.send_count = self.receive_count = 0
        for line_num, line in recent_lines:
            self.parse_line(line_num, line)
        self.first_stat_time = self.last_stat_time
    def add_comment(self, comment):
        if comment is not None:
            self.comments.append(comment)
    def trans_clock(self, clock, ts):
        sample_time, sample_clock, freq = self.clock_est
        exp_clock = int(sample_clock + (ts - sample_time) * freq)
        ext_clock = add_high_bits(clock, exp_clock, 0xffffffff)
        return sample_time + (ext_clock - sample_clock) / freq
    def annotate(self, line, seq, ts):
        if seq is not None:
            line = repl_seq_r.sub(r"\g<0>(%d)" % (seq,), line)
        def clock_update(m):
            return m.group(0)[:-1] + "(%.6f) " % (
                self.trans_clock(int(m.group('clock')), ts),)
        line = repl_clock_r.sub(clock_update, line)
        return line
    def add_line(self, line_num, line):
        self.parse_line(line_num, line)
        if (self.first_stat_time is not None
            and self.last_stat_time > self.first_stat_time + 5.):
            self.finalize()
            return False
        if (line.startswith('Git version') or line.startswith('Start printer at')
            or line == '===== Config file ====='):
            self.finalize()
            return False
        return True
    def parse_line(self, line_num, line):
        m = sent_r.match(line)
        if m is not None:
            shortseq = int(m.group('shortseq'), 16)
            seq = self.shutdown_seq + int(m.group('count')) - self.send_count
            seq = add_high_bits(shortseq, seq, 0xf)
            ts = float(m.group('time'))
            esttime = float(m.group('esttime'))
            self.sent_time_to_seq[(esttime, seq & 0xf)] = seq
            self.sent_seq_to_time[seq] = ts
            line = self.annotate(line, seq, ts)
            self.sent_stream.append((ts, line_num, line))
            return
        m = receive_r.match(line)
        if m is not None:
            shortseq = int(m.group('shortseq'), 16)
            ts = float(m.group('time'))
            esttime = float(m.group('esttime'))
            seq = self.sent_time_to_seq.get((esttime, (shortseq - 1) & 0xf))
            if seq is not None:
                self.receive_seq_to_time[seq + 1] = ts
            line = self.annotate(line, seq, ts)
            self.receive_stream.append((ts, line_num, line))
            return
        m = gcode_r.match(line)
        if m is not None:
            ts = float(m.group('time'))
            self.gcode_stream.append((ts, line_num, line))
            return
        m = stats_r.match(line)
        if m is not None:
            ts = float(m.group('time'))
            sseq = int(m.group('sseq'))
            rseq = int(m.group('rseq'))
            self.last_stat_time = ts
            if self.first_stat_time is None:
                self.first_stat_time = ts
            self.stats_stream.append((ts, sseq, rseq, line_num, line))
            return
        m = clock_r.match(line)
        if m is not None:
            st = float(m.group('st'))
            sc = int(m.group('sc'))
            f = float(m.group('f'))
            self.clock_est = (st, sc, f)
        m = serial_dump_r.match(line)
        if m is not None:
            self.shutdown_seq = int(m.group('rseq'))
        m = send_dump_r.match(line)
        if m is not None:
            self.send_count = int(m.group('count'))
        m = receive_dump_r.match(line)
        if m is not None:
            self.receive_count = int(m.group('count'))
        self.stats_stream.append((None, None, None, line_num, line))
    def finalize(self):
        # Ignore old stats
        stream_ts = [i[0] for i in self.sent_stream + self.receive_stream]
        if not stream_ts:
            return
        min_stream_ts = min(stream_ts)
        max_stream_ts = max(stream_ts)
        for i, info in enumerate(self.stats_stream):
            if info[0] is not None and info[0] >= min_stream_ts - 5.:
                del self.stats_stream[:i]
                break
        # Improve accuracy of stats timestamps
        last_ts = self.stats_stream[0][0]
        out = []
        for i in range(len(self.stats_stream)):
            ts, sseq, rseq, line_num, line = self.stats_stream[i]
            if ts is not None:
                min_ts = max(self.sent_seq_to_time.get(sseq-1, 0),
                             self.receive_seq_to_time.get(rseq, 0))
                max_ts = min(self.sent_seq_to_time.get(sseq, 999999999999),
                             self.receive_seq_to_time.get(rseq+1, 999999999999))
                last_ts = min(max(ts, min_ts + 0.00000001),
                              max_ts - 0.00000001)
            elif line_num >= self.shutdown_line_num and last_ts <= max_stream_ts:
                last_ts = max_stream_ts + 0.00000001
            out.append((last_ts, line_num, line))
        # Make sure no timestamp goes backwards
        for s in [self.sent_stream, self.receive_stream, self.gcode_stream]:
            for i in range(1, len(s)):
                if s[i-1][0] > s[i][0]:
                    s[i] = (s[i-1][0], s[i][1], s[i][2])
        # Produce output sorted by timestamp
        out = out + self.sent_stream + self.receive_stream + self.gcode_stream
        out.sort()
        out = [i[2] for i in out]
        f = open(self.filename, 'wb')
        f.write('\n'.join(self.comments + out))
        f.close()


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
    f = open(logname, 'rb')
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
            handler = GatherConfig(configs, line_num, recent_lines, logname)
            handler.add_comment(last_git)
            handler.add_comment(last_start)
        elif 'shutdown: ' in line or line.startswith('Dumping '):
            handler = GatherShutdown(configs, line_num, recent_lines, logname)
            handler.add_comment(last_git)
            handler.add_comment(last_start)
    if handler is not None:
        handler.finalize()
    # Write found config files
    for cfg in configs.values():
        cfg.write_file()

if __name__ == '__main__':
    main()
