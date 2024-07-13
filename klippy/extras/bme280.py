# Support for i2c based temperature sensors
#
# Copyright (C) 2020  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

REPORT_TIME = .8
BME280_CHIP_ADDR = 0x76

BME280_REGS = {
    'RESET': 0xE0, 'CTRL_HUM': 0xF2,
    'STATUS': 0xF3, 'CTRL_MEAS': 0xF4, 'CONFIG': 0xF5,
    'PRESSURE_MSB': 0xF7, 'PRESSURE_LSB': 0xF8, 'PRESSURE_XLSB': 0xF9,
    'TEMP_MSB': 0xFA, 'TEMP_LSB': 0xFB, 'TEMP_XLSB': 0xFC,
    'HUM_MSB': 0xFD, 'HUM_LSB': 0xFE, 'CAL_1': 0x88, 'CAL_2': 0xE1
}

BMP388_REGS = {
    "CMD": 0x7E,
    "STATUS": 0x03,
    "PWR_CTRL": 0x1B,
    "OSR": 0x1C,
    "ORD": 0x1D,
    "INT_CTRL": 0x19,
    "CAL_1": 0x31,
    "TEMP_MSB": 0x09,
    "TEMP_LSB": 0x08,
    "TEMP_XLSB": 0x07,
    "PRESS_MSB": 0x06,
    "PRESS_LSB": 0x05,
    "PRESS_XLSB": 0x04,
}
BMP388_REG_VAL_PRESS_EN = 0x01
BMP388_REG_VAL_TEMP_EN = 0x02
BMP388_REG_VAL_PRESS_OS_NO = 0b000
BMP388_REG_VAL_TEMP_OS_NO = 0b000000
BMP388_REG_VAL_ODR_50_HZ = 0x02
BMP388_REG_VAL_DRDY_EN = 0b100000
BMP388_REG_VAL_NORMAL_MODE = 0x30

BME680_REGS = {
    'RESET': 0xE0, 'CTRL_HUM': 0x72, 'CTRL_GAS_1': 0x71, 'CTRL_GAS_0': 0x70,
    'GAS_WAIT_0': 0x64, 'RES_HEAT_0': 0x5A, 'IDAC_HEAT_0': 0x50,
    'STATUS': 0x73, 'EAS_STATUS_0': 0x1D, 'CTRL_MEAS': 0x74, 'CONFIG': 0x75,
    'GAS_R_LSB': 0x2B, 'GAS_R_MSB': 0x2A,
    'PRESSURE_MSB': 0x1F, 'PRESSURE_LSB': 0x20, 'PRESSURE_XLSB': 0x21,
    'TEMP_MSB': 0x22, 'TEMP_LSB': 0x23, 'TEMP_XLSB': 0x24,
    'HUM_MSB': 0x25, 'HUM_LSB': 0x26, 'CAL_1': 0x88, 'CAL_2': 0xE1,
    'RES_HEAT_VAL': 0x00, 'RES_HEAT_RANGE': 0x02, 'RANGE_SWITCHING_ERROR': 0x04
}

BME680_GAS_CONSTANTS = {
    0: (1., 8000000.),
    1: (1., 4000000.),
    2: (1., 2000000.),
    3: (1., 1000000.),
    4: (1., 499500.4995),
    5: (0.99, 248262.1648),
    6: (1., 125000.),
    7: (0.992, 63004.03226),
    8: (1., 31281.28128),
    9: (1., 15625.),
    10: (0.998, 7812.5),
    11: (0.995, 3906.25),
    12: (1., 1953.125),
    13: (0.99, 976.5625),
    14: (1., 488.28125),
    15: (1., 244.140625)
}

BMP180_REGS = {
    'RESET': 0xE0,
    'CAL_1': 0xAA,
    'CTRL_MEAS': 0xF4,
    'REG_MSB': 0xF6,
    'REG_LSB': 0xF7,
    'CRV_TEMP': 0x2E,
    'CRV_PRES': 0x34
}

