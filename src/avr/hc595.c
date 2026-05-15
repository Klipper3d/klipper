// SNx4HC595 shift register support for AVR
//
// Copyright (C) 2018 Trevor Jones <trevorjones141@gmail.com>
// Copyright (C) 2026
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_HC595_LENGTH
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_setup
#include "command.h" // DECL_COMMAND
#include "compiler.h" // ARRAY_SIZE
#include "hc595.h"

struct hc595 *hc595_sr;

void
command_config_hc595(uint32_t *args)
{
    hc595_sr = oid_alloc(args[0], command_config_hc595, sizeof(*hc595_sr));
    hc595_sr->data_pin = gpio_out_setup(args[1], 0);
    hc595_sr->latch_pin = gpio_out_setup(args[2], 1);
    hc595_sr->clock_pin = gpio_out_setup(args[3], 0);
}
DECL_COMMAND(command_config_hc595,
             "config_hc595 oid=%c data_pin=%c latch_pin=%c clock_pin=%c");

void
hc595_set_bit(uint8_t bit, uint8_t value)
{
    uint8_t ic = bit / 8;
    uint8_t ic_bit = bit % 8;
    uint8_t mask = 1 << ic_bit;
    if (value)
        hc595_sr->val[ic] |= mask;
    else
        hc595_sr->val[ic] &= ~mask;
    hc595_flush();
}

void
hc595_toggle_bit(uint8_t bit)
{
    uint8_t ic = bit / 8;
    uint8_t ic_bit = bit % 8;
    uint8_t mask = 1 << ic_bit;
    hc595_sr->val[ic] ^= mask;
    hc595_flush();
}

void
hc595_flush(void)
{
    // Latch low (prepare for data)
    gpio_out_write(hc595_sr->latch_pin, 0);
    // Shift out each bit, MSB first, last IC first (daisy chain order)
    for (int ic = ARRAY_SIZE(hc595_sr->val) - 1; ic >= 0; --ic) {
        for (int bit = 7; bit >= 0; --bit) {
            uint8_t mask = 1 << bit;
            gpio_out_write(hc595_sr->data_pin,
                           (hc595_sr->val[ic] & mask) ? 1 : 0);
            // Clock pulse: rising edge shifts data in
            gpio_out_write(hc595_sr->clock_pin, 1);
            gpio_out_write(hc595_sr->clock_pin, 0);
        }
    }
    // Latch high (transfer shift register to output register)
    gpio_out_write(hc595_sr->latch_pin, 1);
}

// Host command to update entire shift register state (for LEDs etc.)
void
command_hc595_update(uint32_t *args)
{
    uint8_t oid = args[0];
    struct hc595 *sr = oid_lookup(oid, command_config_hc595);
    uint8_t data_len = args[1];
    uint8_t *data = command_decode_ptr(args[2]);
    uint8_t len = data_len < CONFIG_HC595_LENGTH
                  ? data_len : CONFIG_HC595_LENGTH;
    for (uint8_t i = 0; i < len; i++)
        sr->val[i] = data[i];
    hc595_flush();
}
DECL_COMMAND(command_hc595_update, "hc595_update oid=%c data=%*s");
