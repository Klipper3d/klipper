# Printer heater support
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, threading
import pins


######################################################################
# Sensors
######################################################################

KELVIN_TO_CELCIUS = -273.15

# Thermistor calibrated with three temp measurements
class Thermistor:
    def __init__(self, config, params):
        self.pullup = config.getfloat('pullup_resistor', 4700., above=0.)
        # Calculate Steinhart-Hart coefficents from temp measurements
        inv_t1 = 1. / (params['t1'] - KELVIN_TO_CELCIUS)
        inv_t2 = 1. / (params['t2'] - KELVIN_TO_CELCIUS)
        inv_t3 = 1. / (params['t3'] - KELVIN_TO_CELCIUS)
        ln_r1 = math.log(params['r1'])
        ln_r2 = math.log(params['r2'])
        ln_r3 = math.log(params['r3'])
        ln3_r1, ln3_r2, ln3_r3 = ln_r1**3, ln_r2**3, ln_r3**3

        inv_t12, inv_t13 = inv_t1 - inv_t2, inv_t1 - inv_t3
        ln_r12, ln_r13 = ln_r1 - ln_r2, ln_r1 - ln_r3
        ln3_r12, ln3_r13 = ln3_r1 - ln3_r2, ln3_r1 - ln3_r3

        self.c3 = ((inv_t12 - inv_t13 * ln_r12 / ln_r13)
                   / (ln3_r12 - ln3_r13 * ln_r12 / ln_r13))
        self.c2 = (inv_t12 - self.c3 * ln3_r12) / ln_r12
        self.c1 = inv_t1 - self.c2 * ln_r1 - self.c3 * ln3_r1
    def calc_temp(self, adc):
        adc = max(.00001, min(.99999, adc))
        r = self.pullup * adc / (1.0 - adc)
        ln_r = math.log(r)
        inv_t = self.c1 + self.c2 * ln_r + self.c3 * ln_r**3
        return 1.0/inv_t + KELVIN_TO_CELCIUS
    def calc_adc(self, temp):
        inv_t = 1. / (temp - KELVIN_TO_CELCIUS)
        if self.c3:
            y = (self.c1 - inv_t) / (2. * self.c3)
            x = math.sqrt((self.c2 / (3. * self.c3))**3 + y**2)
            ln_r = math.pow(x - y, 1./3.) - math.pow(x + y, 1./3.)
        else:
            ln_r = (inv_t - self.c1) / self.c2
        r = math.exp(ln_r)
        return r / (self.pullup + r)

# Thermistor calibrated from one temp measurement and its beta
class ThermistorBeta(Thermistor):
    def __init__(self, config, params):
        self.pullup = config.getfloat('pullup_resistor', 4700., above=0.)
        # Calculate Steinhart-Hart coefficents from beta
        inv_t1 = 1. / (params['t1'] - KELVIN_TO_CELCIUS)
        ln_r1 = math.log(params['r1'])
        self.c3 = 0.
        self.c2 = 1. / params['beta']
        self.c1 = inv_t1 - self.c2 * ln_r1

# Linear style conversion chips calibrated with two temp measurements
class Linear:
    def __init__(self, config, params):
        adc_voltage = config.getfloat('adc_voltage', 5., above=0.)
        slope = (params['t2'] - params['t1']) / (params['v2'] - params['v1'])
        self.gain = adc_voltage * slope
        self.offset = params['t1'] - params['v1'] * slope
    def calc_temp(self, adc):
        return adc * self.gain + self.offset
    def calc_adc(self, temp):
        return (temp - self.offset) / self.gain

# Available sensors
Sensors = {
    "EPCOS 100K B57560G104F": {
        'class': Thermistor, 't1': 25., 'r1': 100000.,
        't2': 150., 'r2': 1641.9, 't3': 250., 'r3': 226.15 },
    "ATC Semitec 104GT-2": {
        'class': Thermistor, 't1': 20., 'r1': 126800.,
        't2': 150., 'r2': 1360., 't3': 300., 'r3': 80.65 },
    "NTC 100K beta 3950": {
        'class': ThermistorBeta, 't1': 25., 'r1': 100000., 'beta': 3950. },
    "AD595": { 'class': Linear, 't1': 25., 'v1': .25, 't2': 300., 'v2': 3.022 },
}


