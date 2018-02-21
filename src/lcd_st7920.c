// Commands for sending messages to an st7920 lcd driver
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_*
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_poll
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK

struct st7920 {
    uint32_t last_cmd_time;
    struct gpio_out sclk, sid;
};


/****************************************************************
 * Transmit functions
 ****************************************************************/

#define SYNC_CMD  0xf8
#define SYNC_DATA 0xfa

#define CMD_WAIT_TICKS timer_from_us(72)

// Delay between SCLK pulses while bit banging to the lcd chip
static void
st7920_bitbang_delay(void)
{
    if (CONFIG_MACH_AVR) {
        // On the AVR, a single NOP should be sufficient
        asm("nop");
        return;
    }
    uint32_t end = timer_read_time() + timer_from_us(1);
    while (!timer_is_before(end, timer_read_time()))
        irq_poll();
}

// Write eight bits to the st7920 via the serial interface
static __always_inline void
st7920_xmit_byte(struct gpio_out sclk, struct gpio_out sid, uint8_t data)
{
    uint8_t bits = 8;
    for (;;) {
        if (data & 0x80) {
            data = ~data;
            gpio_out_toggle(sid);
        }
        gpio_out_toggle(sclk);
        st7920_bitbang_delay();
        data <<= 1;
        if (!--bits)
            break;
        gpio_out_toggle(sclk);
        st7920_bitbang_delay();
    }
    gpio_out_toggle(sclk);
}

// Transmit a series of data bytes to the chip
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

// Transmit a series of command bytes to the chip
static void
st7920_xmit_cmds(struct st7920 *s, uint8_t count, uint8_t *cmds)
{
    struct gpio_out sclk = s->sclk, sid = s->sid;
    st7920_xmit_byte(sclk, sid, SYNC_CMD);
    while (count--) {
        uint8_t cmd = *cmds++;
        st7920_xmit_byte(sclk, sid, cmd & 0xf0);
        // Can't complete transfer until 72us from last command
        while (timer_read_time() - s->last_cmd_time < CMD_WAIT_TICKS)
            irq_poll();
        st7920_xmit_byte(sclk, sid, cmd << 4);
        s->last_cmd_time = timer_read_time();
    }
}


/****************************************************************
 * Interface
 ****************************************************************/

void
command_config_st7920(uint32_t *args)
{
    struct st7920 *s = oid_alloc(args[0], command_config_st7920, sizeof(*s));
    s->sclk = gpio_out_setup(args[1], 0);
    s->sid = gpio_out_setup(args[2], 0);
}
DECL_COMMAND(command_config_st7920,
             "config_st7920 oid=%c sclk_pin=%u sid_pin=%u");

void
command_st7920_send_cmds(uint32_t *args)
{
    struct st7920 *s = oid_lookup(args[0], command_config_st7920);
    uint8_t len = args[1], *cmds = (void*)(size_t)args[2];
    st7920_xmit_cmds(s, len, cmds);
}
DECL_COMMAND(command_st7920_send_cmds, "st7920_send_cmds oid=%c cmds=%*s");

void
command_st7920_send_data(uint32_t *args)
{
    struct st7920 *s = oid_lookup(args[0], command_config_st7920);
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    st7920_xmit_data(s, len, data);
}
DECL_COMMAND(command_st7920_send_data, "st7920_send_data oid=%c data=%*s");

void
st7920_shutdown(void)
{
    uint8_t i;
    struct st7920 *s;
    foreach_oid(i, s, command_config_st7920) {
        gpio_out_write(s->sclk, 0);
        gpio_out_write(s->sid, 0);
    }
}
DECL_SHUTDOWN(st7920_shutdown);
