# Prevent the whole printer (or subset) using more than a set amount of power.
#
# Copyright (C) 2021  Oliver Mattos <oliver@omattos.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PowerLimiter:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:ready",
                                            self.handle_ready)
        self.device_names = [x.strip()
                             for x in config.get("devices").split(',')]
        self.device_powers = [float(x)
                              for x in config.get("device_powers").split(',')]
        if len(self.device_names) != len(self.device_powers):
            raise self.printer.config_error(
                    "You need to specify device powers for all devices!"
                    "  Try setting device_powers: 100%s" % (
                    ', 100'*(len(self.devices)-1)))

        self.limit_watts = config.getfloat("limit_watts")
        if (self.limit_watts > sum(self.device_powers)):
            raise self.printer.config_error(
                "limit_watts must be less than all of the device_powers added")

        self.pheaters = self.printer.load_object(config, 'heaters')

    def handle_ready(self):
        self.devices = [ self.make_PowerLimitedDevice(name, power)
                         for name, power in
                         zip(self.device_names, self.device_powers)]

    def make_PowerLimitedDevice(self, name, power):
        heater = self.pheaters.lookup_heater(name)
        return PowerLimitedDevice(heater, power, self)

    # Power not in use by any device in this group
    def get_available_power(self):
        power_used = sum([x.get_watts_used() for x in self.devices])
        return self.limit_watts - power_used

    def get_group_size(self):
        return len(self.devices)


class PowerLimitedDevice:
    def __init__(self, heater, heater_power_watts, group):
        self.group = group
        self.heater = heater
        self.heater_power_watts = heater_power_watts
        self.upstream_power_limiter = self.heater.power_limiter
        self.heater.power_limiter = self

    # PWM fraction that should be the maximum allowable right now.
    def get_power_limit(self):
        # Let this device use most of it's current usage, plus an even share
        # of available power.  The long term stable point of this is an
        # approximately even share between heaters.
        share_of_available = (self.group.get_available_power()
                                / self.group.get_group_size())
        new_limit = ( share_of_available / self.heater_power_watts
                     + self.heater.last_pwm_value * 0.95)
        return min(new_limit, self.upstream_power_limiter.get_power_limit())

    def get_watts_used(self):
        return self.heater.last_pwm_value * self.heater_power_watts

def load_config(config):
    return PowerLimiter(config)
