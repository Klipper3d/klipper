import logging
import math
import time
import textwrap
from .display.menu_keys import MenuKeys
from .TJC3224 import TJC3224_LCD
from .printerInterface import PrinterData
from .serial_bridge import PrinterSerialBridge

def current_milli_time():
    return round(time.time() * 1000)


def _MAX(lhs, rhs):
    if lhs > rhs:
        return lhs
    else:
        return rhs


def _MIN(lhs, rhs):
    if lhs < rhs:
        return lhs
    else:
        return rhs


class select_t:
    now = 0
    last = 0

    def set(self, v):
        self.now = self.last = v

    def reset(self):
        self.set(0)

    def changed(self):
        c = self.now != self.last
        if c:
            self.last = self.now
            return c

    def dec(self):
        if self.now:
            self.now -= 1
        return self.changed()

    def inc(self, v):
        if self.now < (v - 1):
            self.now += 1
        else:
            self.now = v - 1
        return self.changed()

class E3V3SEMenuKeys(MenuKeys):
    """
    E3V3SEMenuKeys is a subclass of MenuKeys designed to
    customize initialization without modifying the original MenuKeys class.

    This class defaults the pins for the menu keys to the corresponding
    pins of the ender 3 v3 se 3d printer.
    """
    def __init__(self, config, callback):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.callback = callback
        buttons = self.printer.load_object(config, "buttons")
        # Register rotary encoder
        encoder_pins = config.get('encoder_pins', '^PA12, ^PA11')
        encoder_steps_per_detent = config.getchoice('encoder_steps_per_detent',
                                                    {2: 2, 4: 4}, 4)
        if encoder_pins is not None:
            try:
                pin1, pin2 = encoder_pins.split(',')
            except:
                raise config.error("Unable to parse encoder_pins")
            buttons.register_rotary_encoder(pin1.strip(), pin2.strip(),
                                            self.encoder_cw_callback,
                                            self.encoder_ccw_callback,
                                            encoder_steps_per_detent)
        self.encoder_fast_rate = config.getfloat('encoder_fast_rate',
                                                 .030, above=0.)
        self.last_encoder_cw_eventtime = 0
        self.last_encoder_ccw_eventtime = 0
        # Register click button
        self.is_short_click = False
        self.click_timer = self.reactor.register_timer(self.long_click_event)
        self.register_click_button(config, 'click_pin', self.click_callback, False,  '^!PB1')

    def register_click_button(self, config, name, callback, push_only=True, default_value=None,):
        pin = config.get(name, default_value)
        if pin is None:
            return
        buttons = self.printer.lookup_object("buttons")
        if config.get('analog_range_' + name, None) is None:
            if push_only:
                buttons.register_button_push(pin, callback)
            else:
                buttons.register_buttons([pin], callback)
            return
        amin, amax = config.getfloatlist('analog_range_' + name, count=2)
        pullup = config.getfloat('analog_pullup_resistor', 4700., above=0.)
        if push_only:
            buttons.register_adc_button_push(pin, amin, amax, pullup, callback)
        else:
            buttons.register_adc_button(pin, amin, amax, pullup, callback)


class E3V3SEPrinterSerialBridge(PrinterSerialBridge):
    """
    E3V3SEPrinterSerialBridge is a subclass of PrinterSerialBridge designed to
    customize initialization without modifying the original PrinterSerialBridge class.

    This class defaults the serial pins, baud, eol and serial bridge config to specific
    values used for the ender 3 v3 se 3d printer.
    """
    def __init__(self, config):
        self.callbacks = []
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.eol = config.get('eol', default='')
        self._ready = False
        self.baud = config.getint("baud", 115200)
        self.serial_config = config.getint("config", 3)
        self._logging = config.getboolean("logging", False)

        self.reactor = self.printer.get_reactor()
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler("klippy:disconnect",
            self.handle_disconnect)

        ppins = self.printer.lookup_object("pins")
        pin_params = ppins.lookup_pin(config.get("tx_pin", 'PA2'))
        rx_pin_params = ppins.lookup_pin(config.get("rx_pin" , 'PA3'))
        self.mcu = pin_params['chip']
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self.build_config)

        self.input_buffer = ""

        self.serial_bridge = self.printer.load_object(config, "serial_bridge")
        self.serial_bridge.setup_bridge(self)


class E3v3seDisplayMacro:
    def __init__(self, config):
        self.config = config
        printer = self.config.get_printer()
        parent = printer.lookup_object("e3v3se_display")
        id = config.get_name().split()[1]

        if not id.startswith("MACRO"):
            parent.error("Invalid section '%s'" % id)
            raise Exception("Invalid section '%s'" % id)

        gcode = config.get("gcode")
        label = config.get("label")
        icon = config.getint("icon", 14) # Defaults to icon_file

        parent.log("E3V3SE Display MACRO: id: '%s' label: '%s' icon: '%i' gcode: '%s'" % (id, label, icon, gcode))

        parent.custom_macros.append({
            "id": id,
            "icon": icon,
            "label": label,
            "gcode": gcode
        })


