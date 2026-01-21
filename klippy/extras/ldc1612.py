# Support for reading frequency samples from ldc1612
#
# Copyright (C) 2020-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bus, bulk_sensor

MIN_MSG_TIME = 0.100

BATCH_UPDATES = 0.100

LDC1612_ADDR = 0x2a

DEFAULT_LDC1612_FREQ = 12000000
SETTLETIME = 0.005
DRIVECUR = 15
DEGLITCH = 0x05 # 10 Mhz

LDC1612_MANUF_ID = 0x5449
LDC1612_DEV_ID = 0x3055

REG_RCOUNT0 = 0x08
REG_OFFSET0 = 0x0c
REG_SETTLECOUNT0 = 0x10
REG_CLOCK_DIVIDERS0 = 0x14
REG_ERROR_CONFIG = 0x19
REG_CONFIG = 0x1a
REG_MUX_CONFIG = 0x1b
REG_DRIVE_CURRENT0 = 0x1e
REG_MANUFACTURER_ID = 0x7e
REG_DEVICE_ID = 0x7f

# Tool for determining appropriate DRIVE_CURRENT register
class DriveCurrentCalibrate:
    def __init__(self, config, sensor):
        self.printer = config.get_printer()
        self.sensor = sensor
        self.drive_cur = config.getint("reg_drive_current", DRIVECUR,
                                       minval=0, maxval=31)
        self.name = config.get_name()
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("LDC_CALIBRATE_DRIVE_CURRENT",
                                   "CHIP", self.name.split()[-1],
                                   self.cmd_LDC_CALIBRATE,
                                   desc=self.cmd_LDC_CALIBRATE_help)
    def get_drive_current(self):
        return self.drive_cur
    cmd_LDC_CALIBRATE_help = "Calibrate LDC1612 DRIVE_CURRENT register"
    def cmd_LDC_CALIBRATE(self, gcmd):
        is_in_progress = True
        def handle_batch(msg):
            return is_in_progress
        self.sensor.add_client(handle_batch)
        toolhead = self.printer.lookup_object("toolhead")
        toolhead.dwell(0.100)
        toolhead.wait_moves()
        old_config = self.sensor.read_reg(REG_CONFIG)
        self.sensor.set_reg(REG_CONFIG, 0x001 | (1<<9))
        toolhead.wait_moves()
        toolhead.dwell(0.100)
        toolhead.wait_moves()
        reg_drive_current0 = self.sensor.read_reg(REG_DRIVE_CURRENT0)
        self.sensor.set_reg(REG_CONFIG, old_config)
        is_in_progress = False
        # Report found value to user
        drive_cur = (reg_drive_current0 >> 6) & 0x1f
        gcmd.respond_info(
            "%s: reg_drive_current: %d\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the above and restart the printer." % (self.name, drive_cur))
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.name, 'reg_drive_current', "%d" % (drive_cur,))

