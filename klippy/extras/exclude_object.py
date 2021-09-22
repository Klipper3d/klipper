# Exclude moves toward and inside objects
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
# Copyright (C) 2021  Troy Jacobson <troy.d.jacobson@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import json
from datetime import datetime

class ExcludeObject:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.load_object(config, 'gcode_move')
        self.printer.register_event_handler("sdcard:reset_file",
                                            self._reset_file)
        self.next_transform = None
        self.objects = {}
        self.excluded_objects = []
        self.current_object = None
        self.in_excluded_region = False
        self.last_position = [0., 0., 0., 0.]
        self.last_position_extruded = [0., 0., 0., 0.]
        self.last_position_excluded = [0., 0., 0., 0.]
        self.gcode.register_command(
            'START_CURRENT_OBJECT', self.cmd_START_CURRENT_OBJECT,
            desc=self.cmd_START_CURRENT_OBJECT_help)
        self.gcode.register_command(
            'END_CURRENT_OBJECT', self.cmd_END_CURRENT_OBJECT,
            desc=self.cmd_END_CURRENT_OBJECT_help)
        self.gcode.register_command(
            'EXCLUDE_OBJECT', self.cmd_EXCLUDE_OBJECT,
            desc=self.cmd_EXCLUDE_OBJECT_help)
        self.gcode.register_command(
            'EXCLUDE_OBJECT_RESET', self.cmd_EXCLUDE_OBJECT_RESET,
            desc=self.cmd_EXCLUDE_OBJECT_RESET_help)
        self.gcode.register_command(
            'DEFINE_OBJECT', self.cmd_DEFINE_OBJECT,
            desc=self.cmd_DEFINE_OBJECT_help)
        self.gcode.register_command(
            'LIST_OBJECTS', self.cmd_LIST_OBJECTS,
            desc=self.cmd_LIST_OBJECTS_help)
        self.gcode.register_command(
            'LIST_EXCLUDED_OBJECTS', self.cmd_LIST_EXCLUDED_OBJECTS,
            desc=self.cmd_LIST_EXCLUDED_OBJECTS_help)
    def _setup_transform(self):
        if not self.next_transform:
            logging.debug('Enabling ExcludeObject as a move transform')
            self.next_transform = self.gcode_move.set_move_transform(self,
                                                                     force=True)
    def _reset_file(self):
        self.objects = {}
        self.excluded_objects = []
        self.current_object = None
        if self.next_transform:
            logging.debug('Disabling ExcludeObject as a move transform')
            self.gcode_move.set_move_transform(self.next_transform, force=True)
            self.next_transform = None

    def get_position(self):
        self.last_position[:] = self.next_transform.get_position()
        self.last_delta = [0., 0., 0., 0.]
        return list(self.last_position)

    def _normal_move(self, newpos, speed):
        self.last_position_extruded[:] = newpos
        self.last_position[:] = newpos
        self.next_transform.move(newpos, speed)

    def _ignore_move(self, newpos, speed):
        self.last_position_excluded[:] = newpos
        self.last_position[:] = newpos
        return

    def _move_into_excluded_region(self, newpos, speed):
        logging.debug("Moving to excluded object: %s",
                      (self.current_object or "---"))
        self.in_excluded_region = True
        self.last_position_excluded[:] = newpos
        self.last_position[:] = newpos

    def _move_from_excluded_region(self, newpos, speed):
        logging.debug("Moving to included object: %s",
                      (self.current_object or "---"))
        logging.debug("last position: %s",
                      " ".join(str(x) for x in self.last_position))
        logging.debug("last extruded position: %s",
                      " ".join(str(x) for x in self.last_position_extruded))
        logging.debug("last excluded position: %s",
                      " ".join(str(x) for x in self.last_position_excluded))
        logging.debug("New position: %s", " ".join(str(x) for x in newpos))
        if self.last_position[0] == newpos[0] and \
                self.last_position[1] == newpos[1]:
            # If the X,Y position didn't change for this transitional move,
            #  assume that this move should happen at the last extruded location
            newpos[0] = self.last_position_extruded[0]
            newpos[1] = self.last_position_extruded[1]
        newpos[3] = newpos[3] - self.last_position_excluded[3] + \
                    self.last_position_extruded[3]
        logging.debug("Modified position: " + " ".join(str(x) for x in newpos))
        self.last_position[:] = newpos
        self.last_position_extruded[:] = newpos
        self.next_transform.move(newpos, speed)
        self.in_excluded_region = False

    def _test_in_excluded_region(self):
        # Inside cancelled object
        if self.current_object in self.excluded_objects:
            return True

    def get_status(self, eventtime=None):
        status = {
            "objects": list(self.objects.values()),
            "excluded_objects": list(self.excluded_objects),
            "current_object": self.current_object
        }
        return status

    def move(self, newpos, speed):
        move_in_excluded_region = self._test_in_excluded_region()

        if move_in_excluded_region:
            if self.in_excluded_region:
                self._ignore_move(newpos, speed)
            else:
                self._move_into_excluded_region(newpos, speed)
        else:
            if self.in_excluded_region:
                self._move_from_excluded_region(newpos, speed)
            else:
                self._normal_move(newpos, speed)

    cmd_START_CURRENT_OBJECT_help = "Marks the beginning the current object" \
                                    " as labeled"
    def cmd_START_CURRENT_OBJECT(self, params):
        name = params.get('NAME').upper()
        self.current_object = name
    cmd_END_CURRENT_OBJECT_help = "Markes the end the current object"
    def cmd_END_CURRENT_OBJECT(self, gcmd):
        if self.current_object == None:
            gcmd.respond_info("END_CURRENT_OBJECT called, but no object is"
                              " currently active")
            return
        name = gcmd.get('NAME', default=None)
        if name != None and name.upper() != self.current_object:
            gcmd.respond_info("END_CURRENT_OBJECT NAME=%s does not match the"
                              " current object NAME=%s" %
                              (name.upper(), self.current_object))
        self.current_object = None
    cmd_EXCLUDE_OBJECT_help = "Cancel moves inside a specified objects"
    def cmd_EXCLUDE_OBJECT(self, params):
        name = params.get('NAME').upper()
        if name not in self.excluded_objects:
            self.excluded_objects.append(name)
    cmd_EXCLUDE_OBJECT_RESET_help = "Resets the exclude_object state by" \
                                    " clearing the list of object definitions" \
                                    " and removed objects"
    def cmd_EXCLUDE_OBJECT_RESET(self, params):
        self._reset_file()
    cmd_LIST_OBJECTS_help = "Lists the known objects"
    def cmd_LIST_OBJECTS(self, gcmd):
        if gcmd.get('VERBOSE', None) is not None:
            object_list = " ".join (str(x) for x in self.objects.values())
        else:
            object_list = " ".join(self.objects.keys())
        gcmd.respond_info(object_list)
    cmd_LIST_EXCLUDED_OBJECTS_help = "Lists the excluded objects"
    def cmd_LIST_EXCLUDED_OBJECTS(self, gcmd):
        object_list = " ".join (str(x) for x in self.excluded_objects)
        gcmd.respond_info(object_list)
    cmd_DEFINE_OBJECT_help = "Provides a summary of an object"
    def cmd_DEFINE_OBJECT(self, params):
        self._setup_transform()

        name = params.get('NAME').upper()
        center = params.get('CENTER', default=None)
        polygon = params.get('POLYGON', default=None)

        obj = {
            "name": name,
        }

        if center != None:
            c = [float(coord) for coord in center.split(',')]
            obj['center'] = c

        if polygon != None:
            obj['polygon'] = json.loads(polygon)

        logging.debug('Object %s defined %r', name, obj)
        self.objects[name] = obj


def load_config(config):
    return ExcludeObject(config)
