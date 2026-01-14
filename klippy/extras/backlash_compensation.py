# A code for per-axis backlash compensation
#
# Copyright (C) 2026  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import collections, math, logging
import chelper

class AxisBacklashConfig:
    def __init__(self, axis, config):
        self.axis = axis
        self.backlash = config.getfloat(axis + '_backlash', 0.0, minval=0.0)
    def update(self, gcmd):
        axis = self.axis.upper()
        self.backlash = gcmd.get_float(axis + '_BACKLASH',
                                       self.backlash, minval=0.0)
    def is_enabled(self):
        return self.backlash > 0.
    def get_status(self):
        return collections.OrderedDict([
            ('backlash', self.backlash)])

class BacklashCompensation:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.input_shaper = self.printer.load_object(config, 'input_shaper')
        self.printer.register_event_handler("klippy:connect", self.connect)
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SET_BACKLASH_COMPENSATION",
                               self.cmd_SET_BACKLASH_COMPENSATION,
                               desc=self.cmd_SET_BACKLASH_COMPENSATION_help)
        self.backlash_configs = [AxisBacklashConfig('x', config),
                                 AxisBacklashConfig('y', config),
                                 AxisBacklashConfig('z', config)]
        self.smooth_time = config.getfloat('smooth_time', 0.005,
                                           above=0.0, maxval=0.1)
    def connect(self):
        dual_carriage = self.printer.lookup_object('dual_carriage', None)
        if dual_carriage is not None:
            for backlash_compensation in self.backlash_configs:
                if backlash_compensation.is_enabled():
                    raise self.printer.config_error(
                            'Backlash compensation parameters cannot be'
                            ' configured via [backlash_compensation] section'
                            ' with dual_carriage(s)  enabled. Refer to Klipper'
                            ' documentation on how to configure backlash '
                            ' compensation for dual_carriage(s).')
            return
        # Make sure input shaper is configured before configuring
        # backlash compensation parameters
        self.input_shaper.connect()
        # Configure initial values
        self._update_backlash_compensation()
    def _update_backlash_compensation(self):
        toolhead = self.printer.lookup_object("toolhead")
        toolhead.flush_step_generation()
        kin = toolhead.get_kinematics()
        axes_backlash = [bc.backlash for bc in self.backlash_configs]
        ffi_main, ffi_lib = chelper.get_ffi()
        for s in kin.get_steppers():
            if s.get_trapq() is None:
                continue
            is_sk = self.input_shaper.get_input_shaper_stepper_kinematics(s)
            if is_sk is None:
                continue
            ffi_lib.input_shaper_set_backlash_compensation(
                    is_sk, axes_backlash, self.smooth_time)
        motion_queuing = self.printer.lookup_object("motion_queuing")
        motion_queuing.check_step_generation_scan_windows()
    cmd_SET_BACKLASH_COMPENSATION_help = "Set backlash compensation parameters"
    def cmd_SET_BACKLASH_COMPENSATION(self, gcmd):
        if gcmd.get_command_parameters():
            self.smooth_time = gcmd.get_float('SMOOTH_TIME', self.smooth_time,
                                              minval=0.0, maxval=0.1)
            for bc in self.backlash_configs:
                bc.update(gcmd)
            self._update_backlash_compensation()
        infos = [' '.join(["%s_%s:%s" % (bc.axis, key, value)
                           for (key, value) in bc.get_status().items()])
                 for bc in self.backlash_configs]
        gcmd.respond_info(' '.join(infos)
                          + (' smooth_time:%.6f' % self.smooth_time))

def load_config(config):
    return BacklashCompensation(config)
