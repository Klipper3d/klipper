// Hardware PWM pin support
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_atmega644p
#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO2REGS
#include "irq.h" // irq_save
#include "pgm.h" // PROGMEM
#include "sched.h" // sched_shutdown

struct gpio_pwm_info {
    uint8_t pin;
    volatile void *ocr;
    volatile uint8_t *rega, *regb;
    uint8_t en_bit, flags;
};

enum { GP_8BIT=1, GP_AFMT=2 };

static const struct gpio_pwm_info pwm_regs[] PROGMEM = {
    { GPIO('D', 6), &OCR0A, &TCCR0A, &TCCR0B, 1<<COM0A1, GP_8BIT },
    { GPIO('D', 5), &OCR0B, &TCCR0A, &TCCR0B, 1<<COM0B1, GP_8BIT },
    { GPIO('B', 1), &OCR1A, &TCCR1A, &TCCR1B, 1<<COM1A1, 0 },
    { GPIO('B', 2), &OCR1B, &TCCR1A, &TCCR1B, 1<<COM1B1, 0 },
    { GPIO('B', 3), &OCR2A, &TCCR2A, &TCCR2B, 1<<COM2A1, GP_8BIT|GP_AFMT },
    { GPIO('D', 3), &OCR2B, &TCCR2A, &TCCR2B, 1<<COM2B1, GP_8BIT|GP_AFMT },
};

DECL_CONSTANT("PWM_MAX", 255);

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    // Find pin in pwm_regs table
    const struct gpio_pwm_info *p = pwm_regs;
    for (; ; p++) {
        if (p >= &pwm_regs[ARRAY_SIZE(pwm_regs)])
            shutdown("Not a valid PWM pin");
        if (READP(p->pin) == pin)
            break;
    }

    // Map cycle_time to pwm clock divisor
    uint8_t flags = READP(p->flags), cs;
    if (flags & GP_AFMT) {
        switch (cycle_time) {
        case                    0 ...      (1+8) * 510L / 2 - 1: cs = 1; break;
        case     (1+8) * 510L / 2 ...     (8+32) * 510L / 2 - 1: cs = 2; break;
        case    (8+32) * 510L / 2 ...    (32+64) * 510L / 2 - 1: cs = 3; break;
        case   (32+64) * 510L / 2 ...   (64+128) * 510L / 2 - 1: cs = 4; break;
        case  (64+128) * 510L / 2 ...  (128+256) * 510L / 2 - 1: cs = 5; break;
        case (128+256) * 510L / 2 ... (256+1024) * 510L / 2 - 1: cs = 6; break;
        default:                                                 cs = 7; break;
        }
    } else {
        switch (cycle_time) {
        case                    0 ...      (1+8) * 510L / 2 - 1: cs = 1; break;
        case     (1+8) * 510L / 2 ...     (8+64) * 510L / 2 - 1: cs = 2; break;
        case    (8+64) * 510L / 2 ...   (64+256) * 510L / 2 - 1: cs = 3; break;
        case  (64+256) * 510L / 2 ... (256+1024) * 510L / 2 - 1: cs = 4; break;
        default:                                                 cs = 5; break;
        }
    }
    volatile uint8_t *rega = READP(p->rega), *regb = READP(p->regb);
    uint8_t en_bit = READP(p->en_bit);
    struct gpio_digital_regs *gpio_regs = GPIO2REGS(pin);
    uint8_t gpio_bit = GPIO2BIT(pin);
    struct gpio_pwm g = (struct gpio_pwm) {
        (void*)READP(p->ocr), flags & GP_8BIT };
    if (rega == &TCCR1A)
        shutdown("Can not use timer1 for PWM; timer1 is used for timers");

    // Setup PWM timer
    irqstatus_t flag = irq_save();
    uint8_t old_cs = *regb & 0x07;
    if (old_cs && old_cs != cs)
        shutdown("PWM already programmed at different speed");
    *regb = cs;

    // Set default value and enable output
    gpio_pwm_write(g, val);
    *rega |= (1<<WGM00) | en_bit;
    gpio_regs->mode |= gpio_bit;
    irq_restore(flag);

    return g;
}

void
gpio_pwm_write(struct gpio_pwm g, uint8_t val)
{
    if (g.size8) {
        *(volatile uint8_t*)g.reg = val;
    } else {
        irqstatus_t flag = irq_save();
        *(volatile uint16_t*)g.reg = val;
        irq_restore(flag);
    }
}
