# Code to configure miscellaneous chips
#
# Copyright (C) 2017-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, os
import pins, mcu, bus

recore_MAX_CURRENT = 3.84
recore_PCA9685_BUS = 2
recore_PCA9685_ADDRESS = 0x70
recore_PCA9685_CYCLE_TIME = .001
PIN_MIN_TIME = 0.100


class pca9685_pwm:
  def __init__(self, recore, channel, pin_type, pin_params):
    self._recore = recore
    self._channel = channel
    if pin_type not in ['digital_out', 'pwm']:
      raise pins.error("Pin type not supported on recore")
    self._mcu = recore.host_mcu
    self._mcu.register_config_callback(self._build_config)
    self._bus = recore_PCA9685_BUS
    self._address = recore_PCA9685_ADDRESS
    self._cycle_time = recore_PCA9685_CYCLE_TIME
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
      raise pins.error("pca9685 does not support hardware_pwm parameter")
    if cycle_time != self._cycle_time:
      logging.info("Ignoring pca9685 cycle time of %.6f (using %.6f)", cycle_time, self._cycle_time)

  def setup_start_value(self, start_value, shutdown_value, is_static=False):
    if is_static and start_value != shutdown_value:
      raise pins.error("Static pin can not have shutdown value")
    if self._invert:
      start_value = 1. - start_value
      shutdown_value = 1. - shutdown_value
    self._start_value = max(0., min(1., start_value))
    self._shutdown_value = max(0., min(1., shutdown_value))
    self._is_static = is_static
    self._recore.note_pwm_start_value(self._channel, self._start_value, self._shutdown_value)

  def _build_config(self):
    self._pwm_max = self._mcu.get_constant_float("PCA9685_MAX")
    cycle_ticks = self._mcu.seconds_to_clock(self._cycle_time)
    if self._is_static:
      self._mcu.add_config_cmd(
          "set_pca9685_out bus=%d addr=%d channel=%d"
          " cycle_ticks=%d value=%d" % (self._bus, self._address, self._channel, cycle_ticks,
                                        self._start_value * self._pwm_max))
      return
    self._oid = self._mcu.create_oid()
    self._mcu.add_config_cmd(
        "config_pca9685 oid=%d bus=%d addr=%d channel=%d cycle_ticks=%d"
        " value=%d default_value=%d max_duration=%d" %
        (self._oid, self._bus, self._address, self._channel, cycle_ticks,
         self._start_value * self._pwm_max, self._shutdown_value * self._pwm_max,
         self._mcu.seconds_to_clock(self._max_duration)))
    cmd_queue = self._mcu.alloc_command_queue()
    self._set_cmd = self._mcu.lookup_command(
        "schedule_pca9685_out oid=%c clock=%u value=%hu", cq=cmd_queue)

  def set_pwm(self, print_time, value):
    clock = self._mcu.print_time_to_clock(print_time)
    if self._invert:
      value = 1. - value
    value = int(max(0., min(1., value)) * self._pwm_max + 0.5)
    self._recore.note_pwm_enable(print_time, self._channel, value)
    self._set_cmd.send([self._oid, clock, value], minclock=self._last_clock, reqclock=clock)
    self._last_clock = clock

  def set_digital(self, print_time, value):
    if value:
      self.set_pwm(print_time, 1.)
    else:
      self.set_pwm(print_time, 0.)


class recoreDACEnable:
  def __init__(self, recore, channel, pin_type, pin_params):
    if pin_type != 'digital_out':
      raise pins.error("recore virtual enable pin must be digital_out")
    if pin_params['invert']:
      raise pins.error("recore virtual enable pin can not be inverted")
    self.mcu = recore.host_mcu
    self.value = recore.stepper_dacs[channel]
    self.pwm = pca9685_pwm(recore, channel, pin_type, pin_params)

  def get_mcu(self):
    return self.mcu

  def setup_max_duration(self, max_duration):
    self.pwm.setup_max_duration(max_duration)

  def set_digital(self, print_time, value):
    if value:
      self.pwm.set_pwm(print_time, self.value)
    else:
      self.pwm.set_pwm(print_time, 0.)


SERVO_PINS = {
    "servo0": ("pwmchip0/pwm0", "gpio0_30", "gpio1_18"),    # P9_11, P9_14
    "servo1": ("pwmchip0/pwm1", "gpio3_17", "gpio1_19"),    # P9_28, P9_16
}


