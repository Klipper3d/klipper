"""
Important: This file is imported from the DWIN_T5UIC1_LCD
repository available on (https://github.com/odwdinc/DWIN_T5UIC1_LCD)
with no to minimal changes. All credits go to the original author.
"""
import logging
import os, re


class xyze_t:
    x = 0.0
    y = 0.0
    z = 0.0
    e = 0.0
    home_x = False
    home_y = False
    home_z = False

    def homing(self):
        self.home_x = False
        self.home_y = False
        self.home_z = False


class AxisEnum:
    X_AXIS = 0
    A_AXIS = 0
    Y_AXIS = 1
    B_AXIS = 1
    Z_AXIS = 2
    C_AXIS = 2
    E_AXIS = 3
    X_HEAD = 4
    Y_HEAD = 5
    Z_HEAD = 6
    E0_AXIS = 3
    E1_AXIS = 4
    E2_AXIS = 5
    E3_AXIS = 6
    E4_AXIS = 7
    E5_AXIS = 8
    E6_AXIS = 9
    E7_AXIS = 10
    ALL_AXES = 0xFE
    NO_AXIS = 0xFF


class HMI_value_t:
    E_Temp = 0
    Bed_Temp = 0
    Fan_speed = 0
    print_speed = 100
    Max_Feedspeed = 0.0
    Max_Acceleration = 0.0
    Max_Jerk = 0.0
    Max_Step = 0.0
    Move_X_scale = 0.0
    Move_Y_scale = 0.0
    Move_Z_scale = 0.0
    Move_E_scale = 0.0
    offset_value = 0.0
    show_mode = 0  # -1: Temperature control    0: Printing temperature


class HMI_Flag_t:
    language = 0
    pause_flag = False
    pause_action = False
    print_finish = False
    done_confirm_flag = False
    select_flag = False
    home_flag = False
    heat_flag = False  # 0: heating done  1: during heating
    ETempTooLow_flag = False
    leveling_offset_flag = False
    feedspeed_axis = AxisEnum()
    acc_axis = AxisEnum()
    jerk_axis = AxisEnum()
    step_axis = AxisEnum()


class material_preset_t:
    def __init__(self, name, hotend_temp, bed_temp, fan_speed=100):
        self.name = name
        self.hotend_temp = hotend_temp
        self.bed_temp = bed_temp
        self.fan_speed = fan_speed

