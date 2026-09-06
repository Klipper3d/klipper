# TMC5262 configuration
#
# Copyright (C) 2026  Kalico Contributors
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import math

from . import tmc, tmc2130

# The PLL reference is divided to 1MHz; the driver timing clock remains 16MHz.
TMC_FREQUENCY = 16000000.0

# Rev.0 pp.208-211: seed the whole optional register before overriding one
# field. FieldHelper otherwise starts an uncached register at zero, erasing
# the filter, the PI partner, or the other load reserves.
OPTIONAL_REGISTER_DEFAULTS = {
    "COOLSTEPPLUS_CONF": 0x12,
    "COOLSTEPPLUS_PI_REG": (16 << 16) | 128,
    "COOLSTEPPLUS_PI_DOWN": (64 << 16) | 128,
    "COOLSTEPPLUS_RESERVE_CONF": (100 << 24) | (220 << 16) | (50 << 8) | 150,
    "SGP_CONF": 1 << 14,
}


def configure_optional_field(config, fields, field_name):
    option = "driver_" + field_name.upper()
    if config.get(option, None) is None:
        return
    register = fields.lookup_register(field_name)
    if (register not in fields.registers
            and register in OPTIONAL_REGISTER_DEFAULTS):
        fields.registers[register] = OPTIONAL_REGISTER_DEFAULTS[register]
    fields.set_config_field(config, field_name, 0)


def pwm_measurement_defaults(pwm_freq):
    # Rev.0 Table 8, p.41.
    if not 0 <= pwm_freq <= 8:
        raise ValueError("driver_PWM_FREQ must be in range 0..8")
    return (14, 15) if pwm_freq <= 2 else (
        (13, 14) if pwm_freq <= 5 else (12, 13))

# A complete filtered RCOIL update needs up to 100ms (datasheet pp.46-48).
# Allow additional time for Klipper's queued enable/register writes before the
# first homing move.  This wait never changes current or issues a step pulse.
RCOIL_PREFLIGHT_DWELL = 0.250

# Coil-temperature reads are deliberately slower than the homing preflight.
# Enabling a stepper schedules TMC register restoration through the reactor, so
# yield before starting the data-sheet filter wait.  Then require a stable
# three-sample window in each of two independent enable cycles.  The tolerance
# allows a few raw-code counts of normal quantization without accepting a stale
# first-after-enable result as a temperature reference.
RCOIL_ENABLE_CALLBACK_DWELL = 0.010
RCOIL_SAMPLE_INTERVAL = 0.100
RCOIL_STABLE_SAMPLE_COUNT = 3
RCOIL_MAX_SAMPLE_COUNT = 8
RCOIL_ACQUISITION_CYCLES = 2
RCOIL_REPEATABILITY_COUNTS = 4
RCOIL_REPEATABILITY_FRACTION = 0.02
# The data sheet requires CS_ACTUAL >= 50.  Use a small margin and set IHOLD
# equal to IRUN so a no-motion initial RCOIL measurement is permitted.  The
# configured current is restored before the command returns.
RCOIL_SAMPLE_CURRENT_SCALE = 64

# Copper winding resistance rises by approximately 0.39 percent per kelvin.
# RCOIL includes the motor, bridge, and wiring, so this supports a referenced
# temperature estimate, not an absolute calibrated motor sensor.
COPPER_TEMPERATURE_COEFFICIENT = 0.0039

# TMCCommandHelper writes the FieldHelper cache in insertion order.  Keep the
# TMC5262-specific StealthChop+ sequence deterministic: current and motor model
# first, mode enable next, and CHOPCONF.TOFF (driver enable) last.
TMC5262_REGISTER_INIT_ORDER = (
    "DRV_CONF",
    "IHOLD_IRUN",
    "TPOWERDOWN",
    "COIL_INDUCT",
    "R_COIL_USER",
    "T_RCOIL_MEAS",
    "CURRENT_PI_REG",
    "ANGLE_PI_REG",
    "CUR_ANGLE_LIMIT",
    "ANGLE_LOWER_LIMIT",
    "PWMCONF",
    "TPWMTHRS",
    "TCOOLTHRS",
    "THIGH",
    "TSGP_LOW_VEL_THRS",
    "DO_CONF",
    "DO_SCOPE_CONF",
    "GCONF",
    "CHOPCONF",
)


# ADC conversion formulas (datasheet pg 134, 9-bit ADC)
def _adc_to_celsius(adc_value):
    return 1.042 * adc_value - 264.6


def _adc_to_volts(adc_value):
    return adc_value * 0.1409


def _rcoil_to_effective_resistance(rcoil_value, current_range_rms):
    # Datasheet p.47: RCOIL ~= 40 * R_path * CurrentRange[A RMS].
    if (not math.isfinite(float(rcoil_value))
            or not math.isfinite(current_range_rms)
            or rcoil_value <= 0 or current_range_rms <= 0.0):
        raise ValueError("RCOIL and current range must be positive")
    return rcoil_value / (40.0 * current_range_rms)


def _estimate_coil_temperature(resistance, reference_resistance,
                               reference_temperature):
    if (not all(math.isfinite(value) for value in (
            resistance, reference_resistance, reference_temperature))
            or resistance <= 0.0 or reference_resistance <= 0.0):
        raise ValueError("coil reference values must be finite and positive")
    return reference_temperature + (
        (resistance - reference_resistance)
        / (reference_resistance * COPPER_TEMPERATURE_COEFFICIENT)
    )


def _rcoil_values_repeatable(values):
    if not values:
        return False
    mean = sum(values) / float(len(values))
    tolerance = max(
        RCOIL_REPEATABILITY_COUNTS,
        abs(mean) * RCOIL_REPEATABILITY_FRACTION,
    )
    return max(values) - min(values) <= tolerance


######################################################################
# Register addresses (TMC5262 datasheet pp 121..136)
######################################################################

Registers = {
    # General configuration
    "GCONF": 0x00,
    "GSTAT": 0x01,
    "DO_CONF": 0x02,
    "DO_SCOPE_CONF": 0x03,
    "IOIN": 0x04,
    "X_COMPARE": 0x05,
    "X_COMPARE_REPEAT": 0x06,
    "DRV_CONF": 0x0A,
    "PLL": 0x0B,
    # Velocity dependent configuration
    "IHOLD_IRUN": 0x10,
    "TPOWERDOWN": 0x11,
    "TSTEP": 0x12,
    "TPWMTHRS": 0x13,
    "TCOOLTHRS": 0x14,
    "THIGH": 0x15,
    "TSGP_LOW_VEL_THRS": 0x16,
    "T_RCOIL_MEAS": 0x17,
    "TUDCSTEP": 0x18,
    "UDC_CONF": 0x19,
    "STEPS_LOST": 0x1A,
    # StealthChop+ PI regulator tuning + readbacks (datasheet pp 33-40)
    "CURRENT_PI_REG": 0x40,
    "ANGLE_PI_REG": 0x41,
    "CUR_ANGLE_LIMIT": 0x42,
    "ANGLE_LOWER_LIMIT": 0x43,
    "CUR_ANGLE_MEAS": 0x44,
    "PI_RESULTS": 0x45,
    # Motor model, StallGuard+ and CoolStep+ telemetry/configuration
    "COIL_INDUCT": 0x46,
    "R_COIL": 0x47,
    "R_COIL_USER": 0x48,
    "SGP_CONF": 0x49,
    "SGP_IND_2_3": 0x4A,
    "SGP_IND_0_1": 0x4B,
    "INDUCTANCE_VOLTAGE": 0x4C,
    "SGP_BEMF": 0x4D,
    "COOLSTEPPLUS_CONF": 0x4E,
    "COOLSTEPPLUS_PI_REG": 0x4F,
    "COOLSTEPPLUS_PI_DOWN": 0x50,
    "COOLSTEPPLUS_RESERVE_CONF": 0x51,
    "COOLSTEPPLUS_LOAD_RESERVE": 0x52,
    "TSTEP_VELOCITY": 0x53,
    # ADC
    "ADC_VSUPPLY_TEMP": 0x58,
    "ADC_I": 0x59,
    "OTW_OV_VTH": 0x5A,
    # Motor driver / wave table
    "MSLUT0": 0x60,
    "MSLUT1": 0x61,
    "MSLUT2": 0x62,
    "MSLUT3": 0x63,
    "MSLUT4": 0x64,
    "MSLUT5": 0x65,
    "MSLUT6": 0x66,
    "MSLUT7": 0x67,
    "MSLUTSEL": 0x68,
    "MSLUTSTART": 0x69,
    "MSCNT": 0x6A,
    "MSCURACT": 0x6B,
    "CHOPCONF": 0x6C,
    "COOLCONF": 0x6D,
    "DRV_STATUS": 0x6F,
    "PWMCONF": 0x70,
}

