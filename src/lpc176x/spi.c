// SPI support on lpc176x
//
// Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

struct spi_info {
    LPC_SSP_TypeDef *spi;
    uint8_t miso_pin, mosi_pin, sck_pin, pclk;
};

DECL_ENUMERATION("spi_bus", "ssp0", 0);
DECL_CONSTANT_STR("BUS_PINS_ssp0", "P0.17,P0.18,P0.15");
DECL_ENUMERATION("spi_bus", "ssp1", 1);
DECL_CONSTANT_STR("BUS_PINS_ssp1", "P0.8,P0.9,P0.7");

static const struct spi_info spi_bus[] = {
    { LPC_SSP0, GPIO(0, 17), GPIO(0, 18), GPIO(0, 15), PCLK_SSP0 },
    { LPC_SSP1, GPIO(0, 8), GPIO(0, 9), GPIO(0, 7), PCLK_SSP1 },
};

static void
spi_init(uint32_t bus)
{
    static int have_run_init[ARRAY_SIZE(spi_bus)];
    if (have_run_init[bus])
        return;
    have_run_init[bus] = 1;

    // Configure MISO0, MOSI0, SCK0 pins
    gpio_peripheral(spi_bus[bus].miso_pin, 2, 0);
    gpio_peripheral(spi_bus[bus].mosi_pin, 2, 0);
    gpio_peripheral(spi_bus[bus].sck_pin, 2, 0);

    // Setup clock
    enable_pclock(spi_bus[bus].pclk);

    // Set initial registers
    LPC_SSP_TypeDef *spi = spi_bus[bus].spi;
    spi->CR0 = 0x07;
    spi->CPSR = 254;
    spi->CR1 = 1<<1;
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus >= ARRAY_SIZE(spi_bus))
        shutdown("Invalid spi_setup parameters");

    // Make sure bus is enabled
    spi_init(bus);

    // Setup clock rate and mode
    struct spi_config res = {spi_bus[bus].spi, 0, 0};
    uint32_t pclk = get_pclock_frequency(spi_bus[bus].pclk);
    uint32_t div = DIV_ROUND_UP(pclk/2, rate) << 1;
    res.cpsr = div < 2 ? 2 : (div > 254 ? 254 : div);
    res.cr0 = 0x07 | ((mode & 2) << 5) | ((mode & 1) << 7);

    return res;
}

void
spi_prepare(struct spi_config config)
{
    LPC_SSP_TypeDef *spi = config.spi;
    spi->CR0 = config.cr0;
    spi->CPSR = config.cpsr;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    LPC_SSP_TypeDef *spi = config.spi;
    if (receive_data) {
        while (len--) {
            spi->DR = *data;
            // wait for read data to be ready
            while (!(spi->SR & (1<<2)))
                ;
            // get data
            *data++ = spi->DR;
        }
    } else {
        while (len--) {
            spi->DR = *data++;
            // wait for read data to be ready
            while (!(spi->SR & (1<<2)))
                ;
            // read data (to clear receive fifo)
            spi->DR;
        }
    }
}
