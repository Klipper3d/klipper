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
#include "board/misc.h" // timer_is_before


DECL_ENUMERATION("spi_bus", "spi0_gpio0_gpio3_gpio2", 0);
DECL_CONSTANT_STR("BUS_PINS_spi0_gpio0_gpio3_gpio2", "gpio0,gpio3,gpio2");
DECL_ENUMERATION("spi_bus", "spi0_gpio4_gpio7_gpio6", 1);
DECL_CONSTANT_STR("BUS_PINS_spi0_gpio4_gpio7_gpio6", "gpio4,gpio7,gpio6");
DECL_ENUMERATION("spi_bus", "spi0_gpio16_gpio19_gpio18", 2);
DECL_CONSTANT_STR("BUS_PINS_spi0_gpio16_gpio19_gpio18", "gpio16,gpio19,gpio18");
DECL_ENUMERATION("spi_bus", "spi0_gpio20_gpio23_gpio22", 3);
DECL_CONSTANT_STR("BUS_PINS_spi0_gpio20_gpio23_gpio22", "gpio20,gpio23,gpio22");
DECL_ENUMERATION("spi_bus", "spi0_gpio4_gpio3_gpio2", 4);
DECL_CONSTANT_STR("BUS_PINS_spi0_gpio4_gpio3_gpio2", "gpio4,gpio3,gpio2");

DECL_ENUMERATION("spi_bus", "spi1_gpio8_gpio11_gpio10", 5);
DECL_CONSTANT_STR("BUS_PINS_spi1_gpio8_gpio11_gpio10", "gpio8,gpio11,gpio10");
DECL_ENUMERATION("spi_bus", "spi1_gpio12_gpio15_gpio14", 6);
DECL_CONSTANT_STR("BUS_PINS_spi1_gpio12_gpio15_gpio14", "gpio12,gpio15,gpio14");
DECL_ENUMERATION("spi_bus", "spi1_gpio24_gpio27_gpio26", 7);
DECL_CONSTANT_STR("BUS_PINS_spi1_gpio24_gpio27_gpio26", "gpio24,gpio27,gpio26");
DECL_ENUMERATION("spi_bus", "spi1_gpio12_gpio11_gpio10", 8);
DECL_CONSTANT_STR("BUS_PINS_spi1_gpio12_gpio11_gpio10", "gpio12,gpio11,gpio10");

//Deprecated "spi0a" style mappings
DECL_ENUMERATION("spi_bus", "spi0a", 0);
DECL_CONSTANT_STR("BUS_PINS_spi0a", "gpio0,gpio3,gpio2");
DECL_ENUMERATION("spi_bus", "spi0b", 1);
DECL_CONSTANT_STR("BUS_PINS_spi0b", "gpio4,gpio7,gpio6");
DECL_ENUMERATION("spi_bus", "spi0c", 2);
DECL_CONSTANT_STR("BUS_PINS_spi0c", "gpio16,gpio19,gpio18");
DECL_ENUMERATION("spi_bus", "spi0d", 3);
DECL_CONSTANT_STR("BUS_PINS_spi0d", "gpio20,gpio23,gpio22");

DECL_ENUMERATION("spi_bus", "spi1a", 5);
DECL_CONSTANT_STR("BUS_PINS_spi1a", "gpio8,gpio11,gpio10");
DECL_ENUMERATION("spi_bus", "spi1b", 6);
DECL_CONSTANT_STR("BUS_PINS_spi1b", "gpio12,gpio15,gpio14");
DECL_ENUMERATION("spi_bus", "spi1c", 7);
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
    {spi0_hw, 4,  3,  2,  RESETS_RESET_SPI0_BITS},

    {spi1_hw, 8,  11, 10, RESETS_RESET_SPI1_BITS},
    {spi1_hw, 12, 15, 14, RESETS_RESET_SPI1_BITS},
    {spi1_hw, 24, 27, 26, RESETS_RESET_SPI1_BITS},
    {spi1_hw, 12, 11, 10, RESETS_RESET_SPI1_BITS},
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
    if (spi->cr0 == config.cr0 && spi->cpsr == config.cpsr)
        return;
    uint32_t diff = spi->cr0 ^ config.cr0;
    spi->cr1 = 0;
    spi->cr0 = config.cr0;
    spi->cpsr = config.cpsr;
    spi->cr1 = SPI_SSPCR1_SSE_BITS;
    // Give time for state to update before caller changes CS pin
    uint32_t end = timer_read_time() + timer_from_us(1);
    if (diff & SPI_SSPCR0_SPO_BITS)
        while (timer_is_before(timer_read_time(), end))
            ;
}

#define MAX_FIFO 8 // Max rx fifo size (don't tx past this size)

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    uint8_t *wptr = data, *end = data + len;
    spi_hw_t *spi = config.spi;
    while (data < end) {
        uint32_t sr = spi->sr & (SPI_SSPSR_TNF_BITS | SPI_SSPSR_RNE_BITS);
        if (sr == SPI_SSPSR_TNF_BITS && wptr < end && wptr < data + MAX_FIFO)
            spi->dr = *wptr++;
        if (!(sr & SPI_SSPSR_RNE_BITS))
            continue;
        uint8_t rdata = spi->dr;
        if (receive_data)
            *data = rdata;
        data++;
    }
    // Wait for any remaining SCLK updates before returning
    while ((spi->sr & (SPI_SSPSR_TFE_BITS|SPI_SSPSR_BSY_BITS))
           != SPI_SSPSR_TFE_BITS)
        ;
}
