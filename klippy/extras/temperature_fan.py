# Support fans that are enabled when temperature exceeds a set threshold
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import fan

KELVIN_TO_CELCIUS = -273.15
MAX_FAN_TIME = 5.0
AMBIENT_TEMP = 25.
PID_PARAM_BASE = 255.

class TemperatureFan:
    def __init__(self, config):
        self.name = config.get_name().split()[1]
        self.printer = config.get_printer()
        self.fan = fan.PrinterFan(config, default_shutdown_speed=1.)
        self.gcode = self.printer.lookup_object('gcode')
        self.min_temp = config.getfloat('min_temp', minval=KELVIN_TO_CELCIUS)
        self.max_temp = config.getfloat('max_temp', above=self.min_temp)
        self.sensor = self.printer.lookup_object('heater').setup_sensor(config)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self.temperature_callback)
        self.printer.lookup_object('heater').register_sensor(config, self)
        self.speed_delay = self.sensor.get_report_time_delta()
        self.max_speed = config.getfloat('max_speed', 1., above=0., maxval=1.)
        self.min_speed = config.getfloat('min_speed', 0.3, above=0., maxval=1.)
        self.last_temp = 0.
        self.last_temp_time = 0.
        self.target_temp_conf = config.getfloat(
            'target_temp', 40. if self.max_temp > 40. else self.max_temp,
            minval=self.min_temp, maxval=self.max_temp)
        self.target_temp = self.target_temp_conf
        algos = {'watermark': ControlBangBang, 'pid': ControlPID}
        algo = config.getchoice('control', algos)
        self.control = algo(self, config)
        self.next_speed_time = 0.
        self.last_speed_value = 0.
        self.gcode.register_mux_command(
            "SET_TEMPERATURE_FAN_TARGET", "TEMPERATURE_FAN", self.name,
            self.cmd_SET_TEMPERATURE_FAN_TARGET_TEMP,
            desc=self.cmd_SET_TEMPERATURE_FAN_TARGET_TEMP_help)

    def set_speed(self, read_time, value):
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
        self.fan.set_speed(speed_time, value)
    def temperature_callback(self, read_time, temp):
        self.last_temp = temp
        self.control.temperature_callback(read_time, temp)
    def get_temp(self, eventtime):
        return self.last_temp, self.target_temp
    def get_min_speed(self):
        return self.min_speed
    def get_max_speed(self):
        return self.max_speed
    cmd_SET_TEMPERATURE_FAN_TARGET_TEMP_help = "Sets a temperature fan target"
    def cmd_SET_TEMPERATURE_FAN_TARGET_TEMP(self, params):
        temp = self.gcode.get_float('TARGET', params, self.target_temp_conf)
        self.set_temp(temp)
    def set_temp(self, degrees):
        if degrees and (degrees < self.min_temp or degrees > self.max_temp):
            raise self.gcode.error(
                "Requested temperature (%.1f) out of range (%.1f:%.1f)"
                % (degrees, self.min_temp, self.max_temp))
        self.target_temp = degrees

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
            self.temperature_fan.set_speed(read_time, 0.)
        else:
            self.temperature_fan.set_speed(read_time,
                                           self.temperature_fan.get_max_speed())

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
        imax = config.getfloat('pid_integral_max',
                               self.temperature_fan.get_max_speed(), minval=0.)
        self.temp_integ_max = imax / self.Ki
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
        self.temperature_fan.set_speed(
            read_time, max(self.temperature_fan.get_min_speed(),
                           self.temperature_fan.get_max_speed() - bounded_co))
        # Store state for next measurement
        self.prev_temp = temp
        self.prev_temp_time = read_time
        self.prev_temp_deriv = temp_deriv
        if co == bounded_co:
            self.prev_temp_integ = temp_integ

def load_config_prefix(config):
    return TemperatureFan(config)
