# Calibration of heater PID settings
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import heaters

class PIDCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('PID_CALIBRATE', self.cmd_PID_CALIBRATE,
                               desc=self.cmd_PID_CALIBRATE_help)
    cmd_PID_CALIBRATE_help = "Run PID calibration test"

    def _calibrate(self, pheaters, heater, target, fname, gcmd,
                   calibrate_secondary):
        if isinstance(heater.control, heaters.ControlDualLoopPID) and \
                calibrate_secondary:
            sec_target = heater.control.sec_max_temp_target
            calibrate = ControlAutoTune(heater, sec_target, heater.control,
                                        calibrate_secondary=calibrate_secondary)
        elif isinstance(heater.control, heaters.ControlDualLoopPID) and \
                not calibrate_secondary:
            calibrate = ControlAutoTune(heater, target, heater.control,
                                        calibrate_secondary=calibrate_secondary)
        else:
            calibrate = ControlAutoTune(heater, target)
        old_control = heater.set_control(calibrate)
        try:
            pheaters.set_temperature(heater, target, True)
        except self.printer.command_error as e:
            heater.set_control(old_control)
            raise
        heater.set_control(old_control)
        if fname is not None:
            calibrate.write_file(fname)
        if calibrate.check_busy(0., 0., 0.):
            raise gcmd.error("pid_calibrate interrupted")
        # Log and report results
        kp, ki, kd = calibrate.calc_final_pid()

        return kp, ki, kd

    def cmd_PID_CALIBRATE(self, gcmd):
        heater_name = gcmd.get('HEATER')
        target = gcmd.get_float('TARGET')
        write_file = gcmd.get_int('WRITE_FILE', 0)
        pheaters = self.printer.lookup_object('heaters')
        try:
            heater = pheaters.lookup_heater(heater_name)
        except self.printer.config_error as e:
            raise gcmd.error(str(e))
        self.printer.lookup_object('toolhead').get_last_move_time()

        if isinstance(heater.control, heaters.ControlDualLoopPID):
            fname = '/tmp/heattest_secondary.txt' if write_file else None
            kp_s, ki_s, kd_s = self._calibrate(pheaters, heater, target,
                                               fname, gcmd,
                                               calibrate_secondary=True)
            old_kp = heater.control.secondary_pid.Kp
            old_ki = heater.control.secondary_pid.Ki
            old_kd = heater.control.secondary_pid.Kd
            heater.control.secondary_pid.kp = kp_s
            heater.control.secondary_pid.ki = ki_s
            heater.control.secondary_pid.kd = kd_s
            fname = '/tmp/heattest_primary.txt' if write_file else None
            kp_p, ki_p, kd_p = self._calibrate(pheaters, heater, target,
                                               fname, gcmd,
                                               calibrate_secondary=False)
            heater.control.secondary_pid.kp = old_kp
            heater.control.secondary_pid.ki = old_ki
            heater.control.secondary_pid.kd = old_kd
            gcmd.respond_info(
                "PID parameters: primary_pid_Kp=%.3f primary_pid_Ki=%.3f"
                "primary_pid_Kd=%.3f\n"
                "secondary_pid_Kp=%.3f secondary_pid_Ki=%.3f"
                "secondary_pid_Kd=%.3f\n"
                "The SAVE_CONFIG command will update the printer config file\n"
                "with these parameters and restart the printer."
                % (kp_p, ki_p, kd_p, kp_s, ki_s, kd_s))
            # Store results for SAVE_CONFIG
            configfile = self.printer.lookup_object('configfile')
            configfile.set(heater_name, 'control', 'dual_loop_pid')
            configfile.set(heater_name, 'primary_pid_Kp', "%.3f" % (kp_p,))
            configfile.set(heater_name, 'primary_pid_Ki', "%.3f" % (ki_p,))
            configfile.set(heater_name, 'primary_pid_Kd', "%.3f" % (kd_p,))
            configfile.set(heater_name, 'secondary_pid_Kp', "%.3f" % (kp_s,))
            configfile.set(heater_name, 'secondary_pid_Ki', "%.3f" % (ki_s,))
            configfile.set(heater_name, 'secondary_pid_Kd', "%.3f" % (kd_s,))
        else:
            fname = '/tmp/heattest.txt' if write_file else None
            Kp, Ki, Kd = self._calibrate(pheaters, heater, target, fname, gcmd,
                                         calibrate_secondary=False)
            logging.info("Autotune: final: Kp=%f Ki=%f Kd=%f", Kp, Ki, Kd)
            gcmd.respond_info(
                "PID parameters: pid_Kp=%.3f pid_Ki=%.3f pid_Kd=%.3f\n"
                "The SAVE_CONFIG command will update the printer config file\n"
                "with these parameters and restart the printer." % (Kp, Ki, Kd))
            # Store results for SAVE_CONFIG
            configfile = self.printer.lookup_object('configfile')
            configfile.set(heater_name, 'control', 'pid')
            configfile.set(heater_name, 'pid_Kp', "%.3f" % (Kp,))
            configfile.set(heater_name, 'pid_Ki', "%.3f" % (Ki,))
            configfile.set(heater_name, 'pid_Kd', "%.3f" % (Kd,))

