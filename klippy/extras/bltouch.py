# BLTouch support
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import homing, probe

SIGNAL_PERIOD = 0.020

CMD_TIME = 0.150
DEPLOY_CMD_TIME = 0.750
STOW_CMD_TIME = 0.750
RESET_CMD_TIME = 0.500

ENDSTOP_REST_TIME = .001
ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 3

Commands = {
    None: 0.0, 'RESET': 0.002194, 'SELFTEST': 0.001782,
    'DEPLOY': 0.000647, 'STOW': 0.001473,
    'DEPLOY_CHECK': 0.0, 'STOW_CHECK': 0.0,
    'SET_SW_MODE': 0.001162,
    'SET_5V_MODE' : 0.001988, 'SET_OD_MODE' : 0.002091, 'MODE_STORE' : 0.001884,
    'STORE_5V' : 0.0, 'STORE_OD' : 0.0
}

Times = {
    None: 0.0, 'RESET': RESET_CMD_TIME, 'SELFTEST': CMD_TIME,
    'DEPLOY': DEPLOY_CMD_TIME, 'STOW': STOW_CMD_TIME,
    'DEPLOY_CHECK': 0.0, 'STOW_CHECK': 0.0,
    'SET_SW_MODE': CMD_TIME,
    'SET_5V_MODE' : CMD_TIME, 'SET_OD_MODE' : CMD_TIME, 'MODE_STORE' : CMD_TIME,
    'STORE_5V' : 0.0, 'STORE_OD' : 0.0
}

# "BLTOUCH_DEBUG COMMAND=" commands

# BLTouch native commands:
# RESET, SELFTEST, DEPLOY, STOW, SET_SW_MODE,
# SET_5V_MODE, SET_OD_MODE, MODE_STORE

# BLTouch derived commands:
# DEPLOY_CHECK    - Perform a deploy with all the checks as in this module
# STOW_CHECK      - Perform a stow with all the checks as in this module
# STORE_5V        - Perform needed sequence to store 5V mode in a BLTouch V3.1
# STORE_OD        - Perform needed sequence to store OD mode in a BLTouch V3.1

# *** Special directives in the [bltouch] section of the printer.cfg:
#
# high_speed_mode: [2]
#   0 = no high-speed
#   1 = simultaneous stow and toolhead raise
#   2 = no intermediate deploy and stow
#
# retract_speed: [speed]
#   Retract at a faster speed than when going down
#   The maximum is the max z axis speed you have defined
#
# min_cmd_time: [0.150]
#   you can specify an absolute minimum cmd time
#
# test_sensor_before_use [False]
#   set this to false if probe does not support it
#
# heaters_off: [False]
#   turn heaters off during bltouch probing
#
# use_sw_mode: [False]
#   choose to use SW Mode if you want instead of normal mode
#
# set_mode: [none]
#   set_mode: none=do nothing, 5V=set 5V mode, OD=set OD mode
#   Only use a value other than "none" if it is a V3.0 or V3.1 probe and
#   you are SURE that you need a setting other than the BLTouch default
#   of OD mode on your printer board. If you use 5V mode, be SURE that
#   your board is 5V tolerant on the BLTouch signal pin input. If in any
#   doubt, use "none".

# *** Special directives in the [stepper_z] section of the printer.cfg:
#
# homing_retract_speed: [homing_speed]
#   Retract at a faster speed than when going down
#   The maximum is the max z axis speed you have defined

