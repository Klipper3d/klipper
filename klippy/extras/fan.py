# Printer cooling fan
#
# Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import pulse_counter

FAN_MIN_TIME = 0.100

class Fan:
    def __init__(self, config, default_shutdown_speed=0.):
        self.printer = config.get_printer()
        self.last_fan_value = 0.
        self.last_fan_time = 0.
        # Read config
        self.max_power = config.getfloat('max_power', 1., above=0., maxval=1.)
        self.kick_start_time = config.getfloat('kick_start_time', 0.1,
                                               minval=0.)
        self.off_below = config.getfloat('off_below', default=0.,
                                         minval=0., maxval=1.)
        cycle_time = config.getfloat('cycle_time', 0.010, above=0.)
        hardware_pwm = config.getboolean('hardware_pwm', False)
        shutdown_speed = config.getfloat(
            'shutdown_speed', default_shutdown_speed, minval=0., maxval=1.)
        # Setup pwm object
        ppins = self.printer.lookup_object('pins')
        self.mcu_fan = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu_fan.setup_max_duration(0.)
        self.mcu_fan.setup_cycle_time(cycle_time, hardware_pwm)
        shutdown_power = max(0., min(self.max_power, shutdown_speed))
        self.mcu_fan.setup_start_value(0., shutdown_power)

        self.enable_pin = None
        enable_pin = config.get('enable_pin', None)
        if enable_pin is not None:
            self.enable_pin = ppins.setup_pin('digital_out', enable_pin)
            self.enable_pin.setup_max_duration(0.)

        # Setup tachometer
        self.tachometer = FanTachometer(config)

        # Register callbacks
        self.printer.register_event_handler("gcode:request_restart",
                                            self._handle_request_restart)

    def get_mcu(self):
        return self.mcu_fan.get_mcu()
    def set_speed(self, print_time, value):
        if value < self.off_below:
            value = 0.
        value = max(0., min(self.max_power, value * self.max_power))
        if value == self.last_fan_value:
            return
        print_time = max(self.last_fan_time + FAN_MIN_TIME, print_time)
        if self.enable_pin:
            if value > 0 and self.last_fan_value == 0:
                self.enable_pin.set_digital(print_time, 1)
            elif value == 0 and self.last_fan_value > 0:
                self.enable_pin.set_digital(print_time, 0)
        if (value and value < self.max_power and self.kick_start_time
            and (not self.last_fan_value or value - self.last_fan_value > .5)):
            # Run fan at full speed for specified kick_start_time
            self.mcu_fan.set_pwm(print_time, self.max_power)
            print_time += self.kick_start_time
        self.mcu_fan.set_pwm(print_time, value)
        self.last_fan_time = print_time
        self.last_fan_value = value
    def set_speed_from_command(self, value):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback((lambda pt:
                                              self.set_speed(pt, value)))
    def _handle_request_restart(self, print_time):
        self.set_speed(print_time, 0.)

    def get_status(self, eventtime):
        tachometer_status = self.tachometer.get_status(eventtime)
        return {
            'speed': self.last_fan_value,
            'rpm': tachometer_status['rpm'],
        }

class FanTachometer:
    def __init__(self, config):
        printer = config.get_printer()
        self._freq_counter = None

        pin = config.get('tachometer_pin', None)
        if pin is not None:
            self.ppr = config.getint('tachometer_ppr', 2, minval=1)
            poll_time = config.getfloat('tachometer_poll_interval',
                                        0.0015, above=0.)
            sample_time = 1.
            self._freq_counter = pulse_counter.FrequencyCounter(
                printer, pin, sample_time, poll_time)

    def get_status(self, eventtime):
        if self._freq_counter is not None:
            rpm = self._freq_counter.get_frequency() * 30. / self.ppr
        else:
            rpm = None
        return {'rpm': rpm}

class PrinterFan:
    def __init__(self, config):
        self.fan = Fan(config)
        for k,f in config.get_printer().lookup_objects(module='fan'):
            self.controller = f.controller
            break
        else:
            self.controller = FanController(config)
        # Register both the fan name "myfan" and object name "fan myfan".
        name = config.get_name()
        fan_name = name.split()[-1]
        self.controller.register_fan(config, self.fan, fan_name)
        if name != fan_name:
            self.controller.register_fan(config, self.fan, name)

    def get_status(self, eventtime):
        status = self.fan.get_status(eventtime)
        status['active'] = 1 if self.fan in self.controller.active_fans else 0
        return status

# Tracks active fan
class FanController:
    def __init__(self, config):
        self.name_to_fan = {}
        self.active_fans = []
        self.requested_speed = None
        gcode = config.get_printer().lookup_object('gcode')
        gcode.register_command("M106", self.cmd_M106)
        gcode.register_command("M107", self.cmd_M107)
        gcode.register_command("ACTIVATE_FAN", self.cmd_ACTIVATE_FAN)

    def register_fan(self, config, fan, fan_name):
        if fan_name in self.name_to_fan:
            raise config.error("Duplicate fan name: %s" % (fan_name,))
        self.name_to_fan[fan_name] = fan

    def cmd_ACTIVATE_FAN(self, gcmd):
        fan_names = gcmd.get("FAN")
        fans = []
        for name in fan_names.split(","):
            fan = self.name_to_fan.get(name.strip())
            if not fan:
                raise gcmd.error("Fan %s not known" % (name,))
            fans.append(fan)
        self.activate_fans(fans)

    def activate_fans(self, new_fans):
        # Set new active fans and transfer the set speed to those fans.
        if self.active_fans == new_fans:
            return
        old_fans = self.active_fans
        self.active_fans = new_fans
        if self.requested_speed is not None:
            for fan in old_fans:
                if fan not in new_fans:
                    fan.set_speed_from_command(0.)
            for fan in new_fans:
                if fan not in old_fans:
                    fan.set_speed_from_command(self.requested_speed)
    def cmd_M106(self, gcmd):
        # Set fan speed
        self.requested_speed = gcmd.get_float('S', 255., minval=0.) / 255.
        for fan in self.active_fans:
            fan.set_speed_from_command(self.requested_speed)
    def cmd_M107(self, gcmd):
        # Turn fan off
        self.requested_speed = 0.
        for fan in self.active_fans:
            fan.set_speed_from_command(self.requested_speed)

def load_config(config):
    fan = PrinterFan(config)
    # Prefixless is active on startup
    fan.controller.activate_fans([fan.fan])
    return fan

def load_config_prefix(config):
    return PrinterFan(config)
