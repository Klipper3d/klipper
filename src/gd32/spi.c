// Hardware SPI support for GD32F30x and GD32E23x
//
// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h"
#include "command.h"
#include "gpio.h"
#include "internal.h"
#include "sched.h"

struct spi_info {
    uint32_t spi, clock, pclk;
    uint8_t miso_pin, mosi_pin, sck_pin, function;
};

DECL_ENUMERATION("spi_bus", "spi0", 0);
DECL_CONSTANT_STR("BUS_PINS_spi0", "PA6,PA7,PA5");
DECL_ENUMERATION("spi_bus", "spi1", 1);
DECL_CONSTANT_STR("BUS_PINS_spi1", "PB14,PB15,PB13");

static const struct spi_info spi_bus[] = {
    { SPI0, RCU_SPI0, APB2_FREQ, GPIO('A', 6), GPIO('A', 7), GPIO('A', 5), 0 },
    { SPI1, RCU_SPI1, APB1_FREQ,
      GPIO('B', 14), GPIO('B', 15), GPIO('B', 13), 0 },
};

static void
spi_timeout(uint32_t spi, uint32_t ctl0)
{
    SPI_CTL0(spi) = 0;
    (void)SPI_STAT(spi);
    (void)SPI_DATA(spi);
    SPI_CTL0(spi) = ctl0 | SPI_CTL0_SPIEN;
    shutdown("SPI transfer timeout");
}

static void
spi_wait_status(uint32_t spi, uint32_t mask, uint8_t want_set,
                uint32_t ctl0)
{
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    for (;;) {
        uint32_t stat = SPI_STAT(spi);
        if (!!(stat & mask) == want_set)
            return;
        if (!timer_is_before(timer_read_time(), timeout))
            spi_timeout(spi, ctl0);
    }
}

static void
spi_gpio_setup(const struct spi_info *si)
{
#if CONFIG_MACH_GD32F30X
    gpio_peripheral(si->miso_pin, 1, -1);
    gpio_peripheral(si->mosi_pin, 3, 1);
    gpio_peripheral(si->sck_pin, 3, 1);
#else
    gpio_init_af_set(si->miso_pin, si->function);
    gpio_init_af_set(si->mosi_pin, si->function);
    gpio_init_af_set(si->sck_pin, si->function);
    gpio_init_mode_set(si->miso_pin, GPIO_MODE_AF, GPIO_PUPD_NONE);
    gpio_init_mode_set(si->mosi_pin, GPIO_MODE_AF, GPIO_PUPD_NONE);
    gpio_init_mode_set(si->sck_pin, GPIO_MODE_AF, GPIO_PUPD_NONE);
    gpio_init_output_options_set(si->miso_pin, GPIO_OTYPE_PP);
    gpio_init_output_options_set(si->mosi_pin, GPIO_OTYPE_PP);
    gpio_init_output_options_set(si->sck_pin, GPIO_OTYPE_PP);
#endif
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus >= ARRAY_SIZE(spi_bus) || mode > 3 || !rate)
        shutdown("Invalid SPI setup");
    const struct spi_info *si = &spi_bus[bus];
    if (!is_enable_pclock(si->clock)) {
        enable_pclock(si->clock);
        spi_gpio_setup(si);
    }
    uint32_t div = 2, br = 0;
    while (div < 256 && si->pclk / div > rate) {
        div <<= 1;
        br++;
    }
    uint32_t ctl0 = SPI_CTL0_MSTMOD | SPI_CTL0_SWNSS | SPI_CTL0_SWNSSEN
                    | (br << 3);
    if (mode & 1)
        ctl0 |= SPI_CTL0_CKPH;
    if (mode & 2)
        ctl0 |= SPI_CTL0_CKPL;
    return (struct spi_config){ .spi=(void *)si->spi, .cr0=ctl0, .cpsr=div };
}

void
spi_prepare(struct spi_config config)
{
    uint32_t spi = (uint32_t)config.spi;
    if ((SPI_CTL0(spi) & ~SPI_CTL0_SPIEN) == config.cr0)
        return;
    spi_wait_status(spi, SPI_STAT_TRANS, 0, config.cr0);
    SPI_CTL0(spi) = config.cr0;
    SPI_CTL0(spi) = config.cr0 | SPI_CTL0_SPIEN;
    while (SPI_STAT(spi) & SPI_STAT_RBNE)
        (void)SPI_DATA(spi);
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    uint32_t spi = (uint32_t)config.spi;
    spi_prepare(config);
    while (len--) {
        spi_wait_status(spi, SPI_STAT_TBE, 1, config.cr0);
        SPI_DATA(spi) = *data;
        spi_wait_status(spi, SPI_STAT_RBNE, 1, config.cr0);
        uint8_t value = SPI_DATA(spi);
        if (receive_data)
            *data = value;
        data++;
    }
    spi_wait_status(spi, SPI_STAT_TRANS, 0, config.cr0);
}
