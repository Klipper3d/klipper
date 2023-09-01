#ifndef __STM32_T5UID1_STM32F0_SERIAL_H
#define __STM32_T5UID1_STM32F0_SERIAL_H

#include "../internal.h"

#define USART_ISR(usart) (usart)->ISR
#define USART_RDR(usart) (usart)->RDR
#define USART_TDR(usart) (usart)->TDR
#define USART_CR1(usart) (usart)->CR1
#define USART_BRR(usart) (usart)->BRR

#define ISR_RX   (USART_ISR_RXNE | USART_ISR_ORE)
#define ISR_TXEN USART_ISR_TXE

#define CR1_BASE (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE \
                  | USART_CR1_RXNEIE)
#define CR1_TXEN USART_CR1_TXEIE

#define BRR_VAL(div) ((((div) / 16) << USART_BRR_DIV_MANTISSA_Pos) \
                      | (((div) % 16) << USART_BRR_DIV_FRACTION_Pos))

#define USART1_PA10_PA9_FUNCTION  GPIO_FUNCTION(1)
#define USART1_PB7_PB6_FUNCTION   GPIO_FUNCTION(0)
#define USART2_PA3_PA2_FUNCTION   GPIO_FUNCTION(1)
#define USART2_PA15_PA14_FUNCTION GPIO_FUNCTION(1)

#endif // stm32f0_serial.h
