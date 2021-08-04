# Code for reading data logs produced by data_logger.py
#
# Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import json, zlib

class error(Exception):
    pass


######################################################################
# Log data handlers
######################################################################

# Log data handlers: {name: class, ...}
LogHandlers = {}

# Extract requested position, velocity, and accel from a trapq log
class HandleTrapQ:
    ParametersSubscriptionId = 2
    ParametersMin = ParametersMax = 3
    DataSets = [
        ('trapq:<name>:velocity', 'Requested velocity for the given trapq'),
        ('trapq:<name>:<axis>', 'Requested axis (x, y, or z) position'),
        ('trapq:<name>:<axis>_velocity', 'Requested axis velocity'),
        ('trapq:<name>:<axis>_accel', 'Requested axis acceleration'),
    ]
    def __init__(self, lmanager, name):
        self.name = name
        self.jdispatch = lmanager.get_jdispatch()
        self.cur_data = [(0., 0., 0., 0., (0., 0., 0.), (0., 0., 0.))]
        self.data_pos = 0
        tq, trapq_name, datasel = name.split(':')
        ptypes = {}
        ptypes['velocity'] = {
            'label': '%s velocity' % (trapq_name,),
            'units': 'Velocity\n(mm/s)', 'func': self._pull_velocity
        }
        ptypes['accel'] = {
            'label': '%s acceleration' % (trapq_name,),
            'units': 'Acceleration\n(mm/s^2)', 'func': self._pull_accel
        }
        for axis, name in enumerate("xyz"):
            ptypes['%s' % (name,)] = {
                'label': '%s %s position' % (trapq_name, name), 'axis': axis,
                'units': 'Position\n(mm)', 'func': self._pull_axis_position
            }
            ptypes['%s_velocity' % (name,)] = {
                'label': '%s %s velocity' % (trapq_name, name), 'axis': axis,
                'units': 'Velocity\n(mm/s)', 'func': self._pull_axis_velocity
            }
            ptypes['%s_accel' % (name,)] = {
                'label': '%s %s acceleration' % (trapq_name, name),
                'axis': axis, 'units': 'Acceleration\n(mm/s^2)',
                'func': self._pull_axis_accel
            }
        pinfo = ptypes.get(datasel)
        if pinfo is None:
            raise error("Unknown trapq data selection '%s'" % (datasel,))
        self.label = {'label': pinfo['label'], 'units': pinfo['units']}
        self.axis = pinfo.get('axis')
        self.pull_data = pinfo['func']
    def get_label(self):
        return self.label
    def _find_move(self, req_time):
        data_pos = self.data_pos
        while 1:
            move = self.cur_data[data_pos]
            print_time, move_t, start_v, accel, start_pos, axes_r = move
            if req_time <= print_time + move_t:
                return move, req_time >= print_time
            data_pos += 1
            if data_pos < len(self.cur_data):
                self.data_pos = data_pos
                continue
            jmsg = self.jdispatch.pull_msg(req_time, self.name)
            if jmsg is None:
                return move, False
            self.cur_data = jmsg['data']
            self.data_pos = data_pos = 0
    def _pull_axis_position(self, req_time):
        move, in_range = self._find_move(req_time)
        print_time, move_t, start_v, accel, start_pos, axes_r = move
        mtime = max(0., min(move_t, req_time - print_time))
        dist = (start_v + .5 * accel * mtime) * mtime;
        return start_pos[self.axis] + axes_r[self.axis] * dist
    def _pull_axis_velocity(self, req_time):
        move, in_range = self._find_move(req_time)
        if not in_range:
            return 0.
        print_time, move_t, start_v, accel, start_pos, axes_r = move
        return (start_v + accel * (req_time - print_time)) * axes_r[self.axis]
    def _pull_axis_accel(self, req_time):
        move, in_range = self._find_move(req_time)
        if not in_range:
            return 0.
        print_time, move_t, start_v, accel, start_pos, axes_r = move
        return accel * axes_r[self.axis]
    def _pull_velocity(self, req_time):
        move, in_range = self._find_move(req_time)
        if not in_range:
            return 0.
        print_time, move_t, start_v, accel, start_pos, axes_r = move
        return start_v + accel * (req_time - print_time)
    def _pull_accel(self, req_time):
        move, in_range = self._find_move(req_time)
        if not in_range:
            return 0.
        print_time, move_t, start_v, accel, start_pos, axes_r = move
        return accel
