#ifndef __STM32_T5UID1_STM32G0_SERIAL_H
#define __STM32_T5UID1_STM32G0_SERIAL_H

#include "../internal.h"

#define USART_ISR(usart) (usart)->ISR
#define USART_RDR(usart) (usart)->RDR
#define USART_TDR(usart) (usart)->TDR
#define USART_CR1(usart) (usart)->CR1
#define USART_BRR(usart) (usart)->BRR

#define ISR_RX   (USART_ISR_RXNE_RXFNE | USART_ISR_ORE)
#define ISR_TXEN USART_ISR_TXE_TXFNF

#define CR1_BASE (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE \
                  | USART_CR1_RXNEIE_RXFNEIE)
#define CR1_TXEN USART_CR1_TXEIE_TXFNFIE

#define BRR_VAL(div) ((((div) / 16) << 4) \
                      | ((div) % 16))

#define USART1_PA10_PA9_FUNCTION  GPIO_FUNCTION(1)
#define USART1_PB7_PB6_FUNCTION   GPIO_FUNCTION(0)
#define USART2_PA3_PA2_FUNCTION   GPIO_FUNCTION(1)
#define USART2_PA15_PA14_FUNCTION GPIO_FUNCTION(1)
#define USART2_PD6_PD5_FUNCTION   GPIO_FUNCTION(0)
#define USART3_PB11_PB10_FUNCTION GPIO_FUNCTION(4)
#define USART3_PD9_PD8_FUNCTION   GPIO_FUNCTION(0)

#define USART2_IRQn USART2_LPUART2_IRQn
#define USART3_IRQn USART3_4_5_6_LPUART1_IRQn

#endif // stm32g0_serial.h
