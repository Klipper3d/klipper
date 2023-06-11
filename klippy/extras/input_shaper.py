# Kinematic input shaper to minimize motion vibrations in XY plane
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2020-2023  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections
import chelper
from . import shaper_defs

def parse_float_list(list_str):
    def parse_str(s):
        res = []
        for line in s.split('\n'):
            for coeff in line.split(','):
                res.append(float(coeff.strip()))
        return res
    try:
        return parse_str(list_str)
    except:
        return None

class TypedInputShaperParams:
    shapers = {s.name : s.init_func for s in shaper_defs.INPUT_SHAPERS}
    def __init__(self, axis, shaper_type, config):
        self.axis = axis
        self.shaper_type = shaper_type
        self.damping_ratio = shaper_defs.DEFAULT_DAMPING_RATIO
        self.shaper_freq = 0.
        if config is not None:
            if shaper_type not in self.shapers:
                raise config.error(
                        'Unsupported shaper type: %s' % (shaper_type,))
            self.damping_ratio = config.getfloat('damping_ratio_' + axis,
                                                 self.damping_ratio,
                                                 minval=0., maxval=1.)
            self.shaper_freq = config.getfloat('shaper_freq_' + axis,
                                               self.shaper_freq, minval=0.)
    def get_type(self):
        return self.shaper_type
    def get_axis(self):
        return self.axis
    def update(self, shaper_type, gcmd):
        if shaper_type not in self.shapers:
            raise gcmd.error('Unsupported shaper type: %s' % (shaper_type,))
        axis = self.axis.upper()
        self.damping_ratio = gcmd.get_float('DAMPING_RATIO_' + axis,
                                            self.damping_ratio,
                                            minval=0., maxval=1.)
        self.shaper_freq = gcmd.get_float('SHAPER_FREQ_' + axis,
                                          self.shaper_freq, minval=0.)
        self.shaper_type = shaper_type
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

class CustomInputShaperParams:
    SHAPER_TYPE = 'custom'
    def __init__(self, axis, config):
        self.axis = axis
        self.n, self.A, self.T = 0, [], []
        if config is not None:
            shaper_a_str = config.get('shaper_a_' + axis)
            shaper_t_str = config.get('shaper_t_' + axis)
            self.n, self.A, self.T = self._parse_custom_shaper(
                    shaper_a_str, shaper_t_str, config.error)
    def get_type(self):
        return self.SHAPER_TYPE
    def get_axis(self):
        return self.axis
    def update(self, shaper_type, gcmd):
        if shaper_type != self.SHAPER_TYPE:
            raise gcmd.error('Unsupported shaper type: %s' % (shaper_type,))
        axis = self.axis.upper()
        shaper_a_str = gcmd.get('SHAPER_A_' + axis, None)
        shaper_t_str = gcmd.get('SHAPER_T_' + axis, None)
        if (shaper_a_str is None) != (shaper_t_str is None):
            raise gcmd.error("Both SHAPER_A_%s and SHAPER_T_%s parameters"
                             " must be provided" % (axis, axis))
        if shaper_a_str is not None:
            self.n, self.A, self.T = self._parse_custom_shaper(
                    shaper_a_str, shaper_t_str, gcmd.error)
    def _parse_custom_shaper(self, custom_a_str, custom_t_str, parse_error):
        A = parse_float_list(custom_a_str)
        if A is None:
            raise parse_error("Invalid shaper A string: '%s'" % (custom_a_str,))
        if min([abs(a) for a in A]) < 0.001:
            raise parse_error("All shaper A coefficients must be non-zero")
        if sum(A) < 0.001:
            raise parse_error(
                    "Shaper A parameter must sum up to a positive number")
        T = parse_float_list(custom_t_str)
        if T is None:
            raise parse_error("Invalid shaper T string: '%s'" % (custom_t_str,))
        if T != sorted(T):
            raise parse_error("Shaper T parameter is not ordered: %s" % (T,))
        if len(A) != len(T):
            raise parse_error(
                    "Shaper A and T parameters must have the same length:"
                    " %d vs %d" % (len(A), len(T),))
        dur = T[-1] - T[0]
        if len(T) > 1 and dur < 0.001:
            raise parse_error("Shaper duration is too small (%.6f sec)"
                              % (dur,))
        if dur > 0.2:
            raise parse_error("Shaper duration is too large (%.6f sec)"
                              % (dur,))
        return len(A), A, T
    def get_shaper(self):
        return self.n, self.A, self.T
    def get_status(self):
        return collections.OrderedDict([
            ('shaper_type', self.SHAPER_TYPE),
            ('shaper_a', ','.join(['%.6f' % (a,) for a in self.A])),
            ('shaper_t', ','.join(['%.6f' % (t,) for t in self.T]))])

