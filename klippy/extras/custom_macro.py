# Support for 1-wire based temperature sensors
#
# Copyright (C) 2020 Alan Lord <alanslists@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import time

class CUSTOM_MACRO:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.pheaters = None
        self.heater_hot = None
        self.extruder_temp=None
        self.bed_temp=None
        self.prtouch = None
        self.gcode.register_command("CX_PRINT_LEVELING_CALIBRATION", self.cmd_CX_PRINT_LEVELING_CALIBRATION, desc=self.cmd_CX_PRINT_LEVELING_CALIBRATION_help)
        self.gcode.register_command("CX_CLEAN_CALIBRATION_FLAGS", self.cmd_CX_CLEAN_CALIBRATION_FLAGS, desc=self.cmd_CX_CLEAN_CALIBRATION_FLAGS_help)
        self.gcode.register_command("CX_PRINT_DRAW_ONE_LINE", self.cmd_CX_PRINT_DRAW_ONE_LINE, desc=self.cmd_CX_PRINT_DRAW_ONE_LINE_help)
        self.gcode.register_command("CX_ROUGH_G28", self.cmd_CX_ROUGH_G28, desc=self.cmd_CX_ROUGH_G28_help)
        self.gcode.register_command("CX_NOZZLE_CLEAR", self.cmd_CX_NOZZLE_CLEAR, desc=self.cmd_CX_NOZZLE_CLEAR_help)
        self.default_extruder_temp = config.getfloat("default_extruder_temp", default=240.0)
        self.default_bed_temp = config.getfloat("default_bed_temp", default=50.0)
        self.g28_ext_temp = config.getfloat("g28_ext_temp", default=140.0)
        self.nozzle_clear = config.getboolean('nozzle_clear', True)
        self.calibration = config.getint('calibration', default=0)
        self.leveling_calibration = 0
        pass


    def get_status(self, eventtime):
        return {
            'leveling_calibration': self.leveling_calibration,
            'default_extruder_temp': self.default_extruder_temp,
            'default_bed_temp': self.default_bed_temp,
            'g28_ext_temp': self.g28_ext_temp
        }

    cmd_CX_PRINT_LEVELING_CALIBRATION_help = "Start Print function,three parameter:EXTRUDER_TEMP(180-300),BED_TEMP(0-100),CALIBRATION(0 or 1)"
    def cmd_CX_PRINT_LEVELING_CALIBRATION(self, gcmd):
        self.gcode.run_script_from_command('CHECK_BED_MESH AUTO_G29=1')
        pass

    cmd_CX_CLEAN_CALIBRATION_FLAGS_help = "Clean calibration flags"
    def cmd_CX_CLEAN_CALIBRATION_FLAGS(self, gcmd):
        self.leveling_calibration = 0
        pass

    cmd_CX_PRINT_DRAW_ONE_LINE_help = "Draw one line before printing"
    def cmd_CX_PRINT_DRAW_ONE_LINE(self, gcmd):
        self.gcode.run_script_from_command('M83')
        self.gcode.run_script_from_command('G1 X10 Y10 Z2 F6000')
        self.gcode.run_script_from_command('G1 Z0.1 F600')
        self.pheaters = self.printer.lookup_object('heaters')
        self.heater_hot = self.printer.lookup_object('extruder').heater
        self.gcode.respond_info("can_break_flag = %d" % (self.pheaters.can_break_flag))
        self.gcode.run_script_from_command('M104 S%d' % (self.extruder_temp))
        self.gcode.run_script_from_command('M140 S%d' % (self.bed_temp))
        self.pheaters.set_temperature(self.heater_hot, self.extruder_temp, True)
        self.gcode.respond_info("can_break_flag = %d" % (self.pheaters.can_break_flag))
        while self.pheaters.can_break_flag == 1:
            time.sleep(1)
        self.gcode.respond_info("can_break_flag = %d" % (self.pheaters.can_break_flag))
        if self.pheaters.can_break_flag == 3:
            self.pheaters.can_break_flag = 0
            self.gcode.respond_info("can_break_flag is 3")
            self.gcode.run_script_from_command('G21')
            self.gcode.run_script_from_command('G1 F2400 E-0.5')
            self.gcode.run_script_from_command('SET_VELOCITY_LIMIT SQUARE_CORNER_VELOCITY=5')
            self.gcode.run_script_from_command('M204 S12000')
            self.gcode.run_script_from_command('G21')
            self.gcode.run_script_from_command('SET_VELOCITY_LIMIT ACCEL_TO_DECEL=6000')
            # self.gcode.run_script_from_command('SET_PRESSURE_ADVANCE ADVANCE=0.04')
            # self.gcode.run_script_from_command('SET_PRESSURE_ADVANCE SMOOTH_TIME=0.04')
            self.gcode.run_script_from_command('M220 S100')
            self.gcode.run_script_from_command('M221 S100')
            self.gcode.run_script_from_command('G1 Z2.0 F1200')
            self.gcode.run_script_from_command('G1 X0.1 Y20 Z0.3 F6000.0')
            self.gcode.run_script_from_command('G1 X0.1 Y180.0 Z0.3 F3000.0 E10.0')
            self.gcode.run_script_from_command('G1 X0.4 Y180.0 Z0.3 F3000.0')
            self.gcode.run_script_from_command('G1 X0.4 Y20.0 Z0.3 F3000.0 E10.0')
            self.gcode.run_script_from_command('G1 Y10.0 F3000.0')
            self.gcode.run_script_from_command('G1 Z2.0 F600.0')
            self.gcode.run_script_from_command('G1 Z0.3 F600.0')
            self.gcode.run_script_from_command('G1 Z2.0 F600.0')
            # self.gcode.run_script_from_command('G1 X0.4 Y10.0 Z0.3 F6000.0')
            self.gcode.run_script_from_command('M82')
            self.gcode.run_script_from_command('G92 E0')
            # self.gcode.run_script_from_command('G1 Z2.0 F600')
            self.gcode.run_script_from_command('G1 F12000')
            self.gcode.run_script_from_command('G21')
        pass

    cmd_CX_ROUGH_G28_help = "rough G28"
    def cmd_CX_ROUGH_G28(self, gcmd):
        self.extruder_temp = gcmd.get_float('EXTRUDER_TEMP', default=self.default_extruder_temp, minval=180.0, maxval=320.0)
        self.g28_ext_temp = self.extruder_temp - 70
        if self.g28_ext_temp > 180.0:
            self.g28_ext_temp = 180.0
        try:
            self.prtouch = self.printer.lookup_object('prtouch_v2')
        except:
            self.prtouch = self.printer.lookup_object('prtouch')
            gcmd.respond_info("self.prtouch = prtouch")
        self.prtouch.change_hot_min_temp(self.g28_ext_temp)
        self.bed_temp = gcmd.get_float('BED_TEMP', default=self.default_bed_temp, minval=0.0, maxval=130.0)
        self.leveling_calibration = gcmd.get_int('LEVELING_CALIBRATION', default=1, minval=0, maxval=1)
        self.gcode.run_script_from_command('M104 S%d' % (self.g28_ext_temp))
        self.gcode.run_script_from_command('M140 S%d' % (self.bed_temp))
        self.gcode.run_script_from_command('M204 S500')
        self.gcode.run_script_from_command('G28')
        # self.gcode.run_script_from_command('NOZZLE_CLEAR HOT_MIN_TEMP=%d HOT_MAX_TEMP=%d BED_MAX_TEMP=%d' % (self.g28_ext_temp, self.extruder_temp - 20, self.bed_temp))
        pass

    cmd_CX_NOZZLE_CLEAR_help = "nozzle clear with temperature"
    def cmd_CX_NOZZLE_CLEAR(self, gcmd):
        self.gcode.run_script_from_command('NOZZLE_CLEAR HOT_MIN_TEMP=%d HOT_MAX_TEMP=%d BED_MAX_TEMP=%d' % (self.g28_ext_temp, self.extruder_temp - 20, self.bed_temp))
        pass

def load_config(config):
    return CUSTOM_MACRO(config)
