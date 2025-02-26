# Support fans that are enabled when temperature exceeds a set threshold
#
# Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import fan
from .gcode_macro import TemplateWrapper, PrinterGCodeMacro

KELVIN_TO_CELSIUS = -273.15
MAX_FAN_TIME = 5.0
AMBIENT_TEMP = 25.
PID_PARAM_BASE = 255.

class TemperatureFan:
    def __init__(self, config):
        self.name = config.get_name().split()[1]
        self.printer = config.get_printer()
        self.fan = fan.Fan(config, default_shutdown_speed=1.)
        self.min_temp = config.getfloat('min_temp', minval=KELVIN_TO_CELSIUS)
        self.max_temp = config.getfloat('max_temp', above=self.min_temp)
        pheaters = self.printer.load_object(config, 'heaters')
        self.sensor = pheaters.setup_sensor(config)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self.temperature_callback)
        pheaters.register_sensor(config, self)
        self.speed_delay = self.sensor.get_report_time_delta()
        self.max_speed_conf = config.getfloat(
            'max_speed', 1., above=0., maxval=1.)
        self.max_speed = self.max_speed_conf
        self.min_speed_conf = config.getfloat(
            'min_speed', 0.3, minval=0., maxval=1.)
        self.min_speed = self.min_speed_conf
        self.last_temp = 0.
        self.last_temp_time = 0.
        self.target_temp_conf = config.getfloat(
            'target_temp', 40. if self.max_temp > 40. else self.max_temp,
            minval=self.min_temp, maxval=self.max_temp)
        self.target_temp = self.target_temp_conf
        algos = { 'watermark': ControlBangBang, 'pid': ControlPID,
                  'custom': ControlCustom }
        algo = config.getchoice('control', algos)
        self.control = algo(self, config)
        self.next_speed_time = 0.
        self.last_speed_value = 0.
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command(
            "SET_TEMPERATURE_FAN_TARGET", "TEMPERATURE_FAN", self.name,
            self.cmd_SET_TEMPERATURE_FAN_TARGET,
            desc=self.cmd_SET_TEMPERATURE_FAN_TARGET_help)

    def set_tf_speed(self, read_time, value):
        if value <= 0.:
            value = 0.
        elif value < self.min_speed:
            value = self.min_speed
        if self.target_temp <= 0.:
            value = 0.
        if ((read_time < self.next_speed_time or not self.last_speed_value)
                and abs(value - self.last_speed_value) < 0.05):
            # No significant change in value - can suppress update
            return
        speed_time = read_time + self.speed_delay
        self.next_speed_time = speed_time + 0.75 * MAX_FAN_TIME
        self.last_speed_value = value
        self.fan.set_speed(value, speed_time)
    def temperature_callback(self, read_time, temp):
        self.last_temp = temp
        self.control.temperature_callback(read_time, temp)
    def get_temp(self, eventtime):
        return self.last_temp, self.target_temp
    def get_min_speed(self):
        return self.min_speed
    def get_max_speed(self):
        return self.max_speed
    def get_status(self, eventtime):
        status = self.fan.get_status(eventtime)
        status["temperature"] = round(self.last_temp, 2)
        status["target"] = self.target_temp
        return status
    cmd_SET_TEMPERATURE_FAN_TARGET_help = \
        "Sets a temperature fan target and fan speed limits"
    def cmd_SET_TEMPERATURE_FAN_TARGET(self, gcmd):
        temp = gcmd.get_float('TARGET', self.target_temp_conf)
        self.set_temp(temp)
        min_speed = gcmd.get_float('MIN_SPEED', self.min_speed)
        max_speed = gcmd.get_float('MAX_SPEED', self.max_speed)
        if min_speed > max_speed:
            raise self.printer.command_error(
                "Requested min speed (%.1f) is greater than max speed (%.1f)"
                % (min_speed, max_speed))
        self.set_min_speed(min_speed)
        self.set_max_speed(max_speed)

    def set_temp(self, degrees):
        if degrees and (degrees < self.min_temp or degrees > self.max_temp):
            raise self.printer.command_error(
                "Requested temperature (%.1f) out of range (%.1f:%.1f)"
                % (degrees, self.min_temp, self.max_temp))
        self.target_temp = degrees

    def set_min_speed(self, speed):
        if speed and (speed < 0. or speed > 1.):
            raise self.printer.command_error(
                "Requested min speed (%.1f) out of range (0.0 : 1.0)"
                % (speed))
        self.min_speed = speed

    def set_max_speed(self, speed):
        if speed and (speed < 0. or speed > 1.):
            raise self.printer.command_error(
                "Requested max speed (%.1f) out of range (0.0 : 1.0)"
                % (speed))
        self.max_speed = speed

######################################################################
# Bang-bang control algo
######################################################################

