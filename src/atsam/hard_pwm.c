// Hardware PWM support on atsam
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

struct gpio_pwm_info {
    uint8_t gpio, channel, ptype;
};

static const struct gpio_pwm_info pwm_regs[] = {
#if CONFIG_MACH_SAM3X
    { GPIO('A', 21), 0, 'B' },
    { GPIO('B', 16), 0, 'B' },
    { GPIO('A', 12), 1, 'B' },
    { GPIO('B', 17), 1, 'B' },
    { GPIO('A', 20), 2, 'B' },
    { GPIO('B', 18), 2, 'B' },
    { GPIO('A', 0),  3, 'B' },
    { GPIO('B', 19), 3, 'B' },
#if CONFIG_MACH_SAM3X8E
    { GPIO('C', 2),  0, 'B' },
    { GPIO('C', 4),  1, 'B' },
    { GPIO('C', 6),  2, 'B' },
    { GPIO('C', 8),  3, 'B' },
    { GPIO('C', 21), 4, 'B' },
    { GPIO('C', 22), 5, 'B' },
    { GPIO('C', 23), 6, 'B' },
    { GPIO('C', 24), 7, 'B' },
#endif
#elif CONFIG_MACH_SAM4
    { GPIO('A', 19), 0, 'B' },
    { GPIO('B', 5),  0, 'B' },
    { GPIO('C', 0),  0, 'B' },
    { GPIO('C', 13), 0, 'B' },
    { GPIO('A', 20), 1, 'B' },
    { GPIO('B', 12), 1, 'A' },
    { GPIO('C', 1),  1, 'B' },
    { GPIO('C', 15), 1, 'B' },
    { GPIO('A', 16), 2, 'C' },
    { GPIO('A', 30), 2, 'A' },
    { GPIO('B', 13), 2, 'A' },
    { GPIO('C', 2),  2, 'B' },
    { GPIO('A', 15), 3, 'C' },
    { GPIO('C', 3),  3, 'B' },
    { GPIO('C', 22), 3, 'B' },
#endif
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

    // Map cycle_time to pwm clock divisor
    uint32_t div;
    for (div=0; div<10; div++)
        if (cycle_time < ((MAX_PWM << (div + 1)) + (MAX_PWM << div)) / 2)
            break;

    // Enable clock
    enable_pclock(ID_PWM);

    // Enable PWM output
    if (PWM->PWM_SR & (1 << p->channel))
        shutdown("PWM channel already in use");
    gpio_peripheral(pin, p->ptype, 0);
    PWM->PWM_CH_NUM[p->channel].PWM_CMR = div;
    PWM->PWM_CH_NUM[p->channel].PWM_CPRD = MAX_PWM;
    PWM->PWM_CH_NUM[p->channel].PWM_CDTY = val;
    PWM->PWM_ENA = 1 << p->channel;

    return (struct gpio_pwm){ (void*)&PWM->PWM_CH_NUM[p->channel].PWM_CDTYUPD };
}

void
gpio_pwm_write(struct gpio_pwm g, uint8_t val)
{
    *(volatile uint32_t*)g.reg = val;
}
