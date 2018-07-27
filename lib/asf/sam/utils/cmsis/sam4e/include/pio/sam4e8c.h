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

#ifndef _SAM4E8C_PIO_
#define _SAM4E8C_PIO_

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
#define PIO_PA25             (1u << 25) /**< \brief Pin Controlled by PA25 */
#define PIO_PA26             (1u << 26) /**< \brief Pin Controlled by PA26 */
#define PIO_PA27             (1u << 27) /**< \brief Pin Controlled by PA27 */
#define PIO_PA28             (1u << 28) /**< \brief Pin Controlled by PA28 */
#define PIO_PA29             (1u << 29) /**< \brief Pin Controlled by PA29 */
#define PIO_PA30             (1u << 30) /**< \brief Pin Controlled by PA30 */
#define PIO_PA31             (1u << 31) /**< \brief Pin Controlled by PA31 */
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
#define PIO_PB13             (1u << 13) /**< \brief Pin Controlled by PB13 */
#define PIO_PB14             (1u << 14) /**< \brief Pin Controlled by PB14 */
#define PIO_PD0              (1u << 0)  /**< \brief Pin Controlled by PD0 */
#define PIO_PD1              (1u << 1)  /**< \brief Pin Controlled by PD1 */
#define PIO_PD2              (1u << 2)  /**< \brief Pin Controlled by PD2 */
#define PIO_PD3              (1u << 3)  /**< \brief Pin Controlled by PD3 */
#define PIO_PD4              (1u << 4)  /**< \brief Pin Controlled by PD4 */
#define PIO_PD5              (1u << 5)  /**< \brief Pin Controlled by PD5 */
#define PIO_PD6              (1u << 6)  /**< \brief Pin Controlled by PD6 */
#define PIO_PD7              (1u << 7)  /**< \brief Pin Controlled by PD7 */
#define PIO_PD8              (1u << 8)  /**< \brief Pin Controlled by PD8 */
#define PIO_PD9              (1u << 9)  /**< \brief Pin Controlled by PD9 */
#define PIO_PD10             (1u << 10) /**< \brief Pin Controlled by PD10 */
#define PIO_PD11             (1u << 11) /**< \brief Pin Controlled by PD11 */
#define PIO_PD12             (1u << 12) /**< \brief Pin Controlled by PD12 */
#define PIO_PD13             (1u << 13) /**< \brief Pin Controlled by PD13 */
#define PIO_PD14             (1u << 14) /**< \brief Pin Controlled by PD14 */
#define PIO_PD15             (1u << 15) /**< \brief Pin Controlled by PD15 */
#define PIO_PD16             (1u << 16) /**< \brief Pin Controlled by PD16 */
#define PIO_PD17             (1u << 17) /**< \brief Pin Controlled by PD17 */
#define PIO_PD18             (1u << 18) /**< \brief Pin Controlled by PD18 */
#define PIO_PD19             (1u << 19) /**< \brief Pin Controlled by PD19 */
#define PIO_PD20             (1u << 20) /**< \brief Pin Controlled by PD20 */
#define PIO_PD21             (1u << 21) /**< \brief Pin Controlled by PD21 */
#define PIO_PD22             (1u << 22) /**< \brief Pin Controlled by PD22 */
#define PIO_PD23             (1u << 23) /**< \brief Pin Controlled by PD23 */
#define PIO_PD24             (1u << 24) /**< \brief Pin Controlled by PD24 */
#define PIO_PD25             (1u << 25) /**< \brief Pin Controlled by PD25 */
#define PIO_PD26             (1u << 26) /**< \brief Pin Controlled by PD26 */
#define PIO_PD27             (1u << 27) /**< \brief Pin Controlled by PD27 */
#define PIO_PD28             (1u << 28) /**< \brief Pin Controlled by PD28 */
#define PIO_PD29             (1u << 29) /**< \brief Pin Controlled by PD29 */
#define PIO_PD30             (1u << 30) /**< \brief Pin Controlled by PD30 */
#define PIO_PD31             (1u << 31) /**< \brief Pin Controlled by PD31 */
/* ========== Pio definition for AFEC0 peripheral ========== */
#define PIO_PA17X1_AFE0_AD0  (1u << 17) /**< \brief Afec0 signal: AFE0_AD0 */
#define PIO_PA18X1_AFE0_AD1  (1u << 18) /**< \brief Afec0 signal: AFE0_AD1 */
#define PIO_PC30X1_AFE0_AD10 (1u << 30) /**< \brief Afec0 signal: AFE0_AD10 */
#define PIO_PC31X1_AFE0_AD11 (1u << 31) /**< \brief Afec0 signal: AFE0_AD11 */
#define PIO_PC26X1_AFE0_AD12 (1u << 26) /**< \brief Afec0 signal: AFE0_AD12 */
#define PIO_PC27X1_AFE0_AD13 (1u << 27) /**< \brief Afec0 signal: AFE0_AD13 */
#define PIO_PC0X1_AFE0_AD14  (1u << 0)  /**< \brief Afec0 signal: AFE0_AD14 */
#define PIO_PA19X1_AFE0_AD2  (1u << 19) /**< \brief Afec0 signal: AFE0_AD2/WKUP9 */
#define PIO_PA19X1_WKUP9     (1u << 19) /**< \brief Afec0 signal: AFE0_AD2/WKUP9 */
#define PIO_PA20X1_AFE0_AD3  (1u << 20) /**< \brief Afec0 signal: AFE0_AD3/WKUP10 */
#define PIO_PA20X1_WKUP10    (1u << 20) /**< \brief Afec0 signal: AFE0_AD3/WKUP10 */
#define PIO_PB0X1_AFE0_AD4   (1u << 0)  /**< \brief Afec0 signal: AFE0_AD4/RTCOUT0 */
#define PIO_PB0X1_RTCOUT0    (1u << 0)  /**< \brief Afec0 signal: AFE0_AD4/RTCOUT0 */
#define PIO_PB1X1_AFE0_AD5   (1u << 1)  /**< \brief Afec0 signal: AFE0_AD5/RTCOUT1 */
#define PIO_PB1X1_RTCOUT1    (1u << 1)  /**< \brief Afec0 signal: AFE0_AD5/RTCOUT1 */
#define PIO_PC13X1_AFE0_AD6  (1u << 13) /**< \brief Afec0 signal: AFE0_AD6 */
#define PIO_PC15X1_AFE0_AD7  (1u << 15) /**< \brief Afec0 signal: AFE0_AD7 */
#define PIO_PC12X1_AFE0_AD8  (1u << 12) /**< \brief Afec0 signal: AFE0_AD8 */
#define PIO_PC29X1_AFE0_AD9  (1u << 29) /**< \brief Afec0 signal: AFE0_AD9 */
#define PIO_PA8B_AFE0_ADTRG  (1u << 8)  /**< \brief Afec0 signal: AFE0_ADTRG */
/* ========== Pio definition for AFEC1 peripheral ========== */
#define PIO_PB2X1_AFE1_AD0   (1u << 2)  /**< \brief Afec1 signal: AFE1_AD0/WKUP12 */
#define PIO_PB2X1_WKUP12     (1u << 2)  /**< \brief Afec1 signal: AFE1_AD0/WKUP12 */
#define PIO_PB3X1_AFE1_AD1   (1u << 3)  /**< \brief Afec1 signal: AFE1_AD1 */
#define PIO_PA21X1_AFE1_AD2  (1u << 21) /**< \brief Afec1 signal: AFE1_AD2 */
#define PIO_PA22X1_AFE1_AD3  (1u << 22) /**< \brief Afec1 signal: AFE1_AD3 */
#define PIO_PC1X1_AFE1_AD4   (1u << 1)  /**< \brief Afec1 signal: AFE1_AD4 */
#define PIO_PC2X1_AFE1_AD5   (1u << 2)  /**< \brief Afec1 signal: AFE1_AD5 */
#define PIO_PC3X1_AFE1_AD6   (1u << 3)  /**< \brief Afec1 signal: AFE1_AD6 */
#define PIO_PC4X1_AFE1_AD7   (1u << 4)  /**< \brief Afec1 signal: AFE1_AD7 */
/* ========== Pio definition for CAN0 peripheral ========== */
#define PIO_PB3A_CANRX0      (1u << 3)  /**< \brief Can0 signal: CANRX0 */
#define PIO_PB2A_CANTX0      (1u << 2)  /**< \brief Can0 signal: CANTX0 */
/* ========== Pio definition for DACC peripheral ========== */
#define PIO_PB13X1_DAC0      (1u << 13) /**< \brief Dacc signal: DAC0 */
#define PIO_PB14X1_DAC1      (1u << 14) /**< \brief Dacc signal: DAC1 */
#define PIO_PA2C_DATRG       (1u << 2)  /**< \brief Dacc signal: DATRG */
/* ========== Pio definition for GMAC peripheral ========== */
#define PIO_PD13A_GCOL       (1u << 13) /**< \brief Gmac signal: GCOL */
#define PIO_PD10A_GCRS       (1u << 10) /**< \brief Gmac signal: GCRS */
#define PIO_PD4A_GCRSDV      (1u << 4)  /**< \brief Gmac signal: GCRSDV/GRXDV */
#define PIO_PD4A_GRXDV       (1u << 4)  /**< \brief Gmac signal: GCRSDV/GRXDV */
#define PIO_PD8A_GMDC        (1u << 8)  /**< \brief Gmac signal: GMDC */
#define PIO_PD9A_GMDIO       (1u << 9)  /**< \brief Gmac signal: GMDIO */
#define PIO_PD5A_GRX0        (1u << 5)  /**< \brief Gmac signal: GRX0 */
#define PIO_PD6A_GRX1        (1u << 6)  /**< \brief Gmac signal: GRX1 */
#define PIO_PD11A_GRX2       (1u << 11) /**< \brief Gmac signal: GRX2 */
#define PIO_PD12A_GRX3       (1u << 12) /**< \brief Gmac signal: GRX3 */
#define PIO_PD14A_GRXCK      (1u << 14) /**< \brief Gmac signal: GRXCK */
#define PIO_PD7A_GRXER       (1u << 7)  /**< \brief Gmac signal: GRXER */
#define PIO_PD2A_GTX0        (1u << 2)  /**< \brief Gmac signal: GTX0 */
#define PIO_PD3A_GTX1        (1u << 3)  /**< \brief Gmac signal: GTX1 */
#define PIO_PD15A_GTX2       (1u << 15) /**< \brief Gmac signal: GTX2 */
#define PIO_PD16A_GTX3       (1u << 16) /**< \brief Gmac signal: GTX3 */
#define PIO_PD0A_GTXCK       (1u << 0)  /**< \brief Gmac signal: GTXCK/GREFCK */
#define PIO_PD0A_GREFCK      (1u << 0)  /**< \brief Gmac signal: GTXCK/GREFCK */
#define PIO_PD1A_GTXEN       (1u << 1)  /**< \brief Gmac signal: GTXEN */
#define PIO_PD17A_GTXER      (1u << 17) /**< \brief Gmac signal: GTXER */
/* ========== Pio definition for HSMCI peripheral ========== */
#define PIO_PA28C_MCCDA      (1u << 28) /**< \brief Hsmci signal: MCCDA */
#define PIO_PA29C_MCCK       (1u << 29) /**< \brief Hsmci signal: MCCK */
#define PIO_PA30C_MCDA0      (1u << 30) /**< \brief Hsmci signal: MCDA0 */
#define PIO_PA31C_MCDA1      (1u << 31) /**< \brief Hsmci signal: MCDA1 */
#define PIO_PA26C_MCDA2      (1u << 26) /**< \brief Hsmci signal: MCDA2 */
#define PIO_PA27C_MCDA3      (1u << 27) /**< \brief Hsmci signal: MCDA3 */
/* ========== Pio definition for PIOA peripheral ========== */
#define PIO_PA24X1_PIODC0    (1u << 24) /**< \brief Pioa signal: PIODC0 */
#define PIO_PA25X1_PIODC1    (1u << 25) /**< \brief Pioa signal: PIODC1 */
#define PIO_PA26X1_PIODC2    (1u << 26) /**< \brief Pioa signal: PIODC2 */
#define PIO_PA27X1_PIODC3    (1u << 27) /**< \brief Pioa signal: PIODC3 */
#define PIO_PA28X1_PIODC4    (1u << 28) /**< \brief Pioa signal: PIODC4 */
#define PIO_PA29X1_PIODC5    (1u << 29) /**< \brief Pioa signal: PIODC5 */
#define PIO_PA31X1_PIODC7    (1u << 31) /**< \brief Pioa signal: PIODC7 */
#define PIO_PA23X1_PIODCCLK  (1u << 23) /**< \brief Pioa signal: PIODCCLK */
#define PIO_PA30X1_WKUP11    (1u << 30) /**< \brief Pioa signal: WKUP11/PIODC6 */
#define PIO_PA30X1_PIODC6    (1u << 30) /**< \brief Pioa signal: WKUP11/PIODC6 */
#define PIO_PA15X1_WKUP14    (1u << 15) /**< \brief Pioa signal: WKUP14/PIODCEN1 */
#define PIO_PA15X1_PIODCEN1  (1u << 15) /**< \brief Pioa signal: WKUP14/PIODCEN1 */
#define PIO_PA16X1_WKUP15    (1u << 16) /**< \brief Pioa signal: WKUP15/PIODCEN2 */
#define PIO_PA16X1_PIODCEN2  (1u << 16) /**< \brief Pioa signal: WKUP15/PIODCEN2 */
/* ========== Pio definition for PMC peripheral ========== */
#define PIO_PA6B_PCK0        (1u << 6)  /**< \brief Pmc signal: PCK0 */
#define PIO_PB13B_PCK0       (1u << 13) /**< \brief Pmc signal: PCK0 */
#define PIO_PA17B_PCK1       (1u << 17) /**< \brief Pmc signal: PCK1 */
#define PIO_PA21B_PCK1       (1u << 21) /**< \brief Pmc signal: PCK1 */
#define PIO_PA18B_PCK2       (1u << 18) /**< \brief Pmc signal: PCK2 */
#define PIO_PA31B_PCK2       (1u << 31) /**< \brief Pmc signal: PCK2 */
#define PIO_PB3B_PCK2        (1u << 3)  /**< \brief Pmc signal: PCK2 */
/* ========== Pio definition for PWM peripheral ========== */
#define PIO_PA9C_PWMFI0      (1u << 9)  /**< \brief Pwm signal: PWMFI0 */
#define PIO_PA0A_PWMH0       (1u << 0)  /**< \brief Pwm signal: PWMH0 */
#define PIO_PA11B_PWMH0      (1u << 11) /**< \brief Pwm signal: PWMH0 */
#define PIO_PA23B_PWMH0      (1u << 23) /**< \brief Pwm signal: PWMH0 */
#define PIO_PB0A_PWMH0       (1u << 0)  /**< \brief Pwm signal: PWMH0 */
#define PIO_PC18B_PWMH0      (1u << 18) /**< \brief Pwm signal: PWMH0 */
#define PIO_PD20A_PWMH0      (1u << 20) /**< \brief Pwm signal: PWMH0 */
#define PIO_PA1A_PWMH1       (1u << 1)  /**< \brief Pwm signal: PWMH1 */
#define PIO_PA12B_PWMH1      (1u << 12) /**< \brief Pwm signal: PWMH1 */
#define PIO_PA24B_PWMH1      (1u << 24) /**< \brief Pwm signal: PWMH1 */
#define PIO_PB1A_PWMH1       (1u << 1)  /**< \brief Pwm signal: PWMH1 */
#define PIO_PC19B_PWMH1      (1u << 19) /**< \brief Pwm signal: PWMH1 */
#define PIO_PD21A_PWMH1      (1u << 21) /**< \brief Pwm signal: PWMH1 */
#define PIO_PA2A_PWMH2       (1u << 2)  /**< \brief Pwm signal: PWMH2 */
#define PIO_PA13B_PWMH2      (1u << 13) /**< \brief Pwm signal: PWMH2 */
#define PIO_PA25B_PWMH2      (1u << 25) /**< \brief Pwm signal: PWMH2 */
#define PIO_PB4B_PWMH2       (1u << 4)  /**< \brief Pwm signal: PWMH2 */
#define PIO_PC20B_PWMH2      (1u << 20) /**< \brief Pwm signal: PWMH2 */
#define PIO_PD22A_PWMH2      (1u << 22) /**< \brief Pwm signal: PWMH2 */
#define PIO_PA7B_PWMH3       (1u << 7)  /**< \brief Pwm signal: PWMH3 */
#define PIO_PA14B_PWMH3      (1u << 14) /**< \brief Pwm signal: PWMH3 */
#define PIO_PA17C_PWMH3      (1u << 17) /**< \brief Pwm signal: PWMH3 */
#define PIO_PB14B_PWMH3      (1u << 14) /**< \brief Pwm signal: PWMH3 */
#define PIO_PC21B_PWMH3      (1u << 21) /**< \brief Pwm signal: PWMH3 */
#define PIO_PD23A_PWMH3      (1u << 23) /**< \brief Pwm signal: PWMH3 */
#define PIO_PA19B_PWML0      (1u << 19) /**< \brief Pwm signal: PWML0 */
#define PIO_PB5B_PWML0       (1u << 5)  /**< \brief Pwm signal: PWML0 */
#define PIO_PC0B_PWML0       (1u << 0)  /**< \brief Pwm signal: PWML0 */
#define PIO_PC13B_PWML0      (1u << 13) /**< \brief Pwm signal: PWML0 */
#define PIO_PD24A_PWML0      (1u << 24) /**< \brief Pwm signal: PWML0 */
#define PIO_PA20B_PWML1      (1u << 20) /**< \brief Pwm signal: PWML1 */
#define PIO_PB12A_PWML1      (1u << 12) /**< \brief Pwm signal: PWML1 */
#define PIO_PC1B_PWML1       (1u << 1)  /**< \brief Pwm signal: PWML1 */
#define PIO_PC15B_PWML1      (1u << 15) /**< \brief Pwm signal: PWML1 */
#define PIO_PD25A_PWML1      (1u << 25) /**< \brief Pwm signal: PWML1 */
#define PIO_PA16C_PWML2      (1u << 16) /**< \brief Pwm signal: PWML2 */
#define PIO_PA30A_PWML2      (1u << 30) /**< \brief Pwm signal: PWML2 */
#define PIO_PB13A_PWML2      (1u << 13) /**< \brief Pwm signal: PWML2 */
#define PIO_PC2B_PWML2       (1u << 2)  /**< \brief Pwm signal: PWML2 */
#define PIO_PD26A_PWML2      (1u << 26) /**< \brief Pwm signal: PWML2 */
#define PIO_PA15C_PWML3      (1u << 15) /**< \brief Pwm signal: PWML3 */
#define PIO_PC3B_PWML3       (1u << 3)  /**< \brief Pwm signal: PWML3 */
#define PIO_PC22B_PWML3      (1u << 22) /**< \brief Pwm signal: PWML3 */
#define PIO_PD27A_PWML3      (1u << 27) /**< \brief Pwm signal: PWML3 */
/* ========== Pio definition for SPI peripheral ========== */
#define PIO_PA12A_MISO       (1u << 12) /**< \brief Spi signal: MISO */
#define PIO_PA13A_MOSI       (1u << 13) /**< \brief Spi signal: MOSI */
#define PIO_PA11A_NPCS0      (1u << 11) /**< \brief Spi signal: NPCS0 */
#define PIO_PA9B_NPCS1       (1u << 9)  /**< \brief Spi signal: NPCS1 */
#define PIO_PA31A_NPCS1      (1u << 31) /**< \brief Spi signal: NPCS1 */
#define PIO_PB14A_NPCS1      (1u << 14) /**< \brief Spi signal: NPCS1 */
#define PIO_PC4B_NPCS1       (1u << 4)  /**< \brief Spi signal: NPCS1 */
#define PIO_PA10B_NPCS2      (1u << 10) /**< \brief Spi signal: NPCS2 */
#define PIO_PA30B_NPCS2      (1u << 30) /**< \brief Spi signal: NPCS2 */
#define PIO_PB2B_NPCS2       (1u << 2)  /**< \brief Spi signal: NPCS2 */
#define PIO_PA3B_NPCS3       (1u << 3)  /**< \brief Spi signal: NPCS3 */
#define PIO_PA5B_NPCS3       (1u << 5)  /**< \brief Spi signal: NPCS3 */
#define PIO_PA22B_NPCS3      (1u << 22) /**< \brief Spi signal: NPCS3 */
#define PIO_PA14A_SPCK       (1u << 14) /**< \brief Spi signal: SPCK */
/* ========== Pio definition for TC0 peripheral ========== */
#define PIO_PA4B_TCLK0       (1u << 4)  /**< \brief Tc0 signal: TCLK0 */
#define PIO_PA28B_TCLK1      (1u << 28) /**< \brief Tc0 signal: TCLK1 */
#define PIO_PA29B_TCLK2      (1u << 29) /**< \brief Tc0 signal: TCLK2 */
#define PIO_PA0B_TIOA0       (1u << 0)  /**< \brief Tc0 signal: TIOA0 */
#define PIO_PA15B_TIOA1      (1u << 15) /**< \brief Tc0 signal: TIOA1 */
#define PIO_PA26B_TIOA2      (1u << 26) /**< \brief Tc0 signal: TIOA2 */
#define PIO_PA1B_TIOB0       (1u << 1)  /**< \brief Tc0 signal: TIOB0 */
#define PIO_PA16B_TIOB1      (1u << 16) /**< \brief Tc0 signal: TIOB1 */
#define PIO_PA27B_TIOB2      (1u << 27) /**< \brief Tc0 signal: TIOB2 */
/* ========== Pio definition for TWI0 peripheral ========== */
#define PIO_PA4A_TWCK0       (1u << 4)  /**< \brief Twi0 signal: TWCK0 */
#define PIO_PA3A_TWD0        (1u << 3)  /**< \brief Twi0 signal: TWD0 */
/* ========== Pio definition for TWI1 peripheral ========== */
#define PIO_PB5A_TWCK1       (1u << 5)  /**< \brief Twi1 signal: TWCK1 */
#define PIO_PB4A_TWD1        (1u << 4)  /**< \brief Twi1 signal: TWD1 */
/* ========== Pio definition for UART0 peripheral ========== */
#define PIO_PA9A_URXD0       (1u << 9)  /**< \brief Uart0 signal: URXD0 */
#define PIO_PA10A_UTXD0      (1u << 10) /**< \brief Uart0 signal: UTXD0 */
/* ========== Pio definition for UART1 peripheral ========== */
#define PIO_PA5C_URXD1       (1u << 5)  /**< \brief Uart1 signal: URXD1 */
#define PIO_PA6C_UTXD1       (1u << 6)  /**< \brief Uart1 signal: UTXD1 */
/* ========== Pio definition for USART0 peripheral ========== */
#define PIO_PB2C_CTS0        (1u << 2)  /**< \brief Usart0 signal: CTS0 */
#define PIO_PB3C_RTS0        (1u << 3)  /**< \brief Usart0 signal: RTS0 */
#define PIO_PB0C_RXD0        (1u << 0)  /**< \brief Usart0 signal: RXD0 */
#define PIO_PB13C_SCK0       (1u << 13) /**< \brief Usart0 signal: SCK0 */
#define PIO_PB1C_TXD0        (1u << 1)  /**< \brief Usart0 signal: TXD0 */
/* ========== Pio definition for USART1 peripheral ========== */
#define PIO_PA25A_CTS1       (1u << 25) /**< \brief Usart1 signal: CTS1 */
#define PIO_PA26A_DCD1       (1u << 26) /**< \brief Usart1 signal: DCD1 */
#define PIO_PA28A_DSR1       (1u << 28) /**< \brief Usart1 signal: DSR1 */
#define PIO_PA27A_DTR1       (1u << 27) /**< \brief Usart1 signal: DTR1 */
#define PIO_PA29A_RI1        (1u << 29) /**< \brief Usart1 signal: RI1 */
#define PIO_PA24A_RTS1       (1u << 24) /**< \brief Usart1 signal: RTS1 */
#define PIO_PA21A_RXD1       (1u << 21) /**< \brief Usart1 signal: RXD1 */
#define PIO_PA23A_SCK1       (1u << 23) /**< \brief Usart1 signal: SCK1 */
#define PIO_PA22A_TXD1       (1u << 22) /**< \brief Usart1 signal: TXD1 */
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
#define PIO_PA25_IDX         25
#define PIO_PA26_IDX         26
#define PIO_PA27_IDX         27
#define PIO_PA28_IDX         28
#define PIO_PA29_IDX         29
#define PIO_PA30_IDX         30
#define PIO_PA31_IDX         31
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
#define PIO_PB13_IDX         45
#define PIO_PB14_IDX         46
#define PIO_PD0_IDX          96
#define PIO_PD1_IDX          97
#define PIO_PD2_IDX          98
#define PIO_PD3_IDX          99
#define PIO_PD4_IDX          100
#define PIO_PD5_IDX          101
#define PIO_PD6_IDX          102
#define PIO_PD7_IDX          103
#define PIO_PD8_IDX          104
#define PIO_PD9_IDX          105
#define PIO_PD10_IDX         106
#define PIO_PD11_IDX         107
#define PIO_PD12_IDX         108
#define PIO_PD13_IDX         109
#define PIO_PD14_IDX         110
#define PIO_PD15_IDX         111
#define PIO_PD16_IDX         112
#define PIO_PD17_IDX         113
#define PIO_PD18_IDX         114
#define PIO_PD19_IDX         115
#define PIO_PD20_IDX         116
#define PIO_PD21_IDX         117
#define PIO_PD22_IDX         118
#define PIO_PD23_IDX         119
#define PIO_PD24_IDX         120
#define PIO_PD25_IDX         121
#define PIO_PD26_IDX         122
#define PIO_PD27_IDX         123
#define PIO_PD28_IDX         124
#define PIO_PD29_IDX         125
#define PIO_PD30_IDX         126
#define PIO_PD31_IDX         127

#endif /* _SAM4E8C_PIO_ */
