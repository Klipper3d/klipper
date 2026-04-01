# Continuous bed mesh scanning with binary (on/off) inductive probe
#
# Copyright (C) 2026  Matti Airas <matti.airas@hatlabs.fi>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections, logging, math, os, struct
from . import bulk_sensor

# Compatible with manual_probe.ProbeResult
ProbeResult = collections.namedtuple('probe_result', [
    'bed_x', 'bed_y', 'bed_z', 'test_x', 'test_y', 'test_z'])

HYSTERESIS_SAMPLES = 7
HYSTERESIS_LIFT_CLEARANCE = 5.  # mm above z_offset for hysteresis cal
MIN_TRIGGER_EVENTS = 3
POST_SCAN_LIFT = 10.            # mm to raise after scan completes
FINAL_EVENT_DELAY = 0.1         # seconds to wait for trailing events
DEFAULT_HYSTERESIS = 0.2          # mm, fallback when not measured
HYST_Z_ABOVE = 2.0               # mm above trigger to guarantee untrigger
HYST_Z_BELOW = 0.5               # mm below trigger (already triggered zone)
POLL_TIMEOUT = 120.               # seconds wall-clock timeout for event polling


# MCU communication helper - manages probe_scan firmware module.
# Uses the bulk sensor subsystem for MCU-to-host event streaming.
class ProbeScanHelper:
    def __init__(self, config, mcu, pin_params):
        self._printer = config.get_printer()
        self._mcu = mcu
        self._pin = pin_params['pin']
        self._pullup = pin_params['pullup']
        self._invert = pin_params['invert']
        self._oid = self._mcu.create_oid()
        self._all_events = []
        self._last_sequence = 0
        self._start_cmd = self._stop_cmd = None
        self._bulk_queue = None
        self._mcu.register_config_callback(self._build_config)

    def _build_config(self):
        self._mcu.add_config_cmd(
            "config_probe_scan oid=%d pin=%s pull_up=%d"
            % (self._oid, self._pin, self._pullup))
        cmd_queue = self._mcu.alloc_command_queue()
        self._start_cmd = self._mcu.lookup_command(
            "probe_scan_start oid=%c clock=%u rest_ticks=%u invert=%c",
            cq=cmd_queue)
        self._stop_cmd = self._mcu.lookup_command(
            "probe_scan_stop oid=%c", cq=cmd_queue)
        self._bulk_queue = bulk_sensor.BulkDataQueue(self._mcu,
                                                     oid=self._oid)

    def _flush_events(self):
        raw_samples = self._bulk_queue.pull_queue()
        event_size = 5
        for params in raw_samples:
            seq = params['sequence']
            seq_diff = (seq - self._last_sequence) & 0xffff
            if seq_diff > 1 and self._last_sequence != 0:
                logging.warning("probe_scan: sequence gap detected: "
                                "expected %d, got %d (missed %d messages)"
                                % (self._last_sequence + 1, seq,
                                   seq_diff - 1))
            self._last_sequence = (self._last_sequence + seq_diff) & 0xffff
            data = params['data']
            count = len(data) // event_size
            for i in range(count):
                offset = i * event_size
                clock32 = struct.unpack_from('<I', data, offset)[0]
                state = data[offset + 4]
                clock64 = self._mcu.clock32_to_clock64(clock32)
                print_time = self._mcu.clock_to_print_time(clock64)
                logging.info("probe_scan: event state=%d print_time=%.4f "
                             "clock32=%u" % (state, print_time, clock32))
                self._all_events.append((print_time, state))

    def start_collection(self, print_time, poll_us):
        self._all_events = []
        self._last_sequence = 0
        self._bulk_queue.clear_queue()
        clock = self._mcu.print_time_to_clock(print_time)
        rest_ticks = self._mcu.seconds_to_clock(poll_us / 1e6)
        logging.info("probe_scan: start_collection oid=%d clock=%d "
                     "rest_ticks=%d invert=%d pin=%s pullup=%d"
                     % (self._oid, clock, rest_ticks,
                        self._invert, self._pin, self._pullup))
        self._start_cmd.send(
            [self._oid, clock, rest_ticks, self._invert],
            reqclock=clock)

    def stop_collection(self):
        self._stop_cmd.send([self._oid])
        self._flush_events()

    def get_new_events_since(self, index):
        self._flush_events()
        return self._all_events[index:]

    def get_all_events(self):
        self._flush_events()
        return list(self._all_events)


# Endstop wrapper for probe_scan pin - enables probing_move() and
# query_endstop() without depending on a separate [probe] section.
# Uses a standard MCU_endstop on the same pin as the scan GPIO monitor.
class ProbeScanEndstop:
    def __init__(self, config, mcu, pin_params):
        self._printer = config.get_printer()
        # Create a standard MCU_endstop for probing moves
        self._mcu_endstop = mcu.setup_pin('endstop', pin_params)
        # Register Z steppers when kinematics become available
        self._printer.register_event_handler('klippy:mcu_identify',
                                             self._handle_mcu_identify)

    def _handle_mcu_identify(self):
        kin = self._printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self._mcu_endstop.add_stepper(stepper)

    def get_mcu(self):
        return self._mcu_endstop.get_mcu()

    def add_stepper(self, stepper):
        self._mcu_endstop.add_stepper(stepper)

    def get_steppers(self):
        return self._mcu_endstop.get_steppers()

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        return self._mcu_endstop.home_start(
            print_time, sample_time, sample_count, rest_time,
            triggered=triggered)

    def home_wait(self, home_end_time):
        return self._mcu_endstop.home_wait(home_end_time)

    def query_endstop(self, print_time):
        return self._mcu_endstop.query_endstop(print_time)

    # Probe lifecycle - no-ops for a simple inductive probe
    def multi_probe_begin(self):
        pass

    def multi_probe_end(self):
        pass

    def probe_prepare(self, hmove):
        pass

    def probe_finish(self, hmove):
        pass


