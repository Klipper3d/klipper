# -*- coding: utf-8 -*-

import logging
from . import probe

# Makerbase Endstop wrapper
class MakerbaseProbeEndstopWrapper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.position_endstop = config.getfloat('z_offset')
        self.stow_on_each_sample = config.getboolean(
            'deactivate_on_each_sample', True)
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.activate_gcode = gcode_macro.load_template(
            config, 'activate_gcode', '')
        self.deactivate_gcode = gcode_macro.load_template(
            config, 'deactivate_gcode', '')
        ppins = self.printer.lookup_object('pins')
        pin = config.get('pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        mcu = pin_params['chip']
        self.mcu_endstop = mcu.setup_pin('endstop', pin_params)
        self.printer.register_event_handler('klippy:mcu_identify',
                                            self._handle_mcu_identify)
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        # multi probes state
        self.multi = 'OFF'
    def _handle_mcu_identify(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self.add_stepper(stepper)
    def raise_probe(self):
        toolhead = self.printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self.deactivate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe activate_gcode script")
    def lower_probe(self):
        toolhead = self.printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self.activate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe deactivate_gcode script")
    def multi_probe_begin(self):
        if self.stow_on_each_sample:
            return
        self.multi = 'FIRST'
    def multi_probe_end(self):
        if self.stow_on_each_sample:
            return
        self.raise_probe()
        self.multi = 'OFF'
    def probe_prepare(self, hmove):
        if self.multi == 'OFF' or self.multi == 'FIRST':
            self.lower_probe()
            if self.multi == 'FIRST':
                self.multi = 'ON'
    def probe_finish(self, hmove):
        if self.multi == 'OFF':
            self.raise_probe()
    def get_position_endstop(self):
        return self.position_endstop

class MakerBase:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        # self.speed = config.getfloat('speed', 5.0, above=0.)
        # self.lift_speed = config.getfloat('lift_speed', self.speed, above=0.)
        # self.x_offset = config.getfloat('x_offset', 0.)
        # self.y_offset = config.getfloat('y_offset', 0.)
        # self.z_offset = config.getfloat('z_offset')
        self.probe = self.printer.lookup_object('probe')
        self.endstop_wrapper = probe.ProbeEndstopWrapper(config)
        # self.probe_accel = config.getfloat('probe_accel', 0., minval=0.)
        # self.recovery_time = config.getfloat('recovery_time', 0.4, minval=0.)
        # Register MakerBase commands
        self.gcode.register_command('MKS_PROBE_PIN_1', self.cmd_MKS_PROBE_PIN_1,
                                    desc=self.cmd_MKS_PROBE_PIN_1_help)
        self.gcode.register_command('MKS_PROBE_PIN_2', self.cmd_MKS_PROBE_PIN_2,
                                    desc=self.cmd_MKS_PROBE_PIN_2_help)
self.gcode.register_command('QIDI_PROBE_PIN_1', self.cmd_MKS_PROBE_PIN_1,
                                    desc=self.cmd_MKS_PROBE_PIN_1_help)
self.gcode.register_command('QIDI_PROBE_PIN_2', self.cmd_MKS_PROBE_PIN_2,
                                    desc=self.cmd_MKS_PROBE_PIN_2_help)
        self.gcode.register_command('MKS_REMOVE', self.cmd_MKS_REMOVE,
                                    desc=self.cmd_MKS_REMOVE_help)
    cmd_MKS_PROBE_PIN_2_help = 'ENABLE_PROBE_PIN_2'
    def cmd_MKS_PROBE_PIN_2(self, gcmd):
        self.probe.mcu_probe.probe_wrapper = self.endstop_wrapper
        # Wrappers
        self.probe.mcu_probe.get_mcu = self.endstop_wrapper.get_mcu
        self.probe.mcu_probe.add_stepper = self.endstop_wrapper.add_stepper
        self.probe.mcu_probe.get_steppers = self.endstop_wrapper.get_steppers
        self.probe.mcu_probe.home_start = self.endstop_wrapper.home_start
        self.probe.mcu_probe.home_wait = self.endstop_wrapper.home_wait
        self.probe.mcu_probe.query_endstop = self.endstop_wrapper.query_endstop
self.probe.mcu_probe.multi_probe_begin = self.endstop_wrapper.multi_probe_begin
self.probe.mcu_probe.multi_probe_end = self.endstop_wrapper.multi_probe_end
        # gcmd.respond_raw("%s" % (self.cmd_MKS_PROBE_PIN_2_help, ))
    cmd_MKS_REMOVE_help = 'MKS_REMOVE'
    def cmd_MKS_REMOVE(self, gcmd):
        self.printer.remove_object('probe')
        self.printer.lookup_object('gcode').remove_command('PROBE')
        self.printer.lookup_object('gcode').remove_command('QUERY_PROBE')
        self.printer.lookup_object('gcode').remove_command('PROBE_CALIBRATE')
        self.printer.lookup_object('gcode').remove_command('PROBE_ACCURACY')
        self.printer.lookup_object(
                                   'gcode').remove_command('Z_OFFSET_APPLY_PROBE'
                                  )
        self.printer.lookup_object('gcode').remove_command('MKS_SHOW_Z_OFFSET')
        self.printer.lookup_object('pins').remove_chip('probe')
    cmd_MKS_PROBE_PIN_1_help = 'ENABLE_PROBE_PIN_1'
    def cmd_MKS_PROBE_PIN_1(self, gcmd):
self.probe.mcu_probe.probe_wrapper = self.probe.mcu_probe.probe_wrapper_2
        # Wrappers
self.probe.mcu_probe.get_mcu = self.probe.mcu_probe.probe_wrapper_2.get_mcu
self.probe.mcu_probe.add_stepper =
            self.probe.mcu_probe.probe_wrapper_2.add_stepper
self.probe.mcu_probe.get_steppers =
            self.probe.mcu_probe.probe_wrapper_2.get_steppers
self.probe.mcu_probe.home_start = self.probe.mcu_probe.probe_wrapper_2.home_start
self.probe.mcu_probe.home_wait = self.probe.mcu_probe.probe_wrapper_2.home_wait
self.probe.mcu_probe.query_endstop =
            self.probe.mcu_probe.probe_wrapper_2.query_endstop
self.probe.mcu_probe.multi_probe_begin =
            self.probe.mcu_probe.probe_wrapper_2.multi_probe_begin
self.probe.mcu_probe.multi_probe_end =
            self.probe.mcu_probe.probe_wrapper_2.multi_probe_end
        # gcmd.respond_raw("%s" % (self.cmd_MKS_PROBE_PIN_1_help, ))

def load_config(config):
    return MakerBase(config)