TUNE_PID_DELTA = 5.0

class ControlAutoTune:
    def __init__(self, heater, target, control=None, calibrate_secondary=False):
        self.heater = heater
        self.heater_max_power = heater.get_max_power()
        self.calibrate_temp = target
        # Heating control
        self.heating = False
        self.peak = 0.
        self.peak_time = 0.
        # Peak recording
        self.peaks = []
        # Sample recording
        self.last_pwm = 0.
        self.pwm_samples = []
        self.temp_samples = []
        # Control for Dual loop pid
        self._control = control
        self._calibrate_secondary = calibrate_secondary
    # Heater control
    def set_pwm(self, read_time, value):
        if value != self.last_pwm:
            self.pwm_samples.append(
                (read_time + self.heater.get_pwm_delay(), value))
            self.last_pwm = value
        self.heater.set_pwm(read_time, value)
    def temperature_update(self, read_time, primary_temp, target_temp,
                           secondary_temp=None):

        if self._calibrate_secondary:
            ref_temp = secondary_temp
        else:
            ref_temp = primary_temp

        self.temp_samples.append((read_time, ref_temp))
        # Check if the temperature has crossed the target and
        # enable/disable the heater if so.
        if self.heating and ref_temp >= target_temp:
            self.heating = False
            self.check_peaks()
            self.heater.alter_target(self.calibrate_temp - TUNE_PID_DELTA)
        elif not self.heating and ref_temp <= target_temp:
            self.heating = True
            self.check_peaks()
            self.heater.alter_target(self.calibrate_temp)
        # Check if this temperature is a peak and record it if so
        if self.heating:
            if self._control is not None and not self._calibrate_secondary:
                pid = self._control.secondary_pid
                sec_target = self._control.sec_max_temp_target
                _, bounded_co = pid.calculate_output(read_time, secondary_temp,
                                                     sec_target)
                self.set_pwm(read_time, bounded_co)
            else:
                self.set_pwm(read_time, self.heater_max_power)
            if ref_temp < self.peak:
                self.peak = ref_temp
                self.peak_time = read_time
        else:
            self.set_pwm(read_time, 0.)
            if ref_temp > self.peak:
                self.peak = ref_temp
                self.peak_time = read_time
    def check_busy(self, eventtime, smoothed_temp, target_temp):
        if self.heating or len(self.peaks) < 12:
            return True
        return False
    # Analysis
    def check_peaks(self):
        self.peaks.append((self.peak, self.peak_time))
        if self.heating:
            self.peak = 9999999.
        else:
            self.peak = -9999999.
        if len(self.peaks) < 4:
            return
        self.calc_pid(len(self.peaks)-1)
    def calc_pid(self, pos):
        temp_diff = self.peaks[pos][0] - self.peaks[pos-1][0]
        time_diff = self.peaks[pos][1] - self.peaks[pos-2][1]
        # Use Astrom-Hagglund method to estimate Ku and Tu
        amplitude = .5 * abs(temp_diff)
        Ku = 4. * self.heater_max_power / (math.pi * amplitude)
        Tu = time_diff
        # Use Ziegler-Nichols method to generate PID parameters
        Ti = 0.5 * Tu
        Td = 0.125 * Tu
        Kp = 0.6 * Ku * heaters.PID_PARAM_BASE
        Ki = Kp / Ti
        Kd = Kp * Td
        logging.info("Autotune: raw=%f/%f Ku=%f Tu=%f  Kp=%f Ki=%f Kd=%f",
                     temp_diff, self.heater_max_power, Ku, Tu, Kp, Ki, Kd)
        return Kp, Ki, Kd
    def calc_final_pid(self):
        cycle_times = [(self.peaks[pos][1] - self.peaks[pos-2][1], pos)
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