class servo_pwm:
  def __init__(self, recore, pin_params):
    config_name = pin_params['pin']
    pwm_pin, resv1, resv2 = SERVO_PINS[config_name]
    pin_params = dict(pin_params)
    pin_params['pin'] = pwm_pin
    # Setup actual pwm pin using linux hardware pwm on host
    self.mcu_pwm = recore.host_mcu.setup_pin("pwm", pin_params)
    self.get_mcu = self.mcu_pwm.get_mcu
    self.setup_max_duration = self.mcu_pwm.setup_max_duration
    self.setup_start_value = self.mcu_pwm.setup_start_value
    self.set_pwm = self.mcu_pwm.set_pwm
    # Reserve pins to warn user of conflicts
    pru_mcu = recore.mcu_pwm_enable.get_mcu()
    printer = pru_mcu.get_printer()
    ppins = printer.lookup_object('pins')
    pin_resolver = ppins.get_pin_resolver(pru_mcu.get_name())
    pin_resolver.reserve_pin(resv1, config_name)
    pin_resolver.reserve_pin(resv2, config_name)

  def setup_cycle_time(self, cycle_time, hardware_pwm=False):
    self.mcu_pwm.setup_cycle_time(cycle_time, True)


recoreStepConfig = {
    'disable': None,
    '1': (1 << 7) | (1 << 5),
    '2': (1 << 7) | (1 << 5) | (1 << 6),
    'spread2': (1 << 5),
    '4': (1 << 7) | (1 << 5) | (1 << 4),
    '16': (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4),
    'spread4': (1 << 5) | (1 << 4),
    'spread16': (1 << 7),
    'stealth4': (1 << 7) | (1 << 6),
    'stealth16': 0
}


class recore:
  def __init__(self, config):
    printer = config.get_printer()
    ppins = printer.lookup_object('pins')
    ppins.register_chip('recore', self)
    revisions = {'A1': 'A1'}
    config.getchoice('revision', revisions)
    self.host_mcu = mcu.get_printer_mcu(printer, config.get('host_mcu'))
    # Setup enable pin
    enable_pin = config.get('enable_pin', 'ar100:PG1')
    self.mcu_power_enable = ppins.setup_pin('digital_out', enable_pin)
    self.mcu_power_enable.setup_start_value(start_value=1., shutdown_value=0., is_static=False)
    self.mcu_power_enable.setup_max_duration(0.)
    self.mcu_power_start_value = self.mcu_power_shutdown_value = False
    # Setup stepper config
    self.last_stepper_time = 0.

  def note_pwm_start_value(self, channel, start_value, shutdown_value):
    self.mcu_pwm_start_value |= not not start_value
    self.mcu_pwm_shutdown_value |= not not shutdown_value
    self.mcu_pwm_enable.setup_start_value(self.mcu_pwm_start_value, self.mcu_pwm_shutdown_value)
    self.enabled_channels[channel] = not not start_value

  def note_pwm_enable(self, print_time, channel, value):
    is_enable = not not value
    if self.enabled_channels[channel] == is_enable:
      # Nothing to do
      return
    self.enabled_channels[channel] = is_enable
    # Check if need to set the pca9685 enable pin
    on_channels = [1 for c, e in self.enabled_channels.items() if e]
    if not on_channels:
      self.mcu_pwm_enable.set_digital(print_time, 0)
    elif is_enable and len(on_channels) == 1:
      self.mcu_pwm_enable.set_digital(print_time, 1)
    # Check if need to set the stepper enable lines
    if channel not in self.stepper_dacs:
      return
    on_dacs = [1 for c in self.stepper_dacs.keys() if self.enabled_channels[c]]
    if not on_dacs:
      sr = self.sr_disabled
    elif is_enable and len(on_dacs) == 1:
      sr = self.sr_enabled
    else:
      return
    print_time = max(print_time, self.last_stepper_time + PIN_MIN_TIME)
    clock = self.host_mcu.print_time_to_clock(print_time)
    self.sr_spi.spi_send(sr, minclock=clock, reqclock=clock)

  def setup_pin(self, pin_type, pin_params):
    pin = pin_params['pin']
    if pin in self.pins:
      pclass, channel = self.pins[pin]
      return pclass(self, channel, pin_type, pin_params)
    raise pins.error("Unknown recore pin %s" % (pin, ))


def load_config(config):
  return recore(config)
