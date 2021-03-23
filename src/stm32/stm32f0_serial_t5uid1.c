// STM32F0 serial
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_T5UID1_SERIAL_PORT
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/t5uid1_irq.h" // t5uid1_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock

#if CONFIG_SERIAL && CONFIG_T5UID1_SERIAL_PORT == CONFIG_SERIAL_PORT
  #error "The serial port selected for the T5UID1 screen is already used"
#endif

// Select the configured serial port
#if CONFIG_T5UID1_SERIAL_PORT == 1
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PA10,PA9");
  #define GPIO_Rx GPIO('A', 10)
  #define GPIO_Tx GPIO('A', 9)
  #define USARTx USART1
  #define USARTx_IRQn USART1_IRQn
#elif CONFIG_T5UID1_SERIAL_PORT == 2
  #if CONFIG_T5UID1_STM32_SERIAL_USART2_ALT
    DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PA15,PA14");
    #define GPIO_Rx GPIO('A', 15)
    #define GPIO_Tx GPIO('A', 14)
  #else
    DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PA3,PA2");
    #define GPIO_Rx GPIO('A', 3)
    #define GPIO_Tx GPIO('A', 2)
  #endif
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
#endif

#define CR1_FLAGS (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE   \
                   | USART_CR1_RXNEIE)

void
t5uid1_USARTx_IRQHandler(void)
{
    uint32_t sr = USARTx->ISR;
    if (sr & (USART_ISR_RXNE | USART_ISR_ORE))
        t5uid1_rx_byte(USARTx->RDR);
    if (sr & USART_ISR_TXE && USARTx->CR1 & USART_CR1_TXEIE) {
        uint8_t data;
        int ret = t5uid1_get_tx_byte(&data);
        if (ret)
            USARTx->CR1 = CR1_FLAGS;
        else
            USARTx->TDR = data;
    }
}

void
t5uid1_enable_tx_irq(void)
{
    USARTx->CR1 = CR1_FLAGS | USART_CR1_TXEIE;
}

void
t5uid1_init(uint32_t baud)
{
    enable_pclock((uint32_t)USARTx);

    uint32_t pclk = get_pclock_frequency((uint32_t)USARTx);
    uint32_t div = DIV_ROUND_CLOSEST(pclk, baud);
    USARTx->BRR = (((div / 16) << USART_BRR_DIV_MANTISSA_Pos)
                   | ((div % 16) << USART_BRR_DIV_FRACTION_Pos));
    USARTx->CR1 = CR1_FLAGS;
    armcm_enable_irq(t5uid1_USARTx_IRQHandler, USARTx_IRQn, 0);

    gpio_peripheral(GPIO_Rx, GPIO_FUNCTION(1), 1);
    gpio_peripheral(GPIO_Tx, GPIO_FUNCTION(1), 0);
}
