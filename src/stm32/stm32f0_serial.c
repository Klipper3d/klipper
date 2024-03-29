// STM32F0 serial
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/serial_irq.h" // serial_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

// Select the configured serial port
#if CONFIG_STM32_SERIAL_USART1
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA10,PA9");
  #define GPIO_Rx GPIO('A', 10)
  #define GPIO_Tx GPIO('A', 9)
  #define USARTx_FUNCTION GPIO_FUNCTION( \
            (CONFIG_MACH_STM32H7 | CONFIG_MACH_STM32G4) ? 7 : 1)
  #define USARTx USART1
  #define USARTx_IRQn USART1_IRQn
#elif CONFIG_STM32_SERIAL_USART1_ALT_PB7_PB6
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PB7,PB6");
  #define GPIO_Rx GPIO('B', 7)
  #define GPIO_Tx GPIO('B', 6)
  #define USARTx_FUNCTION GPIO_FUNCTION( \
            (CONFIG_MACH_STM32H7 | CONFIG_MACH_STM32G4) ? 7 : 0)
  #define USARTx USART1
  #define USARTx_IRQn USART1_IRQn
#elif CONFIG_STM32_SERIAL_USART2
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA3,PA2");
  #define GPIO_Rx GPIO('A', 3)
  #define GPIO_Tx GPIO('A', 2)
  #define USARTx_FUNCTION GPIO_FUNCTION( \
            (CONFIG_MACH_STM32H7 | CONFIG_MACH_STM32G4) ? 7 : 1)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
#elif CONFIG_STM32_SERIAL_USART2_ALT_PA15_PA14
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA15,PA14");
  #define GPIO_Rx GPIO('A', 15)
  #define GPIO_Tx GPIO('A', 14)
  #define USARTx_FUNCTION GPIO_FUNCTION(CONFIG_MACH_STM32G4 ? 7 : 1)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
#elif CONFIG_STM32_SERIAL_USART2_ALT_PB4_PB3
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PB4,PB3");
  #define GPIO_Rx GPIO('B', 4)
  #define GPIO_Tx GPIO('B', 3)
  #define USARTx_FUNCTION GPIO_FUNCTION(7)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
#elif CONFIG_STM32_SERIAL_USART2_ALT_PD6_PD5
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD6,PD5");
  #define GPIO_Rx GPIO('D', 6)
  #define GPIO_Tx GPIO('D', 5)
  #define USARTx_FUNCTION GPIO_FUNCTION(7)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
#elif CONFIG_STM32_SERIAL_USART3
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PB11,PB10");
  #define GPIO_Rx GPIO('B', 11)
  #define GPIO_Tx GPIO('B', 10)
  #define USARTx_FUNCTION GPIO_FUNCTION(7)
  #define USARTx USART3
  #define USARTx_IRQn USART3_IRQn
#elif CONFIG_STM32_SERIAL_USART3_ALT_PD9_PD8
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD9,PD8");
  #define GPIO_Rx GPIO('D', 9)
  #define GPIO_Tx GPIO('D', 8)
  #define USARTx_FUNCTION GPIO_FUNCTION(CONFIG_MACH_STM32G0 ? 0 : 7)
  #define USARTx USART3
  #define USARTx_IRQn USART3_IRQn
#elif CONFIG_STM32_SERIAL_UART4
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA1,PA0");
  #define GPIO_Rx GPIO('A', 1)
  #define GPIO_Tx GPIO('A', 0)
  #define USARTx_FUNCTION GPIO_FUNCTION(8)
  #define USARTx UART4
  #define USARTx_IRQn UART4_IRQn
#elif CONFIG_STM32_SERIAL_USART5
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD2,PD3");
  #define GPIO_Rx GPIO('D', 2)
  #define GPIO_Tx GPIO('D', 3)
  #define USARTx_FUNCTION GPIO_FUNCTION(3)
  #define USARTx USART5
  #define USARTx_IRQn USART5_IRQn
#endif

#if CONFIG_MACH_STM32F031
  // The stm32f031 has same pins for USART2, but everything is routed to USART1
  #define USART2 USART1
  #define USART2_IRQn USART1_IRQn
#endif

#if CONFIG_MACH_STM32G0
  // Some of the stm32g0 MCUs have slightly different register names
  #if CONFIG_MACH_STM32G0B1
    #define USART2_IRQn USART2_LPUART2_IRQn
    #define USART3_IRQn USART3_4_5_6_LPUART1_IRQn
    #define USART4_IRQn USART3_4_5_6_LPUART1_IRQn
    #define USART5_IRQn USART3_4_5_6_LPUART1_IRQn
    #define USART6_IRQn USART3_4_5_6_LPUART1_IRQn
  #endif
  #if CONFIG_MACH_STM32G0B0
    #define USART2_IRQn USART2_IRQn
    #define USART3_IRQn USART3_4_5_6_IRQn
    #define USART4_IRQn USART3_4_5_6_IRQn
    #define USART5_IRQn USART3_4_5_6_IRQn
    #define USART6_IRQn USART3_4_5_6_IRQn
  #endif
  #define USART_CR1_RXNEIE USART_CR1_RXNEIE_RXFNEIE
  #define USART_CR1_TXEIE USART_CR1_TXEIE_TXFNFIE
  #define USART_ISR_RXNE USART_ISR_RXNE_RXFNE
  #define USART_ISR_TXE USART_ISR_TXE_TXFNF
  #define USART_BRR_DIV_MANTISSA_Pos 4
  #define USART_BRR_DIV_FRACTION_Pos 0
#elif CONFIG_MACH_STM32G4
  #define USART_BRR_DIV_MANTISSA_Pos 4
  #define USART_BRR_DIV_FRACTION_Pos 0
#elif CONFIG_MACH_STM32H7
  // The stm32h7 has slightly different register names
  #define USART_ISR_RXNE USART_ISR_RXNE_RXFNE
  #define USART_ISR_TXE USART_ISR_TXE_TXFNF
#endif

#define CR1_FLAGS (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE   \
                   | USART_CR1_RXNEIE)

void
USARTx_IRQHandler(void)
{
    uint32_t sr = USARTx->ISR;
    if (sr & USART_ISR_RXNE)
        serial_rx_byte(USARTx->RDR);
    if (sr & USART_ISR_TXE && USARTx->CR1 & USART_CR1_TXEIE) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            USARTx->CR1 = CR1_FLAGS;
        else
            USARTx->TDR = data;
    }
}

void
serial_enable_tx_irq(void)
{
    USARTx->CR1 = CR1_FLAGS | USART_CR1_TXEIE;
}

void
serial_init(void)
{
    enable_pclock((uint32_t)USARTx);

    uint32_t pclk = get_pclock_frequency((uint32_t)USARTx);
    uint32_t div = DIV_ROUND_CLOSEST(pclk, CONFIG_SERIAL_BAUD);
    USARTx->BRR = (((div / 16) << USART_BRR_DIV_MANTISSA_Pos)
                   | ((div % 16) << USART_BRR_DIV_FRACTION_Pos));
    USARTx->CR3 = USART_CR3_OVRDIS; // disable the ORE ISR
    USARTx->CR1 = CR1_FLAGS;
    armcm_enable_irq(USARTx_IRQHandler, USARTx_IRQn, 0);

    gpio_peripheral(GPIO_Rx, USARTx_FUNCTION, 1);
    gpio_peripheral(GPIO_Tx, USARTx_FUNCTION, 0);
}
DECL_INIT(serial_init);
