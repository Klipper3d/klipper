# Printer heater support
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, threading

# Mapping from name to Steinhart-Hart coefficients
Thermistors = {
    "EPCOS 100K B57560G104F": (
        0.000722136308968056, 0.000216766566488498, 8.92935804531095e-08)
}

SAMPLE_TIME = 0.001
SAMPLE_COUNT = 8
REPORT_TIME = 0.300
KELVIN_TO_CELCIUS = -273.15
MAX_HEAT_TIME = 5.0
AMBIENT_TEMP = 25.
PWM_MAX = 255

class PrinterHeater:
    def __init__(self, printer, config):
        self.printer = printer
        self.config = config
        self.mcu_pwm = self.mcu_adc = None
        self.thermistor_c = Thermistors.get(config.get('thermistor_type'))
        self.pullup_r = config.getfloat('pullup_resistor', 4700.)
        self.lock = threading.Lock()
        self.last_temp = 0.
        self.last_temp_clock = 0
        self.target_temp = 0.
        self.report_clock = 0
        self.control = None
        # pwm caching
        self.next_pwm_clock = 0
        self.last_pwm_value = 0
        self.resend_clock = 0
        self.pwm_offset_clock = 0
    def build_config(self):
        heater_pin = self.config.get('heater_pin')
        thermistor_pin = self.config.get('thermistor_pin')
        self.mcu_pwm = self.printer.mcu.create_pwm(heater_pin, 0, MAX_HEAT_TIME)
        self.mcu_adc = self.printer.mcu.create_adc(thermistor_pin)
        min_adc = self.calc_adc(self.config.getfloat('max_temp'))
        max_adc = self.calc_adc(self.config.getfloat('min_temp'))
        freq = self.printer.mcu.get_mcu_freq()
        sample_clock = int(SAMPLE_TIME*freq)
        self.mcu_adc.set_minmax(
            sample_clock, SAMPLE_COUNT, minval=min_adc, maxval=max_adc)
        self.mcu_adc.set_adc_callback(self.adc_callback)
        self.report_clock = int(REPORT_TIME*freq)
        control_algo = self.config.get('control', 'watermark')
        algos = {'watermark': ControlBangBang, 'pid': ControlPID}
        self.control = algos[control_algo](self, self.config)
        self.next_pwm_clock = 0
        self.last_pwm_value = 0
        self.resend_clock = int(MAX_HEAT_TIME * freq * 3. / 4.)
        self.pwm_offset_clock = sample_clock*SAMPLE_COUNT + self.report_clock
    def run(self):
        self.mcu_adc.query_analog_in(self.report_clock)
    def set_pwm(self, read_clock, value):
        if value:
            if self.target_temp <= 0.:
                return
            if (read_clock < self.next_pwm_clock
                and abs(value - self.last_pwm_value) < 15):
                return
        elif not self.last_pwm_value:
            return
        pwm_clock = read_clock + self.pwm_offset_clock
        self.next_pwm_clock = pwm_clock + self.resend_clock
        self.last_pwm_value = value
        logging.debug("pwm=%d@%d (%d)" % (value, read_clock, pwm_clock))
        self.mcu_pwm.set_pwm(pwm_clock, value)
    # Temperature calculation
    def calc_temp(self, adc):
        r = self.pullup_r * adc / (1.0 - adc)
        ln_r = math.log(r)
        c1, c2, c3 = self.thermistor_c
        temp_inv = c1 + c2*ln_r + c3*math.pow(ln_r, 3)
        return 1.0/temp_inv + KELVIN_TO_CELCIUS
    def calc_adc(self, temp):
        if temp is None:
            return None
        c1, c2, c3 = self.thermistor_c
        temp -= KELVIN_TO_CELCIUS
        temp_inv = 1./temp
        y = (c1 - temp_inv) / (2*c3)
        x = math.sqrt(math.pow(c2 / (3.*c3), 3.) + math.pow(y, 2.))
        r = math.exp(math.pow(x-y, 1./3.) - math.pow(x+y, 1./3.))
        return r / (self.pullup_r + r)
    def adc_callback(self, read_clock, read_value):
        temp = self.calc_temp(float(read_value))
        with self.lock:
            self.last_temp = temp
            self.last_temp_clock = read_clock
            self.control.adc_callback(read_clock, temp)
        #logging.debug("temp: %d(%d) %f = %f" % (
        #    read_clock, read_clock & 0xffffffff, read_value, temp))
    # External commands
    def set_temp(self, print_time, degrees):
        with self.lock:
            self.target_temp = degrees
    def get_temp(self):
        with self.lock:
            return self.last_temp, self.target_temp
    def check_busy(self, eventtime):
        with self.lock:
            return self.control.check_busy(eventtime)
    def start_auto_tune(self, temp):
        with self.lock:
            self.control = ControlAutoTune(self, self.control, temp)


######################################################################
# Bang-bang control algo
######################################################################

class ControlBangBang:
    def __init__(self, heater, config):
        self.heater = heater
        self.max_delta = config.getfloat('max_delta', 2.0)
        self.heating = False
    def adc_callback(self, read_clock, temp):
        if self.heating and temp >= self.heater.target_temp+self.max_delta:
            self.heating = False
        elif not self.heating and temp <= self.heater.target_temp-self.max_delta:
            self.heating = True
        if self.heating:
            self.heater.set_pwm(read_clock, PWM_MAX)
        else:
            self.heater.set_pwm(read_clock, 0)
    def check_busy(self, eventtime):
        return self.heater.last_temp < self.heater.target_temp-self.max_delta


