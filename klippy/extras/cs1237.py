# Chipsea CS1237 support
#
# Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bulk_sensor

UPDATE_INTERVAL = 0.10
SAMPLE_ERROR_DESYNC = -0x80000000
SAMPLE_ERROR_LONG_READ = 0x40000000
SAMPLE_ERROR_CONFIG = 0x20000000


class CS1237:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.last_error_count = 0
        self.consecutive_fails = 0

        rate_options = {'10': 0, '40': 1, '640': 2, '1280': 3}
        gain_options = {'1': 0, '2': 1, '64': 2, '128': 3}
        self.sps = config.getchoice('sample_rate',
                                    {key: int(key) for key in rate_options},
                                    default='640')
        rate = config.getchoice('sample_rate', rate_options, default='640')
        gain = config.getchoice('gain', gain_options, default='128')
        self.reference_output = config.getboolean('reference_output', True)
        ref_off = 0 if self.reference_output else 1
        self.chip_config = (ref_off << 6) | (rate << 4) | (gain << 2)

        dout_pin_name = config.get('dout_pin')
        sclk_pin_name = config.get('sclk_pin')
        ppins = printer.lookup_object('pins')
        dout_ppin = ppins.lookup_pin(dout_pin_name)
        sclk_ppin = ppins.lookup_pin(sclk_pin_name)
        self.mcu = mcu = dout_ppin['chip']
        if sclk_ppin['chip'] is not mcu:
            raise config.error("CS1237 config error: All pins must be "
                               "connected to the same MCU")
        self.dout_pin = dout_ppin['pin']
        self.sclk_pin = sclk_ppin['pin']
        self.oid = mcu.create_oid()

        chip_smooth = self.sps * UPDATE_INTERVAL * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, "<i")
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            printer, self._process_batch, self._start_measurements,
            self._finish_measurements, UPDATE_INTERVAL)
        self.query_cmd = None
        mcu.add_config_cmd(
            "config_cs1237 oid=%d config=%d dout_pin=%s sclk_pin=%s"
            % (self.oid, self.chip_config, self.dout_pin, self.sclk_pin))
        mcu.add_config_cmd("query_cs1237 oid=%d rest_ticks=0" % self.oid,
                           on_restart=True)
        mcu.register_config_callback(self._build_config)

    def _build_config(self):
        cmd_queue = self.mcu.alloc_command_queue()
        self.query_cmd = self.mcu.lookup_command(
            "query_cs1237 oid=%c rest_ticks=%u", cq=cmd_queue)
        self.ffreader.setup_query_command("query_cs1237_status oid=%c",
                                          oid=self.oid, cq=cmd_queue)

    def setup_trigger_analog(self, trigger_analog_oid):
        self.mcu.add_config_cmd(
            "cs1237_attach_trigger_analog oid=%d trigger_analog_oid=%d"
            % (self.oid, trigger_analog_oid), is_init=True)

    def get_mcu(self):
        return self.mcu

    def get_samples_per_second(self):
        return self.sps

    def get_range(self):
        return -0x800000, 0x7fffff

    def get_status(self, eventtime):
        return {'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows(),
                'sample_rate': self.sps}

    def lookup_sensor_error(self, error_code):
        errors = {SAMPLE_ERROR_DESYNC: "CS1237 serial data desynchronised",
                  SAMPLE_ERROR_LONG_READ: "CS1237 read took too long",
                  SAMPLE_ERROR_CONFIG: "CS1237 configuration was not accepted"}
        return errors.get(error_code, "Unknown CS1237 error %d" % error_code)

    def add_client(self, callback):
        self.batch_bulk.add_client(callback)

    def _convert_samples(self, samples):
        adc_factor = 1. / (1 << 23)
        count = 0
        errors = (SAMPLE_ERROR_DESYNC, SAMPLE_ERROR_LONG_READ,
                  SAMPLE_ERROR_CONFIG)
        for ptime, value in samples:
            if value in errors:
                self.last_error_count += 1
                break
            samples[count] = (round(ptime, 6), value,
                              round(value * adc_factor, 9))
            count += 1
        del samples[count:]

    def _start_measurements(self):
        self.consecutive_fails = 0
        rest_ticks = self.mcu.seconds_to_clock(1. / (10. * self.sps))
        self.query_cmd.send([self.oid, rest_ticks])
        logging.info("CS1237 starting '%s' measurements at %d SPS",
                     self.name, self.sps)
        self.ffreader.note_start()

    def _finish_measurements(self):
        if self.printer.is_shutdown():
            return
        self.query_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("CS1237 finished '%s' measurements", self.name)

    def _process_batch(self, eventtime):
        prev_overflows = self.ffreader.get_last_overflows()
        prev_errors = self.last_error_count
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        # Keep the error visible to load_cell even when restarting the reader.
        # FixedFreqReader resets overflow accounting on a new capture.
        total_overflows = self.ffreader.get_last_overflows()
        overflows = self.ffreader.get_last_overflows() - prev_overflows
        errors = self.last_error_count - prev_errors
        if errors:
            logging.error("%s: restarting CS1237 after sensor error", self.name)
            self._finish_measurements()
            self._start_measurements()
        elif overflows:
            self.consecutive_fails += 1
            if self.consecutive_fails > 4:
                self._finish_measurements()
                self._start_measurements()
        else:
            self.consecutive_fails = 0
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': total_overflows}


CS1237_SENSOR_TYPE = {'cs1237': CS1237}
