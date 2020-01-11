// Commands for sending messages to a sailfish lcd driver
// based on https://github.com/mikeshub/SailfishLCD
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

/* LiquidCrystalSerial
 *
 * This is an implementation of communciation routines for the
 * Makerbot OEM display hardware.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "autoconf.h"   // CONFIG_CLOCK_FREQ
#include "basecmd.h"    // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h"  // irq_poll
#include "board/misc.h" // timer_from_us
#include "command.h"    // DECL_COMMAND
#include "sched.h"      // DECL_SHUTDOWN
#include "stdbool.h"

#define LOW 0
#define HIGH 1

struct hd44780_sr
{
    uint32_t last_cmd_time, cmd_wait_ticks;
    struct gpio_out strobe, data, clk;
};

static void
hd44780_sr_pulse_enable(struct hd44780_sr *h, uint8_t value);

static void
hd44780_sr_write_4bits(struct hd44780_sr *h, uint8_t value, bool dataMode);

static void
hd44780_sr_write_serial(struct hd44780_sr *h, uint8_t value);

static void
hd44780_sr_begin(struct hd44780_sr *h);

static void
hd44780_sr_send(struct hd44780_sr *h, uint8_t value, bool mode);

/****************************************************************
 * Transmit functions
 ****************************************************************/

static uint32_t
usecs_to_ticks(uint32_t us)
{
    return timer_from_us(us) / 1000;
}

static inline void
usdelay(uint32_t usecs)
{
    if (CONFIG_CLOCK_FREQ <= 48000000)
        // Slower MCUs don't require a delay
        return;
    uint32_t end = timer_read_time() + usecs_to_ticks(usecs);
    while (timer_is_before(timer_read_time(), end))
        irq_poll();
}

// Initialization of a standard HD44780 display
static void
hd44780_sr_begin(struct hd44780_sr *h)
{
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    usdelay(50000);

    // Now we pull both RS and R/W low to begin commands
    hd44780_sr_write_serial(h, 0b00000000);

    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    hd44780_sr_write_4bits(h, 0x03, false);
    usdelay(4500); // wait min 4.1ms

    // second try
    hd44780_sr_write_4bits(h, 0x03, false);
    usdelay(4500); // wait min 4.1ms

    // third go!
    hd44780_sr_write_4bits(h, 0x03, false);
    usdelay(150);

    // finally, set to 8-bit interface
    hd44780_sr_write_4bits(h, 0x02, false);
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
static void
hd44780_sr_send(struct hd44780_sr *h, uint8_t value, bool mode)
{
    // serial assumes 4 bit mode
    hd44780_sr_write_4bits(h, (value >> 4), mode);
    hd44780_sr_write_4bits(h, (value & 0x0F), mode);
}

static void
hd44780_sr_write_4bits(struct hd44780_sr *h, uint8_t value, bool dataMode)
{
    // On the standard hardware, the top 4 bits are the data lines
    uint8_t bits = value << 4;

    // Is it a command or data (register select)
    // On standard hardware, the register select is 0b0010
    if (dataMode)
        bits |= 0b0010;

    // Send the data
    hd44780_sr_pulse_enable(h, bits);
}

static void
hd44780_sr_pulse_enable(struct hd44780_sr *h, uint8_t value)
{
    usdelay(1);
    // set enable to true, on standard hardware it is 0b1000
    value |= 0b01000;
    hd44780_sr_write_serial(h, value);
    usdelay(1); // enable pulse must be >450ns
    // set enable to false
    value &= 0b11110111;
    hd44780_sr_write_serial(h, value);
    usdelay(1); // commands need > 37us to settle [citation needed]
}

static void
hd44780_sr_write_serial(struct hd44780_sr *h, uint8_t value)
{
    for (int8_t i = 7; i >= 0; i--)
    {
        gpio_out_write(h->clk, LOW);
        bool data = (value >> i) & 0x01 ? true : false;
        gpio_out_write(h->data, data);
        gpio_out_write(h->clk, HIGH);
        usdelay(1);
    }
    gpio_out_write(h->strobe, HIGH);
    usdelay(1);
    gpio_out_write(h->strobe, LOW);
}

/****************************************************************
 * Interface
 ****************************************************************/

void
command_config_hd44780_sr(uint32_t *args)
{
    struct hd44780_sr *h = oid_alloc(args[0], command_config_hd44780_sr, sizeof(*h));
    h->strobe = gpio_out_setup(args[1], 0);
    h->data = gpio_out_setup(args[2], 0);
    h->clk = gpio_out_setup(args[3], 0);

    // Calibrate cmd_wait_ticks
    irq_disable();
    uint32_t start = timer_read_time();
    hd44780_sr_send(h, 0, false);
    uint32_t end = timer_read_time();
    irq_enable();
    uint32_t diff = end - start, delay_ticks = args[4];
    if (delay_ticks > diff)
        h->cmd_wait_ticks = delay_ticks - diff;

    hd44780_sr_begin(h);
}
DECL_COMMAND(command_config_hd44780_sr,
             "config_hd44780_sr oid=%c strobe_pin=%u data_pin=%u"
             " clk_pin=%u delay_ticks=%u");

void
command_hd44780_sr_send_cmds(uint32_t *args)
{
    struct hd44780_sr *h = oid_lookup(args[0], command_config_hd44780_sr);
    uint32_t last_cmd_time=h->last_cmd_time, cmd_wait_ticks=h->cmd_wait_ticks;
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    while (len--) {
        uint8_t b = *data++;
        while (timer_read_time() - last_cmd_time < cmd_wait_ticks)
            irq_poll();
        hd44780_sr_send(h, b, false);
        last_cmd_time = timer_read_time();
    }
}
DECL_COMMAND(command_hd44780_sr_send_cmds, "hd44780_sr_send_cmds oid=%c cmds=%*s");

void
command_hd44780_sr_send_data(uint32_t *args)
{
    struct hd44780_sr *h = oid_lookup(args[0], command_config_hd44780_sr);
    uint32_t last_cmd_time=h->last_cmd_time, cmd_wait_ticks=h->cmd_wait_ticks;
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    while (len--) {
        uint8_t b = *data++;
        while (timer_read_time() - last_cmd_time < cmd_wait_ticks)
            irq_poll();
        hd44780_sr_send(h, b, true);
        last_cmd_time = timer_read_time();
    }
}
DECL_COMMAND(command_hd44780_sr_send_data, "hd44780_sr_send_data oid=%c data=%*s");