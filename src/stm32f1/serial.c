// STM32F1 serial port
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/serial_irq.h" // serial_rx_byte
#include "sched.h" // DECL_INIT
#include "stm32f1xx.h" // UART
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"

void
serial_init(void)
{
    const uint32_t pclk = __LL_RCC_CALC_PCLK2_FREQ(SystemCoreClock
                                                   , LL_RCC_GetAPB2Prescaler());

    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_USART1);
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_USART1);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    LL_USART_SetBaudRate(USART1, pclk, CONFIG_SERIAL_BAUD);
    LL_USART_SetDataWidth(USART1, LL_USART_DATAWIDTH_8B);
    LL_USART_SetParity(USART1, LL_USART_PARITY_NONE);
    LL_USART_SetStopBitsLength(USART1, LL_USART_STOPBITS_1);
    LL_USART_SetHWFlowCtrl(USART1, LL_USART_HWCONTROL_NONE);
    LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
    LL_USART_EnableIT_RXNE(USART1);
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);
    LL_USART_Enable(USART1);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_GPIO_AF_DisableRemap_USART1();
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);
}
DECL_INIT(serial_init);

void __visible
USART1_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_RXNE(USART1) || LL_USART_IsActiveFlag_ORE(USART1))
        serial_rx_byte(LL_USART_ReceiveData8(USART1));
    if (LL_USART_IsActiveFlag_TXE(USART1)) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            LL_USART_DisableIT_TXE(USART1);
        else
            LL_USART_TransmitData8(USART1, data);
    }
}

void
serial_enable_tx_irq(void)
{
    LL_USART_EnableIT_TXE(USART1);
}
