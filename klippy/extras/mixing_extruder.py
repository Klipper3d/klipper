# Code for supporting mixing extruders.
#
# Copyright (C) 2022 Peter Gruber <nokos@gmx.net>
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
                               desc=self.cmd_MIXING_STATUS_help)
        gcode.register_command("MIXING_STATUS",
                               self.cmd_MIXING_STATUS,
                               desc=self.cmd_MIXING_STATUS_help)
    def _handle_connect(self):
        if self._activated:
            return
        self._activated = True
        try:
            for name in self.stepper_names:
                try:
                    stepper = (self.printer
                               .lookup_object(name).extruder_stepper)
                except:
                    stepper = (self.printer
                               .lookup_object("extruder_stepper " + name)
                               .extruder_stepper)
                self._extruder_steppers.append(stepper)
        except Exception as e:
            while len(self.steppers) > 0:
                self.steppers.pop()
            logging.error("no steppers found: %s: %s",
                          ", ".join(self.stepper_names), e)
        self._rotation_distances = tuple(
            stepper.stepper.get_rotation_distance()[0]
            for stepper in self._extruder_steppers)
        self._trapqs = tuple(
            stepper.stepper.get_trapq()
            for stepper in self._extruder_steppers)
    def get_status(self, eventtime):
        return dict(
            mixing_enabled="enabled" if self._enabled else "disabled",
            mixing=",".join(
                "%0.1f%%" % (m * 100.) for m in self._active_mixing),
            mixing_ticks=",".join(
                "%d" % (extruder_stepper.stepper.get_mcu_position())
                for extruder_stepper in self._extruder_steppers),
            mixing_steppers=",".join(
                extruder_stepper.name
                for extruder_stepper in self._extruder_steppers))
    def get_name(self):
        return self.name
    def _init_mixing(self):
        for i, extruder_stepper in enumerate(self._extruder_steppers):
            extruder_stepper.sync_to_extruder(self.extruder_name)
    def _restore_extruder(self):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        for i, extruder_stepper in enumerate(self._extruder_steppers):
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
        for i, extruder_stepper in enumerate(self._extruder_steppers):
            if self._active_mixing[i] > 0 and \
                extruder_stepper.stepper.get_trapq():
              extruder_stepper.set_rotation_distance(
                  self._rotation_distances[i] / self._active_mixing[i])
            elif self._active_mixing[i] > 0 and \
                not extruder_stepper.stepper.get_trapq():
              extruder_stepper.sync_to_extruder(self.extruder_name)
              extruder_stepper.set_rotation_distance(
                  self._rotation_distances[i] / self._active_mixing[i])
            elif self._active_mixing[i] == 0 and \
                extruder_stepper.stepper.get_trapq():
              extruder_stepper.sync_to_extruder(None)
    cmd_SET_MIXING_EXTRUDER_help = "Set scale on stepper"
    def cmd_SET_MIXING_EXTRUDER(self, gcmd):
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
    cmd_MIXING_STATUS_help = "Display the status of the given MixingExtruder"
    def cmd_MIXING_STATUS(self, gcmd):
        eventtime = self.printer.get_reactor().monotonic()
        status = self.get_status(eventtime)
        gcmd.respond_info(", ".join("%s=%s" % (k, v)
                                    for k, v in status.items()))


def load_config(config):
    mixingextruder = MixingExtruder(config.getsection('mixing_extruder'))
    logging.info("Added mixing extruders")
    return mixingextruder
