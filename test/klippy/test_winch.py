import os
import sys
import math
import unittest
import configparser

HERE = os.path.abspath(__file__)
TEST_DIR = os.path.dirname(HERE)
ROOT = os.path.dirname(TEST_DIR)
REPO_ROOT = os.path.dirname(ROOT)
KLIPPY_ROOT = os.path.join(REPO_ROOT, 'klippy')
if KLIPPY_ROOT not in sys.path:
    sys.path.insert(0, KLIPPY_ROOT)

import configfile  # noqa: E402
import chelper      # noqa: E402
from kinematics.winch import WinchFlexHelper, WinchKinematics  # noqa: E402


ffi, lib = chelper.get_ffi()


def list_str(values):
    return ', '.join(f"{v:.6f}" for v in values)


def int_list_str(values):
    return ', '.join(str(int(v)) for v in values)


class DummyPrinter:
    def lookup_object(self, name, default=None):
        if default is not None:
            return default
        raise configfile.error("Unknown object '%s'" % (name,))


FOUR_ANCHORS_DEFAULT = (
    (16.4, -1610.98, -131.53),
    (1314.22, 1268.14, -121.28),
    (-1415.73, 707.61, -121.82),
    (0.0, 0.0, 2299.83),
)

FOUR_ANCHORS_PROBLEM = (
    (-1000.0, 1000.0, -121.28),
    (500.0, -500.0, -131.53),
    (-1415.73, 707.61, -121.82),
    (0.0, 0.0, 2299.83),
)

FIVE_ANCHORS_DEFAULT = (
    (16.4, -1610.98, -131.53),
    (1314.22, 128.14, -121.28),
    (-15.73, 1415.61, -121.82),
    (-1211.62, 18.14, -111.18),
    (10.0, -10.0, 2299.83),
)


