// Analog to Digital Converter support
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_atmega644p
#include "command.h" // shutdown
#include "gpio.h" // gpio_adc_read
#include "internal.h" // GPIO
#include "pgm.h" // PROGMEM
#include "sched.h" // sched_shutdown

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
#elif CONFIG_MACH_atmega32u4
    GPIO('F', 0), GPIO('F', 1), GPIO('F', 2), GPIO('F', 3),
    GPIO('F', 4), GPIO('F', 5), GPIO('F', 6), GPIO('F', 7),
    GPIO('D', 4), GPIO('D', 6), GPIO('D', 7), GPIO('B', 4),
#elif CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
    GPIO('F', 0), GPIO('F', 1), GPIO('F', 2), GPIO('F', 3),
    GPIO('F', 4), GPIO('F', 5), GPIO('F', 6), GPIO('F', 7),
    GPIO('K', 0), GPIO('K', 1), GPIO('K', 2), GPIO('K', 3),
    GPIO('K', 4), GPIO('K', 5), GPIO('K', 6), GPIO('K', 7),
#endif
};

// The atmega168/328 have two analog only pins
#if CONFIG_MACH_atmega168 || CONFIG_MACH_atmega328 || CONFIG_MACH_atmega328p
DECL_ENUMERATION_RANGE("pin", "PE0", GPIO('E', 0), 2);
#endif

enum { ADMUX_DEFAULT = 0x40 };
enum { ADC_ENABLE = (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)|(1<<ADEN)|(1<<ADIF) };

DECL_CONSTANT("ADC_MAX", 1023);

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
