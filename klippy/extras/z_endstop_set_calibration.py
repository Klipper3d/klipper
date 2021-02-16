# Allow self-calibration of Z endstop using a Z probe.
#
# This file may be distributed under the terms of the GNU GPLv3 license.


class ZEndstopSetCalibration:
    def __init__(self, config):
        self.printer = config.get_printer()
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('Z_ENDSTOP_SET_CALIBRATION',
                               self.cmd_Z_ENDSTOP_SET_CALIBRATION,
                               desc=self.cmd_Z_ENDSTOP_SET_CALIBRATION_help)

        zconfig = config.getsection('stepper_z')
        self.z_position_endstop = zconfig.getfloat('position_endstop', None,
                                                   note_valid=False)
        if self.z_position_endstop is None:
            raise gcode.error("Cannot find position_endstop in [stepper_z]")


    cmd_Z_ENDSTOP_SET_CALIBRATION_help = "Self-calibrate Z endstop after "     \
          "running PROBE command."


    def cmd_Z_ENDSTOP_SET_CALIBRATION(self, gcmd):
        new_z = gcmd.get_float("Z")
        tool = self.printer.lookup_object('toolhead')
        pos = tool.get_position()
        z_pos = self.z_position_endstop - pos[2] + new_z
        gcmd.respond_info(
            "stepper_z: position_endstop: %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer." % (z_pos,))
        configfile = self.printer.lookup_object('configfile')
        configfile.set('stepper_z', 'position_endstop', "%.3f" % z_pos)


def load_config(config):
    return ZEndstopSetCalibration(config)
