# Definitions of the supported input shapers
#
# Copyright (C) 2020-2023  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections, math

SHAPER_VIBRATION_REDUCTION=20.
DEFAULT_DAMPING_RATIO = 0.1

InputShaperCfg = collections.namedtuple(
        'InputShaperCfg', ('name', 'init_func', 'min_freq'))
InputSmootherCfg = collections.namedtuple(
        'InputSmootherCfg', ('name', 'init_func', 'min_freq'))

def get_none_shaper():
    return ([], [])

def get_zv_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)
    A = [1., K]
    T = [0., .5*t_d]
    return (A, T)

def get_zvd_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)
    A = [1., 2.*K, K**2]
    T = [0., .5*t_d, t_d]
    return (A, T)

def get_mzv_shaper(shaper_freq, damping_ratio):
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-.75 * damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    a1 = 1. - 1. / math.sqrt(2.)
    a2 = (math.sqrt(2.) - 1.) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0., .375*t_d, .75*t_d]
    return (A, T)

def get_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1. / SHAPER_VIBRATION_REDUCTION # vibration tolerance
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    a1 = .25 * (1. + v_tol)
    a2 = .5 * (1. - v_tol) * K
    a3 = a1 * K * K

    A = [a1, a2, a3]
    T = [0., .5*t_d, t_d]
    return (A, T)

def get_2hump_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1. / SHAPER_VIBRATION_REDUCTION # vibration tolerance
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    V2 = v_tol**2
    X = pow(V2 * (math.sqrt(1. - V2) + 1.), 1./3.)
    a1 = (3.*X*X + 2.*X + 3.*V2) / (16.*X)
    a2 = (.5 - a1) * K
    a3 = a2 * K
    a4 = a1 * K * K * K

    A = [a1, a2, a3, a4]
    T = [0., .5*t_d, t_d, 1.5*t_d]
    return (A, T)

def get_3hump_ei_shaper(shaper_freq, damping_ratio):
    v_tol = 1. / SHAPER_VIBRATION_REDUCTION # vibration tolerance
    df = math.sqrt(1. - damping_ratio**2)
    K = math.exp(-damping_ratio * math.pi / df)
    t_d = 1. / (shaper_freq * df)

    K2 = K*K
    a1 = 0.0625 * (1. + 3. * v_tol + 2. * math.sqrt(2. * (v_tol + 1.) * v_tol))
    a2 = 0.25 * (1. - v_tol) * K
    a3 = (0.5 * (1. + v_tol) - 2. * a1) * K2
    a4 = a2 * K2
    a5 = a1 * K2 * K2

    A = [a1, a2, a3, a4, a5]
    T = [0., .5*t_d, t_d, 1.5*t_d, 2.*t_d]
    return (A, T)

def init_smoother(coeffs, smooth_time, normalize_coeffs):
    if not normalize_coeffs:
        return (list(reversed(coeffs)), smooth_time)
    inv_t_sm = inv_t_sm_n = 1. / smooth_time
    n = len(coeffs)
    c = [0] * n
    for i in range(n-1,-1,-1):
        c[n-i-1] = coeffs[i] * inv_t_sm_n
        inv_t_sm_n *= inv_t_sm
    return (c, smooth_time)

def get_none_smoother():
    return ([1.], 0.)

