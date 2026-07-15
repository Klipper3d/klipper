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
# Field definitions
# 规则：key = 寄存器名，value = {字段名: 掩码}
# 字段名全局唯一，全16位单值寄存器字段名用小写寄存器名
######################################################################

Fields = {}

_all_registers = [
    # 系统与ID
    "SAVE_PARAM", "BAUDRATE", "COMM_ADDR", "CHIP_MODEL",
    # 电机参数（只读）
    "PHASE_B_RESIST", "PHASE_A_RESIST", "PHASE_B_INDUCT", "PHASE_A_INDUCT",
    # 状态
    "ALARM_CODE", "CURRENT_KP", "CURRENT_KI",
    "MOTOR_POS_H", "MOTOR_POS_L", "MOTOR_SPEED", "ERROR_ANGLE",
    # 基础配置
    "MS_PIN_FUNC", "MOTOR_TYPE", "RUN_CURRENT",
    "HALF_CUR_TIME", "HALF_CUR_RATIO", "HALF_CUR_EN",
    "DIR_POLARITY", "ENA_POLARITY",
    "MICROSTEP_RATIO", "DEAD_TIME",
    "OCL_THRESHOLD", "OCL_FILTER",
    "CUR_ANTISAT", "CUR_KP_GAIN", "CUR_KI_GAIN",
    # 闭环与性能
    "BOOST_LEVEL", "OP_MODE",
    "STALL_ANGLE", "STALL_OUT_EN",
    "FOLLOW_ERR_ANGLE", "FOLLOW_ERR_EN",
    "MIN_SPEED", "BASE_CURRENT", "POS_LEAD_COEFF",
    "ENC_PULSES", "MAG_ENC_TYPE", "POS_INTEG", "SUPER_FILT",
    # 运动控制
    "START_SPEED", "MAX_SPEED", "ACCEL",
    "PULSE_CNT_H", "PULSE_CNT_L", "MOTION_MODE", "MOTION_START",
    # 降噪
    "NOISE_EN"
]

for reg_name in _all_registers:
    Fields[reg_name] = {reg_name.lower(): 0xFFFF}

# 有符号字段
SignedFields = ["error_angle", "motor_speed"]

# 字段格式化（枚举值转可读字符串）
FieldFormatters = {
    "alarm_code": lambda v: {
        0: "OK", 1: "OverCurrent", 2: "MotorDisconnected",
        3: "CoilAbnormal", 4: "FollowError", 5: "Stall"
    }.get(v, str(v)),
    "op_mode": lambda v: {
        0: "OpenLoop", 1: "NormalClosed", 2: "SuperClosed",
        3: "ServoClosed", 4: "TorqueMode"
    }.get(v, str(v)),
    "motor_type": lambda v: "1.8deg(200ppr)" if v == 1 else "0.9deg(400ppr)",
}

######################################################################
# LYX9231 printer object
######################################################################

class LYX9231:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]

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
        self._set_defaults(config)# 注册GCode调试命令
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'LYX_READ_REG', self.cmd_LYX_READ_REG,
            desc=self.cmd_LYX_READ_REG_help)
        self.gcode.register_command(
            'LYX_WRITE_REG', self.cmd_LYX_WRITE_REG,
            desc=self.cmd_LYX_WRITE_REG_help)

    cmd_LYX_READ_REG_help = "读取LYX芯片寄存器原始值"
    def cmd_LYX_READ_REG(self, gcmd):
        reg_name = gcmd.get('REGISTER').upper()
        if reg_name not in self.mcu_lyx.name_to_reg:
            raise gcmd.error(f"未知寄存器: {reg_name}")
        try:
            # 直接传寄存器名称，由 get_register 内部自行查地址
            val = self.mcu_lyx.get_register(reg_name)
            reg_addr = self.mcu_lyx.name_to_reg[reg_name]
            gcmd.respond_info(f"寄存器 {reg_name} (0x{reg_addr:02X}) = {val}")
        except Exception as e:
            import logging
            logging.exception("LYX 读取异常详情")
            raise gcmd.error(f"读取失败: {type(e).__name__}: {str(e)}")

    cmd_LYX_WRITE_REG_help = "写入LYX芯片寄存器（写完自动回读验证）"
    def cmd_LYX_WRITE_REG(self, gcmd):
        reg_name = gcmd.get('REGISTER').upper()
        value = gcmd.get_int('VALUE', minval=0, maxval=65535)
        if reg_name not in self.mcu_lyx.name_to_reg:
            raise gcmd.error(f"未知寄存器: {reg_name}")
        try:
            # 直接传寄存器名称
            self.mcu_lyx.set_register(reg_name, value)
            read_back = self.mcu_lyx.get_register(reg_name)
            reg_addr = self.mcu_lyx.name_to_reg[reg_name]
            if read_back == value:
                gcmd.respond_info(f"✅ {reg_name} (0x{reg_addr:02X}) 写入成功，回读验证 = {value}")
            else:
                gcmd.respond_info(f"⚠️  {reg_name} (0x{reg_addr:02X}) 写入值 = {value}，回读值 = {read_back}")
        except Exception as e:
            import logging
            logging.exception("LYX 写入异常详情")
            raise gcmd.error(f"写入失败: {type(e).__name__}: {str(e)}")

    def _set_defaults(self, config):
        set_config_field = self.fields.set_config_field
        # Motor type
        set_config_field(config, "motor_type", 1)
        # Operating mode: default normal closed loop
        set_config_field(config, "op_mode", 1)
        # Run current: default ~1.4A
        set_config_field(config, "run_current", 434)
        # Half current settings
        set_config_field(config, "half_cur_en", 1)
        set_config_field(config, "half_cur_time", 3000)
        set_config_field(config, "half_cur_ratio", 64)
        # Microstep ratio: default 2x (12800 = 25600/2)
        set_config_field(config, "microstep_ratio", 12800)
        # Dead time: default 30 -> ~312.5ns
        set_config_field(config, "dead_time", 30)
        # Over current protection: default 200
        set_config_field(config, "ocl_threshold", 200)
        set_config_field(config, "ocl_filter", 6)
        # BOOST: default level 1
        set_config_field(config, "boost_level", 1)
        # Stall detection
        set_config_field(config, "stall_angle", 200)
        set_config_field(config, "stall_out_en", 0)
        # Follow error
        set_config_field(config, "follow_err_angle", 1024)
        set_config_field(config, "follow_err_en", 0)
        # Noise reduction: disabled by default
        set_config_field(config, "noise_en", 0)

def load_config_prefix(config):
    return LYX9231(config)
