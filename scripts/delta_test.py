#!/usr/bin/env python2
# This is a very primitive testing script for delta calibration.
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# This script must be hand modified to fill in the DeltaParams,
# Positions, SizeDist, AngleDist parameters!
import sys, math, logging
sys.path.append('./klippy')
import mathutil

# Fill this in with information from the printer.cfg file
DeltaParams = {
    'radius': 146.477116,
    'endstop_a': 257.222855, 'endstop_b': 255.270719, 'endstop_c': 254.908570,
    'stepdist_a': 0.0125, 'stepdist_b': 0.0125, 'stepdist_c': 0.0125,
    'angle_a': 210.643430, 'angle_b': 330.766870, 'angle_c': 90.000000,
    'arm_a': 272.333552, 'arm_b': 271.541059, 'arm_c': 274.083175,
    'x_adjust': 0., 'y_adjust': 0.,
}

# Fill this in with "stable position" of last DELTA_CALIBRATE (fill
# from details in log file from last DELTA_CALIBRATE run).
Positions = [
    [20590, 20410, 20410], [22791, 22611, 18162], [19736, 24154, 19560],
    [18515, 22311, 22300], [19824, 19644, 23107], [22193, 18514, 22004],
    [23791, 19582, 19586]
]
# Fill this in with "center to outer" distances minus center pillar.
# Start with A and go counter-clockwise. (The print must be done with
# a printer.cfg corresponding to params in DeltaParams).
SizeDist = [
    74.59 - 9.29, 74.44 - 9.35, 74.57 - 9.34,
    74.12 - 9.29, 74.51 - 9.35, 74.30 - 9.34,
]
# Fill this in with "outer to outer" distances minus first pillar.
# Start with "A to far C" and go counter-clockwise.
AngleDist = [
    74.47 - 9.30, 74.33 - 9.38,
    74.42 - 9.32, 74.31 - 9.32,
    74.40 - 9.32, 74.49 - 9.40,
]

# Details of printed calibration object
MeasureAngles = [210., 270., 330., 30., 90., 150.]
MeasureOuterRadius = 65
MeasureRidgeRadius = 5. - .5

def calc_stable_position(coord, params):
    angle_names = ['angle_a', 'angle_b', 'angle_c']
    angles = [math.radians(params[an]) for an in angle_names]
    radius = params['radius']
    radius2 = radius**2
    towers = [(math.cos(a) * radius, math.sin(a) * radius) for a in angles]
    arm2 = [params[an]**2 for an in ['arm_a', 'arm_b', 'arm_c']]
    stepdist_names = ['stepdist_a', 'stepdist_b', 'stepdist_c']
    stepdists = [params[sn] for sn in stepdist_names]
    endstop_names = ['endstop_a', 'endstop_b', 'endstop_c']
    endstops = [params[en] + math.sqrt(a2 - radius2)
                for en, a2 in zip(endstop_names, arm2)]
    spos = [math.sqrt(a2 - (t[0]-coord[0])**2 - (t[1]-coord[1])**2) + coord[2]
            for t, a2 in zip(towers, arm2)]
    return [(ep - sp) / sd
            for ep, sp, sd in zip(endstops, spos, stepdists)]

def get_positions_from_stable(stable_positions, params):
    angle_names = ['angle_a', 'angle_b', 'angle_c']
    angles = [math.radians(params[an]) for an in angle_names]
    radius = params['radius']
    radius2 = radius**2
    towers = [(math.cos(a) * radius, math.sin(a) * radius) for a in angles]
    arm2 = [params[an]**2 for an in ['arm_a', 'arm_b', 'arm_c']]
    stepdist_names = ['stepdist_a', 'stepdist_b', 'stepdist_c']
    stepdists = [params[sn] for sn in stepdist_names]
    endstop_names = ['endstop_a', 'endstop_b', 'endstop_c']
    endstops = [params[en] + math.sqrt(a2 - radius2)
                for en, a2 in zip(endstop_names, arm2)]
    out = []
    for spos in stable_positions:
        sphere_coords = [
            (t[0], t[1], es - sp * sd)
            for t, es, sd, sp in zip(towers, endstops, stepdists, spos) ]
        out.append(mathutil.trilateration(sphere_coords, arm2))
    return out

