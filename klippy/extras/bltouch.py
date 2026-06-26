# BLTouch support
#
# Copyright (C) 2018-2026  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import probe

SIGNAL_PERIOD = 0.020
MIN_CMD_TIME = 5 * SIGNAL_PERIOD

TEST_TIME = 5 * 60.
RETRY_RESET_TIME = 1.
ENDSTOP_REST_TIME = .001
ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 4

Commands = {
    'pin_down': 0.000650, 'touch_mode': 0.001165,
    'pin_up': 0.001475, 'self_test': 0.001780, 'reset': 0.002190,
    'set_5V_output_mode' : 0.001988, 'set_OD_output_mode' : 0.002091,
    'output_mode_store' : 0.001884,
}

# BLTouch "endstop" wrapper
class BLTouchProbe:
    def __init__(self, config, probe_offsets, param_helper):
        self.printer = config.get_printer()
        self.stow_on_each_sample = config.getboolean('stow_on_each_sample',
                                                     True)
        self.probe_touch_mode = config.getboolean('probe_with_touch_mode',
                                                  False)
        # Create a pwm object to handle the control pin
        ppins = self.printer.lookup_object('pins')
        self.mcu_pwm = ppins.setup_pin('pwm', config.get('control_pin'))
        self.mcu_pwm.setup_max_duration(0.)
        self.mcu_pwm.setup_cycle_time(SIGNAL_PERIOD)
        # Command timing
        self.next_cmd_time = self.action_end_time = 0.
        self.finish_home_complete = self.wait_trigger_complete = None
        # Create an "endstop" object to handle the sensor pin
        self.mcu_endstop = ppins.setup_pin('endstop', config.get('sensor_pin'))
        # output mode
        omodes = ['5V', 'OD', None]
        self.output_mode = config.getchoice('set_output_mode', omodes, None)
        # Setup for sensor test
        self.next_test_time = 0.
        self.pin_up_not_triggered = config.getboolean(
            'pin_up_reports_not_triggered', True)
        self.pin_up_touch_triggered = config.getboolean(
            'pin_up_touch_mode_reports_triggered', True)
        # Calculate pin move time
        self.pin_move_time = config.getfloat('pin_move_time', 0.680, above=0.)
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        # Probing via homing to endstop
        self.homing_helper = probe.DescendToEndstopHelper(
            config, self, probe_offsets, param_helper,
            always_check_movement=True)
        # multi probes state
        self.multi = 'OFF'
        # Register BLTOUCH_DEBUG command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("BLTOUCH_DEBUG", self.cmd_BLTOUCH_DEBUG,
                                    desc=self.cmd_BLTOUCH_DEBUG_help)
        self.gcode.register_command("BLTOUCH_STORE", self.cmd_BLTOUCH_STORE,
                                    desc=self.cmd_BLTOUCH_STORE_help)
        # Register events
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
    def _handle_connect(self):
        self._sync_mcu_print_time()
        self.next_cmd_time += 0.200
        self._set_output_mode(self.output_mode)
        try:
            self._raise_probe()
            self._verify_raise_probe()
        except self.printer.command_error as e:
            logging.warning("BLTouch raise probe error: %s", str(e))
    def _sync_mcu_print_time(self):
        curtime = self.printer.get_reactor().monotonic()
        est_time = self.mcu_pwm.get_mcu().estimated_print_time(curtime)
        self.next_cmd_time = max(self.next_cmd_time, est_time + MIN_CMD_TIME)
    def _sync_print_time(self):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        if self.next_cmd_time > print_time:
            toolhead.dwell(self.next_cmd_time - print_time)
        else:
            self.next_cmd_time = print_time
    def _send_cmd(self, cmd, duration=MIN_CMD_TIME):
        # Translate duration to ticks to avoid any secondary mcu clock skew
        mcu = self.mcu_pwm.get_mcu()
        cmd_clock = mcu.print_time_to_clock(self.next_cmd_time)
        pulse = int((duration - MIN_CMD_TIME) / SIGNAL_PERIOD) * SIGNAL_PERIOD
        cmd_clock += mcu.seconds_to_clock(max(MIN_CMD_TIME, pulse))
        end_time = mcu.clock_to_print_time(cmd_clock)
        # Schedule command followed by PWM disable
        self.mcu_pwm.set_pwm(self.next_cmd_time, Commands[cmd] / SIGNAL_PERIOD)
        self.mcu_pwm.set_pwm(end_time, 0.)
        # Update time tracking
        self.action_end_time = self.next_cmd_time + duration
        self.next_cmd_time = max(self.action_end_time, end_time + MIN_CMD_TIME)
    def _verify_state(self, triggered):
        # Perform endstop check to verify bltouch reports desired state
        self.mcu_endstop.home_start(self.action_end_time, ENDSTOP_SAMPLE_TIME,
                                    ENDSTOP_SAMPLE_COUNT, ENDSTOP_REST_TIME,
                                    triggered=triggered)
        try:
            trigger_time = self.mcu_endstop.home_wait(
                self.action_end_time + 0.100)
        except self.printer.command_error as e:
            return False
        return trigger_time > 0.
    def _raise_probe(self):
        self._sync_mcu_print_time()
        if not self.pin_up_not_triggered:
            self._send_cmd('reset')
        self._send_cmd('pin_up', duration=self.pin_move_time)
    def _verify_raise_probe(self):
        if not self.pin_up_not_triggered:
            # No way to verify raise attempt
            return
        for retry in range(3):
            success = self._verify_state(False)
            if success:
                # The "probe raised" test completed successfully
                break
            if retry >= 2:
                raise self.printer.command_error(
                    "BLTouch failed to raise probe")
            msg = "Failed to verify BLTouch probe is raised; retrying."
            self.gcode.respond_info(msg)
            self._sync_mcu_print_time()
            self._send_cmd('reset', duration=RETRY_RESET_TIME)
            self._send_cmd('pin_up', duration=self.pin_move_time)
    def _lower_probe(self):
        self._test_sensor()
        self._sync_print_time()
        self._send_cmd('pin_down', duration=self.pin_move_time)
        if self.probe_touch_mode:
            self._send_cmd('touch_mode')
    def _test_sensor(self):
        if not self.pin_up_touch_triggered:
            # Nothing to test
            return
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        if print_time < self.next_test_time:
            self.next_test_time = print_time + TEST_TIME
            return
        # Raise the bltouch probe and test if probe is raised
        self._sync_print_time()
        for retry in range(3):
            self._send_cmd('pin_up', duration=self.pin_move_time)
            self._send_cmd('touch_mode')
            success = self._verify_state(True)
            self._sync_print_time()
            if success:
                # The "bltouch connection" test completed successfully
                self.next_test_time = print_time + TEST_TIME
                return
            msg = "BLTouch failed to verify sensor state"
            if retry >= 2:
                raise self.printer.command_error(msg)
            self.gcode.respond_info(msg + '; retrying.')
            self._send_cmd('reset', duration=RETRY_RESET_TIME)
    # Probe session
    def start_probe_session(self, gcmd):
        self.homing_helper.clear_trigger_positions()
        if not self.stow_on_each_sample:
            self.multi = 'FIRST'
        return self
    def _probe_prepare(self):
        if self.multi == 'OFF' or self.multi == 'FIRST':
            self._lower_probe()
            if self.multi == 'FIRST':
                self.multi = 'ON'
        self._sync_print_time()
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        rest_time = min(rest_time, ENDSTOP_REST_TIME)
        self.finish_home_complete = self.mcu_endstop.home_start(
            print_time, sample_time, sample_count, rest_time, triggered)
        # Schedule wait_for_trigger callback
        r = self.printer.get_reactor()
        self.wait_trigger_complete = r.register_callback(self._wait_for_trigger)
        return self.finish_home_complete
    def _wait_for_trigger(self, eventtime):
        self.finish_home_complete.wait()
        if self.multi == 'OFF':
            self._raise_probe()
    def _probe_finish(self):
        self.wait_trigger_complete.wait()
        if self.multi == 'OFF':
            self._verify_raise_probe()
        self._sync_print_time()
    def run_probe(self, gcmd):
        self._probe_prepare()
        try:
            self.homing_helper.descend_until_trigger(gcmd)
        except self.printer.command_error as e:
            self._probe_finish()
            raise
        self._probe_finish()
    def pull_probed_results(self):
        return self.homing_helper.pull_trigger_positions()
    def end_probe_session(self):
        self.homing_helper.clear_trigger_positions()
        if not self.stow_on_each_sample:
            self._sync_print_time()
            self._raise_probe()
            self._verify_raise_probe()
            self._sync_print_time()
            self.multi = 'OFF'
    # Output mode setting
    def _set_output_mode(self, mode):
        # If this is inadvertently/purposely issued for a
        # BLTOUCH pre V3.0 and clones:
        #   No reaction at all.
        # BLTOUCH V3.0 and V3.1:
        #   This will set the mode.
        if mode is None:
            return
        logging.info("BLTouch set output mode: %s", mode)
        self._sync_mcu_print_time()
        if mode == '5V':
            self._send_cmd('set_5V_output_mode')
        if mode == 'OD':
            self._send_cmd('set_OD_output_mode')
    def _store_output_mode(self, mode):
        # If this command is inadvertently/purposely issued for a
        # BLTOUCH pre V3.0 and clones:
        #   No reaction at all to this sequence apart from a pin-down/pin-up
        # BLTOUCH V3.0:
        #   This will set the mode (twice) and sadly, a pin-up is needed at
        #   the end, because of the pin-down
        # BLTOUCH V3.1:
        #   This will set the mode and store it in the eeprom.
        #   The pin-up is not needed but does not hurt
        logging.info("BLTouch store output mode: %s", mode)
        self._sync_print_time()
        self._send_cmd('pin_down')
        if mode == '5V':
            self._send_cmd('set_5V_output_mode')
        else:
            self._send_cmd('set_OD_output_mode')
        self._send_cmd('output_mode_store')
        if mode == '5V':
            self._send_cmd('set_5V_output_mode')
        else:
            self._send_cmd('set_OD_output_mode')
        self._send_cmd('pin_up')
    cmd_BLTOUCH_DEBUG_help = "Send a command to the bltouch for debugging"
    def cmd_BLTOUCH_DEBUG(self, gcmd):
        cmd = gcmd.get('COMMAND', None)
        if cmd is None or cmd not in Commands:
            gcmd.respond_info("BLTouch commands: %s" % (
                ", ".join(sorted([c for c in Commands if c is not None]))))
            return
        gcmd.respond_info("Sending BLTOUCH_DEBUG COMMAND=%s" % (cmd,))
        self._sync_print_time()
        self._send_cmd(cmd, duration=self.pin_move_time)
        self._sync_print_time()
    cmd_BLTOUCH_STORE_help = "Store an output mode in the BLTouch EEPROM"
    def cmd_BLTOUCH_STORE(self, gcmd):
        cmd = gcmd.get('MODE', None)
        if cmd is None or cmd not in ['5V', 'OD']:
            gcmd.respond_info("BLTouch output modes: 5V, OD")
            return
        gcmd.respond_info("Storing BLTouch output mode: %s" % (cmd,))
        self._sync_print_time()
        self._store_output_mode(cmd)
        self._sync_print_time()

# Main external probe interface
class PrinterBLTouch:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.probe_offsets = probe.ProbeOffsetsHelper(config)
        self.param_helper = probe.ProbeParameterHelper(config)
        self.mcu_probe = BLTouchProbe(config, self.probe_offsets,
                                      self.param_helper)
        self.probe_session = probe.SampleAveragingHelper(
            config, self.param_helper, self.mcu_probe.start_probe_session)
        query_endstop = self.mcu_probe.query_endstop
        self.cmd_helper = probe.ProbeCommandHelper(config, self, query_endstop)
        probe.HomingViaProbeHelper(config, self.probe_offsets.get_offsets()[2],
                                   query_endstop)
    def get_probe_params(self, gcmd=None):
        return self.param_helper.get_probe_params(gcmd)
    def get_offsets(self, gcmd=None):
        return self.probe_offsets.get_offsets(gcmd)
    def get_status(self, eventtime):
        return self.cmd_helper.get_status(eventtime)
    def start_probe_session(self, gcmd):
        return self.probe_session.start_probe_session(gcmd)

def load_config(config):
    blt = PrinterBLTouch(config)
    config.get_printer().add_object('probe', blt)
    return blt
