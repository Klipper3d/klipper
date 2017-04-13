# Printer servo support
#
# Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# All time units are specified in Seconds
#

# Servo Fine Tuning: Specify the movement range (deg) & pulse width range (s)
MAX_ANGLE       = 180.
MIN_PULSE_WIDTH = 0.0008
MAX_PULSE_WIDTH = 0.0020
SIGNAL_WIDTH        = MAX_PULSE_WIDTH - MIN_PULSE_WIDTH
DEGREES_PER_SECOND  = MAX_ANGLE / SIGNAL_WIDTH

class PrinterServo:
    def __init__(self, printer, config):
        self.last_pulsewidth=-1
        self.mcu_servo = printer.mcu.create_pwm(
            config.get('servo_pin'), 0.020, 0, 0.)

    # External commands
    def set_pulsewidth(self, print_time, pulsewidth):
        pulsewidth=max(MIN_PULSE_WIDTH, min(MAX_PULSE_WIDTH, pulsewidth))
        if pulsewidth == self.last_pulsewidth: return
        dutycycle = pulsewidth / 0.020
        mcu_time=self.mcu_servo.print_to_mcu_time(print_time)
        self.mcu_servo.set_pwm(mcu_time, dutycycle)
        self.last_pulsewidth=pulsewidth

    def set_angle(self, print_time, angle):
        angle = max(0, min(MAX_ANGLE, angle))
        pulsewidth = MIN_PULSE_WIDTH + (angle / DEGREES_PER_SECOND)
        self.set_pulsewidth(print_time, pulsewidth)