def calc_dist(p1, p2):
    x1, y1, z1 = p1
    x2, y2, z2 = p2
    return math.sqrt((x1-x2)**2 + (y1-y2)**2 + (z1-z2)**2)

def do_calc(params, positions, z_offset, ridge_spos, dist_list):
    logging.info("Calculating delta_calibrate with: %s", positions)
    logging.info("Initial delta_calibrate parameters: %s", params)
    #adj_params = ('endstop_a', 'endstop_b', 'endstop_c', 'radius',
    #              'angle_a', 'angle_b')
    adj_params = ('endstop_a', 'endstop_b', 'endstop_c',
                  'radius', 'angle_a', 'angle_b',
                  'arm_a', 'arm_b', 'arm_c',
                  #'stepdist_a', 'stepdist_b', 'stepdist_c',
                  #'x_adjust', 'y_adjust'
    )
    def delta_errorfunc(params):
        for sn in ['stepdist_a', 'stepdist_b', 'stepdist_c']:
            if params[sn] <= 0. or params[sn] >= .02:
                return 99999999999999999.
        x_adjust, y_adjust = params['x_adjust'], params['y_adjust']
        total_error = 0.
        for x, y, z in get_positions_from_stable(positions, params):
            adj_z = z - x*x_adjust - y*y_adjust
            total_error += (adj_z - z_offset)**2
        total_error *= 4.
        ridge_pos = get_positions_from_stable(ridge_spos, params)
        for i, d in enumerate(dist_list):
            p1x, p1y, p1z = ridge_pos[i*2]
            p2x, p2y, p2z = ridge_pos[i*2+1]
            p1 = (p1x, p1y, p1z - p1x*x_adjust - p1y*y_adjust)
            p2 = (p2x, p2y, p2z - p2x*x_adjust - p2y*y_adjust)
            pos_dist = calc_dist(p1, p2)
            total_error += (pos_dist - d)**2
        return total_error
    new_params = mathutil.coordinate_descent(
        adj_params, params, delta_errorfunc)
    logging.info("Calculated delta_calibrate parameters: %s", new_params)
    old_positions = get_positions_from_stable(positions, params)
    new_positions = get_positions_from_stable(positions, new_params)
    for oldpos, newpos in zip(old_positions, new_positions):
        logging.info("orig: %s new: %s", oldpos, newpos)
    old_ridges = get_positions_from_stable(ridge_spos, DeltaParams)
    new_ridges = get_positions_from_stable(ridge_spos, new_params)
    for i, d in enumerate(dist_list):
        logging.info("%s\n->%s\norig: %.6f new: %.6f want: %.6f",
                     old_ridges[i*2], new_ridges[i*2],
                     calc_dist(old_ridges[i*2], old_ridges[i*2+1]),
                     calc_dist(new_ridges[i*2], new_ridges[i*2+1]),
                     d)

def translate_dists(params):
    obj_angles = map(math.radians, MeasureAngles)
    xy_angles = zip(map(math.cos, obj_angles), map(math.sin, obj_angles))
    size_dists = [MeasureRidgeRadius, MeasureOuterRadius + MeasureRidgeRadius]
    size_pos = [(ax * d, ay * d, 0.)
                for ax, ay in xy_angles for d in size_dists]
    shifted_angles = xy_angles[2:] + xy_angles[:2]
    angle_pos = [(sx*d + ax*MeasureOuterRadius, sy*d + ay*MeasureOuterRadius, 0.)
                 for (sx, sy), (ax, ay) in zip(shifted_angles, xy_angles)
                 for d in size_dists]
    res = [calc_stable_position(p, params) for p in size_pos + angle_pos]
    return res, SizeDist + AngleDist

def main():
    logging.basicConfig(level=logging.DEBUG)
    z_offset = 0.
    positions = list(Positions)
    params = dict(DeltaParams)

    # ridges
    ridge_spos, ridge_dist = translate_dists(DeltaParams)

    # Calculation
    do_calc(params, positions, z_offset, ridge_spos, ridge_dist)

if __name__ == '__main__':
    main()