class ControlBangBang:
    def __init__(self, temperature_fan, config):
        self.temperature_fan = temperature_fan
        self.max_delta = config.getfloat('max_delta', 2.0, above=0.)
        self.heating = False
    def temperature_callback(self, read_time, temp):
        current_temp, target_temp = self.temperature_fan.get_temp(read_time)
        if (self.heating
            and temp >= target_temp+self.max_delta):
            self.heating = False
        elif (not self.heating
              and temp <= target_temp-self.max_delta):
            self.heating = True
        if self.heating:
            self.temperature_fan.set_tf_speed(read_time, 0.)
        else:
            self.temperature_fan.set_tf_speed(
                read_time, self.temperature_fan.get_max_speed())

######################################################################
# Proportional Integral Derivative (PID) control algo
######################################################################

PID_SETTLE_DELTA = 1.
PID_SETTLE_SLOPE = .1

class ControlPID:
    def __init__(self, temperature_fan, config):
        self.temperature_fan = temperature_fan
        self.Kp = config.getfloat('pid_Kp') / PID_PARAM_BASE
        self.Ki = config.getfloat('pid_Ki') / PID_PARAM_BASE
        self.Kd = config.getfloat('pid_Kd') / PID_PARAM_BASE
        self.min_deriv_time = config.getfloat('pid_deriv_time', 2., above=0.)
        self.temp_integ_max = 0.
        if self.Ki:
            self.temp_integ_max = self.temperature_fan.get_max_speed() / self.Ki
        self.prev_temp = AMBIENT_TEMP
        self.prev_temp_time = 0.
        self.prev_temp_deriv = 0.
        self.prev_temp_integ = 0.
    def temperature_callback(self, read_time, temp):
        current_temp, target_temp = self.temperature_fan.get_temp(read_time)
        time_diff = read_time - self.prev_temp_time
        # Calculate change of temperature
        temp_diff = temp - self.prev_temp
        if time_diff >= self.min_deriv_time:
            temp_deriv = temp_diff / time_diff
        else:
            temp_deriv = (self.prev_temp_deriv * (self.min_deriv_time-time_diff)
                          + temp_diff) / self.min_deriv_time
        # Calculate accumulated temperature "error"
        temp_err = target_temp - temp
        temp_integ = self.prev_temp_integ + temp_err * time_diff
        temp_integ = max(0., min(self.temp_integ_max, temp_integ))
        # Calculate output
        co = self.Kp*temp_err + self.Ki*temp_integ - self.Kd*temp_deriv
        bounded_co = max(0., min(self.temperature_fan.get_max_speed(), co))
        self.temperature_fan.set_tf_speed(
            read_time, max(self.temperature_fan.get_min_speed(),
                           self.temperature_fan.get_max_speed() - bounded_co))
        # Store state for next measurement
        self.prev_temp = temp
        self.prev_temp_time = read_time
        self.prev_temp_deriv = temp_deriv
        if co == bounded_co:
            self.prev_temp_integ = temp_integ

######################################################################
# Custom Control Algo
######################################################################

class ControlCustom:
    def __init__(self, temperature_fan, config):
        self.temperature_fan = temperature_fan
        self.config = config
        self.name = config.get_name().split()[-1]
        script = config.get("custom")
        printer = config.get_printer()
        self.env = PrinterGCodeMacro(config).env
        self.env.globals.update({
            'log': self._log,
            'set_var': self._set_var,
            'get_var': self._get_var,
        })
        current_temp, target_temp = self.temperature_fan.get_temp(.0)
        self.variables = {
            "current_temp": current_temp,
            "target_temp": target_temp,
            "read_time": .0
        }
        self.load_variables()
        self.template = TemplateWrapper(printer, self.env,
                                        config.get_name(), script)
    def temperature_callback(self, read_time, temp):
        current_temp, target_temp = self.temperature_fan.get_temp(read_time)
        max_speed = self.temperature_fan.get_max_speed()
        self.variables["current_temp"] = temp
        self.variables["target_temp"] = target_temp
        self.variables["read_time"] = read_time

        co = float(self.template.render())
        bounded_co = max(0., min(max_speed, co))
        self.temperature_fan.set_tf_speed(read_time, bounded_co)
    def load_variables(self):
        import ast
        import json
        prefix = 'variable_'
        for option in self.config.get_prefix_options(prefix):
            try:
                literal = ast.literal_eval(self.config.get(option))
                json.dumps(literal, separators=(',', ':'))
                self.variables[option[len(prefix):]] = literal
            except (SyntaxError, TypeError, ValueError) as e:
                raise self.config.error(
                    "Option '%s' in section '%s' is not a valid literal: %s" % (
                        option, self.config.get_name(), e))
    def _log(self, output):
        logging.info(output)
        # Make it callable in a block section
        return ""
    def _get_var(self, name):
        v = self.variables.get(name)
        if v is None:
            raise self.config.error(
                "variable_%s is not defined" % (name)
            )
        return v
    def _set_var(self, name, value):
        v = self._get_var(name)
        self.variables[name] = value
        return v

def load_config_prefix(config):
    return TemperatureFan(config)
