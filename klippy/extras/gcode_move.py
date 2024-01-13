# G-Code G1 movement commands (and associated coordinate manipulation)
#
# Copyright (C) 2016-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class GCodeMove:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        self.variable_safe_z = 0
        if config.has_section('gcode_macro PRINTER_PARAM'):
            PRINTER_PARAM = config.getsection('gcode_macro PRINTER_PARAM')
            try:
                self.variable_safe_z = PRINTER_PARAM.getfloat('variable_z_safe_g28')
            except:
                section = "bltouch"
                if config.has_section(section):
                    logging.info("with bltouch")
                    self.variable_safe_z = PRINTER_PARAM.getfloat('variable_z_safe_g28_touch', 0.0)
                else:
                    logging.info("no bltouch")
                    tmp = PRINTER_PARAM.getfloat('variable_z_safe_g28_no_touch', 0.0)
                    if tmp is not None:
                        self.variable_safe_z = tmp
        logging.info("self.variable_safe_z = %s" % self.variable_safe_z)
        printer.register_event_handler("klippy:ready", self._handle_ready)
        printer.register_event_handler("klippy:shutdown", self._handle_shutdown)
        printer.register_event_handler("toolhead:set_position",
                                       self.reset_last_position)
        printer.register_event_handler("toolhead:manual_move",
                                       self.reset_last_position)
        printer.register_event_handler("toolhead:update_extra_axes",
                                       self._update_extra_axes)
        printer.register_event_handler("gcode:command_error",
                                       self.reset_last_position)
        printer.register_event_handler("extruder:activate_extruder",
                                       self._handle_activate_extruder)
        printer.register_event_handler("homing:home_rails_end",
                                       self._handle_home_rails_end)
        self.is_printer_ready = False
        # Register g-code commands
        gcode = printer.lookup_object('gcode')
        handlers = [
            'G1', 'G20', 'G21',
            'M82', 'M83', 'G90', 'G91', 'G92', 'M220', 'M221',
            'SET_GCODE_OFFSET', 'SAVE_GCODE_STATE', 'RESTORE_GCODE_STATE',
        ]
        for cmd in handlers:
            func = getattr(self, 'cmd_' + cmd)
            desc = getattr(self, 'cmd_' + cmd + '_help', None)
            gcode.register_command(cmd, func, False, desc)
        gcode.register_command('G0', self.cmd_G1)
        gcode.register_command('M114', self.cmd_M114, True)
        gcode.register_command('GET_POSITION', self.cmd_GET_POSITION, True,
                               desc=self.cmd_GET_POSITION_help)
        gcode.register_command('SET_POSITION', self.cmd_SET_POSITION, True, desc=self.cmd_SET_POSITION_help)
        self.Coord = gcode.Coord
        # G-Code coordinate manipulation
        self.absolute_coord = self.absolute_extrude = True
        self.base_position = [0.0, 0.0, 0.0, 0.0]
        self.last_position = [0.0, 0.0, 0.0, 0.0]
        self.homing_position = [0.0, 0.0, 0.0, 0.0]
        self.axis_map = {'X':0, 'Y': 1, 'Z': 2, 'E': 3}
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
        self.reset_last_position()
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
    def _handle_home_rails_end(self, homing_state, rails):
        self.reset_last_position()
        for axis in homing_state.get_axes():
            self.base_position[axis] = self.homing_position[axis]
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
            'homing_origin': self.Coord(*self.homing_position[:4]),
            'position': self.Coord(*self.last_position[:4]),
            'gcode_position': self.Coord(*move_position[:4]),
        }
    def reset_last_position(self):
        if self.is_printer_ready:
            self.last_position = self.position_with_transform()
    def _update_extra_axes(self):
        toolhead = self.printer.lookup_object('toolhead')
        axis_map = {'X':0, 'Y': 1, 'Z': 2, 'E': 3}
        extra_axes = toolhead.get_extra_axes()
        for index, ea in enumerate(extra_axes):
            if ea is None:
                continue
            gcode_id = ea.get_axis_gcode_id()
            if gcode_id is None or gcode_id in axis_map or gcode_id in "FN":
                continue
            axis_map[gcode_id] = index
        self.axis_map = axis_map
        self.base_position[4:] = [0.] * (len(extra_axes) - 4)
        self.reset_last_position()
    # G-Code movement commands
    def cmd_G1(self, gcmd):
        # Move
        params = gcmd.get_command_parameters()
        try:
            for axis, pos in self.axis_map.items():
                if axis in params:
                    v = float(params[axis])
                    absolute_coord = self.absolute_coord
                    if axis == 'E':
                        v *= self.extrude_factor
                        if not self.absolute_extrude:
                            absolute_coord = False
                    if not absolute_coord:
                        # value relative to position of last move
                        self.last_position[pos] += v
                    else:
                        # value relative to base coordinate position
                        self.last_position[pos] = v + self.base_position[pos]
            if 'F' in params:
                gcode_speed = float(params['F'])
                if gcode_speed <= 0.:
                    raise gcmd.error("""{"code":"key272": "msg":"Invalid speed in '%s'", "values":["%s"]}"""
                                     % (gcmd.get_commandline(),gcmd.get_commandline()))
                self.speed = gcode_speed * self.speed_factor
        except ValueError as e:
            raise gcmd.error("""{"code":"key273": "msg":"Unable to parse move '%s'", "values":["%s"]}"""
                             % (gcmd.get_commandline(),gcmd.get_commandline()))
        self.move_with_transform(self.last_position, self.speed)
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
            self.base_position[:4] = self.last_position[:4]
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
    def recordPrintFileName(self, path, file_name, fan_state="", filament_used=0, last_print_duration=0, pressure_advance="", slow_print=False):
        import json, os
        fan = {}
        M204_accel = ""
        old_filament_used = 0
        old_last_print_duration = 0
        old_pressure_advance = ""
        last_speed_factor = 0.0166666
        last_speed = 25
        if os.path.exists(path):
            with open(path, "r") as f:
                result = (json.loads(f.read()))
                # fan = result.get("fan_state", "")
                fan = result.get("fan_state", {})
                M204_accel = result.get("M204", "")
                old_filament_used = result.get("filament_used", 0)
                old_last_print_duration = result.get("last_print_duration", 0)
                old_pressure_advance = result.get("pressure_advance", "")
                last_speed_factor = result.get("speed_factor", 0.0166666)
                last_speed = result.get("speed", 25)
        if fan_state.startswith("M106 S"):
            fan["M106 S"] = fan_state
        elif fan_state.startswith("M106 P0"):
            fan["M106 P0"] = fan_state
        elif fan_state.startswith("M106 P1"):
            fan["M106 P1"] = fan_state
        elif fan_state.startswith("M106 P2"):
            fan["M106 P2"] = fan_state
        # if fan_state:
        #     fan.append(fan_state)
        # if fan_state and fan_state != fan:
        #     state = fan_state
        # else:
        #     state = fan
        if filament_used and filament_used != old_filament_used:
            pass
        else:
            filament_used = old_filament_used
        if last_print_duration and last_print_duration != old_last_print_duration:
            pass
        else:
            last_print_duration = old_last_print_duration
        if pressure_advance and pressure_advance != old_pressure_advance:
            pass
        else:
            pressure_advance = old_pressure_advance
        data = {
            'file_path': file_name,
            'absolute_coord': self.absolute_coord,
            'absolute_extrude': self.absolute_extrude,
            'extrude_factor': self.extrude_factor,
            # 'fan_state': state,
            'speed_factor': last_speed_factor if slow_print else self.speed_factor,
            "speed": last_speed if slow_print else self.speed,
            'fan_state': fan,
            'M204': M204_accel,
            'filament_used': filament_used,
            'last_print_duration': last_print_duration,
            'pressure_advance': pressure_advance
        }
        with open(path, "w") as f:
            f.write(json.dumps(data))
            f.flush()
    cmd_CX_RESTORE_GCODE_STATE_help = "Restore a previously saved G-Code state"
    def cmd_CX_RESTORE_GCODE_STATE(self, print_info, file_name_path, XYZE):
        try:
            state = {
                "absolute_extrude": True,
                "file_position": 0,
                "extrude_factor": 1.0,
                "speed_factor": 0.0166666,
                "homing_position": [0.0, 0.0, 0.0, 0.0],
                "last_position": [0.0, 0.0, 0.0, 0.0],
                "speed": 25.0,
                "file_path": "",
                "base_position": [0.0, 0.0, 0.0, -0.0],
                "absolute_coord": True,
                # "fan_state": "",
                "fan_state": {},
                "variable_z_safe_pause": 0,
                "M204": "",
                "filament_used": 0,
                "last_print_duration": 0,
                "pressure_advance": "",
                "z_toolhead_moved": 0
            }
            import os, json
            base_position_e = -1
            state["file_position"] = print_info.get("file_position", 0)
            state["base_position"] = [0.0, 0.0, 0.0, print_info.get("base_position_e", -1)]
            base_position_e = print_info.get("base_position_e", -1)
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE base_position_e:%s" % base_position_e)
            with open(file_name_path, "r") as f:
                file_info = json.loads(f.read())
                state["file_path"] = file_info.get("file_path", "")
                state["absolute_extrude"] = file_info.get("absolute_extrude", True)
                state["absolute_coord"] = file_info.get("absolute_coord", True)
                state["fan_state"] = file_info.get("fan_state", {})
                state["variable_z_safe_pause"] = file_info.get("variable_z_safe_pause", 0)
                state["M204"] = file_info.get("M204", "")
                state["speed_factor"] = file_info.get("speed_factor", 0.016666666)
                state["extrude_factor"] = file_info.get("extrude_factor", 1.0)
                state["speed"] = file_info.get("speed", 25)
                state["pressure_advance"] = file_info.get("pressure_advance", "")
                state["z_toolhead_moved"] = file_info.get("z_toolhead_moved", 0)
            state["last_position"] = [XYZE["X"], XYZE["Y"], XYZE["Z"], XYZE["E"]+base_position_e]
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE state:%s" % str(state))

            # Restore state
            self.absolute_coord = state['absolute_coord']
            # self.absolute_extrude = state['absolute_extrude']
            self.base_position = list(state['base_position'])
            self.homing_position = list(state['homing_position'])
            self.speed = state['speed']
            self.speed_factor = state['speed_factor']
            # self.extrude_factor = state['extrude_factor']
            self.extrude_factor = 1.0
            # Restore the relative E position
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE base_position:%s" % str(self.base_position))
            e_diff = self.last_position[3] - state['last_position'][3] + 1.0
            self.base_position[3] += e_diff
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE self.last_position[3]:%s, state['last_position'][3]:%s, e_diff:%s, \
                         base_position[3]:%s" % (self.last_position[3], state['last_position'][3], e_diff, self.base_position[3]))
            # Move the toolhead back if requested
            gcode = self.printer.lookup_object('gcode')
            if state["fan_state"]:
                logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE fan fan_state:%s" % str(state["fan_state"]))
                for key in state["fan_state"]:
                    logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE fan set fan:%s#" % str(state["fan_state"].get(key, "")))
                    gcode.run_script_from_command(state["fan_state"].get(key, ""))
                # gcode.run_script_from_command(state["fan_state"])
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE before G28 X Y self.last_position:%s" % str(self.last_position))
            gcode.run_script_from_command("SOFT_CHECK_ERROR FLAG=1")
            gcode.run_script_from_command("G28 X Y")
            gcode.run_script_from_command("SOFT_CHECK_ERROR FLAG=0")
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE after G28 X Y self.last_position:%s" % str(self.last_position))
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE before BED_MESH_PROFILE LOAD='default'")
            gcode.run_script_from_command('BED_MESH_PROFILE LOAD="default"')
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE after BED_MESH_PROFILE LOAD='default'")
            x = self.last_position[0]
            y = self.last_position[1]
            z = state['last_position'][2] + self.variable_safe_z + state["variable_z_safe_pause"] + state["z_toolhead_moved"]
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE self.last_position[2]:%s, state['last_position'][2]:%s, self.variable_safe_z:%s, \
                state['variable_z_safe_pause']:%s" % (self.last_position[2], state['last_position'][2], self.variable_safe_z, state["variable_z_safe_pause"]))
            toolhead = self.printer.lookup_object("toolhead")
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE toolhead.set_position:%s" % str([x, y, z, self.last_position[3]]))
            toolhead.set_position([x, y, z, self.last_position[3]], homing_axes=(2,))
            speed = self.speed
            self.last_position[:3] = state['last_position'][:3]
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE G1 X%s Y%s F2400" % (state['last_position'][0], state['last_position'][1]))
            gcode.run_script_from_command("G1 X%s Y%s F2400" % (state['last_position'][0], state['last_position'][1]))
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE move_with_transform:%s, speed:%s" % (self.last_position, speed))
            self.move_with_transform(self.last_position, speed)
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE G1 X%s Y%s F3000" % (state['last_position'][0], state['last_position'][1]))
            gcode.run_script_from_command("G1 X%s Y%s F3000" % (state['last_position'][0], state['last_position'][1]))
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE M400")
            gcode.run_script_from_command("G1 X%s Y%s F%s" % (state['last_position'][0], state['last_position'][1], int(state['speed']/state['speed_factor'])))
            logging.info("power_loss RESTORE F%s" % (int(state['speed']/state['speed_factor'])))
            gcode.run_script_from_command("M400")
            if state["M204"]:
                logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE SET M204:%s#" % state["M204"])
                gcode.run_script_from_command(state["M204"])
            if state["pressure_advance"]:
                gcode.run_script_from_command(state["pressure_advance"])
            self.absolute_extrude = state['absolute_extrude']
            gcode.run_script_from_command("M221 S%s" % int(state['extrude_factor']*100))
            try:
                if os.path.exists(gcode.exclude_object_info):
                    with open(gcode.exclude_object_info, "r") as f:
                        exclude_object_cmds = json.loads(f.read())
                        EXCLUDE_OBJECT_DEFINE = exclude_object_cmds.get("EXCLUDE_OBJECT_DEFINE", [])
                        EXCLUDE_OBJECT = exclude_object_cmds.get("EXCLUDE_OBJECT", [])
                        for line in EXCLUDE_OBJECT_DEFINE:
                            gcode.run_script_from_command(line)
                        for line in EXCLUDE_OBJECT:
                            gcode.run_script_from_command(line)
                        gcode.run_script_from_command("M400")
            except Exception as err:
                logging.exception("RESTORE EXCLUDE_OBJECT err:%s" % err)
            logging.info("power_loss cmd_CX_RESTORE_GCODE_STATE done")
        except Exception as err:
            logging.exception("cmd_CX_RESTORE_GCODE_STATE err:%s" % err)
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
            raise gcmd.error("""{"code":"key274", "msg": "Unknown g-code state: %s", "values":["%s"]}""" % (state_name, state_name))
        # Restore state
        self.absolute_coord = state['absolute_coord']
        self.absolute_extrude = state['absolute_extrude']
        self.base_position[:4] = state['base_position'][:4]
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
    cmd_GET_POSITION_help = (
        "Return information on the current location of the toolhead")
    def cmd_GET_POSITION(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead', None)
        if toolhead is None:
            raise gcmd.error("""{"code": "key283", "msg": ""Printer not ready"}""")
        kin = toolhead.get_kinematics()
        steppers = kin.get_steppers()
        mcu_pos = " ".join(["%s:%d" % (s.get_name(), s.get_mcu_position())
                            for s in steppers])
        cinfo = [(s.get_name(), s.get_commanded_position()) for s in steppers]
        stepper_pos = " ".join(["%s:%.6f" % (a, v) for a, v in cinfo])
        kinfo = zip("XYZ", kin.calc_position(dict(cinfo)))
        kin_pos = " ".join(["%s:%.6f" % (a, v) for a, v in kinfo])
        toolhead_pos = " ".join(["%s:%.6f" % (a, v) for a, v in zip(
            "XYZE", toolhead.get_position()[:4])])
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
    cmd_SET_POSITION_help = (
        "SET_POSITION information on the current location of the toolhead")
    def cmd_SET_POSITION(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead', None)
        if toolhead is None:
            raise gcmd.error("""{"code": "key283", "msg": ""Printer not ready"}""")
        position = toolhead.get_position()
        x = position[0]
        y = position[1]
        z = position[2]
        e = position[3]
        X = gcmd.get_float('X', x)
        Y = gcmd.get_float('Y', y)
        Z = gcmd.get_float('Z', z)
        E = gcmd.get_float('E', e)
        toolhead.set_position([X, Y, Z, E], homing_axes=(2,))
        position = toolhead.get_position()
        msg = "toolhead get_position X:%s, Y:%s, Z:%s, E:%s" % (position[0], position[1], position[2], position[3])
        gcmd.respond_info(msg)
def load_config(config):
    return GCodeMove(config)
