# Support a fan for cooling the MCU whenever a stepper or heater is on
#
# Copyright (C) 2019  Nils Friedchen <nils.friedchen@googlemail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import fan

PIN_MIN_TIME = 0.100

class ControllerFan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.stepper_names = config.getlist("stepper", None)
        self.stepper_enable = self.printer.load_object(config, 'stepper_enable')
        self.printer.load_object(config, 'heaters')
        self.heaters = []
        self.fan = fan.Fan(config)
        self.fan_speed = config.getfloat('fan_speed', default=1.,
                                         minval=0., maxval=1.)
        self.idle_speed = config.getfloat(
            'idle_speed', default=self.fan_speed, minval=0., maxval=1.)
        self.idle_timeout = config.getint("idle_timeout", default=30, minval=0)
        self.heater_names = config.getlist("heater", ("extruder",))
        self.last_on = self.idle_timeout
        self.last_speed = 0.

        # get configured temperature_sensors as ((str, str), (str, str)) => (("pi", "55"), ("spider", "77"))
        temperature_sensors_config = config.getlists(
            "temperature_sensors",
            seps=(':', '\n'),
            count=2,
            default=[]
        )
        # parse given thresholds to floats
        self.temperature_sensors_config = [(sensor[0], float(sensor[1])) for sensor in temperature_sensors_config]
        self.temperature_sensors = dict()
        self.temperature_sensors_only = config.getboolean("temperature_sensors_only", False)
        if self.temperature_sensors_only and len(self.temperature_sensors_config) <= 0:
            raise self.printer.config_error(
                "If temperature_sensors_only is True, there have to have temperature_sensors been configured!"
            )

    def handle_connect(self):
        self.__handle_connect_heaters()
        self.__handle_connect_steppers()
        self.__handle_connect_temperature_sensors()

    def __handle_connect_heaters():
        pheaters = self.printer.lookup_object('heaters')
        self.heaters = [pheaters.lookup_heater(n) for n in self.heater_names]

    def __handle_connect_steppers():
        all_steppers = self.stepper_enable.get_steppers()
        if self.stepper_names is None:
            self.stepper_names = all_steppers
            return

        if not all(x in all_steppers for x in self.stepper_names):
            raise self.printer.config_error(
                "One or more of these steppers are unknown: "
                "%s (valid steppers are: %s)"
                % (self.stepper_names, ", ".join(all_steppers))
            )

    def __handle_connect_temperature_sensors():
        all_temperature_sensor_module_names = [sensor[0] for sensor in self.printer.lookup_objects(module='temperature_sensor')]
        all_temperature_sensor_names = [sensor.split(' ')[1] for sensor in all_temperature_sensor_module_names]

        for sensor_name, temperature_threshold in self.temperature_sensors_config:
            actual_configured_sensor = None
            try:
                actual_configured_sensor = self.printer.lookup_object(
                    'temperature_sensor ' + sensor_name.strip()
                )
            except Exception:
                pass

            if actual_configured_sensor is None:
                raise self.printer.config_error(
                    "temperature_sensor '%s' is unknown, (valid sensors are: %s)"
                    % (sensor_name, ", ".join(all_temperature_sensor_names))
                )

            self.temperature_sensors[sensor_name] = (
                actual_configured_sensor,
                temperature_threshold
            )

    def handle_ready(self):
        reactor = self.printer.get_reactor()
        reactor.register_timer(self.callback, reactor.monotonic()+PIN_MIN_TIME)

    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)

    def callback(self, eventtime):
        speed = 0.
        active = self.__check_fan_active()

        if active:
            self.last_on = 0
            speed = self.fan_speed
        elif self.last_on < self.idle_timeout:
            speed = self.idle_speed
            self.last_on += 1

        if speed != self.last_speed:
            self.last_speed = speed
            curtime = self.printer.get_reactor().monotonic()
            print_time = self.fan.get_mcu().estimated_print_time(curtime)
            self.fan.set_speed(print_time + PIN_MIN_TIME, speed)

        return eventtime + 1.

    def __check_fan_active():
        for sensor, threshold in self.temperature_sensors.items():
            current_temp, _ = sensor.get_temp(eventtime)
            if current_temp > threshold:
                return True

        if self.temperature_sensors_only:
            return False

        for stepper in self.stepper_names:
            if self.stepper_enable.lookup_enable(stepper).is_motor_enabled():
                return True

        for heater in self.heaters:
            _, target_temp = heater.get_temp(eventtime)
            if target_temp:
                return True

def load_config_prefix(config):
    return ControllerFan(config)
