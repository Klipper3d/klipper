// Commands for sending messages to a 4-bit hd44780_sr lcd driver
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN

struct hd44780_sr {
    uint32_t last_cmd_time, cmd_wait_ticks;
    uint8_t last;
    struct gpio_out sclk, strobe, data;
};


/****************************************************************
 * Transmit functions
 ****************************************************************/

// Write 4 bits to the hd44780_sr using the 4bit parallel interface
static __always_inline void
hd44780_sr_xmit_bits(uint8_t toggle, struct gpio_out sclk_pin, struct gpio_out data_pin
                  , struct gpio_out strobe_pin)
{
    for (int8_t i = 7; i >= 0; i--) {
        gpio_out_write(sclk_pin, 0);
        uint8_t data = (toggle >> i) & 0x01 ? 1 : 0;

        gpio_out_write(data_pin, data);
        gpio_out_write(sclk_pin, 1);
    }

    gpio_out_write(strobe_pin, 1);
    gpio_out_write(strobe_pin, 0);
}

// Transmit 8 bits to the chip
static void
hd44780_sr_xmit_byte(struct hd44780_sr *h, uint8_t data)
{
    struct gpio_out sclk_pin = h->sclk, strobe_pin = h->strobe, data_pin = h->data;
    hd44780_sr_xmit_bits(h->last ^ data, sclk_pin, data_pin, strobe_pin);
    h->last = data << 4;
    hd44780_sr_xmit_bits(data ^ h->last, sclk_pin, data_pin, strobe_pin);
}

// Transmit a series of bytes to the chip
static void
hd44780_sr_xmit(struct hd44780_sr *h, uint8_t len, uint8_t *data)
{
    uint32_t last_cmd_time=h->last_cmd_time, cmd_wait_ticks=h->cmd_wait_ticks;
    while (len--) {
        uint8_t b = *data++;
        while (timer_read_time() - last_cmd_time < cmd_wait_ticks)
            irq_poll();
        hd44780_sr_xmit_byte(h, b);
        last_cmd_time = timer_read_time();
    }
    h->last_cmd_time = last_cmd_time;
}


/****************************************************************
 * Interface
 ****************************************************************/

void
command_config_hd44780_sr(uint32_t *args)
{
    struct hd44780_sr *h = oid_alloc(args[0], command_config_hd44780_sr, sizeof(*h));
    h->sclk = gpio_out_setup(args[1], 0);
    h->data = gpio_out_setup(args[2], 0);
    h->strobe = gpio_out_setup(args[3], 0);

    // Calibrate cmd_wait_ticks
    irq_disable();
    uint32_t start = timer_read_time();
    hd44780_sr_xmit_byte(h, 0);
    uint32_t end = timer_read_time();
    irq_enable();
    uint32_t diff = end - start, delay_ticks = args[4];
    if (delay_ticks > diff)
        h->cmd_wait_ticks = delay_ticks - diff;
}
DECL_COMMAND(command_config_hd44780_sr,
             "config_hd44780_sr oid=%c sclk_pin=%u data_pin=%u"
             " strobe_pin=%u delay_ticks=%u");

void
command_hd44780_sr_send_cmds(uint32_t *args)
{
    struct hd44780_sr *h = oid_lookup(args[0], command_config_hd44780_sr);
    uint8_t len = args[1], *cmds = (void*)(size_t)args[2];
    hd44780_sr_xmit(h, len, cmds);
}
DECL_COMMAND(command_hd44780_sr_send_cmds, "hd44780_sr_send_cmds oid=%c cmds=%*s");

void
command_hd44780_sr_send_data(uint32_t *args)
{
    struct hd44780_sr *h = oid_lookup(args[0], command_config_hd44780_sr);
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    hd44780_sr_xmit(h, len, data);
}
DECL_COMMAND(command_hd44780_sr_send_data, "hd44780_sr_send_data oid=%c data=%*s");

void
hd44780_sr_shutdown(void)
{
    uint8_t i;
    struct hd44780_sr *h;
    foreach_oid(i, h, command_config_hd44780_sr) {
        gpio_out_write(h->sclk, 0);
        gpio_out_write(h->data, 0);
        gpio_out_write(h->strobe, 0);
        h->last = 0;
    }
}
DECL_SHUTDOWN(hd44780_sr_shutdown);
