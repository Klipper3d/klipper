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
#include "autoconf.h" // spidev
#include "board/armcm_boot.h" // irq
#include "spi_xra1403.h" // XRA_BANKS
#include "pico/assert.h" //
#include "pico/types.h" // bool_to_bit
#include "hardware/sync.h" // get core num
#include "hardware/structs/sio.h" // sio_hw

#if CONFIG_HAVE_EGPIO
#include "spi_xra1403.h" // spi0 device
#include "spi_vdev.h" // spidev
#define SPI_SEND_RECEIVE 1
#define SPI_SEND 0
static struct spidev_s spi0;
#endif

/****************************************************************
 * Pin mappings
 ****************************************************************/

#if CONFIG_HAVE_EGPIO
    // See spi_xra1403.h -- Pins 0,2-5 are used for SPI0, CS0, and Interrupt
    // GPIOs 30-45 Are on GPIO Expander chip
    #define GPIO_LAST 29
    #define GPIO_COUNT 46
    #define IS_EXPANDER(pin) ((pin)>GPIO_LAST)
    #define MAP_PIN_TO_EXP(pin) ((pin)-(GPIO_LAST+1))
    DECL_ENUMERATION("pin", "gpio1", 1);
    DECL_ENUMERATION_RANGE("pin", "gpio6", 6, 40);
    // Shadow registers for SPI device
    volatile struct egpio_cfg exp_bus[XRA_BANKS];

/****************************************************************
 * Internal functions to read/write to SPI GPIO Expander
 ****************************************************************/
    uint8_t xra_read(uint8_t bus, uint8_t reg) {
        struct egpio_msg msg = {XRA_MSG(XRA_READ, bus, reg, 0x00)};
        spi_prepare(spi0.spi_config);
        gpio_out_write(spi0.pin, 0);
        spi_transfer(spi0.spi_config, SPI_SEND_RECEIVE, XRA_MSG_SIZE
            , (uint8_t *)&msg);
        gpio_out_write(spi0.pin, 1);

        return (uint8_t)(msg.dat);
    }
    void xra_write(uint8_t bus, uint8_t reg, uint8_t data) {
        struct egpio_msg msg = {XRA_MSG(XRA_WRITE, bus, reg, data)};
        spi_prepare(spi0.spi_config);
        gpio_out_write(spi0.pin, 0);
        spi_transfer(spi0.spi_config, SPI_SEND, XRA_MSG_SIZE
            , (uint8_t *)&msg);
        gpio_out_write(spi0.pin, 1);
    }
    void
    gpio_irq_handler(void) {
        io_irq_ctrl_hw_t *irq_ctrl_base = get_core_num() ?
            &iobank0_hw->proc1_irq_ctrl : &iobank0_hw->proc0_irq_ctrl;
        for (uint32_t gpio = 0; gpio < NUM_BANK0_GPIOS; gpio++) {
            io_ro_32 *status_reg = &irq_ctrl_base->ints[gpio / 8];
            uint32_t events = (*status_reg >> 4 * (gpio % 8)) & 0xf;
            if (events) {
                gpio_acknowledge_irq(gpio, events);
                // Interrupt is from gpio expander --
                //   Read registers for values and clear interrupts
                if( SPI0_INTCS10_PIN == gpio ) {
                    for(int b =0; b < XRA_BANKS; b++) {
                        exp_bus[b].val = xra_read(b, XRA_GSR);
                    }
                }
            }
        }
    }
    void
    egpio_init(void) {
        // device CS pin - command_config_spi
        spi0.pin = gpio_out_setup(SPI0_CS10_PIN, 1);
        spi0.flags |= SF_HAVE_PIN;

        // define bus features - command_spi_set_bus
        spi0.spi_config = spi_setup(SPI0_PICKSIX_BUS_OID, SPI0_PICKSIX_MODE
            , MAX_XRA_CLK);
        spi0.flags |= SF_HARDWARE;

        // Initial state for gpio expansion - inputs, no pullup, no interrupt
        exp_bus[0].io  = exp_bus[1].io  = 0xff;
        exp_bus[0].pul = exp_bus[1].pul = 0x00;
        exp_bus[0].ien = exp_bus[1].ien = 0x00;
        exp_bus[0].val = exp_bus[1].val = 0x00;

        for(int i=0; i<XRA_BANKS; i++) {
            xra_write(i,XRA_GCR,exp_bus[i].io);
            xra_write(i,XRA_PUR,exp_bus[i].pul);
            xra_write(i,XRA_IER,exp_bus[i].ien);
            xra_write(i,XRA_OCR,exp_bus[i].val);
        }
        // Setup a gpio callback vector
        armcm_enable_irq(gpio_irq_handler, IO_IRQ_BANK0_IRQn, 3);
    }
    DECL_INIT(egpio_init);
#else
    #define IS_EXPANDER(pin) (false)
    #define GPIO_COUNT 30
    DECL_ENUMERATION_RANGE("pin", "gpio0", 0, 30);
#endif

