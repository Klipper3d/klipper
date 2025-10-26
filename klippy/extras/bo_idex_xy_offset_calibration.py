from __future__ import annotations


class XYOffsetCalibrationTool:
    """Tool that helps the calibration of the XY offsets for Idex printers."""

    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()

        # self.name = config.get_name().split()[-1]

        # * Register event handlers
        self.printer.register_event_handler("klippy:connect", self.handle_connect)
        self.printer.register_event_handler("klippy:ready", self.handle_ready)

        self.gcode = self.printer.lookup_object("gcode")
        self.enable = False
        self.last_state = False
        
        # * Control variables
        self.prime: bool = False
        self.line_number: int = 0
        self.line_spacing: float = 0.0
        self.line_height: float = 0.0
        self.line_top: float = 0.0
        self.initial_x_h: float = 0.0
        self.initial_y_h: float = 0.0
        self.initial_x_h_2: float = 0.0
        self.initial_y_h_2: float = 0.0
        self.initial_x_v: float = 0.0
        self.initial_y_v: float = 0.0
        self.initial_x_v_2: float = 0.0
        self.initial_y_v_2: float = 0.0
        self.extrude_per_mm: float = 0.0

        self.horizontal_line_spacing: float = 0.0
        self.horizontal_line_height: float = 0.0
        self.horizontal_line_top: float = 0.0

        self.vertical_line_spacing: float = 0.0
        self.vertical_line_height: float = 0.0
        self.vertical_line_top: float = 0.0

        self.tool_2_x_offset: float = 0.0
        self.tool_2_y_offset: float = 0.0

        self.filament_temperature: int = 190
        # * Register gcode commands
        self.gcode.register_command(
            "XYCALIBRATION",
            self.cmd_XYCALIBRATION,
            desc=self.cmd_XYCALIBRATION_help,
        )
      
        # self.gcode.register_mux_command(
        #     "XYNOZZLEPRIME",
        #     "XYOffsetCalibrationTool",
        #     self.name,
        #     self.cmd_XYNOZZLEPRIME,
        #     self.cmd_XYNOZZLEPRIME_help,
        # )

    def handle_connect(self):
        """Event handler method for when klippy connects"""
        self.toolhead = self.printer.lookup_object("toolhead")
        # self.configfile = self.printer.lookup_object("configfile")
        # self.manual_probe = self.printer.lookup_object("manual_probe")
        # self.stepper_enable = self.printer.lookup_object("stepper_enable")
        # self.mcu = self.printer.lookup_object("mcu")
        # printer.register_event_handler("toolhead:set_position",
        #                                self.reset_last_position)
        # printer.register_event_handler("toolhead:manual_move",
        #                                self.reset_last_position)
        # printer.register_event_handler("gcode:command_error",
        #                                self.reset_last_position)
        # printer.register_event_handler("extruder:activate_extruder",
        #                                self._handle_activate_extruder)
        # printer.register_event_handler("homing:home_rails_end",
        #                                self._handle_home_rails_end)
        # printer.register_event_handler("gcode:request_restart", self._handle_request_restart)

        # self.printer.send_event("stepper_enable:motor_off", print_time)

        # gcmd.get_int("ENABLE", 1)

        # cur_time = self.printer.get_reactor().monotonic()
        # kin_status = toolhead.get_kinematics().get_status(curtime)

        # toolhead.set_position(pos, homing_axes=[2])
        # toolhead.manual_move([None, None, self.z_hop], self.z_hop_speed)
        # toolhead.get_kinematics().note_z_homed()
        # need_x, need_y, need_z = [gcmd.get(axis, None) is not None for axis in "XYZ"]
        # g28_cmd = self.gcode.create_gcode_command("G28", "G28", new_params)
        # self.prev_G28(g28_cmd)

        # toolhead.get_position()

    def handle_ready(self):
        """Event handler method for when klippy is enabled"""
        self.toolhead = self.printer.lookup_object("toolhead")
        self.kin = self.printer.lookup_object("toolhead").get_kinematics()
        

        
        # self.enable = True
        # self.reactor.update_timer(
        #     self.update_direction_timer, self.reactor.Never
        # )
        # self.enable = False

    cmd_XYCALIBRATION_help = "Handles the calibration of xy axes on Idex printers."
    def cmd_XYCALIBRATION(self, gcmd):
        # * First home axes
        self.printer.send_event(
            "stepper_enable:motor_on", self.printer.get_reactor().monotonic()
        )
        gcmd.respond_info("XY calibration tool for Idex printers loaded.")
        gcmd.respond_info(f"{self.toolhead.get_position()}")

        gcmd.respond_info(f"{self.get_homed_axes()}")
        if self.get_homed_axes() is None or self.get_homed_axes() != "xy": 
            gcmd.respond_info("Must home axes before running the XY offset calibration for Idex printers.")
            # return # I cannot continue if there is no homing 
        
        
        
        _dual_carriage_module = self.kin.dc_module

        if _dual_carriage_module is None: 
            gcmd.respond_info("No dual carriage config section defined. Cannot run tool")
            return 
        
        
        # Set extruder 0 
        _dual_carriage_module.activate_dc_mode(
            index= 0, 
            mode= "PRIMARY"         #This is the default
        )
        
        _cur_extruder = self.toolhead.get_extruder()
        _cur_extruder_name = _cur_extruder.get_name()
        
        
        _available_printer_objects = self.printer.objects
        gcmd.respond_info(f"Available Objects : {_available_printer_objects}")
        gcmd.respond_info(f"Current extruder name : {_cur_extruder_name,}")
        # Check that the kin type is cartesian 
        
        # * Maybe home axes
        # _homexy = "G28"
        # # * Force usage of toolhead 0 T0
        # * Get extruder names, and activate the main toolhead 
        # gcmd.run_script_from_command("G28")

        # # * Horizontal Print in tool 0
        _horizontal_print_tool_0 = [
            "G90",
            "M83",
            f"G1 X{self.initial_x_h_2} Y{self.initial_y_h - 2} E{-0.1} F{12000}",  # Brim
            "G1 Z0.2 E0.5 F2000",  # Brim
            f"G1 X{self.initial_x_h} E{(self.initial_x_h_2 - self.initial_x_h) * self.extrude_per_mm} F3000",  # U turn of the brim
            f"G1 Y{self.initial_y_h} E{2 * self.extrude_per_mm}",  # Start of the calibration
            "G91",
        ]

        # # self.print_horizontal_line
        # # Raise the nozze G1 Z2 E-0.1

        # # * Vertical Print in tool 0
        # _horizontal_print_tool_0 = [
        #     "G90",
        #     "M83 ",
        #     f"G1 X{self.initial_x_v - 2} Y{self.initial_y_v} E{-0.1} F{12000}",  # Brim
        #     "G1 Z0.2 E0.5 F2000",  # Brim
        #     f"G1 X{self.initial_y_v} E{(self.initial_y_h_2 - self.initial_x_h) * self.extrude_per_mm} F3000",  # U turn of the brim
        #     f"G1 Y{self.initial_y_h} E{2 * self.extrude_per_mm}",  # Start of the calibration
        #     "G91",
        # ]

        # # self.print_vertical_line

        # # Park the nozzle
        # # G91
        # # park_nozzle

        # # Change to tool 1
        # # prime
        # # * Horizontal print in tool 1

        # _horizontal_print_tool_1 = [
        #     "G90",
        #     "M83",
        #     f"G1 X{self.initial_x_h} Y{self.initial_y_h_2 - 2 + 2} E{-0.1} F{12000}",  # Brim
        #     "G1 Z0.2 E0.5 F2000",  # Lower nozzle for brim
        #     f"G1 X{self.initial_x_h_2} E{(self.initial_x_h_2 - self.initial_x_h) * self.extrude_per_mm} F3000",  # U turn of the brim
        #     f"G1 Y{self.initial_y_h_2} E{2 * self.extrude_per_mm}",  # Start of the calibration
        #     "G91",
        # ]
        # # self.print_horizontal_line

        # # Raise nozzle G1 Z2 E-0.1

        # # * Vertical Print in tool 1
        # _horizontal_print_tool_0 = [
        #     "G90",
        #     "M83 ",
        #     f"G1 X{self.initial_x_v_2 + 2} Y{self.initial_y_v} E{-0.1} F{12000}",  # Brim
        #     "G1 Z0.2 E0.5 F2000",  # Brim
        #     f"G1 X{self.initial_y_v_2} E{(self.initial_y_h_2 - self.initial_x_h) * self.extrude_per_mm} F3000",  # U turn of the brim
        #     f"G1 Y{self.initial_x_v_2} E{2 * self.extrude_per_mm}",  # Start of the calibration
        #     "G91",
        # ]
        # # self.print_vertical_line

        # # * Park the nozzle
        # # G90
        # # G1 Y150                       # So the user can see the result and select the line

        # # Change to tool 0
        # # T0

        # # Permit the user to choose the line
        # pass

    # cmd_OFFSETCALIBRATION_helpe = (
    #     "calls a ui to select the ranges or something i don't know"
    # )

    # def cmd_OFFSETCALIBRATION(self, gcmd):
    #     pass

    # cmd_XYNOZZLEPRIME_help = "Primes the nozzle that it's going to be used"

    # def cmd_XYNOZZLEPRIME(self, gcmd):
    #     commands = [
    #         f"M109 S{self.filament_temperature}",
    #         "G92 E0",  # Specify that Extruder is at position 0
    #         "M83",  # Relative coordinates
    #         "G1 E50 F300",  # Prime the nozzle
    #         "G92 E0",  # Specify that Extruder is at position 0
    #     ]
    #     for command_line in commands:
    #         gcmd.run_script_as_command(command_line)

    def get_status(self, eventtime):
        return {"last_state": self.last_state, "enabled": self.enable}

    ################################################################################
    # Helper function that are useful for performing the XY calibration
    ################################################################################
        

    def print_horizontal_line(self, gcmd, callback):
        gcode_lines = [
            f"G1 X{self.horizontal_line_spacing} E{ abs(self.horizontal_line_spacing * self.extrude_per_mm)}",
            f"G1 Y{self.horizontal_line_height} E{abs(self.horizontal_line_height * self.extrude_per_mm)}",
            f"G1 X{self.horizontal_line_top} E{ abs(self.horizontal_line_top * self.extrude_per_mm)}",
            f"G1 Y{int(self.horizontal_line_height) * -1} E{abs(self.horizontal_line_height * self.extrude_per_mm)}",
        ]
        for line in range(self.line_number + 1):
            gcmd.run_command(
                next(iter(gcode_lines))
            )  # Something like this to run all the lines

    def print_vertical_lines(self, gcmd, callback):
        gcode_lines = [
            f"G1 Y{self.vertical_line_spacing} E{abs(self.vertical_line_spacing * self.extrude_per_mm)}",
            f"G1 X{self.vertical_line_height} E{abs(self.vertical_line_height * self.extrude_per_mm)}",
            f"G1 Y{self.vertical_line_top} E{abs(self.vertical_line_top * self.extrude_per_mm)}",
            f"G1 X{int(self.vertical_line_height * -1)} E{abs(self.vertical_line_height * self.extrude_per_mm)}",
        ]
        for line in range(self.line_number + 1):
            gcmd.run_command(
                next(iter(gcode_lines))
            )  # Something like this to run all the lines

    def update_offsets(self):
        pass
    
    def get_homed_axes(self) -> str | None: 
        cur_time = self.reactor.monotonic()
        _kin_status = self.kin.get_status(cur_time)

        if not isinstance(_kin_status, dict): 
            return None

        if "homed_axes" in _kin_status.keys(): 
            return _kin_status["homed_axes"]

        return None
    
    # gcmd  respond_info              #Displays stuff on the console
    # gcode run_script                #Don't know what this one does Same as run_script_from_command but with mutex
    # gcode run_script_from_command   #Runs a command from a string
    # toolhead move                   #Moves the toolhead
    # toohead wait_moves()            #Waits for the toolhead to finish its movements

    # When i have the tool head i can use the move command move(newpos, speed)
    # There is also on toolhead a method called check_busy
    # also a get_kinematics
    #