ReadRegisters = [
    "GCONF",
    "GSTAT",
    "DO_CONF",
    "DO_SCOPE_CONF",
    "IOIN",
    "DRV_CONF",
    "PLL",
    "IHOLD_IRUN",
    "TPOWERDOWN",
    "TSTEP",
    "TPWMTHRS",
    "TCOOLTHRS",
    "THIGH",
    "TSGP_LOW_VEL_THRS",
    "T_RCOIL_MEAS",
    "STEPS_LOST",
    "ADC_VSUPPLY_TEMP",
    "ADC_I",
    "CURRENT_PI_REG",
    "ANGLE_PI_REG",
    "CUR_ANGLE_LIMIT",
    "ANGLE_LOWER_LIMIT",
    "CUR_ANGLE_MEAS",
    "PI_RESULTS",
    "COIL_INDUCT",
    "R_COIL",
    "R_COIL_USER",
    "SGP_CONF",
    "SGP_IND_2_3",
    "SGP_IND_0_1",
    "INDUCTANCE_VOLTAGE",
    "SGP_BEMF",
    "COOLSTEPPLUS_CONF",
    "COOLSTEPPLUS_PI_REG",
    "COOLSTEPPLUS_PI_DOWN",
    "COOLSTEPPLUS_RESERVE_CONF",
    "COOLSTEPPLUS_LOAD_RESERVE",
    "TSTEP_VELOCITY",
    "OTW_OV_VTH",
    "MSCNT",
    "MSCURACT",
    "CHOPCONF",
    "COOLCONF",
    "DRV_STATUS",
    "PWMCONF",
]


######################################################################
# Register field maps
######################################################################

Fields = {}

# GCONF (0x00) - p.137
# Bit 1 is "en_stealthchop" in the datasheet; aliased to "en_pwm_mode"
# for tmc.TMCStealthchopHelper compatibility.
Fields["GCONF"] = {
    "fast_standstill": 0x01 << 0,
    "en_pwm_mode": 0x01 << 1,
    "multistep_filt": 0x01 << 2,
    "shaft": 0x01 << 3,
    "small_hysteresis": 0x01 << 4,
    "stop_enable": 0x01 << 5,
    "direct_mode": 0x01 << 6,
    "length_steppulse": 0x0F << 8,
    "ov_nn": 0x01 << 12,
    "step_dir": 0x01 << 31,
}

# GSTAT (0x01) - p.139, write-1-to-clear
Fields["GSTAT"] = {
    "reset": 0x01 << 0,
    "drv_err": 0x01 << 1,
    "uv_cp": 0x01 << 2,
    "register_reset": 0x01 << 3,
    "vm_uvlo": 0x01 << 4,
    "vccio_uv": 0x01 << 5,
}

# DO_CONF (0x02) - p.140 - DIAG output mux
Fields["DO_CONF"] = {
    "do0_error": 0x01 << 0,
    "do0_otpw": 0x01 << 1,
    "do0_stall": 0x01 << 2,
    "do0_index": 0x01 << 3,
    "do0_step": 0x01 << 4,
    "do0_dir": 0x01 << 5,
    "do0_xcomp": 0x01 << 6,
    "do0_ov": 0x01 << 7,
    "do0_udcstep": 0x01 << 8,
    "do0_ev_stop_ref": 0x01 << 9,
    "do0_ev_stop_sg": 0x01 << 10,
    "do0_ev_pos_reached": 0x01 << 11,
    "do0_ev_n_deviation": 0x01 << 12,
    "do1_error": 0x01 << 13,
    "do1_otpw": 0x01 << 14,
    "do1_stall": 0x01 << 15,
    "do1_index": 0x01 << 16,
    "do1_step": 0x01 << 17,
    "do1_dir": 0x01 << 18,
    "do1_xcomp": 0x01 << 19,
    "do1_ov": 0x01 << 20,
    "do1_udcstep": 0x01 << 21,
    "do1_ev_stop_ref": 0x01 << 22,
    "do1_ev_stop_sg": 0x01 << 23,
    "do1_ev_pos_reached": 0x01 << 24,
    "do1_ev_n_deviation": 0x01 << 25,
    "do0_npp_pp": 0x01 << 28,
    "do0_invpp": 0x01 << 29,
    "do1_npp_pp": 0x01 << 30,
    "do1_invpp": 0x01 << 31,
}

# DO_SCOPE_CONF (0x03) - RT-OCSI signal routing to DO0/DO1 for the XIAO
# capture path. The selected signal is not digitized for SPI access.
Fields["DO_SCOPE_CONF"] = {
    "do0_scope_en": 0x01 << 0,
    "do0_scope_sel": 0x1F << 4,
    "do1_scope_en": 0x01 << 12,
    "do1_scope_sel": 0x1F << 16,
}

# IOIN (0x04) - p.144 - read-only inputs / silicon revision
Fields["IOIN"] = {
    "refl": 0x01 << 0,
    "refr": 0x01 << 1,
    "encb": 0x01 << 2,
    "enca": 0x01 << 3,
    "drv_enn": 0x01 << 4,
    "encn": 0x01 << 5,
    "ext_res_det": 0x01 << 13,
    "ext_clk": 0x01 << 14,
    "silicon_rv": 0x07 << 16,
}

# DRV_CONF (0x0A) - p.148 - integrated current sense (ICS) range select
Fields["DRV_CONF"] = {
    "current_range": 0x03 << 0,
    "current_range_scale": 0x03 << 2,
    "slope_control": 0x03 << 4,
}

# PLL (0x0B) - p.149 - mandatory internal-clock setup. The Rev.0 register
# layout has CLK_FSM_ENA at bit 3; bit 4 and bits 10..11/15 are reserved.
Fields["PLL"] = {
    "commit": 0x01 << 0,
    "ext_not_int": 0x01 << 1,
    "clk_sys_sel": 0x01 << 2,
    "clk_fsm_ena": 0x01 << 3,
    "clock_divider": 0x1F << 5,
    "clk_1mo_tmo": 0x01 << 12,
    "clk_loss": 0x01 << 13,
    "clk_is_stuck": 0x01 << 14,
}

# IHOLD_IRUN (0x10) - p.151 - 8-bit IRUN/IHOLD (TMC5262 widened from 5)
Fields["IHOLD_IRUN"] = {
    "ihold": 0xFF << 0,
    "irun": 0xFF << 8,
    "iholddelay": 0xFF << 16,
    "irundelay": 0x0F << 24,
}

