# SmartEffector support
#
# Copyright (C) 2021  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
from . import probe

# SmartEffector communication protocol implemented here originates from
# https://github.com/Duet3D/SmartEffectorFirmware
BITS_PER_SECOND = 1000.

class ControlPinHelper:
    def __init__(self, pin_params):
        self._mcu = pin_params['chip']
        self._pin = pin_params['pin']
        self._start_value = self._invert = pin_params['invert']
        self._oid = None
        self._set_cmd = None
        self._mcu.register_config_callback(self._build_config)
    def _build_config(self):
        self._mcu.request_move_queue_slot()
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s value=%d default_value=%d"
            " max_duration=%d" % (self._oid, self._pin, self._start_value,
                                  self._start_value, 0))
        cmd_queue = self._mcu.alloc_command_queue()
        self._set_cmd = self._mcu.lookup_command(
            "queue_digital_out oid=%c clock=%u on_ticks=%u", cq=cmd_queue)
    def write_bits(self, start_time, bit_stream):
        bit_step = 1. / BITS_PER_SECOND
        last_value = self._start_value
        bit_time = start_time
        for b in bit_stream:
            value = (not not b) ^ self._invert
            if value != last_value:
                clock = self._mcu.print_time_to_clock(bit_time)
                self._set_cmd.send([self._oid, clock, value])
                last_value = value
            bit_time += bit_step
        # After the last bit, the signal on the control pin must go back
        # to its start value.
        if value != self._start_value:
            clock = self._mcu.print_time_to_clock(bit_time)
            self._set_cmd.send([self._oid, clock, self._start_value])
            bit_time += bit_step
        return bit_time

class SmartEffectorProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.probe_accel = config.getfloat('probe_accel', 0., minval=0.)
        self.recovery_time = config.getfloat('recovery_time', 0.4, minval=0.)
        self.probe_wrapper = probe.ProbeEndstopWrapper(config)
        # Wrappers
        self.get_mcu = self.probe_wrapper.get_mcu
        self.add_stepper = self.probe_wrapper.add_stepper
        self.get_steppers = self.probe_wrapper.get_steppers
        self.home_start = self.probe_wrapper.home_start
        self.home_wait = self.probe_wrapper.home_wait
        self.query_endstop = self.probe_wrapper.query_endstop
        self.multi_probe_begin = self.probe_wrapper.multi_probe_begin
        self.multi_probe_end = self.probe_wrapper.multi_probe_end
        self.get_position_endstop = self.probe_wrapper.get_position_endstop
        # Common probe implementation helpers
        self.cmd_helper = probe.ProbeCommandHelper(
            config, self, self.probe_wrapper.query_endstop)
        self.probe_offsets = probe.ProbeOffsetsHelper(config)
        self.param_helper = probe.ProbeParameterHelper(config)
        self.homing_helper = probe.HomingViaProbeHelper(config, self,
                                                        self.param_helper)
        self.probe_session = probe.ProbeSessionHelper(
            config, self.param_helper, self.homing_helper.start_probe_session)
        # SmartEffector control
        control_pin = config.get('control_pin', None)
        if control_pin:
            ppins = self.printer.lookup_object('pins')
            pin_params = ppins.lookup_pin(control_pin, can_invert=True)
            self.control_pin = ControlPinHelper(pin_params)
            self.gcode.register_command("RESET_SMART_EFFECTOR",
                                        self.cmd_RESET_SMART_EFFECTOR,
                                        desc=self.cmd_RESET_SMART_EFFECTOR_help)
        else:
            self.control_pin = None
        self.gcode.register_command("SET_SMART_EFFECTOR",
                                    self.cmd_SET_SMART_EFFECTOR,
                                    desc=self.cmd_SET_SMART_EFFECTOR_help)
    def get_probe_params(self, gcmd=None):
        return self.param_helper.get_probe_params(gcmd)
    def get_offsets(self):
        return self.probe_offsets.get_offsets()
    def get_status(self, eventtime):
        return self.cmd_helper.get_status(eventtime)
    def start_probe_session(self, gcmd):
        return self.probe_session.start_probe_session(gcmd)
    def probe_prepare(self, hmove):
        toolhead = self.printer.lookup_object('toolhead')
        self.probe_wrapper.probe_prepare(hmove)
        if self.probe_accel:
            systime = self.printer.get_reactor().monotonic()
            toolhead_info = toolhead.get_status(systime)
            self.old_max_accel = toolhead_info['max_accel']
            self.gcode.run_script_from_command(
                    "M204 S%.3f" % (self.probe_accel,))
        if self.recovery_time:
            toolhead.dwell(self.recovery_time)
    def probe_finish(self, hmove):
        if self.probe_accel:
            self.gcode.run_script_from_command(
                    "M204 S%.3f" % (self.old_max_accel,))
        self.probe_wrapper.probe_finish(hmove)
    def _send_command(self, buf):
        # Each byte is sent to the SmartEffector as
        # [0 0 1 0 b7 b6 b5 b4 !b4 b3 b2 b1 b0 !b0]
        bit_stream = []
        for b in buf:
            b = b & 0xFF
            bit_stream.extend([0, 0, 1, 0])
            bit_stream.extend([b & 0x80, b & 0x40, b & 0x20, b & 0x10])
            bit_stream.append((~b) & 0x10)
            bit_stream.extend([b & 0x08, b & 0x04, b & 0x02, b & 0x01])
            bit_stream.append((~b) & 0x01)
        # Wait for previous actions to finish
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.wait_moves()
        start_time = toolhead.get_last_move_time()
        # Write generated bits to the control pin
        end_time = self.control_pin.write_bits(start_time, bit_stream)
        # Dwell to make sure no subseqent actions are queued together
        # with the SmartEffector programming
        toolhead.dwell(end_time - start_time)
        toolhead.wait_moves()
    cmd_SET_SMART_EFFECTOR_help = 'Set SmartEffector parameters'
    def cmd_SET_SMART_EFFECTOR(self, gcmd):
        sensitivity = gcmd.get_int('SENSITIVITY', None, minval=0, maxval=255)
        respond_info = []
        if sensitivity is not None:
            if self.control_pin is not None:
                buf = [105, sensitivity, 255 - sensitivity]
                self._send_command(buf)
                respond_info.append("sensitivity: %d" % (sensitivity,))
            else:
                raise gcmd.error("control_pin must be set in [smart_effector] "
                                 "for sensitivity programming")
        self.probe_accel = gcmd.get_float('ACCEL', self.probe_accel, minval=0.)
        self.recovery_time = gcmd.get_float('RECOVERY_TIME', self.recovery_time,
                                            minval=0.)
        if self.probe_accel:
            respond_info.append(
                    "probing accelartion: %.3f" % (self.probe_accel,))
        else:
            respond_info.append("probing acceleration control disabled")
        if self.recovery_time:
            respond_info.append(
                    "probe recovery time: %.3f" % (self.recovery_time,))
        else:
            respond_info.append("probe recovery time disabled")
        gcmd.respond_info("SmartEffector:\n" + "\n".join(respond_info))
    cmd_RESET_SMART_EFFECTOR_help = 'Reset SmartEffector settings (sensitivity)'
    def cmd_RESET_SMART_EFFECTOR(self, gcmd):
        buf = [131, 131]
        self._send_command(buf)
        gcmd.respond_info('SmartEffector sensitivity was reset')

def load_config(config):
    smart_effector = SmartEffectorProbe(config)
    config.get_printer().add_object('probe', smart_effector)
    return smart_effector