class AxisInputShaper:
    def __init__(self, params):
        self.params = params
        self.n, self.A, self.T = params.get_shaper()
        self.t_offs = self._calc_offset()
        self.saved = None
    def get_name(self):
        return 'shaper_' + self.get_axis()
    def get_type(self):
        return self.params.get_type()
    def get_axis(self):
        return self.params.get_axis()
    def is_extruder_smoothing(self, exact_mode):
        return not exact_mode and self.A
    def is_enabled(self):
        return self.n > 0
    def _calc_offset(self):
        if not self.A:
            return 0.
        return sum([a * t for a, t in zip(self.A, self.T)]) / sum(self.A)
    def update(self, shaper_type, gcmd):
        self.params.update(shaper_type, gcmd)
        self.n, self.A, self.T = self.params.get_shaper()
        self.t_offs = self._calc_offset()
    def update_stepper_kinematics(self, sk):
        ffi_main, ffi_lib = chelper.get_ffi()
        axis = self.get_axis().encode()
        success = ffi_lib.input_shaper_set_shaper_params(
                sk, axis, self.n, self.A, self.T) == 0
        if not success:
            self.disable_shaping()
            ffi_lib.input_shaper_set_shaper_params(
                    sk, axis, self.n, self.A, self.T)
        return success
    def update_extruder_kinematics(self, sk, exact_mode):
        ffi_main, ffi_lib = chelper.get_ffi()
        axis = self.get_axis().encode()
        if not self.is_extruder_smoothing(exact_mode):
            # Make sure to disable any active input smoothing
            coeffs, smooth_time = [], 0.
            success = ffi_lib.extruder_set_smoothing_params(
                    sk, axis, len(coeffs), coeffs, smooth_time, 0.) == 0
            success = ffi_lib.extruder_set_shaper_params(
                    sk, axis, self.n, self.A, self.T) == 0
        else:
            shaper_type = self.get_type()
            extr_smoother_func = shaper_defs.EXTURDER_SMOOTHERS.get(
                    shaper_type, shaper_defs.EXTURDER_SMOOTHERS['default'])
            C_e, t_sm = extr_smoother_func(self.T[-1] - self.T[0],
                                           normalize_coeffs=False)
            smoother_offset = self.t_offs - 0.5 * (self.T[0] + self.T[-1])
            success = ffi_lib.extruder_set_smoothing_params(
                    sk, axis, len(C_e), C_e, t_sm, smoother_offset) == 0
        if not success:
            self.disable_shaping()
            ffi_lib.extruder_set_shaper_params(
                    sk, axis, self.n, self.A, self.T)
        return success
    def disable_shaping(self):
        was_enabled = False
        if self.saved is None and self.n:
            self.saved = (self.n, self.A, self.T)
            was_enabled = True
        A, T = shaper_defs.get_none_shaper()
        self.n, self.A, self.T = len(A), A, T
        return was_enabled
    def enable_shaping(self):
        if self.saved is None:
            # Input shaper was not disabled
            return False
        self.n, self.A, self.T = self.saved
        self.saved = None
        return True
    def report(self, gcmd):
        info = ' '.join(["%s_%s:%s" % (key, self.get_axis(), value)
                         for (key, value) in self.params.get_status().items()])
        gcmd.respond_info(info)