Fields["TPOWERDOWN"] = {"tpowerdown": 0xFF << 0}
Fields["TSTEP"] = {"tstep": 0xFFFFF << 0}
Fields["TPWMTHRS"] = {"tpwmthrs": 0xFFFFF << 0}
Fields["TCOOLTHRS"] = {"tcoolthrs": 0xFFFFF << 0}
Fields["THIGH"] = {"thigh": 0xFFFFF << 0}
Fields["TSGP_LOW_VEL_THRS"] = {"tsgp_low_vel_thrs": 0xFFFFF << 0}
Fields["T_RCOIL_MEAS"] = {"t_rcoil_meas": 0xFFFFF << 0}
Fields["STEPS_LOST"] = {"steps_lost": 0xFFFFF << 0}

# ADC registers - p.134 (9-bit Vsupply/temperature, 12-bit coil currents)
#   T(°C) = 1.042 * v - 264.6
#   V_supply(V) = v * 0.1409
Fields["ADC_VSUPPLY_TEMP"] = {
    "adc_vsupply": 0x1FF << 0,
    "adc_temp": 0x1FF << 16,
}
Fields["ADC_I"] = {
    "adc_i_a": 0xFFF << 0,
    "adc_i_b": 0xFFF << 16,
}
# StealthChop+ PI regulator config and state (datasheet pp 193-198).
# Defaults match the chip's reset values.
Fields["CURRENT_PI_REG"] = {
    "cur_p": 0xFFF << 0,
    "cur_i": 0x3FF << 16,
}
Fields["ANGLE_PI_REG"] = {
    "angle_p": 0xFFF << 0,
    "angle_i": 0x3FF << 16,
}
Fields["CUR_ANGLE_LIMIT"] = {
    "angle_pi_limit": 0x3FF << 0,
    "angle_pi_int_pos_clip": 0x01 << 12,
    "angle_pi_int_neg_clip": 0x01 << 13,
    "angle_pi_pos_clip": 0x01 << 14,
    "angle_pi_neg_clip": 0x01 << 15,
    "cur_pi_limit": 0xFFF << 16,
    "cur_pi_int_pos_clip": 0x01 << 28,
    "cur_pi_int_neg_clip": 0x01 << 29,
    "cur_pi_pos_clip": 0x01 << 30,
    "cur_pi_neg_clip": 0x01 << 31,
}
Fields["ANGLE_LOWER_LIMIT"] = {
    "angle_lower_i_limit": 0x3FF << 0,
    "angle_error": 0x3FF << 16,
}
# CUR_ANGLE_MEAS (0x44) - p.197 - StealthChop+ readbacks. Both read 0
# in SpreadCycle mode.
Fields["CUR_ANGLE_MEAS"] = {
    "ampl_meas": 0xFFF << 0,
    "angle_meas": 0x3FF << 16,
}
Fields["PI_RESULTS"] = {
    "pwm_calc": 0x1FFF << 0,
    "angle_corr_calc": 0x3FF << 16,
}

# Motor model and StallGuard+/CoolStep+ state.  These definitions mirror
# Analog Devices' TMC-API TMC5262_HW_Abstraction.h.  Several are read-only
# telemetry; listing a field here makes it decodable by DUMP_TMC and by a
# future tuning module, but does not cause Klipper to write the register.
Fields["COIL_INDUCT"] = {
    "coil_induct": 0x7FFF << 0,
    "rcoil_manual": 0x01 << 16,
    "rcoil_thermal_coupling": 0x01 << 17,
}
Fields["R_COIL"] = {
    "r_coil_auto_b": 0xFFF << 0,
    "r_coil_auto_a": 0xFFF << 16,
}
Fields["R_COIL_USER"] = {
    "r_coil_user_b": 0xFFF << 0,
    "r_coil_user_a": 0xFFF << 16,
}
Fields["SGP_CONF"] = {
    "sgp_thrs": 0x1FF << 0,
    "sgp_filt_en": 0x01 << 12,
    "sgp_low_vel_freeze": 0x01 << 13,
    "sgp_clear_cur_pi": 0x01 << 14,
    "sgp_low_vel_slope": 0xFF << 16,
    "sgp_low_vel_cnts": 0x03 << 28,
}
Fields["SGP_IND_2_3"] = {
    "sgp_ind_2": 0x3FF << 0,
    "sgp_ind_3": 0x3FF << 16,
}
Fields["SGP_IND_0_1"] = {
    "sgp_ind_0": 0x3FF << 0,
    "sgp_ind_1": 0x3FF << 16,
}
Fields["INDUCTANCE_VOLTAGE"] = {
    "ul_b": 0xFFF << 0,
    "ul_a": 0xFFF << 16,
}
Fields["SGP_BEMF"] = {
    "sgp_raw": 0x3FF << 0,
    "ubemf_abs": 0xFFF << 16,
}
Fields["COOLSTEPPLUS_CONF"] = {
    "cool_cur_div": 0x0F << 0,
    "load_filt_en": 0x01 << 4,
}
Fields["COOLSTEPPLUS_PI_REG"] = {
    "coolstep_p": 0xFFF << 0,
    "coolstep_i": 0x3FF << 16,
}
Fields["COOLSTEPPLUS_PI_DOWN"] = {
    "cool_pi_down_limit": 0xFFF << 0,
    "cool_pi_off_speed": 0xFFF << 16,
}
Fields["COOLSTEPPLUS_RESERVE_CONF"] = {
    "cool_low_load_reserve": 0xFF << 0,
    "cool_hi_load_reserve": 0xFF << 8,
    "cool_low_generatoric_reserve": 0xFF << 16,
    "cool_hi_generatoric_reserve": 0xFF << 24,
}
Fields["COOLSTEPPLUS_LOAD_RESERVE"] = {
    "sgp_result": 0x3FF << 0,
    "coolstep_load_reserve": 0x1FF << 16,
}
Fields["TSTEP_VELOCITY"] = {"tstep_velocity": 0x7FFFFF << 0}
Fields["OTW_OV_VTH"] = {
    "overvoltage_vth": 0x1FF << 0,
    "overtempprewarning_vth": 0x1FF << 16,
}

# Wave table - format identical to TMC2240/TMC5160
Fields["MSLUT0"] = {"mslut0": 0xFFFFFFFF}
Fields["MSLUT1"] = {"mslut1": 0xFFFFFFFF}
Fields["MSLUT2"] = {"mslut2": 0xFFFFFFFF}
Fields["MSLUT3"] = {"mslut3": 0xFFFFFFFF}
Fields["MSLUT4"] = {"mslut4": 0xFFFFFFFF}
Fields["MSLUT5"] = {"mslut5": 0xFFFFFFFF}
Fields["MSLUT6"] = {"mslut6": 0xFFFFFFFF}
Fields["MSLUT7"] = {"mslut7": 0xFFFFFFFF}
Fields["MSLUTSEL"] = {
    "x3": 0xFF << 24,
    "x2": 0xFF << 16,
    "x1": 0xFF << 8,
    "w3": 0x03 << 6,
    "w2": 0x03 << 4,
    "w1": 0x03 << 2,
    "w0": 0x03 << 0,
}
Fields["MSLUTSTART"] = {
    "start_sin": 0xFF << 0,
    "start_sin90": 0xFF << 16,
    "offset_sin90": 0xFF << 24,
}

Fields["MSCNT"] = {"mscnt": 0x3FF << 0}

# MSCURACT - coil order is *swapped* relative to TMC2240/TMC5160:
# TMC5262 puts CUR_B at [8:0] and CUR_A at [24:16].
Fields["MSCURACT"] = {"cur_b": 0x1FF << 0, "cur_a": 0x1FF << 16}