// Set the mode and extended function of a pin
void
gpio_peripheral(uint32_t gpio, int func, int pull_up)
{
    gpio_set_function(gpio, (enum gpio_function)func);
    gpio_set_pulls(gpio, (pull_up!=0), (pull_up==0));
}

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (pin > GPIO_COUNT)
        goto fail;
    struct gpio_out g;
    SET_BIT(g,bit,pin);
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

#if CONFIG_HAVE_EGPIO
void gpio_acknowledge_irq(uint32_t gpio, uint32_t events) {
    iobank0_hw->intr[gpio / 8] = events << 4 * (gpio % 8);
}

static void _gpio_set_irq_enabled(uint32_t gpio, uint32_t events, \
    bool enabled, io_irq_ctrl_hw_t *irq_ctrl_base) {
    // Clear stale events which might cause immediate spurious handler entry
    gpio_acknowledge_irq(gpio, events);

    io_rw_32 *en_reg = &irq_ctrl_base->inte[gpio / 8];
    events <<= 4 * (gpio % 8);

    if (enabled)
        hw_set_bits(en_reg, events);
    else
        hw_clear_bits(en_reg, events);
}

void gpio_set_irq_enabled(uint32_t gpio, uint32_t events, bool enabled) {
    // Separate mask/force/status per-core, so check which core called, and
    // set the relevant IRQ controls.
    io_irq_ctrl_hw_t *irq_ctrl_base = get_core_num() ?
        &iobank0_hw->proc1_irq_ctrl : &iobank0_hw->proc0_irq_ctrl;
    _gpio_set_irq_enabled(gpio, events, enabled, irq_ctrl_base);
}

void
gpio_in_setup_interrupt(uint32_t pin, uint8_t val) {
    if( unlikely(IS_EXPANDER(pin)) ) {
        uint8_t bank = XRA_PIN2BANK(MAP_PIN_TO_EXP(pin));
        uint8_t val = exp_bus[bank].ien;
        XRA_SET_PIN(exp_bus[bank].ien,MAP_PIN_TO_EXP(pin),1);
        if(val != exp_bus[bank].ien) {
            // Update interrupt pins
            xra_write(bank,XRA_IER,exp_bus[bank].ien);
        }
        // Setup pin for gpio expander -
        //   input, pullup enabled, interrupt enabled
        gpio_peripheral(SPI0_INTCS10_PIN, GPIO_FUNC_SIO, 1);
        gpio_set_irq_enabled(SPI0_INTCS10_PIN, GPIO_IRQ_LEVEL_LOW, true);
    } else
    {
        gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL \
        | GPIO_IRQ_EDGE_RISE, !!(val));
    }
}
#endif

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    int pin = MASK_TO_PIN(g,bit);
    irqstatus_t flag = irq_save();
#if CONFIG_HAVE_EGPIO
    if( unlikely(IS_EXPANDER(pin)) ) {
        // Get current output values
        uint8_t curv = exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val;
        // Set output value
        XRA_SET_PIN(exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val
            ,MAP_PIN_TO_EXP(pin),val);
        // Set control as output
        uint8_t cur = exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].io;
        exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].io
            &= ~XRA_BIT_PIN(MAP_PIN_TO_EXP(pin));
        if( cur != exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].io)
            xra_write(XRA_PIN2BANK(MAP_PIN_TO_EXP(pin)),XRA_GCR
                ,exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].io);
        // Write output values
        if(curv != exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val)
            xra_write(XRA_PIN2BANK(MAP_PIN_TO_EXP(pin)),XRA_OCR
                ,exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val);
    } else {
        gpio_out_write(g, val);
        sio_hw->gpio_oe_set = g.bit;
        gpio_peripheral(pin, GPIO_FUNC_SIO, 0);
    }
#else
    gpio_out_write(g, val);
    sio_hw->gpio_oe_set = g.bit;
    gpio_peripheral(pin, GPIO_FUNC_SIO, 0);
#endif
    irq_restore(flag);
}

/* Perform an atomic bitwise XOR on GPIO_OUT, i.e. GPIO_OUT ^= wdata
 */
