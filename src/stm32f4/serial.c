// STM32F4 serial
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/serial_irq.h" // serial_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA3,PA2");

#define CR1_FLAGS (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE   \
                   | USART_CR1_RXNEIE)

void
serial_init(void)
{
    enable_pclock(USART2_BASE);

    uint32_t pclk = get_pclock_frequency(USART2_BASE);
    uint32_t div = pclk / (CONFIG_SERIAL_BAUD * 16);
    USART2->BRR = div << USART_BRR_DIV_Mantissa_Pos;
    USART2->CR1 = CR1_FLAGS;
    NVIC_SetPriority(USART2_IRQn, 0);
    NVIC_EnableIRQ(USART2_IRQn);

    gpio_peripheral(GPIO('A', 2), GPIO_FUNCTION, 7, 0);
    gpio_peripheral(GPIO('A', 3), GPIO_FUNCTION, 7, 1);
}
DECL_INIT(serial_init);

void __visible
USART2_IRQHandler(void)
{
    uint32_t sr = USART2->SR;
    if (sr & (USART_SR_RXNE | USART_SR_ORE))
        serial_rx_byte(USART2->DR);
    if (sr & USART_SR_TXE && USART2->CR1 & USART_CR1_TXEIE) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            USART2->CR1 = CR1_FLAGS;
        else
            USART2->DR = data;
    }
}

void
serial_enable_tx_irq(void)
{
    USART2->CR1 = CR1_FLAGS | USART_CR1_TXEIE;
}