#BLTouch "endstop" wrapper
class BLTouchEndstopWrapper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.position_endstop = config.getfloat('z_offset')
        # Create a pwm object to handle the control pin
        ppins = self.printer.lookup_object('pins')
        self.mcu_pwm = ppins.setup_pin('pwm', config.get('control_pin'))
        self.mcu_pwm.setup_max_duration(0.)
        self.mcu_pwm.setup_cycle_time(SIGNAL_PERIOD)
        self.next_cmd_time = 0.
        # Create an "endstop" object to handle the sensor pin
        pin = config.get('sensor_pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        mcu = pin_params['chip']
        mcu.register_config_callback(self._build_config)
        self.mcu_endstop = mcu.setup_pin('endstop', pin_params)
        # high speed mode
        self.hsmode = config.getint('high_speed_mode', 2, 0, 2)
        self.swmode = config.getboolean('use_sw_mode', False)
        self.heaters_off = config.getboolean('heaters_off', False)
        # output mode
        self.mode = config.get('set_mode', 'none')
        # Setup for sensor test
        self.test_sensor_before_use = config.getboolean(
            'test_sensor_before_use', True)
        self.start_mcu_pos = []
        # Calculate user requested minimum cmd time
        self.min_cmd_time = max(config.getfloat('min_cmd_time', 0.650),
                                CMD_TIME)
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        self.TimeoutError = self.mcu_endstop.TimeoutError
        # Register BLTOUCH_DEBUG command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("BLTOUCH_DEBUG", self.cmd_BLTOUCH_DEBUG,
                                    desc=self.cmd_BLTOUCH_DEBUG_help)
    def _build_config(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers('Z'):
            self.add_stepper(stepper)
    def sync_mcu_print_time(self):
        curtime = self.printer.get_reactor().monotonic()
        est_time = self.mcu_pwm.get_mcu().estimated_print_time(curtime)
        self.next_cmd_time = max(self.next_cmd_time,
                                 est_time + self.min_cmd_time)
    def sync_print_time(self):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        if self.next_cmd_time > print_time:
            toolhead.dwell(self.next_cmd_time - print_time)
        else:
            self.next_cmd_time = print_time
    def send_cmd(self, cmd, time_override=0):
        self.mcu_pwm.set_pwm(self.next_cmd_time, Commands[cmd] / SIGNAL_PERIOD)
        time = Times[cmd]
        if time_override > 0:
            time = time_override
        time = max(time, self.min_cmd_time)
        time = math.ceil(time / SIGNAL_PERIOD) * SIGNAL_PERIOD
        # Translate time to ticks to avoid any secondary mcu clock skew
        mcu = self.mcu_pwm.get_mcu()
        cmd_clock = mcu.print_time_to_clock(self.next_cmd_time)
        cmd_clock += mcu.seconds_to_clock(time)
        self.next_cmd_time = mcu.clock_to_print_time(cmd_clock)
        return self.next_cmd_time
    def verify_state(self, check_start_time, check_end_time, triggered, msg):
        # Perform endstop check to verify bltouch reports desired state
        self.mcu_endstop.home_start(check_start_time, ENDSTOP_SAMPLE_TIME,
                                    ENDSTOP_SAMPLE_COUNT, ENDSTOP_REST_TIME,
                                    triggered=triggered)
        try:
            self.mcu_endstop.home_wait(check_end_time)
        except self.mcu_endstop.TimeoutError as e:
            logging.warning("BLTouch probe verify status failed.")
            raise homing.EndstopError("BLTouch failed to %s" % (msg,))
    def handle_connect(self):
        if self.mode != 'none':
            logging.info("BLTouch probe set mode: %s", self.mode)
            self.set_mode(self.mode)
        self.test_sensor()
        try:
            self.stow_probe()
        except homing.CommandError as e:
            logging.warning("BLTouch probe stow error: %s", str(e))
    def test_sensor(self):
        if not self.test_sensor_before_use:
            return
        # Raise the bltouch probe and test if probe is raised
        self.sync_print_time()
        check_start_time = self.send_cmd('RESET')
        self.send_cmd('STOW')
        # SW ("switch") MODE: If the pin is up, it should be triggered
        self.send_cmd('SET_SW_MODE')
        check_end_time = self.send_cmd(None)
        try:
            self.verify_state(check_start_time, check_end_time, True,
                              "BLTouch test failed.")
        except:
            logging.error("BLTouch probe seems unusable.")
        else:
            logging.info("BLTouch probe usable.")
        self.sync_print_time()
    def stow_probe(self, nocheck=False):
        logging.info("BLTouch probe stow.")
        self.sync_print_time()
        check_start_time = self.send_cmd('STOW')
        check_end_time = self.send_cmd(None)
        self.sync_print_time()
        if nocheck:
            return
        try:
            self.verify_state(check_start_time, check_end_time,
                False, "stow probe")
        except homing.CommandError as e:
            logging.info("BLTouch probe stow failed, trying to repair.")
            self.sync_print_time()
            self.send_cmd('RESET')
            check_start_time = self.send_cmd('STOW')
            check_end_time = self.send_cmd(None)
            self.sync_print_time()
            try:
                self.verify_state(check_start_time, check_end_time,
                    False, "stow probe")
            except homing.CommandError as e:
                msg = "BLTouch probe stow failed."
                self.gcode.respond_info(msg)
                raise
            else:
                logging.info("BLTouch probe stow succeeded after fail.")
        else:
            logging.info("BLTouch probe stow succeeded.")
        self.sync_print_time()
    def deploy_probe(self):
        logging.info("BLTouch probe deploy.")
        self.sync_print_time()
        check_start_time = self.send_cmd('DEPLOY')
        check_end_time = self.send_cmd(None)
        self.sync_print_time()
        try:
            self.verify_state(check_start_time, check_end_time,
                False, "deploy probe")
        except homing.CommandError as e:
            logging.info("BLTouch probe deploy failed, trying to repair.")
            self.sync_print_time()
            self.send_cmd('RESET')
            self.send_cmd('STOW')
            self.send_cmd('DEPLOY')
            self.send_cmd('STOW')
            check_start_time = self.send_cmd('DEPLOY')
            check_end_time = self.send_cmd(None)
            self.sync_print_time()
            try:
                self.verify_state(check_start_time, check_end_time,
                    False, "deploy probe")
            except homing.CommandError as e:
                msg = "BLTouch probe deploy failed."
                self.gcode.respond_info(msg)
                raise
            else:
                logging.info("BLTouch probe deploy succeeded after fail.")
        else:
            logging.info("BLTouch probe deploy succeeded.")
        if self.swmode:
            self.sync_print_time()
            self.send_cmd('SET_SW_MODE')
            self.send_cmd(None)
        self.sync_print_time()
    def set_mode(self, mode):
        # BLTOUCH pre V3.0 and clones:
        #   No reaction at all to this sequence apart from a DEPLOY -> STOW
        # BLTOUCH V3.0:
        #   This will set the mode (twice) and sadly, a STOW is needed at
        #   the end, because of the deploy
        # BLTOUCH V3.1:
        #   This will set the mode and store it in the eeprom.
        #   The STOW is not needed but does not hurt
        self.sync_print_time()
        self.send_cmd('DEPLOY')
        if mode == '5V':
            self.send_cmd('SET_5V_MODE')
        else:
            self.send_cmd('SET_OD_MODE')
        self.send_cmd('MODE_STORE')
        if mode == '5V':
            self.send_cmd('SET_5V_MODE')
        else:
            self.send_cmd('SET_OD_MODE')
        self.send_cmd('STOW')
        self.sync_print_time()
    def home_prepare(self):
        self.mcu_endstop.home_prepare()
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        self.start_mcu_pos = [(s, s.get_mcu_position())
                              for s in self.mcu_endstop.get_steppers()]
    def home_finalize(self):
        self.mcu_endstop.home_finalize()
    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   notify=None):
        rest_time = min(rest_time, ENDSTOP_REST_TIME)
        self.mcu_endstop.home_start(print_time, sample_time, sample_count,
                                    rest_time, notify=notify)
    def get_position_endstop(self):
        return self.position_endstop
    cmd_BLTOUCH_DEBUG_help = "Send a command to the bltouch for debugging"
    def cmd_BLTOUCH_DEBUG(self, params):
        cmd = self.gcode.get_str('COMMAND', params, None)
        if cmd is None or cmd not in Commands:
            self.gcode.respond_info("BLTouch commands: %s" % (
                ", ".join(sorted([c for c in Commands if c is not None]))))
            return
        self.gcode.respond_info("Sending BLTOUCH_DEBUG COMMAND=%s" % (cmd,))
        self.sync_print_time()
        if cmd == 'STOW_CHECK':
            self.stow_probe()
        elif cmd == 'DEPLOY_CHECK':
            self.deploy_probe()
        elif cmd == 'STORE_5V':
            self.set_mode('5V')
        elif cmd == 'STORE_OD':
            self.set_mode('OD')
        else:
            self.send_cmd(cmd)
        self.send_cmd(None)
        self.sync_print_time()

def load_config(config):
    blt = BLTouchEndstopWrapper(config)
    config.get_printer().add_object('probe', probe.PrinterProbe(config, blt))
    return blt
