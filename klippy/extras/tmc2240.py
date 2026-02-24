# TMC2240 configuration
#
# Copyright (C) 2018-2023  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2023  Alex Voinea <voinea.dragos.alexandru@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import bus, tmc, tmc2130, tmc_uart

TMC_FREQUENCY=12500000.

Registers = {
    "GCONF":            0x00,
    "GSTAT":            0x01,
    "IFCNT":            0x02,
    "NODECONF":         0x03,
    "IOIN":             0x04,
    "DRV_CONF":         0x0A,
    "GLOBALSCALER":     0x0B,
    "IHOLD_IRUN":       0x10,
    "TPOWERDOWN":       0x11,
    "TSTEP":            0x12,
    "TPWMTHRS":         0x13,
    "TCOOLTHRS":        0x14,
    "THIGH":            0x15,
    "DIRECT_MODE":      0x2D,
    "ENCMODE":          0x38,
    "X_ENC":            0x39,
    "ENC_CONST":        0x3A,
    "ENC_STATUS":       0x3B,
    "ENC_LATCH":        0x3C,
    "ADC_VSUPPLY_AIN":  0x50,
    "ADC_TEMP":         0x51,
    "OTW_OV_VTH":       0x52,
    "MSLUT0":           0x60,
    "MSLUT1":           0x61,
    "MSLUT2":           0x62,
    "MSLUT3":           0x63,
    "MSLUT4":           0x64,
    "MSLUT5":           0x65,
    "MSLUT6":           0x66,
    "MSLUT7":           0x67,
    "MSLUTSEL":         0x68,
    "MSLUTSTART":       0x69,
    "MSCNT":            0x6A,
    "MSCURACT":         0x6B,
    "CHOPCONF":         0x6C,
    "COOLCONF":         0x6D,
    "DRV_STATUS":       0x6F,
    "PWMCONF":          0x70,
    "PWM_SCALE":        0x71,
    "PWM_AUTO":         0x72,
    "SG4_THRS":         0x74,
    "SG4_RESULT":       0x75,
    "SG4_IND":          0x76,
}

ReadRegisters = [
    "GCONF", "GSTAT", "IOIN", "DRV_CONF", "GLOBALSCALER", "IHOLD_IRUN",
    "TPOWERDOWN", "TSTEP", "TPWMTHRS", "TCOOLTHRS", "THIGH", "ADC_VSUPPLY_AIN",
    "ADC_TEMP", "OTW_OV_VTH", "MSCNT", "MSCURACT", "CHOPCONF", "COOLCONF",
    "DRV_STATUS", "PWMCONF", "PWM_SCALE", "PWM_AUTO", "SG4_THRS", "SG4_RESULT",
    "SG4_IND"
]

