# Tests for optional blocking temperature probe calibration.
# Run with: python test/temperature_probe_wait.py
import pathlib
import collections
import sys
import unittest
from unittest.mock import Mock, patch

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'klippy'))
import gcode
from extras import temperature_probe


class Mutex:
    def __init__(self):
        self.locked = False

    def __enter__(self):
        if self.locked:
            raise AssertionError('Attempted to reacquire the G-Code mutex')
        self.locked = True

    def __exit__(self, *args):
        self.locked = False


class CalibrationTest(unittest.TestCase):
    def setUp(self):
        self.dispatch = gcode.GCodeDispatch.__new__(gcode.GCodeDispatch)
        self.dispatch.async_command_handlers = {}
        self.dispatch.is_printer_ready = True
        self.dispatch.mutex = Mutex()
        self.dispatch.register_command = Mock()
        self.dispatch.respond_info = Mock()
        self.dispatch._process_commands = Mock()
        self.dispatch.printer = Mock(config_error=ValueError)
        self.probe = p = temperature_probe.TemperatureProbe.__new__(
            temperature_probe.TemperatureProbe)
        p.name = 'temperature_probe test'
        p.gcode = self.dispatch
        p.cal_helper = Mock()
        p.cal_bed_temp = 80.
        p.printer = Mock(config_error=ValueError)
        p.printer.is_shutdown.return_value = False
        p._wait = p._abort_requested = p.in_calibration = False
        p._tap_probe = None
        p._gcode_params = ''
        p.last_measurement = (20., 20., 20.)
        p.last_zero_pos = None
        p.total_expansion = 0.
        p.next_auto_temp = 99999999.
        p._check_homed = Mock()
        p._get_probe = Mock()
        p._get_probe.return_value.get_status.return_value = {
            'name': 'probe_eddy_current test'}
        p._move_to_start = Mock()
        p._set_extruder_temp = Mock()
        p._set_bed_temp = Mock()
        p._collect_sample = Mock(return_value=20.)
        p._prepare_next_sample = Mock(side_effect=self.prepare)
        self.toolhead = Mock()
        self.toolhead.get_position.return_value = [0., 0., 1., 0.]
        self.result = Mock(bed_z=0.)
        self.tap = Mock(return_value=self.result)
        self.manual = Mock()
        self.manual.get_manual_method.return_value = self.tap
        p.printer.lookup_object.side_effect = {
            'toolhead': self.toolhead, 'manual_probe': self.manual,
            'gcode': self.dispatch}.__getitem__
        self.reactor = Mock()
        self.reactor.monotonic.return_value = 0.
        p.printer.get_reactor.return_value = self.reactor

    def prepare(self, temp, z):
        self.probe.next_auto_temp = temp + self.probe.step

    def command(self, **params):
        params = dict(TARGET='30', MANUAL_METHOD='tap', **params)
        raw = 'TEMPERATURE_PROBE_CALIBRATE ' + ' '.join(
            '%s=%s' % item for item in params.items())
        return gcode.GCodeCommand(self.dispatch,
                                 'TEMPERATURE_PROBE_CALIBRATE', raw,
                                 params, False)

    def run_calibration(self, cmd):
        with self.dispatch.mutex:
            self.probe.cmd_TEMPERATURE_PROBE_CALIBRATE(cmd)

    def assert_clean(self):
        self.assertFalse(self.probe.in_calibration)
        self.assertFalse(self.probe._wait)
        self.assertEqual(self.dispatch.async_command_handlers, {})
        self.probe._set_extruder_temp.assert_called_with(0)
        self.probe._set_bed_temp.assert_called_with(0)

    def test_default_and_wait_zero_return_without_waiting(self):
        for params in ({}, {'WAIT': '0'}):
            with self.subTest(params=params):
                self.setUp()
                cmd = self.command(**params)
                with patch.object(temperature_probe.manual_probe,
                                  'ManualProbeHelper') as helper:
                    self.run_calibration(cmd)
                helper.assert_called_once()
                self.tap.assert_not_called()
                self.reactor.pause.assert_not_called()
                self.assertTrue(self.probe.in_calibration)
                self.assertFalse(self.probe._wait)
                self.assertIn('MANUAL_METHOD=tap', self.probe._gcode_params)

    def test_wait_runs_samples_until_target(self):
        cmd = self.command(WAIT='1')
        self.probe._get_speeds = Mock(return_value=(5., 5., 5.))
        self.probe.horizontal_move_z = 2.
        self.probe._collect_sample.side_effect = [20., 30.]

        def warm_up(deadline):
            self.assertTrue(self.dispatch.mutex.locked)
            self.probe.last_measurement = (30., 20., 30.)

        self.reactor.pause.side_effect = warm_up
        self.run_calibration(cmd)
        self.assertEqual(self.tap.call_count, 2)
        self.assertTrue(all(call.args == (cmd,)
                            for call in self.tap.call_args_list))
        self.probe.cal_helper.finish_calibration.assert_called_once_with(True)
        self.assert_clean()

    def test_abort_during_temperature_wait(self):
        self.reactor.pause.side_effect = lambda deadline: (
            self.dispatch.run_script('ABORT'))
        with self.assertRaisesRegex(gcode.CommandError, 'aborted'):
            self.run_calibration(self.command(WAIT='1'))
        self.dispatch._process_commands.assert_not_called()
        self.probe.cal_helper.finish_calibration.assert_called_once_with(False)
        self.assert_clean()

    def test_abort_during_final_measurement_discards_results(self):
        def collect(result):
            self.dispatch.run_script('abort ; cancel calibration')
            return 30.

        self.probe._collect_sample.side_effect = collect
        with self.assertRaisesRegex(gcode.CommandError, 'aborted'):
            self.run_calibration(self.command(WAIT='1'))
        self.probe.cal_helper.finish_calibration.assert_called_once_with(False)
        self.assert_clean()

    def test_probe_error_cleans_up(self):
        self.tap.side_effect = gcode.CommandError('tap failed')
        with self.assertRaisesRegex(gcode.CommandError, 'tap failed'):
            self.run_calibration(self.command(WAIT='1'))
        self.assert_clean()

    def test_wait_requires_tap(self):
        cmd = self.command(WAIT='1')
        cmd._params['MANUAL_METHOD'] = 'manual'
        with self.assertRaisesRegex(gcode.CommandError, 'requires'):
            self.run_calibration(cmd)
        self.assertFalse(self.probe.in_calibration)

    def test_async_commands_do_not_reorder_scripts(self):
        callback = Mock()
        self.dispatch.register_async_command('ABORT', callback)
        self.assertFalse(self.dispatch.try_async_command('G28\nABORT'))
        self.assertFalse(self.dispatch.try_async_command('ABORT_EXTRA'))
        callback.assert_not_called()
        self.assertTrue(self.dispatch.try_async_command(' ; comment\nabort\n'))
        callback.assert_called_once()
        self.dispatch.register_async_command('ABORT', None)
        self.assertFalse(self.dispatch.try_async_command('ABORT'))

    def test_serial_abort_while_processing(self):
        io = gcode.GCodeIO.__new__(gcode.GCodeIO)
        io.gcode = self.dispatch
        io.fd = 0
        io.input_log = collections.deque()
        io.bytes_read = 0
        io.partial_input = ''
        io.pending_commands = []
        io.is_fileinput = False
        io.is_processing_data = True
        self.dispatch.respond_raw = Mock()
        callback = Mock()
        self.dispatch.register_async_command('ABORT', callback)
        with patch.object(gcode.os, 'read', return_value=b'ABORT\n'):
            io._process_data(0.)
        callback.assert_called_once()
        self.dispatch.respond_raw.assert_called_once_with('ok')
        self.assertEqual(io.pending_commands, [])

    def test_abort_during_initial_heating(self):
        p = self.probe
        p.cal_extruder_temp = 150.
        p._set_extruder_temp = (
            temperature_probe.TemperatureProbe._set_extruder_temp
            .__get__(p))
        p._move_to_start.side_effect = lambda: p._set_extruder_temp(150., True)
        heater = self.toolhead.get_extruder.return_value.get_heater.return_value
        heater.get_temp.return_value = (20., 150.)
        self.reactor.pause.side_effect = lambda deadline: (
            self.dispatch.run_script('ABORT'))
        with self.assertRaisesRegex(gcode.CommandError, 'aborted'):
            self.run_calibration(self.command(WAIT='1'))
        self.tap.assert_not_called()
        self.assertFalse(p.in_calibration)
        self.assertEqual(self.dispatch.async_command_handlers, {})
        p.cal_helper.finish_calibration.assert_called_once_with(False)


if __name__ == '__main__':
    unittest.main()
