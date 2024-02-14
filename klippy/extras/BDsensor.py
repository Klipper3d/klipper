# Bed leveling sensor BDsensor(Bed Distance sensor)
# https://github.com/markniu/Bed_Distance_sensor
# Copyright (C) 2023 Mark yue <niujl123@sina.com>
# This file may be distributed under the terms of the GNU GPLv3 license.
import sched, time,copy
from threading import Timer
from mcu import MCU, MCU_trsync
from . import manual_probe

import chelper
import math
from . import probe

TRSYNC_TIMEOUT = 0.025
TRSYNC_SINGLE_MCU_TIMEOUT = 0.250

# Calculate a move's accel_t, cruise_t, and cruise_v
def calc_move_time(dist, speed, accel):
    axis_r = 1.
    if dist < 0.:
        axis_r = -1.
        dist = -dist
    if not accel or not dist:
        return axis_r, 0., dist / speed, speed
    max_cruise_v2 = dist * accel
    if max_cruise_v2 < speed**2:
        speed = math.sqrt(max_cruise_v2)
    accel_t = speed / accel
    accel_decel_d = accel_t * speed
    cruise_t = (dist - accel_decel_d) / speed
    return axis_r, accel_t, cruise_t, speed


# I2C BD_SENSOR
# devices connected to an MCU via an virtual i2c bus(2 any gpio)

class MCU_I2C_BD:
    def __init__(self,mcu,   sda_pin,scl_pin, delay_t,home_pose,z_adjust):
        self.mcu = mcu
        #print("MCU_I2C_BD:%s"%mcu)
        self.oid = self.mcu.create_oid()
        # Generate I2C bus config message
        self.config_fmt = (
            "config_I2C_BD oid=%d sda_pin=%s scl_pin=%s delay=%s"
            " h_pos=%d z_adjust=%d"
            % (self.oid, sda_pin,scl_pin, delay_t,home_pose,z_adjust))
        self.cmd_queue = mcu.alloc_command_queue()
        mcu.register_config_callback(self.build_config)
        self.mcu.add_config_cmd(self.config_fmt)
        self.I2C_BD_send_cmd = self.I2C_BD_receive_cmd = None
    def build_config(self):
        #print ("self.config_fmt %s" % self.config_fmt)
        self.I2C_BD_send_cmd = self.mcu.lookup_command(
            "I2C_BD_send oid=%c data=%*s", cq=self.cmd_queue)
        self.I2C_BD_receive_cmd = self.mcu.lookup_query_command(
            "I2C_BD_receive oid=%c data=%*s",
            "I2C_BD_receive_response oid=%c response=%*s",
             oid=self.oid, cq=self.cmd_queue)

    def get_oid(self):
        return self.oid
    def get_mcu(self):
        return self.mcu
    def get_command_queue(self):
        return self.cmd_queue
    def I2C_BD_send(self, data):
        self.I2C_BD_send_cmd.send([self.oid, data.encode('utf-8')])
    def I2C_BD_receive(self,  data):
        return self.I2C_BD_receive_cmd.send([self.oid, data])


