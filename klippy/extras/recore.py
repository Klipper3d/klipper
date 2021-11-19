# Config for Recore,
#
# Copyright (C) 2017-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, os
import pins, mcu

class recore:
  def __init__(self, config):
    printer = config.get_printer()
    ppins = printer.lookup_object('pins')
    ppins.register_chip('recore', self)
    revisions = {
        'A1': 'A1',
        'A2': 'A2',
        'A3': 'A5',
        'A4': 'A5',
        'A5': 'A5',
        'A6': 'A6'}
    config.getchoice('revision', revisions)
    # Setup enable pin
    enable_pin = config.get('enable_pin', 'ar100:OC_RESET ')
    self.mcu_power_enable = ppins.setup_pin('digital_out', enable_pin)
    self.mcu_power_enable.setup_start_value(
        start_value=1., shutdown_value=0., is_static=False)
    self.mcu_power_enable.setup_max_duration(0.)
    self.mcu_power_start_value = self.mcu_power_shutdown_value = False
    # Setup stepper config
    self.last_stepper_time = 0.

  def setup_pin(self, pin_type, pin_params):
    pin = pin_params['pin']
    if pin in self.pins:
      pclass, channel = self.pins[pin]
      return pclass(self, channel, pin_type, pin_params)
    raise pins.error("Unknown recore pin %s" % (pin, ))

def load_config(config):
  return recore(config)