def load_config(config):
    return XYOffsetCalibrationTool(config)


# def load_config_prefix(config):
#     return XYOffsetCalibrationTool(config)



""" 
From the kinmatics i can run 

get_steppers()
calc_position(self, stepper_positions)
set_position(self, newpos, homign_axes)
note_z_not_homed(self)
home(homign_state)
_motor_off(slef, pritne_time)
_check_positions(slef, move)
check_move(self, move)
get_status


variables: 
self.rails
max_velocity, max_accel
max_z_velocity
max_z_accel
limits
ranges
axes_min
axes_max


From the main mcu class


canbus_uuid
setup_pin
create_oid(self)
get_printer
get_name
register_response(self, cb, msg, oid=None)
get_query_slot(self, oid)
seconds_to_clock(self, time)
get_max_stepper_error(self)
alloc_command_queue(self)

lookup_command(self, msgformat, cp=None)
lookup_query_command(sef, msgformat, respformat, oid=None, cp=None, is_async=None)
estimate_print_time
get_contatns()
get_enumerations()
get_constants_float()
register_stepqueue(self, stepqueue)
request_move_queue_slot(self)
register_flush_callback(self, callback)
flush_move(self, printe_time, clear_history_time)
check_active()
is_fileoutput(self)
is_shutdown()
stats()


can also go get the extruder object so i can run some specific methods 







No final quando tiver-mos os valores, colocamos um gcode offset na segunda cabeça com o valor obtido
cada vez que cada cabeca e activada colocamos um offse, na cabela um e 0,0 e a segunda cabeca e o valor obtido 
"""