# Support for Marlin/Smoothie/Reprap style firmware retraction via G10/G11
# Zhop funtionality includes:
#   - Standard zhop (vertical move up, travel, vertical move down)
#
# Copyright (C) 2023  Florian-Patrice Nagel <flopana77@gmail.com>
# Copyright (C) 2019  Len Trigg <lenbok@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

# Constants
ZHOP_MOVE_SPEED_FRACTION = 0.8

class FirmwareRetraction:
    ################################################################# Class init
    def __init__(self, config):
        self.config_ref = config
        self.printer = config.get_printer()
              # Get retraction params from config, used also in clear retraction
        self._get_config_params()
                                                          # Initialize variables
        self.unretract_length = (self.retract_length
                                 + self.unretract_extra_length)
        self.currentPos = []
        self.currentZ = 0.0
        self.z_hop_Z = 0.0                           # Z coordinate of zhop move

        self.is_retracted = False                           # Retract state flag
        self.vsdcard_paused = False                         # VSDCard pause flag
        self.G1_toggle_state = False                      # G1 toggle state flag

        # Get maximum printer move velocity for zhop moves
        printer_config = config.getsection('printer')
        self.max_vel = printer_config.getfloat('max_velocity')

        self.printer.register_event_handler("klippy:ready", self._handle_ready)

    ##################### Helper method to get retraction parameters from config
    def _get_config_params(self):
        self.retract_length = self.config_ref.getfloat(\
            'retract_length', 0., minval=0.)
        self.retract_speed = self.config_ref.getfloat(\
            'retract_speed', 20., minval=1)
        self.unretract_extra_length = self.config_ref.getfloat(
            'unretract_extra_length', 0., minval=0.)
        self.unretract_speed = self.config_ref.getfloat(\
            'unretract_speed', 10., minval=1)
            # Zero min. and stand. zhop valueto ensure compatibility with macros
        self.z_hop_height = self.config_ref.getfloat(\
            'z_hop_height', 0., minval=0.)

    ######## Helper method to register commands and instantiate required objects
    def _handle_ready(self):
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.lookup_object('gcode_move')
        self.toolhead = self.printer.lookup_object('toolhead')

        # Register new G-code commands for setting/retrieving retraction
        # parameters as well as clearing retraction state
        self.gcode.register_command('SET_RETRACTION', self.cmd_SET_RETRACTION,
                                    desc=self.cmd_SET_RETRACTION_help)
        self.gcode.register_command('GET_RETRACTION', self.cmd_GET_RETRACTION,
                                    desc=self.cmd_GET_RETRACTION_help)
        self.gcode.register_command('CLEAR_RETRACTION',\
            self.cmd_CLEAR_RETRACTION, desc=self.cmd_CLEAR_RETRACTION_help)

        # Register new G-code commands for firmware retraction/unretraction
        self.gcode.register_command('G10', self.cmd_G10)
        self.gcode.register_command('G11', self.cmd_G11)


        # Register Events to clear retraction when a new print is started, an
        # ongoing print is canceled or a print is finished
        ################ GCode streaming mode (most commonly done via OctoPrint)
        self.printer.register_event_handler("homing:homing_move_begin", \
            self._evaluate_retraction)
        self.printer.register_event_handler("stepper_enable:motor_off", \
            self._evaluate_retraction)

        #### Virtual SD card mode (Mainsail, Fluidd and DWC2-to-Klipper default)
                               # Only register events if Virtual SD Card enabled
        if self.config_ref.has_section('virtual_sdcard'):
            self.vsdcard = self.printer.lookup_object('virtual_sdcard')
            self.printer.register_event_handler("virtual_sdcard:reset_file", \
                self._reset_pause_flag)
            self.printer.register_event_handler("print_stats:start_printing", \
                self._evaluate_retraction)
            self.printer.register_event_handler("print_stats:complete_printing"\
                , self._evaluate_retraction)
            self.printer.register_event_handler("print_stats:cancelled_printing\
                ", self._reset_pause_flag)
            self.printer.register_event_handler("print_stats:paused_printing", \
                self._set_pause_flag)

    ###### Helper method to evaluate to clear retraction if certain events occur
    # (must accept all arguments passed from event handlers)
    def _evaluate_retraction(self, *args):
        if self.is_retracted:                               # Check if retracted
                if self.vsdcard_paused:          # Check if VSDCard print paused
                    # Reset paused flag and hence do not clear retraction on
                    # resume command.
                    self.vsdcard_paused = False
                else:
                    # If cancel command triggered pause event, clear retraction.
                    self._execute_clear_retraction()

    ################## Helper method to return the current retraction parameters
    def get_status(self, eventtime):
        return {
            "retract_length": self.retract_length,
            "retract_speed": self.retract_speed,
            "unretract_extra_length": self.unretract_extra_length,
            "unretract_speed": self.unretract_speed,
            'z_hop_height': self.z_hop_height,
            'unretract_length': self.unretract_length,
            'retract_state': self.is_retracted,
        }

    ########### Helper method to reset pause flags and force evaluate retraction
    def _reset_pause_flag(self, *args):
        self.vsdcard_paused = False
        self._evaluate_retraction()

    ############################################ Helper method to set pause flag
    def _set_pause_flag(self, *args):
        self.vsdcard_paused = True

    ########################## Command to set the firmware retraction parameters
    cmd_SET_RETRACTION_help = ("Set firmware retraction parameters")

    def cmd_SET_RETRACTION(self, gcmd):
        if not self.is_retracted:        # Only execute command when unretracted
            self._execute_set_retraction(gcmd)     # Execute command immediately
        else:
            self.gcode.respond_info(
                "WARNING: Printer in retract state. SET_RETRACTION will not be\
                    executed!")

    ################### Helper to set retraction parameters if command is called
    def _execute_set_retraction(self,gcmd):
        self.retract_length = gcmd.get_float('RETRACT_LENGTH',
                                             self.retract_length, minval=0.)
        self.retract_speed = gcmd.get_float('RETRACT_SPEED',
                                            self.retract_speed, minval=1.)
        self.unretract_extra_length = gcmd.get_float(
            'UNRETRACT_EXTRA_LENGTH', self.unretract_extra_length, minval=-1.)
        self.unretract_speed = gcmd.get_float('UNRETRACT_SPEED',
                                              self.unretract_speed, minval=1.)
        self.z_hop_height = gcmd.get_float('Z_HOP_HEIGHT', self.z_hop_height, \
            minval=0.)      # z_hop_height with 0mm min. to prevent nozzle crash
        self.unretract_length = (self.retract_length
                                 + self.unretract_extra_length)

    ############### Command to report the current firmware retraction parameters
    cmd_GET_RETRACTION_help = ("Report firmware retraction paramters")

    def cmd_GET_RETRACTION(self, gcmd):
        gcmd.respond_info('RETRACT_LENGTH=%.5f RETRACT_SPEED=%.5f '
                          'UNRETRACT_EXTRA_LENGTH=%.5f UNRETRACT_SPEED=%.5f '
                          ' Z_HOP_HEIGHT=%.5f '
                          ' RETRACTED=%s '
                          % (self.retract_length, self.retract_speed,
                             self.unretract_extra_length, self.unretract_speed,
                             self.z_hop_height, self.is_retracted))

    ##### Command to clear FW retraction (add to CANCEL macros at the beginning)
    cmd_CLEAR_RETRACTION_help = ('Clear retraction state without retract move \
        or zhop, if enabled')

    def cmd_CLEAR_RETRACTION(self, gcmd):
        if self.is_retracted:
            self._execute_clear_retraction()
            gcmd.respond_info('Retraction was cleared and reset to config \
                values. zhop is undone on next move.')
        else:
            gcmd.respond_info('WARNING: Printer is not retracted. \
                Command has been ignored!')

    ################################################# Helper to clear retraction
    def _execute_clear_retraction(self):
        if self.z_hop_height > 0.0:
            # Re-establish regular G1 command if zhop enabled.
            # zhop will be reversed on next move with z coordinate
            self._re_register_G1()
            self.G1_toggle_state = False            # Prevent repeat re-register
        self.is_retracted = False     # Reset retract flag to enable G10 command
        self._get_config_params()  #Reset retraction parameters to config values

    ########################### Gcode Command G10 to perform firmware retraction
    def cmd_G10(self, gcmd):
        retract_gcode = ""                                # Reset retract string
        zhop_gcode = ""                                 # Reset zhop move string

        if self.retract_length == 0.0:          # Check if FW retraction enabled
            gcmd.respond_info('Retraction length zero. Firmware retraction \
                disabled. Command ignored!')
        elif not self.is_retracted:  # If filament isn't retracted, build G-Code
            # Incl move command if z_hop_height > 0
            if self.z_hop_height > 0.0:
                # Determine z coordinate for zhop move
                self._set_zhop_move_params()
                # Set zhop gcode move
                zhop_gcode = (
                        "G1 Z{:.5f} F{}\n"
                    ).format(self.z_hop_Z, int(ZHOP_MOVE_SPEED_FRACTION *\
                        self.max_vel * 60))

            retract_gcode = (
                "SAVE_GCODE_STATE NAME=_retract_state\n"
                "G91\n"
                "G1 E-{:.5f} F{}\n"          # Retract filament at retract speed
                "G90\n"                 # Switch to absolute mode (just in case)
                "{}"
                "RESTORE_GCODE_STATE NAME=_retract_state"
            ).format(self.retract_length, int(self.retract_speed * 60), \
                zhop_gcode)

            self.gcode.run_script_from_command(retract_gcode)
            self.is_retracted = True

            if self.z_hop_height > 0.0:
                # Swap original G1 handlers if z_hop enabled to offset following
                # moves in eiter absolute or relative mode
                self._unregister_G1()
                self.G1_toggle_state = True #Prevent repeat unregister with flag

        else:
            gcmd.respond_info('Printer is already retracted. Command ignored!')

    ######################################## Helper to determine zhop coordinate
    def _set_zhop_move_params(self):
        self.currentPos = self._get_gcode_pos()
        self.currentZ = self.currentPos[2]
        self.z_hop_Z = self.currentZ + self.z_hop_height

    ####################################### Helper to get current gcode position
    def _get_gcode_pos(self):
        # Get current gcode position for z_hop move if enabled
        gcodestatus = self.gcode_move.get_status()
        currentPos = gcodestatus['gcode_position']
        return currentPos

    ######################### GCode Command G11 to perform filament unretraction
    def cmd_G11(self, gcmd):
        unretract_gcode = ""                            # Reset unretract string
        unzhop_gcode = ""                            # Reset un-zhop move string

        if self.retract_length == 0.0:          # Check if FW retraction enabled
            gcmd.respond_info('Retraction length zero. Firmware retraction \
                disabled. Command ignored!')
        elif self.is_retracted:             # Check if the filament is retracted
            if self.z_hop_height > 0.0:
                self._re_register_G1()         # Restore G1 handlers if z_hop on
                self.G1_toggle_state = False        # Prevent repeat re-register
                # Set unzhop gcode move
                unzhop_gcode = (
                        "G1 Z-{:.5f} F{}\n"
                    ).format(self.z_hop_height, \
                        int(ZHOP_MOVE_SPEED_FRACTION * self.max_vel * 60))

            unretract_gcode = (
                "SAVE_GCODE_STATE NAME=_unretract_state\n"
                "G91\n"
                "{}"
                "G1 E{:.5f} F{}\n"                      # Unretract filament
                "RESTORE_GCODE_STATE NAME=_unretract_state"
                ).format(unzhop_gcode, self.unretract_length, \
                    int(self.unretract_speed * 60))

            self.gcode.run_script_from_command(unretract_gcode)
            self.is_retracted = False     # Set the flag to filament unretracted
        else:
            gcmd.respond_info('Printer is not retracted. Command ignored!')

    ############################################ Register new G1 command handler
    def _unregister_G1(self):
        # Change handler only if G1 command has not been toggled before
        if self.G1_toggle_state == False:
            self._toggle_gcode_comms('G1.20140114', 'G1', self._G1_zhop, \
                'G1 command that accounts for z hop when retracted', \
                self.G1_toggle_state)
            self._toggle_gcode_comms('G0.20140114', 'G0', self._G1_zhop, \
                'G0 command that accounts for z hop when retracted', \
                self.G1_toggle_state)

    ##################### Helper to toggle/untoggle command handlers and methods
    def _toggle_gcode_comms(self, new_cmd_name, old_cmd_name, new_cmd_func, \
        new_cmd_desc, toggle_state):
        # Unregister current method from current handler and store in prev_cmd
        prev_cmd = self.gcode.register_command(old_cmd_name, None)
        pdesc = 'Renamed builtin of "%s"' % old_cmd_name
        if not toggle_state:
            # Register prev method to toggled handler, indicate built-in command
            self.gcode.register_command(new_cmd_name, prev_cmd, desc=pdesc)
            self.gcode.register_command(old_cmd_name, new_cmd_func, \
                desc=new_cmd_desc)  # Register toggled method to command handler
        else:
            # Unregister toggled command from the untoggled handler
            self.gcode.register_command(new_cmd_name, new_cmd_func)
            self.gcode.register_command(new_cmd_name, prev_cmd, \
                desc=new_cmd_desc)  # Register untoggled method to untog handler

    ############ G1 method that accounts for z-hop by altering the z-coordinates
    # Offsets are not touched to prevent incompatibility issues with user macros
    def _G1_zhop(self,gcmd):
        params = gcmd.get_command_parameters()
        is_relative = self._toolhead_is_relative()

        if 'Z' in params and not is_relative:
                       # In absolute, adjust Z param to account for Z-hop offset
            params['Z'] = str(float(params['Z']) + self.z_hop_height)
                   # In relative, don't adjust as Z-hop offset considered before

        new_g1_command = 'G1.20140114'
        for key, value in params.items():
            new_g1_command += ' {0}{1}'.format(key, value)

        # Run originl G1 (renamed G1.20140114) with adjusted parameters
        self.gcode.run_script_from_command(new_g1_command)

    ####################################### Helper to get absolute/relative mode
    def _toolhead_is_relative(self):
        gcodestatus = self.gcode_move.get_status()
        movemode = gcodestatus['absolute_coordinates']
        return not movemode

    ######################################### Re-register old G1 command handler
    def _re_register_G1(self):
        # Change handler only if G1 command has been toggled before
        if self.G1_toggle_state == True:
            self._toggle_gcode_comms('G1', 'G1.20140114', None, 'cmd_G1_help', \
                                    self.G1_toggle_state)
            self._toggle_gcode_comms('G0', 'G0.20140114', None, 'cmd_G1_help', \
                                    self.G1_toggle_state)

def load_config(config):
    return FirmwareRetraction(config)
