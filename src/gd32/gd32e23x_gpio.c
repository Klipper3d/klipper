// GPIO functions on GD32E23x
//
// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h>
#include "board/irq.h"
#include "command.h"
#include "gpio.h"
#include "internal.h"
#include "sched.h"


#if CONFIG_MACH_GD32E230X6 || CONFIG_MACH_GD32E230X8
DECL_ENUMERATION_RANGE("pin","PA0",GPIO('A',0),16);
DECL_ENUMERATION_RANGE("pin","PB0",GPIO('B',0),16);
DECL_ENUMERATION_RANGE("pin","PC0",GPIO('C',0),16);
DECL_ENUMERATION_RANGE("pin","PF0",GPIO('F',0),16);

uint32_t gpio_port_base[] =
{
        GPIOA,
        GPIOB,
        GPIOC,
        0,
        0,
        GPIOF
};

uint32_t gpio_pclk[] =
{
        RCU_GPIOA,
        RCU_GPIOB,
        RCU_GPIOC,
        0,
        0,
        RCU_GPIOF
};
#endif

static uint32_t
gpio_port(uint8_t pin)
{
    uint32_t index = GPIO2PORT(pin);
    if (index >= ARRAY_SIZE(gpio_port_base) || !gpio_port_base[index])
        shutdown("Invalid GPIO pin");
    return gpio_port_base[index];
}

uint32_t
get_pclock_frequency(uint32_t periph_base)
{
    if(periph_base == 0)
    {
        return AHB_FREQ;
    }
    else
    {
        return APB2_ADC_FREQ;
    }
}


uint32_t
is_enable_pclock(uint32_t pclk)
{
    return RCU_REG_VAL(pclk) & BIT(RCU_BIT_POS(pclk));
}

void
enable_pclock(uint32_t pclk)
{
    RCU_REG_VAL(pclk) |= BIT(RCU_BIT_POS(pclk));
}


void
gpio_init_mode_set(uint32_t gpio, uint32_t mode, uint32_t pull_up_down)
{
    uint16_t i;

    uint32_t ctl, pupd;

    uint32_t pin = GPIO2BIT(gpio);

    uint32_t port = GPIO2PORT(gpio);
    uint32_t gpio_periph = gpio_port(gpio);

    enable_pclock(gpio_pclk[port]);

    ctl = GPIO_CTL(gpio_periph);

    pupd = GPIO_PUD(gpio_periph);

    for(i = 0U;i < 16U;i++)
    {
        if((1U << i) & pin)
        {
            /* clear the specified pin mode bits */
            ctl &= ~GPIO_MODE_MASK(i);
            /* set the specified pin mode bits */
            ctl |= GPIO_MODE_SET(i, mode);

            /* clear the specified pin pupd bits */
            pupd &= ~GPIO_PUPD_MASK(i);
            /* set the specified pin pupd bits */
            pupd |= GPIO_PUPD_SET(i, pull_up_down);
        }
    }

    GPIO_CTL(gpio_periph) = ctl;

    GPIO_PUD(gpio_periph) = pupd;
}

void
gpio_init_output_options_set(uint32_t gpio, uint8_t otype)
{
    uint32_t speed = GPIO_OSPEED_50MHZ;

    uint32_t pin = GPIO2BIT(gpio);

    uint32_t gpio_periph = gpio_port(gpio);

    uint16_t i;

    uint32_t ospeed;

    if(GPIO_OTYPE_OD == otype)
    {
        GPIO_OMODE(gpio_periph) |= (uint32_t)pin;
    }
    else
    {
        GPIO_OMODE(gpio_periph) &= (uint32_t)(~pin);
    }

    /* get the specified pin output speed bits value */
    ospeed = GPIO_OSPD(gpio_periph);

    for(i = 0U;i < 16U;i++)
    {
        if((1U << i) & pin)
        {
            /* clear the specified pin output speed bits */
            ospeed &= ~GPIO_OSPEED_MASK(i);
            /* set the specified pin output speed bits */
            ospeed |= GPIO_OSPEED_SET(i,speed);
        }
    }

    GPIO_OSPD(gpio_periph) = ospeed;
}


