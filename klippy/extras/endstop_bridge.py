# Shared MCU-local endstop output and load-cell hardware trigger transport
#
# Copyright (C) 2026 Xiaoyue Cui <2508041672@qq.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import re
import mcu


class LocalEndstop(mcu.MCU_endstop):
    def add_stepper(self, stepper):
        if stepper.get_mcu() is not self.get_mcu():
            raise self.get_mcu().get_printer().config_error(
                "Endstop bridging receiver and its steppers must share an MCU")
        mcu.MCU_endstop.add_stepper(self, stepper)


class EndstopBridge:
    def __init__(self, config):
        self.config = config
        self.printer = config.get_printer()
        parts = config.get_name().split()
        if len(parts) != 2 or not re.match(r'[A-Za-z0-9_-]+\Z', parts[1]):
            raise config.error('Invalid endstop bridge name')
        self.name = parts[1]
        self.inputs = {}
        self.claimed_inputs = set()
        self.owner = None
        self.poll_interval = config.getfloat('poll_interval', .000050,
                                             minval=.000010, maxval=.010)
        ppins = self.printer.lookup_object('pins')
        output = ppins.lookup_pin(config.get('output_pin'), can_invert=True)
        self.receive_params = ppins.lookup_pin(
            config.get('receive_pin'), can_invert=True, can_pullup=True)
        self.mcu = output['chip']
        if not hasattr(self.mcu, 'create_oid'):
            raise config.error('Endstop bridging requires an MCU output pin')
        self.oid = self.mcu.create_oid()
        self.output_pin = output['pin']
        self.invert = output['invert']
        self.set_cmd = None
        self.mcu.register_config_callback(self._build_config)
        ppins.register_chip('bridge:' + self.name, self)
        # Keep source inputs together in the bridge section.
        for option in config.get_prefix_options('input_'):
            name = option[6:-4] if option.endswith('_pin') else ''
            if not re.match(r'[a-z0-9_]+\Z', name):
                raise config.error('Expected input_<name>_pin, got %s' % option)
            self.inputs[name] = EndstopBridgeInput(config, self, name)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command('TEST_ENDSTOP_BRIDGE', 'BRIDGE', self.name,
                                  self.cmd_TEST_ENDSTOP_BRIDGE)

    def _build_config(self):
        self.mcu.add_config_cmd(
            'config_endstop_bridge oid=%d output_pin=%s invert=%d'
            % (self.oid, self.output_pin, self.invert))
        self.mcu.add_config_cmd(
            'set_endstop_bridge oid=%d mode=0 input_oid=0 value=0'
            % (self.oid,), on_restart=True)
        self.set_cmd = self.mcu.lookup_query_command(
            'set_endstop_bridge oid=%c mode=%c input_oid=%c value=%c',
            'endstop_bridge_state oid=%c mode=%c value=%c', oid=self.oid)

    def new_receiver(self):
        # Reserve the physical pin once, but keep each route's stepper list
        # and trsync separate. No duplicate_pin_override is needed.
        return LocalEndstop(self.receive_params['chip'], self.receive_params)

    def setup_pin(self, pin_type, pin_params):
        if (pin_type != 'endstop' or pin_params['invert']
                or pin_params['pullup']):
            raise self.config.error(
                'Endstop bridging virtual pins are unmodified endstop inputs')
        name = pin_params['pin']
        route = self.inputs.get(name)
        if route is None:
            raise self.config.error('Unknown input %s on endstop bridge %s'
                                    % (name, self.name))
        if name in self.claimed_inputs:
            raise self.config.error(
                'Endstop bridge input %s used multiple times' % name)
        self.claimed_inputs.add(name)
        return route

    def _set(self, mode, input_oid=0, value=0):
        self.set_cmd.send([self.oid, mode, input_oid, value])

    def prepare(self, owner, receiver):
        if self.owner is not None:
            raise self.printer.command_error(
                'Endstop bridge %s is already in use; home axes separately'
                % (self.name,))
        self.owner = owner
        try:
            # Complete the two-level wire check before motion is scheduled.
            for value in (1, 0):
                self._set(2, value=value)
                actual = receiver.query_endstop(0.)
                if (not self.mcu.is_fileoutput()
                        and actual != value):
                    raise self.printer.command_error(
                        'Endstop bridge %s wire test failed (expected %d)'
                        % (self.name, value))
            self._set(0)
        except Exception:
            self.release(owner)
            raise

    def select_input(self, owner, oid):
        if self.owner is not owner:
            raise self.printer.command_error('Endstop bridge not acquired')
        self._set(1, input_oid=oid)

    def arm_trigger(self, owner, dispatch):
        if self.owner is not owner:
            raise self.printer.command_error('Endstop bridge not acquired')
        # Use the sensor's queue: trsync_start -> wire arm -> analog_home.
        # This also covers sensor faults and the sensor's local trsync timeout.
        cmd = self.mcu.lookup_command(
            'arm_endstop_bridge oid=%c trsync_oid=%c',
            cq=dispatch.get_command_queue())
        cmd.send([self.oid, dispatch.get_oid()])

    def release(self, owner):
        if self.owner is not owner:
            return
        try:
            self._set(0)
        finally:
            self.owner = None

    def cmd_TEST_ENDSTOP_BRIDGE(self, gcmd):
        self.printer.lookup_object('toolhead').wait_moves()
        # A receiver is allocated at configuration time, not at command time.
        receiver = self.test_receiver
        self.prepare(self, receiver)
        self.release(self)
        gcmd.respond_info('Endstop bridge %s wire test passed' % self.name)

    def get_status(self, eventtime):
        return {'active_source': getattr(self.owner, 'name', None)}


