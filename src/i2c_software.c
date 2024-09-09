// Software I2C emulation
//
// Copyright (C) 2023  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2023  Alan.Ma <tech@biqu3d.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/gpio.h" // gpio_out_setup
#include "board/internal.h" // gpio_peripheral
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_shutdown
#include "i2ccmds.h" // i2cdev_set_software_bus

struct i2c_software {
    struct gpio_out scl_out, sda_out;
    struct gpio_in scl_in, sda_in;
    uint8_t addr;
    unsigned int ticks;
    unsigned int ticks_half;

    // callback to return to
    uint_fast8_t (*func)(struct timer *timer);

    // bit-banging
    uint8_t byte;
    uint8_t nack;
    int i;
};

static unsigned int
nsecs_to_ticks(uint32_t ns)
{
    return timer_from_us(ns * 1000) / 1000000;
}

void
command_i2c_set_software_bus(uint32_t *args)
{
    struct i2cdev_s *i2c = i2cdev_oid_lookup(args[0]);
    struct i2c_software *is = alloc_chunk(sizeof(*is));
    int rate = args[3] / 1000;
    int ns = 1000000 / rate / 2;
    is->ticks = nsecs_to_ticks(ns);
    is->ticks_half = is->ticks / 2;
    is->addr = (args[4] & 0x7f) << 1; // address format shifted
    is->scl_in = gpio_in_setup(args[1], 1);
    is->scl_out = gpio_out_setup(args[1], 1);
    is->sda_in = gpio_in_setup(args[2], 1);
    is->sda_out = gpio_out_setup(args[2], 1);
    i2cdev_set_software_bus(i2c, is);

    // time correction
    uint32_t start = timer_read_time();
    gpio_out_reset(is->scl_out, 0);
    gpio_in_reset(is->scl_in, 1);
    uint32_t end = timer_read_time();
    uint32_t t_diff = (end - start) / 2;
    if (t_diff < is->ticks) {
        is->ticks = is->ticks - t_diff;
        is->ticks_half = is->ticks / 2;
    }
}
DECL_COMMAND(command_i2c_set_software_bus,
             "i2c_set_software_bus oid=%c scl_pin=%u sda_pin=%u"
             " rate=%u address=%u");

// Example implementation of Async RW of I2C
// Function chaining used for simplify flag checks & overral logic

uint_fast8_t i2c_software_async(struct timer *timer);

static uint_fast8_t
i2c_async_end(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;

    is->i++;
    switch (is->i) {
    case 1:
        gpio_out_reset(is->sda_out, 0);
        timer->waketime = timer_read_time() + is->ticks_half;
        return SF_RESCHEDULE;
    case 2:
        gpio_in_reset(is->scl_in, 1);
        timer->waketime = timer_read_time() + is->ticks_half;
        return SF_RESCHEDULE;
    }
    gpio_in_reset(is->sda_in, 1);

    if (is->nack)
        shutdown("soft_i2c NACK");

    timer->func = i2c_software_async;
    if (i2c->flags & IF_RW)
        return i2c->callback(timer);
    timer->waketime = timer_read_time() + is->ticks;
    return SF_RESCHEDULE;
}

static uint_fast8_t
i2c_async_read_byte(struct timer *timer);

static uint_fast8_t sda_read(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;
    i2c->buf[i2c->cur] <<= 1;
    i2c->buf[i2c->cur] |= gpio_in_read(is->sda_in);
    gpio_out_reset(is->scl_out, 0);
    is->i++;
    timer->func = i2c_async_read_byte;
    if (is->i == 8)
        timer->waketime = timer_read_time() + is->ticks_half;
    else
        timer->waketime = timer_read_time() + is->ticks;
    return SF_RESCHEDULE;
}

static uint_fast8_t
i2c_async_read_byte(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;

    if (is->i < 8) {
        gpio_in_reset(is->scl_in, 1);
        timer->func = sda_read;
        timer->waketime = timer_read_time() + is->ticks;
        return SF_RESCHEDULE;
    }

    if (is->i == 8) {
        is->i++;
        if (i2c->data_len[0] == 0) {
            gpio_in_reset(is->sda_in, 1);
        } else {
            gpio_out_reset(is->sda_out, 0);
        }
        timer->waketime = timer_read_time() + is->ticks_half;
        return SF_RESCHEDULE;
    }

    if (is->i == 9) {
        gpio_in_reset(is->scl_in, 1);
        is->i++;
        timer->waketime = timer_read_time() + is->ticks;
        return SF_RESCHEDULE;
    }

    gpio_out_reset(is->scl_out, 0);
    i2c->cur++;
    is->i = 0;
    if (i2c->data_len[0] == 0) {
        timer->func = i2c_async_end;
        return i2c_async_end(timer);
    }

    timer->func = is->func;
    timer->waketime = timer_read_time() + is->ticks_half;
    return SF_RESCHEDULE;
}

