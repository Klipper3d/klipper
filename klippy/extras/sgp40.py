# SGP40 I2C-based humiditure sensor support
#
# Copyright (C) 2024 Stefan Dej <meteyou@gmail.com>
# Based on the Nevermore sgp40.py code:
# https://github.com/nevermore3d/Nevermore_Max/blob/master/Software/Klipper/
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# The `VOCAlgorithm` class is based on the following work:
# SPDX-FileCopyrightText:
# Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
# SPDX-License-Identifier: MIT
# Class and algorithm to convert Sensirion sgp40 raw reading to indexed voc
# readings.
# Author(s): yangfeng

import logging
import math
from . import bus
from struct import unpack_from

SGP40_I2C_ADDR = 0x59
SGP40_WORD_LEN = 2

SGP40_CMD = {
    "GET_SERIAL": [0x36, 0x82],
    "SOFT_RESET": [0x00, 0x06],
    "SELF_TEST": [0x28, 0x0E],
    "MEASURE_RAW_NO_COMP": [0x26, 0x0F, 0x80, 0x00, 0xA2, 0x66, 0x66,
                             0x93],
    "MEASURE_RAW": [0x26, 0x0F]
}

# Constants for the VOC algorithm
_VOC_SAMPLING_INTERVAL = 1
_VOC_INITIAL_BLACKOUT = 45
_VOC_INDEX_GAIN = 230
_VOC_SRAW_STD_INITIAL = 50
_VOC_SRAW_STD_BONUS = 220
_VOC_TAU_MEAN_VARIANCE_HOURS = 12
_VOC_TAU_INITIAL_MEAN = 20
_VOC_INITI_DURATION_MEAN = 2700
_VOC_INITI_TRANSITION_MEAN = 0.01
_VOC_TAU_INITIAL_VARIANCE = 2500
_VOC_INITI_DURATION_VARIANCE = 5220
_VOC_INITI_TRANSITION_VARIANCE = 0.01
_VOC_GATING_THRESHOLD = 340
_VOC_GATING_THRESHOLD_INITIAL = 510
_VOC_GATING_THRESHOLD_TRANSITION = 0.09
_VOC_GATING_MAX_DURATION_MINUTES = 180
_VOC_GATING_MAX_RATIO = 0.3
_VOC_SIGMOID_L = 500
_VOC_SIGMOID_K = -0.0065
_VOC_SIGMOID_X0 = 213
_VOC_INDEX_OFFSET_DEFAULT = 100
_VOC_LP_TAU_FAST = 20
_VOC_LP_TAU_SLOW = 500
_VOC_LP_ALPHA = -0.2
_VOC_PERSISTENCE_UPTIME_GAMMA = 10800
_VOC_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING = 64
_VOC_MEAN_VARIANCE_ESTIMATOR__FIX16_MAX = 32767
_FIX16_MAXIMUM = 0x7FFFFFFF
_FIX16_MINIMUM = 0x80000000
_FIX16_OVERFLOW = 0x80000000
_FIX16_ONE = 0x00010000

