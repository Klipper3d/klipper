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
    uint32_t next_cmd_time;
    uint8_t is_busy, last;
    struct gpio_out rs, e, d4, d5, d6, d7;
};

static struct hd44780 *main_hd44780;

#define CMD_WAIT_TICKS timer_from_us(37)

// Write 4 bits to the hd44780 using the 4bit parallel interface
static inline void
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
        if (h->is_busy)
            while (timer_is_before(timer_read_time(), h->next_cmd_time))
                ;
        hd44780_xmit_bits(last, b >> 4, e, d4, d5, d6, d7);
        last = b >> 4;
        hd44780_xmit_bits(last, b, e, d4, d5, d6, d7);
        h->next_cmd_time = timer_read_time() + CMD_WAIT_TICKS;
        last = b;
        h->is_busy = 1;
    }
    h->last = last;
}

void
command_config_hd44780(uint32_t *args)
{
    if (main_hd44780)
        shutdown("hd44780 already configured");
    struct hd44780 *h = oid_alloc(args[0], command_config_hd44780, sizeof(*h));
    h->rs = gpio_out_setup(args[1], 0);
    h->e = gpio_out_setup(args[2], 0);
    h->d4 = gpio_out_setup(args[3], 0);
    h->d5 = gpio_out_setup(args[4], 0);
    h->d6 = gpio_out_setup(args[5], 0);
    h->d7 = gpio_out_setup(args[6], 0);
    main_hd44780 = h;
}
DECL_COMMAND(command_config_hd44780,
             "config_hd44780 oid=%c rs_pin=%u e_pin=%u"
             " d4_pin=%u d5_pin=%u d6_pin=%u d7_pin=%u");

void
command_hd44780_send_cmds(uint32_t *args)
{
    struct hd44780 *h = main_hd44780;
    if (!h)
        shutdown("hd44780 not configured");
    gpio_out_write(h->rs, 0);
    uint8_t len = args[0], *cmds = (void*)(size_t)args[1];
    hd44780_xmit(h, len, cmds);
}
DECL_COMMAND(command_hd44780_send_cmds, "hd44780_send_cmds cmds=%*s");

void
command_hd44780_send_data(uint32_t *args)
{
    struct hd44780 *h = main_hd44780;
    if (!h)
        shutdown("hd44780 not configured");
    gpio_out_write(h->rs, 1);
    uint8_t len = args[0], *data = (void*)(size_t)args[1];
    hd44780_xmit(h, len, data);
}
DECL_COMMAND(command_hd44780_send_data, "hd44780_send_data data=%*s");

void
hd44780_task(void)
{
    struct hd44780 *h = main_hd44780;
    if (!h || !h->is_busy)
        return;
    // Avoid 32bit rollover on h->next_cmd_time
    if (!timer_is_before(timer_read_time(), h->next_cmd_time))
        h->is_busy = 0;
}
DECL_TASK(hd44780_task);

void
hd44780_shutdown(void)
{
    struct hd44780 *h = main_hd44780;
    if (!h)
        return;
    gpio_out_write(h->rs, 0);
    gpio_out_write(h->e, 0);
    gpio_out_write(h->d4, 0);
    gpio_out_write(h->d5, 0);
    gpio_out_write(h->d6, 0);
    gpio_out_write(h->d7, 0);
    h->last = 0;
}
DECL_SHUTDOWN(hd44780_shutdown);
