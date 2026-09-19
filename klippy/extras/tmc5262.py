# TMC5262 configuration
#
# Copyright (C) 2026  Kalico Contributors
# Copyright (C) 2026  Uriah Kessman <fishyfabspnw@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
from . import tmc, tmc2130

TMC_FREQUENCY = 16000000.

# Preserve silicon defaults when a user overrides one field in an optional
# TMC5262-specific register.
OPTIONAL_REGISTER_DEFAULTS = {
    "DO_SCOPE_CONF": 0x00,
    "TSGP_LOW_VEL_THRS": 0x1000,
    "COOLSTEPPLUS_CONF": 0x12,
    "COOLSTEPPLUS_PI_REG": (16 << 16) | 128,
    "COOLSTEPPLUS_PI_DOWN": (64 << 16) | 128,
    "COOLSTEPPLUS_RESERVE_CONF": (100 << 24) | (220 << 16) | (50 << 8) | 150,
    "SGP_CONF": 1 << 14,
}

# A complete filtered RCOIL update can take up to 100ms. Allow additional
# time for queued enable and register writes before a homing move.
RCOIL_PREFLIGHT_DWELL = .250
RCOIL_ENABLE_CALLBACK_DWELL = .010

# Keep TMC5262-specific initialization deterministic. In particular, configure
# the motor model before enabling StealthChop+ and write CHOPCONF.TOFF last.
TMC5262_REGISTER_INIT_ORDER = (
    "DRV_CONF", "IHOLD_IRUN", "TPOWERDOWN", "COIL_INDUCT",
    "R_COIL_USER", "T_RCOIL_MEAS", "CURRENT_PI_REG", "ANGLE_PI_REG",
    "CUR_ANGLE_LIMIT", "ANGLE_LOWER_LIMIT", "PWMCONF", "TPWMTHRS",
    "TCOOLTHRS", "THIGH", "TSGP_LOW_VEL_THRS", "DO_CONF",
    "DO_SCOPE_CONF", "GCONF", "CHOPCONF",
)


OPTIONAL_FIELD_LIMITS = {
    "do0_scope_sel": 0x1c,
    "do1_scope_sel": 0x1c,
    "cool_cur_div": 10,
}


def configure_optional_field(config, fields, field_name):
    option = "driver_" + field_name.upper()
    if config.get(option, None) is None:
        return
    register = fields.lookup_register(field_name)
    if (register not in fields.registers
            and register in OPTIONAL_REGISTER_DEFAULTS):
        fields.registers[register] = OPTIONAL_REGISTER_DEFAULTS[register]
    maxval = OPTIONAL_FIELD_LIMITS.get(field_name)
    if maxval is not None:
        value = config.getint(option, minval=0, maxval=maxval)
        fields.set_field(field_name, value)
    else:
        fields.set_config_field(config, field_name, 0)


def pwm_measurement_defaults(pwm_freq):
    # TMC5262 datasheet table 8.
    if not 0 <= pwm_freq <= 8:
        raise ValueError("driver_PWM_FREQ must be in range 0..8")
    if pwm_freq <= 2:
        return 14, 15
    if pwm_freq <= 5:
        return 13, 14
    return 12, 13


def _adc_to_celsius(adc_value):
    return 1.042 * adc_value - 264.6


def _adc_to_volts(adc_value):
    return adc_value * .1409


######################################################################
# Register addresses
######################################################################

Registers = {
    "GCONF": 0x00,
    "GSTAT": 0x01,
    "DO_CONF": 0x02,
    "DO_SCOPE_CONF": 0x03,
    "IOIN": 0x04,
    "DRV_CONF": 0x0a,
    "PLL": 0x0b,
    "IHOLD_IRUN": 0x10,
    "TPOWERDOWN": 0x11,
    "TSTEP": 0x12,
    "TPWMTHRS": 0x13,
    "TCOOLTHRS": 0x14,
    "THIGH": 0x15,
    "TSGP_LOW_VEL_THRS": 0x16,
    "T_RCOIL_MEAS": 0x17,
    "STEPS_LOST": 0x1a,
    "CURRENT_PI_REG": 0x40,
    "ANGLE_PI_REG": 0x41,
    "CUR_ANGLE_LIMIT": 0x42,
    "ANGLE_LOWER_LIMIT": 0x43,
    "CUR_ANGLE_MEAS": 0x44,
    "PI_RESULTS": 0x45,
    "COIL_INDUCT": 0x46,
    "R_COIL": 0x47,
    "R_COIL_USER": 0x48,
    "SGP_CONF": 0x49,
    "SGP_IND_2_3": 0x4a,
    "SGP_IND_0_1": 0x4b,
    "INDUCTANCE_VOLTAGE": 0x4c,
    "SGP_BEMF": 0x4d,
    "COOLSTEPPLUS_CONF": 0x4e,
    "COOLSTEPPLUS_PI_REG": 0x4f,
    "COOLSTEPPLUS_PI_DOWN": 0x50,
    "COOLSTEPPLUS_RESERVE_CONF": 0x51,
    "COOLSTEPPLUS_LOAD_RESERVE": 0x52,
    "TSTEP_VELOCITY": 0x53,
    "ADC_VSUPPLY_TEMP": 0x58,
    "ADC_I": 0x59,
    "OTW_OV_VTH": 0x5a,
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
    "MSCNT": 0x6a,
    "MSCURACT": 0x6b,
    "CHOPCONF": 0x6c,
    "COOLCONF": 0x6d,
    "DRV_STATUS": 0x6f,
    "PWMCONF": 0x70,
}

