PROBE_REST_TIME = .001

class CncProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.probes = []

        probe_pin = config.get('probe_pin')
        ppins = self.printer.lookup_object('pins')

        self.mcu_multiaxis_probe = ppins.setup_pin('multiaxis_probe', probe_pin)

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G38.2", self.cmd_G38_2)
        self.gcode.register_command("G38.3", self.cmd_G38_3)
        self.gcode.register_command("G38.4", self.cmd_G38_4)
        self.gcode.register_command("G38.5", self.cmd_G38_5)

        self.speed = 25.
        self.speed_factor = 1. / 60.
        return

    def _lookup_toolhead_pos(self, pos_time):
        toolhead = self.printer.lookup_object('toolhead')
        kin = toolhead.get_kinematics()
        kin_spos = {s.get_name():
                    s.mcu_to_commanded_position(
                        s.get_past_mcu_position(pos_time))
                     for s in kin.get_steppers()}
        return kin.calc_position(kin_spos)

    def probe_end_move_condition_func(self):
        return self.mcu_multiaxis_probe.query_probe()[2]

    def probe_move(self, movepos, speed, stop_on_contact=True):
        toolhead = self.printer.lookup_object('toolhead')
        x, y, z, e = movepos

        # Enable probe
        print_time = toolhead.get_last_move_time()
        self.mcu_multiaxis_probe.probe_start(print_time,
                                             PROBE_REST_TIME, stop_on_contact)

        # Move to desire position
        toolhead.move([x, y, z, e], speed)

        # Wait for move to finish
        toolhead.wait_moves(self.probe_end_move_condition_func)

        # Check if probe triggered during move
        probe_query = self.mcu_multiaxis_probe.query_probe()
        ret = probe_query[2]
        trigger_time = probe_query[0]

        # Clear probe
        self.mcu_multiaxis_probe.probe_clear()

        # Set correct position
        if trigger_time != 0:
            new_pos = self._lookup_toolhead_pos(trigger_time)
            new_pos.append(e)
            toolhead.set_position(new_pos)

        # Check if probe was triggered
        return ret

    def cmd_G38_2(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.get_last_move_time()
        curpos = toolhead.get_position()

        x = gcmd.get_float('X', curpos[0])
        y = gcmd.get_float('Y', curpos[1])
        z = gcmd.get_float('Z', curpos[2])
        e = curpos[3]
        gcode_speed = gcmd.get_float('F', self.speed)

        target_pos = [x, y, z, e]

        ret = self.probe_move(target_pos, gcode_speed)
        if ret == 0:
            gcmd.respond_info("Error : Probe not triggered")
        return

    def cmd_G38_3(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.get_last_move_time()
        curpos = toolhead.get_position()

        x = gcmd.get_float('X', curpos[0])
        y = gcmd.get_float('Y', curpos[1])
        z = gcmd.get_float('Z', curpos[2])
        e = curpos[3]
        gcode_speed = gcmd.get_float('F', self.speed)

        target_pos = [x, y, z, e]

        self.probe_move(target_pos, gcode_speed)
        return

    def cmd_G38_4(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.get_last_move_time()
        curpos = toolhead.get_position()

        x = gcmd.get_float('X', curpos[0])
        y = gcmd.get_float('Y', curpos[1])
        z = gcmd.get_float('Z', curpos[2])
        e = curpos[3]
        gcode_speed = gcmd.get_float('F', self.speed)

        target_pos = [x, y, z, e]

        ret = self.probe_move(target_pos, gcode_speed, False)
        if ret == 0:
            gcmd.respond_info("Error : Probe not triggered")
        return

    def cmd_G38_5(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.get_last_move_time()
        curpos = toolhead.get_position()

        x = gcmd.get_float('X', curpos[0])
        y = gcmd.get_float('Y', curpos[1])
        z = gcmd.get_float('Z', curpos[2])
        e = curpos[3]
        gcode_speed = gcmd.get_float('F', self.speed)

        target_pos = [x, y, z, e]

        self.probe_move(target_pos, gcode_speed, False)
        return

def load_config(config):
    return CncProbe(config)
