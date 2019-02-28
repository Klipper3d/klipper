# Run a repeatability test using the z-probe
#
# Copyright (C) 2018  Maks Zolin <mzolin@vorondesign.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import probe
from math import sqrt

class ProbeTest:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.config = config
        self.probe_helper = None
        self.toolhead = None
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'PROBE_TEST', self.cmd_PROBE_TEST, desc=self.cmd_PROBE_TEST_help)
    cmd_PROBE_TEST_help = "Run a repeatability test using the z-probe"
    def cmd_PROBE_TEST(self, params):
        self.toolhead = self.printer.lookup_object('toolhead')
        samples = self.gcode.get_int('SAMPLES', params, 5)
        points = self._generate_points(samples,self.toolhead.get_position())
        self.probe_helper = probe.ProbePointsHelper(
            self.config, self.probe_finalize, points)
        self.probe_helper.start_probe(params)
    def _generate_points(self,samples,pos):
        points = []
        for i in range(samples):
            points.append([pos[0],pos[1]])
        return points
    def probe_finalize(self, offsets, positions):
        z_samples = [p[2] for p in positions]
        # Doing the math the hard way, because statistics module is not baked into python2.7
        num_samples = len(z_samples)
        mean = sum(z_samples) / num_samples
        differences = [x - mean for x in z_samples]
        sq_differences = [d ** 2 for d in differences]
        ssd = sum(sq_differences)
        # Sample standard deviation
        variance = ssd / (num_samples - 1)
        sd = sqrt(variance)
        msg = "Probe Test Results:\nMax:    %.6f\nMin:    %.6f\nMean:   %.6f\nStdDev: %.6f" % (
            max(z_samples),min(z_samples),mean,sd)
        self.gcode.respond_info(msg)

def load_config(config):
    return ProbeTest(config)
