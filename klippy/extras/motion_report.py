# Diagnostic tool for reporting stepper and kinematic positions
#
# Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import chelper

API_UPDATE_INTERVAL = 0.500

# Helper to periodically transmit data to a set of API clients
class APIDumpHelper:
    def __init__(self, printer, data_cb, startstop_cb=None,
                 update_interval=API_UPDATE_INTERVAL):
        self.printer = printer
        self.data_cb = data_cb
        if startstop_cb is None:
            startstop_cb = (lambda is_start: None)
        self.startstop_cb = startstop_cb
        self.update_interval = update_interval
        self.update_timer = None
        self.clients = {}
    def _stop(self):
        self.clients.clear()
        if self.update_timer is None:
            return
        reactor = self.printer.get_reactor()
        reactor.unregister_timer(self.update_timer)
        self.update_timer = None
        try:
            self.startstop_cb(False)
        except self.printer.command_error as e:
            logging.exception("API Dump Helper stop callback error")
        return reactor.NEVER
    def _start(self):
        if self.update_timer is not None:
            return
        try:
            self.startstop_cb(True)
        except self.printer.command_error as e:
            logging.exception("API Dump Helper start callback error")
            self._stop()
            raise
        reactor = self.printer.get_reactor()
        systime = reactor.monotonic()
        waketime = systime + self.update_interval
        self.update_timer = reactor.register_timer(self._update, waketime)
    def add_client(self, web_request):
        cconn = web_request.get_client_connection()
        template = web_request.get_dict('response_template', {})
        self.clients[cconn] = template
        self._start()
    def add_internal_client(self):
        cconn = InternalDumpClient()
        self.clients[cconn] = {}
        self._start()
        return cconn
    def _update(self, eventtime):
        try:
            msg = self.data_cb(eventtime)
        except self.printer.command_error as e:
            logging.exception("API Dump Helper data callback error")
            return self._stop()
        if not msg:
            return eventtime + self.update_interval
        for cconn, template in list(self.clients.items()):
            if cconn.is_closed():
                del self.clients[cconn]
                if not self.clients:
                    return self._stop()
                continue
            tmp = dict(template)
            tmp['params'] = msg
            cconn.send(tmp)
        return eventtime + self.update_interval

# An "internal webhooks" wrapper for using APIDumpHelper internally
class InternalDumpClient:
    def __init__(self):
        self.msgs = []
        self.is_done = False
    def get_messages(self):
        return self.msgs
    def finalize(self):
        self.is_done = True
    def is_closed(self):
        return self.is_done
    def send(self, msg):
        self.msgs.append(msg)
        if len(self.msgs) >= 10000:
            # Avoid filling up memory with too many samples
            self.finalize()

# Extract stepper queue_step messages
class DumpStepper:
    def __init__(self, printer, mcu_stepper):
        self.printer = printer
        self.mcu_stepper = mcu_stepper
        self.last_api_clock = 0
        self.api_dump = APIDumpHelper(printer, self._api_update)
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint("motion_report/dump_stepper", "name",
                                 mcu_stepper.get_name(), self._add_api_client)
    def get_step_queue(self, start_clock, end_clock):
        mcu_stepper = self.mcu_stepper
        res = []
        while 1:
            data, count = mcu_stepper.dump_steps(128, start_clock, end_clock)
            if not count:
                break
            res.append((data, count))
            if count < len(data):
                break
            end_clock = data[count-1].first_clock
        res.reverse()
        return ([d[i] for d, cnt in res for i in range(cnt-1, -1, -1)], res)
    def log_steps(self, data):
        if not data:
            return
        out = []
        out.append("Dumping stepper '%s' (%s) %d queue_step:"
                   % (self.mcu_stepper.get_name(),
                      self.mcu_stepper.get_mcu().get_name(), len(data)))
        for i, s in enumerate(data):
            out.append("queue_step %d: t=%d p=%d i=%d c=%d a=%d"
                       % (i, s.first_clock, s.start_position, s.interval,
                          s.step_count, s.add))
        logging.info('\n'.join(out))
    def _api_update(self, eventtime):
        data, cdata = self.get_step_queue(self.last_api_clock, 1<<63)
        if not data:
            return {}
        clock_to_print_time = self.mcu_stepper.get_mcu().clock_to_print_time
        first = data[0]
        first_clock = first.first_clock
        first_time = clock_to_print_time(first_clock)
        self.last_api_clock = last_clock = data[-1].last_clock
        last_time = clock_to_print_time(last_clock)
        mcu_pos = first.start_position
        start_position = self.mcu_stepper.mcu_to_commanded_position(mcu_pos)
        step_dist = self.mcu_stepper.get_step_dist()
        if self.mcu_stepper.is_dir_inverted():
            step_dist = -step_dist
        d = [(s.interval, s.step_count, s.add) for s in data]
        return {"data": d, "start_position": start_position,
                "start_mcu_position": mcu_pos, "step_distance": step_dist,
                "first_clock": first_clock, "first_step_time": first_time,
                "last_clock": last_clock, "last_step_time": last_time}
    def _add_api_client(self, web_request):
        self.api_dump.add_client(web_request)
        hdr = ('interval', 'count', 'add')
        web_request.send({'header': hdr})