class TypedInputSmootherParams:
    smoothers = {s.name : s.init_func for s in shaper_defs.INPUT_SMOOTHERS}
    def __init__(self, axis, smoother_type, config):
        self.axis = axis
        self.smoother_type = smoother_type
        self.smoother_freq = 0.
        if config is not None:
            if smoother_type not in self.smoothers:
                raise config.error(
                        'Unsupported shaper type: %s' % (smoother_type,))
            self.smoother_freq = config.getfloat('smoother_freq_' + axis,
                                                 self.smoother_freq, minval=0.)
    def get_type(self):
        return self.smoother_type
    def get_axis(self):
        return self.axis
    def update(self, smoother_type, gcmd):
        if smoother_type not in self.smoothers:
            raise gcmd.error('Unsupported shaper type: %s' % (smoother_type,))
        axis = self.axis.upper()
        self.smoother_freq = gcmd.get_float('SMOOTHER_FREQ_' + axis,
                                            self.smoother_freq, minval=0.)
        self.smoother_type = smoother_type
    def get_smoother(self):
        if not self.smoother_freq:
            C, tsm = shaper_defs.get_none_smoother()
        else:
            C, tsm = self.smoothers[self.smoother_type](self.smoother_freq,
                                                        normalize_coeffs=False)
        return len(C), C, tsm
    def get_status(self):
        return collections.OrderedDict([
            ('shaper_type', self.smoother_type),
            ('smoother_freq', '%.3f' % (self.smoother_freq,))])

class CustomInputSmootherParams:
    SHAPER_TYPE = 'smoother'
    def __init__(self, axis, config):
        self.axis = axis
        self.coeffs, self.smooth_time = shaper_defs.get_none_smoother()
        if config is not None:
            self.smooth_time = config.getfloat('smooth_time_' + axis,
                                               self.smooth_time, minval=0.)
            self.coeffs = list(reversed(config.getfloatlist('coeffs_' + axis,
                                                            self.coeffs)))
    def get_type(self):
        return self.SHAPER_TYPE
    def get_axis(self):
        return self.axis
    def update(self, shaper_type, gcmd):
        if shaper_type != self.SHAPER_TYPE:
            raise gcmd.error('Unsupported shaper type: %s' % (shaper_type,))
        axis = self.axis.upper()
        self.smooth_time = gcmd.get_float('SMOOTH_TIME_' + axis,
                                          self.smooth_time)
        coeffs_str = gcmd.get('COEFFS_' + axis, None)
        if coeffs_str is not None:
            try:
                coeffs = parse_float_list(coeffs_str)
                coeffs.reverse()
            except:
                raise gcmd.error("Invalid format for COEFFS parameter")
            self.coeffs = coeffs
    def get_smoother(self):
        return len(self.coeffs), self.coeffs, self.smooth_time
    def get_status(self):
        return collections.OrderedDict([
            ('shaper_type', self.SHAPER_TYPE),
            ('shaper_coeffs', ','.join(['%.9e' % (a,)
                                        for a in reversed(self.coeffs)])),
            ('shaper_smooth_time', self.smooth_time)])

class AxisInputSmoother:
    def __init__(self, params):
        self.params = params
        self.n, self.coeffs, self.smooth_time = params.get_smoother()
        self.t_offs = self._calc_offset()
        self.saved_smooth_time = 0.
    def get_name(self):
        return 'smoother_' + self.axis
    def get_type(self):
        return self.params.get_type()
    def get_axis(self):
        return self.params.get_axis()
    def is_extruder_smoothing(self, exact_mode):
        return True
    def is_enabled(self):
        return self.smooth_time > 0.
    def _calc_offset(self):
        int_t0 = int_t1 = 0.
        for i, c in enumerate(self.coeffs):
            if i & 1:
                int_t1 += c / (2**(i+1) * (i+2))
            else:
                int_t0 += c / (2**i * (i+1))
        return int_t1 * self.smooth_time / int_t0
    def update(self, shaper_type, gcmd):
        self.params.update(shaper_type, gcmd)
        self.n, self.coeffs, self.smooth_time = self.params.get_smoother()
        self.t_offs = self._calc_offset()
    def update_stepper_kinematics(self, sk):
        ffi_main, ffi_lib = chelper.get_ffi()
        axis = self.get_axis().encode()
        success = ffi_lib.input_shaper_set_smoother_params(
                sk, axis, self.n, self.coeffs, self.smooth_time) == 0
        if not success:
            self.disable_shaping()
            ffi_lib.input_shaper_set_smoother_params(
                    sk, axis, self.n, self.coeffs, self.smooth_time)
        return success
    def update_extruder_kinematics(self, sk, exact_mode):
        ffi_main, ffi_lib = chelper.get_ffi()
        axis = self.get_axis().encode()
        # Make sure to disable any active input shaping
        A, T = shaper_defs.get_none_shaper()
        ffi_lib.extruder_set_shaper_params(sk, axis, len(A), A, T)
        if exact_mode:
            success = ffi_lib.extruder_set_smoothing_params(
                    sk, axis, self.n, self.coeffs, self.smooth_time,
                    self.t_offs) == 0
        else:
            smoother_type = self.get_type()
            extr_smoother_func = shaper_defs.EXTURDER_SMOOTHERS.get(
                    smoother_type, shaper_defs.EXTURDER_SMOOTHERS['default'])
            C_e, t_sm = extr_smoother_func(self.smooth_time,
                                           normalize_coeffs=False)
            success = ffi_lib.extruder_set_smoothing_params(
                    sk, axis, len(C_e), C_e, t_sm, self.t_offs) == 0
        if not success:
            self.disable_shaping()
            ffi_lib.extruder_set_smoothing_params(
                    sk, axis, self.n, self.coeffs, self.smooth_time)
        return success
    def disable_shaping(self):
        was_enabled = False
        if self.smooth_time:
            self.saved_smooth_time = self.smooth_time
            was_enabled = True
        self.smooth_time = 0.
        return was_enabled
    def enable_shaping(self):
        if not self.saved_smooth_time:
            # Input smoother was not disabled
            return False
        self.smooth_time = self.saved_smooth_time
        self.saved_smooth_time = 0.
        return True
    def report(self, gcmd):
        info = ' '.join(["%s_%s:%s" % (key, self.get_axis(), value)
                         for (key, value) in self.params.get_status().items()])
        gcmd.respond_info(info)

