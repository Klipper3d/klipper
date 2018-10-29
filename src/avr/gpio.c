// GPIO functions on AVR.
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
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
 * General Purpose Input Output (GPIO) pins
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
    &PINE, &PINF,
#endif
#ifdef PING
    &PING, &PINH, NULL, &PINJ, &PINK, &PINL
#endif
};

struct gpio_digital_regs {
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    volatile uint8_t in : 8, mode : 8, out : 8;
};

#define GPIO2REGS(pin)                                                  \
    ((struct gpio_digital_regs*)READP(digital_regs[GPIO2PORT(pin)]))

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    struct gpio_digital_regs *regs = GPIO2REGS(pin);
    if (! regs)
        goto fail;
    struct gpio_out g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    irqstatus_t flag = irq_save();
    g.regs->out = val ? (g.regs->out | g.bit) : (g.regs->out & ~g.bit);
    g.regs->mode |= g.bit;
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    g.regs->in = g.bit;
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    irqstatus_t flag = irq_save();
    g.regs->out = val ? (g.regs->out | g.bit) : (g.regs->out & ~g.bit);
    irq_restore(flag);
}

struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    struct gpio_digital_regs *regs = GPIO2REGS(pin);
    if (! regs)
        goto fail;
    struct gpio_in g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    irqstatus_t flag = irq_save();
    g.regs->out = pull_up > 0 ? (g.regs->out | g.bit) : (g.regs->out & ~g.bit);
    g.regs->mode &= ~g.bit;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return !!(g.regs->in & g.bit);
}


/****************************************************************
 * Hardware Pulse Width Modulation (PWM) pins
 ****************************************************************/

struct gpio_pwm_info {
    volatile void *ocr;
    volatile uint8_t *rega, *regb;
    uint8_t en_bit, flags;
};

enum { GP_8BIT=1, GP_AFMT=2 };

static const struct gpio_pwm_info pwm_regs[] PROGMEM = {
    { &OCR0A, &TCCR0A, &TCCR0B, 1<<COM0A1, GP_8BIT },
    { &OCR0B, &TCCR0A, &TCCR0B, 1<<COM0B1, GP_8BIT },
    { &OCR1A, &TCCR1A, &TCCR1B, 1<<COM1A1, 0 },
    { &OCR1B, &TCCR1A, &TCCR1B, 1<<COM1B1, 0 },
#ifdef OCR1C
    { &OCR1C, &TCCR1A, &TCCR1B, 1<<COM1C1, 0 },
#endif
    { &OCR2A, &TCCR2A, &TCCR2B, 1<<COM2A1, GP_8BIT|GP_AFMT },
    { &OCR2B, &TCCR2A, &TCCR2B, 1<<COM2B1, GP_8BIT|GP_AFMT },
#ifdef OCR3A
    { &OCR3A, &TCCR3A, &TCCR3B, 1<<COM3A1, 0 },
    { &OCR3B, &TCCR3A, &TCCR3B, 1<<COM3B1, 0 },
#endif
#ifdef OCR3C
    { &OCR3C, &TCCR3A, &TCCR3B, 1<<COM3C1, 0 },
#endif
#ifdef OCR4A
    { &OCR4A, &TCCR4A, &TCCR4B, 1<<COM4A1, 0 },
    { &OCR4B, &TCCR4A, &TCCR4B, 1<<COM4B1, 0 },
    { &OCR4C, &TCCR4A, &TCCR4B, 1<<COM4C1, 0 },
    { &OCR5A, &TCCR5A, &TCCR5B, 1<<COM5A1, 0 },
    { &OCR5B, &TCCR5A, &TCCR5B, 1<<COM5B1, 0 },
    { &OCR5C, &TCCR5A, &TCCR5B, 1<<COM5C1, 0 },
#endif
};

