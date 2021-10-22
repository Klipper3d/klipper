# Kinematic input shaper to minimize motion vibrations in XY plane
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import chelper
from . import shaper_defs

class InputShaper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect", self.connect)
        self.toolhead = None
        self.shapers = {s.name : s.init_func for s in shaper_defs.INPUT_SHAPERS}
        self.damping_ratio_x = config.getfloat(
                'damping_ratio_x', shaper_defs.DEFAULT_DAMPING_RATIO,
                minval=0., maxval=1.)
        self.damping_ratio_y = config.getfloat(
                'damping_ratio_y', shaper_defs.DEFAULT_DAMPING_RATIO,
                minval=0., maxval=1.)
        self.shaper_freq_x = config.getfloat('shaper_freq_x', 0., minval=0.)
        self.shaper_freq_y = config.getfloat('shaper_freq_y', 0., minval=0.)
        self.shaper_type_x = config.get('shaper_type_x', 'mzv').lower()
        if self.shaper_type_x not in self.shapers:
            raise config.error(
                    'Unsupported shaper type: %s' % (self.shaper_type_x,))
        self.shaper_type_y = config.get('shaper_type_y', 'mzv').lower()
        if self.shaper_type_y not in self.shapers:
            raise config.error(
                    'Unsupported shaper type: %s' % (self.shaper_type_y,))
        self.saved_shaper_freq_x = self.saved_shaper_freq_y = 0.
        self.stepper_kinematics = []
        self.orig_stepper_kinematics = []
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SET_INPUT_SHAPER",
                               self.cmd_SET_INPUT_SHAPER,
                               desc=self.cmd_SET_INPUT_SHAPER_help)
    def connect(self):
        self.toolhead = self.printer.lookup_object("toolhead")
        kin = self.toolhead.get_kinematics()
        # Lookup stepper kinematics
        ffi_main, ffi_lib = chelper.get_ffi()
        steppers = kin.get_steppers()
        for s in steppers:
            sk = ffi_main.gc(ffi_lib.input_shaper_alloc(), ffi_lib.free)
            orig_sk = s.set_stepper_kinematics(sk)
            res = ffi_lib.input_shaper_set_sk(sk, orig_sk)
            if res < 0:
                s.set_stepper_kinematics(orig_sk)
                continue
            self.stepper_kinematics.append(sk)
            self.orig_stepper_kinematics.append(orig_sk)
        # Configure initial values
        self.old_delay = 0.
        self._set_input_shaper(self.shaper_type_x, self.shaper_type_y,
                               self.shaper_freq_x, self.shaper_freq_y,
                               self.damping_ratio_x, self.damping_ratio_y)
    def _get_shaper(self, shaper_type, shaper_freq, damping_ratio):
        if not shaper_freq:
            return shaper_defs.get_none_shaper()
        A, T = self.shapers[shaper_type](shaper_freq, damping_ratio)
        return len(A), A, T
    def _set_input_shaper(self, shaper_type_x, shaper_type_y
                          , shaper_freq_x, shaper_freq_y
                          , damping_ratio_x, damping_ratio_y):
        if (shaper_type_x != self.shaper_type_x
                or shaper_type_y != self.shaper_type_y):
            self.toolhead.flush_step_generation()
        n_x, A_x, T_x = self._get_shaper(
                shaper_type_x, shaper_freq_x, damping_ratio_x)
        n_y, A_y, T_y = self._get_shaper(
                shaper_type_y, shaper_freq_y, damping_ratio_y)
        ffi_main, ffi_lib = chelper.get_ffi()
        new_delay = max(
                ffi_lib.input_shaper_get_step_generation_window(n_x, A_x, T_x),
                ffi_lib.input_shaper_get_step_generation_window(n_y, A_y, T_y))
        self.toolhead.note_step_generation_scan_time(new_delay,
                                                     old_delay=self.old_delay)
        self.old_delay = new_delay
        self.shaper_type_x = shaper_type_x
        self.shaper_type_y = shaper_type_y
        self.shaper_freq_x = shaper_freq_x
        self.shaper_freq_y = shaper_freq_y
        self.damping_ratio_x = damping_ratio_x
        self.damping_ratio_y = damping_ratio_y
        for sk in self.stepper_kinematics:
            ffi_lib.input_shaper_set_shaper_params(
                    sk, len(A_x), A_x, T_x, len(A_y), A_y, T_y)
    def disable_shaping(self):
        if (self.saved_shaper_freq_x or self.saved_shaper_freq_y) and not (
                self.shaper_freq_x or self.shaper_freq_y):
            # Input shaper is already disabled
            return
        self.saved_shaper_freq_x = self.shaper_freq_x
        self.saved_shaper_freq_y = self.shaper_freq_y
        self._set_input_shaper(self.shaper_type_x, self.shaper_type_y, 0., 0.,
                               self.damping_ratio_x, self.damping_ratio_y)
    def enable_shaping(self):
        saved = self.saved_shaper_freq_x or self.saved_shaper_freq_y
        if saved:
            self._set_input_shaper(self.shaper_type_x, self.shaper_type_y,
                                   self.saved_shaper_freq_x,
                                   self.saved_shaper_freq_y,
                                   self.damping_ratio_x, self.damping_ratio_y)
        self.saved_shaper_freq_x = self.saved_shaper_freq_y = 0.
        return saved
    cmd_SET_INPUT_SHAPER_help = "Set cartesian parameters for input shaper"
    def cmd_SET_INPUT_SHAPER(self, gcmd):
        damping_ratio_x = gcmd.get_float(
                'DAMPING_RATIO_X', self.damping_ratio_x, minval=0., maxval=1.)
        damping_ratio_y = gcmd.get_float(
                'DAMPING_RATIO_Y', self.damping_ratio_y, minval=0., maxval=1.)
        shaper_freq_x = gcmd.get_float(
                'SHAPER_FREQ_X', self.shaper_freq_x, minval=0.)
        shaper_freq_y = gcmd.get_float(
                'SHAPER_FREQ_Y', self.shaper_freq_y, minval=0.)

        shaper_type = gcmd.get('SHAPER_TYPE', None)
        if shaper_type is None:
            shaper_type_x = gcmd.get(
                    'SHAPER_TYPE_X', self.shaper_type_x).lower()
            shaper_type_y = gcmd.get(
                    'SHAPER_TYPE_Y', self.shaper_type_y).lower()
        else:
            shaper_type_x = shaper_type_y = shaper_type.lower()
        if shaper_type_x not in self.shapers:
            raise gcmd.error('Unsupported shaper type: %s' % (shaper_type_x,))
        if shaper_type_y not in self.shapers:
            raise gcmd.error('Unsupported shaper type: %s' % (shaper_type_y,))

        self._set_input_shaper(shaper_type_x, shaper_type_y,
                               shaper_freq_x, shaper_freq_y,
                               damping_ratio_x, damping_ratio_y)

        gcmd.respond_info("shaper_type_x:%s shaper_type_y:%s "
                          "shaper_freq_x:%.3f shaper_freq_y:%.3f "
                          "damping_ratio_x:%.6f damping_ratio_y:%.6f"
                          % (self.shaper_type_x, self.shaper_type_y,
                             self.shaper_freq_x, self.shaper_freq_y,
                             self.damping_ratio_x, self.damping_ratio_y))

def load_config(config):
    return InputShaper(config)