# Input smoother is a polynomial function w(t) which is convolved with the input
# trajectory x(t) as x_sm(T) = integral(x(t) * w(T-t) * dt, t=[T-T_sm...T+T_sm])
# to obtain a modified trajectory which has low residual vibrations in a certain
# frequency range. Similar to discrete input shapers, the amplitude of residual
# vibrations of a step input signal can be calculated as
# *) V_c = integral(exp(-zeta * omega * t) * cos(omega_d * t) *
#                   * w(T_sm/2 - t) * dt, t=[0...T_sm])
# *) V_s = integral(exp(-zeta * omega * t) * sin(omega_d * t) *
#                   * w(T_sm/2 - t) * dt, t=[0...T_sm])
# *) V = sqrt(V_c^2 + V_s^)
#
# The input smoothers defined below were calculated and optimized in the Maxima
# algebra system by calculating V(w, T_sm, omega, zeta) in a closed form for
# several (depending on smoother) omega_i, zeta_i pairs and optimizing T_sm,
# omega_i, and zeta_i in order to obtain a polynomial w(t) with a minimal T_sm
# such that w(t) >= 0 and V(omega) <= 0.05 in a certain frequency range.
#
# Additional constraints that were enforced for w(t) during optimization:
# *) integral(w(t) * dt, t=[-T_sm/2...T_sm/2]) = 1
# *) w(-T_sm/2) = w(T_sm/2) = 0
# The first condition ensures unity norm of w(t) and the second one makes the
# input smoothers usable as extruder smoothers (for the ease of synchronization
# of the extruder with the toolhead motion).

def get_zv_smoother(shaper_freq, damping_ratio_unused=None,
                    normalize_coeffs=True):
    coeffs = [-118.4265334338076,5.861885495127615,29.52796003014231,
              -1.465471373781904,0.01966833207740377]
    return init_smoother(coeffs, 0.8025 / shaper_freq, normalize_coeffs)

def get_mzv_smoother(shaper_freq, damping_ratio_unused=None,
                     normalize_coeffs=True):
    coeffs = [-1906.717580206364,125.8892756660212,698.0200035767849,
              -37.75923018121473,-62.18762409216703,1.57172781617736,
              1.713117990217123]
    return init_smoother(coeffs, 0.95625 / shaper_freq, normalize_coeffs)

def get_ei_smoother(shaper_freq, damping_ratio_unused=None,
                    normalize_coeffs=True):
    coeffs = [-1797.048868963208,120.5310596109878,669.6653197989012,
              -35.71975707450795,-62.49388325512682,1.396748042940248,
              1.848276903900512]
    return init_smoother(coeffs, 1.06625 / shaper_freq, normalize_coeffs)

def get_2hump_ei_smoother(shaper_freq, damping_ratio_unused=None,
                          normalize_coeffs=True):
    coeffs = [-22525.88434486782,2524.826047114184,10554.22832043971,
               -1051.778387878068,-1475.914693073253,121.2177946817349,
              57.95603221424528,-4.018706414213658,0.8375784787864095]
    return init_smoother(coeffs, 1.14875 / shaper_freq, normalize_coeffs)

def get_si_smoother(shaper_freq, damping_ratio_unused=None,
                    normalize_coeffs=True):
    coeffs = [-6186.76006449789,1206.747198930197,2579.985143622855,
              -476.8554763069169,-295.546608490564,52.69679971161049,
              4.234582468800491,-2.226157642004671,1.267781046297883]
    return init_smoother(coeffs, 1.245 / shaper_freq, normalize_coeffs)

def get_zvd_ei_smoother(shaper_freq, damping_ratio_unused=None,
                        normalize_coeffs=True):
    coeffs = [-18835.07746719777,1914.349309746547,8786.608981369287,
              -807.3061869131075,-1209.429748155012,96.48879052981883,
              43.1595785340444,-3.577268915175282,1.083220648523371]
    return init_smoother(coeffs, 1.475 / shaper_freq, normalize_coeffs)

