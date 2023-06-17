# Support for Marlin/Smoothie/Reprap style firmware retraction via G10/G11 and
# M101/M103. Zhop funtionality includes:
#   - Standard zhop (vertical move up, travel, vertical move down),
#   - Diagonal zhop (travel move with vertical zhop component and vertical move
#     down as proposed by the community),
#   - Helix zhop (helix move up, travel, vertical move down as implemented as
#     default by BambuStudio).
#
# Copyright (C) 2023  Florian-Patrice Nagel <flopana77@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

# Constants
RETRACTION_MOVE_SPEED_FRACTION = 0.8

class FirmwareRetraction:
    ################################################################# Class init
    def __init__(self, config):
                                                     # Define valid z_hop styles
        self.valid_z_hop_styles = ['standard','ramp', 'helix']
        self.config_ref = config
        self.printer = config.get_printer()
              # Initialize various retraction-related parameters from the config
        self._get_config_params()

                                                          # Initialize variables
        self.unretract_length = (self.retract_length + \
            self.unretract_extra_length)
        self.currentPos = []
        self.currentZ = 0.0
        self.z_hop_Z = 0.0                           # Z coordinate of zhop move
        self.safe_z_hop_height = self.z_hop_height #Zhop preventing out-of-range

        self.is_retracted = False                           # Retract state flag
        self.ramp_move = False                                  # Ramp move flag
        self.vsdcard_paused = False                         # VSDCard pause flag
        self.G1_toggle_state = False                      # G1 toggle state flag
        self.z_coord_check = False         # Z_hop move check only for cartesian
        self.stored_set_retraction_gcmds = []  # List for delayed SET_RETRACTION
        self.acc_vel_state = []                # List for accel and vel settings

        # Limit use of zhop only to cartesians
        if self.config_ref.has_section('stepper_z'):
            zconfig = config.getsection('stepper_z')
            self.max_z = zconfig.getfloat('position_max')
            self.z_coord_check = True

        printer_config = config.getsection('printer')
        self.max_vel = printer_config.getfloat('max_velocity')
        self.max_acc = printer_config.getfloat('max_accel')
        self.max_acc_to_decel = printer_config.getfloat('max_accel_to_decel', \
            self.max_acc/2.)
        self.max_sqv = printer_config.getfloat('square_corner_velocity', 5.)

        self.printer.register_event_handler("klippy:ready", self._handle_ready)

    ########################## Command to set the firmware retraction parameters
    cmd_SET_RETRACTION_help = ('Set firmware retraction parameters')

    def cmd_SET_RETRACTION(self, gcmd):
        if not self.is_retracted:        # Only execute command when unretracted
            self._execute_set_retraction(gcmd)     # Execute command immediately
        else:
            # Queue command for execution when G11 is called.
            # If CLEAR_RETRACTION is called, stored set_retraction commands are
            # purged for safety.
            self.stored_set_retraction_gcmds.append(gcmd)
            if self.verbose: gcmd.respond_info('Printer in retract state. \
                                               SET_RETRACTION will be executed\
                                                 once unretracted!')

    ############### Command to report the current firmware retraction parameters
    cmd_GET_RETRACTION_help = ('Report firmware retraction paramters')

    def cmd_GET_RETRACTION(self, gcmd):
        gcmd.respond_info('RETRACT_LENGTH=%.5f RETRACT_SPEED=%.5f '
                          'UNRETRACT_EXTRA_LENGTH=%.5f UNRETRACT_SPEED=%.5f '
                          ' Z_HOP_HEIGHT=%.5f Z_HOP_STYLE=%s '
                          ' RETRACTED=%s RAMP_MOVE=%s'
                          % (self.retract_length, self.retract_speed,
                             self.unretract_extra_length, self.unretract_speed,
                             self.z_hop_height, self.z_hop_style,
                             self.is_retracted, self.ramp_move ))

        if self.stored_set_retraction_gcmds:#List queued SET_RETRACTION commands
            for i, stored_gcmd in reversed(list(enumerate(\
                    self.stored_set_retraction_gcmds))):
                params = ' '.join('{} = {}'.format(k, v) for k, v in \
                    stored_gcmd.get_command_parameters().items())
                gcmd.respond_info('Stored command #%d: SET_RETRACTION %s' % \
                    (i + 1, params))                    # Format stored commands

    ##### Command to clear FW retraction (add to CANCEL macros at the beginning)
    cmd_CLEAR_RETRACTION_help = ('Clear retraction state without retract move \
        or zhop, if enabled')

    def cmd_CLEAR_RETRACTION(self, gcmd):
        if self.is_retracted:
            self._execute_clear_retraction()
            if self.verbose: gcmd.respond_info('Retraction, including \
                SET_RETRACTION command queue, was cleared and reset to config \
                values. zhop is undone on next move.')
        else:
            if self.verbose: gcmd.respond_info('Printer is not retracted. \
                Command ignored!')

    ########################### Gcode Command G10 to perform firmware retraction
    def cmd_G10(self, gcmd):
        retract_gcode = ""                                # Reset retract string
        homing_status = self._get_homing_status()          # Check homing status
        if 'xyz' not in homing_status: # If printer is not homed, ignore command
            if self.verbose: gcmd.respond_info('Printer is not homed. \
                Command ignored!')
        elif not self.extruder.heater.can_extrude:   # Check extruder min. temp.
            if self.verbose: gcmd.respond_info('Extruder temperature too low. \
                Command ignored!')
        elif self.retract_length == 0.0:        # Check if FW retraction enabled
            if self.verbose: gcmd.respond_info('Retraction length cero. \
                Firmware retraction disabled. Command ignored!')
        elif not self.is_retracted:  # If filament isn't retracted, build G-Code
            self._save_acc_vel_state()     # Save current accel and vel settings
            retract_gcode = (
                "SAVE_GCODE_STATE NAME=_retract_state\n"
                "G91\n"
                "M204 S{:.5f}\n"                # Set max accel for retract move
                "G1 E-{:.5f} F{}\n"          # Retract filament at retract speed
                "G90\n"                 # Switch to absolute mode (just in case)
            ).format(self.max_acc, self.retract_length,\
                int(self.retract_speed * 60))

            # Incl move command if z_hop_height>0 depending on z_hop_style
            if self.z_hop_height > 0.0:
                # Set safe zhop parameters to prevent out-of-range moves when
                # canceling or finishing print while retracted - Only Cartesian!
                self._set_safe_zhop_retract_params()
                retract_gcode += (
                    "SET_VELOCITY_LIMIT VELOCITY={:.5f} \
                        SQUARE_CORNER_VELOCITY={:.5f}\n"
                    ).format(self.max_vel, self.max_sqv)     # Set max vel limit

                if self.z_hop_style == 'helix':
                    # --> ADD CODE HERE TO GET NEXT COORD TO CALC HELIX CENTER
                    retract_gcode += (
                        "G17\n" # Set XY plane for full arc (incl z for a helix)
                        "G2 Z{:.5f} I-1.22 J0.0 F{}\n"
                    ).format(self.z_hop_Z, int(RETRACTION_MOVE_SPEED_FRACTION *\
                        self.max_vel * 60))      # Set 80% of max. vel for zhop.
                                # Z speed limit will be enforced by the firmware
                # Standard vertical move with enabled z_hop_height
                elif self.z_hop_style == 'standard':
                    retract_gcode += (
                        "G1 Z{:.5f} F{}\n"
                    ).format(self.z_hop_Z, int(RETRACTION_MOVE_SPEED_FRACTION *\
                        self.max_vel * 60))
                # Ramp move: z_hop during 1st move after retract
                elif self.z_hop_style == 'ramp':
                    self.ramp_move = True #Set flag to ramp move in next G1 move

            retract_gcode += (
                # Restore previous accel and speed value limits
                "SET_VELOCITY_LIMIT VELOCITY={:.5f} ACCEL={:.5f} \
                    ACCEL_TO_DECEL={:.5f} SQUARE_CORNER_VELOCITY={:.5f}\n"
                # Restore gcode state, velocity and acceleration values
                "RESTORE_GCODE_STATE NAME=_retract_state"
                ).format(self.acc_vel_state[0], self.acc_vel_state[1], \
                    self.acc_vel_state[2], self.acc_vel_state[3])

            self.gcode.run_script_from_command(retract_gcode)
            self.is_retracted = True        # Set the flag to filament retracted
            self.acc_vel_state =[]              # Reset acc and vel setting list

            if self.z_hop_height > 0.0:
                # Swap original G1 handlers if z_hop enabled to offset following
                # moves in eiter absolute or relative mode
                self._unregister_G1()
                self.G1_toggle_state = True #Prevent repeat unregister with flag
        else:
            if self.verbose: gcmd.respond_info('Printer is already in retract \
                state. Command ignored!')

    ######################### GCode Command G11 to perform filament unretraction
    def cmd_G11(self, gcmd):
        unretract_gcode = ""                            # Reset unretract string
        if self.retract_length == 0.0:          # Check if FW retraction enabled
            if self.verbose: gcmd.respond_info('Retraction length cero. \
                Firmware retraction disabled. Command ignored!')
        elif self.is_retracted:             # Check if the filament is retracted
            # Check if extruder is above min. temperature. If not, don't retract
            # but clear_retraction to prevent damage to extruder and/or filament
            if not self.extruder.heater.can_extrude:
                self._execute_clear_retraction()
                if self.verbose: gcmd.respond_info('Extruder temperature low. \
                    Retraction cleared without retract move. zhop will be \
                    undone on next toolhead move.')
            else:
                if self.z_hop_height > 0.0:    # Restore G1 handlers if z_hop on
                    self._re_register_G1()
                    self.G1_toggle_state = False    # Prevent repeat re-register

                self._save_acc_vel_state()         # Save accel and vel settings
                unretract_gcode = (
                    "SAVE_GCODE_STATE NAME=_unretract_state\n"
                    "M204 S{:.5f}\n"          # Set max accel for unretract move
                    "G91\n"
                    ).format(self.max_acc)

                # Incl move command only if z_hop enabled and ramp move was used
                # This is a move in relative mode, which was already set
                if self.z_hop_height > 0.0 and self.ramp_move:
                    self.ramp_move = False  # Reset ramp flag if not used before
                elif self.z_hop_height > 0.0:
                    # Set maximum unretract z move to 0.0 coordinate
                    self._set_safe_zhop_unretract_params()
                    unretract_gcode += (
                        "SET_VELOCITY_LIMIT VELOCITY={:.5f} \
                            SQUARE_CORNER_VELOCITY={:.5f}\n"       # Set max vel
                        "G1 Z-{:.5f} F{}\n"
                    ).format(self.max_vel, self.max_sqv, \
                        self.safe_z_hop_height, \
                        int(RETRACTION_MOVE_SPEED_FRACTION * self.max_vel* 60))
                        # If ramp move was used or standard or helix move were
                        # done, un_zhop at 80% of maximum speed (to have a bit
                        # of a safety margin)

                unretract_gcode += (
                    "G1 E{:.5f} F{}\n"                      # Unretract filament
                    "SET_VELOCITY_LIMIT VELOCITY={:.5f} ACCEL={:.5f} \
                        ACCEL_TO_DECEL={:.5f} \
                        SQUARE_CORNER_VELOCITY={:.5f}\n"
                          # Restore previous accel values and speed value limits
                    "RESTORE_GCODE_STATE NAME=_unretract_state"
                ).format(self.unretract_length, int(self.unretract_speed * 60),\
                    self.acc_vel_state[0], self.acc_vel_state[1], \
                    self.acc_vel_state[2], self.acc_vel_state[3])

                self.gcode.run_script_from_command(unretract_gcode)
                self.is_retracted = False # Set the flag to filament unretracted
                self.acc_vel_state =[]          # Reset acc and vel setting list

                # If any SET_RETRACTION commands were stored, execute them now
                if self.stored_set_retraction_gcmds:
                    for stored_gcmd in self.stored_set_retraction_gcmds:
                        self._execute_set_retraction(stored_gcmd)
                    self.stored_set_retraction_gcmds=[] #Reset stored comms list
        else:
            if self.verbose: gcmd.respond_info('Printer is not retracted. \
                Command ignored!')

    ##################### Helper method to get retraction parameters from config
    def _get_config_params(self):
        self.retract_length = self.config_ref.getfloat(\
            'retract_length', 0., minval=0.)
        self.retract_speed = self.config_ref.getfloat(\
            'retract_speed', 20., minval=1.)
        self.unretract_extra_length = self.config_ref.getfloat(\
            'unretract_extra_length', 0., minval=-1.)
        self.unretract_speed = self.config_ref.getfloat(\
            'unretract_speed', 10., minval=1.)
        # Zero min. and standard val to ensure compatibility with macros
        self.z_hop_height = self.config_ref.getfloat(\
            'z_hop_height', 0., minval=0.)
        self.z_hop_style = self.config_ref.get(\
            'z_hop_style', default='standard').strip().lower()
        self._check_z_hop_style()   # Safe guard that zhop style is properly set
        # verbose to enable/disable user messages
        self.verbose = self.config_ref.getboolean('verbose', False)
        # Control retraction parameter behaviour when retraction is cleared.
        # Default is to reset retraction parameters to config values.
        self.config_params_on_clear = self.config_ref.getboolean(\
            'config_params_on_clear', True)

    ################### Helper to check that z_hop_style was input and is valid.
    def _check_z_hop_style(self):
        if self.z_hop_style not in self.valid_z_hop_styles:
            self.z_hop_style = 'standard'
            logging.warning('The provided z_hop_style value is invalid. Using \
                "standard" as default.')

    ######## Helper method to register commands and instantiate required objects
    def _handle_ready(self):
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_move = self.printer.lookup_object('gcode_move')
        self.toolhead = self.printer.lookup_object('toolhead')
        self.extruder = self.printer.lookup_object('extruder')

        # Register new G-code commands for setting/retrieving retraction
        # parameters and clearing retraction
        self.gcode.register_command('SET_RETRACTION', self.cmd_SET_RETRACTION, \
            desc=self.cmd_SET_RETRACTION_help)
        self.gcode.register_command('GET_RETRACTION', self.cmd_GET_RETRACTION, \
            desc=self.cmd_GET_RETRACTION_help)
        self.gcode.register_command('CLEAR_RETRACTION', \
            self.cmd_CLEAR_RETRACTION, desc=self.cmd_CLEAR_RETRACTION_help)

        # Register new G-code commands for firmware retraction/unretraction
        self.gcode.register_command('G10', self.cmd_G10)
        self.gcode.register_command('G11', self.cmd_G11)
        # Add M103 and M101 aliases for G10 and G11
        self.gcode.register_command('M103', self.cmd_G10)
        self.gcode.register_command('M101', self.cmd_G11)

        # Register Events to clear retraction when a new print is started, an
        # ongoing print is canceled or a print is finished
        # Consider two operational modes: Printing from Virtual SD Card or via
        # GCode streaming
        ################ GCode streaming mode (most commonly done via OctoPrint)
        # Print is started:  Most start gcodes include a G28 command to home all
        # axes, which is generally NOT repeated during printing. Using homing
        # move as an indicator to evaluate if a printjob has started. G28
        # requirement added in function description. Bz using homing_move rather
        # than home_rails event, bed mesh calibration is also detected and
        # and triggers clearing retract state.
        #
        # Print is canceled: On cancel, OctoPrint automatically disables
        # stepper, which allows identifying a canceled print.
        #
        # Print finishes: Most end gcodes disable steppers once a print is
        # finished. This allows identifying a finished print. M84 requirement
        # for end gcode was added in function description. Steppers are also
        # disabled on host and firmware restart, thus triggering clear
        # retraction as well. Shutdown requires host and/or firmware restart,
        # thus also triggerung clear retraction.
        self.printer.register_event_handler("homing:homing_move_begin", \
            self._evaluate_retraction)
        self.printer.register_event_handler("stepper_enable:motor_off", \
            self._evaluate_retraction)


        ################# Virtual SD card mode (Default for Mainsail, Fluidd and
        # DWC2-to-Klipper. Also possible via OctoPrint) Printing via virtual SD
        # Card is recommended as start, cancel and finish print can be detected
        # more reliably! If Virtual SD Card is avilable, additional events can
        # be used to track the state of the printer.
        if self.config_ref.has_section('virtual_sdcard'):
            # Get ref to VSD Card object
            self.vsdcard = self.printer.lookup_object('virtual_sdcard')
            # Print is started: If started using the SDCARD_PRINT_FILE command,
            # any previously loaded file is reset first. Hence, the rest_file
            # event indicates a starting print. If instead a file is loaded
            # using M23 and a print is started using M24, the M23 also sends the
            # reset_file event. The reset_file event is tracked as means of
            # redundancy. However, if the print is repeated using the M24
            # command and there is no disable motor or homing command in the
            # end-/start-gcode, the newly started print from virtual SD Card
            # will pass unnoticed. Therefore, a print start event was included
            # in print_stats, being automatically available if the VSD Card
            # module is loaded.
            #
            # Print finishes: A print complete event was included in print_stat
            # to identify a completed print. If retraction is active at the end
            # of the print, and steppers are not disabled or a homing command is
            # not issued shortly after, this event ensures that retraction is
            # cleared anyways.
            #
            # Print is canceled: If a VSD Card print is cancelled and no
            # end_print gcode which disables motors is in place, the cancel
            # event ensures that retraction is cleared for the next print.
            #
            # Print is paused: This is a tricky failure case. The pause itself
            # is not the issue. On resume, start_printing event is triggered,
            # thus clearing retraction. Hence, the pause event needs to be
            # monitored to prevent a retraction clear when paused. Otherwise,
            # the printer prints in air...
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

    ########### Helper method to reset pause flags and force evaluate retraction
    def _reset_pause_flag(self, *args):
        self.vsdcard_paused = False
        self._evaluate_retraction()

    ############################################ Helper method to set pause flag
    def _set_pause_flag(self, *args):
        self.vsdcard_paused = True

    ################### Helper to set retraction parameters if command is called
    def _execute_set_retraction(self,gcmd):
        self.retract_length = gcmd.get_float('RETRACT_LENGTH', \
            self.retract_length, minval=0.)
        self.retract_speed = gcmd.get_float('RETRACT_SPEED', \
            self.retract_speed, minval=1.)
        self.unretract_extra_length = gcmd.get_float('UNRETRACT_EXTRA_LENGTH', \
            self.unretract_extra_length, minval=0.)
        self.unretract_speed = gcmd.get_float('UNRETRACT_SPEED', \
            self.unretract_speed, minval=1.)
        self.z_hop_height = gcmd.get_float('Z_HOP_HEIGHT', self.z_hop_height, \
            minval=0.)      # z_hop_height with 0mm min. to prevent nozzle crash
        self.z_hop_style = gcmd.get('Z_HOP_STYLE', \
            self.z_hop_style).strip().lower()
        self._check_z_hop_style()
        self.unretract_length = (self.retract_length + \
            self.unretract_extra_length)

    ################################################# Helper to clear retraction
    def _execute_clear_retraction(self):
        if self.z_hop_height > 0.0:
            # Re-establish regular G1 command if zhop enabled.
            # zhop will be reversed on next move with z coordinate
            # Note that disabling zhop while retracted id not possible as the
            # SET_RETRACTION command will not execute while retracted.
            self._re_register_G1()
            self.G1_toggle_state = False            # Prevent repeat re-register
        self.is_retracted = False     # Reset retract flag to enable G10 command
        self.ramp_move = False                            # Reset ramp move flag
        self.stored_set_retraction_gcmds = []       # Reset list of stored comms
        # Reset retraction parameters to config values.
        # Can be disabled in config but not in set_retraction
        if self.config_params_on_clear:
            self._get_config_params()

    ################################################ Helper to get homing status
    def _get_homing_status(self):
        curtime = self.printer.get_reactor().monotonic() # Check if Z axis homed
        kin_status = self.toolhead.get_kinematics().get_status(curtime)
        return kin_status['homed_axes']

    ######## Helper to save current acceleration and velocity values (values are
    # covered by gcode state, accelerations are not)
    def _save_acc_vel_state(self):
        self.acc_vel_state = [
            self.toolhead.max_velocity,
            self.toolhead.max_accel,
            self.toolhead.max_accel_to_decel,
            self.toolhead.square_corner_velocity]

    ### Helper to evaluate max. possible zhop height to stay within build volume
    def _set_safe_zhop_retract_params(self):
        self.safe_z_hop_height = self.z_hop_height

        # Check z move - only for cartesians
        if self.z_coord_check:
            self.currentPos = self._get_gcode_pos()
            self.currentZ = self.currentPos[2]
            # Set safe z_hop height to prevent out-of-range moves.
            # Variables is used in zhop-G1 command
            if self.currentZ + self.z_hop_height > self.max_z:
                self.safe_z_hop_height = self.max_z - self.currentZ

        self.z_hop_Z = self.currentZ + self.safe_z_hop_height

    ####### Helper to evaluate max. possible zhop height to prevent nozzle crash
    def _set_safe_zhop_unretract_params(self):
        self.currentPos = self._get_gcode_pos()
        self.currentZ = self.currentPos[2]
        # Set safe z_hop height to prevent nozzle crashes
        # Variables is used in G11 command
        if self.currentZ - self.safe_z_hop_height < 0.0:
            self.safe_z_hop_height = -1.0 * self.currentZ

    ####################################### Helper to get current gcode position
    def _get_gcode_pos(self):
        # Get current gcode position for z_hop move if enabled
        gcodestatus = self.gcode_move.get_status()
        currentPos = gcodestatus['gcode_position']
        return currentPos

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

         # Check if ramp flag set and thus move is a ramp move
        if self.ramp_move:
            self.ramp_move = False                             # Reset ramp flag
            if not 'Z' in params:
                # If the first move after retract does not have a Z parameter,
                # add parameter to force ramp move
                if is_relative:
                    # In relative mode
                    params['Z'] = str(self.safe_z_hop_height)
                else:
                    # In absolute mode
                    params['Z'] = str(self.z_hop_Z)
            else:
                # If the first move after retract does have a Z parameter,
                # adjust the Z value to account for the additonal Z-hop offset
                # of the ramp move (works the same in rel and abs mode)
                params['Z'] = str(float(params['Z']) + self.safe_z_hop_height)
        elif 'Z' in params:
            if not is_relative:
                # In absolute mode, adjust the Z value to account for the Z-hop
                # offset after retract and ramp move (if applicable)
                params['Z'] = str(float(params['Z']) + self.safe_z_hop_height)
                # Note, in relative mode, don't adjust z params given that the
                # zhop offset was already considered in a previous move

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

    ################## Helper method to return the current retraction parameters
    def get_status(self, eventtime):
        return {
            'retract_length': self.retract_length,
            'retract_speed': self.retract_speed,
            'unretract_extra_length': self.unretract_extra_length,
            'unretract_speed': self.unretract_speed,
            'z_hop_height': self.z_hop_height,
            'safe_z_hop_height': self.safe_z_hop_height,
            'z_hop_style': self.z_hop_style,
            'unretract_length': self.unretract_length,
            'retract_state': self.is_retracted
        }

###### Function to load the FirmwareRetraction class from the configuration file
def load_config(config):
    return FirmwareRetraction(config)