class SGP40:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=SGP40_I2C_ADDR, default_speed=100000)
        self.report_time = config.getint('sgp40_report_time', 1, minval=1)
        self.temp_sensor = config.get('ref_temp_sensor', None)
        self.humidity_sensor = config.get('ref_humidity_sensor', None)

        self.temp = 25
        self.humidity = 50
        self.sample_timer = self.reactor.register_timer(self._sample_sgp40)

        self.voc_algorithm = VOCAlgorithm()
        self.raw = self.voc = 0.
        self.printer.add_object("sgp40 " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self._init_sgp40()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def _init_sgp40(self):
        logging.info(self._log_message("Initializing sensor"))

        # Perform a soft reset with increased delay
        logging.info(self._log_message("Performing soft reset"))
        # Data sheet specifies a maximum of 0.6ms for the soft reset
        self._write_command(SGP40_CMD["SOFT_RESET"], delay=.001)

        # Get the serial number to verify communication
        serial = self._write_command(SGP40_CMD["GET_SERIAL"], read_len=6,
                                     delay=.001)
        if serial is None:
            logging.error(self._log_message(
                "Failed to read serial number, communication error"))
        else:
            serial_str = ''.join(["{:02x}".format(x) for x in serial])
            logging.info(self._log_message(
                "Serial number: {serial}".format(serial=serial_str)))

        # Perform a self-test with increased delay
        logging.info(self._log_message("Performing self-test"))
        # Data sheet specifies a maximum of 320ms for the self-test
        self_test_result = self._write_command(SGP40_CMD["SELF_TEST"],
                                               read_len=3, delay=.32)
        if self_test_result is None or self_test_result[0] != 0xD400:
            logging.error(self._log_message("Self-test failed"))
        else:
            logging.info(self._log_message("Self-test passed successfully"))

    def _sample_sgp40(self, eventtime):
        if self.temp_sensor is not None:
            sensor = self.printer.lookup_object(self.temp_sensor)
            self.temp = sensor.get_status(eventtime)["temperature"]

        if self.humidity_sensor is not None:
            sensor = self.printer.lookup_object(self.humidity_sensor)
            self.humidity = sensor.get_status(eventtime)["humidity"]
        else:
            # calculate humidity from temperature
            self.humidity = self._calculate_humidity(self.temp)


        try:
            cmd = (SGP40_CMD["MEASURE_RAW"]
                   + self._humidity_to_ticks(self.humidity)
                   + self._temperature_to_ticks(self.temp))
            value = self._write_command(cmd, read_len=1, delay=.03)
            self.raw = value[0]
            self.voc = self.voc_algorithm.process(self.raw)
        except Exception as e:
            logging.error(self._log_message(
                "Error processing sample: {error}".format(error=e)))

        measured_time = self.reactor.monotonic()
        print_time = self.i2c.get_mcu().estimated_print_time(measured_time)
        self._callback(print_time, self.temp)
        return measured_time + self.report_time

    def _write_command(self, command, delay=0, read_len=0):
        self.i2c.i2c_write(command)

        if read_len == 0:
            return None

        # Delay for the command to be processed
        if delay > 0:
            self.reactor.pause(self.reactor.monotonic() + delay)

        reply_len = read_len * SGP40_WORD_LEN
        params = self.i2c.i2c_read([], reply_len)

        if 'response' not in params or len(params['response']) != reply_len:
            logging.error(self._log_message("Invalid response length from "
                                            "sensor"))
            return None

        response = bytearray(params['response'])
        data = [unpack_from(">H", response, i)[0] for i in range(0, reply_len,
                SGP40_WORD_LEN)]

        return data

    def _calculate_humidity(self, temp):
        # Magnus formula for estimating the saturation vapor pressure curve
        a = 17.62
        b = 243.12
        saturation_vapor_pressure = 6.112 * math.exp((a * temp) / (b + temp))
        actual_vapor_pressure = 6.112 * math.exp( (a * 25) / (b + 25))
        relative_humidity = ((actual_vapor_pressure / saturation_vapor_pressure)
                             * 50)
        return max(0, min(100, relative_humidity))

    def _temperature_to_ticks(self, temperature):
        ticks = int(round(((temperature + 45) * 65535) / 175)) & 0xFFFF
        data = [(ticks >> 8) & 0xFF, ticks & 0xFF]
        crc = self._generate_crc(data)

        return data + [crc]

    def _humidity_to_ticks(self, humidity):
        ticks = int(round((humidity * 65535) / 100)) & 0xFFFF
        data = [(ticks >> 8) & 0xFF, ticks & 0xFF]
        crc = self._generate_crc(data)

        return data + [crc]

    def _generate_crc(self, data):
        # From SGP40 data sheet
        crc = 0xFF
        for byte in data:
            crc ^= byte
            for _ in range(8):
                if crc & 0x80:
                    crc = (crc << 1) ^ 0x31
                else:
                    crc = crc << 1
        return crc & 0xFF

    def _log_message(self, message):
        return "SGP40 {name}: {msg}".format(name=self.name, msg=message)

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
            'humidity': round(self.humidity, 1),
            'gas_row': self.raw,
            'gas': self.voc,
        }

