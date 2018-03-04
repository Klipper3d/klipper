# Printer heater support
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, threading
import pins

SAMPLE_TIME = 0.001
SAMPLE_COUNT = 8

######################################################################
# Sensors
######################################################################

KELVIN_TO_CELCIUS = -273.15

class TempBase(object):
    spi          = False
    sample_time  = SAMPLE_TIME
    sample_count = SAMPLE_COUNT
    def __init__(self, is_spi = False,
                 sample_time  = SAMPLE_TIME,
                 sample_count = SAMPLE_COUNT):
        self.spi          = is_spi;
        self.sample_time  = sample_time
        self.sample_count = sample_count

# Thermistor calibrated with three temp measurements
class Thermistor(TempBase):
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
    def check_faults(self, fault):
        pass

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
    def check_faults(self, fault):
        pass

# Linear style conversion chips calibrated with two temp measurements
class Linear(TempBase):
    def __init__(self, config, params):
        adc_voltage = config.getfloat('adc_voltage', 5., above=0.)
        slope = (params['t2'] - params['t1']) / (params['v2'] - params['v1'])
        self.gain = adc_voltage * slope
        self.offset = params['t1'] - params['v1'] * slope
    def calc_temp(self, adc):
        return adc * self.gain + self.offset
    def calc_adc(self, temp):
        return (temp - self.offset) / self.gain
    def check_faults(self, fault):
        pass

################################################################################
MAX31865_CONFIG_REG            = 0x00
MAX31865_RTDMSB_REG            = 0x01
MAX31865_RTDLSB_REG            = 0x02
MAX31865_HFAULTMSB_REG         = 0x03
MAX31865_HFAULTLSB_REG         = 0x04
MAX31865_LFAULTMSB_REG         = 0x05
MAX31865_LFAULTLSB_REG         = 0x06
MAX31865_FAULTSTAT_REG         = 0x07

MAX31865_CONFIG_BIAS           = 0x80
MAX31865_CONFIG_MODEAUTO       = 0x40
MAX31865_CONFIG_1SHOT          = 0x20
MAX31865_CONFIG_3WIRE          = 0x10
MAX31865_CONFIG_FAULTCLEAR     = 0x02
MAX31865_CONFIG_FILT50HZ       = 0x01

MAX31865_FAULT_HIGHTHRESH      = 0x80
MAX31865_FAULT_LOWTHRESH       = 0x40
MAX31865_FAULT_REFINLOW        = 0x20
MAX31865_FAULT_REFINHIGH       = 0x10
MAX31865_FAULT_RTDINLOW        = 0x08
MAX31865_FAULT_OVUV            = 0x04

