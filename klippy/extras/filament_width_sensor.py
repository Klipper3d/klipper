# Support for filament width sensor
#
# Copyright (C) 2021 Dawid Murawski <dawid.m@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from . import filament_switch_sensor
import logging
from time import strftime

ADC_REPORT_TIME = 0.500
ADC_SAMPLE_TIME = 0.001
ADC_SAMPLE_COUNT = 8

class FilamentWidthSensor:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.pin1 = config.get('adc1')
        self.pin2 = config.get('adc2', None)
        self.dia1=config.getfloat('Cal_dia1', 1.5)
        self.dia2=config.getfloat('Cal_dia2', 2.0)
        self.rawdia1=config.getint('Raw_dia1', 9500)
        self.rawdia2=config.getint('Raw_dia2', 10500)
        if self.pin2 == None:
            self.lastFilamentWidthReading2 = 0
        self.extruder_name = config.get('extruder')
        # Calibration of sensor: y=mx+b,
        # Filament width = adc_value * slope + offset
        self.measurement_offset1 = config.getfloat('offset1', 0)
        self.measurement_slope1 = config.getfloat('slope1', 1)
        self.measurement_offset2 = config.getfloat('offset2', 0)
        self.measurement_slope2 = config.getfloat('slope2', 1)
        self.MEASUREMENT_INTERVAL_MM=config.getint('measurement_interval',10)
        self.nominal_filament_dia = config.getfloat(
            'default_nominal_filament_diameter', above=1, default=1.75)
        self.measurement_delay = config.getfloat('measurement_delay', above=0.)
        self.measurement_max_difference = 
            config.getfloat('max_difference', 0.2)
        self.measurement_min_difference = config.getfloat('min_difference',
            self.measurement_max_difference)
        self.max_diameter = (self.nominal_filament_dia
                             + self.measurement_max_difference)
        self.min_diameter = (self.nominal_filament_dia
                             - self.measurement_min_difference)
        self.diameter =self.nominal_filament_dia
        self.is_active = config.getboolean('enable', False)
        self.runout_dia = config.getfloat('min_diameter', 1.0)
        self.clog_dia = config.getfloat('max_diameter', 2.0)
        self.is_log =config.getboolean('logging', False)

        # Enable extrusion modifier and/or use sensor as Runout switch only
        self.extrude_factor_enable = config.getboolean(
            'extrude_factor_enable', False)
        # Use the current diameter instead of nominal while the first
        # measurement isn't in place
        self.use_current_dia_while_delay = config.getboolean(
            'use_current_dia_while_delay', False)
        # filament array [position, filamentWidth]
        self.filament_array = []
        self.lastFilamentWidthReading = 0
        self.lastFilamentWidthReading2 = 0
        self.lastTime = self.lastTime2 = 0
        self.firstExtruderUpdatePosition = 0
        self.filament_width = self.nominal_filament_dia
        # printer objects
        self.extruder = self.estimated_print_time = None
        self.ppins = self.mcu_adc = None
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        # Start adc
        self.ppins = self.printer.lookup_object('pins')
        self.mcu_adc = self.ppins.setup_pin('adc', self.pin1)
        self.mcu_adc.setup_minmax(ADC_SAMPLE_TIME, ADC_SAMPLE_COUNT)
        self.mcu_adc.setup_adc_callback(ADC_REPORT_TIME, self.adc_callback)
        if self.pin2 != None:
            self.mcu_adc2 = self.ppins.setup_pin('adc', self.pin2)
            self.mcu_adc2.setup_minmax(ADC_SAMPLE_TIME, ADC_SAMPLE_COUNT)
            self.mcu_adc2.setup_adc_callback(ADC_REPORT_TIME, 
                self.adc2_callback)
        # extrude factor updating
        self.extrude_factor_update_timer = self.reactor.register_timer(
            self.extrude_factor_update_event)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('QUERY_FILAMENT_WIDTH', self.cmd_M407)
        self.gcode.register_command('RESET_FILAMENT_WIDTH_SENSOR',
                                    self.cmd_ClearFilamentArray)
        self.gcode.register_command('DISABLE_FILAMENT_WIDTH_SENSOR',
                                    self.cmd_M406)
        self.gcode.register_command('ENABLE_FILAMENT_WIDTH_SENSOR',
                                    self.cmd_M405)
        self.gcode.register_command('QUERY_RAW_FILAMENT_WIDTH',
                                    self.cmd_Get_Raw_Values)
        self.gcode.register_command('ENABLE_FILAMENT_WIDTH_LOG',
                                    self.cmd_log_enable)
        self.gcode.register_command('DISABLE_FILAMENT_WIDTH_LOG',
                                    self.cmd_log_disable)
        self.runout_helper = filament_switch_sensor.RunoutHelper(config)

    # Initialization
    def handle_ready(self):
        # Load printer objects
        self.extruder = self.printer.lookup_object(self.extruder_name)
        self.estimated_print_time = (
            self.printer.lookup_object('mcu').estimated_print_time)
        # Start extrude factor update timer
        self.reactor.update_timer(self.extrude_factor_update_timer,
                                  self.reactor.NOW)

    def adc_callback(self, read_time, read_value):
        # read sensor value
        self.lastFilamentWidthReading = read_value
        self.lastTime = read_time
        if self.pin2 == None:
            self.calc_diameter()

    def adc2_callback(self, read_time, read_value):
        # read sensor value
        self.lastFilamentWidthReading2 = read_value
        self.lastTime2 = read_time
        # calculate diameter
        self.calc_diameter()

    def calc_diameter(self):
        if self.pin2 != None:
            self.lastFilamentWidthReading = self.lastFilamentWidthReading \
                * self.measurement_slope2 + self.measurement_offset2
            self.lastFilamentWidthReading2 = self.lastFilamentWidthReading2 \
                * self.measurement_slope2 + self.measurement_offset2
            diameter_new = round((self.dia2 - self.dia1)/
                (self.rawdia2-self.rawdia1)*
              ((round(self.lastFilamentWidthReading*10000)
              +round(self.lastFilamentWidthReading2*10000))
               -self.rawdia1)+self.dia1,2)
            self._diameter=(5.0 * self.diameter + diameter_new)/6
        else:
            self._diameter = self.lastFilamentWidthReading
        self.diameter = self._diameter * self.measurement_slope1 
            + self.measurement_offset1

    def update_filament_array(self, last_epos):
        # Fill array
        if len(self.filament_array) > 0:
            # Get last reading position in array & calculate next
            # reading position
          next_reading_position = (self.filament_array[-1][0] +
          self.MEASUREMENT_INTERVAL_MM)
          if next_reading_position <= (last_epos + self.measurement_delay):
            self.filament_array.append([last_epos + self.measurement_delay,
                                            self.diameter])
            if self.is_log:
                 self.gcode.respond_info("Filament width:%.3f" %
                                         ( self.diameter ))
        else:
            # add first item to array
            self.filament_array.append([self.measurement_delay + last_epos,
                                        self.diameter])
            self.firstExtruderUpdatePosition = (self.measurement_delay
                                                + last_epos)

        # Logging
        if self.is_log:
            with open("/tmp/width.log", "a") as width_log:
                width_log.write(strftime("%a, %d %b %Y %H:%M:%S")
                    + " Filament dia: "
                    + str(self.diameter) + " mm\n")

    def get_extruder_pos(self, eventtime = None):
        if eventtime is None:
            eventtime = self.reactor.monotonic()
        print_time = self.estimated_print_time(eventtime)
        return self.extruder.find_past_position(print_time)

    def extrude_factor_update_event(self, eventtime):
        # Update extrude factor
        last_epos = self.get_extruder_pos(eventtime)
        # Update filament array for lastFilamentWidthReading
        self.update_filament_array(last_epos)
        # Check runout/clog
        self.runout_helper.note_filament_present(
            self.diameter > self.runout_dia)
        self.runout_helper.note_filament_present(
            self.diameter < self.clog_dia)

        # Is extrusion modifier enabled
        if self.extrude_factor_enable:
            # Does filament exists
            if self.diameter > 0.5:
                if len(self.filament_array) > 0:
                    # Get first position in filament array
                    pending_position = self.filament_array[0][0]
                    if pending_position <= last_epos:
                        # Get first item in filament_array queue
                        item = self.filament_array.pop(0)
                        self.filament_width = item[1]
                    else:
                        if ((self.use_current_dia_while_delay)
                            and (self.firstExtruderUpdatePosition
                                 == pending_position)):
                            self.filament_width = self.diameter
                        elif  self.firstExtruderUpdatePosition == 
                            pending_position:
                            self.filament_width = self.nominal_filament_dia
                    if ((self.filament_width <= self.max_diameter)
                        and (self.filament_width >= self.min_diameter)):
                        percentage = round(self.nominal_filament_dia**2
                                           / self.filament_width**2 * 100)
                        self.gcode.run_script("M221 S" + str(percentage))
                    else:
                        self.gcode.run_script("M221 S100")
            else:
                self.gcode.run_script("M221 S100")
                self.filament_array = []

        if self.is_active:
            return eventtime + 1
        else:
            return self.reactor.NEVER

    def cmd_M407(self, gcmd):
        response = ""
        if self.diameter > 0:
            response += ("Filament dia (measured mm): "
                         + str(self.diameter))
        else:
            response += "Filament NOT present"
        gcmd.respond_info(response)

    def cmd_ClearFilamentArray(self, gcmd):
        self.filament_array = []
        gcmd.respond_info("Filament width measurements cleared!")
        # Set extrude multiplier to 100%
        self.gcode.run_script_from_command("M221 S100")

    def cmd_M405(self, gcmd):
        response = "Filament width sensor Turned On"
        if self.is_active:
            response = "Filament width sensor is already On"
        else:
            self.is_active = True
            # Start extrude factor update timer
            self.reactor.update_timer(self.extrude_factor_update_timer,
                                      self.reactor.NOW)
        gcmd.respond_info(response)

    def cmd_M406(self, gcmd):
        response = "Filament width sensor Turned Off"
        if not self.is_active:
            response = "Filament width sensor is already Off"
        else:
            self.is_active = False
            # Stop extrude factor update timer
            self.reactor.update_timer(self.extrude_factor_update_timer,
                                      self.reactor.NEVER)
            # Clear filament array
            self.filament_array = []
            # Set extrude multiplier to 100%
            self.gcode.run_script_from_command("M221 S100")
        gcmd.respond_info(response)

    def cmd_Get_Raw_Values(self, gcmd):
        response = "ADC1="
        response +=  (" "+str(self.lastFilamentWidthReading))
        if self.pin2 != None:
            response +=  (" ADC2="+str(self.lastFilamentWidthReading2))
            response +=  (" RAW="+
                      str(self.lastFilamentWidthReading
                      +self.lastFilamentWidthReading2))
        else:
            response +=  (" Width="+
                str(self.diameter))
        gcmd.respond_info(response)
    def get_status(self, eventtime):
        return {'Diameter': self.diameter,
                'Raw':(self.lastFilamentWidthReading+
                 self.lastFilamentWidthReading2),
                'is_active':self.is_active}
    def cmd_log_enable(self, gcmd):
        self.is_log = True
        gcmd.respond_info("Filament width logging Turned On")

    def cmd_log_disable(self, gcmd):
        self.is_log = False
        gcmd.respond_info("Filament width logging Turned Off")

def load_config(config):
    return FilamentWidthSensor(config)
