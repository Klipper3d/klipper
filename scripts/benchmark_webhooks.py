#!/usr/bin/env python3
# Regression tests and performance benchmarks for klippy/webhooks.py
#
# Copyright (C) 2026  Hunter Laux <hunterlaux@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, time, unittest, statistics

KLIPPER_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, os.path.join(KLIPPER_ROOT, 'klippy'))

import webhooks

class DummyReactor:
    NEVER = 9999999999.0
    def __init__(self):
        self._time = 1000.0
    def monotonic(self):
        return self._time
    def assert_no_pause(self):
        class NoPause:
            def __enter__(self): return self
            def __exit__(self, *args): pass
        return NoPause()
    def unregister_timer(self, timer):
        pass

class DummyPrinter:
    def __init__(self, objects):
        self.objects = objects
        self.reactor = DummyReactor()
    def get_reactor(self):
        return self.reactor
    def lookup_object(self, name, default=None):
        return self.objects.get(name, default)
    def lookup_objects(self):
        return list(self.objects.items())

class DummyClient:
    def __init__(self):
        self.closed = False
        self.messages = []
    def is_closed(self):
        return self.closed
    def send(self, msg):
        self.messages.append(msg)

class DummyPrinterObject:
    def __init__(self, status_dict, dynamic=False):
        self.status = dict(status_dict)
        self.dynamic = dynamic
        self.counter = 0
    def get_status(self, eventtime):
        if self.dynamic:
            self.counter += 1
            d = dict(self.status)
            d['counter'] = self.counter
            return d
        return self.status

class TestQueryStatusHelper(unittest.TestCase):
    def setUp(self):
        self.printer = DummyPrinter({})
        class DummyWebhooks:
            def register_endpoint(self, path, handler): pass
        self.printer.objects['webhooks'] = DummyWebhooks()
        self.qsh = webhooks.QueryStatusHelper(self.printer)

    def test_single_client_unchanged_object_skipped(self):
        static_status = {'var': 123, 'text': 'hello'}
        obj = DummyPrinterObject(static_status)
        self.printer.objects['dummy_obj'] = obj

        client = DummyClient()
        sub = {'dummy_obj': None}
        self.qsh.clients[client] = (client, sub, client.send, {})

        # Pass 1: Initial query (detects initial state)
        self.qsh._do_query(1000.0)
        self.assertEqual(len(client.messages), 1)
        self.assertEqual(client.messages[0]['params']['status'],
                         {'dummy_obj': {'var': 123, 'text': 'hello'}})

        # Pass 2: Object returns same dict -> identity skip
        self.qsh._do_query(1000.25)
        self.assertEqual(len(client.messages), 1,
                         "Unchanged object should not send update")

        # Pass 3: Object returns new dict with modified field
        obj.status = {'var': 124, 'text': 'hello'}
        self.qsh._do_query(1000.5)
        self.assertEqual(len(client.messages), 2)
        self.assertEqual(client.messages[1]['params']['status'],
                         {'dummy_obj': {'var': 124}})

    def test_multi_client_field_filtering(self):
        obj = DummyPrinterObject({'a': 1, 'b': 2, 'c': 3})
        self.printer.objects['test_obj'] = obj

        c1 = DummyClient()
        self.qsh.clients[c1] = (c1, {'test_obj': None}, c1.send, {})

        c2 = DummyClient()
        self.qsh.clients[c2] = (c2, {'test_obj': ['b']}, c2.send, {})

        self.qsh._do_query(1000.0)
        self.assertEqual(c1.messages[0]['params']['status'],
                         {'test_obj': {'a': 1, 'b': 2, 'c': 3}})
        self.assertEqual(c2.messages[0]['params']['status'],
                         {'test_obj': {'b': 2}})

        obj.status = {'a': 10, 'b': 2, 'c': 3}
        self.qsh._do_query(1000.25)
        self.assertEqual(len(c1.messages), 2)
        self.assertEqual(c1.messages[1]['params']['status'],
                         {'test_obj': {'a': 10}})
        self.assertEqual(len(c2.messages), 1)

        obj.status = {'a': 10, 'b': 20, 'c': 3}
        self.qsh._do_query(1000.5)
        self.assertEqual(c1.messages[2]['params']['status'],
                         {'test_obj': {'b': 20}})
        self.assertEqual(c2.messages[1]['params']['status'],
                         {'test_obj': {'b': 20}})

    def test_closed_client_cleanup(self):
        obj = DummyPrinterObject({'val': 1})
        self.printer.objects['test_obj'] = obj

        client = DummyClient()
        sub = {'test_obj': None}
        self.qsh.clients[client] = (client, sub, client.send, {})

        self.qsh._do_query(1000.0)
        self.assertIn(client, self.qsh.clients)

        client.closed = True
        self.qsh._do_query(1000.25)
        self.assertNotIn(client, self.qsh.clients)

    def test_one_off_query(self):
        obj = DummyPrinterObject({'val': 42})
        self.printer.objects['query_obj'] = obj

        c = DummyClient()
        self.qsh.pending_queries.append(
            (None, {'query_obj': None}, c.send, {}))

        self.qsh._do_query(1000.0)
        self.assertEqual(len(c.messages), 1)
        self.assertEqual(c.messages[0]['params']['status'],
                         {'query_obj': {'val': 42}})

