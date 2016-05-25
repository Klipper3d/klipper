// GPIO functions on AVR.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // NULL
#include "autoconf.h" // CONFIG_MACH_atmega644p
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_write
#include "irq.h" // irq_save
#include "pgm.h" // PROGMEM
#include "sched.h" // DECL_INIT


/****************************************************************
 * AVR chip definitions
 ****************************************************************/

#define GPIO(PORT, NUM) (((PORT)-'A') * 8 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 8)
#define GPIO2BIT(PIN) (1<<((PIN) % 8))

static volatile uint8_t * const digital_regs[] PROGMEM = {
#ifdef PINA
    &PINA,
#else
    NULL,
#endif
    &PINB, &PINC, &PIND,
#ifdef PINE
    &PINE, &PINF, &PING, &PINH, NULL, &PINJ, &PINK, &PINL
#endif
};

struct gpio_digital_regs {
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    volatile uint8_t in : 8, mode : 8, out : 8;
};

#define GPIO2REGS(pin)                                                  \
    ((struct gpio_digital_regs*)READP(digital_regs[GPIO2PORT(pin)]))

struct gpio_pwm_info {
    volatile void *ocr;
    volatile uint8_t *rega, *regb;
    uint8_t en_bit, pin, flags;
};

enum { GP_8BIT=1, GP_AFMT=2 };

static const struct gpio_pwm_info pwm_regs[] PROGMEM = {
#if CONFIG_MACH_atmega168
    { &OCR0A, &TCCR0A, &TCCR0B, 1<<COM0A1, GPIO('D', 6), GP_8BIT         },
    { &OCR0B, &TCCR0A, &TCCR0B, 1<<COM0B1, GPIO('D', 5), GP_8BIT         },
//    { &OCR1A, &TCCR1A, &TCCR1B, 1<<COM1A1, GPIO('B', 1), 0               },
//    { &OCR1B, &TCCR1A, &TCCR1B, 1<<COM1B1, GPIO('B', 2), 0               },
    { &OCR2A, &TCCR2A, &TCCR2B, 1<<COM2A1, GPIO('B', 3), GP_8BIT|GP_AFMT },
    { &OCR2B, &TCCR2A, &TCCR2B, 1<<COM2B1, GPIO('D', 3), GP_8BIT|GP_AFMT },
#elif CONFIG_MACH_atmega644p
    { &OCR0A, &TCCR0A, &TCCR0B, 1<<COM0A1, GPIO('B', 3), GP_8BIT         },
    { &OCR0B, &TCCR0A, &TCCR0B, 1<<COM0B1, GPIO('B', 4), GP_8BIT         },
//    { &OCR1A, &TCCR1A, &TCCR1B, 1<<COM1A1, GPIO('D', 5), 0               },
//    { &OCR1B, &TCCR1A, &TCCR1B, 1<<COM1B1, GPIO('D', 4), 0               },
    { &OCR2A, &TCCR2A, &TCCR2B, 1<<COM2A1, GPIO('D', 7), GP_8BIT|GP_AFMT },
    { &OCR2B, &TCCR2A, &TCCR2B, 1<<COM2B1, GPIO('D', 6), GP_8BIT|GP_AFMT },
#elif CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
    { &OCR0A, &TCCR0A, &TCCR0B, 1<<COM0A1, GPIO('B', 7), GP_8BIT         },
    { &OCR0B, &TCCR0A, &TCCR0B, 1<<COM0B1, GPIO('G', 5), GP_8BIT         },
//    { &OCR1A, &TCCR1A, &TCCR1B, 1<<COM1A1, GPIO('B', 5), 0               },
//    { &OCR1B, &TCCR1A, &TCCR1B, 1<<COM1B1, GPIO('B', 6), 0               },
//    { &OCR1C, &TCCR1A, &TCCR1B, 1<<COM1C1, GPIO('B', 7), 0               },
    { &OCR2A, &TCCR2A, &TCCR2B, 1<<COM2A1, GPIO('B', 4), GP_8BIT|GP_AFMT },
    { &OCR2B, &TCCR2A, &TCCR2B, 1<<COM2B1, GPIO('H', 6), GP_8BIT|GP_AFMT },
    { &OCR3A, &TCCR3A, &TCCR3B, 1<<COM3A1, GPIO('E', 3), 0               },
    { &OCR3B, &TCCR3A, &TCCR3B, 1<<COM3B1, GPIO('E', 4), 0               },
    { &OCR3C, &TCCR3A, &TCCR3B, 1<<COM3C1, GPIO('E', 5), 0               },
    { &OCR4A, &TCCR4A, &TCCR4B, 1<<COM4A1, GPIO('H', 3), 0               },
    { &OCR4B, &TCCR4A, &TCCR4B, 1<<COM4B1, GPIO('H', 4), 0               },
    { &OCR4C, &TCCR4A, &TCCR4B, 1<<COM4C1, GPIO('H', 5), 0               },
    { &OCR5A, &TCCR5A, &TCCR5B, 1<<COM5A1, GPIO('L', 3), 0               },
    { &OCR5B, &TCCR5A, &TCCR5B, 1<<COM5B1, GPIO('L', 4), 0               },
    { &OCR5C, &TCCR5A, &TCCR5B, 1<<COM5C1, GPIO('L', 5), 0               },
#endif
};

