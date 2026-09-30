# TMC5262 configuration
#
# Copyright (C) 2026  Kalico Contributors
# Copyright (C) 2026  Uriah Kessman <fishyfabspnw@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
from . import tmc, tmc2130

TMC_FREQUENCY = 16000000.

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
        self.fields.set_field("current_range", cr)
        self.fields.set_field("current_range_scale", crs)
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
    def __init__(self, config, mcu_tmc, current_helper, init_callback):
        self.init_callback = init_callback
        tmc.TMCCommandHelper.__init__(self, config, mcu_tmc, current_helper,
                                     _adc_to_celsius)

    def _init_registers(self, print_time=None):
        if self.init_callback():
            # PLL COMMIT resets the driver control logic, including MSCNT.
            # Do not retain a phase offset measured before that reset.
            self.mcu_phase_offset = None
        tmc.TMCCommandHelper._init_registers(self, print_time)


class TMC5262:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.mcu_tmc = tmc2130.MCU_TMC_SPI(config, Registers, self.fields,
                                           TMC_FREQUENCY)
        current_helper = TMC5262CurrentHelper(config, self.mcu_tmc)
        cmdhelper = TMC5262CommandHelper(
            config, self.mcu_tmc, current_helper, self._handle_pll_init)
        cmdhelper.setup_register_dump(ReadRegisters)
        self.get_phase_offset = cmdhelper.get_phase_offset
        self.get_status = cmdhelper.get_status

        tmc.TMCWaveTableHelper(config, self.mcu_tmc)
        self.fields.set_config_field(config, "offset_sin90", 0)
        self.fields.set_field("en_pwm_mode", 0)
        self.fields.set_field("tpwmthrs", 0)
        self.fields.set_field("tcoolthrs", 0)
        self.fields.set_field("thigh", 0)
        self.fields.registers["DO_CONF"] = 0
        self.fields.registers["DO_SCOPE_CONF"] = 0

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

        self.fields.registers["COOLCONF"] = 0

        set_config_field(config, "iholddelay", 7)
        set_config_field(config, "irundelay", 4)

        set_config_field(config, "tpowerdown", 10)
        set_config_field(config, "slope_control", 3)

        # Write the chopper configuration last.
        for register in ("CHOPCONF",):
            value = self.fields.registers.pop(register)
            self.fields.registers[register] = value

    def _handle_pll_init(self):
        # Check on every initialization, including re-enable after power loss.
        fields = tmc.FieldHelper({"PLL": Fields["PLL"]})
        desired = {"commit": 0, "ext_not_int": 0, "clk_sys_sel": 1,
                   "clk_fsm_ena": 1, "clock_divider": 15,
                   "clk_1m0_tmo": 0, "clk_loss": 0, "clk_is_stuck": 0}
        pll = self.mcu_tmc.get_register("PLL")
        if all(fields.get_field(f, pll) == v for f, v in desired.items()):
            return False
        for field, value in desired.items():
            fields.set_field(field, value)
        # Disable the FSM before retrying configuration (datasheet p.106).
        self.mcu_tmc.set_register("PLL", fields.set_field("clk_fsm_ena", 0))
        fields.set_field("clk_fsm_ena", 1)
        self.mcu_tmc.set_register("PLL", fields.set_field("commit", 1))
        reactor = self.printer.get_reactor()
        deadline = reactor.monotonic() + .500
        while True:
            pll = self.mcu_tmc.get_register("PLL")
            if not fields.get_field("commit", pll):
                break
            if reactor.monotonic() >= deadline:
                raise self.printer.command_error(
                    "TMC5262 %s PLL commit timed out" % (self.name,))
            reactor.pause(reactor.monotonic() + .001)
        fields.set_field("commit", 0)
        fields.set_field("clk_loss", 1)
        val = fields.set_field("clk_is_stuck", 1)
        self.mcu_tmc.set_register("PLL", val)
        pll = self.mcu_tmc.get_register("PLL")
        if any(fields.get_field(f, pll)
               for f in ("clk_1m0_tmo", "clk_loss", "clk_is_stuck")):
            raise self.printer.command_error(
                "TMC5262 %s PLL fault after initialization: %s"
                % (self.name, fields.pretty_format("PLL", pll)))
        return True


def load_config_prefix(config):
    return TMC5262(config)
