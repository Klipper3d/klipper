# Positional smoother on cartesian XY axes
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import chelper

MAX_DAMPING_COMPENSATION = 0.05
MAX_ACCEL_COMPENSATION = 0.005

class SmoothAxis:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect", self.connect)
        self.toolhead = None
        self.damping_comp_x = config.getfloat('damping_comp_x'
                , 0., minval=0., maxval=MAX_DAMPING_COMPENSATION)
        self.damping_comp_y = config.getfloat('damping_comp_y'
                , 0., minval=0., maxval=MAX_DAMPING_COMPENSATION)
        self.accel_comp_x = config.getfloat('accel_comp_x'
                , 0., minval=0., maxval=MAX_ACCEL_COMPENSATION)
        self.accel_comp_y = config.getfloat('accel_comp_y'
                , 0., minval=0., maxval=MAX_ACCEL_COMPENSATION)
        self.smooth_x = config.getfloat('smooth_x', 0., minval=0., maxval=.200)
        self.smooth_y = config.getfloat('smooth_y', 0., minval=0., maxval=.200)
        self.stepper_kinematics = []
        self.orig_stepper_kinematics = []
        # Stepper kinematics code
        ffi_main, ffi_lib = chelper.get_ffi()
        self._set_time = ffi_lib.smooth_axis_set_time
        self._set_sk = ffi_lib.smooth_axis_set_sk
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SET_SMOOTH_AXIS",
                               self.cmd_SET_SMOOTH_AXIS,
                               desc=self.cmd_SET_SMOOTH_AXIS_help)
    def connect(self):
        self.toolhead = self.printer.lookup_object("toolhead")
        kin = self.toolhead.get_kinematics()
        # Lookup stepper kinematics
        ffi_main, ffi_lib = chelper.get_ffi()
        steppers = kin.get_steppers()
        for s in steppers:
            sk = ffi_main.gc(ffi_lib.smooth_axis_alloc(), ffi_lib.free)
            orig_sk = s.set_stepper_kinematics(sk)
            res = ffi_lib.smooth_axis_set_sk(sk, orig_sk)
            if res < 0:
                s.set_stepper_kinematics(orig_sk)
                continue
            s.set_trapq(self.toolhead.get_trapq())
            self.stepper_kinematics.append(sk)
            self.orig_stepper_kinematics.append(orig_sk)
        # Configure initial values
        config_smooth_x = self.smooth_x
        config_smooth_y = self.smooth_y
        self.smooth_x = self.smooth_y = 0.
        self._set_smoothing(self.damping_comp_x, self.damping_comp_y,
                            self.accel_comp_x, self.accel_comp_y,
                            config_smooth_x, config_smooth_y)
    def _set_smoothing(self, damping_comp_x, damping_comp_y
                       , accel_comp_x, accel_comp_y
                       , smooth_x, smooth_y):
        old_smooth_time = max(self.smooth_x, self.smooth_y) * .5
        new_smooth_time = max(smooth_x, smooth_y) * .5
        self.toolhead.note_step_generation_scan_time(new_smooth_time,
                                                     old_delay=old_smooth_time)
        self.smooth_x = smooth_x
        self.smooth_y = smooth_y
        self.damping_comp_x = damping_comp_x
        self.damping_comp_y = damping_comp_y
        self.accel_comp_x = accel_comp_x
        self.accel_comp_y = accel_comp_y
        ffi_main, ffi_lib = chelper.get_ffi()
        for sk in self.stepper_kinematics:
            ffi_lib.smooth_axis_set_time(sk, smooth_x, smooth_y)
            ffi_lib.smooth_axis_set_damping_comp(sk, damping_comp_x,
                                                 damping_comp_y)
            ffi_lib.smooth_axis_set_accel_comp(sk, accel_comp_x, accel_comp_y)
    cmd_SET_SMOOTH_AXIS_help = "Set cartesian time smoothing parameters"
    def cmd_SET_SMOOTH_AXIS(self, params):
        gcode = self.printer.lookup_object('gcode')
        damping_comp_xy = gcode.get_float(
                'DAMPING_COMP_XY', params, -1.0,
                minval=-1.0, maxval=MAX_DAMPING_COMPENSATION)
        if damping_comp_xy < 0:
            damping_comp_x = gcode.get_float(
                    'DAMPING_COMP_X', params, self.damping_comp_x,
                    minval=0., maxval=MAX_DAMPING_COMPENSATION)
            damping_comp_y = gcode.get_float(
                    'DAMPING_COMP_Y', params, self.damping_comp_y,
                    minval=0., maxval=MAX_DAMPING_COMPENSATION)
        else:
            damping_comp_x = damping_comp_y = damping_comp_xy
        accel_comp_xy = gcode.get_float(
                'ACCEL_COMP_XY', params, -1.0,
                minval=-1.0, maxval=MAX_ACCEL_COMPENSATION)
        if accel_comp_xy < 0:
            accel_comp_x = gcode.get_float(
                    'ACCEL_COMP_X', params, self.accel_comp_x,
                    minval=0., maxval=MAX_ACCEL_COMPENSATION)
            accel_comp_y = gcode.get_float(
                    'ACCEL_COMP_Y', params, self.accel_comp_y,
                    minval=0., maxval=MAX_ACCEL_COMPENSATION)
        else:
            accel_comp_x = accel_comp_y = accel_comp_xy
        smooth_xy = gcode.get_float('SMOOTH_XY', params, -1.0,
                                    minval=-1.0, maxval=.200)
        if smooth_xy < 0:
            smooth_x = gcode.get_float('SMOOTH_X', params, self.smooth_x,
                                       minval=0., maxval=.200)
            smooth_y = gcode.get_float('SMOOTH_Y', params, self.smooth_y,
                                       minval=0., maxval=.200)
        else:
            smooth_x = smooth_y = smooth_xy
        self._set_smoothing(damping_comp_x, damping_comp_y,
                            accel_comp_x, accel_comp_y,
                            smooth_x, smooth_y)
        gcode.respond_info("damping_comp_x:%.9f damping_comp_y:%.9f "
                           "accel_comp_x:%.9f accel_comp_y:%.9f "
                           "smooth_x:%.6f smooth_y:%.6f" % (
                               damping_comp_x, damping_comp_y
                               , accel_comp_x, accel_comp_y
                               , smooth_x, smooth_y))

def load_config(config):
    return SmoothAxis(config)
