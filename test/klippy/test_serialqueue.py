import os
import sys
import tempfile
import unittest

KLIPPY_ROOT = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__)))), 'klippy')
if KLIPPY_ROOT not in sys.path:
    sys.path.insert(0, KLIPPY_ROOT)

import chelper  # noqa: E402

ffi, lib = chelper.get_ffi()


class BatchOutputTests(unittest.TestCase):
    def test_exit_writes_messages_scheduled_in_the_future(self):
        # Batch output (klippy -o) exits as soon as the last step is queued,
        # long before the clock reaches the queued messages' send times.
        freq = 1000000.
        payloads = [bytes([0x20 + i, 0x55, i]) for i in range(40)]
        with tempfile.TemporaryFile() as out:
            sq = lib.serialqueue_alloc(out.fileno(), b'f', 0,
                                       ffi.new('char[16]', b'batch_test'))
            lib.serialqueue_set_clock_est(sq, freq, lib.get_monotonic(), 0)
            cq = lib.serialqueue_alloc_commandqueue()
            for i, payload in enumerate(payloads):
                clock = int((i + 1) * 5. * freq)  # 5 to 200 seconds ahead
                lib.serialqueue_send(sq, cq, payload, len(payload),
                                     clock, clock, 0)
            lib.serialqueue_exit(sq)
            lib.serialqueue_free(sq)
            lib.serialqueue_free_commandqueue(cq)
            out.seek(0)
            data = out.read()
        for payload in payloads:
            self.assertIn(payload, data)


if __name__ == '__main__':
    unittest.main()
