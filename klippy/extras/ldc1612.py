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

LDC1612_FREQ = 12000000
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
        self.oid = oid = mcu.create_oid()
        self.query_ldc1612_cmd = None
        self.ldc1612_setup_home_cmd = self.query_ldc1612_home_state_cmd = None
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
    def _build_config(self):
        cmdqueue = self.i2c.get_command_queue()
        self.query_ldc1612_cmd = self.mcu.lookup_command(
            "query_ldc1612 oid=%c rest_ticks=%u", cq=cmdqueue)
        self.ffreader.setup_query_command("query_status_ldc1612 oid=%c",
                                          oid=self.oid, cq=cmdqueue)
        self.ldc1612_setup_home_cmd = self.mcu.lookup_command(
            "ldc1612_setup_home oid=%c clock=%u threshold=%u"
            " trsync_oid=%c trigger_reason=%c error_reason=%c", cq=cmdqueue)
        self.query_ldc1612_home_state_cmd = self.mcu.lookup_query_command(
            "query_ldc1612_home_state oid=%c",
            "ldc1612_home_state oid=%c homing=%c trigger_clock=%u",
            oid=self.oid, cq=cmdqueue)
    def get_mcu(self):
        return self.i2c.get_mcu()
    def read_reg(self, reg):
        params = self.i2c.i2c_read([reg], 2)
        response = bytearray(params['response'])
        return (response[0] << 8) | response[1]
    def set_reg(self, reg, val, minclock=0):
        self.i2c.i2c_write([reg, (val >> 8) & 0xff, val & 0xff],
                           minclock=minclock)
    def add_client(self, cb):
        self.batch_bulk.add_client(cb)
    # Homing
    def setup_home(self, print_time, trigger_freq,
                   trsync_oid, hit_reason, err_reason):
        clock = self.mcu.print_time_to_clock(print_time)
        tfreq = int(trigger_freq * (1<<28) / float(LDC1612_FREQ) + 0.5)
        self.ldc1612_setup_home_cmd.send(
            [self.oid, clock, tfreq, trsync_oid, hit_reason, err_reason])
    def clear_home(self):
        self.ldc1612_setup_home_cmd.send([self.oid, 0, 0, 0, 0, 0])
        if self.mcu.is_fileoutput():
            return 0.
        params = self.query_ldc1612_home_state_cmd.send([self.oid])
        tclock = self.mcu.clock32_to_clock64(params['trigger_clock'])
        return self.mcu.clock_to_print_time(tclock)
    # Measurement decoding
    def _convert_samples(self, samples):
        freq_conv = float(LDC1612_FREQ) / (1<<28)
        count = 0
        for ptime, val in samples:
            mv = val & 0x0fffffff
            if mv != val:
                self.last_error_count += 1
            samples[count] = (round(ptime, 6), round(freq_conv * mv, 3), 999.9)
            count += 1
    # Start, stop, and process message batches
    def _start_measurements(self):
        # In case of miswiring, testing LDC1612 device ID prevents treating
        # noise or wrong signal as a correctly initialized device
        manuf_id = self.read_reg(REG_MANUFACTURER_ID)
        dev_id = self.read_reg(REG_DEVICE_ID)
        if manuf_id != LDC1612_MANUF_ID or dev_id != LDC1612_DEV_ID:
            raise self.printer.command_error(
                "Invalid ldc1612 id (got %x,%x vs %x,%x).\n"
                "This is generally indicative of connection problems\n"
                "(e.g. faulty wiring) or a faulty ldc1612 chip."
                % (manuf_id, dev_id, LDC1612_MANUF_ID, LDC1612_DEV_ID))
        # Setup chip in requested query rate
        rcount0 = LDC1612_FREQ / (16. * (self.data_rate - 4))
        self.set_reg(REG_RCOUNT0, int(rcount0 + 0.5))
        self.set_reg(REG_OFFSET0, 0)
        self.set_reg(REG_SETTLECOUNT0, int(SETTLETIME*LDC1612_FREQ/16. + .5))
        self.set_reg(REG_CLOCK_DIVIDERS0, (1 << 12) | 1)
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
