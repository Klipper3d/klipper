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

def get_mzv20_smoother(shaper_freq, damping_ratio_unused=None,
                       normalize_coeffs=True):
    coeffs = [-1928.625991372561,174.47238885576,680.8819871456237,
              -54.08725118644957,-55.28572183696386,2.617288493127363,
              1.401087377835678]
    return init_smoother(coeffs, 0.85175 / shaper_freq, normalize_coeffs)

def get_mzv16_smoother(shaper_freq, damping_ratio_unused=None,
                       normalize_coeffs=True):
    coeffs = [-1938.774071514084,157.7065914461193,711.7324202087348,
              -48.34288782208795,-63.72538103023892,2.22905999013954,
              1.741413861921329]
    return init_smoother(coeffs, 0.9665 / shaper_freq, normalize_coeffs)

def get_2hump_ei_smoother(shaper_freq, damping_ratio_unused=None,
                          normalize_coeffs=True):
    coeffs = [-22525.88434486782,2524.826047114184,10554.22832043971,
               -1051.778387878068,-1475.914693073253,121.2177946817349,
              57.95603221424528,-4.018706414213658,0.8375784787864095]
    return init_smoother(coeffs, 1.14875 / shaper_freq, normalize_coeffs)

def get_si_smoother(shaper_freq, damping_ratio_unused=None,
                    normalize_coeffs=True):
    coeffs = [-121313.2032178889,26921.56924860698,63029.77630233325,
              -13293.49355097946,-10648.58182242004,2050.103214332665,
              666.126576474768,-102.9883580154977,-17.0691108398123,
              0.1640954647660932,1.278318992073449]
    return init_smoother(coeffs, 1.2 / shaper_freq, normalize_coeffs)

def get_nei_smoother(shaper_freq, damping_ratio_unused=None,
                    normalize_coeffs=True):
    coeffs = [4883445.221306241,-720494.0751766556,-3148819.580240736,
              424777.3712490448,721441.1426576816,-85447.8333563426,
              -70054.38903665643,6764.372246231036,2669.737943114452,
              -180.6374244922258,-30.4660136303673,1.82937577459115]
    return init_smoother(coeffs, 1.42 / shaper_freq, normalize_coeffs)

def get_nsi_smoother(shaper_freq, damping_ratio_unused=None,
                    normalize_coeffs=True):
    coeffs = [1.576037300103821e7,-3450496.635175148,-1.138914512941258e7,
              2263562.40616947,3103579.174607452,-537674.9863778015,
              -397766.323762382,56331.41234254206,24270.29707072854,
              -2532.279663870225,-598.7901101230622,36.92208279328751,
              -0.9886556775166597,1.047798776818597]
    return init_smoother(coeffs, 1.58 / shaper_freq, normalize_coeffs)

# min_freq for each shaper is chosen to have projected max_accel ~= 1500
INPUT_SHAPERS = [
    InputShaperCfg('zv', get_zv_shaper, min_freq=21.),
    InputShaperCfg('mzv', get_mzv_shaper, min_freq=23.),
    InputShaperCfg('zvd', get_zvd_shaper, min_freq=29.),
    InputShaperCfg('ei', get_ei_shaper, min_freq=29.),
    InputShaperCfg('2hump_ei', get_2hump_ei_shaper, min_freq=39.),
    InputShaperCfg('3hump_ei', get_3hump_ei_shaper, min_freq=48.),
]

INPUT_SMOOTHERS = [
    InputSmootherCfg('smooth_zv', get_zv_smoother, min_freq=21.),
    InputSmootherCfg('smooth_mzv20', get_mzv20_smoother, min_freq=23.),
    InputSmootherCfg('smooth_mzv16', get_mzv16_smoother, min_freq=23.),
    InputSmootherCfg('smooth_2hump_ei', get_2hump_ei_smoother, min_freq=23.),
    InputSmootherCfg('smooth_si', get_si_smoother, min_freq=23.),
    InputSmootherCfg('smooth_nei', get_nei_smoother, min_freq=23.),
    InputSmootherCfg('smooth_nsi', get_nsi_smoother, min_freq=23.),
]
