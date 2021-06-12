import logging
from mcu import MCU_endstop
#
# TODO name command to Z_OFFSET_CALIBRATE !? remove probe triggered check
#
ERROR_VIRTUAL_ENDSTOP = ("No virtual endstops for z are supported"
                         " for calibrate_z module!")
ERROR_NO_PROBE = ("A configured probe is needed"
                  " for calibrate_z module!")
ERROR_BED_SITE_OR_MESH = ("Either configure probe_bed_x and probe_bed_y"
                          " or configure a mesh with a"
                          " relative_reference_index"
                          " for calibrate_z module!")
ERROR_NO_PROBE = "Probe switch not closed - Probe not attached?"
class ZCalibrationHelper:
    def __init__(self, config):
        self.state = None
        self.z_endstop = None
        self.z_homing = None
        self.last_state = False
        self.last_z_offset = 0.
        self.config = config
        self.printer = config.get_printer()
        self.switch_offset = config.getfloat('switch_offset', 0.0, above=0.)
        self.max_deviation = config.getfloat('max_deviation', 1.0, above=0.)
        self.speed = config.getfloat('speed', 50.0, above=0.)
        self.samples = config.getint('samples', None, minval=1)
        self.tolerance = config.getfloat('samples_tolerance', None, above=0.)
        self.retries = config.getint('samples_tolerance_retries',
                                     None, minval=0)
        atypes = {'none': None, 'median': 'median', 'average': 'average'}
        self.samples_result = config.getchoice('samples_result', atypes,
                                               'none')
        self.lift_speed = config.getfloat('lift_speed', None, above=0.)
        self.clearance = config.getfloat('clearance', None, above=0.)
        self.probing_speed = config.getfloat('probing_speed', None, above=0.)
        self.second_speed = config.getfloat('probing_second_speed',
                                            None, above=0.)
        self.retract_dist = config.getfloat('probing_retract_dist',
                                            None, above=0.)
        self.position_min = config.getfloat('position_min', None)
        self.first_fast = config.getboolean('probing_first_fast', False)
        self.probe_nozzle_site = [
            config.getfloat('probe_nozzle_x'),
            config.getfloat('probe_nozzle_y'),
            None,
        ]
        self.probe_switch_site = [
            config.getfloat('probe_switch_x'),
            config.getfloat('probe_switch_y'),
            None,
        ]
        self.probe_bed_site = [
            config.getfloat('probe_bed_x', None),
            config.getfloat('probe_bed_y', None),
            None,
        ]
        self.query_endstops = self.printer.load_object(config,
                                                       'query_endstops')
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self.handle_home_rails_end)
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('CALIBRATE_Z', self.cmd_CALIBRATE_Z,
                                    desc=self.cmd_CALIBRATE_Z_help)
    def get_status(self, eventtime):
        return {'last_query': self.last_state,
                'last_z_offset': self.last_z_offset}
    def handle_connect(self):
        # get z-endstop
        for endstop, name in self.query_endstops.endstops:
            if name == 'z':
                # check for virtual endstops..
                if not isinstance(endstop, MCU_endstop):
                    raise self.printer.config_error(ERROR_VIRTUAL_ENDSTOP)
                self.z_endstop = EndstopWrapper(self.config, endstop)
        # get probing settings
        probe = self.printer.lookup_object('probe', default=None)
        if probe is None:
            raise self.printer.config_error(ERROR_NO_PROBE)
        if self.samples is None:
            self.samples = probe.sample_count
        if self.tolerance is None:
            self.tolerance = probe.samples_tolerance
        if self.retries is None:
            self.retries = probe.samples_retries
        if self.lift_speed is None:
            self.lift_speed = probe.lift_speed
        if self.clearance is None:
            self.clearance = probe.z_offset * 2
        if self.samples_result is None:
            self.samples_result = probe.samples_result
        # get the mesh's relative reference point
        # a round mesh/bed would not work here so far...
        if self.probe_bed_site[0] is None or self.probe_bed_site[1] is None:
            mesh = self.printer.lookup_object('bed_mesh', default=None)
            if mesh is None or mesh.bmc.relative_reference_index is None:
                raise self.printer.config_error(ERROR_BED_SITE_OR_MESH)
            rri = mesh.bmc.relative_reference_index
            self.probe_bed_site[0] = mesh.bmc.points[rri][0]
            self.probe_bed_site[1] = mesh.bmc.points[rri][1]
        logging.debug("Z-CALIBRATION probe_bed_x=%.3f probe_bed_y=%.3f"
                      % (self.probe_bed_site[0], self.probe_bed_site[1]))
    def handle_home_rails_end(self, homing_state, rails):
        # get z homing position
        for rail in rails:
            if rail.get_steppers()[0].is_active_axis('z'):
                kin_spos = homing_state.get_stepper_trigger_positions()
                self.z_homing = kin_spos.get(rail.get_name())
                # get homing settings from z rail
                if self.probing_speed is None:
                    self.probing_speed = rail.homing_speed
                if self.second_speed is None:
                    self.second_speed = rail.second_homing_speed
                if self.retract_dist is None:
                    self.retract_dist = rail.homing_retract_dist
                if self.position_min is None:
                    self.position_min = rail.position_min
    def _build_config(self):
        pass
    cmd_CALIBRATE_Z_help = ("Automatically calibrates the nozzles offset"
                            " to the print surface")
    def cmd_CALIBRATE_Z(self, gcmd):
        if self.state is not None:
            raise self.printer.command_error("Already performing CALIBRATE_Z")
            return
        # check if probe is attached and the switch is closing it
        probe = self.printer.lookup_object('probe')
        time = self.printer.lookup_object('toolhead').get_last_move_time()
        if probe.mcu_probe.query_endstop(time):
            raise self.printer.command_error(ERROR_NO_PROBE)
            return
        self._log_config()
        state = CalibrationState(self, gcmd)
        state.calibrate_z()
    def _log_config(self):
        logging.debug("Z-CALIBRATION: switch_offset=%.3f, max_deviation=%.3f,"
                      " speed=%.3f, samples=%i, tolerance=%.3f, retries=%i,"
                      " samples_result=%s, lift_speed=%.3f, clearance=%.3f,"
                      " probing_speed=%.3f, second_speed=%.3f,"
                      " retract_dist=%.3f, position_min=%.3f,"
                      " probe_nozzle_x=%.3f, probe_nozzle_y=%.3f,"
                      " probe_switch_x=%.3f, probe_switch_y=%.3f,"
                      " probe_bed_x=%.3f, probe_bed_y=%.3f"
                      % (self.switch_offset, self.max_deviation, self.speed,
                         self.samples, self.tolerance, self.retries,
                         self.samples_result, self.lift_speed, self.clearance,
                         self.probing_speed, self.second_speed,
                         self.retract_dist, self.position_min,
                         self.probe_nozzle_site[0], self.probe_nozzle_site[1],
                         self.probe_switch_site[0], self.probe_switch_site[1],
                         self.probe_bed_site[0], self.probe_bed_site[1]))
