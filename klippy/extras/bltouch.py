# BLTouch support
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import homing, probe

SIGNAL_PERIOD = 0.025600
PIN_MOVE_TIME = 8 * SIGNAL_PERIOD
MIN_CMD_TIME = 4 * SIGNAL_PERIOD

TEST_TIME = 5 * 60.
ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 4

CMD_PIN_DOWN = 0.000700
CMD_PIN_UP = 0.001500
CMD_SELF_TEST = 0.001800
CMD_RESET = 0.002200
CMD_TOUCH_MODE = 0.001200

# BLTouch "endstop" wrapper
class BLTouchEndstopWrapper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.next_test_time = 0.
        self.position_endstop = config.getfloat('z_offset')
        # Create a pwm object to handle the control pin
        ppins = self.printer.lookup_object('pins')
        self.mcu_pwm = ppins.setup_pin('pwm', config.get('control_pin'))
        self.mcu_pwm.setup_max_duration(0.)
        self.mcu_pwm.setup_cycle_time(SIGNAL_PERIOD)
        # Create an "endstop" object to handle the sensor pin
        pin = config.get('sensor_pin')
        pin_params = ppins.lookup_pin(pin, can_invert=True, can_pullup=True)
        mcu = pin_params['chip']
        mcu.register_config_callback(self._build_config)
        self.mcu_endstop = mcu.setup_pin('endstop', pin_params)
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        self.query_endstop_wait = self.mcu_endstop.query_endstop_wait
        self.TimeoutError = self.mcu_endstop.TimeoutError
    def _build_config(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers('Z'):
            stepper.add_to_endstop(self)
    def send_cmd(self, print_time, cmd):
        self.mcu_pwm.set_pwm(print_time, cmd / SIGNAL_PERIOD)
    def test_sensor(self):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        if print_time < self.next_test_time:
            self.next_test_time = print_time + TEST_TIME
            return
        self.send_cmd(print_time, CMD_RESET)
        home_time = print_time + PIN_MOVE_TIME
        self.send_cmd(home_time, CMD_TOUCH_MODE)
        self.send_cmd(home_time + MIN_CMD_TIME, 0.)
        self.mcu_endstop.home_start(
            home_time, ENDSTOP_SAMPLE_TIME, ENDSTOP_SAMPLE_COUNT, .001)
        try:
            self.mcu_endstop.home_wait(home_time + MIN_CMD_TIME)
        except self.mcu_endstop.TimeoutError as e:
            raise homing.EndstopError("BLTouch sensor test failed")
        self.next_test_time = home_time + TEST_TIME
        toolhead.reset_print_time(home_time + 2. * MIN_CMD_TIME)
    def home_prepare(self):
        self.test_sensor()
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        self.send_cmd(print_time, CMD_PIN_DOWN)
        self.send_cmd(print_time + PIN_MOVE_TIME, CMD_TOUCH_MODE)
        toolhead.dwell(PIN_MOVE_TIME + MIN_CMD_TIME)
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        self.send_cmd(print_time, CMD_RESET)
        self.send_cmd(print_time + PIN_MOVE_TIME, 0.)
        toolhead.dwell(PIN_MOVE_TIME + MIN_CMD_TIME)
        self.mcu_endstop.home_finalize()
    def get_position_endstop(self):
        return self.position_endstop

def load_config(config):
    blt = BLTouchEndstopWrapper(config)
    config.get_printer().add_object('probe', probe.PrinterProbe(config, blt))
    return blt