struct gpio_adc_info {
    uint8_t pin;
};

static const struct gpio_adc_info adc_pins[] PROGMEM = {
#if CONFIG_MACH_atmega168
    { GPIO('C', 0) }, { GPIO('C', 1) }, { GPIO('C', 2) }, { GPIO('C', 3) },
    { GPIO('C', 4) }, { GPIO('C', 5) }, { GPIO('E', 0) }, { GPIO('E', 1) },
#elif CONFIG_MACH_atmega644p
    { GPIO('A', 0) }, { GPIO('A', 1) }, { GPIO('A', 2) }, { GPIO('A', 3) },
    { GPIO('A', 4) }, { GPIO('A', 5) }, { GPIO('A', 6) }, { GPIO('A', 7) },
#elif CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
    { GPIO('F', 0) }, { GPIO('F', 1) }, { GPIO('F', 2) }, { GPIO('F', 3) },
    { GPIO('F', 4) }, { GPIO('F', 5) }, { GPIO('F', 6) }, { GPIO('F', 7) },
    { GPIO('K', 0) }, { GPIO('K', 1) }, { GPIO('K', 2) }, { GPIO('K', 3) },
    { GPIO('K', 4) }, { GPIO('K', 5) }, { GPIO('K', 6) }, { GPIO('K', 7) },
#endif
};

#if CONFIG_MACH_atmega168
static const uint8_t SS = GPIO('B', 2), SCK = GPIO('B', 5), MOSI = GPIO('B', 3);
#elif CONFIG_MACH_atmega644p
static const uint8_t SS = GPIO('B', 4), SCK = GPIO('B', 7), MOSI = GPIO('B', 5);
#elif CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
static const uint8_t SS = GPIO('B', 0), SCK = GPIO('B', 1), MOSI = GPIO('B', 2);
#endif

static const uint8_t ADMUX_DEFAULT = 0x40;