# CHOPCONF (0x6C) - p.136 - layout differs from TMC2240:
#   * tbl is contiguous bits [16:15]
#   * hend is contiguous bits [10:7]
#   * vhighfs / vhighchm / diss2g / diss2vs are gone
Fields["CHOPCONF"] = {
    "toff": 0x0F << 0,
    "hstrt": 0x07 << 4,
    "hend": 0x0F << 7,
    "fd3": 0x01 << 11,
    "disfdcc": 0x01 << 12,
    "chm": 0x01 << 14,
    "tbl": 0x03 << 15,
    "tpfd": 0x0F << 20,
    "mres": 0x0F << 24,
    "intpol": 0x01 << 28,
    "dedge": 0x01 << 29,
}

# COOLCONF (0x6D) - p.136 - sedn widened to 3 bits, +thigh_sg_off
Fields["COOLCONF"] = {
    "semin": 0x0F << 0,
    "seup": 0x03 << 5,
    "semax": 0x0F << 8,
    "sedn": 0x07 << 12,
    "seimin": 0x01 << 15,
    "sgt": 0x7F << 16,
    "thigh_sg_off": 0x01 << 23,
    "sfilt": 0x01 << 24,
}

# DRV_STATUS (0x6F) - p.136 - 10-bit sg_result, 8-bit cs_actual,
# +ov / +seq_stopped, no fsactive
Fields["DRV_STATUS"] = {
    "sg_result": 0x3FF << 0,
    "seq_stopped": 0x01 << 10,
    "ov": 0x01 << 11,
    "s2vsa": 0x01 << 12,
    "s2vsb": 0x01 << 13,
    "stealth": 0x01 << 14,
    "cs_actual": 0xFF << 16,
    "stallguard": 0x01 << 24,
    "ot": 0x01 << 25,
    "otpw": 0x01 << 26,
    "s2ga": 0x01 << 27,
    "s2gb": 0x01 << 28,
    "ola": 0x01 << 29,
    "olb": 0x01 << 30,
    "stst": 0x01 << 31,
}

# PWMCONF (0x70) - p.136 - simplified vs TMC2240
Fields["PWMCONF"] = {
    "pwm_freq": 0x0F << 0,
    "freewheel": 0x03 << 4,
    "ol_thrsh": 0x03 << 6,
    "sd_on_meas_lo": 0x0F << 12,
    "sd_on_meas_hi": 0x0F << 16,
}


SignedFields = [
    "cur_a",
    "cur_b",
    "sgt",
    "offset_sin90",
    "adc_i_a",
    "adc_i_b",
    "angle_error",
    "angle_meas",
    "angle_corr_calc",
    "sgp_thrs",
    "sgp_ind_0",
    "sgp_ind_1",
    "sgp_ind_2",
    "sgp_ind_3",
    "ul_a",
    "ul_b",
    "sgp_raw",
    "sgp_result",
]


FieldFormatters = dict(tmc2130.FieldFormatters)
FieldFormatters.update(
    {
        "ov": lambda v: "1(Overvoltage!)" if v else "",
        "s2vsa": lambda v: "1(ShortToSupply_A!)" if v else "",
        "s2vsb": lambda v: "1(ShortToSupply_B!)" if v else "",
        "vm_uvlo": lambda v: "1(VMUndervoltage!)" if v else "",
        "vccio_uv": lambda v: "1(VCCIOUndervoltage!)" if v else "",
        "register_reset": lambda v: "1(RegisterReset!)" if v else "",
        "ext_clk": lambda v: "1(ExternalClock)" if v else "0(Internal16MHz)",
        "ext_res_det": lambda v: "1" if v else "0(MissingRREF!)",
        "drv_enn": lambda v: "1(Disabled)" if v else "0(Enabled)",
        "silicon_rv": lambda v: "%d" % v,
        "clk_loss": lambda v: "1(ClockLoss!)" if v else "",
        "clk_is_stuck": lambda v: "1(ClockStuck!)" if v else "",
        "clk_1mo_tmo": lambda v: "1(ClockTimeout!)" if v else "",
        "adc_temp": (lambda v: "0x%03x(%.1fC)" % (v, _adc_to_celsius(v))),
        "adc_vsupply": (lambda v: "0x%03x(%.3fV)" % (v, _adc_to_volts(v))),
        # ADC_I_*/AMPL_MEAS are unavailable in SpreadCycle (datasheet p.76).
        # DO_SCOPE_CONF selectors 0x13/0x14 expose DAC_A/B chopper-command
        # timing separately; they are not measured phase-current registers.
        "adc_i_a": (lambda v: "%d(SpreadCycle?)" % v if v == 0 else "%d" % v),
        "adc_i_b": (lambda v: "%d(SpreadCycle?)" % v if v == 0 else "%d" % v),
        "ampl_meas": (lambda v: "%d(SpreadCycle?)" % v if v == 0 else "%d" % v),
        "angle_meas": (lambda v: "%d(%.1fdeg)" % (v, v * 360.0 / 1024.0)),
        "overvoltage_vth": (lambda v: "0x%03x(%.3fV)" % (v, _adc_to_volts(v))),
        "overtempprewarning_vth": (
            lambda v: "0x%03x(%.1fC)" % (v, _adc_to_celsius(v))
        ),
    }
)


######################################################################
# TMC5262 current helper (Integrated Current Sense, no GLOBALSCALER)
######################################################################
# Datasheet Table 21 (p.74):
#   I_FS_peak = 18 * (CR+1) * (CRS+1) / (4 * RREF_kohm)         [A peak]
#   I_FS_RMS  = I_FS_peak / sqrt(2)                              [A RMS]
#   I_RMS(IRUN) = I_FS_RMS * IRUN / 250
# CRS != 3 is only meaningful when CR == 0 per datasheet.

DEFAULT_RREF = 12000.0
KIFS_BASE = 18.0  # peak full-scale * Rref_kohm at CR=0, CRS=0


class TMC5262CurrentHelper:
    def __init__(self, config, mcu_tmc):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        self.Rref = config.getfloat(
            "rref", DEFAULT_RREF, minval=10000.0, maxval=14000.0
        )
        max_current = self._ifs_rms_for(3, 3)
        run_current = config.getfloat(
            "run_current", above=0.0, maxval=max_current
        )
        hold_current = config.getfloat(
            "hold_current", run_current, above=0.0, maxval=max_current
        )
        self.req_hold_current = hold_current

        cr, crs = self._calc_ranges(run_current)
        self.current_range = config.getint(
            "current_range", cr, minval=0, maxval=3
        )
        self.current_range_scale = config.getint(
            "current_range_scale", crs, minval=0, maxval=3
        )
        if self.current_range != 0 and self.current_range_scale != 3:
            raise config.error(
                "tmc5262 %s: current_range_scale<3 is only valid with "
                "current_range=0 per datasheet" % (self.name,)
            )
        self.fields.set_field("current_range", self.current_range)
        self.fields.set_field("current_range_scale", self.current_range_scale)
        if run_current > self._ifs_rms():
            raise config.error(
                "tmc5262 %s: run_current %.3f exceeds %.3fA RMS for "
                "current_range=%d current_range_scale=%d"
                % (
                    self.name,
                    run_current,
                    self._ifs_rms(),
                    self.current_range,
                    self.current_range_scale,
                )
            )

        irun, ihold = self._calc_current(run_current, hold_current)
        self.fields.set_field("irun", irun)
        self.fields.set_field("ihold", ihold)

    def _ifs_rms_for(self, cr, crs):
        i_peak = KIFS_BASE * (cr + 1) * (crs + 1) / (4.0 * self.Rref / 1000.0)
        return i_peak / math.sqrt(2.0)

    def _ifs_rms(self):
        cr = self.fields.get_field("current_range")
        crs = self.fields.get_field("current_range_scale")
        return self._ifs_rms_for(cr, crs)

    def _calc_ranges(self, current):
        # Smallest CR that fits at CRS=3; fall back to CRS<3 only at CR=0.
        for cr in range(4):
            if current <= self._ifs_rms_for(cr, 3):
                if cr == 0:
                    for crs in range(4):
                        if current <= self._ifs_rms_for(0, crs):
                            return 0, crs
                return cr, 3
        return 3, 3

    def _calc_current(self, run_current, hold_current):
        ifs = self._ifs_rms()
        # IRUN 251..255 are clipped to 250 internally.
        irun = max(1, min(250, int(round(run_current / ifs * 250.0))))
        if run_current > 0.0:
            ihold = int(round(hold_current / run_current * irun))
        else:
            ihold = 0
        ihold = max(0, min(irun, ihold))
        return irun, ihold

    def _calc_current_from_field(self, field_name):
        return self._ifs_rms() * self.fields.get_field(field_name) / 250.0

    def get_current(self):
        return (
            self._calc_current_from_field("irun"),
            self._calc_current_from_field("ihold"),
            self.req_hold_current,
            self._ifs_rms(),
        )

    def get_current_range(self):
        return self._ifs_rms()

    def set_current(self, run_current, hold_current, print_time):
        self.req_hold_current = hold_current
        irun, ihold = self._calc_current(run_current, hold_current)
        self.fields.set_field("ihold", ihold)
        val = self.fields.set_field("irun", irun)
        self.mcu_tmc.set_register("IHOLD_IRUN", val, print_time)


