# Helper code for implementing homing operations
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math, collections

HOMING_START_DELAY = 0.001
ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 4

# Return a completion that completes when all completions in a list complete
def multi_complete(printer, completions):
    if len(completions) == 1:
        return completions[0]
    cb = (lambda e: all([c.wait() for c in completions]))
    return printer.get_reactor().register_callback(cb)

# Implementation of homing/probing moves
class HomingMove:
    def __init__(self, printer, endstops, toolhead=None):
        self.printer = printer
        self.endstops = endstops
        if toolhead is None:
            toolhead = printer.lookup_object('toolhead')
        self.toolhead = toolhead
        self.end_mcu_pos = []
    def get_mcu_endstops(self):
        return [es for es, name in self.endstops]
    def _calc_endstop_rate(self, mcu_endstop, movepos, speed):
        startpos = self.toolhead.get_position()
        axes_d = [mp - sp for mp, sp in zip(movepos, startpos)]
        move_d = math.sqrt(sum([d*d for d in axes_d[:3]]))
        move_t = move_d / speed
        max_steps = max([(abs(s.calc_position_from_coord(startpos)
                              - s.calc_position_from_coord(movepos))
                          / s.get_step_dist())
                         for s in mcu_endstop.get_steppers()])
        if max_steps <= 0.:
            return .001
        return move_t / max_steps
    def homing_move(self, movepos, speed, probe_pos=False,
                    triggered=True, check_triggered=True):
        # Notify start of homing/probing move
        self.printer.send_event("homing:homing_move_begin", self)
        # Note start location
        self.toolhead.flush_step_generation()
        kin = self.toolhead.get_kinematics()
        for s in kin.get_steppers():
            s.set_tag_position(s.get_commanded_position())
        start_mcu_pos = [(s, name, s.get_mcu_position())
                         for es, name in self.endstops
                         for s in es.get_steppers()]
        # Start endstop checking
        print_time = self.toolhead.get_last_move_time()
        endstop_triggers = []
        for mcu_endstop, name in self.endstops:
            rest_time = self._calc_endstop_rate(mcu_endstop, movepos, speed)
            wait = mcu_endstop.home_start(print_time, ENDSTOP_SAMPLE_TIME,
                                          ENDSTOP_SAMPLE_COUNT, rest_time,
                                          triggered=triggered)
            endstop_triggers.append(wait)
        all_endstop_trigger = multi_complete(self.printer, endstop_triggers)
        self.toolhead.dwell(HOMING_START_DELAY)
        # Issue move
        error = None
        try:
            self.toolhead.drip_move(movepos, speed, all_endstop_trigger)
        except self.printer.command_error as e:
            error = "Error during homing move: %s" % (str(e),)
        # Wait for endstops to trigger
        move_end_print_time = self.toolhead.get_last_move_time()
        for mcu_endstop, name in self.endstops:
            did_trigger = mcu_endstop.home_wait(move_end_print_time)
            if not did_trigger and check_triggered and error is None:
                error = "Failed to home %s: Timeout during homing" % (name,)
        # Determine stepper halt positions
        self.toolhead.flush_step_generation()
        self.end_mcu_pos = [(s, name, spos, s.get_mcu_position())
                            for s, name, spos in start_mcu_pos]
        if probe_pos:
            for s, name, spos, epos in self.end_mcu_pos:
                md = (epos - spos) * s.get_step_dist()
                s.set_tag_position(s.get_tag_position() + md)
            movepos = list(kin.calc_tag_position())[:3] + movepos[3:]
        self.toolhead.set_position(movepos)
        # Signal homing/probing move complete
        try:
            self.printer.send_event("homing:homing_move_end", self)
        except self.printer.command_error as e:
            if error is None:
                error = str(e)
        if error is not None:
            raise self.printer.command_error(error)
        return movepos
    def check_no_movement(self):
        if self.printer.get_start_args().get('debuginput') is not None:
            return None
        for s, name, spos, epos in self.end_mcu_pos:
            if spos == epos:
                return name
        return None

