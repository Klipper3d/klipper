/*
 *  SPI support on STM32F0 - without HAL for space saving
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include <string.h> // memcpy
#include "stm32f0xx_hal.h"
#include "command.h" // shutdown
#include "board/gpio.h" // spi_setup
#include "sched.h" // sched_shutdown
#include "log.h"

void spi_hw_setup(uint32_t rate)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* SPI1 init - no HAL */
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    if (rate > 800000) {
        SPI1->CR1 = SPI_BAUDRATEPRESCALER_64;
    } else {
        SPI1->CR1 = SPI_BAUDRATEPRESCALER_256;
    }

    SPI1->CR1 |= SPI_MODE_MASTER | SPI_DIRECTION_2LINES | SPI_CR1_SSM |
            SPI_POLARITY_LOW | SPI_PHASE_1EDGE |
            SPI_FIRSTBIT_MSB | SPI_CRCCALCULATION_DISABLE;

    SPI1->CR2 =  SPI_NSS_PULSE_DISABLE | SPI_DATASIZE_8BIT |
            SPI_RXFIFO_THRESHOLD_QF;

    SPI1->CR1 |= SPI_CR1_SPE; // Enable
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus > 0 || !rate)
        shutdown("Invalid spi_setup parameters");

    spi_hw_setup(rate);
    return (struct spi_config){ .cfg = SPI1->CR1 };
}

void
spi_prepare(struct spi_config config)
{
    SPI1->CR1 = config.cfg;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    uint8_t rxptr = 0;
    while (rxptr<len) {
        while(!(SPI1->SR & SPI_SR_TXE));
        __DMB();
        *((uint8_t*)&(SPI1->DR)) = data[rxptr]; // Hack with pointers
        // to write/read only 8 bits from 16-bit DR (see errata)
        while(!(SPI1->SR & SPI_SR_RXNE));
        __DMB();
        if(receive_data) {
            data[rxptr] = *((uint8_t*)&(SPI1->DR));
        }
        rxptr ++;
    }
}
