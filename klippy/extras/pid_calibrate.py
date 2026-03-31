# Calibration of heater PID settings
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import heaters

TUNE_HYSTERESIS = 2.5

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
            "Expected temperature +%.1f C at 100%% power\n"
            "Time constant: %.1f s\n"
            "Heater to thermistor lag: %.3fs" % (
                calibrate.gain, calibrate.tau, calibrate.dead_time_avg
            )
        )
        if calibrate.min_gain > calibrate.gain:
            gcmd.respond_raw("!! Fit can be suboptimal")
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
        self.start_temp = start_temp + self.hysteresis
        self.heatup_samples = []
        # Model FIT
        self.ys = []
        # Time constant fit
        self.L = 0.001
        self.R = 3600
        # First Order Plus Dead Time model params
        self.min_gain = .0
        self.gain = .0
        self.tau = .0
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
    def _populate_ys(self, samples):
        if self.ys:
            return self.ys
        T0 = samples[0][1]
        ys = [.0]
        for t, temp in samples[1:]:
            ys.append(temp - T0)
        self.ys = ys
        return self.ys
    def calc_eval_error(self, samples, tau_candidate):
        start_time = samples[0][0]
        # We work with part of the asymptote
        # Let's imagine the "full/real" one and fit over it
        xs = [.0]
        ys = self._populate_ys(samples)
        for t, temp in samples[1:]:
            dt = t - start_time
            e = math.exp(-dt/tau_candidate)
            xs.append(1.0 - e)
        sum_xy = .0
        sum_xx = .0
        for x, y in zip(xs, ys):
            sum_xy += x * y
            sum_xx += x * x
        A = sum_xy / sum_xx
        err = .0
        for x, y in zip(xs, ys):
            err += (A * x - y) ** 2
        return err, A
    def fit_model(self, samples, final=False):
        # ternary search
        while abs(self.R - self.L) > 0.01:
            L_third = self.L + (self.R - self.L) / 3
            R_third = self.R - (self.R - self.L) / 3
            L_err, _ = self.calc_eval_error(samples, L_third)
            R_err, _ = self.calc_eval_error(samples, R_third)
            if L_err < R_err:
                self.R = R_third
            else:
                self.L = L_third
            # Fit over several calls
            if not final:
                break
        tau = (self.L + self.R) / 2
        _, A = self.calc_eval_error(samples, tau)
        return A, tau
    def calc_pid(self, pos, final=False):
        peak_temp = max([temp for time, temp in self.peaks])
        self.min_gain = peak_temp - self.start_temp + TUNE_HYSTERESIS
        self.initial_heatup()
        # dead time is theta
        dead_time = [dtime for _, dtime in self.dead_time]
        self.dead_time_avg = max(0.3, sum(dead_time)/len(dead_time))
        # Use Skogestad IMC to estimate Kc, Ti
        # Lambda/tau constant adjustment coefficient, stabilization time target
        # Must be >= dead_time
        tau_const = max(self.dead_time_avg, 1.0)
        # Guess "real" params
        A, self.tau = self.fit_model(self.heatup_samples, final)
        self.gain = A / self.heater_max_power
        # Fallback
        gain = max(self.gain, self.min_gain)
        tau = self.tau
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
        return self.calc_pid(midpoint_pos, final=True)
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
