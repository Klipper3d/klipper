# Filament selector based on Idler/Pulley/Selector style control
#
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper, homing


class IdlerPulleyFilamentSelector:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        # Setup axis rails
        self.rails = [stepper.LookupMultiRail(config.getsection('stepper_' + n))
                      for n in ['i', 'p', 's']]
        for rail, axis in zip(self.rails, 'ips'):
            rail.setup_itersolve('cartesian_stepper_alloc', axis)

def load_config(config):
    return IdlerPulleyFilamentSelector(config)
    