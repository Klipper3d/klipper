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

#ifndef _SAMG55G19_PIO_
#define _SAMG55G19_PIO_

#define PIO_PA0              (1u << 0)  /**< \brief Pin Controlled by PA0 */
#define PIO_PA1              (1u << 1)  /**< \brief Pin Controlled by PA1 */
#define PIO_PA2              (1u << 2)  /**< \brief Pin Controlled by PA2 */
#define PIO_PA3              (1u << 3)  /**< \brief Pin Controlled by PA3 */
#define PIO_PA4              (1u << 4)  /**< \brief Pin Controlled by PA4 */
#define PIO_PA5              (1u << 5)  /**< \brief Pin Controlled by PA5 */
#define PIO_PA6              (1u << 6)  /**< \brief Pin Controlled by PA6 */
#define PIO_PA7              (1u << 7)  /**< \brief Pin Controlled by PA7 */
#define PIO_PA8              (1u << 8)  /**< \brief Pin Controlled by PA8 */
#define PIO_PA9              (1u << 9)  /**< \brief Pin Controlled by PA9 */
#define PIO_PA10             (1u << 10) /**< \brief Pin Controlled by PA10 */
#define PIO_PA11             (1u << 11) /**< \brief Pin Controlled by PA11 */
#define PIO_PA12             (1u << 12) /**< \brief Pin Controlled by PA12 */
#define PIO_PA13             (1u << 13) /**< \brief Pin Controlled by PA13 */
#define PIO_PA14             (1u << 14) /**< \brief Pin Controlled by PA14 */
#define PIO_PA15             (1u << 15) /**< \brief Pin Controlled by PA15 */
#define PIO_PA16             (1u << 16) /**< \brief Pin Controlled by PA16 */
#define PIO_PA17             (1u << 17) /**< \brief Pin Controlled by PA17 */
#define PIO_PA18             (1u << 18) /**< \brief Pin Controlled by PA18 */
#define PIO_PA19             (1u << 19) /**< \brief Pin Controlled by PA19 */
#define PIO_PA20             (1u << 20) /**< \brief Pin Controlled by PA20 */
#define PIO_PA21             (1u << 21) /**< \brief Pin Controlled by PA21 */
#define PIO_PA22             (1u << 22) /**< \brief Pin Controlled by PA22 */
#define PIO_PA23             (1u << 23) /**< \brief Pin Controlled by PA23 */
#define PIO_PA24             (1u << 24) /**< \brief Pin Controlled by PA24 */
#define PIO_PB0              (1u << 0)  /**< \brief Pin Controlled by PB0 */
#define PIO_PB1              (1u << 1)  /**< \brief Pin Controlled by PB1 */
#define PIO_PB2              (1u << 2)  /**< \brief Pin Controlled by PB2 */
#define PIO_PB3              (1u << 3)  /**< \brief Pin Controlled by PB3 */
#define PIO_PB4              (1u << 4)  /**< \brief Pin Controlled by PB4 */
#define PIO_PB5              (1u << 5)  /**< \brief Pin Controlled by PB5 */
#define PIO_PB6              (1u << 6)  /**< \brief Pin Controlled by PB6 */
#define PIO_PB7              (1u << 7)  /**< \brief Pin Controlled by PB7 */
#define PIO_PB8              (1u << 8)  /**< \brief Pin Controlled by PB8 */
#define PIO_PB9              (1u << 9)  /**< \brief Pin Controlled by PB9 */
#define PIO_PB10             (1u << 10) /**< \brief Pin Controlled by PB10 */
#define PIO_PB11             (1u << 11) /**< \brief Pin Controlled by PB11 */
#define PIO_PB12             (1u << 12) /**< \brief Pin Controlled by PB12 */
/* ========== Pio definition for ADC peripheral ========== */
#define PIO_PA17X1_AD0       (1u << 17) /**< \brief Adc signal: AD0 */
#define PIO_PA18X1_AD1       (1u << 18) /**< \brief Adc signal: AD1 */
#define PIO_PA19X1_AD2       (1u << 19) /**< \brief Adc signal: AD2 */
#define PIO_PA20X1_AD3       (1u << 20) /**< \brief Adc signal: AD3 */
#define PIO_PB0X1_AD4        (1u << 0)  /**< \brief Adc signal: AD4 */
#define PIO_PB1X1_AD5        (1u << 1)  /**< \brief Adc signal: AD5 */
#define PIO_PB2X1_AD6        (1u << 2)  /**< \brief Adc signal: AD6/WKUP12 */
#define PIO_PB2X1_WKUP12     (1u << 2)  /**< \brief Adc signal: AD6/WKUP12 */
#define PIO_PB3X1_AD7        (1u << 3)  /**< \brief Adc signal: AD7/WKUP13 */
#define PIO_PB3X1_WKUP13     (1u << 3)  /**< \brief Adc signal: AD7/WKUP13 */
#define PIO_PA8B_ADTRG       (1u << 8)  /**< \brief Adc signal: ADTRG */
/* ========== Pio definition for I2SC0 peripheral ========== */
#define PIO_PA0A_I2SCK0      (1u << 0)  /**< \brief I2sc0 signal: I2SCK0 */
#define PIO_PA2B_I2SDI0      (1u << 2)  /**< \brief I2sc0 signal: I2SDI0 */
#define PIO_PA3B_I2SDO0      (1u << 3)  /**< \brief I2sc0 signal: I2SDO0 */
#define PIO_PA17A_I2SDO0     (1u << 17) /**< \brief I2sc0 signal: I2SDO0 */
#define PIO_PA4B_I2SMCK0     (1u << 4)  /**< \brief I2sc0 signal: I2SMCK0 */
#define PIO_PA18A_I2SMCK0    (1u << 18) /**< \brief I2sc0 signal: I2SMCK0 */
#define PIO_PA1A_I2SWS0      (1u << 1)  /**< \brief I2sc0 signal: I2SWS0 */
#define PIO_PA10B_PDMCLK0    (1u << 10) /**< \brief I2sc0 signal: PDMCLK0 */
#define PIO_PA9B_PDMDAT0     (1u << 9)  /**< \brief I2sc0 signal: PDMDAT0 */
/* ========== Pio definition for I2SC1 peripheral ========== */
#define PIO_PA19B_I2SCK1     (1u << 19) /**< \brief I2sc1 signal: I2SCK1 */
#define PIO_PA22B_I2SDI1     (1u << 22) /**< \brief I2sc1 signal: I2SDI1 */
#define PIO_PA23A_I2SDO1     (1u << 23) /**< \brief I2sc1 signal: I2SDO1 */
#define PIO_PA25B_I2SDO1     (1u << 25) /**< \brief I2sc1 signal: I2SDO1 */
#define PIO_PA24A_I2SMCK1    (1u << 24) /**< \brief I2sc1 signal: I2SMCK1 */
#define PIO_PA26B_I2SMCK1    (1u << 26) /**< \brief I2sc1 signal: I2SMCK1 */
#define PIO_PA20B_I2SWS1     (1u << 20) /**< \brief I2sc1 signal: I2SWS1 */
/* ========== Pio definition for PMC peripheral ========== */
#define PIO_PA6B_PCK0        (1u << 6)  /**< \brief Pmc signal: PCK0 */
#define PIO_PA17B_PCK1       (1u << 17) /**< \brief Pmc signal: PCK1 */
#define PIO_PA21B_PCK1       (1u << 21) /**< \brief Pmc signal: PCK1 */
#define PIO_PA30A_PCK1       (1u << 30) /**< \brief Pmc signal: PCK1 */
#define PIO_PA18B_PCK2       (1u << 18) /**< \brief Pmc signal: PCK2 */
#define PIO_PA31A_PCK2       (1u << 31) /**< \brief Pmc signal: PCK2 */
#define PIO_PB3B_PCK2        (1u << 3)  /**< \brief Pmc signal: PCK2 */
/* ========== Pio definition for SPI0 peripheral ========== */
#define PIO_PA9A_RXD0        (1u << 9)  /**< \brief Spi0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PA9A_SPI0_MISO   (1u << 9)  /**< \brief Spi0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PA9A_TWCK0       (1u << 9)  /**< \brief Spi0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PB0A_SCK0        (1u << 0)  /**< \brief Spi0 signal: SCK0/SPI0_SPCK */
#define PIO_PB0A_SPI0_SPCK   (1u << 0)  /**< \brief Spi0 signal: SCK0/SPI0_SPCK */
#define PIO_PA25A_SPI0_NPCS0 (1u << 25) /**< \brief Spi0 signal: SPI0_NPCS0/CTS0 */
#define PIO_PA25A_CTS0       (1u << 25) /**< \brief Spi0 signal: SPI0_NPCS0/CTS0 */
#define PIO_PA26A_SPI0_NPCS1 (1u << 26) /**< \brief Spi0 signal: SPI0_NPCS1/RTS0 */
#define PIO_PA26A_RTS0       (1u << 26) /**< \brief Spi0 signal: SPI0_NPCS1/RTS0 */
#define PIO_PA10A_TXD0       (1u << 10) /**< \brief Spi0 signal: TXD0/SPI0_MOSI/TWD0 */
#define PIO_PA10A_SPI0_MOSI  (1u << 10) /**< \brief Spi0 signal: TXD0/SPI0_MOSI/TWD0 */
#define PIO_PA10A_TWD0       (1u << 10) /**< \brief Spi0 signal: TXD0/SPI0_MOSI/TWD0 */
/* ========== Pio definition for SPI1 peripheral ========== */
#define PIO_PB2A_RXD1        (1u << 2)  /**< \brief Spi1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PB2A_SPI1_MISO   (1u << 2)  /**< \brief Spi1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PB2A_TWCK1       (1u << 2)  /**< \brief Spi1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PA27A_SCK1       (1u << 27) /**< \brief Spi1 signal: SCK1/SPI1_SPCK */
#define PIO_PA27A_SPI1_SPCK  (1u << 27) /**< \brief Spi1 signal: SCK1/SPI1_SPCK */
#define PIO_PA28A_SPI1_NPCS0 (1u << 28) /**< \brief Spi1 signal: SPI1_NPCS0/CTS1 */
#define PIO_PA28A_CTS1       (1u << 28) /**< \brief Spi1 signal: SPI1_NPCS0/CTS1 */
#define PIO_PA29A_SPI1_NPCS1 (1u << 29) /**< \brief Spi1 signal: SPI1_NPCS1/RTS1 */
#define PIO_PA29A_RTS1       (1u << 29) /**< \brief Spi1 signal: SPI1_NPCS1/RTS1 */
#define PIO_PB3A_TXD1        (1u << 3)  /**< \brief Spi1 signal: TXD1/SPI1_MOSI/TWD1 */
#define PIO_PB3A_SPI1_MOSI   (1u << 3)  /**< \brief Spi1 signal: TXD1/SPI1_MOSI/TWD1 */
#define PIO_PB3A_TWD1        (1u << 3)  /**< \brief Spi1 signal: TXD1/SPI1_MOSI/TWD1 */
/* ========== Pio definition for SPI2 peripheral ========== */
#define PIO_PA5A_RXD2        (1u << 5)  /**< \brief Spi2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA5A_SPI2_MISO   (1u << 5)  /**< \brief Spi2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA5A_TWCK2       (1u << 5)  /**< \brief Spi2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA15B_SCK2       (1u << 15) /**< \brief Spi2 signal: SCK2/SPI2_SPCK */
#define PIO_PA15B_SPI2_SPCK  (1u << 15) /**< \brief Spi2 signal: SCK2/SPI2_SPCK */
#define PIO_PA24B_SCK2       (1u << 24) /**< \brief Spi2 signal: SCK2/SPI2_SPCK */
#define PIO_PA24B_SPI2_SPCK  (1u << 24) /**< \brief Spi2 signal: SCK2/SPI2_SPCK */
#define PIO_PA16A_SPI2_NPCS0 (1u << 16) /**< \brief Spi2 signal: SPI2_NPCS0/CTS2 */
#define PIO_PA16A_CTS2       (1u << 16) /**< \brief Spi2 signal: SPI2_NPCS0/CTS2 */
#define PIO_PA15A_SPI2_NPCS1 (1u << 15) /**< \brief Spi2 signal: SPI2_NPCS1/RTS2 */
#define PIO_PA15A_RTS2       (1u << 15) /**< \brief Spi2 signal: SPI2_NPCS1/RTS2 */
#define PIO_PA6A_TXD2        (1u << 6)  /**< \brief Spi2 signal: TXD2/SPI2_MOSI/TWD2 */
#define PIO_PA6A_SPI2_MOSI   (1u << 6)  /**< \brief Spi2 signal: TXD2/SPI2_MOSI/TWD2 */
#define PIO_PA6A_TWD2        (1u << 6)  /**< \brief Spi2 signal: TXD2/SPI2_MOSI/TWD2 */
/* ========== Pio definition for SPI3 peripheral ========== */
#define PIO_PA4A_RXD3        (1u << 4)  /**< \brief Spi3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PA4A_SPI3_MISO   (1u << 4)  /**< \brief Spi3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PA4A_TWCK3       (1u << 4)  /**< \brief Spi3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PB13A_SCK3       (1u << 13) /**< \brief Spi3 signal: SCK3/SPI3_SPCK */
#define PIO_PB13A_SPI3_SPCK  (1u << 13) /**< \brief Spi3 signal: SCK3/SPI3_SPCK */
#define PIO_PB14A_SPI3_NPCS0 (1u << 14) /**< \brief Spi3 signal: SPI3_NPCS0/CTS3 */
#define PIO_PB14A_CTS3       (1u << 14) /**< \brief Spi3 signal: SPI3_NPCS0/CTS3 */
#define PIO_PB15A_SPI3_NPCS1 (1u << 15) /**< \brief Spi3 signal: SPI3_NPCS1/RTS3 */
#define PIO_PB15A_RTS3       (1u << 15) /**< \brief Spi3 signal: SPI3_NPCS1/RTS3 */
#define PIO_PA3A_TXD3        (1u << 3)  /**< \brief Spi3 signal: TXD3/SPI3_MOSI/TWD3 */
#define PIO_PA3A_SPI3_MOSI   (1u << 3)  /**< \brief Spi3 signal: TXD3/SPI3_MOSI/TWD3 */
#define PIO_PA3A_TWD3        (1u << 3)  /**< \brief Spi3 signal: TXD3/SPI3_MOSI/TWD3 */
/* ========== Pio definition for SPI4 peripheral ========== */
#define PIO_PB9A_RXD4        (1u << 9)  /**< \brief Spi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB9A_SPI4_MISO   (1u << 9)  /**< \brief Spi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB9A_TWCK4       (1u << 9)  /**< \brief Spi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_RXD4       (1u << 11) /**< \brief Spi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_SPI4_MISO  (1u << 11) /**< \brief Spi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_TWCK4      (1u << 11) /**< \brief Spi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB1A_SCK4        (1u << 1)  /**< \brief Spi4 signal: SCK4/SPI4_SPCK */
#define PIO_PB1A_SPI4_SPCK   (1u << 1)  /**< \brief Spi4 signal: SCK4/SPI4_SPCK */
#define PIO_PB8B_SPI4_NPCS0  (1u << 8)  /**< \brief Spi4 signal: SPI4_NPCS0/CTS4 */
#define PIO_PB8B_CTS4        (1u << 8)  /**< \brief Spi4 signal: SPI4_NPCS0/CTS4 */
#define PIO_PB9B_SPI4_NPCS1  (1u << 9)  /**< \brief Spi4 signal: SPI4_NPCS1/RTS4 */
#define PIO_PB9B_RTS4        (1u << 9)  /**< \brief Spi4 signal: SPI4_NPCS1/RTS4 */
#define PIO_PB8A_TXD4        (1u << 8)  /**< \brief Spi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB8A_SPI4_MOSI   (1u << 8)  /**< \brief Spi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB8A_TWD4        (1u << 8)  /**< \brief Spi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_TXD4       (1u << 10) /**< \brief Spi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_SPI4_MOSI  (1u << 10) /**< \brief Spi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_TWD4       (1u << 10) /**< \brief Spi4 signal: TXD4/SPI4_MOSI/TWD4 */
/* ========== Pio definition for SPI5 peripheral ========== */
#define PIO_PA12A_RXD5       (1u << 12) /**< \brief Spi5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA12A_SPI5_MISO  (1u << 12) /**< \brief Spi5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA12A_TWCK5      (1u << 12) /**< \brief Spi5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA14A_SCK5       (1u << 14) /**< \brief Spi5 signal: SCK5/SPI5_SPCK */
#define PIO_PA14A_SPI5_SPCK  (1u << 14) /**< \brief Spi5 signal: SCK5/SPI5_SPCK */
#define PIO_PA11A_SPI5_NPCS0 (1u << 11) /**< \brief Spi5 signal: SPI5_NPCS0/CTS5 */
#define PIO_PA11A_CTS5       (1u << 11) /**< \brief Spi5 signal: SPI5_NPCS0/CTS5 */
#define PIO_PA5B_SPI5_NPCS1  (1u << 5)  /**< \brief Spi5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PA5B_RTS5        (1u << 5)  /**< \brief Spi5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PB2B_SPI5_NPCS1  (1u << 2)  /**< \brief Spi5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PB2B_RTS5        (1u << 2)  /**< \brief Spi5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PA13A_TXD5       (1u << 13) /**< \brief Spi5 signal: TXD5/SPI5_MOSI/TWD5 */
#define PIO_PA13A_SPI5_MOSI  (1u << 13) /**< \brief Spi5 signal: TXD5/SPI5_MOSI/TWD5 */
#define PIO_PA13A_TWD5       (1u << 13) /**< \brief Spi5 signal: TXD5/SPI5_MOSI/TWD5 */
/* ========== Pio definition for SPI6 peripheral ========== */
#define PIO_PB1B_RXD6        (1u << 1)  /**< \brief Spi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB1B_SPI6_MISO   (1u << 1)  /**< \brief Spi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB1B_TWCK6       (1u << 1)  /**< \brief Spi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_RXD6       (1u << 11) /**< \brief Spi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_SPI6_MISO  (1u << 11) /**< \brief Spi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_TWCK6      (1u << 11) /**< \brief Spi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB13B_SCK6       (1u << 13) /**< \brief Spi6 signal: SCK6/SPI6_SPCK */
#define PIO_PB13B_SPI6_SPCK  (1u << 13) /**< \brief Spi6 signal: SCK6/SPI6_SPCK */
#define PIO_PB14B_SPI6_NPCS0 (1u << 14) /**< \brief Spi6 signal: SPI6_NPCS0/CTS6 */
#define PIO_PB14B_CTS6       (1u << 14) /**< \brief Spi6 signal: SPI6_NPCS0/CTS6 */
#define PIO_PB15B_SPI6_NPCS1 (1u << 15) /**< \brief Spi6 signal: SPI6_NPCS1/RTS6 */
#define PIO_PB15B_RTS6       (1u << 15) /**< \brief Spi6 signal: SPI6_NPCS1/RTS6 */
#define PIO_PB0B_TXD6        (1u << 0)  /**< \brief Spi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB0B_SPI6_MOSI   (1u << 0)  /**< \brief Spi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB0B_TWD6        (1u << 0)  /**< \brief Spi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_TXD6       (1u << 10) /**< \brief Spi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_SPI6_MOSI  (1u << 10) /**< \brief Spi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_TWD6       (1u << 10) /**< \brief Spi6 signal: TXD6/SPI6_MOSI/TWD6 */
/* ========== Pio definition for TC0 peripheral ========== */
#define PIO_PA2A_TCLK0       (1u << 2)  /**< \brief Tc0 signal: TCLK0 */
#define PIO_PA19A_TCLK1      (1u << 19) /**< \brief Tc0 signal: TCLK1 */
#define PIO_PA20A_TCLK2      (1u << 20) /**< \brief Tc0 signal: TCLK2 */
#define PIO_PA0B_TIOA0       (1u << 0)  /**< \brief Tc0 signal: TIOA0 */
#define PIO_PA23B_TIOA1      (1u << 23) /**< \brief Tc0 signal: TIOA1 */
#define PIO_PA21A_TIOA2      (1u << 21) /**< \brief Tc0 signal: TIOA2 */
#define PIO_PA1B_TIOB0       (1u << 1)  /**< \brief Tc0 signal: TIOB0 */
#define PIO_PA16B_TIOB1      (1u << 16) /**< \brief Tc0 signal: TIOB1 */
#define PIO_PA22A_TIOB2      (1u << 22) /**< \brief Tc0 signal: TIOB2 */
/* ========== Pio definition for TWI0 peripheral ========== */
#define PIO_PA9A_RXD0        (1u << 9)  /**< \brief Twi0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PA9A_SPI0_MISO   (1u << 9)  /**< \brief Twi0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PA9A_TWCK0       (1u << 9)  /**< \brief Twi0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PA10A_TXD0       (1u << 10) /**< \brief Twi0 signal: TXD0/SPI0_MOSI/TWD0 */
#define PIO_PA10A_SPI0_MOSI  (1u << 10) /**< \brief Twi0 signal: TXD0/SPI0_MOSI/TWD0 */
#define PIO_PA10A_TWD0       (1u << 10) /**< \brief Twi0 signal: TXD0/SPI0_MOSI/TWD0 */
/* ========== Pio definition for TWI1 peripheral ========== */
#define PIO_PB2A_RXD1        (1u << 2)  /**< \brief Twi1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PB2A_SPI1_MISO   (1u << 2)  /**< \brief Twi1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PB2A_TWCK1       (1u << 2)  /**< \brief Twi1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PB3A_TXD1        (1u << 3)  /**< \brief Twi1 signal: TXD1/SPI1_MOSI/TWD1 */
#define PIO_PB3A_SPI1_MOSI   (1u << 3)  /**< \brief Twi1 signal: TXD1/SPI1_MOSI/TWD1 */
#define PIO_PB3A_TWD1        (1u << 3)  /**< \brief Twi1 signal: TXD1/SPI1_MOSI/TWD1 */
/* ========== Pio definition for TWI2 peripheral ========== */
#define PIO_PA5A_RXD2        (1u << 5)  /**< \brief Twi2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA5A_SPI2_MISO   (1u << 5)  /**< \brief Twi2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA5A_TWCK2       (1u << 5)  /**< \brief Twi2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA6A_TXD2        (1u << 6)  /**< \brief Twi2 signal: TXD2/SPI2_MOSI/TWD2 */
#define PIO_PA6A_SPI2_MOSI   (1u << 6)  /**< \brief Twi2 signal: TXD2/SPI2_MOSI/TWD2 */
#define PIO_PA6A_TWD2        (1u << 6)  /**< \brief Twi2 signal: TXD2/SPI2_MOSI/TWD2 */
/* ========== Pio definition for TWI3 peripheral ========== */
#define PIO_PA4A_RXD3        (1u << 4)  /**< \brief Twi3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PA4A_SPI3_MISO   (1u << 4)  /**< \brief Twi3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PA4A_TWCK3       (1u << 4)  /**< \brief Twi3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PA3A_TXD3        (1u << 3)  /**< \brief Twi3 signal: TXD3/SPI3_MOSI/TWD3 */
#define PIO_PA3A_SPI3_MOSI   (1u << 3)  /**< \brief Twi3 signal: TXD3/SPI3_MOSI/TWD3 */
#define PIO_PA3A_TWD3        (1u << 3)  /**< \brief Twi3 signal: TXD3/SPI3_MOSI/TWD3 */
/* ========== Pio definition for TWI4 peripheral ========== */
#define PIO_PB9A_RXD4        (1u << 9)  /**< \brief Twi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB9A_SPI4_MISO   (1u << 9)  /**< \brief Twi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB9A_TWCK4       (1u << 9)  /**< \brief Twi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_RXD4       (1u << 11) /**< \brief Twi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_SPI4_MISO  (1u << 11) /**< \brief Twi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_TWCK4      (1u << 11) /**< \brief Twi4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB8A_TXD4        (1u << 8)  /**< \brief Twi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB8A_SPI4_MOSI   (1u << 8)  /**< \brief Twi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB8A_TWD4        (1u << 8)  /**< \brief Twi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_TXD4       (1u << 10) /**< \brief Twi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_SPI4_MOSI  (1u << 10) /**< \brief Twi4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_TWD4       (1u << 10) /**< \brief Twi4 signal: TXD4/SPI4_MOSI/TWD4 */
/* ========== Pio definition for TWI5 peripheral ========== */
#define PIO_PA12A_RXD5       (1u << 12) /**< \brief Twi5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA12A_SPI5_MISO  (1u << 12) /**< \brief Twi5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA12A_TWCK5      (1u << 12) /**< \brief Twi5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA13A_TXD5       (1u << 13) /**< \brief Twi5 signal: TXD5/SPI5_MOSI/TWD5 */
#define PIO_PA13A_SPI5_MOSI  (1u << 13) /**< \brief Twi5 signal: TXD5/SPI5_MOSI/TWD5 */
#define PIO_PA13A_TWD5       (1u << 13) /**< \brief Twi5 signal: TXD5/SPI5_MOSI/TWD5 */
/* ========== Pio definition for TWI6 peripheral ========== */
#define PIO_PB1B_RXD6        (1u << 1)  /**< \brief Twi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB1B_SPI6_MISO   (1u << 1)  /**< \brief Twi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB1B_TWCK6       (1u << 1)  /**< \brief Twi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_RXD6       (1u << 11) /**< \brief Twi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_SPI6_MISO  (1u << 11) /**< \brief Twi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_TWCK6      (1u << 11) /**< \brief Twi6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB0B_TXD6        (1u << 0)  /**< \brief Twi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB0B_SPI6_MOSI   (1u << 0)  /**< \brief Twi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB0B_TWD6        (1u << 0)  /**< \brief Twi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_TXD6       (1u << 10) /**< \brief Twi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_SPI6_MOSI  (1u << 10) /**< \brief Twi6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_TWD6       (1u << 10) /**< \brief Twi6 signal: TXD6/SPI6_MOSI/TWD6 */
/* ========== Pio definition for USART0 peripheral ========== */
#define PIO_PA9A_RXD0        (1u << 9)  /**< \brief Usart0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PA9A_SPI0_MISO   (1u << 9)  /**< \brief Usart0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PA9A_TWCK0       (1u << 9)  /**< \brief Usart0 signal: RXD0/SPI0_MISO/TWCK0 */
#define PIO_PB0A_SCK0        (1u << 0)  /**< \brief Usart0 signal: SCK0/SPI0_SPCK */
#define PIO_PB0A_SPI0_SPCK   (1u << 0)  /**< \brief Usart0 signal: SCK0/SPI0_SPCK */
#define PIO_PA25A_SPI0_NPCS0 (1u << 25) /**< \brief Usart0 signal: SPI0_NPCS0/CTS0 */
#define PIO_PA25A_CTS0       (1u << 25) /**< \brief Usart0 signal: SPI0_NPCS0/CTS0 */
#define PIO_PA26A_SPI0_NPCS1 (1u << 26) /**< \brief Usart0 signal: SPI0_NPCS1/RTS0 */
#define PIO_PA26A_RTS0       (1u << 26) /**< \brief Usart0 signal: SPI0_NPCS1/RTS0 */
#define PIO_PA10A_TXD0       (1u << 10) /**< \brief Usart0 signal: TXD0/SPI0_MOSI/TWD0 */
#define PIO_PA10A_SPI0_MOSI  (1u << 10) /**< \brief Usart0 signal: TXD0/SPI0_MOSI/TWD0 */
#define PIO_PA10A_TWD0       (1u << 10) /**< \brief Usart0 signal: TXD0/SPI0_MOSI/TWD0 */
/* ========== Pio definition for USART1 peripheral ========== */
#define PIO_PB2A_RXD1        (1u << 2)  /**< \brief Usart1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PB2A_SPI1_MISO   (1u << 2)  /**< \brief Usart1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PB2A_TWCK1       (1u << 2)  /**< \brief Usart1 signal: RXD1/SPI1_MISO/TWCK1 */
#define PIO_PA27A_SCK1       (1u << 27) /**< \brief Usart1 signal: SCK1/SPI1_SPCK */
#define PIO_PA27A_SPI1_SPCK  (1u << 27) /**< \brief Usart1 signal: SCK1/SPI1_SPCK */
#define PIO_PA28A_SPI1_NPCS0 (1u << 28) /**< \brief Usart1 signal: SPI1_NPCS0/CTS1 */
#define PIO_PA28A_CTS1       (1u << 28) /**< \brief Usart1 signal: SPI1_NPCS0/CTS1 */
#define PIO_PA29A_SPI1_NPCS1 (1u << 29) /**< \brief Usart1 signal: SPI1_NPCS1/RTS1 */
#define PIO_PA29A_RTS1       (1u << 29) /**< \brief Usart1 signal: SPI1_NPCS1/RTS1 */
#define PIO_PB3A_TXD1        (1u << 3)  /**< \brief Usart1 signal: TXD1/SPI1_MOSI/TWD1 */
#define PIO_PB3A_SPI1_MOSI   (1u << 3)  /**< \brief Usart1 signal: TXD1/SPI1_MOSI/TWD1 */
#define PIO_PB3A_TWD1        (1u << 3)  /**< \brief Usart1 signal: TXD1/SPI1_MOSI/TWD1 */
/* ========== Pio definition for USART2 peripheral ========== */
#define PIO_PA5A_RXD2        (1u << 5)  /**< \brief Usart2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA5A_SPI2_MISO   (1u << 5)  /**< \brief Usart2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA5A_TWCK2       (1u << 5)  /**< \brief Usart2 signal: RXD2/SPI2_MISO/TWCK2 */
#define PIO_PA15B_SCK2       (1u << 15) /**< \brief Usart2 signal: SCK2/SPI2_SPCK */
#define PIO_PA15B_SPI2_SPCK  (1u << 15) /**< \brief Usart2 signal: SCK2/SPI2_SPCK */
#define PIO_PA24B_SCK2       (1u << 24) /**< \brief Usart2 signal: SCK2/SPI2_SPCK */
#define PIO_PA24B_SPI2_SPCK  (1u << 24) /**< \brief Usart2 signal: SCK2/SPI2_SPCK */
#define PIO_PA16A_SPI2_NPCS0 (1u << 16) /**< \brief Usart2 signal: SPI2_NPCS0/CTS2 */
#define PIO_PA16A_CTS2       (1u << 16) /**< \brief Usart2 signal: SPI2_NPCS0/CTS2 */
#define PIO_PA15A_SPI2_NPCS1 (1u << 15) /**< \brief Usart2 signal: SPI2_NPCS1/RTS2 */
#define PIO_PA15A_RTS2       (1u << 15) /**< \brief Usart2 signal: SPI2_NPCS1/RTS2 */
#define PIO_PA6A_TXD2        (1u << 6)  /**< \brief Usart2 signal: TXD2/SPI2_MOSI/TWD2 */
#define PIO_PA6A_SPI2_MOSI   (1u << 6)  /**< \brief Usart2 signal: TXD2/SPI2_MOSI/TWD2 */
#define PIO_PA6A_TWD2        (1u << 6)  /**< \brief Usart2 signal: TXD2/SPI2_MOSI/TWD2 */
/* ========== Pio definition for USART3 peripheral ========== */
#define PIO_PA4A_RXD3        (1u << 4)  /**< \brief Usart3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PA4A_SPI3_MISO   (1u << 4)  /**< \brief Usart3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PA4A_TWCK3       (1u << 4)  /**< \brief Usart3 signal: RXD3/SPI3_MISO/TWCK3 */
#define PIO_PB13A_SCK3       (1u << 13) /**< \brief Usart3 signal: SCK3/SPI3_SPCK */
#define PIO_PB13A_SPI3_SPCK  (1u << 13) /**< \brief Usart3 signal: SCK3/SPI3_SPCK */
#define PIO_PB14A_SPI3_NPCS0 (1u << 14) /**< \brief Usart3 signal: SPI3_NPCS0/CTS3 */
#define PIO_PB14A_CTS3       (1u << 14) /**< \brief Usart3 signal: SPI3_NPCS0/CTS3 */
#define PIO_PB15A_SPI3_NPCS1 (1u << 15) /**< \brief Usart3 signal: SPI3_NPCS1/RTS3 */
#define PIO_PB15A_RTS3       (1u << 15) /**< \brief Usart3 signal: SPI3_NPCS1/RTS3 */
#define PIO_PA3A_TXD3        (1u << 3)  /**< \brief Usart3 signal: TXD3/SPI3_MOSI/TWD3 */
#define PIO_PA3A_SPI3_MOSI   (1u << 3)  /**< \brief Usart3 signal: TXD3/SPI3_MOSI/TWD3 */
#define PIO_PA3A_TWD3        (1u << 3)  /**< \brief Usart3 signal: TXD3/SPI3_MOSI/TWD3 */
/* ========== Pio definition for USART4 peripheral ========== */
#define PIO_PB9A_RXD4        (1u << 9)  /**< \brief Usart4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB9A_SPI4_MISO   (1u << 9)  /**< \brief Usart4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB9A_TWCK4       (1u << 9)  /**< \brief Usart4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_RXD4       (1u << 11) /**< \brief Usart4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_SPI4_MISO  (1u << 11) /**< \brief Usart4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB11A_TWCK4      (1u << 11) /**< \brief Usart4 signal: RXD4/SPI4_MISO/TWCK4 */
#define PIO_PB1A_SCK4        (1u << 1)  /**< \brief Usart4 signal: SCK4/SPI4_SPCK */
#define PIO_PB1A_SPI4_SPCK   (1u << 1)  /**< \brief Usart4 signal: SCK4/SPI4_SPCK */
#define PIO_PB8B_SPI4_NPCS0  (1u << 8)  /**< \brief Usart4 signal: SPI4_NPCS0/CTS4 */
#define PIO_PB8B_CTS4        (1u << 8)  /**< \brief Usart4 signal: SPI4_NPCS0/CTS4 */
#define PIO_PB9B_SPI4_NPCS1  (1u << 9)  /**< \brief Usart4 signal: SPI4_NPCS1/RTS4 */
#define PIO_PB9B_RTS4        (1u << 9)  /**< \brief Usart4 signal: SPI4_NPCS1/RTS4 */
#define PIO_PB8A_TXD4        (1u << 8)  /**< \brief Usart4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB8A_SPI4_MOSI   (1u << 8)  /**< \brief Usart4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB8A_TWD4        (1u << 8)  /**< \brief Usart4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_TXD4       (1u << 10) /**< \brief Usart4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_SPI4_MOSI  (1u << 10) /**< \brief Usart4 signal: TXD4/SPI4_MOSI/TWD4 */
#define PIO_PB10A_TWD4       (1u << 10) /**< \brief Usart4 signal: TXD4/SPI4_MOSI/TWD4 */
/* ========== Pio definition for USART5 peripheral ========== */
#define PIO_PA12A_RXD5       (1u << 12) /**< \brief Usart5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA12A_SPI5_MISO  (1u << 12) /**< \brief Usart5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA12A_TWCK5      (1u << 12) /**< \brief Usart5 signal: RXD5/SPI5_MISO/TWCK5 */
#define PIO_PA14A_SCK5       (1u << 14) /**< \brief Usart5 signal: SCK5/SPI5_SPCK */
#define PIO_PA14A_SPI5_SPCK  (1u << 14) /**< \brief Usart5 signal: SCK5/SPI5_SPCK */
#define PIO_PA11A_SPI5_NPCS0 (1u << 11) /**< \brief Usart5 signal: SPI5_NPCS0/CTS5 */
#define PIO_PA11A_CTS5       (1u << 11) /**< \brief Usart5 signal: SPI5_NPCS0/CTS5 */
#define PIO_PA5B_SPI5_NPCS1  (1u << 5)  /**< \brief Usart5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PA5B_RTS5        (1u << 5)  /**< \brief Usart5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PB2B_SPI5_NPCS1  (1u << 2)  /**< \brief Usart5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PB2B_RTS5        (1u << 2)  /**< \brief Usart5 signal: SPI5_NPCS1/RTS5 */
#define PIO_PA13A_TXD5       (1u << 13) /**< \brief Usart5 signal: TXD5/SPI5_MOSI/TWD5 */
#define PIO_PA13A_SPI5_MOSI  (1u << 13) /**< \brief Usart5 signal: TXD5/SPI5_MOSI/TWD5 */
#define PIO_PA13A_TWD5       (1u << 13) /**< \brief Usart5 signal: TXD5/SPI5_MOSI/TWD5 */
/* ========== Pio definition for USART6 peripheral ========== */
#define PIO_PB1B_RXD6        (1u << 1)  /**< \brief Usart6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB1B_SPI6_MISO   (1u << 1)  /**< \brief Usart6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB1B_TWCK6       (1u << 1)  /**< \brief Usart6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_RXD6       (1u << 11) /**< \brief Usart6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_SPI6_MISO  (1u << 11) /**< \brief Usart6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB11B_TWCK6      (1u << 11) /**< \brief Usart6 signal: RXD6/SPI6_MISO/TWCK6 */
#define PIO_PB13B_SCK6       (1u << 13) /**< \brief Usart6 signal: SCK6/SPI6_SPCK */
#define PIO_PB13B_SPI6_SPCK  (1u << 13) /**< \brief Usart6 signal: SCK6/SPI6_SPCK */
#define PIO_PB14B_SPI6_NPCS0 (1u << 14) /**< \brief Usart6 signal: SPI6_NPCS0/CTS6 */
#define PIO_PB14B_CTS6       (1u << 14) /**< \brief Usart6 signal: SPI6_NPCS0/CTS6 */
#define PIO_PB15B_SPI6_NPCS1 (1u << 15) /**< \brief Usart6 signal: SPI6_NPCS1/RTS6 */
#define PIO_PB15B_RTS6       (1u << 15) /**< \brief Usart6 signal: SPI6_NPCS1/RTS6 */
#define PIO_PB0B_TXD6        (1u << 0)  /**< \brief Usart6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB0B_SPI6_MOSI   (1u << 0)  /**< \brief Usart6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB0B_TWD6        (1u << 0)  /**< \brief Usart6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_TXD6       (1u << 10) /**< \brief Usart6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_SPI6_MOSI  (1u << 10) /**< \brief Usart6 signal: TXD6/SPI6_MOSI/TWD6 */
#define PIO_PB10B_TWD6       (1u << 10) /**< \brief Usart6 signal: TXD6/SPI6_MOSI/TWD6 */
/* ========== Pio indexes ========== */
#define PIO_PA0_IDX          0
#define PIO_PA1_IDX          1
#define PIO_PA2_IDX          2
#define PIO_PA3_IDX          3
#define PIO_PA4_IDX          4
#define PIO_PA5_IDX          5
#define PIO_PA6_IDX          6
#define PIO_PA7_IDX          7
#define PIO_PA8_IDX          8
#define PIO_PA9_IDX          9
#define PIO_PA10_IDX         10
#define PIO_PA11_IDX         11
#define PIO_PA12_IDX         12
#define PIO_PA13_IDX         13
#define PIO_PA14_IDX         14
#define PIO_PA15_IDX         15
#define PIO_PA16_IDX         16
#define PIO_PA17_IDX         17
#define PIO_PA18_IDX         18
#define PIO_PA19_IDX         19
#define PIO_PA20_IDX         20
#define PIO_PA21_IDX         21
#define PIO_PA22_IDX         22
#define PIO_PA23_IDX         23
#define PIO_PA24_IDX         24
#define PIO_PB0_IDX          32
#define PIO_PB1_IDX          33
#define PIO_PB2_IDX          34
#define PIO_PB3_IDX          35
#define PIO_PB4_IDX          36
#define PIO_PB5_IDX          37
#define PIO_PB6_IDX          38
#define PIO_PB7_IDX          39
#define PIO_PB8_IDX          40
#define PIO_PB9_IDX          41
#define PIO_PB10_IDX         42
#define PIO_PB11_IDX         43
#define PIO_PB12_IDX         44

#endif /* _SAMG55G19_PIO_ */
