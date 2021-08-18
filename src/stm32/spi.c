// SPI functions on STM32
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/io.h" // readb, writeb
#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

struct spi_info {
    SPI_TypeDef *spi;
    uint8_t miso_pin, mosi_pin, sck_pin, function;
};

DECL_ENUMERATION("spi_bus", "spi2", 0);
DECL_CONSTANT_STR("BUS_PINS_spi2", "PB14,PB15,PB13");
DECL_ENUMERATION("spi_bus", "spi1", 1);
DECL_CONSTANT_STR("BUS_PINS_spi1", "PA6,PA7,PA5");
DECL_ENUMERATION("spi_bus", "spi1a", 2);
DECL_CONSTANT_STR("BUS_PINS_spi1a", "PB4,PB5,PB3");
#if CONFIG_MACH_STM32F0 || CONFIG_MACH_STM32F2 || CONFIG_MACH_STM32F4 || CONFIG_MACH_STM32H7
 DECL_ENUMERATION("spi_bus", "spi2a", 3);
 DECL_CONSTANT_STR("BUS_PINS_spi2a", "PC2,PC3,PB10");
#endif
#ifdef SPI3
 DECL_ENUMERATION("spi_bus", "spi3", 4);
 DECL_CONSTANT_STR("BUS_PINS_spi3", "PB4,PB5,PB3");
 #if CONFIG_MACH_STM32F4 || CONFIG_MACH_STM32H7
  DECL_ENUMERATION("spi_bus", "spi3a", 5);
  DECL_CONSTANT_STR("BUS_PINS_spi3a", "PC11,PC12,PC10");
  #ifdef SPI4
   DECL_ENUMERATION("spi_bus", "spi4", 6);
   DECL_CONSTANT_STR("BUS_PINS_spi4", "PE13,PE14,PE12");
   #ifdef SPI5
    DECL_ENUMERATION("spi_bus", "spi5", 7);
    DECL_CONSTANT_STR("BUS_PINS_spi5", "PF8,PF9,PF7");
    DECL_ENUMERATION("spi_bus", "spi5a", 8);
    DECL_CONSTANT_STR("BUS_PINS_spi5a", "PH7,PF11,PH6");
    #ifdef SPI6
      DECL_ENUMERATION("spi_bus", "spi6", 9);
      DECL_CONSTANT_STR("BUS_PINS_spi6", "PG12,PG14,PG13");
    #endif
   #endif
  #elif defined(GPIOI)
   DECL_ENUMERATION("spi_bus", "spi2b", 6);
   DECL_CONSTANT_STR("BUS_PINS_spi2b", "PI2,PI3,PI1");
  #endif
 #endif
#endif

#define SPI_FUNCTION GPIO_FUNCTION(CONFIG_MACH_STM32F0 ? 0 : 5)
//not complete for stm32h7
static const struct spi_info spi_bus[] = {
    { SPI2, GPIO('B', 14), GPIO('B', 15), GPIO('B', 13), SPI_FUNCTION },
    { SPI1, GPIO('A', 6), GPIO('A', 7), GPIO('A', 5), SPI_FUNCTION },
    { SPI1, GPIO('B', 4), GPIO('B', 5), GPIO('B', 3), SPI_FUNCTION },
    { SPI2, GPIO('C', 2), GPIO('C', 3), GPIO('B', 10), SPI_FUNCTION },
#ifdef SPI3
    { SPI3, GPIO('B', 4), GPIO('B', 5), GPIO('B', 3), GPIO_FUNCTION(6) },
 #if CONFIG_MACH_STM32F4 || CONFIG_MACH_STM32H7
    { SPI3, GPIO('C', 11), GPIO('C', 12), GPIO('C', 10), GPIO_FUNCTION(6) },
  #ifdef SPI4
    { SPI4, GPIO('E', 13), GPIO('E', 14), GPIO('E', 12), SPI_FUNCTION },
   #ifdef SPI5
    { SPI5, GPIO('F', 8), GPIO('F', 9), GPIO('F', 7), SPI_FUNCTION },
    { SPI5, GPIO('H', 7), GPIO('F', 11), GPIO('H', 6), SPI_FUNCTION },
    #ifdef SPI6
    { SPI6, GPIO('G', 12), GPIO('G', 14), GPIO('G', 13), SPI_FUNCTION },
    #endif
   #endif
  #elif defined(GPIOI)  // Also for STM32H7
    { SPI2, GPIO('I', 2), GPIO('I', 3), GPIO('I', 1), SPI_FUNCTION },
  #endif
 #endif
#endif
};

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus >= ARRAY_SIZE(spi_bus))
        shutdown("Invalid spi bus");

    // Enable SPI
    SPI_TypeDef *spi = spi_bus[bus].spi;
    if (!is_enabled_pclock((uint32_t)spi)) {
        enable_pclock((uint32_t)spi);
        gpio_peripheral(spi_bus[bus].miso_pin, spi_bus[bus].function, 1);
        gpio_peripheral(spi_bus[bus].mosi_pin, spi_bus[bus].function, 0);
        gpio_peripheral(spi_bus[bus].sck_pin, spi_bus[bus].function, 0);

        // Configure CR2 on stm32f0
#if CONFIG_MACH_STM32F0
        spi->CR2 = SPI_CR2_FRXTH | (7 << SPI_CR2_DS_Pos);
#endif
    }

    // Calculate CR1 register
    uint32_t pclk = get_pclock_frequency((uint32_t)spi);
    uint32_t div = 0;
    while ((pclk >> (div + 1)) > rate && div < 7)
        div++;

#if CONFIG_MACH_STM32H7
    uint32_t cr1 = (SPI_CR1_SPE | SPI_CR1_SSI);
    spi->CFG1 |= (div << SPI_CFG1_MBR_Pos);
    spi->CFG2 |= ((mode << SPI_CFG2_CPHA_Pos) | SPI_CFG2_MASTER | SPI_CFG2_SSM);
#else
    uint32_t cr1 = ((mode << SPI_CR1_CPHA_Pos) | (div << SPI_CR1_BR_Pos)
                    | SPI_CR1_SPE | SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI);
#endif
    return (struct spi_config){ .spi = spi, .spi_cr1 = cr1 };
}

void
spi_prepare(struct spi_config config)
{
    SPI_TypeDef *spi = config.spi;
    spi->CR1 = config.spi_cr1;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    SPI_TypeDef *spi = config.spi;
    while (len--) {
#if CONFIG_MACH_STM32H7
        writeb((void *)&spi->TXDR, *data);
        while (!(spi->SR & SPI_SR_RXWNE))
            ;
        uint8_t rdata = readb((void *)&spi->RXDR);
#else
        writeb((void *)&spi->DR, *data);
        while (!(spi->SR & SPI_SR_RXNE))
            ;
        uint8_t rdata = readb((void *)&spi->DR);
#endif
        if (receive_data)
            *data = rdata;
        data++;
    }
}
