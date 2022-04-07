# Module for reading load cell ADC values as a weight scale
#
# Copyright (C) 2022 Chris Lombardi <clearchris@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

WS_REPORT_TIME = 1
WS_SAMPLE_TIME = 0.2
WS_SAMPLE_COUNT = 4

class WeighScale:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name =  config.get_name().split()[1]
        self.full_name = config.get_name()
        self.pin_name = config.get('adc')
        self.ppins = self.printer.lookup_object('pins')
        self.mcu_adc = self.ppins.setup_pin('adc', self.pin_name)
        self.mcu_adc.setup_minmax(WS_SAMPLE_TIME, WS_SAMPLE_COUNT)
        self.max_weight = config.get('max_weight')
        self.weight_subscribers = []

        self.calibrate_v1 = 0.
        self.calibrate_v2 = 0.
        self.calibrate_weight1 = 0.
        self.calibrate_weight2 = 0.
        self.tare_offset = 0.
        self.last_measurement = 0.
        self.multiplier = 0.

        self.calibrated = False
        # attempt to read the calibration configuration
        # not all configuration is essential to startup
        try:
            self.calibrate_v1 = config.getfloat('calibrate_v1')
            self.calibrate_v2 = config.getfloat('calibrate_v2')
            self.calibrate_weight1 = config.getfloat('calibrate_weight1')
            self.calibrate_weight2 = config.getfloat('calibrate_weight2')
            self.calibrated = True
        except:
            logging.warn("Please calibrate the scale using" \
                " the CALIBRATE_SCALE command.")

        # check the calibration configuration
        if self.calibrated:
            if (self.calibrate_weight1 > self.max_weight or
                self.calibrate_weight2 > self.max_weight):
                logging.error("Calibration weights cannot be" \
                    " greater than max weight")
                self.calibrated = False
            elif (self.calibrate_weight1 == self.calibrate_weight2):
                logging.error("Please calibrate the scale with" \
                    " two different weights")
                self.calibrated = False

        # if calibration config is ok, calculate calibration vaules
        if self.calibrated:
            weight_range = self.calibrate_weight2 - self.calibrate_weight1
            voltage_range = self.calibrate_v2 - self.calibrate_v1
            self.multiplier = weight_range/voltage_range
            self.offset_g = self.calibrate_weight1
            self.offset_v = self.calibrate_v1

        # tare is optional, ignore errors
        if self.calibrated:
            try:
                self.tare_offset = config.getfloat('tare_offset')
            except:
                pass

        self.printer.register_event_handler("klippy:ready", self._handle_ready)

        # register gcode commands
        self.gcode = self.printer.lookup_object('gcode')

        self.gcode.register_mux_command('READ_SCALE', 'SCALE', self.name,
                                    self.cmd_READ_SCALE,
                                    desc=self.cmd_READ_SCALE_help)

        self.gcode.register_mux_command('READ_SCALE_RAW', 'SCALE', self.name,
                                    self.cmd_READ_SCALE_RAW,
                                    desc=self.cmd_READ_SCALE_RAW_help)

        self.gcode.register_mux_command('TARE_SCALE', 'SCALE', self.name,
                                    self.cmd_TARE_SCALE,
                                    desc=self.cmd_TARE_SCALE_help)

        self.gcode.register_mux_command('CALIBRATE_SCALE', 'SCALE', self.name,
                                    self.cmd_CALIBRATE_SCALE,
                                    desc=self.cmd_CALIBRATE_SCALE_help)

    def _handle_ready(self):
        self.mcu_adc.setup_adc_callback(WS_REPORT_TIME, self._adc_callback)


    def _adc_callback(self, time, value):
        self.last_measurement = value
        for sub in self.weight_subscribers :
            sub(value)


    def _get_weight(self):
        #logging.debug("%f %f %f %f" % ( self.offset_g, self.last_measurement, \
        #     self.offset_v, self.multiplier))
        return self.offset_g + (self.last_measurement - self.offset_v) * \
            self.multiplier


    def _set_tare(self):
        self.tare_offset = self._get_weight()

    def _set_tare_manual(self, manual):
        self.tare_offset = manual;


    def _get_tare_weight(self):
        return self._get_weight() - self.tare_offset


    def _check_calibrated(self, gcmd):
        if not self.calibrated:
            raise gcmd.error(
                "Please calibrate the scale with the CALIBRATE_SCALE " + \
                "command. Use SAVE_CONFIG after calibration.")


    cmd_READ_SCALE_help = "Print current scale measurement, " \
        "compensated by the last tare"

    def cmd_READ_SCALE(self, gcmd):
        self._check_calibrated(gcmd)
        weight = self._get_tare_weight()
        gcmd.respond_info("Weight: %.3f" % (weight))


    cmd_READ_SCALE_RAW_help = "Print current scale measurement adc value"

    def cmd_READ_SCALE_RAW(self, gcmd):
        gcmd.respond_info("Raw ADC Value: %.20f" % self.last_measurement)


    cmd_TARE_SCALE_help = "Set scale reading to zero"

    def cmd_TARE_SCALE(self, gcmd):
        self._check_calibrated(gcmd)
        try:
            manual_tare = gcmd.get_float('WEIGHT')
            self._set_tare_manual(manual_tare)
        except:
            self._set_tare()
        gcmd.respond_info("Setting tare offset: %.3f" % self.tare_offset)
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.full_name, 'tare_offset', "%.5f" % self.tare_offset)
        gcmd.respond_info(
            "The SAVE_CONFIG command will update the printer config file\n"
            "with the new tare offset and restart the printer." )


    cmd_CALIBRATE_SCALE_help = "Using an object of known weight, " \
        "calibrate the scale"

    def cmd_CALIBRATE_SCALE(self, gcmd):
        # set current reading and entered weight
        value = gcmd.get_int('VALUE')
        if not (value == 1 or value == 2):
            raise gcmd.error("Calibration VALUE must be 1 or 2")
        weight = gcmd.get_float('WEIGHT', 0., minval=0.)
        if weight > self.max_weight:
            raise gcmd.error("Calibration weight must be less than max weight")
        raw_value = self.last_measurement
        configfile = self.printer.lookup_object('configfile')
        if value == 1:
           self.calibrate_v1 = raw_value
           self.calibrate_weight1 = weight
           configfile.set(self.full_name, 'calibrate_v1', "%.12f" %
               (raw_value,))
           configfile.set(self.full_name, 'calibrate_weight1',
               "%.3f" %(weight,))
        else:
           self.calibrate_v2 = raw_value
           self.calibrate_weight2 = weight
           configfile.set(self.full_name, 'calibrate_v2', "%.12f" %
               (raw_value,))
           configfile.set(self.full_name, 'calibrate_weight2',
               "%.3f" %(weight,))
        gcmd.respond_info("Calibration value %1d set to %.3f for %.3f weight" %
           (value, raw_value, weight) )
        gcmd.respond_info(
                "The SAVE_CONFIG command will update the printer config file\n"
                "with the new calibration values and restart the printer." )


    def get_status(self, eventtime):
        if not self.calibrated:
            return { 'calibrated': False, 'weight': 0. }
        else:
            return { 'calibrated': True, 'weight': self._get_tare_weight() }


class PrinterWeighScale:

    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.name = config.get_name().split()[1]
        scale = WeighScale(config)
        config.printer.add_object("weigh_scale " + self.name, scale)

def load_config_prefix(config):
    return PrinterWeighScale(config)
