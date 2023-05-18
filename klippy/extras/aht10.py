# AHT10/AHT20/AHT21 I2c-based humiditure sensor support
#
# Copyright (C) 2023 Scott Mudge <mail@scottmudge.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

######################################################################
# Compatible Sensors:
#       AHT10      -    Tested w/ BTT GTR 1.0 MCU on i2c3
#       AHT20      -    Untested but should work
#       AHT21      -    Tested w/ BTT GTR 1.0 MCU on i2c3
######################################################################

AHT10_I2C_ADDR= 0x38

AHT10_COMMANDS = {
    'INIT'              :[0xE1, 0x08, 0x00],
    'MEASURE'           :[0xAC, 0x33, 0x00],
    'RESET'             :[0xBA, 0x08, 0x00]
}

AHT10_MAX_BUSY_CYCLES= 5

class AHT10:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=AHT10_I2C_ADDR, default_speed=100000)
        self.report_time = config.getint('aht10_report_time',30,minval=5)
        self.temp = self.min_temp = self.max_temp = self.humidity = 0.
        self.sample_timer = self.reactor.register_timer(self._sample_aht10)
        self.printer.add_object("aht10 " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.is_calibrated  = False
        self.init_sent = False

    def handle_connect(self):
        self._init_aht10()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def _make_measurement(self):
        if not self.init_sent:
            return False

        data = None

        is_busy = True
        cycles = 0

        try:
            while is_busy:
                # Check if we're constantly busy. If so, send soft-reset
                # and issue warning.
                if is_busy and cycles > AHT10_MAX_BUSY_CYCLES:
                    logging.warning("aht10: device reported busy after " +
                        "%d cycles, resetting device"% AHT10_MAX_BUSY_CYCLES)
                    self._reset_device()
                    data = None
                    break

                cycles += 1
                # Write command for updating temperature+status bit
                self.i2c.i2c_write(AHT10_COMMANDS['MEASURE'])
                # Wait 110ms after first read, 75ms minimum
                self.reactor.pause(self.reactor.monotonic() + .110)

                # Read data
                read = self.i2c.i2c_read([], 6)
                if read is None:
                    logging.warning("aht10: received data from" +
                                    " i2c_read is None")
                    continue
                data = bytearray(read['response'])
                if len(data) < 6:
                    logging.warning("aht10: received bytes less than" +
                                    " expected 6 [%d]"%len(data))
                    continue

                self.is_calibrated = True if (data[0] & 0b00000100) else False
                is_busy = True if (data[0] & 0b01000000) else False

            if is_busy:
                return False
        except Exception as e:
            logging.exception("aht10: exception encountered" +
                              " reading data: %s"%str(e))
            return False

        temp = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]
        self.temp = ((temp*200) / 1048576) - 50
        hum = ((data[1] << 16) | (data[2] << 8) | data[3]) >> 4
        self.humidity = int(hum * 100 / 1048576)

        # Clamp humidity
        if (self.humidity > 100):
            self.humidity = 100
        elif (self.humidity < 0):
            self.humidity = 0

        return True

    def _reset_device(self):
        if not self.init_sent:
            return

        # Reset device
        self.i2c.i2c_write(AHT10_COMMANDS['RESET'])
        # Wait 100ms after reset
        self.reactor.pause(self.reactor.monotonic() + .10)

    def _init_aht10(self):
        # Init device
        self.i2c.i2c_write(AHT10_COMMANDS['INIT'])
        # Wait 100ms after init
        self.reactor.pause(self.reactor.monotonic() + .10)
        self.init_sent = True

        if self._make_measurement():
            logging.info("aht10: successfully initialized, initial temp: " +
                         "%.3f, humidity: %.3f"%(self.temp, self.humidity))

    def _sample_aht10(self, eventtime):
        if not self._make_measurement():
            self.temp = self.humidity = .0
            return self.reactor.NEVER

        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "AHT10 temperature %0.1f outside range of %0.1f:%.01f"
                % (self.temp, self.min_temp, self.max_temp))

        measured_time = self.reactor.monotonic()
        print_time = self.i2c.get_mcu().estimated_print_time(measured_time)
        self._callback(print_time, self.temp)
        return measured_time + self.report_time

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
            'humidity': self.humidity,
        }


def load_config(config):
    # Register sensor
    pheater = config.get_printer().lookup_object("heaters")
    pheater.add_sensor_factory("AHT10", AHT10)
