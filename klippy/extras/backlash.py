# Backlash compensation.
#
# Copyright (C) 2025 Oleksii Shchetinin <willkaxxx@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.


class BacklashCompensation:
    """Base class for backlash compensation."""

    def __init__(self, config):
        # System objects
        self.printer = config.get_printer()
        self.toolhead = None
        self.root_transform = None
        self.gcode_move = None
        self.motion_report = None

        # Reporting tool for UI (hide compensation)
        self.toolhead_status = None
        self.motion_report_status = None

        # Z axis backlash (common for all kinematics)
        self.backlash_z = config.getfloat('z', 0.0)
        self.last_dir_z = 0

        # Events handling registration
        self.printer.register_event_handler("klippy:connect", self._on_connect)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self._on_homing_move_end)

    def _on_connect(self):
        # System objects assignment
        self.toolhead = self.printer.lookup_object("toolhead")
        self.gcode_move = self.printer.lookup_object('gcode_move')
        self.motion_report = self.printer.lookup_object("motion_report")

        # Updates UI display section
        self.toolhead_status = self.toolhead.get_status
        self.toolhead.get_status = self._toolhead_status_interceptor
        self.motion_report_status = self.motion_report.get_status
        self.motion_report.get_status = self._motion_report_status_interceptor

        # Movement transform registration (calls original)
        self.root_transform = self.gcode_move.set_move_transform(
            self, force=True)

    def _on_homing_move_end(self, homing_state, rails):
        axes = homing_state.get_axes()
        self._reset_xy_directions(axes)
        if 2 in axes:
            self.last_dir_z = -1

    def _reset_xy_directions(self, axes):
        """Reset X/Y direction tracking after homing. Override in subclass."""
        pass

    def _get_xy_offset(self):
        """Get current X/Y offset. Override in subclass."""
        return 0.0, 0.0

    def _update_xy_directions(self, start_x, start_y, end_x, end_y):
        """Update X/Y direction tracking. Override in subclass."""
        pass

    def _get_current_offset(self):
        """Calculate current XYZ offset based on direction state."""
        offset_x, offset_y = self._get_xy_offset()
        offset_z = self.backlash_z if self.last_dir_z == 1 else 0.0
        return offset_x, offset_y, offset_z

    def get_position(self):
        # Return logical position (without backlash offset applied)
        pos = list(self.root_transform.get_position())
        offset_x, offset_y, offset_z = self._get_current_offset()

        if offset_x != 0.0 or offset_y != 0.0 or offset_z != 0.0:
            pos[0] -= offset_x
            pos[1] -= offset_y
            pos[2] -= offset_z
        return pos

    def move(self, newpos, speed) -> None:
        # Get current logical position
        cur_pos = self.get_position()
        start_x, start_y, start_z = cur_pos[0], cur_pos[1], cur_pos[2]
        end_x, end_y, end_z = newpos[0], newpos[1], newpos[2]
        dz = end_z - start_z

        # Update X/Y direction states (subclass specific)
        self._update_xy_directions(start_x, start_y, end_x, end_y)

        # Update Z direction state
        if dz != 0.0:
            dir_now = 1 if dz > 0 else -1
            if self.last_dir_z != 0:
                self.last_dir_z = dir_now

        # Calculate and apply offsets
        offset_x, offset_y, offset_z = self._get_current_offset()

        adjusted_newpos = list(newpos)
        adjusted_newpos[0] += offset_x
        adjusted_newpos[1] += offset_y
        adjusted_newpos[2] += offset_z

        self.root_transform.move(adjusted_newpos, speed)

    def _toolhead_status_interceptor(self, eventtime):
        # Updates static position display (hide compensation from UI)
        res = self.toolhead_status(eventtime)
        offset_x, offset_y, offset_z = self._get_current_offset()

        if offset_x != 0.0 or offset_y != 0.0 or offset_z != 0.0:
            commanded_pos = list(self.toolhead.commanded_pos[:4])
            commanded_pos[0] -= offset_x
            commanded_pos[1] -= offset_y
            commanded_pos[2] -= offset_z
            res['position'] = self.toolhead.Coord(*commanded_pos)
        return res

    def _motion_report_status_interceptor(self, eventtime):
        # Updates real-time motion tracking (hide compensation from UI)
        res = self.motion_report_status(eventtime)
        offset_x, offset_y, offset_z = self._get_current_offset()

        if offset_x != 0.0 or offset_y != 0.0 or offset_z != 0.0:
            mutated_coord = list(res['live_position'])
            mutated_coord[0] -= offset_x
            mutated_coord[1] -= offset_y
            mutated_coord[2] -= offset_z
            res['live_position'] = self.toolhead.Coord(*mutated_coord)
        return res


