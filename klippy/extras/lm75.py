# Support for I2C based LM75/LM75A temperature sensors
#
# Copyright (C) 2020  Boleslaw Ciesielski <combolek@users.noreply.github.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

LM75_CHIP_ADDR = 0x48
LM75_I2C_SPEED = 100000
LM75_REGS = {
    'TEMP'   : 0x00,
    'CONF'   : 0x01,
    'THYST'  : 0x02,
    'TOS'    : 0x03,
    'PRODID' : 0x07    # TI LM75A chips only?
}
LM75_REPORT_TIME = .8
# Temperature can be sampled at any time but the read aborts
# the current conversion. Conversion time is 300ms so make
# sure not to read too often.
LM75_MIN_REPORT_TIME = .5

class LM75:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config,
            default_addr=LM75_CHIP_ADDR,
            default_speed=LM75_I2C_SPEED
        )
        self.mcu = self.i2c.get_mcu()
        self.report_time = config.getint(
            'lm75_report_time',
            LM75_REPORT_TIME,
            minval=LM75_MIN_REPORT_TIME
        )
        self.temp = 0.0
        self.sample_timer = self.reactor.register_timer(self._sample_lm75)
        self.printer.add_object("lm75 " + self.name, self)
        self.printer.register_event_handler("klippy:ready", self.handle_ready)

    def handle_ready(self):
        self._init_lm75()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        pass

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def degrees_from_sample(self, x):
        # The temp sample is encoded in the top 9 bits of a 16-bit
        # value. Resolution is 0.5 degrees C.
        return x[0] + (x[1] >> 7) * 0.5

    def _init_lm75(self):
        # Check and report the chip ID but ignore errors since many
        # chips don't have it
        try:
            prodid = self.read_register('PRODID', 1)[0]
            logging.info("lm75: Chip ID %#x" % prodid)
        except:
            pass

    def _sample_lm75(self, eventtime):
        try:
            sample = self.read_register('TEMP', 2)
            self.temp = self.degrees_from_sample(sample)
        except Exception:
            logging.exception("lm75: Error reading data")
            self.temp = 0.0
            return self.reactor.NEVER

        measured_time = self.reactor.monotonic()
        self._callback(self.mcu.estimated_print_time(measured_time), self.temp)
        return measured_time + self.report_time

    def read_register(self, reg_name, read_len):
        # read a single register
        regs = [LM75_REGS[reg_name]]
        params = self.i2c.i2c_read(regs, read_len)
        return bytearray(params['response'])

    def write_register(self, reg_name, data):
        if type(data) is not list:
            data = [data]
        reg = LM75_REGS[reg_name]
        data.insert(0, reg)
        self.i2c.i2c_write(data)

    def get_status(self, eventtime):
        return {
            'temperature': self.temp,
        }


def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("LM75", LM75)