Fields = {}
Fields["COOLCONF"] = {
    "semin":                    0x0F << 0,
    "seup":                     0x03 << 5,
    "semax":                    0x0F << 8,
    "sedn":                     0x03 << 13,
    "seimin":                   0x01 << 15,
    "sgt":                      0x7F << 16,
    "sfilt":                    0x01 << 24
}
Fields["CHOPCONF"] = {
    "toff":                     0x0F << 0,
    "hstrt":                    0x07 << 4,
    "hend":                     0x0F << 7,
    "fd3":                      0x01 << 11,
    "disfdcc":                  0x01 << 12,
    "chm":                      0x01 << 14,
    "tbl":                      0x03 << 15,
    "vhighfs":                  0x01 << 18,
    "vhighchm":                 0x01 << 19,
    "tpfd":                     0x0F << 20, # midrange resonances
    "mres":                     0x0F << 24,
    "intpol":                   0x01 << 28,
    "dedge":                    0x01 << 29,
    "diss2g":                   0x01 << 30,
    "diss2vs":                  0x01 << 31
}
Fields["DRV_STATUS"] = {
    "sg_result":                0x3FF << 0,
    "s2vsa":                    0x01 << 12,
    "s2vsb":                    0x01 << 13,
    "stealth":                  0x01 << 14,
    "fsactive":                 0x01 << 15,
    "cs_actual":                0x1F << 16,
    "stallguard":               0x01 << 24,
    "ot":                       0x01 << 25,
    "otpw":                     0x01 << 26,
    "s2ga":                     0x01 << 27,
    "s2gb":                     0x01 << 28,
    "ola":                      0x01 << 29,
    "olb":                      0x01 << 30,
    "stst":                     0x01 << 31
}
Fields["GCONF"] = {
    "faststandstill":           0x01 << 1,
    "en_pwm_mode":              0x01 << 2,
    "multistep_filt":           0x01 << 3,
    "shaft":                    0x01 << 4,
    "diag0_error":              0x01 << 5,
    "diag0_otpw":               0x01 << 6,
    "diag0_stall":              0x01 << 7,
    "diag1_stall":              0x01 << 8,
    "diag1_index":              0x01 << 9,
    "diag1_onstate":            0x01 << 10,
    "diag0_pushpull":           0x01 << 12,
    "diag1_pushpull":           0x01 << 13,
    "small_hysteresis":         0x01 << 14,
    "stop_enable":              0x01 << 15,
    "direct_mode":              0x01 << 16
}
Fields["GSTAT"] = {
    "reset":                    0x01 << 0,
    "drv_err":                  0x01 << 1,
    "uv_cp":                    0x01 << 2,
    "register_reset":           0x01 << 3,
    "vm_uvlo":                  0x01 << 4
}
Fields["GLOBALSCALER"] = {
    "globalscaler":             0xFF << 0
}
Fields["IHOLD_IRUN"] = {
    "ihold":                    0x1F << 0,
    "irun":                     0x1F << 8,
    "iholddelay":               0x0F << 16,
    "irundelay":                0x0F << 24
}
Fields["IOIN"] = {
    "step":                     0x01 << 0,
    "dir":                      0x01 << 1,
    "encb":                     0x01 << 2,
    "enca":                     0x01 << 3,
    "drv_enn":                  0x01 << 4,
    "encn":                     0x01 << 5,
    "uart_en":                  0x01 << 6,
    "comp_a":                   0x01 << 8,
    "comp_b":                   0x01 << 9,
    "comp_a1_a2":               0x01 << 10,
    "comp_b1_b2":               0x01 << 11,
    "output":                   0x01 << 12,
    "ext_res_det":              0x01 << 13,
    "ext_clk":                  0x01 << 14,
    "adc_err":                  0x01 << 15,
    "silicon_rv":               0x07 << 16,
    "version":                  0xFF << 24
}
Fields["MSLUT0"] = { "mslut0": 0xffffffff }
Fields["MSLUT1"] = { "mslut1": 0xffffffff }
Fields["MSLUT2"] = { "mslut2": 0xffffffff }
Fields["MSLUT3"] = { "mslut3": 0xffffffff }
Fields["MSLUT4"] = { "mslut4": 0xffffffff }
Fields["MSLUT5"] = { "mslut5": 0xffffffff }
Fields["MSLUT6"] = { "mslut6": 0xffffffff }
Fields["MSLUT7"] = { "mslut7": 0xffffffff }
Fields["MSLUTSEL"] = {
    "x3":                       0xFF << 24,
    "x2":                       0xFF << 16,
    "x1":                       0xFF << 8,
    "w3":                       0x03 << 6,
    "w2":                       0x03 << 4,
    "w1":                       0x03 << 2,
    "w0":                       0x03 << 0,
}
Fields["MSLUTSTART"] = {
    "start_sin":                0xFF << 0,
    "start_sin90":              0xFF << 16,
    "offset_sin90":             0xFF << 24,
}
Fields["MSCNT"] = {
    "mscnt":                    0x3ff << 0
}
Fields["MSCURACT"] = {
    "cur_a":                    0x1ff << 0,
    "cur_b":                    0x1ff << 16
}
Fields["PWM_AUTO"] = {
    "pwm_ofs_auto":             0xff << 0,
    "pwm_grad_auto":            0xff << 16
}
Fields["PWMCONF"] = {
    "pwm_ofs":                  0xFF << 0,
    "pwm_grad":                 0xFF << 8,
    "pwm_freq":                 0x03 << 16,
    "pwm_autoscale":            0x01 << 18,
    "pwm_autograd":             0x01 << 19,
    "freewheel":                0x03 << 20,
    "pwm_meas_sd_enable":       0x01 << 22,
    "pwm_dis_reg_stst":         0x01 << 23,
    "pwm_reg":                  0x0F << 24,
    "pwm_lim":                  0x0F << 28
}
Fields["PWM_SCALE"] = {
    "pwm_scale_sum":            0x3ff << 0,
    "pwm_scale_auto":           0x1ff << 16
}
Fields["TPOWERDOWN"] = {
    "tpowerdown":               0xff << 0
}
Fields["TPWMTHRS"] = {
    "tpwmthrs":                 0xfffff << 0
}
Fields["TCOOLTHRS"] = {
    "tcoolthrs":                0xfffff << 0
}
Fields["TSTEP"] = {
    "tstep":                    0xfffff << 0
}
Fields["THIGH"] = {
    "thigh":                    0xfffff << 0
}
Fields["DRV_CONF"] = {
    "current_range":            0x03 << 0,
    "slope_control":            0x03 << 4
}
Fields["ADC_VSUPPLY_AIN"] = {
    "adc_vsupply":              0x1fff << 0,
    "adc_ain":                  0x1fff << 16
}
Fields["ADC_TEMP"] = {
    "adc_temp":                 0x1fff << 0
}
Fields["OTW_OV_VTH"] = {
    "overvoltage_vth":          0x1fff << 0,
    "overtempprewarning_vth":   0x1fff << 16
}
Fields["SG4_THRS"] = {
    "sg4_thrs":                 0xFF << 0,
    "sg4_filt_en":              0x01 << 8,
    "sg4_angle_offset":         0x01 << 9
}
Fields["SG4_RESULT"] = {
    "sg4_result":               0x3FF << 0
}
Fields["SG4_IND"] = {
    "sg4_ind_0":                0xFF << 0,
    "sg4_ind_1":                0xFF << 8,
    "sg4_ind_2":                0xFF << 16,
    "sg4_ind_3":                0xFF << 24
}


