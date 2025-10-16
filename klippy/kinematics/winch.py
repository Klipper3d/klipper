# Code for handling the kinematics of cable winch robots
#
# Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import stepper, mathutil, chelper


class WinchFlexHelper:
    ALGORITHMS = {'tikhonov': 0, 'qp': 1}
    ALGO_NAMES = {v: k for k, v in ALGORITHMS.items()}

    def __init__(self, anchors, config):
        self._anchors = tuple(anchors)
        self.num = len(self._anchors)
        self.ptr = None
        self.enabled = False
        self.flex_compensation_algorithm_name = 'tikhonov'
        self.flex_compensation_algorithm = self.ALGORITHMS[self.flex_compensation_algorithm_name]
        self.ignore_gravity = False
        self.ignore_pretension = False
        self.ffi_main = self.ffi_lib = None
        self._read_config(config)
        if not self.num:
            return
        self.ffi_main, self.ffi_lib = chelper.get_ffi()
        self.ptr = self.ffi_main.gc(
            self.ffi_lib.winch_flex_alloc(), self.ffi_lib.winch_flex_free)
        self._configure()
        self.set_active(self.enabled)

    def _read_config(self, config):
        self.enabled = config.getboolean('flex_compensation_enable', True)
        self.mover_weight = config.getfloat('winch_mover_weight', 0., minval=0.)
        self.spring_constant = config.getfloat('winch_spring_constant', 0., minval=0.)
        if self.num:
            default_min = tuple(0. for _ in range(self.num))
            default_max = tuple(120. for _ in range(self.num))
            default_guy_wires = tuple(0. for _ in range(self.num))
            self.min_force = list(config.getfloatlist(
                'winch_min_force', default_min, count=self.num))
            self.max_force = list(config.getfloatlist(
                'winch_max_force', default_max, count=self.num))
            self.guy_wires = list(config.getfloatlist(
                'winch_guy_wire_lengths', default_guy_wires, count=self.num))
        else:
            self.min_force = []
            self.max_force = []
            self.guy_wires = []
        algo_choices = list(self.ALGORITHMS.keys())
        self.flex_compensation_algorithm_name = config.getchoice(
            'flex_compensation_algorithm', algo_choices, default='tikhonov')
        self.flex_compensation_algorithm = self.ALGORITHMS[self.flex_compensation_algorithm_name]
        self.ignore_gravity = config.getboolean('ignore_gravity', False)
        self.ignore_pretension = config.getboolean('ignore_pretension', False)

    def _configure(self):
        anchors_flat = [float(coord) for anchor in self._anchors for coord in anchor]
        anchors_c = self.ffi_main.new("double[]", anchors_flat)
        min_c = self.ffi_main.new("double[]", self.min_force)
        max_c = self.ffi_main.new("double[]", self.max_force)
        guy_ptr = self.ffi_main.new("double[]", self.guy_wires)
        self.ffi_lib.winch_flex_configure(
            self.ptr, self.num, anchors_c, self.mover_weight,
            self.spring_constant, min_c, max_c,
            guy_ptr, self.flex_compensation_algorithm, self.ignore_gravity, self.ignore_pretension)

    def get_ptr(self):
        if self.ptr is not None:
            return self.ptr
        if self.ffi_main is None:
            self.ffi_main, _ = chelper.get_ffi()
        return self.ffi_main.NULL

    def is_active(self):
        return bool(self.ptr) and bool(self.enabled)

    def set_active(self, enable):
        enable = bool(enable)
        self.enabled = enable
        if self.ptr and self.ffi_lib is not None:
            self.ffi_lib.winch_flex_set_enabled(self.ptr, 1 if enable else 0)
        return self.is_active()

    def config_valid(self):
        return True

    def get_algorithm_name(self):
        return self.flex_compensation_algorithm_name

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
        self.printer = config.get_printer()
        self.toolhead = toolhead
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
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command(
            'M666', self.cmd_M666,
            desc="Toggle winch flex compensation. Use M666 F0/F1.")
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
        if not self.flex_helper.is_active():
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

    def cmd_M666(self, gcmd):
        param = gcmd.get_int('F', None)
        if param is None:
            state = "enabled" if self.flex_helper.is_active() else "disabled"
            algo = self.flex_helper.get_algorithm_name()
            gcmd.respond_info(
                "Winch flex compensation is %s (algo=%s)"
                % (state, algo))
            return
        requested = bool(param)
        current_state = self.flex_helper.is_active()
        if requested == current_state:
            gcmd.respond_info(
                f"Winch flex compensation already {'enabled' if current_state else 'disabled'}.")
            return
        self.toolhead.wait_moves()
        current_pos = list(self.toolhead.get_position())
        if requested:
            actual = (self.flex_helper.config_valid()
                      and self.flex_helper.set_active(True))
        else:
            actual = self.flex_helper.set_active(False)
        if requested and not actual:
            gcmd.respond_info(
                "Unable to enable flex compensation; check winch configuration parameters.")
            return
        if actual != current_state:
            self.set_position(current_pos, "")
        state = "enabled" if actual else "disabled"
        gcmd.respond_info(f"Winch flex compensation {state}.")

def load_kinematics(toolhead, config):
    return WinchKinematics(toolhead, config)
