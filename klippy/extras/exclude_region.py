# Exclude moves toward and inside set regions
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import math
import logging

def parse_pair(pair):
    pts = pair.strip().split(',', 1)
    if len(pts) != 2:
        raise Exception
    return [float(p.strip()) for p in pts]

class RectRegion:
    def __init__(self, minpt, maxpt):
        self.xmin = minpt[0]
        self.ymin = minpt[1]
        self.xmax = maxpt[0]
        self.ymax = maxpt[1]
    def check_within(self, pos):
        return (self.xmin <= pos[0] <= self.xmax and
                self.ymin <= pos[1] <= self.ymax)

class CircRegion:
    def __init__(self, center, radius):
        self.x = center[0]
        self.y = center[1]
        self.radius = radius
    def check_within(self, pos):
        a = self.x - pos[0]
        b = self.y - pos[1]
        dist_from_pt = math.sqrt(a*a + b*b)
        return dist_from_pt <= self.radius

class ExcludeRegion:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        # Temporary workaround to get skew_correction to register
        # its "klippy:ready" event handler before Exclude Region.  Exclude
        # Region needs to be the highest priority transform, thus it must be
        # the last module that calls set_move_transform()
        if config.has_section('skew_correction'):
            self.printer.try_load_module(config, 'skew_correction')
        # Now ExcludeRegion can register its own event handler
        self.printer.register_event_handler("klippy:ready",
                                            self._handle_ready)
        self.objects = []
        self.current_object = ""
        self.in_excluded_region = False
        self.last_position = [0., 0., 0., 0.]
        self.last_delta = [0., 0., 0., 0.]
        self.gcode.register_command(
            'SET_CURRENT_OBJECT', self.cmd_SET_CURRENT_OBJECT,
            desc=self.cmd_SET_CURRENT_OBJECT_help)
        self.gcode.register_command(
            'CANCEL_OBJECT', self.cmd_CANCEL_OBJECT,
            desc=self.cmd_CANCEL_OBJECT_help)
        self.gcode.register_command(
            'REMOVE_CANCELED_OBJECT', self.cmd_REMOVE_CANCELED_OBJECT,
            desc=self.cmd_REMOVE_CANCELED_OBJECT_help)
        self.gcode.register_command(
            'REMOVE_ALL_EXCLUDED', self.cmd_REMOVE_ALL_EXCLUDED,
            desc=self.cmd_REMOVE_ALL_EXCLUDED_help)
        # debugging
        self.current_region = None
    def _handle_ready(self):
        gcode_move = self.printer.lookup_object('gcode_move')
        self.next_transform = gcode_move.set_move_transform(self, force=True)
    def get_position(self):
        self.last_position[:] = self.next_transform.get_position()
        self.last_delta = [0., 0., 0., 0.]
        return list(self.last_position)

    def _normal_move(self, newpos, speed):
        self.last_delta = [newpos[i] - self.last_position[i] for i in range(4)]
        self.last_position[:] = newpos
        self.next_transform.move(newpos, speed)

    def _ignore_move(self, newpos, speed):
        return

    def _move_to_excluded_region(self, newpos, speed):
        logging.info("Moving to excluded object: " + self.current_object)
        self.in_excluded_region = True
        self.last_extruder = newpos[3]

    def _move_to_included_region(self, newpos, speed):
        logging.info("Moving to included object: " + self.current_object)
        logging.info("Filament position: " + str(self.last_extruder))
        logging.info("last_position: " + " ".join(str(x) for x in self.last_position))
        logging.info("New position: " + " ".join(str(x) for x in newpos))
        self.last_delta = [newpos[i] - self.last_position[i] for i in range(4)]
        newpos[3] = self.last_extruder
        self.last_position[:] = newpos
        self.next_transform.move(newpos, speed)
        self.in_excluded_region = False

    def move(self, newpos, speed):
        move_in_excluded_region = False
        if self.current_object in self.objects:
            # Inside cancelled object
            move_in_excluded_region = True

        if move_in_excluded_region:
            if self.in_excluded_region:
                self._ignore_move(newpos, speed)
            else:
                self._move_to_excluded_region(newpos, speed)
        else:
            if self.in_excluded_region:
                self._move_to_included_region(newpos, speed)
            else:
                self._normal_move(newpos, speed)

    cmd_SET_CURRENT_OBJECT_help = "Set the current object as labeled"
    def cmd_SET_CURRENT_OBJECT(self, params):
        self.current_object = params.get_command_parameters()['NAME'].upper()
    cmd_CANCEL_OBJECT_help = "Cancel moves inside a specified objects"
    def cmd_CANCEL_OBJECT(self, params):
        name = params.get_command_parameters()['NAME'].upper()
        if name not in self.objects:
            self.objects.append(name)
    cmd_REMOVE_CANCELED_OBJECT_help = "Remove cancelled object"
    def cmd_REMOVE_CANCELED_OBJECT(self, params):
        name = self.gcode.get_str('NAME', params).upper()
        if name in self.objects:
            self.objects.remove(name)
    cmd_REMOVE_ALL_EXCLUDED_help = "Removes all excluded objects and regions"
    def cmd_REMOVE_ALL_EXCLUDED(self, params):
        self.objects = []

def load_config(config):
    return ExcludeRegion(config)
