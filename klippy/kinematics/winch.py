# Code for handling the kinematics of cable winch robots
#
# Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import stepper, mathutil, chelper

class WinchFlexHelper:
    def __init__(self, anchors, config):
        self._anchors = tuple(anchors)
        self.num = len(self._anchors)
        self.ptr = None
        self.enabled = False
        self.ffi_main = self.ffi_lib = None
        self._read_config(config)
        if not self.num:
            return
        self.ffi_main, self.ffi_lib = chelper.get_ffi()
        self.ptr = self.ffi_main.gc(
            self.ffi_lib.winch_flex_alloc(), self.ffi_lib.winch_flex_free)
        self._configure()

    def _read_config(self, config):
        self.mover_weight = config.getfloat('winch_mover_weight', 0., minval=0.)
        self.spring_constant = config.getfloat('winch_spring_constant', 0., minval=0.)
        self.target_force = config.getfloat('winch_target_force', 0., minval=0.)
        if self.num:
            default_min = tuple(0. for _ in range(self.num))
            default_max = tuple(120. for _ in range(self.num))
            self.min_force = list(config.getfloatlist(
                'winch_min_force', default_min, count=self.num))
            self.max_force = list(config.getfloatlist(
                'winch_max_force', default_max, count=self.num))
            guy_raw = config.get('winch_guy_wire_lengths', None, note_valid=False)
            self.guy_wires = []
            self.guy_wires_valid = 0
            if guy_raw is not None:
                self.guy_wires = list(config.getfloatlist(
                    'winch_guy_wire_lengths', count=self.num))
                self.guy_wires_valid = 1
        else:
            self.min_force = []
            self.max_force = []
            self.guy_wires = []
            self.guy_wires_valid = 0
        self.enabled = bool(self.num >= 4
                            and self.mover_weight > 0.
                            and self.spring_constant > 0.)

    def _configure(self):
        anchors_flat = [float(coord) for anchor in self._anchors for coord in anchor]
        anchors_c = self.ffi_main.new("double[]", anchors_flat)
        min_c = self.ffi_main.new("double[]", self.min_force)
        max_c = self.ffi_main.new("double[]", self.max_force)
        if self.guy_wires_valid:
            guy_ptr = self.ffi_main.new("double[]", self.guy_wires)
        else:
            guy_ptr = self.ffi_main.NULL
        self.ffi_lib.winch_flex_configure(
            self.ptr, self.num, anchors_c, self.mover_weight,
            self.spring_constant, self.target_force, min_c, max_c,
            guy_ptr, self.guy_wires_valid)

    def get_ptr(self):
        if self.ptr is not None:
            return self.ptr
        if self.ffi_main is None:
            self.ffi_main, _ = chelper.get_ffi()
        return self.ffi_main.NULL

    def calc_arrays(self, pos):
        if not self.ptr or not self.num:
            return [], []
        distances = self.ffi_main.new("double[]", self.num)
        flex = self.ffi_main.new("double[]", self.num)
        self.ffi_lib.winch_flex_calc_arrays(self.ptr, pos[0], pos[1], pos[2],
                                            distances, flex)
        return ([distances[i] for i in range(self.num)],
                [flex[i] for i in range(self.num)])

class WinchKinematics:
    def __init__(self, toolhead, config):
        # Setup steppers at each anchor
        self.steppers = []
        self.anchors = []
        for i in range(26):
            name = 'stepper_' + chr(ord('a') + i)
            if i >= 3 and not config.has_section(name):
                break
            stepper_config = config.getsection(name)
            s = stepper.PrinterStepper(stepper_config)
            self.steppers.append(s)
            a = tuple(stepper_config.getfloat('anchor_' + n) for n in 'xyz')
            self.anchors.append(a)
        self.flex_helper = WinchFlexHelper(self.anchors, config)
        flex_ptr = self.flex_helper.get_ptr()
        if not flex_ptr:
            raise config.error("Failed to initialise winch flex helper")
        for idx, s in enumerate(self.steppers):
            s.setup_itersolve('winch_stepper_alloc', flex_ptr, idx)
            s.set_trapq(toolhead.get_trapq())
        # Setup boundary checks
        acoords = list(zip(*self.anchors))
        self.axes_min = toolhead.Coord(*[min(a) for a in acoords], e=0.)
        self.axes_max = toolhead.Coord(*[max(a) for a in acoords], e=0.)
        self.set_position([0., 0., 0.], "")
    def get_steppers(self):
        return list(self.steppers)
    def calc_position(self, stepper_positions):
        # Use only first three steppers to calculate cartesian position
        pos = [stepper_positions[rail.get_name()] for rail in self.steppers[:3]]
        cart = mathutil.trilateration(self.anchors[:3], [sp * sp for sp in pos])
        if not self.flex_helper.enabled:
            return cart
        cart_guess = list(cart)
        for _ in range(3):
            distances, flex = self.flex_helper.calc_arrays(cart_guess)
            if len(distances) < 3:
                break
            adjusted = [pos[i] + flex[i] for i in range(3)]
            cart_guess = list(mathutil.trilateration(
                self.anchors[:3], [d * d for d in adjusted]))
        return cart_guess
    def set_position(self, newpos, homing_axes):
        for s in self.steppers:
            s.set_position(newpos)
    def clear_homing_state(self, clear_axes):
        # XXX - homing not implemented
        pass
    def home(self, homing_state):
        # XXX - homing not implemented
        homing_state.set_axes([0, 1, 2])
        homing_state.set_homed_position([0., 0., 0.])
    def check_move(self, move):
        # XXX - boundary checks and speed limits not implemented
        pass
    def get_status(self, eventtime):
        # XXX - homed_checks and rail limits not implemented
        return {
            'homed_axes': 'xyz',
            'axis_minimum': self.axes_min,
            'axis_maximum': self.axes_max,
        }

def load_kinematics(toolhead, config):
    return WinchKinematics(toolhead, config)