# Static zig-zag scan controller - pre-computes entire Z oscillation path
class ProbeScanController:
    def __init__(self, config):
        self.scan_speed = config.getfloat('speed', 50., above=0.)
        self.z_speed = config.getfloat('z_speed', 5., above=0.)
        self.z_scan_margin = config.getfloat('z_margin', 0.1, above=0.)
        self.segment_length = config.getfloat('segment_length', 2., above=0.5)
        self.z_top = None
        self.z_bottom = None

    def set_z_range(self, z_offset, hysteresis):
        self.z_bottom = z_offset - self.z_scan_margin
        self.z_top = z_offset + hysteresis + self.z_scan_margin

    def run_scan(self, printer, probe_helper, waypoints, start_x, start_y,
                 x_offset, y_offset, collector=None):
        """Run a zig-zag scan along a sequence of waypoints.

        waypoints: list of (target_x, target_y) in bed coords.
        The scan generates segments from (start_x, start_y) through each
        waypoint sequentially as one continuous motion.  Z oscillates
        between self.z_top and self.z_bottom.
        """
        if self.z_top is None or self.z_bottom is None:
            raise printer.command_error(
                "probe_scan: set_z_range() must be called before run_scan()")
        toolhead = printer.lookup_object('toolhead')

        scan_speed = self.scan_speed
        segment_length = self.segment_length
        segment_time = segment_length / scan_speed

        half_amplitude = (self.z_top - self.z_bottom) / 2.
        # Cap Z step so there are at least 4 segments per half-cycle
        dz_per_segment = min(self.z_speed * segment_time,
                             half_amplitude / 4.)

        # Current toolhead position in toolhead coords
        cur_x = start_x - x_offset
        cur_y = start_y - y_offset

        z_goal_dir = -1        # start descending (approach left us high)
        z_last = self.z_top    # start at top
        total_queued = 0

        logging.info("probe_scan: static zig-zag scan z_top=%.3f "
                     "z_bottom=%.3f dz_per_seg=%.3f waypoints=%d"
                     % (self.z_top, self.z_bottom, dz_per_segment,
                        len(waypoints)))

        all_segments = []
        reactor = printer.get_reactor()
        event_cursor = 0

        # Phase 1: Queue the entire scan path upfront
        # Use list for Python 2 closure mutability
        wp = [0, 0., 0., 0., 0.]  # idx, dist_done, dist_total, ux, uy

        def _advance_waypoint():
            while wp[0] < len(waypoints):
                target_x, target_y = waypoints[int(wp[0])]
                tx = target_x - x_offset
                ty = target_y - y_offset
                dx = tx - cur_x
                dy = ty - cur_y
                wp[2] = math.hypot(dx, dy)
                if wp[2] >= 1e-6:
                    wp[3] = dx / wp[2]
                    wp[4] = dy / wp[2]
                    wp[1] = 0.
                    return True
                wp[0] += 1
            return False

        if not _advance_waypoint():
            return

        # Correlate events every N segments to prevent step history
        # expiration (~30s retention at scan speed).
        CORRELATE_INTERVAL = 200  # segments between correlation passes

        scan_done = False
        while not scan_done:
            remaining = wp[2] - wp[1]
            step = min(segment_length, remaining)

            z_goal = self.z_top if z_goal_dir == 1 else self.z_bottom
            if z_goal_dir == 1:
                z_last = min(z_last + dz_per_segment, z_goal)
            else:
                z_last = max(z_last - dz_per_segment, z_goal)

            new_x = cur_x + wp[3] * step
            new_y = cur_y + wp[4] * step
            toolhead.manual_move([new_x, new_y, z_last], scan_speed)

            all_segments.append((new_x, new_y, z_last))
            cur_x = new_x
            cur_y = new_y
            wp[1] += step
            total_queued += 1

            if z_goal_dir == 1 and z_last >= z_goal:
                z_goal_dir = -1
            elif z_goal_dir == -1 and z_last <= z_goal:
                z_goal_dir = 1

            if wp[1] >= wp[2]:
                wp[0] += 1
                if not _advance_waypoint():
                    scan_done = True

            # Periodically correlate events during queuing to prevent
            # step history from expiring before positions are looked up
            if total_queued % CORRELATE_INTERVAL == 0:
                if collector is not None:
                    collector.add_segments(all_segments)
                    all_segments = []
                new_events = probe_helper.get_new_events_since(
                    event_cursor)
                if new_events:
                    if collector is not None:
                        collector.correlate_events(new_events)
                    event_cursor += len(new_events)

        if collector is not None:
            collector.add_segments(all_segments)

        logging.info("probe_scan: queued %d segments, waiting for "
                     "motion to complete" % total_queued)

        # Phase 2: Wait for motion, periodically correlating events
        # before step history expires (Klipper retains 30s)
        mcu = printer.lookup_object('mcu')
        scan_end_print_time = toolhead.get_last_move_time()
        EVENT_POLL_INTERVAL = 0.5  # seconds between correlation passes
        poll_deadline = reactor.monotonic() + POLL_TIMEOUT
        while True:
            now = reactor.monotonic()
            est_pt = mcu.estimated_print_time(now)
            if est_pt >= scan_end_print_time:
                break
            if now >= poll_deadline:
                raise printer.command_error(
                    "probe_scan: Timed out waiting for scan "
                    "motion to complete (%.0fs)" % POLL_TIMEOUT)
            reactor.pause(now + EVENT_POLL_INTERVAL)
            new_events = probe_helper.get_new_events_since(event_cursor)
            if new_events:
                if collector is not None:
                    collector.correlate_events(new_events)
                event_cursor += len(new_events)

        toolhead.wait_moves()
        reactor.pause(reactor.monotonic() + FINAL_EVENT_DELAY)
        new_events = probe_helper.get_new_events_since(event_cursor)
        if new_events:
            if collector is not None:
                collector.correlate_events(new_events)
            event_cursor += len(new_events)

        logging.info("probe_scan: scan complete, %d segments queued, "
                     "%d events" % (total_queued, event_cursor))


