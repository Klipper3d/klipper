/*
 *  GPIO functions on STM32F042 boards.
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include "board/gpio.h" // gpio_out_setup
#include "stm32f0xx_hal.h"
#include "internal.h" // GPIO
#include "board/irq.h" // irq_save
#include "compiler.h" // ARRAY_SIZE
#include "sched.h" // sched_shutdown
#include "command.h" // shutdown

DECL_ENUMERATION_RANGE("pin", "PA0", GPIO('A', 0), 11);
DECL_ENUMERATION_RANGE("pin", "PB0", GPIO('B', 0), 9);
DECL_ENUMERATION_RANGE("pin", "PF0", GPIO('F', 0), 2);

GPIO_TypeDef *const digital_regs[] = {
    GPIOA, GPIOB, GPIOC, 0, 0, GPIOF
};

// <port:4><pin:4>
uint8_t const avail_pins[] = {
    0x0 + 0x0, //PA0
    0x0 + 0x1, //PA1
#if !(CONFIG_SERIAL)
    0x0 + 0x2, //PA2 - USART pins
    0x0 + 0x3, //PA3
#endif
    0x0 + 0x4, //PA4
    0x0 + 0x5, //PA5
    0x0 + 0x6, //PA6
    0x0 + 0x7, //PA7
#if !(CONFIG_CANSERIAL)
    0x0 + 0x9, //PA9 - but remapped in CAN mode to PA11,PA12
    0x0 + 0xa, //PA10
#endif
    0x10 + 0x1, //PB1
    0x10 + 0x8, //PB8
    0x50 + 0x0, //PF0
    0x50 + 0x1, //PF1
};

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

static uint8_t
gpio_check_pin(uint8_t pin)
{
    int i;
    for(i=0; i<ARRAY_SIZE(avail_pins); i++) {
        if (avail_pins[i] == pin)
            return i;
    }
    return 0xFF;
}

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    struct gpio_out g = { .pin=pin };
    if (gpio_check_pin(pin) < 0xFF) {
        gpio_out_reset(g, val);
    } else {
        shutdown("Not an output pin");
    }
    return g;
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = 1 << (g.pin % 16);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(digital_regs[GPIO2PORT(g.pin)], &GPIO_InitStruct);
    HAL_GPIO_WritePin(digital_regs[GPIO2PORT(g.pin)], 1 << (g.pin % 16), val);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    HAL_GPIO_WritePin(digital_regs[GPIO2PORT(g.pin)], 1 << (g.pin % 16), val);
}

struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    struct gpio_in g = { .pin=pin };
    if (gpio_check_pin(pin) < 0xFF) {
        gpio_in_reset(g, pull_up);
    } else {
        shutdown("Not an input pin");
    }
    return g;
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    irqstatus_t flag = irq_save();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = 1 << (g.pin % 16);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = (pull_up)?GPIO_PULLUP:GPIO_NOPULL;
    HAL_GPIO_Init(digital_regs[GPIO2PORT(g.pin)], &GPIO_InitStruct);
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return HAL_GPIO_ReadPin(digital_regs[GPIO2PORT(g.pin)], 1 << (g.pin % 16));
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    HAL_GPIO_TogglePin(digital_regs[GPIO2PORT(g.pin)], 1 << (g.pin % 16));
}

void
gpio_out_toggle(struct gpio_out g)
{
    irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    irq_restore(flag);
}

void gpio_init(void)
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
}
