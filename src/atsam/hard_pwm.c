// Hardware PWM support on atsam
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

#define MAX_PWM 255
DECL_CONSTANT("PWM_MAX", MAX_PWM);


/****************************************************************
 * TC hardware device
 ****************************************************************/

struct gpio_tc_info {
    uint8_t gpio, ptype, id;
    volatile void *reg;
};

static const struct gpio_tc_info tc_regs[] = {
#if CONFIG_MACH_SAM3X
    { GPIO('B', 25), 'B', ID_TC0, &TC0->TC_CHANNEL[0].TC_RA },
    { GPIO('A', 2),  'A', ID_TC1, &TC0->TC_CHANNEL[1].TC_RA },
    { GPIO('A', 5),  'A', ID_TC2, &TC0->TC_CHANNEL[2].TC_RA },
    { GPIO('B', 27), 'B', ID_TC0, &TC0->TC_CHANNEL[0].TC_RB },
    { GPIO('A', 3),  'A', ID_TC1, &TC0->TC_CHANNEL[1].TC_RB },
    { GPIO('A', 6),  'A', ID_TC2, &TC0->TC_CHANNEL[2].TC_RB },
    { GPIO('B', 0),  'B', ID_TC3, &TC1->TC_CHANNEL[0].TC_RA },
    { GPIO('B', 2),  'B', ID_TC4, &TC1->TC_CHANNEL[1].TC_RA },
    { GPIO('B', 4),  'B', ID_TC5, &TC1->TC_CHANNEL[2].TC_RA },
    { GPIO('B', 1),  'B', ID_TC3, &TC1->TC_CHANNEL[0].TC_RB },
    { GPIO('B', 3),  'B', ID_TC4, &TC1->TC_CHANNEL[1].TC_RB },
    { GPIO('B', 5),  'B', ID_TC5, &TC1->TC_CHANNEL[2].TC_RB },
#if CONFIG_MACH_SAM3X8E
    { GPIO('C', 25), 'B', ID_TC6, &TC2->TC_CHANNEL[0].TC_RA },
    { GPIO('C', 28), 'B', ID_TC7, &TC2->TC_CHANNEL[1].TC_RA },
    { GPIO('D', 7),  'B', ID_TC8, &TC2->TC_CHANNEL[2].TC_RA },
    { GPIO('C', 26), 'B', ID_TC6, &TC2->TC_CHANNEL[0].TC_RB },
    { GPIO('C', 29), 'B', ID_TC7, &TC2->TC_CHANNEL[1].TC_RB },
    { GPIO('D', 8),  'B', ID_TC8, &TC2->TC_CHANNEL[2].TC_RB },
#endif
#elif CONFIG_MACH_SAM4
    { GPIO('A', 0),  'B', ID_TC0, &TC0->TC_CHANNEL[0].TC_RA },
    { GPIO('A', 15), 'B', ID_TC1, &TC0->TC_CHANNEL[1].TC_RA },
    { GPIO('A', 26), 'B', ID_TC2, &TC0->TC_CHANNEL[2].TC_RA },
    { GPIO('A', 1),  'B', ID_TC0, &TC0->TC_CHANNEL[0].TC_RB },
    { GPIO('A', 16), 'B', ID_TC1, &TC0->TC_CHANNEL[1].TC_RB },
    { GPIO('A', 27), 'B', ID_TC2, &TC0->TC_CHANNEL[2].TC_RB },
    { GPIO('C', 23), 'B', ID_TC3, &TC1->TC_CHANNEL[0].TC_RA },
    { GPIO('C', 26), 'B', ID_TC4, &TC1->TC_CHANNEL[1].TC_RA },
    { GPIO('C', 29), 'B', ID_TC5, &TC1->TC_CHANNEL[2].TC_RA },
    { GPIO('C', 24), 'B', ID_TC3, &TC1->TC_CHANNEL[0].TC_RB },
    { GPIO('C', 27), 'B', ID_TC4, &TC1->TC_CHANNEL[1].TC_RB },
    { GPIO('C', 30), 'B', ID_TC5, &TC1->TC_CHANNEL[2].TC_RB },
#if CONFIG_MACH_SAM4E8E
    { GPIO('C', 5),  'B', ID_TC6, &TC2->TC_CHANNEL[0].TC_RA },
    { GPIO('C', 8),  'B', ID_TC7, &TC2->TC_CHANNEL[1].TC_RA },
    { GPIO('C', 11), 'B', ID_TC8, &TC2->TC_CHANNEL[2].TC_RA },
    { GPIO('C', 6),  'B', ID_TC6, &TC2->TC_CHANNEL[0].TC_RB },
    { GPIO('C', 9),  'B', ID_TC7, &TC2->TC_CHANNEL[1].TC_RB },
    { GPIO('C', 12), 'B', ID_TC8, &TC2->TC_CHANNEL[2].TC_RB },
#endif
#endif
};

