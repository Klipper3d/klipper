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

#ifndef _SAM4C32E_PIO_
#define _SAM4C32E_PIO_

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
#define PIO_PB15             (1u << 15) /**< \brief Pin Controlled by PB15 */
#define PIO_PB16             (1u << 16) /**< \brief Pin Controlled by PB16 */
#define PIO_PB17             (1u << 17) /**< \brief Pin Controlled by PB17 */
#define PIO_PB18             (1u << 18) /**< \brief Pin Controlled by PB18 */
#define PIO_PB19             (1u << 19) /**< \brief Pin Controlled by PB19 */
#define PIO_PB20             (1u << 20) /**< \brief Pin Controlled by PB20 */
#define PIO_PB21             (1u << 21) /**< \brief Pin Controlled by PB21 */
#define PIO_PB22             (1u << 22) /**< \brief Pin Controlled by PB22 */
#define PIO_PB23             (1u << 23) /**< \brief Pin Controlled by PB23 */
#define PIO_PB24             (1u << 24) /**< \brief Pin Controlled by PB24 */
#define PIO_PB25             (1u << 25) /**< \brief Pin Controlled by PB25 */
#define PIO_PB26             (1u << 26) /**< \brief Pin Controlled by PB26 */
#define PIO_PB27             (1u << 27) /**< \brief Pin Controlled by PB27 */
#define PIO_PB28             (1u << 28) /**< \brief Pin Controlled by PB28 */
#define PIO_PB29             (1u << 29) /**< \brief Pin Controlled by PB29 */
#define PIO_PB30             (1u << 30) /**< \brief Pin Controlled by PB30 */
#define PIO_PB31             (1u << 31) /**< \brief Pin Controlled by PB31 */
#define PIO_PC0              (1u << 0)  /**< \brief Pin Controlled by PC0 */
#define PIO_PC1              (1u << 1)  /**< \brief Pin Controlled by PC1 */
#define PIO_PC2              (1u << 2)  /**< \brief Pin Controlled by PC2 */
#define PIO_PC3              (1u << 3)  /**< \brief Pin Controlled by PC3 */
#define PIO_PC4              (1u << 4)  /**< \brief Pin Controlled by PC4 */
#define PIO_PC5              (1u << 5)  /**< \brief Pin Controlled by PC5 */
#define PIO_PC6              (1u << 6)  /**< \brief Pin Controlled by PC6 */
#define PIO_PC7              (1u << 7)  /**< \brief Pin Controlled by PC7 */
#define PIO_PC8              (1u << 8)  /**< \brief Pin Controlled by PC8 */
#define PIO_PC9              (1u << 9)  /**< \brief Pin Controlled by PC9 */
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
/* ========== Pio definition for ADC peripheral ========== */
#define PIO_PB23A_ADTRG      (1u << 23) /**< \brief Adc signal: ADTRG */
#define PIO_PA4X1_COM4       (1u << 4)  /**< \brief Adc signal: COM4/AD1 */
#define PIO_PA4X1_AD1        (1u << 4)  /**< \brief Adc signal: COM4/AD1 */
#define PIO_PA5X1_COM5       (1u << 5)  /**< \brief Adc signal: COM5/AD2 */
#define PIO_PA5X1_AD2        (1u << 5)  /**< \brief Adc signal: COM5/AD2 */
#define PIO_PB13X1_SEG31     (1u << 13) /**< \brief Adc signal: SEG31/AD3 */
#define PIO_PB13X1_AD3       (1u << 13) /**< \brief Adc signal: SEG31/AD3 */
#define PIO_PB23X1_SEG41     (1u << 23) /**< \brief Adc signal: SEG41/AD4 */
#define PIO_PB23X1_AD4       (1u << 23) /**< \brief Adc signal: SEG41/AD4 */
#define PIO_PB31X1_SEG49     (1u << 31) /**< \brief Adc signal: SEG49/AD5 */
#define PIO_PB31X1_AD5       (1u << 31) /**< \brief Adc signal: SEG49/AD5 */
#define PIO_PA12X1_SEG6      (1u << 12) /**< \brief Adc signal: SEG6/AD0 */
#define PIO_PA12X1_AD0       (1u << 12) /**< \brief Adc signal: SEG6/AD0 */
/* ========== Pio definition for EBI peripheral ========== */
#define PIO_PA10C_A0         (1u << 10) /**< \brief Ebi signal: A0/NBS0 */
#define PIO_PA10C_NBS0       (1u << 10) /**< \brief Ebi signal: A0/NBS0 */
#define PIO_PB31C_A0         (1u << 31) /**< \brief Ebi signal: A0/NBS0 */
#define PIO_PB31C_NBS0       (1u << 31) /**< \brief Ebi signal: A0/NBS0 */
#define PIO_PA9C_A1          (1u << 9)  /**< \brief Ebi signal: A1 */
#define PIO_PB30C_A1         (1u << 30) /**< \brief Ebi signal: A1 */
#define PIO_PA0C_A10         (1u << 0)  /**< \brief Ebi signal: A10 */
#define PIO_PA24C_A10        (1u << 24) /**< \brief Ebi signal: A10 */
#define PIO_PA23C_A11        (1u << 23) /**< \brief Ebi signal: A11 */
#define PIO_PA22C_A12        (1u << 22) /**< \brief Ebi signal: A12 */
#define PIO_PA21C_A13        (1u << 21) /**< \brief Ebi signal: A13 */
#define PIO_PA20C_A14        (1u << 20) /**< \brief Ebi signal: A14 */
#define PIO_PA31C_A14        (1u << 31) /**< \brief Ebi signal: A14 */
#define PIO_PA19C_A15        (1u << 19) /**< \brief Ebi signal: A15 */
#define PIO_PA30C_A15        (1u << 30) /**< \brief Ebi signal: A15 */
#define PIO_PA18C_A16        (1u << 18) /**< \brief Ebi signal: A16 */
#define PIO_PB5C_A16         (1u << 5)  /**< \brief Ebi signal: A16 */
#define PIO_PA17C_A17        (1u << 17) /**< \brief Ebi signal: A17 */
#define PIO_PB4C_A17         (1u << 4)  /**< \brief Ebi signal: A17 */
#define PIO_PA16C_A18        (1u << 16) /**< \brief Ebi signal: A18 */
#define PIO_PA15C_A19        (1u << 15) /**< \brief Ebi signal: A19 */
#define PIO_PA8C_A2          (1u << 8)  /**< \brief Ebi signal: A2 */
#define PIO_PB29C_A2         (1u << 29) /**< \brief Ebi signal: A2 */
#define PIO_PA14C_A20        (1u << 14) /**< \brief Ebi signal: A20 */
#define PIO_PA13C_A21        (1u << 13) /**< \brief Ebi signal: A21/NANDALE */
#define PIO_PA13C_NANDALE    (1u << 13) /**< \brief Ebi signal: A21/NANDALE */
#define PIO_PA12C_A22        (1u << 12) /**< \brief Ebi signal: A22/NANDCLE */
#define PIO_PA12C_NANDCLE    (1u << 12) /**< \brief Ebi signal: A22/NANDCLE */
#define PIO_PA11C_A23        (1u << 11) /**< \brief Ebi signal: A23 */
#define PIO_PA7C_A3          (1u << 7)  /**< \brief Ebi signal: A3 */
#define PIO_PB28C_A3         (1u << 28) /**< \brief Ebi signal: A3 */
#define PIO_PA6C_A4          (1u << 6)  /**< \brief Ebi signal: A4 */
#define PIO_PB27C_A4         (1u << 27) /**< \brief Ebi signal: A4 */
#define PIO_PA5C_A5          (1u << 5)  /**< \brief Ebi signal: A5 */
#define PIO_PB26C_A5         (1u << 26) /**< \brief Ebi signal: A5 */
#define PIO_PA4C_A6          (1u << 4)  /**< \brief Ebi signal: A6 */
#define PIO_PB25C_A6         (1u << 25) /**< \brief Ebi signal: A6 */
#define PIO_PA3C_A7          (1u << 3)  /**< \brief Ebi signal: A7 */
#define PIO_PB24C_A7         (1u << 24) /**< \brief Ebi signal: A7 */
#define PIO_PA2C_A8          (1u << 2)  /**< \brief Ebi signal: A8 */
#define PIO_PA26C_A8         (1u << 26) /**< \brief Ebi signal: A8 */
#define PIO_PA1C_A9          (1u << 1)  /**< \brief Ebi signal: A9 */
#define PIO_PA25C_A9         (1u << 25) /**< \brief Ebi signal: A9 */
#define PIO_PB6C_D0          (1u << 6)  /**< \brief Ebi signal: D0 */
#define PIO_PB7C_D1          (1u << 7)  /**< \brief Ebi signal: D1 */
#define PIO_PB18C_D10        (1u << 18) /**< \brief Ebi signal: D10 */
#define PIO_PB19C_D11        (1u << 19) /**< \brief Ebi signal: D11 */
#define PIO_PB20C_D12        (1u << 20) /**< \brief Ebi signal: D12 */
#define PIO_PB21C_D13        (1u << 21) /**< \brief Ebi signal: D13 */
#define PIO_PB22C_D14        (1u << 22) /**< \brief Ebi signal: D14 */
#define PIO_PB23C_D15        (1u << 23) /**< \brief Ebi signal: D15 */
#define PIO_PB8C_D2          (1u << 8)  /**< \brief Ebi signal: D2 */
#define PIO_PB9C_D3          (1u << 9)  /**< \brief Ebi signal: D3 */
#define PIO_PB10C_D4         (1u << 10) /**< \brief Ebi signal: D4 */
#define PIO_PB11C_D5         (1u << 11) /**< \brief Ebi signal: D5 */
#define PIO_PB12C_D6         (1u << 12) /**< \brief Ebi signal: D6 */
#define PIO_PB13C_D7         (1u << 13) /**< \brief Ebi signal: D7 */
#define PIO_PB16C_D8         (1u << 16) /**< \brief Ebi signal: D8 */
#define PIO_PB17C_D9         (1u << 17) /**< \brief Ebi signal: D9 */
#define PIO_PB21B_NANDOE     (1u << 21) /**< \brief Ebi signal: NANDOE */
#define PIO_PB22B_NANDWE     (1u << 22) /**< \brief Ebi signal: NANDWE */
#define PIO_PA27C_NCS0       (1u << 27) /**< \brief Ebi signal: NCS0 */
#define PIO_PA1B_NCS1        (1u << 1)  /**< \brief Ebi signal: NCS1 */
#define PIO_PA2B_NCS2        (1u << 2)  /**< \brief Ebi signal: NCS2 */
#define PIO_PA3B_NCS3        (1u << 3)  /**< \brief Ebi signal: NCS3 */
#define PIO_PA28C_NRD        (1u << 28) /**< \brief Ebi signal: NRD */
#define PIO_PA29C_NWAIT      (1u << 29) /**< \brief Ebi signal: NWAIT */
#define PIO_PB14C_NWR0       (1u << 14) /**< \brief Ebi signal: NWR0/NWE */
#define PIO_PB14C_NWE        (1u << 14) /**< \brief Ebi signal: NWR0/NWE */
#define PIO_PB15C_NWR1       (1u << 15) /**< \brief Ebi signal: NWR1/NBS1 */
#define PIO_PB15C_NBS1       (1u << 15) /**< \brief Ebi signal: NWR1/NBS1 */
/* ========== Pio definition for PMC peripheral ========== */
#define PIO_PA31A_PCK0       (1u << 31) /**< \brief Pmc signal: PCK0 */
#define PIO_PB13A_PCK0       (1u << 13) /**< \brief Pmc signal: PCK0 */
#define PIO_PA29A_PCK1       (1u << 29) /**< \brief Pmc signal: PCK1 */
#define PIO_PA30A_PCK1       (1u << 30) /**< \brief Pmc signal: PCK1 */
#define PIO_PA0B_PCK2        (1u << 0)  /**< \brief Pmc signal: PCK2 */
#define PIO_PB18B_PCK2       (1u << 18) /**< \brief Pmc signal: PCK2 */
/* ========== Pio definition for PWM peripheral ========== */
#define PIO_PC0B_PWM0        (1u << 0)  /**< \brief Pwm signal: PWM0 */
#define PIO_PC6A_PWM0        (1u << 6)  /**< \brief Pwm signal: PWM0 */
#define PIO_PC1B_PWM1        (1u << 1)  /**< \brief Pwm signal: PWM1 */
#define PIO_PC7A_PWM1        (1u << 7)  /**< \brief Pwm signal: PWM1 */
#define PIO_PC2B_PWM2        (1u << 2)  /**< \brief Pwm signal: PWM2 */
#define PIO_PC8A_PWM2        (1u << 8)  /**< \brief Pwm signal: PWM2 */
#define PIO_PC3B_PWM3        (1u << 3)  /**< \brief Pwm signal: PWM3 */
#define PIO_PC9A_PWM3        (1u << 9)  /**< \brief Pwm signal: PWM3 */
/* ========== Pio definition for SLCDC peripheral ========== */
#define PIO_PA0X1_COM0       (1u << 0)  /**< \brief Slcdc signal: COM0 */
#define PIO_PA1X1_COM1       (1u << 1)  /**< \brief Slcdc signal: COM1 */
#define PIO_PA2X1_COM2       (1u << 2)  /**< \brief Slcdc signal: COM2 */
#define PIO_PA3X1_COM3       (1u << 3)  /**< \brief Slcdc signal: COM3 */
#define PIO_PA4X1_COM4       (1u << 4)  /**< \brief Slcdc signal: COM4/AD1 */
#define PIO_PA4X1_AD1        (1u << 4)  /**< \brief Slcdc signal: COM4/AD1 */
#define PIO_PA5X1_COM5       (1u << 5)  /**< \brief Slcdc signal: COM5/AD2 */
#define PIO_PA5X1_AD2        (1u << 5)  /**< \brief Slcdc signal: COM5/AD2 */
#define PIO_PA6X1_SEG0       (1u << 6)  /**< \brief Slcdc signal: SEG0 */
#define PIO_PA7X1_SEG1       (1u << 7)  /**< \brief Slcdc signal: SEG1 */
#define PIO_PA16X1_SEG10     (1u << 16) /**< \brief Slcdc signal: SEG10 */
#define PIO_PA17X1_SEG11     (1u << 17) /**< \brief Slcdc signal: SEG11 */
#define PIO_PA18X1_SEG12     (1u << 18) /**< \brief Slcdc signal: SEG12 */
#define PIO_PA19X1_SEG13     (1u << 19) /**< \brief Slcdc signal: SEG13 */
#define PIO_PA20X1_SEG14     (1u << 20) /**< \brief Slcdc signal: SEG14 */
#define PIO_PA21X1_SEG15     (1u << 21) /**< \brief Slcdc signal: SEG15 */
#define PIO_PA22X1_SEG16     (1u << 22) /**< \brief Slcdc signal: SEG16 */
#define PIO_PA23X1_SEG17     (1u << 23) /**< \brief Slcdc signal: SEG17 */
#define PIO_PA24X1_SEG18     (1u << 24) /**< \brief Slcdc signal: SEG18 */
#define PIO_PA25X1_SEG19     (1u << 25) /**< \brief Slcdc signal: SEG19 */
#define PIO_PA8X1_SEG2       (1u << 8)  /**< \brief Slcdc signal: SEG2 */
#define PIO_PA26X1_SEG20     (1u << 26) /**< \brief Slcdc signal: SEG20 */
#define PIO_PA27X1_SEG21     (1u << 27) /**< \brief Slcdc signal: SEG21 */
#define PIO_PA28X1_SEG22     (1u << 28) /**< \brief Slcdc signal: SEG22 */
#define PIO_PA29X1_SEG23     (1u << 29) /**< \brief Slcdc signal: SEG23 */
#define PIO_PB6X1_SEG24      (1u << 6)  /**< \brief Slcdc signal: SEG24 */
#define PIO_PB7X1_SEG25      (1u << 7)  /**< \brief Slcdc signal: SEG25 */
#define PIO_PB8X1_SEG26      (1u << 8)  /**< \brief Slcdc signal: SEG26 */
#define PIO_PB9X1_SEG27      (1u << 9)  /**< \brief Slcdc signal: SEG27 */
#define PIO_PB10X1_SEG28     (1u << 10) /**< \brief Slcdc signal: SEG28 */
#define PIO_PB11X1_SEG29     (1u << 11) /**< \brief Slcdc signal: SEG29 */
#define PIO_PA9X1_SEG3       (1u << 9)  /**< \brief Slcdc signal: SEG3 */
#define PIO_PB12X1_SEG30     (1u << 12) /**< \brief Slcdc signal: SEG30 */
#define PIO_PB13X1_SEG31     (1u << 13) /**< \brief Slcdc signal: SEG31/AD3 */
#define PIO_PB13X1_AD3       (1u << 13) /**< \brief Slcdc signal: SEG31/AD3 */
#define PIO_PB14X1_SEG32     (1u << 14) /**< \brief Slcdc signal: SEG32 */
#define PIO_PB15X1_SEG33     (1u << 15) /**< \brief Slcdc signal: SEG33 */
#define PIO_PB16X1_SEG34     (1u << 16) /**< \brief Slcdc signal: SEG34 */
#define PIO_PB17X1_SEG35     (1u << 17) /**< \brief Slcdc signal: SEG35 */
#define PIO_PB18X1_SEG36     (1u << 18) /**< \brief Slcdc signal: SEG36 */
#define PIO_PB19X1_SEG37     (1u << 19) /**< \brief Slcdc signal: SEG37 */
#define PIO_PB20X1_SEG38     (1u << 20) /**< \brief Slcdc signal: SEG38 */
#define PIO_PB21X1_SEG39     (1u << 21) /**< \brief Slcdc signal: SEG39 */
#define PIO_PA10X1_SEG4      (1u << 10) /**< \brief Slcdc signal: SEG4 */
#define PIO_PB22X1_SEG40     (1u << 22) /**< \brief Slcdc signal: SEG40 */
#define PIO_PB23X1_SEG41     (1u << 23) /**< \brief Slcdc signal: SEG41/AD4 */
#define PIO_PB23X1_AD4       (1u << 23) /**< \brief Slcdc signal: SEG41/AD4 */
#define PIO_PB24X1_SEG42     (1u << 24) /**< \brief Slcdc signal: SEG42 */
#define PIO_PB25X1_SEG43     (1u << 25) /**< \brief Slcdc signal: SEG43 */
#define PIO_PB26X1_SEG44     (1u << 26) /**< \brief Slcdc signal: SEG44 */
#define PIO_PB27X1_SEG45     (1u << 27) /**< \brief Slcdc signal: SEG45 */
#define PIO_PB28X1_SEG46     (1u << 28) /**< \brief Slcdc signal: SEG46 */
#define PIO_PB29X1_SEG47     (1u << 29) /**< \brief Slcdc signal: SEG47 */
#define PIO_PB30X1_SEG48     (1u << 30) /**< \brief Slcdc signal: SEG48 */
#define PIO_PB31X1_SEG49     (1u << 31) /**< \brief Slcdc signal: SEG49/AD5 */
#define PIO_PB31X1_AD5       (1u << 31) /**< \brief Slcdc signal: SEG49/AD5 */
#define PIO_PA11X1_SEG5      (1u << 11) /**< \brief Slcdc signal: SEG5 */
#define PIO_PA12X1_SEG6      (1u << 12) /**< \brief Slcdc signal: SEG6/AD0 */
#define PIO_PA12X1_AD0       (1u << 12) /**< \brief Slcdc signal: SEG6/AD0 */
#define PIO_PA13X1_SEG7      (1u << 13) /**< \brief Slcdc signal: SEG7 */
#define PIO_PA14X1_SEG8      (1u << 14) /**< \brief Slcdc signal: SEG8 */
#define PIO_PA15X1_SEG9      (1u << 15) /**< \brief Slcdc signal: SEG9 */
/* ========== Pio definition for SPI0 peripheral ========== */
#define PIO_PA6A_SPI0_MISO   (1u << 6)  /**< \brief Spi0 signal: SPI0_MISO */
#define PIO_PA7A_SPI0_MOSI   (1u << 7)  /**< \brief Spi0 signal: SPI0_MOSI */
#define PIO_PA5A_SPI0_NPCS0  (1u << 5)  /**< \brief Spi0 signal: SPI0_NPCS0 */
#define PIO_PA21A_SPI0_NPCS1 (1u << 21) /**< \brief Spi0 signal: SPI0_NPCS1 */
#define PIO_PA22A_SPI0_NPCS2 (1u << 22) /**< \brief Spi0 signal: SPI0_NPCS2 */
#define PIO_PA23A_SPI0_NPCS3 (1u << 23) /**< \brief Spi0 signal: SPI0_NPCS3 */
#define PIO_PA8A_SPI0_SPCK   (1u << 8)  /**< \brief Spi0 signal: SPI0_SPCK */
/* ========== Pio definition for SPI1 peripheral ========== */
#define PIO_PC3A_SPI1_MISO   (1u << 3)  /**< \brief Spi1 signal: SPI1_MISO */
#define PIO_PC4A_SPI1_MOSI   (1u << 4)  /**< \brief Spi1 signal: SPI1_MOSI */
#define PIO_PC2A_SPI1_NPCS0  (1u << 2)  /**< \brief Spi1 signal: SPI1_NPCS0 */
#define PIO_PC6B_SPI1_NPCS1  (1u << 6)  /**< \brief Spi1 signal: SPI1_NPCS1 */
#define PIO_PC7B_SPI1_NPCS2  (1u << 7)  /**< \brief Spi1 signal: SPI1_NPCS2 */
#define PIO_PC8B_SPI1_NPCS3  (1u << 8)  /**< \brief Spi1 signal: SPI1_NPCS3 */
#define PIO_PC5A_SPI1_SPCK   (1u << 5)  /**< \brief Spi1 signal: SPI1_SPCK */
/* ========== Pio definition for TC0 peripheral ========== */
#define PIO_PB4B_TCLK0       (1u << 4)  /**< \brief Tc0 signal: TCLK0 */
#define PIO_PB9A_TCLK1       (1u << 9)  /**< \brief Tc0 signal: TCLK1 */
#define PIO_PB12A_TCLK2      (1u << 12) /**< \brief Tc0 signal: TCLK2 */
#define PIO_PA13B_TIOA0      (1u << 13) /**< \brief Tc0 signal: TIOA0 */
#define PIO_PB7A_TIOA1       (1u << 7)  /**< \brief Tc0 signal: TIOA1 */
#define PIO_PB10A_TIOA2      (1u << 10) /**< \brief Tc0 signal: TIOA2 */
#define PIO_PA14B_TIOB0      (1u << 14) /**< \brief Tc0 signal: TIOB0 */
#define PIO_PB8A_TIOB1       (1u << 8)  /**< \brief Tc0 signal: TIOB1 */
#define PIO_PB11A_TIOB2      (1u << 11) /**< \brief Tc0 signal: TIOB2 */
/* ========== Pio definition for TC1 peripheral ========== */
#define PIO_PB26A_TCLK3      (1u << 26) /**< \brief Tc1 signal: TCLK3 */
#define PIO_PA17B_TCLK4      (1u << 17) /**< \brief Tc1 signal: TCLK4 */
#define PIO_PA19B_TCLK5      (1u << 19) /**< \brief Tc1 signal: TCLK5 */
#define PIO_PB24A_TIOA3      (1u << 24) /**< \brief Tc1 signal: TIOA3 */
#define PIO_PA15B_TIOA4      (1u << 15) /**< \brief Tc1 signal: TIOA4 */
#define PIO_PA18B_TIOA5      (1u << 18) /**< \brief Tc1 signal: TIOA5 */
#define PIO_PB25A_TIOB3      (1u << 25) /**< \brief Tc1 signal: TIOB3 */
#define PIO_PA16B_TIOB4      (1u << 16) /**< \brief Tc1 signal: TIOB4 */
#define PIO_PA20B_TIOB5      (1u << 20) /**< \brief Tc1 signal: TIOB5 */
/* ========== Pio definition for TWI0 peripheral ========== */
#define PIO_PA25A_TWCK0      (1u << 25) /**< \brief Twi0 signal: TWCK0 */
#define PIO_PA24A_TWD0       (1u << 24) /**< \brief Twi0 signal: TWD0 */
/* ========== Pio definition for TWI1 peripheral ========== */
#define PIO_PB1A_TWCK1       (1u << 1)  /**< \brief Twi1 signal: TWCK1 */
#define PIO_PB0A_TWD1        (1u << 0)  /**< \brief Twi1 signal: TWD1 */
/* ========== Pio definition for UART0 peripheral ========== */
#define PIO_PB4A_URXD0       (1u << 4)  /**< \brief Uart0 signal: URXD0 */
#define PIO_PB5A_UTXD0       (1u << 5)  /**< \brief Uart0 signal: UTXD0 */
/* ========== Pio definition for UART1 peripheral ========== */
#define PIO_PC1A_URXD1       (1u << 1)  /**< \brief Uart1 signal: URXD1 */
#define PIO_PC0A_UTXD1       (1u << 0)  /**< \brief Uart1 signal: UTXD1 */
/* ========== Pio definition for USART0 peripheral ========== */
#define PIO_PA20A_CTS0       (1u << 20) /**< \brief Usart0 signal: CTS0 */
#define PIO_PA19A_RTS0       (1u << 19) /**< \brief Usart0 signal: RTS0 */
#define PIO_PB16A_RXD0       (1u << 16) /**< \brief Usart0 signal: RXD0 */
#define PIO_PB18A_SCK0       (1u << 18) /**< \brief Usart0 signal: SCK0 */
#define PIO_PB17A_TXD0       (1u << 17) /**< \brief Usart0 signal: TXD0 */
/* ========== Pio definition for USART1 peripheral ========== */
#define PIO_PA18A_CTS1       (1u << 18) /**< \brief Usart1 signal: CTS1 */
#define PIO_PA17A_RTS1       (1u << 17) /**< \brief Usart1 signal: RTS1 */
#define PIO_PA11A_RXD1       (1u << 11) /**< \brief Usart1 signal: RXD1 */
#define PIO_PA16A_SCK1       (1u << 16) /**< \brief Usart1 signal: SCK1 */
#define PIO_PA12A_TXD1       (1u << 12) /**< \brief Usart1 signal: TXD1 */
/* ========== Pio definition for USART2 peripheral ========== */
#define PIO_PA15A_CTS2       (1u << 15) /**< \brief Usart2 signal: CTS2 */
#define PIO_PA14A_RTS2       (1u << 14) /**< \brief Usart2 signal: RTS2 */
#define PIO_PA9A_RXD2        (1u << 9)  /**< \brief Usart2 signal: RXD2 */
#define PIO_PA13A_SCK2       (1u << 13) /**< \brief Usart2 signal: SCK2 */
#define PIO_PA10A_TXD2       (1u << 10) /**< \brief Usart2 signal: TXD2 */
/* ========== Pio definition for USART3 peripheral ========== */
#define PIO_PA1A_CTS3        (1u << 1)  /**< \brief Usart3 signal: CTS3 */
#define PIO_PA0A_RTS3        (1u << 0)  /**< \brief Usart3 signal: RTS3 */
#define PIO_PA3A_RXD3        (1u << 3)  /**< \brief Usart3 signal: RXD3 */
#define PIO_PA2A_SCK3        (1u << 2)  /**< \brief Usart3 signal: SCK3 */
#define PIO_PA4A_TXD3        (1u << 4)  /**< \brief Usart3 signal: TXD3 */
/* ========== Pio definition for USART4 peripheral ========== */
#define PIO_PA26A_CTS4       (1u << 26) /**< \brief Usart4 signal: CTS4 */
#define PIO_PB22A_RTS4       (1u << 22) /**< \brief Usart4 signal: RTS4 */
#define PIO_PB19A_RXD4       (1u << 19) /**< \brief Usart4 signal: RXD4 */
#define PIO_PB21A_SCK4       (1u << 21) /**< \brief Usart4 signal: SCK4 */
#define PIO_PB20A_TXD4       (1u << 20) /**< \brief Usart4 signal: TXD4 */
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
#define PIO_PB15_IDX         47
#define PIO_PB16_IDX         48
#define PIO_PB17_IDX         49
#define PIO_PB18_IDX         50
#define PIO_PB19_IDX         51
#define PIO_PB20_IDX         52
#define PIO_PB21_IDX         53
#define PIO_PB22_IDX         54
#define PIO_PB23_IDX         55
#define PIO_PB24_IDX         56
#define PIO_PB25_IDX         57
#define PIO_PB26_IDX         58
#define PIO_PB27_IDX         59
#define PIO_PB28_IDX         60
#define PIO_PB29_IDX         61
#define PIO_PB30_IDX         62
#define PIO_PB31_IDX         63
#define PIO_PC0_IDX          64
#define PIO_PC1_IDX          65
#define PIO_PC2_IDX          66
#define PIO_PC3_IDX          67
#define PIO_PC4_IDX          68
#define PIO_PC5_IDX          69
#define PIO_PC6_IDX          70
#define PIO_PC7_IDX          71
#define PIO_PC8_IDX          72
#define PIO_PC9_IDX          73
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

#endif /* _SAM4C32E_PIO_ */
