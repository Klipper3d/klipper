# Package definition for the extras/t5uid1/dugs_reloaded directory
#
# Copyright (C) 2020  Desuuuu <contact@desuuuu.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
controls = {
    # print_status
    'pause':    1, # popup_window
    'resume':   2, # popup_window

    # leveling_automatic
    'disable':  5, # return_key_code

    # settings_menu2
    'extra2':   4, # return_key_code

    # wait
    'abort':    1, # popup_window
    'continue': 2  # return_key_code
}

constants = {
    'temp_pla': {
        'hotend': 200,
        'bed': 60
    },
    'temp_abs': {
        'hotend': 240,
        'bed': 80
    },
    'temp_petg': {
        'hotend': 240,
        'bed': 60
    },

    'popup_confirmed':  1,

    'adjust_increment': 0,
    'adjust_decrement': 1,

    'preset_pla':       1,
    'preset_abs':       2,
    'preset_petg':      3,

    'extruder_current': -1,
    'extruder_e0':      0,
    'extruder_e1':      1,

    'heater_all':       -2,
    'heater_bed':       -1,
    'heater_h0':        0,
    'heater_h1':        1,

    'stepper_enable':   1,
    'stepper_disable':  2,

    'step_size_10':     0,
    'step_size_1':      1,
    'step_size_0.1':    2,
    'step_size_0.01':   3,

    'filament_retract': 0,
    'filament_extrude': 1,

    'axis_xyz':         0,
    'axis_xy':          1,
    'axis_z':           2,

    'move_x+':          0,
    'move_x-':          1,
    'move_y+':          2,
    'move_y-':          3,
    'move_z+':          4,
    'move_z-':          5,

    'extra_button1':    0,
    'extra_button2':    1,

    'disabled':           0,
    'enabled':            1,

    'status_icon_pause':  1 << 0,
    'status_icon_resume': 1 << 1,

    'step_icon_10':       1 << 0,
    'step_icon_1':        1 << 1,
    'step_icon_0.1':      1 << 2,
    'step_icon_0.01':     1 << 3,

    'extruder_icon_e0':   1 << 0,
    'extruder_icon_e1':   1 << 1,

    'heater_icon_bed':    1 << 0,
    'heater_icon_h0':     1 << 1,
    'heater_icon_h1':     1 << 2,

    'wait_icon_abort':    1 << 0,
    'wait_icon_continue': 1 << 1
}

configuration = {
    'config_files': ['routines.cfg',
                     'pages.cfg',
                     'vars_in.cfg',
                     'vars_out.cfg'],
    'boot_sound': 1,
    'notification_sound': 3,
    'controls': controls,
    'constants': constants
}