def _get_toolhead_pos(toolhead, print_time):
    """Get toolhead position at print_time from stepper history."""
    kin = toolhead.get_kinematics()
    kin_spos = {
        s.get_name(): s.mcu_to_commanded_position(
            s.get_past_mcu_position(print_time))
        for s in kin.get_steppers()
    }
    return kin.calc_position(kin_spos)


# Correlates MCU events with toolhead positions via trapq lookup.
# Stores offsets at construction so it can accumulate points
# incrementally during the scan (before step history expires).
class ProbeScanEventCollector:
    def __init__(self, printer, hysteresis, x_offset, y_offset, z_offset):
        self._printer = printer
        self._hysteresis = hysteresis
        self._x_offset = x_offset
        self._y_offset = y_offset
        self._z_offset = z_offset
        self._points = []
        self._segments = []  # (bed_x, bed_y, toolhead_z) per segment endpoint

    def add_segments(self, segments):
        """Append segment waypoints: list of (x, y, z) in toolhead coords."""
        for x, y, z in segments:
            self._segments.append(
                (x + self._x_offset, y + self._y_offset, z))

    def get_segments(self):
        return list(self._segments)

    def correlate_events(self, events):
        """Correlate events and append to internal point list.
        Returns list of (event_time, state, raw_z) for caller use."""
        toolhead = self._printer.lookup_object('toolhead')
        results = []
        for print_time, state in events:
            pos = _get_toolhead_pos(toolhead, print_time)

            bed_x = pos[0] + self._x_offset
            bed_y = pos[1] + self._y_offset
            if state == 1:
                bed_z = pos[2] - self._z_offset
            else:
                bed_z = pos[2] - self._z_offset - self._hysteresis

            self._points.append((bed_x, bed_y, bed_z, state, pos[2]))
            results.append((print_time, state, pos[2]))
        return results

    def get_points(self):
        """Return (bed_x, bed_y, bed_z) tuples for mesh generation."""
        return [(x, y, z) for x, y, z, _s, _rz in self._points]

    def get_detailed_points(self):
        """Return (bed_x, bed_y, bed_z, state, raw_z) for diagnostics."""
        return list(self._points)


# Interpolates scattered probe points onto a regular grid using IDW
class ProbeScanMeshGenerator:
    def __init__(self, search_radius=10.0, power=2.0):
        self._search_radius = search_radius
        self._power = power

    def generate_mesh(self, scatter_points, grid_points):
        results = []
        r2_max = self._search_radius ** 2

        for gx, gy in grid_points:
            weighted_z = 0.
            weight_sum = 0.
            for sx, sy, sz in scatter_points:
                dx = gx - sx
                dy = gy - sy
                d2 = dx * dx + dy * dy
                if d2 > r2_max:
                    continue
                if d2 < 1e-10:
                    weighted_z = sz
                    weight_sum = 1.
                    break
                w = 1. / (d2 ** (self._power / 2.))
                weighted_z += w * sz
                weight_sum += w

            if weight_sum < 1e-10:
                raise Exception(
                    "probe_scan: No probe data near grid point (%.1f, %.1f). "
                    "Try increasing z_margin." % (gx, gy))

            bed_z = weighted_z / weight_sum
            results.append(ProbeResult(
                gx, gy, bed_z, gx, gy, bed_z))

        return results