HINT_TIMEOUT = """
If the probe did not move far enough to trigger, then
consider reducing the Z axis minimum position so the probe
can travel further (the Z minimum position can be negative).
"""
class BDPrinterProbe:
    def __init__(self, config, mcu_probe):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.config = config
        self.mcu_probe = mcu_probe
        self.speed = config.getfloat('speed', 5.0, above=0.)
        self.lift_speed = config.getfloat('lift_speed', self.speed, above=0.)
        self.x_offset = config.getfloat('x_offset', 0.)
        self.y_offset = config.getfloat('y_offset', 0.)
        self.z_offset = config.getfloat('z_offset')
        self.probe_calibrate_z = 0.
        self.multi_probe_pending = False
        self.last_state = False
        self.last_z_result = 0.
        self.gcode_move = self.printer.load_object(config, "gcode_move")
        # Infer Z position to move to during a probe
        if config.has_section('stepper_z'):
            zconfig = config.getsection('stepper_z')
            self.z_position = zconfig.getfloat('position_min', 0.,
                                               note_valid=False)
        else:
            pconfig = config.getsection('printer')
            self.z_position = pconfig.getfloat('minimum_z_position', 0.,
                                               note_valid=False)
        # Multi-sample support (for improved accuracy)
        self.sample_count = config.getint('samples', 1, minval=1)
        self.sample_retract_dist = config.getfloat('sample_retract_dist', 2.,
                                                   above=0.)
        atypes = {'median': 'median', 'average': 'average'}
        self.samples_result = config.getchoice('samples_result', atypes,
                                               'average')
        self.samples_tolerance = config.getfloat('samples_tolerance', 0.100,
                                                 minval=0.)
        self.samples_retries = config.getint('samples_tolerance_retries', 0,
                                             minval=0)
        # Register z_virtual_endstop pin
        self.printer.lookup_object('pins').register_chip('probe', self)
        # Register homing event handlers
        self.printer.register_event_handler("homing:homing_move_begin",
                                            self._handle_homing_move_begin)
        self.printer.register_event_handler("homing:homing_move_end",
                                            self._handle_homing_move_end)
        self.printer.register_event_handler("homing:home_rails_begin",
                                            self._handle_home_rails_begin)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self._handle_home_rails_end)
        self.printer.register_event_handler("gcode:command_error",
                                            self._handle_command_error)
        # Register PROBE/QUERY_PROBE commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('PROBE', self.cmd_PROBE,
                                    desc=self.cmd_PROBE_help)
        self.gcode.register_command('QUERY_PROBE', self.cmd_QUERY_PROBE,
                                    desc=self.cmd_QUERY_PROBE_help)
        self.gcode.register_command('PROBE_CALIBRATE',
                                    self.cmd_PROBE_CALIBRATE,
                                    desc=self.cmd_PROBE_CALIBRATE_help)
        self.gcode.register_command('PROBE_ACCURACY', self.cmd_PROBE_ACCURACY,
                                    desc=self.cmd_PROBE_ACCURACY_help)
        self.gcode.register_command('Z_OFFSET_APPLY_PROBE',
                                    self.cmd_Z_OFFSET_APPLY_PROBE,
                                    desc=self.cmd_Z_OFFSET_APPLY_PROBE_help)
    def _handle_homing_move_begin(self, hmove):
        if self.mcu_probe in hmove.get_mcu_endstops():
            self.mcu_probe.probe_prepare(hmove)
    def _handle_homing_move_end(self, hmove):
        if self.mcu_probe in hmove.get_mcu_endstops():
            self.mcu_probe.probe_finish(hmove)
    def _handle_home_rails_begin(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        self.bedmesh = self.printer.lookup_object('bed_mesh', None)
        self.bedmesh.bmc.probe_helper = BDProbePointsHelper(
             self.config.getsection('bed_mesh'),
             self.bedmesh.bmc.probe_finalize,
             self.bedmesh.bmc._get_adjusted_points())
        #self.gcode.respond_info("3_handle_home_rails_begin")
        if self.mcu_probe in endstops:
            self.multi_probe_begin()
    def _handle_home_rails_end(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        if self.mcu_probe in endstops:
            self.multi_probe_end()
    def _handle_command_error(self):
        try:
            self.multi_probe_end()
        except:
            logging.exception("Multi-probe end")
    def multi_probe_begin(self):
        self.mcu_probe.multi_probe_begin()
        self.multi_probe_pending = True
    def multi_probe_end(self):
        if self.multi_probe_pending:
            self.multi_probe_pending = False
            self.mcu_probe.multi_probe_end()
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'z_virtual_endstop':
            raise pins.error("Probe virtual endstop only"
                             "useful as endstop pin")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert probe virtual endstop")
        return self.mcu_probe
    def get_lift_speed(self, gcmd=None):
        if gcmd is not None:
            return gcmd.get_float("LIFT_SPEED", self.lift_speed, above=0.)
        return self.lift_speed
    def get_offsets(self):
        return self.x_offset, self.y_offset, self.z_offset
    def _probe(self, speed):
        toolhead = self.printer.lookup_object('toolhead')
        curtime = self.printer.get_reactor().monotonic()
        if 'z' not in toolhead.get_status(curtime)['homed_axes']:
            raise self.printer.command_error("Must home before probe")
        phoming = self.printer.lookup_object('homing')
        pos = toolhead.get_position()
        pos[2] = self.z_position
        try:
            epos = phoming.probing_move(self.mcu_probe, pos, speed)
        except self.printer.command_error as e:
            reason = str(e)
            if "Timeout during endstop homing" in reason:
                reason += HINT_TIMEOUT
            raise self.printer.command_error(reason)
        time.sleep(0.1)
        b_value=self.mcu_probe.BD_Sensor_Read(2)
        pos_new = toolhead.get_position()
        epos[2] = epos[2]-b_value + self.mcu_probe.endstop_bdsensor_offset
       # toolhead.set_position(pos_new)
        self.gcode.respond_info("probe at %.3f,%.3f is z=%.6f"
                                "  (pos:%.6f - bd:%.3f) "
                                % (epos[0], epos[1], epos[2],
                                   pos_new[2],b_value))
        self.mcu_probe.homeing=0
        return epos[:3]
    def _move(self, coord, speed):
        self.printer.lookup_object('toolhead').manual_move(coord, speed)
    def _calc_mean(self, positions):
        count = float(len(positions))
        return [sum([pos[i] for pos in positions]) / count
                for i in range(3)]
    def _calc_median(self, positions):
        z_sorted = sorted(positions, key=(lambda p: p[2]))
        middle = len(positions) // 2
        if (len(positions) & 1) == 1:
            # odd number of samples
            return z_sorted[middle]
        # even number of samples
        return self._calc_mean(z_sorted[middle-1:middle+1])
    def run_probe(self, gcmd):
        speed = gcmd.get_float("PROBE_SPEED", self.speed, above=0.)
        lift_speed = self.get_lift_speed(gcmd)
        sample_count = gcmd.get_int("SAMPLES", self.sample_count, minval=1)
        sample_retract_dist = gcmd.get_float("SAMPLE_RETRACT_DIST",
                                             self.sample_retract_dist,above=0.)
        samples_tolerance = gcmd.get_float("SAMPLES_TOLERANCE",
                                           self.samples_tolerance, minval=0.)
        samples_retries = gcmd.get_int("SAMPLES_TOLERANCE_RETRIES",
                                       self.samples_retries, minval=0)
        samples_result = gcmd.get("SAMPLES_RESULT", self.samples_result)
        must_notify_multi_probe = not self.multi_probe_pending
        if must_notify_multi_probe:
            self.multi_probe_begin()
        probexy = self.printer.lookup_object('toolhead').get_position()[:2]
        retries = 0
        positions = []
        toolhead = self.printer.lookup_object('toolhead')
        #gcmd.respond_info("speed:%.3f"%speed)
        while len(positions) < sample_count:
            # Probe position
            try:
                if ((self.mcu_probe.bd_sensor is not None) and
                        (( "BED_MESH_CALIBRATE" in gcmd.get_command()) or
                        ("QUAD_GANTRY_LEVEL" in gcmd.get_command()))):
                    #pos = self._probe(speed)
                    toolhead.wait_moves()
                    time.sleep(0.004)
                    pos = toolhead.get_position()
                    intd=self.mcu_probe.BD_Sensor_Read(0)
                    pos[2]=pos[2]-intd + self.mcu_probe.endstop_bdsensor_offset
                    self.gcode.respond_info("probe at %.3f,%.3f is z=%.6f"
                                            % (pos[0], pos[1], pos[2]))
                    #return pos[:3]
                    positions.append(pos[:3])
                    # Check samples tolerance
                    z_positions = [p[2] for p in positions]
                    if max(z_positions) - min(z_positions) > samples_tolerance:
                        if retries >= samples_retries:
                            raise gcmd.error("Probe samples "
                                             "exceed samples_tolerance")
                        gcmd.respond_info("Probe samples exceed tolerance."
                                          "Retrying...")
                        retries += 1
                        positions = []
                    continue
            except Exception as e:
                gcmd.respond_info("%s"%str(e))
                raise gcmd.error("%s"%str(e))
                #pass
            pos = self._probe(speed)
            positions.append(pos)
            # Check samples tolerance
            z_positions = [p[2] for p in positions]
            if max(z_positions) - min(z_positions) > samples_tolerance:
                if retries >= samples_retries:
                    raise gcmd.error("Probe samples exceed samples_tolerance")
                gcmd.respond_info("Probe samples exceed tolerance. Retrying...")
                retries += 1
                positions = []
            # Retract
            if len(positions) < sample_count:
                self._move(probexy + [pos[2] + sample_retract_dist],lift_speed)
        if must_notify_multi_probe:
            self.multi_probe_end()
        # Calculate and return result
        if samples_result == 'median':
            return self._calc_median(positions)
        return self._calc_mean(positions)

    cmd_PROBE_help = "Probe Z-height at current XY position"
    def cmd_PROBE(self, gcmd):
        pos = self.run_probe(gcmd)
        gcmd.respond_info("Result is z=%.6f" % (pos[2],))
        self.last_z_result = pos[2]
    cmd_QUERY_PROBE_help = "Return the status of the z-probe"
    def cmd_QUERY_PROBE(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        res = self.mcu_probe.query_endstop(print_time)
        self.last_state = res
        gcmd.respond_info("probe: %s" % (["open", "TRIGGERED"][not not res],))
    def get_status(self, eventtime):
        return {'name': self.name,
                'last_query': self.last_state,
                'last_z_result': self.last_z_result}
    cmd_PROBE_ACCURACY_help = "Probe Z-height accuracy at current XY position"
    def cmd_PROBE_ACCURACY(self, gcmd):
        speed = gcmd.get_float("PROBE_SPEED", self.speed, above=0.)
        lift_speed = self.get_lift_speed(gcmd)
        sample_count = gcmd.get_int("SAMPLES", 10, minval=1)
        sample_retract_dist = gcmd.get_float("SAMPLE_RETRACT_DIST",
                                             self.sample_retract_dist,above=0.)
        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        pos[2] = 1.0
        gcmd.respond_info("PROBE_ACCURACY at X:%.3f Y:%.3f Z:%.3f"
                          " (samples=%d retract=%.3f"
                          " speed=%.1f lift_speed=%.1f)\n"
                          % (pos[0], pos[1], pos[2],
                             sample_count, sample_retract_dist,
                             speed, lift_speed))
        # Probe bed sample_count times
        #self.multi_probe_begin()
        toolhead.manual_move([None, None, pos[2]], speed)
        toolhead.wait_moves()
        positions = []
        while len(positions) < sample_count:
            time.sleep(0.3)
            pos[2]=self.mcu_probe.BD_Sensor_Read(2)
            ## Probe position
            #pos = self._probe(speed)
            positions.append(pos)
            ## Retract
            #liftpos = [None, None, pos[2] + sample_retract_dist]
            #self._move(liftpos, lift_speed)
        #self.multi_probe_end()
        # Calculate maximum, minimum and average values
        max_value = max([p[2] for p in positions])
        min_value = min([p[2] for p in positions])
        range_value = max_value - min_value
        avg_value = self._calc_mean(positions)[2]
        median = self._calc_median(positions)[2]
        # calculate the standard deviation
        deviation_sum = 0
        for i in range(len(positions)):
            deviation_sum += pow(positions[i][2] - avg_value, 2.)
        sigma = (deviation_sum / len(positions)) ** 0.5
        # Show information
        gcmd.respond_info(
            "probe accuracy results: maximum %.6f, minimum %.6f, range %.6f, "
            "average %.6f, median %.6f, standard deviation %.6f" % (
            max_value, min_value, range_value, avg_value, median, sigma))
    def probe_calibrate_finalize(self, kin_pos):
        if kin_pos is None:
            return
        z_offset = self.probe_calibrate_z - kin_pos[2]
        self.gcode.respond_info(
            "%s: z_offset: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer." % (self.name, z_offset))
        configfile = self.printer.lookup_object('configfile')
        #configfile.set(self.name, 'z_offset', "%.3f" % (z_offset,))
    cmd_PROBE_CALIBRATE_help = "Calibrate the probe's z_offset"
    def cmd_PROBE_CALIBRATE(self, gcmd):
        manual_probe.verify_no_manual_probe(self.printer)
        # Perform initial probe
        lift_speed = self.get_lift_speed(gcmd)
        curpos = self.run_probe(gcmd)
        # Move away from the bed
        self.probe_calibrate_z = curpos[2]
        curpos[2] += 5.
        self._move(curpos, lift_speed)
        # Move the nozzle over the probe point
        curpos[0] += self.x_offset
        curpos[1] += self.y_offset
        self._move(curpos, self.speed)
        # Start manual probe
        manual_probe.ManualProbeHelper(self.printer, gcmd,
                                       self.probe_calibrate_finalize)
    def cmd_Z_OFFSET_APPLY_PROBE(self,gcmd):
        offset = self.gcode_move.get_status()['homing_origin'].z
        configfile = self.printer.lookup_object('configfile')
        if offset == 0:
            self.gcode.respond_info("Nothing to do: Z Offset is 0")
        else:
            new_calibrate = self.z_offset - offset
            self.gcode.respond_info(
                "%s: z_offset: %.3f\n"
                "The SAVE_CONFIG command will update the printer config file\n"
                "with the above and restart the printer."
                % (self.name, new_calibrate))
            #configfile.set(self.name, 'z_offset', "%.3f" % (new_calibrate,))
    cmd_Z_OFFSET_APPLY_PROBE_help = "Adjust the probe's z_offset"

# Helper code that can probe a series of points and report the
# position at each point.
class BDProbePointsHelper:
    def __init__(self, config, finalize_callback, default_points=None):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.finalize_callback = finalize_callback
        self.probe_points = default_points
        self.name = config.get_name()
        self.gcode = self.printer.lookup_object('gcode')
        # Read config settings
        if default_points is None or config.get('points', None) is not None:
            self.probe_points = config.getlists('points', seps=(',', '\n'),
                                                parser=float, count=2)
        def_move_z = config.getfloat('horizontal_move_z', 5.)
        self.default_horizontal_move_z = def_move_z
        self.speed = config.getfloat('speed', 50., above=0.)
        self.use_offsets = True#False
        # Internal probing state
        self.lift_speed = self.speed
        self.probe_offsets = (0., 0., 0.)
        self.results = []
    def minimum_points(self,n):
        if len(self.probe_points) < n:
            raise self.printer.config_error(
                "Need at least %d probe points for %s" % (n, self.name))
    def update_probe_points(self, points, min_points):
        self.probe_points = points
        self.minimum_points(min_points)
    def use_xy_offsets(self, use_offsets):
        self.use_offsets = use_offsets
    def get_lift_speed(self):
        return self.lift_speed
    def _move_next(self):
        toolhead = self.printer.lookup_object('toolhead')
        # Lift toolhead
        speed = self.lift_speed
        if not self.results:
            # Use full speed to first probe position
            speed = self.speed
        toolhead.manual_move([None, None, self.horizontal_move_z], speed)
        # Check if done probing
        if len(self.results) >= len(self.probe_points):
            toolhead.get_last_move_time()
            res = self.finalize_callback(self.probe_offsets, self.results)
            if res != "retry":
                return True
            self.results = []
        # Move to next XY probe point
        nextpos = list(self.probe_points[len(self.results)])
        if self.use_offsets:
            nextpos[0] -= self.probe_offsets[0]
            nextpos[1] -= self.probe_offsets[1]
        toolhead.manual_move(nextpos, self.speed)
        return False

    def fast_probe_oneline(self, direction):
        probe = self.printer.lookup_object('probe', None)
        toolhead = self.printer.lookup_object('toolhead')
        oneline_points = []
        start_point = []
        if "backward" in  direction:
            start_point=list(self.probe_points[len(self.results_copy)-1])
        else:
            start_point=list(self.probe_points[len(self.results)])
        end_point = []
        for point in self.probe_points:
            if start_point[1] == point[1]:
                oneline_points.append(point)
        n_count=len(oneline_points)

        if n_count<1:
            raise self.printer.config_error(
                "Seems the mesh direction is not X, points count on x is %d"
                % (n_count))
        if "backward" in  direction:
            oneline_points.reverse()
        end_point = list(oneline_points[n_count-1])
        if self.use_offsets:
            start_point[0] -= self.probe_offsets[0]
            start_point[1] -= self.probe_offsets[1]
            end_point[0] -= self.probe_offsets[0]
            end_point[1] -= self.probe_offsets[1]
        toolhead.manual_move(start_point, self.speed)
        toolhead.wait_moves()
        if n_count == 1:
            toolhead.wait_moves()
            pos = toolhead.get_position()
            if self.use_offsets:
                pos[0] -= self.probe_offsets[0]
                pos[1] -= self.probe_offsets[1]
            intd=probe.mcu_probe.BD_Sensor_Read(0)
            pos[2]=pos[2]-intd
            if "backward" in  direction:
                self.results_1.append(pos)
                self.results_copy.pop()
            else:
                self.results.append(pos)
            return
        toolhead.manual_move(end_point, self.speed)
        ####
        toolhead._flush_lookahead()
        curtime = toolhead.reactor.monotonic()
        est_time =toolhead.mcu.estimated_print_time(curtime)
        line_time = toolhead.print_time-est_time
        start_time = est_time
        x_index = 0
        while (not toolhead.special_queuing_state
               or toolhead.print_time >= est_time):
            if not toolhead.can_pause:
                break
            est_time =toolhead.mcu.estimated_print_time(curtime)
            if (est_time-start_time) >= x_index*line_time/(n_count-1):
                pos = toolhead.get_position()
                pos[0] = oneline_points[x_index][0]
                pos[1] = oneline_points[x_index][1]
                if self.use_offsets:
                    pos[0] -= self.probe_offsets[0]
                    pos[1] -= self.probe_offsets[1]
                intd=probe.mcu_probe.BD_Sensor_Read(0)
                pos[2]=pos[2]-intd
                if "backward" in  direction:
                    self.results_1.append(pos)
                    self.results_copy.pop()
                else:
                    self.results.append(pos)
                x_index += 1;
            curtime = toolhead.reactor.pause(curtime + 0.001)

    def fast_probe(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        probe = self.printer.lookup_object('probe', None)
        speed = self.lift_speed
        if not self.results:
            # Use full speed to first probe position
            speed = self.speed
        toolhead.manual_move([None, None, self.horizontal_move_z], speed)
        self.results = []
        while len(self.results) < len(self.probe_points):
            self.fast_probe_oneline("forward")
        self.results_copy = copy.copy(self.results)
        self.results_1 = []
        while len(self.results_1) < len(self.probe_points):
            self.fast_probe_oneline("backward")
        self.results_1.reverse()
        #print("results_1_1:",self.results_1)
        for index in range(len(self.results)):
            self.results[index][2] =  (self.results[index][2] + \
                self.results_1[index][2])/2 + \
                probe.mcu_probe.endstop_bdsensor_offset
            if index < 1000:
                probe.gcode.respond_info("probe at %.3f,%.3f is z=%.6f"
                                        % (self.results[index][0],
                                        self.results[index][1],
                                        self.results[index][2]))
        res = self.finalize_callback(self.probe_offsets, self.results)
        self.results = []
        self.results_1 = []
        if res != "retry":
            return True

    def start_probe(self, gcmd):
        manual_probe.verify_no_manual_probe(self.printer)
        # Lookup objects
        probe = self.printer.lookup_object('probe', None)
        method = gcmd.get('METHOD', 'automatic').lower()
        self.results = []
        def_move_z = self.default_horizontal_move_z
        self.horizontal_move_z = gcmd.get_float('HORIZONTAL_MOVE_Z',
                                                def_move_z)
        if probe is None or method != 'automatic':
            # Manual probe
            self.lift_speed = self.speed
            self.probe_offsets = (0., 0., 0.)
            self._manual_probe_start()
            return
        # Perform automatic probing
        self.lift_speed = probe.get_lift_speed(gcmd)
        self.probe_offsets = probe.get_offsets()
        if self.horizontal_move_z < self.probe_offsets[2]:
            raise gcmd.error("horizontal_move_z can't be less than"
                             " probe's z_offset")
        probe.multi_probe_begin()
        gcmd.respond_info("BDsensor:gcode %s"%gcmd.get_command())
        if "BED_MESH_CALIBRATE" in gcmd.get_command():
            try:
                if probe.mcu_probe.no_stop_probe is not None:
                    self.fast_probe(gcmd)
                    probe.multi_probe_end()
                    return
            except AttributeError as e:
                gcmd.respond_info("%s"%str(e))
                raise gcmd.error("%s"%str(e))
                #pass

        while 1:
            done = self._move_next()
            if done:
                break
            pos = probe.run_probe(gcmd)
            self.results.append(pos)
        probe.multi_probe_end()
    def _manual_probe_start(self):
        done = self._move_next()
        if not done:
            gcmd = self.gcode.create_gcode_command("", "", {})
            manual_probe.ManualProbeHelper(self.printer, gcmd,
                                           self._manual_probe_finalize)
    def _manual_probe_finalize(self, kin_pos):
        if kin_pos is None:
            return
        self.results.append(kin_pos)
        self._manual_probe_start()


# BDsensor wrapper that enables probe specific features
# set this type of sda_pin 2 as virtual endstop
# add new gcode command M102 for BDsensor
class BDsensorEndstopWrapper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.name = config.get_name()
        self.z_adjust = config.getfloat('z_adjust',0., minval=-0.3,below=0.3)
        self.z_offset = config.getfloat('z_offset',0., minval=0.,maxval=0.0)
        self.position_endstop = config.getfloat('position_endstop',0.,
                                                minval=0.,below=2)
        if self.z_adjust > self.position_endstop :
            raise self.printer.command_error("The 'z_adjust' cannot be greater"
            " than 'position_endstop' in section [BDsensor]")
        self.stow_on_each_sample = config.getboolean(
            'deactivate_on_each_sample', True)
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.activate_gcode = gcode_macro.load_template(
            config, 'activate_gcode', '')
        self.deactivate_gcode = gcode_macro.load_template(
            config, 'deactivate_gcode', '')
        self.printer.register_event_handler('klippy:mcu_identify',
                                            self._handle_mcu_identify)
        # Create an "endstop" object to handle the probe pin
        self.collision_homing = config.getint('collision_homing', 0)
        self.switch_mode = 0
        ppins = self.printer.lookup_object('pins')
        #self.mcu_pwm = ppins.setup_pin('pwm', config.get('scl_pin'))
        self.bdversion = ''
        # Command timing
        self.next_cmd_time = self.action_end_time = 0.
        self.finish_home_complete = self.wait_trigger_complete = None
        pin = config.get('sda_pin')
        pin_params = ppins.lookup_pin(pin, can_invert=False, can_pullup=True)
        mcu = pin_params['chip']
        self.sda_pin_num = pin_params['pin']
        self.mcu = mcu
        #print("b2:%s"%mcu)
        pin_s = []
        try:
            pin_s=config.get('scl_pin')
        except Exception as e:
            try:
                pin_s=config.get('clk_pin')
            except Exception as e:
                raise self.printer.command_error("%s"%str(e))
            pass
        pin_params = ppins.lookup_pin(pin_s, can_invert=False, can_pullup=True)
        mcu = pin_params['chip']
        scl_pin_num = pin_params['pin']
        #print("b3:%s"%mcu)
        #pin_params['pullup']=2
        #self.mcu_endstop = mcu.setup_pin('endstop', pin_params)
        self._invert = pin_params['invert']

        self.oid = self.mcu.create_oid()

        self.mcu_endstop = self.mcu
        self._invert_endstop =  self._invert
        self.oid_endstop = self.oid
        self.endstop_pin_num = self.sda_pin_num
        self.endstop_bdsensor_offset = 0
        try:
            pin = config.get('endstop_pin')
            pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
            self.endstop_pin_num = pin_params['pin']
            self.mcu_endstop = pin_params['chip']
            self._invert_endstop =  pin_params['invert']
            if self.mcu_endstop is not self.mcu:
                self.oid_endstop = self.mcu_endstop.create_oid()
        except Exception as e:
            pass
        self.cmd_queue = self.mcu.alloc_command_queue()
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.trapq = ffi_main.gc(ffi_lib.trapq_alloc(), ffi_lib.trapq_free)
        self.trapq_append = ffi_lib.trapq_append
        self.trapq_finalize_moves = ffi_lib.trapq_finalize_moves
        self.stepper_kinematics = ffi_main.gc(
            ffi_lib.cartesian_stepper_alloc(b'x'), ffi_lib.free)
        home_pos = self.position_endstop*100
        z_adjust = self.z_adjust*100
        self.bd_sensor=MCU_I2C_BD(mcu,self.sda_pin_num,scl_pin_num,
                                  config.get('delay'),home_pos,z_adjust)
        #MCU_BD_I2C_from_config(self.mcu,config)
        self.distance=5;
        # Register M102 commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('M102', self.cmd_M102)
        self.gcode.register_command('BDSENSOR_VERSION', self.BD_version)
        self.gcode.register_command('BDSENSOR_CALIBRATE', self.BD_calibrate)
        self.gcode.register_command('BDSENSOR_READ_CALIBRATION',
                                    self.BD_read_calibration)
        self.gcode.register_command('BDSENSOR_DISTANCE', self.BD_distance)
        self.gcode.register_command('BDSENSOR_SET', self.BD_set)
        self.no_stop_probe = None
        self.no_stop_probe = config.get('no_stop_probe', None)
        self.I2C_BD_receive_cmd2 = None
        self.gcode_move = self.printer.load_object(config, "gcode_move")
        self.gcode = self.printer.lookup_object('gcode')
        # Wrappers
        self.process_m102=0
        self.gcode_que=None
        self.zl=0
        self.bd_value=10.24
        self.x_offset = config.getfloat('x_offset', 0.)
        self.y_offset = config.getfloat('y_offset', 0.)
        self.results = []
        self.finish_home_complete = self.wait_trigger_complete = None
        # multi probes state
        self.multi = 'OFF'
        self.mcu.register_config_callback(self.build_config)
        self.adjust_range=0;
        self.old_count=1000;
        self.homeing=0
        self.reactor = self.printer.get_reactor()
        self.status_dis = None
        try:
            status_dis=self.printer.lookup_object('display_status')
        except Exception as e:
            pass
        self._rest_ticks = 0
        ffi_main, ffi_lib = chelper.get_ffi()
        self._trdispatch = ffi_main.gc(ffi_lib.trdispatch_alloc(),ffi_lib.free)
        self._trsyncs = [MCU_trsync(self.mcu_endstop, self._trdispatch)]
        self.ncont =0
        self.bedmesh  = self.printer.lookup_object('bed_mesh', None)
        self.z_last= 0

    def get_mcu(self):
        return self.mcu

    def build_config(self):
        self.I2C_BD_receive_cmd = self.mcu.lookup_query_command(
            "I2C_BD_receive oid=%c data=%*s",
            "I2C_BD_receive_response oid=%c response=%*s",
            oid=self.oid, cq=self.cmd_queue)
        self.mcu.register_response(self._handle_BD_Update,
                                    "BD_Update", self.bd_sensor.oid)
        self.mcu.register_response(self._handle_probe_Update,
                                    "X_probe_Update", self.bd_sensor.oid)
        self.mcu_endstop.add_config_cmd(
            "BDendstop_home oid=%d clock=0 sample_ticks=0 sample_count=0"
            " rest_ticks=0 pin_value=0 trsync_oid=0 trigger_reason=0"
            " endstop_pin=%s"
            % (self.oid_endstop,self.endstop_pin_num), on_restart=True)
        # Lookup commands
        cmd_queue = self._trsyncs[0].get_command_queue()
        self._home_cmd = self.mcu_endstop.lookup_command(
            "BDendstop_home oid=%c clock=%u sample_ticks=%u sample_count=%c"
            " rest_ticks=%u pin_value=%c trsync_oid=%c trigger_reason=%c"
            " endstop_pin=%c",
            cq=cmd_queue)
    def _handle_BD_Update(self, params):
        #print("_handle_BD_Update :%s " %params['distance_val'])
        try:
            self.bd_value=int(params['distance_val'])/100.00
            if self.status_dis is not None:
                strd=str(self.bd_value)+"mm"
            if self.bd_value == 10.24:
                strd="BDs:ConnectErr"
            if self.bd_value == 3.9:
               strd="BDs:Out Range"
            self.status_dis.message=strd
        except ValueError as e:
            pass
        #else:
            #print (" handle self.bd_value %.4f" % self.bd_value)
    def _handle_probe_Update(self, params):
        print("_handle_probe_Update:%s " %params['distance_val'])
        #print ("split :%s " %params['distance_val'].split(b' '))
        count=int(params['distance_val'].split(b' ')[1])
        print(len(self.results))

        self.old_count=count
        print ("split:%s " %params['distance_val'].split(b' '))
        try:
            self.results.append(int(params['distance_val'].split(b' ')[0]))
        except ValueError as e:
            pass

    def _force_enable(self,stepper):
        self.toolhead = self.printer.lookup_object('toolhead')
        print_time = self.toolhead.get_last_move_time()
        stepper_enable = self.printer.lookup_object('stepper_enable')
        enable = stepper_enable.lookup_enable(stepper.get_name())
        was_enable = enable.is_motor_enabled()
        STALL_TIME = 0.100
        if not was_enable:
            enable.motor_enable(print_time)
            self.toolhead.dwell(STALL_TIME)
        return was_enable

    def manual_move(self, stepper, dist, speed, accel=0.):
        self.toolhead = self.printer.lookup_object('toolhead')
        self.toolhead.flush_step_generation()
        prev_sk = stepper.set_stepper_kinematics(self.stepper_kinematics)
        prev_trapq = stepper.set_trapq(self.trapq)
        stepper.set_position((0., 0., 0.))
        axis_r, accel_t,cruise_t,cruise_v=calc_move_time(dist, speed, accel)
        print_time = self.toolhead.get_last_move_time()
        self.trapq_append(self.trapq, print_time, accel_t, cruise_t, accel_t,
                          0., 0., 0., axis_r, 0., 0., 0., cruise_v, accel)
        print_time = print_time + accel_t + cruise_t + accel_t
        stepper.generate_steps(print_time)
        self.trapq_finalize_moves(self.trapq, print_time + 99999.9)
        stepper.set_trapq(prev_trapq)
        stepper.set_stepper_kinematics(prev_sk)
        self.toolhead.note_kinematic_activity(print_time)
        self.toolhead.dwell(accel_t + cruise_t + accel_t)

    def cmd_M102(self, gcmd, wait=False):
         #self.gcode_que=gcmd
         self.process_M102(gcmd)
    def BD_Sensor_Read(self,fore_r):
        if fore_r > 0:
            self.bd_sensor.I2C_BD_send("1018")#1015   read distance data
        pr = self.I2C_BD_receive_cmd.send([self.oid, "32".encode('utf-8')])
        intr = int(pr['response'])
        if intr >= 1024:
            pr = self.I2C_BD_receive_cmd.send([self.oid, "32".encode('utf-8')])
            intr = int(pr['response'])
        self.bd_value=intr/100.00
        if fore_r == 0:
            if self.bd_value >= 10.24:
                raise self.printer.command_error("Bed Distance Sensor "
                                   "data error:%.2f" % (self.bd_value))
            elif self.bd_value > 3.8:
                raise self.printer.command_error("Bed Distance Sensor, "
                               "out of range.:%.2f " % (self.bd_value))
        elif fore_r == 2:
            if self.bd_value >= 10.24:
                raise self.printer.command_error("Bed Distance Sensor"
                                 "data error:%.2f" % (self.bd_value))

        return self.bd_value

    def BD_version(self, gcmd):
        self.bd_sensor.I2C_BD_send("1016")#1016 // // read sensor version
        self.bd_sensor.I2C_BD_send("1016")
        self.toolhead = self.printer.lookup_object('toolhead')
        ncount1=0
        x=[]
        while 1:
            pr=self.I2C_BD_receive_cmd.send([self.oid,"3".encode('utf-8')])
          #  print"params:%s" % pr['response']
            intd=int(pr['response'])
            if intd>127:
                intd=127
            if intd<0x20:
                intd=0x20
            x.append(intd)
           # self.toolhead.dwell(0.1)
            ncount1=ncount1+1
            if ncount1>=20:
                self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
                res = ''.join(map(chr, x))
                self.bdversion = res
                #self.gcode.respond_info('BDsensor version:'+self.bdversion)
                break
        self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
        self.bd_sensor.I2C_BD_send("1018")
        if "V1.1b" in self.bdversion or "V1.2b" in self.bdversion \
                             or "V1.2c" in self.bdversion:#switch mode
            self.switch_mode = 1
        if "andapi" in self.bdversion:
            self.gcode.respond_info("BDsensorVer:%s,switch_mode=%d,"
                                    "collision_homing=%d"
                                    %(self.bdversion,self.switch_mode,
                                      self.collision_homing))
        else:
            self.gcode.respond_info("No data from BDsensor,"
                                    "please check connection")

    def BD_calibrate(self, gcmd):
        if "V1." not in self.bdversion:
            self.BD_version(self.gcode)
        if self.switch_mode == 1 and self.collision_homing == 1:
            gcmd.respond_info("Homing")
            self.gcode.run_script_from_command("G28")
            self.gcode.run_script_from_command("G1 Z0")
        gcmd.respond_info("Calibrating, don't power off the printer")
        self.toolhead = self.printer.lookup_object('toolhead')
        kin = self.toolhead.get_kinematics()
        self.bd_sensor.I2C_BD_send("1019")
        self.bd_sensor.I2C_BD_send("1019")
        #distance = 0.5#gcmd.get_float('DISTANCE')
        speed = 5#gcmd.get_float('VELOCITY', above=0.)
        accel = 1000#gcmd.get_float('ACCEL', 0., minval=0.)
        self.distance=0.1
        for stepper in kin.get_steppers():
            #if stepper.is_active_axis('z'):
            self._force_enable(stepper)
            self.toolhead.wait_moves()
        ncount=0
        gcmd.respond_info("Please Wait... ")
        self.gcode.run_script_from_command("SET_KINEMATIC_POSITION Z=0")
        self.toolhead.dwell(0.8)
        while 1:
            self.bd_sensor.I2C_BD_send(str(ncount))
            self.bd_sensor.I2C_BD_send(str(ncount))
            self.bd_sensor.I2C_BD_send(str(ncount))
            self.bd_sensor.I2C_BD_send(str(ncount))
            self.toolhead.dwell(0.2)
            self.gcode.run_script_from_command("G91")
            self.gcode.run_script_from_command("G1 Z+0.1 F1500")
            self.gcode.run_script_from_command("G90")
            self.toolhead.wait_moves()
            self.toolhead.dwell(0.2)
            ncount=ncount+1
            if ncount>=40:
                self.bd_sensor.I2C_BD_send("1021")
                self.toolhead.dwell(1)
                gcmd.respond_info("Calibrate Finished!")
                gcmd.respond_info("You can send command "
                    "BDSENSOR_READ_CALIBRATION to check the calibration data")
                self.z_adjust = 0
                configfile = self.printer.lookup_object('configfile')
                configfile.set(self.name, 'z_adjust', "0.0")
                break
        self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
        self.bd_sensor.I2C_BD_send("1018")

    def BD_read_calibration(self, gcmd):
        self.bd_sensor.I2C_BD_send("1017")#tart read raw calibrate data
        self.bd_sensor.I2C_BD_send("1017")
        self.toolhead = self.printer.lookup_object('toolhead')
        ncount1=0
        while 1:
            pr=self.I2C_BD_receive_cmd.send([self.oid,"3".encode('utf-8')])
            intd=int(pr['response'])
            strd=str(intd)
            gcmd.respond_raw(strd)
            if ncount1 <= 3 and intd > 500 :
                gcmd.respond_raw("BDSensor mounted too high!"
                                 "0.4mm to 2.4mm from BED at"
                                 "zero position is recommended")
                #break
            if intd < 45 :
                gcmd.respond_raw("BDSensor mounted too close! please mount"
                                 "the BDsensor 0.2~0.4mm higher")
               # break
            self.toolhead.dwell(0.1)
            ncount1=ncount1+1
            if ncount1>=40:
                break
        self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
        self.bd_sensor.I2C_BD_send("1018")

    def BD_distance(self, gcmd):
        self.bd_value=self.BD_Sensor_Read(1)
        strd=str(self.bd_value)+"mm"
        if self.bd_value == 10.24:
            strd+=" BDsensor:Connection Error or not calibrated"
        elif self.bd_value >= 3.9:
            strd+=" BDsensor:Out of measure Range or too close to the bed"
        gcmd.respond_raw(strd)

        self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
        self.bd_sensor.I2C_BD_send("1018")

    def BD_set(self, gcmd):
        CMD_BD=0.0
        CMD_BD = gcmd.get_float('Z_ADJUST', None)
        try:
            gcmd.respond_info("z_adjust:%f, recommend to move the nozzle "
                              "close to bed and calibrate again the BDsensor"
                              "instead of chang z_adjust" % CMD_BD)
            if CMD_BD >= 0.3:
                CMD_BD = 0.29
                gcmd.respond_info("it has been set to the max value 0.29")
            elif CMD_BD <= -0.3:
                CMD_BD = -0.29
                gcmd.respond_info("it has been set to the min value -0.29")
            self.z_adjust = CMD_BD
            configfile = self.printer.lookup_object('configfile')
            configfile.set(self.name, 'z_adjust', "%.3f" % (CMD_BD))
            gcmd.respond_info("The SAVE_CONFIG command will update"
                              "the printer config")
            return
        except Exception as e:
            pass

        CMD_BD = gcmd.get_float('REAL_TIME_HEIGHT', None)
        try:
            self.BD_real_time(CMD_BD)
        except Exception as e:
            pass
        CMD_BD = gcmd.get_int('NO_STOP_PROBE', None)
        try:
            configfile = self.printer.lookup_object('configfile')
            configfile.set(self.name, 'no_stop_probe', "%d" % (CMD_BD))
            gcmd.respond_info("no_stop_probe is setted:%d The SAVE_CONFIG"
                              "command will update the printer config",CMD_BD)
        except Exception as e:
            pass

    def BD_real_time(self, BD_height):
        if BD_height >= 3.0:
            BD_height = 3
        elif BD_height < 0.0:
            BD_height = 0
        self.gcode.respond_info("Real time leveling height:%f  " % BD_height)
        self.adjust_range = int((BD_height+0.01)*1000)
        self.bd_sensor.I2C_BD_send("1022")
       # step_time=100
        self.toolhead = self.printer.lookup_object('toolhead')
        kin = self.toolhead.get_kinematics()
        z_index = 0
        for stepper in kin.get_steppers():
         if stepper.is_active_axis('z'):
             steps_per_mm = 1.0/stepper.get_step_dist()
             z=self.gcode_move.last_position[2]
             stepper._query_mcu_position()
             invert_dir, orig_invert_dir = stepper.get_dir_inverted()
             z=int(z*1000)
             #print("z step_at_zero:%d"% z)
             pr=self.Z_Move_Live_cmd.send([self.oid, ("0 %d\0"
                 % z_index).encode('utf-8')])
             pr=self.Z_Move_Live_cmd.send([self.oid, ("1 %d\0"
                 % z).encode('utf-8')])
             pr=self.Z_Move_Live_cmd.send([self.oid, ("2 %u\0"
                 % self.adjust_range).encode('utf-8')])
             pr=self.Z_Move_Live_cmd.send([self.oid, ("3 %u\0"
                 % orig_invert_dir).encode('utf-8')])
             pr=self.Z_Move_Live_cmd.send([self.oid, ("4 %u\0"
                 % steps_per_mm).encode('utf-8')])
             #pr=self.Z_Move_Live_cmd.send([self.oid, ("5 %u\0"
             #    % step_time).encode('utf-8')])
             pr=self.Z_Move_Live_cmd.send([self.oid, ("6 %u\0"
                 % stepper.get_oid()).encode('utf-8')])
             z_index = z_index + 1
        self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
    def process_M102(self, gcmd):
        self.process_m102=1
        try:
            CMD_BD = gcmd.get_int('S', None)
        except Exception as e:
            pass
            return
        self.toolhead = self.printer.lookup_object('toolhead')
        if CMD_BD == -6:
           self.BD_calibrate(gcmd)
        elif  CMD_BD == -5:
           self.BD_read_calibration(gcmd)
        elif  CMD_BD == -1:
           self.BD_version(gcmd)
        elif  CMD_BD == -2:# gcode M102 S-2 read distance data
            self.BD_distance(gcmd)
        elif  CMD_BD ==-7:
            self.bd_sensor.I2C_BD_send("1020") #read raw data
            return;
        elif  CMD_BD ==-8:
            self.bd_sensor.I2C_BD_send("1022") #reboot sensor
        elif  CMD_BD ==-9:
            self.bd_sensor.I2C_BD_send("1023") #reboot sensor
            self.bd_sensor.I2C_BD_send(str(int(self.position_endstop*100)))
            self.gcode.respond_info("in switch mode, the endstop position is"
                "%.3f mm"%(self.position_endstop))
            return
        else:
            return
        self.bd_sensor.I2C_BD_send("1018")#1018// finish reading
        self.bd_sensor.I2C_BD_send("1018")
        #self.process_m102=0
    def _handle_mcu_identify(self):
        #print("BD _handle_mcu_identify")
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self.add_stepper(stepper)
    def raise_probe(self):
        print("BD raise_probe")
        return
        self.toolhead = self.printer.lookup_object('toolhead')
        start_pos = self.toolhead.get_position()
        self.deactivate_gcode.run_gcode_from_command()
        if self.toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe activate_gcode script")
    def lower_probe(self):
        print("BD lower_probe0")
        return
        self.toolhead = self.printer.lookup_object('toolhead')
        start_pos = self.toolhead.get_position()
        self.activate_gcode.run_gcode_from_command()
        if self.toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe deactivate_gcode script")

    def query_endstop(self, print_time):
        self.bd_value=self.BD_Sensor_Read(2)
        params = 1 #trigered
        if self.bd_value > self.position_endstop:# open
           params=0
        return params

    def add_stepper(self, stepper):
        trsyncs = {trsync.get_mcu(): trsync for trsync in self._trsyncs}
        trsync = trsyncs.get(stepper.get_mcu())
        if trsync is None:
            trsync = MCU_trsync(stepper.get_mcu(), self._trdispatch)
            self._trsyncs.append(trsync)
        trsync.add_stepper(stepper)
        # Check for unsupported multi-mcu shared stepper rails
        sname = stepper.get_name()
        if sname.startswith('stepper_'):
            for ot in self._trsyncs:
                for s in ot.get_steppers():
                    if ot is not trsync and s.get_name().startswith(sname[:9]):
                        cerror = self.mcu.get_printer().config_error
                        raise cerror("Multi-mcu homing not supported on"
                                     " multi-mcu shared axis")
    def get_steppers(self):
        return [s for trsync in self._trsyncs for s in trsync.get_steppers()]
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        self.BD_Sensor_Read(2)
        if "V1." not in self.bdversion:
            self.BD_version(self.gcode)
        clock = self.mcu_endstop.print_time_to_clock(print_time)
        rest_ticks = \
           self.mcu_endstop.print_time_to_clock(print_time+rest_time)\
            - clock
        self._rest_ticks = rest_ticks
        reactor = self.mcu_endstop.get_printer().get_reactor()
        self.wait_trigger_complete = \
            reactor.register_callback(self.wait_for_trigger)
        self.trigger_completion = reactor.completion()
        expire_timeout = TRSYNC_TIMEOUT
        if len(self._trsyncs) == 1:
            expire_timeout = TRSYNC_SINGLE_MCU_TIMEOUT
        for i, trsync in enumerate(self._trsyncs):
            report_offset = float(i) / len(self._trsyncs)
            trsync.start(print_time, report_offset,
                         self.trigger_completion, expire_timeout)
        self.etrsync = self._trsyncs[0]
        ffi_main, ffi_lib = chelper.get_ffi()
        ffi_lib.trdispatch_start(self._trdispatch,
            self.etrsync.REASON_HOST_REQUEST)
        self.homeing=1
        if self.switch_mode == 1:
            self.bd_sensor.I2C_BD_send("1023")
            sample_time =.005
            sample_count =2
            if self.collision_homing == 1:
                self.bd_sensor.I2C_BD_send(str(1))
            else:
                self.bd_sensor.I2C_BD_send(str(int(self.position_endstop*100)))
        else:
            sample_time =.03
            sample_count =1
        self._home_cmd.send(
             [self.oid_endstop, clock,
             self.mcu_endstop.seconds_to_clock(sample_time),
             sample_count, self.mcu_endstop.seconds_to_clock(sample_time),
             triggered ^ self._invert_endstop,self.etrsync.get_oid(),
             self.etrsync.REASON_ENDSTOP_HIT,self.endstop_pin_num],
             reqclock=clock)

        self.finish_home_complete = self.trigger_completion
        return self.trigger_completion

    def wait_for_trigger(self, eventtime):
        self.trigger_completion.wait()
        if self.multi == 'OFF':
            self.raise_probe()

    def home_wait(self, home_end_time):
        etrsync = self._trsyncs[0]
        etrsync.set_home_end_time(home_end_time)
        if self.mcu_endstop.is_fileoutput():
            self.trigger_completion.complete(True)
        self.trigger_completion.wait()
        self._home_cmd.send([self.oid_endstop, 0, 0, 0, 0, 0, 0, 0,
                            self.endstop_pin_num])
        ffi_main, ffi_lib = chelper.get_ffi()
        ffi_lib.trdispatch_stop(self._trdispatch)
        res = [trsync.stop() for trsync in self._trsyncs]
        if any([r == etrsync.REASON_COMMS_TIMEOUT for r in res]):
            return -1.
        if res[0] != etrsync.REASON_ENDSTOP_HIT:
            return 0.
        if self.mcu_endstop.is_fileoutput():
            return home_end_time
        return home_end_time
    def multi_probe_begin(self):
        self.BD_Sensor_Read(2)
        #self.bd_sensor.I2C_BD_send("1022")
        if self.stow_on_each_sample:
            return
        self.multi = 'FIRST'

    def multi_probe_end(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        homepos = self.toolhead.get_position()
        if self.switch_mode==1 and self.homeing==1 \
                     and self.collision_homing == 1:
            self.bd_sensor.I2C_BD_send("1020")
            pr = self.I2C_BD_receive_cmd.send([self.oid, "32".encode('utf-8')])
            intr = int(pr['response'])
            self.gcode.respond_info("Z axis triggered at 0 %d mm " %(intr))
            while 1:
                homepos[2] +=0.04
                self.toolhead.manual_move([None, None, homepos[2]],50)
                self.toolhead.wait_moves()
                pr = self.I2C_BD_receive_cmd.send([self.oid,
                                                  "32".encode('utf-8')])
                raw_d = int(pr['response'])
                self.gcode.respond_info("auto adjust Z axis %.2f  %.1f mm "
                                        %(homepos[2],raw_d))
                if (raw_d - intr)>5:
                    homepos[2]=0
                    self.toolhead.set_position(homepos)
                    break;
                intr = raw_d
            self.bd_sensor.I2C_BD_send("1018")
        elif self.homeing==1:
            self.bd_sensor.I2C_BD_send("1018")
            time.sleep(0.1)
            self.bd_value=self.BD_Sensor_Read(2)
            if self.bd_value > (self.position_endstop + 2):
                self.gcode.respond_info("triggered at %.3f mm !"
                    % (self.bd_value))
                self.bd_sensor.I2C_BD_send("1022") #reboot bdsensor
                time.sleep(0.9)
                self.bd_value=self.BD_Sensor_Read(2)
                if self.bd_value > (self.position_endstop + 0.7):
                    raise self.printer.command_error("Home z failed! "
                        "the triggered z position is %.3f mm"
                        % (self.bd_value))
            if self.bd_value <=0:
                self.gcode.respond_info("warning:triggered at 0mm, "
                    "Please slow down the homing z speed and the "
                    "position_endstop in BDsensor >=0.5 ")
            #time.sleep(0.1)
            self.endstop_bdsensor_offset = 0
            if self.sda_pin_num is not self.endstop_pin_num:
                self.endstop_bdsensor_offset = homepos[2]-self.bd_value
                self.gcode.respond_info("offset of endstop to bdsensor %.3fmm"
                    %self.endstop_bdsensor_offset)
            else:
                homepos[2] = self.bd_value
                self.toolhead.set_position(homepos)
            #time.sleep(0.1)
            self.gcode.respond_info("Z axis triggered at %.3f mm "
                %(self.bd_value))
        self.homeing=0
        if self.stow_on_each_sample:
            return
        self.raise_probe()
        self.multi = 'OFF'
    def probe_prepare(self, hmove):
        if self.multi == 'OFF' or self.multi == 'FIRST':
            self.lower_probe()
            if self.multi == 'FIRST':
                self.multi = 'ON'
    def probe_finish(self, hmove):
        self.bd_sensor.I2C_BD_send("1018")
        if self.multi == 'OFF':
            self.raise_probe()

    def get_position_endstop(self):
        #print("BD get_position_endstop")
        return self.position_endstop

def load_config(config):
    bdl=BDsensorEndstopWrapper(config)
    config.get_printer().add_object('probe', BDPrinterProbe(config, bdl))
    return bdl
