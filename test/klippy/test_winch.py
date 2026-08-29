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
from kinematics.winch import WinchFlexHelper  # noqa: E402


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
                     mechanical_advantage=None):
        num = len(anchors)
        if min_force is None:
            min_force = [0.0] * num
        if max_force is None:
            max_force = [120.0] * num
        if mechanical_advantage is None:
            mechanical_advantage = [1] * num
        cfg = configparser.ConfigParser()
        options = {
            'winch_mover_weight': f"{mover_weight}",
            'winch_spring_constant': f"{spring}",
            'winch_target_force': f"{target}",
            'winch_min_force': list_str(min_force),
            'winch_max_force': list_str(max_force),
            'winch_buildup_factor': f"{buildup_factor}",
            'winch_mechanical_advantage': int_list_str(mechanical_advantage),
        }
        if guy_wires is not None:
            options['winch_guy_wire_lengths'] = list_str(guy_wires)
        cfg['printer'] = options
        wrapper = configfile.ConfigWrapper(DummyPrinter(), cfg, {}, 'printer')
        return WinchFlexHelper(tuple(anchors), wrapper)

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