/****************************************************************
 * gpio functions
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) > ARRAY_SIZE(digital_regs))
        goto fail;
    struct gpio_digital_regs *regs = GPIO2REGS(pin);
    if (! regs)
        goto fail;
    uint8_t bit = GPIO2BIT(pin);
    uint8_t flag = irq_save();
    regs->out = val ? (regs->out | bit) : (regs->out & ~bit);
    regs->mode |= bit;
    irq_restore(flag);
    return (struct gpio_out){ .regs=regs, .bit=bit };
fail:
    shutdown("Not an output pin");
}

void gpio_out_toggle(struct gpio_out g)
{
    g.regs->in = g.bit;
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    uint8_t flag = irq_save();
    g.regs->out = val ? (g.regs->out | g.bit) : (g.regs->out & ~g.bit);
    irq_restore(flag);
}

struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) > ARRAY_SIZE(digital_regs))
        goto fail;
    struct gpio_digital_regs *regs = GPIO2REGS(pin);
    if (! regs)
        goto fail;
    uint8_t bit = GPIO2BIT(pin);
    uint8_t flag = irq_save();
    regs->out = pull_up > 0 ? (regs->out | bit) : (regs->out & ~bit);
    regs->mode &= ~bit;
    irq_restore(flag);
    return (struct gpio_in){ .regs=regs, .bit=bit };
fail:
    shutdown("Not an input pin");
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return !!(g.regs->in & g.bit);
}


void
gpio_pwm_write(struct gpio_pwm g, uint8_t val)
{
    if (g.size8) {
        *(volatile uint8_t*)g.reg = val;
    } else {
        uint8_t flag = irq_save();
        *(volatile uint16_t*)g.reg = val;
        irq_restore(flag);
    }
}

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    uint8_t chan;
    for (chan=0; chan<ARRAY_SIZE(pwm_regs); chan++) {
        const struct gpio_pwm_info *p = &pwm_regs[chan];
        if (READP(p->pin) != pin)
            continue;
        uint8_t flags = READP(p->flags), cs;
        if (flags & GP_AFMT) {
            switch (cycle_time) {
            case 0        ...    8*510L - 1: cs = 1; break;
            case 8*510L   ...   32*510L - 1: cs = 2; break;
            case 32*510L  ...   64*510L - 1: cs = 3; break;
            case 64*510L  ...  128*510L - 1: cs = 4; break;
            case 128*510L ...  256*510L - 1: cs = 5; break;
            case 256*510L ... 1024*510L - 1: cs = 6; break;
            default:                         cs = 7; break;
            }
        } else {
            switch (cycle_time) {
            case 0        ...    8*510L - 1: cs = 1; break;
            case 8*510L   ...   64*510L - 1: cs = 2; break;
            case 64*510L  ...  256*510L - 1: cs = 3; break;
            case 256*510L ... 1024*510L - 1: cs = 4; break;
            default:                         cs = 5; break;
            }
        }
        volatile uint8_t *rega = READP(p->rega), *regb = READP(p->regb);
        uint8_t en_bit = READP(p->en_bit);
        struct gpio_digital_regs *regs = GPIO2REGS(pin);
        uint8_t bit = GPIO2BIT(pin);
        struct gpio_pwm g = (struct gpio_pwm) {
            (void*)READP(p->ocr), flags & GP_8BIT };

        // Setup PWM timer
        uint8_t flag = irq_save();
        uint8_t old_cs = *regb & 0x07;
        if (old_cs && old_cs != cs)
            shutdown("PWM already programmed at different speed");
        *regb = cs;

        // Set default value and enable output
        gpio_pwm_write(g, val);
        *rega |= (1<<WGM00) | en_bit;
        regs->mode |= bit;
        irq_restore(flag);

        return g;
    }
    shutdown("Not a valid PWM pin");
}


struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    uint8_t chan;
    for (chan=0; chan<ARRAY_SIZE(adc_pins); chan++) {
        const struct gpio_adc_info *a = &adc_pins[chan];
        if (READP(a->pin) != pin)
            continue;

        // Enable ADC
        ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)|(1<<ADEN);

        // Disable digital input for this pin
#ifdef DIDR2
        if (chan >= 8)
            DIDR2 |= 1 << (chan & 0x07);
        else
#endif
            DIDR0 |= 1 << chan;

        return (struct gpio_adc){ chan };
    }
    shutdown("Not a valid ADC pin");
}

uint32_t
gpio_adc_sample_time(void)
{
    return (13 + 1) * 128 + 200;
}

enum { ADC_DUMMY=0xff };
static uint8_t last_analog_read = ADC_DUMMY;

uint8_t
gpio_adc_sample(struct gpio_adc g)
{
    if (ADCSRA & (1<<ADSC))
        // Busy
        return 1;
    if (last_analog_read == g.chan)
        // Sample now ready
        return 0;
    if (last_analog_read != ADC_DUMMY)
        // Sample on another channel in progress
        return 1;
    last_analog_read = g.chan;

#if defined(ADCSRB) && defined(MUX5)
    // the MUX5 bit of ADCSRB selects whether we're reading from channels
    // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((g.chan >> 3) & 0x01) << MUX5);
#endif

    ADMUX = ADMUX_DEFAULT | (g.chan & 0x07);

    // start the conversion
    ADCSRA |= 1<<ADSC;
    return 1;
}

void
gpio_adc_clear_sample(struct gpio_adc g)
{
    if (last_analog_read == g.chan)
        last_analog_read = ADC_DUMMY;
}

uint16_t
gpio_adc_read(struct gpio_adc g)
{
    last_analog_read = ADC_DUMMY;
    return ADC;
}


void
spi_config(void)
{
    gpio_out_setup(SS, 1);
    gpio_out_setup(SCK, 0);
    gpio_out_setup(MOSI, 0);
    SPCR = (1<<MSTR) | (1<<SPE);
}

void
spi_transfer(char *data, uint8_t len)
{
    while (len--) {
        SPDR = *data;
        while (!(SPSR & (1<<SPIF)))
            ;
        *data++ = SPDR;
    }
}
