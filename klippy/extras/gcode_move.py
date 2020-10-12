# G-Code G1 movement commands (and associated coordinate manipulation)
#
# Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import homing

class GCodeMove:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        printer.register_event_handler("klippy:ready", self._handle_ready)
        printer.register_event_handler("klippy:shutdown", self._handle_shutdown)
        printer.register_event_handler("toolhead:set_position",
                                       self.reset_last_position)
        printer.register_event_handler("toolhead:manual_move",
                                       self.reset_last_position)
        printer.register_event_handler("gcode:command_error",
                                       self.reset_last_position)
        printer.register_event_handler("extruder:activate_extruder",
                                       self._handle_activate_extruder)
        self.is_printer_ready = False
        # Register g-code commands
        gcode = printer.lookup_object('gcode')
        handlers = [
            'G1', 'G28', 'G20', 'G21',
            'M82', 'M83', 'G90', 'G91', 'G92', 'M220', 'M221',
            'SET_GCODE_OFFSET', 'SAVE_GCODE_STATE', 'RESTORE_GCODE_STATE',
        ]
        for cmd in handlers:
            func = getattr(self, 'cmd_' + cmd)
            desc = getattr(self, 'cmd_' + cmd + '_help', None)
            gcode.register_command(cmd, func, False, desc)
        gcode.register_command('G0', self.cmd_G1)
        gcode.register_command('M114', self.cmd_M114, True)
        gcode.register_command('GET_POSITION', self.cmd_GET_POSITION, True)
        # G-Code coordinate manipulation
        self.absolute_coord = self.absolute_extrude = True
        self.base_position = [0.0, 0.0, 0.0, 0.0]
        self.last_position = [0.0, 0.0, 0.0, 0.0]
        self.homing_position = [0.0, 0.0, 0.0, 0.0]
        self.speed = 25.
        self.speed_factor = 1. / 60.
        self.extrude_factor = 1.
        # G-Code state
        self.saved_states = {}
        self.move_transform = self.move_with_transform = None
        self.position_with_transform = (lambda: [0., 0., 0., 0.])
    def _handle_ready(self):
        self.is_printer_ready = True
        if self.move_transform is None:
            toolhead = self.printer.lookup_object('toolhead')
            self.move_with_transform = toolhead.move
            self.position_with_transform = toolhead.get_position
    def _handle_shutdown(self):
        if not self.is_printer_ready:
            return
        self.is_printer_ready = False
        logging.info("gcode state: absolute_coord=%s absolute_extrude=%s"
                     " base_position=%s last_position=%s homing_position=%s"
                     " speed_factor=%s extrude_factor=%s speed=%s",
                     self.absolute_coord, self.absolute_extrude,
                     self.base_position, self.last_position,
                     self.homing_position, self.speed_factor,
                     self.extrude_factor, self.speed)
    def _handle_activate_extruder(self):
        self.reset_last_position()
        self.extrude_factor = 1.
        self.base_position[3] = self.last_position[3]
    def set_move_transform(self, transform, force=False):
        if self.move_transform is not None and not force:
            raise self.printer.config_error(
                "G-Code move transform already specified")
        old_transform = self.move_transform
        if old_transform is None:
            old_transform = self.printer.lookup_object('toolhead', None)
        self.move_transform = transform
        self.move_with_transform = transform.move
        self.position_with_transform = transform.get_position
        return old_transform
    def _get_gcode_position(self):
        p = [lp - bp for lp, bp in zip(self.last_position, self.base_position)]
        p[3] /= self.extrude_factor
        return p
    def _get_gcode_speed(self):
        return self.speed / self.speed_factor
    def _get_gcode_speed_override(self):
        return self.speed_factor * 60.
    def get_status(self, eventtime=None):
        move_position = self._get_gcode_position()
        return {
            'speed_factor': self._get_gcode_speed_override(),
            'speed': self._get_gcode_speed(),
            'extrude_factor': self.extrude_factor,
            'absolute_coordinates': self.absolute_coord,
            'absolute_extrude': self.absolute_extrude,
            'homing_origin': homing.Coord(*self.homing_position),
            'position': homing.Coord(*self.last_position),
            'gcode_position': homing.Coord(*move_position),
        }
    def reset_last_position(self):
        if self.is_printer_ready:
            self.last_position = self.position_with_transform()
    # G-Code movement commands
    def cmd_G1(self, gcmd):
        # Move
        params = gcmd.get_command_parameters()
        try:
            for pos, axis in enumerate('XYZ'):
                if axis in params:
                    v = float(params[axis])
                    if not self.absolute_coord:
                        # value relative to position of last move
                        self.last_position[pos] += v
                    else:
                        # value relative to base coordinate position
                        self.last_position[pos] = v + self.base_position[pos]
            if 'E' in params:
                v = float(params['E']) * self.extrude_factor
                if not self.absolute_coord or not self.absolute_extrude:
                    # value relative to position of last move
                    self.last_position[3] += v
                else:
                    # value relative to base coordinate position
                    self.last_position[3] = v + self.base_position[3]
            if 'F' in params:
                gcode_speed = float(params['F'])
                if gcode_speed <= 0.:
                    raise gcmd.error("Invalid speed in '%s'"
                                     % (gcmd.get_commandline(),))
                self.speed = gcode_speed * self.speed_factor
        except ValueError as e:
            raise gcmd.error("Unable to parse move '%s'"
                             % (gcmd.get_commandline(),))
        self.move_with_transform(self.last_position, self.speed)
    def cmd_G28(self, gcmd):
        # Move to origin
        axes = []
        for pos, axis in enumerate('XYZ'):
            if gcmd.get(axis, None) is not None:
                axes.append(pos)
        if not axes:
            axes = [0, 1, 2]
        homing_state = homing.Homing(self.printer)
        homing_state.home_axes(axes)
        for axis in homing_state.get_axes():
            self.base_position[axis] = self.homing_position[axis]
    # G-Code coordinate manipulation
    def cmd_G20(self, gcmd):
        # Set units to inches
        raise gcmd.error('Machine does not support G20 (inches) command')
    def cmd_G21(self, gcmd):
        # Set units to millimeters
        pass
    def cmd_M82(self, gcmd):
        # Use absolute distances for extrusion
        self.absolute_extrude = True
    def cmd_M83(self, gcmd):
        # Use relative distances for extrusion
        self.absolute_extrude = False
    def cmd_G90(self, gcmd):
        # Use absolute coordinates
        self.absolute_coord = True
    def cmd_G91(self, gcmd):
        # Use relative coordinates
        self.absolute_coord = False
    def cmd_G92(self, gcmd):
        # Set position
        offsets = [ gcmd.get_float(a, None) for a in 'XYZE' ]
        for i, offset in enumerate(offsets):
            if offset is not None:
                if i == 3:
                    offset *= self.extrude_factor
                self.base_position[i] = self.last_position[i] - offset
        if offsets == [None, None, None, None]:
            self.base_position = list(self.last_position)
    def cmd_M114(self, gcmd):
        # Get Current Position
        p = self._get_gcode_position()
        gcmd.respond_raw("X:%.3f Y:%.3f Z:%.3f E:%.3f" % tuple(p))
    def cmd_M220(self, gcmd):
        # Set speed factor override percentage
        value = gcmd.get_float('S', 100., above=0.) / (60. * 100.)
        self.speed = self._get_gcode_speed() * value
        self.speed_factor = value
    def cmd_M221(self, gcmd):
        # Set extrude factor override percentage
        new_extrude_factor = gcmd.get_float('S', 100., above=0.) / 100.
        last_e_pos = self.last_position[3]
        e_value = (last_e_pos - self.base_position[3]) / self.extrude_factor
        self.base_position[3] = last_e_pos - e_value * new_extrude_factor
        self.extrude_factor = new_extrude_factor
    cmd_SET_GCODE_OFFSET_help = "Set a virtual offset to g-code positions"
    def cmd_SET_GCODE_OFFSET(self, gcmd):
        move_delta = [0., 0., 0., 0.]
        for pos, axis in enumerate('XYZE'):
            offset = gcmd.get_float(axis, None)
            if offset is None:
                offset = gcmd.get_float(axis + '_ADJUST', None)
                if offset is None:
                    continue
                offset += self.homing_position[pos]
            delta = offset - self.homing_position[pos]
            move_delta[pos] = delta
            self.base_position[pos] += delta
            self.homing_position[pos] = offset
        # Move the toolhead the given offset if requested
        if gcmd.get_int('MOVE', 0):
            speed = gcmd.get_float('MOVE_SPEED', self.speed, above=0.)
            for pos, delta in enumerate(move_delta):
                self.last_position[pos] += delta
            self.move_with_transform(self.last_position, speed)
    cmd_SAVE_GCODE_STATE_help = "Save G-Code coordinate state"
    def cmd_SAVE_GCODE_STATE(self, gcmd):
        state_name = gcmd.get('NAME', 'default')
        self.saved_states[state_name] = {
            'absolute_coord': self.absolute_coord,
            'absolute_extrude': self.absolute_extrude,
            'base_position': list(self.base_position),
            'last_position': list(self.last_position),
            'homing_position': list(self.homing_position),
            'speed': self.speed, 'speed_factor': self.speed_factor,
            'extrude_factor': self.extrude_factor,
        }
    cmd_RESTORE_GCODE_STATE_help = "Restore a previously saved G-Code state"
    def cmd_RESTORE_GCODE_STATE(self, gcmd):
        state_name = gcmd.get('NAME', 'default')
        state = self.saved_states.get(state_name)
        if state is None:
            raise gcmd.error("Unknown g-code state: %s" % (state_name,))
        # Restore state
        self.absolute_coord = state['absolute_coord']
        self.absolute_extrude = state['absolute_extrude']
        self.base_position = list(state['base_position'])
        self.homing_position = list(state['homing_position'])
        self.speed = state['speed']
        self.speed_factor = state['speed_factor']
        self.extrude_factor = state['extrude_factor']
        # Restore the relative E position
        e_diff = self.last_position[3] - state['last_position'][3]
        self.base_position[3] += e_diff
        # Move the toolhead back if requested
        if gcmd.get_int('MOVE', 0):
            speed = gcmd.get_float('MOVE_SPEED', self.speed, above=0.)
            self.last_position[:3] = state['last_position'][:3]
            self.move_with_transform(self.last_position, speed)
    def cmd_GET_POSITION(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead', None)
        if toolhead is None:
            raise gcmd.error("Printer not ready")
        kin = toolhead.get_kinematics()
        steppers = kin.get_steppers()
        mcu_pos = " ".join(["%s:%d" % (s.get_name(), s.get_mcu_position())
                            for s in steppers])
        for s in steppers:
            s.set_tag_position(s.get_commanded_position())
        stepper_pos = " ".join(["%s:%.6f" % (s.get_name(), s.get_tag_position())
                                for s in steppers])
        kin_pos = " ".join(["%s:%.6f" % (a, v)
                            for a, v in zip("XYZ", kin.calc_tag_position())])
        toolhead_pos = " ".join(["%s:%.6f" % (a, v) for a, v in zip(
            "XYZE", toolhead.get_position())])
        gcode_pos = " ".join(["%s:%.6f"  % (a, v)
                              for a, v in zip("XYZE", self.last_position)])
        base_pos = " ".join(["%s:%.6f"  % (a, v)
                             for a, v in zip("XYZE", self.base_position)])
        homing_pos = " ".join(["%s:%.6f"  % (a, v)
                               for a, v in zip("XYZ", self.homing_position)])
        gcmd.respond_info("mcu: %s\n"
                          "stepper: %s\n"
                          "kinematic: %s\n"
                          "toolhead: %s\n"
                          "gcode: %s\n"
                          "gcode base: %s\n"
                          "gcode homing: %s"
                          % (mcu_pos, stepper_pos, kin_pos, toolhead_pos,
                             gcode_pos, base_pos, homing_pos))

def load_config(config):
    return GCodeMove(config)