def run_benchmark(passes=200):
    printer = DummyPrinter({})
    class DummyWebhooks:
        def register_endpoint(self, path, handler): pass
    printer.objects['webhooks'] = DummyWebhooks()

    def make_benchmark_objects(total, dynamic):
        objs = {}
        for i in range(total):
            name = f"obj_{i:04d}"
            fields = {f"field_{k}": k * 10 for k in range(4)}
            is_dyn = (i < dynamic)
            objs[name] = DummyPrinterObject(fields, dynamic=is_dyn)
        return objs

    print("=" * 68)
    print("QueryStatusHelper Benchmark Results")
    print(f"Timing median of {passes} passes (times in milliseconds)")
    print("=" * 68)

    # 1. Object Scaling (5 dynamic objects, 1 client)
    print(f"\n1. Object Count Scaling (5 dynamic objects, 1 client):")
    print(f"{'Total Objects':<16} {'Static Objs':<16} {'Time (ms)':<16}")
    for count in [25, 50, 100, 200, 300, 500]:
        objs = make_benchmark_objects(count, 5)
        printer.objects = {'webhooks': DummyWebhooks(), **objs}
        qsh = webhooks.QueryStatusHelper(printer)
        client = DummyClient()
        sub = {k: None for k in objs}
        qsh.clients[client] = (client, sub, client.send, {})
        for i in range(5):
            qsh._do_query(1000.0 + i * 0.25)
        times = []
        t = 1005.0
        for _ in range(passes):
            t0 = time.perf_counter()
            qsh._do_query(t)
            times.append(time.perf_counter() - t0)
            t += 0.25
        med = statistics.median(times) * 1e3
        print(f"{count:<16} {count-5:<16} {med:<16.3f}")

    # 2. Client Scaling (250 total objects, 8 dynamic)
    print(f"\n2. Client Count Scaling (250 objects, 8 dynamic):")
    print(f"{'Clients':<16} {'Time (ms)':<16}")
    for n_clients in [1, 2, 3, 5]:
        objs = make_benchmark_objects(250, 8)
        printer.objects = {'webhooks': DummyWebhooks(), **objs}
        qsh = webhooks.QueryStatusHelper(printer)
        sub = {k: None for k in objs}
        for _ in range(n_clients):
            client = DummyClient()
            qsh.clients[client] = (client, sub, client.send, {})
        for i in range(5):
            qsh._do_query(1000.0 + i * 0.25)
        times = []
        t = 1005.0
        for _ in range(passes):
            t0 = time.perf_counter()
            qsh._do_query(t)
            times.append(time.perf_counter() - t0)
            t += 0.25
        med = statistics.median(times) * 1e3
        print(f"{n_clients:<16} {med:<16.3f}")

if __name__ == '__main__':
    if '--bench' in sys.argv:
        sys.argv.remove('--bench')
        run_benchmark()
    else:
        unittest.main()
