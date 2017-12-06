# Z-Probe support
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import homing

class PrinterProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.speed = config.getfloat('speed', 5.0)
        self.z_position = config.getfloat('z_position', 0.)
        ppins = self.printer.lookup_object('pins')
        pin_params = ppins.lookup_pin('endstop', config.get('pin'))
        mcu = pin_params['chip']
        mcu.add_config_object(self)
        self.mcu_probe = mcu.setup_pin(pin_params)
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'PROBE', self.cmd_PROBE, desc=self.cmd_PROBE_help)
        self.gcode.register_command(
            'QUERY_PROBE', self.cmd_QUERY_PROBE, desc=self.cmd_QUERY_PROBE_help)
    def build_config(self):
        toolhead = self.printer.lookup_object('toolhead')
        z_steppers = toolhead.get_kinematics().get_steppers("Z")
        for s in z_steppers:
            for mcu_endstop, name in s.get_endstops():
                for mcu_stepper in mcu_endstop.get_steppers():
                    self.mcu_probe.add_stepper(mcu_stepper)
    cmd_PROBE_help = "Probe Z-height at current XY position"
    def cmd_PROBE(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        homing_state = homing.Homing(toolhead)
        pos = toolhead.get_position()
        pos[2] = self.z_position
        try:
            homing_state.homing_move(
                pos, [(self.mcu_probe, "probe")], self.speed, probe_pos=True)
        except homing.EndstopError as e:
            raise self.gcode.error(str(e))
        self.gcode.reset_last_position()
    cmd_QUERY_PROBE_help = "Return the status of the z-probe"
    def cmd_QUERY_PROBE(self, params):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        self.mcu_probe.query_endstop(print_time)
        res = self.mcu_probe.query_endstop_wait()
        self.gcode.respond_info(
            "probe: %s" % (["open", "TRIGGERED"][not not res],))

def load_config(config):
    if config.get_name() != 'probe':
        raise config.error("Invalid probe config name")
    return PrinterProbe(config)
