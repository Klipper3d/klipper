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
        # Calculate Steinhart-Hart coefficents from temp measurements.
        # Arrange samples as 3 linear equations and solve for c1, c2, and c3.
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
            # Solve for ln_r using Cardano's formula
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
PWM_DELAY = REPORT_TIME + SAMPLE_TIME*SAMPLE_COUNT

class error(Exception):
    pass

class PrinterHeater:
    error = error
    def __init__(self, printer, config):
        self.printer = printer
        self.name = config.get_name()
        sensor_params = config.getchoice('sensor_type', Sensors)
        self.sensor = sensor_params['class'](config, sensor_params)
        self.min_temp = config.getfloat('min_temp', minval=KELVIN_TO_CELCIUS)
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
        self.last_pwm_value = 0.
        # Load additional modules
        printer.try_load_module(config, "verify_heater %s" % (self.name,))
        printer.try_load_module(config, "pid_calibrate")
    def set_pwm(self, read_time, value):
        if self.target_temp <= 0.:
            value = 0.
        if ((read_time < self.next_pwm_time or not self.last_pwm_value)
            and abs(value - self.last_pwm_value) < 0.05):
            # No significant change in value - can suppress update
            return
        pwm_time = read_time + PWM_DELAY
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
    def set_control(self, control):
        with self.lock:
            old_control = self.control
            self.control = control
            self.target_temp = 0.
        return old_control
    def stats(self, eventtime):
        with self.lock:
            target_temp = self.target_temp
            last_temp = self.last_temp
            last_pwm_value = self.last_pwm_value
        is_active = target_temp or last_temp > 50.
        return is_active, '%s: target=%.0f temp=%.1f pwm=%.3f' % (
            self.name, target_temp, last_temp, last_pwm_value)
    def get_status(self, eventtime):
        with self.lock:
            target_temp = self.target_temp
            last_temp = self.last_temp
        return {'temperature': last_temp, 'target': target_temp}


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

def add_printer_objects(printer, config):
    if config.has_section('heater_bed'):
        printer.add_object('heater_bed', PrinterHeater(
            printer, config.getsection('heater_bed')))
