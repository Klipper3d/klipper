# Utility for manually moving a stepper for diagnostic purposes
#
# Copyright (C) 2018-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import chelper

BUZZ_DISTANCE = 1.
BUZZ_VELOCITY = BUZZ_DISTANCE / .250
BUZZ_RADIANS_DISTANCE = math.radians(1.)
BUZZ_RADIANS_VELOCITY = BUZZ_RADIANS_DISTANCE / .250

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

class ForceMove:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = {}
        # Setup iterative solver
        self.motion_queuing = self.printer.load_object(config, 'motion_queuing')
        self.trapq = self.motion_queuing.allocate_trapq()
        self.trapq_append = self.motion_queuing.lookup_trapq_append()
        ffi_main, ffi_lib = chelper.get_ffi()
        self.stepper_kinematics = ffi_main.gc(
            ffi_lib.cartesian_stepper_alloc(b'x'), ffi_lib.free)
        # Register commands
        self._enable_force_move = config.getboolean("enable_force_move", False)
        if self._enable_force_move:
            gcode = self.printer.lookup_object('gcode')
            gcode.register_command('SET_KINEMATIC_POSITION',
                                   self.cmd_SET_KINEMATIC_POSITION,
                                   desc=self.cmd_SET_KINEMATIC_POSITION_help)
    def register_stepper(self, config, mcu_stepper):
        name = mcu_stepper.get_name()
        self.steppers[name] = mcu_stepper
        # Reuse mux helper args checks
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command('STEPPER_BUZZ', "STEPPER", name,
                                   self.cmd_STEPPER_BUZZ,
                                   desc=self.cmd_STEPPER_BUZZ_help)
        if self._enable_force_move:
            gcode.register_mux_command('FORCE_MOVE', "STEPPER", name,
                                        self.cmd_FORCE_MOVE,
                                        desc=self.cmd_FORCE_MOVE_help)
    def lookup_stepper(self, name):
        if name not in self.steppers:
            raise self.printer.config_error("Unknown stepper %s" % (name,))
        return self.steppers[name]
    def _force_enable(self, stepper):
        stepper_name = stepper.get_name()
        stepper_enable = self.printer.lookup_object('stepper_enable')
        did_enable = stepper_enable.set_motors_enable([stepper_name], True)
        return did_enable
    def _restore_enable(self, stepper, did_enable):
        if not did_enable:
            return
        stepper_name = stepper.get_name()
        stepper_enable = self.printer.lookup_object('stepper_enable')
        stepper_enable.set_motors_enable([stepper_name], False)
    def manual_move(self, stepper, dist, speed, accel=0.):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        prev_sk = stepper.set_stepper_kinematics(self.stepper_kinematics)
        prev_trapq = stepper.set_trapq(self.trapq)
        stepper.set_position((0., 0., 0.))
        axis_r, accel_t, cruise_t, cruise_v = calc_move_time(dist, speed, accel)
        print_time = toolhead.get_last_move_time()
        self.trapq_append(self.trapq, print_time, accel_t, cruise_t, accel_t,
                          0., 0., 0., axis_r, 0., 0., 0., cruise_v, accel)
        print_time = print_time + accel_t + cruise_t + accel_t
        self.motion_queuing.note_mcu_movequeue_activity(print_time)
        toolhead.dwell(accel_t + cruise_t + accel_t)
        toolhead.flush_step_generation()
        stepper.set_trapq(prev_trapq)
        stepper.set_stepper_kinematics(prev_sk)
        self.motion_queuing.wipe_trapq(self.trapq)
    cmd_STEPPER_BUZZ_help = "Oscillate a given stepper to help id it"
    def cmd_STEPPER_BUZZ(self, gcmd):
        stepper = self.lookup_stepper(gcmd.get('STEPPER'))
        logging.info("Stepper buzz %s", stepper.get_name())
        did_enable = self._force_enable(stepper)
        toolhead = self.printer.lookup_object('toolhead')
        dist, speed = BUZZ_DISTANCE, BUZZ_VELOCITY
        if stepper.units_in_radians():
            dist, speed = BUZZ_RADIANS_DISTANCE, BUZZ_RADIANS_VELOCITY
        for i in range(10):
            self.manual_move(stepper, dist, speed)
            toolhead.dwell(.050)
            self.manual_move(stepper, -dist, speed)
            toolhead.dwell(.450)
        self._restore_enable(stepper, did_enable)
    cmd_FORCE_MOVE_help = "Manually move a stepper; invalidates kinematics"
    def cmd_FORCE_MOVE(self, gcmd):
        stepper = self.lookup_stepper(gcmd.get('STEPPER'))
        distance = gcmd.get_float('DISTANCE')
        speed = gcmd.get_float('VELOCITY', above=0.)
        accel = gcmd.get_float('ACCEL', 0., minval=0.)
        logging.info("FORCE_MOVE %s distance=%.3f velocity=%.3f accel=%.3f",
                     stepper.get_name(), distance, speed, accel)
        self._force_enable(stepper)
        self.manual_move(stepper, distance, speed, accel)
    cmd_SET_KINEMATIC_POSITION_help = "Force a low-level kinematic position"
    def cmd_SET_KINEMATIC_POSITION(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.get_last_move_time()
        curpos = toolhead.get_position()
        x = gcmd.get_float('X', curpos[0])
        y = gcmd.get_float('Y', curpos[1])
        z = gcmd.get_float('Z', curpos[2])
        set_homed = gcmd.get('SET_HOMED', 'xyz').lower()
        set_homed_axes = "".join([a for a in "xyz" if a in set_homed])
        if gcmd.get('CLEAR_HOMED', None) is None:
            # "CLEAR" is an alias for "CLEAR_HOMED"; should deprecate
            clear_homed = gcmd.get('CLEAR', '').lower()
        else:
            clear_homed = gcmd.get('CLEAR_HOMED', '').lower()
        clear_homed_axes = "".join([a for a in "xyz" if a in clear_homed])
        logging.info("SET_KINEMATIC_POSITION pos=%.3f,%.3f,%.3f"
                     " set_homed=%s clear_homed=%s",
                     x, y, z, set_homed_axes, clear_homed_axes)
        toolhead.set_position([x, y, z], homing_axes=set_homed_axes)
        toolhead.get_kinematics().clear_homing_state(clear_homed_axes)

def load_config(config):
    return ForceMove(config)
