/**
 * \file
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

#ifndef _SAMG54G19_PIO_
#define _SAMG54G19_PIO_

#define PIO_PA0           (1u << 0)  /**< \brief Pin Controlled by PA0 */
#define PIO_PA1           (1u << 1)  /**< \brief Pin Controlled by PA1 */
#define PIO_PA2           (1u << 2)  /**< \brief Pin Controlled by PA2 */
#define PIO_PA3           (1u << 3)  /**< \brief Pin Controlled by PA3 */
#define PIO_PA4           (1u << 4)  /**< \brief Pin Controlled by PA4 */
#define PIO_PA5           (1u << 5)  /**< \brief Pin Controlled by PA5 */
#define PIO_PA6           (1u << 6)  /**< \brief Pin Controlled by PA6 */
#define PIO_PA7           (1u << 7)  /**< \brief Pin Controlled by PA7 */
#define PIO_PA8           (1u << 8)  /**< \brief Pin Controlled by PA8 */
#define PIO_PA9           (1u << 9)  /**< \brief Pin Controlled by PA9 */
#define PIO_PA10          (1u << 10) /**< \brief Pin Controlled by PA10 */
#define PIO_PA11          (1u << 11) /**< \brief Pin Controlled by PA11 */
#define PIO_PA12          (1u << 12) /**< \brief Pin Controlled by PA12 */
#define PIO_PA13          (1u << 13) /**< \brief Pin Controlled by PA13 */
#define PIO_PA14          (1u << 14) /**< \brief Pin Controlled by PA14 */
#define PIO_PA15          (1u << 15) /**< \brief Pin Controlled by PA15 */
#define PIO_PA16          (1u << 16) /**< \brief Pin Controlled by PA16 */
#define PIO_PA17          (1u << 17) /**< \brief Pin Controlled by PA17 */
#define PIO_PA18          (1u << 18) /**< \brief Pin Controlled by PA18 */
#define PIO_PA19          (1u << 19) /**< \brief Pin Controlled by PA19 */
#define PIO_PA20          (1u << 20) /**< \brief Pin Controlled by PA20 */
#define PIO_PA21          (1u << 21) /**< \brief Pin Controlled by PA21 */
#define PIO_PA22          (1u << 22) /**< \brief Pin Controlled by PA22 */
#define PIO_PA23          (1u << 23) /**< \brief Pin Controlled by PA23 */
#define PIO_PA24          (1u << 24) /**< \brief Pin Controlled by PA24 */
#define PIO_PB0           (1u << 0)  /**< \brief Pin Controlled by PB0 */
#define PIO_PB1           (1u << 1)  /**< \brief Pin Controlled by PB1 */
#define PIO_PB2           (1u << 2)  /**< \brief Pin Controlled by PB2 */
#define PIO_PB3           (1u << 3)  /**< \brief Pin Controlled by PB3 */
#define PIO_PB4           (1u << 4)  /**< \brief Pin Controlled by PB4 */
#define PIO_PB5           (1u << 5)  /**< \brief Pin Controlled by PB5 */
#define PIO_PB6           (1u << 6)  /**< \brief Pin Controlled by PB6 */
#define PIO_PB7           (1u << 7)  /**< \brief Pin Controlled by PB7 */
#define PIO_PB8           (1u << 8)  /**< \brief Pin Controlled by PB8 */
#define PIO_PB9           (1u << 9)  /**< \brief Pin Controlled by PB9 */
#define PIO_PB10          (1u << 10) /**< \brief Pin Controlled by PB10 */
#define PIO_PB11          (1u << 11) /**< \brief Pin Controlled by PB11 */
#define PIO_PB12          (1u << 12) /**< \brief Pin Controlled by PB12 */
/* ========== Pio definition for ADC peripheral ========== */
#define PIO_PA17X1_AD0    (1u << 17) /**< \brief Adc signal: AD0 */
#define PIO_PA18X1_AD1    (1u << 18) /**< \brief Adc signal: AD1 */
#define PIO_PA19X1_AD2    (1u << 19) /**< \brief Adc signal: AD2 */
#define PIO_PA20X1_AD3    (1u << 20) /**< \brief Adc signal: AD3 */
#define PIO_PB0X1_AD4     (1u << 0)  /**< \brief Adc signal: AD4 */
#define PIO_PB1X1_AD5     (1u << 1)  /**< \brief Adc signal: AD5 */
#define PIO_PB2X1_AD6     (1u << 2)  /**< \brief Adc signal: AD6/WKUP12 */
#define PIO_PB2X1_WKUP12  (1u << 2)  /**< \brief Adc signal: AD6/WKUP12 */
#define PIO_PB3X1_AD7     (1u << 3)  /**< \brief Adc signal: AD7/WKUP13 */
#define PIO_PB3X1_WKUP13  (1u << 3)  /**< \brief Adc signal: AD7/WKUP13 */
#define PIO_PA8B_ADTRG    (1u << 8)  /**< \brief Adc signal: ADTRG */
/* ========== Pio definition for I2SC0 peripheral ========== */
#define PIO_PA0A_I2SCK0   (1u << 0)  /**< \brief I2sc0 signal: I2SCK0 */
#define PIO_PA2B_I2SDI0   (1u << 2)  /**< \brief I2sc0 signal: I2SDI0 */
#define PIO_PA3B_I2SDO0   (1u << 3)  /**< \brief I2sc0 signal: I2SDO0 */
#define PIO_PA17A_I2SDO0  (1u << 17) /**< \brief I2sc0 signal: I2SDO0 */
#define PIO_PA4B_I2SMCK0  (1u << 4)  /**< \brief I2sc0 signal: I2SMCK0 */
#define PIO_PA18A_I2SMCK0 (1u << 18) /**< \brief I2sc0 signal: I2SMCK0 */
#define PIO_PA1A_I2SWS0   (1u << 1)  /**< \brief I2sc0 signal: I2SWS0 */
#define PIO_PA10B_PDMCLK0 (1u << 10) /**< \brief I2sc0 signal: PDMCLK0 */
#define PIO_PA9B_PDMDAT0  (1u << 9)  /**< \brief I2sc0 signal: PDMDAT0 */
/* ========== Pio definition for I2SC1 peripheral ========== */
#define PIO_PA19B_I2SCK1  (1u << 19) /**< \brief I2sc1 signal: I2SCK1 */
#define PIO_PA22B_I2SDI1  (1u << 22) /**< \brief I2sc1 signal: I2SDI1 */
#define PIO_PA23A_I2SDO1  (1u << 23) /**< \brief I2sc1 signal: I2SDO1 */
#define PIO_PA24A_I2SMCK1 (1u << 24) /**< \brief I2sc1 signal: I2SMCK1 */
#define PIO_PA20B_I2SWS1  (1u << 20) /**< \brief I2sc1 signal: I2SWS1 */
/* ========== Pio definition for PMC peripheral ========== */
#define PIO_PA6B_PCK0     (1u << 6)  /**< \brief Pmc signal: PCK0 */
#define PIO_PA17B_PCK1    (1u << 17) /**< \brief Pmc signal: PCK1 */
#define PIO_PA21B_PCK1    (1u << 21) /**< \brief Pmc signal: PCK1 */
#define PIO_PA18B_PCK2    (1u << 18) /**< \brief Pmc signal: PCK2 */
#define PIO_PB3B_PCK2     (1u << 3)  /**< \brief Pmc signal: PCK2 */
/* ========== Pio definition for SPI peripheral ========== */
#define PIO_PA12A_MISO    (1u << 12) /**< \brief Spi signal: MISO */
#define PIO_PA13A_MOSI    (1u << 13) /**< \brief Spi signal: MOSI */
#define PIO_PA11A_NPCS0   (1u << 11) /**< \brief Spi signal: NPCS0 */
#define PIO_PA5B_NPCS1    (1u << 5)  /**< \brief Spi signal: NPCS1 */
#define PIO_PB2B_NPCS1    (1u << 2)  /**< \brief Spi signal: NPCS1 */
#define PIO_PA14A_SPCK    (1u << 14) /**< \brief Spi signal: SPCK */
/* ========== Pio definition for TC0 peripheral ========== */
#define PIO_PA2A_TCLK0    (1u << 2)  /**< \brief Tc0 signal: TCLK0 */
#define PIO_PA19A_TCLK1   (1u << 19) /**< \brief Tc0 signal: TCLK1 */
#define PIO_PA20A_TCLK2   (1u << 20) /**< \brief Tc0 signal: TCLK2 */
#define PIO_PA0B_TIOA0    (1u << 0)  /**< \brief Tc0 signal: TIOA0 */
#define PIO_PA23B_TIOA1   (1u << 23) /**< \brief Tc0 signal: TIOA1 */
#define PIO_PA21A_TIOA2   (1u << 21) /**< \brief Tc0 signal: TIOA2 */
#define PIO_PA1B_TIOB0    (1u << 1)  /**< \brief Tc0 signal: TIOB0 */
#define PIO_PA16B_TIOB1   (1u << 16) /**< \brief Tc0 signal: TIOB1 */
#define PIO_PA22A_TIOB2   (1u << 22) /**< \brief Tc0 signal: TIOB2 */
/* ========== Pio definition for TWI0 peripheral ========== */
#define PIO_PA4A_TWCK0    (1u << 4)  /**< \brief Twi0 signal: TWCK0 */
#define PIO_PA3A_TWD0     (1u << 3)  /**< \brief Twi0 signal: TWD0 */
/* ========== Pio definition for TWI1 peripheral ========== */
#define PIO_PB9A_TWCK1    (1u << 9)  /**< \brief Twi1 signal: TWCK1 */
#define PIO_PB11A_TWCK1   (1u << 11) /**< \brief Twi1 signal: TWCK1 */
#define PIO_PB8A_TWD1     (1u << 8)  /**< \brief Twi1 signal: TWD1 */
#define PIO_PB10A_TWD1    (1u << 10) /**< \brief Twi1 signal: TWD1 */
/* ========== Pio definition for TWI2 peripheral ========== */
#define PIO_PB1B_TWCK2    (1u << 1)  /**< \brief Twi2 signal: TWCK2 */
#define PIO_PB11B_TWCK2   (1u << 11) /**< \brief Twi2 signal: TWCK2 */
#define PIO_PB0B_TWD2     (1u << 0)  /**< \brief Twi2 signal: TWD2 */
#define PIO_PB10B_TWD2    (1u << 10) /**< \brief Twi2 signal: TWD2 */
/* ========== Pio definition for UART0 peripheral ========== */
#define PIO_PA9A_URXD0    (1u << 9)  /**< \brief Uart0 signal: URXD0 */
#define PIO_PA10A_UTXD0   (1u << 10) /**< \brief Uart0 signal: UTXD0 */
/* ========== Pio definition for UART1 peripheral ========== */
#define PIO_PB2A_URXD1    (1u << 2)  /**< \brief Uart1 signal: URXD1 */
#define PIO_PB3A_UTXD1    (1u << 3)  /**< \brief Uart1 signal: UTXD1 */
/* ========== Pio definition for USART peripheral ========== */
#define PIO_PA16A_CTS     (1u << 16) /**< \brief Usart signal: CTS */
#define PIO_PA15A_RTS     (1u << 15) /**< \brief Usart signal: RTS */
#define PIO_PA5A_RXD      (1u << 5)  /**< \brief Usart signal: RXD */
#define PIO_PA15B_SCK     (1u << 15) /**< \brief Usart signal: SCK */
#define PIO_PA6A_TXD      (1u << 6)  /**< \brief Usart signal: TXD */
/* ========== Pio indexes ========== */
#define PIO_PA0_IDX       0
#define PIO_PA1_IDX       1
#define PIO_PA2_IDX       2
#define PIO_PA3_IDX       3
#define PIO_PA4_IDX       4
#define PIO_PA5_IDX       5
#define PIO_PA6_IDX       6
#define PIO_PA7_IDX       7
#define PIO_PA8_IDX       8
#define PIO_PA9_IDX       9
#define PIO_PA10_IDX      10
#define PIO_PA11_IDX      11
#define PIO_PA12_IDX      12
#define PIO_PA13_IDX      13
#define PIO_PA14_IDX      14
#define PIO_PA15_IDX      15
#define PIO_PA16_IDX      16
#define PIO_PA17_IDX      17
#define PIO_PA18_IDX      18
#define PIO_PA19_IDX      19
#define PIO_PA20_IDX      20
#define PIO_PA21_IDX      21
#define PIO_PA22_IDX      22
#define PIO_PA23_IDX      23
#define PIO_PA24_IDX      24
#define PIO_PB0_IDX       32
#define PIO_PB1_IDX       33
#define PIO_PB2_IDX       34
#define PIO_PB3_IDX       35
#define PIO_PB4_IDX       36
#define PIO_PB5_IDX       37
#define PIO_PB6_IDX       38
#define PIO_PB7_IDX       39
#define PIO_PB8_IDX       40
#define PIO_PB9_IDX       41
#define PIO_PB10_IDX      42
#define PIO_PB11_IDX      43
#define PIO_PB12_IDX      44

#endif /* _SAMG54G19_PIO_ */