def get_extr_zv_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [30., 0., -15., 0., 1.875]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_mzv_shaper_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [341.76438367888272,-27.466545717280418,-153.088062685115716,
              13.7332728586402091,12.4632094027673599,-1.7166591073300261,
              1.1121330721453511]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_zvd_shaper_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [630.74844408176386,-22.308531752627160,-307.90095218665919,
              11.1542658763135805,35.685142827998881,-1.3942832345391976,
              0.4670793658889200]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_ei_shaper_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [479.19339444799374,-30.811311356643330,-226.71074702571096,
              15.4056556783216649,23.506612053856646,-1.9257069597902081,
              0.8053718873928709]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_2hump_ei_shaper_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [6732.1368755766744,-451.56419782599357,-3806.9806651540493,
              208.13010436901791,666.92541201595748,-19.3967650921351584,
              -40.412386015136598,-1.1032496589986802,1.6069214755974468]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_3hump_ei_shaper_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [4862.4802465588073,-540.91386960353532,-2666.8882179975822,
              256.61643727342101,445.67339875133428,-26.886868086047631,
              -26.700173911045901,-0.8650310955216656,1.4965209988949661]
    # Ideally, the following coefficients should be used, but this order
    # is currently not supported by C integration routines
    # coeffs = [101624.001181926069,-10140.6488068767758,-67034.280052740767,
    #           5924.1010170704049,15212.5854866606205,-1110.64163123568596,
    #           -1371.73807717349200,78.342425346940715,38.307990049684413,
    #           -3.1226733743674644,1.0704518836439141]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_mzv_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [128.462077073626915,-42.165459650911941,-38.818969860871568,
              21.0827298254559707,-4.6771545208692649,-2.6353412281819963,
              1.5882542922463685]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_ei_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [276.191048111389079,-41.835391901208922,-117.95949005967274,
              20.9176959506044611,7.1939235089509097,-2.6147119938255576,
              1.2585021247513637]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_2hump_ei_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [1672.36152804372318,-70.342256841452126,-786.13315506927438,
              -0.9529934764915708,102.73410996847843,13.665669896018062,
              -8.4896839114829827,-2.2577576185761026,1.4522074338774609]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_zvd_ei_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [1829.54723817218109,-746.70578009311191,-927.21920387422937,
              393.61004675467614,145.568898884847072,-56.797689609879597,
              -13.2775511017668428,1.2660722942575129,1.5624627565635203]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)

def get_extr_si_smoother(smooth_time, normalize_coeffs=True):
    coeffs = [114.962413010742423,164.830749583661599,90.845373435141965,
              -132.210144372387873,-42.617667050825957,35.199306639257386,
              -2.9098248068475399,-3.1121730987848171,1.5225295066412017]
    return init_smoother(coeffs, smooth_time, normalize_coeffs)


# min_freq for each shaper is chosen to have projected max_accel ~= 1500
INPUT_SHAPERS = [
    InputShaperCfg('zv', get_zv_shaper, min_freq=21.),
    InputShaperCfg('mzv', get_mzv_shaper, min_freq=23.),
    InputShaperCfg('zvd', get_zvd_shaper, min_freq=29.),
    InputShaperCfg('ei', get_ei_shaper, min_freq=29.),
    InputShaperCfg('2hump_ei', get_2hump_ei_shaper, min_freq=39.),
    InputShaperCfg('3hump_ei', get_3hump_ei_shaper, min_freq=48.),
]

# min_freq for each smoother is chosen to have projected max_accel ~= 1000
INPUT_SMOOTHERS = [
    InputSmootherCfg('smooth_zv', get_zv_smoother, min_freq=18.),
    InputSmootherCfg('smooth_mzv', get_mzv_smoother, min_freq=20.),
    InputSmootherCfg('smooth_ei', get_ei_smoother, min_freq=21.),
    InputSmootherCfg('smooth_2hump_ei', get_2hump_ei_smoother, min_freq=21.5),
    InputSmootherCfg('smooth_zvd_ei', get_zvd_ei_smoother, min_freq=26.),
    InputSmootherCfg('smooth_si', get_si_smoother, min_freq=21.5),
]

EXTURDER_SMOOTHERS = {
        'default' : get_extr_zv_smoother,
        'mzv' : get_extr_mzv_shaper_smoother,
        'zvd' : get_extr_zvd_shaper_smoother,
        'ei' : get_extr_ei_shaper_smoother,
        '2hump_ei' : get_extr_2hump_ei_shaper_smoother,
        '3hump_ei' : get_extr_3hump_ei_shaper_smoother,
        'smooth_mzv' : get_extr_mzv_smoother,
        'smooth_ei' : get_extr_ei_smoother,
        'smooth_2hump_ei' : get_extr_2hump_ei_smoother,
        'smooth_zvd_ei' : get_extr_zvd_ei_smoother,
        'smooth_si' : get_extr_si_smoother,
}
