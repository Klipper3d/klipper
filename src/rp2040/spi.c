// SPI functions on rp2040
//
// Copyright (C) 2021  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpio.h" // spi_setup, spi_prepare, spi_transfer
#include "command.h" // shutdown"
#include "sched.h" // sched_shutdown"
#include "internal.h" // pclock, gpio_peripheral
#include "hardware/structs/spi.h" // spi_hw_t
#include "hardware/regs/resets.h" // RESETS_RESET_SPI*_BITS

DECL_ENUMERATION("spi_bus", "spi0a", 0);
DECL_CONSTANT_STR("BUS_PINS_spi0a", "gpio0,gpio3,gpio2");
DECL_ENUMERATION("spi_bus", "spi0b", 1);
DECL_CONSTANT_STR("BUS_PINS_spi0b", "gpio4,gpio7,gpio6");
DECL_ENUMERATION("spi_bus", "spi0c", 2);
DECL_CONSTANT_STR("BUS_PINS_spi0c", "gpio16,gpio19,gpio18");
DECL_ENUMERATION("spi_bus", "spi0d", 3);
DECL_CONSTANT_STR("BUS_PINS_spi0d", "gpio20,gpio23,gpio22");

DECL_ENUMERATION("spi_bus", "spi1a", 4);
DECL_CONSTANT_STR("BUS_PINS_spi1a", "gpio8,gpio11,gpio10");
DECL_ENUMERATION("spi_bus", "spi1b", 5);
DECL_CONSTANT_STR("BUS_PINS_spi1b", "gpio12,gpio15,gpio14");
DECL_ENUMERATION("spi_bus", "spi1c", 6);
DECL_CONSTANT_STR("BUS_PINS_spi1c", "gpio24,gpio27,gpio26");

struct spi_info {
    spi_hw_t *spi;
    uint8_t miso_pin, mosi_pin, sck_pin;
    uint32_t pclk;
};

static const struct spi_info spi_bus[] = {
    {spi0_hw, 0,  3,  2,  RESETS_RESET_SPI0_BITS},
    {spi0_hw, 4,  7,  6,  RESETS_RESET_SPI0_BITS},
    {spi0_hw, 16, 19, 18, RESETS_RESET_SPI0_BITS},
    {spi0_hw, 20, 23, 22, RESETS_RESET_SPI0_BITS},

    {spi1_hw, 8,  11, 10, RESETS_RESET_SPI1_BITS},
    {spi1_hw, 12, 15, 14, RESETS_RESET_SPI1_BITS},
    {spi1_hw, 24, 27, 26, RESETS_RESET_SPI1_BITS},
};

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus >= ARRAY_SIZE(spi_bus))
        shutdown("Invalid spi bus");

    if (!is_enabled_pclock(spi_bus[bus].pclk))
            enable_pclock(spi_bus[bus].pclk);

    gpio_peripheral(spi_bus[bus].miso_pin, 1, 1);
    gpio_peripheral(spi_bus[bus].mosi_pin, 1, 0);
    gpio_peripheral(spi_bus[bus].sck_pin, 1, 0);

    uint32_t pclk = get_pclock_frequency(spi_bus[bus].pclk);

    struct spi_config res = {spi_bus[bus].spi, 0, 0};

    uint8_t prescale;
    for (prescale = 2; prescale <= 254; prescale += 2) {
        if (pclk < (prescale + 2) * 256 * rate)
            break;
    }

    uint8_t postdiv;
    for (postdiv = 255; postdiv > 0; --postdiv) {
        if ((pclk / (prescale * postdiv)) > rate)
            break;
    }

    res.cr0 |= postdiv << SPI_SSPCR0_SCR_LSB;
    res.cr0 |= ((mode & 2) != 0) << SPI_SSPCR0_SPO_LSB;
    res.cr0 |= ((mode & 1) != 0) << SPI_SSPCR0_SPH_LSB;
    res.cr0 |= 7 << SPI_SSPCR0_DSS_LSB; // 8bit mode
    res.cpsr = prescale;

    // Enable the peripheral
    spi_bus[bus].spi->cr1 = SPI_SSPCR1_SSE_BITS;

    return res;
}

void
spi_prepare(struct spi_config config)
{
    spi_hw_t *spi = config.spi;
    spi->cr0 = config.cr0;
    spi->cpsr = config.cpsr;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    spi_hw_t *spi = config.spi;
    while (len--) {
        spi->dr = *data;
        while (!(spi->sr & SPI_SSPSR_RNE_BITS))
            ;
        uint8_t rdata = spi->dr;
        if(receive_data)
            *data = rdata;
        data++;
    }
}
