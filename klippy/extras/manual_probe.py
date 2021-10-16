# Helper script for manual axis height probing
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, bisect

class ManualProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.load_object(config, "gcode_move")
        self.gcode.register_command('MANUAL_PROBE', self.cmd_MANUAL_PROBE,
                                    desc=self.cmd_MANUAL_PROBE_help)

        xconfig = config.getsection('stepper_x')
        yconfig = config.getsection('stepper_y')
        zconfig = config.getsection('stepper_z')

        self.endstop_positions = [
            xconfig.getfloat('position_endstop', None, note_valid=False),
            yconfig.getfloat('position_endstop', None, note_valid=False),
            zconfig.getfloat('position_endstop', None, note_valid=False)
        ]

        if self.endstop_positions[0] is not None:
            self.gcode.register_command(
                'X_ENDSTOP_CALIBRATE', self.cmd_X_ENDSTOP_CALIBRATE,
                desc=self.cmd_X_ENDSTOP_CALIBRATE_help)
            self.gcode.register_command(
                'X_OFFSET_APPLY_ENDSTOP',
                self.cmd_X_OFFSET_APPLY_ENDSTOP,
                desc=self.cmd_X_OFFSET_APPLY_ENDSTOP_help)

        if self.endstop_positions[1] is not None:
            self.gcode.register_command(
                'Y_ENDSTOP_CALIBRATE', self.cmd_Y_ENDSTOP_CALIBRATE,
                desc=self.cmd_Y_ENDSTOP_CALIBRATE_help)
            self.gcode.register_command(
                'Y_OFFSET_APPLY_ENDSTOP',
                self.cmd_Y_OFFSET_APPLY_ENDSTOP,
                desc=self.cmd_Y_OFFSET_APPLY_ENDSTOP_help)

        if self.endstop_positions[2] is not None:
            self.gcode.register_command(
                'Z_ENDSTOP_CALIBRATE', self.cmd_Z_ENDSTOP_CALIBRATE,
                desc=self.cmd_Z_ENDSTOP_CALIBRATE_help)
            self.gcode.register_command(
                'Z_OFFSET_APPLY_ENDSTOP',
                self.cmd_Z_OFFSET_APPLY_ENDSTOP,
                desc=self.cmd_Z_OFFSET_APPLY_ENDSTOP_help)

    def manual_probe_finalize(self, kin_pos, axis, axis_label):
        if kin_pos is not None:
            self.gcode.respond_info("%s position is %.3f"
                % (axis_label, kin_pos[axis],))

    cmd_MANUAL_PROBE_help = "Start manual probe helper script"
    def cmd_MANUAL_PROBE(self, gcmd):
        ManualProbeHelper(self.printer, gcmd, 2, self.manual_probe_finalize)

    def endstop_finalize(self, kin_pos, axis, axis_label):
        if kin_pos is None:
            return
        pos = self.endstop_positions[axis] - kin_pos[axis]
        self.gcode.respond_info(
            "stepper_%s: position_endstop: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer." % (axis_label, pos,))
        configfile = self.printer.lookup_object('configfile')
        configfile.set('stepper_'+axis_label, 'position_endstop',
            "%.3f" % (pos,))

    def offset_apply_endstop(self, gcmd, axis, axis_label):
        offset = self.gcode_move.get_status()['homing_origin'][axis]
        configfile = self.printer.lookup_object('configfile')
        if offset == 0:
            self.gcode.respond_info("Nothing to do: %s Offset is 0"
                % (axis_label))
        else:
            new_calibrate = self.z_position_endstop - offset
            self.gcode.respond_info(
                "stepper_%s: position_endstop: %.3f\n"
                "The SAVE_CONFIG command will update the printer config file\n"
                "with the above and restart the printer."
                % (axis_label, new_calibrate))
            configfile.set('stepper_'+axis_label, 'position_endstop',
                "%.3f" % (new_calibrate,))


    cmd_X_ENDSTOP_CALIBRATE_help = "Calibrate an x endstop"
    def cmd_X_ENDSTOP_CALIBRATE(self, gcmd):
        ManualProbeHelper(self.printer, gcmd, 0, self.endstop_finalize)

    cmd_X_OFFSET_APPLY_ENDSTOP_help = "Adjust the x endstop_position"
    def cmd_X_OFFSET_APPLY_ENDSTOP(self,gcmd):
        self.offset_apply_endstop(gcmd, 0, 'x')

    cmd_Y_ENDSTOP_CALIBRATE_help = "Calibrate a y endstop"
    def cmd_Y_ENDSTOP_CALIBRATE(self, gcmd):
        ManualProbeHelper(self.printer, gcmd, 1, self.endstop_finalize)

    cmd_Y_OFFSET_APPLY_ENDSTOP_help = "Adjust the y endstop_position"
    def cmd_Y_OFFSET_APPLY_ENDSTOP(self,gcmd):
        self.offset_apply_endstop(gcmd, 1, 'y')

    cmd_Z_ENDSTOP_CALIBRATE_help = "Calibrate a z endstop"
    def cmd_Z_ENDSTOP_CALIBRATE(self, gcmd):
        ManualProbeHelper(self.printer, gcmd, 2, self.endstop_finalize)

    cmd_Z_OFFSET_APPLY_ENDSTOP_help = "Adjust the z endstop_position"
    def cmd_Z_OFFSET_APPLY_ENDSTOP(self,gcmd):
        self.offset_apply_endstop(gcmd, 2, 'z')