######################################################################
# Proportional Integral Derivative (PID) control algo
######################################################################

class ControlPID:
    def __init__(self, heater, config):
        self.heater = heater
        self.Kp = config.getfloat('pid_Kp')
        self.Ki = config.getfloat('pid_Ki')
        self.Kd = config.getfloat('pid_Kd')
        self.min_deriv_time = config.getfloat('pid_deriv_time', 2.)
        imax = config.getint('pid_integral_max', PWM_MAX)
        self.temp_integ_max = imax / self.Ki
        self.prev_temp = AMBIENT_TEMP
        self.prev_temp_clock = 0
        self.prev_temp_deriv = 0.
        self.prev_temp_integ = 0.
        self.inv_mcu_freq = 1. / self.heater.printer.mcu.get_mcu_freq()
    def adc_callback(self, read_clock, temp):
        time_diff = (read_clock - self.prev_temp_clock) * self.inv_mcu_freq
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
        co = int(self.Kp*temp_err + self.Ki*temp_integ - self.Kd*temp_deriv)
        #logging.debug("pid: %f@%d -> diff=%f deriv=%f err=%f integ=%f co=%d" % (
        #    temp, read_clock, temp_diff, temp_deriv, temp_err, temp_integ, co))
        bounded_co = max(0, min(PWM_MAX, co))
        self.heater.set_pwm(read_clock, bounded_co)
        # Store state for next measurement
        self.prev_temp = temp
        self.prev_temp_clock = read_clock
        self.prev_temp_deriv = temp_deriv
        if co == bounded_co:
            self.prev_temp_integ = temp_integ
    def check_busy(self, eventtime):
        temp_diff = self.heater.target_temp - self.heater.last_temp
        return abs(temp_diff) > 1. or abs(self.prev_temp_deriv) > 0.1


######################################################################
# Ziegler-Nichols PID autotuning
######################################################################

TUNE_PID_DELTA = 5.0

class ControlAutoTune:
    def __init__(self, heater, old_control, target_temp):
        self.heater = heater
        self.old_control = old_control
        self.target_temp = target_temp
        self.heating = False
        self.peaks = []
        self.peak = 0.
        self.peak_clock = 0
    def adc_callback(self, read_clock, temp):
        if self.heating and temp >= self.target_temp:
            self.heating = False
            self.check_peaks()
        elif not self.heating and temp <= self.target_temp - TUNE_PID_DELTA:
            self.heating = True
            self.check_peaks()
        if self.heating:
            self.heater.set_pwm(read_clock, PWM_MAX)
            if temp < self.peak:
                self.peak = temp
                self.peak_clock = read_clock
        else:
            self.heater.set_pwm(read_clock, 0)
            if temp > self.peak:
                self.peak = temp
                self.peak_clock = read_clock
    def check_peaks(self):
        self.peaks.append((self.peak, self.peak_clock))
        if self.heating:
            self.peak = 9999999.
        else:
            self.peak = -9999999.
        if len(self.peaks) < 4:
            return
        temp_diff = self.peaks[-1][0] - self.peaks[-2][0]
        clock_diff = self.peaks[-1][1] - self.peaks[-3][1]
        pwm_diff = PWM_MAX - 0
        Ku = 4. * (2. * pwm_diff) / (abs(temp_diff) * math.pi)
        Tu = clock_diff / self.heater.printer.mcu.get_mcu_freq()

        Kp = 0.6 * Ku
        Ti = 0.5 * Tu
        Td = 0.125 * Tu
        Ki = Kp / Ti
        Kd = Kp * Td
        logging.info("Autotune: raw=%f/%d/%d Ku=%f Tu=%f  Kp=%f Ki=%f Kd=%f" % (
            temp_diff, clock_diff, pwm_diff, Ku, Tu, Kp, Ki, Kd))
    def check_busy(self, eventtime):
        if self.heating or len(self.peaks) < 12:
            return True
        self.heater.control = self.old_control
        return False


######################################################################
# Tuning information test
######################################################################

class ControlBumpTest:
    def __init__(self, heater, old_control, target_temp):
        self.heater = heater
        self.old_control = old_control
        self.target_temp = target_temp
        self.temp_samples = {}
        self.pwm_samples = {}
        self.state = 0
    def set_pwm(self, read_clock, value):
        self.pwm_samples[read_clock + 2*self.heater.report_clock] = value
        self.heater.set_pwm(read_clock, value)
    def adc_callback(self, read_clock, temp):
        self.temp_samples[read_clock] = temp
        if not self.state:
            self.set_pwm(read_clock, 0)
            if len(self.temp_samples) >= 20:
                self.state += 1
        elif self.state == 1:
            if temp < self.target_temp:
                self.set_pwm(read_clock, PWM_MAX)
                return
            self.set_pwm(read_clock, 0)
            self.state += 1
        elif self.state == 2:
            self.set_pwm(read_clock, 0)
            if temp <= (self.target_temp + AMBIENT_TEMP) / 2.:
                self.dump_stats()
                self.state += 1
    def dump_stats(self):
        out = ["%d %.1f %d" % (clock, temp, self.pwm_samples.get(clock, -1))
               for clock, temp in sorted(self.temp_samples.items())]
        f = open("/tmp/heattest.txt", "wb")
        f.write('\n'.join(out))
        f.close()
    def check_busy(self, eventtime):
        if self.state < 3:
            return True
        self.heater.control = self.old_control
        return False