class ShaperFactory:
    def __init__(self):
        pass
    def _create_shaper(self, axis, type_name, config=None):
        if type_name == CustomInputSmootherParams.SHAPER_TYPE:
            return AxisInputSmoother(CustomInputSmootherParams(axis, config))
        if type_name == CustomInputShaperParams.SHAPER_TYPE:
            return AxisInputShaper(CustomInputShaperParams(axis, config))
        if type_name in TypedInputShaperParams.shapers:
            return AxisInputShaper(
                    TypedInputShaperParams(axis, type_name, config))
        if type_name in TypedInputSmootherParams.smoothers:
            return AxisInputSmoother(
                    TypedInputSmootherParams(axis, type_name, config))
        return None
    def create_shaper(self, axis, config):
        shaper_type = config.get('shaper_type', 'mzv')
        shaper_type = config.get('shaper_type_' + axis, shaper_type).lower()
        shaper = self._create_shaper(axis, shaper_type, config)
        if shaper is None:
            raise config.error("Unsupported shaper type '%s'" % (shaper_type,))
        return shaper
    def update_shaper(self, shaper, gcmd):
        shaper_type = gcmd.get('SHAPER_TYPE', None)
        if shaper_type is None:
            shaper_type = gcmd.get('SHAPER_TYPE_' + shaper.get_axis().upper(),
                                   shaper.get_type())
        shaper_type = shaper_type.lower()
        try:
            shaper.update(shaper_type, gcmd)
            return shaper
        except gcmd.error:
            pass
        shaper = self._create_shaper(shaper.get_axis(), shaper_type)
        if shaper is None:
            raise gcmd.error("Unsupported shaper type '%s'" % (shaper_type,))
        shaper.update(shaper_type, gcmd)
        return shaper