# Verify that a manual probe isn't already in progress
def verify_no_manual_probe(printer):
    gcode = printer.lookup_object('gcode')
    try:
        gcode.register_command('ACCEPT', 'dummy')
    except printer.config_error as e:
        raise gcode.error(
            "Already in a manual probe. Use ABORT to abort it.")
    gcode.register_command('ACCEPT', None)

AXIS_BOB_MINIMUM = 0.500
BISECT_MAX = 0.200

# Helper script to determine an axis height
class ManualProbeHelper:
    def __init__(self, printer, gcmd, axis, finalize_callback):
        self.printer = printer
        self.finalize_callback = finalize_callback
        self.gcode = self.printer.lookup_object('gcode')
        self.toolhead = self.printer.lookup_object('toolhead')
        self.speed = gcmd.get_float("SPEED", 5.)
        self.past_positions = []
        self.last_toolhead_pos = self.last_kinematics_pos = None
        self.axis = axis
        self.axis_label = ['X', 'Y', 'Z'][self.axis]
        # Register commands
        verify_no_manual_probe(printer)
        self.gcode.register_command('ACCEPT', self.cmd_ACCEPT,
                                    desc=self.cmd_ACCEPT_help)
        self.gcode.register_command('NEXT', self.cmd_ACCEPT)
        self.gcode.register_command('ABORT', self.cmd_ABORT,
                                    desc=self.cmd_ABORT_help)
        self.gcode.register_command('TEST'+self.axis_label, self.cmd_TEST_AXIS,
                                    desc=self.cmd_TEST_AXIS_help)
        self.gcode.respond_info(
            "Starting manual %s probe. Use TEST%s to adjust position.\n"
            "Finish with ACCEPT or ABORT command."
            % (self.axis_label, self.axis_label))
        self.start_position = self.toolhead.get_position()
        self.report_axis_status()

    def get_kinematics_pos(self):
        toolhead_pos = self.toolhead.get_position()
        if toolhead_pos == self.last_toolhead_pos:
            return self.last_kinematics_pos
        self.toolhead.flush_step_generation()
        kin = self.toolhead.get_kinematics()
        kin_spos = {s.get_name(): s.get_commanded_position()
                    for s in kin.get_steppers()}
        kin_pos = kin.calc_position(kin_spos)
        self.last_toolhead_pos = toolhead_pos
        self.last_kinematics_pos = kin_pos
        return kin_pos

    def move_axis(self, axis_pos):
        curpos = self.toolhead.get_position()
        non_move = [None, None, None]
        try:
            axis_bob_pos = axis_pos + AXIS_BOB_MINIMUM
            if curpos[self.axis] < axis_bob_pos:
                self.toolhead.manual_move(
                    non_move[:self.axis]+[axis_bob_pos]+non_move[self.axis+1:],
                    self.speed)
            self.toolhead.manual_move(
                non_move[:self.axis]+[axis_pos]+non_move[self.axis+1:],
                self.speed)
        except self.printer.command_error as e:
            self.finalize(False)
            raise

    def report_axis_status(self, warn_no_change=False, prev_pos=None):
        # Get position
        kin_pos = self.get_kinematics_pos()
        axis_pos = kin_pos[self.axis]
        if warn_no_change and axis_pos == prev_pos:
            self.gcode.respond_info(
                "WARNING: No change in position (reached stepper resolution)")
        # Find recent positions that were tested
        pp = self.past_positions
        next_pos = bisect.bisect_left(pp, axis_pos)
        prev_pos = next_pos - 1
        if next_pos < len(pp) and pp[next_pos] == axis_pos:
            next_pos += 1
        prev_str = next_str = "??????"
        if prev_pos >= 0:
            prev_str = "%.3f" % (pp[prev_pos],)
        if next_pos < len(pp):
            next_str = "%.3f" % (pp[next_pos],)
        # Find recent positions
        self.gcode.respond_info("%s position: %s --> %.3f <-- %s"
            % (self.axis_label, prev_str, axis_pos, next_str))

    cmd_ACCEPT_help = "Accept the current position"
    def cmd_ACCEPT(self, gcmd):
        pos = self.toolhead.get_position()
        start_pos = self.start_position
        if pos[:self.axis]+pos[self.axis+1:] != \
                start_pos[:self.axis]+start_pos[self.axis+1:]
            or pos[self.axis] >= start_pos[self.axis]:
            gcmd.respond_info(
                "Manual probe failed! Use TEST%s commands to position the\n"
                "nozzle prior to running ACCEPT." % (self.axis_label))
            self.finalize(False)
            return
        self.finalize(True)

    cmd_ABORT_help = "Abort manual probing tool"
    def cmd_ABORT(self, gcmd):
        self.finalize(False)

    cmd_TEST_AXIS_help = "Move to new location"
    def cmd_TEST_AXIS(self, gcmd):
        # Store current position for later reference
        kin_pos = self.get_kinematics_pos()
        axis_pos = kin_pos[self.axis]
        pp = self.past_positions
        insert_pos = bisect.bisect_left(pp, axis_pos)
        if insert_pos >= len(pp) or pp[insert_pos] != axis_pos:
            pp.insert(insert_pos, axis_pos)
        # Determine next position to move to
        req = gcmd.get(self.axis_label)
        if req in ('+', '++'):
            check_axis = 9999999999999.9
            if insert_pos < len(self.past_positions) - 1:
                check_axis = self.past_positions[insert_pos + 1]
            if req == '+':
                check_axis = (check_axis + axis_pos) / 2.
            next_axis_pos = min(check_axis, axis_pos + BISECT_MAX)
        elif req in ('-', '--'):
            check_axis = -9999999999999.9
            if insert_pos > 0:
                check_axis = self.past_positions[insert_pos - 1]
            if req == '-':
                check_axis = (check_axis + axis_pos) / 2.
            next_axis_pos = max(check_axis, axis_pos - BISECT_MAX)
        else:
            next_axis_pos = axis_pos + gcmd.get_float(self.axis_label)
        # Move to given position and report it
        self.move_axis(next_axis_pos)
        self.report_axis_status(next_axis_pos != axis_pos, axis_pos)

    def finalize(self, success):
        self.gcode.register_command('ACCEPT', None)
        self.gcode.register_command('NEXT', None)
        self.gcode.register_command('ABORT', None)
        self.gcode.register_command('TEST'+self.axis_label, None)
        kin_pos = None
        if success:
            kin_pos = self.get_kinematics_pos()
        self.finalize_callback(kin_pos, self.axis, self.axis_label)

def load_config(config):
    return ManualProbe(config)
