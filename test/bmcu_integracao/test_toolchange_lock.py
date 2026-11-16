import ast
import json
import types
import unittest

from test_bmcu_112 import CONTRACT
from test_cortador import BancadaCortador
from test_integracao import BASE, Gcmd, NATIVE_MANAGER, installer, protocol


LOCK_SOURCE = (BASE / 'operation_lock_112_original.txt').read_text()


def run_nodes(nodes, namespace):
    exec(compile(ast.Module(body=nodes, type_ignores=[]),
                 '<native lock boundary>', 'exec'), namespace)


class ToolchangeLockTests(unittest.TestCase):
    def bench(self, source=1, target=2, source_device=True,
              driver='generic_single_extruder', macro='BMCU_RELEASE_ENDER'):
        b = BancadaCortador()
        native = {'BMCUError': RuntimeError, 'json': json}
        exec(compile(LOCK_SOURCE, '<native lock methods>', 'exec'), native)
        manager = b.bmcu
        manager.active_operations = {}
        manager.endpoint_locks = set()
        manager.path_locks = set()
        manager.debug_enabled = False
        manager._maintenance_prepared = lambda: False
        manager._u1_background_channel_conflict = lambda *args: False
        manager._publish_active_operations = lambda: None
        manager._release_u1_foreground_async_slot = lambda *args: None
        manager.reactor = b.reactor
        for name in ('_lock', '_unlock', '_lock_refill', '_unlock_refill',
                     '_set_phase'):
            setattr(manager, name, types.MethodType(
                getattr(native['BMCUManager'], name), manager))
        b.endpoint.driver = driver
        b.endpoint.get = lambda name, default=None: (
            macro if name == 'before_pullback_macro' else default)
        b.endpoint.shared_path_group = lambda: 'shared-extruder'
        b.device.status['present'] = [True] * 4
        b.lock_source = installer.patch_toolchange_lock(NATIVE_MANAGER)
        b.namespace = dict(self=manager, device=b.device, endpoint=b.endpoint,
                           source_device=b.device if source_device else None,
                           source_channel=source, channel=target,
                           operation_label='T%d' % target)
        return b

    def acquire(self, b):
        run_nodes(CONTRACT.toolchange_boundaries(b.lock_source)[0], b.namespace)

    def release_lock(self, b):
        run_nodes(
            [CONTRACT.toolchange_boundaries(b.lock_source)[1]], b.namespace)
        self.assertFalse(b.bmcu.active_operations)
        self.assertFalse(b.bmcu.endpoint_locks)
        self.assertFalse(b.bmcu.path_locks)

    def test_all_same_device_swaps_reach_native_cut_phase_and_release(self):
        for source in range(4):
            for target in range(4):
                if source == target:
                    continue
                with self.subTest(source=source, target=target):
                    b = self.bench(source, target)
                    self.acquire(b)
                    b.bmcu._set_phase(b.device, 'PRECHECK', channel=target)
                    for phase in ('UNLOAD_SOURCE', 'PRECHECK_UNLOAD',
                                  'SELECT_ENDPOINT', 'CUT_OR_FORM'):
                        b.bmcu._set_phase(b.device, phase, channel=source)
                    operation = b.bmcu.active_operations['bmcu0']
                    self.assertEqual('CUT_OR_FORM', operation['phase'])
                    self.assertEqual(sorted([source, target]),
                                     operation['channels'])
                    b.endpoint._ender_unload_context = ('bmcu0', source, 3)
                    self.assertEqual(
                        (b.device, b.endpoint, source),
                        b.obj.context(Gcmd(), False))
                    b.bmcu._set_phase(b.device, 'TOOLHEAD_PREPARATION',
                                      channel=target)
                    b.endpoint._ender_load_context = ('bmcu0', target)
                    self.assertEqual(
                        (b.device, b.endpoint, target),
                        b.obj.context(Gcmd(), True))
                    self.release_lock(b)

    def test_unrelated_channel_cannot_change_paired_operation_phase(self):
        b = self.bench()
        self.acquire(b)
        b.bmcu._set_phase(b.device, 'PRECHECK', channel=2)
        b.bmcu._set_phase(b.device, 'CUT_OR_FORM', channel=0)
        self.assertEqual('PRECHECK', b.bmcu.active_operations['bmcu0']['phase'])
        self.release_lock(b)

    def test_empty_load_other_callbacks_and_same_channel_keep_single_lock(self):
        for settings in (
            dict(source_device=False),
            dict(driver='snapmaker_u1'),
            dict(macro='OTHER_CALLBACK'),
            dict(source=2, target=2),
        ):
            with self.subTest(settings=settings):
                b = self.bench(**settings)
                self.acquire(b)
                record = b.bmcu.active_operations['bmcu0']
                self.assertEqual(2, record['channel'])
                self.assertNotIn('channels', record)
                self.assertNotIn('refill', record)
                self.release_lock(b)

    def test_source_and_target_background_conflicts_block_before_any_lock(self):
        for busy in (1, 2):
            with self.subTest(busy=busy):
                b = self.bench()
                b.bmcu._u1_background_channel_conflict = (
                    lambda device, channel: channel == busy)
                with self.assertRaisesRegex(RuntimeError, 'background'):
                    self.acquire(b)
                self.assertFalse(b.bmcu.active_operations)
                self.assertFalse(b.bmcu.endpoint_locks)
                self.assertFalse(b.bmcu.path_locks)
                self.assertFalse(b.moves)

    def test_unload_failure_releases_paired_locks_without_cutting(self):
        b = self.bench()
        self.acquire(b)
        try:
            b.bmcu._set_phase(b.device, 'CUT_OR_FORM', channel=1)
            b.endpoint._ender_unload_context = ('bmcu0', 1, 2)
            with self.assertRaisesRegex(RuntimeError, 'context changed'):
                b.obj.release(Gcmd())
        finally:
            self.release_lock(b)
        self.assertFalse(b.moves)
        self.assertFalse(b.policies)

    def test_native_release_cuts_and_withdraws_with_paired_lock(self):
        for source in range(4):
            for target in range(4):
                if source == target:
                    continue
                with self.subTest(source=source, target=target):
                    b = self.bench(source, target)
                    self.acquire(b)
                    b.bmcu._set_phase(b.device, 'CUT_OR_FORM', channel=source)
                    operation = b.bmcu.active_operations['bmcu0']

                    def set_motion(channel, mode):
                        # Do not use the old bench's synthetic phase assignment.
                        b.motion(channel, mode)
                        b.bmcu.active_operations['bmcu0'] = operation

                    b.device.set_motion = set_motion
                    b.endpoint.prepare_toolhead_for_pullback = (
                        lambda *args, **kwargs: b.obj.release(Gcmd()))
                    native = {'protocol': protocol}
                    exec(compile(installer.patch_pre_unload(NATIVE_MANAGER),
                                 '<native unload boundary>', 'exec'), native)
                    try:
                        context = native['BMCUManager']._begin_unload_locked(
                            b.bmcu, b.device, b.endpoint, source)
                        self.assertFalse(context['before_pullback_started'])
                        self.assertFalse(b.obj.sensor(b.endpoint))
                        self.assertEqual([(source, 55.0)], b.policies)
                        self.assertEqual('awaiting_parking',
                                         b.obj.last_unload_timing['result'])
                        self.assertEqual(sorted([source, target]),
                                         operation['channels'])
                        self.assertEqual([0] * 4, b.device.status['motion'])
                    finally:
                        self.release_lock(b)

    def test_busy_resources_and_maintenance_keep_existing_locks(self):
        for conflict in ('maintenance', 'device', 'endpoint', 'path'):
            with self.subTest(conflict=conflict):
                b = self.bench()
                if conflict == 'maintenance':
                    b.bmcu._maintenance_prepared = lambda: True
                elif conflict == 'device':
                    b.bmcu.active_operations['bmcu0'] = {'name': 'EXISTING'}
                elif conflict == 'endpoint':
                    b.bmcu.endpoint_locks.add('extruder')
                else:
                    b.bmcu.path_locks.add('shared-extruder')
                before = (dict(b.bmcu.active_operations),
                          set(b.bmcu.endpoint_locks), set(b.bmcu.path_locks))
                with self.assertRaisesRegex(RuntimeError, 'blocked|busy'):
                    self.acquire(b)
                self.assertEqual(before, (b.bmcu.active_operations,
                                          b.bmcu.endpoint_locks,
                                          b.bmcu.path_locks))
                self.assertFalse(b.moves)

    def test_multi_device_switch_keeps_paired_locks_for_every_driver(self):
        for driver in ('generic_single_extruder', 'snapmaker_u1'):
            with self.subTest(driver=driver):
                b = self.bench(driver=driver)
                b.namespace['source_device'] = types.SimpleNamespace(
                    name='bmcu1')
                self.acquire(b)
                source = b.bmcu.active_operations['bmcu1']
                target = b.bmcu.active_operations['bmcu0']
                self.assertEqual(1, source['channel'])
                self.assertEqual(2, target['channel'])
                self.assertTrue(source['refill'])
                self.assertTrue(target['refill'])
                self.release_lock(b)

    def test_patch_is_idempotent_on_clean_and_installed_callback_sources(self):
        for source in (NATIVE_MANAGER, installer.patch_pre_unload(
                installer.patch_pre_capture(NATIVE_MANAGER))):
            with self.subTest(installed=source != NATIVE_MANAGER):
                result = installer.patch_toolchange_lock(source)
                self.assertEqual(
                    result, installer.patch_toolchange_lock(result))
                self.assertEqual(2, result.count(
                    'if multi_device_switch or ender_channel_switch:'))

    def test_unknown_or_partially_patched_lock_layout_is_rejected(self):
        patched = installer.patch_toolchange_lock(NATIVE_MANAGER)
        cases = (
            NATIVE_MANAGER.replace('replacement_channel=channel',
                                   'replacement_channel=None', 1),
            NATIVE_MANAGER.replace(
                'self._unlock_refill(source_device, device, endpoint)',
                'self._unlock_refill(device, source_device, endpoint)', 1),
            patched.replace(
                'if multi_device_switch or ender_channel_switch:',
                'if multi_device_switch:', 1),
            patched.replace(
                'if multi_device_switch or ender_channel_switch:',
                'if multi_device_switch:', 2),
        )
        for source in cases:
            with self.subTest(source=source[-200:]):
                with self.assertRaisesRegex(RuntimeError, 'tool-change lock'):
                    installer.patch_toolchange_lock(source)

    def test_native_lock_phase_and_boundary_fixtures_detect_drift(self):
        original = NATIVE_MANAGER + '\n' + LOCK_SOURCE
        CONTRACT.check_toolchange_contract(original)
        for changed, message in (
            (original.replace('wanted_channel not in operation_channels',
                              'wanted_channel in operation_channels'),
             '_set_phase'),
            (original.replace('source_channel=source_channel',
                              'source_channel=channel', 1),
             'lock boundary'),
        ):
            with self.subTest(message=message):
                with self.assertRaisesRegex(ValueError, message):
                    CONTRACT.check_toolchange_contract(changed)


if __name__ == '__main__':
    unittest.main()
