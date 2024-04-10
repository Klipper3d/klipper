# Support for servos
#
# Copyright (C) 2017,2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

SERVO_SIGNAL_PERIOD = 0.020
PIN_MIN_TIME = 0.100

class PrinterServo:
    def __init__(self, config):
        self.printer = config.get_printer()
        ppins = self.printer.lookup_object('pins')
        self.mcu_servo = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu_servo.setup_max_duration(0.)
        self.mcu_servo.setup_cycle_time(SERVO_SIGNAL_PERIOD)
        self.min_width = config.getfloat(
            'minimum_pulse_width', .001, above=0., below=SERVO_SIGNAL_PERIOD)
        self.max_width = config.getfloat(
            'maximum_pulse_width', .002
            , above=self.min_width, below=SERVO_SIGNAL_PERIOD)
        self.max_angle = config.getfloat('maximum_servo_angle', 180.)
        self.angle_to_width = (self.max_width - self.min_width) / self.max_angle
        self.width_to_value = 1. / SERVO_SIGNAL_PERIOD
        self.last_value = self.last_value_time = 0.
        self.enable = config.getboolean('enable', True)
        self.last_enable = not self.enable
        servo_name = config.get_name().split()[1]
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command("SET_SERVO", "SERVO", servo_name,
                                        self.cmd_SET_SERVO,
                                        desc=self.cmd_SET_SERVO_help)
        # Check to see if an initial angle or pulse width is
        # configured and set it as required
        self.initial_pwm_value = None
        initial_angle = config.getfloat('initial_angle', None,
                                        minval=0., maxval=360.)
        if initial_angle is not None:
            self.initial_pwm_value = self._get_pwm_from_angle(initial_angle)
        else:
            initial_pulse_width = config.getfloat('initial_pulse_width', None,
                                                  minval=self.min_width,
                                                  maxval=self.max_width)
            if initial_pulse_width is not None:
                self.initial_pwm_value = self._get_pwm_from_pulse_width(
                    initial_pulse_width)
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
    def handle_ready(self):
        if self.initial_pwm_value is not None:
            toolhead = self.printer.lookup_object('toolhead')
            print_time = toolhead.get_last_move_time()
            self._set_pwm(print_time, self.initial_pwm_value)
    def get_status(self, eventtime):
        return {'value': self.last_value}
    def _set_pwm(self, print_time, value):
        if value == self.last_value and self.enable == self.last_enable:
            return
        print_time = max(print_time, self.last_value_time + PIN_MIN_TIME)
        if self.enable:
          self.mcu_servo.set_pwm(print_time, value)
        else:
          self.mcu_servo.set_pwm(print_time, 0)
        self.last_value = value
        self.last_enable = self.enable
        self.last_value_time = print_time
    def _get_pwm_from_angle(self, angle):
        angle = max(0., min(self.max_angle, angle))
        width = self.min_width + angle * self.angle_to_width
        return width * self.width_to_value
    def _get_pwm_from_pulse_width(self, width):
        width = max(self.min_width, min(self.max_width, width))
        return width * self.width_to_value
    cmd_SET_SERVO_help = "Set servo angle"
    def cmd_SET_SERVO(self, params):
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        if 'ENABLE' in params:
            value = self.gcode.get_int('ENABLE', params)
            self.enable = value != 0
        if 'WIDTH' in params:
            self._set_pwm(print_time, self._get_pwm_from_pulse_width(
                self.gcode.get_float('WIDTH', params)))
        else:
            if 'ANGLE' in params:
                self._set_pwm(print_time, self._get_pwm_from_angle(
                    self.gcode.get_float('ANGLE', params)))
            else:
                self._set_pwm(print_time, self.last_value)

def load_config_prefix(config):
    return PrinterServo(config)