class RTD(TempBase):
    rtd_nominal_r   = 100;
    reference_r     = 430;
    num_wires       = 2;
    use_50Hz_filter = False;
    val_a = 0.00390830
    val_b = 0.0000005775
    val_c = -0.00000000000418301
    scale = 1
    def __init__(self, config, params):
        super(RTD, self).__init__(is_spi = True, sample_count = 1)
        self.rtd_nominal_r   = config.getint('rtd_nominal_r', 100)
        self.reference_r     = config.getfloat('rtd_reference_r', 430., above=0.)
        self.num_wires       = config.getint('rtd_num_of_wires', 2)
        self.use_50Hz_filter = config.getboolean('rtd_use_50Hz_filter', False)
    def check_faults(self, fault):
        if (fault & 0x80):
            raise error("Max31865 RTD input is disconnected")
        if (fault & 0x40):
            raise error("Max31865 RTD input is shorted")
        if (fault & 0x20):
            raise error("Max31865 VREF- is greater than 0.85 * VBIAS, FORCE- open")
        if (fault & 0x10):
            raise error("Max31865 VREF- is less than 0.85 * VBIAS, FORCE- open")
        if (fault & 0x08):
            raise error("Max31865 VRTD- is less than 0.85 * VBIAS, FORCE- open")
        if (fault & 0x04):
            raise error("Max31865 Overvoltage or undervoltage fault")
        if (fault & 0x03):
            raise error("Max31865 Unspecified error")
    def calc_temp(self, adc):
        adc = adc >> 1 # Scale result
        R_rtd = (self.reference_r * adc) / 32768.0; # 2^15
        temp = ( ( ( -1 * self.rtd_nominal_r ) * self.val_a ) +
                 math.sqrt( ( self.rtd_nominal_r * self.rtd_nominal_r * self.val_a * self.val_a ) -
                            ( 4 * self.rtd_nominal_r * self.val_b * ( self.rtd_nominal_r - R_rtd ) )
                   )
             ) / (2 * self.rtd_nominal_r * self.val_b);
        return (temp);
    def calc_adc(self, temp):
        R_rtd = temp * ( 2 * self.rtd_nominal_r * self.val_b )
        R_rtd = math.pow( ( R_rtd + ( self.rtd_nominal_r * self.val_a ) ), 2)
        R_rtd = -1 * ( R_rtd - ( self.rtd_nominal_r * self.rtd_nominal_r * self.val_a * self.val_a ) )
        R_rtd = R_rtd / ( 4 * self.rtd_nominal_r * self.val_b )
        R_rtd = ( -1 * R_rtd ) + self.rtd_nominal_r
        adc = int ( ( ( R_rtd * 32768.0 ) / self.reference_r) + 0.5 ) # convert to ADC value
        # Scale result
        adc = adc << self.scale
        return adc
    def get_read_cmd(self):
        return MAX31865_RTDMSB_REG
    def get_read_bytes(self):
        return 2 # 16bit value
    def get_configs(self):
        value = (MAX31865_CONFIG_BIAS |
                 MAX31865_CONFIG_MODEAUTO |
                 MAX31865_CONFIG_FAULTCLEAR)
        if (self.use_50Hz_filter):
            value |= MAX31865_CONFIG_FILT50HZ
        if (self.num_wires == 3):
            value |= MAX31865_CONFIG_3WIRE
        cmd = 0x80 + MAX31865_CONFIG_REG
        return [cmd, value]
    def get_fault_filter(self):
        return 0x0001;

################################################################################
MAX31856_CR0_REG           = 0x00
MAX31856_CR0_AUTOCONVERT   = 0x80
MAX31856_CR0_1SHOT         = 0x40
MAX31856_CR0_OCFAULT1      = 0x20
MAX31856_CR0_OCFAULT0      = 0x10
MAX31856_CR0_CJ            = 0x08
MAX31856_CR0_FAULT         = 0x04
MAX31856_CR0_FAULTCLR      = 0x02
MAX31856_CR0_FILT50HZ      = 0x01
MAX31856_CR0_FILT60HZ      = 0x00

MAX31856_CR1_REG           = 0x01
MAX31856_CR1_AVGSEL1       = 0x00
MAX31856_CR1_AVGSEL2       = 0x10
MAX31856_CR1_AVGSEL4       = 0x20
MAX31856_CR1_AVGSEL8       = 0x30
MAX31856_CR1_AVGSEL16      = 0x70

MAX31856_MASK_REG                          = 0x02
MAX31856_MASK_COLD_JUNCTION_HIGH_FAULT     = 0x20
MAX31856_MASK_COLD_JUNCTION_LOW_FAULT      = 0x10
MAX31856_MASK_THERMOCOUPLE_HIGH_FAULT      = 0x08
MAX31856_MASK_THERMOCOUPLE_LOW_FAULT       = 0x04
MAX31856_MASK_VOLTAGE_UNDER_OVER_FAULT     = 0x02
MAX31856_MASK_THERMOCOUPLE_OPEN_FAULT      = 0x01

MAX31856_CJHF_REG          = 0x03
MAX31856_CJLF_REG          = 0x04
MAX31856_LTHFTH_REG        = 0x05
MAX31856_LTHFTL_REG        = 0x06
MAX31856_LTLFTH_REG        = 0x07
MAX31856_LTLFTL_REG        = 0x08
MAX31856_CJTO_REG          = 0x09
MAX31856_CJTH_REG          = 0x0A
MAX31856_CJTL_REG          = 0x0B
MAX31856_LTCBH_REG         = 0x0C
MAX31856_LTCBM_REG         = 0x0D
MAX31856_LTCBL_REG         = 0x0E

