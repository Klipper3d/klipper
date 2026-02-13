# BLTouch support
#
# Copyright (C) 2018-2024  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2025  Marco Abbattista <max.abbattista@llive.it>
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
    def __init__(self, config):
        self.printer = config.get_printer()
        self.position_endstop = config.getfloat('z_offset', minval=0.)
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
        # Setup nozzle probe pin for auto z-offset calibration (optional)
        nozzle_probe_pin = config.get('nozzle_probe_pin', None)
        if nozzle_probe_pin is not None:
            self.nozzle_endstop = ppins.setup_pin('endstop', nozzle_probe_pin)
            # Register Z steppers with the nozzle endstop
            probe.LookupZSteppers(config, self.nozzle_endstop.add_stepper)
            self.calibration_position = config.getfloatlist('calibration_position',
                                                           default=[5., 5.], count=2)
            self.nozzle_temp = config.getfloat('nozzle_temp', 200., above=0.)
            # Calibration probe speeds and distances
            self.cal_probe_speed = config.getfloat('calibration_probe_speed', 5.0, above=0.)
            self.cal_probe_speed_slow = config.getfloat('calibration_probe_speed_slow', 1.0, above=0.)
            self.cal_lift_speed = config.getfloat('calibration_lift_speed', 10.0, above=0.)
            self.cal_retract_dist = config.getfloat('calibration_retract_dist', 2.0, above=0.)
            self.cal_nozzle_retract_dist = config.getfloat('calibration_nozzle_retract_dist', 2.0, above=0.)
            self.cal_nozzle_samples = config.getint('calibration_nozzle_samples', 1, minval=1, maxval=10)
            self.cal_nozzle_diameter = config.getfloat('calibration_nozzle_diameter', 0.4, above=0.)
        else:
            self.nozzle_endstop = None
            self.calibration_position = None
            self.nozzle_temp = None
            self.cal_probe_speed = None
            self.cal_probe_speed_slow = None
            self.cal_lift_speed = None
            self.cal_retract_dist = None
            self.cal_nozzle_retract_dist = None
            self.cal_nozzle_samples = None
            self.cal_nozzle_diameter = None
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
        # multi probes state
        self.multi = 'OFF'
        # Common probe implementation helpers
        self.cmd_helper = probe.ProbeCommandHelper(
            config, self, self.mcu_endstop.query_endstop)
        self.probe_offsets = probe.ProbeOffsetsHelper(config)
        self.param_helper = probe.ProbeParameterHelper(config)
        self.homing_helper = probe.HomingViaProbeHelper(
            config, self, self.probe_offsets, self.param_helper)
        self.probe_session = probe.ProbeSessionHelper(
            config, self.param_helper, self.homing_helper.start_probe_session)
        # Register BLTOUCH_DEBUG command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("BLTOUCH_DEBUG", self.cmd_BLTOUCH_DEBUG,
                                    desc=self.cmd_BLTOUCH_DEBUG_help)
        self.gcode.register_command("BLTOUCH_STORE", self.cmd_BLTOUCH_STORE,
                                    desc=self.cmd_BLTOUCH_STORE_help)
        # Register auto z-offset calibration command if nozzle probe is configured
        if self.nozzle_endstop is not None:
            self.gcode.register_command("BLTOUCH_AUTO_Z_OFFSET",
                                      self.cmd_BLTOUCH_AUTO_Z_OFFSET,
                                      desc=self.cmd_BLTOUCH_AUTO_Z_OFFSET_help)
        # Register events
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
    def get_probe_params(self, gcmd=None):
        return self.param_helper.get_probe_params(gcmd)
    def get_offsets(self, gcmd=None):
        return self.probe_offsets.get_offsets(gcmd)
    def get_status(self, eventtime):
        return self.cmd_helper.get_status(eventtime)
    def start_probe_session(self, gcmd):
        return self.probe_session.start_probe_session(gcmd)
    def handle_connect(self):
        self.sync_mcu_print_time()
        self.next_cmd_time += 0.200
        self.set_output_mode(self.output_mode)
        try:
            self.raise_probe()
            self.verify_raise_probe()
        except self.printer.command_error as e:
            logging.warning("BLTouch raise probe error: %s", str(e))
    def sync_mcu_print_time(self):
        curtime = self.printer.get_reactor().monotonic()
        est_time = self.mcu_pwm.get_mcu().estimated_print_time(curtime)
        self.next_cmd_time = max(self.next_cmd_time, est_time + MIN_CMD_TIME)
    def sync_print_time(self):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        if self.next_cmd_time > print_time:
            toolhead.dwell(self.next_cmd_time - print_time)
        else:
            self.next_cmd_time = print_time
    def send_cmd(self, cmd, duration=MIN_CMD_TIME):
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
    def verify_state(self, triggered):
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
    def raise_probe(self):
        self.sync_mcu_print_time()
        if not self.pin_up_not_triggered:
            self.send_cmd('reset')
        self.send_cmd('pin_up', duration=self.pin_move_time)
    def verify_raise_probe(self):
        if not self.pin_up_not_triggered:
            # No way to verify raise attempt
            return
        for retry in range(3):
            success = self.verify_state(False)
            if success:
                # The "probe raised" test completed successfully
                break
            if retry >= 2:
                raise self.printer.command_error(
                    "BLTouch failed to raise probe")
            msg = "Failed to verify BLTouch probe is raised; retrying."
            self.gcode.respond_info(msg)
            self.sync_mcu_print_time()
            self.send_cmd('reset', duration=RETRY_RESET_TIME)
            self.send_cmd('pin_up', duration=self.pin_move_time)
    def lower_probe(self):
        self.test_sensor()
        self.sync_print_time()
        self.send_cmd('pin_down', duration=self.pin_move_time)
        if self.probe_touch_mode:
            self.send_cmd('touch_mode')
    def test_sensor(self):
        if not self.pin_up_touch_triggered:
            # Nothing to test
            return
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        if print_time < self.next_test_time:
            self.next_test_time = print_time + TEST_TIME
            return
        # Raise the bltouch probe and test if probe is raised
        self.sync_print_time()
        for retry in range(3):
            self.send_cmd('pin_up', duration=self.pin_move_time)
            self.send_cmd('touch_mode')
            success = self.verify_state(True)
            self.sync_print_time()
            if success:
                # The "bltouch connection" test completed successfully
                self.next_test_time = print_time + TEST_TIME
                return
            msg = "BLTouch failed to verify sensor state"
            if retry >= 2:
                raise self.printer.command_error(msg)
            self.gcode.respond_info(msg + '; retrying.')
            self.send_cmd('reset', duration=RETRY_RESET_TIME)
    def multi_probe_begin(self):
        if self.stow_on_each_sample:
            return
        self.multi = 'FIRST'
    def multi_probe_end(self):
        if self.stow_on_each_sample:
            return
        self.sync_print_time()
        self.raise_probe()
        self.verify_raise_probe()
        self.sync_print_time()
        self.multi = 'OFF'
    def probe_prepare(self, hmove):
        if self.multi == 'OFF' or self.multi == 'FIRST':
            self.lower_probe()
            if self.multi == 'FIRST':
                self.multi = 'ON'
        self.sync_print_time()
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        rest_time = min(rest_time, ENDSTOP_REST_TIME)
        self.finish_home_complete = self.mcu_endstop.home_start(
            print_time, sample_time, sample_count, rest_time, triggered)
        # Schedule wait_for_trigger callback
        r = self.printer.get_reactor()
        self.wait_trigger_complete = r.register_callback(self.wait_for_trigger)
        return self.finish_home_complete
    def wait_for_trigger(self, eventtime):
        self.finish_home_complete.wait()
        if self.multi == 'OFF':
            self.raise_probe()
    def probe_finish(self, hmove):
        self.wait_trigger_complete.wait()
        if self.multi == 'OFF':
            self.verify_raise_probe()
        self.sync_print_time()
        if hmove.check_no_movement() is not None:
            raise self.printer.command_error("BLTouch failed to deploy")
    def get_position_endstop(self):
        return self.position_endstop
    def set_output_mode(self, mode):
        # If this is inadvertently/purposely issued for a
        # BLTOUCH pre V3.0 and clones:
        #   No reaction at all.
        # BLTOUCH V3.0 and V3.1:
        #   This will set the mode.
        if mode is None:
            return
        logging.info("BLTouch set output mode: %s", mode)
        self.sync_mcu_print_time()
        if mode == '5V':
            self.send_cmd('set_5V_output_mode')
        if mode == 'OD':
            self.send_cmd('set_OD_output_mode')
    def store_output_mode(self, mode):
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
        self.sync_print_time()
        self.send_cmd('pin_down')
        if mode == '5V':
            self.send_cmd('set_5V_output_mode')
        else:
            self.send_cmd('set_OD_output_mode')
        self.send_cmd('output_mode_store')
        if mode == '5V':
            self.send_cmd('set_5V_output_mode')
        else:
            self.send_cmd('set_OD_output_mode')
        self.send_cmd('pin_up')
    cmd_BLTOUCH_DEBUG_help = "Send a command to the bltouch for debugging"
    def cmd_BLTOUCH_DEBUG(self, gcmd):
        cmd = gcmd.get('COMMAND', None)
        if cmd is None or cmd not in Commands:
            gcmd.respond_info("BLTouch commands: %s" % (
                ", ".join(sorted([c for c in Commands if c is not None]))))
            return
        gcmd.respond_info("Sending BLTOUCH_DEBUG COMMAND=%s" % (cmd,))
        self.sync_print_time()
        self.send_cmd(cmd, duration=self.pin_move_time)
        self.sync_print_time()
    cmd_BLTOUCH_STORE_help = "Store an output mode in the BLTouch EEPROM"
    def cmd_BLTOUCH_STORE(self, gcmd):
        cmd = gcmd.get('MODE', None)
        if cmd is None or cmd not in ['5V', 'OD']:
            gcmd.respond_info("BLTouch output modes: 5V, OD")
            return
        gcmd.respond_info("Storing BLTouch output mode: %s" % (cmd,))
        self.sync_print_time()
        self.store_output_mode(cmd)
        self.sync_print_time()
    cmd_BLTOUCH_AUTO_Z_OFFSET_help = "Automatically calibrate BLTouch z-offset using nozzle probe"
    def cmd_BLTOUCH_AUTO_Z_OFFSET(self, gcmd):
        if self.nozzle_endstop is None:
            raise gcmd.error("nozzle_probe_pin not configured in [bltouch] section")
        
        # Check if nozzle probe is already triggered before starting
        curtime = self.printer.get_reactor().monotonic()
        nozzle_triggered = self.nozzle_endstop.query_endstop(curtime)
        if nozzle_triggered:
            raise gcmd.error("Nozzle probe is already triggered. Please check for obstructions and ensure the nozzle is clear before starting calibration.")
        
        toolhead = self.printer.lookup_object('toolhead')
        gcode = self.printer.lookup_object('gcode')
        heaters = self.printer.lookup_object('heaters')
        
        # Get optional parameters
        heat_nozzle = gcmd.get_int('HEAT_NOZZLE', 1)  # Default: 1 (enabled)
        nozzle_temp = gcmd.get_float('NOZZLE_TEMP', self.nozzle_temp, above=0.)
        cal_pos = gcmd.get('POSITION', None)
        if cal_pos:
            cal_x, cal_y = [float(v.strip()) for v in cal_pos.split(',')]
        else:
            cal_x, cal_y = self.calibration_position
        
        # Step 1: Home all axes
        gcode.run_script_from_command("G28")
        
        # Step 2: Move to calibration position considering BLTouch offset
        # If we want BLTouch at position (cal_x, cal_y), we need to move toolhead to:
        # toolhead_x = cal_x - x_offset, toolhead_y = cal_y - y_offset
        x_offset, y_offset, _ = self.get_offsets()
        toolhead_x = cal_x - x_offset
        toolhead_y = cal_y - y_offset
        toolhead.manual_move([toolhead_x, toolhead_y, None], 50.)
        
        # Step 3: Heat nozzle if needed and enabled
        if heat_nozzle and nozzle_temp > 0:
            gcmd.respond_info("Heating nozzle to %.1f°C..." % nozzle_temp)
            extruder = toolhead.get_extruder()
            heaters.set_temperature(extruder.get_heater(), nozzle_temp, True)
        
        # Step 4: BLTouch probe - EXACT SAME as PROBE_CALIBRATE
        gcmd.respond_info("Performing BLTouch probe (PROBE_CALIBRATE method)...")
        
        # Import probe utilities
        from . import probe
        
        # Use run_single_probe - EXACT SAME method as PROBE_CALIBRATE
        # This ensures same coordinate reference system
        bltouch_pos = probe.run_single_probe(self, gcmd)
        z_bltouch_reference = bltouch_pos[2]
        
        gcmd.respond_info("BLTouch reference position: %.6f" % z_bltouch_reference)
        
        # Step 5: Move up and position nozzle - SAME as PROBE_CALIBRATE
        # Move away from the bed (same 5mm lift as PROBE_CALIBRATE)
        lift_pos = list(bltouch_pos)
        lift_pos[2] += 5.
        toolhead.manual_move(lift_pos, self.cal_lift_speed)
        
        # Move the nozzle over the probe point - SAME as PROBE_CALIBRATE
        x_offset, y_offset, z_offset = self.get_offsets()
        lift_pos[0] += x_offset
        lift_pos[1] += y_offset
        toolhead.manual_move(lift_pos, self.cal_probe_speed)
        
        gcmd.respond_info("Nozzle positioned over probe point at Z=%.6f" % lift_pos[2])
        
        # Step 6: Nozzle probe - Use direct probing method for endstop
        gcmd.respond_info("Performing nozzle probe (PROBE_CALIBRATE coordinate system)...")
        
        # Use homing module directly with nozzle endstop - SAME coordinate system
        phoming = self.printer.lookup_object('homing')
        
        # Get current position and probe down to find bed
        current_pos = toolhead.get_position()
        
        # Get Z minimum position safely
        z_min = -5.0  # Use position_min from config
        try:
            config = self.printer.lookup_object('configfile')
            z_config = config.status_raw_config.get('stepper_z', {})
            z_min = float(z_config.get('position_min', -5.0))
        except:
            z_min = -5.0
        
        # Create probe target position - probe down to find bed
        probe_target = [current_pos[0], current_pos[1], z_min]
        
        gcmd.respond_info("Nozzle probing from Z=%.3f to Z=%.3f..." % (current_pos[2], z_min))
        
        # Use probing_move with nozzle endstop - SAME coordinate system as BLTouch
        nozzle_pos = phoming.probing_move(self.nozzle_endstop, probe_target, self.cal_probe_speed_slow)
        z_nozzle_reference = nozzle_pos[2]
        
        gcmd.respond_info("Nozzle reference position: %.6f" % z_nozzle_reference)
        
        # Step 7: Calculate z_offset - EXACT SAME formula as PROBE_CALIBRATE
        # z_offset = probe_calibrate_z - final_nozzle_z
        # In PROBE_CALIBRATE: z_offset = self.probe_calibrate_z - kin_pos[2]
        new_z_offset = z_bltouch_reference - z_nozzle_reference
        
        gcmd.respond_info("BLTouch reference: %.6f, Nozzle reference: %.6f" % 
                         (z_bltouch_reference, z_nozzle_reference))
        gcmd.respond_info("Calculated z_offset: %.6f" % new_z_offset)
        
        # Validate against expected physical measurement
        expected_offset = 4.1  # Your measured physical distance
        offset_error = abs(new_z_offset - expected_offset)
        
        gcmd.respond_info("Calculated: %.6f, Expected: %.6f, Error: %.6f" % 
                         (new_z_offset, expected_offset, offset_error))
        
        if offset_error > 1.0:
            gcmd.respond_info("WARNING: Large offset error detected. Check BLTouch mounting and nozzle probe configuration.")
        
        # Validate that the offset makes physical sense
        if new_z_offset < 0:
            raise gcmd.error(
                "Invalid z-offset calculation: %.6f\n"
                "BLTouch Z: %.6f, Nozzle Z: %.6f\n"
                "The nozzle should trigger at a lower Z position than the BLTouch.\n"
                "Please check your wiring and probe configuration." % 
                (new_z_offset, z_bltouch_reference, z_nozzle_reference))
        
        # Move up for safety
        toolhead.manual_move([None, None, max(z_bltouch_reference, z_nozzle_reference) + 10.], 50.)
        
        # Save the positive offset value
        save_z_offset = new_z_offset
        
        # Save the new z-offset
        self.position_endstop = save_z_offset
        configfile = self.printer.lookup_object('configfile')
        configfile.set('bltouch', 'z_offset', "%.6f" % save_z_offset)
        
        gcmd.respond_info(
            "BLTouch z_offset calibration complete!\n"
            "New z_offset: %.6f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer." % save_z_offset)
        
        # Re-home to restore coordinate system integrity
        gcmd.respond_info("Re-homing to restore coordinate system...")
        gcode.run_script_from_command("G28")
        
        # Verify coordinate restoration
        restored_pos = toolhead.get_position()
        gcmd.respond_info("Coordinate system restored. Current Z position: %.6f" % restored_pos[2])

def load_config(config):
    blt = BLTouchProbe(config)
    config.get_printer().add_object('probe', blt)
    return blt