class InputShaper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect", self.connect)
        self.toolhead = None
        self.extruders = []
        self.exact_mode = 0
        self.config_extruder_names = config.getlist('enabled_extruders', [])
        self.shaper_factory = ShaperFactory()
        self.shapers = [self.shaper_factory.create_shaper('x', config),
                        self.shaper_factory.create_shaper('y', config)]
        self.input_shaper_stepper_kinematics = []
        self.orig_stepper_kinematics = []
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SET_INPUT_SHAPER",
                               self.cmd_SET_INPUT_SHAPER,
                               desc=self.cmd_SET_INPUT_SHAPER_help)
        gcode.register_command("ENABLE_INPUT_SHAPER",
                               self.cmd_ENABLE_INPUT_SHAPER,
                               desc=self.cmd_ENABLE_INPUT_SHAPER_help)
        gcode.register_command("DISABLE_INPUT_SHAPER",
                               self.cmd_DISABLE_INPUT_SHAPER,
                               desc=self.cmd_DISABLE_INPUT_SHAPER_help)
    def get_shapers(self):
        return self.shapers
    def connect(self):
        self.toolhead = self.printer.lookup_object("toolhead")
        for en in self.config_extruder_names:
            extruder = self.printer.lookup_object(en)
            if not hasattr(extruder, 'get_extruder_steppers'):
                raise self.printer.config_error(
                        "Invalid extruder '%s' in [input_shaper]" % (en,))
            self.extruders.append(extruder)
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
            old_delay = ffi_lib.input_shaper_get_step_gen_window(is_sk)
            for shaper in self.shapers:
                if shaper in failed_shapers:
                    continue
                if not shaper.update_stepper_kinematics(is_sk):
                    failed_shapers.append(shaper)
            new_delay = ffi_lib.input_shaper_get_step_gen_window(is_sk)
            if old_delay != new_delay:
                self.toolhead.note_step_generation_scan_time(new_delay,
                                                             old_delay)
        for e in self.extruders:
            for es in e.get_extruder_steppers():
                failed_shapers.extend(es.update_input_shaping(self.shapers,
                                                              self.exact_mode))
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
            self.shapers = [self.shaper_factory.update_shaper(shaper, gcmd)
                            for shaper in self.shapers]
            self._update_input_shaping()
        for shaper in self.shapers:
            shaper.report(gcmd)
    cmd_ENABLE_INPUT_SHAPER_help = "Enable input shaper for given objects"
    def cmd_ENABLE_INPUT_SHAPER(self, gcmd):
        self.toolhead.flush_step_generation()
        axes = gcmd.get('AXIS', '')
        msg = ''
        for axis_str in axes.split(','):
            axis = axis_str.strip().lower()
            if not axis:
                continue
            shapers = [s for s in self.shapers if s.get_axis() == axis]
            if not shapers:
                raise gcmd.error("Invalid AXIS='%s'" % (axis_str,))
            for s in shapers:
                if s.enable_shaping():
                    msg += "Enabled input shaper for AXIS='%s'\n" % (axis_str,)
                else:
                    msg += ("Cannot enable input shaper for AXIS='%s': "
                            "was not disabled\n" % (axis_str,))
        extruders = gcmd.get('EXTRUDER', '')
        self.exact_mode = gcmd.get_int('EXACT', self.exact_mode)
        for en in extruders.split(','):
            extruder_name = en.strip()
            if not extruder_name:
                continue
            extruder = self.printer.lookup_object(extruder_name)
            if not hasattr(extruder, 'get_extruder_steppers'):
                raise gcmd.error("Invalid EXTRUDER='%s'" % (en,))
            if extruder not in self.extruders:
                self.extruders.append(extruder)
                msg += "Enabled input shaper for '%s'\n" % (en,)
            else:
                msg += "Input shaper already enabled for '%s'\n" % (en,)
        self._update_input_shaping()
        gcmd.respond_info(msg)
    cmd_DISABLE_INPUT_SHAPER_help = "Disable input shaper for given objects"
    def cmd_DISABLE_INPUT_SHAPER(self, gcmd):
        self.toolhead.flush_step_generation()
        axes = gcmd.get('AXIS', '')
        msg = ''
        for axis_str in axes.split(','):
            axis = axis_str.strip().lower()
            if not axis:
                continue
            shapers = [s for s in self.shapers if s.get_axis() == axis]
            if not shapers:
                raise gcmd.error("Invalid AXIS='%s'" % (axis_str,))
            for s in shapers:
                if s.disable_shaping():
                    msg += "Disabled input shaper for AXIS='%s'\n" % (axis_str,)
                else:
                    msg += ("Cannot disable input shaper for AXIS='%s': not "
                            "enabled or was already disabled\n" % (axis_str,))
        extruders = gcmd.get('EXTRUDER', '')
        for en in extruders.split(','):
            extruder_name = en.strip()
            if not extruder_name:
                continue
            extruder = self.printer.lookup_object(extruder_name)
            if extruder in self.extruders:
                to_re_enable = [s for s in self.shapers if s.disable_shaping()]
                for es in extruder.get_extruder_steppers():
                    es.update_input_shaping(self.shapers, self.exact_mode)
                for shaper in to_re_enable:
                    shaper.enable_shaping()
                self.extruders.remove(extruder)
                msg += "Disabled input shaper for '%s'\n" % (en,)
            else:
                msg += "Input shaper not enabled for '%s'\n" % (en,)
        self._update_input_shaping()
        gcmd.respond_info(msg)

def load_config(config):
    return InputShaper(config)
