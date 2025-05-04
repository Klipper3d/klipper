# Code for supporting mixing extruders.
#
# Copyright (C) 2022-2023 Peter Gruber <nokos@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
import logging

EXTRUSION_FACTOR_THRESHOLD=0.01


# Remove scales which are to close to zero to be used as
# recipocal of the rotation_distance. Rescale others to compensate.
def normalize_mixing(mixing):
    normalized = tuple(0 if m < EXTRUSION_FACTOR_THRESHOLD else m
                       for m in mixing)
    s = sum(normalized)
    return tuple(m / s for m in normalized)


def mixing_preset(idx, steppers_count):
    if idx == 0:
        return [1. / steppers_count for p in range(steppers_count)]
    idx = idx - 1
    if idx < steppers_count:
        return [1. if p == idx else 0. for p in range(steppers_count)]
    idx = idx - steppers_count
    if idx < steppers_count:
        return [0. if p == idx else 1. / (steppers_count - 1)
                for p in range(steppers_count)]
    idx = idx - steppers_count
    if steppers_count == 3:
        if idx < 2 * steppers_count:
            return [[0. if p == x else (1 + (((p - x) % 3 + y) % 2)) / 3.
                     for p in range(steppers_count)]
                    for x in range(steppers_count) for y in (0, 1)][idx]
        idx = idx - 2 * steppers_count
    elif steppers_count > 3:
        if idx < (steppers_count * (steppers_count - 1) / 2):
            return [[0. if p == x or p == y else 1. / (steppers_count - 2)
                     for p in range(steppers_count)]
                    for x in range(steppers_count)
                    for y in range(x + 1, steppers_count)][idx]
    return [1. / steppers_count for p in range(steppers_count)]


