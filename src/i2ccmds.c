// Commands for sending messages on an I2C bus
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_WANT_SOFTWARE_I2C
#include "basecmd.h" //oid_alloc
#include "command.h"  //sendf
#include "sched.h" //DECL_COMMAND
#include "board/gpio.h" //i2c_write/read/setup
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "i2c_software.h" // i2c_software_setup
#include "i2ccmds.h"

void
command_config_i2c(uint32_t *args)
{
    struct i2cdev_s *i2c = oid_alloc(args[0], command_config_i2c
                                     , sizeof(*i2c));
}
DECL_COMMAND(command_config_i2c, "config_i2c oid=%c");

struct i2cdev_s *
i2cdev_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_i2c);
}

void
command_i2c_set_bus(uint32_t *args)
{
    uint8_t addr = args[3] & 0x7f;
    struct i2cdev_s *i2c = i2cdev_oid_lookup(args[0]);
    i2c->i2c_config = i2c_setup(args[1], args[2], addr);
    i2c->timer.func = i2c_async;
}
DECL_COMMAND(command_i2c_set_bus,
             "i2c_set_bus oid=%c i2c_bus=%u rate=%u address=%u");

void
i2cdev_set_software_bus(struct i2cdev_s *i2c, struct i2c_software *is)
{
    i2c->i2c_software = is;
    i2c->flags |= IF_SOFTWARE;
    i2c->timer.func = i2c_software_async;
}

void i2c_buf_write_b(struct i2cdev_s *i2c, uint8_t byte)
{
    i2c->buf[i2c->head] = byte;
    i2c->head = (i2c->head + 1) % sizeof(i2c->buf);
    i2c->size++;
}

uint8_t i2c_buf_read_b(struct i2cdev_s *i2c)
{
    uint8_t b = i2c->buf[i2c->tail];
    i2c->tail = (i2c->tail + 1) % sizeof(i2c->buf);
    i2c->size--;
    return b;
}

// shift cmd buffer cmd[N] = cmd[N+1]
void i2c_cmd_done(struct i2cdev_s *i2c)
{
    memmove(i2c->data_len, &i2c->data_len[1], sizeof(i2c->data_len) - 1);
    i2c->data_len[sizeof(i2c->data_len) - 1] = 0;
}

void command_i2c_write(uint32_t *args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t data_len = args[1];
    uint8_t *data = command_decode_ptr(args[2]);
    uint8_t start = 0;

    if (i2c->data_len[sizeof(i2c->data_len)-1]) {
        shutdown("i2c_write: cmd buffer overload");
    }

    // write register -> read, write can be empty
    if (i2c->flags & IF_RW) {
        start = 1;
    }

    while (i2c->data_len[start]) {
        start++;
    }

    if (data_len > sizeof(i2c->buf) - i2c->size)
        shutdown("i2c_write: buffer overload - data too large");

    i2c->data_len[start] = data_len;
    for (uint8_t i = 0; i < data_len; i++) {
        i2c_buf_write_b(i2c, data[i]);
    }

    if (i2c->flags & IF_ACTIVE)
        return;

    i2c->flags |= IF_ACTIVE;
    irq_disable();
    i2c->timer.waketime = timer_read_time() + timer_from_us(200);
    sched_add_timer(&i2c->timer);
    irq_enable();
}
DECL_COMMAND(command_i2c_write, "i2c_write oid=%c data=%*s");

// Expects that i2c->read len is 0 and read_len = i2c->cur - i2c->tail;
static uint_fast8_t
i2c_read_response(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    uint8_t read_len = i2c->cur - i2c->tail;
    sendf("i2c_read_response oid=%c response=%*s", i2c->oid, read_len
          , &i2c->buf[i2c->tail]);
    for (int i = 0; i < read_len; i++) {
        i2c_buf_read_b(i2c);
    }
    i2c_cmd_done(i2c);

    if (CONFIG_WANT_SOFTWARE_I2C && i2c->flags & IF_SOFTWARE)
        i2c->timer.func = i2c_software_async;
    else
        i2c->timer.func = i2c_async;
    i2c->flags &= ~(IF_RW);
    i2c->timer.waketime = timer_read_time() + timer_from_us(50);
    return SF_RESCHEDULE;
}