static const uint8_t pwm_pins[ARRAY_SIZE(pwm_regs)] PROGMEM = {
#if CONFIG_MACH_atmega168 || CONFIG_MACH_atmega328 || CONFIG_MACH_atmega328p
    GPIO('D', 6), GPIO('D', 5),
    GPIO('B', 1), GPIO('B', 2),
    GPIO('B', 3), GPIO('D', 3),
#elif CONFIG_MACH_atmega644p || CONFIG_MACH_atmega1284p
    GPIO('B', 3), GPIO('B', 4),
    GPIO('D', 5), GPIO('D', 4),
    GPIO('D', 7), GPIO('D', 6),
# ifdef OCR3A
    GPIO('B', 6), GPIO('B', 7),
# endif
#elif CONFIG_MACH_at90usb1286 || CONFIG_MACH_at90usb646
    GPIO('B', 7), GPIO('D', 0),
    GPIO('B', 5), GPIO('B', 6), GPIO('B', 7),
    GPIO('B', 4), GPIO('D', 1),
    GPIO('C', 6), GPIO('C', 5), GPIO('C', 4),
#elif CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
    GPIO('B', 7), GPIO('G', 5),
    GPIO('B', 5), GPIO('B', 6), GPIO('B', 7),
    GPIO('B', 4), GPIO('H', 6),
    GPIO('E', 3), GPIO('E', 4), GPIO('E', 5),
    GPIO('H', 3), GPIO('H', 4), GPIO('H', 5),
    GPIO('L', 3), GPIO('L', 4), GPIO('L', 5),
#endif
};

DECL_CONSTANT(PWM_MAX, 255);

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    // Find pin in pwm_pins table
    uint8_t chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(pwm_pins))
            shutdown("Not a valid PWM pin");
        if (READP(pwm_pins[chan]) == pin)
            break;
    }

    // Map cycle_time to pwm clock divisor
    const struct gpio_pwm_info *p = &pwm_regs[chan];
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


/****************************************************************
 * Analog to Digital Converter (ADC) pins
 ****************************************************************/

static const uint8_t adc_pins[] PROGMEM = {
#if CONFIG_MACH_atmega168 || CONFIG_MACH_atmega328 || CONFIG_MACH_atmega328p
    GPIO('C', 0), GPIO('C', 1), GPIO('C', 2), GPIO('C', 3),
    GPIO('C', 4), GPIO('C', 5), GPIO('E', 0), GPIO('E', 1),
#elif CONFIG_MACH_atmega644p || CONFIG_MACH_atmega1284p
    GPIO('A', 0), GPIO('A', 1), GPIO('A', 2), GPIO('A', 3),
    GPIO('A', 4), GPIO('A', 5), GPIO('A', 6), GPIO('A', 7),
#elif CONFIG_MACH_at90usb1286 || CONFIG_MACH_at90usb646
    GPIO('F', 0), GPIO('F', 1), GPIO('F', 2), GPIO('F', 3),
    GPIO('F', 4), GPIO('F', 5), GPIO('F', 6), GPIO('F', 7),
#elif CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
    GPIO('F', 0), GPIO('F', 1), GPIO('F', 2), GPIO('F', 3),
    GPIO('F', 4), GPIO('F', 5), GPIO('F', 6), GPIO('F', 7),
    GPIO('K', 0), GPIO('K', 1), GPIO('K', 2), GPIO('K', 3),
    GPIO('K', 4), GPIO('K', 5), GPIO('K', 6), GPIO('K', 7),
#endif
};

enum { ADMUX_DEFAULT = 0x40 };
enum { ADC_ENABLE = (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)|(1<<ADEN)|(1<<ADIF) };

DECL_CONSTANT(ADC_MAX, 1023);

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    // Find pin in adc_pins table
    uint8_t chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (READP(adc_pins[chan]) == pin)
            break;
    }

    // Enable ADC
    ADCSRA = ADC_ENABLE;

    // Disable digital input for this pin
#ifdef DIDR2
    if (chan >= 8)
        DIDR2 |= 1 << (chan & 0x07);
    else
#endif
        DIDR0 |= 1 << chan;

    return (struct gpio_adc){ chan };
}

enum { ADC_DUMMY=0xff };
static uint8_t last_analog_read = ADC_DUMMY;

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    if (ADCSRA & (1<<ADSC))
        // Busy
        goto need_delay;
    if (last_analog_read == g.chan)
        // Sample now ready
        return 0;
    if (last_analog_read != ADC_DUMMY)
        // Sample on another channel in progress
        goto need_delay;
    last_analog_read = g.chan;

    // Set the channel to sample
