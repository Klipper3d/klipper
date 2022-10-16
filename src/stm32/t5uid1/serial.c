// STM32 serial
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_T5UID1_SERIAL_PORT
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/t5uid1/serial_irq.h" // t5uid1_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "../internal.h" // enable_pclock

#if CONFIG_MACH_STM32F0
  #include "stm32f0_serial.h"
#elif CONFIG_MACH_STM32G0
  #include "stm32g0_serial.h"
#elif CONFIG_MACH_STM32H7
  #include "stm32h7_serial.h"
#else
  #include "stm32_serial.h"
#endif

#if CONFIG_STM32_T5UID1_SERIAL_USART1_PA10_PA9
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PA10,PA9");
  #define GPIO_Rx GPIO('A', 10)
  #define GPIO_Tx GPIO('A', 9)
  #define USARTx USART1
  #define USARTx_IRQn USART1_IRQn
  #define USARTx_FUNCTION USART1_PA10_PA9_FUNCTION
#elif CONFIG_STM32_T5UID1_SERIAL_USART1_PB7_PB6
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PB7,PB6");
  #define GPIO_Rx GPIO('B', 7)
  #define GPIO_Tx GPIO('B', 6)
  #define USARTx USART1
  #define USARTx_IRQn USART1_IRQn
  #define USARTx_FUNCTION USART1_PB7_PB6_FUNCTION
#elif CONFIG_STM32_T5UID1_SERIAL_USART2_PA3_PA2
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PA3,PA2");
  #define GPIO_Rx GPIO('A', 3)
  #define GPIO_Tx GPIO('A', 2)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
  #define USARTx_FUNCTION USART2_PA3_PA2_FUNCTION
#elif CONFIG_STM32_T5UID1_SERIAL_USART2_PA15_PA14
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PA15,PA14");
  #define GPIO_Rx GPIO('A', 15)
  #define GPIO_Tx GPIO('A', 14)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
  #define USARTx_FUNCTION USART2_PA15_PA14_FUNCTION
#elif CONFIG_STM32_T5UID1_SERIAL_USART2_PD6_PD5
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PD6,PD5");
  #define GPIO_Rx GPIO('D', 6)
  #define GPIO_Tx GPIO('D', 5)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
  #define USARTx_FUNCTION USART2_PD6_PD5_FUNCTION
#elif CONFIG_STM32_T5UID1_SERIAL_USART3_PB11_PB10
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PB11,PB10");
  #define GPIO_Rx GPIO('B', 11)
  #define GPIO_Tx GPIO('B', 10)
  #define USARTx USART3
  #define USARTx_IRQn USART3_IRQn
  #define USARTx_FUNCTION USART3_PB11_PB10_FUNCTION
#elif CONFIG_STM32_T5UID1_SERIAL_USART3_PD9_PD8
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PD9,PD8");
  #define GPIO_Rx GPIO('D', 9)
  #define GPIO_Tx GPIO('D', 8)
  #define USARTx USART3
  #define USARTx_IRQn USART3_IRQn
  #define USARTx_FUNCTION USART3_PD9_PD8_FUNCTION
#elif CONFIG_STM32_T5UID1_SERIAL_UART4_PA1_PA0
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "PA1,PA0");
  #define GPIO_Rx GPIO('A', 1)
  #define GPIO_Tx GPIO('A', 0)
  #define USARTx UART4
  #define USARTx_IRQn UART4_IRQn
  #define USARTx_FUNCTION UART4_PA1_PA0_FUNCTION
#endif

void
t5uid1_USARTx_IRQHandler(void)
{
    uint32_t isr = USART_ISR(USARTx);
    if (isr & ISR_RX)
        t5uid1_rx_byte(USART_RDR(USARTx));
    if (isr & ISR_TXEN && USART_CR1(USARTx) & CR1_TXEN) {
        uint8_t data;
        int ret = t5uid1_get_tx_byte(&data);
        if (ret)
            USART_CR1(USARTx) = CR1_BASE;
        else
            USART_TDR(USARTx) = data;
    }
}

void
t5uid1_enable_tx_irq(void)
{
    USART_CR1(USARTx) = CR1_BASE | CR1_TXEN;
}

void
t5uid1_init(uint32_t baud)
{
    enable_pclock((uint32_t)USARTx);

    uint32_t pclk = get_pclock_frequency((uint32_t)USARTx);
    uint32_t div = DIV_ROUND_CLOSEST(pclk, baud);
    USART_BRR(USARTx) = BRR_VAL(div);
    USART_CR1(USARTx) = CR1_BASE;
    armcm_enable_irq(t5uid1_USARTx_IRQHandler, USARTx_IRQn, 0);

    gpio_peripheral(GPIO_Rx, USARTx_FUNCTION, 1);
    gpio_peripheral(GPIO_Tx, USARTx_FUNCTION, 0);
}
