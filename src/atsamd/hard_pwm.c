// Hardware PWM support on samd21
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

// Available TCC devices
struct tcc_info_s {
    Tcc *tcc;
    uint32_t pclk_id, pm_id;
};
static const struct tcc_info_s tcc_info[] = {
    { TCC0, TCC0_GCLK_ID, ID_TCC0 },
    { TCC1, TCC1_GCLK_ID, ID_TCC1 },
    { TCC2, TCC2_GCLK_ID, ID_TCC2 },
};

// PWM pins and their TCC device/channel
struct gpio_pwm_info {
    uint8_t gpio, ptype, tcc, channel;
};
static const struct gpio_pwm_info pwm_regs[] = {
    { GPIO('A', 4),  'E', 0, 0 },
    { GPIO('A', 5),  'E', 0, 1 },
    { GPIO('A', 6),  'E', 1, 0 },
    { GPIO('A', 7),  'E', 1, 1 },
    { GPIO('A', 8),  'E', 0, 0 },
    { GPIO('A', 9),  'E', 0, 1 },
    { GPIO('A', 10), 'E', 1, 0 },
    { GPIO('A', 11), 'E', 1, 1 },
    { GPIO('A', 12), 'E', 2, 0 },
    { GPIO('A', 13), 'E', 2, 1 },
    { GPIO('A', 16), 'E', 2, 0 },
    { GPIO('A', 17), 'E', 2, 1 },
    { GPIO('A', 18), 'F', 0, 2 },
    { GPIO('A', 19), 'F', 0, 3 },
    { GPIO('A', 24), 'F', 1, 2 },
    { GPIO('A', 25), 'F', 1, 3 },
    { GPIO('A', 30), 'E', 1, 0 },
    { GPIO('A', 31), 'E', 1, 1 },
    { GPIO('B', 30), 'E', 0, 0 },
    { GPIO('B', 31), 'E', 0, 1 },
};

#define MAX_PWM 255
DECL_CONSTANT("PWM_MAX", MAX_PWM);

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
    enable_pclock(tcc_info[p->tcc].pclk_id, tcc_info[p->tcc].pm_id);

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
    Tcc *tcc = tcc_info[p->tcc].tcc;
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
    struct gpio_pwm g = (struct gpio_pwm) {
#if CONFIG_MACH_SAMD21
        (void*)&tcc->CCB[p->channel].reg
#elif CONFIG_MACH_SAMC21
        (void*)&tcc->CCBUF[p->channel].reg
#endif
    };
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
