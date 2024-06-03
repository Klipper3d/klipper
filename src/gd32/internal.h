#ifndef __GD32F30X_INTERNAL_H
#define __GD32F30X_INTERNAL_H

#include "autoconf.h" //CONFIG_MACH_GD32F30X

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#if CONFIG_MACH_GD32F30X
#include "gd32f30x.h"
#elif CONFIG_MACH_GD32E23X
#include "gd32e23x.h"
#endif

#define GPIO_OSPEED_SETTING		GPIO_OSPEED_10MHZ
#define AHB_FREQ				(CONFIG_CLOCK_FREQ) 		
#define APB1_FREQ				(CONFIG_CLOCK_FREQ >> 1)
#define APB2_FREQ				(CONFIG_CLOCK_FREQ >> 1)

#if CONFIG_MACH_GD32F303XX
#define SYSTEM_CLOCK_PLL_HXTAL	120000000
#define APB2_ADC_FREQ			(APB2_FREQ >> 1)
#elif CONFIG_MACH_GD32F305XX

#elif CONFIG_MACH_GD32E23X
#define SYSTEM_CLOCK_PLL_HXTAL	72000000 
#define APB2_ADC_FREQ			(14000000)
#else
#error "check src/gd32/Kconfig"
#endif

#define GPIO(PORT,NUM)	(((PORT) - 'A') * 16 + (NUM))
#define GPIO2PORT(PIN)	((PIN) / 16)
#define GPIO2BIT(PIN)	(1 << ((PIN) % 16 ))


uint32_t get_pclock_frequency(uint32_t periph_base);
uint32_t is_enable_pclock(uint32_t pclk);
void enable_pclock(uint32_t pclk);

#if CONFIG_MACH_GD32F30X
void gpio_peripheral(uint32_t gpio, uint32_t otype, uint32_t pull_up);
#elif CONFIG_MACH_GD32E23X
void gpio_init_af_set(uint32_t gpio, uint32_t alt_func_num);
void gpio_init_mode_set(uint32_t gpio, uint32_t mode, uint32_t pull_up_down);
void gpio_init_output_options_set(uint32_t gpio, uint8_t otype);
#endif

#endif
