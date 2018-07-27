/**
 * \file
 *
 * \brief Peripheral I/O description for SAM4LC8A
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _SAM4LC8A_PIO_
#define _SAM4LC8A_PIO_

#define PIN_PA00                           0  /**< \brief Pin Number for PA00 */
#define GPIO_PA00                  (1u <<  0) /**< \brief GPIO Mask  for PA00 */
#define PIN_PA01                           1  /**< \brief Pin Number for PA01 */
#define GPIO_PA01                  (1u <<  1) /**< \brief GPIO Mask  for PA01 */
#define PIN_PA02                           2  /**< \brief Pin Number for PA02 */
#define GPIO_PA02                  (1u <<  2) /**< \brief GPIO Mask  for PA02 */
#define PIN_PA03                           3  /**< \brief Pin Number for PA03 */
#define GPIO_PA03                  (1u <<  3) /**< \brief GPIO Mask  for PA03 */
#define PIN_PA04                           4  /**< \brief Pin Number for PA04 */
#define GPIO_PA04                  (1u <<  4) /**< \brief GPIO Mask  for PA04 */
#define PIN_PA05                           5  /**< \brief Pin Number for PA05 */
#define GPIO_PA05                  (1u <<  5) /**< \brief GPIO Mask  for PA05 */
#define PIN_PA06                           6  /**< \brief Pin Number for PA06 */
#define GPIO_PA06                  (1u <<  6) /**< \brief GPIO Mask  for PA06 */
#define PIN_PA07                           7  /**< \brief Pin Number for PA07 */
#define GPIO_PA07                  (1u <<  7) /**< \brief GPIO Mask  for PA07 */
#define PIN_PA08                           8  /**< \brief Pin Number for PA08 */
#define GPIO_PA08                  (1u <<  8) /**< \brief GPIO Mask  for PA08 */
#define PIN_PA09                           9  /**< \brief Pin Number for PA09 */
#define GPIO_PA09                  (1u <<  9) /**< \brief GPIO Mask  for PA09 */
#define PIN_PA10                          10  /**< \brief Pin Number for PA10 */
#define GPIO_PA10                  (1u << 10) /**< \brief GPIO Mask  for PA10 */
#define PIN_PA11                          11  /**< \brief Pin Number for PA11 */
#define GPIO_PA11                  (1u << 11) /**< \brief GPIO Mask  for PA11 */
#define PIN_PA12                          12  /**< \brief Pin Number for PA12 */
#define GPIO_PA12                  (1u << 12) /**< \brief GPIO Mask  for PA12 */
#define PIN_PA13                          13  /**< \brief Pin Number for PA13 */
#define GPIO_PA13                  (1u << 13) /**< \brief GPIO Mask  for PA13 */
#define PIN_PA14                          14  /**< \brief Pin Number for PA14 */
#define GPIO_PA14                  (1u << 14) /**< \brief GPIO Mask  for PA14 */
#define PIN_PA15                          15  /**< \brief Pin Number for PA15 */
#define GPIO_PA15                  (1u << 15) /**< \brief GPIO Mask  for PA15 */
#define PIN_PA16                          16  /**< \brief Pin Number for PA16 */
#define GPIO_PA16                  (1u << 16) /**< \brief GPIO Mask  for PA16 */
#define PIN_PA17                          17  /**< \brief Pin Number for PA17 */
#define GPIO_PA17                  (1u << 17) /**< \brief GPIO Mask  for PA17 */
#define PIN_PA18                          18  /**< \brief Pin Number for PA18 */
#define GPIO_PA18                  (1u << 18) /**< \brief GPIO Mask  for PA18 */
#define PIN_PA19                          19  /**< \brief Pin Number for PA19 */
#define GPIO_PA19                  (1u << 19) /**< \brief GPIO Mask  for PA19 */
#define PIN_PA20                          20  /**< \brief Pin Number for PA20 */
#define GPIO_PA20                  (1u << 20) /**< \brief GPIO Mask  for PA20 */
#define PIN_PA21                          21  /**< \brief Pin Number for PA21 */
#define GPIO_PA21                  (1u << 21) /**< \brief GPIO Mask  for PA21 */
#define PIN_PA22                          22  /**< \brief Pin Number for PA22 */
#define GPIO_PA22                  (1u << 22) /**< \brief GPIO Mask  for PA22 */
#define PIN_PA23                          23  /**< \brief Pin Number for PA23 */
#define GPIO_PA23                  (1u << 23) /**< \brief GPIO Mask  for PA23 */
#define PIN_PA24                          24  /**< \brief Pin Number for PA24 */
#define GPIO_PA24                  (1u << 24) /**< \brief GPIO Mask  for PA24 */
#define PIN_PA25                          25  /**< \brief Pin Number for PA25 */
#define GPIO_PA25                  (1u << 25) /**< \brief GPIO Mask  for PA25 */
#define PIN_PA26                          26  /**< \brief Pin Number for PA26 */
#define GPIO_PA26                  (1u << 26) /**< \brief GPIO Mask  for PA26 */
/* ========== GPIO definition for TWIMS0 peripheral ========== */
#define PIN_PA24B_TWIMS0_TWCK             24  /**< \brief TWIMS0 signal: TWCK on PA24 mux B */
#define MUX_PA24B_TWIMS0_TWCK              1
#define PINMUX_PA24B_TWIMS0_TWCK   ((PIN_PA24B_TWIMS0_TWCK << 16) | MUX_PA24B_TWIMS0_TWCK)
#define GPIO_PA24B_TWIMS0_TWCK     (1u << 24)
#define PIN_PA23B_TWIMS0_TWD              23  /**< \brief TWIMS0 signal: TWD on PA23 mux B */
#define MUX_PA23B_TWIMS0_TWD               1
#define PINMUX_PA23B_TWIMS0_TWD    ((PIN_PA23B_TWIMS0_TWD << 16) | MUX_PA23B_TWIMS0_TWD)
#define GPIO_PA23B_TWIMS0_TWD      (1u << 23)
/* ========== GPIO definition for TWIMS2 peripheral ========== */
#define PIN_PA22E_TWIMS2_TWCK             22  /**< \brief TWIMS2 signal: TWCK on PA22 mux E */
#define MUX_PA22E_TWIMS2_TWCK              4
#define PINMUX_PA22E_TWIMS2_TWCK   ((PIN_PA22E_TWIMS2_TWCK << 16) | MUX_PA22E_TWIMS2_TWCK)
#define GPIO_PA22E_TWIMS2_TWCK     (1u << 22)
#define PIN_PA21E_TWIMS2_TWD              21  /**< \brief TWIMS2 signal: TWD on PA21 mux E */
#define MUX_PA21E_TWIMS2_TWD               4
#define PINMUX_PA21E_TWIMS2_TWD    ((PIN_PA21E_TWIMS2_TWD << 16) | MUX_PA21E_TWIMS2_TWD)
#define GPIO_PA21E_TWIMS2_TWD      (1u << 21)
/* ========== GPIO definition for SPI peripheral ========== */
#define PIN_PA03B_SPI_MISO                 3  /**< \brief SPI signal: MISO on PA03 mux B */
#define MUX_PA03B_SPI_MISO                 1
#define PINMUX_PA03B_SPI_MISO      ((PIN_PA03B_SPI_MISO << 16) | MUX_PA03B_SPI_MISO)
#define GPIO_PA03B_SPI_MISO        (1u <<  3)
#define PIN_PA21A_SPI_MISO                21  /**< \brief SPI signal: MISO on PA21 mux A */
#define MUX_PA21A_SPI_MISO                 0
#define PINMUX_PA21A_SPI_MISO      ((PIN_PA21A_SPI_MISO << 16) | MUX_PA21A_SPI_MISO)
#define GPIO_PA21A_SPI_MISO        (1u << 21)
#define PIN_PA22A_SPI_MOSI                22  /**< \brief SPI signal: MOSI on PA22 mux A */
#define MUX_PA22A_SPI_MOSI                 0
#define PINMUX_PA22A_SPI_MOSI      ((PIN_PA22A_SPI_MOSI << 16) | MUX_PA22A_SPI_MOSI)
#define GPIO_PA22A_SPI_MOSI        (1u << 22)
#define PIN_PA02B_SPI_NPCS0                2  /**< \brief SPI signal: NPCS0 on PA02 mux B */
#define MUX_PA02B_SPI_NPCS0                1
#define PINMUX_PA02B_SPI_NPCS0     ((PIN_PA02B_SPI_NPCS0 << 16) | MUX_PA02B_SPI_NPCS0)
#define GPIO_PA02B_SPI_NPCS0       (1u <<  2)
#define PIN_PA24A_SPI_NPCS0               24  /**< \brief SPI signal: NPCS0 on PA24 mux A */
#define MUX_PA24A_SPI_NPCS0                0
#define PINMUX_PA24A_SPI_NPCS0     ((PIN_PA24A_SPI_NPCS0 << 16) | MUX_PA24A_SPI_NPCS0)
#define GPIO_PA24A_SPI_NPCS0       (1u << 24)
#define PIN_PA13C_SPI_NPCS1               13  /**< \brief SPI signal: NPCS1 on PA13 mux C */
#define MUX_PA13C_SPI_NPCS1                2
#define PINMUX_PA13C_SPI_NPCS1     ((PIN_PA13C_SPI_NPCS1 << 16) | MUX_PA13C_SPI_NPCS1)
#define GPIO_PA13C_SPI_NPCS1       (1u << 13)
#define PIN_PA14C_SPI_NPCS2               14  /**< \brief SPI signal: NPCS2 on PA14 mux C */
#define MUX_PA14C_SPI_NPCS2                2
#define PINMUX_PA14C_SPI_NPCS2     ((PIN_PA14C_SPI_NPCS2 << 16) | MUX_PA14C_SPI_NPCS2)
#define GPIO_PA14C_SPI_NPCS2       (1u << 14)
#define PIN_PA15C_SPI_NPCS3               15  /**< \brief SPI signal: NPCS3 on PA15 mux C */
#define MUX_PA15C_SPI_NPCS3                2
#define PINMUX_PA15C_SPI_NPCS3     ((PIN_PA15C_SPI_NPCS3 << 16) | MUX_PA15C_SPI_NPCS3)
#define GPIO_PA15C_SPI_NPCS3       (1u << 15)
#define PIN_PA23A_SPI_SCK                 23  /**< \brief SPI signal: SCK on PA23 mux A */
#define MUX_PA23A_SPI_SCK                  0
#define PINMUX_PA23A_SPI_SCK       ((PIN_PA23A_SPI_SCK << 16) | MUX_PA23A_SPI_SCK)
#define GPIO_PA23A_SPI_SCK         (1u << 23)
/* ========== GPIO definition for TC0 peripheral ========== */
#define PIN_PA08B_TC0_A0                   8  /**< \brief TC0 signal: A0 on PA08 mux B */
#define MUX_PA08B_TC0_A0                   1
#define PINMUX_PA08B_TC0_A0        ((PIN_PA08B_TC0_A0 << 16) | MUX_PA08B_TC0_A0)
#define GPIO_PA08B_TC0_A0          (1u <<  8)
#define PIN_PA10B_TC0_A1                  10  /**< \brief TC0 signal: A1 on PA10 mux B */
#define MUX_PA10B_TC0_A1                   1
#define PINMUX_PA10B_TC0_A1        ((PIN_PA10B_TC0_A1 << 16) | MUX_PA10B_TC0_A1)
#define GPIO_PA10B_TC0_A1          (1u << 10)
#define PIN_PA12B_TC0_A2                  12  /**< \brief TC0 signal: A2 on PA12 mux B */
#define MUX_PA12B_TC0_A2                   1
#define PINMUX_PA12B_TC0_A2        ((PIN_PA12B_TC0_A2 << 16) | MUX_PA12B_TC0_A2)
#define GPIO_PA12B_TC0_A2          (1u << 12)
#define PIN_PA09B_TC0_B0                   9  /**< \brief TC0 signal: B0 on PA09 mux B */
#define MUX_PA09B_TC0_B0                   1
#define PINMUX_PA09B_TC0_B0        ((PIN_PA09B_TC0_B0 << 16) | MUX_PA09B_TC0_B0)
#define GPIO_PA09B_TC0_B0          (1u <<  9)
#define PIN_PA11B_TC0_B1                  11  /**< \brief TC0 signal: B1 on PA11 mux B */
#define MUX_PA11B_TC0_B1                   1
#define PINMUX_PA11B_TC0_B1        ((PIN_PA11B_TC0_B1 << 16) | MUX_PA11B_TC0_B1)
#define GPIO_PA11B_TC0_B1          (1u << 11)
#define PIN_PA13B_TC0_B2                  13  /**< \brief TC0 signal: B2 on PA13 mux B */
#define MUX_PA13B_TC0_B2                   1
#define PINMUX_PA13B_TC0_B2        ((PIN_PA13B_TC0_B2 << 16) | MUX_PA13B_TC0_B2)
#define GPIO_PA13B_TC0_B2          (1u << 13)
#define PIN_PA14B_TC0_CLK0                14  /**< \brief TC0 signal: CLK0 on PA14 mux B */
#define MUX_PA14B_TC0_CLK0                 1
#define PINMUX_PA14B_TC0_CLK0      ((PIN_PA14B_TC0_CLK0 << 16) | MUX_PA14B_TC0_CLK0)
#define GPIO_PA14B_TC0_CLK0        (1u << 14)
#define PIN_PA15B_TC0_CLK1                15  /**< \brief TC0 signal: CLK1 on PA15 mux B */
#define MUX_PA15B_TC0_CLK1                 1
#define PINMUX_PA15B_TC0_CLK1      ((PIN_PA15B_TC0_CLK1 << 16) | MUX_PA15B_TC0_CLK1)
#define GPIO_PA15B_TC0_CLK1        (1u << 15)
#define PIN_PA16B_TC0_CLK2                16  /**< \brief TC0 signal: CLK2 on PA16 mux B */
#define MUX_PA16B_TC0_CLK2                 1
#define PINMUX_PA16B_TC0_CLK2      ((PIN_PA16B_TC0_CLK2 << 16) | MUX_PA16B_TC0_CLK2)
#define GPIO_PA16B_TC0_CLK2        (1u << 16)
/* ========== GPIO definition for USART0 peripheral ========== */
#define PIN_PA04B_USART0_CLK               4  /**< \brief USART0 signal: CLK on PA04 mux B */
#define MUX_PA04B_USART0_CLK               1
#define PINMUX_PA04B_USART0_CLK    ((PIN_PA04B_USART0_CLK << 16) | MUX_PA04B_USART0_CLK)
#define GPIO_PA04B_USART0_CLK      (1u <<  4)
#define PIN_PA10A_USART0_CLK              10  /**< \brief USART0 signal: CLK on PA10 mux A */
#define MUX_PA10A_USART0_CLK               0
#define PINMUX_PA10A_USART0_CLK    ((PIN_PA10A_USART0_CLK << 16) | MUX_PA10A_USART0_CLK)
#define GPIO_PA10A_USART0_CLK      (1u << 10)
#define PIN_PA09A_USART0_CTS               9  /**< \brief USART0 signal: CTS on PA09 mux A */
#define MUX_PA09A_USART0_CTS               0
#define PINMUX_PA09A_USART0_CTS    ((PIN_PA09A_USART0_CTS << 16) | MUX_PA09A_USART0_CTS)
#define GPIO_PA09A_USART0_CTS      (1u <<  9)
#define PIN_PA06B_USART0_RTS               6  /**< \brief USART0 signal: RTS on PA06 mux B */
#define MUX_PA06B_USART0_RTS               1
#define PINMUX_PA06B_USART0_RTS    ((PIN_PA06B_USART0_RTS << 16) | MUX_PA06B_USART0_RTS)
#define GPIO_PA06B_USART0_RTS      (1u <<  6)
#define PIN_PA08A_USART0_RTS               8  /**< \brief USART0 signal: RTS on PA08 mux A */
#define MUX_PA08A_USART0_RTS               0
#define PINMUX_PA08A_USART0_RTS    ((PIN_PA08A_USART0_RTS << 16) | MUX_PA08A_USART0_RTS)
#define GPIO_PA08A_USART0_RTS      (1u <<  8)
#define PIN_PA05B_USART0_RXD               5  /**< \brief USART0 signal: RXD on PA05 mux B */
#define MUX_PA05B_USART0_RXD               1
#define PINMUX_PA05B_USART0_RXD    ((PIN_PA05B_USART0_RXD << 16) | MUX_PA05B_USART0_RXD)
#define GPIO_PA05B_USART0_RXD      (1u <<  5)
#define PIN_PA11A_USART0_RXD              11  /**< \brief USART0 signal: RXD on PA11 mux A */
#define MUX_PA11A_USART0_RXD               0
#define PINMUX_PA11A_USART0_RXD    ((PIN_PA11A_USART0_RXD << 16) | MUX_PA11A_USART0_RXD)
#define GPIO_PA11A_USART0_RXD      (1u << 11)
#define PIN_PA07B_USART0_TXD               7  /**< \brief USART0 signal: TXD on PA07 mux B */
#define MUX_PA07B_USART0_TXD               1
#define PINMUX_PA07B_USART0_TXD    ((PIN_PA07B_USART0_TXD << 16) | MUX_PA07B_USART0_TXD)
#define GPIO_PA07B_USART0_TXD      (1u <<  7)
#define PIN_PA12A_USART0_TXD              12  /**< \brief USART0 signal: TXD on PA12 mux A */
#define MUX_PA12A_USART0_TXD               0
#define PINMUX_PA12A_USART0_TXD    ((PIN_PA12A_USART0_TXD << 16) | MUX_PA12A_USART0_TXD)
#define GPIO_PA12A_USART0_TXD      (1u << 12)
/* ========== GPIO definition for USART1 peripheral ========== */
#define PIN_PA14A_USART1_CLK              14  /**< \brief USART1 signal: CLK on PA14 mux A */
#define MUX_PA14A_USART1_CLK               0
#define PINMUX_PA14A_USART1_CLK    ((PIN_PA14A_USART1_CLK << 16) | MUX_PA14A_USART1_CLK)
#define GPIO_PA14A_USART1_CLK      (1u << 14)
#define PIN_PA21B_USART1_CTS              21  /**< \brief USART1 signal: CTS on PA21 mux B */
#define MUX_PA21B_USART1_CTS               1
#define PINMUX_PA21B_USART1_CTS    ((PIN_PA21B_USART1_CTS << 16) | MUX_PA21B_USART1_CTS)
#define GPIO_PA21B_USART1_CTS      (1u << 21)
#define PIN_PA13A_USART1_RTS              13  /**< \brief USART1 signal: RTS on PA13 mux A */
#define MUX_PA13A_USART1_RTS               0
#define PINMUX_PA13A_USART1_RTS    ((PIN_PA13A_USART1_RTS << 16) | MUX_PA13A_USART1_RTS)
#define GPIO_PA13A_USART1_RTS      (1u << 13)
#define PIN_PA15A_USART1_RXD              15  /**< \brief USART1 signal: RXD on PA15 mux A */
#define MUX_PA15A_USART1_RXD               0
#define PINMUX_PA15A_USART1_RXD    ((PIN_PA15A_USART1_RXD << 16) | MUX_PA15A_USART1_RXD)
#define GPIO_PA15A_USART1_RXD      (1u << 15)
#define PIN_PA16A_USART1_TXD              16  /**< \brief USART1 signal: TXD on PA16 mux A */
#define MUX_PA16A_USART1_TXD               0
#define PINMUX_PA16A_USART1_TXD    ((PIN_PA16A_USART1_TXD << 16) | MUX_PA16A_USART1_TXD)
#define GPIO_PA16A_USART1_TXD      (1u << 16)
/* ========== GPIO definition for USART2 peripheral ========== */
#define PIN_PA18A_USART2_CLK              18  /**< \brief USART2 signal: CLK on PA18 mux A */
#define MUX_PA18A_USART2_CLK               0
#define PINMUX_PA18A_USART2_CLK    ((PIN_PA18A_USART2_CLK << 16) | MUX_PA18A_USART2_CLK)
#define GPIO_PA18A_USART2_CLK      (1u << 18)
#define PIN_PA22B_USART2_CTS              22  /**< \brief USART2 signal: CTS on PA22 mux B */
#define MUX_PA22B_USART2_CTS               1
#define PINMUX_PA22B_USART2_CTS    ((PIN_PA22B_USART2_CTS << 16) | MUX_PA22B_USART2_CTS)
#define GPIO_PA22B_USART2_CTS      (1u << 22)
#define PIN_PA17A_USART2_RTS              17  /**< \brief USART2 signal: RTS on PA17 mux A */
#define MUX_PA17A_USART2_RTS               0
#define PINMUX_PA17A_USART2_RTS    ((PIN_PA17A_USART2_RTS << 16) | MUX_PA17A_USART2_RTS)
#define GPIO_PA17A_USART2_RTS      (1u << 17)
#define PIN_PA25B_USART2_RXD              25  /**< \brief USART2 signal: RXD on PA25 mux B */
#define MUX_PA25B_USART2_RXD               1
#define PINMUX_PA25B_USART2_RXD    ((PIN_PA25B_USART2_RXD << 16) | MUX_PA25B_USART2_RXD)
#define GPIO_PA25B_USART2_RXD      (1u << 25)
#define PIN_PA19A_USART2_RXD              19  /**< \brief USART2 signal: RXD on PA19 mux A */
#define MUX_PA19A_USART2_RXD               0
#define PINMUX_PA19A_USART2_RXD    ((PIN_PA19A_USART2_RXD << 16) | MUX_PA19A_USART2_RXD)
#define GPIO_PA19A_USART2_RXD      (1u << 19)
#define PIN_PA26B_USART2_TXD              26  /**< \brief USART2 signal: TXD on PA26 mux B */
#define MUX_PA26B_USART2_TXD               1
#define PINMUX_PA26B_USART2_TXD    ((PIN_PA26B_USART2_TXD << 16) | MUX_PA26B_USART2_TXD)
#define GPIO_PA26B_USART2_TXD      (1u << 26)
#define PIN_PA20A_USART2_TXD              20  /**< \brief USART2 signal: TXD on PA20 mux A */
#define MUX_PA20A_USART2_TXD               0
#define PINMUX_PA20A_USART2_TXD    ((PIN_PA20A_USART2_TXD << 16) | MUX_PA20A_USART2_TXD)
#define GPIO_PA20A_USART2_TXD      (1u << 20)
/* ========== GPIO definition for ADCIFE peripheral ========== */
#define PIN_PA04A_ADCIFE_AD0               4  /**< \brief ADCIFE signal: AD0 on PA04 mux A */
#define MUX_PA04A_ADCIFE_AD0               0
#define PINMUX_PA04A_ADCIFE_AD0    ((PIN_PA04A_ADCIFE_AD0 << 16) | MUX_PA04A_ADCIFE_AD0)
#define GPIO_PA04A_ADCIFE_AD0      (1u <<  4)
#define PIN_PA05A_ADCIFE_AD1               5  /**< \brief ADCIFE signal: AD1 on PA05 mux A */
#define MUX_PA05A_ADCIFE_AD1               0
#define PINMUX_PA05A_ADCIFE_AD1    ((PIN_PA05A_ADCIFE_AD1 << 16) | MUX_PA05A_ADCIFE_AD1)
#define GPIO_PA05A_ADCIFE_AD1      (1u <<  5)
#define PIN_PA07A_ADCIFE_AD2               7  /**< \brief ADCIFE signal: AD2 on PA07 mux A */
#define MUX_PA07A_ADCIFE_AD2               0
#define PINMUX_PA07A_ADCIFE_AD2    ((PIN_PA07A_ADCIFE_AD2 << 16) | MUX_PA07A_ADCIFE_AD2)
#define GPIO_PA07A_ADCIFE_AD2      (1u <<  7)
#define PIN_PA05E_ADCIFE_TRIGGER           5  /**< \brief ADCIFE signal: TRIGGER on PA05 mux E */
#define MUX_PA05E_ADCIFE_TRIGGER           4
#define PINMUX_PA05E_ADCIFE_TRIGGER  ((PIN_PA05E_ADCIFE_TRIGGER << 16) | MUX_PA05E_ADCIFE_TRIGGER)
#define GPIO_PA05E_ADCIFE_TRIGGER  (1u <<  5)
/* ========== GPIO definition for DACC peripheral ========== */
#define PIN_PA06A_DACC_VOUT                6  /**< \brief DACC signal: VOUT on PA06 mux A */
#define MUX_PA06A_DACC_VOUT                0
#define PINMUX_PA06A_DACC_VOUT     ((PIN_PA06A_DACC_VOUT << 16) | MUX_PA06A_DACC_VOUT)
#define GPIO_PA06A_DACC_VOUT       (1u <<  6)
/* ========== GPIO definition for ACIFC peripheral ========== */
#define PIN_PA06E_ACIFC_ACAN0              6  /**< \brief ACIFC signal: ACAN0 on PA06 mux E */
#define MUX_PA06E_ACIFC_ACAN0              4
#define PINMUX_PA06E_ACIFC_ACAN0   ((PIN_PA06E_ACIFC_ACAN0 << 16) | MUX_PA06E_ACIFC_ACAN0)
#define GPIO_PA06E_ACIFC_ACAN0     (1u <<  6)
#define PIN_PA07E_ACIFC_ACAP0              7  /**< \brief ACIFC signal: ACAP0 on PA07 mux E */
#define MUX_PA07E_ACIFC_ACAP0              4
#define PINMUX_PA07E_ACIFC_ACAP0   ((PIN_PA07E_ACIFC_ACAP0 << 16) | MUX_PA07E_ACIFC_ACAP0)
#define GPIO_PA07E_ACIFC_ACAP0     (1u <<  7)
/* ========== GPIO definition for GLOC peripheral ========== */
#define PIN_PA06D_GLOC_IN0                 6  /**< \brief GLOC signal: IN0 on PA06 mux D */
#define MUX_PA06D_GLOC_IN0                 3
#define PINMUX_PA06D_GLOC_IN0      ((PIN_PA06D_GLOC_IN0 << 16) | MUX_PA06D_GLOC_IN0)
#define GPIO_PA06D_GLOC_IN0        (1u <<  6)
#define PIN_PA20D_GLOC_IN0                20  /**< \brief GLOC signal: IN0 on PA20 mux D */
#define MUX_PA20D_GLOC_IN0                 3
#define PINMUX_PA20D_GLOC_IN0      ((PIN_PA20D_GLOC_IN0 << 16) | MUX_PA20D_GLOC_IN0)
#define GPIO_PA20D_GLOC_IN0        (1u << 20)
#define PIN_PA04D_GLOC_IN1                 4  /**< \brief GLOC signal: IN1 on PA04 mux D */
#define MUX_PA04D_GLOC_IN1                 3
#define PINMUX_PA04D_GLOC_IN1      ((PIN_PA04D_GLOC_IN1 << 16) | MUX_PA04D_GLOC_IN1)
#define GPIO_PA04D_GLOC_IN1        (1u <<  4)
#define PIN_PA21D_GLOC_IN1                21  /**< \brief GLOC signal: IN1 on PA21 mux D */
#define MUX_PA21D_GLOC_IN1                 3
#define PINMUX_PA21D_GLOC_IN1      ((PIN_PA21D_GLOC_IN1 << 16) | MUX_PA21D_GLOC_IN1)
#define GPIO_PA21D_GLOC_IN1        (1u << 21)
#define PIN_PA05D_GLOC_IN2                 5  /**< \brief GLOC signal: IN2 on PA05 mux D */
#define MUX_PA05D_GLOC_IN2                 3
#define PINMUX_PA05D_GLOC_IN2      ((PIN_PA05D_GLOC_IN2 << 16) | MUX_PA05D_GLOC_IN2)
#define GPIO_PA05D_GLOC_IN2        (1u <<  5)
#define PIN_PA22D_GLOC_IN2                22  /**< \brief GLOC signal: IN2 on PA22 mux D */
#define MUX_PA22D_GLOC_IN2                 3
#define PINMUX_PA22D_GLOC_IN2      ((PIN_PA22D_GLOC_IN2 << 16) | MUX_PA22D_GLOC_IN2)
#define GPIO_PA22D_GLOC_IN2        (1u << 22)
#define PIN_PA07D_GLOC_IN3                 7  /**< \brief GLOC signal: IN3 on PA07 mux D */
#define MUX_PA07D_GLOC_IN3                 3
#define PINMUX_PA07D_GLOC_IN3      ((PIN_PA07D_GLOC_IN3 << 16) | MUX_PA07D_GLOC_IN3)
#define GPIO_PA07D_GLOC_IN3        (1u <<  7)
#define PIN_PA23D_GLOC_IN3                23  /**< \brief GLOC signal: IN3 on PA23 mux D */
#define MUX_PA23D_GLOC_IN3                 3
#define PINMUX_PA23D_GLOC_IN3      ((PIN_PA23D_GLOC_IN3 << 16) | MUX_PA23D_GLOC_IN3)
#define GPIO_PA23D_GLOC_IN3        (1u << 23)
#define PIN_PA08D_GLOC_OUT0                8  /**< \brief GLOC signal: OUT0 on PA08 mux D */
#define MUX_PA08D_GLOC_OUT0                3
#define PINMUX_PA08D_GLOC_OUT0     ((PIN_PA08D_GLOC_OUT0 << 16) | MUX_PA08D_GLOC_OUT0)
#define GPIO_PA08D_GLOC_OUT0       (1u <<  8)
#define PIN_PA24D_GLOC_OUT0               24  /**< \brief GLOC signal: OUT0 on PA24 mux D */
#define MUX_PA24D_GLOC_OUT0                3
#define PINMUX_PA24D_GLOC_OUT0     ((PIN_PA24D_GLOC_OUT0 << 16) | MUX_PA24D_GLOC_OUT0)
#define GPIO_PA24D_GLOC_OUT0       (1u << 24)
/* ========== GPIO definition for ABDACB peripheral ========== */
#define PIN_PA17B_ABDACB_DAC0             17  /**< \brief ABDACB signal: DAC0 on PA17 mux B */
#define MUX_PA17B_ABDACB_DAC0              1
#define PINMUX_PA17B_ABDACB_DAC0   ((PIN_PA17B_ABDACB_DAC0 << 16) | MUX_PA17B_ABDACB_DAC0)
#define GPIO_PA17B_ABDACB_DAC0     (1u << 17)
#define PIN_PA19B_ABDACB_DAC1             19  /**< \brief ABDACB signal: DAC1 on PA19 mux B */
#define MUX_PA19B_ABDACB_DAC1              1
#define PINMUX_PA19B_ABDACB_DAC1   ((PIN_PA19B_ABDACB_DAC1 << 16) | MUX_PA19B_ABDACB_DAC1)
#define GPIO_PA19B_ABDACB_DAC1     (1u << 19)
#define PIN_PA18B_ABDACB_DACN0            18  /**< \brief ABDACB signal: DACN0 on PA18 mux B */
#define MUX_PA18B_ABDACB_DACN0             1
#define PINMUX_PA18B_ABDACB_DACN0  ((PIN_PA18B_ABDACB_DACN0 << 16) | MUX_PA18B_ABDACB_DACN0)
#define GPIO_PA18B_ABDACB_DACN0    (1u << 18)
#define PIN_PA20B_ABDACB_DACN1            20  /**< \brief ABDACB signal: DACN1 on PA20 mux B */
#define MUX_PA20B_ABDACB_DACN1             1
#define PINMUX_PA20B_ABDACB_DACN1  ((PIN_PA20B_ABDACB_DACN1 << 16) | MUX_PA20B_ABDACB_DACN1)
#define GPIO_PA20B_ABDACB_DACN1    (1u << 20)
/* ========== GPIO definition for PARC peripheral ========== */
#define PIN_PA17D_PARC_PCCK               17  /**< \brief PARC signal: PCCK on PA17 mux D */
#define MUX_PA17D_PARC_PCCK                3
#define PINMUX_PA17D_PARC_PCCK     ((PIN_PA17D_PARC_PCCK << 16) | MUX_PA17D_PARC_PCCK)
#define GPIO_PA17D_PARC_PCCK       (1u << 17)
#define PIN_PA09D_PARC_PCDATA0             9  /**< \brief PARC signal: PCDATA0 on PA09 mux D */
#define MUX_PA09D_PARC_PCDATA0             3
#define PINMUX_PA09D_PARC_PCDATA0  ((PIN_PA09D_PARC_PCDATA0 << 16) | MUX_PA09D_PARC_PCDATA0)
#define GPIO_PA09D_PARC_PCDATA0    (1u <<  9)
#define PIN_PA10D_PARC_PCDATA1            10  /**< \brief PARC signal: PCDATA1 on PA10 mux D */
#define MUX_PA10D_PARC_PCDATA1             3
#define PINMUX_PA10D_PARC_PCDATA1  ((PIN_PA10D_PARC_PCDATA1 << 16) | MUX_PA10D_PARC_PCDATA1)
#define GPIO_PA10D_PARC_PCDATA1    (1u << 10)
#define PIN_PA11D_PARC_PCDATA2            11  /**< \brief PARC signal: PCDATA2 on PA11 mux D */
#define MUX_PA11D_PARC_PCDATA2             3
#define PINMUX_PA11D_PARC_PCDATA2  ((PIN_PA11D_PARC_PCDATA2 << 16) | MUX_PA11D_PARC_PCDATA2)
#define GPIO_PA11D_PARC_PCDATA2    (1u << 11)
#define PIN_PA12D_PARC_PCDATA3            12  /**< \brief PARC signal: PCDATA3 on PA12 mux D */
#define MUX_PA12D_PARC_PCDATA3             3
#define PINMUX_PA12D_PARC_PCDATA3  ((PIN_PA12D_PARC_PCDATA3 << 16) | MUX_PA12D_PARC_PCDATA3)
#define GPIO_PA12D_PARC_PCDATA3    (1u << 12)
#define PIN_PA13D_PARC_PCDATA4            13  /**< \brief PARC signal: PCDATA4 on PA13 mux D */
#define MUX_PA13D_PARC_PCDATA4             3
#define PINMUX_PA13D_PARC_PCDATA4  ((PIN_PA13D_PARC_PCDATA4 << 16) | MUX_PA13D_PARC_PCDATA4)
#define GPIO_PA13D_PARC_PCDATA4    (1u << 13)
#define PIN_PA14D_PARC_PCDATA5            14  /**< \brief PARC signal: PCDATA5 on PA14 mux D */
#define MUX_PA14D_PARC_PCDATA5             3
#define PINMUX_PA14D_PARC_PCDATA5  ((PIN_PA14D_PARC_PCDATA5 << 16) | MUX_PA14D_PARC_PCDATA5)
#define GPIO_PA14D_PARC_PCDATA5    (1u << 14)
#define PIN_PA15D_PARC_PCDATA6            15  /**< \brief PARC signal: PCDATA6 on PA15 mux D */
#define MUX_PA15D_PARC_PCDATA6             3
#define PINMUX_PA15D_PARC_PCDATA6  ((PIN_PA15D_PARC_PCDATA6 << 16) | MUX_PA15D_PARC_PCDATA6)
#define GPIO_PA15D_PARC_PCDATA6    (1u << 15)
#define PIN_PA16D_PARC_PCDATA7            16  /**< \brief PARC signal: PCDATA7 on PA16 mux D */
#define MUX_PA16D_PARC_PCDATA7             3
#define PINMUX_PA16D_PARC_PCDATA7  ((PIN_PA16D_PARC_PCDATA7 << 16) | MUX_PA16D_PARC_PCDATA7)
#define GPIO_PA16D_PARC_PCDATA7    (1u << 16)
#define PIN_PA18D_PARC_PCEN1              18  /**< \brief PARC signal: PCEN1 on PA18 mux D */
#define MUX_PA18D_PARC_PCEN1               3
#define PINMUX_PA18D_PARC_PCEN1    ((PIN_PA18D_PARC_PCEN1 << 16) | MUX_PA18D_PARC_PCEN1)
#define GPIO_PA18D_PARC_PCEN1      (1u << 18)
#define PIN_PA19D_PARC_PCEN2              19  /**< \brief PARC signal: PCEN2 on PA19 mux D */
#define MUX_PA19D_PARC_PCEN2               3
#define PINMUX_PA19D_PARC_PCEN2    ((PIN_PA19D_PARC_PCEN2 << 16) | MUX_PA19D_PARC_PCEN2)
#define GPIO_PA19D_PARC_PCEN2      (1u << 19)
/* ========== GPIO definition for CATB peripheral ========== */
#define PIN_PA02G_CATB_DIS                 2  /**< \brief CATB signal: DIS on PA02 mux G */
#define MUX_PA02G_CATB_DIS                 6
#define PINMUX_PA02G_CATB_DIS      ((PIN_PA02G_CATB_DIS << 16) | MUX_PA02G_CATB_DIS)
#define GPIO_PA02G_CATB_DIS        (1u <<  2)
#define PIN_PA12G_CATB_DIS                12  /**< \brief CATB signal: DIS on PA12 mux G */
#define MUX_PA12G_CATB_DIS                 6
#define PINMUX_PA12G_CATB_DIS      ((PIN_PA12G_CATB_DIS << 16) | MUX_PA12G_CATB_DIS)
#define GPIO_PA12G_CATB_DIS        (1u << 12)
#define PIN_PA23G_CATB_DIS                23  /**< \brief CATB signal: DIS on PA23 mux G */
#define MUX_PA23G_CATB_DIS                 6
#define PINMUX_PA23G_CATB_DIS      ((PIN_PA23G_CATB_DIS << 16) | MUX_PA23G_CATB_DIS)
#define GPIO_PA23G_CATB_DIS        (1u << 23)
#define PIN_PA04G_CATB_SENSE0              4  /**< \brief CATB signal: SENSE0 on PA04 mux G */
#define MUX_PA04G_CATB_SENSE0              6
#define PINMUX_PA04G_CATB_SENSE0   ((PIN_PA04G_CATB_SENSE0 << 16) | MUX_PA04G_CATB_SENSE0)
#define GPIO_PA04G_CATB_SENSE0     (1u <<  4)
#define PIN_PA05G_CATB_SENSE1              5  /**< \brief CATB signal: SENSE1 on PA05 mux G */
#define MUX_PA05G_CATB_SENSE1              6
#define PINMUX_PA05G_CATB_SENSE1   ((PIN_PA05G_CATB_SENSE1 << 16) | MUX_PA05G_CATB_SENSE1)
#define GPIO_PA05G_CATB_SENSE1     (1u <<  5)
#define PIN_PA06G_CATB_SENSE2              6  /**< \brief CATB signal: SENSE2 on PA06 mux G */
#define MUX_PA06G_CATB_SENSE2              6
#define PINMUX_PA06G_CATB_SENSE2   ((PIN_PA06G_CATB_SENSE2 << 16) | MUX_PA06G_CATB_SENSE2)
#define GPIO_PA06G_CATB_SENSE2     (1u <<  6)
#define PIN_PA07G_CATB_SENSE3              7  /**< \brief CATB signal: SENSE3 on PA07 mux G */
#define MUX_PA07G_CATB_SENSE3              6
#define PINMUX_PA07G_CATB_SENSE3   ((PIN_PA07G_CATB_SENSE3 << 16) | MUX_PA07G_CATB_SENSE3)
#define GPIO_PA07G_CATB_SENSE3     (1u <<  7)
#define PIN_PA08G_CATB_SENSE4              8  /**< \brief CATB signal: SENSE4 on PA08 mux G */
#define MUX_PA08G_CATB_SENSE4              6
#define PINMUX_PA08G_CATB_SENSE4   ((PIN_PA08G_CATB_SENSE4 << 16) | MUX_PA08G_CATB_SENSE4)
#define GPIO_PA08G_CATB_SENSE4     (1u <<  8)
#define PIN_PA09G_CATB_SENSE5              9  /**< \brief CATB signal: SENSE5 on PA09 mux G */
#define MUX_PA09G_CATB_SENSE5              6
#define PINMUX_PA09G_CATB_SENSE5   ((PIN_PA09G_CATB_SENSE5 << 16) | MUX_PA09G_CATB_SENSE5)
#define GPIO_PA09G_CATB_SENSE5     (1u <<  9)
#define PIN_PA10G_CATB_SENSE6             10  /**< \brief CATB signal: SENSE6 on PA10 mux G */
#define MUX_PA10G_CATB_SENSE6              6
#define PINMUX_PA10G_CATB_SENSE6   ((PIN_PA10G_CATB_SENSE6 << 16) | MUX_PA10G_CATB_SENSE6)
#define GPIO_PA10G_CATB_SENSE6     (1u << 10)
#define PIN_PA11G_CATB_SENSE7             11  /**< \brief CATB signal: SENSE7 on PA11 mux G */
#define MUX_PA11G_CATB_SENSE7              6
#define PINMUX_PA11G_CATB_SENSE7   ((PIN_PA11G_CATB_SENSE7 << 16) | MUX_PA11G_CATB_SENSE7)
#define GPIO_PA11G_CATB_SENSE7     (1u << 11)
#define PIN_PA13G_CATB_SENSE8             13  /**< \brief CATB signal: SENSE8 on PA13 mux G */
#define MUX_PA13G_CATB_SENSE8              6
#define PINMUX_PA13G_CATB_SENSE8   ((PIN_PA13G_CATB_SENSE8 << 16) | MUX_PA13G_CATB_SENSE8)
#define GPIO_PA13G_CATB_SENSE8     (1u << 13)
#define PIN_PA14G_CATB_SENSE9             14  /**< \brief CATB signal: SENSE9 on PA14 mux G */
#define MUX_PA14G_CATB_SENSE9              6
#define PINMUX_PA14G_CATB_SENSE9   ((PIN_PA14G_CATB_SENSE9 << 16) | MUX_PA14G_CATB_SENSE9)
#define GPIO_PA14G_CATB_SENSE9     (1u << 14)
#define PIN_PA15G_CATB_SENSE10            15  /**< \brief CATB signal: SENSE10 on PA15 mux G */
#define MUX_PA15G_CATB_SENSE10             6
#define PINMUX_PA15G_CATB_SENSE10  ((PIN_PA15G_CATB_SENSE10 << 16) | MUX_PA15G_CATB_SENSE10)
#define GPIO_PA15G_CATB_SENSE10    (1u << 15)
#define PIN_PA16G_CATB_SENSE11            16  /**< \brief CATB signal: SENSE11 on PA16 mux G */
#define MUX_PA16G_CATB_SENSE11             6
#define PINMUX_PA16G_CATB_SENSE11  ((PIN_PA16G_CATB_SENSE11 << 16) | MUX_PA16G_CATB_SENSE11)
#define GPIO_PA16G_CATB_SENSE11    (1u << 16)
#define PIN_PA17G_CATB_SENSE12            17  /**< \brief CATB signal: SENSE12 on PA17 mux G */
#define MUX_PA17G_CATB_SENSE12             6
#define PINMUX_PA17G_CATB_SENSE12  ((PIN_PA17G_CATB_SENSE12 << 16) | MUX_PA17G_CATB_SENSE12)
#define GPIO_PA17G_CATB_SENSE12    (1u << 17)
#define PIN_PA18G_CATB_SENSE13            18  /**< \brief CATB signal: SENSE13 on PA18 mux G */
#define MUX_PA18G_CATB_SENSE13             6
#define PINMUX_PA18G_CATB_SENSE13  ((PIN_PA18G_CATB_SENSE13 << 16) | MUX_PA18G_CATB_SENSE13)
#define GPIO_PA18G_CATB_SENSE13    (1u << 18)
#define PIN_PA19G_CATB_SENSE14            19  /**< \brief CATB signal: SENSE14 on PA19 mux G */
#define MUX_PA19G_CATB_SENSE14             6
#define PINMUX_PA19G_CATB_SENSE14  ((PIN_PA19G_CATB_SENSE14 << 16) | MUX_PA19G_CATB_SENSE14)
#define GPIO_PA19G_CATB_SENSE14    (1u << 19)
#define PIN_PA20G_CATB_SENSE15            20  /**< \brief CATB signal: SENSE15 on PA20 mux G */
#define MUX_PA20G_CATB_SENSE15             6
#define PINMUX_PA20G_CATB_SENSE15  ((PIN_PA20G_CATB_SENSE15 << 16) | MUX_PA20G_CATB_SENSE15)
#define GPIO_PA20G_CATB_SENSE15    (1u << 20)
#define PIN_PA21G_CATB_SENSE16            21  /**< \brief CATB signal: SENSE16 on PA21 mux G */
#define MUX_PA21G_CATB_SENSE16             6
#define PINMUX_PA21G_CATB_SENSE16  ((PIN_PA21G_CATB_SENSE16 << 16) | MUX_PA21G_CATB_SENSE16)
#define GPIO_PA21G_CATB_SENSE16    (1u << 21)
#define PIN_PA22G_CATB_SENSE17            22  /**< \brief CATB signal: SENSE17 on PA22 mux G */
#define MUX_PA22G_CATB_SENSE17             6
#define PINMUX_PA22G_CATB_SENSE17  ((PIN_PA22G_CATB_SENSE17 << 16) | MUX_PA22G_CATB_SENSE17)
#define GPIO_PA22G_CATB_SENSE17    (1u << 22)
#define PIN_PA24G_CATB_SENSE18            24  /**< \brief CATB signal: SENSE18 on PA24 mux G */
#define MUX_PA24G_CATB_SENSE18             6
#define PINMUX_PA24G_CATB_SENSE18  ((PIN_PA24G_CATB_SENSE18 << 16) | MUX_PA24G_CATB_SENSE18)
#define GPIO_PA24G_CATB_SENSE18    (1u << 24)
#define PIN_PA25G_CATB_SENSE19            25  /**< \brief CATB signal: SENSE19 on PA25 mux G */
#define MUX_PA25G_CATB_SENSE19             6
#define PINMUX_PA25G_CATB_SENSE19  ((PIN_PA25G_CATB_SENSE19 << 16) | MUX_PA25G_CATB_SENSE19)
#define GPIO_PA25G_CATB_SENSE19    (1u << 25)
#define PIN_PA26G_CATB_SENSE20            26  /**< \brief CATB signal: SENSE20 on PA26 mux G */
#define MUX_PA26G_CATB_SENSE20             6
#define PINMUX_PA26G_CATB_SENSE20  ((PIN_PA26G_CATB_SENSE20 << 16) | MUX_PA26G_CATB_SENSE20)
#define GPIO_PA26G_CATB_SENSE20    (1u << 26)
/* ========== GPIO definition for LCDCA peripheral ========== */
#define PIN_PA12F_LCDCA_COM0              12  /**< \brief LCDCA signal: COM0 on PA12 mux F */
#define MUX_PA12F_LCDCA_COM0               5
#define PINMUX_PA12F_LCDCA_COM0    ((PIN_PA12F_LCDCA_COM0 << 16) | MUX_PA12F_LCDCA_COM0)
#define GPIO_PA12F_LCDCA_COM0      (1u << 12)
#define PIN_PA11F_LCDCA_COM1              11  /**< \brief LCDCA signal: COM1 on PA11 mux F */
#define MUX_PA11F_LCDCA_COM1               5
#define PINMUX_PA11F_LCDCA_COM1    ((PIN_PA11F_LCDCA_COM1 << 16) | MUX_PA11F_LCDCA_COM1)
#define GPIO_PA11F_LCDCA_COM1      (1u << 11)
#define PIN_PA10F_LCDCA_COM2              10  /**< \brief LCDCA signal: COM2 on PA10 mux F */
#define MUX_PA10F_LCDCA_COM2               5
#define PINMUX_PA10F_LCDCA_COM2    ((PIN_PA10F_LCDCA_COM2 << 16) | MUX_PA10F_LCDCA_COM2)
#define GPIO_PA10F_LCDCA_COM2      (1u << 10)
#define PIN_PA09F_LCDCA_COM3               9  /**< \brief LCDCA signal: COM3 on PA09 mux F */
#define MUX_PA09F_LCDCA_COM3               5
#define PINMUX_PA09F_LCDCA_COM3    ((PIN_PA09F_LCDCA_COM3 << 16) | MUX_PA09F_LCDCA_COM3)
#define GPIO_PA09F_LCDCA_COM3      (1u <<  9)
#define PIN_PA13F_LCDCA_SEG5              13  /**< \brief LCDCA signal: SEG5 on PA13 mux F */
#define MUX_PA13F_LCDCA_SEG5               5
#define PINMUX_PA13F_LCDCA_SEG5    ((PIN_PA13F_LCDCA_SEG5 << 16) | MUX_PA13F_LCDCA_SEG5)
#define GPIO_PA13F_LCDCA_SEG5      (1u << 13)
#define PIN_PA14F_LCDCA_SEG6              14  /**< \brief LCDCA signal: SEG6 on PA14 mux F */
#define MUX_PA14F_LCDCA_SEG6               5
#define PINMUX_PA14F_LCDCA_SEG6    ((PIN_PA14F_LCDCA_SEG6 << 16) | MUX_PA14F_LCDCA_SEG6)
#define GPIO_PA14F_LCDCA_SEG6      (1u << 14)
#define PIN_PA15F_LCDCA_SEG7              15  /**< \brief LCDCA signal: SEG7 on PA15 mux F */
#define MUX_PA15F_LCDCA_SEG7               5
#define PINMUX_PA15F_LCDCA_SEG7    ((PIN_PA15F_LCDCA_SEG7 << 16) | MUX_PA15F_LCDCA_SEG7)
#define GPIO_PA15F_LCDCA_SEG7      (1u << 15)
#define PIN_PA16F_LCDCA_SEG8              16  /**< \brief LCDCA signal: SEG8 on PA16 mux F */
#define MUX_PA16F_LCDCA_SEG8               5
#define PINMUX_PA16F_LCDCA_SEG8    ((PIN_PA16F_LCDCA_SEG8 << 16) | MUX_PA16F_LCDCA_SEG8)
#define GPIO_PA16F_LCDCA_SEG8      (1u << 16)
#define PIN_PA17F_LCDCA_SEG9              17  /**< \brief LCDCA signal: SEG9 on PA17 mux F */
#define MUX_PA17F_LCDCA_SEG9               5
#define PINMUX_PA17F_LCDCA_SEG9    ((PIN_PA17F_LCDCA_SEG9 << 16) | MUX_PA17F_LCDCA_SEG9)
#define GPIO_PA17F_LCDCA_SEG9      (1u << 17)
#define PIN_PA18F_LCDCA_SEG18             18  /**< \brief LCDCA signal: SEG18 on PA18 mux F */
#define MUX_PA18F_LCDCA_SEG18              5
#define PINMUX_PA18F_LCDCA_SEG18   ((PIN_PA18F_LCDCA_SEG18 << 16) | MUX_PA18F_LCDCA_SEG18)
#define GPIO_PA18F_LCDCA_SEG18     (1u << 18)
#define PIN_PA19F_LCDCA_SEG19             19  /**< \brief LCDCA signal: SEG19 on PA19 mux F */
#define MUX_PA19F_LCDCA_SEG19              5
#define PINMUX_PA19F_LCDCA_SEG19   ((PIN_PA19F_LCDCA_SEG19 << 16) | MUX_PA19F_LCDCA_SEG19)
#define GPIO_PA19F_LCDCA_SEG19     (1u << 19)
#define PIN_PA20F_LCDCA_SEG20             20  /**< \brief LCDCA signal: SEG20 on PA20 mux F */
#define MUX_PA20F_LCDCA_SEG20              5
#define PINMUX_PA20F_LCDCA_SEG20   ((PIN_PA20F_LCDCA_SEG20 << 16) | MUX_PA20F_LCDCA_SEG20)
#define GPIO_PA20F_LCDCA_SEG20     (1u << 20)
#define PIN_PA08F_LCDCA_SEG23              8  /**< \brief LCDCA signal: SEG23 on PA08 mux F */
#define MUX_PA08F_LCDCA_SEG23              5
#define PINMUX_PA08F_LCDCA_SEG23   ((PIN_PA08F_LCDCA_SEG23 << 16) | MUX_PA08F_LCDCA_SEG23)
#define GPIO_PA08F_LCDCA_SEG23     (1u <<  8)
#define PIN_PA21F_LCDCA_SEG34             21  /**< \brief LCDCA signal: SEG34 on PA21 mux F */
#define MUX_PA21F_LCDCA_SEG34              5
#define PINMUX_PA21F_LCDCA_SEG34   ((PIN_PA21F_LCDCA_SEG34 << 16) | MUX_PA21F_LCDCA_SEG34)
#define GPIO_PA21F_LCDCA_SEG34     (1u << 21)
#define PIN_PA22F_LCDCA_SEG35             22  /**< \brief LCDCA signal: SEG35 on PA22 mux F */
#define MUX_PA22F_LCDCA_SEG35              5
#define PINMUX_PA22F_LCDCA_SEG35   ((PIN_PA22F_LCDCA_SEG35 << 16) | MUX_PA22F_LCDCA_SEG35)
#define GPIO_PA22F_LCDCA_SEG35     (1u << 22)
#define PIN_PA23F_LCDCA_SEG38             23  /**< \brief LCDCA signal: SEG38 on PA23 mux F */
#define MUX_PA23F_LCDCA_SEG38              5
#define PINMUX_PA23F_LCDCA_SEG38   ((PIN_PA23F_LCDCA_SEG38 << 16) | MUX_PA23F_LCDCA_SEG38)
#define GPIO_PA23F_LCDCA_SEG38     (1u << 23)
#define PIN_PA24F_LCDCA_SEG39             24  /**< \brief LCDCA signal: SEG39 on PA24 mux F */
#define MUX_PA24F_LCDCA_SEG39              5
#define PINMUX_PA24F_LCDCA_SEG39   ((PIN_PA24F_LCDCA_SEG39 << 16) | MUX_PA24F_LCDCA_SEG39)
#define GPIO_PA24F_LCDCA_SEG39     (1u << 24)
/* ========== GPIO definition for USBC peripheral ========== */
#define PIN_PA25A_USBC_DM                 25  /**< \brief USBC signal: DM on PA25 mux A */
#define MUX_PA25A_USBC_DM                  0
#define PINMUX_PA25A_USBC_DM       ((PIN_PA25A_USBC_DM << 16) | MUX_PA25A_USBC_DM)
#define GPIO_PA25A_USBC_DM         (1u << 25)
#define PIN_PA26A_USBC_DP                 26  /**< \brief USBC signal: DP on PA26 mux A */
#define MUX_PA26A_USBC_DP                  0
#define PINMUX_PA26A_USBC_DP       ((PIN_PA26A_USBC_DP << 16) | MUX_PA26A_USBC_DP)
#define GPIO_PA26A_USBC_DP         (1u << 26)
/* ========== GPIO definition for PEVC peripheral ========== */
#define PIN_PA08C_PEVC_PAD_EVT0            8  /**< \brief PEVC signal: PAD_EVT0 on PA08 mux C */
#define MUX_PA08C_PEVC_PAD_EVT0            2
#define PINMUX_PA08C_PEVC_PAD_EVT0  ((PIN_PA08C_PEVC_PAD_EVT0 << 16) | MUX_PA08C_PEVC_PAD_EVT0)
#define GPIO_PA08C_PEVC_PAD_EVT0   (1u <<  8)
#define PIN_PA09C_PEVC_PAD_EVT1            9  /**< \brief PEVC signal: PAD_EVT1 on PA09 mux C */
#define MUX_PA09C_PEVC_PAD_EVT1            2
#define PINMUX_PA09C_PEVC_PAD_EVT1  ((PIN_PA09C_PEVC_PAD_EVT1 << 16) | MUX_PA09C_PEVC_PAD_EVT1)
#define GPIO_PA09C_PEVC_PAD_EVT1   (1u <<  9)
#define PIN_PA10C_PEVC_PAD_EVT2           10  /**< \brief PEVC signal: PAD_EVT2 on PA10 mux C */
#define MUX_PA10C_PEVC_PAD_EVT2            2
#define PINMUX_PA10C_PEVC_PAD_EVT2  ((PIN_PA10C_PEVC_PAD_EVT2 << 16) | MUX_PA10C_PEVC_PAD_EVT2)
#define GPIO_PA10C_PEVC_PAD_EVT2   (1u << 10)
#define PIN_PA11C_PEVC_PAD_EVT3           11  /**< \brief PEVC signal: PAD_EVT3 on PA11 mux C */
#define MUX_PA11C_PEVC_PAD_EVT3            2
#define PINMUX_PA11C_PEVC_PAD_EVT3  ((PIN_PA11C_PEVC_PAD_EVT3 << 16) | MUX_PA11C_PEVC_PAD_EVT3)
#define GPIO_PA11C_PEVC_PAD_EVT3   (1u << 11)
/* ========== GPIO definition for SCIF peripheral ========== */
#define PIN_PA19E_SCIF_GCLK0              19  /**< \brief SCIF signal: GCLK0 on PA19 mux E */
#define MUX_PA19E_SCIF_GCLK0               4
#define PINMUX_PA19E_SCIF_GCLK0    ((PIN_PA19E_SCIF_GCLK0 << 16) | MUX_PA19E_SCIF_GCLK0)
#define GPIO_PA19E_SCIF_GCLK0      (1u << 19)
#define PIN_PA02A_SCIF_GCLK0               2  /**< \brief SCIF signal: GCLK0 on PA02 mux A */
#define MUX_PA02A_SCIF_GCLK0               0
#define PINMUX_PA02A_SCIF_GCLK0    ((PIN_PA02A_SCIF_GCLK0 << 16) | MUX_PA02A_SCIF_GCLK0)
#define GPIO_PA02A_SCIF_GCLK0      (1u <<  2)
#define PIN_PA20E_SCIF_GCLK1              20  /**< \brief SCIF signal: GCLK1 on PA20 mux E */
#define MUX_PA20E_SCIF_GCLK1               4
#define PINMUX_PA20E_SCIF_GCLK1    ((PIN_PA20E_SCIF_GCLK1 << 16) | MUX_PA20E_SCIF_GCLK1)
#define GPIO_PA20E_SCIF_GCLK1      (1u << 20)
#define PIN_PA23E_SCIF_GCLK_IN0           23  /**< \brief SCIF signal: GCLK_IN0 on PA23 mux E */
#define MUX_PA23E_SCIF_GCLK_IN0            4
#define PINMUX_PA23E_SCIF_GCLK_IN0  ((PIN_PA23E_SCIF_GCLK_IN0 << 16) | MUX_PA23E_SCIF_GCLK_IN0)
#define GPIO_PA23E_SCIF_GCLK_IN0   (1u << 23)
#define PIN_PA24E_SCIF_GCLK_IN1           24  /**< \brief SCIF signal: GCLK_IN1 on PA24 mux E */
#define MUX_PA24E_SCIF_GCLK_IN1            4
#define PINMUX_PA24E_SCIF_GCLK_IN1  ((PIN_PA24E_SCIF_GCLK_IN1 << 16) | MUX_PA24E_SCIF_GCLK_IN1)
#define GPIO_PA24E_SCIF_GCLK_IN1   (1u << 24)
/* ========== GPIO definition for EIC peripheral ========== */
#define PIN_PA06C_EIC_EXTINT1              6  /**< \brief EIC signal: EXTINT1 on PA06 mux C */
#define MUX_PA06C_EIC_EXTINT1              2
#define PINMUX_PA06C_EIC_EXTINT1   ((PIN_PA06C_EIC_EXTINT1 << 16) | MUX_PA06C_EIC_EXTINT1)
#define GPIO_PA06C_EIC_EXTINT1     (1u <<  6)
#define PIN_PA16C_EIC_EXTINT1             16  /**< \brief EIC signal: EXTINT1 on PA16 mux C */
#define MUX_PA16C_EIC_EXTINT1              2
#define PINMUX_PA16C_EIC_EXTINT1   ((PIN_PA16C_EIC_EXTINT1 << 16) | MUX_PA16C_EIC_EXTINT1)
#define GPIO_PA16C_EIC_EXTINT1     (1u << 16)
#define PIN_PA04C_EIC_EXTINT2              4  /**< \brief EIC signal: EXTINT2 on PA04 mux C */
#define MUX_PA04C_EIC_EXTINT2              2
#define PINMUX_PA04C_EIC_EXTINT2   ((PIN_PA04C_EIC_EXTINT2 << 16) | MUX_PA04C_EIC_EXTINT2)
#define GPIO_PA04C_EIC_EXTINT2     (1u <<  4)
#define PIN_PA17C_EIC_EXTINT2             17  /**< \brief EIC signal: EXTINT2 on PA17 mux C */
#define MUX_PA17C_EIC_EXTINT2              2
#define PINMUX_PA17C_EIC_EXTINT2   ((PIN_PA17C_EIC_EXTINT2 << 16) | MUX_PA17C_EIC_EXTINT2)
#define GPIO_PA17C_EIC_EXTINT2     (1u << 17)
#define PIN_PA05C_EIC_EXTINT3              5  /**< \brief EIC signal: EXTINT3 on PA05 mux C */
#define MUX_PA05C_EIC_EXTINT3              2
#define PINMUX_PA05C_EIC_EXTINT3   ((PIN_PA05C_EIC_EXTINT3 << 16) | MUX_PA05C_EIC_EXTINT3)
#define GPIO_PA05C_EIC_EXTINT3     (1u <<  5)
#define PIN_PA18C_EIC_EXTINT3             18  /**< \brief EIC signal: EXTINT3 on PA18 mux C */
#define MUX_PA18C_EIC_EXTINT3              2
#define PINMUX_PA18C_EIC_EXTINT3   ((PIN_PA18C_EIC_EXTINT3 << 16) | MUX_PA18C_EIC_EXTINT3)
#define GPIO_PA18C_EIC_EXTINT3     (1u << 18)
#define PIN_PA07C_EIC_EXTINT4              7  /**< \brief EIC signal: EXTINT4 on PA07 mux C */
#define MUX_PA07C_EIC_EXTINT4              2
#define PINMUX_PA07C_EIC_EXTINT4   ((PIN_PA07C_EIC_EXTINT4 << 16) | MUX_PA07C_EIC_EXTINT4)
#define GPIO_PA07C_EIC_EXTINT4     (1u <<  7)
#define PIN_PA19C_EIC_EXTINT4             19  /**< \brief EIC signal: EXTINT4 on PA19 mux C */
#define MUX_PA19C_EIC_EXTINT4              2
#define PINMUX_PA19C_EIC_EXTINT4   ((PIN_PA19C_EIC_EXTINT4 << 16) | MUX_PA19C_EIC_EXTINT4)
#define GPIO_PA19C_EIC_EXTINT4     (1u << 19)
#define PIN_PA20C_EIC_EXTINT5             20  /**< \brief EIC signal: EXTINT5 on PA20 mux C */
#define MUX_PA20C_EIC_EXTINT5              2
#define PINMUX_PA20C_EIC_EXTINT5   ((PIN_PA20C_EIC_EXTINT5 << 16) | MUX_PA20C_EIC_EXTINT5)
#define GPIO_PA20C_EIC_EXTINT5     (1u << 20)
#define PIN_PA21C_EIC_EXTINT6             21  /**< \brief EIC signal: EXTINT6 on PA21 mux C */
#define MUX_PA21C_EIC_EXTINT6              2
#define PINMUX_PA21C_EIC_EXTINT6   ((PIN_PA21C_EIC_EXTINT6 << 16) | MUX_PA21C_EIC_EXTINT6)
#define GPIO_PA21C_EIC_EXTINT6     (1u << 21)
#define PIN_PA22C_EIC_EXTINT7             22  /**< \brief EIC signal: EXTINT7 on PA22 mux C */
#define MUX_PA22C_EIC_EXTINT7              2
#define PINMUX_PA22C_EIC_EXTINT7   ((PIN_PA22C_EIC_EXTINT7 << 16) | MUX_PA22C_EIC_EXTINT7)
#define GPIO_PA22C_EIC_EXTINT7     (1u << 22)
#define PIN_PA23C_EIC_EXTINT8             23  /**< \brief EIC signal: EXTINT8 on PA23 mux C */
#define MUX_PA23C_EIC_EXTINT8              2
#define PINMUX_PA23C_EIC_EXTINT8   ((PIN_PA23C_EIC_EXTINT8 << 16) | MUX_PA23C_EIC_EXTINT8)
#define GPIO_PA23C_EIC_EXTINT8     (1u << 23)

#endif /* _SAM4LC8A_PIO_ */
