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
#include "hardware/structs/ioqspi.h" // ioqspi_hw
#include "hardware/structs/padsbank0.h" // padsbank0_hw
#include "hardware/structs/pads_qspi.h" // pads_qspi_hw
#include "hardware/structs/sio.h" // sio_hw
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown
#include "autoconf.h" // CONFIG_RP2040_FLASH_NOFLASH


/****************************************************************
 * Pin mappings
 ****************************************************************/

#if CONFIG_RP2040_FLASH_NOFLASH
#define GPIO_MAX 36
#define GPIO_GET_BANK(gpio, a, b) (gpio <= 30 ? (a) : (b))
#define GPIO_GET_PIN(gpio) (gpio <= 30 ? (gpio) : (gpio - 30))
#else
#define GPIO_MAX 30
#define GPIO_GET_BANK(gpio, a, b) (a)
#define GPIO_GET_PIN(gpio) (gpio)
#endif

DECL_ENUMERATION_RANGE("pin", "gpio0", 0, GPIO_MAX);

struct io_status {
    io_rw_32 status;
    io_rw_32 ctrl;
};

// Set the mode and extended function of a pin
void
gpio_peripheral(uint32_t gpio, int func, int pull_up)
{
    uint32_t pin = GPIO_GET_PIN(gpio);
    io_rw_32 *pad = GPIO_GET_BANK(gpio, padsbank0_hw->io , pads_qspi_hw->io);
    struct io_status *io = GPIO_GET_BANK(gpio,
                                        (struct io_status*)(iobank0_hw->io),
                                        (struct io_status*)(ioqspi_hw->io));

    pad[pin] = (PADS_BANK0_GPIO0_IE_BITS
             | (PADS_BANK0_GPIO0_DRIVE_VALUE_4MA << PADS_BANK0_GPIO0_DRIVE_MSB)
             | (pull_up > 0 ? PADS_BANK0_GPIO0_PUE_BITS : 0)
             | (pull_up < 0 ? PADS_BANK0_GPIO0_PDE_BITS : 0));
    io[pin].ctrl = func << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
}

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (pin >= GPIO_MAX)
        goto fail;
    struct gpio_out g = { .pin = pin };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    io_wo_32 *io_oe_set = GPIO_GET_BANK(g.pin, &sio_hw->gpio_oe_set,
                                               &sio_hw->gpio_hi_oe_set);
    uint32_t pin_mask = 1 << GPIO_GET_PIN(g.pin);

    irqstatus_t flag = irq_save();
    gpio_out_write(g, val);
    *io_oe_set = pin_mask;
    gpio_peripheral(g.pin, 5, 0);
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    io_wo_32 *io_togl = GPIO_GET_BANK(g.pin, &sio_hw->gpio_togl,
                                             &sio_hw->gpio_hi_togl);
    uint32_t pin_mask = 1 << GPIO_GET_PIN(g.pin);

    *io_togl = pin_mask;
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    io_wo_32 *io_set = GPIO_GET_BANK(g.pin, &sio_hw->gpio_set,
                                            &sio_hw->gpio_hi_set);
    io_wo_32 *io_clr = GPIO_GET_BANK(g.pin, &sio_hw->gpio_clr,
                                            &sio_hw->gpio_hi_clr);
    uint32_t pin_mask = 1 << GPIO_GET_PIN(g.pin);

    if (val)
        *io_set = pin_mask;
    else
        *io_clr = pin_mask;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (pin >= GPIO_MAX)
        goto fail;
    struct gpio_in g = { .pin = pin };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    io_wo_32 *io_oe_clr = GPIO_GET_BANK(g.pin, &sio_hw->gpio_oe_clr,
                                               &sio_hw->gpio_hi_oe_clr);
    uint32_t pin_mask = 1 << GPIO_GET_PIN(g.pin);

    irqstatus_t flag = irq_save();
    gpio_peripheral(g.pin, 5, pull_up);
    *io_oe_clr = pin_mask;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    io_ro_32 *io_in = GPIO_GET_BANK(g.pin, &sio_hw->gpio_in,
                                           &sio_hw->gpio_hi_in);
    uint32_t pin_mask = 1 << GPIO_GET_PIN(g.pin);

    return !!(*io_in & pin_mask);
}
