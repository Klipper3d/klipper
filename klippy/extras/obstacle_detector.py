# Obstacle Detector for TMC5160 using Hybrid Detection
#
# This module monitors Z homing to detect obstacles using multiple methods:
# 1. StallGuard (SG_RESULT) - Motor load detection
# 2. Timeout - Expected vs actual homing time
# 3. Position stall - No movement despite commands
#
# Copyright (C) 2024 MicroLay
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import os
import time

# TMC5160 register addresses
TMC5160_TCOOLTHRS = 0x14
TMC5160_SGTHRS = 0x74
TMC5160_COOLCONF = 0x6d

class ObstacleDetector:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()

        # Configuration - SG threshold detection
        # At 10mm/s, normal SG values are 200-350. Obstacles cause SG drop to near 0.
        # We use a low threshold to catch real obstacles while avoiding false positives
        # from endstop trigger (which also causes SG=0 briefly)
        self.sg_threshold = config.getint('sg_threshold', 30)  # Very low - only catch real stalls
        self.check_interval = config.getfloat('check_interval', 0.05)
        self.min_samples = config.getint('min_samples', 8)  # ~400ms of sustained low SG
        self.flag_file = config.get('flag_file', '/tmp/homing_obstacle_flag')
        self.drop_threshold = config.getint('drop_threshold', 50)  # Detect rapid SG drop

        # Configuration - Timeout detection
        self.expected_travel = config.getfloat('expected_travel', 80.0)  # Expected Z travel in mm
        self.homing_speed = config.getfloat('homing_speed', 5.0)  # Homing speed in mm/s
        self.timeout_margin = config.getfloat('timeout_margin', 1.5)  # Safety margin multiplier

        # Calculate expected homing time with margin
        self.expected_time = (self.expected_travel / self.homing_speed) * self.timeout_margin

        # TCOOLTHRS: velocity threshold for StallGuard activation
        self.tcoolthrs = config.getint('tcoolthrs', 0xFFFFF)

        # State
        self.monitoring = False
        self.tmc = None
        self.toolhead = None
        self.check_timer = None
        self.low_sg_count = 0
        self.last_sg_values = []
        self.original_tcoolthrs = None
        self.last_sg_reading = None

        # Hybrid detection state
        self.homing_start_time = None
        self.last_z_position = None
        self.stall_count = 0
        self.detection_reason = None

        # Register events
        self.printer.register_event_handler("klippy:connect", self._handle_connect)
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        self.printer.register_event_handler("homing:homing_move_begin", self._start_monitor)
        self.printer.register_event_handler("homing:homing_move_end", self._stop_monitor)

        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('QUERY_SG', self.cmd_QUERY_SG,
                                    desc=self.cmd_QUERY_SG_help)
        self.gcode.register_command('SET_SG_THRESHOLD', self.cmd_SET_SG_THRESHOLD,
                                    desc=self.cmd_SET_SG_THRESHOLD_help)
        self.gcode.register_command('SG_MONITOR_TEST', self.cmd_SG_MONITOR_TEST,
                                    desc=self.cmd_SG_MONITOR_TEST_help)
        self.gcode.register_command('ENABLE_STALLGUARD', self.cmd_ENABLE_STALLGUARD,
                                    desc=self.cmd_ENABLE_STALLGUARD_help)
        self.gcode.register_command('QUERY_TMC_REGS', self.cmd_QUERY_TMC_REGS,
                                    desc=self.cmd_QUERY_TMC_REGS_help)

        logging.info(f"ObstacleDetector: HYBRID mode initialized")
        logging.info(f"  SG threshold={self.sg_threshold}, min_samples={self.min_samples}")
        logging.info(f"  Expected travel={self.expected_travel}mm, speed={self.homing_speed}mm/s")
        logging.info(f"  Expected time={self.expected_time:.1f}s (with {self.timeout_margin}x margin)")

    def _handle_connect(self):
        """Called when Klipper connects to MCU"""
        # Try to get TMC5160 object for stepper_z
        self.tmc = self.printer.lookup_object('tmc5160 stepper_z', None)
        if self.tmc is None:
            logging.warning("ObstacleDetector: TMC5160 stepper_z not found - SG detection disabled")
        else:
            logging.info("ObstacleDetector: Connected to TMC5160 stepper_z")

        # Get toolhead for position monitoring
        self.toolhead = self.printer.lookup_object('toolhead', None)
        if self.toolhead is None:
            logging.warning("ObstacleDetector: Toolhead not found - position monitoring disabled")

    def _handle_ready(self):
        """Called when Klipper is fully ready"""
        logging.info("ObstacleDetector: Klipper ready, hybrid detection available")
        if self.tmc is not None:
            logging.info("  - StallGuard monitoring: ENABLED")
        else:
            logging.info("  - StallGuard monitoring: DISABLED")
        if self.toolhead is not None:
            logging.info("  - Position/timeout monitoring: ENABLED")

    def _set_tcoolthrs_for_homing(self):
        """Set TCOOLTHRS for StallGuard during homing - follows Klipper's TMCEndstopHelper pattern

        IMPORTANT: TCOOLTHRS is a WRITE-ONLY register on TMC5160!
        Reading it back returns garbage/SPI shift register value, not actual content.
        We trust set_register() works (it would throw exception on SPI failure).
        We verify StallGuard activation by checking TSTEP instead.
        """
        try:
            mcu_tmc = self.tmc.mcu_tmc
            fields = self.tmc.fields

            # Save current field cache value (not from register read - it's write-only)
            self.original_tcoolthrs = fields.get_field("tcoolthrs")
            logging.info(f"ObstacleDetector: Saving TCOOLTHRS cache value=0x{self.original_tcoolthrs:X}")

            # Set TCOOLTHRS to max value (0xFFFFF) to enable StallGuard at all velocities
            # Using the proper Klipper pattern: set_field returns full register value
            new_tcoolthrs = 0xFFFFF
            tc_val = fields.set_field("tcoolthrs", new_tcoolthrs)

            # Write to register - set_register() will throw exception if SPI fails
            mcu_tmc.set_register("TCOOLTHRS", tc_val)
            logging.info(f"ObstacleDetector: Set TCOOLTHRS=0x{new_tcoolthrs:X} (write-only, trusting SPI)")

        except Exception as e:
            logging.error(f"ObstacleDetector: Error setting TCOOLTHRS for homing: {e}")
            import traceback
            logging.error(traceback.format_exc())

    def _restore_tcoolthrs(self):
        """Restore TCOOLTHRS to original value after homing"""
        if self.original_tcoolthrs is None:
            return

        try:
            fields = self.tmc.fields
            mcu_tmc = self.tmc.mcu_tmc

            tc_val = fields.set_field("tcoolthrs", self.original_tcoolthrs)
            mcu_tmc.set_register("TCOOLTHRS", tc_val)
            logging.info(f"ObstacleDetector: Restored TCOOLTHRS to 0x{self.original_tcoolthrs:X}")
            self.original_tcoolthrs = None

        except Exception as e:
            logging.error(f"ObstacleDetector: Error restoring TCOOLTHRS: {e}")

    def _start_monitor(self, hmove):
        """Called when homing move begins"""
        if self.tmc is None:
            return

        # Check if this is Z-min homing (going DOWN toward the tank)
        # We only want to detect obstacles when going DOWN, not during ZMAX_HOME (going up)
        #
        # ZMAX_HOME uses 'y' endstop (but moves stepper_z) -> DON'T monitor
        # G28 Z / HOME_AXIS uses 'z' endstop -> DO monitor
        #
        # hmove.endstops is a list of (mcu_endstop, name) tuples
        is_z_min_homing = False
        for mcu_endstop, name in hmove.endstops:
            # Check if this is the Z endstop (name 'z' or 'stepper_z')
            # ZMAX_HOME uses 'y' endstop, so it won't match
            if name in ('z', 'stepper_z'):
                steppers = mcu_endstop.get_steppers()
                for stepper in steppers:
                    if stepper.get_name() == 'stepper_z':
                        is_z_min_homing = True
                        break
            if is_z_min_homing:
                break

        if not is_z_min_homing:
            return

        logging.info(f"ObstacleDetector: Starting timeout monitoring for Z homing")
        logging.info(f"  Expected time={self.expected_time:.1f}s (travel={self.expected_travel}mm @ {self.homing_speed}mm/s)")

        # Reset SG detection state
        self.monitoring = True
        self.low_sg_count = 0
        self.last_sg_values = []
        self.last_sg_reading = None
        self.obstacle_detected = False
        self.obstacle_sg_value = None
        self.detection_reason = None

        # Reset hybrid detection state
        self.homing_start_time = time.time()
        self.last_z_position = None
        self.stall_count = 0
        if self.toolhead:
            pos = self.toolhead.get_position()
            self.last_z_position = pos[2]
            logging.info(f"  Starting Z position: {self.last_z_position:.3f}mm")

        # Start polling timer with initial delay to allow motor to accelerate
        # At startup, velocity is 0 and StealthChop is active (SG=0)
        # We need to wait for the motor to reach homing speed and switch to SpreadCycle
        initial_delay = 0.3  # 300ms delay to allow acceleration
        reactor = self.printer.get_reactor()
        self.check_timer = reactor.register_timer(
            self._check_callback,
            reactor.monotonic() + initial_delay
        )
        logging.info(f"ObstacleDetector: Monitoring active after {initial_delay}s delay")

    def _stop_monitor(self, hmove):
        """Called when homing move ends"""
        if not self.monitoring:
            return

        self.monitoring = False
        if self.check_timer:
            self.printer.get_reactor().unregister_timer(self.check_timer)
            self.check_timer = None

        # Calculate elapsed time
        elapsed = time.time() - self.homing_start_time if self.homing_start_time else 0

        # Get final Z position for verification
        final_z = None
        if self.toolhead:
            pos = self.toolhead.get_position()
            final_z = pos[2]

        # Log final stats
        if self.last_sg_values:
            avg_sg = sum(self.last_sg_values) / len(self.last_sg_values)
            min_sg = min(self.last_sg_values)
            max_sg = max(self.last_sg_values)
            logging.info(f"ObstacleDetector: Homing completed in {elapsed:.2f}s")
            logging.info(f"  SG stats: avg={avg_sg:.0f}, min={min_sg}, max={max_sg}, samples={len(self.last_sg_values)}")
            if final_z is not None:
                logging.info(f"  Final Z position: {final_z:.3f}mm")

        # Verify if potential obstacle was a false positive
        #
        # IMPORTANT: When SG threshold detection triggers (SG=0), the endstop often
        # triggers shortly after due to vibration from hitting the obstacle.
        # This makes the final Z position unreliable for verification.
        #
        # New logic:
        # - sg_threshold detection: ALWAYS treat as real obstacle (SG=0 is very specific)
        # - timeout detection: Only treat as real if Z > threshold (motor might just be slow)
        if self.obstacle_detected:
            is_real_obstacle = False
            z_threshold = 5.0  # mm

            if self.detection_reason == "sg_threshold":
                # SG threshold detection is highly reliable - SG=0 means real stall
                is_real_obstacle = True
                logging.warning(f"ObstacleDetector: REAL OBSTACLE confirmed by StallGuard!")
                logging.warning(f"  Final Z={final_z:.3f}mm, SG dropped to threshold")
                logging.warning(f"  Flag file preserved for MicLoop: {self.flag_file}")
            elif self.detection_reason == "timeout":
                # Timeout detection - homing took longer than expected
                # This usually means an obstacle blocked the endstop from being reached
                #
                # IMPORTANT: We can't use Z position for verification because:
                # - When motor skips steps hitting an obstacle, Klipper loses position
                # - Klipper may think Z=1mm when physically it's at 30mm+
                # - The position is UNRELIABLE after step loss
                #
                # A timeout during homing almost always means something went wrong
                # (obstacle, mechanical issue, etc). Better to keep the flag and let
                # MicLoop handle it rather than clear a valid detection.
                is_real_obstacle = True
                logging.warning(f"ObstacleDetector: REAL OBSTACLE confirmed by timeout!")
                logging.warning(f"  Homing took {elapsed:.2f}s (expected {self.expected_time:.2f}s)")
                logging.warning(f"  Klipper position Z={final_z:.3f}mm (may be inaccurate due to step loss)")
                logging.warning(f"  Flag file preserved for MicLoop: {self.flag_file}")
            else:
                # Unknown detection reason - keep the flag to be safe
                is_real_obstacle = True
                logging.warning(f"ObstacleDetector: Unknown detection reason '{self.detection_reason}', keeping flag")

            if not is_real_obstacle:
                self.obstacle_detected = False
                self.detection_reason = None

        # Reset hybrid detection state
        self.homing_start_time = None
        self.last_z_position = None
        self.stall_count = 0

    def _check_callback(self, eventtime):
        """Timer callback for HYBRID obstacle detection

        Detection methods:
        1. SG threshold - TMC5160 StallGuard value drops below threshold
        2. Timeout - Homing takes longer than expected (obstacle blocking endstop)
        3. Position stall - Z position not changing despite motor commands
        """
        if not self.monitoring:
            return self.printer.get_reactor().NEVER

        try:
            # ===== METHOD 1: StallGuard Detection =====
            # Read DRV_STATUS register
            drv_status = self.tmc.mcu_tmc.get_register("DRV_STATUS")
            sg_result = drv_status & 0x3FF  # bits 0-9
            stallguard_flag = (drv_status >> 24) & 0x01
            stst = (drv_status >> 31) & 0x01  # standstill flag
            cs_actual = (drv_status >> 16) & 0x1F

            # Read TSTEP (readable!) to verify velocity
            tstep = self.tmc.mcu_tmc.get_register("TSTEP")
            tstep_val = tstep & 0xFFFFF  # 20-bit value

            # Log detailed debug info for first few samples
            if len(self.last_sg_values) < 5:
                elapsed = time.time() - self.homing_start_time if self.homing_start_time else 0
                logging.info(f"ObstacleDetector DEBUG: SG={sg_result} TSTEP={tstep_val} "
                           f"stst={stst} CS={cs_actual} elapsed={elapsed:.2f}s")

            # Skip SG check if motor is at standstill (not moving)
            if not stst:
                # Track values for statistics
                self.last_sg_values.append(sg_result)
                if len(self.last_sg_values) > 100:
                    self.last_sg_values.pop(0)
                self.last_sg_reading = sg_result

                # Check for obstacle by SG threshold
                if sg_result < self.sg_threshold:
                    self.low_sg_count += 1
                    if self.low_sg_count >= self.min_samples and not self.obstacle_detected:
                        logging.warning(f"ObstacleDetector: OBSTACLE by SG threshold "
                                      f"(SG={sg_result}, count={self.low_sg_count})")
                        self._handle_obstacle(sg_result, reason="sg_threshold")
                else:
                    if self.low_sg_count > 0:
                        self.low_sg_count = 0

            # ===== METHOD 2: Timeout Detection =====
            if self.homing_start_time:
                elapsed = time.time() - self.homing_start_time
                if elapsed > self.expected_time and not self.obstacle_detected:
                    logging.warning(f"ObstacleDetector: OBSTACLE by timeout "
                                  f"(elapsed={elapsed:.2f}s > expected={self.expected_time:.2f}s)")
                    self._handle_obstacle(sg_result if self.last_sg_reading else 0, reason="timeout")

            # ===== METHOD 3: Position Stall Detection =====
            # NOTE: Position stall detection is DISABLED during homing because
            # toolhead.get_position() doesn't update reliably during drip moves.
            # We rely on SG threshold and timeout detection instead.
            # This method is kept for future use with regular moves.
            #
            # if self.toolhead and self.last_z_position is not None:
            #     ... position stall detection code ...

        except Exception as e:
            logging.error(f"ObstacleDetector: Error in check callback: {e}")

        return eventtime + self.check_interval

    def _handle_obstacle(self, sg_value, reason="unknown"):
        """Handle detected obstacle - mark for later verification

        We DON'T shutdown immediately because SG=0 also occurs when the
        Z-min endstop is triggered (normal homing completion). Instead:
        1. Create a flag file
        2. Let homing continue
        3. If homing completes normally (endstop triggered), the flag will be
           cleared in _stop_monitor
        4. If homing times out (obstacle blocked endstop), flag persists for MicLoop

        Detection reasons:
        - sg_threshold: StallGuard value dropped below threshold
        - timeout: Homing took longer than expected
        - position_stall: Z position not changing despite motor running
        """
        # Don't stop monitoring - let homing continue
        # The flag will be verified in _stop_monitor

        # Calculate elapsed time for diagnostics
        elapsed = time.time() - self.homing_start_time if self.homing_start_time else 0

        # Create flag file for potential obstacle
        try:
            with open(self.flag_file, 'w') as f:
                f.write(f'OBSTACLE:{reason}:SG={sg_value}:elapsed={elapsed:.2f}s')
            logging.warning(f"ObstacleDetector: Potential obstacle detected by {reason} "
                           f"(SG={sg_value}, elapsed={elapsed:.2f}s), flag created")
        except Exception as e:
            logging.error(f"ObstacleDetector: Error creating flag file: {e}")

        # Mark that we detected something (for verification in _stop_monitor)
        self.obstacle_detected = True
        self.obstacle_sg_value = sg_value
        self.detection_reason = reason

    cmd_QUERY_SG_help = "Query current StallGuard value from TMC5160"
    def cmd_QUERY_SG(self, gcmd):
        """Manual command to query SG_RESULT"""
        if self.tmc is None:
            raise gcmd.error("TMC5160 stepper_z not found")

        try:
            drv_status = self.tmc.mcu_tmc.get_register("DRV_STATUS")
            sg_result = drv_status & 0x3FF
            stallguard_flag = (drv_status >> 24) & 0x01
            cs_actual = (drv_status >> 16) & 0x1F
            stst = (drv_status >> 31) & 0x01

            gcmd.respond_info(f"TMC5160 stepper_z Status:")
            gcmd.respond_info(f"  SG_RESULT: {sg_result} (threshold: {self.sg_threshold})")
            gcmd.respond_info(f"  STALLGUARD flag: {stallguard_flag}")
            gcmd.respond_info(f"  CS_ACTUAL: {cs_actual}")
            gcmd.respond_info(f"  Standstill: {stst}")
            gcmd.respond_info(f"  Raw DRV_STATUS: 0x{drv_status:08X}")
        except Exception as e:
            raise gcmd.error(f"Error reading DRV_STATUS: {e}")

    cmd_SET_SG_THRESHOLD_help = "Set StallGuard threshold for obstacle detection"
    def cmd_SET_SG_THRESHOLD(self, gcmd):
        """Set the SG threshold dynamically"""
        threshold = gcmd.get_int('THRESHOLD', None)
        if threshold is None:
            gcmd.respond_info(f"Current SG threshold: {self.sg_threshold}")
            return

        if threshold < 0 or threshold > 1023:
            raise gcmd.error("Threshold must be between 0 and 1023")

        old_threshold = self.sg_threshold
        self.sg_threshold = threshold
        gcmd.respond_info(f"SG threshold changed: {old_threshold} -> {self.sg_threshold}")

    cmd_SG_MONITOR_TEST_help = "Test SG monitoring for specified duration"
    def cmd_SG_MONITOR_TEST(self, gcmd):
        """Test command to monitor SG for a duration without homing"""
        if self.tmc is None:
            raise gcmd.error("TMC5160 stepper_z not found")

        duration = gcmd.get_float('DURATION', 5.0)
        gcmd.respond_info(f"Monitoring SG_RESULT for {duration}s...")
        gcmd.respond_info("Move Z axis manually or run a G1 Z command to see values")

        # Start a simple monitoring loop
        import time
        start_time = time.time()
        values = []

        while time.time() - start_time < duration:
            try:
                drv_status = self.tmc.mcu_tmc.get_register("DRV_STATUS")
                sg_result = drv_status & 0x3FF
                stst = (drv_status >> 31) & 0x01

                if not stst:  # Only log when moving
                    values.append(sg_result)
                    gcmd.respond_info(f"SG={sg_result}")

                time.sleep(0.1)
            except:
                pass

        if values:
            gcmd.respond_info(f"Results: min={min(values)}, max={max(values)}, "
                            f"avg={sum(values)/len(values):.0f}, samples={len(values)}")
        else:
            gcmd.respond_info("No movement detected during test")

    cmd_ENABLE_STALLGUARD_help = "Enable StallGuard by setting TCOOLTHRS"
    def cmd_ENABLE_STALLGUARD(self, gcmd):
        """Command to enable/re-enable StallGuard

        Note: TCOOLTHRS is WRITE-ONLY. We cannot verify by read-back.
        We trust set_register() works (would throw exception on SPI failure).
        """
        if self.tmc is None:
            raise gcmd.error("TMC5160 stepper_z not found")

        tcoolthrs = gcmd.get_int('TCOOLTHRS', self.tcoolthrs)

        try:
            # Use fields API for proper register setting
            fields = self.tmc.fields
            old_cached = fields.get_field("tcoolthrs")
            tc_val = fields.set_field("tcoolthrs", tcoolthrs)

            # Get synchronized print time
            toolhead = self.printer.lookup_object('toolhead')
            print_time = toolhead.get_last_move_time()

            # Write to register (TCOOLTHRS is write-only, cannot verify by read)
            self.tmc.mcu_tmc.set_register("TCOOLTHRS", tc_val, print_time)

            gcmd.respond_info(f"TCOOLTHRS set: 0x{old_cached:X} -> 0x{tcoolthrs:X}")
            gcmd.respond_info(f"Note: TCOOLTHRS is write-only, cannot verify by read-back")
            gcmd.respond_info(f"Run QUERY_TMC_REGS while motor is moving to verify SG is working")
        except Exception as e:
            import traceback
            gcmd.respond_info(f"Error: {traceback.format_exc()}")
            raise gcmd.error(f"Error setting TCOOLTHRS: {e}")

    cmd_QUERY_TMC_REGS_help = "Query TMC5160 registers relevant to StallGuard"
    def cmd_QUERY_TMC_REGS(self, gcmd):
        """Query TMC5160 registers for debugging"""
        if self.tmc is None:
            raise gcmd.error("TMC5160 stepper_z not found")

        try:
            # Read key registers (only readable ones!)
            drv_status = self.tmc.mcu_tmc.get_register("DRV_STATUS")
            tstep = self.tmc.mcu_tmc.get_register("TSTEP")  # Readable: current velocity
            chopconf = self.tmc.mcu_tmc.get_register("CHOPCONF")
            gconf = self.tmc.mcu_tmc.get_register("GCONF")

            # Get cached values for write-only registers
            fields = self.tmc.fields
            tcoolthrs_cached = fields.get_field("tcoolthrs")
            tpwmthrs_cached = fields.get_field("tpwmthrs")
            sgt_cached = fields.get_field("sgt")

            # Parse DRV_STATUS
            sg_result = drv_status & 0x3FF
            cs_actual = (drv_status >> 16) & 0x1F
            stallguard = (drv_status >> 24) & 0x01
            stst = (drv_status >> 31) & 0x01

            # Parse TSTEP
            tstep_val = tstep & 0xFFFFF

            # Parse CHOPCONF
            toff = chopconf & 0x0F
            mres = (chopconf >> 24) & 0x0F

            # Parse GCONF - TMC5160 uses en_spreadcycle (inverted logic from en_pwm_mode)
            en_spreadcycle = (gconf >> 2) & 0x01  # bit 2: en_spreadcycle
            stealthchop_active = not en_spreadcycle

            # StallGuard is active when: motor moving AND TSTEP <= TCOOLTHRS AND not in StealthChop
            # Note: Higher TSTEP = lower velocity, so TSTEP <= TCOOLTHRS means "velocity below threshold"
            sg_should_work = (not stst) and (tstep_val <= tcoolthrs_cached) and (not stealthchop_active)

            gcmd.respond_info("TMC5160 StallGuard Configuration:")
            gcmd.respond_info(f"  TCOOLTHRS (cached): 0x{tcoolthrs_cached:X} ({tcoolthrs_cached}) [WRITE-ONLY]")
            gcmd.respond_info(f"  TPWMTHRS (cached): 0x{tpwmthrs_cached:X} ({tpwmthrs_cached})")
            gcmd.respond_info(f"  SGT (cached): {sgt_cached}")
            gcmd.respond_info(f"  TSTEP (live): {tstep_val} (lower=faster)")
            gcmd.respond_info(f"  GCONF: en_spreadcycle={en_spreadcycle} -> StealthChop {'OFF' if en_spreadcycle else 'ON'}")
            gcmd.respond_info(f"  CHOPCONF: TOFF={toff}, MRES={mres} ({256 >> mres} usteps)")
            gcmd.respond_info(f"DRV_STATUS (live):")
            gcmd.respond_info(f"  SG_RESULT: {sg_result}")
            gcmd.respond_info(f"  CS_ACTUAL: {cs_actual}")
            gcmd.respond_info(f"  STALLGUARD flag: {stallguard}")
            gcmd.respond_info(f"  Standstill: {stst}")
            gcmd.respond_info(f"  Raw: 0x{drv_status:08X}")
            gcmd.respond_info(f"Analysis: SG should work = {sg_should_work}")
            if stst:
                gcmd.respond_info(f"  -> Motor at standstill")
            if stealthchop_active:
                gcmd.respond_info(f"  -> StealthChop active (SG disabled)")
            if tstep_val > tcoolthrs_cached:
                gcmd.respond_info(f"  -> TSTEP > TCOOLTHRS (velocity too low for SG)")

        except Exception as e:
            raise gcmd.error(f"Error reading registers: {e}")


def load_config(config):
    return ObstacleDetector(config)