class BridgedHoming:
    def __init__(self, config, bridge):
        self.printer = config.get_printer()
        self.bridge = bridge
        self.name = config.get_name()
        self.receiver = bridge.new_receiver()
        self.receiver_active = False
        self.printer.register_event_handler(
            'homing:homing_move_begin', self._homing_begin)
        self.printer.register_event_handler(
            'gcode:command_error', self._command_error)

    def _homing_begin(self, hmove):
        if self in hmove.get_mcu_endstops():
            self.prepare_homing()

    def prepare_homing(self):
        self.bridge.prepare(self, self.receiver)

    def add_stepper(self, stepper):
        self.receiver.add_stepper(stepper)

    def get_steppers(self):
        return self.receiver.get_steppers()

    def _start_receiver(self, print_time, sample_time, sample_count, rest_time,
                        triggered):
        if self.bridge.owner is not self:
            raise self.printer.command_error(
                'Endstop bridging must be prepared before scheduling homing')
        self.receiver_active = True
        return self.receiver.home_start(
            print_time, sample_time or .000015, sample_count or 4,
            min(rest_time or self.bridge.poll_interval,
                self.bridge.poll_interval),
            triggered=triggered)

    def _stop_receiver(self):
        if not self.receiver_active:
            return
        self.receiver_active = False
        self.receiver.abort_home()

    def _cleanup(self):
        try:
            self._stop_receiver()
        finally:
            self.bridge.release(self)

    def _command_error(self):
        if self.bridge.owner is self:
            try:
                self._cleanup()
            except Exception:
                logging.exception('Endstop bridging cleanup failed')
                self.printer.invoke_shutdown('Endstop bridging cleanup failed')


class EndstopBridgeInput(BridgedHoming):
    def __init__(self, config, bridge, input_name):
        BridgedHoming.__init__(self, config, bridge)
        self.name = '%s:%s' % (bridge.name, input_name)
        ppins = self.printer.lookup_object('pins')
        self.pin = ppins.lookup_pin(config.get('input_%s_pin' % input_name),
                                    can_invert=True, can_pullup=True)
        if self.pin['chip'] is not bridge.mcu:
            raise config.error(
                'Forwarded input and bridge output must share an MCU')
        self.filter_count = config.getint('filter_count', 2,
                                          minval=1, maxval=255)
        self.period = config.getfloat('period', .000050,
                                      minval=.000010, maxval=.010)
        self.oid = bridge.mcu.create_oid()
        self.query_cmd = None
        bridge.mcu.register_config_callback(self._build_config)

    def _build_config(self):
        self.bridge.mcu.add_config_cmd(
            'config_endstop_bridge_input oid=%d bridge_oid=%d input_pin=%s'
            ' pull_up=%d invert=%d filter_count=%d period_ticks=%d'
            % (self.oid, self.bridge.oid, self.pin['pin'], self.pin['pullup'],
               self.pin['invert'], self.filter_count,
               self.bridge.mcu.seconds_to_clock(self.period)))
        self.query_cmd = self.bridge.mcu.lookup_query_command(
            'query_endstop_bridge_input oid=%c',
            'endstop_bridge_input_state oid=%c value=%c', oid=self.oid)

    def prepare_homing(self):
        BridgedHoming.prepare_homing(self)
        try:
            self.bridge.select_input(self, self.oid)
        except Exception:
            self.bridge.release(self)
            raise

    def get_mcu(self):
        return self.receiver.get_mcu()

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        return self._start_receiver(print_time, sample_time, sample_count,
                                    rest_time, triggered)

    def home_wait(self, home_end_time):
        try:
            result = self.receiver.home_wait(home_end_time)
            self.receiver_active = False
            return result
        finally:
            self._cleanup()

    def query_endstop(self, print_time):
        # Read the source GPIO without selecting it on the shared wire.
        if self.bridge.mcu.is_fileoutput():
            return 0
        clock = self.bridge.mcu.print_time_to_clock(print_time)
        return self.query_cmd.send([self.oid], minclock=clock)['value']


class HardwareAnalogTrigger(BridgedHoming):
    def __init__(self, config, bridge, source):
        BridgedHoming.__init__(self, config, bridge)
        self.source = source
        if source.get_mcu() is not bridge.mcu:
            raise config.error(
                'Load cell and Endstop bridging output must share an MCU')
        source.setup_trigger_callback(self._arm_wire)

    def __getattr__(self, name):
        return getattr(self.source, name)

    def get_dispatch(self):
        # LookupZSteppers attaches ONLY to the receiving MCU's local endstop.
        return self

    def _arm_wire(self, dispatch):
        self.bridge.arm_trigger(self, dispatch)

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        if not triggered:
            raise self.printer.command_error(
                'Analog forwarding requires a trigger')
        try:
            completion = self._start_receiver(
                print_time, sample_time, sample_count, rest_time, True)
            self.source.home_start(print_time, sample_time, sample_count,
                                   rest_time)
            return completion
        except Exception:
            self._cleanup()
            raise

    def home_wait(self, home_end_time):
        try:
            receiver_time = self.receiver.home_wait(home_end_time)
            self.receiver_active = False
            # The wire stops the motor for success AND faults. The source's
            # original error code must still be checked, never counted as a tap.
            source_time = self.source.home_wait(home_end_time)
            if bool(source_time) != bool(receiver_time):
                raise self.printer.command_error(
                    'Load cell trigger and Endstop bridging receiver disagree')
            return receiver_time
        finally:
            self._cleanup()

    def _cleanup(self):
        try:
            self._stop_receiver()
        finally:
            try:
                self.source.abort_home()
            finally:
                self.bridge.release(self)


def load_config_prefix(config):
    bridge = EndstopBridge(config)
    bridge.test_receiver = bridge.new_receiver()
    return bridge
