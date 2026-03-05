# Support for filament width sensor
#
# Copyright (C) 2019  Mustafa YILDIZ <mydiz@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import filament_switch_sensor

ADC_REPORT_TIME = 0.500
ADC_SAMPLE_TIME = 0.03
ADC_SAMPLE_COUNT = 15

class HallFilamentWidthSensor:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.pin1 = config.get('adc1')
        self.pin2 = config.get('adc2')
        self.dia1=config.getfloat('Cal_dia1', 1.5)
        self.dia2=config.getfloat('Cal_dia2', 2.0)
        self.rawdia1=config.getint('Raw_dia1', 9500)
        self.rawdia2=config.getint('Raw_dia2', 10500)
        self.MEASUREMENT_INTERVAL_MM=config.getint('measurement_interval',10)
        self.nominal_filament_dia = config.getfloat(
            'default_nominal_filament_diameter', above=1)
        self.measurement_delay = config.getfloat('measurement_delay', above=0.)
        self.measurement_max_difference = config.getfloat('max_difference', 0.2)
        self.max_diameter = (self.nominal_filament_dia
                             + self.measurement_max_difference)
        self.min_diameter = (self.nominal_filament_dia
                             - self.measurement_max_difference)
        self.diameter =self.nominal_filament_dia
        self.is_active =config.getboolean('enable', False)
        self.runout_dia_min=config.getfloat('min_diameter', 1.0)
        self.runout_dia_max=config.getfloat('max_diameter', self.max_diameter)
        self.is_log =config.getboolean('logging', False)
        self.enable_flow_compensation = config.getboolean(
            'enable_flow_compensation', True)
        # Use the current diameter instead of nominal while the first
        # measurement isn't in place
        self.use_current_dia_while_delay = config.getboolean(
            'use_current_dia_while_delay', False)
        if self.rawdia1 == self.rawdia2:
            raise config.error(
                "hall_filament_width_sensor: raw_dia1 and raw_dia2 must be"
                " different")
        if self.min_diameter <= 0:
            raise config.error(
                "hall_filament_width_sensor: max_difference must be less than"
                " default_nominal_filament_diameter")
        # Flags for deferred actions
        self.sensor_disable_pending = False
        self.flow_comp_disable_pending = False
        self.array_clear_pending = False
        # filament array [position, filamentWidth]
        self.filament_array = []
        self.lastFilamentWidthReading = 0
        self.lastFilamentWidthReading2 = 0
        self.firstExtruderUpdatePosition = 0
        self.filament_width = self.nominal_filament_dia
        # printer objects
        self.toolhead = self.ppins = self.mcu_adc = None
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        # Start adc
        self.ppins = self.printer.lookup_object('pins')
        self.mcu_adc = self.ppins.setup_pin('adc', self.pin1)
        self.mcu_adc.setup_adc_sample(ADC_REPORT_TIME,
                                      ADC_SAMPLE_TIME, ADC_SAMPLE_COUNT)
        self.mcu_adc.setup_adc_callback(self.adc_callback)
        self.mcu_adc2 = self.ppins.setup_pin('adc', self.pin2)
        self.mcu_adc2.setup_adc_sample(ADC_REPORT_TIME,
                                       ADC_SAMPLE_TIME, ADC_SAMPLE_COUNT)
        self.mcu_adc2.setup_adc_callback(self.adc2_callback)
        # extrude factor updating
        self.extrude_factor_update_timer = self.reactor.register_timer(
            self.extrude_factor_update_event)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('QUERY_FILAMENT_WIDTH',
            self.cmd_M407,
            desc=self.cmd_QUERY_FILAMENT_WIDTH_help)
        self.gcode.register_command('RESET_FILAMENT_WIDTH_SENSOR',
            self.cmd_ClearFilamentArray,
            desc=self.cmd_RESET_FILAMENT_WIDTH_SENSOR_help)
        self.gcode.register_command('DISABLE_FILAMENT_WIDTH_SENSOR',
            self.cmd_M406,
            desc=self.cmd_DISABLE_FILAMENT_WIDTH_SENSOR_help)
        self.gcode.register_command('ENABLE_FILAMENT_WIDTH_SENSOR',
            self.cmd_M405,
            desc=self.cmd_ENABLE_FILAMENT_WIDTH_SENSOR_help)
        self.gcode.register_command('QUERY_RAW_FILAMENT_WIDTH',
            self.cmd_Get_Raw_Values,
            desc=self.cmd_QUERY_RAW_FILAMENT_WIDTH_help)
        self.gcode.register_command('ENABLE_FILAMENT_WIDTH_LOG',
            self.cmd_log_enable,
            desc=self.cmd_ENABLE_FILAMENT_WIDTH_LOG_help)
        self.gcode.register_command('DISABLE_FILAMENT_WIDTH_LOG',
            self.cmd_log_disable,
            desc=self.cmd_DISABLE_FILAMENT_WIDTH_LOG_help)

        self.runout_helper = filament_switch_sensor.RunoutHelper(config)
        self.runout_helper.sensor_enabled = self.is_active
    # Initialization
    def handle_ready(self):
        # Load printer objects
        self.toolhead = self.printer.lookup_object('toolhead')

        # Start extrude factor update timer
        self.reactor.update_timer(self.extrude_factor_update_timer,
                                  self.reactor.NOW)

    def adc_callback(self, samples):
        # read sensor value
        read_time, read_value = samples[-1]
        self.lastFilamentWidthReading = round(read_value * 10000)

    def adc2_callback(self, samples):
        # read sensor value
        read_time, read_value = samples[-1]
        self.lastFilamentWidthReading2 = round(read_value * 10000)
        if self.is_active:
            # calculate diameter
            diameter_new = round((self.dia2 - self.dia1)/
                (self.rawdia2-self.rawdia1)*
            ((self.lastFilamentWidthReading+self.lastFilamentWidthReading2)
            -self.rawdia1)+self.dia1,2)
            self.diameter=(5.0 * self.diameter + diameter_new)/6
        else:
            self.diameter = self.nominal_filament_dia

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

    def extrude_factor_update_event(self, eventtime):
        # Handle any pending disable/clear operations
        if (self.sensor_disable_pending or
            self.flow_comp_disable_pending or
            self.array_clear_pending):
            self.gcode.run_script("M221 S100")
        if self.array_clear_pending:
            self.array_clear_pending = False
            self.filament_array = []
        if self.flow_comp_disable_pending:
            self.flow_comp_disable_pending = False
        if self.sensor_disable_pending:
            self.sensor_disable_pending = False
            self.filament_array = []
            self.is_active = False
            self.runout_helper.sensor_enabled = False
            self.runout_helper.note_filament_present(eventtime, True)
            return self.reactor.NEVER
        # Update extrude factor
        pos = self.toolhead.get_position()
        last_epos = pos[3]
        # Update filament array for lastFilamentWidthReading
        self.update_filament_array(last_epos)
        # Check runout
        self.runout_helper.note_filament_present(eventtime,
            self.runout_dia_min <= self.diameter <= self.runout_dia_max)
        # Does filament exists
        if self.diameter > self.runout_dia_min:
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
                    elif  self.firstExtruderUpdatePosition == pending_position:
                        self.filament_width = self.nominal_filament_dia
                if self.enable_flow_compensation:
                    if ((self.filament_width <= self.max_diameter)
                        and (self.filament_width >= self.min_diameter)):
                        percentage = round(self.nominal_filament_dia**2
                                        / self.filament_width**2 * 100)
                        self.gcode.run_script("M221 S" + str(percentage))
                    else:
                        self.gcode.run_script("M221 S100")
        else:
            if self.enable_flow_compensation:
                self.gcode.run_script("M221 S100")
            self.filament_array = []

        if self.is_active:
            return eventtime + 1
        else:
            return self.reactor.NEVER

    cmd_QUERY_FILAMENT_WIDTH_help = (
        "Report the filament width in mm and show sensor status")
    def cmd_M407(self, gcmd):
        response = "Filament diameter: "
        if self.is_active:
            response += "%.3f mm" % (self.diameter)
        else:
            response += "sensor disabled"
        response += "\nFilament state:"
        if self.is_active:
            if self.runout_helper.filament_present:
                response += " present"
            else:
                response += " not present"
        else:
            response += " sensor disabled"
        response += ("\nWidth sensor:"
            + (" ON" if self.is_active else " OFF"))
        response += ("\nFlow compensation:"
            + (" ON" if self.enable_flow_compensation else " OFF"))
        gcmd.respond_info(response)

    cmd_RESET_FILAMENT_WIDTH_SENSOR_help = "Clear all filament width readings"
    def cmd_ClearFilamentArray(self, gcmd):
        self.filament_array = []
        if self.is_active:
            self.array_clear_pending = True
        else:
            self.gcode.run_script_from_command("M221 S100")
        gcmd.respond_info("Filament width measurements cleared!")

    cmd_ENABLE_FILAMENT_WIDTH_SENSOR_help = (
        "Enable the filament width sensor and enable or disable flow "
        + "compensation")
    def cmd_M405(self, gcmd):
        flow_comp = gcmd.get_int("FLOW_COMPENSATION", None, minval=0, maxval=1)
        if flow_comp is not None:
            if self.enable_flow_compensation and not flow_comp:
                self.flow_comp_disable_pending = True
                self.enable_flow_compensation = False
            elif not self.enable_flow_compensation and flow_comp:
                self.flow_comp_disable_pending = False
                self.enable_flow_compensation = True

        was_active = self.is_active
        self.is_active = True
        self.runout_helper.sensor_enabled = True
        response = ("Filament width sensor: ON"
            + "\nFlow compensation: "
            + ("ON" if self.enable_flow_compensation else "OFF"))
        if not was_active:
            self.sensor_disable_pending = False
            self.reactor.update_timer(self.extrude_factor_update_timer,
                                        self.reactor.NOW)
        gcmd.respond_info(response)

    cmd_DISABLE_FILAMENT_WIDTH_SENSOR_help = (
        "Disable the filament width sensor")
    def cmd_M406(self, gcmd):
        response = "Filament width sensor: OFF"
        if self.is_active:
            self.sensor_disable_pending = True
        else:
            self.filament_array = []
            self.runout_helper.sensor_enabled = False
            self.runout_helper.note_filament_present(self.reactor.NOW, True)
            self.gcode.run_script_from_command("M221 S100")
        gcmd.respond_info(response)

    cmd_QUERY_RAW_FILAMENT_WIDTH_help = (
        "Report the raw filament width sensor values")
    def cmd_Get_Raw_Values(self, gcmd):
        response =  ("ADC1="+str(self.lastFilamentWidthReading))
        response += (" ADC2="+str(self.lastFilamentWidthReading2))
        response += (" RAW="+
                     str(self.lastFilamentWidthReading
                     +self.lastFilamentWidthReading2))
        gcmd.respond_info(response)

    def get_status(self, eventtime):
        status = self.runout_helper.get_status(eventtime)
        status.update({'Diameter': self.diameter,
                'Raw':(self.lastFilamentWidthReading+
                 self.lastFilamentWidthReading2),
                'is_active':self.is_active,
                'flow_compensation_enabled':self.enable_flow_compensation})
        return status

    cmd_ENABLE_FILAMENT_WIDTH_LOG_help = (
        "Enable filament width sensor logging")
    def cmd_log_enable(self, gcmd):
        self.is_log = True
        gcmd.respond_info("Filament width logging: ON")

    cmd_DISABLE_FILAMENT_WIDTH_LOG_help = (
        "Disable filament width sensor logging")
    def cmd_log_disable(self, gcmd):
        self.is_log = False
        gcmd.respond_info("Filament width logging: OFF")

def load_config(config):
    return HallFilamentWidthSensor(config)
