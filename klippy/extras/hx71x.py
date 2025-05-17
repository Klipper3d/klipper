# HX711/HX717 Support
#
# Copyright (C) 2024 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import bulk_sensor

#
# Constants
#
UPDATE_INTERVAL = 0.10
SAMPLE_ERROR_DESYNC = -0x80000000
SAMPLE_ERROR_LONG_READ = 0x40000000

# Implementation of HX711 and HX717
class HX71xBase:
    def __init__(self, config, sensor_type,
                 sample_rate_options, default_sample_rate,
                 gain_options, default_gain):
        self.printer = printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.last_error_count = 0
        self.consecutive_fails = 0
        self.sensor_type = sensor_type
        # Chip options
        dout_pin_name = config.get('dout_pin')
        sclk_pin_name = config.get('sclk_pin')
        ppins = printer.lookup_object('pins')
        dout_ppin = ppins.lookup_pin(dout_pin_name)
        sclk_ppin = ppins.lookup_pin(sclk_pin_name)
        self.mcu = mcu = dout_ppin['chip']
        self.oid = mcu.create_oid()
        if sclk_ppin['chip'] is not mcu:
            raise config.error("%s config error: All pins must be "
                               "connected to the same MCU" % (self.name,))
        self.dout_pin = dout_ppin['pin']
        self.sclk_pin = sclk_ppin['pin']
        # Samples per second choices
        self.sps = config.getchoice('sample_rate', sample_rate_options,
                                    default=default_sample_rate)
        # gain/channel choices
        self.gain_channel = int(config.getchoice('gain', gain_options,
                                                 default=default_gain))
        ## Bulk Sensor Setup
        self.bulk_queue = bulk_sensor.BulkDataQueue(mcu, oid=self.oid)
        # Clock tracking
        chip_smooth = self.sps * UPDATE_INTERVAL * 2
        self.ffreader = bulk_sensor.FixedFreqReader(mcu, chip_smooth, "<i")
        # Process messages in batches
        self.batch_bulk = bulk_sensor.BatchBulkHelper(
            self.printer, self._process_batch, self._start_measurements,
            self._finish_measurements, UPDATE_INTERVAL)
        # Command Configuration
        self.query_hx71x_cmd = None
        self.attach_probe_cmd = None
        mcu.add_config_cmd(
            "config_hx71x oid=%d gain_channel=%d dout_pin=%s sclk_pin=%s"
            % (self.oid, self.gain_channel, self.dout_pin, self.sclk_pin))
        mcu.add_config_cmd("query_hx71x oid=%d rest_ticks=0"
                           % (self.oid,), on_restart=True)

        mcu.register_config_callback(self._build_config)

    def _build_config(self):
        self.query_hx71x_cmd = self.mcu.lookup_command(
            "query_hx71x oid=%c rest_ticks=%u")
        self.attach_probe_cmd = self.mcu.lookup_command(
            "hx71x_attach_load_cell_probe oid=%c load_cell_probe_oid=%c")
        self.ffreader.setup_query_command("query_hx71x_status oid=%c",
                                          oid=self.oid,
                                          cq=self.mcu.alloc_command_queue())


    def get_mcu(self):
        return self.mcu

    def get_samples_per_second(self):
        return self.sps

    # returns a tuple of the minimum and maximum value of the sensor, used to
    # detect if a data value is saturated
    def get_range(self):
        return -0x800000, 0x7FFFFF

    # add_client interface, direct pass through to bulk_sensor API
    def add_client(self, callback):
        self.batch_bulk.add_client(callback)

    def attach_load_cell_probe(self, load_cell_probe_oid):
        self.attach_probe_cmd.send([self.oid, load_cell_probe_oid])

    # Measurement decoding
    def _convert_samples(self, samples):
        adc_factor = 1. / (1 << 23)
        count = 0
        for ptime, val in samples:
            if val == SAMPLE_ERROR_DESYNC or val == SAMPLE_ERROR_LONG_READ:
                self.last_error_count += 1
                break  # additional errors are duplicates
            samples[count] = (round(ptime, 6), val, round(val * adc_factor, 9))
            count += 1
        del samples[count:]

    # Start, stop, and process message batches
    def _start_measurements(self):
        self.consecutive_fails = 0
        self.last_error_count = 0
        # Start bulk reading
        rest_ticks = self.mcu.seconds_to_clock(1. / (10. * self.sps))
        self.query_hx71x_cmd.send([self.oid, rest_ticks])
        logging.info("%s starting '%s' measurements",
                     self.sensor_type, self.name)
        # Initialize clock tracking
        self.ffreader.note_start()

    def _finish_measurements(self):
        # don't use serial connection after shutdown
        if self.printer.is_shutdown():
            return
        # Halt bulk reading
        self.query_hx71x_cmd.send_wait_ack([self.oid, 0])
        self.ffreader.note_end()
        logging.info("%s finished '%s' measurements",
                    self.sensor_type, self.name)

    def _process_batch(self, eventtime):
        prev_overflows = self.ffreader.get_last_overflows()
        prev_error_count = self.last_error_count
        samples = self.ffreader.pull_samples()
        self._convert_samples(samples)
        overflows = self.ffreader.get_last_overflows() - prev_overflows
        errors = self.last_error_count - prev_error_count
        if errors > 0:
            logging.error("%s: Forced sensor restart due to error", self.name)
            self._finish_measurements()
            self._start_measurements()
        elif overflows > 0:
            self.consecutive_fails += 1
            if self.consecutive_fails > 4:
                logging.error("%s: Forced sensor restart due to overflows",
                              self.name)
                self._finish_measurements()
                self._start_measurements()
        else:
            self.consecutive_fails = 0
        return {'data': samples, 'errors': self.last_error_count,
                'overflows': self.ffreader.get_last_overflows()}


def HX711(config):
    return HX71xBase(config, "hx711",
                     # HX711 sps options
                     {80: 80, 10: 10}, 80,
                     # HX711 gain/channel options
                     {'A-128': 1, 'B-32': 2, 'A-64': 3}, 'A-128')


def HX717(config):
    return HX71xBase(config, "hx717",
                     # HX717 sps options
                     {320: 320, 80: 80, 20: 20, 10: 10}, 320,
                     # HX717 gain/channel options
                     {'A-128': 1, 'B-64': 2, 'A-64': 3,
                      'B-8': 4}, 'A-128')


HX71X_SENSOR_TYPES = {
    "hx711": HX711,
    "hx717": HX717
}