class CartesianBacklash(BacklashCompensation):
    """Backlash compensation for Cartesian kinematics (independent X, Y, Z)."""

    def __init__(self, config):
        super().__init__(config)
        # Backlash values per axis
        self.backlash_x = config.getfloat('x', 0.0)
        self.backlash_y = config.getfloat('y', 0.0)

        # Direction tracking: 0=unknown, 1=positive, -1=negative
        self.last_dir_x = 0
        self.last_dir_y = 0

    def _reset_xy_directions(self, axes):
        if 0 in axes:
            self.last_dir_x = -1
        if 1 in axes:
            self.last_dir_y = -1

    def _get_xy_offset(self):
        offset_x = self.backlash_x if self.last_dir_x == 1 else 0.0
        offset_y = self.backlash_y if self.last_dir_y == 1 else 0.0
        return offset_x, offset_y

    def _update_xy_directions(self, start_x, start_y, end_x, end_y):
        dx = end_x - start_x
        dy = end_y - start_y

        if dx != 0.0:
            dir_now = 1 if dx > 0 else -1
            if self.last_dir_x != 0:
                self.last_dir_x = dir_now

        if dy != 0.0:
            dir_now = 1 if dy > 0 else -1
            if self.last_dir_y != 0:
                self.last_dir_y = dir_now


class CoreXYBacklash(BacklashCompensation):
    """Backlash compensation for CoreXY kinematics (A/B diagonal motors)."""

    def __init__(self, config):
        super().__init__(config)
        # Backlash values per motor
        # A = X + Y, B = X - Y
        self.backlash_a = config.getfloat('a', 0.0)
        self.backlash_b = config.getfloat('b', 0.0)

        # Direction tracking: 0=unknown, 1=positive, -1=negative
        self.last_dir_a = 0
        self.last_dir_b = 0

    def _xy_to_ab(self, x, y):
        """Convert XY coordinates to AB motor positions."""
        a = x + y
        b = x - y
        return a, b

    def _reset_xy_directions(self, axes):
        # X or Y homing affects both A and B motors in CoreXY
        if 0 in axes or 1 in axes:
            self.last_dir_a = -1
            self.last_dir_b = -1

    def _get_xy_offset(self):
        # Offset in AB space
        offset_a = self.backlash_a if self.last_dir_a == 1 else 0.0
        offset_b = self.backlash_b if self.last_dir_b == 1 else 0.0
        # Convert to XY offset
        offset_x = (offset_a + offset_b) / 2.0
        offset_y = (offset_a - offset_b) / 2.0
        return offset_x, offset_y

    def _update_xy_directions(self, start_x, start_y, end_x, end_y):
        # Convert to AB space
        start_a, start_b = self._xy_to_ab(start_x, start_y)
        end_a, end_b = self._xy_to_ab(end_x, end_y)

        da = end_a - start_a
        db = end_b - start_b

        if da != 0.0:
            dir_now = 1 if da > 0 else -1
            if self.last_dir_a != 0:
                self.last_dir_a = dir_now

        if db != 0.0:
            dir_now = 1 if db > 0 else -1
            if self.last_dir_b != 0:
                self.last_dir_b = dir_now


def load_config(config):
    kinematics = config.get('kinematics', 'cartesian')
    if kinematics == 'cartesian':
        return CartesianBacklash(config)
    elif kinematics == 'corexy':
        return CoreXYBacklash(config)
    else:
        raise config.error("Unknown backlash kinematics '%s'. "
                           "Must be 'cartesian' or 'corexy'." % (kinematics,))
