#ifndef __STM32_T5UID1_STM32_SERIAL_H
#define __STM32_T5UID1_STM32_SERIAL_H

#include "../internal.h"

#define USART_ISR(usart) (usart)->SR
#define USART_RDR(usart) (usart)->DR
#define USART_TDR(usart) (usart)->DR
#define USART_CR1(usart) (usart)->CR1
#define USART_BRR(usart) (usart)->BRR

#define ISR_RX   (USART_SR_RXNE | USART_SR_ORE)
#define ISR_TXEN USART_SR_TXE

#define CR1_BASE (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE \
                  | USART_CR1_RXNEIE)
#define CR1_TXEN USART_CR1_TXEIE

#define BRR_VAL(div) ((((div) / 16) << USART_BRR_DIV_Mantissa_Pos) \
                      | (((div) % 16) << USART_BRR_DIV_Fraction_Pos))

#define USART1_PA10_PA9_FUNCTION  GPIO_FUNCTION(7)
#define USART1_PB7_PB6_FUNCTION   GPIO_FUNCTION(7)
#define USART2_PA3_PA2_FUNCTION   GPIO_FUNCTION(7)
#define USART2_PD6_PD5_FUNCTION   GPIO_FUNCTION(7)
#define USART3_PB11_PB10_FUNCTION GPIO_FUNCTION(7)
#define USART3_PD9_PD8_FUNCTION   GPIO_FUNCTION(7)

#endif // stm32_serial.h