MAX31856_SR_REG            = 0x0F
MAX31856_FAULT_CJRANGE     = 0x80  # Cold Junction out of range
MAX31856_FAULT_TCRANGE     = 0x40  # Thermocouple out of range
MAX31856_FAULT_CJHIGH      = 0x20  # Cold Junction High
MAX31856_FAULT_CJLOW       = 0x10  # Cold Junction Low
MAX31856_FAULT_TCHIGH      = 0x08  # Thermocouple Low
MAX31856_FAULT_TCLOW       = 0x04  # Thermocouple Low
MAX31856_FAULT_OVUV        = 0x02  # Under Over Voltage
MAX31856_FAULT_OPEN        = 0x01

class Thermocouple(TempBase):
    tc_type         = 100;
    use_50Hz_filter = False;
    average_count   = 1;
    types = {
        "B" : 0b0000,
        "E" : 0b0001,
        "J" : 0b0010,
        "K" : 0b0011,
        "N" : 0b0100,
        "R" : 0b0101,
        "S" : 0b0110,
        "T" : 0b0111,
    }
    averages = {
        1  : MAX31856_CR1_AVGSEL1,
        2  : MAX31856_CR1_AVGSEL2,
        4  : MAX31856_CR1_AVGSEL4,
        8  : MAX31856_CR1_AVGSEL8,
        16 : MAX31856_CR1_AVGSEL16
    }
    is_k_simple = False # Check faults
    def __init__(self, config, params):
        super(Thermocouple, self).__init__(is_spi = True, sample_count = 1)
        self.tc_type         = types[config.get('tc_type', "K")]
        self.use_50Hz_filter = config.getboolean('tc_use_50Hz_filter', False)
        self.average_count   = averages[config.getint('tc_averaging_count', 1)]
        self.is_k_simple     = params["simple"]

        if (self.is_k_simple): # MAX6675/MAX31855
            self.val_a = 0.25
            self.scale = 18
        else:
            self.val_a = 0.0078125
            self.scale = 5
    def _check_faults_simple(self, val):
        if self.is_k_simple:
            if (val & 0x1):
                raise error("MAX6675/MAX31855 : Open Circuit")
            if (val & 0x2):
                raise error("MAX6675/MAX31855 : Short to GND")
            if (val & 0x4):
                raise error("MAX6675/MAX31855 : Short to Vcc")
        else:
            if (val & 0x1):
                pass
    def check_faults(self, fault):
        if self.is_k_simple == False:
            if (fault & MAX31856_FAULT_CJRANGE):
                raise error("Max31856: Cold Junction Range Fault")
            if (fault & MAX31856_FAULT_TCRANGE):
                raise error("Max31856: Thermocouple Range Fault")
            if (fault & MAX31856_FAULT_CJHIGH):
                raise error("Max31856: Cold Junction High Fault")
            if (fault & MAX31856_FAULT_CJLOW):
                raise error("Max31856: Cold Junction Low Fault")
            if (fault & MAX31856_FAULT_TCHIGH):
                raise error("Max31856: Thermocouple High Fault")
            if (fault & MAX31856_FAULT_TCLOW):
                raise error("Max31856: Thermocouple Low Fault")
            if (fault & MAX31856_FAULT_OVUV):
                raise error("Max31856: Over/Under Voltage Fault")
            if (fault & MAX31856_FAULT_OPEN):
                raise error("Max31856: Thermocouple Open Fault")
    def calc_temp(self, adc):
        if self.is_k_simple:
            self._check_faults_simple(adc)
        adc = adc >> self.scale
        # Fix sign bit:
        if self.is_k_simple:
            if (adc & 0x2000):
                adc = ((adc & 0x1FFF) + 1) * -1
        else:
            if (adc & 0x40000):
                adc = ((adc & 0x3FFFF) + 1) * -1
        temp = self.val_a * adc;
        return (temp);
    def calc_adc(self, temp):
        adc = int ( ( temp / self.val_a ) + 0.5 ) # convert to ADC value
        adc = adc << self.scale
        return adc
    def get_read_cmd(self):
        if self.is_k_simple == False:
            return MAX31856_LTCBH_REG
        return 0x00
    def get_read_bytes(self):
        if self.is_k_simple == False:
            return 3 # 24bit value (MAX31856)
        return 4 # 32bit (MAX6675 / MAX31855)
    def get_configs(self):
        cmds = []
        if self.is_k_simple == False:
            value = MAX31856_CR0_AUTOCONVERT
            if (self.use_50Hz_filter):
                value |= MAX31856_CR0_FILT50HZ
            if (self.num_wires == 3):
                value |= MAX31865_CONFIG_3WIRE
            cmds.append(0x80 + MAX31856_CR0_REG)
            cmds.append(value)
            value  = self.tc_type
            value |= self.average_count
            cmds.append(0x80 + MAX31856_CR1_REG)
            cmds.append(value)
            value = (MAX31856_MASK_VOLTAGE_UNDER_OVER_FAULT |
                     MAX31856_MASK_THERMOCOUPLE_OPEN_FAULT)
            cmds.append(0x80 + MAX31856_MASK_REG)
            cmds.append(value)
        return cmds
    def get_fault_filter(self):
        if self.is_k_simple:
            return 0x4;
        return 0;

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
    "AD595": {
        'class': Linear, 't1': 25., 'v1': .25, 't2': 300., 'v2': 3.022 },
    "MAX6675":  { 'class': Thermocouple, 'simple': True },
    "MAX31855": { 'class': Thermocouple, 'simlpe': True },
    "MAX31856": { 'class': Thermocouple, 'simple': False },
    "MAX31865": { 'class': RTD },
}


