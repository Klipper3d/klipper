// SAM4e8e GPIO port
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpio.h"
#include "sam4e.h"

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

static Pio * const digital_regs[] = {
    PIOA, PIOB, PIOC, PIOD, PIOE
};

void
gpio_set_peripheral(char bank, const uint32_t bit, char ptype, uint32_t pull_up) {

    Pio *regs = digital_regs[bank - 'A'];
    regs ->PIO_IDR = bit;

    // Enable peripheral for pin
    uint32_t sr;

    switch (ptype) {
    case 'A':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] &= (~bit & sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] &= (~bit & sr);
        break;
    case 'B':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] = (bit | sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] &= (~bit & sr);
        break;
    case 'C':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] &= (~bit & sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] = (bit | sr);
        break;
    case 'D':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] = (bit | sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] = (bit | sr);
        break;
    }

    // Disable pin in IO controller
    regs->PIO_PDR = bit;

    // Set pullup
    if (pull_up) {
        regs->PIO_PUER = bit;
    } else {
        regs->PIO_PUDR = bit;
    }
}

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    uint32_t port = GPIO2PORT(pin);
    Pio *regs = digital_regs[port];
    uint32_t bank_id = ID_PIOA + port;
    if ((PMC->PMC_PCSR0 & (1u << bank_id)) == 0) {
        PMC->PMC_PCER0 = 1 << bank_id;
    }
    struct gpio_out g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    Pio *regs = g.regs;
    irqstatus_t flag = irq_save();
    if (val)
        regs->PIO_SODR = g.bit;
    else
        regs->PIO_CODR = g.bit;
    regs->PIO_OER = g.bit;
    regs->PIO_OWER = g.bit;
    regs->PIO_PER = g.bit;
    regs->PIO_PUDR = g.bit;
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    Pio *regs = g.regs;
    regs->PIO_ODSR ^= g.bit;
}

void
gpio_out_toggle(struct gpio_out g)
{
    irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    irq_restore(flag);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    Pio *regs = g.regs;
    if (val)
        regs->PIO_SODR = g.bit;
    else
        regs->PIO_CODR = g.bit;
}

struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    uint32_t port = GPIO2PORT(pin);
    Pio *regs = digital_regs[port];
    uint32_t bank_id = ID_PIOA + port;
    if ((PMC->PMC_PCSR0 & (1u << bank_id)) == 0) {
        PMC->PMC_PCER0 = 1 << bank_id;
    }
    struct gpio_in g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    Pio *regs = g.regs;
    irqstatus_t flag = irq_save();
    regs->PIO_IDR = g.bit;
    if (pull_up)
        regs->PIO_PUER = g.bit;
    else
        regs->PIO_PUDR = g.bit;
    regs->PIO_ODR = g.bit;
    regs->PIO_PER = g.bit;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    Pio *regs = g.regs;
    return !!(regs->PIO_PDSR & g.bit);
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

int
init_afec(Afec* afec) {

    // Enable PMC
    if (afec == AFEC0)
        PMC->PMC_PCER0 = 1 << ID_AFEC0;
    else
        PMC->PMC_PCER0 = 1 << ID_AFEC1;

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
    struct gpio_adc adc_pin = gpio_pin_to_afec(pin);
    Afec *afec = adc_pin.afec;

    //config channel
    uint32_t reg = afec->AFE_DIFFR;
    reg &= ~(1u << adc_pin.chan);
    afec->AFE_DIFFR = reg;
    reg = afec->AFE_CGR;
    reg &= ~(0x03u << (2 * adc_pin.chan));
    reg |= 1 << (2 * adc_pin.chan);
    afec->AFE_CGR = reg;

    // Configure channel
    // afec_ch_get_config_defaults(&ch_cfg);
    // afec_ch_set_config(adc_pin.afec, adc_pin.chan, &ch_cfg);
    // Remove default internal offset from channel
    // See Atmel Appnote AT03078 Section 1.5
    afec->AFE_CSELR = adc_pin.chan;
    afec->AFE_COCR = (0x800 & AFE_COCR_AOFF_Msk);

    // Enable and calibrate Channel
    afec->AFE_CHER = 1 << adc_pin.chan;

    reg = afec->AFE_CHSR;
    afec->AFE_CDOR = reg;
    afec->AFE_CR = AFE_CR_AUTOCAL;

    return adc_pin;
}

enum { AFE_DUMMY=0xff };
uint8_t active_channel_afec0 = AFE_DUMMY;
uint8_t active_channel_afec1 = AFE_DUMMY;

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
    Afec* afec = g.afec;
    if (get_active_afec_channel(afec) == g.chan) {
        if ((afec->AFE_ISR & AFE_ISR_DRDY) && (afec->AFE_ISR & (1 << g.chan))) {
            // Sample now ready
            return 0;
        } else {
            // Busy
            goto need_delay;
        }
    } else if (get_active_afec_channel(g.afec) != AFE_DUMMY) {
        goto need_delay;
    }

    afec->AFE_CHDR = 0x803F; // Disable all channels
    afec->AFE_CHER = 1 << g.chan;

    set_active_afec_channel(afec, g.chan);

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
    return ADC_FREQ_MAX * 10000ULL / CONFIG_CLOCK_FREQ; // about 400 mcu clock cycles or 40 afec cycles
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    Afec *afec = g.afec;
    set_active_afec_channel(g.afec, AFE_DUMMY);
    afec->AFE_CSELR = g.chan;
    return afec->AFE_CDR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (get_active_afec_channel(g.afec) == g.chan) {
        set_active_afec_channel(g.afec, AFE_DUMMY);
    }
}
