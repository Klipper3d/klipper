// SPI functions on STM32F1
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "sched.h" // sched_shutdown
#include "stm32f1xx_ll_gpio.h" // LL_GPIO_SetPinMode
#include "stm32f1xx_ll_rcc.h" // __LL_RCC_CALC_PCLK1_FREQ
#include "stm32f1xx_ll_spi.h" // LL_SPI_Enable

static void spi_set_mode(SPI_TypeDef *spi, uint8_t mode)
{
    switch (mode) {
    case 0:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
        break;
    case 1:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_2EDGE);
        break;
    case 2:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_HIGH);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
        break;
    case 3:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_HIGH);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_2EDGE);
        break;
    }
}

static void spi_set_baudrate(SPI_TypeDef *spi, uint32_t rate)
{
    const uint32_t pclk = __LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock
                                                   , LL_RCC_GetAPB1Prescaler());
    const uint32_t prescaler = pclk / rate;

    uint32_t setting = LL_SPI_BAUDRATEPRESCALER_DIV256;
    if (prescaler <= 2)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV2;
    else if (prescaler <= 4)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV4;
    else if (prescaler <= 8)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV8;
    else if (prescaler <= 16)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV16;
    else if (prescaler <= 32)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV32;
    else if (prescaler <= 64)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV64;
    else if (prescaler <= 128)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV128;

    LL_SPI_SetBaudRatePrescaler(spi, setting);
}

static void spi_init_pins(void)
{
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_14, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_PUSHPULL);
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus > 0 || !rate)
        shutdown("Invalid spi_setup parameters");

    spi_init_pins();
    SPI_TypeDef spi_hw = { };
    LL_SPI_SetNSSMode(&spi_hw, LL_SPI_NSS_SOFT);
    LL_SPI_SetMode(&spi_hw, LL_SPI_MODE_MASTER);
    spi_set_mode(&spi_hw, mode);
    spi_set_baudrate(&spi_hw, rate);
    LL_SPI_Enable(&spi_hw);

    return (struct spi_config){ .spi_cr1 = spi_hw.CR1 };
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
        LL_SPI_TransmitData8(SPI2, *data);
        while (!LL_SPI_IsActiveFlag_TXE(SPI2));
        while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
        uint8_t rdata = LL_SPI_ReceiveData8(SPI2);
        if (receive_data) {
            *data = rdata;
        }
        data++;
    }

    while (LL_SPI_IsActiveFlag_BSY(SPI2));
    LL_SPI_Disable(SPI2);
}
