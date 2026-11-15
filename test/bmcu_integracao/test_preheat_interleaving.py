import contextlib
import unittest

from test_integracao import ROOT, import_module
from test_preheat_guard import HeatingBench


DISPLAY = import_module(
    'preheat_interleaving_display',
    ROOT / 'test/e3v3se_display/test_display.py',
)


class InterleavingBench:
    # Real display, dispatcher, G-code state and integration; only physical
    # motion/heaters and the reactor are simulated.
    def __init__(self):
        self.h = h = DISPLAY.FilamentHarness()
        integration = HeatingBench().obj
        integration.printer = h.printer
        integration.gcode = h.gcode
        integration.reactor = h.printer.reactor
        h.printer.objects['bmcu_ender_integration'] = integration
        self.integration = integration
        h.extruder.min_temp = 0.0
        h.position = [100.0, 110.0, 5.0, 0.0]
        self.moves = []
        self.heating = []
        self.waits = 0
        self.on_wait = lambda: None
        self.cancelled = False
        self.moves_after_cancel = []
        def move(position, speed):
            self.moves.append((list(position), speed))
            if self.cancelled:
                self.moves_after_cancel.append(list(position))
            h.position = list(position)
        h.move = move
        toolhead = h.printer.objects['toolhead']
        toolhead.get_status = lambda t: {
            'homed_axes': 'xyz', 'axis_minimum': [-8, 0, 0],
            'axis_maximum': [230, 230, 250],
        }
        def manual(coords, speed):
            position = list(h.position)
            for i, value in enumerate(coords):
                if value is not None:
                    position[i] = value
            move(position, speed)
            h.gmove.reset_last_position()
        toolhead.manual_move = manual
        def wait():
            self.waits += 1
            self.on_wait()
        toolhead.wait_moves = wait
        h.gmove.reset_last_position()
        def heat(heater, target):
            self.heating.append((target, list(h.position)))
            heater.target = target
        h.printer.objects['heaters'].set_temperature = heat

    def begin(self):
        return self.h.display._begin_operation('EXTRUDE', delta=1)


class PreheatInterleavingTests(unittest.TestCase):
    def test_async_generic_command_does_not_reacquire_its_existing_mutex(self):
        b = InterleavingBench()
        lock = b.h.gcode.get_mutex()
        class NoReentry:
            def __enter__(self):
                if not lock.acquire(False):
                    raise RuntimeError('G-code mutex acquired recursively')
            def __exit__(self, *args):
                lock.release()
            def locked(self):
                return lock.locked()
        b.h.gcode.mutex = NoReentry()
        held = []
        b.on_wait = lambda: held.append(b.h.gcode.get_mutex().locked())
        b.h.gcode.run_script('ENDER_FILAMENT ACTION=LOAD')
        self.assertTrue(held and all(held))
        self.assertEqual([-6.0, 123.0], b.heating[0][1][:2])

    def test_async_preparation_and_target_hold_real_gcode_mutex(self):
        b = InterleavingBench()
        locks = []
        def interleave():
            locked = b.h.gcode.get_mutex().locked()
            locks.append(locked)
            if b.waits == 5 and not locked:
                b.h.gcode.run_script('G1 X100 Y110 F1200')
        b.on_wait = interleave
        self.assertTrue(b.begin()[0])
        self.assertTrue(locks and all(locks))
        self.assertEqual([-6.0, 123.0], b.heating[0][1][:2])

    def test_final_position_is_revalidated_even_for_native_actor(self):
        b = InterleavingBench()
        def interleave():
            if b.waits == 5:
                # Simulate a native actor bypassing the public G-code mutex.
                b.h.gcode.run_script_from_command('G1 X100 Y110 F1200')
        b.on_wait = interleave
        self.assertFalse(b.begin()[0])
        self.assertFalse(b.heating)

    def test_print_start_after_z_prevents_subsequent_xy_segments(self):
        b = InterleavingBench()
        def start_print():
            if b.waits == 3:
                b.h.state = 'printing'
        b.on_wait = start_print
        self.assertFalse(b.begin()[0])
        self.assertEqual(1, len(b.moves))
        self.assertEqual([100.0, 110.0, 7.0], b.moves[0][0][:3])
        self.assertFalse(b.heating)

    def test_cancel_at_each_wait_keeps_claim_and_stops_next_segment(self):
        for cancel_at in range(1, 6):
            with self.subTest(wait=cancel_at):
                b = InterleavingBench()
                observations = []
                def cancel():
                    if b.waits != cancel_at or b.cancelled:
                        return
                    b.cancelled = True
                    b.h.display.cancel_operation()
                    observations.append(b.h.display.is_operation_busy())
                    observations.append(b.begin()[0])
                b.on_wait = cancel
                self.assertFalse(b.begin()[0])
                self.assertEqual([True, False], observations)
                self.assertFalse(b.moves_after_cancel)
                self.assertFalse(b.heating)
                self.assertIsNone(b.h.display.operation)

    def test_cancel_while_entering_critical_section_emits_no_move(self):
        b = InterleavingBench()
        @contextlib.contextmanager
        def critical(reason):
            b.cancelled = True
            b.h.display.cancel_operation()
            yield
        b.integration.bmcu.printer_critical_section = critical
        self.assertFalse(b.begin()[0])
        self.assertFalse(b.moves)
        self.assertFalse(b.heating)

    def test_manual_bmcu_session_cancel_after_z_stops_following_xy(self):
        b = InterleavingBench()
        b.integration.manual_operation = 'LOAD'
        b.h.display.bmcu_session = {'cancelled': False}
        def cancel():
            if b.waits == 3:
                b.cancelled = True
                b.h.display.cancel_operation()
        b.on_wait = cancel
        with self.assertRaisesRegex(
            RuntimeError, 'Filament operation cancelled',
        ):
            b.integration.heat_manual_filament()
        self.assertEqual(1, len(b.moves))
        self.assertFalse(b.moves_after_cancel)
        self.assertFalse(b.heating)

    def test_recovery_validations_remain_outside_motion_critical_section(self):
        b = InterleavingBench()
        b.h.state = 'paused'
        b.integration.recovering_tool_change = True
        critical = []
        validations = []
        @contextlib.contextmanager
        def section(reason):
            critical.append(True)
            try:
                yield
            finally:
                critical.pop()
        b.integration.bmcu.printer_critical_section = section
        def validate():
            self.assertFalse(critical, 'Recovery validation may access UART')
            validations.append(True)
        b.integration.validate_print_recovery = validate
        def heat(command):
            target = command.get_float('TARGET')
            b.h.extruder.target = b.h.extruder.temperature = target
        b.h.gcode.register_command('SET_HEATER_TEMPERATURE', heat)
        b.integration.heat_manual_filament()
        self.assertEqual(3, len(validations))
        self.assertEqual([-6.0, 123.0], b.h.position[:2])
        self.assertEqual(230, b.h.extruder.target)


if __name__ == '__main__':
    unittest.main()