LogHandlers["trapq"] = HandleTrapQ

# Extract positions from queue_step log
class HandleStepQ:
    ParametersSubscriptionId = 2
    ParametersMin = 2
    ParametersMax = 3
    DataSets = [
        ('stepq:<stepper>', 'Commanded position of the given stepper'),
        ('stepq:<stepper>:raw', 'Commanded position without smoothing'),
    ]
    def __init__(self, lmanager, name):
        self.name = name
        self.jdispatch = lmanager.get_jdispatch()
        self.step_data = [(0., 0., 0.), (0., 0., 0.)] # [(time, half_pos, pos)]
        self.data_pos = 0
        self.smooth_time = 0.010
        name_parts = name.split(':')
        if len(name_parts) == 3:
            if name_parts[2] != 'raw':
                raise error("Unknown stepq data selection '%s'" % (name,))
            self.smooth_time = 0.
    def get_label(self):
        label = '%s position' % (self.name.split(':')[1],)
        return {'label': label, 'units': 'Position\n(mm)'}
    def pull_data(self, req_time):
        smooth_time = self.smooth_time
        while 1:
            data_pos = self.data_pos
            step_data = self.step_data
            # Find steps before and after req_time
            next_time, next_halfpos, next_pos = step_data[data_pos + 1]
            if req_time >= next_time:
                if data_pos + 2 < len(step_data):
                    self.data_pos = data_pos + 1
                    continue
                self._pull_block(req_time)
                continue
            last_time, last_halfpos, last_pos = step_data[data_pos]
            # Perform step smoothing
            rtdiff = req_time - last_time
            stime = next_time - last_time
            if stime <= smooth_time:
                pdiff = next_halfpos - last_halfpos
                return last_halfpos + rtdiff * pdiff / stime
            stime = .5 * smooth_time
            if rtdiff < stime:
                pdiff = last_pos - last_halfpos
                return last_halfpos + rtdiff * pdiff / stime
            rtdiff = next_time - req_time
            if rtdiff < stime:
                pdiff = last_pos - next_halfpos
                return next_halfpos + rtdiff * pdiff / stime
            return last_pos
    def _pull_block(self, req_time):
        step_data = self.step_data
        del step_data[:-1]
        self.data_pos = 0
        # Read data block containing requested time frame
        while 1:
            jmsg = self.jdispatch.pull_msg(req_time, self.name)
            if jmsg is None:
                last_time, last_halfpos, last_pos = step_data[0]
                self.step_data.append((req_time + .1, last_pos, last_pos))
                return
            last_time = jmsg['last_step_time']
            if req_time <= last_time:
                break
        # Process block into (time, half_position, position) 3-tuples
        first_time = step_time = jmsg['first_step_time']
        first_clock = jmsg['first_clock']
        step_clock = first_clock - jmsg['data'][0][0]
        cdiff = jmsg['last_clock'] - first_clock
        tdiff = last_time - first_time
        inv_freq = 0.
        if cdiff:
            inv_freq = tdiff / cdiff
        step_dist = jmsg['step_distance']
        step_pos = jmsg['start_position']
        for interval, raw_count, add in jmsg['data']:
            qs_dist = step_dist
            count = raw_count
            if count < 0:
                qs_dist = -qs_dist
                count = -count
            for i in range(count):
                step_clock += interval
                interval += add
                step_time = first_time + (step_clock - first_clock) * inv_freq
                step_halfpos = step_pos + .5 * qs_dist
                step_pos += qs_dist
                step_data.append((step_time, step_halfpos, step_pos))
LogHandlers["stepq"] = HandleStepQ


######################################################################
# List datasets
######################################################################

def list_datasets():
    datasets = []
    for lh in sorted(LogHandlers.keys()):
        datasets += LogHandlers[lh].DataSets
    return datasets


######################################################################
# Log reading
######################################################################

