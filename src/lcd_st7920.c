// Commands for sending messages to an st7920 lcd driver
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK

struct st7920 {
    struct task_wake wake;
    uint32_t nexttime;
    uint8_t flags;
    struct gpio_out sclk, sid;
    uint8_t cur_pos, next_pos, end_pos;
    uint8_t buffer[32];
};

static struct st7920 *main_st7920;

#define SYNC_CMD  0xf80000UL
#define SYNC_DATA 0xfa0000UL

static void
st7920_xmit(struct st7920 *s, uint32_t data)
{
    struct gpio_out sclk = s->sclk, sid = s->sid;
    uint8_t i, last_b = 0;
    for (i=0; i<3; i++) {
        uint8_t b = data >> 16, j;
        data <<= 8;
        for (j=0; j<8; j++) {
            if ((b ^ last_b) & 0x80)
                gpio_out_toggle(sid);
            gpio_out_toggle(sclk);
            last_b = b;
            b <<= 1;
            asm("nop\n nop");
            gpio_out_toggle(sclk);
        }
    }
    s->nexttime = timer_read_time() + timer_from_us(72);
    if (last_b & 0x80)
        gpio_out_toggle(sid);
}

void
command_config_st7920(uint32_t *args)
{
    if (main_st7920)
        shutdown("st7920 already configured");
    struct st7920 *s = oid_alloc(
        args[0], command_config_st7920, sizeof(*main_st7920));
    s->sclk = gpio_out_setup(args[1], 0);
    s->sid = gpio_out_setup(args[2], 0);
    main_st7920 = s;
}
DECL_COMMAND(command_config_st7920,
             "config_st7920 oid=%c sclk_pin=%u sid_pin=%u");

void
command_st7920_burst(uint32_t *args)
{
    struct st7920 *s = main_st7920;
    if (!s)
        shutdown("st7920 not configured");
    uint8_t len = args[0];
    char *cmd_then_data = (void*)(size_t)args[1];
    uint8_t end_pos = s->end_pos;
    if (!len || end_pos + len + 1 > sizeof(s->buffer))
        shutdown("st7920 buffer overflow");
    s->buffer[end_pos] = len;
    memcpy(&s->buffer[end_pos+1], cmd_then_data, len);
    s->end_pos = end_pos + len + 1;
    if (!end_pos) {
        s->nexttime = timer_read_time();
        sched_wake_task(&s->wake);
    }
}
DECL_COMMAND(command_st7920_burst, "st7920_burst cmd_then_data=%*s");

static void
st7920_report(uint8_t end_pos)
{
    sendf("st7920_position position=%c", end_pos);
}

void
command_st7920_get_position(uint32_t *args)
{
    struct st7920 *s = main_st7920;
    if (!s)
        shutdown("st7920 not configured");
    st7920_report(s->end_pos);
}
DECL_COMMAND(command_st7920_get_position, "st7920_get_position");

void
st7920_task(void)
{
    struct st7920 *s = main_st7920;
    if (!s || !sched_check_wake(&s->wake))
        // Not active
        return;
    if (timer_is_before(timer_read_time(), s->nexttime)) {
        // Need to wait longer before writing next byte
        sched_wake_task(&s->wake);
        return;
    }
    uint32_t xmit;
    if (s->cur_pos == s->next_pos) {
        // Move buffer to make space for new data at end of buffer
        uint8_t count = s->end_pos - s->next_pos;
        if (s->next_pos && count)
            memmove(s->buffer, &s->buffer[s->next_pos], count);
        s->end_pos = count;
        st7920_report(s->end_pos);
        if (!count) {
            // No more bytes to write
            s->cur_pos = s->next_pos = 0;
            return;
        }
        s->next_pos = s->buffer[0] + 1;
        s->cur_pos = 2;
        // Send command byte
        uint8_t cmd = s->buffer[1];
        xmit = SYNC_CMD | (unsigned)((cmd & 0xf0) << 8) | (uint8_t)(cmd << 4);
    } else {
        // Send data byte
        uint8_t data = s->buffer[s->cur_pos++];
        xmit = SYNC_DATA | (unsigned)((data & 0xf0) << 8) | (uint8_t)(data << 4);
    }
    // XXX - ideally xmit would be done prior to bookkeeping..
    st7920_xmit(s, xmit);
    sched_wake_task(&s->wake);
}
DECL_TASK(st7920_task);

void
st7920_shutdown(void)
{
    struct st7920 *s = main_st7920;
    if (!s)
        return;
    gpio_out_write(s->sclk, 0);
    gpio_out_write(s->sid, 0);
    s->cur_pos = s->next_pos = s->end_pos = 0;
}
DECL_SHUTDOWN(st7920_shutdown);
