# Extra PCA9685 pins through host mcu
#
# Copyright (C) 2017-2019  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2022 Serge Rabyking <serge@rabyking.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import pins, mcu
from . import bus

PCA9685_BUS = 1
PCA9685_ADDRESS = 0x70
PCA9685_CYCLE_TIME = 0.02 # the same as servo

class PCA9685_pin:
    def __init__(self, pca9685, channel, pin_type, pin_params):
        self._channel = channel
        if pin_type not in ['digital_out', 'pwm']:
            raise pins.error("Pin type not supported on PCA9685")
        self._mcu = pca9685.host_mcu
        self._mcu.register_config_callback(self._build_config)
        self._bus = pca9685._bus
        self._address = pca9685._address
        self._cycle_time = pca9685._cycle_time
        self._max_duration = 2.
        self._oid = None
        self._invert = pin_params['invert']
        self._start_value = self._shutdown_value = float(self._invert)
        self._is_static = False
        self._last_clock = 0
        self._pwm_max = 0.
        self._set_cmd = None
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_cycle_time(self, cycle_time, hardware_pwm=False):
        if hardware_pwm:
            logging.info("pca9685: ignoring hardware_pwm parameter")
        if cycle_time != self._cycle_time:
            logging.info("pca9685: ignoring cycle time of %.6f (using %.6f)",
                         cycle_time, self._cycle_time)
    def setup_start_value(self, start_value, shutdown_value, is_static=False):
        if is_static and start_value != shutdown_value:
            raise pins.error("Static pin can not have shutdown value")
        if self._invert:
            start_value = 1. - start_value
            shutdown_value = 1. - shutdown_value
        self._start_value = max(0., min(1., start_value))
        self._shutdown_value = max(0., min(1., shutdown_value))
        self._is_static = is_static
    def _build_config(self):
        self._pwm_max = self._mcu.get_constant_float("PCA9685_MAX")
        cycle_ticks = self._mcu.seconds_to_clock(self._cycle_time)
        if self._is_static:
            self._mcu.add_config_cmd(
                "set_pca9685_out bus=%d addr=%d channel=%d"
                " cycle_ticks=%d value=%d" % (
                    self._bus, self._address, self._channel,
                    cycle_ticks, self._start_value * self._pwm_max))
            return
        self._mcu.request_move_queue_slot()
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_pca9685 oid=%d bus=%d addr=%d channel=%d cycle_ticks=%d"
            " value=%d default_value=%d max_duration=%d" % (
                self._oid, self._bus, self._address, self._channel, cycle_ticks,
                self._start_value * self._pwm_max,
                self._shutdown_value * self._pwm_max,
                self._mcu.seconds_to_clock(self._max_duration)))
        cmd_queue = self._mcu.alloc_command_queue()
        self._set_cmd = self._mcu.lookup_command(
            "queue_pca9685_out oid=%c clock=%u value=%hu", cq=cmd_queue)
    def set_pwm(self, print_time, value, cycle_time=None):
        clock = self._mcu.print_time_to_clock(print_time)
        if self._invert:
            value = 1. - value
        value = int(max(0., min(1., value)) * self._pwm_max + 0.5)
        self._set_cmd.send([self._oid, clock, value],
                           minclock=self._last_clock, reqclock=clock)
        self._last_clock = clock
    def set_digital(self, print_time, value):
        if value:
            self.set_pwm(print_time, 1.)
        else:
            self.set_pwm(print_time, 0.)

class PCA9685:
    def __init__(self, config):
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        ppins.register_chip('pca9685', self)
        self.host_mcu = mcu.get_printer_mcu(printer, config.get('host_mcu'))
        self._bus = config.get('i2c_bus',PCA9685_BUS)
        self._address = config.get('i2c_address',PCA9685_ADDRESS)
        self._cycle_time = config.get('cycle_time',PCA9685_CYCLE_TIME)
    def setup_pin(self, pin_type, pin_params):
        pin = pin_params['pin']
        if pin.startswith('P') and pin[1:].isdigit():
            idx=int(pin[1:])
            if idx<16:
                return PCA9685_pin(self,idx,pin_type,pin_params)
        raise pins.error("Unknown PCA9685 pin %s" % (pin,))

def load_config(config):
    return PCA9685(config)