ReadRegisters = [
    "GCONF", "GSTAT", "DO_CONF", "DO_SCOPE_CONF", "IOIN", "DRV_CONF",
    "PLL", "IHOLD_IRUN", "TPOWERDOWN", "TSTEP", "TPWMTHRS",
    "TCOOLTHRS", "THIGH", "TSGP_LOW_VEL_THRS", "T_RCOIL_MEAS",
    "STEPS_LOST", "CURRENT_PI_REG", "ANGLE_PI_REG", "CUR_ANGLE_LIMIT",
    "ANGLE_LOWER_LIMIT", "CUR_ANGLE_MEAS", "PI_RESULTS", "COIL_INDUCT",
    "R_COIL", "R_COIL_USER", "SGP_CONF", "SGP_IND_2_3",
    "SGP_IND_0_1", "INDUCTANCE_VOLTAGE", "SGP_BEMF",
    "COOLSTEPPLUS_CONF", "COOLSTEPPLUS_PI_REG", "COOLSTEPPLUS_PI_DOWN",
    "COOLSTEPPLUS_RESERVE_CONF", "COOLSTEPPLUS_LOAD_RESERVE",
    "TSTEP_VELOCITY", "ADC_VSUPPLY_TEMP", "ADC_I", "OTW_OV_VTH",
    "MSCNT", "MSCURACT", "CHOPCONF", "COOLCONF", "DRV_STATUS",
    "PWMCONF",
]


######################################################################
# Register field maps
######################################################################

