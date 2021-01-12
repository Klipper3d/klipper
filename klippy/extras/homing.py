# Helper code for implementing homing operations
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math, collections

HOMING_START_DELAY = 0.001
ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 4

# State tracking during toolhead homing/probing operations
class Homing:
    def __init__(self, printer):
        self.printer = printer
        self.toolhead = printer.lookup_object('toolhead')
        self.changed_axes = []
        self.verify_retract = True
        if self.printer.get_start_args().get("debuginput"):
            self.verify_retract = False
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
    def homing_move(self, movepos, endstops, speed,
                    probe_pos=False, verify_movement=False):
        # Notify start of homing/probing move
        self.printer.send_event("homing:homing_move_begin",
                                [es for es, name in endstops])
        # Note start location
        self.toolhead.flush_step_generation()
        kin = self.toolhead.get_kinematics()
        for s in kin.get_steppers():
            s.set_tag_position(s.get_commanded_position())
        start_mcu_pos = [(s, name, s.get_mcu_position())
                         for es, name in endstops for s in es.get_steppers()]
        # Start endstop checking
        print_time = self.toolhead.get_last_move_time()
        endstop_triggers = []
        for mcu_endstop, name in endstops:
            rest_time = self._calc_endstop_rate(mcu_endstop, movepos, speed)
            wait = mcu_endstop.home_start(print_time, ENDSTOP_SAMPLE_TIME,
                                          ENDSTOP_SAMPLE_COUNT, rest_time)
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
        for mcu_endstop, name in endstops:
            did_trigger = mcu_endstop.home_wait(move_end_print_time)
            if not did_trigger and error is None:
                error = "Failed to home %s: Timeout during homing" % (name,)
        # Determine stepper halt positions
        self.toolhead.flush_step_generation()
        end_mcu_pos = [(s, name, spos, s.get_mcu_position())
                       for s, name, spos in start_mcu_pos]
        if probe_pos:
            for s, name, spos, epos in end_mcu_pos:
                md = (epos - spos) * s.get_step_dist()
                s.set_tag_position(s.get_tag_position() + md)
            self.set_homed_position(kin.calc_tag_position())
        else:
            self.toolhead.set_position(movepos)
        # Signal homing/probing move complete
        try:
            self.printer.send_event("homing:homing_move_end",
                                    [es for es, name in endstops])
        except self.printer.command_error as e:
            if error is None:
                error = str(e)
        if error is not None:
            raise self.printer.command_error(error)
        # Check if some movement occurred
        if verify_movement:
            for s, name, spos, epos in end_mcu_pos:
                if spos == epos:
                    if probe_pos:
                        raise self.printer.command_error(
                            "Probe triggered prior to movement")
                    raise self.printer.command_error(
                        "Endstop %s still triggered after retract" % (name,))
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
        self.homing_move(movepos, endstops, hi.speed)
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
            self.homing_move(movepos, endstops, hi.second_homing_speed,
                             verify_movement=self.verify_retract)
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
    def home_axes(self, axes):
        self.changed_axes = axes
        try:
            self.toolhead.get_kinematics().home(self)
        except self.printer.command_error:
            self.printer.lookup_object('stepper_enable').motor_off()
            raise

# Return a completion that completes when all completions in a list complete
def multi_complete(printer, completions):
    if len(completions) == 1:
        return completions[0]
    cb = (lambda e: all([c.wait() for c in completions]))
    return printer.get_reactor().register_callback(cb)

class PrinterHoming:
    def __init__(self, config):
        self.printer = config.get_printer()
        # Register g-code commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('G28', self.cmd_G28)
    def new_homing_state(self):
        return Homing(self.printer)
    def cmd_G28(self, gcmd):
        # Move to origin
        axes = []
        for pos, axis in enumerate('XYZ'):
            if gcmd.get(axis, None) is not None:
                axes.append(pos)
        if not axes:
            axes = [0, 1, 2]
        homing_state = Homing(self.printer)
        homing_state.home_axes(axes)

def load_config(config):
    return PrinterHoming(config)
