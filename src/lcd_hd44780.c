// Commands for sending messages to a 4-bit hd44780 lcd driver
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK

struct hd44780 {
    uint32_t last_cmd_time;
    uint8_t last;
    struct gpio_out rs, e, d4, d5, d6, d7;
};


/****************************************************************
 * Transmit functions
 ****************************************************************/

#define CMD_WAIT_TICKS timer_from_us(37)

// Write 4 bits to the hd44780 using the 4bit parallel interface
static __always_inline void
hd44780_xmit_bits(uint8_t last, uint8_t data, struct gpio_out e
                  , struct gpio_out d4, struct gpio_out d5
                  , struct gpio_out d6, struct gpio_out d7)
{
    uint8_t toggle = data ^ last;
    gpio_out_toggle(e);
    if (toggle & 0x01)
        gpio_out_toggle(d4);
    if (toggle & 0x02)
        gpio_out_toggle(d5);
    if (toggle & 0x04)
        gpio_out_toggle(d6);
    if (toggle & 0x08)
        gpio_out_toggle(d7);
    gpio_out_toggle(e);
}

static void
hd44780_xmit(struct hd44780 *h, uint8_t len, uint8_t *data)
{
    uint8_t last = h->last;
    struct gpio_out e = h->e, d4 = h->d4, d5 = h->d5, d6 = h->d6, d7 = h->d7;
    while (len--) {
        uint8_t b = *data++;
        while (timer_read_time() - h->last_cmd_time < CMD_WAIT_TICKS)
            ;
        hd44780_xmit_bits(last, b >> 4, e, d4, d5, d6, d7);
        hd44780_xmit_bits(b >> 4, b, e, d4, d5, d6, d7);
        h->last_cmd_time = timer_read_time();
        last = b;
    }
    h->last = last;
}


/****************************************************************
 * Interface
 ****************************************************************/

void
command_config_hd44780(uint32_t *args)
{
    struct hd44780 *h = oid_alloc(args[0], command_config_hd44780, sizeof(*h));
    h->rs = gpio_out_setup(args[1], 0);
    h->e = gpio_out_setup(args[2], 0);
    h->d4 = gpio_out_setup(args[3], 0);
    h->d5 = gpio_out_setup(args[4], 0);
    h->d6 = gpio_out_setup(args[5], 0);
    h->d7 = gpio_out_setup(args[6], 0);
}
DECL_COMMAND(command_config_hd44780,
             "config_hd44780 oid=%c rs_pin=%u e_pin=%u"
             " d4_pin=%u d5_pin=%u d6_pin=%u d7_pin=%u");

void
command_hd44780_send_cmds(uint32_t *args)
{
    struct hd44780 *h = oid_lookup(args[0], command_config_hd44780);
    gpio_out_write(h->rs, 0);
    uint8_t len = args[1], *cmds = (void*)(size_t)args[2];
    hd44780_xmit(h, len, cmds);
}
DECL_COMMAND(command_hd44780_send_cmds, "hd44780_send_cmds oid=%c cmds=%*s");

void
command_hd44780_send_data(uint32_t *args)
{
    struct hd44780 *h = oid_lookup(args[0], command_config_hd44780);
    gpio_out_write(h->rs, 1);
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    hd44780_xmit(h, len, data);
}
DECL_COMMAND(command_hd44780_send_data, "hd44780_send_data oid=%c data=%*s");

void
hd44780_shutdown(void)
{
    uint8_t i;
    struct hd44780 *h;
    foreach_oid(i, h, command_config_hd44780) {
        gpio_out_write(h->rs, 0);
        gpio_out_write(h->e, 0);
        gpio_out_write(h->d4, 0);
        gpio_out_write(h->d5, 0);
        gpio_out_write(h->d6, 0);
        gpio_out_write(h->d7, 0);
        h->last = 0;
    }
}
DECL_SHUTDOWN(hd44780_shutdown);
