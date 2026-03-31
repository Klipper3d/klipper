# Calibration of heater PID settings
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import heaters

TUNE_HYSTERESIS = 5.0

class PIDCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('PID_CALIBRATE', self.cmd_PID_CALIBRATE,
                               desc=self.cmd_PID_CALIBRATE_help)
    cmd_PID_CALIBRATE_help = "Run PID calibration test"
    def cmd_PID_CALIBRATE(self, gcmd):
        heater_name = gcmd.get('HEATER')
        target = gcmd.get_float('TARGET')
        write_file = gcmd.get_int('WRITE_FILE', 0)
        pheaters = self.printer.lookup_object('heaters')
        try:
            heater = pheaters.lookup_heater(heater_name)
        except self.printer.config_error as e:
            raise gcmd.error(str(e))
        cfg_max_power = heater.get_max_power()
        max_power = gcmd.get_float('MAX_POWER', cfg_max_power,
                                   maxval=cfg_max_power, above=0.)
        self.printer.lookup_object('toolhead').get_last_move_time()
        reactor = self.printer.get_reactor()
        eventtime = reactor.monotonic()
        ctemp, target_temp = heater.get_temp(eventtime)
        if ctemp > target - TUNE_HYSTERESIS:
           raise gcmd.error("Starting temperature should be less than target")
        calibrate = ControlAutoTune(heater, target, ctemp, max_power)
        old_control = heater.set_control(calibrate)
        try:
            pheaters.set_temperature(heater, target, True)
        except self.printer.command_error as e:
            heater.set_control(old_control)
            raise
        heater.set_control(old_control)
        if write_file:
            calibrate.write_file('/tmp/heattest.txt')
        if calibrate.check_busy(0., 0., 0.):
            raise gcmd.error("pid_calibrate interrupted")
        # Log and report results
        Kp, Ki, Kd = calibrate.calc_final_pid()
        logging.info("Autotune: final: Kp=%f Ki=%f Kd=%f", Kp, Ki, Kd)
        gcmd.respond_info(
            "PID parameters: pid_Kp=%.3f pid_Ki=%.3f pid_Kd=%.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with these parameters and restart the printer." % (Kp, Ki, Kd))
        # Store results for SAVE_CONFIG
        cfgname = heater.get_name()
        configfile = self.printer.lookup_object('configfile')
        configfile.set(cfgname, 'control', 'pid')
        configfile.set(cfgname, 'pid_Kp', "%.3f" % (Kp,))
        configfile.set(cfgname, 'pid_Ki', "%.3f" % (Ki,))
        configfile.set(cfgname, 'pid_Kd', "%.3f" % (Kd,))

