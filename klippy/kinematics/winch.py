# Code for handling the kinematics of cable winch robots
#
# Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
import stepper, mathutil, chelper


class WinchFlexHelper:
    ALGORITHMS = {'tikhonov': 0, 'qp': 1}
    ALGO_NAMES = {v: k for k, v in ALGORITHMS.items()}

    def __init__(self, anchors, config):
        self._anchors = tuple(anchors)
        self.num = len(self._anchors)
        self.ptr = None
        self.enabled = False
        self.flex_compensation_algorithm_name = 'qp'
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
        self.buildup_factor = config.getfloat('winch_buildup_factor', 0., minval=0.)
        self.mover_weight = config.getfloat('winch_mover_weight', 0., minval=0.)
        self.spring_constant = config.getfloat('winch_spring_constant', 0., minval=0.)
        if self.num:
            default_min = tuple(0. for _ in range(self.num))
            default_max = tuple(120. for _ in range(self.num))
            default_guy_wires = tuple(0. for _ in range(self.num))
            default_mechanical_advantage = tuple(1 for _ in range(self.num))
            self.min_force = list(config.getfloatlist(
                'winch_min_force', default_min, count=self.num))
            self.max_force = list(config.getfloatlist(
                'winch_max_force', default_max, count=self.num))
            self.guy_wires = list(config.getfloatlist(
                'winch_guy_wire_lengths', default_guy_wires, count=self.num))
            self.mechanical_advantage = list(config.getintlist(
                'winch_mechanical_advantage', default_mechanical_advantage, count=self.num))
        else:
            self.min_force = []
            self.max_force = []
            self.guy_wires = []
            self.mechanical_advantage = []
        algo_choices = list(self.ALGORITHMS.keys())
        self.flex_compensation_algorithm_name = config.getchoice(
            'flex_compensation_algorithm', algo_choices, default='qp')
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
            self.ptr, self.num, anchors_c, self.buildup_factor, self.mover_weight,
            self.spring_constant, min_c, max_c,
            guy_ptr, self.flex_compensation_algorithm,
            self.ignore_gravity, self.ignore_pretension, self.mechanical_advantage)

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
    def set_spool_params(self, index, rotation_distance, steps_per_rotation):
        if not self.ptr or self.ffi_lib is None:
            return
        self.ffi_lib.winch_flex_set_spool_params(
            self.ptr, index, rotation_distance, steps_per_rotation)
    def motor_to_line_pos(self, index, motor_pos):
        if not self.ptr or self.ffi_lib is None:
            return motor_pos
        return self.ffi_lib.winch_flex_motor_to_line_pos(
            self.ptr, index, motor_pos)

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
        self.force_move = self.printer.lookup_object('force_move')
        self.stepper_enable = self.printer.lookup_object('stepper_enable')
        self.pretension_speed = config.getfloat(
            'flex_pretension_speed', 5., minval=0.001)
        self.pretension_accel = config.getfloat(
            'flex_pretension_accel', 0., minval=0.)
        self.pretension_min_delta = config.getfloat(
            'flex_pretension_min_delta', 0.0005, minval=0.)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command(
            'M666', self.cmd_M666,
            desc="Toggle winch flex compensation. Use M666 F0/F1.")
        flex_ptr = self.flex_helper.get_ptr()
        if not flex_ptr:
            raise config.error("Failed to initialise winch flex helper")
        for idx, s in enumerate(self.steppers):
            rdist, steps_per_rot = s.get_rotation_distance()
            self.flex_helper.set_spool_params(idx, rdist, steps_per_rot)
            s.setup_itersolve('winch_stepper_alloc', flex_ptr, idx)
            s.set_trapq(toolhead.get_trapq())
        # Setup boundary checks
        acoords = list(zip(*self.anchors))
        self.axes_min = toolhead.Coord(*[min(a) for a in acoords], e=0.)
        self.axes_max = toolhead.Coord(*[max(a) for a in acoords], e=0.)
        self.set_position([0., 0., 0.], "")
        # Halley/LM hybrid parameters for the forward transform (3T only)
        self._halley_eta = 1e-3
        self._halley_tol = 1e-3
        self._halley_max_iters = 30
        self._halley_hybrid_iters = 3
        self._last_forward = None
    def get_steppers(self):
        return list(self.steppers)
    def calc_position(self, stepper_positions):
        flex_ptr = self.flex_helper.get_ptr()
        ffi_main, ffi_lib = self.flex_helper.ffi_main, self.flex_helper.ffi_lib
        if not flex_ptr or ffi_main is None or ffi_lib is None:
            return [None, None, None]
        try:
            motor_pos = [stepper_positions[s.get_name()] for s in self.steppers]
        except KeyError:
            return [None, None, None]
        motor_c = ffi_main.new("double[]", motor_pos)
        guess = self._last_forward if self._last_forward is not None else [0., 0., 0.]
        guess_c = ffi_main.new("double[3]", guess)
        out_pos = ffi_main.new("double[3]")
        out_cost = ffi_main.new("double[1]")
        out_iters = ffi_main.new("int[1]")
        ok = ffi_lib.winch_forward_solve(
            flex_ptr, motor_c, guess_c,
            self._halley_eta, self._halley_tol,
            self._halley_hybrid_iters, self._halley_max_iters,
            out_pos, out_cost, out_iters)
        if not ok:
            return [None, None, None]
        result = [out_pos[0], out_pos[1], out_pos[2]]
        self._last_forward = result
        return result
    def set_position(self, newpos, homing_axes):
        for s in self.steppers:
            s.set_position(newpos)
        if len(newpos) >= 3:
            self._last_forward = list(newpos[:3])
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

    def _apply_flex_transition(self, current_pos, prev_lengths):
        distances, flex = self.flex_helper.calc_arrays(current_pos[:3])
        anchor_count = min(len(distances), len(self.steppers))
        if not anchor_count:
            return []
        moves = []
        for idx in range(anchor_count):
            prev_length = prev_lengths[idx]
            target_length = distances[idx] + flex[idx]
            delta = target_length - prev_length
            if abs(delta) <= self.pretension_min_delta:
                continue
            moves.append((self.steppers[idx], delta))
        if not moves:
            return []
        stepper_names = [s.get_name() for s, _ in moves]
        self.stepper_enable.set_motors_enable(stepper_names, True)
        speed = max(self.pretension_speed, 0.001)
        accel = self.pretension_accel
        for stp, delta in moves:
            self.force_move.manual_move(stp, delta, speed, accel)
        return [(stp.get_name(), delta) for stp, delta in moves]

    # ---- Halley-based forward transform helpers ----
    @staticmethod
    def _vec_norm(vec):
        return math.sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2])

    def _effective_lengths(self, lengths, pos_guess):
        if not self.flex_helper.is_active():
            return list(lengths), min(len(lengths), len(self.anchors))
        distances, flex = self.flex_helper.calc_arrays(pos_guess)
        anchor_count = min(len(lengths), len(distances), len(flex), len(self.anchors))
        if anchor_count <= 0:
            return [], 0
        return [lengths[i] + flex[i] for i in range(anchor_count)], anchor_count

    @staticmethod
    def _residuals_and_derivatives(anchors, lengths, pos):
        residuals = []
        jac = []
        hess = []
        for anchor, target_len in zip(anchors, lengths):
            dx = pos[0] - anchor[0]
            dy = pos[1] - anchor[1]
            dz = pos[2] - anchor[2]
            dist = math.sqrt(dx * dx + dy * dy + dz * dz)
            if dist < 1e-9:
                dist = 1e-9
            inv_len = 1.0 / dist
            inv_len3 = inv_len * inv_len * inv_len
            residuals.append(dist - target_len)
            jrow = [dx * inv_len, dy * inv_len, dz * inv_len]
            jac.append(jrow)
            hrow = [
                [inv_len - dx * dx * inv_len3, -dx * dy * inv_len3, -dx * dz * inv_len3],
                [-dy * dx * inv_len3, inv_len - dy * dy * inv_len3, -dy * dz * inv_len3],
                [-dz * dx * inv_len3, -dz * dy * inv_len3, inv_len - dz * dz * inv_len3],
            ]
            hess.append(hrow)
        return residuals, jac, hess

    @staticmethod
    def _accumulate_normals(jac, residuals):
        grad = [0., 0., 0.]
        jtj = [[0., 0., 0.], [0., 0., 0.], [0., 0., 0.]]
        for row, res in zip(jac, residuals):
            grad[0] += row[0] * res
            grad[1] += row[1] * res
            grad[2] += row[2] * res
            jtj[0][0] += row[0] * row[0]
            jtj[0][1] += row[0] * row[1]
            jtj[0][2] += row[0] * row[2]
            jtj[1][0] += row[1] * row[0]
            jtj[1][1] += row[1] * row[1]
            jtj[1][2] += row[1] * row[2]
            jtj[2][0] += row[2] * row[0]
            jtj[2][1] += row[2] * row[1]
            jtj[2][2] += row[2] * row[2]
        return jtj, grad

    @staticmethod
    def _solve_3x3(system, rhs):
        try:
            inv = mathutil.matrix_inv(system)
        except ZeroDivisionError:
            return None
        return [
            inv[0][0] * rhs[0] + inv[0][1] * rhs[1] + inv[0][2] * rhs[2],
            inv[1][0] * rhs[0] + inv[1][1] * rhs[1] + inv[1][2] * rhs[2],
            inv[2][0] * rhs[0] + inv[2][1] * rhs[1] + inv[2][2] * rhs[2],
        ]

    def _halley_iteration(self, anchors, lengths, pos):
        residuals, jac, hess = self._residuals_and_derivatives(anchors, lengths, pos)
        jtj, grad = self._accumulate_normals(jac, residuals)
        for i in range(3):
            jtj[i][i] += self._halley_eta
        delta_lm = self._solve_3x3(jtj, [-g for g in grad])
        if delta_lm is None:
            return pos, False

        hbar = []
        for h in hess:
            hbar.append([
                delta_lm[0] * h[0][0] + delta_lm[1] * h[1][0] + delta_lm[2] * h[2][0],
                delta_lm[0] * h[0][1] + delta_lm[1] * h[1][1] + delta_lm[2] * h[2][1],
                delta_lm[0] * h[0][2] + delta_lm[1] * h[1][2] + delta_lm[2] * h[2][2],
            ])

        jbar = []
        for jrow, hrow in zip(jac, hbar):
            jbar.append([
                jrow[0] + 0.5 * hrow[0],
                jrow[1] + 0.5 * hrow[1],
                jrow[2] + 0.5 * hrow[2],
            ])
        jtj2, grad2 = self._accumulate_normals(jbar, residuals)
        for i in range(3):
            jtj2[i][i] += self._halley_eta
        delta = self._solve_3x3(jtj2, [-g for g in grad2])
        if delta is None:
            return pos, False
        new_pos = [
            pos[0] + delta[0],
            pos[1] + delta[1],
            pos[2] + delta[2],
        ]
        return new_pos, self._vec_norm(delta) < self._halley_tol

    def _lm_iteration(self, anchors, lengths, pos):
        residuals, jac, _ = self._residuals_and_derivatives(anchors, lengths, pos)
        jtj, grad = self._accumulate_normals(jac, residuals)
        for i in range(3):
            jtj[i][i] += self._halley_eta
        delta = self._solve_3x3(jtj, [-g for g in grad])
        if delta is None:
            return pos, False
        new_pos = [
            pos[0] + delta[0],
            pos[1] + delta[1],
            pos[2] + delta[2],
        ]
        return new_pos, self._vec_norm(delta) < self._halley_tol

    def _solve_hybrid_halley(self, anchors, lengths, seed):
        pos = list(seed)
        # Phase 1: Halley updates
        for _ in range(self._halley_hybrid_iters):
            pos, ok = self._halley_iteration(anchors, lengths, pos)
            if ok:
                return pos, True
        # Phase 2: LM fallback
        for _ in range(self._halley_hybrid_iters, self._halley_max_iters):
            pos, ok = self._lm_iteration(anchors, lengths, pos)
            if ok:
                return pos, True
        return pos, False

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
        prev_lengths = [
            self.flex_helper.motor_to_line_pos(idx, s.get_commanded_position())
            for idx, s in enumerate(self.steppers)
        ]
        if requested:
            actual = (self.flex_helper.config_valid()
                      and self.flex_helper.set_active(True))
        else:
            actual = self.flex_helper.set_active(False)
        if requested and not actual:
            gcmd.respond_info(
                "Unable to enable flex compensation; check winch configuration parameters.")
            return
        extra_msg = ""
        if actual != current_state:
            try:
                adjustments = self._apply_flex_transition(current_pos, prev_lengths)
            except Exception as exc:
                self.flex_helper.set_active(current_state)
                self.set_position(current_pos, "")
                raise gcmd.error(f"Failed to apply flex transition: {exc}")
            self.set_position(current_pos, "")
            if adjustments:
                deltas = ", ".join(
                    f"{name}:{delta:+.6f}" for name, delta in adjustments)
                extra_msg = f" Applied pretension deltas ({deltas})."
        state = "enabled" if actual else "disabled"
        gcmd.respond_info(f"Winch flex compensation {state}.{extra_msg}")

def load_kinematics(toolhead, config):
    return WinchKinematics(toolhead, config)