static uint_fast8_t
i2c_async_read(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;

    i2c->data_len[0]--;
    gpio_in_reset(is->sda_in, 1);
    is->i = 0;
    is->func = i2c_async_read;
    i2c->buf[i2c->cur] = 0;
    timer->func = i2c_async_read_byte;
    timer->waketime = timer_read_time() + is->ticks_half;
    return SF_RESCHEDULE;
}

static uint_fast8_t
i2c_async_send_byte(struct timer *timer);
static uint_fast8_t send_bit(struct timer *timer);
static uint_fast8_t scl_high(struct timer *timer);

static uint_fast8_t scl_low(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;
    gpio_out_reset(is->scl_out, 0);
    timer->func = send_bit;
    timer->waketime = timer_read_time() + is->ticks_half;
    return SF_RESCHEDULE;
}

static uint_fast8_t send_bit(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;
    if (is->byte & 0x80) {
        gpio_in_reset(is->sda_in, 1);
    } else {
        gpio_out_reset(is->sda_out, 0);
    }
    is->byte <<= 1;
    is->i++;
    timer->func = scl_high;
    timer->waketime = timer_read_time() + is->ticks_half;
    return SF_RESCHEDULE;
}

static uint_fast8_t scl_high(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;
    gpio_in_reset(is->scl_in, 1);
    timer->func = i2c_async_send_byte;
    timer->waketime = timer_read_time() + is->ticks_half;
    return SF_RESCHEDULE;
}

static uint_fast8_t fin_scl_low(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;
    gpio_out_reset(is->scl_out, 0);
    timer->func = i2c_async_send_byte;
    timer->waketime = timer_read_time() + is->ticks_half;
    return SF_RESCHEDULE;
}

static uint_fast8_t
i2c_async_send_byte(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;

    if (is->i < 8) {
        timer->func = scl_low;
        goto sched;
    }

    // prepare ACK
    if (is->i == 8) {
        is->byte = 1 << 7;
        timer->func = scl_low;
        goto sched;
    }

    if (is->i == 9) {
        is->nack = gpio_in_read(is->sda_in);
        is->i++;
        timer->func = fin_scl_low;
        goto sched;
    }

    gpio_in_reset(is->sda_in, 1);
    // HW controllers ignore last NACK
    if (is->nack && i2c->data_len[0] != 0) {
        // Stop as HW controller
        timer->func = i2c_async_end;
        return i2c_async_end(timer);
    }
    is->i = 0;
    timer->func = is->func;
    return is->func(timer);
sched:
    timer->waketime = timer_read_time() + is->ticks_half;
    return SF_RESCHEDULE;
}

static uint_fast8_t
i2c_async_start(struct timer *timer);

static uint_fast8_t
i2c_async_write(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;
    is->i = 0;
    is->byte = i2c_buf_read_b(i2c);
    i2c->data_len[0]--;

    timer->func = i2c_async_send_byte;
    if (i2c->data_len[0]) {
        is->func = i2c_async_write;
        return i2c_async_send_byte(timer);
    }
    i2c_cmd_done(i2c);
    is->i = 0;
    if (i2c->flags & IF_RW)
        is->func = i2c_async_start;
    else
        is->func = i2c_async_end;
    return i2c_async_send_byte(timer);
}

static uint_fast8_t
i2c_async_start(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    struct i2c_software *is = i2c->i2c_software;
    uint8_t rw_bit;

    // start/re-start and read data
    is->i++;
    switch (is->i)
    {
    case 1:
        gpio_in_reset(is->sda_in, 1);
        timer->func = i2c_async_start;
        timer->waketime = timer_read_time() + is->ticks_half;
        return SF_RESCHEDULE;
    case 2:
        gpio_in_reset(is->scl_in, 1);
        timer->func = i2c_async_start;
        timer->waketime = timer_read_time() + is->ticks_half;
        return SF_RESCHEDULE;
    }
    gpio_out_reset(is->sda_out, 0);

    rw_bit = ((i2c->data_len[0] & IF_RW) >> 7);
    is->i = 0;
    is->byte = is->addr | rw_bit;
    i2c->cur = i2c->tail;
    i2c->data_len[0] &= ~(I2C_R);
    if (rw_bit)
        is->func = i2c_async_read;
    else
        is->func = i2c_async_write;
    timer->func = i2c_async_send_byte;
    return i2c_async_send_byte(timer);
}

uint_fast8_t i2c_software_async(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    // write register + read
    if (i2c->data_len[0] || i2c->data_len[1] & IF_RW) {
        if (i2c->data_len[0] == 0)
            // cleanup empty write
            i2c_cmd_done(i2c);
        i2c->i2c_software->i = 0;
        return i2c_async_start(timer);
    }

    i2c->flags &= ~IF_ACTIVE;
    return SF_DONE;
}
