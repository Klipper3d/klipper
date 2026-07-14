# LYX9231 UART (Modbus RTU) configuration
#
# Author: zylo117
# Aligned with tmc2208.py architecture
# License: GNU GPLv3

import logging
from . import lyx, lyx_uart

LYX_FREQUENCY = 40000.

######################################################################
# Register definitions (from spec section 5.1)
######################################################################

Registers = {
    # System & ID
    "SAVE_PARAM":        0x00,
    "BAUDRATE":          0x01,
    "COMM_ADDR":         0x02,
    "CHIP_MODEL":        0x03,
    # Motor parameters (read-only)
    "PHASE_B_RESIST":    0x04,
    "PHASE_A_RESIST":    0x05,
    "PHASE_B_INDUCT":    0x06,
    "PHASE_A_INDUCT":    0x07,
    # Status
    "ALARM_CODE":        0x08,
    "CURRENT_KP":        0x09,
    "CURRENT_KI":        0x0A,
    "MOTOR_POS_H":       0x0C,
    "MOTOR_POS_L":       0x0D,
    "MOTOR_SPEED":       0x0E,
    "ERROR_ANGLE":       0x10,
    # Configuration
    "MS_PIN_FUNC":       0x11,
    "MOTOR_TYPE":        0x12,
    "RUN_CURRENT":       0x13,
    "HALF_CUR_TIME":     0x14,
    "HALF_CUR_RATIO":    0x15,
    "HALF_CUR_EN":       0x16,
    "DIR_POLARITY":      0x17,
    "ENA_POLARITY":      0x18,
    "MICROSTEP_RATIO":   0x19,
    "DEAD_TIME":         0x1A,
    "OCL_THRESHOLD":     0x1B,
    "OCL_FILTER":        0x1C,
    "CUR_ANTISAT":       0x1D,
    "CUR_KP_GAIN":       0x1E,
    "CUR_KI_GAIN":       0x1F,
    "BOOST_LEVEL":       0x20,
    "OP_MODE":           0x21,
    "STALL_ANGLE":       0x22,
    "STALL_OUT_EN":      0x23,
    "FOLLOW_ERR_ANGLE":  0x24,
    "FOLLOW_ERR_EN":     0x25,
    "MIN_SPEED":         0x26,
    "BASE_CURRENT":      0x27,
    "POS_LEAD_COEFF":    0x28,
    "ENC_PULSES":        0x29,
    "MAG_ENC_TYPE":      0x2A,
    "POS_INTEG":         0x2B,
    "SUPER_FILT":        0x2C,
    # Motion control
    "START_SPEED":       0x3A,
    "MAX_SPEED":         0x3B,
    "ACCEL":             0x3C,
    "PULSE_CNT_H":       0x3D,
    "PULSE_CNT_L":       0x3E,
    "MOTION_MODE":       0x3F,
    "MOTION_START":      0x40,
    # Noise reduction
    "NOISE_EN":          0x41,
}

ReadRegisters = [
    "CHIP_MODEL", "PHASE_B_RESIST", "PHASE_A_RESIST",
    "PHASE_B_INDUCT", "PHASE_A_INDUCT", "ALARM_CODE",
    "CURRENT_KP", "CURRENT_KI", "MOTOR_POS_H", "MOTOR_POS_L",
    "MOTOR_SPEED", "ERROR_ANGLE",
]

######################################################################
# Field bit definitions
# LYX registers are mostly full 16-bit values, few have bitfields
######################################################################

Fields = {}

# 0x08 ALARM_CODE: bitfield status register
Fields["ALARM_CODE"] = {
    "no_alarm":    0x0000,
    "over_current": 0x0001,
    "motor_discon": 0x0002,
    "coil_abnormal": 0x0003,
    "follow_err":  0x0004,
    "stall":       0x0005,
}

# 0x21 OP_MODE: operating mode
Fields["OP_MODE"] = {
    "open_loop":       0x0000,
    "normal_closed":   0x0001,
    "super_closed":    0x0002,
    "servo_closed":    0x0003,
    "torque_mode":     0x0004,
}

# 0x12 MOTOR_TYPE
Fields["MOTOR_TYPE"] = {
    "motor_18deg":  0x0001,  # 200 steps/rev
    "motor_09deg":  0x0002,  # 400 steps/rev
}