class ControlAutoTune:
    def __init__(self, heater, target, start_temp, max_power):
        self.heater = heater
        self.heater_max_power = max_power
        self.calibrate_temp = target
        # Heating control
        self.heating = False
        self.peak = 0.
        self.peak_time = 0.
        self.hysteresis = TUNE_HYSTERESIS
        # Peak recording
        self.peaks = []
        # Sample recording
        self.last_pwm = 0.
        self.pwm_samples = []
        self.temp_samples = []
        # Track dead time
        self.dead_time = []
        # Track initial heat-up curve
        self.start_temp = start_temp + TUNE_HYSTERESIS / 2
        self.heatup_samples = []
        # First Order Plus Dead Time model params
        self.min_gain = .0
        self.min_tau = .0
        self.dead_time_avg = .0
    # Heater control
    def set_pwm(self, read_time, value):
        if value != self.last_pwm:
            self.pwm_samples.append(
                (read_time + self.heater.get_pwm_delay(), value))
            self.last_pwm = value
        self.heater.set_pwm(read_time, value)
    def temperature_update(self, read_time, temp, target_temp):
        self.temp_samples.append((read_time, temp))
        # Check if the temperature has crossed the target and
        # enable/disable the heater if so.
        if self.heating and temp >= target_temp:
            self.heating = False
            self.check_peaks()
            self.heater.alter_target(self.calibrate_temp - self.hysteresis)
        elif not self.heating and temp <= target_temp:
            self.heating = True
            self.check_peaks()
            self.heater.alter_target(self.calibrate_temp)
        # Check if this temperature is a peak and record it if so
        if self.heating:
            self.set_pwm(read_time, self.heater_max_power)
            if temp < self.peak:
                self.peak = temp
                self.peak_time = read_time
        else:
            self.set_pwm(read_time, 0.)
            if temp > self.peak:
                self.peak = temp
                self.peak_time = read_time
    def check_busy(self, eventtime, smoothed_temp, target_temp):
        if self.heating or len(self.peaks) < 12:
            return True
        return False
    def track_dead_time(self):
        if not self.pwm_samples:
            return
        last_pwm = self.pwm_samples[-1]
        last_peak = self.peaks[-1]
        control_time = last_pwm[0] - self.heater.get_pwm_delay()
        dead_time = last_peak[0] - control_time
        dead_time = max(dead_time, self.heater.get_pwm_delay())
        # Rough estimate
        # We can only forcefully add power to the system
        if last_pwm[1] > 0:
            self.dead_time.append((control_time, dead_time))
    # Analysis
    def check_peaks(self):
        # Filter initial dummy 0, 0 peak
        if self.peak_time:
            self.peaks.append((self.peak_time, self.peak))
            self.track_dead_time()
        if self.heating:
            self.peak = 9999999.
        else:
            self.peak = -9999999.
        if len(self.peaks) < 4:
            return
        self.calc_pid(len(self.peaks)-1)
    def initial_heatup(self):
        if self.heatup_samples:
            return self.heatup_samples
        start_time = .0
        for sample in self.temp_samples:
            if sample[1] > self.start_temp:
                start_time = sample[0]
                break
        end_time = self.pwm_samples[1][0]
        for sample in self.temp_samples:
            if start_time < sample[0] < end_time:
                self.heatup_samples.append(sample)
        return self.heatup_samples
    def calc_pid(self, pos):
        peak_temp = max([temp for time, temp in self.peaks])
        self.min_gain = peak_temp - self.start_temp + TUNE_HYSTERESIS/2
        self.initial_heatup()
        # Tau is the time when the temperature reaches 63.2% of plateau
        # Describes how dynamic the system is
        temp_at_tau = self.start_temp + self.min_gain * 0.632
        for sample in self.heatup_samples:
            if sample[1] > temp_at_tau:
                self.min_tau = sample[0] - self.heatup_samples[0][0]
        # dead time is theta
        dead_time = [dtime for _, dtime in self.dead_time]
        self.dead_time_avg = max(0.3, sum(dead_time)/len(dead_time))
        # Use Skogestad IMC to estimate Kc, Ti
        # Lambda/tau constant adjustment coefficient, stabilization time target
        # Must be >= dead_time
        tau_const = max(self.dead_time_avg, 1.0)
        gain = self.min_gain
        tau = self.min_tau
        theta = self.dead_time_avg
        Kc = (1 / gain) * tau / (theta + tau_const)
        Ti = min(tau, 4 * (theta + tau_const))
        # Use IMC PID to estimate the Td
        Td = (tau * theta) / (2 * tau + theta)
        Kp = Kc * heaters.PID_PARAM_BASE
        Ki = Kp / Ti
        Kd = Kp * Td
        msg = "Autotune: %.3fC/%.3f | " % (peak_temp, self.heater_max_power)
        msg += "Gain=%.3f Tau=%.3f DeadT=%.3f | " % (gain, tau, theta)
        msg += "Kp=%f Ki=%f Kd=%f" % (Kp, Ki, Kd)
        logging.info(msg)
        return Kp, Ki, Kd
    def calc_final_pid(self):
        cycle_times = [(self.peaks[pos][0] - self.peaks[pos-2][0], pos)
                       for pos in range(4, len(self.peaks))]
        midpoint_pos = sorted(cycle_times)[len(cycle_times)//2][1]
        return self.calc_pid(midpoint_pos)
    # Offline analysis helper
    def write_file(self, filename):
        pwm = ["pwm: %.3f %.3f" % (time, value)
               for time, value in self.pwm_samples]
        out = ["%.3f %.3f" % (time, temp) for time, temp in self.temp_samples]
        f = open(filename, "w")
        f.write('\n'.join(pwm + out))
        f.close()

def load_config(config):
    return PIDCalibrate(config)