static inline int tc_is_tc(struct gpio_pwm g) {
    return (((uint32_t)g.reg & ~0xffff) == ((uint32_t)TC0 & ~0xffff));
}
static inline TcChannel *tc_from_reg(struct gpio_pwm g) {
    return (void*)((uint32_t)g.reg & ~0x3f);
}
static inline int tc_is_b(struct gpio_pwm g) {
    return (((uint32_t)g.reg & 0x3f)
            == ((uint32_t)&TC0->TC_CHANNEL[0].TC_RB & 0x3f));
}

static void
gpio_tc_write(struct gpio_pwm g, uint32_t val)
{
    TcChannel *tc = tc_from_reg(g);
    uint32_t mask = TC_CMR_ACPA_Msk | TC_CMR_ACPC_Msk;
    uint32_t bits = TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;
    if (!val)
        bits = TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR;
    else if (val >= MAX_PWM)
        bits = TC_CMR_ACPA_SET | TC_CMR_ACPC_SET;
    if (tc_is_b(g)) {
        mask <<= 8;
        bits <<= 8;
    }
    irqstatus_t flag = irq_save();
    tc->TC_CMR = (tc->TC_CMR & ~mask) | bits;
    *(volatile uint32_t*)g.reg = val;
    irq_restore(flag);
}

static struct gpio_pwm
gpio_tc_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    // Find pin in tc_regs table
    const struct gpio_tc_info *p = tc_regs;
    for (; ; p++) {
        if (p >= &tc_regs[ARRAY_SIZE(tc_regs)])
            shutdown("Not a valid PWM pin");
        if (p->gpio == pin)
            break;
    }

    // Map cycle_time to clock divisor
    uint32_t div = TC_CMR_TCCLKS_TIMER_CLOCK4;
    if (cycle_time < (MAX_PWM*8 + MAX_PWM*2) / 2)
        div = TC_CMR_TCCLKS_TIMER_CLOCK1;
    else if (cycle_time < (MAX_PWM*32 + MAX_PWM*8) / 2)
        div = TC_CMR_TCCLKS_TIMER_CLOCK2;
    else if (cycle_time < (MAX_PWM*128 + MAX_PWM*32) / 2)
        div = TC_CMR_TCCLKS_TIMER_CLOCK3;

    // Enable clock
    enable_pclock(p->id);

    // Enable PWM output
    struct gpio_pwm g = (struct gpio_pwm){ (void*)p->reg };
    TcChannel *tc = tc_from_reg(g);
    uint32_t prev_cmr = tc->TC_CMR;
    if (prev_cmr && (prev_cmr & TC_CMR_TCCLKS_Msk) != div)
        shutdown("PWM already programmed at different speed");
    gpio_peripheral(pin, p->ptype, 0);
    if (prev_cmr) {
        gpio_tc_write(g, val);
    } else {
        tc->TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | div | TC_CMR_EEVT_XC0;
        gpio_tc_write(g, val);
        tc->TC_RC = MAX_PWM;
        tc->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    }
    return g;
}


/****************************************************************
 * PWM hardware device
 ****************************************************************/

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

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    // Find pin in pwm_regs table
    const struct gpio_pwm_info *p = pwm_regs;
    for (; ; p++) {
        if (p >= &pwm_regs[ARRAY_SIZE(pwm_regs)])
            return gpio_tc_setup(pin, cycle_time, val);
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
gpio_pwm_write(struct gpio_pwm g, uint32_t val)
{
    if (tc_is_tc(g))
        gpio_tc_write(g, val);
    else
        *(volatile uint32_t*)g.reg = val;
}
