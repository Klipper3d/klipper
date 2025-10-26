import pandas as pd 
import logging 
import os 
import stepper

class Repetability: 
    DATAFRAME_SETUP = [
        "timestamp", 
        "iteration", 
        "motion_report_live_position", 
        "motion_report_velocity", 
        "gcode_move_absolute_coordinates", 
        "gcode_move_homing_origin", 
        "gcode_move_position", 
        "toolhead_position", 
        "kin_homed_axes",
        "kin_axis_minimum", 
        "kin_axis_maximum", 
        "limits",
        "kin_pos"
    ]
    def __init__(self, config): 
        
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.toolhead = None
        self.gcode = self.printer.lookup_object("gcode")

        self.start_position= config.getfloatlist("start_position",None, count=2)
        self.end_position= config.getfloatlist("end_position",None, count=2)
        self.bed_boundary = config.getboolean("bed_boundary", False)
        
        self.movement = config.getboolean("movement", False)
        self.z_raise = config.getfloat("z_raise")
        self.travel_speed = config.getfloat("travel_speed", minval=50.0, maxval=500.0)
        self.iterations = config.getint("iterations", minval=1, maxval=1000)
        self.save_path = config.get("save_path")
        
        self.gcode.register_command(
            "TEST_ENDSTOP", 
            self.cmd_TEST_ENDSTOP,
            "Tests an z enstop"
        )
        
        self.data_save: pd.DataFrame = pd.DataFrame(columns=self.DATAFRAME_SETUP)

        self.printer.register_event_handler("klippy:connect", self.handle_connect)
    

    def cmd_TEST_ENDSTOP(self, gcmd): 
        if self.toolhead is None: 
            return 
        if self.bed_boundary: 
            if self.printer.lookup_object("bed_custom_bound") is not None: 
                self.bed_boundary_object = self.printer.lookup_object("bed_custom_bound")
                
        for iteration in range(0, self.iterations): 
            if self.bed_boundary:
                self.bed_boundary_object.restore_default_boundary()
            
            gcmd.respond_info(f"[TEST REPEATABILITY] Iteration: {iteration}")    
            
            # self.move(x=self.start_position[0],y= self.start_position[1])

            self.gcode.run_script_from_command("G28 \nM400")

            self.save_info(iteration)

        
        self.reactor.register_callback(self.save_to_file)
    
    def handle_connect(self): 
        self.toolhead = self.printer.lookup_object("toolhead")

        
    
    def move(self, x, y): 
        if self.toolhead is None: 
            return 
        self.toolhead.manual_move([x, y], self.travel_speed)

    def get_kinematics_pos(self): 
        self.toolhead.flush_step_generation()
        kin = self.toolhead.flush_step_generation()
        kin_spos = {s.get_name(): s.get_commanded_position() for s in kin.get_steppers()}
        kin_pos = kin.calc_position(kin_spos)
        return kin_pos 

    
    def save_info(self, iteration): 
        eventtime = self.reactor.monotonic()
        _gcode_move_status = self.get_gcode_move_status(eventtime)
        _toolhead_status = self.get_toolhead_status(eventtime)
        _extruder_status = self.get_extruder_status(eventtime)
        _motion_report_status = self.get_motion_report_status(eventtime)
        _kin = self.toolhead.get_kinematics()
        _kin_status = _kin.get_status(eventtime)
        
        _row_data = [
            self.reactor.monotonic(), 
            iteration, 
            _motion_report_status["live_position"], 
            _motion_report_status["live_velocity"], 
            _gcode_move_status["absolute_coordinates"], 
            _gcode_move_status["homing_origin"], 
            _gcode_move_status["position"], 
            _toolhead_status["position"], 
            _kin_status["homed_axes"],
            _kin_status["axis_minimum"], 
            _kin_status["axis_maximum"],
            _kin.limits, 
            self.get_kinematics_pos()
        ]
    
        _new_row_entry: pd.DataFrame = pd.DataFrame(
            [_row_data], columns=self.data_save.columns
        )

        # * Update the csv file.
        self.data_save = pd.concat(
            [
                self.data_save,
                _new_row_entry,
            ],
            ignore_index=True,
        )

        return

    def save_to_file(self, eventtime): 
        """Saves the print data dataframe into a csv file

        Returns:
            bool: True if saved, False if an exception occurred
        """
        try:
            self.data_save.to_csv(
                os.path.join(self.save_path, "repetability_test.csv"),
                index=False,
            )
            return True
        except Exception as e:
            logging.error(
                f"Exception occurred when saving print data to csv file. MSG= {e}"
            )
            # raise EdnaException("Error saving dataframe to directory")
            return False


    
################################################################################
    def get_gcode_move_status(self, eventtime):
        gcode_move = self.printer.lookup_object("gcode_move")
        return gcode_move.get_status(eventtime)

    def get_toolhead_status(self, eventtime):
        toolhead = self.printer.lookup_object("toolhead")
        return toolhead.get_status(eventtime)

    def get_extruder_status(self, eventtime):
        toolhead = self.printer.lookup_object("toolhead")
        extruder = toolhead.get_extruder()
        return extruder.get_status(eventtime)

    def get_motion_report_status(self, eventtime):
        motion_report = self.printer.lookup_object("motion_report")
        return motion_report.get_status(eventtime)

    def get_kin_status(self, eventtime):
        toolhead = self.printer.lookup_object("toolhead")
        kin = toolhead.get_kinematics()
        return kin.get_status(eventtime)
    
    
def load_config(config): 
    return Repetability(config)