void
gpio_out_toggle_noirq(struct gpio_out g)
{
#if CONFIG_HAVE_EGPIO
    int pin = MASK_TO_PIN(g,bit);
    if( unlikely(IS_EXPANDER(pin)) ) {
        exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val
            ^= XRA_BIT_PIN(MAP_PIN_TO_EXP(pin));
        xra_write(XRA_PIN2BANK(MAP_PIN_TO_EXP(pin)), XRA_OCR
            , exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val);
    } else {
        sio_hw->gpio_togl = g.bit;
    }
#else
    sio_hw->gpio_togl = g.bit;
#endif
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
#if CONFIG_HAVE_EGPIO
    int pin = MASK_TO_PIN(g,bit);
    if( unlikely(IS_EXPANDER(pin)) ) {
        XRA_SET_PIN(exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val
            , MAP_PIN_TO_EXP(pin),val);
        xra_write(XRA_PIN2BANK(MAP_PIN_TO_EXP(pin)), XRA_OCR
            , exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val);
    } else {
        if (val)
            sio_hw->gpio_set = g.bit;
        else
            sio_hw->gpio_clr = g.bit;
    }
#else
    if (val)
        sio_hw->gpio_set = g.bit;
    else
        sio_hw->gpio_clr = g.bit;
#endif
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (pin > GPIO_COUNT)
        goto fail;
    struct gpio_in g;
    SET_BIT(g,bit,pin);
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    int pin = MASK_TO_PIN(g,bit);
    irqstatus_t flag = irq_save();
    gpio_peripheral(pin, GPIO_FUNC_SIO, pull_up);
#if CONFIG_HAVE_EGPIO
    // Want to enable interrupts only for Expander
    //  gpio and keep polling off the spi bus
    if(unlikely(IS_EXPANDER(pin)) ) {
        gpio_in_setup_interrupt(pin, 1);
    }
#endif
    sio_hw->gpio_oe_clr = g.bit;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
#if CONFIG_HAVE_EGPIO
    int pin = MASK_TO_PIN(g,bit);
    if( IS_EXPANDER(pin) ) {
        uint8_t mapped_bit = 0x00;
        XRA_SET_PIN(mapped_bit,MAP_PIN_TO_EXP(pin),1);
        return !!( exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(pin))].val
            & mapped_bit );
    } else {
        return !!(sio_hw->gpio_in & g.bit);
    }
#endif
    return !!(sio_hw->gpio_in & g.bit);
}

/// \tag::gpio_set_function[]
// Select function for this GPIO, and ensure input/output are
// enabled at the pad. This also clears the input/output/irq
// override bits.
void gpio_set_function(uint32_t gpio, enum gpio_function fn) {
    invalid_params_if(GPIO, gpio >= NUM_BANK0_GPIOS);
    invalid_params_if(GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB)
        & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
#if CONFIG_HAVE_EGPIO
    if(unlikely(IS_EXPANDER(gpio)) ) {
        // Set as input
        uint8_t cur = exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].io;
        exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].io
            |= XRA_BIT_PIN(MAP_PIN_TO_EXP(gpio));
        if(cur != exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].io)
            xra_write(XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio)),XRA_GCR
                ,exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].io);
    } else {
        // Set input enable on, output disable off
        hw_write_masked(&padsbank0_hw->io[gpio],
                    PADS_BANK0_GPIO0_IE_BITS,
                    PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
        );
        // Zero all fields apart from fsel; we want this IO to do what the
        // peripheral tells it. This doesn't affect e.g. pullup/pulldown,
        // as these are in pad controls.
        iobank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    }
#else
    // Set input enable on, output disable off
    hw_write_masked(&padsbank0_hw->io[gpio],
                   PADS_BANK0_GPIO0_IE_BITS,
                   PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
    );
    // Zero all fields apart from fsel; we want this IO to do what the
    // peripheral tells it. This doesn't affect e.g. pullup/pulldown,
    // as these are in pad controls.
    iobank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
#endif
}
/// \end::gpio_set_function[]

enum gpio_function gpio_get_function(uint32_t gpio) {
    invalid_params_if(GPIO, gpio >= NUM_BANK0_GPIOS);
#if CONFIG_HAVE_EGPIO
    if(unlikely(IS_EXPANDER(gpio)) ) {
        return GPIO_FUNC_SIO;
    } else {
        return (enum gpio_function) ((iobank0_hw->io[gpio].ctrl
            & IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS)
                >> IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB);
    }
#else
    return (enum gpio_function) ((iobank0_hw->io[gpio].ctrl
        & IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS)
            >> IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB);
#endif
}

// Note that, on RP2040, setting both pulls enables a "bus keep" function,
// i.e. weak pull to whatever is current high/low state of GPIO.
void gpio_set_pulls(uint32_t gpio, bool up, bool down) {
    invalid_params_if(GPIO, gpio >= NUM_BANK0_GPIOS);
#if CONFIG_HAVE_EGPIO
    if( unlikely(IS_EXPANDER(gpio)) ) {
        uint8_t cur = exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].pul;
        exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].pul = up
            ? exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].pul
                | XRA_BIT_PIN(MAP_PIN_TO_EXP(gpio))
            : exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].pul
                & ~(XRA_BIT_PIN(MAP_PIN_TO_EXP(gpio)));
        if( cur != exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].pul)
            xra_write(XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio)), XRA_PUR
                , exp_bus[XRA_PIN2BANK(MAP_PIN_TO_EXP(gpio))].pul);
    } else {
        hw_write_masked(
            &padsbank0_hw->io[gpio],
            (bool_to_bit(up) << PADS_BANK0_GPIO0_PUE_LSB) | (bool_to_bit(down)
                << PADS_BANK0_GPIO0_PDE_LSB),
            PADS_BANK0_GPIO0_PUE_BITS | PADS_BANK0_GPIO0_PDE_BITS
        );
    }
#else
    hw_write_masked(
        &padsbank0_hw->io[gpio],
        (bool_to_bit(up) << PADS_BANK0_GPIO0_PUE_LSB) | (bool_to_bit(down)
            << PADS_BANK0_GPIO0_PDE_LSB),
        PADS_BANK0_GPIO0_PUE_BITS | PADS_BANK0_GPIO0_PDE_BITS
    );
#endif
}