STATUS_MEASURING = 1 << 3
STATUS_IM_UPDATE = 1
MODE = 1
RUN_GAS = 1 << 4
NB_CONV_0 = 0
EAS_NEW_DATA = 1 << 7
GAS_DONE = 1 << 6
MEASURE_DONE = 1 << 5
RESET_CHIP_VALUE = 0xB6

BME_CHIPS = {
    0x58: 'BMP280', 0x60: 'BME280', 0x61: 'BME680', 0x55: 'BMP180',
    0x50: 'BMP388'
}
BME_CHIP_ID_REG = 0xD0
BMP3_CHIP_ID_REG = 0x00


def get_twos_complement(val, bit_size):
    if val & (1 << (bit_size - 1)):
        val -= (1 << bit_size)
    return val


def get_unsigned_short(bits):
    return bits[1] << 8 | bits[0]


def get_signed_short(bits):
    val = get_unsigned_short(bits)
    return get_twos_complement(val, 16)


def get_signed_byte(bits):
    return get_twos_complement(bits, 8)


def get_unsigned_short_msb(bits):
    return bits[0] << 8 | bits[1]


def get_signed_short_msb(bits):
    val = get_unsigned_short_msb(bits)
    return get_twos_complement(val, 16)

class BME280:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=BME280_CHIP_ADDR, default_speed=100000)
        self.mcu = self.i2c.get_mcu()
        self.iir_filter = config.getint('bme280_iir_filter', 1)
        self.os_temp = config.getint('bme280_oversample_temp', 2)
        self.os_hum = config.getint('bme280_oversample_hum', 2)
        self.os_pres = config.getint('bme280_oversample_pressure', 2)
        self.gas_heat_temp = config.getint('bme280_gas_target_temp', 320)
        self.gas_heat_duration = config.getint('bme280_gas_heat_duration', 150)
        logging.info("BMxx80: Oversampling: Temp %dx Humid %dx Pressure %dx" % (
            pow(2, self.os_temp - 1), pow(2, self.os_hum - 1),
            pow(2, self.os_pres - 1)))
        logging.info("BMxx80: IIR: %dx" % (pow(2, self.iir_filter) - 1))
        self.iir_filter = self.iir_filter & 0x07

        self.temp = self.pressure = self.humidity = self.gas = self.t_fine = 0.
        self.min_temp = self.max_temp = self.range_switching_error = 0.
        self.max_sample_time = None
        self.dig = self.sample_timer = None
        self.chip_type = 'BMP280'
        self.chip_registers = BME280_REGS
        self.printer.add_object("bme280 " + self.name, self)
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self._init_bmxx80()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return REPORT_TIME

    def _init_bmxx80(self):
        def read_calibration_data_bmp280(calib_data_1):
            dig = {}
            dig['T1'] = get_unsigned_short(calib_data_1[0:2])
            dig['T2'] = get_signed_short(calib_data_1[2:4])
            dig['T3'] = get_signed_short(calib_data_1[4:6])

            dig['P1'] = get_unsigned_short(calib_data_1[6:8])
            dig['P2'] = get_signed_short(calib_data_1[8:10])
            dig['P3'] = get_signed_short(calib_data_1[10:12])
            dig['P4'] = get_signed_short(calib_data_1[12:14])
            dig['P5'] = get_signed_short(calib_data_1[14:16])
            dig['P6'] = get_signed_short(calib_data_1[16:18])
            dig['P7'] = get_signed_short(calib_data_1[18:20])
            dig['P8'] = get_signed_short(calib_data_1[20:22])
            dig['P9'] = get_signed_short(calib_data_1[22:24])
            return dig

        def read_calibration_data_bmp388(calib_data_1):
            dig = {}
            dig["T1"] = get_unsigned_short(calib_data_1[0:2]) / 0.00390625
            dig["T2"] = get_unsigned_short(calib_data_1[2:4]) / 1073741824.0
            dig["T3"] = get_signed_byte(calib_data_1[4]) / 281474976710656.0

            dig["P1"] = get_signed_short(calib_data_1[5:7]) - 16384
            dig["P1"] /= 1048576.0
            dig["P2"] = get_signed_short(calib_data_1[7:9]) - 16384
            dig["P2"] /= 536870912.0
            dig["P3"] = get_signed_byte(calib_data_1[9]) / 4294967296.0
            dig["P4"] = get_signed_byte(calib_data_1[10]) / 137438953472.0
            dig["P5"] = get_unsigned_short(calib_data_1[11:13]) / 0.125
            dig["P6"] = get_unsigned_short(calib_data_1[13:15]) / 64.0
            dig["P7"] = get_signed_byte(calib_data_1[15]) / 256.0
            dig["P8"] = get_signed_byte(calib_data_1[16]) / 32768.0
            dig["P9"] = get_signed_short(calib_data_1[17:19])
            dig["P9"] /= 281474976710656.0
            dig["P10"] = get_signed_byte(calib_data_1[19]) / 281474976710656.0
            dig["P11"] = get_signed_byte(calib_data_1[20])
            dig["P11"] /= 36893488147419103232.0
            return dig

        def read_calibration_data_bme280(calib_data_1, calib_data_2):
            dig = read_calibration_data_bmp280(calib_data_1)
            dig['H1'] = calib_data_1[25] & 0xFF
            dig['H2'] = get_signed_short(calib_data_2[0:2])
            dig['H3'] = calib_data_2[2] & 0xFF
            dig['H4'] = get_twos_complement(
                (calib_data_2[3] << 4) | (calib_data_2[4] & 0x0F), 12)
            dig['H5'] = get_twos_complement(
                (calib_data_2[5] << 4) | ((calib_data_2[4] & 0xF0) >> 4), 12)
            dig['H6'] = get_twos_complement(calib_data_2[6], 8)
            return dig

        def read_calibration_data_bme680(calib_data_1, calib_data_2):
            dig = {}
            dig['T1'] = get_unsigned_short(calib_data_2[8:10])
            dig['T2'] = get_signed_short(calib_data_1[2:4])
            dig['T3'] = get_signed_byte(calib_data_1[4])

            dig['P1'] = get_unsigned_short(calib_data_1[6:8])
            dig['P2'] = get_signed_short(calib_data_1[8:10])
            dig['P3'] = calib_data_1[10]
            dig['P4'] = get_signed_short(calib_data_1[12:14])
            dig['P5'] = get_signed_short(calib_data_1[14:16])
            dig['P6'] = get_signed_byte(calib_data_1[17])
            dig['P7'] = get_signed_byte(calib_data_1[16])
            dig['P8'] = get_signed_short(calib_data_1[20:22])
            dig['P9'] = get_signed_short(calib_data_1[22:24])
            dig['P10'] = calib_data_1[24]

            dig['H1'] = get_twos_complement(
                (calib_data_2[2] << 4) | (calib_data_2[1] & 0x0F), 12)
            dig['H2'] = get_twos_complement(
                (calib_data_2[0] << 4) | ((calib_data_2[1] & 0xF0) >> 4), 12)
            dig['H3'] = get_signed_byte(calib_data_2[3])
            dig['H4'] = get_signed_byte(calib_data_2[4])
            dig['H5'] = get_signed_byte(calib_data_2[5])
            dig['H6'] = calib_data_2[6]
            dig['H7'] = get_signed_byte(calib_data_2[7])

            dig['G1'] = get_signed_byte(calib_data_2[12])
            dig['G2'] = get_signed_short(calib_data_2[10:12])
            dig['G3'] = get_signed_byte(calib_data_2[13])
            return dig

        def read_calibration_data_bmp180(calib_data_1):
            dig = {}
            dig['AC1'] = get_signed_short_msb(calib_data_1[0:2])
            dig['AC2'] = get_signed_short_msb(calib_data_1[2:4])
            dig['AC3'] = get_signed_short_msb(calib_data_1[4:6])
            dig['AC4'] = get_unsigned_short_msb(calib_data_1[6:8])
            dig['AC5'] = get_unsigned_short_msb(calib_data_1[8:10])
            dig['AC6'] = get_unsigned_short_msb(calib_data_1[10:12])

            dig['B1'] = get_signed_short_msb(calib_data_1[12:14])
            dig['B2'] = get_signed_short_msb(calib_data_1[14:16])

            dig['MB'] = get_signed_short_msb(calib_data_1[16:18])
            dig['MC'] = get_signed_short_msb(calib_data_1[18:20])
            dig['MD'] = get_signed_short_msb(calib_data_1[20:22])
            return dig

        chip_id = self.read_id() or self.read_bmp3_id()
        if chip_id not in BME_CHIPS.keys():
            logging.info("bme280: Unknown Chip ID received %#x" % chip_id)
        else:
            self.chip_type = BME_CHIPS[chip_id]
            logging.info("bme280: Found Chip %s at %#x" % (
                self.chip_type, self.i2c.i2c_address))

        # Reset chip
        self.write_register('RESET', [RESET_CHIP_VALUE])
        self.reactor.pause(self.reactor.monotonic() + .5)

        # Make sure non-volatile memory has been copied to registers
        if self.chip_type != 'BMP180':
            # BMP180 has no status register available
            status = self.read_register('STATUS', 1)[0]
            while status & STATUS_IM_UPDATE:
                self.reactor.pause(self.reactor.monotonic() + .01)
                status = self.read_register('STATUS', 1)[0]

        if self.chip_type == 'BME680':
            self.max_sample_time = 0.5
            self.sample_timer = self.reactor.register_timer(self._sample_bme680)
            self.chip_registers = BME680_REGS
        elif self.chip_type == 'BMP180':
            self.max_sample_time = (1.25 + ((2.3 * self.os_pres) + .575)) / 1000
            self.sample_timer = self.reactor.register_timer(self._sample_bmp180)
            self.chip_registers = BMP180_REGS
        elif self.chip_type == 'BMP388':
            self.max_sample_time = 0.5
            self.chip_registers = BMP388_REGS
            self.write_register(
                "PWR_CTRL",
                [
                    BMP388_REG_VAL_PRESS_EN
                    | BMP388_REG_VAL_TEMP_EN
                    | BMP388_REG_VAL_NORMAL_MODE
                ],
            )
            self.write_register(
                "OSR", [BMP388_REG_VAL_PRESS_OS_NO | BMP388_REG_VAL_TEMP_OS_NO]
            )
            self.write_register("ORD", [BMP388_REG_VAL_ODR_50_HZ])
            self.write_register("INT_CTRL", [BMP388_REG_VAL_DRDY_EN])

            self.sample_timer = self.reactor.register_timer(self._sample_bmp388)
        else:
            self.max_sample_time = \
                (1.25 + (2.3 * self.os_temp) + ((2.3 * self.os_pres) + .575)
                 + ((2.3 * self.os_hum) + .575)) / 1000
            self.sample_timer = self.reactor.register_timer(self._sample_bme280)
            self.chip_registers = BME280_REGS

        if self.chip_type in ('BME680', 'BME280'):
            self.write_register('CONFIG', self.iir_filter << 2)

        # Read out and calculate the trimming parameters
        if self.chip_type == 'BMP180':
            cal_1 = self.read_register('CAL_1', 22)
        elif self.chip_type == 'BMP388':
            cal_1 = self.read_register('CAL_1', 21)
        else:
            cal_1 = self.read_register('CAL_1', 26)
            cal_2 = self.read_register('CAL_2', 16)
        if self.chip_type == 'BME280':
            self.dig = read_calibration_data_bme280(cal_1, cal_2)
        elif self.chip_type == 'BMP280':
            self.dig = read_calibration_data_bmp280(cal_1)
        elif self.chip_type == 'BME680':
            self.dig = read_calibration_data_bme680(cal_1, cal_2)
        elif self.chip_type == 'BMP180':
            self.dig = read_calibration_data_bmp180(cal_1)
        elif self.chip_type == 'BMP388':
            self.dig = read_calibration_data_bmp388(cal_1)

    def _sample_bme280(self, eventtime):
        # Enter forced mode
        if self.chip_type == 'BME280':
            self.write_register('CTRL_HUM', self.os_hum)
        meas = self.os_temp << 5 | self.os_pres << 2 | MODE
        self.write_register('CTRL_MEAS', meas)

        try:
            # wait until results are ready
            status = self.read_register('STATUS', 1)[0]
            while status & STATUS_MEASURING:
                self.reactor.pause(
                    self.reactor.monotonic() + self.max_sample_time)
                status = self.read_register('STATUS', 1)[0]

            if self.chip_type == 'BME280':
                data = self.read_register('PRESSURE_MSB', 8)
            elif self.chip_type == 'BMP280':
                data = self.read_register('PRESSURE_MSB', 6)
            else:
                return self.reactor.NEVER
        except Exception:
            logging.exception("BME280: Error reading data")
            self.temp = self.pressure = self.humidity = .0
            return self.reactor.NEVER

        temp_raw = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4)
        self.temp = self._compensate_temp(temp_raw)
        pressure_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4)
        self.pressure = self._compensate_pressure_bme280(pressure_raw) / 100.
        if self.chip_type == 'BME280':
            humid_raw = (data[6] << 8) | data[7]
            self.humidity = self._compensate_humidity_bme280(humid_raw)
        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "BME280 temperature %0.1f outside range of %0.1f:%.01f"
                % (self.temp, self.min_temp, self.max_temp))
        measured_time = self.reactor.monotonic()
        self._callback(self.mcu.estimated_print_time(measured_time), self.temp)
        return measured_time + REPORT_TIME

    def _sample_bmp388(self, eventtime):
        status = self.read_register("STATUS", 1)
        if status[0] & 0b100000:
            self.temp = self._sample_bmp388_temp()
            if self.temp < self.min_temp or self.temp > self.max_temp:
                self.printer.invoke_shutdown(
                    "BME280 temperature %0.1f outside range of %0.1f:%.01f"
                    % (self.temp, self.min_temp, self.max_temp)
                )

        if status[0] & 0b010000:
            self.pressure = self._sample_bmp388_press() / 100.0

        measured_time = self.reactor.monotonic()
        self._callback(self.mcu.estimated_print_time(measured_time), self.temp)
        return measured_time + REPORT_TIME

    def _sample_bmp388_temp(self):
        xlsb = self.read_register("TEMP_XLSB", 1)
        lsb = self.read_register("TEMP_LSB", 1)
        msb = self.read_register("TEMP_MSB", 1)
        adc_T = (msb[0] << 16) + (lsb[0] << 8) + (xlsb[0])

        partial_data1 = adc_T - self.dig["T1"]
        partial_data2 = self.dig["T2"] * partial_data1

        self.t_fine = partial_data2
        self.t_fine += (partial_data1 * partial_data1) * self.dig["T3"]

        if self.t_fine < -40.0:
            self.t_fine = -40.0

        if self.t_fine > 85.0:
            self.t_fine = 85.0

        return self.t_fine

    def _sample_bmp388_press(self):
        xlsb = self.read_register("PRESS_XLSB", 1)
        lsb = self.read_register("PRESS_LSB", 1)
        msb = self.read_register("PRESS_MSB", 1)
        adc_P = (msb[0] << 16) + (lsb[0] << 8) + (xlsb[0])

        partial_data1 = self.dig["P6"] * self.t_fine
        partial_data2 = self.dig["P7"] * (self.t_fine * self.t_fine)
        partial_data3 = self.dig["P8"]
        partial_data3 *= self.t_fine * self.t_fine * self.t_fine
        partial_out1 = self.dig["P5"]
        partial_out1 += partial_data1 + partial_data2 + partial_data3

        partial_data1 = self.dig["P2"] * self.t_fine
        partial_data2 = self.dig["P3"] * (self.t_fine * self.t_fine)
        partial_data3 = self.dig["P4"]
        partial_data3 *= (self.t_fine * self.t_fine * self.t_fine)
        partial_out2 = adc_P * (
            self.dig["P1"] + partial_data1 + partial_data2 + partial_data3
        )

        partial_data1 = adc_P * adc_P
        partial_data2 = self.dig["P9"] + (self.dig["P10"] * self.t_fine)
        partial_data3 = partial_data1 * partial_data2
        partial_data4 = partial_data3 + adc_P * adc_P * adc_P * self.dig["P11"]

        comp_press = partial_out1 + partial_out2 + partial_data4

        if comp_press < 30000:
            comp_press = 30000

        if comp_press > 125000:
            comp_press = 125000

        return comp_press

    def _sample_bme680(self, eventtime):
        self.write_register('CTRL_HUM', self.os_hum & 0x07)
        meas = self.os_temp << 5 | self.os_pres << 2
        self.write_register('CTRL_MEAS', [meas])

        gas_wait_0 = self._calculate_gas_heater_duration(self.gas_heat_duration)
        self.write_register('GAS_WAIT_0', [gas_wait_0])
        res_heat_0 = self._calculate_gas_heater_resistance(self.gas_heat_temp)
        self.write_register('RES_HEAT_0', [res_heat_0])
        gas_config = RUN_GAS | NB_CONV_0
        self.write_register('CTRL_GAS_1', [gas_config])

        def data_ready(stat):
            new_data = (stat & EAS_NEW_DATA)
            gas_done = not (stat & GAS_DONE)
            meas_done = not (stat & MEASURE_DONE)
            return new_data and gas_done and meas_done

        # Enter forced mode
        meas = meas | MODE
        self.write_register('CTRL_MEAS', meas)
        try:
            # wait until results are ready
            status = self.read_register('EAS_STATUS_0', 1)[0]
            while not data_ready(status):
                self.reactor.pause(
                    self.reactor.monotonic() + self.max_sample_time)
                status = self.read_register('EAS_STATUS_0', 1)[0]

            data = self.read_register('PRESSURE_MSB', 8)
            gas_data = self.read_register('GAS_R_MSB', 2)
        except Exception:
            logging.exception("BME680: Error reading data")
            self.temp = self.pressure = self.humidity = self.gas = .0
            return self.reactor.NEVER

        temp_raw = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4)
        if temp_raw != 0x80000:
            self.temp = self._compensate_temp(temp_raw)
        pressure_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4)
        if pressure_raw != 0x80000:
            self.pressure = self._compensate_pressure_bme680(
                pressure_raw) / 100.
        humid_raw = (data[6] << 8) | data[7]
        self.humidity = self._compensate_humidity_bme680(humid_raw)

        gas_valid = ((gas_data[1] & 0x20) == 0x20)
        if gas_valid:
            gas_heater_stable = ((gas_data[1] & 0x10) == 0x10)
            if not gas_heater_stable:
                logging.warning("BME680: Gas heater didn't reach target")
            gas_raw = (gas_data[0] << 2) | ((gas_data[1] & 0xC0) >> 6)
            gas_range = (gas_data[1] & 0x0F)
            self.gas = self._compensate_gas(gas_raw, gas_range)

        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "BME680 temperature %0.1f outside range of %0.1f:%.01f"
                % (self.temp, self.min_temp, self.max_temp))
        measured_time = self.reactor.monotonic()
        self._callback(self.mcu.estimated_print_time(measured_time), self.temp)
        return measured_time + REPORT_TIME

    def _sample_bmp180(self, eventtime):
        meas = self.chip_registers['CRV_TEMP']
        self.write_register('CTRL_MEAS', meas)

        try:
            self.reactor.pause(self.reactor.monotonic() + .01)
            data = self.read_register('REG_MSB', 2)
            temp_raw = (data[0] << 8) | data[1]
        except Exception:
            logging.exception("BMP180: Error reading temperature")
            self.temp = self.pressure = .0
            return self.reactor.NEVER

        meas = self.chip_registers['CRV_PRES'] | (self.os_pres << 6)
        self.write_register('CTRL_MEAS', meas)

        try:
            self.reactor.pause(self.reactor.monotonic() + .01)
            data = self.read_register('REG_MSB', 3)
            pressure_raw = \
                ((data[0] << 16)|(data[1] << 8)|data[2]) >> (8 - self.os_pres)
        except Exception:
            logging.exception("BMP180: Error reading pressure")
            self.temp = self.pressure = .0
            return self.reactor.NEVER

        self.temp = self._compensate_temp_bmp180(temp_raw)
        self.pressure = self._compensate_pressure_bmp180(pressure_raw) / 100.
        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "BMP180 temperature %0.1f outside range of %0.1f:%.01f"
                % (self.temp, self.min_temp, self.max_temp))
        measured_time = self.reactor.monotonic()
        self._callback(self.mcu.estimated_print_time(measured_time), self.temp)
        return measured_time + REPORT_TIME


    def _compensate_temp(self, raw_temp):
        dig = self.dig
        var1 = ((raw_temp / 16384. - (dig['T1'] / 1024.)) * dig['T2'])
        var2 = (
                ((raw_temp / 131072.) - (dig['T1'] / 8192.)) *
                ((raw_temp / 131072.) - (dig['T1'] / 8192.)) * dig['T3'])
        self.t_fine = var1 + var2
        return self.t_fine / 5120.0

    def _compensate_pressure_bme280(self, raw_pressure):
        dig = self.dig
        t_fine = self.t_fine
        var1 = t_fine / 2. - 64000.
        var2 = var1 * var1 * dig['P6'] / 32768.
        var2 = var2 + var1 * dig['P5'] * 2.
        var2 = var2 / 4. + (dig['P4'] * 65536.)
        var1 = (dig['P3'] * var1 * var1 / 524288. + dig['P2'] * var1) / 524288.
        var1 = (1. + var1 / 32768.) * dig['P1']
        if var1 == 0:
            return 0.
        else:
            pressure = 1048576.0 - raw_pressure
            pressure = ((pressure - var2 / 4096.) * 6250.) / var1
            var1 = dig['P9'] * pressure * pressure / 2147483648.
            var2 = pressure * dig['P8'] / 32768.
            return pressure + (var1 + var2 + dig['P7']) / 16.

    def _compensate_pressure_bme680(self, raw_pressure):
        dig = self.dig
        t_fine = self.t_fine
        var1 = t_fine / 2. - 64000.
        var2 = var1 * var1 * dig['P6'] / 131072.
        var2 = var2 + var1 * dig['P5'] * 2.
        var2 = var2 / 4. + (dig['P4'] * 65536.)
        var1 = (dig['P3'] * var1 * var1 / 16384. + dig['P2'] * var1) / 524288.
        var1 = (1. + var1 / 32768.) * dig['P1']
        if var1 == 0:
            return 0.
        else:
            pressure = 1048576.0 - raw_pressure
            pressure = ((pressure - var2 / 4096.) * 6250.) / var1
            var1 = dig['P9'] * pressure * pressure / 2147483648.
            var2 = pressure * dig['P8'] / 32768.
            var3 = (pressure / 256.) * (pressure / 256.) * (pressure / 256.) * (
                    dig['P10'] / 131072.)
            return pressure + (var1 + var2 + var3 + (dig['P7'] * 128.)) / 16.

    def _compensate_humidity_bme280(self, raw_humidity):
        dig = self.dig
        t_fine = self.t_fine
        humidity = t_fine - 76800.
        h1 = (
                raw_humidity - (
                dig['H4'] * 64. + dig['H5'] / 16384. * humidity))
        h2 = (dig['H2'] / 65536. * (1. + dig['H6'] / 67108864. * humidity *
                                    (1. + dig['H3'] / 67108864. * humidity)))
        humidity = h1 * h2
        humidity = humidity * (1. - dig['H1'] * humidity / 524288.)
        return min(100., max(0., humidity))

    def _compensate_humidity_bme680(self, raw_humidity):
        dig = self.dig
        temp_comp = self.temp

        var1 = raw_humidity - (
                (dig['H1'] * 16.) + ((dig['H3'] / 2.) * temp_comp))
        var2 = var1 * ((dig['H2'] / 262144.) *
                       (1. + ((dig['H4'] / 16384.) * temp_comp) +
                        ((dig['H5'] / 1048576.) * temp_comp * temp_comp)))
        var3 = dig['H6'] / 16384.
        var4 = dig['H7'] / 2097152.
        humidity = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2)
        return min(100., max(0., humidity))

    def _compensate_gas(self, gas_raw, gas_range):
        gas_switching_error = self.read_register('RANGE_SWITCHING_ERROR', 1)[0]
        var1 = (1340. + 5. * gas_switching_error) * \
               BME680_GAS_CONSTANTS[gas_range][0]
        gas = var1 * BME680_GAS_CONSTANTS[gas_range][1] / (
                gas_raw - 512. + var1)
        return gas

    def _calculate_gas_heater_resistance(self, target_temp):
        amb_temp = self.temp
        heater_data = self.read_register('RES_HEAT_VAL', 3)
        res_heat_val = get_signed_byte(heater_data[0])
        res_heat_range = (heater_data[2] & 0x30) >> 4
        dig = self.dig
        var1 = (dig['G1'] / 16.) + 49.
        var2 = ((dig['G2'] / 32768.) * 0.0005) + 0.00235
        var3 = dig['G3'] / 1024.
        var4 = var1 * (1. + (var2 * target_temp))
        var5 = var4 + (var3 * amb_temp)
        res_heat = (3.4 * ((var5 * (4. / (4. + res_heat_range))
                            * (1. / (1. + (res_heat_val * 0.002)))) - 25))
        return int(res_heat)

    def _calculate_gas_heater_duration(self, duration_ms):
        if duration_ms >= 4032:
            duration_reg = 0xff
        else:
            factor = 0
            while duration_ms > 0x3F:
                duration_ms //= 4
                factor += 1
            duration_reg = duration_ms + (factor * 64)

        return duration_reg

    def _compensate_temp_bmp180(self, raw_temp):
        dig = self.dig
        x1 = (raw_temp - dig['AC6']) * dig['AC5'] / 32768.
        x2 = dig['MC'] * 2048 / (x1 + dig['MD'])
        b5 = x1 + x2
        self.t_fine = b5
        return (b5 + 8)/16./10.

    def _compensate_pressure_bmp180(self, raw_pressure):
        dig = self.dig
        b5 = self.t_fine
        b6 = b5 - 4000
        x1 = (dig['B2'] * (b6 * b6 / 4096)) / 2048
        x2 = dig['AC2'] * b6 / 2048
        x3 = x1 + x2
        b3 = ((int(dig['AC1'] * 4 + x3) << self.os_pres) + 2) / 4
        x1 = dig['AC3'] * b6 / 8192
        x2 = (dig['B1'] * (b6 * b6 / 4096)) / 65536
        x3 = ((x1 + x2) + 2) / 4
        b4 = dig['AC4'] * (x3 + 32768) / 32768
        b7 = (raw_pressure - b3) * (50000 >> self.os_pres)
        if (b7 < 0x80000000):
            p = (b7 * 2) / b4
        else:
            p = (b7 / b4) * 2
        x1 = (p / 256) * (p / 256)
        x1 = (x1 * 3038) / 65536
        x2 = (-7357 * p) / 65536
        p = p + (x1 + x2 + 3791) / 16.
        return p

    def read_id(self):
        # read chip id register
        regs = [BME_CHIP_ID_REG]
        params = self.i2c.i2c_read(regs, 1)
        return bytearray(params['response'])[0]

    def read_bmp3_id(self):
        # read chip id register
        regs = [BMP3_CHIP_ID_REG]
        params = self.i2c.i2c_read(regs, 1)
        return bytearray(params['response'])[0]

    def read_register(self, reg_name, read_len):
        # read a single register
        regs = [self.chip_registers[reg_name]]
        params = self.i2c.i2c_read(regs, read_len)
        return bytearray(params['response'])

    def write_register(self, reg_name, data):
        if type(data) is not list:
            data = [data]
        reg = self.chip_registers[reg_name]
        data.insert(0, reg)
        self.i2c.i2c_write(data)

    def get_status(self, eventtime):
        data = {
            'temperature': round(self.temp, 2),
            'pressure': self.pressure
        }
        if self.chip_type in ('BME280', 'BME680'):
            data['humidity'] = self.humidity
        if self.chip_type == 'BME680':
            data['gas'] = self.gas
        return data


def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("BME280", BME280)
