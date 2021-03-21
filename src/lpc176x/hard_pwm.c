// Hardware PWM support on lpc176x
//
// Copyright (C) 2021  Michael Kurz <michi.kurz@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

#define MAX_PWM 255
DECL_CONSTANT("PWM_MAX", MAX_PWM);

struct gpio_pwm_info {
    uint8_t pin, channel, function;
    void *reg;
};

static const struct gpio_pwm_info pwm_regs[] = {
    {GPIO(1, 18), 1, 2, (void*)&LPC_PWM1->MR1},
    {GPIO(2,  0), 1, 1, (void*)&LPC_PWM1->MR1},
    {GPIO(1, 20), 2, 2, (void*)&LPC_PWM1->MR2},
    {GPIO(2,  1), 2, 1, (void*)&LPC_PWM1->MR2},
    {GPIO(3, 25), 2, 3, (void*)&LPC_PWM1->MR2},
    {GPIO(1, 21), 3, 2, (void*)&LPC_PWM1->MR3},
    {GPIO(2,  2), 3, 1, (void*)&LPC_PWM1->MR3},
    {GPIO(3, 26), 3, 3, (void*)&LPC_PWM1->MR3},
    {GPIO(1, 23), 4, 2, (void*)&LPC_PWM1->MR4},
    {GPIO(2,  3), 4, 1, (void*)&LPC_PWM1->MR4},
    {GPIO(1, 24), 5, 2, (void*)&LPC_PWM1->MR5},
    {GPIO(2,  4), 5, 1, (void*)&LPC_PWM1->MR5},
    {GPIO(1, 26), 6, 2, (void*)&LPC_PWM1->MR6},
    {GPIO(2,  5), 6, 1, (void*)&LPC_PWM1->MR6}
};

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val) {
    // Find pin in pwm_regs table
    const struct gpio_pwm_info* p = pwm_regs;
    for (;; p++) {
        if (p >= &pwm_regs[ARRAY_SIZE(pwm_regs)])
            shutdown("Not a valid PWM pin");
        if (p->pin == pin)
            break;
    }

    // Map cycle_time to pwm clock divisor
    uint32_t clock_div = CONFIG_CLOCK_FREQ / get_pclock_frequency(PCLK_PWM1);
    uint32_t prescaler = (cycle_time / clock_div) / (MAX_PWM - 1);
    if (prescaler > 0) {
        prescaler -= 1;
    }

    gpio_peripheral(p->pin, p->function, 0);

    // Enable clock
    if (!is_enabled_pclock(PCLK_PWM1)) {
        enable_pclock(PCLK_PWM1);
    }

    if (LPC_PWM1->TCR & 1) {
        if (LPC_PWM1->PR != prescaler) {
            shutdown("PWM already programmed at different speed");
        }
    } else {
        LPC_PWM1->PR = (uint16_t) prescaler;
        LPC_PWM1->MCR = 2;
        LPC_PWM1->MR0 = MAX_PWM - 1;
        LPC_PWM1->LER |= 1 << 0;
        LPC_PWM1->TCR = 0x2; // Reset PWM
        LPC_PWM1->TCR = 0x8; // Enable PWM mode
    }

    if (LPC_PWM1->PCR & (1 << (p->channel + 8))) {
        shutdown("PWM output already in use");
    }

    struct gpio_pwm channel = {.reg = p->reg, .channel = p->channel};
    gpio_pwm_write(channel, val);

    // Enable channel
    LPC_PWM1->PCR |= (1 << (p->channel + 8));

    // Enable PWM peripheral
    LPC_PWM1->TCR |= 0x1;

    return channel;
}

void
gpio_pwm_write(struct gpio_pwm g, uint32_t val) {
    *(volatile uint32_t*) g.reg = val;
    LPC_PWM1->LER |= (1 << g.channel);
}
