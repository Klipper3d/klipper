// SAM4e8e Analog Front-End Converter (AFEC) support
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "command.h" // shutdown
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sam4e.h" // AFEC0
#include "sched.h" // sched_shutdown

static const uint8_t afec_pins[] = {
    //remove first channel, since it offsets the channel number: GPIO('A', 8),
    GPIO('A', 17), GPIO('A', 18), GPIO('A', 19),
    GPIO('A', 20), GPIO('B', 0),  GPIO('B', 1), GPIO('C', 13),
    GPIO('C', 15), GPIO('C', 12), GPIO('C', 29), GPIO('C', 30),
    GPIO('C', 31), GPIO('C', 26), GPIO('C', 27), GPIO('C',0),
    // AFEC1
    GPIO('B', 2), GPIO('B', 3), GPIO('A', 21), GPIO('A', 22),
    GPIO('C', 1), GPIO('C', 2), GPIO('C', 3), GPIO('C', 4),
};

#define AFEC1_START 15 // The first 15 pins are on afec0

static inline struct gpio_adc
pin_to_gpio_adc(uint8_t pin)
{
    int chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(afec_pins))
            shutdown("Not a valid ADC pin");
        if (afec_pins[chan] == pin) {
            break;
        }
    }
    return (struct gpio_adc){ .chan=chan };
}

static inline Afec *
gpio_adc_to_afec(struct gpio_adc g)
{
    return (g.chan >= AFEC1_START ? AFEC1 : AFEC0);
}

static inline uint32_t
gpio_adc_to_afec_chan(struct gpio_adc g)
{
    return (g.chan >= AFEC1_START ? g.chan - AFEC1_START : g.chan);
}

#define ADC_FREQ_MAX 6000000UL
DECL_CONSTANT("ADC_MAX", 4095);

static int
init_afec(Afec* afec) {

    // Enable PMC
    enable_pclock(afec == AFEC0 ? ID_AFEC0 : ID_AFEC1);

    // If busy, return busy
    if ((afec->AFE_ISR & AFE_ISR_DRDY) == AFE_ISR_DRDY) {
        return -1;
    }

    // Reset
    afec->AFE_CR = AFE_CR_SWRST;

    // Configure afec
    afec->AFE_MR = AFE_MR_ANACH_ALLOWED | \
                    AFE_MR_PRESCAL (SystemCoreClock / (2 * ADC_FREQ_MAX) -1) | \
                    AFE_MR_SETTLING_AST3 | \
                    AFE_MR_TRACKTIM(2) | \
                    AFE_MR_TRANSFER(1) | \
                    AFE_MR_STARTUP_SUT64;
    afec->AFE_EMR = AFE_EMR_TAG | \
                     AFE_EMR_RES_NO_AVERAGE | \
                     AFE_EMR_STM;
    afec->AFE_ACR = AFE_ACR_IBCTL(1);

    // Disable interrupts
    afec->AFE_IDR = 0xDF00803F;

    // Disable SW triggering
    uint32_t mr = afec->AFE_MR;

    mr &= ~(AFE_MR_TRGSEL_Msk | AFE_MR_TRGEN | AFE_MR_FREERUN_ON);
    mr |= AFE_MR_TRGEN_DIS;
    afec->AFE_MR = mr;

    return 0;
}

void
gpio_afec_init(void) {

    while(init_afec(AFEC0) != 0) {
        (void)(AFEC0->AFE_LCDR & AFE_LCDR_LDATA_Msk);
    }
    while(init_afec(AFEC1) != 0) {
        (void)(AFEC1->AFE_LCDR & AFE_LCDR_LDATA_Msk);
    }

}
DECL_INIT(gpio_afec_init);

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    struct gpio_adc adc_pin = pin_to_gpio_adc(pin);
    Afec *afec = gpio_adc_to_afec(adc_pin);
    uint32_t afec_chan = gpio_adc_to_afec_chan(adc_pin);

    //config channel
    uint32_t reg = afec->AFE_DIFFR;
    reg &= ~(1u << afec_chan);
    afec->AFE_DIFFR = reg;
    reg = afec->AFE_CGR;
    reg &= ~(0x03u << (2 * afec_chan));
    reg |= 1 << (2 * afec_chan);
    afec->AFE_CGR = reg;

    // Configure channel
    // afec_ch_get_config_defaults(&ch_cfg);
    // afec_ch_set_config(afec, afec_chan, &ch_cfg);
    // Remove default internal offset from channel
    // See Atmel Appnote AT03078 Section 1.5
    afec->AFE_CSELR = afec_chan;
    afec->AFE_COCR = (0x800 & AFE_COCR_AOFF_Msk);

    // Enable and calibrate Channel
    afec->AFE_CHER = 1 << afec_chan;

    reg = afec->AFE_CHSR;
    afec->AFE_CDOR = reg;
    afec->AFE_CR = AFE_CR_AUTOCAL;

    return adc_pin;
}

enum { AFE_DUMMY=0xff };
uint8_t active_channel = AFE_DUMMY;

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    Afec *afec = gpio_adc_to_afec(g);
    uint32_t afec_chan = gpio_adc_to_afec_chan(g);
    if (active_channel == g.chan) {
        if ((afec->AFE_ISR & AFE_ISR_DRDY)
            && (afec->AFE_ISR & (1 << afec_chan))) {
            // Sample now ready
            return 0;
        } else {
            // Busy
            goto need_delay;
        }
    } else if (active_channel != AFE_DUMMY) {
        goto need_delay;
    }

    afec->AFE_CHDR = 0x803F; // Disable all channels
    afec->AFE_CHER = 1 << afec_chan;

    active_channel = g.chan;

    for (uint32_t chan = 0; chan < 16; ++chan)
    {
        if ((afec->AFE_ISR & (1 << chan)) != 0)
        {
            afec->AFE_CSELR = chan;
            (void)(afec->AFE_CDR);
        }
    }
    afec->AFE_CR = AFE_CR_START;

need_delay:
    // about 400 mcu clock cycles or 40 afec cycles
    return ADC_FREQ_MAX * 10000ULL / CONFIG_CLOCK_FREQ;
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    Afec *afec = gpio_adc_to_afec(g);
    uint32_t afec_chan = gpio_adc_to_afec_chan(g);
    active_channel = AFE_DUMMY;
    afec->AFE_CSELR = afec_chan;
    return afec->AFE_CDR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (active_channel == g.chan) {
        active_channel = AFE_DUMMY;
    }
}
