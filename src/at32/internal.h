#ifndef __AT32F403A_407_INTERNAL_H
#define __AT32F403A_407_INTERNAL_H

#include "autoconf.h" //CONFIG_MACH_AT32F40X

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#if (CONFIG_MACH_AT32F403A || CONFIG_MACH_AT32F407)
#include "at32f403a_407.h"
#elif CONFIG_MACH_AT32F415
#include "at32f415.h"
#elif CONFIG_MACH_AT32F425
#include "at32f425.h"
#elif CONFIG_MACH_AT32F421
#include "at32f421.h"
#else
#error "check src/at32/Kconfig"
#endif

#define GPIO(PORT,NUM)	(((PORT) - 'A') * 16 + (NUM))
#define GPIO2PORT(PIN)	((PIN) / 16)
#define GPIO2BIT(PIN)	(1 << ((PIN) % 16 ))

#define PCLK_GPIOA	CRM_GPIOA_PERIPH_CLOCK
#define PCLK_GPIOB	CRM_GPIOB_PERIPH_CLOCK
#define PCLK_GPIOC	CRM_GPIOC_PERIPH_CLOCK

#if !defined(AT32F421F6P7)
#define PCLK_GPIOD	CRM_GPIOD_PERIPH_CLOCK
#endif

#if (!defined(AT32F415CCU7) && !defined(AT32F425F8P7) && !defined(AT32F421F6P7))
#define PCLK_GPIOE	CRM_GPIOE_PERIPH_CLOCK
#endif

#define PCLK_GPIOF	CRM_GPIOF_PERIPH_CLOCK

#define PCLK_UART1	CRM_USART1_PERIPH_CLOCK
#define PCLK_ADC1	CRM_ADC1_PERIPH_CLOCK
#define PCLK_TMR1	CRM_TMR1_PERIPH_CLOCK
#define PCLK_I2C3	CRM_I2C3_PERIPH_CLOCK
#define PCLK_SPI1	CRM_SPI1_PERIPH_CLOCK
#define PCLK_USB	CRM_USB_PERIPH_CLOCK

#define GPIO_INIT_STRUCT_DEF	{\
								 .gpio_pins  = GPIO_PINS_ALL,\
								 .gpio_mode = GPIO_MODE_INPUT,\
								 .gpio_out_type = GPIO_OUTPUT_PUSH_PULL,\
								 .gpio_pull = GPIO_PULL_NONE,\
								 .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER\
								}

uint32_t get_pclock_frequency(uint32_t periph_base);
uint32_t is_enable_pclock(uint32_t pclk);
void enable_pclock(uint32_t pclk);
void gpio_peripheral(uint32_t gpio, gpio_init_type* gpio_init_struct, uint32_t pullup);

#if (CONFIG_MACH_AT32F403A || CONFIG_MACH_AT32F407)
void at32f403a_407_gpio_remap(uint32_t gpio_remap, uint8_t new_state);
#endif

#if (CONFIG_MACH_AT32F425 || CONFIG_MACH_AT32F421)
void gpio_mux_cfg(uint32_t gpio, gpio_pins_source_type gpio_pin_source, gpio_mux_sel_type gpio_mux);
#endif

#endif
