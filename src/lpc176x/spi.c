// SPI support on lpc176x
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // LPC_SSP0
#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

DECL_ENUMERATION("spi_bus", "ssp0", 0);
DECL_CONSTANT_STR("BUS_PINS_ssp0", "P0.17,P0.18,P0.15");

static void
spi_init(void)
{
    static int have_run_init;
    if (have_run_init)
        return;
    have_run_init = 1;

    // Configure MISO0, MOSI0, SCK0 pins
    gpio_peripheral(GPIO(0, 17), 2, 0);
    gpio_peripheral(GPIO(0, 18), 2, 0);
    gpio_peripheral(GPIO(0, 15), 2, 0);

    // Setup clock
    enable_pclock(PCLK_SSP0);

    // Set initial registers
    LPC_SSP0->CR0 = 0x07;
    LPC_SSP0->CPSR = 254;
    LPC_SSP0->CR1 = 1<<1;
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus)
        shutdown("Invalid spi_setup parameters");

    // Make sure bus is enabled
    spi_init();

    // Setup clock rate and mode
    struct spi_config res = {0, 0};
    uint32_t pclk = SystemCoreClock;
    uint32_t div = DIV_ROUND_UP(pclk/2, rate) << 1;
    res.cpsr = div < 2 ? 2 : (div > 254 ? 254 : div);
    res.cr0 = 0x07 | (mode << 6);

    return res;
}

void
spi_prepare(struct spi_config config)
{
    LPC_SSP0->CR0 = config.cr0;
    LPC_SSP0->CPSR = config.cpsr;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    if (receive_data) {
        while (len--) {
            LPC_SSP0->DR = *data;
            // wait for read data to be ready
            while (!(LPC_SSP0->SR & (1<<2)))
                ;
            // get data
            *data++ = LPC_SSP0->DR;
        }
    } else {
        while (len--) {
            LPC_SSP0->DR = *data++;
            // wait for read data to be ready
            while (!(LPC_SSP0->SR & (1<<2)))
                ;
            // read data (to clear receive fifo)
            LPC_SSP0->DR;
        }
    }
}
