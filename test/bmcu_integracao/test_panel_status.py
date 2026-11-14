import re
import types
import unittest

from test_integracao import Gcmd, ROOT, TestBench, import_module

REAL_GCODE = import_module('panel_gcode_parser', ROOT / 'klippy/gcode.py')


class PanelBench(TestBench):
    def __init__(self):
        super().__init__()
        self.obj.manual_operation = None
        self.obj.pending_tool_change = None
        self.print_state = 'standby'
        self.printer.is_shutdown = lambda: False
        lookup = self.printer.lookup_object
        self.printer.lookup_object = lambda name, *args: (
            types.SimpleNamespace(get_status=lambda t: {
                'state': self.print_state,
            }) if name == 'print_stats' else lookup(name, *args)
        )
        self.device.uid = 'SYNTHETIC_BMCU'
        self.device.last_error = ''
        self.device.caps = {'firmware': '1.0.4'}
        self.device.status['route_state'] = [0, 0, 0, 1]
        self.bmcu.active_operations = {}
        self.bmcu.print_plan_open = False
        self.bmcu.print_map_active = False
        self.bmcu.loaded_tools = {'bmcu0:3': 4}
        self.bmcu._route_state = lambda device, channel: (
            device.status['route_state'][channel]
        )
        self.bmcu.refill = types.SimpleNamespace(manual_pending=None)
        self.metadata = [dict(material='PETG', color='#123ABC')
                         for _ in range(4)]
        self.bmcu._channel_metadata = lambda device, channel: (
            self.metadata[channel]
        )
        self.writes = []
        self.bmcu.cmd_SET_FILAMENT = lambda gcmd: self.writes.append(
            gcmd.get_command_parameters()
        )
        self.gcode.create_gcode_command = (
            lambda cmd, text, params: Gcmd(**params))

    def panel(self):
        status = self.obj.get_status(0.)
        if 'panel' not in status:
            raise AssertionError('Missing live BMCU dashboard status')
        return status['panel']


class PanelStatusTests(unittest.TestCase):
    def test_frontend_command_survives_the_real_klipper_parser(self):
        source = (ROOT / 'integrations/mainsail/overlay/src/components/panels'
                  '/Bmcu/bmcu.ts').read_text()
        template = re.search(
            r'return `(BMCU_ENDER_SET_FILAMENT[^`]+)`', source)[1]
        for key, value in (('channel', '2'), ('identifier', 'PETG'),
                           ('hex', '1122AA'), ('target.revision', 'a' * 64)):
            template = template.replace('${' + key + '}', value)
        dispatcher = object.__new__(REAL_GCODE.GCodeDispatch)
        command = REAL_GCODE.GCodeCommand(
            dispatcher, 'BMCU_ENDER_SET_FILAMENT', template, {}, False)
        dispatcher._get_extended_params(command)
        self.assertEqual('1122AA', command.get('COLOR'))
        self.assertEqual('a' * 64, command.get('REVISION'))

    def test_live_snapshot_does_not_refresh_uart_or_use_frozen_status(self):
        b = PanelBench()
        b.device.refresh = lambda: self.fail('UART from status rendering')
        b.bmcu.get_status = lambda t: self.fail('Frozen native public snapshot')
        status = b.panel()
        self.assertEqual(1, status['schema'])
        self.assertEqual('1.1.2', status['package_version'])
        self.assertTrue(status['can_edit'])
        self.assertEqual(['empty', 'parked', 'empty', 'loaded'],
                         [ch['state'] for ch in status['channels']])
        self.assertTrue(all(len(ch['revision']) == 64
                            for ch in status['channels']))

    def test_incompatible_or_offline_never_reports_loaded_or_editable(self):
        for field in ('connected', 'ready', 'runtime_configured',
                      'firmware_compatible', 'status_reconciled'):
            with self.subTest(field=field):
                b = PanelBench()
                setattr(b.device, field, False)
                status = b.panel()
                self.assertFalse(status['can_edit'])
                self.assertTrue(all(ch['state'] == 'offline'
                                    for ch in status['channels']))

    def test_untrusted_routes_shutdown_and_conflicts_cannot_be_edited(self):
        for failure in ('route', 'conflict', 'error', 'shutdown'):
            with self.subTest(failure=failure):
                b = PanelBench()
                if failure == 'route':
                    b.device.status['route_state'][0] = 77
                elif failure == 'conflict':
                    b.device.status['route_state'][0] = 1
                elif failure == 'error':
                    b.bmcu.last_error = {'code': 'UNCERTAIN_PATH'}
                else:
                    b.printer.is_shutdown = lambda: True
                self.assertFalse(b.panel()['can_edit'])

    def test_revision_is_bound_to_channel_device_and_session(self):
        b = PanelBench()
        revisions = [ch['revision'] for ch in b.panel()['channels']]
        self.assertEqual(4, len(set(revisions)))
        b.device.status['session_id'] += 1
        self.assertNotEqual(revisions[0], b.panel()['channels'][0]['revision'])

    def test_native_runout_is_visible_but_never_cleared_by_reading(self):
        b = PanelBench()
        pending = dict(device='bmcu0', channel=3, endpoint='extruder', tool=4)
        b.bmcu.refill.manual_pending = pending
        b.print_state = 'paused'
        status = b.panel()
        self.assertEqual(4, status['refill_channel'])
        self.assertTrue(status['refill_pending'])
        self.assertFalse(status['can_edit'])
        self.assertIs(pending, b.bmcu.refill.manual_pending)

    def test_safe_editor_delegates_only_metadata_to_native_command(self):
        b = PanelBench()
        revision = b.panel()['channels'][1]['revision']
        self.assertTrue(hasattr(b.obj, 'set_panel_filament'))
        b.obj.set_panel_filament(Gcmd(
            CHANNEL=2, MATERIAL='petg-cf', COLOR='1122aa', REVISION=revision,
        ))
        self.assertEqual([dict(DEVICE='bmcu0', CHANNEL='1',
                               MATERIAL='PETG-CF', COLOR='#1122AA')], b.writes)
        self.assertFalse(b.commands)
        self.assertFalse(b.actuators)

    def test_editor_rejects_stale_dialog_injection_and_busy_state(self):
        for invalid in ('revision', 'material', 'color', 'printing',
                        'paused', 'operation', 'firmware', 'manual'):
            with self.subTest(invalid=invalid):
                b = PanelBench()
                params = dict(CHANNEL=1, MATERIAL='PETG', COLOR='#FFFFFF',
                              REVISION=b.panel()['channels'][0]['revision'])
                if invalid == 'revision':
                    b.metadata[0]['color'] = '#000000'
                elif invalid == 'material':
                    params['MATERIAL'] = 'PETG\nG28'
                elif invalid == 'color':
                    params['COLOR'] = '#FFFFFF\nG28'
                elif invalid in ('printing', 'paused'):
                    b.print_state = invalid
                elif invalid == 'operation':
                    b.bmcu.active_operations = {'bmcu0': {'phase': 'LOAD'}}
                elif invalid == 'firmware':
                    b.device.firmware_compatible = False
                else:
                    b.obj.manual_operation = 'LOAD'
                self.assertTrue(hasattr(b.obj, 'set_panel_filament'))
                with self.assertRaises(RuntimeError):
                    b.obj.set_panel_filament(Gcmd(**params))
                self.assertFalse(b.writes)
                self.assertFalse(b.actuators)


if __name__ == '__main__':
    unittest.main()
