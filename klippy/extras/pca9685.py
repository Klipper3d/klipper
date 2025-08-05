# pca9685.py
# PCA9685 PWM controller support for Klipper, providing virtual PWM pins
# Inspired by extras/servo.py, extras/output_pin.py, and klippy/mcu.py
#
# Copyright (C) 2025  Maja Stanislawska (maja@makershop.ie)
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from . import bus
import logging

MIN_SCHEDULE_TIME = 0.100
MAX_NOMINAL_DURATION = 3.0

class PCA9685Controller:
    def __init__(self, config):
        self._printer = config.get_printer()
        self._reactor = self._printer.get_reactor()
        self.name = config.get_name().split()[-1]
        self._i2c_addr = config.getint(
            'i2c_address', 0x40, minval=0x40, maxval=0x7F)
        self._frequency = config.getfloat(
            'frequency', 50.0, minval=1.0, maxval=1000.0)
        self._pwm_max = 4096.0 #used in few places to clamp output
        self._invert_output = config.getboolean('invert_output', False)
        self._totem_pole = config.getboolean('totem_pole', True)
        # Initialize I2C
        self._i2c = bus.MCU_I2C_from_config(config, self._i2c_addr)
        self._mcu = self._i2c.get_mcu()
        self._printer.add_object(self.name, self)
        self._ppins = self._printer.lookup_object("pins")
        self._ppins.register_chip(self.name, self)
        self._pins = {}
        self._flush_callbacks = []
        self._printer.register_event_handler(
            "klippy:connect", self._handle_connect)
        self._printer.register_event_handler(
            "klippy:shutdown", self._handle_shutdown)

    def _handle_connect(self):
        self._init_pca9685()
        # Set initial values for registered pins
        for pin in self._pins.values():
            pin._set_pwm_value(pin._start_value)
        #inject yourself into tooolheads mcu list
        self.toolhead = self._printer.lookup_object('toolhead')
        self.toolhead.all_mcus.append(self)

    def _handle_shutdown(self):
        for pin in self._pins.values():
            pin._set_pwm_value(pin._shutdown_value)

    def min_schedule_time(self):
        return MIN_SCHEDULE_TIME

    def max_nominal_duration(self):
        return MAX_NOMINAL_DURATION

    def check_active(self, print_time, eventtime):
        return

    def _init_pca9685(self):
        # Full reset: MODE1 = 0x01 (ALLCALL, no SLEEP, restart enabled)
        self._i2c.i2c_write_wait_ack([0x00, 0x01])
        self._reactor.pause(self._reactor.monotonic() + .01)
        # Set frequency
        prescale = round(25000000.0 / (4096.0 * self._frequency)) - 1
        if prescale < 3 or prescale > 255:
            raise self._printer.config_error(
                "PCA9685 %s: Frequency out of range"%(self.name,))
        self._i2c.i2c_write_wait_ack([0x00, 0x10])
        self._i2c.i2c_write_wait_ack([0xFE, prescale])
        self._i2c.i2c_write_wait_ack([0x00, 0xA1])
        # Set MODE2: INVRT and OUTDRV from config
        mode2_value = 0
        if self._invert_output:
            mode2_value |= 0x10  # INVRT=1
        if self._totem_pole:
            mode2_value |= 0x04  # OUTDRV=1
        self._i2c.i2c_write_wait_ack([0x01, mode2_value])
        self._reactor.pause(self._reactor.monotonic() + .01)
        logging.info("PCA9685 %s: Initialized with frequency %s Hz"%(
            self.name,self._frequency))

    def _set_pwm(self, channel, value):
        base_reg = 0x06 + (4 * channel)
        # no need for fancy phase-shifting PWM
        on_time = 0  # Standard for servos
        off_time = int(min(max(value, 0), self._pwm_max))
        on_time = 0
        data = [ base_reg,
            on_time & 0xFF,         # LEDn_ON_L
            (on_time >> 8) & 0x0F,  # LEDn_ON_H
            off_time & 0xFF,        # LEDn_OFF_L
            (off_time >> 8) & 0x0F  # LEDn_OFF_H
        ]
        self._i2c.i2c_write_wait_ack(data)

    def _set_digital(self, channel, value):
        base_reg = 0x06 + (4 * channel)
        if value: # Fully on: LEDn_ON_H bit 4 (0x10)
            self._i2c.i2c_write_wait_ack([base_reg, 0x00, 0x10, 0x00, 0x00])
        else:     # Fully off: LEDn_OFF_H bit 4 (0x10)
            self._i2c.i2c_write_wait_ack([base_reg, 0x00, 0x00, 0x00, 0x10])

    def print_time_to_clock(self, print_time):
        return self._mcu.print_time_to_clock(print_time)

    def register_flush_callback(self, callback):
        self._flush_callbacks.append(callback)

    def flush_moves(self, flush_time, clear_history_time):
        clock = self.print_time_to_clock(flush_time)
        if clock < 0:
            return
        for cb in self._flush_callbacks:
            cb(flush_time, clock)

    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'pwm':
            raise self._printer.config_error(
                "PCA9685 %s: Only PWM pins supported"%(self.name,))
        pin_name = pin_params['pin']
        channel = int(pin_name.split('pwm')[-1])
        if channel < 0 or channel > 15:
            raise self._printer.config_error(
                "PCA9685 %s: Invalid pin %s"%(self.name,pin_name))
        # Inject frequency and pwm_max into pin_params
        pin_params['frequency'] = self._frequency
        pin_params['pwm_max'] = self._pwm_max
        pin = PCA9685PWMPin(self, pin_params)
        self._pins[pin_name] = pin
        return pin