# Read, uncompress, and parse messages in a log built by data_logger.py
class JsonLogReader:
    def __init__(self, filename):
        self.file = open(filename, "rb")
        self.comp = zlib.decompressobj(31)
        self.msgs = [b""]
    def seek(self, pos):
        self.file.seek(pos)
        self.comp = zlib.decompressobj(-15)
    def pull_msg(self):
        msgs = self.msgs
        while 1:
            if len(msgs) > 1:
                msg = msgs.pop(0)
                try:
                    json_msg = json.loads(msg)
                except:
                    logging.exception("Unable to parse line")
                    continue
                return json_msg
            raw_data = self.file.read(8192)
            if not raw_data:
                return None
            data = self.comp.decompress(raw_data)
            parts = data.split(b'\x03')
            parts[0] = msgs[0] + parts[0]
            self.msgs = msgs = parts

# Store messages in per-subscription queues until handlers are ready for them
class JsonDispatcher:
    def __init__(self, log_prefix):
        self.names = {}
        self.queues = {}
        self.last_read_time = 0.
        self.log_reader = JsonLogReader(log_prefix + ".json.gz")
        self.is_eof = False
    def check_end_of_data(self):
        return self.is_eof and not any(self.queues.values())
    def add_handler(self, name, subscription_id):
        self.names[name] = q = []
        self.queues.setdefault(subscription_id, []).append(q)
    def pull_msg(self, req_time, name):
        q = self.names[name]
        while 1:
            if q:
                return q.pop(0)
            if req_time + 1. < self.last_read_time:
                return None
            json_msg = self.log_reader.pull_msg()
            if json_msg is None:
                self.is_eof = True
                return None
            qid = json_msg.get('q')
            if qid == 'status':
                pt = json_msg.get('toolhead', {}).get('estimated_print_time')
                if pt is not None:
                    self.last_read_time = pt
            for mq in self.queues.get(qid, []):
                mq.append(json_msg['params'])

# Main log access management
class LogManager:
    error = error
    def __init__(self, log_prefix):
        self.index_reader = JsonLogReader(log_prefix + ".index.gz")
        self.jdispatch = JsonDispatcher(log_prefix)
        self.initial_start_time = self.start_time = 0.
        self.datasets = {}
        self.initial_status = {}
        self.log_subscriptions = {}
        self.status = {}
    def setup_index(self):
        fmsg = self.index_reader.pull_msg()
        self.initial_status = status = fmsg['status']
        self.status = dict(status)
        start_time = status['toolhead']['estimated_print_time']
        self.initial_start_time = self.start_time = start_time
        self.log_subscriptions = fmsg.get('subscriptions', {})
    def get_initial_status(self):
        return self.initial_status
    def available_dataset_types(self):
        return {name: None for name in LogHandlers}
    def get_jdispatch(self):
        return self.jdispatch
    def seek_time(self, req_time):
        self.start_time = req_start_time = self.initial_start_time + req_time
        seek_time = max(self.initial_start_time, req_start_time - 1.)
        file_position = 0
        while 1:
            fmsg = self.index_reader.pull_msg()
            if fmsg is None:
                break
            th = fmsg['status']['toolhead']
            ptime = max(th['estimated_print_time'], th.get('print_time', 0.))
            if ptime > seek_time:
                break
            file_position = fmsg['file_position']
        if file_position:
            self.jdispatch.log_reader.seek(file_position)
    def get_initial_start_time(self):
        return self.initial_start_time
    def get_start_time(self):
        return self.start_time
    def setup_dataset(self, name):
        if name in self.datasets:
            return self.datasets[name]
        parts = name.split(':')
        cls = LogHandlers.get(parts[0])
        if cls is None:
            raise error("Unknown dataset '%s'" % (parts[0],))
        if len(parts) < cls.ParametersMin or len(parts) > cls.ParametersMax:
            raise error("Invalid number of parameters for %s" % (parts[0],))
        subscription_id = ":".join(parts[:cls.ParametersSubscriptionId])
        if subscription_id not in self.log_subscriptions:
            raise error("Dataset '%s' not in capture" % (subscription_id,))
        self.datasets[name] = hdl = cls(self, name)
        self.jdispatch.add_handler(name, subscription_id)
        return hdl
