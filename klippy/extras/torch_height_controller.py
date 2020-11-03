# Support for torch height controller
#
# Copyright (C) 2020  Lucas Felix <lucas.felix0738@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from math import sqrt

class TorchHeightController:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        all_mcus = [m for n, m in self.printer.lookup_objects(module='mcu')]
        self.mcu = all_mcus[0]
        self.toolhead = None
        self.x_stepper = self.y_stepper = self.z_stepper = None

        self.thc_oid = self.mcu.create_oid()
        self.mcu.add_config_cmd("config_thc oid=%d rate=%u a_coeff=%i"
            " b_coeff=%i" % (self.thc_oid, config.getint('rate'),
            config.getfloat('a_coeff')*(2**10),
            config.getfloat('b_coeff')*1000))
        # conversion from volts to milivolts
        mm_per_s_per_mv = config.getfloat('speed_coeff') / 1000
        self.speed_coeff = int(mm_per_s_per_mv * 2**14)

        self.cmd_queue = self.mcu.alloc_command_queue()
        self.mcu.register_config_callback(self.build_config)
        self.thc_start_cmd = None
        self.thc_stop_cmd = None

        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("M6", self.cmd_M6)
        self.gcode.register_command("M7", self.cmd_M7)
        self.gcode.register_command("M8", self.cmd_M8)
        self.mcu.register_response(self._handle_sample, 'thc_sample')
        self.enable = False
        self.last_M7 = None

    def build_config(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        kin = self.toolhead.get_kinematics()
        self.x_stepper = kin.rails[0].steppers[0]
        self.y_stepper = kin.rails[1].steppers[0]
        z_rail = kin.rails[2]
        self.z_stepper = z_rail.steppers[0]
        self.abs_min_z_pos = z_rail.position_min
        self.abs_max_z_pos = z_rail.position_max
        if self.z_stepper.is_dir_inverted():
            self.speed_coeff = -self.speed_coeff
        self.thc_start_cmd = self.mcu.lookup_command("start_thc oid=%c"
            " x_stepper_oid=%c y_stepper_oid=%c z_stepper_oid=%c clock=%u"
            " voltage_mv=%i speed_coeff=%i threshold=%u min_pos=%i max_pos=%i",
            cq=self.cmd_queue)
        self.thc_stop_cmd = self.mcu.lookup_command(
            "stop_thc oid=%c clock=%u", cq=self.cmd_queue)

    def _handle_sample(self, params):
        z_mcu_pos = params['z_pos'] * self.z_stepper._step_dist
        if self.z_stepper._invert_dir:
            z_mcu_pos = -z_mcu_pos
        z_pos = z_mcu_pos - self.z_stepper._mcu_position_offset
        voltage = float(params['voltage_mv']) / 1000
        xy_speed = sqrt(params['xy_speed_squared'])
        self.gcode.respond_info('echo: THC_error ' + str(z_pos) + ' ' +
            str(voltage) + ' ' + str(xy_speed))

    def cmd_M6(self, gcmd):
        if not self.enable:
            voltage = gcmd.get_float('V', minval=0, maxval=300)
            min_xy_feedrate = gcmd.get_float('T', default=0) / 60
            if min_xy_feedrate < 0:
                threshold = 0xFFFFFFFF
            else:
                threshold = int(min_xy_feedrate**2)

            min_z_pos = gcmd.get_float('A', default=self.abs_min_z_pos,
                minval=self.abs_min_z_pos, maxval=self.abs_max_z_pos)
            max_z_pos = gcmd.get_float('B', default=self.abs_max_z_pos,
                minval=self.abs_min_z_pos, maxval=self.abs_max_z_pos)
            if min_z_pos > max_z_pos:
                self.gcode._respond_error('min position <= max position')
                return
            min_mcu_z_pos = int((self.z_stepper._mcu_position_offset +
                                min_z_pos) / self.z_stepper._step_dist)
            max_mcu_z_pos = int((self.z_stepper._mcu_position_offset +
                                max_z_pos) / self.z_stepper._step_dist)
            if self.z_stepper._invert_dir:
                min_mcu_z_pos, max_mcu_z_pos = -max_mcu_z_pos, -min_mcu_z_pos

            last_move = self.toolhead.get_last_move_time()
            clock = self.mcu.print_time_to_clock(last_move)
            self.thc_start_cmd.send(
                [self.thc_oid, self.x_stepper._oid, self.y_stepper._oid,
                 self.z_stepper._oid, clock, int(voltage  * 1000),
                 self.speed_coeff, threshold, min_mcu_z_pos, max_mcu_z_pos],
                 reqclock=clock)
            self.enable = True
        else:
            self.gcode._respond_error('THC already ON')

    def cmd_M7(self, gcmd):
        if self.enable:
            self.last_M7 = self.toolhead.get_last_move_time()
            clock = self.mcu.print_time_to_clock(self.last_M7)
            self.thc_stop_cmd.send([self.thc_oid, clock], reqclock=clock)
            self.enable = False

    def cmd_M8(self, gcmd):
        if self.enable:
            self.gcode._respond_error('Cannot resync with THC running.')
            return
        if self.last_M7 is None:
            return

        # M8 has to be blocking to ensure not trying to synchronize before M7
        # is effective.
        now = self.reactor.monotonic()
        self.reactor.pause(now + self.last_M7
                           - self.mcu.estimated_print_time(now) + 0.05)
        self.last_M7 = None

        z_pos = self.z_stepper.resync_mcu_position()
        cur_pos = self.toolhead.get_position()
        self.toolhead.set_position([cur_pos[0], cur_pos[1], z_pos, cur_pos[3]],
                                   homing_axes=(0, 1, 2))

def load_config(config):
    return TorchHeightController(config)