SignedFields = ["cur_a", "cur_b", "sgt", "pwm_scale_auto", "offset_sin90"]

FieldFormatters = dict(tmc2130.FieldFormatters)
FieldFormatters.update({
    "s2vsa":            (lambda v: "1(ShortToSupply_A!)" if v else ""),
    "s2vsb":            (lambda v: "1(ShortToSupply_B!)" if v else ""),
    "adc_temp":         (lambda v: "0x%04x(%.1fC)" % (v, ((v - 2038) / 7.7))),
    "adc_vsupply":      (lambda v: "0x%04x(%.3fV)" % (v, v * 0.009732)),
    "adc_ain":          (lambda v: "0x%04x(%.3fmV)" % (v, v * 0.3052)),
    "overvoltage_vth":  (lambda v: "0x%04x(%.3fV)" % (v, v * 0.009732)),
    "overtempprewarning_vth": (lambda v:
                               "0x%04x(%.1fC)" % (v, ((v - 2038) / 7.7))),
})


######################################################################
# TMC stepper current config helper
######################################################################

class TMC2240CurrentHelper:
    def __init__(self, config, mcu_tmc):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        self.Rref = config.getfloat('rref', 12000.,
                                    minval=12000., maxval=60000.)
        max_cur = self._get_ifs_rms(3)
        run_current = config.getfloat('run_current', above=0., maxval=max_cur)
        hold_current = config.getfloat('hold_current', max_cur,
                                       above=0., maxval=max_cur)
        self.req_hold_current = hold_current
        current_range = self._calc_current_range(run_current)
        self.fields.set_field("current_range", current_range)
        gscaler, irun, ihold = self._calc_current(run_current, hold_current)
        self.fields.set_field("globalscaler", gscaler)
        self.fields.set_field("ihold", ihold)
        self.fields.set_field("irun", irun)
    def _get_ifs_rms(self, current_range=None):
        if current_range is None:
            current_range = self.fields.get_field("current_range")
        KIFS = [11750., 24000., 36000., 36000.]
        return (KIFS[current_range] / self.Rref) / math.sqrt(2.)
    def _calc_current_range(self, current):
        for current_range in range(4):
            if current <= self._get_ifs_rms(current_range):
                break
        return current_range
    def _calc_globalscaler(self, current):
        ifs_rms = self._get_ifs_rms()
        globalscaler = int(((current * 256.) / ifs_rms) + .5)
        globalscaler = max(32, globalscaler)
        if globalscaler >= 256:
            globalscaler = 0
        return globalscaler
    def _calc_current_bits(self, current, globalscaler):
        ifs_rms = self._get_ifs_rms()
        if not globalscaler:
            globalscaler = 256
        cs = int((current * 256. * 32.) / (globalscaler * ifs_rms) - 1. + .5)
        return max(0, min(31, cs))
    def _calc_current(self, run_current, hold_current):
        gscaler = self._calc_globalscaler(run_current)
        irun = self._calc_current_bits(run_current, gscaler)
        ihold = self._calc_current_bits(min(hold_current, run_current), gscaler)
        return gscaler, irun, ihold
    def _calc_current_from_field(self, field_name):
        ifs_rms = self._get_ifs_rms()
        globalscaler = self.fields.get_field("globalscaler")
        if not globalscaler:
            globalscaler = 256
        bits = self.fields.get_field(field_name)
        return globalscaler * (bits + 1) * ifs_rms / (256. * 32.)
    def get_current(self):
        ifs_rms = self._get_ifs_rms()
        run_current = self._calc_current_from_field("irun")
        hold_current = self._calc_current_from_field("ihold")
        return (run_current, hold_current, self.req_hold_current, ifs_rms)
    def set_current(self, run_current, hold_current, print_time):
        self.req_hold_current = hold_current
        gscaler, irun, ihold = self._calc_current(run_current, hold_current)
        val = self.fields.set_field("globalscaler", gscaler)
        self.mcu_tmc.set_register("GLOBALSCALER", val, print_time)
        self.fields.set_field("ihold", ihold)
        val = self.fields.set_field("irun", irun)
        self.mcu_tmc.set_register("IHOLD_IRUN", val, print_time)

