# Definitions of the supported input shapers
#
# Copyright (C) 2020-2021  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections, math

SHAPER_VIBRATION_REDUCTION = 20.0
DEFAULT_DAMPING_RATIO = 0.1

InputShaperCfg = collections.namedtuple(
    "InputShaperCfg", ("name", "init_func", "min_freq")
)


def get_none_shaper():
    return ([], [])


def get_zv_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1.0 - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1.0 / (shaper_freq * df)
    A = [1.0, K]
    T = [0.0, 0.5 * t_d]
    return (A, T)


def get_zvd_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1.0 - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1.0 / (shaper_freq * df)
    A = [1.0, 2.0 * K, K**2]
    T = [0.0, 0.5 * t_d, t_d]
    return (A, T)


def get_mzv_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1.0 - damping_ratio**2)
    K = math.exp(-0.75 * damping_ratio * math.pi / df)
    t_d = 1.0 / (shaper_freq * df)

    a1 = 1.0 - 1.0 / math.sqrt(2.0)
    a2 = (math.sqrt(2.0) - 1.0) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0.0, 0.375 * t_d, 0.75 * t_d]
    return (A, T)


def get_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1.0 / SHAPER_VIBRATION_REDUCTION  # vibration tolerance
    df = math.sqrt(1.0 - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1.0 / (shaper_freq * df)

    a1 = 0.25 * (1.0 + v_tol)
    a2 = 0.5 * (1.0 - v_tol) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0.0, 0.5 * t_d, t_d]
    return (A, T)


def get_2hump_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1.0 / SHAPER_VIBRATION_REDUCTION  # vibration tolerance
    df = math.sqrt(1.0 - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1.0 / (shaper_freq * df)

    V2 = v_tol**2
    X = pow(V2 * (math.sqrt(1.0 - V2) + 1.0), 1.0 / 3.0)
    a1 = (3.0 * X * X + 2.0 * X + 3.0 * V2) / (16.0 * X)
    a2 = (0.5 - a1) * K
    a3 = a2 * K
    a4 = a1 * K * K * K

    A = [a1, a2, a3, a4]
    T = [0.0, 0.5 * t_d, t_d, 1.5 * t_d]
    return (A, T)


def get_3hump_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1.0 / SHAPER_VIBRATION_REDUCTION  # vibration tolerance
    df = math.sqrt(1.0 - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1.0 / (shaper_freq * df)

    K2 = K * K
    a1 = 0.0625 * (
        1.0 + 3.0 * v_tol + 2.0 * math.sqrt(2.0 * (v_tol + 1.0) * v_tol)
    )
    a2 = 0.25 * (1.0 - v_tol) * K
    a3 = (0.5 * (1.0 + v_tol) - 2.0 * a1) * K2
    a4 = a2 * K2
    a5 = a1 * K2 * K2

    A = [a1, a2, a3, a4, a5]
    T = [0.0, 0.5 * t_d, t_d, 1.5 * t_d, 2.0 * t_d]
    return (A, T)


# min_freq for each shaper is chosen to have projected max_accel ~= 1500
INPUT_SHAPERS = [
    InputShaperCfg("zv", get_zv_shaper, min_freq=21.0),
    InputShaperCfg("mzv", get_mzv_shaper, min_freq=23.0),
    InputShaperCfg("zvd", get_zvd_shaper, min_freq=29.0),
    InputShaperCfg("ei", get_ei_shaper, min_freq=29.0),
    InputShaperCfg("2hump_ei", get_2hump_ei_shaper, min_freq=39.0),
    InputShaperCfg("3hump_ei", get_3hump_ei_shaper, min_freq=48.0),
]