######################################################################
# Heater
######################################################################

SAMPLE_TIME = 0.001
SAMPLE_COUNT = 8
REPORT_TIME = 0.300
MAX_HEAT_TIME = 5.0
AMBIENT_TEMP = 25.
PID_PARAM_BASE = 255.

class error(Exception):
    pass

class PrinterHeater:
    error = error
    def __init__(self, printer, config):
        self.printer = printer
        self.name = config.get_name()
        sensor_params = config.getchoice('sensor_type', Sensors)
        self.sensor = sensor_params['class'](config, sensor_params)
        self.min_temp = config.getfloat('min_temp', minval=0.)
        self.max_temp = config.getfloat('max_temp', above=self.min_temp)
        self.min_extrude_temp = config.getfloat(
            'min_extrude_temp', 170., minval=self.min_temp, maxval=self.max_temp)
        self.max_power = config.getfloat('max_power', 1., above=0., maxval=1.)
        self.lock = threading.Lock()
        self.last_temp = 0.
        self.last_temp_time = 0.
        self.target_temp = 0.
        algos = {'watermark': ControlBangBang, 'pid': ControlPID}
        algo = config.getchoice('control', algos)
        heater_pin = config.get('heater_pin')
        if algo is ControlBangBang and self.max_power == 1.:
            self.mcu_pwm = pins.setup_pin(printer, 'digital_out', heater_pin)
        else:
            self.mcu_pwm = pins.setup_pin(printer, 'pwm', heater_pin)
            pwm_cycle_time = config.getfloat(
                'pwm_cycle_time', 0.100, above=0., maxval=REPORT_TIME)
            self.mcu_pwm.setup_cycle_time(pwm_cycle_time)
        self.mcu_pwm.setup_max_duration(MAX_HEAT_TIME)
        self.mcu_adc = pins.setup_pin(printer, 'adc', config.get('sensor_pin'))
        adc_range = [self.sensor.calc_adc(self.min_temp),
                     self.sensor.calc_adc(self.max_temp)]
        self.mcu_adc.setup_minmax(SAMPLE_TIME, SAMPLE_COUNT,
                                  minval=min(adc_range), maxval=max(adc_range))
        self.mcu_adc.setup_adc_callback(REPORT_TIME, self.adc_callback)
        is_fileoutput = self.mcu_adc.get_mcu().is_fileoutput()
        self.can_extrude = self.min_extrude_temp <= 0. or is_fileoutput
        self.control = algo(self, config)
        # pwm caching
        self.next_pwm_time = 0.
        self.last_pwm_value = 0
    def set_pwm(self, read_time, value):
        if self.target_temp <= 0.:
            value = 0.
        if ((read_time < self.next_pwm_time or not self.last_pwm_value)
            and abs(value - self.last_pwm_value) < 0.05):
            # No significant change in value - can suppress update
            return
        pwm_time = read_time + REPORT_TIME + SAMPLE_TIME*SAMPLE_COUNT
        self.next_pwm_time = pwm_time + 0.75 * MAX_HEAT_TIME
        self.last_pwm_value = value
        logging.debug("%s: pwm=%.3f@%.3f (from %.3f@%.3f [%.3f])",
                      self.name, value, pwm_time,
                      self.last_temp, self.last_temp_time, self.target_temp)
        self.mcu_pwm.set_pwm(pwm_time, value)
    def adc_callback(self, read_time, read_value):
        temp = self.sensor.calc_temp(read_value)
        with self.lock:
            self.last_temp = temp
            self.last_temp_time = read_time
            self.can_extrude = (temp >= self.min_extrude_temp)
            self.control.adc_callback(read_time, temp)
        #logging.debug("temp: %.3f %f = %f", read_time, read_value, temp)
    # External commands
    def set_temp(self, print_time, degrees):
        if degrees and (degrees < self.min_temp or degrees > self.max_temp):
            raise error("Requested temperature (%.1f) out of range (%.1f:%.1f)"
                        % (degrees, self.min_temp, self.max_temp))
        with self.lock:
            self.target_temp = degrees
    def get_temp(self, eventtime):
        print_time = self.mcu_adc.get_mcu().estimated_print_time(eventtime) - 5.
        with self.lock:
            if self.last_temp_time < print_time:
                return 0., self.target_temp
            return self.last_temp, self.target_temp
    def check_busy(self, eventtime):
        with self.lock:
            return self.control.check_busy(eventtime)
    def start_auto_tune(self, degrees):
        if degrees and (degrees < self.min_temp or degrees > self.max_temp):
            raise error("Requested temperature (%.1f) out of range (%.1f:%.1f)"
                        % (degrees, self.min_temp, self.max_temp))
        with self.lock:
            self.control = ControlAutoTune(self, self.control)
            self.target_temp = degrees
    def finish_auto_tune(self, old_control):
        self.control = old_control
        self.target_temp = 0


