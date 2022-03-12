// GPIO functions on rp2040
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // ffs
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_setup
#include "hardware/structs/iobank0.h" // iobank0_hw
#include "hardware/structs/padsbank0.h" // padsbank0_hw
#include "hardware/structs/sio.h" // sio_hw
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown


/****************************************************************
 * Pin mappings
 ****************************************************************/

DECL_ENUMERATION_RANGE("pin", "gpio0", 0, 30);

// Set the mode and extended function of a pin
void
gpio_peripheral(uint32_t gpio, int func, int pull_up)
{
    gpio_set_function(gpio, (enum gpio_function)func);
    gpio_set_pulls(gpio, (pull_up!=0), (pull_up==0));
}

// Convert a register and bit location back to an integer pin identifier
static int
mask_to_pin(uint32_t mask)
{
    return ffs(mask)-1;
}

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (pin > 30)
        goto fail;
    struct gpio_out g = { .bit=1<<pin };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    int pin = mask_to_pin(g.bit);
    irqstatus_t flag = irq_save();
    gpio_out_write(g, val);
    sio_hw->gpio_oe_set = g.bit;
    gpio_peripheral(pin, GPIO_FUNC_SIO, 0);
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    sio_hw->gpio_togl = g.bit;
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    if (val)
        sio_hw->gpio_set = g.bit;
    else
        sio_hw->gpio_clr = g.bit;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (pin > 30)
        goto fail;
    struct gpio_in g = { .bit=1<<pin };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    int pin = mask_to_pin(g.bit);
    irqstatus_t flag = irq_save();
    gpio_peripheral(pin, GPIO_FUNC_SIO, pull_up);
    sio_hw->gpio_oe_clr = g.bit;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return !!(sio_hw->gpio_in & g.bit);
}

/// \tag::gpio_set_function[]
// Select function for this GPIO, and ensure input/output are enabled
// at the pad.
// This also clears the input/output/irq override bits.
void gpio_set_function(uint gpio, enum gpio_function fn) {
    invalid_params_if(GPIO, gpio >= NUM_BANK0_GPIOS);
    invalid_params_if(GPIO, ((uint32_t)fn
        << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB)
            & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
    // Set input enable on, output disable off
    hw_write_masked(&padsbank0_hw->io[gpio],
                   PADS_BANK0_GPIO0_IE_BITS,
                   PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
    );
    // Zero all fields apart from fsel; we want this IO to do what the
    // peripheral tells it.
    // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
    iobank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
}
/// \end::gpio_set_function[]

enum gpio_function gpio_get_function(uint gpio) {
    invalid_params_if(GPIO, gpio >= NUM_BANK0_GPIOS);
    return (enum gpio_function) ((iobank0_hw->io[gpio].ctrl
    & IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS) >> IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB);
}

// Note that, on RP2040, setting both pulls enables a "bus keep" function,
// i.e. weak pull to whatever is current high/low state of GPIO.
void gpio_set_pulls(uint gpio, bool up, bool down) {
    invalid_params_if(GPIO, gpio >= NUM_BANK0_GPIOS);
    hw_write_masked(
            &padsbank0_hw->io[gpio],
            (bool_to_bit(up) << PADS_BANK0_GPIO0_PUE_LSB)
                | (bool_to_bit(down) << PADS_BANK0_GPIO0_PDE_LSB),
            PADS_BANK0_GPIO0_PUE_BITS | PADS_BANK0_GPIO0_PDE_BITS
    );
}
