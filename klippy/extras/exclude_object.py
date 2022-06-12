# Exclude moves toward and inside objects
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
# Copyright (C) 2021  Troy Jacobson <troy.d.jacobson@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import json

class ExcludeObject:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.load_object(config, 'gcode_move')
        self.printer.register_event_handler('klippy:connect',
                                        self._handle_connect)
        self.printer.register_event_handler("virtual_sdcard:reset_file",
                                            self._reset_file)
        self.next_transform = None
        self.last_position_extruded = [0., 0., 0., 0.]
        self.last_position_excluded = [0., 0., 0., 0.]

        self._reset_state()
        self.gcode.register_command(
            'EXCLUDE_OBJECT_START', self.cmd_EXCLUDE_OBJECT_START,
            desc=self.cmd_EXCLUDE_OBJECT_START_help)
        self.gcode.register_command(
            'EXCLUDE_OBJECT_END', self.cmd_EXCLUDE_OBJECT_END,
            desc=self.cmd_EXCLUDE_OBJECT_END_help)
        self.gcode.register_command(
            'EXCLUDE_OBJECT', self.cmd_EXCLUDE_OBJECT,
            desc=self.cmd_EXCLUDE_OBJECT_help)
        self.gcode.register_command(
            'EXCLUDE_OBJECT_DEFINE', self.cmd_EXCLUDE_OBJECT_DEFINE,
            desc=self.cmd_EXCLUDE_OBJECT_DEFINE_help)

    def _register_transform(self):
        if self.next_transform is None:
            tuning_tower = self.printer.lookup_object('tuning_tower')
            if tuning_tower.is_active():
                logging.info('The ExcludeObject move transform is not being '
                    'loaded due to Tuning tower being Active')
                return

            self.next_transform = self.gcode_move.set_move_transform(self,
                                                                     force=True)
            self.extrusion_offsets = {}
            self.max_position_extruded = 0
            self.max_position_excluded = 0
            self.extruder_adj = 0
            self.initial_extrusion_moves = 5
            self.last_position = [0., 0., 0., 0.]

            self.get_position()
            self.last_position_extruded[:] = self.last_position
            self.last_position_excluded[:] = self.last_position

    def _handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')

    def _unregister_transform(self):
        if self.next_transform:
            tuning_tower = self.printer.lookup_object('tuning_tower')
            if tuning_tower.is_active():
                logging.error('The Exclude Object move transform was not '
                    'unregistered because it is not at the head of the '
                    'transform chain.')
                return

            self.gcode_move.set_move_transform(self.next_transform, force=True)
            self.next_transform = None
            self.gcode_move.reset_last_position()

    def _reset_state(self):
        self.objects = []
        self.excluded_objects = []
        self.current_object = None
        self.in_excluded_region = False

    def _reset_file(self):
        self._reset_state()
        self._unregister_transform()

    def _get_extrusion_offsets(self):
        offset = self.extrusion_offsets.get(
            self.toolhead.get_extruder().get_name())
        if offset is None:
            offset = [0., 0., 0., 0.]
            self.extrusion_offsets[self.toolhead.get_extruder().get_name()] = \
                offset
        return offset

    def get_position(self):
        offset = self._get_extrusion_offsets()
        pos = self.next_transform.get_position()
        for i in range(4):
            self.last_position[i] = pos[i] + offset[i]
        return list(self.last_position)

    def _normal_move(self, newpos, speed):
        offset = self._get_extrusion_offsets()

        if self.initial_extrusion_moves > 0 and \
            self.last_position[3] != newpos[3]:
            # Since the transform is not loaded until there is a request to
            # exclude an object, the transform needs to track a few extrusions
            # to get the state of the extruder
            self.initial_extrusion_moves -= 1

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

        if self.extruder_adj != 0 and \
            newpos[3] != self.last_position_excluded[3]:
            offset[3] += self.extruder_adj
            self.extruder_adj = 0

        tx_pos = newpos[:]
        for i in range(4):
            tx_pos[i] = newpos[i] - offset[i]
        self.next_transform.move(tx_pos, speed)

    def _ignore_move(self, newpos, speed):
        offset = self._get_extrusion_offsets()
        for i in range(3):
            offset[i] = newpos[i] - self.last_position_extruded[i]
        offset[3] = offset[3] + newpos[3] - self.last_position[3]
        self.last_position[:] = newpos
        self.last_position_excluded[:] =self.last_position
        self.max_position_excluded = max(self.max_position_excluded, newpos[3])

    def _move_into_excluded_region(self, newpos, speed):
        self.in_excluded_region = True
        self._ignore_move(newpos, speed)

    def _move_from_excluded_region(self, newpos, speed):
        self.in_excluded_region = False

        # This adjustment value is used to compensate for any retraction
        # differences between the last object printed and excluded one.
        self.extruder_adj = self.max_position_excluded \
            - self.last_position_excluded[3] \
            - (self.max_position_extruded - self.last_position_extruded[3])
        self._normal_move(newpos, speed)

    def _test_in_excluded_region(self):
        # Inside cancelled object
        return self.current_object in self.excluded_objects \
            and self.initial_extrusion_moves == 0

    def get_status(self, eventtime=None):
        status = {
            "objects": self.objects,
            "excluded_objects": self.excluded_objects,
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

    cmd_EXCLUDE_OBJECT_START_help = "Marks the beginning the current object" \
                                    " as labeled"
    def cmd_EXCLUDE_OBJECT_START(self, gcmd):
        name = gcmd.get('NAME').upper()
        if not any(obj["name"] == name for obj in self.objects):
            self._add_object_definition({"name": name})
        self.current_object = name
        self.was_excluded_at_start = self._test_in_excluded_region()

    cmd_EXCLUDE_OBJECT_END_help = "Marks the end the current object"
    def cmd_EXCLUDE_OBJECT_END(self, gcmd):
        if self.current_object == None and self.next_transform:
            gcmd.respond_info("EXCLUDE_OBJECT_END called, but no object is"
                              " currently active")
            return
        name = gcmd.get('NAME', default=None)
        if name != None and name.upper() != self.current_object:
            gcmd.respond_info("EXCLUDE_OBJECT_END NAME=%s does not match the"
                              " current object NAME=%s" %
                              (name.upper(), self.current_object))

        self.current_object = None

    cmd_EXCLUDE_OBJECT_help = "Cancel moves inside a specified objects"
    def cmd_EXCLUDE_OBJECT(self, gcmd):
        reset = gcmd.get('RESET', None)
        current = gcmd.get('CURRENT', None)
        name = gcmd.get('NAME', '').upper()

        if reset:
            if name:
                self._unexclude_object(name)

            else:
                self.excluded_objects = []

        elif name:
            if name.upper() not in self.excluded_objects:
                self._exclude_object(name.upper())

        elif current:
            if not self.current_object:
                gcmd.respond_error('There is no current object to cancel')

            else:
                self._exclude_object(self.current_object)

        else:
            self._list_excluded_objects(gcmd)

    cmd_EXCLUDE_OBJECT_DEFINE_help = "Provides a summary of an object"
    def cmd_EXCLUDE_OBJECT_DEFINE(self, gcmd):
        reset = gcmd.get('RESET', None)
        name = gcmd.get('NAME', '').upper()

        if reset:
            self._reset_file()

        elif name:
            parameters = gcmd.get_command_parameters().copy()
            parameters.pop('NAME')
            center = parameters.pop('CENTER', None)
            polygon = parameters.pop('POLYGON', None)

            obj = {"name": name.upper()}
            obj.update(parameters)

            if center != None:
                obj['center'] = json.loads('[%s]' % center)

            if polygon != None:
                obj['polygon'] = json.loads(polygon)

            self._add_object_definition(obj)

        else:
            self._list_objects(gcmd)

    def _add_object_definition(self, definition):
        self.objects = sorted(self.objects + [definition],
                              key=lambda o: o["name"])

    def _exclude_object(self, name):
        self._register_transform()
        self.gcode.respond_info('Excluding object {}'.format(name.upper()))
        if name not in self.excluded_objects:
            self.excluded_objects = sorted(self.excluded_objects + [name])

    def _unexclude_object(self, name):
        self.gcode.respond_info('Unexcluding object {}'.format(name.upper()))
        if name in self.excluded_objects:
            excluded_objects = list(self.excluded_objects)
            excluded_objects.remove(name)
            self.excluded_objects = sorted(excluded_objects)

    def _list_objects(self, gcmd):
        if gcmd.get('JSON', None) is not None:
            object_list = json.dumps(self.objects)
        else:
            object_list = " ".join(obj['name'] for obj in self.objects)
        gcmd.respond_info('Known objects: {}'.format(object_list))

    def _list_excluded_objects(self, gcmd):
        object_list = " ".join(self.excluded_objects)
        gcmd.respond_info('Excluded objects: {}'.format(object_list))

def load_config(config):
    return ExcludeObject(config)