######################################################################
# Bang-bang control algo
######################################################################

class ControlBangBang:
    def __init__(self, heater, config):
        self.heater = heater
        self.max_delta = config.getfloat('max_delta', 2.0, above=0.)
        self.heating = False
    def adc_callback(self, read_time, temp):
        if self.heating and temp >= self.heater.target_temp+self.max_delta:
            self.heating = False
        elif not self.heating and temp <= self.heater.target_temp-self.max_delta:
            self.heating = True
        if self.heating:
            self.heater.set_pwm(read_time, self.heater.max_power)
        else:
            self.heater.set_pwm(read_time, 0.)
    def check_busy(self, eventtime):
        return self.heater.last_temp < self.heater.target_temp-self.max_delta


######################################################################
# Proportional Integral Derivative (PID) control algo
######################################################################

PID_SETTLE_DELTA = 1.
PID_SETTLE_SLOPE = .1

class ControlPID:
    def __init__(self, heater, config):
        self.heater = heater
        self.Kp = config.getfloat('pid_Kp') / PID_PARAM_BASE
        self.Ki = config.getfloat('pid_Ki') / PID_PARAM_BASE
        self.Kd = config.getfloat('pid_Kd') / PID_PARAM_BASE
        self.min_deriv_time = config.getfloat('pid_deriv_time', 2., above=0.)
        imax = config.getfloat('pid_integral_max', heater.max_power, minval=0.)
        self.temp_integ_max = imax / self.Ki
        self.prev_temp = AMBIENT_TEMP
        self.prev_temp_time = 0.
        self.prev_temp_deriv = 0.
        self.prev_temp_integ = 0.
    def adc_callback(self, read_time, temp):
        time_diff = read_time - self.prev_temp_time
        # Calculate change of temperature
        temp_diff = temp - self.prev_temp
        if time_diff >= self.min_deriv_time:
            temp_deriv = temp_diff / time_diff
        else:
            temp_deriv = (self.prev_temp_deriv * (self.min_deriv_time-time_diff)
                          + temp_diff) / self.min_deriv_time
        # Calculate accumulated temperature "error"
        temp_err = self.heater.target_temp - temp
        temp_integ = self.prev_temp_integ + temp_err * time_diff
        temp_integ = max(0., min(self.temp_integ_max, temp_integ))
        # Calculate output
        co = self.Kp*temp_err + self.Ki*temp_integ - self.Kd*temp_deriv
        #logging.debug("pid: %f@%.3f -> diff=%f deriv=%f err=%f integ=%f co=%d",
        #    temp, read_time, temp_diff, temp_deriv, temp_err, temp_integ, co)
        bounded_co = max(0., min(self.heater.max_power, co))
        self.heater.set_pwm(read_time, bounded_co)
        # Store state for next measurement
        self.prev_temp = temp
        self.prev_temp_time = read_time
        self.prev_temp_deriv = temp_deriv
        if co == bounded_co:
            self.prev_temp_integ = temp_integ
    def check_busy(self, eventtime):
        temp_diff = self.heater.target_temp - self.heater.last_temp
        return (abs(temp_diff) > PID_SETTLE_DELTA
                or abs(self.prev_temp_deriv) > PID_SETTLE_SLOPE)