NEVER_TIME = 9999999999999999.

# Extract trapezoidal motion queue (trapq)
class DumpTrapQ:
    def __init__(self, printer, name, trapq):
        self.printer = printer
        self.name = name
        self.trapq = trapq
        self.last_api_msg = (0., 0.)
        self.api_dump = APIDumpHelper(printer, self._api_update)
        wh = self.printer.lookup_object('webhooks')
        wh.register_mux_endpoint("motion_report/dump_trapq", "name", name,
                                 self._add_api_client)
    def extract_trapq(self, start_time, end_time):
        ffi_main, ffi_lib = chelper.get_ffi()
        res = []
        while 1:
            data = ffi_main.new('struct pull_move[128]')
            count = ffi_lib.trapq_extract_old(self.trapq, data, len(data),
                                              start_time, end_time)
            if not count:
                break
            res.append((data, count))
            if count < len(data):
                break
            end_time = data[count-1].print_time
        res.reverse()
        return ([d[i] for d, cnt in res for i in range(cnt-1, -1, -1)], res)
    def log_trapq(self, data):
        if not data:
            return
        out = ["Dumping trapq '%s' %d moves:" % (self.name, len(data))]
        for i, m in enumerate(data):
            out.append("move %d: pt=%.6f mt=%.6f sv=%.6f a=%.6f"
                       " sp=(%.6f,%.6f,%.6f) ar=(%.6f,%.6f,%.6f)"
                       % (i, m.print_time, m.move_t, m.start_v, m.accel,
                          m.start_x, m.start_y, m.start_z, m.x_r, m.y_r, m.z_r))
        logging.info('\n'.join(out))
    def get_trapq_position(self, print_time):
        ffi_main, ffi_lib = chelper.get_ffi()
        data = ffi_main.new('struct pull_move[1]')
        count = ffi_lib.trapq_extract_old(self.trapq, data, 1, 0., print_time)
        if not count:
            return None, None
        move = data[0]
        move_time = max(0., min(move.move_t, print_time - move.print_time))
        dist = (move.start_v + .5 * move.accel * move_time) * move_time;
        pos = (move.start_x + move.x_r * dist, move.start_y + move.y_r * dist,
               move.start_z + move.z_r * dist)
        velocity = move.start_v + move.accel * move_time
        return pos, velocity
    def _api_update(self, eventtime):
        qtime = self.last_api_msg[0] + min(self.last_api_msg[1], 0.100)
        data, cdata = self.extract_trapq(qtime, NEVER_TIME)
        d = [(m.print_time, m.move_t, m.start_v, m.accel,
              (m.start_x, m.start_y, m.start_z), (m.x_r, m.y_r, m.z_r))
             for m in data]
        if d and d[0] == self.last_api_msg:
            d.pop(0)
        if not d:
            return {}
        self.last_api_msg = d[-1]
        return {"data": d}
    def _add_api_client(self, web_request):
        self.api_dump.add_client(web_request)
        hdr = ('time', 'duration', 'start_velocity', 'acceleration',
               'start_position', 'direction')
        web_request.send({'header': hdr})

STATUS_REFRESH_TIME = 0.250

