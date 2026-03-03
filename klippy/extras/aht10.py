# Support for AHTxx family I2C temperature and humidity sensors
#
# Copyright (C) 2023 Scott Mudge <mail@scottmudge.com>
# Copyright (C) 2025 Lev Voronov <minicx@disroot.org>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus

######################################################################
# Compatible Sensors:
#       AHT10      -    Tested w/ BTT GTR 1.0 MCU on i2c3
#       AHT20      -    Tested w/ N32G455 on i2c2
#       AHT21      -    Tested w/ BTT GTR 1.0 MCU on i2c3
#       AHT30      -    Untested, but should work
######################################################################

I2C_ADDR = 0x38

CMD_MEASURE = [0xAC, 0x33, 0x00]
CMD_RESET = [0xBA]
CMD_INIT_AHT1X = [0xE1, 0x08, 0x00]
CMD_INIT_AHT2X = [0xBE, 0x08, 0x00]

# Status bits
STATUS_BUSY = 0x80
STATUS_CALIBRATED = 0x08

MAX_BUSY_CYCLES = 5

class AHTBase:
    model = None

    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c = bus.MCU_I2C_from_config(
            config, default_addr=I2C_ADDR, default_speed=100000)
        self.report_time = config.getint('aht10_report_time', 30, minval=5)
        self.temp = self.min_temp = self.max_temp = self.humidity = 0.
        self.sample_timer = self.reactor.register_timer(self._sample_aht)

        self.printer.add_object("aht10 " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                                self.handle_connect)
        self.is_calibrated = False
        self.init_sent = False
        self._callback = None

    def handle_connect(self):
        self._init_sensor()
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def _send_init(self):
        raise NotImplementedError("Subclass must implement _send_init")

    def _init_sensor(self):
        self._send_init()

        self.init_sent = True

        if self._make_measurement():
            if not self.is_calibrated:
                logging.warning("%s %s: not calibrated, possible OTP fault"
                                % (self.model, self.name))
            logging.info("%s %s: successfully initialized, "
                         "initial temp: %.3f, humidity: %.3f"
                         % (self.model, self.name, self.temp, self.humidity))
    def _soft_reset(self):
        logging.info("%s %s: performing soft reset" % (self.model, self.name))
        self.i2c.i2c_write(CMD_RESET)
        self.reactor.pause(self.reactor.monotonic() + 0.020)

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
                if is_busy and cycles > MAX_BUSY_CYCLES:
                    logging.warning("%s %s: device reported busy after "
                                    "%d cycles, resetting device"
                                    % (self.model, self.name, MAX_BUSY_CYCLES))
                    self._soft_reset()
                    data = None
                    break

                cycles += 1
                # Write command for updating temperature+status bit
                self.i2c.i2c_write(CMD_MEASURE)
                # Wait 110ms after first read, 75ms minimum
                self.reactor.pause(self.reactor.monotonic() + .110)

                # Read 6 bytes of data
                read = self.i2c.i2c_read([], 6)
                if read is None:
                    logging.warning("%s %s: received data from i2c_read is None"
                                    % (self.model, self.name))
                    continue

                data = bytearray(read['response'])
                if len(data) < 6:
                    logging.warning("%s %s: received bytes less than expected:"
                                    " got %d, need 6"
                                    % (self.model, self.name, len(data)))
                    continue

                self.is_calibrated = bool(data[0] & STATUS_CALIBRATED)
                is_busy = bool(data[0] & STATUS_BUSY)

            if is_busy:
                return False
        except Exception as e:
            logging.exception("%s %s: exception encountered reading data: %s"
                              % (self.model, self.name, str(e)))
            return False

        # Parse temperature: 20 bits starting at data[3] (low nibble)
        temp_raw = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]
        self.temp = ((temp_raw * 200.0) / 1048576.0) - 50.0

        # Parse humidity: 20 bits starting at data[1]
        hum_raw = ((data[1] << 16) | (data[2] << 8) | data[3]) >> 4
        self.humidity = int(hum_raw * 100 / 1048576)

        # Clamp humidity
        if (self.humidity > 100):
            self.humidity = 100
        elif (self.humidity < 0):
            self.humidity = 0

        return True

    def _sample_aht(self, eventtime):
        if not self._make_measurement():
            self.temp = self.humidity = .0
            return self.reactor.NEVER

        if self.temp < self.min_temp or self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "%s temperature %.1f outside range of %.1f:%.1f" %
                (self.model.upper(), self.temp, self.min_temp, self.max_temp))

        measured_time = self.reactor.monotonic()
        print_time = self.i2c.get_mcu().estimated_print_time(measured_time)
        self._callback(print_time, self.temp)
        return measured_time + self.report_time

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def get_status(self, eventtime):
        return {
            'temperature': round(self.temp, 2),
            'humidity': self.humidity,
        }

class AHT1x(AHTBase):
    model = "aht1x"

    def _send_init(self):
        self.i2c.i2c_write(CMD_INIT_AHT1X)
        self.reactor.pause(self.reactor.monotonic() + 0.040)

class AHT2x(AHTBase):
    model = "aht2x"

    def _send_init(self):
        self.i2c.i2c_write(CMD_INIT_AHT2X)
        self.reactor.pause(self.reactor.monotonic() + 0.100)

class AHT3x(AHTBase):
    model = "aht3x"

    def _send_init(self):
        # Wait for auto-calibration at power-on
        self.reactor.pause(self.reactor.monotonic() + 0.100)

def load_config(config):
    # Register sensor
    pheater = config.get_printer().lookup_object("heaters")

    # Backwards compatibility alias
    pheater.add_sensor_factory("AHT10", AHT1x)

    pheater.add_sensor_factory("AHT1X", AHT1x)
    pheater.add_sensor_factory("AHT2X", AHT2x)
    pheater.add_sensor_factory("AHT3X", AHT3x)