######################################################################
# TMC2240 Calibrations
######################################################################

class TMC2240PhaseOffset:
    _table_256 = {
        "mslut0": 1431655765,
        "mslut1": 1251289770,
        "mslut2": 2299677001,
        "mslut3": 67375240,
        "mslut4": 4261412864,
        "mslut5": 1533918174,
        "mslut6": 614804141,
        "mslut7": 2105617,
        "start_sin": 0,
        "start_sin90": 246,
        "x1": 153,
        "x2": 255,
        "x3": 255,
        "w0": 2,
        "w1": 1,
        "w2": 1,
        "w3": 1
    }
    def __init__(self, config, mcu_tmc):
        self.printer = config.get_printer()
        self.config_name = config.get_name()
        self.stepper_name = ' '.join(self.config_name.split()[1:])
        self.mcu_tmc = mcu_tmc
        self._fields = self.mcu_tmc.get_fields()
        self._prev_state = {}
        self._drv_fields = ("tpwmthrs", "sg4_filt_en", "sg4_filt_en", "intpol")
        self._offset_sin90 = 0
        self._threshold = 0xfffff
        self._timer = None
        self._gcmd = None
        self._mslut_changed = False
        self._up = 0
        self._down = 0
        self._equal = 0
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("TMC_PHASE_OFFSET_CALIBRATE", "STEPPER",
                                   self.stepper_name,
                                   self.cmd_TMC_CALIBRATE,
                                   desc=self.cmd_TMC_CALIBRATE_help)
    def set_field(self, field_name, value):
        reg_name = self._fields.lookup_register(field_name)
        reg_val = self._fields.set_field(field_name, value)
        self.mcu_tmc.set_register(reg_name, reg_val)

    def get_field(self, field_name):
        return self._fields.get_field(field_name)
    def _get_sg4_ind(self):
        reg = self.mcu_tmc.get_register("SG4_IND")
        ind_a = [
            reg & 0xFF,        # A Falling
            (reg >> 8) & 0xFF, # A Rising
        ]
        ind_b = [
            (reg >> 16) & 0xFF, # B Falling
            (reg >> 24) & 0xFF  # B Rising
        ]
        return sum(ind_a), sum(ind_b)
    # Migrate to compressed table
    def save_sin_246(self):
        cfgname = self.config_name
        configfile = self.printer.lookup_object('configfile')
        for k in self._table_256:
            v = self._table_256[k]
            configfile.set(cfgname, 'driver_%s' % k.upper(), v)
    def _update_offset_90(self):
        degree = round(90 + (90/127 * self._offset_sin90))
        self._gcmd.respond_info(
            "Test offset: %i ~ %i degree" % (self._offset_sin90, degree))
        self.set_field("offset_sin90", self._offset_sin90)
    def _calibration_event(self, eventtime):
        # Datasheet explains calibration only up to +-17
        offset_max = 17
        offset_min = -17

        tstep = max(1, self.mcu_tmc.get_register("TSTEP"))
        # Speed too low
        if tstep > self._threshold:
            return eventtime + 0.01

        ind_a, ind_b = self._get_sg4_ind()
        # Adapt the phase offset to match the StallGuard4 results
        # phase A (SG4_IND_0+SG4_IND_1) and B (SG4_IND_2+SG4_IND_3)
        # If phase A value is > phase B value, increment OFFSET_SIN90,
        # otherwise decrement.
        # Limited to fit default SIN
        if (ind_a - ind_b) > 1:
            self._up += 1
        elif (ind_b - ind_a) > 1:
            self._down += 1
        else:
            self._equal += 1
        # Collect more samples
        if self._up + self._down + self._equal < 50:
            return eventtime + 0.02
        if self._equal > (self._up + self._down):
            self._gcmd.respond_info("Done. No more moves required.")
            self._gcmd.respond_info("Please, finish the calibration.")
            return self.printer.get_reactor().NEVER
        if self._up > self._down:
            if self._offset_sin90 < offset_max:
                self._offset_sin90 += 1
                self._update_offset_90()
        elif self._up < self._down:
            if self._offset_sin90 > offset_min:
                self._offset_sin90 -= 1
                self._update_offset_90()
        self._up = 0
        self._down = 0
        self._equal = 0
        if self._offset_sin90 > 8 or self._offset_sin90 < -8:
            if not self._mslut_changed:
                self._mslut_changed = True
                self._gcmd.respond_info(
                    "Finish the calibration and powercycle the machine")
                return self.printer.get_reactor().NEVER
        return eventtime + 0.02

    def _start(self, gcmd):
        self._gcmd = gcmd
        self._offset_sin90 = self.get_field("offset_sin90")
        for field in self._drv_fields:
            self._prev_state[field] = self.get_field(field)
        toolhead = self.printer.lookup_object('toolhead')
        fmove = self.printer.lookup_object('force_move')
        mcu_stepper = fmove.lookup_stepper(self.stepper_name)
        reactor = self.printer.get_reactor()
        toolhead.wait_moves()
        # Force enable StealthChop
        self.set_field("en_pwm_mode", 1)
        self.set_field("tpwmthrs", 0)
        self.set_field("sg4_filt_en", 1)
        self.set_field("intpol", 1)
        self._mslut_changed = self.get_field("start_sin90") <= 246
        # Minimal speed is 1 RPS
        rt_dist, steps_per_rotation = mcu_stepper.get_rotation_distance()
        self._threshold = tmc.TMCtstepHelper(self.mcu_tmc, rt_dist,
                                             mcu_stepper)
        # Run calibration
        self._timer = reactor.register_timer(self._calibration_event,
                                             reactor.NOW)
    def _finish(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.wait_moves()
        reactor = self.printer.get_reactor()
        reactor.unregister_timer(self._timer)
        self._timer = None
        degree = round(90 + (90/127 * self._offset_sin90))
        gcmd.respond_info("New offset: %i ~ %i degree" % (self._offset_sin90,
                                                          degree))
        for field in self._drv_fields:
            val = self._prev_state[field]
            self.set_field(field, val)
        self._prev_state = {}
        if self._mslut_changed and self.get_field("start_sin90") > 246:
            self.save_sin_246()
            msg = "Use new sin table, require power cycle to apply"
            gcmd.respond_info(msg)
        configfile = self.printer.lookup_object('configfile')
        configfile.set(self.config_name, 'driver_OFFSET_SIN90',
                       self._offset_sin90)
        self._gcmd = None
    cmd_TMC_CALIBRATE_help = "Run TMC2240 phase offset calibration"
    def cmd_TMC_CALIBRATE(self, gcmd):
        msg = """
        Driver will be switched to the StealthChop mode during the calibration
        Use long moves at axis medium speeds > 1 RPS
        """
        if not self._prev_state:
            gcmd.respond_info(msg)
            self._start(gcmd)
        else:
            self._finish(gcmd)

######################################################################
# TMC2240 printer object
######################################################################

class TMC2240:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        if config.get("uart_pin", None) is not None:
            # use UART for communication
            self.mcu_tmc = tmc_uart.MCU_TMC_uart(config, Registers, self.fields,
                                                 7, TMC_FREQUENCY)
        else:
            # Use SPI bus for communication
            self.mcu_tmc = tmc2130.MCU_TMC_SPI(config, Registers, self.fields,
                                               TMC_FREQUENCY)
        # Allow virtual pins to be created
        tmc.TMCVirtualPinHelper(config, self.mcu_tmc)
        # Register commands
        current_helper = TMC2240CurrentHelper(config, self.mcu_tmc)
        cmdhelper = tmc.TMCCommandHelper(config, self.mcu_tmc, current_helper)
        cmdhelper.setup_register_dump(ReadRegisters)
        self.calibraion = TMC2240PhaseOffset(config, self.mcu_tmc)
        self.get_phase_offset = cmdhelper.get_phase_offset
        self.get_status = cmdhelper.get_status
        # Setup basic register values
        tmc.TMCWaveTableHelper(config, self.mcu_tmc)
        self.fields.set_config_field(config, "offset_sin90", 0)
        tmc.TMCStealthchopHelper(config, self.mcu_tmc)
        tmc.TMCVcoolthrsHelper(config, self.mcu_tmc)
        tmc.TMCVhighHelper(config, self.mcu_tmc)
        # Allow other registers to be set from the config
        set_config_field = self.fields.set_config_field
        #   GCONF
        set_config_field(config, "multistep_filt", True)
        #   CHOPCONF
        set_config_field(config, "toff", 3)
        set_config_field(config, "hstrt", 5)
        set_config_field(config, "hend", 2)
        set_config_field(config, "fd3", 0)
        set_config_field(config, "disfdcc", 0)
        set_config_field(config, "chm", 0)
        set_config_field(config, "tbl", 2)
        set_config_field(config, "vhighfs", 0)
        set_config_field(config, "vhighchm", 0)
        set_config_field(config, "tpfd", 4)
        set_config_field(config, "diss2g", 0)
        set_config_field(config, "diss2vs", 0)
        #   COOLCONF
        set_config_field(config, "semin", 0)
        set_config_field(config, "seup", 0)
        set_config_field(config, "semax", 0)
        set_config_field(config, "sedn", 0)
        set_config_field(config, "seimin", 0)
        set_config_field(config, "sgt", 0)
        set_config_field(config, "sfilt", 0)
        #   IHOLDIRUN
        set_config_field(config, "iholddelay", 6)
        set_config_field(config, "irundelay", 4)
        #   PWMCONF
        set_config_field(config, "pwm_ofs", 29)
        set_config_field(config, "pwm_grad", 0)
        set_config_field(config, "pwm_freq", 0)
        set_config_field(config, "pwm_autoscale", True)
        set_config_field(config, "pwm_autograd", True)
        set_config_field(config, "freewheel", 0)
        set_config_field(config, "pwm_reg", 4)
        set_config_field(config, "pwm_lim", 12)
        #   TPOWERDOWN
        set_config_field(config, "tpowerdown", 10)
        #   SG4_THRS
        set_config_field(config, "sg4_angle_offset", 1)
        #   DRV_CONF
        set_config_field(config, "slope_control", 0)

def load_config_prefix(config):
    return TMC2240(config)
