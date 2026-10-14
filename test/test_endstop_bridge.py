"""Host-side ownership, wire checks and native trigger error propagation."""
# Copyright (C) 2026 Xiaoyue Cui <2508041672@qq.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pathlib
import sys
import unittest
from types import SimpleNamespace
from unittest.mock import Mock, call, patch

sys.path.insert(0, str(pathlib.Path(__file__).parents[1] / 'klippy'))
from extras.endstop_bridge import (EndstopBridge, HardwareAnalogTrigger,
                                     LocalEndstop, EndstopBridgeInput)
from extras.trigger_analog import MCU_trigger_analog
import mcu
import pins


class ConfigTests(unittest.TestCase):
    def make_config(self, **overrides):
        options = dict(output_pin='!sender:PA15', receive_pin='^!PC7',
                       input_x_pin='^sender:PB0', input_y_pin='^sender:PB1',
                       input_z_pin='^sender:PB2', input_aux_pin='^sender:PB3')
        options.update(overrides)
        ppins = pins.PrinterPins()
        sender, receiver = Mock(), Mock()
        sender.create_oid.side_effect = iter(range(100))
        sender.seconds_to_clock.side_effect = lambda value: int(value * 1000000)
        ppins.register_chip('sender', sender)
        ppins.register_chip('mcu', receiver)
        printer = Mock()
        objects = {'pins': ppins, 'gcode': Mock()}
        printer.lookup_object.side_effect = objects.__getitem__
        config = Mock()
        config.error = ValueError
        config.get_printer.return_value = printer
        config.get_name.return_value = 'endstop_bridge sync'
        config.get.side_effect = options.__getitem__
        config.get_prefix_options.side_effect = lambda prefix: [
            name for name in options if name.startswith(prefix)]
        get_option = lambda name, default, **kw: options.get(name, default)
        config.getfloat.side_effect = get_option
        config.getint.side_effect = get_option
        return config, ppins, sender

    def build(self, **options):
        config, ppins, sender = self.make_config(**options)
        with patch.object(EndstopBridge, 'new_receiver', return_value=Mock()):
            bridge = EndstopBridge(config)
        return bridge, ppins, sender

    def test_four_inputs_and_defaults(self):
        bridge, ppins, sender = self.build()
        self.assertEqual(set(bridge.inputs), {'x', 'y', 'z', 'aux'})
        self.assertEqual(sender.create_oid.call_count, 5)
        for name, route in bridge.inputs.items():
            self.assertEqual(route.filter_count, 2)
            self.assertEqual(route.period, .000050)
            self.assertEqual(route.name, 'sync:' + name)
            self.assertIs(ppins.setup_pin('endstop', 'bridge:sync:' + name),
                          route)
            route._build_config()
        self.assertEqual(sender.add_config_cmd.call_count, 4)

    def test_advanced_settings_apply_to_inputs(self):
        bridge, unused, sender = self.build(period=.000100, filter_count=3)
        for route in bridge.inputs.values():
            self.assertEqual((route.period, route.filter_count), (.000100, 3))

    def test_unknown_input_and_modifiers_rejected(self):
        for desc in ('bridge:sync:missing', '!bridge:sync:x', '^bridge:sync:x'):
            bridge, ppins, unused = self.build()
            with self.assertRaises(ValueError):
                ppins.setup_pin('endstop', desc)

    def test_old_chip_not_registered(self):
        bridge, ppins, unused = self.build()
        with self.assertRaises(pins.error):
            ppins.setup_pin('endstop', 'endstop_bridge_sync:x')

    def test_input_on_wrong_mcu_rejected(self):
        with self.assertRaisesRegex(ValueError, 'share an MCU'):
            self.build(input_aux_pin='PC8')

    def test_empty_or_misspelled_input_rejected(self):
        for option in ('input__pin', 'input_aux_pni'):
            with self.assertRaisesRegex(ValueError, 'Expected input_'):
                self.build(**{option: 'sender:PB4'})

    def test_duplicate_source_pin_rejected(self):
        with self.assertRaises(pins.error):
            self.build(input_aux_pin='^sender:PB0')

    def test_physical_pin_syntax_unchanged(self):
        config, ppins, unused = self.make_config()
        self.assertEqual(ppins.parse_pin('^!sender:PB0', True, True)['pin'],
                         'PB0')
        self.assertEqual(ppins.parse_pin('PC7')['chip_name'], 'mcu')
        with self.assertRaises(pins.error):
            ppins.parse_pin('sender:bogus:PB0')

    def test_multiple_bridge_namespaces(self):
        bridge, ppins, unused = self.build()
        other = Mock()
        ppins.register_chip('bridge:other', other)
        self.assertIs(ppins.parse_pin('bridge:sync:x')['chip'], bridge)
        self.assertIs(ppins.parse_pin('bridge:other:x')['chip'], other)


