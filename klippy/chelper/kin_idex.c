// Idex dual carriage kinematics
//
// Copyright (C) 2023  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // struct move

#define DUMMY_T 500.0

struct dual_carriage_stepper {
    struct stepper_kinematics sk;
    struct stepper_kinematics *orig_sk;
    struct move m;
    double x_scale, x_offs, y_scale, y_offs;
};

double
dual_carriage_calc_position(struct stepper_kinematics *sk, struct move *m
                            , double move_time)
{
    struct dual_carriage_stepper *dc = container_of(
            sk, struct dual_carriage_stepper, sk);
    struct coord pos = move_get_coord(m, move_time);
    dc->m.start_pos.x = pos.x * dc->x_scale + dc->x_offs;
    dc->m.start_pos.y = pos.y * dc->y_scale + dc->y_offs;
    dc->m.start_pos.z = pos.z;
    return dc->orig_sk->calc_position_cb(dc->orig_sk, &dc->m, DUMMY_T);
}

void __visible
dual_carriage_set_sk(struct stepper_kinematics *sk
                     , struct stepper_kinematics *orig_sk)
{
    struct dual_carriage_stepper *dc = container_of(
            sk, struct dual_carriage_stepper, sk);
    dc->sk.calc_position_cb = dual_carriage_calc_position;
    dc->sk.active_flags = orig_sk->active_flags;
    dc->orig_sk = orig_sk;
    dc->sk.commanded_pos = orig_sk->commanded_pos;
    dc->sk.last_flush_time = orig_sk->last_flush_time;
    dc->sk.last_move_time = orig_sk->last_move_time;
}

void __visible
dual_carriage_set_transform(struct stepper_kinematics *sk
                            , double x_scale, double x_offs
                            , double y_scale, double y_offs)
{
    struct dual_carriage_stepper *dc = container_of(
            sk, struct dual_carriage_stepper, sk);
    dc->x_scale = x_scale;
    dc->x_offs = x_offs;
    dc->y_scale = y_scale;
    dc->y_offs = y_offs;
}

struct stepper_kinematics * __visible
dual_carriage_alloc(void)
{
    struct dual_carriage_stepper *dc = malloc(sizeof(*dc));
    memset(dc, 0, sizeof(*dc));
    dc->m.move_t = 2. * DUMMY_T;
    return &dc->sk;
}
