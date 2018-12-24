#ifndef __HC595_H
#define __HC595_H

#include "autoconf.h" // CONFIG_HC595_LENGTH
#include <stdint.h> // uint8_t
#include "gpio.h" // gpio_out

struct hc595
{
	struct gpio_out data_pin, latch_pin, clock_pin;
	uint8_t val[CONFIG_HC595_LENGTH];
	struct hc595_pin* outputs[CONFIG_HC595_LENGTH*8];
};

struct hc595_pin
{
	uint8_t oid, ic, bit;
	struct hc595* shift_reg;
};

void hc595_clear(struct hc595 *shift_reg);
void hc595_write_bits(struct hc595 *shift_reg);
void hc595_send_bits_to_host(struct hc595 *shift_reg);

#endif //__HC595_H