class PCA9685PWMPin:
    def __init__(self, mcu, pin_params):
        self.name = pin_params['pin']
        self._mcu = mcu
        self._channel = int(self.name.split('pwm')[-1])
        self._cycle_time = 1. / pin_params['frequency']
        self._pwm_max = pin_params['pwm_max']
        self._invert = pin_params['invert']  # Per-pin invert from config
        self._last_value = 0.
        self._start_value = self._shutdown_value = float(self._invert)
        self._max_duration = 0.
        self._hardware_pwm = False
        self._last_time = 0
        logging.info("PCA9685 %s:%s: Initialized pin=%s, invert=%s"%(
                self._mcu.name,self.name,self._channel,self._invert))

    def get_mcu(self):
        return self._mcu

    def setup_max_duration(self, max_duration):
        #not really used, just for compatibility
        self._max_duration = max_duration

    def setup_cycle_time(self, cycle_time, hardware_pwm=False):
        expected_cycle_time = 1. / self._mcu._frequency
        if abs(cycle_time - expected_cycle_time) > 0.000001:
            logging.warning("PCA9685 %s:%s: "
                "Requested cycle_time %s does not match "
                "controller frequency %s Hz"%(
                self._mcu.name,self.name,cycle_time,self._mcu._frequency))
        self._cycle_time = cycle_time
        self._hardware_pwm = hardware_pwm

    def setup_start_value(self, start_value, shutdown_value):
        self._start_value = max(0., min(1., start_value))
        self._shutdown_value = max(0., min(1., shutdown_value))

    def _apply_start_value(self):
        # Apply inversion to start value
        value = 1. - self._start_value if self._invert else self._start_value
        self._set_pwm_value(value)

    def set_pwm(self, print_time, value, cycle_time=None):
        if (cycle_time is not None and
            abs(cycle_time - self._cycle_time) > 0.000001):
            logging.warning("PCA9685 %s:%s: "
                "Cycle time change to %s ignored, using %s"%(
                self._mcu.name,self.name,cycle_time,self._cycle_time))
        # Apply inversion
        effective_value = 1. - value if self._invert else value
        self._set_pwm_value(effective_value)
        self._last_value = value

    def set_digital(self, print_time, value):
        # Invert digital value if needed
        effective_value = not value if self._invert else bool(value)
        self._mcu._set_digital(self._channel, effective_value)
        self._last_value = 1.0 if value else 0.0

    def _set_pwm_value(self, value):
        # Convert Klipper's 0-1 value to PCA9685 counts
        pulse_width = value * self._cycle_time
        pwm_counts = int(pulse_width * self._mcu._frequency * self._pwm_max)
        self._mcu._set_pwm(self._channel, pwm_counts)

    def get_status(self, eventtime):
        return {
            'value': self._last_value,
            # 'cycle_time': self._cycle_time,
            # 'start_value': self._start_value,
            # 'shutdown_value': self._shutdown_value,
            # 'max_duration': self._max_duration,
            # 'invert': self._invert
        }

def load_config_prefix(config):
    return PCA9685Controller(config)
