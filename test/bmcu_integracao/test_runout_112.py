import io
import types
import unittest

from test_integracao import BASE, Gcmd, module, protocol
from test_print_recovery import PrintBench, RecoveryCommand


NATIVE = {'RefillError': RuntimeError}
exec(compile((BASE / 'refill_112_original.txt').read_text(),
             '<native_refill_112>', 'exec'), NATIVE)


class RunoutBench(PrintBench):
    def __init__(self):
        super().__init__()
        self.endpoint.driver = 'generic_single_extruder'
        self.obj.wipe = types.MethodType(module.EnderIntegration.wipe, self.obj)
        self.obj.purge_distance = 35.0
        self.obj.wipe_x = -3.0
        self.obj.wipe_passes = 6
        self.errors = []
        self.native_refill = NATIVE['AutoRefillController']()
        self.native_refill.manager = self.bmcu
        self.native_refill.manual_pending = dict(
            device='bmcu0', device_uid='TEST_BMCU', channel=3,
            endpoint='extruder', tool=4,
        )
        self.native_refill.last = None
        self.bmcu.refill = self.native_refill
        self.bmcu._device_uid = lambda device: 'TEST_BMCU'
        self.bmcu._print_state = lambda: self.print_state
        self.bmcu._generic_tail_detached_matches = lambda *args: True
        self.bmcu._manual_refill_resume_original = self.pause_control.cmd_RESUME
        self.bmcu._record_error = (
            lambda *args, **kwargs: self.errors.append(args))
        self.bmcu.load_channel = self.arrive_and_load
        self.sd.current_file = io.StringIO('TEST_RUNOUT\nG1 X101 E1 F600\n')
        self.sd.file_size = len(self.sd.current_file.getvalue())
        execute = self.execute

        def run(text):
            if text == 'TEST_RUNOUT':
                self.pause_control.cmd_PAUSE(RecoveryCommand())
                return
            execute(text)

        self.gcode.run_script = run
        self.start_print()

    def arrive_and_load(self, device, channel, **kwargs):
        self.assert_paused()
        self.present = True
        self.device.status['motion'] = [0] * 4
        self.endpoint._ender_load_context = (device.name, channel)
        self.bmcu.active_operations = {
            'bmcu0': {'phase': 'TOOLHEAD_PREPARATION'}}
        try:
            self.obj.load(Gcmd(ENDPOINT='extruder', REASON='refill'))
            self.device.status['route_state'][channel] = protocol.ROUTE_LOADED
            self.bmcu.loaded_tools['bmcu0:3'] = 4
        finally:
            self.bmcu.active_operations = {}

    def assert_paused(self):
        assert self.pause_control.is_paused
        assert self.sd.current_file is not None
        assert not self.sd.is_active()

    def confirm(self):
        native_resume = lambda gcmd: NATIVE[
            'BMCUManager'
        ]._cmd_manual_refill_resume_wrapper(self.bmcu, gcmd)
        return self.obj.resume_print(native_resume)(RecoveryCommand())


class Runout112Tests(unittest.TestCase):
    def test_confirmation_loads_and_wipes_before_native_print_resume(self):
        b = RunoutBench()
        saved = dict(b.move.saved_states['PAUSE_STATE'])
        original_map = dict(b.bmcu.print_tools)
        b.confirm()
        self.assertIsNone(b.native_refill.manual_pending)
        self.assertFalse(b.pause_control.is_paused)
        self.assertAlmostEqual(135.0, b.e)
        self.assertEqual([100.0, 110.0, 5.0], b.position[:3])
        self.assertEqual(saved, b.move.saved_states['PAUSE_STATE'])
        self.assertEqual(original_map, b.bmcu.print_tools)
        self.assertNotIn('G1 X101 E1 F600', b.commands)
        b.sd.work_handler(0.)
        self.assertIn('G1 X101 E1 F600', b.commands)
        self.assertEqual('complete', b.stats.state)

    def test_absent_replacement_stays_paused_with_same_file_and_map(self):
        b = RunoutBench()
        b.device.status['present'][3] = False
        file_before, pos_before = b.sd.current_file, b.sd.file_position
        with self.assertRaisesRegex(RuntimeError, 'insert replacement'):
            b.confirm()
        self.assertIs(file_before, b.sd.current_file)
        self.assertEqual(pos_before, b.sd.file_position)
        self.assertTrue(b.pause_control.is_paused)
        self.assertIsNotNone(b.native_refill.manual_pending)
        self.assertEqual(0.0, b.e)

    def test_assistance_loss_at_purge_entry_keeps_refill_paused(self):
        b = RunoutBench()
        file_before = b.sd.current_file
        map_before = dict(b.bmcu.print_tools)
        saved = dict(b.move.saved_states['PAUSE_STATE'])
        refresh = b.device.refresh
        boundary_reads = []

        def lose_assistance():
            refresh()
            if b.e == 100.0:
                boundary_reads.append(b.e)
                # First read is the final nozzle-feed check; the second is
                # the first purge readback, before any purge command.
                if len(boundary_reads) == 2:
                    b.device.status['motion'][3] = protocol.MOTION_IDLE

        b.device.refresh = lose_assistance
        with self.assertRaisesRegex(RuntimeError, 'assistance stopped'):
            b.confirm()
        self.assertEqual([100.0, 100.0], boundary_reads)
        self.assertEqual(100.0, b.e)
        self.assertEqual(1, b.actuators.count(protocol.MOTION_ON_USE))
        self.assertEqual('parar', b.actuators[-1])
        self.assertNotIn('G4 P1000', b.commands)
        self.assertFalse(any(speed == 10.0 for _, _, speed in b.head_moves))
        self.assertIs(file_before, b.sd.current_file)
        self.assertEqual(map_before, b.bmcu.print_tools)
        self.assertEqual(saved, b.move.saved_states['PAUSE_STATE'])
        self.assertTrue(b.pause_control.is_paused)
        self.assertIsNotNone(b.native_refill.manual_pending)
        self.assertNotIn('G1 X101 E1 F600', b.commands)
        self.assertTrue(b.errors)

    def test_failed_purge_or_cold_nozzle_never_resumes_or_loses_request(self):
        for failure in ('cold', 'purge'):
            with self.subTest(failure=failure):
                b = RunoutBench()
                file_before = b.sd.current_file
                if failure == 'cold':
                    b.temperature = 25.0
                else:
                    execute = b.gcode.run_script_from_command

                    def interrupted(text):
                        execute(text)
                        if text.startswith('G1 E') and b.e >= 135.0:
                            b.present = False

                    b.gcode.run_script_from_command = interrupted
                with self.assertRaisesRegex(RuntimeError, 'not ready'):
                    b.confirm()
                self.assertIs(file_before, b.sd.current_file)
                self.assertTrue(b.pause_control.is_paused)
                self.assertIsNotNone(b.native_refill.manual_pending)
                self.assertNotIn('G1 X101 E1 F600', b.commands)
                self.assertEqual('parar', b.actuators[-1])


if __name__ == '__main__':
    unittest.main()
