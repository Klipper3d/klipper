# Support for servos
#
# Copyright (C) 2017-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import output_pin

SERVO_SIGNAL_PERIOD = 0.020

class PrinterServo:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.min_width = config.getfloat('minimum_pulse_width', .001,
                                         above=0., below=SERVO_SIGNAL_PERIOD)
        self.max_width = config.getfloat('maximum_pulse_width', .002,
                                         above=self.min_width,
                                         below=SERVO_SIGNAL_PERIOD)
        self.max_angle = config.getfloat('maximum_servo_angle', 180.)
        self.angle_to_width = (self.max_width - self.min_width) / self.max_angle
        self.width_to_value = 1. / SERVO_SIGNAL_PERIOD
        self.last_value = 0.
        initial_pwm = 0.
        iangle = config.getfloat('initial_angle', None, minval=0., maxval=360.)
        if iangle is not None:
            initial_pwm = self._get_pwm_from_angle(iangle)
        else:
            iwidth = config.getfloat('initial_pulse_width', 0.,
                                     minval=0., maxval=self.max_width)
            initial_pwm = self._get_pwm_from_pulse_width(iwidth)
        # Setup mcu_servo pin
        ppins = self.printer.lookup_object('pins')
        self.mcu_servo = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu_servo.setup_max_duration(0.)
        self.mcu_servo.setup_cycle_time(SERVO_SIGNAL_PERIOD)
        self.mcu_servo.setup_start_value(initial_pwm, 0.)
        # Create gcode request queue
        self.gcrq = output_pin.GCodeRequestQueue(
            config, self.mcu_servo.get_mcu(), self._set_pwm)
        # Register commands
        servo_name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_SERVO", "SERVO", servo_name,
                                   self.cmd_SET_SERVO,
                                   desc=self.cmd_SET_SERVO_help)
    def get_status(self, eventtime):
        return {'value': self.last_value}
    def _set_pwm(self, print_time, value):
        if value != self.last_value:
            self.last_value = value
            self.mcu_servo.set_pwm(print_time, value)
        return (True, 0.)
    def _get_pwm_from_angle(self, angle):
        angle = max(0., min(self.max_angle, angle))
        width = self.min_width + angle * self.angle_to_width
        return width * self.width_to_value
    def _get_pwm_from_pulse_width(self, width):
        if width:
            width = max(self.min_width, min(self.max_width, width))
        return width * self.width_to_value
    cmd_SET_SERVO_help = "Set servo angle"
    def cmd_SET_SERVO(self, gcmd):
        width = gcmd.get_float('WIDTH', None)
        if width is not None:
            value = self._get_pwm_from_pulse_width(width)
        else:
            angle = gcmd.get_float('ANGLE')
            value = self._get_pwm_from_angle(angle)
        self.gcrq.queue_gcode_request(value)

def load_config_prefix(config):
    return PrinterServo(config)
