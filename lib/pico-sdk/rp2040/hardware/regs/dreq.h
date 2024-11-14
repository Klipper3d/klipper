// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DREQ_H
#define _DREQ_H

/**
 * \file rp2040/dreq.h
 */

#ifdef __ASSEMBLER__
#define DREQ_PIO0_TX0 0
#define DREQ_PIO0_TX1 1
#define DREQ_PIO0_TX2 2
#define DREQ_PIO0_TX3 3
#define DREQ_PIO0_RX0 4
#define DREQ_PIO0_RX1 5
#define DREQ_PIO0_RX2 6
#define DREQ_PIO0_RX3 7
#define DREQ_PIO1_TX0 8
#define DREQ_PIO1_TX1 9
#define DREQ_PIO1_TX2 10
#define DREQ_PIO1_TX3 11
#define DREQ_PIO1_RX0 12
#define DREQ_PIO1_RX1 13
#define DREQ_PIO1_RX2 14
#define DREQ_PIO1_RX3 15
#define DREQ_SPI0_TX 16
#define DREQ_SPI0_RX 17
#define DREQ_SPI1_TX 18
#define DREQ_SPI1_RX 19
#define DREQ_UART0_TX 20
#define DREQ_UART0_RX 21
#define DREQ_UART1_TX 22
#define DREQ_UART1_RX 23
#define DREQ_PWM_WRAP0 24
#define DREQ_PWM_WRAP1 25
#define DREQ_PWM_WRAP2 26
#define DREQ_PWM_WRAP3 27
#define DREQ_PWM_WRAP4 28
#define DREQ_PWM_WRAP5 29
#define DREQ_PWM_WRAP6 30
#define DREQ_PWM_WRAP7 31
#define DREQ_I2C0_TX 32
#define DREQ_I2C0_RX 33
#define DREQ_I2C1_TX 34
#define DREQ_I2C1_RX 35
#define DREQ_ADC 36
#define DREQ_XIP_STREAM 37
#define DREQ_XIP_SSITX 38
#define DREQ_XIP_SSIRX 39
#define DREQ_DMA_TIMER0 59
#define DREQ_DMA_TIMER1 60
#define DREQ_DMA_TIMER2 61
#define DREQ_DMA_TIMER3 62
#define DREQ_FORCE 63
#else
/**
 * \brief DREQ numbers for DMA pacing on RP2040 (used as typedef \ref dreq_num_t)
 * \ingroup hardware_dma
 */
typedef enum dreq_num_rp2040 {
    DREQ_PIO0_TX0 = 0, ///< Select PIO0's TX FIFO 0 as DREQ
    DREQ_PIO0_TX1 = 1, ///< Select PIO0's TX FIFO 1 as DREQ
    DREQ_PIO0_TX2 = 2, ///< Select PIO0's TX FIFO 2 as DREQ
    DREQ_PIO0_TX3 = 3, ///< Select PIO0's TX FIFO 3 as DREQ
    DREQ_PIO0_RX0 = 4, ///< Select PIO0's RX FIFO 0 as DREQ
    DREQ_PIO0_RX1 = 5, ///< Select PIO0's RX FIFO 1 as DREQ
    DREQ_PIO0_RX2 = 6, ///< Select PIO0's RX FIFO 2 as DREQ
    DREQ_PIO0_RX3 = 7, ///< Select PIO0's RX FIFO 3 as DREQ
    DREQ_PIO1_TX0 = 8, ///< Select PIO1's TX FIFO 0 as DREQ
    DREQ_PIO1_TX1 = 9, ///< Select PIO1's TX FIFO 1 as DREQ
    DREQ_PIO1_TX2 = 10, ///< Select PIO1's TX FIFO 2 as DREQ
    DREQ_PIO1_TX3 = 11, ///< Select PIO1's TX FIFO 3 as DREQ
    DREQ_PIO1_RX0 = 12, ///< Select PIO1's RX FIFO 0 as DREQ
    DREQ_PIO1_RX1 = 13, ///< Select PIO1's RX FIFO 1 as DREQ
    DREQ_PIO1_RX2 = 14, ///< Select PIO1's RX FIFO 2 as DREQ
    DREQ_PIO1_RX3 = 15, ///< Select PIO1's RX FIFO 3 as DREQ
    DREQ_SPI0_TX = 16, ///< Select SPI0's TX FIFO as DREQ
    DREQ_SPI0_RX = 17, ///< Select SPI0's RX FIFO as DREQ
    DREQ_SPI1_TX = 18, ///< Select SPI1's TX FIFO as DREQ
    DREQ_SPI1_RX = 19, ///< Select SPI1's RX FIFO as DREQ
    DREQ_UART0_TX = 20, ///< Select UART0's TX FIFO as DREQ
    DREQ_UART0_RX = 21, ///< Select UART0's RX FIFO as DREQ
    DREQ_UART1_TX = 22, ///< Select UART1's TX FIFO as DREQ
    DREQ_UART1_RX = 23, ///< Select UART1's RX FIFO as DREQ
    DREQ_PWM_WRAP0 = 24, ///< Select PWM Counter 0's Wrap Value as DREQ
    DREQ_PWM_WRAP1 = 25, ///< Select PWM Counter 1's Wrap Value as DREQ
    DREQ_PWM_WRAP2 = 26, ///< Select PWM Counter 2's Wrap Value as DREQ
    DREQ_PWM_WRAP3 = 27, ///< Select PWM Counter 3's Wrap Value as DREQ
    DREQ_PWM_WRAP4 = 28, ///< Select PWM Counter 4's Wrap Value as DREQ
    DREQ_PWM_WRAP5 = 29, ///< Select PWM Counter 5's Wrap Value as DREQ
    DREQ_PWM_WRAP6 = 30, ///< Select PWM Counter 6's Wrap Value as DREQ
    DREQ_PWM_WRAP7 = 31, ///< Select PWM Counter 7's Wrap Value as DREQ
    DREQ_I2C0_TX = 32, ///< Select I2C0's TX FIFO as DREQ
    DREQ_I2C0_RX = 33, ///< Select I2C0's RX FIFO as DREQ
    DREQ_I2C1_TX = 34, ///< Select I2C1's TX FIFO as DREQ
    DREQ_I2C1_RX = 35, ///< Select I2C1's RX FIFO as DREQ
    DREQ_ADC = 36, ///< Select the ADC as DREQ
    DREQ_XIP_STREAM = 37, ///< Select the XIP Streaming FIFO as DREQ
    DREQ_XIP_SSITX = 38, ///< Select the XIP SSI TX FIFO as DREQ
    DREQ_XIP_SSIRX = 39, ///< Select the XIP SSI RX FIFO as DREQ
    DREQ_DMA_TIMER0 = 59, ///< Select DMA_TIMER0 as DREQ
    DREQ_DMA_TIMER1 = 60, ///< Select DMA_TIMER0 as DREQ
    DREQ_DMA_TIMER2 = 61, ///< Select DMA_TIMER1 as DREQ
    DREQ_DMA_TIMER3 = 62, ///< Select DMA_TIMER3 as DREQ
    DREQ_FORCE = 63, ///< Select FORCE as DREQ
    DREQ_COUNT
} dreq_num_t;
#endif

#endif // _DREQ_H

