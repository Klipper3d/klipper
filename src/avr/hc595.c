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
	struct hc595 *shift_reg = oid_alloc(args[0], command_config_hc595, sizeof(*shift_reg));
	shift_reg->data_pin = gpio_out_setup(args[1], 0);
	shift_reg->latch_pin = gpio_out_setup(args[2], 1);
	shift_reg->clock_pin = gpio_out_setup(args[3], 0);
}
DECL_COMMAND(command_config_hc595, "config_hc595 oid=%c data_pin=%c latch_pin=%c clock_pin=%c");


void
command_hc595_clear(uint32_t *args)
{
	struct hc595 *shift_reg = oid_lookup(args[0], command_config_hc595);
	hc595_clear(shift_reg);
}
DECL_COMMAND(command_hc595_clear, "hc595_clear oid=%c");

void
hc595_clear(struct hc595 *shift_reg)
{
	gpio_out_toggle(shift_reg->latch_pin);
	gpio_out_write(shift_reg->data_pin, 0);
	for (int i = 0; i < 8 * ARRAY_SIZE(shift_reg->val); ++i)
	{
		gpio_out_toggle(shift_reg->clock_pin);
		gpio_out_toggle(shift_reg->clock_pin);
	}
	gpio_out_toggle(shift_reg->latch_pin);
}

void
command_hc595_write_bits(uint32_t *args)
{
	struct hc595 *sr = oid_lookup(args[0], command_config_hc595);
	for (int ic = 0; ic < ARRAY_SIZE(sr->val); ++ic)
	{
		sr->val[ic] = args[1] >> (ic * 8);
	}
	hc595_write_bits(sr);
}
DECL_COMMAND(command_hc595_write_bits, "hc595_write_bits oid=%c value=%u");

void hc595_write_bits(struct hc595 *shift_reg)
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
	hc595_send_bits_to_host(shift_reg);
}

void
command_hc595_query_bits(uint32_t *args)
{
	struct hc595 *shift_reg = oid_lookup(args[0], command_config_hc595);
	hc595_send_bits_to_host(shift_reg);
}
DECL_COMMAND(command_hc595_query_bits, "hc595_query_bits oid=%c");

void
hc595_send_bits_to_host(struct hc595 *shift_reg)
{
	uint32_t value = 0;
	for (int ic = 0; ic < ARRAY_SIZE(shift_reg->val); ++ic)
	{
		value |= shift_reg->val[ic] << ( ic * 8);
	}
	sendf("hc595_bits value=%u", value);
}