# Interface class to LDC1612 mcu support
class LDC1612:
    def __init__(self, config, calibration=None):
        self.printer = config.get_printer()
        self.calibration = calibration
        self.dccal = DriveCurrentCalibrate(config, self)
        self.data_rate = 250
        # Setup mcu sensor_ldc1612 bulk query code
        self.i2c = bus.MCU_I2C_from_config(config,
                                           default_addr=LDC1612_ADDR,
                                           default_speed=400000)
        self.mcu = mcu = self.i2c.get_mcu()
        self._sensor_errors = {}
        self.oid = oid = mcu.create_oid()
        self.query_ldc1612_cmd = None
        self.clock_freq = config.getint("frequency", DEFAULT_LDC1612_FREQ,
                                        2000000, 40000000)
        # Coil frequency divider, assume 12MHz is BTT Eddy
        # BTT Eddy's coil frequency is > 1/4 of reference clock
        self.sensor_div = 1 if self.clock_freq != DEFAULT_LDC1612_FREQ else 2
        self.freq_conv = float(self.clock_freq * self.sensor_div) / (1<<28)
        if config.get('intb_pin', None) is not None:
            ppins = config.get_printer().lookup_object("pins")
            pin_params = ppins.lookup_pin(config.get('intb_pin'))
            if pin_params['chip'] != mcu:
                raise config.error("ldc1612 intb_pin must be on same mcu")
            mcu.add_config_cmd(
                "config_ldc1612_with_intb oid=%d i2c_oid=%d intb_pin=%s"
                % (oid, self.i2c.get_oid(), pin_params['pin']))
        else:
            mcu.add_config_cmd("config_ldc1612 oid=%d i2c_oid=%d"
                               % (oid, self.i2c.get_oid()))
        mcu.add_config_cmd("query_ldc1612 oid=%d rest_ticks=0"
                           % (oid,), on_restart=True)
        mcu.register_config_callback(self._build_config)
        # Bulk sample message reading
        chip_smooth = self.data_rate * BATCH_UPDATES * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, ">I")
        self.last_error_count = 0
        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch,
            self._start_measurements, self._finish_measurements, BATCH_UPDATES)
        self.name = config.get_name().split()[-1]
        hdr = ('time', 'frequency', 'z')
        self.batch_bulk.add_mux_endpoint("ldc1612/dump_ldc1612", "sensor",
                                         self.name, {'header': hdr})
    def setup_trigger_analog(self, trigger_analog_oid):
        self.mcu.add_config_cmd(
            "ldc1612_attach_trigger_analog oid=%d trigger_analog_oid=%d"
            % (self.oid, trigger_analog_oid), is_init=True)
    def _build_config(self):
        cmdqueue = self.i2c.get_command_queue()
        self.query_ldc1612_cmd = self.mcu.lookup_command(
            "query_ldc1612 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_status_ldc1612 oid=%c",
                                          oid=self.oid, cq=cmdqueue)
        errors = self.mcu.get_enumerations().get("ldc1612_error:", {})
        self._sensor_errors = {v: k for k, v in errors.items()}
    def get_mcu(self):
        return self.i2c.get_mcu()
    def get_samples_per_second(self):
        return self.data_rate
    def read_reg(self, reg):
        if self.mcu.is_fileoutput():
            return 0
        params = self.i2c.i2c_read([reg], 2)
        response = bytearray(params['response'])
        return (response[0] << 8) | response[1]
    def set_reg(self, reg, val, minclock=0):
        self.i2c.i2c_write([reg, (val >> 8) & 0xff, val & 0xff],
                           minclock=minclock)
    def add_client(self, cb):
        self.batch_bulk.add_client(cb)
    def lookup_sensor_error(self, error):
        return self._sensor_errors.get(error, "Unknown ldc1612 error")
    def convert_frequency(self, freq):
        return int(freq / self.freq_conv + 0.5)
    # Measurement decoding
    def _convert_samples(self, samples):
        freq_conv = self.freq_conv
        count = 0
        errors = {}
        def log_once(msg):
            if not errors.get(msg, 0):
                errors[msg] = 0
            errors[msg] += 1
        for ptime, val in samples:
            mv = val & 0x0fffffff
            if val > 0x03ffffff or val == 0x0:
                self.last_error_count += 1
                if (val >> 16 & 0xffff) == 0xffff:
                    # Encoded error from sensor_ldc1612.c
                    log_once(self.lookup_sensor_error(val & 0xffff))
                    continue
                error_bits = (val >> 28) & 0x0f
                if error_bits & 0x8 or mv == 0x0000000:
                    log_once("Frequency under valid range")
                if error_bits & 0x4 or mv > 0x3ffffff:
                    type = "hard" if error_bits & 0x4 else "soft"
                    log_once("Frequency over valid %s range" % (type))
                if error_bits & 0x2:
                    log_once("Conversion Watchdog timeout")
                if error_bits & 0x1:
                    log_once("Amplitude Low/High warning")
            samples[count] = (round(ptime, 6), round(freq_conv * mv, 3), 999.9)
            count += 1
        del samples[count:]
        for msg in errors:
            logging.error("%s: %s (%d)" % (self.name, msg, errors[msg]))
    # Start, stop, and process message batches
    def _start_measurements(self):
        # In case of miswiring, testing LDC1612 device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        manuf_id = self.read_reg(REG_MANUFACTURER_ID)
        dev_id = self.read_reg(REG_DEVICE_ID)
        if ((manuf_id != LDC1612_MANUF_ID or dev_id != LDC1612_DEV_ID)
            and not self.mcu.is_fileoutput()):
            raise self.printer.command_error(
                "Invalid ldc1612 id (got %x,%x vs %x,%x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty ldc1612 chip."
                % (manuf_id, dev_id, LDC1612_MANUF_ID, LDC1612_DEV_ID))
        # Setup chip in requested query rate
        rcount0 = self.clock_freq / (16. * self.data_rate)
        self.set_reg(REG_RCOUNT0, int(rcount0 + 0.5))
        self.set_reg(REG_OFFSET0, 0)
        self.set_reg(REG_SETTLECOUNT0, int(SETTLETIME*self.clock_freq/16. + .5))
        self.set_reg(REG_CLOCK_DIVIDERS0, (self.sensor_div << 12) | 1)
        self.set_reg(REG_ERROR_CONFIG, (0x1f << 11) | 1)
        self.set_reg(REG_MUX_CONFIG, 0x0208 | DEGLITCH)
        self.set_reg(REG_CONFIG, 0x001 | (1<<12) | (1<<10) | (1<<9))
        self.set_reg(REG_DRIVE_CURRENT0, self.dccal.get_drive_current() << 11)
        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(0.5 / self.data_rate)
        self.query_ldc1612_cmd.send([self.oid, rest_ticks])
        logging.info("LDC1612 starting '%s' measurements", self.name)
        # Initialize clock tracking
        self.ffreader.note_start()
        self.last_error_count = 0
    def _finish_measurements(self):
        # Halt bulk reading
        self.query_ldc1612_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("LDC1612 finished '%s' measurements", self.name)
    def _process_batch(self, eventtime):
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        if not samples:
            return {}
        if self.calibration is not None:
            self.calibration.apply_calibration(samples)
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}
