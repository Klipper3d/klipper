# Calibration of heater PID settings
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, mathutil, logging
from . import heaters

TUNE_HYSTERESIS = 2.5

class PIDCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
    def _handle_connect(self):
        gcode = self.printer.lookup_object('gcode')
        pheaters = self.printer.lookup_object('heaters')
        all_heaters = pheaters.get_all_heaters()
        for name in all_heaters:
            short_name = name.split()[-1]
            gcode.register_mux_command('PID_CALIBRATE', 'HEATER', short_name,
                                       self.cmd_PID_CALIBRATE,
                                       desc=self.cmd_PID_CALIBRATE_help)
    cmd_PID_CALIBRATE_help = "Run PID calibration test"
    def cmd_PID_CALIBRATE(self, gcmd):
        heater_name = gcmd.get('HEATER')
        target = gcmd.get_float('TARGET')
        write_file = gcmd.get_int('WRITE_FILE', 0)
        pheaters = self.printer.lookup_object('heaters')
        heater = pheaters.lookup_heater(heater_name)
        cfg_max_power = heater.get_max_power()
        max_power = gcmd.get_float('MAX_POWER', cfg_max_power * 0.75,
                                   maxval=cfg_max_power, above=0.)
        gcmd.respond_info("heat up pwm: %.2f, cycle pwm: %.2f" % (
                          max_power, cfg_max_power))
        self.printer.lookup_object('toolhead').get_last_move_time()
        reactor = self.printer.get_reactor()
        eventtime = reactor.monotonic()
        ctemp, target_temp = heater.get_temp(eventtime)
        if ctemp > target - TUNE_HYSTERESIS * 2:
           raise gcmd.error("Starting temperature should be less than target")
        calibrate = ControlAutoTune(heater, target, max_power)
        old_control = heater.set_control(calibrate)
        try:
            pheaters.set_temperature(heater, target, True)
            if self.printer.is_shutdown():
                raise self.printer.command_error("shutdown")
            calibrate.initial_heatup()
            calibrate.fit(self.printer)
            gcmd.respond_info(
                "Expected temperature +%.1f C at 100%% power\n"
                "Time constant: %.1f s" % (
                    calibrate.gain, calibrate.tau
                )
            )
            time_step = heater.get_pwm_delay()
            while calibrate.check_busy(0., 0., 0.):
                now = reactor.monotonic()
                reactor.pause(now + time_step)
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
            "Heater to thermistor lag: %.3f s" % (calibrate.dead_time_avg))
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
    def __init__(self, heater, target, max_power):
        self.heater = heater
        self.cycle_max_power = heater.get_max_power()
        self.heat_up_max_power = max_power
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
        self.heatup_samples = []
        # First Order Plus Dead Time model params
        self.gain = .0
        self.tau = .0
        self.tau_b = .0
        self.dead_time_avg = .0
    # Heater control
    def set_pwm(self, read_time, value):
        if value != self.last_pwm:
            self.pwm_samples.append((read_time, value))
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
        # Heat up stage
        if self.heating or len(self.peaks) <= 2:
            return True
        if not self.heatup_samples:
            return False
        # Oscillation stage
        if self.heating or len(self.peaks) < 9:
            return True
        return False
    def track_dead_time(self):
        if not self.pwm_samples:
            return
        last_pwm = self.pwm_samples[-1]
        # We can only forcefully add power to the system
        if last_pwm[1] == 0:
            return
        peak_time = self.peaks[-1][0]
        control_time = last_pwm[0]
        dead_time = max(peak_time - control_time, self.heater.get_pwm_delay())
        self.dead_time.append((control_time, dead_time))
    # Analysis
    def check_peaks(self):
        # Filter initial dummy 0, 0 peak
        if self.peak_time:
            self.peaks.append((self.peak_time, self.peak))
        if self.heating:
            self.peak = 9999999.
        else:
            self.peak = -9999999.
        if len(self.peaks) < 2:
            return
        # Control the balance between tight and sluggish control
        self.heater_max_power = self.cycle_max_power
        self.track_dead_time()
    def initial_heatup(self):
        if self.heatup_samples:
            return self.heatup_samples
        self.heatup_samples = self.temp_samples[:]
        end_time, _ = self.pwm_samples[1]
        while self.heatup_samples[-1][0] > end_time:
            self.heatup_samples.pop(-1)
        start_time, _ = self.pwm_samples[0]
        while self.heatup_samples[0][0] < start_time:
            self.heatup_samples.pop(0)
        # Find minimum temperature as a start point
        min_time, min_temp = self.heatup_samples[0]
        for time, temp in self.heatup_samples:
            if temp <= min_temp:
                min_time = time
                min_temp = temp
        while self.heatup_samples[0][0] < min_time:
            self.heatup_samples.pop(0)
        _, start_temp = self.heatup_samples[0]
        _, end_temp = self.heatup_samples[-1]
        self.gain = end_temp - start_temp
        # Tau is the time when the temperature reaches 63.2% of plateau
        # Describes how dynamic the system is
        temp_at_tau = start_temp + self.gain * 0.632
        for sample in self.heatup_samples:
            if sample[1] > temp_at_tau:
                self.tau = sample[0] - self.heatup_samples[0][0]
                break
        return self.heatup_samples
    def model(self, params):
        gain = params['gain']
        time_constant = params['time_constant']
        tau_b = params['tau_b']
        samples = self.heatup_samples
        T0 = samples[0][1]
        start_time = samples[0][0]
        for time, temp in samples:
            dt = time - start_time
            if dt <= .0:
                yield T0
            else:
                x = (1.0 - math.exp(-dt/time_constant))
                x2 = (1.0 - math.exp(-dt/tau_b))
                yield T0 + gain * x * x2
    def least_squares_error(self, params):
        gain = params['gain']
        time_constant = params['time_constant']
        tau_b = params['tau_b']
        if gain <= 0. or time_constant <= 0. or tau_b <= 0.:
            return 9.9e99
        temps = (temp for time, temp in self.heatup_samples)
        temps_fit = self.model(params)
        err = .0
        for temp, temp_fit in zip(temps, temps_fit):
            err += (temp_fit - temp)**2
        return err
    def fit(self, printer):
        params = {
            'gain': self.gain,
            'time_constant': self.tau,
            # Allow filtering the first-order term from the S curve for free
            # Roughly represents secondary lag in the system
            'tau_b': 1.0
        }
        # Fit FOPDT model to measured temperatures
        adj_params = ('gain', 'time_constant', 'tau_b')
        new_params = mathutil.background_coordinate_descent(
            printer, adj_params, params, self.least_squares_error)
        self.gain = new_params['gain'] / self.heat_up_max_power
        self.tau = new_params['time_constant']
        self.tau_b = new_params['tau_b']
    def calc_final_pid(self):
        peak_temp = max([temp for time, temp in self.peaks])
        # dead time is theta
        dead_time = [dtime for _, dtime in self.dead_time]
        self.dead_time_avg = max(0.6, sum(dead_time)/len(dead_time))
        # Use Skogestad IMC to estimate Kc, Ti, Td
        gain = self.gain
        tau = self.tau
        theta = self.dead_time_avg
        Kc = (1 / gain) * tau / (theta + theta)
        Ti = min(tau, 8 * theta)
        Td = theta / 2
        Kp = Kc * heaters.PID_PARAM_BASE
        Ki = Kp / Ti
        Kd = Kp * Td
        msg = "Autotune: %.3fC/%.3f | " % (peak_temp, self.heat_up_max_power)
        msg += "Gain=%.3f Tau=%.3f DeadT=%.3f TauB=%.3f | " % (
                gain, tau, theta, self.tau_b)
        msg += "Kp=%f Ki=%f Kd=%f" % (Kp, Ki, Kd)
        logging.info(msg)
        return Kp, Ki, Kd
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
