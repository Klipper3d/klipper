# Annexed Probe
#   This provides support for probes that are annexed to
#   the toolhead prior to probing and then stowed upon
#   completion.
#
# Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2021       Paul McGowan <mental405@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import probe
from math import sin, cos, atan2, pi, radians, degrees, sqrt

DEBUG                      = True
PROBE_DETACH_DELAY         = .01
PROBE_VERIFY_DELAY         = .05

PROBE_UNKNOWN   = 0
PROBE_ATTACHED  = 1
PROBE_DOCKED    = 2

MULTI_OFF       = 0
MULTI_FIRST     = 1
MULTI_ON        = 2

# Helper class to handle polling endstop pins
class EndstopPollingHelper:
    def __init__(self, config, endstop):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                        self._handle_connect)
        self.query_endstop = endstop
        self.last_verify_time  = 0
        self.last_verify_state = None
    def _handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')
    def query_pin(self, curtime):
        if (curtime > (self.last_verify_time + PROBE_VERIFY_DELAY)
            or self.last_verify_state is None):
            self.last_verify_time = curtime
            query_time = self.toolhead.get_last_move_time()
            self.last_verify_state = not not self.query_endstop(query_time)
        return self.last_verify_state
    def query_pin_inv(self, curtime):
        return not self.query_pin(curtime)

# Helper class to verify probe status
class ProbeVerificationHelper:
    def __init__(self, config, annexedProbe):
        self.printer = config.get_printer()
        self.check_open_attach = config.getboolean('check_open_attach', False)
        self.probe_sense_pin = config.get('probe_sense_pin', None)
        self.dock_sense_pin = config.get('dock_sense_pin', None)

        if self.check_open_attach and self.probe_sense_pin:
            raise self.printer.config_error(
                "annexed_probe: only one of check_open_attach or " +
                "should be provided but not both")

        ppins = self.printer.lookup_object('pins')
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("GET_PROBE_STATUS", self.cmd_GET_PROBE_STATUS)

        if self.check_open_attach:
            ehelper = EndstopPollingHelper(config, annexedProbe.query_endstop)
        # If no automatic checks are configured, register
        # manual state setting command
        elif not self.probe_sense_pin and not self.dock_sense_pin:
            self.gcode.register_command("SET_PROBE_STATE",
                                         self.cmd_SET_PROBE_STATE)
        # Configure sense pins as endstops since they
        # can be polled at specific times
        def configEndstop(pin):
            pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
            mcu = pin_params['chip']
            mcu_endstop = mcu.setup_pin('endstop', pin_params)
            helper = EndstopPollingHelper(config, mcu_endstop.query_endstop)
            return helper

        probe_sense_helper = None
        dock_sense_helper = None

        # If a probe_sense_pin is defined, use it to determine
        # if the probe is attached
        if self.probe_sense_pin:
            probe_sense_helper = configEndstop(self.probe_sense_pin)
            self.probe_sense_pin = probe_sense_helper.query_pin
        else:
            self.probe_sense_pin = ehelper.query_pin_inv
        if self.dock_sense_pin:
            dock_sense_helper = configEndstop(self.dock_sense_pin)
            self.dock_sense_pin = dock_sense_helper.query_pin
        else:
            if probe_sense_helper is not None:
                self.dock_sense_pin = probe_sense_helper.query_pin_inv
            else:
                self.dock_sense_pin = ehelper.query_pin
        self.last_verify_time = 0
        self.last_verify_state = PROBE_UNKNOWN

    def get_status(self):
        curtime = self.printer.get_reactor().monotonic()
        if (self.last_verify_state == PROBE_UNKNOWN
            or curtime > self.last_verify_time + PROBE_VERIFY_DELAY):
            self.last_verify_time = curtime
            a = self.probe_sense_pin(curtime)
            d = self.dock_sense_pin(curtime)
            if a and not d:
                self.last_verify_state = PROBE_ATTACHED
            elif d and not a:
                self.last_verify_state = PROBE_DOCKED
            else:
                self.last_verify_state = PROBE_UNKNOWN
        gcode = self.printer.lookup_object('gcode')
        if self.last_verify_state == PROBE_UNKNOWN:
            gcode.respond_info('Probe Status: Unknown')
        if self.last_verify_state == PROBE_ATTACHED:
            gcode.respond_info('Probe Status: Attached')
        if self.last_verify_state == PROBE_DOCKED:
            gcode.respond_info('Probe Status: Docked')
        return self.last_verify_state

    def cmd_GET_PROBE_STATUS(self, gcmd):
        s = self.get_status()
        if s == PROBE_UNKNOWN:
            gcmd.respond_info('Probe Status: Unknown')
        if s == PROBE_ATTACHED:
            gcmd.respond_info('Probe Status: Attached')
        if s == PROBE_DOCKED:
            gcmd.respond_info('Probe Status: Docked')

class AnnexedProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')

        #Pins
        ppins = self.printer.lookup_object('pins')
        pin = config.get('pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        mcu = pin_params['chip']
        mcu.register_config_callback(self._build_config)

        self.mcu_endstop = mcu.setup_pin('endstop', pin_params)

        # Wrappers
        self.get_mcu              = self.mcu_endstop.get_mcu
        self.add_stepper          = self.mcu_endstop.add_stepper
        self.get_steppers         = self.mcu_endstop.get_steppers
        self.home_wait            = self.mcu_endstop.home_wait
        self.query_endstop        = self.mcu_endstop.query_endstop
        self.finish_home_complete = self.wait_trigger_complete = None

        # Configuration Options
        self.position_endstop = config.getfloat('z_offset')
        self.x_offset = config.getfloat('x_offset', 0.)
        self.y_offset = config.getfloat('y_offset', 0.)
        self.speed = config.getfloat('speed', 5.0, above=0.)
        self.lift_speed = config.getfloat('lift_speed', self.speed, above=0.)
        self.dock_angle   = radians(config.getfloat('dock_angle')) - pi
        self.detach_angle = radians(config.getfloat('detach_angle')) - pi
        self.dock_safe_distance = config.getfloat('dock_safe_distance', 0.)
        self.attach_speed = config.getfloat('attach_speed',
                                             self.speed, above=0.)
        self.decouple_speed = config.getfloat('decouple_speed',
                                             self.speed, above=0.)
        self.travel_speed = config.getfloat('travel_speed',
                                             self.speed, above=0.)
        self.dock_fixed_z = config.getboolean('dock_fixed_z', False)
        self.sample_retract_dist = config.getfloat('sample_retract_dist', 2.,
                                                   above=0.)

        dock_position = config.get("dock_position")

        helper = ProbeVerificationHelper(config, self)
        self.get_probe_status = helper.get_status

        try:
            dock_position = dock_position.split(',')
            self.dock_position = [float(dock_position[0].strip()),
                                  float(dock_position[1].strip()),
                                  float(dock_position[2].strip())]
        except:
            raise config.error("Unable to parse dock_position in %s" % (
                    config.get_name()))

        home_xy = config.get("safe_z_position", None)
        if home_xy:
            try:
                home_xy = home_xy.split(',')
                self.safe_z_position  = [float(home_xy[0].strip()),
                                         float(home_xy[1].strip()),
                                         None]
            except:
                raise config.error("Unable to parse safe_z_position in %s" % (
                        config.get_name()))
        else:
            self.safe_z_position = None

        #Custom User Macros
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.pre_attach_gcode = gcode_macro.load_template(
            config, 'pre_attach_gcode', '')
        self.attach_gcode = gcode_macro.load_template(
            config, 'attach_gcode', '')
        self.post_attach_gcode = gcode_macro.load_template(
            config, 'post_attach_gcode', '')
        self.pre_detach_gcode = gcode_macro.load_template(
            config, 'pre_detach_gcode', '')
        self.detach_gcode = gcode_macro.load_template(
            config, 'detach_gcode', '')
        self.post_detach_gcode = gcode_macro.load_template(
            config, 'post_detach_gcode', '')

        #Gcode Commands
        self.gcode.register_command("ATTACH_PROBE",
                                    self.cmd_ATTACH_PROBE)
        self.gcode.register_command("DETACH_PROBE",
                                    self.cmd_DETACH_PROBE)

        #Event Handlers
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        self.printer.register_event_handler("klippy:ready",
                                            self._handle_ready)
        self.printer.register_event_handler("homing:home_rails_begin",
                                            self._handle_homing_rails_begin)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self._handle_homing_rails_end)
        self.printer.register_event_handler("homing:homing_move_begin",
                                            self._handle_homing_move_begin)
        self.printer.register_event_handler("homing:homing_move_end",
                                            self._handle_homing_move_end)
    def debugOutput(self, output):
        if DEBUG:
            self.gcode.respond_info(output)

    def _handle_homing_move_begin(self, hmove):
        self.debugOutput('_handle_homing_move_begin')

    def _handle_homing_move_end(self, hmove):
        self.debugOutput('_handle_homing_move_end')


    def _build_config(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self.add_stepper(stepper)
        endstops = [es for rail in kin.rails
                    for es, name in rail.get_endstops()]
        if self in endstops:
            if self.printer.lookup_object('safe_z_home', None):
                raise self.printer.config_error(
                    "annexed_probe cannot be used with safe_z_home if " +
                    "probe is being used as z endstop")

    def _handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')

    def _handle_ready(self):
        # set initial states
        # self.z_hopped = False
        self.last_z = -9999
        self.is_z_endstop = False
        self.multi = MULTI_OFF
        self._last_homed_axes = None
        self.init_pos = None
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        if not self.safe_z_position:
            home_xy = [0,0]
            for rail in kin.rails:
                hi = rail.get_homing_info()
                rmin, rmax = rail.get_range()
                if hi.positive_dir:
                    mid = (hi.position_endstop - rmin) *.5
                else:
                    mid = rmax * .5
                for s in rail.get_steppers():
                    a = s.get_name(short=True)
                    if a == 'x':
                        home_xy[0] = mid - self.x_offset
                    if a == 'x':
                        home_xy[1] = mid - self.y_offset
                    if a == 'z':
                        if hi.positive_dir:
                            self.z_homes_positive = True
                        else:
                            self.z_homes_positive = False
            self.safe_z_position = home_xy

    def cmd_ATTACH_PROBE(self, gcmd):
        self.debugOutput('cmd_ATTACH_PROBE')
        return_pos = self.toolhead.get_position()
        self.attach_probe(return_pos)

    def cmd_DETACH_PROBE(self, gcmd):
        self.debugOutput('cmd_DETACH_PROBE')
        return_pos = self.toolhead.get_position()
        self.detach_probe(return_pos)

    def _move_to_vector(self, angle):
        x, y = self._get_point_on_vector(self.dock_position[:2],
                                         angle,
                                         self.dock_safe_distance)
        self.debugOutput('_move_to_vector %s %s %s %s' % (x,y, angle, degrees(angle)))
        self.toolhead.manual_move([x,y,None], self.travel_speed)

    def _align_to_vector(self, angle):
        d = self.dock_safe_distance
        dock = self.dock_position
        tool = self.toolhead.get_position()
        approach = self._get_intercept(self.toolhead.get_position(),
                                       angle + (pi/2),
                                       self.dock_position,
                                       angle)
        self.debugOutput('_align_to_vector %s %s %s %s' % (approach[0], approach[1], angle, degrees(angle)))
        self.toolhead.manual_move([approach[0], approach[1], None],
                                  self.attach_speed)

    def _align_z(self):
        self.debugOutput('_align_z')

        if not self._last_homed_axes:
            curtime = self.printer.get_reactor().monotonic()
            homed_axes = self.toolhead.get_status(curtime)['homed_axes']
            self._last_homed_axes = homed_axes

        if self.dock_fixed_z:
            dock_z = (self.dock_position[2] +
                      self.position_endstop +
                      self.sample_retract_dist)
            tpos = self.toolhead.get_position()
            if 'z' in self._last_homed_axes:
                if tpos[2] < dock_z:
                    self.debugOutput('_align_z: moving to dock safe z')
                    self.toolhead.set_position(tpos)
                    self.toolhead.manual_move(
                        [None, None, dock_z],
                        self.lift_speed)
            else:
                self.debugOutput('_align_z: forcing hop')
                self._force_z_hop()
        else:
            if 'z' in self._last_homed_axes:
                self.debugOutput('_align_z: moving to dock z')
                self.toolhead.manual_move(
                                        [None, None, self.dock_position[2]],
                                        self.lift_speed)
            else:
                self.debugOutput('_align_z: move failed')
                raise self.printer.command_error(
                    "Unable to probe, must home Z axis first")

    def _force_z_hop(self):
        self.debugOutput('_force_z_hop')
        this_z = self.toolhead.get_position()[2]
        if self.last_z != this_z:
            self.debugOutput('_force_z_hop: hopping z %s' % (this_z))
            tpos = self.toolhead.get_position()
            dock_z = self.dock_position[2] + self.position_endstop
            self.toolhead.set_position([tpos[0], tpos[1], 0.0, tpos[3]],
                                homing_axes=[2])
            self.toolhead.manual_move(
                [None, None, dock_z],
                self.lift_speed)
            kin = self.toolhead.get_kinematics()
            kin.note_z_not_homed()
            self.last_z = self.toolhead.get_position()[2]
            self.debugOutput('_force_z_hop: z_hopped z %s' % ( self.last_z))

    def _check_probe_distance(self, pos=None):
        self.debugOutput('_check_probe_distance')
        if not pos:
            pos = self.toolhead.get_position()
        dock = self.dock_position
        if self.dock_safe_distance > sqrt((pos[0]-dock[0])**2 +
                                          (pos[1]-dock[1])**2 ):
            return True
        else:
            return False

    def _get_point_on_vector(self, point, angle, magnitude=1):
        x = point[0] - magnitude * cos(angle)
        y = point[1] - magnitude * sin(angle)
        return (x, y)

    def _get_intercept(self, point1, angle1, point2, angle2):
        x1, y1 = point1[:2]
        x2, y2 = self._get_point_on_vector(point1, angle1, 10.0)
        x3, y3 = point2[:2]
        x4, y4 = self._get_point_on_vector(point2, angle2, 10.0)
        det1 = ((x1 * y2) - (y1 * x2))
        det2 = ((x3 * y4) - (y3 * x4))
        d = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4))
        x = float((det1 * (x3 - x4)) - ((x1 - x2) * det2)) / d
        y = float((det1 * (y3 - y4)) - ((y1 - y2) * det2)) / d
        return (x, y)

    def attach_probe(self, return_pos=None):
        self.debugOutput('attach_probe')
        if self.get_probe_status() == PROBE_DOCKED:
            self.debugOutput('attach_probe: _align_z')
            self._align_z()
        if not return_pos:
            self.debugOutput('attach_probe: setting return pos to (%s, %s)' %
                            (self.init_pos[0], self.init_pos[1]))
            return_pos = self.init_pos
        # Back probe out of dock if it is within minimum safe distance
        if self._check_probe_distance():
            self._align_to_vector(self.dock_angle)
            self._move_to_vector(self.dock_angle)
        # Attempt 3 times to attach the probe
        retry = 0
        while self.get_probe_status() != PROBE_ATTACHED and retry <= 2:
            self.debugOutput('attach_probe: attempting attach')
            self._do_attach()
            retry += 1
        if self.get_probe_status() != PROBE_ATTACHED:
            raise self.printer.command_error("Probe attach failed!")
        if return_pos:
            if not self._check_probe_distance(return_pos):
                self.debugOutput('attach_probe: moving to return_pos')
                self.toolhead.manual_move(
                    [return_pos[0], return_pos[1], None],
                    self.travel_speed)

    def _do_attach(self):
        self.debugOutput('_do_attach')
        if self.get_probe_status() != PROBE_DOCKED:
            raise self.printer.command_error(
                "Attach Probe: Probe not detected in dock, aborting")
        self.pre_attach_gcode.run_gcode_from_command()
        if self._check_probe_distance():
            self._align_to_vector(self.dock_angle)
        else:
            self._move_to_vector(self.dock_angle)
        self.toolhead.manual_move(
            [self.dock_position[0], self.dock_position[1], None],
            self.attach_speed)
        self.attach_gcode.run_gcode_from_command()
        if self._check_probe_distance():
            self._align_to_vector(self.dock_angle)
        self._move_to_vector(self.dock_angle)
        self.post_attach_gcode.run_gcode_from_command()

    def _delayed_detach(self):
        self.debugOutput('_delayed_detach')
        if self.init_pos:
            return_pos = self.init_pos
        else:
            return_pos = self.toolhead.get_position()
        def reactor_bgfunc(print_time):
            if self.multi == MULTI_OFF:
                with self.gcode.get_mutex():
                    self.debugOutput('_delayed_detach: callback')
                    self.detach_probe(return_pos)
        def lookahead_bgfunc(print_time):
            reactor = self.printer.get_reactor()
            reactor.register_callback(
                ( lambda et: reactor_bgfunc(print_time + PROBE_DETACH_DELAY )))
        self.toolhead.register_lookahead_callback(lookahead_bgfunc)

    def detach_probe(self, return_pos=None):
        self.debugOutput('detach_probe')
        if self.get_probe_status() == PROBE_ATTACHED:
            self._align_z()
        # Back probe out of dock if it is within minimum safe distance
        if self._check_probe_distance():
            self._align_to_vector(self.dock_angle)
            self._move_to_vector(self.dock_angle)
        retry = 0
        while self.get_probe_status() != PROBE_DOCKED and retry <= 2:
            self._do_detach()
            retry += 1
        if self._check_probe_distance():
            self._align_to_vector(self.detach_angle)
            self._move_to_vector(self.detach_angle)
        if self.get_probe_status() != PROBE_DOCKED:
            raise self.printer.command_error("Probe detach failed!")
        if return_pos:
            if not self._check_probe_distance(return_pos):
                self.debugOutput('detach_probe: moving to return_pos')
                self.toolhead.manual_move(
                    [return_pos[0], return_pos[1], None],
                    self.travel_speed)

    def _do_detach(self):
        self.debugOutput('_do_detach')
        self.pre_detach_gcode.run_gcode_from_command()
        if self._check_probe_distance():
            self._align_to_vector(self.dock_angle)
        else:
            self._move_to_vector(self.dock_angle)
        self.toolhead.manual_move(
            [self.dock_position[0],
             self.dock_position[1],
             None],
             self.attach_speed)
        self.detach_gcode.run_gcode_from_command()
        self._move_to_vector(self.detach_angle)
        self.post_detach_gcode.run_gcode_from_command()

    def _handle_homing_rails_begin(self, homing_state, rails):
        self.init_pos = self.toolhead.get_position()
        self.debugOutput('_handle_homing_rails_begin')

        homing_axes = [a for i, a in enumerate(['x','y','z'])
                        if i in homing_state.get_axes()]

        if self in [es for rail in rails for es, name in rail.get_endstops()]:
            self.is_z_endstop = True


        # Get homed axes now as they will report they are
        # homed later even if they arent
        curtime = self.printer.get_reactor().monotonic()
        self._last_homed_axes = self.toolhead.get_status(curtime)['homed_axes']

        if self.get_probe_status() == PROBE_UNKNOWN:
                self.debugOutput('_handle_homing_rails_begin: probe unknown ' +
                                 'forcing z hop')
                self._force_z_hop()

        if self.get_probe_status() == PROBE_ATTACHED:
            self.debugOutput('_handle_homing_rails_begin: probe attached')

            if any(a in ['x','y'] for a in homing_axes):
                self.debugOutput('_handle_homing_rails_begin: z not homed, hopping z')
                self._force_z_hop()

            if 'z' in homing_axes:
                if self.is_z_endstop:
                    self.debugOutput('_handle_homing_rails_begin: z requested, aligning z')
                    self._align_z()
                else:
                    if ('xy' in self._last_homed_axes
                        and not self.z_homes_positive):
                        self.debugOutput('_handle_homing_rails_begin: detach_probe')
                        self.detach_probe(self.toolhead.get_position())

    def multi_probe_begin(self):
        self.debugOutput('multi_probe_begin')
        self.multi = MULTI_FIRST
        if not self.init_pos:
            self.init_pos = self.toolhead.get_position()
            self.debugOutput('multi_probe_begin: init_pos (%s, %s)' %
                            (self.init_pos[0], self.init_pos[1]))
        if not self._last_homed_axes:
            self.debugOutput('multi_probe_begin: updating homed_axes')
            curtime = self.printer.get_reactor().monotonic()
            self._last_homed_axes = self.toolhead.get_status(curtime)['homed_axes']
        if (self.is_z_endstop
            and self.dock_fixed_z
            and 'xy' in self._last_homed_axes):
            self.debugOutput('multi_probe_begin: safe_z_position')
            self.init_pos = self.safe_z_position
        self.attach_probe()

    def probe_prepare(self, hmove):
        self.debugOutput('probe_prepare')
        if self.multi == MULTI_FIRST:
            self.multi = MULTI_ON

    def multi_probe_end(self):
        self.debugOutput('multi_probe_end')
        self.multi = MULTI_OFF
        #self.z_hopped = False
        self._delayed_detach()
        self.init_pos = None

    def probe_finish(self, hmove):
        self.debugOutput('probe_finish')
        self.wait_trigger_complete.wait()
        pos = self.toolhead.get_position()
        retract_z = (self.position_endstop + self.sample_retract_dist)
        if pos[2] < retract_z:
            self.toolhead.manual_move(
                [None,
                 None,
                 retract_z],
                 self.lift_speed)

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        self.debugOutput('home_start')
        self.finish_home_complete = self.mcu_endstop.home_start(
            print_time, sample_time, sample_count, rest_time, triggered)
        r = self.printer.get_reactor()
        self.wait_trigger_complete = r.register_callback(self.wait_for_trigger)
        return self.finish_home_complete

    def wait_for_trigger(self, eventtime):
        self.debugOutput('wait_for_trigger')
        self.finish_home_complete.wait()

    def _handle_homing_rails_end(self, homing_state, rails):
        self.debugOutput('_handle_homing_rails_end')
        self.is_z_endstop = False
        self._last_homed_axes = None

    def get_position_endstop(self):
        return self.position_endstop

def load_config(config):
    msp = AnnexedProbe(config)
    config.get_printer().add_object('probe', probe.PrinterProbe(config, msp))
    return msp
