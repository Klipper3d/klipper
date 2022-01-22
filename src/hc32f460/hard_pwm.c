// Hardware PWM support on HC32F460
//
// Copyright (C) 2022  Steven Gotthardt
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/irq.h"  // irq_save
#include "command.h"    // shutdown
#include "board/gpio.h" // gpio_pwm_write
#include "internal.h"   // GPIO
#include "sched.h"      // sched_shutdown

#include "hc32_ddl.h"


#define MAX_PWM 255
DECL_CONSTANT("PWM_MAX", MAX_PWM);

// timer 6 has PWM
struct gpio_pwm_info {
    M4_TMR6_TypeDef* timer;
    uint8_t pin, channel, function;
};

static const struct gpio_pwm_info pwm_regs[] = {

};


struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    return (struct gpio_pwm){.reg=0};
}


void
gpio_pwm_write(struct gpio_pwm g, uint32_t val)
{
    return;
}