class MixingExtruder(object):
    def __init__(self, config, parent=None):
        self.printer = config.get_printer()
        self._activated = False
        self.name = "mixingextruder"
        self.stepper_names = tuple(e.strip()
                               for e in config.get('steppers', '').split(",")
                               if len(e))
        if not len(self.stepper_names):
            raise self.printer.config_error(
                "No steppers configured for mixing")
        self._extruder_steppers = []
        self._rotation_distances = ()
        self._trapqs = ()
        self.extruder_name = config.get('extruder_name',
                                        self.stepper_names[0])
        self._mixing = self._active_mixing = \
            mixing_preset(0, len(self.stepper_names))
        self._enabled = False

        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SET_MIXING_EXTRUDER",
                               self.cmd_SET_MIXING_EXTRUDER,
                               desc=self.cmd_SET_MIXING_EXTRUDER_help)
    def _handle_connect(self):
        if self._activated:
            return
        self._activated = True
        for name in self.stepper_names:
            printer_extruder_name = name
            if name != "extruder":
                printer_extruder_name = "extruder_stepper " + name
            extruder_stepper = self.printer.lookup_object(
                printer_extruder_name).extruder_stepper
            self._extruder_steppers.append(
                (extruder_stepper, extruder_stepper.stepper))
        self._rotation_distances = tuple(
            stepper.get_rotation_distance()[0]
            for _, stepper in self._extruder_steppers)
        self._trapqs = tuple(
            stepper.get_trapq()
            for _, stepper in self._extruder_steppers)
    def get_status(self, eventtime):
        return dict(
            mixing=",".join("%0.1f" % (m * 100.)
                            for m in self._active_mixing),
            ticks=",".join("%d" % (stepper.get_mcu_position())
                           for _, stepper in self._extruder_steppers))
    def get_name(self):
        return self.name
    def _init_mixing(self):
        for i, (extruder_stepper, _) in enumerate(self._extruder_steppers):
            extruder_stepper.sync_to_extruder(self.extruder_name)
    def _restore_extruder(self):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        for i, (extruder_stepper, _) in enumerate(self._extruder_steppers):
            extruder_stepper.stepper.set_rotation_distance(
                self._rotation_distances[i]
            )
            extruder_stepper.stepper.set_trapq(
                self._trapqs[i]
            )
    def _apply_mixing(self, mixing):
        if mixing and not self._enabled:
            self._init_mixing()
        elif self._enabled and not mixing:
            self._restore_extruder()
            return
        if not self._enabled:
            return
        self._active_mixing = normalize_mixing(mixing)
        for i, (extruder_stepper,
                stepper) in enumerate(self._extruder_steppers):
            if self._active_mixing[i] > 0 and stepper.get_trapq():
              extruder_stepper.set_rotation_distance(
                  self._rotation_distances[i] / self._active_mixing[i])
            elif self._active_mixing[i] > 0 and not stepper.get_trapq():
              extruder_stepper.sync_to_extruder(self.extruder_name)
              extruder_stepper.set_rotation_distance(
                  self._rotation_distances[i] / self._active_mixing[i])
            elif self._active_mixing[i] == 0 and stepper.get_trapq():
              extruder_stepper.sync_to_extruder(None)
    cmd_SET_MIXING_EXTRUDER_help = "Set scale on stepper"
    def cmd_SET_MIXING_EXTRUDER(self, gcmd):
        if (gcmd.get('FACTORS', None) or
            gcmd.get('PRESET', None) or
            gcmd.get('ENABLE', None)):
            factors = gcmd.get('FACTORS', None)
            preset = gcmd.get_int('PRESET', 0, minval=0)
            enable = gcmd.get_int('ENABLE', 1, minval=0, maxval=1)
            ratios = [1 if p == 0 else 0
                    for p in range(len(self.stepper_names))]
            if factors:
                try:
                    factors = [float(s)
                            for s in factors.replace(',', ':').split(":")]
                    if len(factors) > len(self.stepper_names):
                        raise Exception("Invalid index")
                except Exception as e:
                    raise gcmd.error(
                        "Invalid stepper/motor factor: %s" % (e.message))
                for index, factor in enumerate(factors):
                    ratios[index] = factor
            elif preset >= 0:
                ratios = mixing_preset(preset, len(self.stepper_names))
            if enable:
                s = sum(ratios)
                if s <= 0:
                    raise gcmd.error("Could not save ratio: its empty")
                for i, v in enumerate(ratios):
                    self._mixing[i] = v / s
                self._enabled = bool(enable)
                self._apply_mixing(self._mixing)
            else:
                self._apply_mixing(None)
                self._enabled = bool(enable)
        else:
            mix = "mixing "
            gcmd.respond_info(
                mix + ("enabled" if self._enabled else "disabled"))
            gcmd.respond_info(
                mix + ",".join("%0.1f%%" % (m * 100.)
                               for m in self._active_mixing))
            gcmd.respond_info(
                mix + ",".join("%d" % (stepper.get_mcu_position())
                               for _, stepper in self._extruder_steppers))
            gcmd.respond_info(
                mix + ",".join(extruder_stepper.name
                    for extruder_stepper, _ in self._extruder_steppers))


