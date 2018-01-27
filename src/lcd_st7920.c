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
    uint32_t next_cmd_time;
    uint8_t is_busy;
    struct gpio_out sclk, sid;
};

static struct st7920 *main_st7920;


/****************************************************************
 * Transmit functions
 ****************************************************************/

#define SYNC_CMD  0xf8
#define SYNC_DATA 0xfa

#define CMD_WAIT_TICKS timer_from_us(72)

static inline void
st7920_xmit_byte(struct gpio_out sclk, struct gpio_out sid, uint8_t data)
{
    uint8_t bits = 8, last_b = 0;
    for (;;) {
        if ((data ^ last_b) & 0x80)
            gpio_out_toggle(sid);
        gpio_out_toggle(sclk);
        last_b = data;
        data <<= 1;
        if (!--bits)
            break;
        gpio_out_toggle(sclk);
    }
    gpio_out_toggle(sclk);
}

static void
st7920_xmit_data(struct st7920 *s, uint8_t count, uint8_t *data)
{
    struct gpio_out sclk = s->sclk, sid = s->sid;
    st7920_xmit_byte(sclk, sid, SYNC_DATA);
    while (count--) {
        uint8_t b = *data++;
        st7920_xmit_byte(sclk, sid, b & 0xf0);
        st7920_xmit_byte(sclk, sid, b << 4);
    }
}

static void
st7920_xmit_cmds(struct st7920 *s, uint8_t count, uint8_t *cmds)
{
    struct gpio_out sclk = s->sclk, sid = s->sid;
    while (count--) {
        uint8_t cmd = *cmds++;
        if (s->is_busy)
            while (timer_is_before(timer_read_time(), s->next_cmd_time))
                ;
        st7920_xmit_byte(sclk, sid, SYNC_CMD);
        st7920_xmit_byte(sclk, sid, cmd & 0xf0);
        st7920_xmit_byte(sclk, sid, cmd << 4);
        s->next_cmd_time = timer_read_time() + CMD_WAIT_TICKS;
        s->is_busy = 1;
    }
}


/****************************************************************
 * Interface
 ****************************************************************/

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
command_st7920_send_cmds(uint32_t *args)
{
    struct st7920 *s = main_st7920;
    if (!s)
        shutdown("st7920 not configured");
    uint8_t len = args[0];
    uint8_t *cmds = (void*)(size_t)args[1];
    st7920_xmit_cmds(s, len, cmds);
}
DECL_COMMAND(command_st7920_send_cmds, "st7920_send_cmds cmds=%*s");

void
command_st7920_send_data(uint32_t *args)
{
    struct st7920 *s = main_st7920;
    if (!s)
        shutdown("st7920 not configured");
    uint8_t len = args[0];
    uint8_t *data = (void*)(size_t)args[1];
    st7920_xmit_data(s, len, data);
}
DECL_COMMAND(command_st7920_send_data, "st7920_send_data data=%*s");

void
st7920_task(void)
{
    struct st7920 *s = main_st7920;
    if (!s || !s->is_busy)
        return;
    // Avoid 32bit rollover on s->next_cmd_time
    if (!timer_is_before(timer_read_time(), s->next_cmd_time))
        s->is_busy = 0;
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
}
DECL_SHUTDOWN(st7920_shutdown);
