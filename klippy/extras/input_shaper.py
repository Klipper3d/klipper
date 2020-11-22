# Kinematic input shaper to minimize motion vibrations in XY plane
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import chelper

class InputShaper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect", self.connect)
        self.toolhead = None
        self.damping_ratio_x = config.getfloat(
                'damping_ratio_x', 0.1, minval=0., maxval=1.)
        self.damping_ratio_y = config.getfloat(
                'damping_ratio_y', 0.1, minval=0., maxval=1.)
        self.shaper_freq_x = config.getfloat('shaper_freq_x', 0., minval=0.)
        self.shaper_freq_y = config.getfloat('shaper_freq_y', 0., minval=0.)
        ffi_main, ffi_lib = chelper.get_ffi()
        self.shapers = {None: None
                , 'zv': ffi_lib.INPUT_SHAPER_ZV
                , 'zvd': ffi_lib.INPUT_SHAPER_ZVD
                , 'mzv': ffi_lib.INPUT_SHAPER_MZV
                , 'ei': ffi_lib.INPUT_SHAPER_EI
                , '2hump_ei': ffi_lib.INPUT_SHAPER_2HUMP_EI
                , '3hump_ei': ffi_lib.INPUT_SHAPER_3HUMP_EI}
        shaper_type = config.get('shaper_type', 'mzv')
        self.shaper_type_x = config.getchoice(
                'shaper_type_x', self.shapers, shaper_type)
        self.shaper_type_y = config.getchoice(
                'shaper_type_y', self.shapers, shaper_type)
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
    def _set_input_shaper(self, shaper_type_x, shaper_type_y
                          , shaper_freq_x, shaper_freq_y
                          , damping_ratio_x, damping_ratio_y):
        if (shaper_type_x != self.shaper_type_x
                or shaper_type_y != self.shaper_type_y):
            self.toolhead.flush_step_generation()
        ffi_main, ffi_lib = chelper.get_ffi()
        new_delay = max(
                ffi_lib.input_shaper_get_step_generation_window(
                    shaper_type_x, shaper_freq_x, damping_ratio_x),
                ffi_lib.input_shaper_get_step_generation_window(
                    shaper_type_y, shaper_freq_y, damping_ratio_y))
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
            ffi_lib.input_shaper_set_shaper_params(sk
                    , shaper_type_x, shaper_type_y
                    , shaper_freq_x, shaper_freq_y
                    , damping_ratio_x, damping_ratio_y)
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

        def parse_shaper(shaper_type_str):
            shaper_type_str = shaper_type_str.lower()
            if shaper_type_str not in self.shapers:
                raise gcmd.error(
                    "Requested shaper type '%s' is not supported" % (
                        shaper_type_str))
            return self.shapers[shaper_type_str]

        shaper_type = gcmd.get('SHAPER_TYPE', None, parser=parse_shaper)
        if shaper_type is None:
            shaper_type_x = gcmd.get('SHAPER_TYPE_X', self.shaper_type_x,
                                     parser=parse_shaper)
            shaper_type_y = gcmd.get('SHAPER_TYPE_Y', self.shaper_type_y,
                                     parser=parse_shaper)
        else:
            shaper_type_x = shaper_type_y = shaper_type

        self._set_input_shaper(shaper_type_x, shaper_type_y,
                               shaper_freq_x, shaper_freq_y,
                               damping_ratio_x, damping_ratio_y)

        gcmd.respond_info("shaper_type_x:%s shaper_type_y:%s "
                          "shaper_freq_x:%.3f shaper_freq_y:%.3f "
                          "damping_ratio_x:%.6f damping_ratio_y:%.6f" % (
                              self.shapers.keys()[
                                  self.shapers.values().index(shaper_type_x)]
                              , self.shapers.keys()[
                                  self.shapers.values().index(shaper_type_y)]
                              , shaper_freq_x, shaper_freq_y
                              , damping_ratio_x, damping_ratio_y))

def load_config(config):
    return InputShaper(config)
