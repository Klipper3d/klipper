// GPIO functions on sam4e8e
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board.h"
#include "gpio.h"
#include "pio/pio.h"
#include "pmc/pmc.h"
#include "afec/afec.h"

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    Pio *p_pio = pio_get_pin_group(pin);
    uint32_t ul_mask = pio_get_pin_group_mask(pin);
    uint32_t ul_id = pio_get_pin_group_id(pin);

    irqstatus_t flag = irq_save();

    pmc_enable_periph_clk(ul_id);
    pio_set_output(p_pio, ul_mask, val, false, false);

    irq_restore(flag);
    return (struct gpio_out){ .pin=pin, .regs=p_pio, .bit=ul_mask };
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    pio_toggle_pin(g.pin);
}

void
gpio_out_toggle(struct gpio_out g)
{
    // irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    // irq_restore(flag);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    if(val) {
        pio_set_pin_high(g.pin);
    } else {
        pio_set_pin_low(g.pin);
    }
}

struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{

    Pio *p_pio = pio_get_pin_group(pin);
    uint32_t ul_mask = pio_get_pin_group_mask(pin);
    uint32_t ul_id = pio_get_pin_group_id(pin);

    irqstatus_t flag = irq_save();

    pmc_enable_periph_clk(ul_id);
    pio_set_input(p_pio, ul_mask, PIO_PULLUP);

    irq_restore(flag);
    return (struct gpio_in){ .pin=pin, .regs=p_pio, .bit=ul_mask };
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return pio_get_pin_value(g.pin);
}
/****************************************************************
 * Analog Front-End Converter (AFEC) pins (see datasheet sec. 43.5)
 ****************************************************************/
static const uint8_t afec0_pins[] = {
    //remove first channel, since it offsets the channel number: GPIO('A', 8),
    GPIO('A', 17), GPIO('A', 18), GPIO('A', 19),
    GPIO('A', 20), GPIO('B', 0),  GPIO('B', 1), GPIO('C', 13),
    GPIO('C', 15), GPIO('C', 12), GPIO('C', 29), GPIO('C', 30),
    GPIO('C', 31), GPIO('C', 26), GPIO('C', 27), GPIO('C',0)
};

static const uint8_t afec1_pins[] = {
    GPIO('B', 2), GPIO('B', 3), GPIO('A', 21), GPIO('A', 22),
    GPIO('C', 1), GPIO('C', 2), GPIO('C', 3), GPIO('C', 4),
    /* Artificially pad this array so we can safely iterate over it */
    GPIO('B', 2), GPIO('B', 2), GPIO('B', 2), GPIO('B', 2),
    GPIO('B', 2), GPIO('B', 2), GPIO('B', 2), GPIO('B', 2)
};

#define ADC_FREQ_MAX 6000000UL
DECL_CONSTANT(ADC_MAX, 4095);

inline struct gpio_adc
gpio_pin_to_afec(uint8_t pin)
{
    int chan;
    Afec* afec_device;

    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(afec0_pins))
            shutdown("Not a valid ADC pin");
        if (afec0_pins[chan] == pin) {
            afec_device = AFEC0;
            break;
        }
        if (afec1_pins[chan] == pin) {
            afec_device = AFEC1;
            break;
        }
    }
    return (struct gpio_adc){.pin=pin, .chan=chan, .afec=afec_device};
}

void
gpio_afec_init(void) {
    afec_enable(AFEC0);
    afec_enable(AFEC1);
    struct afec_config cfg;
    afec_get_config_defaults(&cfg);
    while(afec_init(AFEC0, &cfg) != STATUS_OK) {
        (void)afec_get_latest_value(AFEC0);
    }
    while(afec_init(AFEC1, &cfg) != STATUS_OK) {
        (void)afec_get_latest_value(AFEC1);
    }
    afec_disable_interrupt(AFEC0, AFEC_INTERRUPT_ALL);
    afec_disable_interrupt(AFEC1, AFEC_INTERRUPT_ALL);
    afec_set_trigger(AFEC0, AFEC_TRIG_SW);
    afec_set_trigger(AFEC1, AFEC_TRIG_SW);
}
DECL_INIT(gpio_afec_init);

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    struct gpio_adc adc_pin = gpio_pin_to_afec(pin);
    struct afec_ch_config ch_cfg;

    // Configure channel
    afec_ch_get_config_defaults(&ch_cfg);
    afec_ch_set_config(adc_pin.afec, adc_pin.chan, &ch_cfg);
    // Remove default internal offset from channel
    // See Atmel Appnote AT03078 Section 1.5
    afec_channel_set_analog_offset(adc_pin.afec, adc_pin.chan, 0x800);

    // Enable and calibrate Channel
    afec_channel_enable(adc_pin.afec, adc_pin.chan);
    afec_start_calibration(adc_pin.afec);

    return adc_pin;
}

enum { AFEC_DUMMY=0xff };
uint8_t active_channel_afec0 = AFEC_DUMMY;
uint8_t active_channel_afec1 = AFEC_DUMMY;



inline uint8_t
get_active_afec_channel(Afec* afec) {
    if (afec == AFEC0) {
        return active_channel_afec0;
    }
    return active_channel_afec1;
}

inline void
set_active_afec_channel(Afec* afec, uint8_t chan) {
    if (afec == AFEC0) {
        active_channel_afec0 = chan;
    } else {
        active_channel_afec1 = chan;
    }
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    if (get_active_afec_channel(g.afec) == g.chan) {
        if ((afec_get_interrupt_status(g.afec) & AFEC_ISR_DRDY) && (afec_get_interrupt_status(g.afec) & (1 << g.chan))) {
            // Sample now ready
            return 0;
        } else {
            // Busy
            goto need_delay;
        }
    } else if (get_active_afec_channel(g.afec) != AFEC_DUMMY) {
        goto need_delay;
    }
    afec_channel_disable(g.afec, AFEC_CHANNEL_ALL);
    afec_channel_enable(g.afec, g.chan);
    set_active_afec_channel(g.afec, g.chan);

    for (uint32_t chan = 0; chan < 16; ++chan)
    {
        if ((afec_get_interrupt_status(g.afec) & (1 << chan)) != 0)
        {
            (void)afec_channel_get_value(g.afec, chan);
        }
    }
    afec_start_software_conversion(g.afec);

need_delay:
    return ADC_FREQ_MAX * 10000ULL / CONFIG_CLOCK_FREQ; // about 400 mcu clock cycles or 40 afec cycles
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    set_active_afec_channel(g.afec, AFEC_DUMMY);
    uint32_t result = afec_channel_get_value(g.afec, g.chan);
    return (uint16_t)result & 0xFFF;
}


// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (get_active_afec_channel(g.afec) == g.chan) {
        set_active_afec_channel(g.afec, AFEC_DUMMY);
    }
}