class PrinterMotionReport:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = {}
        self.trapqs = {}
        # get_status information
        self.next_status_time = 0.
        gcode = self.printer.lookup_object('gcode')
        self.last_status = {
            'live_position': gcode.Coord(0., 0., 0., 0.),
            'live_velocity': 0., 'live_extruder_velocity': 0.,
            'steppers': [], 'trapq': [],
        }
        # Register handlers
        self.printer.register_event_handler("klippy:connect", self._connect)
        self.printer.register_event_handler("klippy:shutdown", self._shutdown)
    def register_stepper(self, config, mcu_stepper):
        ds = DumpStepper(self.printer, mcu_stepper)
        self.steppers[mcu_stepper.get_name()] = ds
    def _connect(self):
        # Lookup toolhead trapq
        toolhead = self.printer.lookup_object("toolhead")
        trapq = toolhead.get_trapq()
        self.trapqs['toolhead'] = DumpTrapQ(self.printer, 'toolhead', trapq)
        # Lookup extruder trapqs
        for i in range(99):
            ename = "extruder%d" % (i,)
            if ename == "extruder0":
                ename = "extruder"
            extruder = self.printer.lookup_object(ename, None)
            if extruder is None:
                break
            etrapq = extruder.get_trapq()
            self.trapqs[ename] = DumpTrapQ(self.printer, ename, etrapq)
        # Populate 'trapq' and 'steppers' in get_status result
        self.last_status['steppers'] = list(sorted(self.steppers.keys()))
        self.last_status['trapq'] = list(sorted(self.trapqs.keys()))
    # Shutdown handling
    def _dump_shutdown(self, eventtime):
        # Log stepper queue_steps on mcu that started shutdown (if any)
        shutdown_time = NEVER_TIME
        for dstepper in self.steppers.values():
            mcu = dstepper.mcu_stepper.get_mcu()
            sc = mcu.get_shutdown_clock()
            if not sc:
                continue
            shutdown_time = min(shutdown_time, mcu.clock_to_print_time(sc))
            clock_100ms = mcu.seconds_to_clock(0.100)
            start_clock = max(0, sc - clock_100ms)
            end_clock = sc + clock_100ms
            data, cdata = dstepper.get_step_queue(start_clock, end_clock)
            dstepper.log_steps(data)
        if shutdown_time >= NEVER_TIME:
            return
        # Log trapqs around time of shutdown
        for dtrapq in self.trapqs.values():
            data, cdata = dtrapq.extract_trapq(shutdown_time - .100,
                                               shutdown_time + .100)
            dtrapq.log_trapq(data)
        # Log estimated toolhead position at time of shutdown
        dtrapq = self.trapqs.get('toolhead')
        if dtrapq is None:
            return
        pos, velocity = dtrapq.get_trapq_position(shutdown_time)
        if pos is not None:
            logging.info("Requested toolhead position at shutdown time %.6f: %s"
                         , shutdown_time, pos)
    def _shutdown(self):
        self.printer.get_reactor().register_callback(self._dump_shutdown)
    # Status reporting
    def get_status(self, eventtime):
        if eventtime < self.next_status_time or not self.trapqs:
            return self.last_status
        self.next_status_time = eventtime + STATUS_REFRESH_TIME
        xyzpos = (0., 0., 0.)
        epos = (0.,)
        xyzvelocity = evelocity = 0.
        # Calculate current requested toolhead position
        mcu = self.printer.lookup_object('mcu')
        print_time = mcu.estimated_print_time(eventtime)
        pos, velocity = self.trapqs['toolhead'].get_trapq_position(print_time)
        if pos is not None:
            xyzpos = pos[:3]
            xyzvelocity = velocity
        # Calculate requested position of currently active extruder
        toolhead = self.printer.lookup_object('toolhead')
        ehandler = self.trapqs.get(toolhead.get_extruder().get_name())
        if ehandler is not None:
            pos, velocity = ehandler.get_trapq_position(print_time)
            if pos is not None:
                epos = (pos[0],)
                evelocity = velocity
        # Report status
        self.last_status = dict(self.last_status)
        self.last_status['live_position'] = toolhead.Coord(*(xyzpos + epos))
        self.last_status['live_velocity'] = xyzvelocity
        self.last_status['live_extruder_velocity'] = evelocity
        return self.last_status

def load_config(config):
    return PrinterMotionReport(config)
