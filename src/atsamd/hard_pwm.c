// Hardware PWM support on samd21
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

struct gpio_pwm_info {
    uint32_t gpio;
    Tcc *tcc;
    uint32_t pclk_id, pm_id, channel;
    char ptype;
};

static const struct gpio_pwm_info pwm_regs[] = {
    { GPIO('A', 4), TCC0, TCC0_GCLK_ID, ID_TCC0, 0, 'E' },
    { GPIO('A', 5), TCC0, TCC0_GCLK_ID, ID_TCC0, 1, 'E' },
    { GPIO('A', 6), TCC1, TCC1_GCLK_ID, ID_TCC1, 0, 'E' },
    { GPIO('A', 7), TCC1, TCC1_GCLK_ID, ID_TCC1, 1, 'E' },
    { GPIO('A', 8), TCC0, TCC0_GCLK_ID, ID_TCC0, 0, 'E' },
    { GPIO('A', 9), TCC0, TCC0_GCLK_ID, ID_TCC0, 1, 'E' },
    { GPIO('A', 10), TCC1, TCC1_GCLK_ID, ID_TCC1, 0, 'E' },
    { GPIO('A', 11), TCC1, TCC1_GCLK_ID, ID_TCC1, 1, 'E' },
    { GPIO('A', 12), TCC2, TCC2_GCLK_ID, ID_TCC2, 0, 'E' },
    { GPIO('A', 13), TCC2, TCC2_GCLK_ID, ID_TCC2, 1, 'E' },
    { GPIO('A', 16), TCC2, TCC2_GCLK_ID, ID_TCC2, 0, 'E' },
    { GPIO('A', 17), TCC2, TCC2_GCLK_ID, ID_TCC2, 1, 'E' },
    { GPIO('A', 18), TCC0, TCC0_GCLK_ID, ID_TCC0, 2, 'F' },
    { GPIO('A', 19), TCC0, TCC0_GCLK_ID, ID_TCC0, 3, 'F' },
    { GPIO('A', 24), TCC1, TCC1_GCLK_ID, ID_TCC1, 2, 'F' },
    { GPIO('A', 25), TCC1, TCC1_GCLK_ID, ID_TCC1, 3, 'F' },
    { GPIO('A', 30), TCC1, TCC1_GCLK_ID, ID_TCC1, 0, 'E' },
    { GPIO('A', 31), TCC1, TCC1_GCLK_ID, ID_TCC1, 1, 'E' },
    { GPIO('B', 30), TCC0, TCC0_GCLK_ID, ID_TCC0, 0, 'E' },
    { GPIO('B', 31), TCC0, TCC0_GCLK_ID, ID_TCC0, 1, 'E' },
};

#define MAX_PWM 255

DECL_CONSTANT(PWM_MAX, MAX_PWM);

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    // Find pin in pwm_regs table
    const struct gpio_pwm_info *p = pwm_regs;
    for (; ; p++) {
        if (p >= &pwm_regs[ARRAY_SIZE(pwm_regs)])
            shutdown("Not a valid PWM pin");
        if (p->gpio == pin)
            break;
    }

    // Enable timer clock
    enable_pclock(p->pclk_id, p->pm_id);

    // Map cycle_time to pwm clock divisor
    uint32_t cs;
    switch (cycle_time) {
    case                      0 ...      (1+2) * MAX_PWM / 2 - 1: cs = 0; break;
    case    (1+2) * MAX_PWM / 2 ...      (2+4) * MAX_PWM / 2 - 1: cs = 1; break;
    case    (2+4) * MAX_PWM / 2 ...      (4+8) * MAX_PWM / 2 - 1: cs = 2; break;
    case    (4+8) * MAX_PWM / 2 ...     (8+16) * MAX_PWM / 2 - 1: cs = 3; break;
    case   (8+16) * MAX_PWM / 2 ...    (16+64) * MAX_PWM / 2 - 1: cs = 4; break;
    case  (16+64) * MAX_PWM / 2 ...   (64+256) * MAX_PWM / 2 - 1: cs = 5; break;
    case (64+256) * MAX_PWM / 2 ... (256+1024) * MAX_PWM / 2 - 1: cs = 6; break;
    default:                                                      cs = 7; break;
    }
    uint32_t ctrla = TCC_CTRLA_ENABLE | TCC_CTRLA_PRESCALER(cs);

    // Enable timer
    Tcc *tcc = p->tcc;
    uint32_t old_ctrla = tcc->CTRLA.reg;
    if (old_ctrla != ctrla) {
        if (old_ctrla & TCC_CTRLA_ENABLE)
            shutdown("PWM already programmed at different speed");
        tcc->CTRLA.reg = ctrla & ~TCC_CTRLA_ENABLE;
        tcc->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;
        tcc->PER.reg = MAX_PWM;
        tcc->CTRLA.reg = ctrla;
    }

    // Set initial value
    struct gpio_pwm g = (struct gpio_pwm) { (void*)&tcc->CCB[p->channel].reg };
    gpio_pwm_write(g, val);

    // Route output to pin
    gpio_peripheral(pin, p->ptype, 0);

    return g;
}

void
gpio_pwm_write(struct gpio_pwm g, uint8_t val)
{
    *(volatile uint32_t*)g.reg = val;
}
