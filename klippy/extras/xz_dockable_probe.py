# Support for probes that dock on the XZ plane, like on bedslingers
#
# Copyright (C) 2022  Lasse Dalegaard <dalegaard@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# Place in `klippy/extras/` folder as `xz_dockable_probe.py`, then add
# the following line to your `.git/info/exclude` file:
# klippy/extras/xz_dockable_probe.py

import logging
from .homing import HomingMove

class DockableProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
            self.handle_connect)

        ppins = self.printer.lookup_object('pins')
        pin = config.get('dock_pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        mcu = pin_params['chip']
        mcu.register_config_callback(self._build_config)
        self.mcu_endstop = mcu.setup_pin('endstop', pin_params)
        query_endstops = self.printer.load_object(config, 'query_endstops')
        query_endstops.register_endstop(self.mcu_endstop, 'probe_dock')
        self.printer.lookup_object('pins').register_chip('probe_dock', self)

        self.printer.load_object(config, 'homing')

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('PROBE_ATTACH', self.cmd_PROBE_ATTACH,
                                    desc=self.cmd_PROBE_ATTACH_help)
        self.gcode.register_command('PROBE_DETACH', self.cmd_PROBE_DETACH,
                                    desc=self.cmd_PROBE_DETACH_help)
        self.gcode.register_command('PROBE_LOCK', self.cmd_PROBE_LOCK,
                                    desc=self.cmd_PROBE_LOCK_help)
        self.gcode.register_command('PROBE_UNLOCK', self.cmd_PROBE_UNLOCK,
                                    desc=self.cmd_PROBE_UNLOCK_help)
        self.gcode.register_command('PROBE_STATE', self.cmd_PROBE_STATE,
                                    desc=self.cmd_PROBE_STATE_help)
        self.gcode.register_command('G28', None)
        self.gcode.register_command('G28', self.cmd_G28,
                                    desc=self.cmd_G28_help)

        self.dock_x = config.getint('dock_x')
        self.park_dx = config.getint('park_delta_x')
        self.detach_dx = config.getint('detach_delta_x')
        self.z_hop = config.getint('z_hop', minval=0)
        self.attachment_check_hop = config.get('attachment_check_hop', 5)
        self.z_speed = config.getint('z_speed', 30)
        self.xy_speed = config.getint('xy_speed', 70)
        self.hook_commands = config.getboolean('hook_commands', True)

        try:
            x_pos, y_pos = config.get("home_xy_position").split(',')
            self.home_x_pos, self.home_y_pos = float(x_pos), float(y_pos)
        except:
            raise config.error("Unable to parse home_xy_position in %s"
                               % (config.get_name(),))

        self.locked = False

    def handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        pin_params = {
            'pin': 'z_virtual_endstop',
            'invert': False,
            'pullup': False,
        }
        self.probe = self.printer.lookup_object('probe').setup_pin('endstop',
                                                                   pin_params)
        self.phoming = self.printer.lookup_object('homing')

        # Hook all relevant gcode commands. Saves user having to make macros
        # for these.
        if self.hook_commands:
            self._hook_gcode("PROBE", True)
            self._hook_gcode("PROBE_CALIBRATE", True)
            self._hook_gcode("PROBE_ACCURACY", True)
            self._hook_gcode("BED_MESH_CALIBRATE")
            self._hook_gcode("SCREWS_TILT_CALCULATE")
            self._hook_gcode("BED_TILT_CALIBRATE")
            self._hook_gcode("Z_TILT_ADJUST")

    def _build_config(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self.mcu_endstop.add_stepper(stepper)

    def _hook_gcode(self, cmd, restore_pos=False):
        desc = self.gcode.gcode_help.get(cmd)
        prev = self.gcode.register_command(cmd, None)
        if prev is None:
            return
        def handler(gcmd):
            attached = False
            pos = None
            if self._attach_state(may_hop=True) != "attached":
                attached = True
                self._check_homed_xz()
                if restore_pos:
                    pos = self.toolhead.get_position()
                self._do_z_hop(self.z_hop)
                self._do_attach()
                if restore_pos:
                    self.toolhead.move(pos, self.xy_speed)
            prev(gcmd)
            if attached and not self.locked:
                self._do_z_hop(self.z_hop)
                self._do_detach()
                if restore_pos:
                    self.toolhead.move(pos, self.xy_speed)
        self.gcode.register_command(cmd, handler, desc=desc)

    cmd_G28_help = "Homes the printer"
    def cmd_G28(self, gcmd):
        # Check which axes we want, all if none given
        want_x, want_y, want_z = [gcmd.get(axis, None) is not None
                for axis in "XYZ"]
        if not want_x and not want_y and not want_z:
            want_x = want_y = want_z = True

        if not (want_x or want_y or want_z):
            # No axes wanted, ignore this
            return

        self._do_z_hop(self.z_hop)

        # Home X and Y first
        if want_x or want_y:
            hcmd = self.gcode.create_gcode_command("G28", "G28",
                                                   {'X': want_x, 'Y': want_y})
            self.phoming.cmd_G28(hcmd)

        if want_z:
            self._check_homed_xz()
            attached = False
            if self._attach_state(after_hop=True) != "attached":
                attached = True
                self._do_attach()
            # Do the actual Z homing with the probe now attached
            self.toolhead.manual_move([self.home_x_pos, self.home_y_pos],
                                      self.xy_speed)
            hcmd = self.gcode.create_gcode_command("G28", "G28", {'Z': '0'})
            self.phoming.cmd_G28(hcmd)
            self._do_z_hop(self.z_hop)

            if gcmd.get("LOCK_PROBE", None) is not None:
                self.locked = True
            elif attached and not self.locked:
                self._do_detach()

    cmd_PROBE_ATTACH_help = "Attaches the dockable probe to the toolhead"
    def cmd_PROBE_ATTACH(self, gcmd):
        if self._attach_state(may_hop=True) == "attached":
            return

        self._check_homed_xz()
        self._do_z_hop(self.z_hop)
        self._do_attach()

        if gcmd.get("LOCK", None) is not None:
            self.locked = True

    cmd_PROBE_DETACH_help = "Detaches the dockable probe to the toolhead"
    def cmd_PROBE_DETACH(self, gcmd):
        if self.locked and gcmd.get("FORCE", None) is None:
            return
        if self._attach_state(may_hop=True) == "detached":
            return

        self._check_homed_xz()
        self._do_z_hop(self.z_hop)
        self._do_detach()
        self.locked = False

    cmd_PROBE_LOCK_help = "Locks the probe, preventing any detaching operations"
    def cmd_PROBE_LOCK(self, gcmd):
        self.locked = True

    cmd_PROBE_UNLOCK_help = "Unlocks the probe, allowing detaching operations"
    def cmd_PROBE_UNLOCK(self, gcmd):
        self.locked = False

    cmd_PROBE_STATE_help = "Queries the docking state of the probe"
    def cmd_PROBE_STATE(self, gcmd):
        refresh = gcmd.get("REFRESH", None) is not None
        state = self._attach_state(may_hop=refresh)
        self.gcode.respond_info("Probe state is %s" % (state,))

    # Perform attaching operation, assuming we already Z hopped and XY homed
    def _do_attach(self):
        self._dock_plane(0)
        self._endstop_descend()
        self._dock_plane(self.park_dx) # Move out of dock
        self._do_z_hop(self.z_hop)
        self.toolhead.wait_moves()
        if self._is_probe_triggered():
            self.printer.invoke_shutdown("Probe was still triggered after "
                                         "attaching")
        self.toolhead.wait_moves()

    # Perform detaching operation, assuming we already Z hopped and XY homed
    def _do_detach(self):
        self._dock_plane(self.park_dx)
        self._endstop_descend()
        self._dock_plane(self.detach_dx) # Move across the dock
        self._do_z_hop(self.z_hop)
        self.toolhead.wait_moves()
        if not self._is_probe_triggered():
            self.printer.invoke_shutdown("Probe wasn't triggered after "
                                         "detaching")

    # Moves to a location relative to the docking X coordinate
    def _dock_plane(self, offset=0):
        kin_status = self.toolhead.get_kinematics().get_status(self._get_time())
        ymin = kin_status['axis_minimum'][1]
        self.toolhead.manual_move([self.dock_x+offset,ymin+1], self.xy_speed)

    # Descend on Z until the docking endstop is triggered
    def _endstop_descend(self):
        self.toolhead.get_last_move_time() # Synchronizes move time
        pos = self.toolhead.get_position()
        hmove = HomingMove(self.printer, [(self.mcu_endstop, "dock")])
        kin_status = self.toolhead.get_kinematics().get_status(self._get_time())
        pos[2] = kin_status['axis_minimum'][2]
        if self._is_homed('z'):
            hmove.homing_move(pos, self.z_speed, probe_pos=True)
        else:
            fakepos = self.toolhead.get_position()
            fakepos[2] = kin_status['axis_maximum'][2]
            self.toolhead.set_position(fakepos, homing_axes=(2,))
            hmove.homing_move(pos, self.z_speed)

    def _attach_state(self, after_hop=False, may_hop=False):
        if self._is_probe_triggered():
            # Triggered means we are either touching the plate or the probe
            # is not attached. To figure out which it is, we need to move
            # the toolhead up a little bit. If we already did a hop, no
            # need to do another of course!
            if not after_hop:
                if not may_hop:
                    msg = ("Can't determine state of docked probe without"
                          " moving toolhead")
                    raise self.printer.command_error(msg)
                self._do_z_hop(-self.attachment_check_hop)

            # Read state of probe again
            if self._is_probe_triggered():
                # Probe was still triggered, so we are detached
                return "detached"
            else:
                # Probe isn't triggered anymore, so it was touching bed
                return "attached"
        else:
            # Probe isn't triggered which means it must be attached and
            # not currently pressed
            return "attached"

    def _is_probe_triggered(self):
        self.toolhead.wait_moves()
        move_time = self.toolhead.get_last_move_time()
        return self.probe.query_endstop(move_time)

    def _get_time(self):
        return self.printer.get_reactor().monotonic()

    def _is_homed(self, axis):
        toolhead = self.toolhead
        status = toolhead.get_kinematics().get_status(self._get_time())
        return axis in status['homed_axes']

    def _check_homed_xz(self, gcmd=None):
        if not self._is_homed('x') or not self._is_homed('y'):
            raise self.printer.command_error("Must home X and Y axes first")
    
    def _do_z_hop(self, amount):
        toolhead = self.toolhead
        pos = toolhead.get_position()
        if self._is_homed('z'):
            if amount < 0:
                amount = pos[2] - amount # Negative amount means relative
            toolhead.manual_move([None, None, amount], self.z_speed)
        else:
            toolhead.set_position(pos, homing_axes=[2])
            toolhead.manual_move([None, None, pos[2]+abs(amount)], self.z_speed)
            toolhead.get_kinematics().note_z_not_homed()

def load_config(config):
    return DockableProbe(config)