######################################################################
# Heater
######################################################################

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
        adc_range = [self.sensor.calc_adc(self.min_temp),
                     self.sensor.calc_adc(self.max_temp)]
        if (self.sensor.spi == True):
            self.mcu_sensor = pins.setup_pin(printer,
                                             'thermocouple',
                                             config.get('sensor_pin'))
            self.mcu_sensor.setup_spi_settings(config.getint('spi_mode', 1,
                                                             minval=0, maxval=3),
                                               config.getint('spi_speed', 2000000))
            self.mcu_sensor.setup_minmax(self.sensor.sample_time,
                                         32767,
                                         minval=min(adc_range),
                                         maxval=max(adc_range))
            self.mcu_sensor.setup_read_command(
                self.sensor.get_read_cmd(),
                self.sensor.get_read_bytes(),
                self.sensor.get_configs(),
                self.sensor.get_fault_filter())
        else:
            self.mcu_sensor = pins.setup_pin(printer,
                                             'adc',
                                             config.get('sensor_pin'))
            self.mcu_sensor.setup_minmax(self.sensor.sample_time,
                                         self.sensor.sample_count,
                                         minval=min(adc_range),
                                         maxval=max(adc_range))
        self.mcu_sensor.setup_adc_callback(REPORT_TIME, self.adc_callback)
        is_fileoutput = self.mcu_sensor.get_mcu().is_fileoutput()
        self.can_extrude = self.min_extrude_temp <= 0. or is_fileoutput
        self.control = algo(self, config)
        # pwm caching
        self.next_pwm_time = 0.
        self.last_pwm_value = 0.
    def set_pwm(self, read_time, value):
        if self.target_temp <= 0.:
            value = 0.
        if ((read_time < self.next_pwm_time or not self.last_pwm_value)
            and abs(value - self.last_pwm_value) < 0.05):
            # No significant change in value - can suppress update
            return
        pwm_time = read_time + REPORT_TIME + self.sensor.sample_time*self.sensor.sample_count
        self.next_pwm_time = pwm_time + 0.75 * MAX_HEAT_TIME
        self.last_pwm_value = value
        logging.debug("%s: pwm=%.3f@%.3f (from %.3f@%.3f [%.3f])",
                      self.name, value, pwm_time,
                      self.last_temp, self.last_temp_time, self.target_temp)
        self.mcu_pwm.set_pwm(pwm_time, value)
    def adc_callback(self, read_time, read_value, fault = 0):
        if (fault):
            self.sensor.check_faults(fault)

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
        print_time = self.mcu_sensor.get_mcu().estimated_print_time(eventtime) - 5.
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
    def stats(self, eventtime):
        with self.lock:
            target_temp = self.target_temp
            last_temp = self.last_temp
            last_pwm_value = self.last_pwm_value
        is_active = target_temp or last_temp > 50.
        return is_active, '%s: target=%.0f temp=%.1f pwm=%.3f' % (
            self.name, target_temp, last_temp, last_pwm_value)


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
