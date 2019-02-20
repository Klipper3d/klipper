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

void
delay(uint32_t ticks) {
    uint32_t start = timer_read_time();
    while (timer_read_time() - start < 1)
        irq_poll();
}

/****************************************************************
 * Transmit functions
 ****************************************************************/
void
hd44780_sr_write_serial(uint8_t value, struct hd44780_sr *h, uint8_t dataMode) {
    for (int8_t i = 7; i >= 0; i--) {
        gpio_out_write(h->sclk, 0);
        uint8_t data = (value >> i) & 0x01 ? 1 : 0;
        gpio_out_write(h->data, data);

        gpio_out_write(h->sclk, 1);
        delay(h->cmd_wait_ticks);
    }

    gpio_out_write(h->strobe, 1);
    delay(h->cmd_wait_ticks);
    gpio_out_write(h->strobe, 0);
}

void
hd44780_sr_pulse_enable(uint8_t value, struct hd44780_sr *h, uint8_t dataMode) {
    delay(h->cmd_wait_ticks);
    // set enable to true, on standard hardware it is 0b1000
    value |= 0b01000;
    hd44780_sr_write_serial(value, h, dataMode);
    delay(h->cmd_wait_ticks); // enable pulse must be >450ns
    // set enable to false
    value &= 0b11110111;
    hd44780_sr_write_serial(value, h, dataMode);
    delay(h->cmd_wait_ticks); // commands need > 37us to settle [citation needed]
}

// Write 4 bits to the hd44780_sr using the 4bit parallel interface
static void
hd44780_sr_xmit_bits(uint8_t value, struct hd44780_sr *h, uint8_t dataMode)
{
    // On the standard hardware, the top 4 bits are the data lines
    uint8_t bits = value << 4;

    // Is it a command or data (register select)
    // On standard hardware, the register select is 0b0010
    if (dataMode)
        bits |= 0b0010;

    // Send the data
    hd44780_sr_pulse_enable(bits, h, dataMode);
}

// Transmit 8 bits to the chip
static void
hd44780_sr_xmit_byte(struct hd44780_sr *h, uint8_t data, uint8_t dataMode)
{
    hd44780_sr_xmit_bits((data >> 4), h, dataMode);
    hd44780_sr_xmit_bits((data & 0x0F), h, dataMode);
}

// Transmit a series of bytes to the chip
static void
hd44780_sr_xmit(struct hd44780_sr *h, uint8_t len, uint8_t *data, uint8_t dataMode)
{
    uint32_t last_cmd_time=h->last_cmd_time, cmd_wait_ticks=h->cmd_wait_ticks;
    while (len--) {
        uint8_t b = *data++;
        while (timer_read_time() - last_cmd_time < cmd_wait_ticks)
            irq_poll();
        hd44780_sr_xmit_byte(h, b, dataMode);
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
    h->sclk = gpio_out_setup(args[1], 1);
    h->data = gpio_out_setup(args[2], 0);
    h->strobe = gpio_out_setup(args[3], 0);
    h->cmd_wait_ticks = args[4];
}
DECL_COMMAND(command_config_hd44780_sr,
             "config_hd44780_sr oid=%c sclk_pin=%u data_pin=%u"
             " strobe_pin=%u delay_ticks=%u");

void
command_hd44780_sr_send_cmds(uint32_t *args)
{
    struct hd44780_sr *h = oid_lookup(args[0], command_config_hd44780_sr);
    uint8_t len = args[1], *cmds = (void*)(size_t)args[2];
    hd44780_sr_xmit(h, len, cmds, 0);
}
DECL_COMMAND(command_hd44780_sr_send_cmds, "hd44780_sr_send_cmds oid=%c cmds=%*s");

void
command_hd44780_sr_send_data(uint32_t *args)
{
    struct hd44780_sr *h = oid_lookup(args[0], command_config_hd44780_sr);
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    hd44780_sr_xmit(h, len, data, 1);
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
