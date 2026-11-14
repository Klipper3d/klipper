import ast
import types
import unittest
from unittest.mock import patch

from test_cutter_safety import configured_integration
from test_integracao import (
    BASE, Gcmd, ROOT, TestBench, import_module, installer, module, protocol,
)

CONTRACT_PATH = ROOT / 'scripts/ender3-v3-se/check_bmcu_contracts.py'
CONTRACT = (import_module('test_bmcu_contract_checker', CONTRACT_PATH)
            if CONTRACT_PATH.exists() else None)


class Bmcu112Tests(unittest.TestCase):
    def test_refill_fixture_is_compared_to_native_method_bodies(self):
        self.assertIsNotNone(CONTRACT, 'Missing pinned native source checker')
        original = (BASE / 'refill_112_original.txt').read_text()
        CONTRACT.check_refill_contract(original, original)
        changed = original.replace('insert replacement filament', 'skip refill')
        with self.assertRaisesRegex(ValueError, 'resume_manual_refill'):
            CONTRACT.check_refill_contract(changed, original)

    def test_supported_package_is_112_not_106(self):
        with patch.object(module, 'PACKAGE_VERSION', '1.1.2'):
            errors = []
            try:
                configured_integration()
            except RuntimeError as error:
                errors.append(str(error))
            self.assertEqual([], errors)
        with patch.object(module, 'PACKAGE_VERSION', '1.0.6'):
            with self.assertRaisesRegex(RuntimeError, '1.1.2'):
                configured_integration()

    def test_incompatible_firmware_never_starts_load(self):
        b = TestBench()
        b.device.firmware_compatible = False
        b.device.firmware_error = 'Requires firmware 1.0.4'
        with self.assertRaisesRegex(RuntimeError, 'firmware 1.0.4'):
            b.obj.load(Gcmd())
        self.assertEqual(0.0, b.e)
        self.assertFalse(b.head_moves)

    def test_real_112_capture_branch_is_supported_and_idempotent(self):
        original = (BASE / 'manager_original.txt').read_text()
        old = (
            '            else:\n'
            '                device.set_motion(channel, '
            'protocol.MOTION_BEFORE_ON_USE)\n\n'
            '            if not generic_contract:'
        )
        new = (
            '            else:\n'
            "                if endpoint.driver == 'snapmaker_u1':\n"
            '                    self._set_u1_foreground_load_motion(\n'
            '                        device, channel, '
            'protocol.MOTION_BEFORE_ON_USE)\n'
            '                else:\n'
            '                    device.set_motion(channel, '
            'protocol.MOTION_BEFORE_ON_USE)\n\n'
            '            if not generic_contract:'
        )
        self.assertEqual(1, original.count(old))
        source = original.replace(old, new)
        errors = []
        try:
            updated = installer.patch_pre_capture(source)
        except RuntimeError as error:
            errors.append(str(error))
        self.assertEqual([], errors)
        self.assertEqual(updated, installer.patch_pre_capture(updated))
        tree = ast.parse(updated)
        conditional = next(
            n for n in ast.walk(tree) if isinstance(n, ast.If)
            and 'BMCU_PREPARE_ENDER' in ast.dump(n.test)
        )
        actions = []
        device = types.SimpleNamespace(
            name='bmcu0', stop_all=lambda: actions.append('stop'),
        )
        namespace = dict(
            generic_contract=True, device=device, channel=3,
            endpoint=types.SimpleNamespace(
                get=lambda *args: 'BMCU_PREPARE_ENDER'),
            protocol=protocol,
        )
        exec(compile(ast.Module(body=[conditional], type_ignores=[]),
                     '<112_capture>', 'exec'), namespace)
        self.assertEqual(['stop'], actions)
        self.assertEqual(('bmcu0', 3),
                         namespace['endpoint']._ender_load_context)


if __name__ == '__main__':
    unittest.main()