#if defined(ADCSRB) && defined(MUX5)
    // The MUX5 bit of ADCSRB selects whether we're reading from
    // channels 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    ADCSRB = ((g.chan >> 3) & 0x01) << MUX5;
#endif
    ADMUX = ADMUX_DEFAULT | (g.chan & 0x07);

    // Start the sample
    ADCSRA = ADC_ENABLE | (1<<ADSC);

    // Schedule next attempt after sample is likely to be complete
need_delay:
    return (13 + 1) * 128 + 200;
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    last_analog_read = ADC_DUMMY;
    return ADC;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (last_analog_read == g.chan)
        last_analog_read = ADC_DUMMY;
}


/****************************************************************
 * Serial Peripheral Interface (SPI) hardware
 ****************************************************************/

#if CONFIG_MACH_atmega168 || CONFIG_MACH_atmega328 || CONFIG_MACH_atmega328p
static const uint8_t SS = GPIO('B', 2), SCK = GPIO('B', 5);
static const uint8_t MOSI = GPIO('B', 3), MISO = GPIO('B', 4);
#elif CONFIG_MACH_atmega644p || CONFIG_MACH_atmega1284p
static const uint8_t SS = GPIO('B', 4), SCK = GPIO('B', 7);
static const uint8_t MOSI = GPIO('B', 5), MISO = GPIO('B', 6);
#elif CONFIG_MACH_at90usb1286 || CONFIG_MACH_at90usb646 || CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
static const uint8_t SS = GPIO('B', 0), SCK = GPIO('B', 1);
static const uint8_t MOSI = GPIO('B', 2), MISO = GPIO('B', 3);
#endif

static void
spi_init(void)
{
    if (!(GPIO2REGS(SS)->mode & GPIO2BIT(SS)))
        // The SS pin must be an output pin (but is otherwise unused)
        gpio_out_setup(SS, 0);
    gpio_out_setup(SCK, 0);
    gpio_out_setup(MOSI, 0);
    gpio_in_setup(MISO, 0);

    SPCR = (1<<MSTR) | (1<<SPE);
    SPSR = 0;
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus || mode > 3)
        shutdown("Invalid spi_setup parameters");

    // Make sure the SPI interface is enabled
    spi_init();

    // Setup rate
    struct spi_config config = {0, 0};
    if (rate >= (CONFIG_CLOCK_FREQ / 2)) {
        config.spsr = (1<<SPI2X);
    } else if (rate >= (CONFIG_CLOCK_FREQ / 4)) {
        config.spcr = 0;
    } else if (rate >= (CONFIG_CLOCK_FREQ / 8)) {
        config.spcr = 1;
        config.spsr = (1<<SPI2X);
    } else if (rate >= (CONFIG_CLOCK_FREQ / 16)) {
        config.spcr = 1;
    } else if (rate >= (CONFIG_CLOCK_FREQ / 32)) {
        config.spcr = 2;
        config.spsr = (1<<SPI2X);
    } else if (rate >= (CONFIG_CLOCK_FREQ / 64)) {
        config.spcr = 2;
    } else {
        config.spcr = 3;
    }

    // Setup mode
    config.spcr |= (1<<SPE) | (1<<MSTR);
    switch(mode) {
        case 0: {
            // MODE 0 - CPOL=0, CPHA=0
            break;
        }
        case 1: {
            // MODE 1 - CPOL=0, CPHA=1
            config.spcr |= (1<<CPHA);
            break;
        }
        case 2: {
            // MODE 2 - CPOL=1, CPHA=0
            config.spcr |= (1<<CPOL);
            break;
        }
        case 3: {
            // MODE 3 - CPOL=1, CPHA=1
            config.spcr |= (1<<CPOL) | (1<<CPHA);
            break;
        }
    }

    return config;
}

void
spi_prepare(struct spi_config config)
{
    SPCR = config.spcr;
    SPSR = config.spsr;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    if (receive_data) {
        while (len--) {
            SPDR = *data;
            while (!(SPSR & (1<<SPIF)))
                ;
            *data++ = SPDR;
        }
    } else {
        while (len--) {
            SPDR = *data++;
            while (!(SPSR & (1<<SPIF)))
                ;
        }
    }
}
