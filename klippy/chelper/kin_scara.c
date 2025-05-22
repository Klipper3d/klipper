// SCARA kinematics stepper pulse time generation
//
// Copyright (C) 2025  sun <summerkiller_sun@163.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include <math.h>   // M_PI, cos, sin, atan2, sqrt
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_coord
#include "kin_scara.h"

static void scara_forward_kinematics(double theta, double phi, double *x, double *y,
                                     const struct scara_params *params) {
    theta = theta * M_PI / 180.0; // 转换为弧度
    phi = phi * M_PI / 180.0;     // 转换为弧度
    *x = params->arm1 * cos(theta) + params->arm2 * cos(theta + phi);
    *y = params->arm1 * sin(theta) + params->arm2 * sin(theta + phi);
}

// 逆向运动学：根据末端位置计算关节角度
static void scara_inverse_kinematics(double x, double y, double *theta, double *phi,
                                     const struct scara_params *params) {
    double c2 = (x * x + y * y - params->arm1 * params->arm1 - params->arm2 * params->arm2) /
                (2 * params->arm1 * params->arm2);
    double s2 = sqrt(1 - c2 * c2);
    *phi = atan2(s2, c2);

    double k1 = params->arm1 + params->arm2 * c2;
    double k2 = params->arm2 * s2;
    *theta = atan2(y, x) - atan2(k2, k1);

    // 转换为角度
    *theta = *theta * 180.0 / M_PI;
    *phi = *phi * 180.0 / M_PI;
}

// 计算关节1的位置
static double
scara_stepper_theta_calc_position(struct stepper_kinematics *sk, struct move *m, double move_time) {
    struct scara_params *params = (struct scara_params *)sk->data;
    double x = move_get_coord(m, move_time).x;
    double y = move_get_coord(m, move_time).y;
    double theta, phi;
    scara_inverse_kinematics(x, y, &theta, &phi, params);
    return theta;
}

// 计算关节2的位置
static double
scara_stepper_phi_calc_position(struct stepper_kinematics *sk, struct move *m, double move_time) {
    struct scara_params *params = (struct scara_params *)sk->data;
    double x = move_get_coord(m, move_time).x;
    double y = move_get_coord(m, move_time).y;
    double theta, phi;
    scara_inverse_kinematics(x, y, &theta, &phi, params);
    return phi;
}

// 计算Z轴的位置
static double
scara_stepper_z_calc_position(struct stepper_kinematics *sk, struct move *m, double move_time) {
    return move_get_coord(m, move_time).z;
}

// 初始化 SCARA 运动学解释器
struct stepper_kinematics * __visible
scara_stepper_alloc(char axis, double arm1, double arm2,
                    double j1, double j2, double g1, double g2) {
    struct stepper_kinematics *sk = malloc(sizeof(*sk));
    memset(sk, 0, sizeof(*sk));

    // 分配 SCARA 参数
    struct scara_params *params = malloc(sizeof(struct scara_params));
    params->arm1 = arm1;
    params->arm2 = arm2;
    params->j1 = j1;
    params->j2 = j2;
    params->g1 = g1;
    params->g2 = g2;
    sk->data = params;

    // 设置计算回调函数
    if (axis == 't') { // 关节1 (theta)
        sk->calc_position_cb = scara_stepper_theta_calc_position;
        sk->active_flags = AF_X; // 假设关节1对应X轴
    } else if (axis == 'p') { // 关节2 (phi)
        sk->calc_position_cb = scara_stepper_phi_calc_position;
        sk->active_flags = AF_Y; // 假设关节2对应Y轴
    } else if (axis == 'z') { // Z轴
        sk->calc_position_cb = scara_stepper_z_calc_position;
        sk->active_flags = AF_Z;
    } else {
        free(params);
        free(sk);
        return NULL;
    }

    return sk;
}