class GradientMixingExtruder(MixingExtruder):
    def __init__(self, config, **kwargs):
        super(GradientMixingExtruder, self).__init__(config, **kwargs)
        self._gradient_enabled = False
        # assumed to be sorted list of ((start, middle, end), (ref1, ref2))
        self._gradients = []
        self._gradient_method = 'linear'
        self._gradient_vector = (0., 0., 1.)

        # Register commands
        gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.load_object(config, "gcode_move")
        self.normal_transform = None

        gcode.register_command("SET_MIXING_EXTRUDER_GRADIENT",
                               self.cmd_SET_MIXING_EXTRUDER_GRADIENT,
                               desc=self.cmd_SET_MIXING_EXTRUDER_GRADIENT_help)
    def _enable_gradient(self):
        if self.normal_transform:
            return
        if not self._enabled:
            self._init_mixing()
        nt = self.gcode_move.set_move_transform(self, force=True)
        self.normal_transform = nt
        gcode_pos = self.gcode_move.get_status()['gcode_position']
        self._apply_mixing(self._get_gradient(gcode_pos))
    def _disable_gradient(self):
        if not self.normal_transform:
            return
        self.gcode_move.set_move_transform(self.normal_transform, force=True)
        self.normal_transform = None
    def _get_gradient(self, pos):
        mixing = self._mixing
        for heights, refs in self._gradients:
            start, _, end = heights
            start_mix, end_mix = refs
            if self._gradient_method == 'linear':
                zpos = sum(pos[i]*self._gradient_vector[i] for i in range(3))
                if zpos <= start:
                    return start_mix
                if zpos >= end:
                    mixing = end_mix
                    continue
                w = (zpos - start) / (end - start)
                return list(((1. - w) * s + w * e)
                            for s, e in zip(start_mix, end_mix))
            if self._gradient_method == 'spherical':
                dist = math.sqrt(sum((x-self._gradient_vector[i])**2
                                     for i, x in enumerate(pos)))
                if dist <= start:
                    return start_mix
                if dist >= end:
                    mixing = end_mix
                    continue
                w = (dist - start) / (end - start)
                mix = list(((1. - w) * s + w * e)
                           for s, e in zip(start_mix, end_mix))
                return mix
        return mixing
    def get_position(self):
        self.normal_transform.get_position()
    def move(self, newpos, speed):
        gcode_pos = self.gcode_move.get_status()['gcode_position']
        self._apply_mixing(self._get_gradient(gcode_pos))
        self.normal_transform.move(newpos, speed)
    def _check_scales(self, gcmd, s):
        try:
            scales = s.replace(',', ':').split(':')
            if len(scales) != len(self._mixing):
                raise gcmd.error("Could not configure gradient: invalid scales")
            return tuple(float(scales[i]) for i in range(len(scales)))
        except ValueError:
            raise gcmd.error(
                "Could not configure gradient: could not parse scales")
    cmd_SET_MIXING_EXTRUDER_GRADIENT_help = "Configure gradient mixing"
    def cmd_SET_MIXING_EXTRUDER_GRADIENT(self, gcmd):
        if gcmd.get('START_FACTORS', ''):
            start_scales = self._check_scales(gcmd,gcmd.get('START_FACTORS'))
            end_scales = self._check_scales(gcmd, gcmd.get('END_FACTORS'))
            start_height = gcmd.get_float('START_HEIGHT', minval=0.)
            end_height = gcmd.get_float('END_HEIGHT', minval=0.)
            if start_height > end_height:
                start_height, end_height = end_height, start_height
                start_scales, end_scales = end_scales, start_scales
            for gradient in self._gradients:
                s, _, e = gradient[0]
                if e > start_height and end_height > s:
                    raise gcmd.error(
                        "Could not configure gradient: overlapping starts/ends")
            self._gradients.append((
                (start_height, (start_height + end_height) / 2, end_height),
                (start_scales, end_scales)))
            self._gradients.sort(key=lambda x: x[0][0])
        elif gcmd.get('ENABLE', '') == 'RESET':
            self._gradient_enabled, self._gradients, self._gradient_method = \
                False, [], 'linear'
            self._disable_gradient()
        elif gcmd.get('ENABLE', '') or gcmd.get('METHOD', ''):
            method = gcmd.get('METHOD', '')
            if method in ["linear", "spherical"]:
                self._gradient_method = method
            try:
                vector = gcmd.get('VECTOR', '0,0,1').split(',')
                self._gradient_vector = tuple(float(vector[i])
                                              for i in range(3))
            except Exception:
                raise gcmd.error(
                    "Could not configure gradient: invalid vector")
            try:
                enable = gcmd.get_int('ENABLE', 1, minval=0, maxval=1) == 1
            except Exception:
                enable = gcmd.get('ENABLE', '').lower() == 'true'
            if enable:
                self._enable_gradient()
            else:
                self._disable_gradient()
            self._gradient_enabled = enable
        else:
            for i, gradient in enumerate(self._gradients):
                grad = "gradient[%d] " % (i)
                gcmd.respond_info(
                    grad + "heights=%.1f-(%.1f)-%.1f" % gradient[0])
                gcmd.respond_info(
                    grad + "mixing=%s-%s" % tuple(
                        "/".join("%.1f" % (x) for x in m)
                        for m in gradient[1]))
                gcmd.respond_info(
                    grad + "method=%s" % (self._gradient_method))
                gcmd.respond_info(
                    grad + "vector=%s" % ("/".join("%.1f" % (x)
                        for x in self._gradient_vector)))
                gcmd.respond_info(
                    grad + "enabled=%s" % (str(self._gradient_enabled)))


def load_config(config):
    mixingextruder = GradientMixingExtruder(
        config.getsection('mixing_extruder'))
    return mixingextruder