Fields = {}
Fields["GCONF"] = {
    "fast_standstill": 0x01 << 0,
    "en_pwm_mode": 0x01 << 1,
    "multistep_filt": 0x01 << 2,
    "shaft": 0x01 << 3,
    "small_hysteresis": 0x01 << 4,
    "stop_enable": 0x01 << 5,
    "direct_mode": 0x01 << 6,
    "length_steppulse": 0x0f << 8,
    "ov_nn": 0x01 << 12,
    "step_dir": 0x01 << 31,
}
Fields["GSTAT"] = {
    "reset": 0x01 << 0,
    "drv_err": 0x01 << 1,
    "uv_cp": 0x01 << 2,
    "register_reset": 0x01 << 3,
    "vm_uvlo": 0x01 << 4,
    "vccio_uv": 0x01 << 5,
}
Fields["DO_CONF"] = {
    "do0_error": 0x01 << 0,
    "do0_otpw": 0x01 << 1,
    "diag0_stall": 0x01 << 2,
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
    "diag1_stall": 0x01 << 15,
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
Fields["DO_SCOPE_CONF"] = {
    "do0_scope_en": 0x01 << 0,
    "do0_scope_sel": 0x1f << 4,
    "do1_scope_en": 0x01 << 12,
    "do1_scope_sel": 0x1f << 16,
}
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
Fields["DRV_CONF"] = {
    "current_range": 0x03 << 0,
    "current_range_scale": 0x03 << 2,
    "slope_control": 0x03 << 4,
}
Fields["PLL"] = {
    "commit": 0x01 << 0,
    "ext_not_int": 0x01 << 1,
    "clk_sys_sel": 0x01 << 2,
    "clk_fsm_ena": 0x01 << 3,
    "clock_divider": 0x1f << 5,
    "clk_1m0_tmo": 0x01 << 12,
    "clk_loss": 0x01 << 13,
    "clk_is_stuck": 0x01 << 14,
}
Fields["IHOLD_IRUN"] = {
    "ihold": 0xff << 0,
    "irun": 0xff << 8,
    "iholddelay": 0xff << 16,
    "irundelay": 0x0f << 24,
}
Fields["TPOWERDOWN"] = {"tpowerdown": 0xff << 0}
Fields["TSTEP"] = {"tstep": 0xfffff << 0}
Fields["TPWMTHRS"] = {"tpwmthrs": 0xfffff << 0}
Fields["TCOOLTHRS"] = {"tcoolthrs": 0xfffff << 0}
Fields["THIGH"] = {"thigh": 0xfffff << 0}
Fields["TSGP_LOW_VEL_THRS"] = {"tsgp_low_vel_thrs": 0xfffff << 0}
Fields["T_RCOIL_MEAS"] = {"t_rcoil_meas": 0xfffff << 0}
Fields["STEPS_LOST"] = {"steps_lost": 0xfffff << 0}
Fields["CURRENT_PI_REG"] = {
    "cur_p": 0xfff << 0,
    "cur_i": 0x3ff << 16,
}
Fields["ANGLE_PI_REG"] = {
    "angle_p": 0xfff << 0,
    "angle_i": 0x3ff << 16,
}
Fields["CUR_ANGLE_LIMIT"] = {
    "angle_pi_limit": 0x3ff << 0,
    "angle_pi_int_pos_clip": 0x01 << 12,
    "angle_pi_int_neg_clip": 0x01 << 13,
    "angle_pi_pos_clip": 0x01 << 14,
    "angle_pi_neg_clip": 0x01 << 15,
    "cur_pi_limit": 0xfff << 16,
    "cur_pi_int_pos_clip": 0x01 << 28,
    "cur_pi_int_neg_clip": 0x01 << 29,
    "cur_pi_pos_clip": 0x01 << 30,
    "cur_pi_neg_clip": 0x01 << 31,
}
Fields["ANGLE_LOWER_LIMIT"] = {
    "angle_lower_i_limit": 0x3ff << 0,
    "angle_error": 0x3ff << 16,
}
Fields["CUR_ANGLE_MEAS"] = {
    "ampl_meas": 0xfff << 0,
    "angle_meas": 0x3ff << 16,
}
Fields["PI_RESULTS"] = {
    "pwm_calc": 0x1fff << 0,
    "angle_corr_calc": 0x3ff << 16,
}
Fields["COIL_INDUCT"] = {
    "coil_induct": 0x7fff << 0,
    "rcoil_manual": 0x01 << 16,
    "rcoil_thermal_coupling": 0x01 << 17,
}
Fields["R_COIL"] = {
    "r_coil_auto_b": 0xfff << 0,
    "r_coil_auto_a": 0xfff << 16,
}
Fields["R_COIL_USER"] = {
    "r_coil_user_b": 0xfff << 0,
    "r_coil_user_a": 0xfff << 16,
}
Fields["SGP_CONF"] = {
    "sgp_thrs": 0x1ff << 0,
    "sgp_filt_en": 0x01 << 12,
    "sgp_low_vel_freeze": 0x01 << 13,
    "sgp_clear_cur_pi": 0x01 << 14,
    "sgp_low_vel_slope": 0xff << 16,
    "sgp_low_vel_cnts": 0x03 << 28,
}
Fields["SGP_IND_2_3"] = {
    "sgp_ind_2": 0x3ff << 0,
    "sgp_ind_3": 0x3ff << 16,
}
Fields["SGP_IND_0_1"] = {
    "sgp_ind_0": 0x3ff << 0,
    "sgp_ind_1": 0x3ff << 16,
}
Fields["INDUCTANCE_VOLTAGE"] = {
    "ul_b": 0xfff << 0,
    "ul_a": 0xfff << 16,
}
Fields["SGP_BEMF"] = {
    "sgp_raw": 0x3ff << 0,
    "ubemf_abs": 0xfff << 16,
}
Fields["COOLSTEPPLUS_CONF"] = {
    "cool_cur_div": 0x0f << 0,
    "load_filt_en": 0x01 << 4,
}
Fields["COOLSTEPPLUS_PI_REG"] = {
    "coolstep_p": 0xfff << 0,
    "coolstep_i": 0x3ff << 16,
}
Fields["COOLSTEPPLUS_PI_DOWN"] = {
    "cool_pi_down_limit": 0xfff << 0,
    "cool_pi_off_speed": 0xfff << 16,
}
Fields["COOLSTEPPLUS_RESERVE_CONF"] = {
    "cool_low_load_reserve": 0xff << 0,
    "cool_hi_load_reserve": 0xff << 8,
    "cool_low_generatoric_reserve": 0xff << 16,
    "cool_hi_generatoric_reserve": 0xff << 24,
}
Fields["COOLSTEPPLUS_LOAD_RESERVE"] = {
    "sgp_result": 0x3ff << 0,
    "coolstep_load_reserve": 0x1ff << 16,
}
Fields["TSTEP_VELOCITY"] = {"tstep_velocity": 0x7fffff << 0}
Fields["ADC_VSUPPLY_TEMP"] = {
    "adc_vsupply": 0x1ff << 0,
    "adc_temp": 0x1ff << 16,
}
Fields["ADC_I"] = {
    "adc_i_a": 0xfff << 0,
    "adc_i_b": 0xfff << 16,
}
Fields["OTW_OV_VTH"] = {
    "overvoltage_vth": 0x1ff << 0,
    "overtempprewarning_vth": 0x1ff << 16,
}
Fields["MSLUT0"] = {"mslut0": 0xffffffff}
Fields["MSLUT1"] = {"mslut1": 0xffffffff}
Fields["MSLUT2"] = {"mslut2": 0xffffffff}
Fields["MSLUT3"] = {"mslut3": 0xffffffff}
Fields["MSLUT4"] = {"mslut4": 0xffffffff}
Fields["MSLUT5"] = {"mslut5": 0xffffffff}
Fields["MSLUT6"] = {"mslut6": 0xffffffff}
Fields["MSLUT7"] = {"mslut7": 0xffffffff}
Fields["MSLUTSEL"] = {
    "x3": 0xff << 24,
    "x2": 0xff << 16,
    "x1": 0xff << 8,
    "w3": 0x03 << 6,
    "w2": 0x03 << 4,
    "w1": 0x03 << 2,
    "w0": 0x03 << 0,
}
Fields["MSLUTSTART"] = {
    "start_sin": 0xff << 0,
    "start_sin90": 0xff << 16,
    "offset_sin90": 0xff << 24,
}
Fields["MSCNT"] = {"mscnt": 0x3ff << 0}
Fields["MSCURACT"] = {"cur_b": 0x1ff << 0, "cur_a": 0x1ff << 16}
Fields["CHOPCONF"] = {
    "toff": 0x0f << 0,
    "hstrt": 0x07 << 4,
    "hend": 0x0f << 7,
    "fd3": 0x01 << 11,
    "disfdcc": 0x01 << 12,
    "chm": 0x01 << 14,
    "tbl": 0x03 << 15,
    "tpfd": 0x0f << 20,
    "mres": 0x0f << 24,
    "intpol": 0x01 << 28,
    "dedge": 0x01 << 29,
}
Fields["COOLCONF"] = {
    "semin": 0x0f << 0,
    "seup": 0x03 << 5,
    "semax": 0x0f << 8,
    "sedn": 0x07 << 12,
    "seimin": 0x01 << 15,
    "sgt": 0x7f << 16,
    "thigh_sg_off": 0x01 << 23,
    "sfilt": 0x01 << 24,
}
Fields["DRV_STATUS"] = {
    "sg_result": 0x3ff << 0,
    "seq_stopped": 0x01 << 10,
    "ov": 0x01 << 11,
    "s2vsa": 0x01 << 12,
    "s2vsb": 0x01 << 13,
    "stealth": 0x01 << 14,
    "cs_actual": 0xff << 16,
    "stallguard": 0x01 << 24,
    "ot": 0x01 << 25,
    "otpw": 0x01 << 26,
    "s2ga": 0x01 << 27,
    "s2gb": 0x01 << 28,
    "ola": 0x01 << 29,
    "olb": 0x01 << 30,
    "stst": 0x01 << 31,
}
Fields["PWMCONF"] = {
    "pwm_freq": 0x0f << 0,
    "freewheel": 0x03 << 4,
    "ol_thrsh": 0x03 << 6,
    "sd_on_meas_lo": 0x0f << 12,
    "sd_on_meas_hi": 0x0f << 16,
}

SignedFields = [
    "cur_a", "cur_b", "sgt", "offset_sin90", "adc_i_a", "adc_i_b",
    "angle_error", "angle_meas", "angle_corr_calc", "sgp_thrs",
    "sgp_ind_0", "sgp_ind_1", "sgp_ind_2", "sgp_ind_3", "ul_a", "ul_b",
    "sgp_raw", "sgp_result",
]

FieldFormatters = dict(tmc2130.FieldFormatters)
FieldFormatters.update({
    "ov": lambda v: "1(Overvoltage!)" if v else "",
    "s2vsa": lambda v: "1(ShortToSupply_A!)" if v else "",
    "s2vsb": lambda v: "1(ShortToSupply_B!)" if v else "",
    "vm_uvlo": lambda v: "1(VMUndervoltage!)" if v else "",
    "vccio_uv": lambda v: "1(VCCIOUndervoltage!)" if v else "",
    "register_reset": lambda v: "1(RegisterReset!)" if v else "",
    "ext_clk": lambda v: "1(ExternalClock)" if v else "0(Internal16MHz)",
    "ext_res_det": lambda v: "1" if v else "0(MissingRREF!)",
    "drv_enn": lambda v: "1(Disabled)" if v else "0(Enabled)",
    "silicon_rv": lambda v: "%d" % (v,),
    "clk_loss": lambda v: "1(ClockLoss!)" if v else "",
    "clk_is_stuck": lambda v: "1(ClockStuck!)" if v else "",
    "clk_1m0_tmo": lambda v: "1(ClockTimeout!)" if v else "",
    "adc_temp": lambda v: "0x%03x(%.1fC)" % (v, _adc_to_celsius(v)),
    "adc_vsupply": lambda v: "0x%03x(%.3fV)" % (v, _adc_to_volts(v)),
    "angle_meas": lambda v: "%d(%.1fdeg)" % (v, v * 360. / 1024.),
    "overvoltage_vth": lambda v: "0x%03x(%.3fV)" % (v, _adc_to_volts(v)),
    "overtempprewarning_vth":
        lambda v: "0x%03x(%.1fC)" % (v, _adc_to_celsius(v)),
})


######################################################################
# TMC5262 current helper
######################################################################

DEFAULT_RREF = 12000.
KIFS_BASE = 18.

class TMC5262CurrentHelper:
    def __init__(self, config, mcu_tmc):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        self.rref = config.getfloat("rref", DEFAULT_RREF,
                                   minval=10000., maxval=14000.)
        max_current = self._ifs_rms_for(3, 3)
        run_current = config.getfloat("run_current", above=0.,
                                      maxval=max_current)
        hold_current = config.getfloat("hold_current", run_current,
                                       above=0., maxval=max_current)
        self.req_hold_current = hold_current
        cr, crs = self._calc_ranges(run_current)
        current_range = config.getint("current_range", cr, minval=0, maxval=3)
        default_scale = crs if current_range == 0 else 3
        current_range_scale = config.getint(
            "current_range_scale", default_scale, minval=0, maxval=3)
        if current_range != 0 and current_range_scale != 3:
            raise config.error("tmc5262 %s: current_range_scale below 3 is "
                               "only valid with current_range=0" % (self.name,))
        self.fields.set_field("current_range", current_range)
        self.fields.set_field("current_range_scale", current_range_scale)
        if run_current > self._ifs_rms():
            raise config.error("tmc5262 %s: run_current %.3f exceeds %.3fA "
                               "RMS for the selected current range"
                               % (self.name, run_current, self._ifs_rms()))
        irun, ihold = self._calc_current(run_current, hold_current)
        self.fields.set_field("irun", irun)
        self.fields.set_field("ihold", ihold)

    def _ifs_rms_for(self, cr, crs):
        i_peak = KIFS_BASE * (cr + 1) * (crs + 1) / (4. * self.rref / 1000.)
        return i_peak / math.sqrt(2.)

    def _ifs_rms(self):
        return self._ifs_rms_for(self.fields.get_field("current_range"),
                                 self.fields.get_field("current_range_scale"))

    def _calc_ranges(self, current):
        for cr in range(4):
            if current <= self._ifs_rms_for(cr, 3):
                if cr == 0:
                    for crs in range(4):
                        if current <= self._ifs_rms_for(0, crs):
                            return 0, crs
                return cr, 3
        return 3, 3

    def _calc_current(self, run_current, hold_current):
        if run_current <= 0.:
            return 0, 0
        ifs = self._ifs_rms()
        irun = max(1, min(250, int(round(run_current / ifs * 250.))))
        ihold = int(round(min(hold_current, run_current) / run_current * irun))
        return irun, max(0, min(irun, ihold))

    def _calc_current_from_field(self, field_name):
        return self._ifs_rms() * self.fields.get_field(field_name) / 250.

    def get_current(self):
        return (self._calc_current_from_field("irun"),
                self._calc_current_from_field("ihold"),
                self.req_hold_current, self._ifs_rms())

    def set_current(self, run_current, hold_current, print_time):
        self.req_hold_current = hold_current
        irun, ihold = self._calc_current(run_current, hold_current)
        self.fields.set_field("ihold", ihold)
        val = self.fields.set_field("irun", irun)
        self.mcu_tmc.set_register("IHOLD_IRUN", val, print_time)


######################################################################
# TMC5262 printer object
######################################################################

class TMC5262CommandHelper(tmc.TMCCommandHelper):
    def __init__(self, config, mcu_tmc, current_helper, init_callback,
                 field_validator):
        self.init_callback = init_callback
        self.field_validator = field_validator
        tmc.TMCCommandHelper.__init__(self, config, mcu_tmc, current_helper)

    def _init_registers(self, print_time=None):
        if self.init_callback():
            # PLL COMMIT resets the driver control logic, including MSCNT.
            # Do not retain a phase offset measured before that reset.
            self.mcu_phase_offset = None
        tmc.TMCCommandHelper._init_registers(self, print_time)

    def cmd_SET_TMC_FIELD(self, gcmd):
        field_name = gcmd.get('FIELD').lower()
        value = gcmd.get_int('VALUE', None)
        velocity = gcmd.get_float('VELOCITY', None, minval=0.)
        self.field_validator(gcmd, field_name, value, velocity)
        tmc.TMCCommandHelper.cmd_SET_TMC_FIELD(self, gcmd)


class TMC5262:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.mcu_tmc = tmc2130.MCU_TMC_SPI(config, Registers, self.fields,
                                           TMC_FREQUENCY)
        self.mcu_tmc.temp_from_adc = self._temp_from_adc_register
        self.mcu_tmc.sg_result_from_drv_status = self._sg_result_from_drv_status
        # PLL initialization must run before TMCCommandHelper's connect handler.
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_pll_init)

        # The TMC5262 routes stall output through DO_CONF instead of GCONF.
        # The field aliases above let Klipper's common virtual pin helper
        # handle the hardware DO0/DO1 stall bits as diag0/diag1 signals.
        self.fields.set_field("do0_invpp", 1)
        self.fields.set_field("do1_invpp", 1)
        diag0_pin = config.get("diag0_pin", None)
        diag1_pin = config.get("diag1_pin", None)
        if diag0_pin is not None and diag1_pin is not None:
            raise config.error("tmc5262 %s: specify only one diag pin"
                               % (self.name,))
        tmc.TMCVirtualPinHelper(config, self.mcu_tmc)

        current_helper = TMC5262CurrentHelper(config, self.mcu_tmc)
        self.diag0_pin = diag0_pin
        self.diag1_pin = diag1_pin
        cmdhelper = TMC5262CommandHelper(
            config, self.mcu_tmc, current_helper, self._handle_pll_init,
            self._validate_runtime_field)
        cmdhelper.setup_register_dump(ReadRegisters)
        self.get_phase_offset = cmdhelper.get_phase_offset
        self.get_status = cmdhelper.get_status

        tmc.TMCWaveTableHelper(config, self.mcu_tmc)
        self.fields.set_config_field(config, "offset_sin90", 0)
        self.stealthchop_velocity = config.getfloat(
            "stealthchop_threshold", None, minval=0.)
        self.stealthchop_plus_configured = (
            self.stealthchop_velocity is not None
            and self.stealthchop_velocity > 0.)
        tmc.TMCStealthchopHelper(config, self.mcu_tmc)
        # Reuse Klipper's established CoolStep / StallGuard velocity helpers.
        tmc.TMCVcoolthrsHelper(config, self.mcu_tmc)
        tmc.TMCVhighHelper(config, self.mcu_tmc)

        set_config_field = self.fields.set_config_field
        self.fields.set_field("clock_divider", 15)
        self.fields.set_field("clk_sys_sel", 1)
        self.fields.set_field("clk_fsm_ena", 1)

        self.fields.set_field("step_dir", True)
        set_config_field(config, "multistep_filt", True)

        set_config_field(config, "toff", 3)
        set_config_field(config, "hstrt", 5)
        set_config_field(config, "hend", 2)
        set_config_field(config, "fd3", 0)
        set_config_field(config, "disfdcc", 0)
        set_config_field(config, "chm", 0)
        set_config_field(config, "tbl", 2)
        set_config_field(config, "tpfd", 4)

        set_config_field(config, "semin", 0)
        set_config_field(config, "seup", 0)
        set_config_field(config, "semax", 0)
        set_config_field(config, "sedn", 0)
        set_config_field(config, "seimin", 0)
        set_config_field(config, "sgt", 0)
        set_config_field(config, "sfilt", 0)

        set_config_field(config, "iholddelay", 7)
        set_config_field(config, "irundelay", 4)

        set_config_field(config, "pwm_freq", 0)
        set_config_field(config, "freewheel", 0)
        try:
            sd_lo, sd_hi = pwm_measurement_defaults(
                self.fields.get_field("pwm_freq"))
        except ValueError as e:
            raise config.error(str(e))
        set_config_field(config, "sd_on_meas_lo", sd_lo)
        set_config_field(config, "sd_on_meas_hi", sd_hi)

        set_config_field(config, "tpowerdown", 10)
        set_config_field(config, "slope_control", 3)

        # TMC5262 StealthChop+ PI regulators.
        set_config_field(config, "cur_p", 64)
        set_config_field(config, "cur_i", 10)
        set_config_field(config, "angle_p", 50)
        set_config_field(config, "angle_i", 20)
        set_config_field(config, "cur_pi_limit", 0xfff)
        set_config_field(config, "angle_pi_limit", 256)
        set_config_field(config, "angle_lower_i_limit", 256)

        # TMC5262 motor model used by StealthChop+ and StallGuard+.
        set_config_field(config, "t_rcoil_meas", 4096)
        set_config_field(config, "coil_induct", 0)
        set_config_field(config, "rcoil_manual", False)
        set_config_field(config, "rcoil_thermal_coupling",
                         self.stealthchop_plus_configured)
        set_config_field(config, "r_coil_user_a", 0)
        set_config_field(config, "r_coil_user_b", 0)
        if self.stealthchop_plus_configured:
            if not self.fields.get_field("coil_induct"):
                raise config.error("tmc5262 %s: StealthChop+ requires "
                                   "driver_COIL_INDUCT in microhenries"
                                   % (self.name,))
            if self.fields.get_field("rcoil_manual"):
                rcoil_a = self.fields.get_field("r_coil_user_a")
                rcoil_b = self.fields.get_field("r_coil_user_b")
                if not rcoil_a or not rcoil_b:
                    raise config.error("tmc5262 %s: driver_RCOIL_MANUAL "
                                       "requires nonzero R_COIL_USER_A/B"
                                       % (self.name,))

        # Always restore TMC5262-only registers to their silicon reset
        # values before applying optional user overrides. This prevents a
        # removed option from surviving a host-only FIRMWARE_RESTART.
        for register, value in OPTIONAL_REGISTER_DEFAULTS.items():
            self.fields.registers.setdefault(register, value)

        # Optional RT-OCSI, StallGuard+, and CoolStep+ controls. These are
        # TMC5262-specific extensions to Klipper's common threshold helpers.
        for field_name in (
            "do0_scope_en", "do0_scope_sel", "do1_scope_en",
            "do1_scope_sel", "tsgp_low_vel_thrs", "sgp_thrs",
            "sgp_filt_en", "sgp_low_vel_freeze", "sgp_clear_cur_pi",
            "sgp_low_vel_slope", "sgp_low_vel_cnts", "cool_cur_div",
            "load_filt_en", "coolstep_p", "coolstep_i",
            "cool_pi_down_limit", "cool_pi_off_speed",
            "cool_low_load_reserve", "cool_hi_load_reserve",
            "cool_low_generatoric_reserve", "cool_hi_generatoric_reserve",
        ):
            configure_optional_field(config, self.fields, field_name)

        # DO0/DO1 are physically shared between diagnostic output and the
        # RT-OCSI DAC. Prevent a virtual endstop from being masked by scope
        # routing on the same pin.
        if diag0_pin is not None and self.fields.get_field("do0_scope_en"):
            raise config.error("tmc5262 %s: driver_DO0_SCOPE_EN conflicts "
                               "with diag0_pin" % (self.name,))
        if diag1_pin is not None and self.fields.get_field("do1_scope_en"):
            raise config.error("tmc5262 %s: driver_DO1_SCOPE_EN conflicts "
                               "with diag1_pin" % (self.name,))

        self._order_register_cache()
        if self.stealthchop_plus_configured:
            self.stepper_enable = self.printer.load_object(config,
                                                           "stepper_enable")
            self.printer.register_event_handler(
                "homing:home_rails_begin", self._handle_home_rails_begin)

    def _validate_runtime_field(self, gcmd, field_name, value, velocity):
        limited_fields = {
            "do0_scope_en": 1,
            "do1_scope_en": 1,
            "do0_scope_sel": 0x1c,
            "do1_scope_sel": 0x1c,
            "cool_cur_div": 10,
        }
        if field_name not in limited_fields:
            return
        if velocity is not None:
            raise gcmd.error("FIELD=%s requires VALUE" % (field_name,))
        if value is None or value < 0 or value > limited_fields[field_name]:
            raise gcmd.error("Invalid value for FIELD=%s" % (field_name,))
        if (field_name == "do0_scope_en" and value
                and self.diag0_pin is not None):
            raise gcmd.error("do0_scope_en conflicts with diag0_pin")
        if (field_name == "do1_scope_en" and value
                and self.diag1_pin is not None):
            raise gcmd.error("do1_scope_en conflicts with diag1_pin")

    def _temp_from_adc_register(self, reg_value):
        adc_temp = self.fields.get_field(
            "adc_temp", reg_value, reg_name="ADC_VSUPPLY_TEMP")
        return _adc_to_celsius(adc_temp)

    def _sg_result_from_drv_status(self, reg_value):
        result = self.fields.get_field(
            "sg_result", reg_value, reg_name="DRV_STATUS")
        if self.fields.get_field(
                "stealth", reg_value, reg_name="DRV_STATUS"):
            # In StealthChop+ the same 10-bit DRV_STATUS field mirrors the
            # signed StallGuard+ result; in SpreadCycle it is unsigned SG2.
            if result & 0x200:
                result -= 0x400
        return result

    def _order_register_cache(self):
        registers = self.fields.registers
        for register in TMC5262_REGISTER_INIT_ORDER:
            if register in registers:
                value = registers.pop(register)
                registers[register] = value

    def _home_uses_this_stepper(self, rails):
        return any(stepper.get_name() == self.name
                   for rail in rails for stepper in rail.get_steppers())

    def _abort_rcoil_preflight(self, message):
        try:
            self.stepper_enable.motor_off()
        except self.printer.command_error:
            logging.exception("TMC5262 %s could not disable motors after "
                              "RCOIL failure", self.name)
        raise self.printer.command_error(
            "TMC5262 %s StealthChop+ RCOIL preflight failed: %s. "
            "No homing move was started." % (self.name, message))

    def _handle_home_rails_begin(self, homing_state, rails):
        del homing_state
        if not self._home_uses_this_stepper(rails):
            return
        if self.fields.get_field("rcoil_manual"):
            return
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
            cs_actual = self.fields.get_field("cs_actual", drv_status,
                                              reg_name="DRV_STATUS")
            rcoil = self.mcu_tmc.get_register("R_COIL")
            rcoil_a = self.fields.get_field("r_coil_auto_a", rcoil,
                                            reg_name="R_COIL")
            rcoil_b = self.fields.get_field("r_coil_auto_b", rcoil,
                                            reg_name="R_COIL")
        except self.printer.command_error as e:
            self._abort_rcoil_preflight(str(e))

        hard_faults = [
            field for field in ("s2vsa", "s2vsb", "s2ga", "s2gb", "ot")
            if self.fields.get_field(field, drv_status,
                                     reg_name="DRV_STATUS")
        ]
        if hard_faults:
            self._abort_rcoil_preflight(
                "driver reported %s" % (", ".join(hard_faults),))
        if cs_actual < 50:
            self._abort_rcoil_preflight(
                "CS_ACTUAL=%d is below the required 50" % (cs_actual,))
        if not rcoil_a or not rcoil_b:
            self._abort_rcoil_preflight(
                "R_COIL_AUTO_A/B did not both become nonzero (A=%d, B=%d)"
                % (rcoil_a, rcoil_b))
        logging.info("TMC5262 %s StealthChop+ RCOIL ready: A=%d B=%d "
                     "CS_ACTUAL=%d", self.name, rcoil_a, rcoil_b, cs_actual)

    def _build_pll_value(self, commit, clear_flags=False):
        val = ((1 if commit else 0) << 0 | (1 << 2) | (1 << 3)
               | (15 << 5))
        if clear_flags:
            val |= (1 << 13) | (1 << 14)
        return val

    def _pll_is_ready(self, pll):
        control_mask = ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)
                        | (0x1f << 5))
        fault_mask = (1 << 12) | (1 << 13) | (1 << 14)
        expected = self._build_pll_value(False)
        return ((pll & control_mask) == expected and not (pll & fault_mask))

    def _handle_pll_init(self):
        try:
            pll = self.mcu_tmc.get_register("PLL")
            if self._pll_is_ready(pll):
                return False
            fault_mask = (1 << 12) | (1 << 13) | (1 << 14)
            if pll & fault_mask:
                # Datasheet PLL recovery: disable the FSM before restarting
                # the normal 0x01ED -> 0x61EC initialization sequence.
                self.mcu_tmc.set_register(
                    "PLL", self._build_pll_value(False) & ~(1 << 3))
            self.mcu_tmc.set_register("PLL", self._build_pll_value(True))
            reactor = self.printer.get_reactor()
            deadline = reactor.monotonic() + .020
            while reactor.monotonic() < deadline:
                reactor.pause(reactor.monotonic() + .001)
                pll = self.mcu_tmc.get_register("PLL")
                if not (pll & 0x01):
                    break
            else:
                raise self.printer.command_error(
                    "TMC5262 %s PLL commit timed out" % (self.name,))
            self.mcu_tmc.set_register("PLL",
                                      self._build_pll_value(False, True))
            pll = self.mcu_tmc.get_register("PLL")
            fault_mask = (1 << 12) | (1 << 13) | (1 << 14)
            if pll & fault_mask:
                raise self.printer.command_error(
                    "TMC5262 %s PLL fault after initialization: %s"
                    % (self.name, self.fields.pretty_format("PLL", pll)))
            self.mcu_tmc.set_register("GSTAT", 0x3f)
            return True
        except self.printer.command_error as e:
            logging.error("TMC5262 %s PLL init failed: %s", self.name, str(e))
            raise


def load_config_prefix(config):
    return TMC5262(config)