######################################################################
# Ziegler-Nichols PID autotuning
######################################################################

TUNE_PID_DELTA = 5.0

class ControlAutoTune:
    def __init__(self, heater, old_control):
        self.heater = heater
        self.old_control = old_control
        self.heating = False
        self.peaks = []
        self.peak = 0.
        self.peak_time = 0.
    def adc_callback(self, read_time, temp):
        if self.heating and temp >= self.heater.target_temp:
            self.heating = False
            self.check_peaks()
        elif (not self.heating
              and temp <= self.heater.target_temp - TUNE_PID_DELTA):
            self.heating = True
            self.check_peaks()
        if self.heating:
            self.heater.set_pwm(read_time, self.heater.max_power)
            if temp < self.peak:
                self.peak = temp
                self.peak_time = read_time
        else:
            self.heater.set_pwm(read_time, 0.)
            if temp > self.peak:
                self.peak = temp
                self.peak_time = read_time
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
        max_power = self.heater.max_power
        Ku = 4. * (2. * max_power) / (abs(temp_diff) * math.pi)
        Tu = time_diff

        Ti = 0.5 * Tu
        Td = 0.125 * Tu
        Kp = 0.6 * Ku * PID_PARAM_BASE
        Ki = Kp / Ti
        Kd = Kp * Td
        logging.info("Autotune: raw=%f/%f Ku=%f Tu=%f  Kp=%f Ki=%f Kd=%f",
                     temp_diff, max_power, Ku, Tu, Kp, Ki, Kd)
        return Kp, Ki, Kd
    def final_calc(self):
        cycle_times = [(self.peaks[pos][1] - self.peaks[pos-2][1], pos)
                       for pos in range(4, len(self.peaks))]
        midpoint_pos = sorted(cycle_times)[len(cycle_times)/2][1]
        Kp, Ki, Kd = self.calc_pid(midpoint_pos)
        logging.info("Autotune: final: Kp=%f Ki=%f Kd=%f", Kp, Ki, Kd)
        gcode = self.heater.printer.lookup_object('gcode')
        gcode.respond_info(
            "PID parameters: pid_Kp=%.3f pid_Ki=%.3f pid_Kd=%.3f\n"
            "To use these parameters, update the printer config file with\n"
            "the above and then issue a RESTART command" % (Kp, Ki, Kd))
    def check_busy(self, eventtime):
        if self.heating or len(self.peaks) < 12:
            return True
        self.final_calc()
        self.heater.finish_auto_tune(self.old_control)
        return False


######################################################################
# Tuning information test
######################################################################

class ControlBumpTest:
    def __init__(self, heater, old_control):
        self.heater = heater
        self.old_control = old_control
        self.temp_samples = {}
        self.pwm_samples = {}
        self.state = 0
    def set_pwm(self, read_time, value):
        self.pwm_samples[read_time + 2*REPORT_TIME] = value
        self.heater.set_pwm(read_time, value)
    def adc_callback(self, read_time, temp):
        self.temp_samples[read_time] = temp
        if not self.state:
            self.set_pwm(read_time, 0.)
            if len(self.temp_samples) >= 20:
                self.state += 1
        elif self.state == 1:
            if temp < self.heater.target_temp:
                self.set_pwm(read_time, self.heater.max_power)
                return
            self.set_pwm(read_time, 0.)
            self.state += 1
        elif self.state == 2:
            self.set_pwm(read_time, 0.)
            if temp <= (self.heater.target_temp + AMBIENT_TEMP) / 2.:
                self.dump_stats()
                self.state += 1
    def dump_stats(self):
        out = ["%.3f %.1f %d" % (time, temp, self.pwm_samples.get(time, -1.))
               for time, temp in sorted(self.temp_samples.items())]
        f = open("/tmp/heattest.txt", "wb")
        f.write('\n'.join(out))
        f.close()
    def check_busy(self, eventtime):
        if self.state < 3:
            return True
        self.heater.finish_auto_tune(self.old_control)
        return False

def add_printer_objects(printer, config):
    if config.has_section('heater_bed'):
        printer.add_object('heater_bed', PrinterHeater(
            printer, config.getsection('heater_bed')))
