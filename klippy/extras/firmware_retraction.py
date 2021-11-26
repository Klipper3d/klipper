# Support for Marlin/Smoothie/Reprap style firmware retraction via G10/G11
#
# Copyright (C) 2019  Len Trigg <lenbok@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class FirmwareRetraction:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.retract_length = config.getfloat('retract_length', 0., minval=0.)
        self.retract_speed = config.getfloat('retract_speed', 20., minval=1)
        self.unretract_extra_length = config.getfloat(
            'unretract_extra_length', 0., minval=0.)
        self.unretract_speed = config.getfloat('unretract_speed', 10., minval=1)
        self.unretract_length = (self.retract_length
                                 + self.unretract_extra_length)
        self.lift_z = config.getfloat('lift_z', 0., minval=0.)
        self.current_lift_z = 0.
        self.is_retracted = False
        self.prev_G0 = None
        self.prev_G1 = None
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('SET_RETRACTION', self.cmd_SET_RETRACTION,
                                    desc=self.cmd_SET_RETRACTION_help)
        self.gcode.register_command('GET_RETRACTION', self.cmd_GET_RETRACTION,
                                    desc=self.cmd_GET_RETRACTION_help)
        self.gcode.register_command('G10', self.cmd_G10)
        self.gcode.register_command('G11', self.cmd_G11)
    def get_status(self, eventtime):
        return {
            "retract_length": self.retract_length,
            "retract_speed": self.retract_speed,
            "unretract_extra_length": self.unretract_extra_length,
            "unretract_speed": self.unretract_speed,
            "lift_z": self.lift_z,
        }

    def _override_gcode_move(self, undo=False):
        if not undo and not self.prev_G0 and not self.prev_G1:
            self.prev_G0 = self.gcode.register_command('G0', None)
            self.gcode.register_command('G0', self.cmd_G0)
            self.prev_G1 = self.gcode.register_command('G1', None)
            self.gcode.register_command('G1', self.cmd_G1)
        elif self.prev_G0 and self.prev_G1:
            self.gcode.register_command('G0', None)
            self.gcode.register_command('G0', self.prev_G0)
            self.gcode.register_command('G1', None)
            self.gcode.register_command('G1', self.prev_G1)
            self.prev_G0 = self.prev_G1 = None
    def _check_z_lift(self, gcmd):
        params = gcmd.get_command_parameters()
        if ("Z" in params and float(params["Z"]) != 0. and self.is_retracted and
                self.current_lift_z > 0.):
            self.current_lift_z = 0.
            self._override_gcode_move(undo=True)

    cmd_SET_RETRACTION_help = ("Set firmware retraction parameters")
    def cmd_SET_RETRACTION(self, gcmd):
        self.retract_length = gcmd.get_float('RETRACT_LENGTH',
                                             self.retract_length, minval=0.)
        self.retract_speed = gcmd.get_float('RETRACT_SPEED',
                                            self.retract_speed, minval=1)
        self.unretract_extra_length = gcmd.get_float(
            'UNRETRACT_EXTRA_LENGTH', self.unretract_extra_length, minval=0.)
        self.unretract_speed = gcmd.get_float('UNRETRACT_SPEED',
                                              self.unretract_speed, minval=1)
        self.unretract_length = (self.retract_length
                                 + self.unretract_extra_length)
        self.lift_z = gcmd.get_float('LIFT_Z', self.lift_z, minval=0.)
        self.is_retracted = False
        self.current_lift_z = 0.
        self._override_gcode_move(undo=True)
    cmd_GET_RETRACTION_help = ("Report firmware retraction paramters")
    def cmd_GET_RETRACTION(self, gcmd):
        gcmd.respond_info("RETRACT_LENGTH=%.5f RETRACT_SPEED=%.5f"
                          " UNRETRACT_EXTRA_LENGTH=%.5f UNRETRACT_SPEED=%.5f"
                          " LIFT_Z=%.5f"
                          % (self.retract_length, self.retract_speed,
                             self.unretract_extra_length, self.unretract_speed,
                             self.lift_z))

    def cmd_G0(self, gcmd):
        self.prev_G0(gcmd)
        self._check_z_lift(gcmd)
    def cmd_G1(self, gcmd):
        self.prev_G1(gcmd)
        self._check_z_lift(gcmd)

    def cmd_G10(self, gcmd):
        if not self.is_retracted:
            self.gcode.run_script_from_command(
                "SAVE_GCODE_STATE NAME=_retract_state\n"
                "G91\n"
                "G1 E-%.5f F%d\n"
                "G1 Z%.5f\n"
                "RESTORE_GCODE_STATE NAME=_retract_state"
                % (self.retract_length, self.retract_speed*60, self.lift_z))
            self.is_retracted = True
            if self.lift_z != 0.:
                self.current_lift_z = self.lift_z
                self._override_gcode_move()
    def cmd_G11(self, gcmd):
        if self.is_retracted:
            if self.current_lift_z != 0.:
                self._override_gcode_move(undo=True)
            self.gcode.run_script_from_command(
                "SAVE_GCODE_STATE NAME=_retract_state\n"
                "G91\n"
                "G1 E%.5f F%d\n"
                "G1 Z-%.5f\n"
                "RESTORE_GCODE_STATE NAME=_retract_state"
                % (self.unretract_length, self.unretract_speed*60,
                   self.current_lift_z))
            self.is_retracted = False
            self.current_lift_z = 0.

def load_config(config):
    return FirmwareRetraction(config)
