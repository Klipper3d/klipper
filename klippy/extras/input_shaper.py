# Kinematic input shaper to minimize motion vibrations in XY plane
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections
import chelper
from . import shaper_defs

class InputShaperParams:
    def __init__(self, axis, config):
        self.axis = axis
        self.shapers = {s.name : s.init_func for s in shaper_defs.INPUT_SHAPERS}
        shaper_type = config.get('shaper_type', 'mzv')
        self.shaper_type = config.get('shaper_type_' + axis, shaper_type)
        if self.shaper_type not in self.shapers:
            raise config.error(
                    'Unsupported shaper type: %s' % (self.shaper_type,))
        self.damping_ratio = config.getfloat('damping_ratio_' + axis,
                                             shaper_defs.DEFAULT_DAMPING_RATIO,
                                             minval=0., maxval=1.)
        self.shaper_freq = config.getfloat('shaper_freq_' + axis, 0., minval=0.)
    def update(self, gcmd):
        axis = self.axis.upper()
        self.damping_ratio = gcmd.get_float('DAMPING_RATIO_' + axis,
                                            self.damping_ratio,
                                            minval=0., maxval=1.)
        self.shaper_freq = gcmd.get_float('SHAPER_FREQ_' + axis,
                                          self.shaper_freq, minval=0.)
        shaper_type = gcmd.get('SHAPER_TYPE', None)
        if shaper_type is None:
            shaper_type = gcmd.get('SHAPER_TYPE_' + axis, self.shaper_type)
        if shaper_type.lower() not in self.shapers:
            raise gcmd.error('Unsupported shaper type: %s' % (shaper_type,))
        self.shaper_type = shaper_type.lower()
    def get_shaper(self):
        if not self.shaper_freq:
            A, T = shaper_defs.get_none_shaper()
        else:
            A, T = self.shapers[self.shaper_type](
                    self.shaper_freq, self.damping_ratio)
        return len(A), A, T
    def get_status(self):
        return collections.OrderedDict([
            ('shaper_type', self.shaper_type),
            ('shaper_freq', '%.3f' % (self.shaper_freq,)),
            ('damping_ratio', '%.6f' % (self.damping_ratio,))])

class AxisInputShaper:
    def __init__(self, axis, config):
        self.axis = axis
        self.params = InputShaperParams(axis, config)
        self.n, self.A, self.T = self.params.get_shaper()
        self.saved = None
    def get_name(self):
        return 'shaper_' + self.axis
    def get_shaper(self):
        return self.n, self.A, self.T
    def update(self, gcmd):
        self.params.update(gcmd)
        self.n, self.A, self.T = self.params.get_shaper()
    def set_shaper_kinematics(self, sk):
        ffi_main, ffi_lib = chelper.get_ffi()
        success = ffi_lib.input_shaper_set_shaper_params(
                sk, self.axis.encode(), self.n, self.A, self.T) == 0
        if not success:
            self.disable_shaping()
            ffi_lib.input_shaper_set_shaper_params(
                    sk, self.axis.encode(), self.n, self.A, self.T)
        return success
    def disable_shaping(self):
        if self.saved is None and self.n:
            self.saved = (self.n, self.A, self.T)
        A, T = shaper_defs.get_none_shaper()
        self.n, self.A, self.T = len(A), A, T
    def enable_shaping(self):
        if self.saved is None:
            # Input shaper was not disabled
            return
        self.n, self.A, self.T = self.saved
        self.saved = None
    def report(self, gcmd):
        info = ' '.join(["%s_%s:%s" % (key, self.axis, value)
                         for (key, value) in self.params.get_status().items()])
        gcmd.respond_info(info)

class InputShaper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect", self.connect)
        self.toolhead = None
        self.shapers = [AxisInputShaper('x', config),
                        AxisInputShaper('y', config)]
        self.input_shaper_stepper_kinematics = []
        self.orig_stepper_kinematics = []
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SET_INPUT_SHAPER",
                               self.cmd_SET_INPUT_SHAPER,
                               desc=self.cmd_SET_INPUT_SHAPER_help)
    def get_shapers(self):
        return self.shapers
    def connect(self):
        self.toolhead = self.printer.lookup_object("toolhead")
        # Configure initial values
        self._update_input_shaping(error=self.printer.config_error)
    def _get_input_shaper_stepper_kinematics(self, stepper):
        # Lookup stepper kinematics
        sk = stepper.get_stepper_kinematics()
        if sk in self.orig_stepper_kinematics:
            # Already processed this stepper kinematics unsuccessfully
            return None
        if sk in self.input_shaper_stepper_kinematics:
            return sk
        self.orig_stepper_kinematics.append(sk)
        ffi_main, ffi_lib = chelper.get_ffi()
        is_sk = ffi_main.gc(ffi_lib.input_shaper_alloc(), ffi_lib.free)
        stepper.set_stepper_kinematics(is_sk)
        res = ffi_lib.input_shaper_set_sk(is_sk, sk)
        if res < 0:
            stepper.set_stepper_kinematics(sk)
            return None
        self.input_shaper_stepper_kinematics.append(is_sk)
        return is_sk
    def _update_input_shaping(self, error=None):
        self.toolhead.flush_step_generation()
        ffi_main, ffi_lib = chelper.get_ffi()
        kin = self.toolhead.get_kinematics()
        failed_shapers = []
        for s in kin.get_steppers():
            if s.get_trapq() is None:
                continue
            is_sk = self._get_input_shaper_stepper_kinematics(s)
            if is_sk is None:
                continue
            old_delay = ffi_lib.input_shaper_get_step_generation_window(is_sk)
            for shaper in self.shapers:
                if shaper in failed_shapers:
                    continue
                if not shaper.set_shaper_kinematics(is_sk):
                    failed_shapers.append(shaper)
            new_delay = ffi_lib.input_shaper_get_step_generation_window(is_sk)
            if old_delay != new_delay:
                self.toolhead.note_step_generation_scan_time(new_delay,
                                                             old_delay)
        if failed_shapers:
            error = error or self.printer.command_error
            raise error("Failed to configure shaper(s) %s with given parameters"
                        % (', '.join([s.get_name() for s in failed_shapers])))
    def disable_shaping(self):
        for shaper in self.shapers:
            shaper.disable_shaping()
        self._update_input_shaping()
    def enable_shaping(self):
        for shaper in self.shapers:
            shaper.enable_shaping()
        self._update_input_shaping()
    cmd_SET_INPUT_SHAPER_help = "Set cartesian parameters for input shaper"
    def cmd_SET_INPUT_SHAPER(self, gcmd):
        if gcmd.get_command_parameters():
            for shaper in self.shapers:
                shaper.update(gcmd)
            self._update_input_shaping()
        for shaper in self.shapers:
            shaper.report(gcmd)

def load_config(config):
    return InputShaper(config)