class WinchFlexHelperTests(unittest.TestCase):
    def build_helper(self, anchors, mover_weight=2.0, spring=20000.0,
                     target=20.0, min_force=None, max_force=None,
                     guy_wires=None, buildup_factor=0.0,
                     mechanical_advantage=None, lines_per_axis=None,
                     attachments=None, algorithm='qp', ignore_pretension=False, anchor_mode='none'):
        num = len(anchors)
        if attachments is None:
            attachments = [(0.0, 0.0, 0.0)] * num
        if min_force is None:
            min_force = [0.0] * num
        if max_force is None:
            max_force = [120.0] * num
        if mechanical_advantage is None:
            mechanical_advantage = [1] * num
        cfg = configparser.ConfigParser()
        options = {
            'winch_anchor_mode': anchor_mode,
            'winch_mover_weight': f"{mover_weight}",
            'winch_spring_constant': f"{spring}",
            'winch_target_force': f"{target}",
            'winch_min_force': list_str(min_force),
            'winch_max_force': list_str(max_force),
            'winch_buildup_factor': f"{buildup_factor}",
            'winch_mechanical_advantage': int_list_str(mechanical_advantage),
            'flex_compensation_algorithm': algorithm,
            'ignore_pretension': str(ignore_pretension),
        }
        if guy_wires is not None:
            options['winch_guy_wire_lengths'] = list_str(guy_wires)
        if lines_per_axis is not None:
            options['winch_lines_per_axis'] = int_list_str(lines_per_axis)
        cfg['printer'] = options
        wrapper = configfile.ConfigWrapper(DummyPrinter(), cfg, {}, 'printer')
        return WinchFlexHelper(tuple(anchors), tuple(attachments), wrapper)

    def test_tikhonov_without_pretension_still_obeys_physical_force_limits(self):
        anchors = [(0., -1900., -280.), (1900., 0., -280.),
                   (0., 1900., -280.), (-1900., 0., -280.), (0., 0., 1900.)]
        for mass in (.081, 10.):
            helper = self.build_helper(anchors, mover_weight=mass, min_force=[3.]*5,
                max_force=[5.]*5, algorithm='tikhonov', ignore_pretension=True)
            helper.set_active(True)
            distances, flex = helper.calc_arrays((1800., 300., 1800.))
            forces = [-f*20000./d for f,d in zip(flex, distances)]
            self.assertTrue(all(-1e-9 <= force <= 5.+1e-9 for force in forces), forces)

    def test_tikhonov_minimum_is_a_soft_target(self):
        anchors = [(0., -1900., -280.), (1900., 0., -280.),
                   (0., 1900., -280.), (-1900., 0., -280.), (0., 0., 1900.)]
        helper = self.build_helper(anchors, mover_weight=.081, min_force=[3.]*5,
            max_force=[170.]*5, algorithm='tikhonov')
        helper.set_active(True)
        distances, flex = helper.calc_arrays((0., 0., 0.))
        forces = [-f*20000./d for f,d in zip(flex, distances)]
        self.assertGreater(max(3.-f for f in forces), .2)
        self.assertLess(max(3.-f for f in forces), .3)

    def spool_lengths(self, helper, pos):
        distances, flex = helper.calc_arrays(pos)
        lengths = [d + f for d, f in zip(distances, flex)]
        return lengths, distances, flex

    def expected_motor_to_line_pos(self, motor_pos, rotation_distance,
                                   steps_per_rotation, buildup_factor,
                                   mechanical_advantage=1):
        radius = rotation_distance * float(mechanical_advantage) / (2.0 * math.pi)
        steps_per_mm = steps_per_rotation / rotation_distance
        k2 = -float(buildup_factor) * float(mechanical_advantage)
        if abs(k2) <= 1.0e-12 or steps_per_mm <= 0.0:
            return motor_pos
        k0 = 2.0 * steps_per_mm * radius / k2
        motor_steps = motor_pos * steps_per_mm
        term = motor_steps / k0 + radius
        return ((term * term) - (radius * radius)) / k2

    def test_hp5_force_balance_and_flex_forward_round_trip(self):
        anchors = [(0., -1900., -280.), (1900., 0., -280.),
                   (0., 1900., -280.), (-1900., 0., -280.), (0., 0., 1900.)]
        strands = [2, 2, 2, 2, 4]
        for ma in ([1]*5, [2, 2, 2, 2, 4]):
            guy = [429.1]*4 + [352.3]
            helper = self.build_helper(anchors, mover_weight=.081,
                mechanical_advantage=ma, lines_per_axis=strands, guy_wires=guy,
                min_force=[3. if ma[0] == 1 else 10.]*5)
            helper.set_active(True)
            steppers = [ffi.gc(lib.winch_stepper_alloc(helper.ptr, i), lib.free) for i in range(5)]
            for pos in ((0., 0., 0.), (300., -200., 50.), (-400., 350., 400.)):
                distances, flex = helper.calc_arrays(pos)
                forces = [-f * 20000. * u*u*n / (d*u+y)
                          for f, d, u, n, y in zip(flex, distances, ma, strands, guy)]
                residual, _ = self.wrench_residual(anchors, [(0.,0.,0.)]*5,
                    pos, forces, distances, .081*9.81)
                # This firmware minimizes ridge-regularized force error rather
                # than enforcing exact equilibrium. Check its KKT conditions.
                minimum = 3. if ma[0] == 1 else 10.
                for anchor, distance, force in zip(anchors, distances, forces):
                    gradient = sum((anchor[j]-pos[j])/distance * residual[j] for j in range(3)) + 1e-3*force
                    if force > minimum + 1e-5:
                        self.assertLess(abs(gradient), .003)
                    else:
                        self.assertGreaterEqual(gradient, -.003)
                self.assertTrue(all(force > 0 for force in forces))
                motor = ffi.new('double[]', [lib.itersolve_calc_position_from_coord(stp, *pos) for stp in steppers])
                out, cost, iters = ffi.new('double[3]'), ffi.new('double[1]'), ffi.new('int[1]')
                ok = lib.winch_forward_solve(helper.ptr, motor, ffi.new('double[3]', [0.,0.,0.]),
                    .001, .001, 3, 30, out, cost, iters)
                self.assertTrue(ok)
                for actual, wanted in zip(out, pos):
                    self.assertAlmostEqual(actual, wanted, delta=.01)

    def test_forward_uses_gravity_and_near_planar_range_information(self):
        for height in (1900., .01, 0.):
            anchors = [(0.,-1900.,0.), (1900.,0.,0.), (0.,1900.,0.),
                       (-1900.,0.,0.), (0.,0.,height)]
            helper = self.build_helper(anchors, buildup_factor=0.)
            steppers = [ffi.gc(lib.winch_stepper_alloc(helper.ptr, i), lib.free) for i in range(5)]
            pos = (10., 20., 100.)
            motor = ffi.new('double[]', [lib.itersolve_calc_position_from_coord(stp, *pos) for stp in steppers])
            out, cost, iters = ffi.new('double[3]'), ffi.new('double[1]'), ffi.new('int[1]')
            ok = lib.winch_forward_solve(helper.ptr, motor, ffi.new('double[3]', [0.,0.,0.]),
                .001, .001, 3, 30, out, cost, iters)
            self.assertTrue(ok)
            wanted_pos = pos if height else (10.,20.,-100.)
            for actual, wanted in zip(out, wanted_pos):
                self.assertAlmostEqual(actual, wanted, delta=.01)

    def test_cube_corner_mode_and_move_checks(self):
        anchors = [(1000.,-1000.,1900.),(1000.,1000.,1900.),(-1000.,1000.,1900.),(-1000.,-1000.,1900.),
                   (1000.,-1000.,-280.),(1000.,1000.,-280.),(-1000.,1000.,-280.),(-1000.,-1000.,-280.)]
        with self.assertRaises(configfile.error):
            self.build_helper(anchors[:4], anchor_mode='cube_corner')
        helper = self.build_helper(anchors, anchor_mode='cube_corner')
        kin = WinchKinematics.__new__(WinchKinematics)
        kin.flex_helper = helper
        class Move:
            def __init__(self, position): self.end_pos = position
            def move_error(self, message): return ValueError(message)
        for position in ((0.,0.,0.), (1000.,1000.,1900.), (0.,0.,-1500.)):
            kin.check_move(Move(position))
        for position in ((1001.,0.,0.), (0.,0.,2000.), (0.,1100.,-1500.)):
            with self.assertRaises(ValueError): kin.check_move(Move(position))
        helper.set_anchor_mode(0)
        kin.check_move(Move((10000.,10000.,10000.)))
        class Toolhead:
            def wait_moves(self): pass
        class Command:
            def get_int(self, key, default=None, **kwargs): return 3 if key=='A' else default
            def respond_info(self, message): self.response=message
        kin.toolhead=Toolhead()
        command=Command()
        kin.cmd_M666(command)
        self.assertEqual(helper.anchor_mode, 3)
        self.assertIn('cube_corner', command.response)
        with self.assertRaises(ValueError): helper.set_anchor_mode(4)

    def test_last_top_lower_prism_and_all_top_no_height_limit(self):
        helper = self.build_helper(FOUR_ANCHORS_DEFAULT, anchor_mode='last_on_top')
        self.assertTrue(helper.is_reachable((0.,0.,-1000.)))
        self.assertFalse(helper.is_reachable((0.,0.,3000.)))
        anchors = [(1000.,-1000.,1900.),(1000.,1000.,1900.),(-1000.,1000.,1900.),(-1000.,-1000.,1900.)]
        helper = self.build_helper(anchors, anchor_mode='all_on_top')
        self.assertTrue(helper.is_reachable((0.,0.,3000.)))
        for pos in ((1100.,0.,0.),(-1100.,0.,0.),(0.,1100.,0.),(0.,-1100.,0.)):
            self.assertFalse(helper.is_reachable(pos))

    def test_disabled_flex_matches_geometry(self):
        helper = self.build_helper(FOUR_ANCHORS_DEFAULT,
                                   mover_weight=0.0,
                                   spring=0.0,
                                   target=0.0)
        self.assertFalse(helper.enabled)
        ptr = helper.get_ptr()
        self.assertTrue(ptr)
        stepper = ffi.gc(lib.winch_stepper_alloc(ptr, 0), lib.free)
        dist = lib.itersolve_calc_position_from_coord(stepper, 0.0, 0.0, 0.0)
        expected = 0.0
        self.assertAlmostEqual(dist, expected, places=6)
        for pos in ((0.0, 0.0, 0.0), (100.0, -50.0, 10.0), (-200.0, 150.0, 75.0)):
            lengths, distances, flex = self.spool_lengths(helper, pos)
            for d, f, l in zip(distances, flex, lengths):
                self.assertAlmostEqual(f, 0.0, places=9)
                self.assertAlmostEqual(l, d, places=9)

    def test_parallel_lines_share_flex(self):
        single = self.build_helper(FOUR_ANCHORS_DEFAULT, mechanical_advantage=[2, 2, 2, 4],
                                   min_force=[10.0] * 4)
        shared = self.build_helper(FOUR_ANCHORS_DEFAULT, mechanical_advantage=[2, 2, 2, 4],
                                   min_force=[10.0] * 4,
                                   lines_per_axis=[2, 2, 2, 3])
        single.set_active(True)
        shared.set_active(True)
        for pos in ((0.0, 0.0, 0.0), (300.0, -200.0, 50.0)):
            _, _, flex_single = self.spool_lengths(single, pos)
            _, _, flex_shared = self.spool_lengths(shared, pos)
            for f1, fn, n in zip(flex_single, flex_shared, (2, 2, 2, 3)):
                self.assertNotAlmostEqual(f1, 0.0, places=3)
                self.assertAlmostEqual(fn, f1 / n, places=9)

    def test_flex_reels_the_elastic_stretch_of_each_line(self):
        force = 20.0
        spring = 20000.0
        mechanical_advantage = (1, 2, 2, 4)
        lines_per_axis = (1, 2, 2, 3)
        guy_wires = (100.0, 200.0, 300.0, 400.0)
        helper = self.build_helper(
            FOUR_ANCHORS_DEFAULT, spring=spring,
            min_force=[force] * 4, max_force=[force] * 4,
            mechanical_advantage=mechanical_advantage,
            lines_per_axis=lines_per_axis, guy_wires=guy_wires)
        helper.set_active(True)
        for pos in ((0.0, 0.0, 0.0), (300.0, -200.0, 50.0)):
            distances, flex = helper.calc_arrays(pos)
            for distance, offset, ma, n, guy in zip(
                    distances, flex, mechanical_advantage,
                    lines_per_axis, guy_wires):
                reeled_line = -offset * ma
                strain = reeled_line / (distance * ma + guy)
                self.assertAlmostEqual(strain * spring, force / (ma * n),
                                       places=9)

    def line_forces(self, helper, pos, spring, guy_wire):
        distances, flex = helper.calc_arrays(pos)
        return [-f * spring / (d + guy_wire)
                for d, f in zip(distances, flex)], distances

    def wrench_residual(self, anchors, attachments, pos, forces, distances,
                        weight):
        force = [0.0, 0.0, -weight]
        moment = [0.0, 0.0, 0.0]
        for anchor, arm, tension, dist in zip(anchors, attachments, forces,
                                              distances):
            pull = [tension * (anchor[i] - pos[i]) / dist for i in range(3)]
            for i in range(3):
                force[i] += pull[i]
            moment[0] += arm[1] * pull[2] - arm[2] * pull[1]
            moment[1] += arm[2] * pull[0] - arm[0] * pull[2]
            moment[2] += arm[0] * pull[1] - arm[1] * pull[0]
        return force, moment

    def test_attachments_balance_moments_on_the_mover(self):
        # Eight lines from the corners of a 400 mm cube
        corners = [(x, y, z) for z in (200.0, -200.0)
                   for x, y in ((200.0, -200.0), (200.0, 200.0),
                                (-200.0, 200.0), (-200.0, -200.0))]
        anchors = [(1284.9 * x / 200.0, 1284.9 * y / 200.0,
                    1900.0 if z > 0.0 else -280.0) for x, y, z in corners]
        mass, spring, guy_wire = 0.216, 20000.0, 117.2
        weight = mass * 9.81
        options = dict(mover_weight=mass, spring=spring,
                       min_force=[3.0] * 8, max_force=[170.0] * 8,
                       guy_wires=[guy_wire] * 8)
        point = self.build_helper(anchors, **options)
        rigid = self.build_helper(anchors, attachments=corners, **options)
        for helper in (point, rigid):
            helper.set_active(True)
        previous = None
        for step in range(61):
            pos = (10.0 * step, 6.0 * step, 0.0)
            forces, distances = self.line_forces(rigid, pos, spring, guy_wire)
            force, moment = self.wrench_residual(
                anchors, corners, pos, forces, distances, weight)
            for value in force:
                self.assertAlmostEqual(value, 0.0, delta=0.05)
            for value in moment:
                # N*mm
                self.assertAlmostEqual(value, 0.0, delta=20.0)
            for tension in forces:
                self.assertGreaterEqual(tension, 3.0 - 1e-6)
            if previous is not None:
                for now, before in zip(forces, previous):
                    self.assertLess(abs(now - before), 2.0)
            previous = forces
        # The point model leaves a moment for the lines to resolve by
        # tilting the mover.
        pos = (500.0, 300.0, 0.0)
        forces, distances = self.line_forces(point, pos, spring, guy_wire)
        _, moment = self.wrench_residual(
            anchors, corners, pos, forces, distances, weight)
        self.assertGreater(max(abs(value) for value in moment), 200.0)

    def test_four_anchor_small_perturbation(self):
        helper = self.build_helper(FOUR_ANCHORS_DEFAULT)
        origin_lengths, _, _ = self.spool_lengths(helper, (0.0, 0.0, 0.0))

        def rel(lengths):
            return [l - o for l, o in zip(lengths, origin_lengths)]

        left = rel(self.spool_lengths(helper, (16.3, 0.0, 0.0))[0])
        nominal = rel(self.spool_lengths(helper, (16.4, 0.0, 0.0))[0])
        right = rel(self.spool_lengths(helper, (16.5, 0.0, 0.0))[0])

        for i in range(4):
            self.assertLess(abs(left[i] - nominal[i]), 0.2)
            self.assertLess(abs(right[i] - nominal[i]), 0.2)
            self.assertLess(abs(left[i] - right[i]), 0.2)
        for i in range(3):
            self.assertGreaterEqual(abs(left[i] - nominal[i]), 0.0)
            self.assertGreaterEqual(abs(right[i] - nominal[i]), 0.0)
            self.assertGreaterEqual(abs(left[i] - right[i]), 0.0)

    def test_buildup_model_matches_closed_form_with_gearing(self):
        buildup = 0.6366197723675814
        rotation_distance = 246.20
        steps_per_rotation = 3200.0 * 12.75
        helper = self.build_helper(((0.0, 0.0, 0.0),),
                                   mover_weight=0.0,
                                   spring=0.0,
                                   target=0.0,
                                   buildup_factor=buildup,
                                   mechanical_advantage=[1])
        helper.set_spool_params(0, rotation_distance, steps_per_rotation)
        motor_pos = 18.5
        actual = helper.motor_to_line_pos(0, motor_pos)
        expected = self.expected_motor_to_line_pos(
            motor_pos, rotation_distance, steps_per_rotation, buildup, 1)
        self.assertAlmostEqual(actual, expected, places=12)

    def test_buildup_model_scales_with_mechanical_advantage(self):
        buildup = 0.6366197723675814
        rotation_distance = 123.10
        steps_per_rotation = 3200.0 * 12.75
        helper = self.build_helper(((0.0, 0.0, 0.0),),
                                   mover_weight=0.0,
                                   spring=0.0,
                                   target=0.0,
                                   buildup_factor=buildup,
                                   mechanical_advantage=[2])
        helper.set_spool_params(0, rotation_distance, steps_per_rotation)
        motor_pos = 18.5
        actual = helper.motor_to_line_pos(0, motor_pos)
        expected = self.expected_motor_to_line_pos(
            motor_pos, rotation_distance, steps_per_rotation, buildup, 2)
        self.assertAlmostEqual(actual, expected, places=12)

    def test_immediate_flex_transition_uses_origin_relative_lengths(self):
        helper = self.build_helper(
            FOUR_ANCHORS_DEFAULT,
            mover_weight=0.006,
            spring=20000.0,
            buildup_factor=0.6366197723675814,
            guy_wires=[528.3, 528.3, 528.3, 528.3],
            mechanical_advantage=[1, 1, 1, 1],
        )
        for idx in range(4):
            helper.set_spool_params(idx, 246.20, 3200.0)
        current_pos = (0.0, 0.0, 0.0)
        distances, flex = helper.calc_arrays(current_pos)
        prev_lengths = [helper.motor_to_line_pos(idx, 0.0) for idx in range(4)]
        deltas = []
        for idx in range(4):
            target_length = (distances[idx]
                             - helper.origin_distances[idx]
                             + flex[idx])
            deltas.append(target_length - prev_lengths[idx])
        for delta in deltas:
            self.assertLess(abs(delta), 1.0)

    def test_four_anchor_near_singularity(self):
        helper = self.build_helper(FOUR_ANCHORS_PROBLEM)
        origin_lengths, _, _ = self.spool_lengths(helper, (0.0, 0.0, 0.0))

        def rel(lengths):
            return [l - o for l, o in zip(lengths, origin_lengths)]

        left = rel(self.spool_lengths(helper, (-0.1, 0.0, 0.0))[0])
        nominal = [0.0] * len(origin_lengths)
        right = rel(self.spool_lengths(helper, (0.1, 0.0, 0.0))[0])

        for i in range(4):
            self.assertLess(abs(left[i] - nominal[i]), 10.0)
            self.assertLess(abs(right[i] - nominal[i]), 10.0)
            self.assertLess(abs(left[i] - right[i]), 10.0)
        for i in range(3):
            self.assertGreater(abs(left[i] - nominal[i]), 0.0001)
            self.assertGreater(abs(right[i] - nominal[i]), 0.0001)
            self.assertGreater(abs(left[i] - right[i]), 0.0001)

    def test_five_anchor_mass_zero(self):
        helper = self.build_helper(FIVE_ANCHORS_DEFAULT,
                                   mover_weight=0.0,
                                   spring=0.0,
                                   target=0.0)
        self.assertFalse(helper.enabled)
        for pos in ((0.0, 0.0, 0.0), (100.0, -50.0, 10.0)):
            lengths, distances, flex = self.spool_lengths(helper, pos)
            for d, f, l in zip(distances, flex, lengths):
                self.assertAlmostEqual(f, 0.0, places=9)
                self.assertAlmostEqual(l, d, places=9)

    def test_five_anchor_origin(self):
        helper = self.build_helper(FIVE_ANCHORS_DEFAULT)
        lengths, distances, flex = self.spool_lengths(helper, (0.0, 0.0, 0.0))
        for d, f, l in zip(distances, flex, lengths):
            self.assertAlmostEqual(f, 0.0, places=9)
            self.assertAlmostEqual(l, d, places=9)


if __name__ == '__main__':
    unittest.main()