class E3v3seDisplay:

    ENCODER_DIFF_NO = 0  # no state
    ENCODER_DIFF_CW = 1  # clockwise rotation
    ENCODER_DIFF_FAST_CW = 2  # fast clockwise rotation
    ENCODER_DIFF_CCW = 3  # counterclockwise rotation
    ENCODER_DIFF_FAST_CCW = 4  # fast counterclockwise rotation
    ENCODER_DIFF_ENTER = 5  # click
    ENCODER_DIFF_LONG_ENTER = 6  # long click

    TROWS = 6
    MROWS = TROWS - 1  # Total rows, and other-than-Back
    HEADER_HEIGHT = 24  # Title bar height
    STATUS_Y = 260  # Y position of status area
    MLINE = 39  # Menu line height
    LBLX = 45  # Menu item label X
    MENU_CHR_W = 8
    STAT_CHR_W = 10

    MANUAL_PROBE_STEPS = [1.0, 0.1, 0.05]

    dwin_abort_flag = False  # Flag to reset feedrate, return to Home

    scroll_up = 2
    scroll_down = 3

    select_page = select_t()
    select_file = select_t()
    select_print = select_t()
    select_prepare = select_t()

    select_control = select_t()
    select_axis = select_t()
    select_temp = select_t()
    select_motion = select_t()
    select_tune = select_t()
    select_PLA = select_t()
    select_TPU = select_t()
    select_confirm = select_t()
    select_cancel = select_t()
    select_misc = select_t()
    select_icon_finder = select_t()
    select_print_preview = select_t()

    index_file = MROWS
    index_prepare = MROWS
    index_control = MROWS
    index_misc = MROWS
    index_tune = MROWS

    is_dimmed = False
    time_since_movement = 0
    display_dim_timeout = 300

    MainMenu = 0
    SelectFile = 1
    Prepare = 2
    Control = 3
    Misc = 4
    PrintProcess = 5
    AxisMove = 6
    TemperatureID = 7
    Motion = 8
    Info = 9
    Tune = 10
    PLAPreheat = 11
    TPUPreheat = 12
    MaxSpeed = 13
    MaxSpeed_value = 14
    MaxAcceleration = 15
    MaxAcceleration_value = 16
    MaxJerk = 17
    MaxJerk_value = 18
    Step = 19
    Step_value = 20
    FeatureNotAvailable = 21

    # Last Process ID
    Last_Prepare = 21
    last_status = ""

    # Back Process ID
    Back_Main = 22
    Back_Print = 23

    # Date variable ID
    Move_X = 24
    Move_Y = 25
    Move_Z = 26
    Extruder = 27
    ETemp = 28
    Homeoffset = 29
    BedTemp = 30
    FanSpeed = 31
    PrintSpeed = 32

    Print_window = 33
    Popup_Window = 34

    # New menus
    ManualProbeProcess = 35
    MessagePopup = 36
    IconFinder = 37

    print_preview = 38

    MINUNITMULT = 10

    EncoderRateLimit = True

    dwin_zoffset = 0.0
    last_zoffset = 0.0

    # Picture ID
    Start_Process = 0

    # IMAGE LIBRARIES ID
    ICON = 0
    GIF_ICON = 27
    languages = {
        "chinese": 2,
        "english": 4,
        "german": 6,
        "russian": 9,
        "french": 12,
        "turkish": 15,
        "spanish": 17,
        "italian": 19,
        "portuguese": 21,
        "japanese": 23,
        "korean": 25
    }

    # ICON ID
    icon_logo = 0
    icon_print = 1
    icon_print_selected = 2
    icon_prepare = 3
    icon_prepare_selected = 4
    icon_control = 5
    icon_control_selected = 6
    icon_leveling = 7
    icon_leveling_selected = 8
    icon_hotend_temp = 9
    icon_bedtemp = 10
    icon_speed = 11
    icon_z_offset = 12
    icon_back = 13
    icon_file = 14
    icon_print_time = 15
    icon_remain_time = 16
    icon_tune = 17
    icon_tune_selected = 18
    icon_pause = 19
    icon_pause_selected = 20
    icon_continue = 21
    icon_continue_selected = 22
    icon_stop = 23
    icon_stop_selected = 24
    icon_bar = 25
    icon_more = 26

    icon_axis = 27
    icon_close_motor = 28
    icon_homing = 29
    icon_set_home = 30
    icon_preheat_pla = 31
    icon_preheat_tpu = 32
    icon_cool = 33
    icon_language = 34

    icon_move_x = 35
    icon_move_y = 36
    icon_move_z = 37
    icon_move_e = 38

    icon_temperature = 40
    icon_motion = 41
    icon_write_eeprom = 42
    icon_read_eeprom = 43
    icon_resume_eeprom = 44
    icon_info = 45

    icon_SetEndTemp = 46
    icon_SetBedTemp = 47
    icon_FanSpeed = 48
    icon_SetPLAPreheat = 49
    icon_SetTPUPreheat = 50

    icon_MaxSpeed = 51
    icon_MaxAccelerated = 52
    icon_MaxJerk = 53
    icon_Step = 54
    icon_PrintSize = 55
    icon_Version = 56
    icon_Contact = 57
    icon_StockConfiguraton = 58
    icon_MaxSpeedX = 59
    icon_MaxSpeedY = 60
    icon_MaxSpeedZ = 61
    icon_MaxSpeedE = 62
    icon_MaxAccX = 63
    icon_MaxAccY = 64
    icon_MaxAccZ = 65
    icon_MaxAccE = 66
    icon_MaxSpeedJerkX = 67
    icon_MaxSpeedJerkY = 68
    icon_MaxSpeedJerkZ = 69
    icon_MaxSpeedJerkE = 70
    icon_StepX = 71
    icon_StepY = 72
    icon_StepZ = 73
    icon_StepE = 74
    icon_Setspeed = 75
    icon_SetZOffset = 76
    icon_Rectangle = 77
    icon_BLTouch = 78
    icon_TempTooLow = 79
    icon_AutoLeveling = 80
    icon_TempTooHigh = 81
    icon_NoTips_C = 82
    icon_NoTips_E = 83
    icon_continue_button = 96
    icon_continue_button_hovered = 96
    icon_cancel_button = 72
    icon_confirm_button = 73
    icon_Info_0 = 90
    icon_Info_1 = 91
    icon_preview_placeholder = 143

    icon_progress_0 = 145
    icon_nozzle_heating_0 = 110
    icon_bed_heating_0 = 125

    # TEXT ICON ID
    icon_TEXT_header_main = 1
    icon_TEXT_header_printing = 20
    icon_TEXT_header_tune = 17
    icon_TEXT_header_file_selection = 26
    icon_TEXT_header_motion = 33
    icon_TEXT_header_move = 39
    icon_TEXT_header_prepare = 40
    icon_TEXT_header_pause = 27
    icon_TEXT_header_temperature = 49
    icon_TEXT_header_control = 80
    icon_TEXT_header_print_finish = 81
    icon_TEXT_header_leveling = 85
    icon_TEXT_header_max_speed = 88
    icon_TEXT_header_language_selection = 106
    icon_TEXT_header_info = 61
    icon_TEXT_header_PLA_settings = 62
    icon_TEXT_header_TPU_settings = 64

    icon_popup_nozzle_temp_too_high = 71
    icon_popup_nozzle_temp_too_low = 69
    icon_popup_bed_temp_too_low = 140
    icon_popup_bed_temp_too_high = 141
    icon_popup_unexpected_stoppage = 70
    icon_popup_filament_reloaded = 67
    icon_popup_filament_runout = 68
    icon_popup_pause_print = 76
    icon_popup_stop_print = 77
    icon_popup_homing = 84

    icon_TEXT_Print = 2
    icon_TEXT_Print_selected = 8
    icon_TEXT_Prepare = 3
    icon_TEXT_Prepare_selected = 9
    icon_TEXT_Control = 4
    icon_TEXT_Control_selected = 10
    icon_TEXT_Leveling = 14
    icon_TEXT_Leveling_selected = 15
    icon_TEXT_Info = 5
    icon_TEXT_Info_selected = 11
    icon_TEXT_Stop = 6
    icon_TEXT_Stop_selected = 12
    icon_TEXT_Pause = 7
    icon_TEXT_Pause_selected = 13
    icon_TEXT_Z_Offset = 16
    icon_TEXT_Tune = 78
    icon_TEXT_Tune_selected = 79
    icon_TEXT_Printing_Speed = 19
    icon_TEXT_continue = 96
    icon_TEXT_nozzle_temperature = 23
    icon_TEXT_fan_speed = 22
    icon_TEXT_back = 21
    icon_TEXT_bed_temperature = 24
    icon_TEXT_remain = 25
    icon_TEXT_store_configuration = 28
    icon_TEXT_read_configuration = 29
    icon_TEXT_reset_configuration = 30
    icon_TEXT_temperature = 31
    icon_TEXT_motion = 32
    # icon_TEXT_ =33
    icon_TEXT_preheat_tpu = 34
    icon_TEXT_preheat_pla = 35
    icon_TEXT_auto_home = 36
    icon_TEXT_Info = 37
    icon_TEXT_disable_stepper = 38
    icon_TEXT_cooldown = 41
    icon_TEXT_move_x = 42
    icon_TEXT_move_y = 43
    icon_TEXT_move_z = 44
    icon_TEXT_move_e = 45
    icon_TEXT_move_axis = 46
    icon_TEXT_preheat_pla_settings = 47
    icon_TEXT_preheat_tpu_settings = 48
    icon_TEXT_steps_per_mm = 54
    icon_TEXT_max_acceleration = 55
    icon_TEXT_max_corner = 56
    icon_TEXT_max_speed = 57
    icon_TEXT_save_pla_parameters = 63
    icon_TEXT_save_tpu_parameters = 65
    icon_TEXT_language_selection = 66
    icon_TEXT_pla_fan_speed = 100
    icon_TEXT_pla_nozzle_temperature = 101
    icon_TEXT_pla_bed_temperature = 102
    icon_TEXT_tpu_nozzle_temperature = 103
    icon_TEXT_tpu_bed_temperature = 104
    icon_TEXT_tpu_fan_speed = 105
    icon_TEXT_max_speed_x = 107
    icon_TEXT_max_speed_y = 108
    icon_TEXT_max_speed_z = 109
    icon_TEXT_max_speed_e = 110
    icon_TEXT_max_acceleration_x = 112
    icon_TEXT_max_acceleration_y = 113
    icon_TEXT_max_acceleration_z = 114
    icon_TEXT_max_acceleration_e = 115
    icon_TEXT_estimated_time = 127
    icon_TEXT_filament_used = 128
    icon_TEXT_layer_height = 129
    icon_TEXT_hardware_version = 156
    icon_TEXT_bed_size = 59
    icon_TEXT_contact = 60

    icon_text_printing_time = 18
    icon_text_remain_time = 25

    # Color Palette
    color_white = 0xFFFF
    color_yellow = 0xFF0F
    color_popup_background = 0x31E8  # Popup background color
    color_background_grey = 0x1145  # Dark grey background color
    color_background_black = 0x0841  # Black background color
    color_background_red = 0xF00F  # Red background color
    color_popup_text = 0xD6BA  # Popup font_ background color
    color_line = 0x3A6A  # Split line color
    Rectangle_color = 0xEE2F  # Blue square cursor color
    Percent_color = 0xFE29  # Percentage color
    BarFill_color = 0x10E4  # Fill color of progress bar
    Selected_color = 0x33BB  # Selected color

    MENU_CHAR_LIMIT = 24

    MOTION_CASE_RATE = 1
    MOTION_CASE_ACCEL = 2
    MOTION_CASE_JERK = MOTION_CASE_ACCEL + 0
    MOTION_CASE_STEPS = MOTION_CASE_JERK + 1
    MOTION_CASE_TOTAL = MOTION_CASE_STEPS

    PREPARE_CASE_MOVE = 1
    PREPARE_CASE_DISA = 2
    PREPARE_CASE_HOME = 3
    PREPARE_CASE_ZOFF = PREPARE_CASE_HOME + 1
    PREPARE_CASE_PLA = PREPARE_CASE_ZOFF + 1
    PREPARE_CASE_TPU = PREPARE_CASE_PLA + 1
    PREPARE_CASE_COOL = PREPARE_CASE_TPU + 1
    PREPARE_CASE_LANG = PREPARE_CASE_COOL + 0
    PREPARE_CASE_TOTAL = PREPARE_CASE_LANG

    CONTROL_CASE_TEMP = 1
    CONTROL_CASE_MOVE = 2
    CONTROL_CASE_INFO = 3
    CONTROL_CASE_TOTAL = 3

    TUNE_CASE_SPEED = 1
    TUNE_CASE_TEMP = TUNE_CASE_SPEED + 1
    TUNE_CASE_BED = TUNE_CASE_TEMP + 1
    TUNE_CASE_FAN = TUNE_CASE_BED + 0
    TUNE_CASE_ZOFF = TUNE_CASE_FAN + 1
    TUNE_CASE_TOTAL = TUNE_CASE_ZOFF

    TEMP_CASE_TEMP = 0 + 1
    TEMP_CASE_BED = TEMP_CASE_TEMP + 1
    TEMP_CASE_FAN = TEMP_CASE_BED + 0
    TEMP_CASE_PLA = TEMP_CASE_FAN + 1
    TEMP_CASE_TPU = TEMP_CASE_PLA + 1
    TEMP_CASE_TOTAL = TEMP_CASE_TPU

    PREHEAT_CASE_TEMP = 0 + 1
    PREHEAT_CASE_BED = PREHEAT_CASE_TEMP + 1
    PREHEAT_CASE_FAN = PREHEAT_CASE_BED + 0
    PREHEAT_CASE_SAVE = PREHEAT_CASE_FAN + 1
    PREHEAT_CASE_TOTAL = PREHEAT_CASE_SAVE

    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.mutex = self.printer.get_reactor().mutex()
        self.name = config.get_name()
        self.reactor = self.printer.get_reactor()
        self._logging = config.getboolean("logging", False)
        self.gcode = self.printer.lookup_object("gcode")
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.encoder_state = self.ENCODER_DIFF_NO
        language = config.get("language", "english")
        self.selected_language = self.languages[language]

        # Manual probe feature
        self.manual_probe = None
        self.manual_probe_step_index = 0
        self.stepper_z = config.getsection("stepper_z")

        # Custom macro feature
        self.custom_macros = []

        # register for key events
        E3V3SEMenuKeys(config, self.key_event)

        # Message popup feature
        self.display_status = None
        self.last_display_status = None
        self.printer.register_event_handler("klippy:notify_mcu_error", self.handle_mcu_error)

        # Icon finder feature
        self.gcode.register_command("ENDER_SE_DISPLAY_ICON_FINDER", self.cmd_IconFinder, desc="Icon Finder")

        self.serial_bridge = E3V3SEPrinterSerialBridge(self.config)

        self.serial_bridge.register_callback(
            self._handle_serial_bridge_response)

        self.lcd = TJC3224_LCD(self.serial_bridge)
        self.checkkey = self.MainMenu
        self.pd = PrinterData(config)

        self._update_interval = 1
        self._update_timer = self.reactor.register_timer(self.EachMomentUpdate)

    def key_event(self, key, eventtime):
        if key == 'click':
            self.encoder_state = self.ENCODER_DIFF_ENTER
        elif key == 'long_click':
            self.encoder_state = self.ENCODER_DIFF_LONG_ENTER
        elif key == 'up':
            self.encoder_state = self.ENCODER_DIFF_CCW
        elif key == 'down':
            self.encoder_state = self.ENCODER_DIFF_CW
        elif key == 'fast_up':
            self.encoder_state = self.ENCODER_DIFF_FAST_CCW
        elif key == 'fast_down':
            self.encoder_state = self.ENCODER_DIFF_FAST_CW

        self.encoder_has_data()

    def get_encoder_state(self):
        last_state = self.encoder_state
        self.encoder_state = self.ENCODER_DIFF_NO
        return last_state

    def _handle_serial_bridge_response(self, data):
        byte_debug = ' '.join(['0x{:02x}'.format(byte) for byte in data])
        self.log("Received message: " + byte_debug)

    def cmd_IconFinder(self, gcmd):
        self.checkkey = self.IconFinder
        self.select_icon_finder.reset()
        self.Clear_Screen()
        self.Draw_IconFinder()

    def send_text(self, text):
        self.serial_bridge.send_text(text)

    def _screen_update(self, eventtime):
        self.pd.update_variable()
        return eventtime + self._update_interval

    def _screen_init(self, eventtime):
        self.reactor.update_timer(
            self._update_timer, eventtime + self._update_interval)
        return self.reactor.NEVER

    def handle_ready(self):
        self.pd.handle_ready()
        self.reactor.register_timer(self._reset_screen, self.reactor.monotonic())

        # Message popup feature
        try:
            self.display_status = self.printer.lookup_object("display_status", default=None)
            if self.display_status is None and "M117" not in self.gcode.ready_gcode_handlers:
                self.gcode.register_command("M117", self.cmd_M117)
        except Exception as e: 
            self.error("Error registering M117: %s" % e)

    def handle_mcu_error(self):
        self.show_popup(self.printer.get_state_message())

    def cmd_M117(self, gcmd):
        msg = gcmd.get_raw_command_parameters() or None
        if msg is not None:
            self.show_popup(msg)
        
    def _reset_screen(self, eventtime):
        self.log("Reset")
        self.reactor.register_timer(
            self._screen_init, self.reactor.monotonic() + 2.)
        return self.reactor.NEVER

    def is_manual_probe_active(self):
        try:
            self.manual_probe = self.printer.lookup_object("manual_probe")
            if self.manual_probe is not None:
                return self.manual_probe.status["is_active"]
        except:
            pass

        return False

    def lcdExit(self):
        logging.info("Shutting down the LCD")
        self.lcd.set_backlight_brightness(0)

    def MBASE(self, L):
        return 45 + self.MLINE * L

    def HMI_StartFrame(self, with_update):
        self.Clear_Screen()
        self.last_status = self.pd.status
        if self.pd.status == "printing":
            self.Goto_PrintProcess()
            self.Draw_Status_Area(with_update)
        elif self.pd.status in ["operational", "complete", "standby", "cancelled"]:
            self.Goto_MainMenu()
        else:
            self.Goto_MainMenu()

    def HMI_MainMenu(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_page.inc(4):
                if self.select_page.now == 0:
                    self.icon_Print()
                if self.select_page.now == 1:
                    self.icon_Print()
                    self.icon_Prepare()
                if self.select_page.now == 2:
                    self.icon_Prepare()
                    self.icon_Control()
                if self.select_page.now == 3:
                    self.icon_Control()
                    if self.pd.HAS_ONESTEP_LEVELING:
                        self.icon_Misc(True)
                    else:
                        self.icon_StartInfo(True)
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_page.dec():
                if self.select_page.now == 0:
                    self.icon_Print()
                    self.icon_Prepare()
                elif self.select_page.now == 1:
                    self.icon_Prepare()
                    self.icon_Control()
                elif self.select_page.now == 2:
                    self.icon_Control()
                    if self.pd.HAS_ONESTEP_LEVELING:
                        self.icon_Misc(False)
                    else:
                        self.icon_StartInfo(False)
                elif self.select_page.now == 3:
                    if self.pd.HAS_ONESTEP_LEVELING:
                        self.icon_Misc(True)
                    else:
                        self.icon_StartInfo(True)
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_page.now == 0:  # Print File
                self.checkkey = self.SelectFile
                self.Draw_Print_File_Menu()
            if self.select_page.now == 1:  # Prepare
                self.checkkey = self.Prepare
                self.select_prepare.reset()
                self.index_prepare = self.MROWS
                self.Draw_Prepare_Menu()
            if self.select_page.now == 2:  # Control
                self.checkkey = self.Control
                self.select_control.reset()
                self.index_control = self.MROWS
                self.Draw_Control_Menu()
            if self.select_page.now == 3:  # Leveling or Info
                if self.pd.HAS_ONESTEP_LEVELING:
                    self.checkkey = self.Misc
                    self.Clear_Screen()
                    self.Draw_Misc_Menu()
                else:
                    self.checkkey = self.Info
                    self.Draw_Info_Menu()

    def HMI_SelectFile(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        fullCnt = len(self.pd.GetFiles())

        if (encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW) and fullCnt:
            if self.select_file.inc(1 + fullCnt):
                itemnum = self.select_file.now - 1  # -1 for "Back"
                if (
                    self.select_file.now > self.MROWS
                    and self.select_file.now > self.index_file
                ):  # Cursor past the bottom
                    self.index_file = self.select_file.now  # New bottom line
                    self.Scroll_Menu(self.scroll_up)
                    self.Draw_SDItem(
                        itemnum, self.MROWS
                    )  # Draw and init the shift name
                else:
                    self.Move_Highlight(
                        1, self.select_file.now + self.MROWS - self.index_file
                    )  # Just move highlight
        elif (encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW) and fullCnt:
            if self.select_file.dec():
                itemnum = self.select_file.now - 1  # -1 for "Back"
                if (
                    self.select_file.now < self.index_file - self.MROWS
                ):  # Cursor past the top
                    self.index_file -= 1  # New bottom line
                    self.Scroll_Menu(self.scroll_down)
                    if self.index_file == self.MROWS:
                        self.Draw_Back_First()
                    else:
                        self.Draw_SDItem(
                            itemnum, 0
                        )  # Draw the item (and init shift name)
                else:
                    self.Move_Highlight(
                        -1, self.select_file.now + self.MROWS - self.index_file
                    )  # Just move highlight
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_file.now == 0:  # Back
                if self.pd.subdirIndex == 0:
                    self.select_page.set(0)
                    self.Goto_MainMenu()
                else:
                    self.pd.fileListBack()
                    self.Redraw_SD_List()
            else:
                if self.pd.selectFile(self.select_file.now - 1):
                    self.pd.openFile(self.pd.fl[self.select_file.now - 1])

                    self.select_print.reset()
                    self.select_file.reset()

                    self.checkkey = self.print_preview
                    self.Draw_Print_Preview()
                else:
                    self.Redraw_SD_List()

    def HMI_Misc(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        custom_macro_count = len(self.custom_macros)

        if (encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW) and custom_macro_count:
            if self.select_misc.inc(1 + custom_macro_count):
                index = self.select_misc.now - 1 # -1 for "Back"
                if self.select_misc.now > self.MROWS and self.select_misc.now > self.index_misc:
                    # Cursor past the last item, scroll up
                    self.index_misc = self.select_misc.now
                    self.Scroll_Menu(self.scroll_up)
                    self.Draw_CustomMacroItem(index, self.MROWS)
                else:
                    self.Move_Highlight(1, self.select_misc.now + self.MROWS - self.index_misc)# Move highlight
        elif (encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW) and custom_macro_count:
            if self.select_misc.dec():
                index = self.select_misc.now - 1 # -1 for "Back"
                if self.select_misc.now < self.index_misc - self.MROWS:
                    # Cursor past the first item, scroll down
                    self.index_misc -= 1
                    self.Scroll_Menu(self.scroll_down)
                    if self.index_misc == self.MROWS:
                        self.Draw_Back_First()
                    else:
                        self.Draw_CustomMacroItem(index, self.MROWS)
                else:
                    self.Move_Highlight(-1, self.select_misc.now + self.MROWS - self.index_misc)# Move highlight
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_misc.now == 0: # Back
                self.select_misc.set(0)
                self.Goto_MainMenu()
            else:
                macro = self.custom_macros[self.select_misc.now - 1]
                self.log("Running custom macro '%s'" % macro["id"])

                try:
                    self.gcode.run_script_from_command(macro["gcode"])
                except Exception as e:
                    self.error("Error running custom macro '%s': %s" % (macro["id"], e))
                    self.gcode.respond_info("Error running custom macro '%s': %s" % (macro["id"], e))
                    self.show_popup("Error: %s" % e)

    def HMI_Prepare(self):
        """
        This function handles the logic for scrolling through the prepare menu options,
        selecting different actions, and executing the corresponding actions based on user input.
        """
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_prepare.inc(1 + self.PREPARE_CASE_TOTAL):
                if (
                    self.select_prepare.now > self.MROWS
                    and self.select_prepare.now > self.index_prepare
                ):
                    self.index_prepare = self.select_prepare.now

                    # Scroll up and draw a blank bottom line
                    self.Scroll_Menu(self.scroll_up)
                    self.Draw_Menu_Icon(
                        self.MROWS, self.icon_axis + self.select_prepare.now - 1
                    )

                    # Draw "More" icon for sub-menus
                    if self.index_prepare < 7:
                        self.Draw_More_Icon(self.MROWS - self.index_prepare + 1)

                    if self.pd.HAS_HOTEND:
                        if self.index_prepare == self.PREPARE_CASE_TPU:
                            self.Item_Prepare_TPU(self.MROWS)
                    if self.pd.HAS_PREHEAT:
                        if self.index_prepare == self.PREPARE_CASE_COOL:
                            self.Item_Prepare_Cool(self.MROWS)
                else:
                    self.Move_Highlight(
                        1, self.select_prepare.now + self.MROWS - self.index_prepare
                    )
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_prepare.dec():
                if self.select_prepare.now < self.index_prepare - self.MROWS:
                    self.index_prepare -= 1
                    self.Scroll_Menu(self.scroll_down)

                    if self.index_prepare == self.MROWS:
                        self.Draw_Back_First()
                    else:
                        self.Draw_Menu_Line(
                            0, self.icon_axis + self.select_prepare.now - 1
                        )

                    if self.index_prepare < 7:
                        self.Draw_More_Icon(self.MROWS - self.index_prepare + 1)

                    if self.index_prepare == 6:
                        self.Item_Prepare_Move(0)
                    elif self.index_prepare == 7:
                        self.Item_Prepare_Disable(0)
                    elif self.index_prepare == 8:
                        self.Item_Prepare_Home(0)
                else:
                    self.Move_Highlight(
                        -1, self.select_prepare.now + self.MROWS - self.index_prepare
                    )

        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_prepare.now == 0:  # Back
                self.select_page.set(1)
                self.Goto_MainMenu()

            elif self.select_prepare.now == self.PREPARE_CASE_MOVE:  # Axis move
                self.checkkey = self.AxisMove
                self.select_axis.reset()
                self.Draw_Move_Menu()
                self.pd.sendGCode("G92 E0")
                self.pd.current_position.e = self.pd.HMI_ValueStruct.Move_E_scale = 0
            elif self.select_prepare.now == self.PREPARE_CASE_DISA:  # Disable steppers
                self.pd.sendGCode("M84")
            elif self.select_prepare.now == self.PREPARE_CASE_HOME:  # Homing
                self.checkkey = self.Last_Prepare
                self.index_prepare = self.MROWS
                self.pd.current_position.homing()
                self.pd.HMI_flag.home_flag = True
                self.Popup_Window_Home()
                self.pd.sendGCode("G28")
            elif self.select_prepare.now == self.PREPARE_CASE_ZOFF:  # Z-offset
                self.checkkey = self.Homeoffset

                self.pd.HMI_ValueStruct.show_mode = -4

                self.lcd.draw_signed_float(
                    True,
                    self.lcd.font_8x8,
                    self.color_white,
                    self.color_background_black,
                    2,
                    3,
                    175,
                    self.MBASE(self.PREPARE_CASE_ZOFF + self.MROWS - self.index_prepare)
                    - 10,
                    self.pd.HMI_ValueStruct.offset_value,
                )

                self.EncoderRateLimit = False

            elif self.select_prepare.now == self.PREPARE_CASE_PLA:  # PLA preheat
                self.pd.preheat("PLA")

            elif self.select_prepare.now == self.PREPARE_CASE_TPU:  # TPU preheat
                self.pd.preheat("TPU")

            elif self.select_prepare.now == self.PREPARE_CASE_COOL:  # Cool
                if self.pd.HAS_FAN:
                    self.pd.zero_fan_speeds()
                self.pd.disable_all_heaters()

            elif self.select_prepare.now == self.PREPARE_CASE_LANG:  # Toggle Language
                #self.HMI_ToggleLanguage()
                self.Draw_Prepare_Menu()

    def HMI_Control(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_control.inc(1 + self.CONTROL_CASE_TOTAL):
                if (
                    self.select_control.now > self.MROWS
                    and self.select_control.now > self.index_control
                ):
                    self.index_control = self.select_control.now
                    self.Scroll_Menu(self.scroll_up)
                    self.Draw_Menu_Icon(
                        self.MROWS, self.icon_temperature + self.index_control - 1
                    )
                    self.Draw_More_Icon(
                        self.CONTROL_CASE_TEMP + self.MROWS - self.index_control
                    )  # Temperature >
                    self.Draw_More_Icon(
                        self.CONTROL_CASE_MOVE + self.MROWS - self.index_control
                    )  # Motion >
                    if self.index_control > self.MROWS:
                        self.Draw_More_Icon(
                            self.CONTROL_CASE_INFO + self.MROWS - self.index_control
                        )  # Info >
                        self.lcd.move_screen_area(
                            1,
                            0,
                            104,
                            24,
                            114,
                            self.LBLX,
                            self.MBASE(self.CONTROL_CASE_INFO - 1),
                        )
                else:
                    self.Move_Highlight(
                        1, self.select_control.now + self.MROWS - self.index_control
                    )
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_control.dec():
                if self.select_control.now < self.index_control - self.MROWS:
                    self.index_control -= 1
                    self.Scroll_Menu(self.scroll_down)
                    if self.index_control == self.MROWS:
                        self.Draw_Back_First()
                    else:
                        self.Draw_Menu_Line(
                            0, self.icon_temperature + self.select_control.now - 1
                        )
                    self.Draw_More_Icon(
                        0 + self.MROWS - self.index_control + 1
                    )  # Temperature >
                    self.Draw_More_Icon(
                        1 + self.MROWS - self.index_control + 1
                    )  # Motion >
                else:
                    self.Move_Highlight(
                        -1, self.select_control.now + self.MROWS - self.index_control
                    )
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_control.now == 0:  # Back
                self.select_page.set(2)
                self.Goto_MainMenu()
            if self.select_control.now == self.CONTROL_CASE_TEMP:  # Temperature
                self.checkkey = self.TemperatureID
                self.pd.HMI_ValueStruct.show_mode = -1
                self.select_temp.reset()
                self.Draw_Temperature_Menu()
            if self.select_control.now == self.CONTROL_CASE_MOVE:  # Motion
                self.checkkey = self.Motion
                self.select_motion.reset()
                self.Draw_Motion_Menu()
            if self.select_control.now == self.CONTROL_CASE_INFO:  # Info
                self.checkkey = self.Info
                self.Draw_Info_Menu()

    def HMI_FeatureNotAvailable(self):
        """
        Handles the "feature not available" popup.
        This method is called when the user enters a menu that
        is not implemented yet in the display interface. After
        the user presses the confirmation, he needs to go back to
        where he came from.
        """
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        if encoder_state == self.ENCODER_DIFF_ENTER:
            self.checkkey = self.popup_caller
            if self.popup_caller == self.MainMenu:
                self.Goto_MainMenu()
            if self.popup_caller == self.Motion:
                self.Draw_Motion_Menu()

    def HMI_Info(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        if encoder_state == self.ENCODER_DIFF_ENTER:
            if self.pd.HAS_ONESTEP_LEVELING:
                self.checkkey = self.Control
                self.select_control.set(self.CONTROL_CASE_INFO)
                self.Draw_Control_Menu()
            else:
                self.select_page.set(3)
                self.Goto_MainMenu()

    def HMI_Printing(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        if self.pd.HMI_flag.done_confirm_flag:
            if encoder_state == self.ENCODER_DIFF_ENTER:
                self.pd.HMI_flag.done_confirm_flag = False
                self.dwin_abort_flag = True  # Reset feedrate, return to Home
            return

        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_print.inc(3):
                if self.select_print.now == 0:
                    self.show_tune()
                elif self.select_print.now == 1:
                    self.show_tune()
                    if self.pd.printingIsPaused():
                        self.show_continue()
                    else:
                        self.show_pause()
                elif self.select_print.now == 2:
                    if self.pd.printingIsPaused():
                        self.show_continue()
                    else:
                        self.show_pause()
                    self.show_stop()
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_print.dec():
                if self.select_print.now == 0:
                    self.show_tune()
                    if self.pd.printingIsPaused():
                        self.show_continue()
                    else:
                        self.show_pause()
                elif self.select_print.now == 1:
                    if self.pd.printingIsPaused():
                        self.show_continue()
                    else:
                        self.show_pause()
                    self.show_stop()
                elif self.select_print.now == 2:
                    self.show_stop()
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_print.now == 0:  # Tune
                self.checkkey = self.Tune
                self.pd.HMI_ValueStruct.show_mode = 0
                self.select_tune.reset()
                self.index_tune = self.MROWS
                self.Draw_Tune_Menu()
            elif self.select_print.now == 1:  # Pause
                if self.pd.HMI_flag.pause_flag:
                    self.show_pause()
                    self.pd.resume_job()
                else:
                    self.pd.HMI_flag.select_flag = True
                    self.checkkey = self.Print_window
                    self.Popup_window_PauseOrStop()
            elif self.select_print.now == 2:  # Stop
                self.pd.HMI_flag.select_flag = True
                self.checkkey = self.Print_window
                self.Popup_window_PauseOrStop()

    def HMI_PrintPreview(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        elif encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.select_cancel.set(1)
            self.select_confirm.reset()
            self.Draw_Print_Confirmation_Buttons()
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.select_confirm.set(1)
            self.select_cancel.reset()
            self.Draw_Print_Confirmation_Buttons()
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_confirm.now:
                self.pd.HMI_flag.heat_flag = True
                self.pd.HMI_flag.print_finish = False
                self.pd.HMI_ValueStruct.show_mode = 0

                self.pd.printSelectedFile()
                self.Goto_PrintProcess()
            else:
                self.pd.selectedFile = None
                self.checkkey = self.SelectFile
                self.Draw_Print_File_Menu()
    def HMI_ManualProbe(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO or self.manual_probe == None:
            return

        step = self.MANUAL_PROBE_STEPS[self.manual_probe_step_index]
        current_pos = self.manual_probe.status["z_position"]
        min_z = self.stepper_z.getfloat("position_min", self.pd.Z_MIN_POS, note_valid=False)
        max_z = self.stepper_z.getfloat("position_max", self.pd.Z_MAX_POS, note_valid=False)
        update = False
        error = False

        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if math.floor(current_pos - step) <= min_z:
                self.error(f"Ignoring move '{current_pos - step}' as it is lower than Z limit '{min_z}'!")
                error = True
            else:
                self.gcode.run_script_from_command(f"TESTZ Z=-{step}")
            update = True
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            step = self.MANUAL_PROBE_STEPS[self.manual_probe_step_index]
            if math.ceil(current_pos + step) >= max_z:
                self.error(f"Ignoring move '{current_pos + step}' as it is greater than Z limit '{max_z}'!")
                error = True
            else:
                self.gcode.run_script_from_command(f"TESTZ Z={step}")
            update = True
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            self.manual_probe_step_index = (self.manual_probe_step_index + 1) % len(self.MANUAL_PROBE_STEPS)
            update = True
        elif encoder_state == self.ENCODER_DIFF_LONG_ENTER:
            self.gcode.run_script_from_command("ACCEPT")
            self.Goto_MainMenu()

        if update:
            self.Draw_Manual_Probe_Menu(draw_static_elements=False, draw_error=error)

    def HMI_MessagePopup(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        if encoder_state == self.ENCODER_DIFF_ENTER:
            self.checkkey = self.popup_caller
            if self.popup_caller == self.Motion:
                self.Draw_Motion_Menu()
            elif self.popup_caller == self.Misc:
                self.Draw_Misc_Menu()
            elif self.popup_caller == self.Info:
                self.Draw_Info_Menu()
            elif self.popup_caller == self.TemperatureID:
                self.Draw_Temperature_Menu()
            elif self.popup_caller == self.Prepare:
                self.Draw_Prepare_Menu()
            elif self.popup_caller == self.Control:
                self.Draw_Control_Menu()
            elif self.popup_caller == self.Tune:
                self.Draw_Tune_Menu()
            else:
                self.Goto_MainMenu()

            self.popup_caller = None

    def HMI_IconFinder(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        
        updated = False
        if encoder_state == self.ENCODER_DIFF_ENTER:
            self.Goto_MainMenu()
        elif encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.select_icon_finder.inc(255)
            updated = True
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.select_icon_finder.dec()
            updated = True

        if updated:
            self.lcd.draw_rectangle(
                1,
                self.color_background_black,
                0, self.HEADER_HEIGHT,
                self.lcd.screen_width,
                self.lcd.screen_height - self.MENU_CHR_W - 15
            )
            self.Draw_IconFinder()

    # Pause and Stop window */
    def HMI_PauseOrStop(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.select_cancel.set(1)
            self.select_confirm.reset()
            self.Draw_Confirm_Cancel_Buttons()
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.select_confirm.set(1)
            self.select_cancel.reset()
            self.Draw_Confirm_Cancel_Buttons()
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_print.now == 1:  # pause window
                if self.pd.HMI_flag.select_flag:
                    self.pd.HMI_flag.pause_action = True
                    self.show_continue()
                    self.pd.pause_job()
                self.Goto_PrintProcess()
            elif self.select_print.now == 2:  # stop window
                if self.pd.HMI_flag.select_flag:
                    self.dwin_abort_flag = True  # Reset feedrate, return to Home
                    self.pd.cancel_job()
                    self.Goto_MainMenu()
                else:
                    self.Goto_PrintProcess()  # cancel stop

    # Tune  */
    def HMI_Tune(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_tune.inc(1 + self.TUNE_CASE_TOTAL):
                if (
                    self.select_tune.now > self.MROWS
                    and self.select_tune.now > self.index_tune
                ):
                    self.index_tune = self.select_tune.now
                    self.Scroll_Menu(self.scroll_up)
                else:
                    self.Move_Highlight(
                        1, self.select_tune.now + self.MROWS - self.index_tune
                    )
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_tune.dec():
                if self.select_tune.now < self.index_tune - self.MROWS:
                    self.index_tune -= 1
                    self.Scroll_Menu(self.scroll_down)
                    if self.index_tune == self.MROWS:
                        self.Draw_Back_First()
                else:
                    self.Move_Highlight(
                        -1, self.select_tune.now + self.MROWS - self.index_tune
                    )
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_tune.now == 0:  # Back
                self.select_print.set(0)
                self.Goto_PrintProcess()
            elif self.select_tune.now == self.TUNE_CASE_SPEED:  # Print speed
                self.checkkey = self.PrintSpeed
                self.pd.HMI_ValueStruct.print_speed = self.pd.feedrate_percentage
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_white,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(self.TUNE_CASE_SPEED + self.MROWS - self.index_tune) - 8,
                    self.pd.feedrate_percentage,
                )
                self.EncoderRateLimit = False
            elif self.select_tune.now == self.TUNE_CASE_ZOFF:  # z offset
                self.checkkey = self.Homeoffset
                self.lcd.draw_signed_float(
                    True,
                    self.lcd.font_8x8,
                    self.color_white,
                    self.color_background_black,
                    2,
                    3,
                    175,
                    self.MBASE(self.TUNE_CASE_ZOFF) - 10,
                    self.pd.HMI_ValueStruct.offset_value,
                )

    def HMI_PrintSpeed(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        if encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.print_speed += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.print_speed -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.print_speed += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.print_speed -= 10

        elif encoder_state == self.ENCODER_DIFF_ENTER:
            self.checkkey = self.Tune
            self.encoderRate = True
            self.pd.set_feedrate(self.pd.HMI_ValueStruct.print_speed)

        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            200,
            self.MBASE(self.select_tune.now + self.MROWS - self.index_tune) - 8,
            self.pd.HMI_ValueStruct.print_speed,
        )

    def HMI_AxisMove(self):
        """
        Handles the axis movement in the HMI (Human Machine Interface).

        This function checks the encoder state and performs the corresponding action based on the state.
        It updates the display and handles the movement of the X, Y, Z axes, and the extruder.
        """
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        # In case of "nozzle too cold" popup is on the screen
        if self.pd.PREVENT_COLD_EXTRUSION:
            if self.pd.HMI_flag.ETempTooLow_flag:
                # Resuming after "nozzle too cold" popup should clear the flag and draw move menu again
                if encoder_state == self.ENCODER_DIFF_ENTER:
                    self.pd.HMI_flag.ETempTooLow_flag = False
                    self.pd.current_position.e = (
                        self.pd.HMI_ValueStruct.Move_E_scale
                    ) = 0
                    self.Draw_Move_Menu()
                return

        # Avoid flicker by updating only the previous menu
        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_axis.inc(1 + 4):
                self.Move_Highlight(1, self.select_axis.now)
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_axis.dec():
                self.Move_Highlight(-1, self.select_axis.now)
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            selected_line = self.select_axis.now
            if selected_line == 0:  # Back
                self.checkkey = self.Prepare
                self.select_prepare.set(1)
                self.index_prepare = self.MROWS
                self.Draw_Prepare_Menu()
            elif selected_line == 1:  # X axis move
                self.checkkey = self.Move_X
                self.pd.HMI_ValueStruct.Move_X_scale = (
                    self.pd.current_position.x * self.MINUNITMULT
                )
                self.lcd.draw_float_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    1,
                    175,
                    self.MBASE(selected_line) - 10,
                    self.pd.HMI_ValueStruct.Move_X_scale,
                )
                self.EncoderRateLimit = False
            elif selected_line == 2:  # Y axis move
                self.checkkey = self.Move_Y
                self.pd.HMI_ValueStruct.Move_Y_scale = (
                    self.pd.current_position.y * self.MINUNITMULT
                )
                self.lcd.draw_float_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    1,
                    175,
                    self.MBASE(selected_line) - 10,
                    self.pd.HMI_ValueStruct.Move_Y_scale,
                )
                self.EncoderRateLimit = False
            elif selected_line == 3:  # Z axis move
                self.checkkey = self.Move_Z
                self.pd.HMI_ValueStruct.Move_Z_scale = (
                    self.pd.current_position.z * self.MINUNITMULT
                )
                self.lcd.draw_float_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    1,
                    175,
                    self.MBASE(selected_line) - 10,
                    self.pd.HMI_ValueStruct.Move_Z_scale,
                )
                self.EncoderRateLimit = False
            elif selected_line == 4:  # Extruder
                # Check if nozzle is too cold and don't allow extrusion. Popup warning instead
                if self.pd.PREVENT_COLD_EXTRUSION:
                    if (
                        self.pd.thermalManager["temp_hotend"][0]["celsius"]
                        < self.pd.EXTRUDE_MINTEMP
                    ):
                        self.pd.HMI_flag.ETempTooLow_flag = True
                        self.Popup_Window_ETempTooLow()
                        return
                self.checkkey = self.Extruder
                self.pd.HMI_ValueStruct.Move_E_scale = (
                    self.pd.current_position.e * self.MINUNITMULT
                )
                self.lcd.draw_signed_float(
                    False,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    1,
                    175,
                    self.MBASE(selected_line) - 10,
                    self.pd.HMI_ValueStruct.Move_E_scale,
                )
                self.EncoderRateLimit = False

    def HMI_Move_X(self):
        """
        Handles the X axis move logic based on the encoder input.
        """
        encoder_state = self.get_encoder_state()

        if encoder_state == self.ENCODER_DIFF_NO:
            return
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            # Enable change in Move X value, draw text as yellow
            self.checkkey = self.AxisMove
            self.EncoderRateLimit = True
            self.lcd.draw_float_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_yellow,
                self.color_background_black,
                3,
                1,
                175,
                self.MBASE(1) - 10,
                self.pd.HMI_ValueStruct.Move_X_scale,
            )
            self.pd.moveAbsolute("X", self.pd.current_position.x, 5000)

            return
        elif encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.Move_X_scale += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.Move_X_scale -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.Move_X_scale += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.Move_X_scale -= 10

        if (
            self.pd.HMI_ValueStruct.Move_X_scale
            < (self.pd.X_MIN_POS) * self.MINUNITMULT
        ):
            self.pd.HMI_ValueStruct.Move_X_scale = (
                self.pd.X_MIN_POS
            ) * self.MINUNITMULT

        if (
            self.pd.HMI_ValueStruct.Move_X_scale
            > (self.pd.X_MAX_POS) * self.MINUNITMULT
        ):
            self.pd.HMI_ValueStruct.Move_X_scale = (
                self.pd.X_MAX_POS
            ) * self.MINUNITMULT

        self.pd.current_position.x = self.pd.HMI_ValueStruct.Move_X_scale / 10
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_yellow,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(1) - 10,
            self.pd.HMI_ValueStruct.Move_X_scale,
        )

    def HMI_Move_Y(self):
        """
        Handles the Y axis move logic based on the encoder input.
        """
        encoder_state = self.get_encoder_state()

        if encoder_state == self.ENCODER_DIFF_NO:
            return
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            self.checkkey = self.AxisMove
            self.EncoderRateLimit = True
            self.lcd.draw_float_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_yellow,
                self.color_background_black,
                3,
                1,
                175,
                self.MBASE(2) - 10,
                self.pd.HMI_ValueStruct.Move_Y_scale,
            )

            self.pd.moveAbsolute("Y", self.pd.current_position.y, 5000)

            return
        elif encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.Move_Y_scale += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.Move_Y_scale -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.Move_Y_scale += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.Move_Y_scale -= 10

        if (
            self.pd.HMI_ValueStruct.Move_Y_scale
            < (self.pd.Y_MIN_POS) * self.MINUNITMULT
        ):
            self.pd.HMI_ValueStruct.Move_Y_scale = (
                self.pd.Y_MIN_POS
            ) * self.MINUNITMULT

        if (
            self.pd.HMI_ValueStruct.Move_Y_scale
            > (self.pd.Y_MAX_POS) * self.MINUNITMULT
        ):
            self.pd.HMI_ValueStruct.Move_Y_scale = (
                self.pd.Y_MAX_POS
            ) * self.MINUNITMULT

        self.pd.current_position.y = self.pd.HMI_ValueStruct.Move_Y_scale / 10
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_yellow,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(2) - 10,
            self.pd.HMI_ValueStruct.Move_Y_scale,
        )

    def HMI_Move_Z(self):
        """
        Handles the Z axis move logic based on the encoder input.
        """
        encoder_state = self.get_encoder_state()

        if encoder_state == self.ENCODER_DIFF_NO:
            return
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            self.checkkey = self.AxisMove
            self.EncoderRateLimit = True
            self.lcd.draw_float_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_yellow,
                self.color_background_black,
                3,
                1,
                175,
                self.MBASE(3) - 10,
                self.pd.HMI_ValueStruct.Move_Z_scale,
            )
            self.pd.moveAbsolute("Z", self.pd.current_position.z, 600)

            return
        elif encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.Move_Z_scale += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.Move_Z_scale -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.Move_Z_scale += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.Move_Z_scale -= 10

        if (
            self.pd.HMI_ValueStruct.Move_Z_scale
            < (self.pd.Z_MIN_POS) * self.MINUNITMULT
        ):
            self.pd.HMI_ValueStruct.Move_Z_scale = (
                self.pd.Z_MIN_POS
            ) * self.MINUNITMULT

        if (
            self.pd.HMI_ValueStruct.Move_Z_scale
            > (self.pd.Z_MAX_POS) * self.MINUNITMULT
        ):
            self.pd.HMI_ValueStruct.Move_Z_scale = (
                self.pd.Z_MAX_POS
            ) * self.MINUNITMULT

        self.pd.current_position.z = self.pd.HMI_ValueStruct.Move_Z_scale / 10
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_yellow,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(3) - 10,
            self.pd.HMI_ValueStruct.Move_Z_scale,
        )

    def HMI_Move_E(self):
        """
        Handles the Extruder move logic based on the encoder input.
        """
        self.pd.last_E_scale = 0
        encoder_state = self.get_encoder_state()

        if encoder_state == self.ENCODER_DIFF_NO:
            return

        elif encoder_state == self.ENCODER_DIFF_ENTER:
            self.checkkey = self.AxisMove
            self.EncoderRateLimit = True
            self.pd.last_E_scale = self.pd.HMI_ValueStruct.Move_E_scale
            self.lcd.draw_signed_float(
                True,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                3,
                1,
                175,
                self.MBASE(4) - 10,
                self.pd.HMI_ValueStruct.Move_E_scale,
            )
            self.pd.moveAbsolute("E", self.pd.current_position.e, 300)
        elif encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.Move_E_scale += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.Move_E_scale -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.Move_E_scale += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.Move_E_scale -= 10

        if (self.pd.HMI_ValueStruct.Move_E_scale - self.pd.last_E_scale) > (
            self.pd.EXTRUDE_MAXLENGTH
        ) * self.MINUNITMULT:
            self.pd.HMI_ValueStruct.Move_E_scale = (
                self.pd.last_E_scale + (self.pd.EXTRUDE_MAXLENGTH) * self.MINUNITMULT
            )
        elif (self.pd.last_E_scale - self.pd.HMI_ValueStruct.Move_E_scale) > (
            self.pd.EXTRUDE_MAXLENGTH
        ) * self.MINUNITMULT:
            self.pd.HMI_ValueStruct.Move_E_scale = (
                self.pd.last_E_scale - (self.pd.EXTRUDE_MAXLENGTH) * self.MINUNITMULT
            )
        self.pd.current_position.e = self.pd.HMI_ValueStruct.Move_E_scale / 10
        self.lcd.draw_signed_float(
            True,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(4) - 10,
            self.pd.HMI_ValueStruct.Move_E_scale,
        )

    def HMI_Temperature(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_temp.inc(1 + self.TEMP_CASE_TOTAL):
                self.Move_Highlight(1, self.select_temp.now)
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_temp.dec():
                self.Move_Highlight(-1, self.select_temp.now)
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_temp.now == 0:  # back
                self.checkkey = self.Control
                self.select_control.set(1)
                self.index_control = self.MROWS
                self.Draw_Control_Menu()
            elif self.select_temp.now == self.TEMP_CASE_TEMP:  # Nozzle temperature
                self.checkkey = self.ETemp
                self.pd.HMI_ValueStruct.E_Temp = self.pd.thermalManager["temp_hotend"][
                    0
                ]["target"]
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(1) - 8,
                    self.pd.thermalManager["temp_hotend"][0]["target"],
                )
                self.EncoderRateLimit = False
            elif self.select_temp.now == self.TEMP_CASE_BED:  # Bed temperature
                self.checkkey = self.BedTemp
                self.pd.HMI_ValueStruct.Bed_Temp = self.pd.thermalManager["temp_bed"][
                    "target"
                ]
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(2) - 8,
                    self.pd.thermalManager["temp_bed"]["target"],
                )
                self.EncoderRateLimit = False
            elif self.select_temp.now == self.TEMP_CASE_FAN:  # Fan speed
                self.checkkey = self.FanSpeed
                self.pd.HMI_ValueStruct.Fan_speed = self.pd.thermalManager["fan_speed"][
                    0
                ]
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(3) - 8,
                    self.pd.thermalManager["fan_speed"][0],
                )
                self.EncoderRateLimit = False

            elif self.select_temp.now == self.TEMP_CASE_PLA:  # PLA preheat setting
                self.checkkey = self.PLAPreheat
                self.select_PLA.reset()
                self.pd.HMI_ValueStruct.show_mode = -2

                self.Clear_Main_Window()
                self.lcd.draw_icon(
                    False,
                    self.selected_language,
                    self.icon_TEXT_header_PLA_settings,
                    self.HEADER_HEIGHT,
                    1,
                )

                self.Draw_Back_First()
                i = 1
                self.Draw_Menu_Line_With_Only_Icons(
                    i, self.icon_SetEndTemp, self.icon_TEXT_pla_nozzle_temperature
                )  # PLA nozzle temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_white,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(i) - 8,
                    self.pd.material_preset[0].hotend_temp,
                )
                if self.pd.HAS_HEATED_BED:
                    i += 1
                    self.Draw_Menu_Line_With_Only_Icons(
                        i, self.icon_SetBedTemp, self.icon_TEXT_pla_bed_temperature
                    )  # PLA bed temp
                    self.lcd.draw_int_value(
                        True,
                        True,
                        0,
                        self.lcd.font_8x8,
                        self.color_white,
                        self.color_background_black,
                        3,
                        200,
                        self.MBASE(i) - 8,
                        self.pd.material_preset[0].bed_temp,
                    )
                if self.pd.HAS_FAN:
                    i += 1
                    self.Draw_Menu_Line_With_Only_Icons(
                        i, self.icon_FanSpeed, self.icon_TEXT_pla_fan_speed
                    )  # PLA fan speed
                    self.lcd.draw_int_value(
                        True,
                        True,
                        0,
                        self.lcd.font_8x8,
                        self.color_white,
                        self.color_background_black,
                        3,
                        200,
                        self.MBASE(i) - 8,
                        self.pd.material_preset[0].fan_speed,
                    )
                i += 1
                self.Draw_Menu_Line_With_Only_Icons(
                    i, self.icon_write_eeprom, self.icon_TEXT_save_pla_parameters
                )  # Save PLA configuration
            elif self.select_temp.now == self.TEMP_CASE_TPU:  # TPU preheat setting
                self.checkkey = self.TPUPreheat
                self.select_TPU.reset()
                self.pd.HMI_ValueStruct.show_mode = -3
                self.Clear_Main_Window()
                self.lcd.draw_icon(
                    False,
                    self.selected_language,
                    self.icon_TEXT_header_TPU_settings,
                    self.HEADER_HEIGHT,
                    1,
                )

                self.Draw_Back_First()
                i = 1
                self.Draw_Menu_Line_With_Only_Icons(
                    i, self.icon_SetEndTemp, self.icon_TEXT_tpu_nozzle_temperature
                )  # TPU nozzle temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_white,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(i) - 8,
                    self.pd.material_preset[1].hotend_temp,
                )
                if self.pd.HAS_HEATED_BED:
                    i += 1
                    self.Draw_Menu_Line_With_Only_Icons(
                        i, self.icon_SetBedTemp, self.icon_TEXT_tpu_bed_temperature
                    )  # TPU bed temp
                    self.lcd.draw_int_value(
                        True,
                        True,
                        0,
                        self.lcd.font_8x8,
                        self.color_white,
                        self.color_background_black,
                        3,
                        200,
                        self.MBASE(i) - 8,
                        self.pd.material_preset[1].bed_temp,
                    )
                if self.pd.HAS_FAN:
                    i += 1
                    self.Draw_Menu_Line_With_Only_Icons(
                        i, self.icon_FanSpeed, self.icon_TEXT_tpu_fan_speed
                    )  # TPU fan speed
                    self.lcd.draw_int_value(
                        True,
                        True,
                        0,
                        self.lcd.font_8x8,
                        self.color_white,
                        self.color_background_black,
                        3,
                        200,
                        self.MBASE(i) - 8,
                        self.pd.material_preset[1].fan_speed,
                    )
                i += 1
                self.Draw_Menu_Line_With_Only_Icons(
                    i, self.icon_write_eeprom, self.icon_TEXT_save_tpu_parameters
                )  # Save TPU configuration

    def HMI_PLAPreheatSetting(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        # Avoid flicker by updating only the previous menu
        elif encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_PLA.inc(1 + self.PREHEAT_CASE_TOTAL):
                self.Move_Highlight(1, self.select_PLA.now)
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_PLA.dec():
                self.Move_Highlight(-1, self.select_PLA.now)
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_PLA.now == 0:  # Back
                self.checkkey = self.TemperatureID
                self.select_temp.now = self.TEMP_CASE_PLA
                self.pd.HMI_ValueStruct.show_mode = -1
                self.Draw_Temperature_Menu()
            elif self.select_PLA.now == self.PREHEAT_CASE_TEMP:  # Nozzle temperature
                self.checkkey = self.ETemp
                self.pd.HMI_ValueStruct.E_Temp = self.pd.material_preset[0].hotend_temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(self.PREHEAT_CASE_TEMP) - 8,
                    self.pd.material_preset[0].hotend_temp,
                )
                self.EncoderRateLimit = False
            elif self.select_PLA.now == self.PREHEAT_CASE_BED:  # Bed temperature
                self.checkkey = self.BedTemp
                self.pd.HMI_ValueStruct.Bed_Temp = self.pd.material_preset[0].bed_temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(self.PREHEAT_CASE_BED) - 8,
                    self.pd.material_preset[0].bed_temp,
                )
                self.EncoderRateLimit = False
            elif self.select_PLA.now == self.PREHEAT_CASE_FAN:  # Fan speed
                self.checkkey = self.FanSpeed
                self.pd.HMI_ValueStruct.Fan_speed = self.pd.material_preset[0].fan_speed
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(self.PREHEAT_CASE_FAN) - 8,
                    self.pd.material_preset[0].fan_speed,
                )
                self.EncoderRateLimit = False
            elif (
                self.select_PLA.now == self.PREHEAT_CASE_SAVE
            ):  # Save PLA configuration
                success = self.pd.save_settings()
                #self.HMI_AudioFeedback(success)

    def HMI_TPUPreheatSetting(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        # Avoid flicker by updating only the previous menu
        elif encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_TPU.inc(1 + self.PREHEAT_CASE_TOTAL):
                self.Move_Highlight(1, self.select_TPU.now)
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_TPU.dec():
                self.Move_Highlight(-1, self.select_TPU.now)
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_TPU.now == 0:  # Back
                self.checkkey = self.TemperatureID
                self.select_temp.now = self.TEMP_CASE_TPU
                self.pd.HMI_ValueStruct.show_mode = -1
                self.Draw_Temperature_Menu()

            elif self.select_TPU.now == self.PREHEAT_CASE_TEMP:  # Nozzle temperature
                self.checkkey = self.ETemp
                self.pd.HMI_ValueStruct.E_Temp = self.pd.material_preset[1].hotend_temp
                print(self.pd.HMI_ValueStruct.E_Temp)
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(self.PREHEAT_CASE_TEMP) - 8,
                    self.pd.material_preset[1].hotend_temp,
                )
                self.EncoderRateLimit = False
            elif self.select_TPU.now == self.PREHEAT_CASE_BED:  # Bed temperature
                self.checkkey = self.BedTemp
                self.pd.HMI_ValueStruct.Bed_Temp = self.pd.material_preset[1].bed_temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(self.PREHEAT_CASE_BED) - 8,
                    self.pd.material_preset[1].bed_temp,
                )
                self.EncoderRateLimit = False
            elif self.select_TPU.now == self.PREHEAT_CASE_FAN:  # Fan speed
                self.checkkey = self.FanSpeed
                self.pd.HMI_ValueStruct.Fan_speed = self.pd.material_preset[1].fan_speed
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(self.PREHEAT_CASE_FAN) - 8,
                    self.pd.material_preset[1].fan_speed,
                )
                self.EncoderRateLimit = False
            elif (
                self.select_TPU.now == self.PREHEAT_CASE_SAVE
            ):  # Save PLA configuration
                success = self.pd.save_settings()
                #self.HMI_AudioFeedback(success)

    def HMI_ETemp(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        if self.pd.HMI_ValueStruct.show_mode == -1:
            temp_line = self.TEMP_CASE_TEMP
        elif self.pd.HMI_ValueStruct.show_mode == -2:
            temp_line = self.PREHEAT_CASE_TEMP
        elif self.pd.HMI_ValueStruct.show_mode == -3:
            temp_line = self.PREHEAT_CASE_TEMP
        else:
            temp_line = self.TUNE_CASE_TEMP + self.MROWS - self.index_tune

        if encoder_state == self.ENCODER_DIFF_ENTER:
            self.EncoderRateLimit = True
            if self.pd.HMI_ValueStruct.show_mode == -1:  # temperature
                self.checkkey = self.TemperatureID
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(temp_line) - 8,
                    self.pd.HMI_ValueStruct.E_Temp,
                )
                self.pd.setExtTemp(self.pd.HMI_ValueStruct.E_Temp)
            elif self.pd.HMI_ValueStruct.show_mode == -2:
                self.checkkey = self.PLAPreheat
                self.pd.material_preset[0].hotend_temp = self.pd.HMI_ValueStruct.E_Temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(temp_line) - 8,
                    self.pd.material_preset[0].hotend_temp,
                )
                return
            elif self.pd.HMI_ValueStruct.show_mode == -3:
                self.checkkey = self.TPUPreheat
                self.pd.material_preset[1].hotend_temp = self.pd.HMI_ValueStruct.E_Temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(temp_line) - 8,
                    self.pd.material_preset[1].hotend_temp,
                )
                return
            else:  # tune
                self.checkkey = self.Tune
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(temp_line) - 8,
                    self.pd.HMI_ValueStruct.E_Temp,
                )
                # self.pd.setTargetHotend(self.pd.HMI_ValueStruct.E_Temp, 0)
            return
        elif encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.E_Temp += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.E_Temp -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.E_Temp += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.E_Temp -= 10

        # E_Temp limit
        if self.pd.HMI_ValueStruct.E_Temp > self.pd.MAX_E_TEMP:
            self.pd.HMI_ValueStruct.E_Temp = self.pd.MAX_E_TEMP
        if self.pd.HMI_ValueStruct.E_Temp < self.pd.MIN_E_TEMP:
            self.pd.HMI_ValueStruct.E_Temp = self.pd.MIN_E_TEMP
        # E_Temp value
        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_yellow,
            self.color_background_black,
            3,
            200,
            self.MBASE(temp_line) - 8,
            self.pd.HMI_ValueStruct.E_Temp,
        )

    def HMI_BedTemp(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

        if self.pd.HMI_ValueStruct.show_mode == -1:
            bed_line = self.TEMP_CASE_BED
        elif self.pd.HMI_ValueStruct.show_mode == -2:
            bed_line = self.PREHEAT_CASE_BED
        elif self.pd.HMI_ValueStruct.show_mode == -3:
            bed_line = self.PREHEAT_CASE_BED
        else:
            bed_line = self.TUNE_CASE_TEMP + self.MROWS - self.index_tune

        if encoder_state == self.ENCODER_DIFF_ENTER:
            self.EncoderRateLimit = True
            if self.pd.HMI_ValueStruct.show_mode == -1:  # temperature
                self.checkkey = self.TemperatureID
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(bed_line) - 8,
                    self.pd.HMI_ValueStruct.Bed_Temp,
                )
                self.pd.setBedTemp(self.pd.HMI_ValueStruct.Bed_Temp)
            elif self.pd.HMI_ValueStruct.show_mode == -2:
                self.checkkey = self.PLAPreheat
                self.pd.material_preset[0].bed_temp = self.pd.HMI_ValueStruct.Bed_Temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(bed_line) - 8,
                    self.pd.material_preset[0].bed_temp,
                )
                return
            elif self.pd.HMI_ValueStruct.show_mode == -3:
                self.checkkey = self.TPUPreheat
                self.pd.material_preset[1].bed_temp = self.pd.HMI_ValueStruct.Bed_Temp
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(bed_line) - 8,
                    self.pd.material_preset[1].bed_temp,
                )
                return
            else:  # tune
                self.checkkey = self.Tune
                self.lcd.draw_int_value(
                    True,
                    True,
                    0,
                    self.lcd.font_8x8,
                    self.color_yellow,
                    self.color_background_black,
                    3,
                    200,
                    self.MBASE(bed_line) - 8,
                    self.pd.HMI_ValueStruct.Bed_Temp,
                )
                # self.pd.setTargetHotend(self.pd.HMI_ValueStruct.Bed_Temp, 0)
            return
        elif encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.Bed_Temp += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.Bed_Temp -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.Bed_Temp += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.Bed_Temp -= 10

        # Bed_Temp limit
        if self.pd.HMI_ValueStruct.Bed_Temp > self.pd.BED_MAX_TARGET:
            self.pd.HMI_ValueStruct.Bed_Temp = self.pd.BED_MAX_TARGET
        if self.pd.HMI_ValueStruct.Bed_Temp < self.pd.MIN_BED_TEMP:
            self.pd.HMI_ValueStruct.Bed_Temp = self.pd.MIN_BED_TEMP
        # Bed_Temp value
        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_yellow,
            self.color_background_black,
            3,
            200,
            self.MBASE(bed_line) - 8,
            self.pd.HMI_ValueStruct.Bed_Temp,
        )

    # ---------------------Todo--------------------------------#

    def HMI_Motion(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        if encoder_state == self.ENCODER_DIFF_CW or encoder_state == self.ENCODER_DIFF_FAST_CW:
            if self.select_motion.inc(1 + self.MOTION_CASE_TOTAL):
                self.Move_Highlight(1, self.select_motion.now)
        elif encoder_state == self.ENCODER_DIFF_CCW or encoder_state == self.ENCODER_DIFF_FAST_CCW:
            if self.select_motion.dec():
                self.Move_Highlight(-1, self.select_motion.now)
        elif encoder_state == self.ENCODER_DIFF_ENTER:
            if self.select_motion.now == 0:  # back
                self.checkkey = self.Control
                self.select_control.set(self.CONTROL_CASE_MOVE)
                self.index_control = self.MROWS
                self.Draw_Control_Menu()
            else:
                # Max speed, acceleration and steps per mm menu
                # are not implemented yet. Popup a "feature not
                # available" window and return to motion menu
                # when confirm is pressed.
                self.popup_caller = self.Motion
                self.checkkey = self.FeatureNotAvailable
                self.Draw_FeatureNotAvailable_Popup()

    def HMI_Zoffset(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return
        zoff_line = 0
        if self.pd.HMI_ValueStruct.show_mode == -4:
            zoff_line = self.PREPARE_CASE_ZOFF + self.MROWS - self.index_prepare
        else:
            zoff_line = self.TUNE_CASE_ZOFF + self.MROWS - self.index_tune

        if encoder_state == self.ENCODER_DIFF_ENTER:
            self.EncoderRateLimit = True
            if self.pd.HAS_BED_PROBE:
                self.pd.offset_z(self.dwin_zoffset)
            else:
                self.pd.setZOffset(self.dwin_zoffset)  # manually set

            self.checkkey = (
                self.Prepare if self.pd.HMI_ValueStruct.show_mode == -4 else self.Tune
            )
            self.lcd.draw_signed_float(
                True,
                self.lcd.font_8x8,
                self.color_yellow,
                self.color_background_black,
                2,
                3,
                175,
                self.MBASE(zoff_line) - 10,
                self.pd.HMI_ValueStruct.offset_value,
            )

            return
        elif encoder_state == self.ENCODER_DIFF_CW:
            self.pd.HMI_ValueStruct.offset_value += 1
        elif encoder_state == self.ENCODER_DIFF_CCW:
            self.pd.HMI_ValueStruct.offset_value -= 1
        elif encoder_state == self.ENCODER_DIFF_FAST_CW:
            self.pd.HMI_ValueStruct.offset_value += 10
        elif encoder_state == self.ENCODER_DIFF_FAST_CCW:
            self.pd.HMI_ValueStruct.offset_value -= 10

        if (
            self.pd.HMI_ValueStruct.offset_value
            < (self.pd.Z_PROBE_OFFSET_RANGE_MIN) * 1000
        ):
            self.pd.HMI_ValueStruct.offset_value = (
                self.pd.Z_PROBE_OFFSET_RANGE_MIN * 1000
            )
        elif (
            self.pd.HMI_ValueStruct.offset_value
            > (self.pd.Z_PROBE_OFFSET_RANGE_MAX) * 1000
        ):
            self.pd.HMI_ValueStruct.offset_value = (
                self.pd.Z_PROBE_OFFSET_RANGE_MAX * 1000
            )

        self.last_zoffset = self.dwin_zoffset
        self.dwin_zoffset = self.pd.HMI_ValueStruct.offset_value / 1000.0
        if self.pd.HAS_BED_PROBE:
            self.pd.add_mm("Z", self.dwin_zoffset - self.last_zoffset)

        self.lcd.draw_signed_float(
            True,
            self.lcd.font_8x8,
            self.color_yellow,
            self.color_background_black,
            2,
            3,
            175,
            self.MBASE(zoff_line) - 10,
            self.pd.HMI_ValueStruct.offset_value,
        )

    def HMI_MaxSpeed(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

    def HMI_MaxAcceleration(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

    def HMI_MaxJerk(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

    def HMI_Step(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

    def HMI_MaxFeedspeedXYZE(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

    def HMI_MaxAccelerationXYZE(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

    def HMI_MaxJerkXYZE(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return

    def HMI_StepXYZE(self):
        encoder_state = self.get_encoder_state()
        if encoder_state == self.ENCODER_DIFF_NO:
            return


    # --------------------------------------------------------------#

    def Draw_Status_Area(self, with_update=True):
        #  Clear the bottom area of the screen
        self.lcd.draw_rectangle(
            1,
            self.color_background_black,
            0,
            self.STATUS_Y,
            self.lcd.screen_width,
            self.lcd.screen_height - 1,
        )

        # nozzle temp area
        if self.pd.nozzleIsHeating():
            self.lcd.draw_icon(True, self.GIF_ICON, self.icon_nozzle_heating_0, 6, 262)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_hotend_temp, 6, 262)

        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_yellow if self.pd.nozzleIsHeating() else self.color_white,
            self.color_background_black,
            3,
            26,
            268,
            self.pd.thermalManager["temp_hotend"][0]["celsius"],
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            26 + 3 * self.STAT_CHR_W + 4,
            268,
            "/",
        )
        self.lcd.draw_int_value(
            False,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            26 + 3 * self.STAT_CHR_W + 5,
            268,
            self.pd.thermalManager["temp_hotend"][0]["target"],
        )

        # bed temp area
        if self.pd.bedIsHeating():
            self.lcd.draw_icon(True, self.GIF_ICON, self.icon_bed_heating_0, 6, 294)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_bedtemp, 6, 294)

        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_yellow if self.pd.bedIsHeating() else self.color_white,
            self.color_background_black,
            3,
            26,
            300,
            self.pd.thermalManager["temp_bed"]["celsius"],
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            26 + 3 * self.STAT_CHR_W + 4,
            300,
            "/",
        )
        self.lcd.draw_int_value(
            False,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            26 + 3 * self.STAT_CHR_W + 5,
            300,
            self.pd.thermalManager["temp_bed"]["target"],
        )

        # speed area
        self.lcd.draw_icon(True, self.ICON, self.icon_speed, 99, 262)
        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            99 + 2 * self.STAT_CHR_W,
            268,
            self.pd.feedrate_percentage,
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            99 + 5 * self.STAT_CHR_W + 2,
            268,
            "%",
        )

        # extrude area
        self.lcd.draw_icon(True, self.ICON, self.icon_MaxSpeedE, 99, 294)
        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            99 + 2 * self.STAT_CHR_W,
            300,
            self.pd.extrusion_multiplier,
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            99 + 5 * self.STAT_CHR_W + 2,
            300,
            "%",
        )

        # fan speed area
        self.lcd.draw_icon(True, self.ICON, self.icon_FanSpeed, 165, 262)
        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            165 + 2 * self.STAT_CHR_W,
            268,
            self.pd.fan_speed,
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            165 + 5 * self.STAT_CHR_W + 2,
            268,
            "%",
        )

        # Z offset area
        self.lcd.draw_icon(True, self.ICON, self.icon_z_offset, 165, 294)
        self.lcd.draw_signed_float(
            True,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            1,
            3,
            191,
            300,
            self.pd.BABY_Z_VAR * 1000,
        )

        if with_update:
            time.sleep(0.005)

    def Draw_Title(self, title):
        self.lcd.draw_string(
            False,
            self.lcd.font_12x24,
            self.color_white,
            self.color_background_grey,
            14,
            4,
            title,
        )

    def Draw_Popup_Bkgd_105(self):
        self.lcd.draw_rectangle(
            1,
            self.color_popup_background,
            6,
            self.HEADER_HEIGHT + 6,
            self.lcd.screen_width - 6,
            self.STATUS_Y + 78,
        )

    def Draw_More_Icon(self, line):
        self.lcd.draw_icon(True, self.ICON, self.icon_more, 206, self.MBASE(line) - 14)

    def Draw_Menu_Cursor(self, line):
        self.lcd.draw_rectangle(
            1,
            self.Rectangle_color,
            0,
            self.MBASE(line) - 18,
            10,
            self.MBASE(line + 1) - 20,
        )

    def Draw_Menu_Icon(self, line, icon):
        self.lcd.draw_icon(True, self.ICON, icon, 20, self.MBASE(line) - 14)

    def Draw_Menu_Text_Icon(self, line, text_icon):
        self.lcd.draw_icon(
            True, self.selected_language, text_icon, self.LBLX, self.MBASE(line) - 16
        )

    def Draw_Menu_Line(self, line, icon=False, label=False):
        if label:
            self.lcd.draw_rectangle(
                1,
                self.color_background_black,
                self.LBLX,
                self.MBASE(line) - 5,
                self.lcd.screen_width,
                self.MBASE(line + 1) - 22,
            )
            self.lcd.draw_string(
                True,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                self.LBLX,
                self.MBASE(line) - 5,
                label,
            )
        if icon:
            self.Draw_Menu_Icon(line, icon)
        self.lcd.draw_line(
            self.color_line,
            15,
            self.MBASE(line + 1) - 22,
            235,
            self.MBASE(line + 1) - 22,
        )

    def Draw_Menu_Line_With_Only_Icons(
        self, line, icon_left=False, text_icon=False, dividing_line=True
    ):
        if icon_left:
            self.Draw_Menu_Icon(line, icon_left)
        if text_icon:
            self.lcd.draw_rectangle(
                1,
                self.color_background_black,
                self.LBLX,
                self.MBASE(line) - 5,
                self.lcd.screen_width,
                self.MBASE(line + 1) - 22,
            )
            self.Draw_Menu_Text_Icon(line, text_icon)
        if dividing_line:
            self.lcd.draw_line(
                self.color_line,
                15,
                self.MBASE(line + 1) - 22,
                235,
                self.MBASE(line + 1) - 22,
            )

    # Draw "Back" line at the top
    def Draw_Back_First(self, is_sel=True):
        self.Draw_Menu_Line_With_Only_Icons(0, self.icon_back, self.icon_TEXT_back)

        if is_sel:
            self.Draw_Menu_Cursor(0)

    def draw_move_en(self, line):
        self.lcd.move_screen_area(1, 69, 61, 102, 71, self.LBLX, line)  # "Move"

    def draw_max_en(self, line):
        self.lcd.move_screen_area(1, 245, 119, 269, 129, self.LBLX, line)  # "Max"

    def draw_max_accel_en(self, line):
        self.draw_max_en(line)
        self.lcd.move_screen_area(
            1, 1, 135, 79, 145, self.LBLX + 27, line
        )  # "Acceleration"

    def draw_speed_en(self, inset, line):
        self.lcd.move_screen_area(
            1, 184, 119, 224, 132, self.LBLX + inset, line
        )  # "Speed"

    def draw_jerk_en(self, line):
        self.lcd.move_screen_area(1, 64, 119, 106, 129, self.LBLX + 27, line)  # "Jerk"

    def draw_steps_per_mm(self, line):
        self.lcd.move_screen_area(
            1, 1, 151, 101, 161, self.LBLX, line
        )  # "Steps-per-mm"

    # Display an SD item
    def Draw_SDItem(self, item, row=0):
        fl = self.pd.GetFiles()[item]
        if len(self.pd.fl[item].split('/')) > self.pd.subdirIndex + 1:
            self.Draw_Menu_Line(row, self.icon_more, fl)
        else:
            self.Draw_Menu_Line(row, self.icon_file, fl)

    # Display a custom macro item
    def Draw_CustomMacroItem(self, item, row=0):
        macro = self.custom_macros[item]
        self.Draw_Menu_Line(row, macro["icon"], macro["label"])

    def Draw_Confirm_Cancel_Buttons(self):
        if self.select_confirm.now == 1:
            c1 = self.color_white
            c2 = self.color_popup_background
        elif self.select_cancel.now == 1:
            c1 = self.color_popup_background
            c2 = self.color_white
        else:
            c1 = self.color_popup_background
            c2 = self.color_popup_background
        self.lcd.draw_rectangle(1, c1, 28, 152, 113, 187)
        self.lcd.draw_rectangle(1, c2, 128, 152, 213, 187)
        self.lcd.draw_icon(
            True,
            self.selected_language,
            self.icon_confirm_button,
            30,
            self.HEADER_HEIGHT + 130,
        )
        self.lcd.draw_icon(
            True,
            self.selected_language,
            self.icon_cancel_button,
            130,
            self.HEADER_HEIGHT + 130,
        )

    def Draw_Print_Confirmation_Buttons(self):
        if self.select_confirm.now:
            c1 = self.color_white
            c2 = self.color_popup_background
        elif self.select_cancel.now == 1:
            c1 = self.color_popup_background
            c2 = self.color_white
        else:
            c1 = self.color_popup_background
            c2 = self.color_popup_background
        self.lcd.draw_rectangle(1, c1, 12, 225, 97, 260)
        self.lcd.draw_rectangle(1, c2, 136, 225, 221, 260)
        self.lcd.draw_icon(
            True,
            self.selected_language,
            self.icon_confirm_button,
            14,
            227
        )
        self.lcd.draw_icon(
            True,
            self.selected_language,
            self.icon_cancel_button,
            138,
            227
        )          

    def Draw_Printing_Screen(self):
        # Tune
        self.lcd.draw_icon(True, self.ICON, self.icon_tune, 12, 191)
        self.lcd.draw_icon(False, self.selected_language, self.icon_TEXT_Tune, 12, 225)
        # Pause
        self.lcd.draw_icon(True, self.ICON, self.icon_pause, 86, 191)
        self.lcd.draw_icon(False, self.selected_language, self.icon_TEXT_Pause, 86, 225)
        # Stop
        self.lcd.draw_icon(True, self.ICON, self.icon_stop, 160, 191)
        self.lcd.draw_icon(False, self.selected_language, self.icon_TEXT_Stop, 160, 225)
        # Print elapsed time
        self.lcd.draw_icon(True, self.ICON, self.icon_print_time, 117, 77)
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_text_printing_time, 170, 61
        )
        # Print remain time
        self.lcd.draw_icon(True, self.ICON, self.icon_remain_time, 117, 138)
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_text_remain_time, 170, 122
        )

    def Draw_Print_ProgressBar(self, Percentrecord=None):
        if not Percentrecord:
            Percentrecord = self.pd.getPercent()
        progress_icon_id = self.icon_progress_0 + Percentrecord
        self.lcd.draw_icon(True, self.GIF_ICON, progress_icon_id, 12, 75)

    def Draw_Print_ProgressElapsed(self):
        elapsed = self.pd.duration()  # print timer
        self.lcd.draw_int_value(
            True,
            True,
            1,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            2,
            160,
            100,
            elapsed / 3600,
        )
        self.lcd.draw_string(
            True,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            183,
            100,
            ":",
        )
        self.lcd.draw_int_value(
            True,
            True,
            1,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            2,
            190,
            100,
            (elapsed % 3600) / 60,
        )

    def Draw_Print_ProgressRemain(self):
        remain_time = self.pd.remain()
        if not remain_time:
            return  # time remaining is None during warmup.
        self.lcd.draw_int_value(
            True,
            True,
            1,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            2,
            160,
            166,
            remain_time / 3600,
        )
        self.lcd.draw_string(
            True,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            183,
            166,
            ":",
        )
        self.lcd.draw_int_value(
            True,
            True,
            1,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            2,
            190,
            166,
            (remain_time % 3600) / 60,
        )

    def Draw_Print_Preview(self):
        self.Clear_Main_Window()

        

        self.lcd.draw_string_centered(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            self.MENU_CHR_W,
            self.MENU_CHR_W,
            self.lcd.screen_width / 2,
            self.HEADER_HEIGHT / 2,
            self.pd.selectedFile.split('/')[-1][:28]
        )

        self.lcd.draw_icon(   # Draw preview image
            False,
            self.ICON,
            self.icon_preview_placeholder,
            self.lcd.screen_width / 2 - 48,
            30
        )

        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_estimated_time,
            12, 134
        )
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_filament_used,
            12, 159
        )
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_layer_height,
            12, 184
        )

        self.pd.scanMetadata()

        # Show print information
        self.lcd.draw_string(
            False,
            self.lcd.font_6x12,
            self.color_white,
            self.color_background_black,
            self.lcd.screen_width - 80 if self.pd.metadata['estimated_time'] else 205,
            134 + 5,
            self.pd.metadata['estimated_time'] if self.pd.metadata['estimated_time'] else "N/A"
        )

        self.lcd.draw_string(
            False,
            self.lcd.font_6x12,
            self.color_white,
            self.color_background_black,
            self.lcd.screen_width - 80 if self.pd.metadata['filament_used'] else 205,
            159 + 5,
            self.pd.metadata['filament_used'] if self.pd.metadata['filament_used'] else "N/A"
        )

        self.lcd.draw_string(
            False,
            self.lcd.font_6x12,
            self.color_white,
            self.color_background_black,
            self.lcd.screen_width - 80 if self.pd.metadata['layer_height'] else 205,
            184 + 5,
            self.pd.metadata['layer_height'] if self.pd.metadata['layer_height'] else 'N/A'
        )

        self.select_confirm.set(1)
        self.select_cancel.reset()

        self.Draw_Print_Confirmation_Buttons()

        self.Draw_Status_Area()

    def Draw_Print_File_Menu(self):
        self.Clear_Title_Bar()
        # Draw "File Selection" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_file_selection,
            self.HEADER_HEIGHT,
            1,
        )
        self.Redraw_SD_List()
        self.Draw_Status_Area()

    def Draw_Prepare_Menu(self):
        self.Clear_Main_Window()
        # Draw "Prepare" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_prepare,
            self.HEADER_HEIGHT,
            1,
        )

        scroll = self.MROWS - self.index_prepare
        # self.Frame_TitleCopy(1, 178, 2, 229, 14)  # "Prepare"
        self.Draw_Back_First(self.select_prepare.now == 0)  # < Back
        if scroll + self.PREPARE_CASE_MOVE <= self.MROWS:
            self.Item_Prepare_Move(self.PREPARE_CASE_MOVE)  # Move >
        if scroll + self.PREPARE_CASE_DISA <= self.MROWS:
            self.Item_Prepare_Disable(self.PREPARE_CASE_DISA)  # Disable Stepper
        if scroll + self.PREPARE_CASE_HOME <= self.MROWS:
            self.Item_Prepare_Home(self.PREPARE_CASE_HOME)  # Auto Home
        if self.pd.HAS_ZOFFSET_ITEM:
            if scroll + self.PREPARE_CASE_ZOFF <= self.MROWS:
                self.Item_Prepare_Offset(
                    self.PREPARE_CASE_ZOFF
                )  # Edit Z-Offset / Babystep / Set Home Offset
        if self.pd.HAS_HOTEND:
            if scroll + self.PREPARE_CASE_PLA <= self.MROWS:
                self.Item_Prepare_PLA(self.PREPARE_CASE_PLA)  # Preheat PLA
            if scroll + self.PREPARE_CASE_TPU <= self.MROWS:
                self.Item_Prepare_TPU(self.PREPARE_CASE_TPU)  # Preheat TPU
        if self.pd.HAS_PREHEAT:
            if scroll + self.PREPARE_CASE_COOL <= self.MROWS:
                self.Item_Prepare_Cool(self.PREPARE_CASE_COOL)  # Cooldown
        if self.select_prepare.now:
            self.Draw_Menu_Cursor(self.select_prepare.now)
        self.Draw_Status_Area()

    def Draw_Control_Menu(self):
        self.Clear_Main_Window()
        # Draw "Control" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_control,
            self.HEADER_HEIGHT,
            1,
        )

        self.Draw_Back_First(self.select_control.now == 0)

        # self.Frame_TitleCopy(1, 128, 2, 176, 12)
        # self.lcd.move_screen_area(1, 1, 89, 83, 101, self.LBLX, self.MBASE(self.CONTROL_CASE_TEMP))  # Temperature >
        # self.lcd.move_screen_area(1, 84, 89, 128, 99, self.LBLX, self.MBASE(self.CONTROL_CASE_MOVE))  # Motion >
        # self.lcd.move_screen_area(1, 0, 104, 25, 115, self.LBLX, self.MBASE(self.CONTROL_CASE_INFO))  # Info >

        if self.select_control.now and self.select_control.now < self.MROWS:
            self.Draw_Menu_Cursor(self.select_control.now)

        # # Draw icons and lines
        self.Draw_Menu_Line_With_Only_Icons(
            1, self.icon_temperature, self.icon_TEXT_temperature
        )
        self.Draw_More_Icon(1)
        self.Draw_Menu_Line_With_Only_Icons(2, self.icon_motion, self.icon_TEXT_motion)
        self.Draw_More_Icon(2)
        self.Draw_Menu_Line_With_Only_Icons(3, self.icon_info, self.icon_TEXT_Info)
        self.Draw_More_Icon(3)
        self.Draw_Status_Area()

    def Draw_Misc_Menu(self):
        # Draw "Miscellaneous" on header
        self.lcd.draw_string_centered(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            self.MENU_CHR_W,
            self.MENU_CHR_W,
            self.lcd.screen_width / 2,
            self.HEADER_HEIGHT / 2,
            "Miscellaneous"
        )

        self.select_misc.reset()
        self.index_misc = self.MROWS
        self.Clear_Menu_Area() # Leave title bar unchanged, clear only middle of screen
        self.Draw_Back_First()

        custom_macro_count = len(self.custom_macros)
        if custom_macro_count > 0:
            if custom_macro_count > self.MROWS:
                custom_macro_count = self.MROWS
            for i in range(custom_macro_count):
                self.Draw_CustomMacroItem(i, i + 1)
        else:
            self.lcd.draw_string_centered(
                False,
                self.lcd.font_16x32,
                self.color_white,
                self.color_background_black,
                14,
                32,
                self.lcd.screen_width / 2,
                self.lcd.screen_height / 2,
                "No macros"
            )

            self.lcd.draw_string_centered(
                False,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                self.MENU_CHR_W,
                0,
                self.lcd.screen_width / 2,
                self.lcd.screen_height / 2 + 32,
                "Please follow WIKI"
            )

            self.lcd.draw_string_centered(
                False,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                self.MENU_CHR_W,
                0,
                self.lcd.screen_width / 2,
                self.lcd.screen_height / 2 + 45,
                "to include macros here"
            )

    def Draw_Info_Menu(self):
        """
        Draws the "Info" menu on the display.
        As the text stays on the bottom of each line instead of
        a normal menu item, this is manually drawn.
        """
        self.Clear_Main_Window()
        # Draw "Info" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_info,
            self.HEADER_HEIGHT,
            1,
        )

        self.Draw_Back_First()

        # Bed size 80,95,110,140,155,170,200,215,230,260
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_TEXT_bed_size, self.LBLX, 75
        )
        self.lcd.draw_icon(True, self.ICON, self.icon_PrintSize, 20, 90)
        self.lcd.draw_string(
            False,
            self.lcd.font_6x12,
            self.color_white,
            self.color_background_black,
            70,
            105,
            self.pd.MACHINE_SIZE,
        )

        # Klipper version
        self.lcd.draw_icon(
            True,
            self.selected_language,
            self.icon_TEXT_hardware_version,
            self.LBLX,
            135,
        )
        self.lcd.draw_icon(True, self.ICON, self.icon_Version, 20, 140)
        self.lcd.draw_string(
            False,
            self.lcd.font_6x12,
            self.color_white,
            self.color_background_black,
            50,
            155,
            "Klipper " + self.pd.SHORT_BUILD_VERSION,
        )

        # Contact details
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_TEXT_contact, self.LBLX, 185
        )
        self.lcd.draw_icon(True, self.ICON, self.icon_Contact, 20, 200)
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            50,
            215,
            "github.com/jpcurti/",
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            30,
            230,
            "ender3-v3-se-klipper-with-display",
        )
        self.Draw_Status_Area()

    def Draw_Tune_Menu(self):
        self.Clear_Main_Window()
        # Draw "Tune" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_tune,
            self.HEADER_HEIGHT,
            1,
        )
        self.lcd.move_screen_area(1, 94, 2, 126, 12, 14, 9)
        self.lcd.move_screen_area(
            1, 1, 179, 92, 190, self.LBLX, self.MBASE(self.TUNE_CASE_SPEED)
        )  # Print speed
        if self.pd.HAS_HOTEND:
            self.lcd.move_screen_area(
                1, 197, 104, 238, 114, self.LBLX, self.MBASE(self.TUNE_CASE_TEMP)
            )  # Hotend...
            self.lcd.move_screen_area(
                1, 1, 89, 83, 101, self.LBLX + 44, self.MBASE(self.TUNE_CASE_TEMP)
            )  # Temperature
        if self.pd.HAS_HEATED_BED:
            self.lcd.move_screen_area(
                1, 240, 104, 264, 114, self.LBLX, self.MBASE(self.TUNE_CASE_BED)
            )  # Bed...
            self.lcd.move_screen_area(
                1, 1, 89, 83, 101, self.LBLX + 27, self.MBASE(self.TUNE_CASE_BED)
            )  # ...Temperature
        if self.pd.HAS_FAN:
            self.lcd.move_screen_area(
                1, 0, 119, 64, 132, self.LBLX, self.MBASE(self.TUNE_CASE_FAN)
            )  # Fan speed
        if self.pd.HAS_ZOFFSET_ITEM:
            self.lcd.move_screen_area(
                1, 93, 179, 141, 189, self.LBLX, self.MBASE(self.TUNE_CASE_ZOFF)
            )  # Z-offset
        self.Draw_Back_First(self.select_tune.now == 0)
        if self.select_tune.now:
            self.Draw_Menu_Cursor(self.select_tune.now)

        self.Draw_Menu_Line_With_Only_Icons(
            self.TUNE_CASE_SPEED, self.icon_speed, self.icon_TEXT_Printing_Speed
        )
        self.lcd.draw_int_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            200,
            self.MBASE(self.TUNE_CASE_SPEED) - 8,
            self.pd.feedrate_percentage,
        )

        if self.pd.HAS_HOTEND:
            self.Draw_Menu_Line_With_Only_Icons(
                self.TUNE_CASE_TEMP,
                self.icon_hotend_temp,
                self.icon_TEXT_nozzle_temperature,
            )
            self.lcd.draw_int_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                3,
                200,
                self.MBASE(self.TUNE_CASE_TEMP) - 8,
                self.pd.thermalManager["temp_hotend"][0]["target"],
            )

        if self.pd.HAS_HEATED_BED:
            self.Draw_Menu_Line_With_Only_Icons(
                self.TUNE_CASE_BED, self.icon_bedtemp, self.icon_TEXT_bed_temperature
            )
            self.lcd.draw_int_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                3,
                200,
                self.MBASE(self.TUNE_CASE_BED) - 8,
                self.pd.thermalManager["temp_bed"]["target"],
            )

        if self.pd.HAS_FAN:
            self.Draw_Menu_Line_With_Only_Icons(
                self.TUNE_CASE_FAN, self.icon_FanSpeed, self.icon_TEXT_fan_speed
            )
            self.lcd.draw_int_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                3,
                200,
                self.MBASE(self.TUNE_CASE_FAN) - 8,
                self.pd.thermalManager["fan_speed"][0],
            )
        if self.pd.HAS_ZOFFSET_ITEM:
            self.Draw_Menu_Line_With_Only_Icons(
                self.TUNE_CASE_ZOFF, self.icon_z_offset, self.icon_TEXT_Z_Offset
            )
            self.lcd.draw_signed_float(
                True,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                2,
                3,
                175,
                self.MBASE(self.TUNE_CASE_ZOFF) - 10,
                self.pd.BABY_Z_VAR * 100,
            )

    def Draw_Temperature_Menu(self):
        self.Clear_Main_Window()
        # Draw "Temperature" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_temperature,
            self.HEADER_HEIGHT,
            1,
        )

        self.Draw_Back_First(self.select_temp.now == 0)
        if self.select_temp.now:
            self.Draw_Menu_Cursor(self.select_temp.now)

        # Draw icons and lines
        i = 0
        if self.pd.HAS_HOTEND:
            i += 1
            # self.Draw_Menu_Line( self.TEMP_CASE_TEMP, self.icon_SetEndTemp, "Nozzle Temperature")
            self.Draw_Menu_Line_With_Only_Icons(
                self.TEMP_CASE_TEMP,
                self.icon_SetEndTemp,
                self.icon_TEXT_nozzle_temperature,
            )
            self.lcd.draw_int_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                3,
                200,
                self.MBASE(i) - 8,
                self.pd.thermalManager["temp_hotend"][0]["target"],
            )
        if self.pd.HAS_HEATED_BED:
            i += 1
            # self.Draw_Menu_Line( (self.TEMP_CASE_BED), self.icon_SetEndTemp, "Bed Temperature")
            self.Draw_Menu_Line_With_Only_Icons(
                self.TEMP_CASE_BED, self.icon_SetEndTemp, self.icon_TEXT_bed_temperature
            )
            self.lcd.draw_int_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                3,
                200,
                self.MBASE(i) - 8,
                self.pd.thermalManager["temp_bed"]["target"],
            )
        if self.pd.HAS_FAN:
            i += 1
            self.Draw_Menu_Line_With_Only_Icons(
                (self.TEMP_CASE_FAN),
                self.icon_SetEndTemp,
                self.icon_TEXT_nozzle_temperature,
            )
            self.lcd.draw_int_value(
                True,
                True,
                0,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                3,
                200,
                self.MBASE(i) - 8,
                self.pd.thermalManager["fan_speed"][0],
            )
        if self.pd.HAS_HOTEND:
            # PLA/TPU items have submenus
            i += 1
            self.Draw_Menu_Line_With_Only_Icons(
                self.TEMP_CASE_PLA,
                self.icon_SetEndTemp,
                self.icon_TEXT_preheat_pla_settings,
            )
            self.Draw_More_Icon(i)
            i += 1
            self.Draw_Menu_Line_With_Only_Icons(
                self.TEMP_CASE_TPU,
                self.icon_SetEndTemp,
                self.icon_TEXT_preheat_tpu_settings,
            )
            self.Draw_More_Icon(i)

    def Draw_Motion_Menu(self):
        self.Clear_Main_Window()
        # Draw "Motion" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_motion,
            self.HEADER_HEIGHT,
            1,
        )
        self.draw_max_en(self.MBASE(self.MOTION_CASE_RATE))
        self.draw_speed_en(27, self.MBASE(self.MOTION_CASE_RATE))  # "Max Speed"
        self.draw_max_accel_en(self.MBASE(self.MOTION_CASE_ACCEL))  # "Max Acceleration"
        self.draw_steps_per_mm(self.MBASE(self.MOTION_CASE_STEPS))  # "Steps-per-mm"

        self.Draw_Back_First(self.select_motion.now == 0)
        if self.select_motion.now:
            self.Draw_Menu_Cursor(self.select_motion.now)

        self.Draw_Menu_Line_With_Only_Icons(
            self.MOTION_CASE_RATE, self.icon_MaxSpeed, self.icon_TEXT_max_speed
        )
        self.Draw_More_Icon(self.MOTION_CASE_RATE)

        self.Draw_Menu_Line_With_Only_Icons(
            self.MOTION_CASE_ACCEL,
            self.icon_MaxAccelerated,
            self.icon_TEXT_max_acceleration,
        )
        self.Draw_More_Icon(self.MOTION_CASE_ACCEL)

        self.Draw_Menu_Line_With_Only_Icons(
            self.MOTION_CASE_STEPS, self.icon_Step, self.icon_TEXT_steps_per_mm
        )
        self.Draw_More_Icon(self.MOTION_CASE_STEPS)

    def Draw_Move_Menu(self):
        """
        Only visual: Draws the "Move" menu on the display.

        This method clears the main window and then proceeds to draw the "Move" header.
        It also draws the options for moving the X, Y, and Z axes, as well as the extruder
        option if available. It also handles drawing the menu cursor and separators.
        """
        self.Clear_Main_Window()
        self.pd.get_additional_values()
        # Draw "Move" on header
        self.lcd.draw_icon(
            False,
            self.selected_language,
            self.icon_TEXT_header_move,
            self.HEADER_HEIGHT,
            1,
        )

        self.Draw_Back_First(self.select_axis.now == 0)
        if self.select_axis.now:
            self.Draw_Menu_Cursor(self.select_axis.now)

        line_count = 1
        self.Draw_Menu_Line_With_Only_Icons(
            line_count, self.icon_move_x, self.icon_TEXT_move_x
        )
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(line_count) - 10,
            self.pd.current_position.x * self.MINUNITMULT,
        )

        line_count += 1
        self.Draw_Menu_Line_With_Only_Icons(
            line_count, self.icon_move_y, self.icon_TEXT_move_y
        )
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(line_count) - 10,
            self.pd.current_position.y * self.MINUNITMULT,
        )

        line_count += 1
        self.Draw_Menu_Line_With_Only_Icons(
            line_count, self.icon_move_z, self.icon_TEXT_move_z
        )
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(line_count) - 10,
            self.pd.current_position.z * self.MINUNITMULT,
        )

        line_count += 1
        self.Draw_Menu_Line_With_Only_Icons(
            line_count, self.icon_move_e, self.icon_TEXT_move_e
        )
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            3,
            1,
            175,
            self.MBASE(line_count) - 10,
            self.pd.current_position.e * self.MINUNITMULT,
        )

    def Draw_Manual_Probe_Menu(self, draw_static_elements=False, draw_error=False):
        if self.manual_probe == None or self.manual_probe.status["is_active"] == False:
            self.Goto_MainMenu()
            return

        PADDING = 15
        WINDOW_X = PADDING
        WINDOW_Y = self.HEADER_HEIGHT + PADDING
        WINDOW_WIDTH = self.lcd.screen_width - PADDING * 2
        line_height = self.MLINE - 10

        if draw_static_elements:
            # Draw header
            self.lcd.draw_icon(
                False,
                self.selected_language,
                self.icon_TEXT_header_leveling,
                self.HEADER_HEIGHT,
                1
            )
    
            self.lcd.draw_string(
                False,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                WINDOW_X,
                WINDOW_Y,
                "Step sizes:"
            )
            
            # Draw footer
            self.lcd.draw_string_centered(
                False,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                self.MENU_CHR_W,
                line_height,
                self.lcd.screen_width / 2.0,
                self.lcd.screen_height - line_height * 2.0,
                "Tap to toggle between steps"
            )
            
            self.lcd.draw_string_centered(
                False,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                self.MENU_CHR_W,
                line_height,
                self.lcd.screen_width / 2.0,
                self.lcd.screen_height - line_height,
                "Long press to confirm"
            )
            
        # Draw the individual steps in the selector
        step_count = len(self.MANUAL_PROBE_STEPS)
        box_margin = 5
        box_width = (WINDOW_WIDTH - (step_count - 1) * box_margin) / step_count
        line_count = 0
        for i in range(step_count):
            box_x = WINDOW_X + i * (box_width + box_margin)
            box_y = WINDOW_Y + self.MBASE(line_count) - 10
            box_bottom = WINDOW_Y + self.MBASE(line_count + 1) - 10
            box_height = box_bottom - box_y
            self.lcd.draw_rectangle(
                0,
                self.color_yellow if i == self.manual_probe_step_index else self.color_popup_background,
                box_x,
                box_y,
                box_x + box_width,
                box_bottom
            )
            
            if self.MANUAL_PROBE_STEPS[i] >= 0.1:
                # Acount for both 1.0 and 0.1 steps
                text = "%.1f" % self.MANUAL_PROBE_STEPS[i]
            else:
                # Account for 0.05 step
                text = "%.2f" % self.MANUAL_PROBE_STEPS[i]
            
            self.lcd.draw_string_centered(
                True,
                self.lcd.font_8x8,
                self.color_yellow if i == self.manual_probe_step_index else self.color_white,
                self.color_background_black,
                self.MENU_CHR_W,
                self.MENU_CHR_W,
                box_x + box_width / 2.0,
                box_y + box_height / 2.0,
                text
            )
        
        # Draw the current offset
        line_count += 2
        if not draw_static_elements:
            # No need to clear the region, as the caller will clear the whole screen
            self.lcd.draw_rectangle(
                1,
                self.color_background_black,
                WINDOW_X,
                WINDOW_Y + self.MBASE(line_count) - 10,
                WINDOW_X + WINDOW_WIDTH,
                WINDOW_Y + self.MBASE(line_count + 1) - 10
            )
        self.lcd.draw_float_value(
            True,
            True,
            0,
            self.lcd.font_8x16,
            self.color_background_red if draw_error else self.color_yellow,
            self.color_background_black,
            3,
            1,
            WINDOW_WIDTH / 2 - (5 * self.STAT_CHR_W) / 2,
            WINDOW_Y + self.MBASE(line_count) - 10,
            self.manual_probe.status["z_position"] * self.MINUNITMULT
        )

        # Draw the error message
        line_count += 1
        self.lcd.draw_rectangle(
            1,
            self.color_background_black,
            WINDOW_X,
            WINDOW_Y + self.MBASE(line_count) - 10,
            WINDOW_X + WINDOW_WIDTH,
            WINDOW_Y + self.MBASE(line_count + 1) - 10
        )
        if draw_error:
            self.lcd.draw_string_centered(
                True,
                self.lcd.font_8x8,
                self.color_background_red,
                self.color_background_black,
                self.MENU_CHR_W,
                line_height,
                self.lcd.screen_width / 2,
                WINDOW_Y + self.MBASE(line_count) - 10,
                "Out of range!"
            )

    def Draw_IconFinder(self):
        # Draw header
        self.lcd.draw_string_centered(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            self.MENU_CHR_W,
            self.MENU_CHR_W,
            self.lcd.screen_width / 2,
            self.HEADER_HEIGHT / 2,
            f"Icon finder"
        )

        self.lcd.draw_icon(
            True,
            self.ICON,
            self.select_icon_finder.now,
            self.lcd.screen_width / 2 - 16,
            self.lcd.screen_height / 2 - 16
        )

        self.lcd.draw_string_centered(
            False,
            self.lcd.font_8x8,
            self.color_yellow,
            self.color_background_black,
            self.MENU_CHR_W,
            0,
            self.lcd.screen_width / 2,
            self.lcd.screen_height - (self.MENU_CHR_W + 15) * 2,
            "Icon %i" % self.select_icon_finder.now
        )

        # Draw footer
        self.lcd.draw_string_centered(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_background_black,
            self.MENU_CHR_W,
            0,
            self.lcd.screen_width / 2,
            self.lcd.screen_height - self.MENU_CHR_W - 15,
            "Press to exit"
        )

    def Goto_MainMenu(self):
        self.checkkey = self.MainMenu
        self.Clear_Screen()
        # Draw "Main" on header
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_TEXT_header_main, 29, 1
        )

        self.icon_Print()
        self.icon_Prepare()
        self.icon_Control()
        if self.pd.HAS_ONESTEP_LEVELING:
            self.icon_Misc(self.select_page.now == 3)
        else:
            self.icon_StartInfo(self.select_page.now == 3)

    def Goto_PrintProcess(self):
        self.checkkey = self.PrintProcess
        self.Clear_Main_Window()
        # Draw "Printing" on header
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_TEXT_header_printing, 29, 1
        )
        self.Draw_Printing_Screen()

        self.show_tune()
        if self.pd.printingIsPaused():
            self.show_continue()
        else:
            self.show_pause()
        self.show_stop()

        # Copy into filebuf string before entry
        name = self.pd.file_name
        if name:
            npos = _MAX(0, self.lcd.screen_width - len(name) * self.MENU_CHR_W) / 2
            self.lcd.draw_string(
                False,
                self.lcd.font_6x12,
                self.color_white,
                self.color_background_black,
                npos,
                40,
                name,
            )

        self.Draw_Print_ProgressBar()
        self.Draw_Print_ProgressElapsed()
        self.Draw_Print_ProgressRemain()
        self.Draw_Status_Area()

    def Goto_ManualProbe_Menu(self):
        self.checkkey = self.ManualProbeProcess
        self.Clear_Screen()
        self.Draw_Manual_Probe_Menu(draw_static_elements=True)

    # --------------------------------------------------------------#
    # --------------------------------------------------------------#

    def Clear_Title_Bar(self):
        self.lcd.draw_rectangle(
            1,
            self.color_background_grey,
            0,
            0,
            self.lcd.screen_width,
            self.HEADER_HEIGHT,
        )

    def Clear_Menu_Area(self):
        self.lcd.draw_rectangle(
            1,
            self.color_background_black,
            0,
            self.HEADER_HEIGHT,
            self.lcd.screen_width,
            self.STATUS_Y,
        )

    def Clear_Status_Area(self):
        self.lcd.draw_rectangle(
            1,
            self.color_background_black,
            0,
            self.STATUS_Y,
            self.lcd.screen_width,
            self.lcd.screen_height,
        )

    def Clear_Main_Window(self):
        self.Clear_Title_Bar()
        self.Clear_Menu_Area()

    def Clear_Screen(self):
        self.Clear_Title_Bar()
        self.Clear_Menu_Area()
        self.Clear_Status_Area()

    def Clear_Popup_Area(self):
        self.Clear_Title_Bar()
        self.lcd.draw_rectangle(
            1,
            self.color_background_black,
            0,
            31,
            self.lcd.screen_width,
            self.lcd.screen_height,
        )

    def Popup_window_PauseOrStop(self):
        self.Clear_Main_Window()
        if self.select_print.now == 1:
            self.lcd.draw_icon(
                True,
                self.selected_language,
                self.icon_popup_pause_print,
                15,
                self.HEADER_HEIGHT + 50,
            )

        elif self.select_print.now == 2:
            self.lcd.draw_icon(
                True,
                self.selected_language,
                self.icon_popup_stop_print,
                15,
                self.HEADER_HEIGHT + 50,
            )
        self.lcd.draw_rectangle(
            0, self.color_white, 15, self.HEADER_HEIGHT + 50, 225, 195
        )
        self.select_cancel.set(1) # Cancel is the default option, so leave it hovered
        self.select_confirm.reset()
        self.Draw_Confirm_Cancel_Buttons()

    def Popup_Window_Home(self):
        """
        Displays a popup window indicating that the printer is homing.
        """
        self.Clear_Main_Window()
        self.lcd.draw_icon(
            True,
            self.selected_language,
            self.icon_popup_homing,
            18,
            self.HEADER_HEIGHT + 60,
        )

    def Popup_Window_ETempTooLow(self):
        """
        Displays a popup window indicating that the extruder temperature is too low.
        """
        self.Clear_Main_Window()
        self.lcd.draw_rectangle(
            1, self.color_popup_background, 15, self.HEADER_HEIGHT + 50, 225, 195
        )
        self.lcd.draw_icon(
            True,
            self.selected_language,
            self.icon_popup_nozzle_temp_too_low,
            15,
            self.HEADER_HEIGHT + 50,
        )
        self.lcd.draw_rectangle(
            0, self.color_white, 15, self.HEADER_HEIGHT + 50, 225, 195
        )

        # Draw ok button
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_confirm_button, 80, 154
        )
        self.lcd.draw_rectangle(0, self.color_white, 80, 154, 160, 185)

    def Draw_FeatureNotAvailable_Popup(self):
        """
        Displays a popup window indicating that this feature is not available.
        """
        # self.Clear_Main_Window()
        self.lcd.draw_rectangle(
            1, self.color_popup_background, 15, self.HEADER_HEIGHT + 50, 225, 195
        )
        self.lcd.draw_rectangle(
            0, self.color_white, 15, self.HEADER_HEIGHT + 50, 225, 195
        )
        # Draw text with "Feature not available on the screen. Please use klipper"
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_popup_background,
            20,
            self.HEADER_HEIGHT + 55,
            "Feature not available on",
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_popup_background,
            20,
            self.HEADER_HEIGHT + 75,
            "the screen yet, please",
        )
        self.lcd.draw_string(
            False,
            self.lcd.font_8x8,
            self.color_white,
            self.color_popup_background,
            20,
            self.HEADER_HEIGHT + 95,
            "use the klipper interface.",
        )
        # Draw ok button
        self.lcd.draw_icon(
            True, self.selected_language, self.icon_confirm_button, 80, 154
        )
        self.lcd.draw_rectangle(0, self.color_white, 80, 154, 160, 185)

    def show_popup(self, message=""):
        try:
            if not message or len(message) == 0:
                return

            self.time_since_movement = 0
            if self.checkkey != self.MessagePopup:
                self.popup_caller = self.checkkey
            self.checkkey = self.MessagePopup

            # Given the message length, split it into multiple lines
            padding = 15
            max_line = int((self.lcd.screen_width - padding * 4) / self.MENU_CHR_W)
            lines = textwrap.wrap(message, width=max_line)

            # Calculate sizing
            line_height = 18
            button_height = 31
            height = line_height * len(lines) + button_height + padding * 3
            y = (self.lcd.screen_height - self.HEADER_HEIGHT) / 2 - height / 2 + self.HEADER_HEIGHT

            # Draw the outline and fill
            self.lcd.draw_rectangle(1, self.color_popup_background, padding, y, self.lcd.screen_width - padding, y + height)
            self.lcd.draw_rectangle(0, self.color_white, padding, y, self.lcd.screen_width - padding, y + height)

            # Draw each line
            y += padding
            for line in lines:
                self.lcd.draw_string(
                    False,
                    self.lcd.font_8x8,
                    self.color_white,
                    self.color_popup_background,
                    padding * 2,
                    y,
                    line.strip()
                )
                y += line_height 
            
            # Draw ok button
            y += padding
            self.lcd.draw_icon(True, self.selected_language, self.icon_confirm_button, 80, y)
            self.lcd.draw_rectangle(0, self.color_white, 80, y, 160, y + 31)
        except Exception as e:
            # I imagine that on an extreme scenario where firmware_restart is called and this tries to communicate with the LCD
            # This could error out, very unlikely scenario, but either way it's better to catch it and log it
            self.error("Error in show_popup: %s" % e)

    def Erase_Menu_Cursor(self, line):
        self.lcd.draw_rectangle(
            1,
            self.color_background_black,
            0,
            self.MBASE(line) - 18,
            14,
            self.MBASE(line + 1) - 20,
        )

    def Erase_Menu_Text(self, line):
        self.lcd.draw_rectangle(
            1,
            self.color_background_black,
            self.LBLX,
            self.MBASE(line) - 14,
            271,
            self.MBASE(line) + 28,
        )

    def Move_Highlight(self, ffrom, newline):
        self.Erase_Menu_Cursor(newline - ffrom)
        self.Draw_Menu_Cursor(newline)

    def Add_Menu_Line(self):
        self.Move_Highlight(1, self.MROWS)
        self.lcd.draw_line(
            self.color_line,
            18,
            self.MBASE(self.MROWS + 1) - 22,
            238,
            self.MBASE(self.MROWS + 1) - 22,
        )

    def Scroll_Menu(self, dir):
        self.lcd.move_screen_area(
            dir,
            self.MLINE,
            self.color_background_grey,
            12,
            self.HEADER_HEIGHT,
            self.lcd.screen_width - 1,
            self.STATUS_Y,
        )
        if dir == self.scroll_down:
            self.Move_Highlight(-1, 0)
        elif dir == self.scroll_up:
            self.Add_Menu_Line()

    # Redraw the first set of SD Files
    def Redraw_SD_List(self):
        self.select_file.reset()
        self.index_file = self.MROWS
        self.Clear_Menu_Area()  # Leave title bar unchanged, clear only middle of screen
        self.Draw_Back_First()
        fl = self.pd.GetFiles()
        ed = len(fl)
        if ed > 0:
            if ed > self.MROWS:
                ed = self.MROWS
            for i in range(ed):
                self.Draw_SDItem(i, i + 1)
        else:
            self.lcd.draw_string_centered(
                False,
                self.lcd.font_16x32,
                self.color_white,
                self.color_background_black,
                14,
                32,
                self.lcd.screen_width / 2,
                self.lcd.screen_height / 2,
                "No Media"
            )

    def CompletedHoming(self):
        self.pd.HMI_flag.home_flag = False
        if self.checkkey == self.Last_Prepare:
            self.checkkey = self.Prepare
            self.select_prepare.now = self.PREPARE_CASE_HOME
            self.index_prepare = self.MROWS
            self.Draw_Prepare_Menu()
        elif self.checkkey == self.Back_Main:
            self.pd.HMI_ValueStruct.print_speed = self.pd.feedrate_percentage = 100
            # dwin_zoffset = TERN0(HAS_BED_PROBE, probe.offset.z)
            # planner.finish_and_disable()
            self.Goto_MainMenu()

    # --------------------------------------------------------------#
    # --------------------------------------------------------------#

    def icon_Print(self):
        if self.select_page.now == 0:
            self.lcd.draw_icon(True, self.ICON, self.icon_print_selected, 12, 51)
            self.lcd.draw_icon(
                True, self.selected_language, self.icon_TEXT_Print_selected, 13, 120
            )
            self.lcd.draw_rectangle(0, self.color_white, 12, 51, 112, 165)
            # self.lcd.move_screen_area(1, 1, 451, 31, 463, 57, 201)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_print, 12, 51)
            self.lcd.draw_icon(
                True, self.selected_language, self.icon_TEXT_Print, 13, 120
            )
            # self.lcd.move_screen_area(1, 1, 423, 31, 435, 57, 201)

    def icon_Prepare(self):
        if self.select_page.now == 1:
            self.lcd.draw_icon(True, self.ICON, self.icon_prepare_selected, 126, 51)
            self.lcd.draw_icon(
                True, self.selected_language, self.icon_TEXT_Prepare_selected, 127, 120
            )
            self.lcd.draw_rectangle(0, self.color_white, 126, 51, 226, 165)
            # self.lcd.move_screen_area(1, 33, 451, 82, 466, 175, 201)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_prepare, 126, 51)
            self.lcd.draw_icon(
                True, self.selected_language, self.icon_TEXT_Prepare, 127, 120
            )
            # self.lcd.move_screen_area(1, 33, 423, 82, 438, 175, 201)

    def icon_Control(self):
        if self.select_page.now == 2:
            self.lcd.draw_icon(True, self.ICON, self.icon_control_selected, 12, 178)
            self.lcd.draw_icon(
                True, self.selected_language, self.icon_TEXT_Control_selected, 13, 247
            )
            self.lcd.draw_rectangle(0, self.color_white, 12, 178, 112, 292)
            # self.lcd.move_screen_area(1, 85, 451, 132, 463, 48, 318)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_control, 12, 178)
            self.lcd.draw_icon(
                True, self.selected_language, self.icon_TEXT_Control, 13, 247
            )
            # self.lcd.move_screen_area(1, 85, 423, 132, 434, 48, 318)

    def icon_Misc(self, show):
        if show:
            self.lcd.draw_icon(True, self.ICON, self.icon_leveling_selected, 126, 178)
            self.lcd.draw_string_centered(
                False,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                self.MENU_CHR_W,
                0,
                175,
                247,
                "Misc"
            )
            self.lcd.draw_rectangle(0, self.color_white, 126, 178, 226, 292)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_leveling, 126, 178)
            self.lcd.draw_string_centered(
                False,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                self.MENU_CHR_W,
                0,
                175,
                247,
                "Misc"
            )

    def icon_StartInfo(self, show):
        if show:
            self.lcd.draw_icon(True, self.ICON, self.icon_Info_1, 145, 246)
            self.lcd.draw_rectangle(0, self.color_white, 126, 178, 226, 292)
            # self.lcd.move_screen_area(1, 132, 451, 159, 466, 186, 318)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_Info_0, 145, 246)
            # self.lcd.move_screen_area(1, 132, 423, 159, 435, 186, 318)

    def show_tune(self):
        if self.select_print.now == 0:
            self.lcd.draw_icon(True, self.ICON, self.icon_tune_selected, 12, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Tune_selected, 12, 225
            )
            self.lcd.draw_rectangle(0, self.color_white, 12, 191, 78, 251)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_tune, 12, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Tune, 12, 225
            )

    def show_continue(self):
        # Todo: Where is icon for continue text? replace for text if not found
        if self.select_print.now == 1:
            self.lcd.draw_icon(True, self.ICON, self.icon_continue_selected, 86, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Pause_selected, 86, 225
            )
            self.lcd.draw_rectangle(0, self.color_white, 86, 191, 151, 251)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_continue, 86, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Pause, 86, 225
            )
            # self.lcd.move_screen_area(1, 1, 424, 31, 434, 121, 325)

    def show_pause(self):
        if self.select_print.now == 1:
            self.lcd.draw_icon(True, self.ICON, self.icon_pause_selected, 86, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Pause_selected, 86, 225
            )
            self.lcd.draw_rectangle(0, self.color_white, 86, 191, 151, 251)
        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_pause, 86, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Pause, 86, 225
            )

    def show_stop(self):
        if self.select_print.now == 2:
            self.lcd.draw_icon(True, self.ICON, self.icon_stop_selected, 160, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Stop_selected, 160, 225
            )
            self.lcd.draw_rectangle(0, self.color_white, 160, 191, 225, 251)

        else:
            self.lcd.draw_icon(True, self.ICON, self.icon_stop, 160, 191)
            self.lcd.draw_icon(
                False, self.selected_language, self.icon_TEXT_Stop, 160, 225
            )

    def Item_Prepare_Move(self, row):
        # Draw Move icon and text
        self.draw_move_en(self.MBASE(row))
        self.Draw_Menu_Line_With_Only_Icons(
            row, self.icon_axis, self.icon_TEXT_move_axis
        )
        self.Draw_More_Icon(row)

    def Item_Prepare_Disable(self, row):
        # Draw Disable Stepper icon and text
        self.Draw_Menu_Line_With_Only_Icons(
            row, self.icon_close_motor, self.icon_TEXT_disable_stepper
        )

    def Item_Prepare_Home(self, row):
        # Draw auto home icon and text
        self.Draw_Menu_Line_With_Only_Icons(
            row, self.icon_homing, self.icon_TEXT_auto_home
        )

    def Item_Prepare_Offset(self, row):
        if self.pd.HAS_BED_PROBE:
            # Draw Z-offset icon and text, iv available
            self.Draw_Menu_Line_With_Only_Icons(
                row, self.icon_z_offset, self.icon_TEXT_Z_Offset
            )
            self.lcd.draw_signed_float(
                True,
                self.lcd.font_8x8,
                self.color_white,
                self.color_background_black,
                2,
                3,
                175,
                self.MBASE(row) - 10,
                self.pd.BABY_Z_VAR * 100,
            )
        else:
            # If not, dont write text, only icon
            self.Draw_Menu_Line(row, self.icon_set_home, "...")

    def Item_Prepare_PLA(self, row):
        # Draw preheat pla icon and text
        self.Draw_Menu_Line_With_Only_Icons(
            row, self.icon_preheat_pla, self.icon_TEXT_preheat_pla
        )

    def Item_Prepare_TPU(self, row):
        # Draw preheat tpu icon and text
        self.Draw_Menu_Line_With_Only_Icons(
            row, self.icon_preheat_tpu, self.icon_TEXT_preheat_tpu
        )

    def Item_Prepare_Cool(self, row):
        # Draw cooldown icon and text
        self.Draw_Menu_Line_With_Only_Icons(
            row, self.icon_cool, self.icon_TEXT_cooldown
        )

    # --------------------------------------------------------------#
    # --------------------------------------------------------------#

    def EachMomentUpdate(self, eventtime):
        # variable update
        update = self.pd.update_variable()
        if self.last_status != self.pd.status:
            self.last_status = self.pd.status
            print(self.pd.status)
            if self.pd.status == "printing":
                self.Goto_PrintProcess()
            elif self.pd.status in ["operational", "complete", "standby", "cancelled"]:
                self.Goto_MainMenu()
            elif self.pd.status == "error":
                self.show_popup(self.printer.get_state_message())

        if self.is_manual_probe_active():
            if self.checkkey != self.ManualProbeProcess:
                self.Goto_ManualProbe_Menu()

            # Ensure the status area won't redraw
            update = False
        elif self.checkkey == self.ManualProbeProcess:
            self.Goto_MainMenu()

        if self.checkkey == self.PrintProcess:
            if self.pd.HMI_flag.print_finish and not self.pd.HMI_flag.done_confirm_flag:
                self.pd.HMI_flag.print_finish = False
                self.pd.HMI_flag.done_confirm_flag = True
                # show percent bar and value
                self.Draw_Print_ProgressBar(0)
                # show print done confirm
                self.lcd.draw_rectangle(
                    1,
                    self.color_background_black,
                    0,
                    250,
                    self.lcd.screen_width - 1,
                    self.STATUS_Y,
                )
                self.lcd.draw_icon(
                    True,
                    self.selected_language,
                    self.icon_confirm_button,
                    86,
                    283,
                )
            elif self.pd.HMI_flag.pause_flag != self.pd.printingIsPaused():
                # print status update
                self.pd.HMI_flag.pause_flag = self.pd.printingIsPaused()
                if self.pd.HMI_flag.pause_flag:
                    self.show_continue()
                else:
                    self.show_pause()
            self.Draw_Print_ProgressBar()
            self.Draw_Print_ProgressElapsed()
            self.Draw_Print_ProgressRemain()

        if self.pd.HMI_flag.home_flag:
            if self.pd.ishomed():
                self.CompletedHoming()

        # If not in the following, draw the status area
        status_area_blocklist = [self.MainMenu, self.MessagePopup, self.Misc, self.ManualProbeProcess, self.IconFinder]
        if update and self.checkkey not in status_area_blocklist:
            self.Draw_Status_Area(update)

        # Check for errors and/or incoming messages
        if self.display_status is not None and self.display_status.message and len(self.display_status.message) > 0 and self.last_display_status != self.display_status.message:
            self.show_popup(self.display_status.message)
            self.last_display_status = self.display_status.message

        self.time_since_movement += 1
        if (self.time_since_movement >= self.display_dim_timeout) & (not self.is_dimmed):
            self.lcd.set_backlight_brightness(5)
            self.is_dimmed = True
        elif (self.time_since_movement < self.display_dim_timeout) & (self.is_dimmed):
            self.lcd.set_backlight_brightness(40)
            self.is_dimmed = False

        return eventtime + self._update_interval

    def encoder_has_data(self):
        if self.checkkey == self.MainMenu:
            self.HMI_MainMenu()
        elif self.checkkey == self.SelectFile:
            self.HMI_SelectFile()
        elif self.checkkey == self.Prepare:
            self.HMI_Prepare()
        elif self.checkkey == self.Control:
            self.HMI_Control()
        elif self.checkkey == self.PrintProcess:
            self.HMI_Printing()
        elif self.checkkey == self.Print_window:
            self.HMI_PauseOrStop()
        elif self.checkkey == self.AxisMove:
            self.HMI_AxisMove()
        elif self.checkkey == self.TemperatureID:
            self.HMI_Temperature()
        elif self.checkkey == self.Motion:
            self.HMI_Motion()
        elif self.checkkey == self.Info:
            self.HMI_Info()
        elif self.checkkey == self.Tune:
            self.HMI_Tune()
        elif self.checkkey == self.PLAPreheat:
            self.HMI_PLAPreheatSetting()
        elif self.checkkey == self.TPUPreheat:
            self.HMI_TPUPreheatSetting()
        elif self.checkkey == self.MaxSpeed:
            self.HMI_MaxSpeed()
        elif self.checkkey == self.MaxAcceleration:
            self.HMI_MaxAcceleration()
        elif self.checkkey == self.MaxJerk:
            self.HMI_MaxJerk()
        elif self.checkkey == self.Step:
            self.HMI_Step()
        elif self.checkkey == self.Move_X:
            self.HMI_Move_X()
        elif self.checkkey == self.Move_Y:
            self.HMI_Move_Y()
        elif self.checkkey == self.Move_Z:
            self.HMI_Move_Z()
        elif self.checkkey == self.Extruder:
            self.HMI_Move_E()
        elif self.checkkey == self.ETemp:
            self.HMI_ETemp()
        elif self.checkkey == self.Homeoffset:
            self.HMI_Zoffset()
        elif self.checkkey == self.BedTemp:
            self.HMI_BedTemp()
        # elif self.checkkey == self.FanSpeed:
        #     self.HMI_FanSpeed()
        elif self.checkkey == self.PrintSpeed:
            self.HMI_PrintSpeed()
        elif self.checkkey == self.MaxSpeed_value:
            self.HMI_MaxFeedspeedXYZE()
        elif self.checkkey == self.MaxAcceleration_value:
            self.HMI_MaxAccelerationXYZE()
        elif self.checkkey == self.MaxJerk_value:
            self.HMI_MaxJerkXYZE()
        elif self.checkkey == self.Step_value:
            self.HMI_StepXYZE()
        elif self.checkkey == self.FeatureNotAvailable:
            self.HMI_FeatureNotAvailable()
        elif self.checkkey == self.ManualProbeProcess:
            self.HMI_ManualProbe()
        elif self.checkkey == self.Misc:
            self.HMI_Misc()
        elif self.checkkey == self.MessagePopup:
            self.HMI_MessagePopup()
        elif self.checkkey == self.IconFinder:
            self.HMI_IconFinder()
        elif self.checkkey == self.print_preview:
            self.HMI_PrintPreview()

        self.time_since_movement = 0

    def log(self, msg, *args, **kwargs):
        if self._logging:
            logging.info("E3V3SE Display: " + str(msg))

    def error(self, msg, *args, **kwargs):
        logging.error("E3V3SE Display: " + str(msg))

def load_config(config):
    return E3v3seDisplay(config)

def load_config_prefix(config):
    return E3v3seDisplayMacro(config)