# Main orchestrator
class PrinterProbeScan:
    def __init__(self, config):
        self._printer = config.get_printer()
        # Allow multi-use so [probe] and [probe_scan] can share the same
        # physical pin. The firmware handles them independently (endstop
        # vs GPIO polling).
        ppins = config.get_printer().lookup_object('pins')
        pin_desc = config.get('pin')
        pre_params = ppins.parse_pin(pin_desc, can_invert=True,
                                     can_pullup=True)
        share_name = "%s:%s" % (pre_params['chip_name'], pre_params['pin'])
        ppins.allow_multi_use_pins[share_name] = True
        pin_params = ppins.lookup_pin(pin_desc,
                                      can_invert=True, can_pullup=True,
                                      share_type='probe_scan')
        mcu = pin_params['chip']

        # Continuous scan GPIO monitor
        self._probe_helper = ProbeScanHelper(config, mcu, pin_params)

        # Standard endstop on same pin for probing moves (hysteresis cal)
        self._endstop = ProbeScanEndstop(config, mcu, pin_params)

        # XY offsets from our config; z_offset from [probe] so it stays
        # in sync with PROBE_CALIBRATE / SAVE_CONFIG
        self._x_offset = config.getfloat('x_offset', 0.)
        self._y_offset = config.getfloat('y_offset', 0.)
        probe_config = config.getsection('probe')
        self._z_offset = probe_config.getfloat('z_offset')

        # Scan parameters
        self._poll_us = config.getfloat('poll_us', 50.)
        self._max_hysteresis = config.getfloat('max_hysteresis', 0.5,
                                               above=0.)
        self._cal_point = config.getfloatlist('calibration_point', None,
                                              count=2)
        self._probe_z_min = config.getfloat('probe_z_min', 0.)
        self._scan_controller = ProbeScanController(config)
        search_radius = config.getfloat('search_radius', 35., above=0.)
        self._mesh_generator = ProbeScanMeshGenerator(
            search_radius=search_radius)
        self._cross_hatch = config.getboolean('cross_hatch', False)
        self._cross_hatch_rows = config.getint('cross_hatch_rows', 7,
                                               minval=3)
        self._cross_hatch_cols = config.getint('cross_hatch_cols', 7,
                                               minval=3)

        self._hysteresis = None  # Cached from last measurement

        # Register commands
        gcode = self._printer.lookup_object('gcode')
        gcode.register_command('PROBE_SCAN_CALIBRATE',
                               self.cmd_PROBE_SCAN_CALIBRATE,
                               desc=self.cmd_PROBE_SCAN_CALIBRATE_help)
        gcode.register_command('PROBE_SCAN_TEST',
                               self.cmd_PROBE_SCAN_TEST,
                               desc=self.cmd_PROBE_SCAN_TEST_help)
        gcode.register_command('PROBE_SCAN_DIAG',
                               self.cmd_PROBE_SCAN_DIAG,
                               desc=self.cmd_PROBE_SCAN_DIAG_help)

    def _travel_speed(self):
        toolhead = self._printer.lookup_object('toolhead')
        return toolhead.get_max_velocity()[0]

    def _get_calibration_point(self):
        """Return XY point for hysteresis calibration.

        Falls back to bed_mesh center if calibration_point not configured.
        Note: the bed_mesh internal access here is for the standalone
        hysteresis calibration path, not perform_scan_mesh (which receives
        all mesh context via arguments).
        """
        if self._cal_point is not None:
            return self._cal_point
        bmc_obj = self._printer.lookup_object('bed_mesh')
        bmc = bmc_obj.bmc
        min_x, min_y = bmc.mesh_min
        max_x, max_y = bmc.mesh_max
        return ((min_x + max_x) / 2., (min_y + max_y) / 2.)

    def _measure_hysteresis(self, gcmd):
        """Measure probe hysteresis by continuous Z oscillation.

        Uses probing_move for initial bed contact, then queues continuous
        Z oscillation moves and correlates MCU probe events with toolhead
        position via step history to compute hysteresis.
        """
        toolhead = self._printer.lookup_object('toolhead')
        phoming = self._printer.lookup_object('homing')

        cal_x, cal_y = self._get_calibration_point()
        gcmd.respond_info(
            "probe_scan: Measuring hysteresis at (%.1f, %.1f)"
            % (cal_x, cal_y))

        lift_z = self._z_offset + HYSTERESIS_LIFT_CLEARANCE
        z_speed = self._scan_controller.z_speed

        # Move above calibration point
        toolhead.manual_move(
            [cal_x - self._x_offset, cal_y - self._y_offset, lift_z],
            self._travel_speed())
        toolhead.wait_moves()

        # Initial approach: use standard probing_move to find the trigger
        # point safely (this is identical to a normal PROBE command).
        pos = toolhead.get_position()
        target = list(pos)
        target[2] = self._probe_z_min
        trig_pos = phoming.probing_move(self._endstop, target, z_speed)
        z_trigger = trig_pos[2]
        gcmd.respond_info(
            "probe_scan: Initial trigger at Z=%.4f" % z_trigger)

        # Continuous oscillation: queue Z moves and collect events
        z_top = z_trigger + HYST_Z_ABOVE
        z_bottom = z_trigger - HYST_Z_BELOW
        # +1 for throwaway first cycle
        num_cycles = HYSTERESIS_SAMPLES + 1

        # Start MCU event collection
        probe_helper = self._probe_helper
        print_time = toolhead.get_last_move_time()
        probe_helper.start_collection(print_time, self._poll_us)

        # Queue all oscillation cycles: from current pos up to z_top,
        # then oscillate between z_bottom and z_top
        toolhead.manual_move([None, None, z_top], z_speed)
        for _i in range(num_cycles):
            toolhead.manual_move([None, None, z_bottom], z_speed)
            toolhead.manual_move([None, None, z_top], z_speed)

        # Wait for motion, periodically correlating events before
        # step history expires
        reactor = self._printer.get_reactor()
        mcu = self._printer.lookup_object('mcu')
        scan_end_print_time = toolhead.get_last_move_time()
        event_cursor = 0
        all_correlated = []  # (print_time, state, raw_z)
        EVENT_POLL_INTERVAL = 0.5
        poll_deadline = reactor.monotonic() + POLL_TIMEOUT

        while True:
            now = reactor.monotonic()
            est_pt = mcu.estimated_print_time(now)
            if est_pt >= scan_end_print_time:
                break
            if now >= poll_deadline:
                raise gcmd.error(
                    "probe_scan: Timed out waiting for hysteresis "
                    "motion to complete (%.0fs)" % POLL_TIMEOUT)
            reactor.pause(now + EVENT_POLL_INTERVAL)
            new_events = probe_helper.get_new_events_since(event_cursor)
            if new_events:
                for pt, state in new_events:
                    pos = _get_toolhead_pos(toolhead, pt)
                    all_correlated.append((pt, state, pos[2]))
                event_cursor += len(new_events)

        toolhead.wait_moves()
        reactor.pause(reactor.monotonic() + FINAL_EVENT_DELAY)
        new_events = probe_helper.get_new_events_since(event_cursor)
        if new_events:
            for pt, state in new_events:
                pos = _get_toolhead_pos(toolhead, pt)
                all_correlated.append((pt, state, pos[2]))
            event_cursor += len(new_events)

        probe_helper.stop_collection()

        # Pair trigger/untrigger events sequentially: each trigger (state=1)
        # is paired with the immediately following untrigger (state=0).
        # This is robust against bounces or duplicate events.
        pairs = []  # (trigger_z, untrigger_z)
        waiting_trigger = None
        for _pt, state, z in all_correlated:
            if state == 1:
                waiting_trigger = z
            elif waiting_trigger is not None:
                pairs.append((waiting_trigger, z))
                waiting_trigger = None

        logging.info("probe_scan: hysteresis events: %d correlated, "
                     "%d pairs" % (len(all_correlated), len(pairs)))

        # Drop first pair (throwaway cycle: initial trigger from
        # probing_move approach + first untrigger from rise to z_top)
        if pairs:
            pairs = pairs[1:]

        if len(pairs) < HYSTERESIS_SAMPLES:
            raise gcmd.error(
                "probe_scan: Not enough hysteresis event pairs "
                "(%d, need %d)" % (len(pairs), HYSTERESIS_SAMPLES))

        # Compute hysteresis from each trigger/untrigger pair
        hysteresis_samples = []
        for i in range(HYSTERESIS_SAMPLES):
            trig_z, untrig_z = pairs[i]
            hyst = untrig_z - trig_z
            hysteresis_samples.append(hyst)
            gcmd.respond_info(
                "  Sample %d: trigger=%.4f untrigger=%.4f hysteresis=%.4f"
                % (i + 1, trig_z, untrig_z, hyst))

        hysteresis_samples.sort()
        hysteresis = hysteresis_samples[HYSTERESIS_SAMPLES // 2]

        # Raise after measurement
        toolhead.manual_move([None, None, lift_z], self._travel_speed())
        toolhead.wait_moves()

        gcmd.respond_info(
            "probe_scan: Measured hysteresis = %.4f mm" % hysteresis)

        if hysteresis > self._max_hysteresis:
            raise gcmd.error(
                "probe_scan: Measured hysteresis (%.4f) exceeds "
                "max_hysteresis (%.4f)" % (hysteresis, self._max_hysteresis))
        if hysteresis < 0.:
            raise gcmd.error(
                "probe_scan: Negative hysteresis (%.4f) - probe malfunction?"
                % hysteresis)

        return hysteresis

    cmd_PROBE_SCAN_CALIBRATE_help = "Calibrate probe scan (measure hysteresis)"
    def cmd_PROBE_SCAN_CALIBRATE(self, gcmd):
        hysteresis = self._measure_hysteresis(gcmd)
        self._hysteresis = hysteresis
        gcmd.respond_info(
            "probe_scan: Hysteresis measurement complete: %.4f mm"
            % hysteresis)

    cmd_PROBE_SCAN_DIAG_help = "Diagnose MCU probe_scan event collection"
    def cmd_PROBE_SCAN_DIAG(self, gcmd):
        """Start MCU collection, trigger+untrigger via endstop, report."""
        from . import homing as homing_mod

        toolhead = self._printer.lookup_object('toolhead')
        phoming = self._printer.lookup_object('homing')
        reactor = self._printer.get_reactor()

        cal_x, cal_y = self._get_calibration_point()
        lift_z = self._z_offset + HYSTERESIS_LIFT_CLEARANCE
        z_speed = self._scan_controller.z_speed

        # Move above calibration point
        toolhead.manual_move(
            [cal_x - self._x_offset, cal_y - self._y_offset, lift_z],
            self._travel_speed())
        toolhead.wait_moves()

        # Query endstop state before collection
        print_time = toolhead.get_last_move_time()
        endstop_state = self._endstop.query_endstop(print_time)
        gcmd.respond_info(
            "probe_scan diag: endstop state at Z=%.2f: %d (0=open, 1=triggered)"
            % (lift_z, endstop_state))

        # Start MCU collection
        print_time = toolhead.get_last_move_time()
        self._probe_helper.start_collection(
            print_time, self._poll_us)
        gcmd.respond_info("probe_scan diag: MCU collection started")

        # Wait a moment and check for spurious events
        eventtime = reactor.monotonic()
        reactor.pause(eventtime + 0.5)
        events_before = self._probe_helper.get_all_events()
        gcmd.respond_info(
            "probe_scan diag: events before move: %d" % len(events_before))

        # Descend until trigger (probing_move)
        pos = toolhead.get_position()
        target = list(pos)
        target[2] = self._probe_z_min
        trig_pos = phoming.probing_move(self._endstop, target, z_speed)
        gcmd.respond_info(
            "probe_scan diag: probing_move triggered at Z=%.4f"
            % trig_pos[2])

        # Wait for MCU events to arrive
        toolhead.wait_moves()
        eventtime = reactor.monotonic()
        reactor.pause(eventtime + 0.5)
        events_after_trigger = self._probe_helper.get_all_events()
        gcmd.respond_info(
            "probe_scan diag: events after trigger: %d"
            % len(events_after_trigger))

        # Ascend until untrigger
        pos = toolhead.get_position()
        target = list(pos)
        target[2] = lift_z
        endstops = [(self._endstop, "probe")]
        hmove = homing_mod.HomingMove(self._printer, endstops, toolhead)
        untrig_pos = hmove.homing_move(target, z_speed, probe_pos=True,
                                       triggered=False,
                                       check_triggered=False)
        gcmd.respond_info(
            "probe_scan diag: ascend untrigger at Z=%.4f" % untrig_pos[2])

        # Wait for MCU events
        toolhead.wait_moves()
        eventtime = reactor.monotonic()
        reactor.pause(eventtime + 0.5)

        # Stop collection
        self._probe_helper.stop_collection()
        eventtime = reactor.monotonic()
        reactor.pause(eventtime + FINAL_EVENT_DELAY)

        all_events = self._probe_helper.get_all_events()
        gcmd.respond_info(
            "probe_scan diag: total events after full cycle: %d"
            % len(all_events))
        for i, (pt, state) in enumerate(all_events):
            gcmd.respond_info(
                "  event %d: print_time=%.6f state=%d" % (i, pt, state))

        # Raise after test
        toolhead.manual_move([None, None, lift_z], self._travel_speed())
        toolhead.wait_moves()

    def _approach_scan_start(self, toolhead, x, y, scan_z, gcmd):
        """Move to scan start XY at safe height, then descend toward bed.

        Uses probing_move (same as hysteresis/PROBE) to descend.
        Returns the trigger Z position for initializing the zig-zag
        center.  Raises on no-trigger (scan cannot proceed without a
        known trigger height).
        """
        phoming = self._printer.lookup_object('homing')

        lift_z = scan_z + HYSTERESIS_LIFT_CLEARANCE
        z_speed = self._scan_controller.z_speed

        # Move to XY at safe height
        toolhead.manual_move([None, None, lift_z], self._travel_speed())
        toolhead.manual_move(
            [x - self._x_offset, y - self._y_offset, None],
            self._travel_speed())
        toolhead.wait_moves()

        # Descend toward bed using probing_move (proven path)
        pos = toolhead.get_position()
        target = list(pos)
        target[2] = self._probe_z_min
        trig_pos = phoming.probing_move(self._endstop, target, z_speed)
        trigger_z = trig_pos[2]
        gcmd.respond_info(
            "probe_scan: Approach triggered at Z=%.4f" % trigger_z)
        return trigger_z

    cmd_PROBE_SCAN_TEST_help = "Short scan line for testing"
    def cmd_PROBE_SCAN_TEST(self, gcmd):
        """Run a short scan line. Parameters:
           X=<start_x>  Y=<start_y>  LENGTH=<mm>  Z=<scan_z>
           SKIP_APPROACH=1 (skip probe approach, go directly to scan)
        """
        x_start = gcmd.get_float('X', 30.)
        y = gcmd.get_float('Y', 0.)
        length = gcmd.get_float('LENGTH', 30.)
        scan_z = gcmd.get_float('Z', self._z_offset)
        skip_approach = gcmd.get_int('SKIP_APPROACH', 0)
        hysteresis = (self._hysteresis if self._hysteresis is not None
                      else DEFAULT_HYSTERESIS)

        self._scan_controller.set_z_range(self._z_offset, hysteresis)
        x_end = x_start + length

        gcmd.respond_info(
            "probe_scan: Test scan X=%.1f..%.1f Y=%.1f Z=%.3f "
            "z_top=%.3f z_bottom=%.3f%s"
            % (x_start, x_end, y, scan_z,
               self._scan_controller.z_top,
               self._scan_controller.z_bottom,
               " SKIP_APPROACH" if skip_approach else ""))

        toolhead = self._printer.lookup_object('toolhead')

        if skip_approach:
            gcmd.respond_info(
                "probe_scan: Skipping approach")
        else:
            # Approach bed to verify probe works
            self._approach_scan_start(toolhead, x_start, y, scan_z, gcmd)

        # Move to scan start position at z_top
        toolhead.manual_move(
            [x_start - self._x_offset, y - self._y_offset,
             self._scan_controller.z_top], self._travel_speed())
        toolhead.wait_moves()

        # Start MCU collection and scan
        collector = ProbeScanEventCollector(
            self._printer, hysteresis,
            self._x_offset, self._y_offset, self._z_offset)
        print_time = toolhead.get_last_move_time()
        self._probe_helper.start_collection(
            print_time, self._poll_us)
        try:
            waypoints = [(x_end, y)]
            self._scan_controller.run_scan(
                self._printer, self._probe_helper,
                waypoints, x_start, y,
                self._x_offset, self._y_offset,
                collector=collector)
            toolhead.flush_step_generation()
        finally:
            self._probe_helper.stop_collection()

        all_events = self._probe_helper.get_all_events()
        points = collector.get_detailed_points()
        gcmd.respond_info(
            "probe_scan: Test complete. %d trigger events collected, "
            "%d correlated." % (len(all_events), len(points)))
        for i, (bx, by, bz, state, raw_z) in enumerate(points):
            gcmd.respond_info(
                "  event %d: state=%d x=%.3f y=%.3f z=%.4f rz=%.4f"
                % (i, state, bx, by, bz, raw_z))

        # Lift after test
        toolhead.manual_move([None, None, scan_z + POST_SCAN_LIFT],
                             self._travel_speed())

    @staticmethod
    def _build_serpentine_waypoints(rows, min_coord, max_coord):
        """Build X-serpentine waypoints across Y rows."""
        waypoints = []
        for row_idx, y in enumerate(rows):
            if row_idx % 2 == 0:
                xs, xe = min_coord, max_coord
            else:
                xs, xe = max_coord, min_coord
            if row_idx > 0:
                waypoints.append((xs, y))
            waypoints.append((xe, y))
        return waypoints

    @staticmethod
    def _linspace(start, end, count):
        """Return count evenly-spaced values from start to end inclusive."""
        if count <= 1:
            return [start]
        step = (end - start) / (count - 1)
        return [start + i * step for i in range(count)]

    @staticmethod
    def _build_cross_hatch_waypoints(min_x, max_x, min_y, max_y,
                                     row_count, col_count):
        """Build cross-hatch waypoints: X-serpentine then Y-serpentine.

        Scan lines are evenly spaced across the bed, independent of the
        mesh grid. IDW interpolation maps the scattered events onto the
        grid.
        """
        x_scan_rows = PrinterProbeScan._linspace(min_y, max_y, row_count)
        # Phase 1: X-serpentine
        waypoints = PrinterProbeScan._build_serpentine_waypoints(
            x_scan_rows, min_x, max_x)

        # Where did phase 1 end?
        last_idx = len(x_scan_rows) - 1
        end_x = max_x if last_idx % 2 == 0 else min_x
        end_y = x_scan_rows[-1]

        # Phase 2: Y-serpentine
        y_scan_cols = PrinterProbeScan._linspace(min_x, max_x, col_count)
        # Start from column nearest to phase 1 end
        if end_x > (min_x + max_x) / 2:
            y_scan_cols = list(reversed(y_scan_cols))

        # Y direction: first column continues from end_y
        if end_y >= max_y:
            y_dirs = [(max_y, min_y), (min_y, max_y)]
        else:
            y_dirs = [(min_y, max_y), (max_y, min_y)]

        for col_idx, x in enumerate(y_scan_cols):
            ys, ye = y_dirs[col_idx % 2]
            if col_idx > 0:
                waypoints.append((x, ys))   # Transition to column start
            waypoints.append((x, ye))       # Scan column

        return waypoints

    def perform_scan_mesh(self, gcmd, base_points, x_count, y_count,
                          zero_ref_pos, finalize_cb):
        """Perform a continuous-motion bed mesh scan.

        Called by BED_MESH_CALIBRATE METHOD=probe_scan. All mesh context
        is passed in as arguments - no private bed_mesh access needed.
        """
        if not base_points:
            raise gcmd.error("probe_scan: No mesh points provided")
        if len(base_points) != x_count * y_count:
            raise gcmd.error(
                "probe_scan: base_points count (%d) != x_count*y_count (%d*%d)"
                % (len(base_points), x_count, y_count))

        skip_approach = gcmd.get_int('SKIP_APPROACH', 0)

        min_x = min(p[0] for p in base_points)
        max_x = max(p[0] for p in base_points)
        min_y = min(p[1] for p in base_points)
        max_y = max(p[1] for p in base_points)

        toolhead = self._printer.lookup_object('toolhead')

        # Step 1: Use cached hysteresis or measure fresh
        if skip_approach:
            hysteresis = (self._hysteresis if self._hysteresis is not None
                          else DEFAULT_HYSTERESIS)
        elif self._hysteresis is not None:
            hysteresis = self._hysteresis
            gcmd.respond_info(
                "probe_scan: Using cached hysteresis = %.4f mm"
                % hysteresis)
        else:
            hysteresis = self._measure_hysteresis(gcmd)
            self._hysteresis = hysteresis

        self._scan_controller.set_z_range(self._z_offset, hysteresis)
        gcmd.respond_info(
            "probe_scan: Mesh %d x %d points, "
            "X: %.1f-%.1f, Y: %.1f-%.1f z_top=%.3f z_bottom=%.3f"
            % (x_count, y_count, min_x, max_x, min_y, max_y,
               self._scan_controller.z_top,
               self._scan_controller.z_bottom))

        # Step 2: Move to scan start
        scan_z = self._z_offset
        if skip_approach:
            gcmd.respond_info("probe_scan: Skipping approach")
        else:
            self._approach_scan_start(
                toolhead, min_x, min_y, scan_z, gcmd)

        # Move to scan start position at z_top
        toolhead.manual_move(
            [min_x - self._x_offset, min_y - self._y_offset,
             self._scan_controller.z_top], self._travel_speed())
        toolhead.wait_moves()

        # Step 3: Start MCU event collection
        print_time = toolhead.get_last_move_time()
        self._probe_helper.start_collection(
            print_time, self._poll_us)

        # Step 4: Run scan (try/finally ensures MCU stops polling)
        y_rows = sorted(set(p[1] for p in base_points))
        x_cols = sorted(set(p[0] for p in base_points))

        collector = ProbeScanEventCollector(
            self._printer, hysteresis,
            self._x_offset, self._y_offset, self._z_offset)

        if self._cross_hatch:
            row_count = self._cross_hatch_rows
            col_count = self._cross_hatch_cols
            waypoints = self._build_cross_hatch_waypoints(
                min_x, max_x, min_y, max_y, row_count, col_count)
            gcmd.respond_info(
                "probe_scan: Starting cross-hatch scan "
                "(%d X-rows, %d Y-columns)..."
                % (row_count, col_count))
        else:
            waypoints = self._build_serpentine_waypoints(
                y_rows, min_x, max_x)
            gcmd.respond_info("probe_scan: Starting scan...")

        try:
            self._scan_controller.run_scan(
                self._printer, self._probe_helper,
                waypoints, min_x, min_y,
                self._x_offset, self._y_offset,
                collector=collector)
            toolhead.flush_step_generation()
        finally:
            # Step 5: Stop collection even on error
            self._probe_helper.stop_collection()

        all_events = self._probe_helper.get_all_events()
        detailed_points = collector.get_detailed_points()
        segments = collector.get_segments()
        gcmd.respond_info(
            "probe_scan: Collected %d trigger events, "
            "%d correlated points, %d segment waypoints"
            % (len(all_events), len(detailed_points), len(segments)))

        # Write scan data for offline analysis (plots, diagnostics)
        scan_dir = os.path.expanduser("~/printer_data/logs")
        scan_path = os.path.join(scan_dir, "probe_scan_data.txt")
        try:
            with open(scan_path, 'w') as f:
                for i, (sx, sy, sz, state, raw_z) in enumerate(
                        detailed_points):
                    f.write("pt %d: x=%.3f y=%.3f z=%.4f s=%d rz=%.4f\n"
                            % (i, sx, sy, sz, state, raw_z))
                for i, (sx, sy, sz) in enumerate(segments):
                    f.write("seg %d: x=%.3f y=%.3f z=%.4f\n"
                            % (i, sx, sy, sz))
            gcmd.respond_info(
                "probe_scan: Scan data written to %s" % scan_path)
        except IOError:
            logging.warning("probe_scan: Could not write scan data to %s"
                            % scan_path)

        if len(all_events) < MIN_TRIGGER_EVENTS:
            raise gcmd.error(
                "probe_scan: Too few trigger events (%d). "
                "Try increasing z_margin." % len(all_events))

        # Step 6: Interpolate onto mesh grid and finalize
        generate_mesh = gcmd.get_int('MESH', 1)
        if generate_mesh:
            scatter_points = collector.get_points()
            try:
                results = self._mesh_generator.generate_mesh(
                    scatter_points, base_points)

                # If zero_reference_position is configured outside the mesh,
                # probe_finalize expects an extra point at the end to pop as
                # the Z reference. Interpolate it from our scatter data.
                if zero_ref_pos is not None:
                    zref_results = self._mesh_generator.generate_mesh(
                        scatter_points, [zero_ref_pos])
                    results.append(zref_results[0])
            except Exception as e:
                logging.exception("probe_scan: Mesh generation failed")
                raise gcmd.error(str(e))

            gcmd.respond_info("probe_scan: Finalizing mesh...")
            finalize_cb(results)

        toolhead.manual_move([None, None, scan_z + POST_SCAN_LIFT],
                             self._travel_speed())
        gcmd.respond_info("probe_scan: Mesh scan complete")


def load_config(config):
    return PrinterProbeScan(config)
