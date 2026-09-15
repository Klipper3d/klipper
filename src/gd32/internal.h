// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GD32E23X_INTERNAL_H
#define __GD32E23X_INTERNAL_H

#include "autoconf.h" // CONFIG_CLOCK_FREQ

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "gd32e23x.h"

#define GPIO_OSPEED_SETTING        GPIO_OSPEED_10MHZ
#define AHB_FREQ                (CONFIG_CLOCK_FREQ)

#define APB1_FREQ                (CONFIG_CLOCK_FREQ)
#define APB2_FREQ                (CONFIG_CLOCK_FREQ)
#define SYSTEM_CLOCK_PLL_HXTAL    72000000
#define APB2_ADC_FREQ            (14000000)

#define GPIO(PORT,NUM)    (((PORT) - 'A') * 16 + (NUM))
#define GPIO2PORT(PIN)    ((PIN) / 16)
#define GPIO2BIT(PIN)    (1 << ((PIN) % 16 ))


uint32_t get_pclock_frequency(uint32_t periph_base);
uint32_t is_enable_pclock(uint32_t pclk);
void enable_pclock(uint32_t pclk);

void gpio_init_af_set(uint32_t gpio, uint32_t alt_func_num);
void gpio_init_mode_set(uint32_t gpio, uint32_t mode, uint32_t pull_up_down);
void gpio_init_output_options_set(uint32_t gpio, uint8_t otype);

#endif