######################################################################
# TMC5262 main object
######################################################################


class TMC5262:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.mcu_tmc = tmc2130.MCU_TMC_SPI(
            config, Registers, self.fields, TMC_FREQUENCY
        )
        # tmc.TMCErrorCheck reads this via getattr to use the chip-specific
        # 9-bit ADC formula instead of the default TMC2240 one.
        self.mcu_tmc.temp_from_adc = _adc_to_celsius
        # Keep the temperature monitor populated while the motor is disabled.
        # The common helper polls ADC_TEMP only for drivers that opt in.
        self.mcu_tmc.poll_temperature_when_idle = True
        # PLL must come up before any other register write the chip is
        # meant to react to - register before TMCCommandHelper.
        self.printer.register_event_handler(
            "klippy:connect", self._handle_pll_init
        )
        # Sensorless homing: TMC5262 routes stall through DO_CONF.do0_stall
        # / do1_stall (datasheet pg 140), unlike GCONF.diag*_stall on older
        # chips. Prime do0_invpp / do1_invpp = 1 (chip-default active-low)
        # so the homing handler's stall-bit writes don't clear them.
        self.fields.set_field("do0_invpp", 1)
        self.fields.set_field("do1_invpp", 1)
        virtual_pin_helper = tmc.TMCVirtualPinHelper(config, self.mcu_tmc)
        if config.get("diag0_pin", None) is not None:
            virtual_pin_helper.diag_pin = config.get("diag0_pin")
            virtual_pin_helper.diag_pin_field = "do0_stall"
        elif config.get("diag1_pin", None) is not None:
            virtual_pin_helper.diag_pin = config.get("diag1_pin")
            virtual_pin_helper.diag_pin_field = "do1_stall"
        # Register commands
        current_helper = TMC5262CurrentHelper(config, self.mcu_tmc)
        self.current_helper = current_helper
        cmdhelper = tmc.TMCCommandHelper(config, self.mcu_tmc, current_helper)
        cmdhelper.setup_register_dump(ReadRegisters)
        self.get_phase_offset = cmdhelper.get_phase_offset
        self.get_status = cmdhelper.get_status

        # Optional cold-reference data for the on-demand motor winding
        # temperature estimate.  The capture command prints these exact
        # settings; nothing is written to printer.cfg automatically.
        reference_temperature = config.getfloat(
            "coil_temperature_reference_celsius", None,
            minval=-40.0, maxval=100.0
        )
        reference_a = config.getfloat(
            "coil_temperature_reference_resistance_a", None, above=0.0
        )
        reference_b = config.getfloat(
            "coil_temperature_reference_resistance_b", None, above=0.0
        )
        reference_current_range = config.getfloat(
            "coil_temperature_reference_current_range", None, above=0.0
        )
        reference_values = (
            reference_temperature, reference_a, reference_b,
            reference_current_range,
        )
        if any(value is not None for value in reference_values):
            if not all(value is not None for value in reference_values):
                raise config.error(
                    "tmc5262 %s: coil temperature reference requires "
                    "coil_temperature_reference_celsius and both "
                    "coil_temperature_reference_resistance_a/b plus "
                    "coil_temperature_reference_current_range"
                    % self.name
                )
            if not all(math.isfinite(value) for value in reference_values):
                raise config.error(
                    "tmc5262 %s: coil temperature reference values must be "
                    "finite" % self.name
                )
            self._coil_temperature_reference = {
                "temperature": reference_temperature,
                "resistance_a": reference_a,
                "resistance_b": reference_b,
                "current_range": reference_current_range,
            }
        else:
            self._coil_temperature_reference = None

        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command(
            "TMC_COIL_TEMP_REFERENCE_5262", "STEPPER", self.name,
            self.cmd_COIL_TEMP_REFERENCE,
            desc=self.cmd_COIL_TEMP_REFERENCE_help,
        )
        gcode.register_mux_command(
            "TMC_COIL_TEMP_5262", "STEPPER", self.name,
            self.cmd_COIL_TEMP,
            desc=self.cmd_COIL_TEMP_help,
        )
        # Microstep wave table
        tmc.TMCWaveTableHelper(config, self.mcu_tmc)
        self.fields.set_config_field(config, "offset_sin90", 0)
        # StealthChop / CoolStep velocity thresholds.  Klipper's common helper
        # provides the same mode-threshold behavior as TMC5160.  A positive
        # threshold additionally activates the TMC5262 motor-model preflight
        # below; a configured zero keeps motion in SpreadCycle.
        self.stealthchop_velocity = config.getfloat(
            "stealthchop_threshold", None, minval=0.0
        )
        self.stealthchop_plus_configured = (
            self.stealthchop_velocity is not None
            and self.stealthchop_velocity > 0.0
        )
        tmc.TMCStealthchopHelper(config, self.mcu_tmc)
        tmc.TMCVcoolthrsHelper(config, self.mcu_tmc)
        tmc.TMCVhighHelper(config, self.mcu_tmc)
        # Allow other registers to be set from the config
        set_config_field = self.fields.set_config_field
        def set_optional_config_field(field_name):
            # Some advanced control registers have silicon defaults that
            # should remain untouched unless the user explicitly opts in.
            configure_optional_field(config, self.fields, field_name)
        # PLL steady-state. _handle_pll_init runs the boot sequence; this
        # makes cmd_INIT_TMC re-issue the same value and DUMP_TMC report it.
        self.fields.set_field("clock_divider", 15)
        self.fields.set_field("clk_sys_sel", 1)
        self.fields.set_field("clk_fsm_ena", 1)
        # GCONF: step_dir is required for use as an external step/dir
        # driver. Bits not set here are written as 0 by INIT_TMC, including
        # small_hysteresis (chip default is 1, ends up 0 - matches TMC2240).
        set_config_field(config, "step_dir", True)
        set_config_field(config, "multistep_filt", True)
        # CHOPCONF
        set_config_field(config, "toff", 3)
        set_config_field(config, "hstrt", 5)
        set_config_field(config, "hend", 2)
        set_config_field(config, "fd3", 0)
        set_config_field(config, "disfdcc", 0)
        set_config_field(config, "chm", 0)
        set_config_field(config, "tbl", 2)
        set_config_field(config, "tpfd", 4)
        # COOLCONF
        set_config_field(config, "semin", 0)
        set_config_field(config, "seup", 0)
        set_config_field(config, "semax", 0)
        set_config_field(config, "sedn", 0)
        set_config_field(config, "seimin", 0)
        set_config_field(config, "sgt", 0)
        set_config_field(config, "sfilt", 0)
        # IHOLDIRUN
        set_config_field(config, "iholddelay", 7)
        set_config_field(config, "irundelay", 4)
        # PWMCONF. SD_ON_MEAS_LO/HI control when the ICS samples coil
        # currents within the chopper cycle (Rev.0 Table 8, p.41).
        # Explicit measurement thresholds remain user overrides.
        set_config_field(config, "pwm_freq", 0)
        set_config_field(config, "freewheel", 0)
        try:
            sd_lo, sd_hi = pwm_measurement_defaults(
                self.fields.get_field("pwm_freq"))
        except ValueError as exc:
            raise config.error(str(exc))
        set_config_field(config, "sd_on_meas_lo", sd_lo)
        set_config_field(config, "sd_on_meas_hi", sd_hi)
        # TPOWERDOWN
        set_config_field(config, "tpowerdown", 10)
        # DRV_CONF
        set_config_field(config, "slope_control", 3)
        # StealthChop+ PI regulators - chip reset values, exposed for tuning.
        set_config_field(config, "cur_p", 64)
        set_config_field(config, "cur_i", 10)
        set_config_field(config, "angle_p", 50)
        set_config_field(config, "angle_i", 20)
        set_config_field(config, "cur_pi_limit", 0xFFF)
        set_config_field(config, "angle_pi_limit", 256)
        set_config_field(config, "angle_lower_i_limit", 256)
        # TMC5262 StealthChop+ motor model.  Unlike TMC5160, COIL_INDUCT is
        # required and RCOIL must be measured before normal-speed motion.
        # Program these fields deterministically even in SpreadCycle so a host
        # FIRMWARE_RESTART cannot retain values written by an earlier session.
        set_config_field(config, "t_rcoil_meas", 4096)
        set_config_field(config, "coil_induct", 0)
        set_config_field(config, "rcoil_manual", False)
        set_config_field(
            config,
            "rcoil_thermal_coupling",
            self.stealthchop_plus_configured,
        )
        set_config_field(config, "r_coil_user_a", 0)
        set_config_field(config, "r_coil_user_b", 0)
        if self.stealthchop_plus_configured:
            coil_induct = self.fields.get_field("coil_induct")
            if not coil_induct:
                raise config.error(
                    "tmc5262 %s: StealthChop+ requires "
                    "driver_COIL_INDUCT in microhenries (for example, "
                    "1.5mH is 1500uH)" % (self.name,)
                )
            if self.fields.get_field("rcoil_manual"):
                rcoil_a = self.fields.get_field("r_coil_user_a")
                rcoil_b = self.fields.get_field("r_coil_user_b")
                if not rcoil_a or not rcoil_b:
                    raise config.error(
                        "tmc5262 %s: driver_RCOIL_MANUAL requires nonzero "
                        "driver_R_COIL_USER_A and driver_R_COIL_USER_B"
                        % (self.name,)
                    )

        # RT-OCSI routing and StallGuard+ / CoolStep+ controls. Read-only
        # companion fields are exposed via DUMP_TMC but intentionally cannot
        # be configured here.
        for field_name in (
            "do0_scope_en",
            "do0_scope_sel",
            "do1_scope_en",
            "do1_scope_sel",
            "tsgp_low_vel_thrs",
            "sgp_thrs",
            "sgp_filt_en",
            "sgp_low_vel_freeze",
            "sgp_clear_cur_pi",
            "sgp_low_vel_slope",
            "sgp_low_vel_cnts",
            "cool_cur_div",
            "load_filt_en",
            "coolstep_p",
            "coolstep_i",
            "cool_pi_down_limit",
            "cool_pi_off_speed",
            "cool_low_load_reserve",
            "cool_hi_load_reserve",
            "cool_low_generatoric_reserve",
            "cool_hi_generatoric_reserve",
        ):
            set_optional_config_field(field_name)

        self._order_register_cache()
        if self.stealthchop_plus_configured:
            self.stepper_enable = self.printer.load_object(
                config, "stepper_enable"
            )
            self.printer.register_event_handler(
                "homing:home_rails_begin", self._handle_home_rails_begin
            )

    cmd_COIL_TEMP_REFERENCE_help = (
        "Capture a known-temperature TMC5262 winding-resistance reference"
    )
    cmd_COIL_TEMP_help = (
        "Read the referenced TMC5262 motor winding temperature estimate"
    )

    def _check_coil_temperature_idle(self):
        print_stats = self.printer.lookup_object("print_stats", None)
        if print_stats is None:
            return
        eventtime = self.printer.get_reactor().monotonic()
        state = print_stats.get_status(eventtime).get("state", "")
        if state in ("printing", "paused"):
            raise self.printer.command_error(
                "TMC5262 %s coil-temperature sampling is not permitted "
                "while a print is %s" % (self.name, state)
            )

    def _read_coil_resistance_snapshot(self):
        drv_status = self.mcu_tmc.get_register("DRV_STATUS")
        tstep_register = self.mcu_tmc.get_register("TSTEP")
        threshold_register = self.mcu_tmc.get_register("T_RCOIL_MEAS")
        rcoil_register = self.mcu_tmc.get_register("R_COIL")
        hard_faults = [
            field for field in ("s2vsa", "s2vsb", "s2ga", "s2gb", "ot")
            if self.fields.get_field(
                field, drv_status, reg_name="DRV_STATUS"
            )
        ]
        if hard_faults:
            raise self.printer.command_error(
                "TMC5262 %s cannot estimate coil temperature: driver "
                "reported %s" % (self.name, ", ".join(hard_faults))
            )
        if not self.fields.get_field(
                "stealth", drv_status, reg_name="DRV_STATUS"):
            raise self.printer.command_error(
                "TMC5262 %s coil resistance is not being measured in "
                "StealthChop+" % self.name
            )
        if not self.fields.get_field(
                "stst", drv_status, reg_name="DRV_STATUS"):
            raise self.printer.command_error(
                "TMC5262 %s must be at standstill for this temperature "
                "sample" % self.name
            )
        cs_actual = self.fields.get_field(
            "cs_actual", drv_status, reg_name="DRV_STATUS"
        )
        if cs_actual < 50:
            raise self.printer.command_error(
                "TMC5262 %s cannot update RCOIL: CS_ACTUAL=%d is below 50"
                % (self.name, cs_actual)
            )
        tstep = self.fields.get_field(
            "tstep", tstep_register, reg_name="TSTEP"
        )
        threshold = self.fields.get_field(
            "t_rcoil_meas", threshold_register, reg_name="T_RCOIL_MEAS"
        )
        if tstep <= threshold:
            raise self.printer.command_error(
                "TMC5262 %s cannot update RCOIL: TSTEP=%d is not above "
                "T_RCOIL_MEAS=%d" % (self.name, tstep, threshold)
            )
        rcoil_a = self.fields.get_field(
            "r_coil_auto_a", rcoil_register, reg_name="R_COIL"
        )
        rcoil_b = self.fields.get_field(
            "r_coil_auto_b", rcoil_register, reg_name="R_COIL"
        )
        if not rcoil_a or not rcoil_b:
            raise self.printer.command_error(
                "TMC5262 %s R_COIL_AUTO_A/B are not both valid "
                "(A=%d, B=%d)" % (self.name, rcoil_a, rcoil_b)
            )
        return {
            "raw_a": rcoil_a,
            "raw_b": rcoil_b,
            "cs_actual": cs_actual,
        }

    def _collect_stable_rcoil_cycle(self, toolhead):
        reactor = self.printer.get_reactor()
        samples = []
        try:
            self.stepper_enable.set_motors_enable([self.name], True)
            # TMCCommandHelper restores registers from an asynchronous enable
            # callback.  Yield first so the RCOIL settling interval begins
            # after that callback, not while it is still pending.
            eventtime = reactor.monotonic()
            reactor.pause(eventtime + RCOIL_ENABLE_CALLBACK_DWELL)
            toolhead.dwell(RCOIL_PREFLIGHT_DWELL)
            toolhead.wait_moves()
            for sample_index in range(RCOIL_MAX_SAMPLE_COUNT):
                samples.append(self._read_coil_resistance_snapshot())
                if len(samples) >= RCOIL_STABLE_SAMPLE_COUNT:
                    window = samples[-RCOIL_STABLE_SAMPLE_COUNT:]
                    raw_a = [sample["raw_a"] for sample in window]
                    raw_b = [sample["raw_b"] for sample in window]
                    if (_rcoil_values_repeatable(raw_a)
                            and _rcoil_values_repeatable(raw_b)):
                        return {
                            "raw_a": int(round(sum(raw_a) / len(raw_a))),
                            "raw_b": int(round(sum(raw_b) / len(raw_b))),
                            "cs_actual": min(
                                sample["cs_actual"] for sample in window
                            ),
                            "history_a": [
                                sample["raw_a"] for sample in samples
                            ],
                            "history_b": [
                                sample["raw_b"] for sample in samples
                            ],
                        }
                if sample_index + 1 < RCOIL_MAX_SAMPLE_COUNT:
                    toolhead.dwell(RCOIL_SAMPLE_INTERVAL)
                    toolhead.wait_moves()
            raise self.printer.command_error(
                "TMC5262 %s RCOIL did not converge after %d samples: "
                "A=%s; B=%s"
                % (
                    self.name, len(samples),
                    ",".join(str(sample["raw_a"]) for sample in samples),
                    ",".join(str(sample["raw_b"]) for sample in samples),
                )
            )
        finally:
            self.stepper_enable.set_motors_enable([self.name], False)
            eventtime = reactor.monotonic()
            reactor.pause(eventtime + RCOIL_ENABLE_CALLBACK_DWELL)
            toolhead.wait_moves()

    def _sample_coil_resistance(self):
        if not self.stealthchop_plus_configured:
            raise self.printer.command_error(
                "TMC5262 %s coil-temperature estimation requires a positive "
                "stealthchop_threshold" % self.name
            )
        self._check_coil_temperature_idle()
        toolhead = self.printer.lookup_object("toolhead")
        toolhead.wait_moves()
        enable_line = self.stepper_enable.lookup_enable(self.name)
        if enable_line.is_motor_enabled():
            raise self.printer.command_error(
                "TMC5262 %s must be disabled before coil-temperature "
                "sampling so two repeatable enable cycles can be measured; "
                "run M84, then retry" % self.name
            )
        configured_irun = self.fields.get_field("irun")
        if configured_irun < RCOIL_SAMPLE_CURRENT_SCALE:
            raise self.printer.command_error(
                "TMC5262 %s configured IRUN scale %d is below the "
                "coil-temperature sampling scale %d; no measurement was "
                "started"
                % (
                    self.name, configured_irun,
                    RCOIL_SAMPLE_CURRENT_SCALE,
                )
            )
        original_run_current, _original_hold_current, original_req_hold, \
            current_range = self.current_helper.get_current()
        sample_current = (
            current_range * RCOIL_SAMPLE_CURRENT_SCALE / 250.0
        )
        try:
            print_time = toolhead.get_last_move_time()
            self.current_helper.set_current(
                sample_current, sample_current, print_time
            )
            toolhead.wait_moves()
            cycles = [
                self._collect_stable_rcoil_cycle(toolhead)
                for _unused in range(RCOIL_ACQUISITION_CYCLES)
            ]
        finally:
            print_time = toolhead.get_last_move_time()
            self.current_helper.set_current(
                original_run_current, original_req_hold, print_time
            )
            toolhead.wait_moves()
        cycle_raw_a = [cycle["raw_a"] for cycle in cycles]
        cycle_raw_b = [cycle["raw_b"] for cycle in cycles]
        if (not _rcoil_values_repeatable(cycle_raw_a)
                or not _rcoil_values_repeatable(cycle_raw_b)):
            raise self.printer.command_error(
                "TMC5262 %s RCOIL enable-cycle repeatability failed: "
                "A=%s; B=%s. No new RCOIL result was accepted."
                % (
                    self.name,
                    ",".join(str(value) for value in cycle_raw_a),
                    ",".join(str(value) for value in cycle_raw_b),
                )
            )
        rcoil_a = int(round(sum(cycle_raw_a) / len(cycle_raw_a)))
        rcoil_b = int(round(sum(cycle_raw_b) / len(cycle_raw_b)))
        try:
            resistance_a = _rcoil_to_effective_resistance(
                rcoil_a, current_range
            )
            resistance_b = _rcoil_to_effective_resistance(
                rcoil_b, current_range
            )
        except ValueError as exc:
            raise self.printer.command_error(
                "TMC5262 %s invalid coil-resistance sample: %s"
                % (self.name, str(exc))
            )
        return {
            "raw_a": rcoil_a,
            "raw_b": rcoil_b,
            "resistance_a": resistance_a,
            "resistance_b": resistance_b,
            "current_range": current_range,
            "cs_actual": min(cycle["cs_actual"] for cycle in cycles),
            "cycle_raw_a": cycle_raw_a,
            "cycle_raw_b": cycle_raw_b,
            "sample_current": sample_current,
            "sample_current_scale": RCOIL_SAMPLE_CURRENT_SCALE,
        }

    def cmd_COIL_TEMP_REFERENCE(self, gcmd):
        reference_temperature = gcmd.get_float(
            "TEMPERATURE", minval=-40.0, maxval=100.0
        )
        if not math.isfinite(reference_temperature):
            raise gcmd.error("TEMPERATURE must be finite")
        sample = self._sample_coil_resistance()
        self._coil_temperature_reference = {
            "temperature": reference_temperature,
            "resistance_a": sample["resistance_a"],
            "resistance_b": sample["resistance_b"],
            "current_range": sample["current_range"],
        }
        gcmd.respond_info(
            "TMC5262 %s coil-temperature reference captured at %.2f C.\n"
            "R_COIL_AUTO: A=%d B=%d; effective paths: A=%.6f ohm "
            "B=%.6f ohm.\n"
            "Independent enable cycles: A=%s B=%s (repeatability passed).\n"
            "Measurement current: %.3f A RMS at scale %d; configured "
            "current restored.\n"
            "The reference is active until restart. To keep it, copy these "
            "lines into [tmc5262 %s]:\n"
            "coil_temperature_reference_celsius: %.6f\n"
            "coil_temperature_reference_resistance_a: %.9f\n"
            "coil_temperature_reference_resistance_b: %.9f\n"
            "coil_temperature_reference_current_range: %.9f"
            % (
                self.name, reference_temperature,
                sample["raw_a"], sample["raw_b"],
                sample["resistance_a"], sample["resistance_b"],
                "/".join(str(value) for value in sample["cycle_raw_a"]),
                "/".join(str(value) for value in sample["cycle_raw_b"]),
                sample["sample_current"], sample["sample_current_scale"],
                self.name, reference_temperature,
                sample["resistance_a"], sample["resistance_b"],
                sample["current_range"],
            )
        )

    def cmd_COIL_TEMP(self, gcmd):
        reference = self._coil_temperature_reference
        if reference is None:
            raise gcmd.error(
                "TMC5262 %s has no coil-temperature reference. Let the "
                "motor reach a known cold temperature, then run "
                "TMC_COIL_TEMP_REFERENCE_5262 STEPPER=%s TEMPERATURE=<C>."
                % (self.name, self.name)
            )
        sample = self._sample_coil_resistance()
        if not math.isclose(
                sample["current_range"], reference["current_range"],
                rel_tol=1.0e-6, abs_tol=1.0e-9):
            raise gcmd.error(
                "TMC5262 %s current range changed from reference %.6f A "
                "RMS to %.6f A RMS; capture a new cold reference"
                % (
                    self.name, reference["current_range"],
                    sample["current_range"],
                )
            )
        try:
            temperature_a = _estimate_coil_temperature(
                sample["resistance_a"], reference["resistance_a"],
                reference["temperature"]
            )
            temperature_b = _estimate_coil_temperature(
                sample["resistance_b"], reference["resistance_b"],
                reference["temperature"]
            )
        except ValueError as exc:
            raise gcmd.error(
                "TMC5262 %s invalid coil-temperature reference: %s"
                % (self.name, str(exc))
            )
        average = 0.5 * (temperature_a + temperature_b)
        gcmd.respond_info(
            "TMC5262 %s estimated motor-coil temperature: %.1f C "
            "average (A=%.1f C, B=%.1f C).\n"
            "R_COIL_AUTO: A=%d B=%d; effective paths: A=%.6f ohm "
            "B=%.6f ohm; reference %.1f C.\n"
            "Independent enable cycles: A=%s B=%s (repeatability passed).\n"
            "Measurement current: %.3f A RMS at scale %d; configured "
            "current restored.\n"
            "Estimate only: RCOIL includes bridge and wiring resistance; "
            "absolute accuracy is reduced for low-resistance motors."
            % (
                self.name, average, temperature_a, temperature_b,
                sample["raw_a"], sample["raw_b"],
                sample["resistance_a"], sample["resistance_b"],
                reference["temperature"],
                "/".join(str(value) for value in sample["cycle_raw_a"]),
                "/".join(str(value) for value in sample["cycle_raw_b"]),
                sample["sample_current"], sample["sample_current_scale"],
            )
        )

    def _order_register_cache(self):
        registers = self.fields.registers
        for register in TMC5262_REGISTER_INIT_ORDER:
            if register in registers:
                registers.move_to_end(register)

    def _home_uses_this_stepper(self, rails):
        return any(
            stepper.get_name() == self.name
            for rail in rails
            for stepper in rail.get_steppers()
        )

    def _abort_rcoil_preflight(self, message):
        # This event runs before homing establishes a forced position or emits
        # any step pulses.  Disable all motors before rejecting the move.
        try:
            self.stepper_enable.motor_off()
        except self.printer.command_error:
            logging.exception(
                "TMC5262 %s could not disable motors after RCOIL failure",
                self.name,
            )
        raise self.printer.command_error(
            "TMC5262 %s StealthChop+ RCOIL preflight failed: %s. "
            "No homing move was started." % (self.name, message)
        )

    def _handle_home_rails_begin(self, homing_state, rails):
        del homing_state
        if not self._home_uses_this_stepper(rails):
            return
        if self.fields.get_field("rcoil_manual"):
            return

        # Energize at the user's configured current, without changing it, and
        # allow the driver's standstill RCOIL filter to complete.  Page 48 of
        # the datasheet says the first measurable coil is copied to both coils.
        try:
            enable_line = self.stepper_enable.lookup_enable(self.name)
            did_enable = False
            if not enable_line.is_motor_enabled():
                self.stepper_enable.set_motors_enable([self.name], True)
                did_enable = True
            toolhead = self.printer.lookup_object("toolhead")
            if did_enable:
                reactor = self.printer.get_reactor()
                eventtime = reactor.monotonic()
                reactor.pause(eventtime + RCOIL_ENABLE_CALLBACK_DWELL)
            toolhead.dwell(RCOIL_PREFLIGHT_DWELL)
            toolhead.wait_moves()

            drv_status = self.mcu_tmc.get_register("DRV_STATUS")
            cs_actual = self.fields.get_field(
                "cs_actual", drv_status, reg_name="DRV_STATUS"
            )
            rcoil = self.mcu_tmc.get_register("R_COIL")
            rcoil_a = self.fields.get_field(
                "r_coil_auto_a", rcoil, reg_name="R_COIL"
            )
            rcoil_b = self.fields.get_field(
                "r_coil_auto_b", rcoil, reg_name="R_COIL"
            )
        except self.printer.command_error as e:
            self._abort_rcoil_preflight(str(e))

        hard_faults = [
            field
            for field in ("s2vsa", "s2vsb", "s2ga", "s2gb", "ot")
            if self.fields.get_field(
                field, drv_status, reg_name="DRV_STATUS"
            )
        ]
        if hard_faults:
            self._abort_rcoil_preflight(
                "driver reported %s" % ", ".join(hard_faults)
            )
        if cs_actual < 50:
            self._abort_rcoil_preflight(
                "CS_ACTUAL=%d is below the required 50" % cs_actual
            )
        if not rcoil_a or not rcoil_b:
            self._abort_rcoil_preflight(
                "R_COIL_AUTO_A/B did not both become nonzero "
                "(A=%d, B=%d)" % (rcoil_a, rcoil_b)
            )
        logging.info(
            "TMC5262 %s StealthChop+ RCOIL ready: A=%d B=%d "
            "CS_ACTUAL=%d",
            self.name,
            rcoil_a,
            rcoil_b,
            cs_actual,
        )

    def _build_pll_value(self, commit, clk_fsm_ena, clear_flags=False):
        # Rev.0 datasheet p.149, internal oscillator sequence on p.106.
        # Built from raw bits so __init__'s steady-state cache stays intact.
        val = (
            (1 if commit else 0) << 0
            | (0 << 1)  # ext_not_int = 0 (internal oscillator)
            | (1 << 2)  # clk_sys_sel = 1 (use PLL)
            | ((1 if clk_fsm_ena else 0) << 3)
            | (15 << 5)  # CLOCK_DIVIDER = 15 -> 1MHz PLL reference
        )
        if clear_flags:
            # CLK_LOSS and CLK_IS_STUCK are write-one-to-clear.
            val |= (1 << 13) | (1 << 14)
        return val

    def _handle_pll_init(self):
        try:
            # Stage 1 is the datasheet's exact 0x01ED internal-clock value:
            # program the divider, enable the FSM, and assert COMMIT.
            self.mcu_tmc.set_register(
                "PLL", self._build_pll_value(commit=1, clk_fsm_ena=1)
            )
            # Wait for commit to self-clear (PLL locked). Cap at 20ms.
            reactor = self.printer.get_reactor()
            deadline = reactor.monotonic() + 0.020
            commit_cleared = False
            while reactor.monotonic() < deadline:
                reactor.pause(reactor.monotonic() + 0.001)
                pll = self.mcu_tmc.get_register("PLL")
                if not (pll & 0x01):
                    commit_cleared = True
                    break
            if not commit_cleared:
                raise self.printer.command_error(
                    "TMC5262 %s PLL commit timed out" % self.name)
            # Stage 2 is the datasheet's exact 0x61EC value: COMMIT is clear
            # and both sticky clock flags are acknowledged once.
            stage2 = self._build_pll_value(
                commit=0, clk_fsm_ena=1, clear_flags=True
            )
            self.mcu_tmc.set_register("PLL", stage2)
            # Stage 1 puts the chip in reset, which sets GSTAT.reset /
            # register_reset / uv_cp. Clear so TMCErrorCheck doesn't trip.
            self.mcu_tmc.set_register("GSTAT", 0x3F)
        except self.printer.command_error as e:
            logging.error("TMC5262 %s PLL init failed: %s", self.name, str(e))
            raise


def load_config_prefix(config):
    return TMC5262(config)
