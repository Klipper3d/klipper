// SPI functions on STM32F4
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

DECL_ENUMERATION("spi_bus", "spi2", 0);
DECL_CONSTANT_STR("BUS_PINS_spi2", "PB14,PB15,PB13");

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus)
        shutdown("Invalid spi bus");

    // Enable SPI
    if (!is_enabled_pclock(SPI2_BASE)) {
        enable_pclock(SPI2_BASE);
        gpio_peripheral(GPIO('B', 14), GPIO_FUNCTION(5), 1);
        gpio_peripheral(GPIO('B', 15), GPIO_FUNCTION(5), 0);
        gpio_peripheral(GPIO('B', 13), GPIO_FUNCTION(5), 0);
    }

    // Calculate CR1 register
    uint32_t pclk = get_pclock_frequency(SPI2_BASE);
    uint32_t div = 0;
    while ((pclk >> (div + 1)) > rate && div < 7)
        div++;
    uint32_t cr1 = ((mode << SPI_CR1_CPHA_Pos) | (div << SPI_CR1_BR_Pos)
                    | SPI_CR1_SPE | SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI);

    return (struct spi_config){ .spi_cr1 = cr1 };
}

void
spi_prepare(struct spi_config config)
{
    SPI2->CR1 = config.spi_cr1;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    while (len--) {
        SPI2->DR = *data;
        while (!(SPI2->SR & SPI_SR_RXNE))
            ;
        uint8_t rdata = SPI2->DR;
        if (receive_data)
            *data = rdata;
        data++;
    }
}