class BridgeTests(unittest.TestCase):
    def test_public_chip_and_command_names(self):
        config = Mock()
        config.get_name.return_value = 'endstop_bridge sync'
        config.getfloat.return_value = .000050
        config.get_prefix_options.return_value = []
        pins, gcode, sender = Mock(), Mock(), Mock()
        config.get_printer.return_value.lookup_object.side_effect = {
            'pins': pins, 'gcode': gcode}.__getitem__
        pins.lookup_pin.side_effect = [
            {'chip': sender, 'pin': 'PA15', 'invert': 1},
            {'chip': Mock(), 'pin': 'PC7', 'invert': 1, 'pullup': 1}]
        bridge = EndstopBridge(config)
        pins.register_chip.assert_called_once_with('bridge:sync', bridge)
        gcode.register_mux_command.assert_called_once_with(
            'TEST_ENDSTOP_BRIDGE', 'BRIDGE', 'sync',
            bridge.cmd_TEST_ENDSTOP_BRIDGE)

    def setUp(self):
        self.bridge = bridge = EndstopBridge.__new__(EndstopBridge)
        bridge.owner = None
        bridge.name = 'sync'
        bridge.oid = 3
        bridge.poll_interval = .000050
        bridge.printer = SimpleNamespace(command_error=ValueError,
                                      register_event_handler=Mock())
        bridge.mcu = Mock()
        bridge.mcu.is_fileoutput.return_value = False
        bridge.set_cmd = Mock()
        self.receiver = Mock()
        self.receiver.query_endstop.side_effect = [1, 0]

    def test_wire_test_and_exclusive_owner(self):
        owner = object()
        self.bridge.prepare(owner, self.receiver)
        self.assertEqual(self.bridge.set_cmd.send.call_args_list,
                         [call([3, 2, 0, 1]), call([3, 2, 0, 0]),
                          call([3, 0, 0, 0])])
        with self.assertRaisesRegex(ValueError, 'already in use'):
            self.bridge.prepare(object(), self.receiver)
        self.bridge.release(object())
        self.assertIs(self.bridge.owner, owner)
        self.bridge.release(owner)
        self.assertIsNone(self.bridge.owner)

    def test_both_stuck_levels_rejected_and_released(self):
        for values in ([0, 0], [1, 1]):
            self.receiver.query_endstop.side_effect = values
            with self.assertRaisesRegex(ValueError, 'wire test failed'):
                self.bridge.prepare(object(), self.receiver)
            self.assertIsNone(self.bridge.owner)
            self.assertEqual(self.bridge.set_cmd.send.call_args.args[0],
                             [3, 0, 0, 0])

    def make_analog(self):
        self.bridge.new_receiver = Mock(return_value=self.receiver)
        config = Mock()
        config.get_printer.return_value = self.bridge.printer
        config.get_name.return_value = 'load_cell_probe'
        config.error.side_effect = ValueError
        source = Mock()
        source.get_mcu.return_value = self.bridge.mcu
        analog = HardwareAnalogTrigger(config, self.bridge, source)
        return analog, source

    def test_analog_steppers_attach_only_to_receiver(self):
        analog, source = self.make_analog()
        motor = object()
        analog.get_dispatch().add_stepper(motor)
        self.receiver.add_stepper.assert_called_once_with(motor)
        source.get_dispatch.assert_not_called()
        analog.prepare_homing()
        self.assertIs(analog.home_start(10., 0., 0, 0.),
                      self.receiver.home_start.return_value)
        self.receiver.home_start.assert_called_once_with(
            10., .000015, 4, .000050, triggered=True)
        self.receiver.home_wait.return_value = 10.01
        source.home_wait.return_value = 10.009
        self.assertEqual(analog.home_wait(11.), 10.01)
        self.assertIsNone(self.bridge.owner)

    def test_analog_errors_cannot_be_reported_as_contact(self):
        analog, source = self.make_analog()
        analog.prepare_homing()
        analog.home_start(10., 0., 0, 0.)
        self.receiver.home_wait.return_value = 10.01
        source.home_wait.side_effect = ValueError('RAW_RANGE')
        with self.assertRaisesRegex(ValueError, 'RAW_RANGE'):
            analog.home_wait(11.)
        source.abort_home.assert_called_once()
        self.assertIsNone(self.bridge.owner)

    def test_missing_wire_trigger_is_error(self):
        analog, source = self.make_analog()
        analog.prepare_homing()
        self.receiver.home_wait.return_value = 0.
        source.home_wait.return_value = 10.01
        with self.assertRaisesRegex(ValueError, 'disagree'):
            analog.home_wait(11.)
        self.assertIsNone(self.bridge.owner)

    def test_start_failure_aborts_both_ends(self):
        analog, source = self.make_analog()
        analog.prepare_homing()
        source.home_start.side_effect = ValueError('start failed')
        with self.assertRaisesRegex(ValueError, 'start failed'):
            analog.home_start(10., 0., 0, 0.)
        self.receiver.abort_home.assert_called_once()
        source.abort_home.assert_called_once()
        self.assertIsNone(self.bridge.owner)

    def test_arm_uses_sensor_command_queue(self):
        analog, source = self.make_analog()
        self.bridge.owner = analog
        dispatch = Mock()
        analog._arm_wire(dispatch)
        self.bridge.mcu.lookup_command.assert_called_once_with(
            'arm_endstop_bridge oid=%c trsync_oid=%c',
            cq=dispatch.get_command_queue())
        command = self.bridge.mcu.lookup_command.return_value
        command.send.assert_called_once_with([3, dispatch.get_oid()])

    def test_query_digital_does_not_switch_bus(self):
        route = EndstopBridgeInput.__new__(EndstopBridgeInput)
        route.bridge, route.oid, route.query_cmd = self.bridge, 7, Mock()
        route.query_cmd.send.return_value = {'value': 1}
        self.assertEqual(route.query_endstop(10.), 1)
        self.bridge.set_cmd.send.assert_not_called()

    def test_receiver_rejects_motor_on_other_mcu(self):
        receiver = LocalEndstop.__new__(LocalEndstop)
        receiver._mcu = Mock()
        receiver._mcu.get_printer.return_value.config_error = ValueError
        with self.assertRaisesRegex(ValueError, 'share an MCU'):
            receiver.add_stepper(Mock())

    def test_native_timeouts_unchanged(self):
        self.assertEqual(mcu.TRSYNC_TIMEOUT, .025)
        self.assertEqual(mcu.TRSYNC_SINGLE_MCU_TIMEOUT, .250)

    def test_native_analog_arms_wire_between_dispatch_and_sampling(self):
        source = MCU_trigger_analog.__new__(MCU_trigger_analog)
        events = []
        source._oid = 9
        source._mcu = Mock()
        source._sensor = Mock()
        source._sensor.get_samples_per_second.return_value = 640.
        source._reset_filter = Mock()
        source._dispatch = Mock()
        source._dispatch.start.side_effect = lambda t: events.append('start')
        source.setup_trigger_callback(lambda d: events.append('wire'))
        source._home_cmd = Mock()
        source._home_cmd.send.side_effect = (
            lambda *a, **kw: events.append('adc'))
        source.home_start(1., 0., 0, 0.)
        self.assertEqual(events, ['start', 'wire', 'adc'])
        source._clear_home = Mock()
        source.abort_home()
        source.abort_home()
        source._dispatch.stop.assert_called_once()
        source._clear_home.assert_called_once()

    def test_receiver_abort_is_idempotent(self):
        receiver = mcu.MCU_endstop.__new__(mcu.MCU_endstop)
        receiver._homing = True
        receiver._oid = 4
        receiver._home_cmd = Mock()
        receiver._dispatch = Mock()
        receiver.abort_home()
        receiver.abort_home()
        receiver._home_cmd.send.assert_called_once_with(
            [4, 0, 0, 0, 0, 0, 0, 0])
        receiver._dispatch.stop.assert_called_once()

    def test_homing_event_selects_only_its_own_source(self):
        route = EndstopBridgeInput.__new__(EndstopBridgeInput)
        route.bridge, route.oid, route.receiver = self.bridge, 7, self.receiver
        route.name = 'endstop_bridge_input x'
        move = Mock()
        move.get_mcu_endstops.return_value = [object()]
        route._homing_begin(move)
        self.bridge.set_cmd.send.assert_not_called()
        move.get_mcu_endstops.return_value = [route]
        route._homing_begin(move)
        self.assertIs(self.bridge.owner, route)
        self.bridge.set_cmd.send.assert_called_with([3, 1, 7, 0])
        self.bridge.release(route)


if __name__ == '__main__':
    unittest.main()