class PrinterData:
    event_loop = None
    HAS_HOTEND = True
    HOTENDS = 1
    HAS_HEATED_BED = True
    HAS_FAN = False
    HAS_ZOFFSET_ITEM = True
    HAS_ONESTEP_LEVELING = True
    HAS_PREHEAT = True
    HAS_BED_PROBE = True
    PREVENT_COLD_EXTRUSION = True
    EXTRUDE_MINTEMP = 170
    EXTRUDE_MAXLENGTH = 200

    HEATER_0_MAXTEMP = 275
    HEATER_0_MINTEMP = 0
    HOTEND_OVERSHOOT = 15

    MAX_E_TEMP = HEATER_0_MAXTEMP - (HOTEND_OVERSHOOT)
    MIN_E_TEMP = HEATER_0_MINTEMP

    BED_OVERSHOOT = 10
    BED_MAXTEMP = 150
    BED_MINTEMP = 0

    BED_MAX_TARGET = BED_MAXTEMP - (BED_OVERSHOOT)
    MIN_BED_TEMP = BED_MINTEMP

    X_MIN_POS = 0.0
    Y_MIN_POS = 0.0
    Z_MIN_POS = 0.0
    Z_MAX_POS = 200

    Z_PROBE_OFFSET_RANGE_MIN = -20
    Z_PROBE_OFFSET_RANGE_MAX = 20


    BABY_Z_VAR = 0
    feedrate_percentage = 100
    extrusion_multiplier = 100
    fan_speed = 0
    temphot = 0
    tempbed = 0

    HMI_ValueStruct = HMI_value_t()
    HMI_flag = HMI_Flag_t()

    current_position = xyze_t()

    thermalManager = {
        "temp_bed": {"celsius": 20, "target": 120},
        "temp_hotend": [{"celsius": 20, "target": 120}],
        "fan_speed": [100],
    }

    material_preset = [
        material_preset_t("PLA", 200, 60),
        material_preset_t("ABS", 210, 100),
    ]
    subdirs = []
    subdirIndex = 0
    fl = []
    subdirPath = ''
    selectedFile = ''

    metadata = {
        'layer_height': None,
        'estimated_time': None,
        'filament_used': None,
        'thumbnail': None
    }

    MACHINE_SIZE = "220x220x250"
    SHORT_BUILD_VERSION = "1.00"
    CORP_WEBSITE_E = "www.klipper3d.org"

    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.mutex = self.printer.get_reactor().mutex()
        self.name = config.get_name()
        self.reactor = self.printer.get_reactor()
        self._logging = config.getboolean("logging", False)
        self.gcode = self.printer.lookup_object("gcode")
        self.status = None

    def handle_ready(self):
        self.update_variable()
        self.get_additional_values()
 

    def get_additional_values(self):
        toolhead = self.printer.lookup_object(
            "toolhead").get_status(self.reactor.monotonic())
        if toolhead:
            if "position" in toolhead:
                self.current_position.x = toolhead["position"][0]
                self.current_position.y = toolhead["position"][1]
                self.current_position.z = toolhead["position"][2]
                self.current_position.e = toolhead["position"][3]
            if "homed_axes" in toolhead:
                if "x" in toolhead["homed_axes"]:
                    self.current_position.home_x = True
                if "y" in toolhead["homed_axes"]:
                    self.current_position.home_y = True
                if "z" in toolhead["homed_axes"]:
                    self.current_position.home_z = True
            if "axis_maximum" in toolhead:
                volume = toolhead["axis_maximum"]  # [x,y,z,w]
                self.MACHINE_SIZE = "{}x{}x{}".format(
                    int(volume[0]), int(volume[1]), int(volume[2])
                )
                self.X_MAX_POS = int(volume[0])
                self.Y_MAX_POS = int(volume[1])

        configfile = self.printer.lookup_object(
            "configfile").get_status(self.reactor.monotonic())
        if "config" in configfile:
            if "bltouch" in configfile["config"]:
                if "z_offset" in configfile["config"]["bltouch"]:
                    if configfile["config"]["bltouch"]["z_offset"]:
                        self.BABY_Z_VAR = float(
                            configfile["config"]["bltouch"][
                                "z_offset"
                            ]
                        )
    
    def ishomed(self):
        if (
            self.current_position.home_x
            and self.current_position.home_y
            and self.current_position.home_z
        ):
            return True
        else:
            self.get_additional_values()
            return False

    def offset_z(self, new_offset):
        self.log('new z offset:', new_offset)
        self.BABY_Z_VAR = new_offset
        self.sendGCode("SET_GCODE_OFFSET Z_ADJUST=%s MOVE=1" % (new_offset))

    def postREST(self, path, json):
        self.log("postREST called")


    def GetFiles(self):
        sdcard = self.printer.lookup_object('virtual_sdcard')
        files = sdcard.get_file_list(True)
        self.subdirIndex = len(self.subdirPath.split('/')) if self.subdirPath else 0
        self.fl = []
        self.names = []
        self.subdirs = []

        # Find all folders in current subdirectory
        for file, _ in files:
            path = file.split('/')
            if(file.startswith(self.subdirPath)):
                name = path[self.subdirIndex]
                if len(path) > self.subdirIndex + 1:
                    if not name in self.subdirs:
                        self.subdirs.append(name) # add to checked subdirs
                        self.names.append(name) # add only the name
                        self.fl.append(file) # add full filepath

        # Find all files
        for file, _ in files:
            path = file.split('/')
            if file.startswith(self.subdirPath) :
                name = path[self.subdirIndex]
                if len(path) == self.subdirIndex + 1:
                    self.names.append(path[self.subdirIndex])
                    self.fl.append(file)
        
        return self.names
    
    def selectFile(self, index):
        file = self.fl[index]
        if len(file.split('/')) == self.subdirIndex + 1:
            return True
        else:
            if(self.subdirPath):
                currentPath = self.subdirPath.split('/')
            else:
                currentPath = []
            newDir = file.split('/')[self.subdirIndex]
            self.subdirIndex += 1
            currentPath.append(newDir)
            self.subdirPath = '/'.join(currentPath)
            return False

    def fileListBack(self):
        self.subdirIndex -= 1
        path = self.subdirPath.split('/')
        path.pop(-1)
        self.subdirPath = '/'.join(path)

    def update_variable(self):
        gcm = self.printer.lookup_object(
            "gcode_move").get_status(self.reactor.monotonic())
        z_offset = gcm["homing_origin"][2]  # z offset
        extrusionMultiplier = gcm["extrude_factor"] * 100  # flow rate percent
        self.absolute_moves = gcm["absolute_coordinates"]  # absolute or relative
        self.absolute_extrude = gcm["absolute_extrude"]  # absolute or relative
        speed = gcm["speed"]  # current speed in mm/s
        print_speed = gcm["speed_factor"] * 100  # print speed percent
        bed = self.printer.lookup_object(
            "heater_bed").get_status(self.reactor.monotonic())
        extruder = self.printer.lookup_object(
            "extruder").get_status(self.reactor.monotonic())
        fan = self.printer.lookup_object(
            "fan").get_status(self.reactor.monotonic())
        fanSpeed = fan['speed'] * 100
        Update = False
        try:
            if self.thermalManager["temp_bed"]["celsius"] != int(bed["temperature"]):
                self.thermalManager["temp_bed"]["celsius"] = int(bed["temperature"])
                Update = True
            if self.thermalManager["temp_bed"]["target"] != int(bed["target"]):
                self.thermalManager["temp_bed"]["target"] = int(bed["target"])
                Update = True
            if self.thermalManager["temp_hotend"][0]["celsius"] != int(
                extruder["temperature"]
            ):
                self.thermalManager["temp_hotend"][0]["celsius"] = int(
                    extruder["temperature"]
                )
                Update = True
            if self.thermalManager["temp_hotend"][0]["target"] != int(
                extruder["target"]
            ):
                self.thermalManager["temp_hotend"][0]["target"] = int(
                    extruder["target"]
                )
                Update = True
            if self.thermalManager["fan_speed"][0] != int(fan["speed"] * 100):
                self.thermalManager["fan_speed"][0] = int(fan["speed"] * 100)
                Update = True
            if self.feedrate_percentage != print_speed:
                self.feedrate_percentage = print_speed
                Update = True
            if self.extrusion_multiplier != extrusionMultiplier:
                self.extrusion_multiplier = extrusionMultiplier
                Update = True
            if self.fan_speed != fanSpeed:
                self.fan_speed = fanSpeed
                Update = True
            if self.BABY_Z_VAR != z_offset:
                self.BABY_Z_VAR = z_offset
                self.HMI_ValueStruct.offset_value = z_offset * 100
                Update = True
        except:
            pass  # missing key, shouldn't happen, fixes misses on conditionals ¯\_(ツ)_/¯
        self.job_Info = self.printer.lookup_object(
            "print_stats").get_status(self.reactor.monotonic())
        if self.job_Info:
            self.file_name = self.job_Info["filename"]
            self.status = self.job_Info["state"]
            self.HMI_flag.print_finish = self.getPercent() == 100.0
        return Update

    def printingIsPaused(self):
        return (
            self.job_Info["state"] == "paused"
            or self.job_Info["state"] == "pausing"
        )

    def getPercent(self):
        self.virtual_sdcard_stats = self.printer.lookup_object(
            "virtual_sdcard").get_status(self.reactor.monotonic())
        if self.virtual_sdcard_stats:
            if self.virtual_sdcard_stats["is_active"]:
                return self.virtual_sdcard_stats["progress"] * 100
        return 0

    def duration(self):
        self.virtual_sdcard_stats = self.printer.lookup_object(
            "virtual_sdcard").get_status(self.reactor.monotonic())
        if self.virtual_sdcard_stats:
            if self.virtual_sdcard_stats["is_active"]:
                return self.job_Info["print_duration"]
        return 0

    def remain(self):
        percent = self.getPercent()
        duration = self.duration()
        if percent:
            total = duration / (percent / 100)
            return total - duration
        return 0

    def home(self, homeZ=False):  # fixed using gcode
        script = "G28 X Y"
        if homeZ:
            script += " Z"
        self.sendGCode(script)

    def moveRelative(self, axis, distance, speed):
        self.sendGCode(
            "%s \n%s %s%s F%s%s"
            % (
                "G91",
                "G1",
                axis,
                distance,
                speed,
                "\nG90" if self.absolute_moves else "",
            )
        )

    def disable_all_heaters(self):
        self.setExtTemp(0)
        self.setBedTemp(0)

    def zero_fan_speeds(self):
        pass

    def preheat(self, profile):
        if profile == "PLA":
            self.preHeat(
                self.material_preset[0].bed_temp, self.material_preset[0].hotend_temp
            )
        elif profile == "ABS":
            self.preHeat(
                self.material_preset[1].bed_temp, self.material_preset[1].hotend_temp
            )

    def preHeat(self, bedtemp, exttemp, toolnum=0):
        # these work but invoke a wait which hangs the screen until they finish.
        # 		self.sendGCode('M140 S%s\nM190 S%s' % (bedtemp, bedtemp))
        # 		self.sendGCode('M104 T%s S%s\nM109 T%s S%s' % (toolnum, exttemp, toolnum, exttemp))
        self.setBedTemp(bedtemp)
        self.setExtTemp(exttemp)
    
    def bedIsHeating(self):
        bed = self.printer.lookup_object(
            "heater_bed").get_status(self.reactor.monotonic())
        return (int(bed["target"]) > int(bed["temperature"])) if bed else False
         
    def nozzleIsHeating(self):
        extruder = self.printer.lookup_object(
            "extruder").get_status(self.reactor.monotonic())
        return (int(extruder["target"]) > int(extruder["temperature"])) if extruder else False
    
    def openFile(self, file):
        self.selectedFile = file
    
    def printSelectedFile(self):
        self.sendGCode('SDCARD_PRINT_FILE FILENAME="{}"'.format(str(self.selectedFile)))

    def scanMetadata(self):
        sdcard = self.printer.lookup_object('virtual_sdcard')
        fileDir = os.path.join(sdcard.sdcard_dirname, self.selectedFile)
        self.metadata = {
            'layer_height': None,
            'estimated_time': None,
            'filament_used': None,
            'thumbnail': None # Default to placeholder image
        }
        
        try:
            with open(fileDir, 'r') as file:
                executable_block_end = False
                for line in file:
                    if executable_block_end:
                        if "; layer_height" in line:
                            match = re.search(r"(\d+\.\d+)", line) # Extract the value
                            if match:
                                self.metadata['layer_height'] = f"{float(match.group(1))}mm"

                        elif "estimated printing time" in line:
                            match = re.search(r'(?:(\d+)h)?\s*(\d+)m?\s*(\d+)s?', line)  # Extract hours, minutes, and seconds
                            if match:
                                hours = match.group(1) if match.group(1) else ""
                                minutes = match.group(2) if match.group(2) else "00"
                                seconds = match.group(3) if match.group(3) else "00"
                                # Format as --h--m--s
                                self.metadata['estimated_time'] = f"{hours}h{minutes}m{seconds}s" if hours else f'{minutes}m{seconds}s'

                        elif "filament used [mm]" in line:
                            match = re.search(r"filament used \[mm\] = (\d+\.\d+)", line) # Extract the value
                            if match:
                                filament_used_mm = float(match.group(1))
                                self.metadata['filament_used'] = f"{round(filament_used_mm / 1000, 2)}m"
                        
                    if "; EXECUTABLE_BLOCK_END" in line:
                        executable_block_end = True

                            
        except FileNotFoundError:
            self.log(f"Unable to find file: {fileDir}")


    def sendGCode(self, Gcode):
        self.gcode._process_commands([Gcode])

    def probe_calibrate(self):
        self.sendGCode('G28') # home the printer
        self.sendGCode('PRTOUCH_PROBE_OFFSET CLEAR_NOZZLE=0 APPLY_Z_ADJUST=1') # use the prtouch to find the z offset and apply it

    def resume_job(self):
        self.sendGCode('RESUME') # resume the print

    def pause_job(self):
        self.sendGCode('PAUSE') # pause the print

    def cancel_job(self):
        self.sendGCode('CANCEL_PRINT') # cancel the print

    def set_feedrate(self, value):
        self.sendGCode('M220 S' + str(value)) # set the feedrate through the M220 gcode command

    def moveAbsolute(self, axis, pos, feedrate):
        self.sendGCode('M82') # change to absolute positioning
        self.sendGCode('G1 {}{} F{}'.format(axis, str(pos), str(feedrate))) # move the specified axis at the set feedrate

    def save_settings(self):
        self.sendGCode('SAVE_CONFIG') # save the current configuration changes

    def setExtTemp(self, target, toolnum=0):
        self.sendGCode("M104 T%s S%s" % (toolnum, str(target)))

    def setBedTemp(self, target):
        self.sendGCode("M140 S%s" % str(target))

    def setZOffset(self, offset):
        self.sendGCode('SET_GCODE_OFFSET Z={} MOVE=1'.format(str(offset)))

    def add_mm(self, axis, zoffset):
        pass # done in offsetZ

    def log(self, msg, *args, **kwargs):
        if self._logging:
            logging.info("PrinterData: " + str(msg))

    def error(self, msg, *args, **kwargs):
        logging.error("PrinterData: " + str(msg))


def load_config(config):
    return PrinterData(config)
