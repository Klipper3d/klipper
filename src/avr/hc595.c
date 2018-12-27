// Generic HC595 Shift Register Support
//
// Copyright (C) 2016-2018  Trevor Jones <trevorjones141@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <basecmd.h>
#include "hc595.h"
#include "command.h" // DECL_COMMAND
#include "compiler.h" // ARRAY_SIZE

void
command_config_hc595(uint32_t *args)
{
    shift_reg = oid_alloc(args[0], command_config_hc595, sizeof(*shift_reg));
    shift_reg->data_pin = gpio_out_setup(args[1], 0);
    shift_reg->latch_pin = gpio_out_setup(args[2], 1);
    shift_reg->clock_pin = gpio_out_setup(args[3], 0);
}
DECL_COMMAND(command_config_hc595, "config_hc595 oid=%c data_pin=%c latch_pin=%c clock_pin=%c");

void
hc595_set_bit(uint8_t bit, uint8_t value)
{
    uint8_t ic = (uint8_t) (bit / 8);
    uint8_t ic_bit = (uint8_t) (bit - ic * 8);
    uint8_t mask = (uint8_t) (1 << ic_bit);
    shift_reg->val[ic] &= ~mask;
    shift_reg->val[ic] |= value << ic_bit;
    hc595_flush();
}

void
hc595_toggle_bit(uint8_t bit)
{
    uint8_t ic = (uint8_t) (bit / 8);
    uint8_t ic_bit = (uint8_t) (bit - ic * 8);
    uint8_t mask = (uint8_t) (1 << ic_bit);
    uint8_t value = (uint8_t) ((shift_reg->val[ic] & mask) == mask ? 0 : 1);
    shift_reg->val[ic] &= ~mask;
    shift_reg->val[ic] |= value << ic_bit;
    hc595_flush();
}

void
hc595_flush()
{
    //latch off, noop
    gpio_out_toggle(shift_reg->latch_pin);
    //for each bit in reverse
    for (int ic = ARRAY_SIZE(shift_reg->val) - 1; ic >= 0; --ic)
    {
        for (int bit = 7; bit >= 0; --bit)
        {
            //set data
            uint8_t mask = 1 << bit;
            gpio_out_write(shift_reg->data_pin, (shift_reg->val[ic] & mask) == mask);
            //clock pulse
            gpio_out_toggle(shift_reg->clock_pin);
            gpio_out_toggle(shift_reg->clock_pin);
        }
    }
    //latch on
    gpio_out_toggle(shift_reg->latch_pin);
}
