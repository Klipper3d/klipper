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
        self.printer.register_event_handler('klippy:connect',
                                        self._handle_connect)
        self.printer.register_event_handler("sdcard:reset_file",
                                            self._reset_file)
        self.next_transform = None
        self.log_next_moves = 0
        self.objects = {}
        self.excluded_objects = []
        self.current_object = None
        self.in_excluded_region = False
        self.extruder_idx = 0
        self.extrusion_offsets = [[0., 0., 0., 0.]]
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
            self.last_position[:] = self.get_position()
            self.last_position_extruded[:] = self.get_position()
            self.last_position_excluded[:] = self.get_position()
            self.extrusion_offsets = [[0., 0., 0., 0.]]
            self.max_position_extruded = 0
            self.max_position_excluded = 0
            self.extruder_adj = 0
            self.objects = {}
            self.excluded_objects = []
            self.current_object = None

    def _handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')

    def _reset_file(self):
        self.objects = {}
        self.excluded_objects = []
        self.current_object = None
        if self.next_transform:
            logging.debug('Disabling ExcludeObject as a move transform')
            self.gcode_move.set_move_transform(self.next_transform, force=True)
            self.next_transform = None
            self.gcode_move.reset_last_position()

    def get_position(self):
        extr_idx = self.toolhead.get_extruder().get_name()[8:]
        if extr_idx:
            self.extruder_idx = int(extr_idx)
        else:
            self.extruder_idx = 0
        len_diff = self.extruder_idx + 1 - len(self.extrusion_offsets)
        if len_diff > 0:
            self.extrusion_offsets.extend([[0., 0., 0., 0.]] * len_diff)
        offset = self.extrusion_offsets[self.extruder_idx]
        pos = self.next_transform.get_position()
        for i in range(4):
            self.last_position[i] = pos[i] + offset[i]
        return list(self.last_position)

    def _normal_move(self, newpos, speed):
        offset = self.extrusion_offsets[self.extruder_idx]
        if self.log_next_moves > 0:
            self.log_next_moves -= 1
            logging.debug("EXO: Toolhead commanded pos: %s",
                        " ".join(str(x) for x in self.toolhead.commanded_pos))
            logging.debug("EXO: Move position: %s", " ".join(str(x) for x in newpos))
            logging.debug("EXO: Offset: %s", " ".join(str(x) for x in offset))

        self.last_position[:] = newpos
        self.last_position_extruded[:] = self.last_position
        self.max_position_extruded = max(self.max_position_extruded, newpos[3])

        # These next few conditionals handle the moves immediately after leaving
        # and excluded object.  The toolhead is at the end of the last printed
        # object and the gcode is at the end of the last excluded object.
        #
        # Ideally, there will be Z and E moves right away to adjust any offsets
        # before moving away from the last position.  Any remaining corrections
        # will be made on the firs XY move.
        if (offset[0] != 0 or offset[1] != 0) and \
            (newpos[0] != self.last_position_excluded[0] or \
            newpos[1] != self.last_position_excluded[1]):
            offset[0] = 0
            offset[1] = 0
            offset[2] = 0
            offset[3] += self.extruder_adj
            self.extruder_adj = 0

        if offset[2] != 0 and newpos[2] != self.last_position_excluded[2]:
            offset[2] = 0

        if self.extruder_adj != 0 and newpos[3] != self.last_position_excluded[3]:
            offset[3] += self.extruder_adj
            self.extruder_adj = 0

        tx_pos = newpos[:]
        for i in range(4):
            tx_pos[i] = newpos[i] - offset[i]
        self.next_transform.move(tx_pos, speed)

    def _ignore_move(self, newpos, speed):
        offset = self.extrusion_offsets[self.extruder_idx]
        for i in range(3):
            offset[i] = newpos[i] - self.last_position_extruded[i]
        offset[3] = offset[3] + newpos[3] - self.last_position[3]
        self.last_position[:] = newpos
        self.last_position_excluded[:] =self.last_position 
        self.max_position_excluded = max(self.max_position_excluded, newpos[3])

    def _move_into_excluded_region(self, newpos, speed):
        self.in_excluded_region = True
        logging.debug("Moving to excluded object: %s",
                      (self.current_object or "---"))
        self._ignore_move(newpos, speed)

    def _move_from_excluded_region(self, newpos, speed):
        self.in_excluded_region = False
        offset = self.extrusion_offsets[self.extruder_idx]
        logging.debug("EXO: Moving to included object: %s",
                      (self.current_object or "---"))
        logging.debug("EXO: Toolhead commanded pos: %s",
                      " ".join(str(x) for x in self.toolhead.commanded_pos))
        logging.debug("EXO: last position: %s",
                      " ".join(str(x) for x in self.last_position))
        logging.debug("EXO: last extruded position: %s",
                      " ".join(str(x) for x in self.last_position_extruded))
        logging.debug("EXO: Max extruded position: %f", self.max_position_extruded)
        logging.debug("EXO: last excluded position: %s",
                      " ".join(str(x) for x in self.last_position_excluded))
        logging.debug("EXO: Max excluded position: %f", self.max_position_excluded)
        logging.debug("EXO: New position: %s", " ".join(str(x) for x in newpos))
        logging.debug("EXO: Offset: %s", " ".join(str(x) for x in offset))
        self.log_next_moves = 10

        # This adjustment value is used to compensate for any retraction
        # differences between the last object printed and excluded one.
        self.extruder_adj = self.max_position_excluded - self.last_position_excluded[3] \
            - (self.max_position_extruded - self.last_position_extruded[3])
        logging.debug("EXO: Adjustment: %f", self.extruder_adj)
        self._normal_move(newpos, speed)

    def _test_in_excluded_region(self):
        # Inside cancelled object
        return self.current_object in self.excluded_objects

    def get_status(self, eventtime=None):
        status = {
            "objects": list(self.objects.values()),
            "excluded_objects": list(self.excluded_objects),
            "current_object": self.current_object
        }
        return status

    def move(self, newpos, speed):
        move_in_excluded_region = self._test_in_excluded_region()
        self.last_speed = speed

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
        self.was_excluded_at_start = self._test_in_excluded_region()
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

        is_excluded = self._test_in_excluded_region()
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