void
command_i2c_read(uint32_t * args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t reg_len = args[1];
    uint8_t *reg = command_decode_ptr(args[2]);
    uint8_t data_len = args[3];
    if (i2c->flags & IF_ACTIVE)
        // i2C is busy - silently drop this request (host should retransmit)
        return;

    // buffer is always empty here
    if (reg_len + data_len > sizeof(i2c->buf))
        shutdown("i2c_read: buffer overload - data too large");

    i2c->oid = oid;
    i2c->tail = 0;
    i2c->head = 0;

    i2c->data_len[0] = reg_len;
    i2c->data_len[1] = data_len | I2C_R;
    for (int i = 0; i < reg_len; i++) {
        i2c_buf_write_b(i2c, reg[i]);
    }
    // reserve space
    for (int i = 0; i < data_len; i++) {
        i2c_buf_write_b(i2c, 0);
    }

    i2c->flags |= IF_ACTIVE | IF_RW;
    i2c->callback = i2c_read_response;
    irq_disable();
    i2c->timer.waketime = timer_read_time() + timer_from_us(200);
    sched_add_timer(&i2c->timer);
    irq_enable();
}
DECL_COMMAND(command_i2c_read, "i2c_read oid=%c reg=%*s read_len=%u");

static uint_fast8_t
i2c_modify_bits(struct timer *timer)
{
    struct i2cdev_s *i2c = container_of(timer, struct i2cdev_s, timer);
    uint8_t read_len = i2c->cur - i2c->tail;
    uint8_t buf[read_len];

    for (int i = 0; i < read_len; i++) {
        buf[i] = i2c_buf_read_b(i2c);
    }
    i2c_cmd_done(i2c);

    for (int i = 0; i < read_len; i++) {
        buf[i] &= i2c_buf_read_b(i2c);
    }
    i2c_cmd_done(i2c);

    for (int i = 0; i < read_len; i++) {
        buf[i] |= i2c_buf_read_b(i2c);
    }
    i2c_cmd_done(i2c);

    // Because of FIFO next write is ours
    uint8_t reg_len = i2c->data_len[0] - read_len;
    i2c->cur = i2c->tail + reg_len;
    for (int i = 0; i < read_len; i++) {
        i2c->buf[i2c->cur] = buf[i];
        i2c->cur++;
    }

    i2c->flags &= ~(IF_RW);
    if (CONFIG_WANT_SOFTWARE_I2C && i2c->flags & IF_SOFTWARE)
        timer->func = i2c_software_async;
    else
        timer->func = i2c_async;
    timer->waketime = timer_read_time() + timer_from_us(200);
    return SF_RESCHEDULE;
}

void
command_i2c_modify_bits(uint32_t *args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t reg_len = args[1];
    uint8_t *reg = command_decode_ptr(args[2]);
    uint32_t clear_set_len = args[3];
    if (clear_set_len % 2 != 0)
        shutdown("i2c_modify_bits: Odd number of bits!");
    uint8_t data_len = clear_set_len/2;
    uint8_t *clear_set = command_decode_ptr(args[4]);

    if (i2c->flags & IF_ACTIVE) {
        shutdown("i2c_modify_bits: busy in process");
        // i2c is busy - drop request for now
        // I think there should be some sort of msg queue encoding,
        // implemented again
        // But every new supported RMW will complicate code.
        return;
    }

    if (reg_len * 2 + data_len * 4 > sizeof(i2c->buf))
        shutdown("i2c_modify_bits: buffer overload - data too large");

    i2c->tail = 0;
    i2c->head = 0;

    i2c->data_len[0] = reg_len;
    for (int i = 0; i < reg_len; i++) {
        i2c_buf_write_b(i2c, reg[i]);
    }

    i2c->data_len[1] = data_len | I2C_R;
    // reserve space for read
    for (int i = 0; i < data_len; i++) {
        i2c_buf_write_b(i2c, 0);
    }

    // save clear bits
    i2c->data_len[2] = data_len | I2C_R;
    for (int i = 0; i < data_len; i++) {
        i2c_buf_write_b(i2c, ~clear_set[i]);
    }

    // save set bits
    i2c->data_len[3] = data_len | I2C_R;
    for (int i = 0; i < data_len; i++) {
        i2c_buf_write_b(i2c, clear_set[data_len + i]);
    }

    // Reserve space for later write
    i2c->data_len[4] = reg_len + data_len;
    for (int i = 0; i < reg_len + data_len; i++) {
        i2c_buf_write_b(i2c, reg[i]);
    }

    i2c->flags |= IF_ACTIVE | IF_RW;
    i2c->callback = i2c_modify_bits;
    irq_disable();
    i2c->timer.waketime = timer_read_time() + timer_from_us(200);
    sched_add_timer(&i2c->timer);
    irq_enable();
}
DECL_COMMAND(command_i2c_modify_bits,
             "i2c_modify_bits oid=%c reg=%*s clear_set_bits=%*s");
