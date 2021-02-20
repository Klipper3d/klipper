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
    'CHIP_ID': 0xD0, 'RESET': 0xE0, 'CTRL_HUM': 0xF2,
    'STATUS': 0xF3, 'CTRL_MEAS': 0xF4, 'CONFIG': 0xF5,
    'PRESSURE_MSB': 0xF7, 'PRESSURE_LSB': 0xF8, 'PRESSURE_XLSB': 0xF9,
    'TEMP_MSB': 0xFA, 'TEMP_LSB': 0xFB, 'TEMP_XLSB': 0xFC,
    'HUM_MSB': 0xFD, 'HUM_LSB': 0xFE, 'CAL_1': 0x88, 'CAL_2': 0xE1
}
# BME default settings
STATUS_MEASURING = 1 << 3
STATUS_IM_UPDATE = 1
MODE = 1

class BME280:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=BME280_CHIP_ADDR, default_speed=100000)
        self.mcu = self.i2c.get_mcu()
        self.os_temp = config.getint('bme280_oversample_temp', 2)
        self.os_hum = config.getint('bme280_oversample_hum', 2)
        self.os_pres = config.getint('bme280_oversample_pressure', 2)
        self.temp = self.pressure = self.humidity = self.t_fine = 0.
        self.min_temp = self.max_temp = 0.
        self.max_sample_time = \
            (1.25 + (2.3 * self.os_temp) + ((2.3 * self.os_pres) +
             .575) + ((2.3 * self.os_hum) + .575)) / 1000
        self.dig = None
        self.sample_timer = self.reactor.register_timer(self._sample_bme280)
        self.printer.add_object("bme280 " + self.name, self)
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self._init_bme280()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return REPORT_TIME

    def _init_bme280(self):
        def get_twos_complement(val, bit_size):
            if val & (1 << (bit_size - 1)):
                val -= (1 << bit_size)
            return val

        def get_unsigned_short(bits):
            return bits[1] << 8 | bits[0]

        def get_signed_short(bits):
            val = get_unsigned_short(bits)
            return get_twos_complement(val, 16)

        # Check the chip ID, should be 0x60
        chip_id = self.read_register('CHIP_ID', 1)[0]
        if chip_id != 0x60:
            logging.info(
                "bme280: Chip ID mismatch, expected 0x60, received %#x"
                % (chip_id))

        # Make sure non-volatile memory has been copied to registers
        status = self.read_register('STATUS', 1)[0]
        while status & STATUS_IM_UPDATE:
            self.reactor.pause(self.reactor.monotonic() + .01)
            status = self.read_register('STATUS', 1)[0]

        c1 = self.read_register('CAL_1', 26)
        c2 = self.read_register('CAL_2', 7)

        # Read out and calculate the trimming parameters
        dig = {}
        unsigned_keys = ['T1', 'P1']
        idx = 0
        for cnt, prefix in [(3, 'T'), (9, 'P')]:
            for i in range(cnt):
                key = prefix + str(i + 1)
                if key in unsigned_keys:
                    dig[key] = get_unsigned_short(c1[idx:idx+2])
                else:
                    dig[key] = get_signed_short(c1[idx:idx+2])
                idx += 2
        dig['H1'] = c1[25] & 0xFF
        dig['H2'] = get_signed_short(c2[0:2])
        dig['H3'] = c2[2] & 0xFF
        dig['H4'] = get_twos_complement(
            ((c2[3] << 4) & 0xFF0) | (c2[4] & 0x0F), 12)
        dig['H5'] = get_twos_complement(
            (c2[4] & 0x0F) | ((c2[5] << 4) & 0xFF0), 12)
        dig['H6'] = get_twos_complement(c2[6], 8)

        self.dig = dig

    def _sample_bme280(self, eventtime):
        # Enter forced mode
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

            data = self.read_register('PRESSURE_MSB', 8)
        except Exception:
            logging.exception("BME280: Error reading data")
            self.temp = self.pressure = self.humidity = .0
            return self.reactor.NEVER

        pressure_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4)
        temp_raw = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4)
        humid_raw = (data[6] << 8) | data[7]

        self.temp = self._compensate_temp(temp_raw)
        self.pressure = self._compensate_pressure(pressure_raw) / 100.
        self.humidity = self._compensate_humidity(humid_raw)
        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "BME280 temperature %0.1f outside range of %0.1f:%.01f"
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

    def _compensate_pressure(self, raw_pressure):
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

    def _compensate_humidity(self, raw_humidity):
        dig = self.dig
        t_fine = self.t_fine
        humidity = t_fine - 76800.
        h1 = (
            raw_humidity - (dig['H4'] * 64. + dig['H5'] / 16384. * humidity))
        h2 = (dig['H2'] / 65536. * (1. + dig['H6'] / 67108864. * humidity *
              (1. + dig['H3'] / 67108864. * humidity)))
        humidity = h1 * h2
        humidity = humidity * (1. - dig['H1'] * humidity / 524288.)
        return min(100., max(0., humidity))

    def read_register(self, reg_name, read_len):
        # read a single register
        regs = [BME280_REGS[reg_name]]
        params = self.i2c.i2c_read(regs, read_len)
        return bytearray(params['response'])

    def write_register(self, reg_name, data):
        if type(data) is not list:
            data = [data]
        reg = BME280_REGS[reg_name]
        data.insert(0, reg)
        self.i2c.i2c_write(data)

    def get_status(self, eventtime):
        return {
            'temperature': self.temp,
            'humidity': self.humidity,
            'pressure': self.pressure
        }


def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("BME280", BME280)