# All other registers are full 16-bit scalar values
for reg_name in ["RUN_CURRENT", "MICROSTEP_RATIO", "BOOST_LEVEL",
                 "HALF_CUR_TIME", "HALF_CUR_RATIO", "HALF_CUR_EN",
                 "DIR_POLARITY", "ENA_POLARITY", "DEAD_TIME",
                 "OCL_THRESHOLD", "OCL_FILTER", "CUR_ANTISAT",
                 "CUR_KP_GAIN", "CUR_KI_GAIN", "STALL_ANGLE",
                 "STALL_OUT_EN", "FOLLOW_ERR_ANGLE", "FOLLOW_ERR_EN",
                 "MIN_SPEED", "BASE_CURRENT", "POS_LEAD_COEFF",
                 "ENC_PULSES", "MAG_ENC_TYPE", "POS_INTEG",
                 "SUPER_FILT", "NOISE_EN",
                 "START_SPEED", "MAX_SPEED", "ACCEL",
                 "PULSE_CNT_H", "PULSE_CNT_L", "MOTION_MODE",
                 "MOTION_START", "BAUDRATE", "COMM_ADDR",
                 "MOTOR_POS_H", "MOTOR_POS_L", "MOTOR_SPEED",
                 "ERROR_ANGLE", "CURRENT_KP", "CURRENT_KI"]:
    Fields[reg_name] = {"value": 0xFFFF}

SignedFields = ["ERROR_ANGLE", "MOTOR_SPEED"]

FieldFormatters = {
    "ALARM_CODE": lambda v: {
        0: "OK", 1: "OverCurrent", 2: "MotorDisconnected",
        3: "CoilAbnormal", 4: "FollowError", 5: "Stall"
    }.get(v, str(v)),
    "OP_MODE": lambda v: {
        0: "OpenLoop", 1: "NormalClosed", 2: "SuperClosed",
        3: "ServoClosed", 4: "TorqueMode"
    }.get(v, str(v)),
}

######################################################################
# LYX9231 printer object
######################################################################

class LYX9231:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = lyx.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.mcu_lyx = lyx_uart.MCU_LYX_uart(config, Registers, self.fields)

        # Register commands (reuse TMC-style command framework)
        current_helper = lyx.LYXCurrentHelper(config, self.mcu_lyx)
        cmdhelper = lyx.LYXCommandHelper(config, self.mcu_lyx, current_helper)
        cmdhelper.setup_register_dump(ReadRegisters)

        self.get_phase_offset = cmdhelper.get_phase_offset
        self.get_status = cmdhelper.get_status

        # Apply default register values from spec
        self._set_defaults(config)

    def _set_defaults(self, config):
        set_config_field = self.fields.set_config_field
        # Motor type
        set_config_field(config, "MOTOR_TYPE", 1)
        # Operating mode: default normal closed loop (LYX9231 is sensorless)
        set_config_field(config, "OP_MODE", 1)
        # Run current: default ~1.4A (434 / 2048 * 6.4A with 25mOhm)
        set_config_field(config, "RUN_CURRENT", 434)
        # Half current settings
        set_config_field(config, "HALF_CUR_EN", 1)
        set_config_field(config, "HALF_CUR_TIME", 3000)
        set_config_field(config, "HALF_CUR_RATIO", 64)
        # Microstep: default 200 steps/rev base -> 256 microstep = 51200
        # LYX9231 has MS pins, register value = 25600 / microstep
        set_config_field(config, "MICROSTEP_RATIO", 12800)  # default 2x
        # Dead time: default 30 -> ~312.5ns
        set_config_field(config, "DEAD_TIME", 30)
        # Over current protection: default 200 -> ~13A @50mOhm
        set_config_field(config, "OCL_THRESHOLD", 200)
        set_config_field(config, "OCL_FILTER", 6)
        # BOOST: default level 1
        set_config_field(config, "BOOST_LEVEL", 1)
        # Stall detection
        set_config_field(config, "STALL_ANGLE", 200)
        set_config_field(config, "STALL_OUT_EN", 0)
        # Follow error
        set_config_field(config, "FOLLOW_ERR_ANGLE", 1024)
        set_config_field(config, "FOLLOW_ERR_EN", 0)
        # Noise reduction: disabled by default
        set_config_field(config, "NOISE_EN", 0)

def load_config_prefix(config):
    return LYX9231(config)
