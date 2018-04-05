# Automatic heater calibration command
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import mathutil

class HeaterCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('HEATER_CALIBRATE', self.cmd_HEATER_CALIBRATE,
                               desc=self.cmd_HEATER_CALIBRATE_help)
    cmd_HEATER_CALIBRATE_help = "Run heater calibration test"
    def cmd_HEATER_CALIBRATE(self, gcmd):
        heater_name = gcmd.get('HEATER')
        target = gcmd.get_float('TARGET')
        write_file = gcmd.get_int('WRITE_FILE', 0)
        pheaters = self.printer.lookup_object('heaters')
        try:
            heater = pheaters.lookup_heater(heater_name)
        except self.printer.config_error as e:
            raise gcmd.error(str(e))
        self.printer.lookup_object('toolhead').get_last_move_time()
        calibrate = ControlBumpTest(self.printer, heater)
        old_control = heater.set_control(calibrate)
        try:
            heater.set_temp(target)
        except self.printer.command_error as e:
            heater.set_control(old_control)
            raise
        pheaters.wait_for_temperature(heater)
        heater.set_control(old_control)
        if write_file:
            calibrate.write_file('/tmp/heattest.txt')
        if calibrate.check_busy(0., 0., 0.):
            raise gcmd.error("heater_calibrate interrupted")
        gain, time_constant, delay = calibrate.calc_fopdt()
        gcmd.respond_info(
            "Heater parameters: gain=%.3f time_constant=%.3f delay_time=%.3f\n"
            "To use these parameters, update the printer config file with\n"
            "the above and then issue a RESTART command" % (
                gain, time_constant, delay))

class ControlBumpTest:
    def __init__(self, printer, heater):
        self.printer = printer
        self.heater = heater
        self.heater_max_power = heater.get_max_power()
        self.heater_pwm_delay = heater.get_pwm_delay()
        # State tracking
        self.state = 0
        self.done_temperature = 0.
        # Samples
        self.last_pwm = 0.
        self.pwm_samples = []
        self.temp_samples = []
        # Calculations
        self.ambient_temp = 0.
    # Heater control
    def set_pwm(self, read_time, value):
        if value != self.last_pwm:
            self.pwm_samples.append((read_time + self.heater_pwm_delay, value))
            self.last_pwm = value
        self.heater.set_pwm(read_time, value)
    def temperature_update(self, read_time, temp, target_temp):
        self.temp_samples.append((read_time, temp))
        if self.state == 0:
            self.set_pwm(read_time, 0.)
            if len(self.temp_samples) >= 20:
                # XXX - verify ambient temperature is valid
                self.state += 1
        elif self.state == 1:
            if temp < target_temp:
                self.set_pwm(read_time, self.heater_max_power)
                return
            self.set_pwm(read_time, 0.)
            start_temp = self.temp_samples[0][1]
            self.done_temperature = (
                start_temp + (target_temp - start_temp) * .35)
            self.heater.alter_target(self.done_temperature)
            self.state += 1
        elif self.state == 2:
            self.set_pwm(read_time, 0.)
            if temp <= self.done_temperature:
                self.state += 1
    def check_busy(self, eventtime, smoothed_temp, target_temp):
        if self.state < 3:
            return True
        return False
    # First Order Plus Delay Time calculation
    def model_smoothed_fopdt(self, gain, time_constant, delay):
        heater_on_time = self.pwm_samples[0][0]
        heater_off_time = self.pwm_samples[1][0]
        gain *= self.pwm_samples[0][1]
        ambient_temp = self.ambient_temp
        inv_time_constant = 1. / time_constant
        inv_delay = 1. / delay
        heat_time = heater_off_time - heater_on_time
        peak_temp = gain * (1. - math.exp(-heat_time * inv_time_constant))
        smooth_temp = last_time = 0.
        out = []
        for time, measured_temp in self.temp_samples:
            rel_temp = 0.
            if time > heater_off_time:
                cool_time = time - heater_off_time
                rel_temp = peak_temp * math.exp(-cool_time * inv_time_constant)
            elif time > heater_on_time:
                heat_time = time - heater_on_time
                rel_temp = gain * (1. - math.exp(-heat_time
                                                 * inv_time_constant))
            time_diff = time - last_time
            last_time = time
            smooth_factor = 1. - math.exp(-time_diff * inv_delay)
            smooth_temp += (rel_temp - smooth_temp) * smooth_factor
            out.append(ambient_temp + smooth_temp)
        return out
    def least_squares_error(self, params):
        gain = params['gain']
        time_constant = params['time_constant']
        delay = params['delay']
        if gain <= 0. or time_constant <= 0. or delay <= 0.:
            return 9.9e99
        model = self.model_smoothed_fopdt(gain, time_constant, delay)
        err = 0.
        for (time, measured_temp), model_temp in zip(self.temp_samples, model):
            err += (measured_temp-model_temp)**2
        return err
    def calc_fopdt(self):
        # Determine the ambient temperature
        heater_on_time, max_power = self.pwm_samples[0]
        pre_heat = [temp for time, temp in self.temp_samples
                    if time <= heater_on_time]
        self.ambient_temp = sum(pre_heat) / len(pre_heat)
        # Initial fopdt guesses
        params = {}
        maxtemp, maxtemptime = max([(temp, time)
                                    for time, temp in self.temp_samples])
        params['gain'] = maxtemp * 2.
        params['time_constant'] = self.temp_samples[-1][0] - maxtemptime
        params['delay'] = 10.
        # Fit smoothed fopdt model to measured temperatures
        adj_params = ('gain', 'time_constant', 'delay')
        new_params = mathutil.background_coordinate_descent(
            self.printer, adj_params, params, self.least_squares_error)
        gain = new_params['gain']
        time_constant = new_params['time_constant']
        delay = new_params['delay']
        logging.info("calc_fopdt: ambient_temp=%.3f gain=%.3f"
                     " time_constant=%.3f delay_time=%.3f",
                     self.ambient_temp, gain, time_constant, delay)
        return gain, time_constant, delay
    # Offline analysis helper
    def write_file(self, filename):
        pwm = ["pwm: %.3f %.3f" % (time, value)
               for time, value in self.pwm_samples]
        out = ["%.3f %.3f" % (time, temp) for time, temp in self.temp_samples]
        f = open(filename, "wb")
        f.write('\n'.join(pwm + out))
        f.close()

def load_config(config):
    return HeaterCalibrate(config)