# State tracking of homing requests
class Homing:
    def __init__(self, printer):
        self.printer = printer
        self.toolhead = printer.lookup_object('toolhead')
        self.changed_axes = []
    def set_axes(self, axes):
        self.changed_axes = axes
    def get_axes(self):
        return self.changed_axes
    def _fill_coord(self, coord):
        # Fill in any None entries in 'coord' with current toolhead position
        thcoord = list(self.toolhead.get_position())
        for i in range(len(coord)):
            if coord[i] is not None:
                thcoord[i] = coord[i]
        return thcoord
    def set_homed_position(self, pos):
        self.toolhead.set_position(self._fill_coord(pos))
    def home_rails(self, rails, forcepos, movepos):
        # Notify of upcoming homing operation
        self.printer.send_event("homing:home_rails_begin", self, rails)
        # Alter kinematics class to think printer is at forcepos
        homing_axes = [axis for axis in range(3) if forcepos[axis] is not None]
        forcepos = self._fill_coord(forcepos)
        movepos = self._fill_coord(movepos)
        self.toolhead.set_position(forcepos, homing_axes=homing_axes)
        # Perform first home
        endstops = [es for rail in rails for es in rail.get_endstops()]
        hi = rails[0].get_homing_info()
        hmove = HomingMove(self.printer, endstops)
        hmove.homing_move(movepos, hi.speed)
        # Perform second home
        if hi.retract_dist:
            # Retract
            axes_d = [mp - fp for mp, fp in zip(movepos, forcepos)]
            move_d = math.sqrt(sum([d*d for d in axes_d[:3]]))
            retract_r = min(1., hi.retract_dist / move_d)
            retractpos = [mp - ad * retract_r
                          for mp, ad in zip(movepos, axes_d)]
            self.toolhead.move(retractpos, hi.retract_speed)
            # Home again
            forcepos = [rp - ad * retract_r
                        for rp, ad in zip(retractpos, axes_d)]
            self.toolhead.set_position(forcepos)
            hmove = HomingMove(self.printer, endstops)
            hmove.homing_move(movepos, hi.second_homing_speed)
            if hmove.check_no_movement() is not None:
                raise self.printer.command_error(
                    "Endstop %s still triggered after retract"
                    % (hmove.check_no_movement(),))
        # Signal home operation complete
        self.toolhead.flush_step_generation()
        kin = self.toolhead.get_kinematics()
        for s in kin.get_steppers():
            s.set_tag_position(s.get_commanded_position())
        ret = self.printer.send_event("homing:home_rails_end", self, rails)
        if any(ret):
            # Apply any homing offsets
            adjustpos = kin.calc_tag_position()
            for axis in homing_axes:
                movepos[axis] = adjustpos[axis]
            self.toolhead.set_position(movepos)

class PrinterHoming:
    def __init__(self, config):
        self.printer = config.get_printer()
        # Register g-code commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('G28', self.cmd_G28)
    def manual_home(self, toolhead, endstops, pos, speed,
                    triggered, check_triggered):
        hmove = HomingMove(self.printer, endstops, toolhead)
        hmove.homing_move(pos, speed, triggered=triggered,
                          check_triggered=check_triggered)
    def probing_move(self, mcu_probe, pos, speed):
        endstops = [(mcu_probe, "probe")]
        hmove = HomingMove(self.printer, endstops)
        epos = hmove.homing_move(pos, speed, probe_pos=True)
        if hmove.check_no_movement() is not None:
            raise self.printer.command_error(
                "Probe triggered prior to movement")
        return epos
    def cmd_G28(self, gcmd):
        # Move to origin
        axes = []
        for pos, axis in enumerate('XYZ'):
            if gcmd.get(axis, None) is not None:
                axes.append(pos)
        if not axes:
            axes = [0, 1, 2]
        homing_state = Homing(self.printer)
        homing_state.set_axes(axes)
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        try:
            kin.home(homing_state)
        except self.printer.command_error:
            self.printer.lookup_object('stepper_enable').motor_off()
            raise

def load_config(config):
    return PrinterHoming(config)
