# Support for 1-wire based temperature sensors
#
# Copyright (C) 2020 Alan Lord <alanslists@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from logging import Filter
from os import remove
from time import time
import mcu
import math


class RCTFilter:
    def __init__(self):
        pass

    def ftr_val(self, vals):
        out_vals = []
        if len(vals) < 3:
            return vals
        for i in range(len(vals) - 2):   
            tmp = [math.fabs(vals[i]), math.fabs(vals[i + 1]), math.fabs(vals[i + 2])]
            index = tmp.index(min(tmp))
            out_vals.append(vals[index + i])
        out_vals.append(vals[-2])
        out_vals.append(vals[-1])
        return out_vals


class RCHFilter:
    def __init__(self, cut_frq_hz, acq_frq_hz):
        self.cut_frq_hz = cut_frq_hz
        self.acq_frq_hz = acq_frq_hz
        pass

    def ftr_val(self, vals):
        out_vals = [0]
        rc = 1. / 2. / math.pi / self.cut_frq_hz
        coff = rc / (rc + 1. / self.acq_frq_hz)
        for i in range(1, len(vals)):
            out_vals.append((vals[i] - vals[i - 1] + out_vals[-1]) * coff)
        return out_vals


class RCLFilter:
    def __init__(self, k1_new):
        self.k1_new = k1_new
        pass

    def ftr_val(self, vals):
        out_vals = [vals[0]]
        for i in range(1, len(vals)):
            out_vals.append(out_vals[-1] * (1 - self.k1_new) + vals[i] * self.k1_new)
        return out_vals


class Filter:
    def __init__(self, config):
        self.hft_hz = config.getfloat('hft_hz', default=5, minval=0.1, maxval=10.)
        self.lft_k1 = config.getfloat('lft_k1', default=0.8, minval=0., maxval=1.)
        self.lft_k1_oft = config.getfloat('lft_k1_oft', default=0.8, minval=0., maxval=1.)
        self.lft_k1_cal = config.getfloat('lft_k1_cal', default=0.8, minval=0., maxval=1.)
        pass
    
    def get_tft(self):
        return RCTFilter()

    def get_lft(self, k1):
        return RCLFilter(k1)

    def get_hft(self, cut_hz, acq_hz):
        return RCHFilter(cut_frq_hz=cut_hz, acq_frq_hz=acq_hz)

    def cal_offset_by_vals(self, s_count, new_valss, lft_k1, cut_len):
        out_vals = []
        tmp_vals = [[], [], [], []]
        tft = RCTFilter()
        lft = RCLFilter(lft_k1)
        for i in range(s_count):
            tmp_vals[i] = tft.ftr_val(new_valss[i])
            tmp_vals[i] = lft.ftr_val(tmp_vals[i])
        for i in range(len(tmp_vals[0])):
            sums = 0
            for j in range(s_count):
                if i < len(tmp_vals[j]):
                    sums += math.fabs(tmp_vals[j][i])
            out_vals.append(sums)
        if len(out_vals) > cut_len:
            del out_vals[0:(len(out_vals) - cut_len)]
        for i in range(s_count):
            if len(tmp_vals[i]) > cut_len:
                del tmp_vals[i][0:(len(tmp_vals[i]) - cut_len)]
            for j in range(len(tmp_vals[i])):
                tmp_vals[i][j] = abs(tmp_vals[i][j])
        return out_vals, tmp_vals


    def cal_filter_by_vals(self, s_count, now_valss, hft_hz, lft_k1, cut_len):
        out_vals = []
        tmp_vals = [[], [], [], []]
        tft = RCTFilter()
        hft = RCHFilter(hft_hz, 80)
        lft = RCLFilter(lft_k1)
        for i in range(0, s_count):
            tmp_vals[i] = tft.ftr_val(now_valss[i])
            tmp_vals[i] = hft.ftr_val(tmp_vals[i])
            tmp_vals[i] = lft.ftr_val(tmp_vals[i])
        for i in range(len(tmp_vals[0])):
            sums = 0
            for j in range(s_count):
                if i < len(tmp_vals[j]):
                    sums += math.fabs(tmp_vals[j][i])
            out_vals.append(sums)
        if len(out_vals) > cut_len:
            del out_vals[0:(len(out_vals) - cut_len)]
        for i in range(s_count):
            if len(tmp_vals[i]) > cut_len:
                del tmp_vals[i][0:(len(tmp_vals[i]) - cut_len)]
            for j in range(len(tmp_vals[i])):
                tmp_vals[i][j] = abs(tmp_vals[i][j])
        return out_vals, tmp_vals


def load_config(config):
    return Filter(config)