class VOCAlgorithm:
    def __init__(self):
        self.mvoc_index_offset = 0
        self.mtau_mean_variance_hours = 0
        self.mgating_max_duration_minutes = 0
        self.msraw_std_initial = 0
        self.muptime = 0
        self.msraw = 0
        self.mvoc_index = 0
        self.m_mean_variance_estimator_gating_max_duration_minutes = 0
        self.m_mean_variance_estimator_initialized = 0
        self.m_mean_variance_estimator_mean = 0
        self.m_mean_variance_estimator_sraw_offset = 0
        self.m_mean_variance_estimator_std = 0
        self.m_mean_variance_estimator_gamma = 0
        self.m_mean_variance_estimator_gamma_initial_mean = 0
        self.m_mean_variance_estimator_gamma_initial_variance = 0
        self.m_mean_variance_estimator_gamma_mean = 0
        self.m_mean_variance_estimator__gamma_variance = 0
        self.m_mean_variance_estimator_uptime_gamma = 0
        self.m_mean_variance_estimator_uptime_gating = 0
        self.m_mean_variance_estimator_gating_duration_minutes = 0
        self.m_mean_variance_estimator_sigmoid_l = 0
        self.m_mean_variance_estimator_sigmoid_k = 0
        self.m_mean_variance_estimator_sigmoid_x0 = 0
        self.m_mox_model_sraw_mean = 0
        self.m_sigmoid_scaled_offset = 0
        self.m_adaptive_lowpass_a1 = 0
        self.m_adaptive_lowpass_a2 = 0
        self.m_adaptive_lowpass_initialized = 0
        self.m_adaptive_lowpass_x1 = 0
        self.m_adaptive_lowpass_x2 = 0
        self.m_adaptive_lowpass_x3 = 0

        self.init()

    def _f16(self, x):
        if x >= 0:
            return int(x * 65536.0 + 0.5)
        else:
            return int(x * 65536.0 - 0.5)

    def _fix16_from_int(self, a):
        return int(a * _FIX16_ONE)

    def _fix16_cast_to_int(self, a):
        return int(a) >> 16

    def _fix16_mul(self, inarg0, inarg1):
        inarg0 = int(inarg0)
        inarg1 = int(inarg1)
        A = inarg0 >> 16
        if inarg0 < 0:
            B = (inarg0 & 0xFFFFFFFF) & 0xFFFF
        else:
            B = inarg0 & 0xFFFF
        C = inarg1 >> 16
        if inarg1 < 0:
            D = (inarg1 & 0xFFFFFFFF) & 0xFFFF
        else:
            D = inarg1 & 0xFFFF
        AC = A * C
        AD_CB = A * D + C * B
        BD = B * D
        product_hi = AC + (AD_CB >> 16)
        ad_cb_temp = ((AD_CB) << 16) & 0xFFFFFFFF
        product_lo = (BD + ad_cb_temp) & 0xFFFFFFFF
        if product_lo < BD:
            product_hi = product_hi + 1
        if (product_hi >> 31) != (product_hi >> 15):
            return _FIX16_OVERFLOW
        product_lo_tmp = product_lo & 0xFFFFFFFF
        product_lo = (product_lo - 0x8000) & 0xFFFFFFFF
        product_lo = (
            product_lo - ((product_hi & 0xFFFFFFFF) >> 31)
        ) & 0xFFFFFFFF
        if product_lo > product_lo_tmp:
            product_hi = product_hi - 1
        result = (product_hi << 16) | (product_lo >> 16)
        result += 1
        return result

    def _fix16_div(self, a, b):
        a = int(a)
        b = int(b)
        if b == 0:
            return _FIX16_MINIMUM
        if a >= 0:
            remainder = a
        else:
            remainder = (a * (-1)) & 0xFFFFFFFF
        if b >= 0:
            divider = b
        else:
            divider = (b * (-1)) & 0xFFFFFFFF
        quotient = 0
        bit = 0x10000
        while divider < remainder:
            divider = divider << 1
            bit <<= 1
        if not bit:
            return _FIX16_OVERFLOW
        if divider & 0x80000000:
            if remainder >= divider:
                quotient |= bit
                remainder -= divider
            divider >>= 1
            bit >>= 1
        while bit and remainder:
            if remainder >= divider:
                quotient |= bit
                remainder -= divider
            remainder <<= 1
            bit >>= 1
        if remainder >= divider:
            quotient += 1
        result = quotient
        if (a ^ b) & 0x80000000:
            if result == _FIX16_MINIMUM:
                return _FIX16_OVERFLOW
            result = -result
        return result

    def _fix16_sqrt(self, x):
        x = int(x)
        num = x & 0xFFFFFFFF
        result = 0
        bit = 1 << 30
        while bit > num:
            bit >>= 2
        for n in range(0, 2):
            while bit:
                if num >= result + bit:
                    num = num - (result + bit) & 0xFFFFFFFF
                    result = (result >> 1) + bit
                else:
                    result = result >> 1
                bit >>= 2
            if n == 0:
                if num > 65535:
                    num = (num - result) & 0xFFFFFFFF
                    num = ((num << 16) - 0x8000) & 0xFFFFFFFF
                    result = ((result << 16) + 0x8000) & 0xFFFFFFFF
                else:
                    num = (num << 16) & 0xFFFFFFFF
                    result = (result << 16) & 0xFFFFFFFF
                bit = 1 << 14
        if num > result:
            result += 1
        return result

    def _fix16_exp(self, x):
        x = int(x)
        exp_pos_values = [
            self._f16(2.7182818),
            self._f16(1.1331485),
            self._f16(1.0157477),
            self._f16(1.0019550),
        ]
        exp_neg_values = [
            self._f16(0.3678794),
            self._f16(0.8824969),
            self._f16(0.9844964),
            self._f16(0.9980488),
        ]
        if x >= self._f16(10.3972):
            return _FIX16_MAXIMUM
        if x <= self._f16(-11.7835):
            return 0
        if x < 0:
            x = -x
            exp_values = exp_neg_values
        else:
            exp_values = exp_pos_values
        res = _FIX16_ONE
        arg = _FIX16_ONE
        for i in range(0, 4):
            while x >= arg:
                res = self._fix16_mul(res, exp_values[i])
                x -= arg
            arg >>= 3
        return res

    def init(self):
        self.mvoc_index_offset = self._f16(_VOC_INDEX_OFFSET_DEFAULT)
        self.mtau_mean_variance_hours = self._f16(_VOC_TAU_MEAN_VARIANCE_HOURS)
        self.mgating_max_duration_minutes = self._f16(
            _VOC_GATING_MAX_DURATION_MINUTES
        )
        self.msraw_std_initial = self._f16(_VOC_SRAW_STD_INITIAL)
        self.muptime = self._f16(0.0)
        self.msraw = self._f16(0.0)
        self.mvoc_index = 0
        self._init_instances()

    def _init_instances(self):
        self._mean_variance_estimator__init()
        self._mean_variance_estimator__set_parameters(
            self._f16(_VOC_SRAW_STD_INITIAL),
            self.mtau_mean_variance_hours,
            self.mgating_max_duration_minutes,
        )
        self._mox_model__init()
        self._mox_model__set_parameters(
            self._mean_variance_estimator__get_std(),
            self._mean_variance_estimator__get_mean(),
        )
        self._sigmoid_scaled__init()
        self._sigmoid_scaled__set_parameters(self.mvoc_index_offset)
        self._adaptive_lowpass__init()
        self._adaptive_lowpass__set_parameters()

    def _mean_variance_estimator__init(self):
        self._mean_variance_estimator__set_parameters(
            self._f16(0.0), self._f16(0.0), self._f16(0.0)
        )
        self._mean_variance_estimator___init_instances()

    def _mean_variance_estimator___init_instances(self):
        self._mean_variance_estimator___sigmoid__init()

    def _mean_variance_estimator__set_parameters(
        self, std_initial, tau_mean_variance_hours, gating_max_duration_minutes
    ):
        self.m_mean_variance_estimator_gating_max_duration_minutes = (
            gating_max_duration_minutes
        )
        self.m_mean_variance_estimator_initialized = 0
        self.m_mean_variance_estimator_mean = self._f16(0.0)
        self.m_mean_variance_estimator_sraw_offset = self._f16(0.0)
        self.m_mean_variance_estimator_std = std_initial
        self.m_mean_variance_estimator_gamma = self._fix16_div(
            self._f16(
                _VOC_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING
                * (_VOC_SAMPLING_INTERVAL / 3600.0)
            ), (
                tau_mean_variance_hours
                + self._f16((_VOC_SAMPLING_INTERVAL / 3600.0))
            ),
        )
        self.m_mean_variance_estimator_gamma_initial_mean = self._f16(
            (
                _VOC_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING
                * _VOC_SAMPLING_INTERVAL
            )
            / (_VOC_TAU_INITIAL_MEAN + _VOC_SAMPLING_INTERVAL)
        )
        self.m_mean_variance_estimator_gamma_initial_variance = self._f16(
            (
                _VOC_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING
                * _VOC_SAMPLING_INTERVAL
            )
            / (_VOC_TAU_INITIAL_VARIANCE + _VOC_SAMPLING_INTERVAL)
        )
        self.m_mean_variance_estimator_gamma_mean = self._f16(0.0)
        self.m_mean_variance_estimator__gamma_variance = self._f16(0.0)
        self.m_mean_variance_estimator_uptime_gamma = self._f16(0.0)
        self.m_mean_variance_estimator_uptime_gating = self._f16(0.0)
        self.m_mean_variance_estimator_gating_duration_minutes = self._f16(0.0)

    def _mean_variance_estimator__set_states(
        self, mean, std, uptime_gamma
    ):
        self.m_mean_variance_estimator_mean = mean
        self.m_mean_variance_estimator_std = std
        self.m_mean_variance_estimator_uptime_gamma = uptime_gamma
        self.m_mean_variance_estimator_initialized = True

    def _mean_variance_estimator__get_std(self):
        return self.m_mean_variance_estimator_std

    def _mean_variance_estimator__get_mean(self):
        return (
            self.m_mean_variance_estimator_mean
            + self.m_mean_variance_estimator_sraw_offset
        )

    def _mean_variance_estimator___calculate_gamma(
        self, voc_index_from_prior
    ):
        uptime_limit = self._f16(
            _VOC_MEAN_VARIANCE_ESTIMATOR__FIX16_MAX - _VOC_SAMPLING_INTERVAL
        )
        if self.m_mean_variance_estimator_uptime_gamma < uptime_limit:
            self.m_mean_variance_estimator_uptime_gamma = (
                self.m_mean_variance_estimator_uptime_gamma
                + self._f16(_VOC_SAMPLING_INTERVAL)
            )

        if self.m_mean_variance_estimator_uptime_gating < uptime_limit:
            self.m_mean_variance_estimator_uptime_gating = (
                self.m_mean_variance_estimator_uptime_gating
                + self._f16(_VOC_SAMPLING_INTERVAL)
            )

        self._mean_variance_estimator___sigmoid__set_parameters(
            self._f16(1.0),
            self._f16(_VOC_INITI_DURATION_MEAN),
            self._f16(_VOC_INITI_TRANSITION_MEAN),
        )
        sigmoid_gamma_mean = self._mean_variance_estimator___sigmoid__process(
            self.m_mean_variance_estimator_uptime_gamma)
        gamma_mean = self.m_mean_variance_estimator_gamma + (
            self._fix16_mul(
                (
                    self.m_mean_variance_estimator_gamma_initial_mean
                    - self.m_mean_variance_estimator_gamma
                ),
                sigmoid_gamma_mean,
            )
        )
        gating_threshold_mean = self._f16(_VOC_GATING_THRESHOLD) + (
            self._fix16_mul(
                self._f16(
                    _VOC_GATING_THRESHOLD_INITIAL - _VOC_GATING_THRESHOLD
                ),
                self._mean_variance_estimator___sigmoid__process(
                    self.m_mean_variance_estimator_uptime_gating
                ),
            )
        )
        self._mean_variance_estimator___sigmoid__set_parameters(
            self._f16(1.0),
            gating_threshold_mean,
            self._f16(_VOC_GATING_THRESHOLD_TRANSITION),
        )

        sigmoid_gating_mean = self._mean_variance_estimator___sigmoid__process(
            voc_index_from_prior)
        self.m_mean_variance_estimator_gamma_mean = self._fix16_mul(
            sigmoid_gating_mean, gamma_mean)

        self._mean_variance_estimator___sigmoid__set_parameters(
            self._f16(1.0),
            self._f16(_VOC_INITI_DURATION_VARIANCE),
            self._f16(_VOC_INITI_TRANSITION_VARIANCE),
        )

        sigmoid_gamma_variance = (
            self._mean_variance_estimator___sigmoid__process(
                self.m_mean_variance_estimator_uptime_gamma
            )
        )

        gamma_variance = self.m_mean_variance_estimator_gamma + (
            self._fix16_mul(
                (
                    self.m_mean_variance_estimator_gamma_initial_variance
                    - self.m_mean_variance_estimator_gamma
                ),
                (sigmoid_gamma_variance - sigmoid_gamma_mean),
            )
        )

        gating_threshold_variance = self._f16(_VOC_GATING_THRESHOLD) + (
            self._fix16_mul(
                self._f16(
                    _VOC_GATING_THRESHOLD_INITIAL - _VOC_GATING_THRESHOLD
                ),
                self._mean_variance_estimator___sigmoid__process(
                    self.m_mean_variance_estimator_uptime_gating
                ),
            )
        )

        self._mean_variance_estimator___sigmoid__set_parameters(
            self._f16(1.0),
            gating_threshold_variance,
            self._f16(_VOC_GATING_THRESHOLD_TRANSITION),
        )

        sigmoid_gating_variance = (
            self._mean_variance_estimator___sigmoid__process(
                voc_index_from_prior
            )
        )

        self.m_mean_variance_estimator__gamma_variance = self._fix16_mul(
            sigmoid_gating_variance, gamma_variance
        )

        self.m_mean_variance_estimator_gating_duration_minutes = (
            self.m_mean_variance_estimator_gating_duration_minutes
            + self._fix16_mul(
                self._f16((_VOC_SAMPLING_INTERVAL / 60.0)),
                self._fix16_mul(
                    self._f16(1.0) - sigmoid_gating_mean,
                    self._f16(1.0 + _VOC_GATING_MAX_RATIO),
                ) - self._f16(_VOC_GATING_MAX_RATIO)
            )
        )

        if self.m_mean_variance_estimator_gating_duration_minutes < self._f16(
            0.0
        ):
            self.m_mean_variance_estimator_gating_duration_minutes = self._f16(
                0.0
            )

        if (
            self.m_mean_variance_estimator_gating_duration_minutes
            > self.m_mean_variance_estimator_gating_max_duration_minutes
        ):
            self.m_mean_variance_estimator_uptime_gating = self._f16(0.0)

    def _mean_variance_estimator__process(
        self, sraw, voc_index_from_prior
    ):
        if self.m_mean_variance_estimator_initialized == 0:
            self.m_mean_variance_estimator_initialized = 1
            self.m_mean_variance_estimator_sraw_offset = sraw
            self.m_mean_variance_estimator_mean = self._f16(0.0)
        else:
            if (self.m_mean_variance_estimator_mean >= self._f16(100.0)) or (
                self.m_mean_variance_estimator_mean <= self._f16(-100.0)
            ):
                self.m_mean_variance_estimator_sraw_offset = (
                    self.m_mean_variance_estimator_sraw_offset
                    + self.m_mean_variance_estimator_mean
                )
                self.m_mean_variance_estimator_mean = self._f16(0.0)

            sraw = sraw - self.m_mean_variance_estimator_sraw_offset
            self._mean_variance_estimator___calculate_gamma(
                voc_index_from_prior
            )
            delta_sgp = self._fix16_div(
                sraw - self.m_mean_variance_estimator_mean,
                self._f16(_VOC_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING),
            )
            if delta_sgp < self._f16(0.0):
                c = self.m_mean_variance_estimator_std - delta_sgp
            else:
                c = self.m_mean_variance_estimator_std + delta_sgp
            additional_scaling = self._f16(1.0)
            if c > self._f16(1440.0):
                additional_scaling = self._f16(4.0)
            self.m_mean_variance_estimator_std = self._fix16_mul(
                self._fix16_sqrt(
                    self._fix16_mul(
                        additional_scaling,
                        self._f16(_VOC_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING)
                        - self.m_mean_variance_estimator__gamma_variance
                    )
                ),
                self._fix16_sqrt(
                    self._fix16_mul(
                        self.m_mean_variance_estimator_std,
                        self._fix16_div(
                            self.m_mean_variance_estimator_std,
                            self._fix16_mul(
                                self._f16(
                                    _VOC_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING
                                ),
                                additional_scaling,
                            )
                        )
                    ) + self._fix16_mul(
                        self._fix16_div(
                            self._fix16_mul(
                                self.m_mean_variance_estimator__gamma_variance,
                                delta_sgp,
                            ),
                            additional_scaling,
                        ),
                        delta_sgp
                    )
                )
            )
            self.m_mean_variance_estimator_mean = (
                self.m_mean_variance_estimator_mean
                + self._fix16_mul(
                    self.m_mean_variance_estimator_gamma_mean, delta_sgp
                )
            )

    def _mean_variance_estimator___sigmoid__init(self):
        self._mean_variance_estimator___sigmoid__set_parameters(
            self._f16(0.0), self._f16(0.0), self._f16(0.0)
        )

    def _mean_variance_estimator___sigmoid__set_parameters(
        self, L, X0, K
    ):
        self.m_mean_variance_estimator_sigmoid_l = L
        self.m_mean_variance_estimator_sigmoid_k = K
        self.m_mean_variance_estimator_sigmoid_x0 = X0

    def _mean_variance_estimator___sigmoid__process(self, sample):
        x = self._fix16_mul(
            self.m_mean_variance_estimator_sigmoid_k,
            sample - self.m_mean_variance_estimator_sigmoid_x0,
        )
        if x < self._f16(-50.0):
            return self.m_mean_variance_estimator_sigmoid_l
        elif x > self._f16(50.0):
            return self._f16(0.0)
        else:
            return self._fix16_div(
                self.m_mean_variance_estimator_sigmoid_l,
                self._f16(1.0) + self._fix16_exp(x),
            )

    def _mox_model__init(self):
        self._mox_model__set_parameters(self._f16(1.0), self._f16(0.0))

    def _mox_model__set_parameters(self, SRAW_STD, SRAW_MEAN):
        self.m_mox_model_sraw_std = SRAW_STD
        self.m_mox_model_sraw_mean = SRAW_MEAN

    def _mox_model__process(self, sraw):
        return self._fix16_mul(
            self._fix16_div(
                sraw - self.m_mox_model_sraw_mean,
                - (self.m_mox_model_sraw_std + self._f16(_VOC_SRAW_STD_BONUS))
            ),
            self._f16(_VOC_INDEX_GAIN),
        )

    def _sigmoid_scaled__init(self):
        self._sigmoid_scaled__set_parameters(self._f16(0.0))

    def _sigmoid_scaled__set_parameters(self, offset):
        self.m_sigmoid_scaled_offset = offset

    def _sigmoid_scaled__process(self, sample):
        x = self._fix16_mul(
            self._f16(_VOC_SIGMOID_K),
            sample - self._f16(_VOC_SIGMOID_X0),
        )
        if x < self._f16(-50.0):
            return self._f16(_VOC_SIGMOID_L)
        elif x > self._f16(50.0):
            return self._f16(0.0)
        else:
            if sample >= self._f16(0.0):
                shift = self._fix16_div(
                    (
                        self._f16(_VOC_SIGMOID_L)
                        - self._fix16_mul(
                            self._f16(5.0), self.m_sigmoid_scaled_offset
                        )
                    ),
                    self._f16(4.0),
                )
                return (
                    self._fix16_div(
                        self._f16(_VOC_SIGMOID_L) + shift,
                        self._f16(1.0) + self._fix16_exp(x),
                    )
                ) - shift
            else:
                return self._fix16_mul(
                    self._fix16_div(
                        self.m_sigmoid_scaled_offset,
                        self._f16(_VOC_INDEX_OFFSET_DEFAULT),
                    ),
                    self._fix16_div(
                        self._f16(_VOC_SIGMOID_L),
                        self._f16(1.0) + self._fix16_exp(x),
                    )
                )

    def _adaptive_lowpass__init(self):
        self._adaptive_lowpass__set_parameters()

    def _adaptive_lowpass__set_parameters(self):
        self.m_adaptive_lowpass_a1 = self._f16(
            _VOC_SAMPLING_INTERVAL
            / (_VOC_LP_TAU_FAST + _VOC_SAMPLING_INTERVAL)
        )
        self.m_adaptive_lowpass_a2 = self._f16(
            _VOC_SAMPLING_INTERVAL
            / (_VOC_LP_TAU_SLOW + _VOC_SAMPLING_INTERVAL)
        )
        self.m_adaptive_lowpass_initialized = 0

    def _adaptive_lowpass__process(self, sample):
        if self.m_adaptive_lowpass_initialized == 0:
            self.m_adaptive_lowpass_x1 = sample
            self.m_adaptive_lowpass_x2 = sample
            self.m_adaptive_lowpass_x3 = sample
            self.m_adaptive_lowpass_initialized = 1
        self.m_adaptive_lowpass_x1 = (
            self._fix16_mul(
                self._f16(1.0) - self.m_adaptive_lowpass_a1,
                self.m_adaptive_lowpass_x1,
            )
        ) + self._fix16_mul(self.m_adaptive_lowpass_a1, sample)

        self.m_adaptive_lowpass_x2 = self._fix16_mul(
            self._f16(1.0) - self.m_adaptive_lowpass_a2,
            self.m_adaptive_lowpass_x2,
        ) + self._fix16_mul(self.m_adaptive_lowpass_a2, sample)

        abs_delta = self.m_adaptive_lowpass_x1 - self.m_adaptive_lowpass_x2

        if abs_delta < self._f16(0.0):
            abs_delta = -abs_delta
        F1 = self._fix16_exp(
            self._fix16_mul(self._f16(_VOC_LP_ALPHA), abs_delta)
        )
        tau_a = self._fix16_mul(
            self._f16(_VOC_LP_TAU_SLOW - _VOC_LP_TAU_FAST), F1
        ) + self._f16(_VOC_LP_TAU_FAST)
        a3 = self._fix16_div(
            self._f16(_VOC_SAMPLING_INTERVAL),
            self._f16(_VOC_SAMPLING_INTERVAL) + tau_a
        )
        self.m_adaptive_lowpass_x3 = (
            self._fix16_mul(self._f16(1.0) - a3, self.m_adaptive_lowpass_x3)
        ) + self._fix16_mul(a3, sample)
        return self.m_adaptive_lowpass_x3

    def process(self, sraw):
        if self.muptime <= self._f16(_VOC_INITIAL_BLACKOUT):
            self.muptime = self.muptime + self._f16(_VOC_SAMPLING_INTERVAL)
        else:
            if (sraw > 0) and (sraw < 65000):
                if sraw < 20001:
                    sraw = 20001
                elif sraw > 52767:
                    sraw = 52767
                self.msraw = self._fix16_from_int((sraw - 20000))
            self.mvoc_index = self._mox_model__process(self.msraw)
            self.mvoc_index = self._sigmoid_scaled__process(self.mvoc_index)
            self.mvoc_index = self._adaptive_lowpass__process(self.mvoc_index)
            if self.mvoc_index < self._f16(0.5):
                self.mvoc_index = self._f16(0.5)
            if self.msraw > self._f16(0.0):
                self._mean_variance_estimator__process(
                    self.msraw, self.mvoc_index
                )
                self._mox_model__set_parameters(
                    self._mean_variance_estimator__get_std(),
                    self._mean_variance_estimator__get_mean(),
                )
        voc_index = self._fix16_cast_to_int((self.mvoc_index + self._f16(0.5)))
        return voc_index

def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("SGP40", SGP40)
