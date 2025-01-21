// GPIO functions on PRU
//
// Copyright (C) 2017-2020  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/io.h" // readl
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_out_setup
#include "sched.h" // sched_shutdown


/****************************************************************
 * Pin mappings
 ****************************************************************/

#define GPIO(PORT, NUM) ((PORT) * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

struct gpio_regs {
    uint32_t pad_0[77]; // 77*4=308=134h
    volatile uint32_t oe;
    volatile uint32_t datain;
    volatile uint32_t dataout;
    uint32_t pad_140[20];
    volatile uint32_t cleardataout;
    volatile uint32_t setdataout;
};

DECL_ENUMERATION_RANGE("pin", "gpio0_0", GPIO(0, 0), 32);
DECL_ENUMERATION_RANGE("pin", "gpio1_0", GPIO(1, 0), 32);
DECL_ENUMERATION_RANGE("pin", "gpio2_0", GPIO(2, 0), 32);
DECL_ENUMERATION_RANGE("pin", "gpio3_0", GPIO(3, 0), 32);

static struct gpio_regs *digital_regs[] = {
    (void*)0x44e07000, (void*)0x4804c000, (void*)0x481ac000, (void*)0x481ae000
};


/****************************************************************
 * Pin mux handling
 ****************************************************************/

#define MUXPORT(offset) (((offset)-0x800) / 4)

static uint8_t gpio_mux_offset[32 * ARRAY_SIZE(digital_regs)] = {
    // gpio 0
    0xff          , 0xff          , MUXPORT(0x950), MUXPORT(0x954),
    MUXPORT(0x958), MUXPORT(0x95c), 0xff          , MUXPORT(0x964),
    MUXPORT(0x8d0), MUXPORT(0x8d4), MUXPORT(0x8d8), MUXPORT(0x8dc),
    MUXPORT(0x978), MUXPORT(0x97c), MUXPORT(0x980), MUXPORT(0x984),
    0xff          , 0xff          , 0xff          , 0xff          ,
    MUXPORT(0x9b4), 0xff          , MUXPORT(0x820), MUXPORT(0x824),
    0xff          , 0xff          , MUXPORT(0x828), MUXPORT(0x82c),
    0xff          , 0xff          , MUXPORT(0x870), MUXPORT(0x874),

    // gpio 1
    MUXPORT(0x800), MUXPORT(0x804), MUXPORT(0x808), MUXPORT(0x80c),
    MUXPORT(0x810), MUXPORT(0x814), MUXPORT(0x818), MUXPORT(0x81c),
    0xff          , 0xff          , 0xff          , 0xff          ,
    MUXPORT(0x830), MUXPORT(0x834), MUXPORT(0x838), MUXPORT(0x83c),
    MUXPORT(0x840), MUXPORT(0x844), MUXPORT(0x848), MUXPORT(0x84c),
    0xff          , 0xff          , 0xff          , 0xff          ,
    0xff          , 0xff          , 0xff          , 0xff          ,
    MUXPORT(0x878), MUXPORT(0x87c), MUXPORT(0x880), MUXPORT(0x884),

    // gpio 2
    0xff          , MUXPORT(0x88c), MUXPORT(0x890), MUXPORT(0x894),
    MUXPORT(0x898), MUXPORT(0x89c), MUXPORT(0x8a0), MUXPORT(0x8a4),
    MUXPORT(0x8a8), MUXPORT(0x8ac), MUXPORT(0x8b0), MUXPORT(0x8b4),
    MUXPORT(0x8b8), MUXPORT(0x8bc), MUXPORT(0x8c0), MUXPORT(0x8c4),
    MUXPORT(0x8c8), MUXPORT(0x8cc), 0xff          , 0xff          ,
    0xff          , 0xff          , MUXPORT(0x8e0), MUXPORT(0x8e4),
    MUXPORT(0x8e8), MUXPORT(0x8ec), 0xff          , 0xff          ,
    0xff          , 0xff          , 0xff          , 0xff          ,

    // gpio 3
    0xff          , 0xff          , 0xff          , 0xff          ,
    0xff          , 0xff          , 0xff          , 0xff          ,
    0xff          , 0xff          , 0xff          , 0xff          ,
    0xff          , 0xff          , MUXPORT(0x990), MUXPORT(0x994),
    MUXPORT(0x998), MUXPORT(0x99c), MUXPORT(0x9a0), MUXPORT(0x9a4),
    MUXPORT(0x9a8), MUXPORT(0x9ac), 0xff          , 0xff          ,
    0xff          , 0xff          , 0xff          , 0xff          ,
    0xff          , 0xff          , 0xff          , 0xff          ,
};

#define MUXREG(mux_offset) ((volatile uint32_t *)0x44e10800 + mux_offset)

static void
set_pin_mux(uint8_t pin, uint8_t val)
{
    return; // XXX - can not set mux value from PRU

    uint8_t mux_offset = gpio_mux_offset[pin];
    if (mux_offset == 0xff)
        shutdown("Invalid mux pin");
    *MUXREG(mux_offset) = val;
}


/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    struct gpio_regs *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = GPIO2BIT(pin);
    struct gpio_out rv = (struct gpio_out){.reg=&regs->cleardataout, .bit=bit};
    gpio_out_write(rv, val);
    regs->oe &= ~bit;
    set_pin_mux(pin, 0x0f);
    return rv;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    shutdown("PRU does not support push/pull pins");
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    gpio_out_write(g, !(*g.reg & g.bit));
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    volatile uint32_t *reg = g.reg;
    if (val)
        reg++;
    *reg = g.bit;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    struct gpio_regs *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = GPIO2BIT(pin);
    regs->oe |= bit;
    set_pin_mux(pin, pull_up > 0 ? 0x37 : (pull_up < 0 ? 0x27 : 0x2f));
    return (struct gpio_in){ .reg=&regs->datain, .bit=bit };
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    shutdown("PRU does not support push/pull pins");
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return !!(*g.reg & g.bit);
}