void
gpio_init_af_set(uint32_t gpio, uint32_t alt_func_num)
{
    uint16_t i;

    uint32_t afrl, afrh;

    uint32_t gpio_periph = gpio_port(gpio);

    uint32_t pin = GPIO2BIT(gpio);

    afrl = GPIO_AFSEL0(gpio_periph);

    afrh = GPIO_AFSEL1(gpio_periph);

    for(i = 0U;i < 8U;i++)
    {
        if((1U << i) & pin)
        {
            /* clear the specified pin alternate function bits */
            afrl &= ~GPIO_AFR_MASK(i);

            afrl |= GPIO_AFR_SET(i,alt_func_num);
        }
    }

    for(i = 8U;i < 16U;i++)
    {
        if((1U << i) & pin)
        {
            /* clear the specified pin alternate function bits */
            afrh &= ~GPIO_AFR_MASK(i - 8U);

            afrh |= GPIO_AFR_SET(i - 8U,alt_func_num);
        }
    }

    GPIO_AFSEL0(gpio_periph) = afrl;

    GPIO_AFSEL1(gpio_periph) = afrh;
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    uint32_t port = GPIO2PORT(g.pin);

    uint32_t pin = GPIO2BIT(g.pin);

    irqstatus_t flag = irq_save();

    // Preload the latch before switching the pin to output mode.
    if(!val)
    {
        GPIO_BC(gpio_port_base[port]) = (uint32_t)pin;
    }
    else
    {
        GPIO_BOP(gpio_port_base[port]) = (uint32_t)pin;
    }
    gpio_init_output_options_set(g.pin, GPIO_OTYPE_PP);
    gpio_init_mode_set(g.pin, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);

    irq_restore(flag);

    return;
}

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    (void)gpio_port(pin);

    struct gpio_out g = {.pin = pin};

    gpio_out_reset(g,val);

    return g;
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    uint32_t port = GPIO2PORT(g.pin);

    uint32_t pin = GPIO2BIT(g.pin);

     GPIO_TG(gpio_port_base[port]) = (uint32_t)pin;

    return;
}

void
gpio_out_toggle(struct gpio_out g)
{
    irqstatus_t flag = irq_save();

    gpio_out_toggle_noirq(g);

    irq_restore(flag);

    return;
}

uint8_t
gpio_out_read(uint8_t gpioIndx)
{
    uint32_t port = GPIO2PORT(gpioIndx);

    uint32_t pin = GPIO2BIT(gpioIndx);

    return !!(GPIO_OCTL(gpio_port_base[port]) & pin);
}


void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    uint32_t port = GPIO2PORT(g.pin);

    uint32_t pin = GPIO2BIT(g.pin);

    if(!val)
    {
        GPIO_BC(gpio_port_base[port]) = (uint32_t)pin;
    }
    else
    {
        GPIO_BOP(gpio_port_base[port]) = (uint32_t)pin;
    }

    return;
}

struct gpio_in
gpio_in_setup(uint8_t pin, uint8_t pull_up)
{

    (void)gpio_port(pin);

    struct gpio_in g = {.pin = pin};

    gpio_in_reset(g,pull_up);

    return g;
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    uint32_t pp_pd = GPIO_PUPD_NONE;
    if (pull_up > 0)
        pp_pd = GPIO_PUPD_PULLUP;
    else if (pull_up == 0)
        pp_pd = GPIO_PUPD_PULLDOWN;

    irqstatus_t flag = irq_save();

    gpio_init_mode_set(g.pin,GPIO_MODE_INPUT, pp_pd);

    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    uint32_t port = GPIO2PORT(g.pin);

    uint32_t pin = GPIO2BIT(g.pin);

    return (!!(GPIO_ISTAT(gpio_port_base[port]) & pin));
}