class EndstopWrapper:
    def __init__(self, config, endstop):
        self.mcu_endstop = endstop
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
class CalibrationState:
    def __init__(self, helper, gcmd):
        self.helper = helper
        self.gcmd = gcmd
        self.gcode = helper.gcode
        self.z_endstop = helper.z_endstop
        self.phoming = helper.printer.lookup_object('homing')
        self.probe = helper.printer.lookup_object('probe')
        self.toolhead = helper.printer.lookup_object('toolhead')
        self.gcode_move = helper.printer.lookup_object('gcode_move')
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
    def _probe(self, mcu_endstop, z_position, speed):
            pos = self.toolhead.get_position()
            pos[2] = z_position
            # probe
            curpos = self.phoming.probing_move(mcu_endstop, pos, speed)
            # retract
            self.toolhead.manual_move([None, None,
                                       curpos[2] + self.helper.retract_dist],
                                      self.probe.lift_speed)
            self.helper.gcode.respond_info("probe at %.3f,%.3f is z=%.6f"
                % (curpos[0], curpos[1], curpos[2]))
            return curpos
    def _probe_on_z_endstop(self, site):
        pos = self.toolhead.get_position()
        if pos[2] < self.helper.clearance:
            # no clearance, better to move up
            self.toolhead.manual_move([None, None,
                                       pos[2] + self.helper.clearance],
                                      self.helper.lift_speed)
        # move to position
        self.toolhead.manual_move(list(site), self.helper.speed)
        if self.helper.first_fast:
            # first probe just to get down faster
            self._probe(self.z_endstop, self.helper.position_min,
                        self.helper.probing_speed)
        retries = 0
        positions = []
        while len(positions) < self.helper.samples:
            # probe with second probing speed
            curpos = self._probe(self.z_endstop, self.helper.position_min,
                                 self.helper.second_speed)
            positions.append(curpos[:3])
            # check tolerance
            z_positions = [p[2] for p in positions]
            if max(z_positions) - min(z_positions) > self.helper.tolerance:
                if retries >= self.helper.retries:
                    raise self.gcmd.error("Probe samples exceed tolerance")
                self.gcmd.respond_info("Probe samples exceed tolerance."
                                       " Retrying...")
                retries += 1
                positions = []
        # calculate result
        if self.helper.samples_result == 'median':
            return self._calc_median(positions)[2]
        return self._calc_mean(positions)[2]
    def _probe_on_bed(self, bed_site):
        # calculate bed position by using the probe's offsets
        probe_offsets = self.probe.get_offsets()
        probe_site = list(bed_site)
        probe_site[0] -= probe_offsets[0]
        probe_site[1] -= probe_offsets[1]
        # move to probing position
        pos = self.toolhead.get_position()
        self.toolhead.manual_move([None, None, pos[2] + self.helper.clearance],
                                  self.helper.lift_speed)
        self.toolhead.manual_move(probe_site, self.helper.speed)
        if self.helper.first_fast:
            # fast probe to get down first
            self._probe(self.probe.mcu_probe, self.probe.z_position,
                        self.helper.probing_speed)
        # probe it
        return self.probe.run_probe(self.gcmd)[2]
    def _set_new_gcode_offset(self, offset):
        # reset gcode z offset to 0
        gcmd_offset = self.gcode.create_gcode_command("SET_GCODE_OFFSET",
                                                      "SET_GCODE_OFFSET",
                                                      {'Z': 0.0, 'MOVE': '1'})
        self.gcode_move.cmd_SET_GCODE_OFFSET(gcmd_offset)
        # set new gcode z offset
        gcmd_offset = self.gcode.create_gcode_command("SET_GCODE_OFFSET",
                                                      "SET_GCODE_OFFSET",
                                                      {'Z_ADJUST': offset,
                                                       'MOVE': '1'})
        self.gcode_move.cmd_SET_GCODE_OFFSET(gcmd_offset)
    def calibrate_z(self):
        # probe the nozzle
        nozzle_zero = self._probe_on_z_endstop(self.helper.probe_nozzle_site)
        # probe the probe-switch
        switch_zero = self._probe_on_z_endstop(self.helper.probe_switch_site)
        # probe position on bed
        probe_zero = self._probe_on_bed(self.helper.probe_bed_site)
        # move up by retract_dist
        self.toolhead.manual_move([None, None,
                                   probe_zero + self.helper.retract_dist],
                                  self.helper.lift_speed)
        # calculate the offset
        offset = probe_zero - (switch_zero - nozzle_zero
                               + self.helper.switch_offset)
        # print result
        self.gcmd.respond_info("Z-CALIBRATION: ENDSTOP=%.3f NOZZLE=%.3f"
                               " SWITCH=%.3f PROBE=%.3f --> OFFSET=%.6f"
                               % (self.helper.z_homing, nozzle_zero,
                                  switch_zero, probe_zero, offset))
        # check max deviation
        if abs(offset) > self.helper.max_deviation:
            raise self.helper.printer.command_error("Offset is larger as"
                                                    " allowed: OFFSET=%.3f"
                                                    " MAX_DEVIATION=%.3f"
                                                    % (offset,
                                                    self.helper.max_deviation))
            return
        # set new offset
        self._set_new_gcode_offset(offset)
        # set states
        self.helper.last_state = True
        self.helper.last_z_offset = offset
def load_config(config):
    return ZCalibrationHelper(config)
