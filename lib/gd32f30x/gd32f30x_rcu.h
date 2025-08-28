/*!
    \file    gd32f30x_rcu.h
    \brief   definitions for the RCU

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F30X_RCU_H
#define GD32F30X_RCU_H

#include "gd32f30x.h"

/* RCU definitions */
#define RCU                             RCU_BASE

/* registers definitions */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_CTL                         REG32(RCU + 0x00U)        /*!< control register */
#define RCU_CFG0                        REG32(RCU + 0x04U)        /*!< clock configuration register 0 */
#define RCU_INT                         REG32(RCU + 0x08U)        /*!< clock interrupt register */
#define RCU_APB2RST                     REG32(RCU + 0x0CU)        /*!< APB2 reset register */
#define RCU_APB1RST                     REG32(RCU + 0x10U)        /*!< APB1 reset register */
#define RCU_AHBEN                       REG32(RCU + 0x14U)        /*!< AHB enable register */
#define RCU_APB2EN                      REG32(RCU + 0x18U)        /*!< APB2 enable register */
#define RCU_APB1EN                      REG32(RCU + 0x1CU)        /*!< APB1 enable register */
#define RCU_BDCTL                       REG32(RCU + 0x20U)        /*!< backup domain control register */
#define RCU_RSTSCK                      REG32(RCU + 0x24U)        /*!< reset source / clock register */
#define RCU_CFG1                        REG32(RCU + 0x2CU)        /*!< clock configuration register 1 */
#define RCU_DSV                         REG32(RCU + 0x34U)        /*!< deep-sleep mode voltage register */
#define RCU_ADDCTL                      REG32(RCU + 0xC0U)        /*!< Additional clock control register */
#define RCU_ADDINT                      REG32(RCU + 0xCCU)        /*!< Additional clock interrupt register */
#define RCU_ADDAPB1RST                  REG32(RCU + 0xE0U)        /*!< APB1 additional reset register */
#define RCU_ADDAPB1EN                   REG32(RCU + 0xE4U)        /*!< APB1 additional enable register */
#elif defined(GD32F30X_CL)
#define RCU_CTL                         REG32(RCU + 0x00U)        /*!< control register */
#define RCU_CFG0                        REG32(RCU + 0x04U)        /*!< clock configuration register 0 */
#define RCU_INT                         REG32(RCU + 0x08U)        /*!< clock interrupt register */
#define RCU_APB2RST                     REG32(RCU + 0x0CU)        /*!< APB2 reset register */
#define RCU_APB1RST                     REG32(RCU + 0x10U)        /*!< APB1 reset register */
#define RCU_AHBEN                       REG32(RCU + 0x14U)        /*!< AHB1 enable register */
#define RCU_APB2EN                      REG32(RCU + 0x18U)        /*!< APB2 enable register */
#define RCU_APB1EN                      REG32(RCU + 0x1CU)        /*!< APB1 enable register */
#define RCU_BDCTL                       REG32(RCU + 0x20U)        /*!< backup domain control register */
#define RCU_RSTSCK                      REG32(RCU + 0x24U)        /*!< reset source / clock register */
#define RCU_AHBRST                      REG32(RCU + 0x28U)        /*!< AHB reset register */
#define RCU_CFG1                        REG32(RCU + 0x2CU)        /*!< clock configuration register 1 */
#define RCU_DSV                         REG32(RCU + 0x34U)        /*!< deep-sleep mode voltage register */
#define RCU_ADDCTL                      REG32(RCU + 0xC0U)        /*!< Additional clock control register */
#define RCU_ADDINT                      REG32(RCU + 0xCCU)        /*!< Additional clock interrupt register */
#define RCU_ADDAPB1RST                  REG32(RCU + 0xE0U)        /*!< APB1 additional reset register */
#define RCU_ADDAPB1EN                   REG32(RCU + 0xE4U)        /*!< APB1 additional enable register */
#endif /* GD32F30X_HD and GD32F30X_XD */

/* bits definitions */
/* RCU_CTL */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_CTL_IRC8MEN                 BIT(0)                    /*!< internal high speed oscillator enable */
#define RCU_CTL_IRC8MSTB                BIT(1)                    /*!< IRC8M high speed internal oscillator stabilization flag */
#define RCU_CTL_IRC8MADJ                BITS(3,7)                 /*!< high speed internal oscillator clock trim adjust value */
#define RCU_CTL_IRC8MCALIB              BITS(8,15)                /*!< high speed internal oscillator calibration value register */
#define RCU_CTL_HXTALEN                 BIT(16)                   /*!< external high speed oscillator enable */
#define RCU_CTL_HXTALSTB                BIT(17)                   /*!< external crystal oscillator clock stabilization flag */
#define RCU_CTL_HXTALBPS                BIT(18)                   /*!< external crystal oscillator clock bypass mode enable */
#define RCU_CTL_CKMEN                   BIT(19)                   /*!< HXTAL clock monitor enable */
#define RCU_CTL_PLLEN                   BIT(24)                   /*!< PLL enable */
#define RCU_CTL_PLLSTB                  BIT(25)                   /*!< PLL clock stabilization flag */
#elif defined(GD32F30X_CL)
#define RCU_CTL_IRC8MEN                 BIT(0)                    /*!< internal high speed oscillator enable */
#define RCU_CTL_IRC8MSTB                BIT(1)                    /*!< IRC8M high speed internal oscillator stabilization flag */
#define RCU_CTL_IRC8MADJ                BITS(3,7)                 /*!< high speed internal oscillator clock trim adjust value */
#define RCU_CTL_IRC8MCALIB              BITS(8,15)                /*!< high speed internal oscillator calibration value register */
#define RCU_CTL_HXTALEN                 BIT(16)                   /*!< external high speed oscillator enable */
#define RCU_CTL_HXTALSTB                BIT(17)                   /*!< external crystal oscillator clock stabilization flag */
#define RCU_CTL_HXTALBPS                BIT(18)                   /*!< external crystal oscillator clock bypass mode enable */
#define RCU_CTL_CKMEN                   BIT(19)                   /*!< HXTAL clock monitor enable */
#define RCU_CTL_PLLEN                   BIT(24)                   /*!< PLL enable */
#define RCU_CTL_PLLSTB                  BIT(25)                   /*!< PLL clock stabilization flag */
#define RCU_CTL_PLL1EN                  BIT(26)                   /*!< PLL1 enable */
#define RCU_CTL_PLL1STB                 BIT(27)                   /*!< PLL1 clock stabilization flag */
#define RCU_CTL_PLL2EN                  BIT(28)                   /*!< PLL2 enable */
#define RCU_CTL_PLL2STB                 BIT(29)                   /*!< PLL2 clock stabilization flag */
#endif /* GD32F30X_HD and GD32F30X_XD */

/* RCU_CFG0 */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_CFG0_SCS                    BITS(0,1)                 /*!< system clock switch */
#define RCU_CFG0_SCSS                   BITS(2,3)                 /*!< system clock switch status */
#define RCU_CFG0_AHBPSC                 BITS(4,7)                 /*!< AHB prescaler selection */
#define RCU_CFG0_APB1PSC                BITS(8,10)                /*!< APB1 prescaler selection */
#define RCU_CFG0_APB2PSC                BITS(11,13)               /*!< APB2 prescaler selection */
#define RCU_CFG0_ADCPSC                 BITS(14,15)               /*!< ADC prescaler selection */
#define RCU_CFG0_PLLSEL                 BIT(16)                   /*!< PLL clock source selection */
#define RCU_CFG0_PREDV0                 BIT(17)                   /*!< PREDV0 division factor */
#define RCU_CFG0_PLLMF                  BITS(18,21)               /*!< PLL clock multiplication factor */
#define RCU_CFG0_USBDPSC                BITS(22,23)               /*!< USBD clock prescaler selection */
#define RCU_CFG0_CKOUT0SEL              BITS(24,26)               /*!< CKOUT0 clock source selection */
#define RCU_CFG0_PLLMF_4                BIT(27)                   /*!< bit 4 of PLLMF */
#define RCU_CFG0_ADCPSC_2               BIT(28)                   /*!< bit 2 of ADCPSC */
#define RCU_CFG0_PLLMF_5                BIT(30)                   /*!< bit 5 of PLLMF */
#define RCU_CFG0_USBDPSC_2              BIT(31)                   /*!< bit 2 of USBDPSC */
#elif defined(GD32F30X_CL)
#define RCU_CFG0_SCS                    BITS(0,1)                 /*!< system clock switch */
#define RCU_CFG0_SCSS                   BITS(2,3)                 /*!< system clock switch status */
#define RCU_CFG0_AHBPSC                 BITS(4,7)                 /*!< AHB prescaler selection */
#define RCU_CFG0_APB1PSC                BITS(8,10)                /*!< APB1 prescaler selection */
#define RCU_CFG0_APB2PSC                BITS(11,13)               /*!< APB2 prescaler selection */
#define RCU_CFG0_ADCPSC                 BITS(14,15)               /*!< ADC prescaler selection */
#define RCU_CFG0_PLLSEL                 BIT(16)                   /*!< PLL clock source selection */
#define RCU_CFG0_PREDV0_LSB             BIT(17)                   /*!< the LSB of PREDV0 division factor */
#define RCU_CFG0_PLLMF                  BITS(18,21)               /*!< PLL clock multiplication factor */
#define RCU_CFG0_USBFSPSC               BITS(22,23)               /*!< USBFS clock prescaler selection */
#define RCU_CFG0_CKOUT0SEL              BITS(24,27)               /*!< CKOUT0 clock source selection */
#define RCU_CFG0_ADCPSC_2               BIT(28)                   /*!< bit 2 of ADCPSC */
#define RCU_CFG0_PLLMF_4                BIT(29)                   /*!< bit 4 of PLLMF */
#define RCU_CFG0_PLLMF_5                BIT(30)                   /*!< bit 5 of PLLMF */
#define RCU_CFG0_USBFSPSC_2             BIT(31)                   /*!< bit 2 of USBFSPSC */
#endif /* GD32F30X_HD and GD32F30X_XD */

/* RCU_INT */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_INT_IRC40KSTBIF             BIT(0)                    /*!< IRC40K stabilization interrupt flag */
#define RCU_INT_LXTALSTBIF              BIT(1)                    /*!< LXTAL stabilization interrupt flag */
#define RCU_INT_IRC8MSTBIF              BIT(2)                    /*!< IRC8M stabilization interrupt flag */
#define RCU_INT_HXTALSTBIF              BIT(3)                    /*!< HXTAL stabilization interrupt flag */
#define RCU_INT_PLLSTBIF                BIT(4)                    /*!< PLL stabilization interrupt flag */
#define RCU_INT_CKMIF                   BIT(7)                    /*!< HXTAL clock stuck interrupt flag */
#define RCU_INT_IRC40KSTBIE             BIT(8)                    /*!< IRC40K stabilization interrupt enable */
#define RCU_INT_LXTALSTBIE              BIT(9)                    /*!< LXTAL stabilization interrupt enable */
#define RCU_INT_IRC8MSTBIE              BIT(10)                   /*!< IRC8M stabilization interrupt enable */
#define RCU_INT_HXTALSTBIE              BIT(11)                   /*!< HXTAL stabilization interrupt enable */
#define RCU_INT_PLLSTBIE                BIT(12)                   /*!< PLL stabilization interrupt enable */
#define RCU_INT_IRC40KSTBIC             BIT(16)                   /*!< IRC40K Stabilization interrupt clear */
#define RCU_INT_LXTALSTBIC              BIT(17)                   /*!< LXTAL Stabilization interrupt clear */
#define RCU_INT_IRC8MSTBIC              BIT(18)                   /*!< IRC8M Stabilization interrupt clear */
#define RCU_INT_HXTALSTBIC              BIT(19)                   /*!< HXTAL Stabilization interrupt clear */
#define RCU_INT_PLLSTBIC                BIT(20)                   /*!< PLL stabilization interrupt clear */
#define RCU_INT_CKMIC                   BIT(23)                   /*!< HXTAL clock stuck interrupt clear */
#elif defined(GD32F30X_CL)
#define RCU_INT_IRC40KSTBIF             BIT(0)                    /*!< IRC40K stabilization interrupt flag */
#define RCU_INT_LXTALSTBIF              BIT(1)                    /*!< LXTAL stabilization interrupt flag */
#define RCU_INT_IRC8MSTBIF              BIT(2)                    /*!< IRC8M stabilization interrupt flag */
#define RCU_INT_HXTALSTBIF              BIT(3)                    /*!< HXTAL stabilization interrupt flag */
#define RCU_INT_PLLSTBIF                BIT(4)                    /*!< PLL stabilization interrupt flag */
#define RCU_INT_PLL1STBIF               BIT(5)                    /*!< PLL1 stabilization interrupt flag */
#define RCU_INT_PLL2STBIF               BIT(6)                    /*!< PLL2 stabilization interrupt flag */
#define RCU_INT_CKMIF                   BIT(7)                    /*!< HXTAL clock stuck interrupt flag */
#define RCU_INT_IRC40KSTBIE             BIT(8)                    /*!< IRC40K stabilization interrupt enable */
#define RCU_INT_LXTALSTBIE              BIT(9)                    /*!< LXTAL stabilization interrupt enable */
#define RCU_INT_IRC8MSTBIE              BIT(10)                   /*!< IRC8M stabilization interrupt enable */
#define RCU_INT_HXTALSTBIE              BIT(11)                   /*!< HXTAL stabilization interrupt enable */
#define RCU_INT_PLLSTBIE                BIT(12)                   /*!< PLL stabilization interrupt enable */
#define RCU_INT_PLL1STBIE               BIT(13)                   /*!< PLL1 stabilization interrupt enable */
#define RCU_INT_PLL2STBIE               BIT(14)                   /*!< PLL2 stabilization interrupt enable */
#define RCU_INT_IRC40KSTBIC             BIT(16)                   /*!< IRC40K stabilization interrupt clear */
#define RCU_INT_LXTALSTBIC              BIT(17)                   /*!< LXTAL stabilization interrupt clear */
#define RCU_INT_IRC8MSTBIC              BIT(18)                   /*!< IRC8M stabilization interrupt clear */
#define RCU_INT_HXTALSTBIC              BIT(19)                   /*!< HXTAL stabilization interrupt clear */
#define RCU_INT_PLLSTBIC                BIT(20)                   /*!< PLL stabilization interrupt clear */
#define RCU_INT_PLL1STBIC               BIT(21)                   /*!< PLL1 stabilization interrupt clear */
#define RCU_INT_PLL2STBIC               BIT(22)                   /*!< PLL2 stabilization interrupt clear */
#define RCU_INT_CKMIC                   BIT(23)                   /*!< HXTAL clock stuck interrupt clear */
#endif /* GD32F30X_HD and GD32F30X_XD */

/* RCU_APB2RST */
#define RCU_APB2RST_AFRST               BIT(0)                    /*!< alternate function I/O reset */
#define RCU_APB2RST_PARST               BIT(2)                    /*!< GPIO port A reset */
#define RCU_APB2RST_PBRST               BIT(3)                    /*!< GPIO port B reset */
#define RCU_APB2RST_PCRST               BIT(4)                    /*!< GPIO port C reset */
#define RCU_APB2RST_PDRST               BIT(5)                    /*!< GPIO port D reset */
#define RCU_APB2RST_PERST               BIT(6)                    /*!< GPIO port E reset */
#define RCU_APB2RST_PFRST               BIT(7)                    /*!< GPIO port F reset */
#define RCU_APB2RST_PGRST               BIT(8)                    /*!< GPIO port G reset */
#define RCU_APB2RST_ADC0RST             BIT(9)                    /*!< ADC0 reset */
#define RCU_APB2RST_ADC1RST             BIT(10)                   /*!< ADC1 reset */
#define RCU_APB2RST_TIMER0RST           BIT(11)                   /*!< TIMER0 reset */
#define RCU_APB2RST_SPI0RST             BIT(12)                   /*!< SPI0 reset */
#define RCU_APB2RST_TIMER7RST           BIT(13)                   /*!< TIMER7 reset */
#define RCU_APB2RST_USART0RST           BIT(14)                   /*!< USART0 reset */
#ifndef GD32F30X_CL
#define RCU_APB2RST_ADC2RST             BIT(15)                   /*!< ADC2 reset */
#endif /* GD32F30X_CL */
#ifndef GD32F30X_HD
#define RCU_APB2RST_TIMER8RST           BIT(19)                   /*!< TIMER8 reset */
#define RCU_APB2RST_TIMER9RST           BIT(20)                   /*!< TIMER9 reset */
#define RCU_APB2RST_TIMER10RST          BIT(21)                   /*!< TIMER10 reset */
#endif /* GD32F30X_HD */

/* RCU_APB1RST */
#define RCU_APB1RST_TIMER1RST           BIT(0)                    /*!< TIMER1 reset */
#define RCU_APB1RST_TIMER2RST           BIT(1)                    /*!< TIMER2 reset */
#define RCU_APB1RST_TIMER3RST           BIT(2)                    /*!< TIMER3 reset */
#define RCU_APB1RST_TIMER4RST           BIT(3)                    /*!< TIMER4 reset */
#define RCU_APB1RST_TIMER5RST           BIT(4)                    /*!< TIMER5 reset */
#define RCU_APB1RST_TIMER6RST           BIT(5)                    /*!< TIMER6 reset */
#ifndef GD32F30X_HD
#define RCU_APB1RST_TIMER11RST          BIT(6)                    /*!< TIMER11 reset */
#define RCU_APB1RST_TIMER12RST          BIT(7)                    /*!< TIMER12 reset */
#define RCU_APB1RST_TIMER13RST          BIT(8)                    /*!< TIMER13 reset */
#endif /* GD32F30X_HD */
#define RCU_APB1RST_WWDGTRST            BIT(11)                   /*!< WWDGT reset */
#define RCU_APB1RST_SPI1RST             BIT(14)                   /*!< SPI1 reset */
#define RCU_APB1RST_SPI2RST             BIT(15)                   /*!< SPI2 reset */
#define RCU_APB1RST_USART1RST           BIT(17)                   /*!< USART1 reset */
#define RCU_APB1RST_USART2RST           BIT(18)                   /*!< USART2 reset */
#define RCU_APB1RST_UART3RST            BIT(19)                   /*!< UART3 reset */
#define RCU_APB1RST_UART4RST            BIT(20)                   /*!< UART4 reset */
#define RCU_APB1RST_I2C0RST             BIT(21)                   /*!< I2C0 reset */
#define RCU_APB1RST_I2C1RST             BIT(22)                   /*!< I2C1 reset */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_APB1RST_USBDRST             BIT(23)                   /*!< USBD reset */
#endif /* GD32F30X_HD and GD32F30X_XD */
#define RCU_APB1RST_CAN0RST             BIT(25)                   /*!< CAN0 reset */
#ifdef GD32F30X_CL
#define RCU_APB1RST_CAN1RST             BIT(26)                   /*!< CAN1 reset */
#endif /* GD32F30X_CL */
#define RCU_APB1RST_BKPIRST             BIT(27)                   /*!< backup interface reset */
#define RCU_APB1RST_PMURST              BIT(28)                   /*!< PMU reset */
#define RCU_APB1RST_DACRST              BIT(29)                   /*!< DAC reset */

/* RCU_AHBEN */
#define RCU_AHBEN_DMA0EN                BIT(0)                    /*!< DMA0 clock enable */
#define RCU_AHBEN_DMA1EN                BIT(1)                    /*!< DMA1 clock enable */
#define RCU_AHBEN_SRAMSPEN              BIT(2)                    /*!< SRAM clock enable when sleep mode */
#define RCU_AHBEN_FMCSPEN               BIT(4)                    /*!< FMC clock enable when sleep mode */
#define RCU_AHBEN_CRCEN                 BIT(6)                    /*!< CRC clock enable */
#define RCU_AHBEN_EXMCEN                BIT(8)                    /*!< EXMC clock enable */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_AHBEN_SDIOEN                BIT(10)                   /*!< SDIO clock enable */
#elif defined(GD32F30X_CL)
#define RCU_AHBEN_USBFSEN               BIT(12)                   /*!< USBFS clock enable */
#define RCU_AHBEN_ENETEN                BIT(14)                   /*!< ENET clock enable */
#define RCU_AHBEN_ENETTXEN              BIT(15)                   /*!< Ethernet TX clock enable */
#define RCU_AHBEN_ENETRXEN              BIT(16)                   /*!< Ethernet RX clock enable */
#endif /* GD32F30X_HD and GD32F30X_XD */

/* RCU_APB2EN */
#define RCU_APB2EN_AFEN                 BIT(0)                    /*!< alternate function IO clock enable */
#define RCU_APB2EN_PAEN                 BIT(2)                    /*!< GPIO port A clock enable */
#define RCU_APB2EN_PBEN                 BIT(3)                    /*!< GPIO port B clock enable */
#define RCU_APB2EN_PCEN                 BIT(4)                    /*!< GPIO port C clock enable */
#define RCU_APB2EN_PDEN                 BIT(5)                    /*!< GPIO port D clock enable */
#define RCU_APB2EN_PEEN                 BIT(6)                    /*!< GPIO port E clock enable */
#define RCU_APB2EN_PFEN                 BIT(7)                    /*!< GPIO port F clock enable */
#define RCU_APB2EN_PGEN                 BIT(8)                    /*!< GPIO port G clock enable */
#define RCU_APB2EN_ADC0EN               BIT(9)                    /*!< ADC0 clock enable */
#define RCU_APB2EN_ADC1EN               BIT(10)                   /*!< ADC1 clock enable */
#define RCU_APB2EN_TIMER0EN             BIT(11)                   /*!< TIMER0 clock enable */
#define RCU_APB2EN_SPI0EN               BIT(12)                   /*!< SPI0 clock enable */
#define RCU_APB2EN_TIMER7EN             BIT(13)                   /*!< TIMER7 clock enable */
#define RCU_APB2EN_USART0EN             BIT(14)                   /*!< USART0 clock enable */
#ifndef GD32F30X_CL
#define RCU_APB2EN_ADC2EN               BIT(15)                   /*!< ADC2 clock enable */
#endif /* GD32F30X_CL */
#ifndef GD32F30X_HD
#define RCU_APB2EN_TIMER8EN             BIT(19)                   /*!< TIMER8 clock enable */
#define RCU_APB2EN_TIMER9EN             BIT(20)                   /*!< TIMER9 clock enable */
#define RCU_APB2EN_TIMER10EN            BIT(21)                   /*!< TIMER10 clock enable */
#endif /* GD32F30X_HD */

/* RCU_APB1EN */
#define RCU_APB1EN_TIMER1EN             BIT(0)                    /*!< TIMER1 clock enable */
#define RCU_APB1EN_TIMER2EN             BIT(1)                    /*!< TIMER2 clock enable */
#define RCU_APB1EN_TIMER3EN             BIT(2)                    /*!< TIMER3 clock enable */
#define RCU_APB1EN_TIMER4EN             BIT(3)                    /*!< TIMER4 clock enable */
#define RCU_APB1EN_TIMER5EN             BIT(4)                    /*!< TIMER5 clock enable */
#define RCU_APB1EN_TIMER6EN             BIT(5)                    /*!< TIMER6 clock enable */
#ifndef GD32F30X_HD
#define RCU_APB1EN_TIMER11EN            BIT(6)                    /*!< TIMER11 clock enable */
#define RCU_APB1EN_TIMER12EN            BIT(7)                    /*!< TIMER12 clock enable */
#define RCU_APB1EN_TIMER13EN            BIT(8)                    /*!< TIMER13 clock enable */
#endif /* GD32F30X_HD */
#define RCU_APB1EN_WWDGTEN              BIT(11)                   /*!< WWDGT clock enable */
#define RCU_APB1EN_SPI1EN               BIT(14)                   /*!< SPI1 clock enable */
#define RCU_APB1EN_SPI2EN               BIT(15)                   /*!< SPI2 clock enable */
#define RCU_APB1EN_USART1EN             BIT(17)                   /*!< USART1 clock enable */
#define RCU_APB1EN_USART2EN             BIT(18)                   /*!< USART2 clock enable */
#define RCU_APB1EN_UART3EN              BIT(19)                   /*!< UART3 clock enable */
#define RCU_APB1EN_UART4EN              BIT(20)                   /*!< UART4 clock enable */
#define RCU_APB1EN_I2C0EN               BIT(21)                   /*!< I2C0 clock enable */
#define RCU_APB1EN_I2C1EN               BIT(22)                   /*!< I2C1 clock enable */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_APB1EN_USBDEN               BIT(23)                   /*!< USBD clock enable */
#endif /* GD32F30X_HD and GD32F30X_XD */
#define RCU_APB1EN_CAN0EN               BIT(25)                   /*!< CAN0 clock enable */
#ifdef GD32F30X_CL
#define RCU_APB1EN_CAN1EN               BIT(26)                   /*!< CAN1 clock enable */
#endif /* GD32F30X_CL */
#define RCU_APB1EN_BKPIEN               BIT(27)                   /*!< backup interface clock enable */
#define RCU_APB1EN_PMUEN                BIT(28)                   /*!< PMU clock enable */
#define RCU_APB1EN_DACEN                BIT(29)                   /*!< DAC clock enable */

/* RCU_BDCTL */
#define RCU_BDCTL_LXTALEN               BIT(0)                    /*!< LXTAL enable */
#define RCU_BDCTL_LXTALSTB              BIT(1)                    /*!< low speed crystal oscillator stabilization flag */
#define RCU_BDCTL_LXTALBPS              BIT(2)                    /*!< LXTAL bypass mode enable */
#define RCU_BDCTL_LXTALDRI              BITS(3,4)                 /*!< LXTAL drive capability */
#define RCU_BDCTL_RTCSRC                BITS(8,9)                 /*!< RTC clock entry selection */
#define RCU_BDCTL_RTCEN                 BIT(15)                   /*!< RTC clock enable */
#define RCU_BDCTL_BKPRST                BIT(16)                   /*!< backup domain reset */

/* RCU_RSTSCK */
#define RCU_RSTSCK_IRC40KEN             BIT(0)                    /*!< IRC40K enable */
#define RCU_RSTSCK_IRC40KSTB            BIT(1)                    /*!< IRC40K stabilization flag */
#define RCU_RSTSCK_RSTFC                BIT(24)                   /*!< reset flag clear */
#define RCU_RSTSCK_EPRSTF               BIT(26)                   /*!< external pin reset flag */
#define RCU_RSTSCK_PORRSTF              BIT(27)                   /*!< power reset flag */
#define RCU_RSTSCK_SWRSTF               BIT(28)                   /*!< software reset flag */
#define RCU_RSTSCK_FWDGTRSTF            BIT(29)                   /*!< free watchdog timer reset flag */
#define RCU_RSTSCK_WWDGTRSTF            BIT(30)                   /*!< window watchdog timer reset flag */
#define RCU_RSTSCK_LPRSTF               BIT(31)                   /*!< low-power reset flag */

#ifdef GD32F30X_CL
/* RCU_AHBRST */
#define RCU_AHBRST_USBFSRST             BIT(12)                   /*!< USBFS reset */
#define RCU_AHBRST_ENETRST              BIT(14)                   /*!< ENET reset */
#endif /* GD32F30X_CL */

/* RCU_CFG1 */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_CFG1_ADCPSC_3               BIT(29)                   /*!< bit 4 of ADCPSC */
#define RCU_CFG1_PLLPRESEL              BIT(30)                   /*!< PLL clock source selection */
#elif defined(GD32F30X_CL)
#define RCU_CFG1_PREDV0                 BITS(0,3)                 /*!< PREDV0 division factor */
#define RCU_CFG1_PREDV1                 BITS(4,7)                 /*!< PREDV1 division factor */
#define RCU_CFG1_PLL1MF                 BITS(8,11)                /*!< PLL1 clock multiplication factor */
#define RCU_CFG1_PLL2MF                 BITS(12,15)               /*!< PLL2 clock multiplication factor */
#define RCU_CFG1_PREDV0SEL              BIT(16)                   /*!< PREDV0 input clock source selection */
#define RCU_CFG1_I2S1SEL                BIT(17)                   /*!< I2S1 clock source selection */
#define RCU_CFG1_I2S2SEL                BIT(18)                   /*!< I2S2 clock source selection  */
#define RCU_CFG1_ADCPSC_3               BIT(29)                   /*!< bit 4 of ADCPSC */
#define RCU_CFG1_PLLPRESEL              BIT(30)                   /*!< PLL clock source selection */
#define RCU_CFG1_PLL2MF_4               BIT(31)                   /*!< bit 5 of PLL2MF */
#endif /* GD32F30X_HD and GD32F30X_XD */

/* RCU_DSV */
#define RCU_DSV_DSLPVS                  BITS(0,2)                 /*!< deep-sleep mode voltage select */

/* RCU_ADDCTL */
#define RCU_ADDCTL_CK48MSEL             BIT(0)                    /*!< 48MHz clock selection */
#define RCU_ADDCTL_IRC48MEN             BIT(16)                   /*!< internal 48MHz RC oscillator enable */
#define RCU_ADDCTL_IRC48MSTB            BIT(17)                   /*!< internal 48MHz RC oscillator clock stabilization flag */
#define RCU_ADDCTL_IRC48MCAL            BITS(24,31)               /*!< internal 48MHz RC oscillator calibration value register */

/* RCU_ADDINT */
#define RCU_ADDINT_IRC48MSTBIF          BIT(6)                    /*!< IRC48M stabilization interrupt flag */
#define RCU_ADDINT_IRC48MSTBIE          BIT(14)                   /*!< internal 48 MHz RC oscillator stabilization interrupt enable */
#define RCU_ADDINT_IRC48MSTBIC          BIT(22)                   /*!< internal 48 MHz RC oscillator stabilization interrupt clear */

/* RCU_ADDAPB1RST */
#define RCU_ADDAPB1RST_CTCRST           BIT(27)                   /*!< CTC reset */

/* RCU_ADDAPB1EN */
#define RCU_ADDAPB1EN_CTCEN             BIT(27)                   /*!< CTC clock enable */


/* constants definitions */
/* define the peripheral clock enable bit position and its register index offset */
#define RCU_REGIDX_BIT(regidx, bitpos)      (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos))
#define RCU_REG_VAL(periph)                 (REG32(RCU + ((uint32_t)(periph) >> 6)))
#define RCU_BIT_POS(val)                    ((uint32_t)(val) & 0x1FU)

/* register offset */
/* peripherals enable */
#define AHBEN_REG_OFFSET                0x14U                     /*!< AHB enable register offset */
#define APB1EN_REG_OFFSET               0x1CU                     /*!< APB1 enable register offset */
#define APB2EN_REG_OFFSET               0x18U                     /*!< APB2 enable register offset */
#define ADD_APB1EN_REG_OFFSET           0xE4U                     /*!< APB1 additional enable register offset */

/* peripherals reset */
#define AHBRST_REG_OFFSET               0x28U                     /*!< AHB reset register offset */
#define APB1RST_REG_OFFSET              0x10U                     /*!< APB1 reset register offset */
#define APB2RST_REG_OFFSET              0x0CU                     /*!< APB2 reset register offset */
#define ADD_APB1RST_REG_OFFSET          0xE0U                     /*!< APB1 additional reset register offset */
#define RSTSCK_REG_OFFSET               0x24U                     /*!< reset source/clock register offset */

/* clock control */
#define CTL_REG_OFFSET                  0x00U                     /*!< control register offset */
#define BDCTL_REG_OFFSET                0x20U                     /*!< backup domain control register offset */
#define ADDCTL_REG_OFFSET               0xC0U                     /*!< additional clock control register offset */

/* clock stabilization and stuck interrupt */
#define INT_REG_OFFSET                  0x08U                     /*!< clock interrupt register offset */
#define ADDINT_REG_OFFSET               0xCCU                     /*!< additional clock interrupt register offset */

/* configuration register */
#define CFG0_REG_OFFSET                 0x04U                     /*!< clock configuration register 0 offset */
#define CFG1_REG_OFFSET                 0x2CU                     /*!< clock configuration register 1 offset */

/* peripheral clock enable */
typedef enum
{
    /* AHB peripherals */
    RCU_DMA0      = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 0U),                   /*!< DMA0 clock */
    RCU_DMA1      = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 1U),                   /*!< DMA1 clock */
    RCU_CRC       = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 6U),                   /*!< CRC clock */
    RCU_EXMC      = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 8U),                   /*!< EXMC clock */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
    RCU_SDIO      = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 10U),                  /*!< SDIO clock */
#elif defined(GD32F30X_CL)
    RCU_USBFS     = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 12U),                  /*!< USBFS clock */
    RCU_ENET      = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 14U),                  /*!< ENET clock */
    RCU_ENETTX    = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 15U),                  /*!< ENETTX clock */
    RCU_ENETRX    = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 16U),                  /*!< ENETRX clock */
#endif /* GD32F30X_HD and GD32F30X_XD */
    
    /* APB1 peripherals */
    RCU_TIMER1    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 0U),                  /*!< TIMER1 clock */
    RCU_TIMER2    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 1U),                  /*!< TIMER2 clock */
    RCU_TIMER3    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 2U),                  /*!< TIMER3 clock */
    RCU_TIMER4    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 3U),                  /*!< TIMER4 clock */
    RCU_TIMER5    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 4U),                  /*!< TIMER5 clock */
    RCU_TIMER6    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 5U),                  /*!< TIMER6 clock */
#ifndef GD32F30X_HD
    RCU_TIMER11   = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 6U),                  /*!< TIMER11 clock */
    RCU_TIMER12   = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 7U),                  /*!< TIMER12 clock */
    RCU_TIMER13   = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 8U),                  /*!< TIMER13 clock */
#endif /* GD32F30X_HD */
    RCU_WWDGT     = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 11U),                 /*!< WWDGT clock */
    RCU_SPI1      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 14U),                 /*!< SPI1 clock */
    RCU_SPI2      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 15U),                 /*!< SPI2 clock */
    RCU_USART1    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 17U),                 /*!< USART1 clock */
    RCU_USART2    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 18U),                 /*!< USART2 clock */
    RCU_UART3     = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 19U),                 /*!< UART3 clock */
    RCU_UART4     = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 20U),                 /*!< UART4 clock */
    RCU_I2C0      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 21U),                 /*!< I2C0 clock */
    RCU_I2C1      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 22U),                 /*!< I2C1 clock */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
    RCU_USBD      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 23U),                 /*!< USBD clock */
#endif /* GD32F30X_HD and GD32F30X_XD */
    RCU_CAN0      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 25U),                 /*!< CAN0 clock */
#ifdef GD32F30X_CL
    RCU_CAN1      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 26U),                 /*!< CAN1 clock */
#endif /* GD32F30X_CL */
    RCU_BKPI      = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 27U),                 /*!< BKPI clock */
    RCU_PMU       = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 28U),                 /*!< PMU clock */
    RCU_DAC       = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 29U),                 /*!< DAC clock */
    RCU_RTC       = RCU_REGIDX_BIT(BDCTL_REG_OFFSET, 15U),                  /*!< RTC clock */
    RCU_CTC       = RCU_REGIDX_BIT(ADD_APB1EN_REG_OFFSET, 27U),             /*!< CTC clock */
    
    /* APB2 peripherals */
    RCU_AF        = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 0U),                  /*!< alternate function clock */
    RCU_GPIOA     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 2U),                  /*!< GPIOA clock */
    RCU_GPIOB     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 3U),                  /*!< GPIOB clock */
    RCU_GPIOC     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 4U),                  /*!< GPIOC clock */
    RCU_GPIOD     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 5U),                  /*!< GPIOD clock */
    RCU_GPIOE     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 6U),                  /*!< GPIOE clock */
    RCU_GPIOF     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 7U),                  /*!< GPIOF clock */
    RCU_GPIOG     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 8U),                  /*!< GPIOG clock */
    RCU_ADC0      = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 9U),                  /*!< ADC0 clock */
    RCU_ADC1      = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 10U),                 /*!< ADC1 clock */
    RCU_TIMER0    = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 11U),                 /*!< TIMER0 clock */
    RCU_SPI0      = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 12U),                 /*!< SPI0 clock */
    RCU_TIMER7    = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 13U),                 /*!< TIMER7 clock */
    RCU_USART0    = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 14U),                 /*!< USART0 clock */
#ifndef GD32F30X_CL
    RCU_ADC2      = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 15U),                 /*!< ADC2 clock */
#endif /* GD32F30X_CL */
#ifndef GD32F30X_HD
    RCU_TIMER8    = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 19U),                 /*!< TIMER8 clock */
    RCU_TIMER9    = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 20U),                 /*!< TIMER9 clock */
    RCU_TIMER10   = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 21U),                 /*!< TIMER10 clock */
#endif /* GD32F30X_HD */
}rcu_periph_enum;

/* peripheral clock enable when sleep mode*/
typedef enum
{
    /* AHB peripherals */
    RCU_SRAM_SLP     = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 2U),                /*!< SRAM clock */
    RCU_FMC_SLP      = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 4U),                /*!< FMC clock */
}rcu_periph_sleep_enum;

/* peripherals reset */
typedef enum
{
    /* AHB peripherals */
#ifdef GD32F30X_CL
    RCU_USBFSRST     = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 12U),              /*!< USBFS clock reset */
    RCU_ENETRST      = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 14U),              /*!< ENET clock reset */
#endif /* GD32F30X_CL */
    
    /* APB1 peripherals */
    RCU_TIMER1RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 0U),              /*!< TIMER1 clock reset */
    RCU_TIMER2RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 1U),              /*!< TIMER2 clock reset */
    RCU_TIMER3RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 2U),              /*!< TIMER3 clock reset */
    RCU_TIMER4RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 3U),              /*!< TIMER4 clock reset */
    RCU_TIMER5RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 4U),              /*!< TIMER5 clock reset */
    RCU_TIMER6RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 5U),              /*!< TIMER6 clock reset */
#ifndef GD32F30X_HD
    RCU_TIMER11RST   = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 6U),              /*!< TIMER11 clock reset */
    RCU_TIMER12RST   = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 7U),              /*!< TIMER12 clock reset */
    RCU_TIMER13RST   = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 8U),              /*!< TIMER13 clock reset */
#endif /* GD32F30X_HD */
    RCU_WWDGTRST     = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 11U),             /*!< WWDGT clock reset */
    RCU_SPI1RST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 14U),             /*!< SPI1 clock reset */
    RCU_SPI2RST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 15U),             /*!< SPI2 clock reset */
    RCU_USART1RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 17U),             /*!< USART1 clock reset */
    RCU_USART2RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 18U),             /*!< USART2 clock reset */
    RCU_UART3RST     = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 19U),             /*!< UART3 clock reset */
    RCU_UART4RST     = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 20U),             /*!< UART4 clock reset */
    RCU_I2C0RST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 21U),             /*!< I2C0 clock reset */
    RCU_I2C1RST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 22U),             /*!< I2C1 clock reset */
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
    RCU_USBDRST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 23U),             /*!< USBD clock reset */
#endif /* GD32F30X_HD and GD32F30X_XD */
    RCU_CAN0RST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 25U),             /*!< CAN0 clock reset */
#ifdef GD32F30X_CL
    RCU_CAN1RST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 26U),             /*!< CAN1 clock reset */
#endif /* GD32F30X_CL */
    RCU_BKPIRST      = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 27U),             /*!< BKPI clock reset */
    RCU_PMURST       = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 28U),             /*!< PMU clock reset */
    RCU_DACRST       = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 29U),             /*!< DAC clock reset */
    RCU_CTCRST       = RCU_REGIDX_BIT(ADD_APB1RST_REG_OFFSET, 27U),         /*!< RTC clock reset */
    
    /* APB2 peripherals */
    RCU_AFRST        = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 0U),              /*!< alternate function clock reset */
    RCU_GPIOARST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 2U),              /*!< GPIOA clock reset */
    RCU_GPIOBRST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 3U),              /*!< GPIOB clock reset */
    RCU_GPIOCRST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 4U),              /*!< GPIOC clock reset */
    RCU_GPIODRST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 5U),              /*!< GPIOD clock reset */
    RCU_GPIOERST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 6U),              /*!< GPIOE clock reset */
    RCU_GPIOFRST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 7U),              /*!< GPIOF clock reset */
    RCU_GPIOGRST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 8U),              /*!< GPIOG clock reset */
    RCU_ADC0RST      = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 9U),              /*!< ADC0 clock reset */
    RCU_ADC1RST      = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 10U),             /*!< ADC1 clock reset */
    RCU_TIMER0RST    = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 11U),             /*!< TIMER0 clock reset */
    RCU_SPI0RST      = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 12U),             /*!< SPI0 clock reset */
    RCU_TIMER7RST    = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 13U),             /*!< TIMER7 clock reset */
    RCU_USART0RST    = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 14U),             /*!< USART0 clock reset */
#ifndef GD32F30X_CL
    RCU_ADC2RST      = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 15U),             /*!< ADC2 clock reset */
#endif /* GD32F30X_CL */
#ifndef GD32F30X_HD
    RCU_TIMER8RST    = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 19U),             /*!< TIMER8 clock reset */
    RCU_TIMER9RST    = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 20U),             /*!< TIMER9 clock reset */
    RCU_TIMER10RST   = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 21U),             /*!< TIMER10 clock reset */
#endif /* GD32F30X_HD */
}rcu_periph_reset_enum;

/* clock stabilization and peripheral reset flags */
typedef enum
{
    /* clock stabilization flags */
    RCU_FLAG_IRC8MSTB      = RCU_REGIDX_BIT(CTL_REG_OFFSET, 1U),            /*!< IRC8M stabilization flags */
    RCU_FLAG_HXTALSTB      = RCU_REGIDX_BIT(CTL_REG_OFFSET, 17U),           /*!< HXTAL stabilization flags */
    RCU_FLAG_PLLSTB        = RCU_REGIDX_BIT(CTL_REG_OFFSET, 25U),           /*!< PLL stabilization flags */
#ifdef GD32F30X_CL
    RCU_FLAG_PLL1STB       = RCU_REGIDX_BIT(CTL_REG_OFFSET, 27U),           /*!< PLL1 stabilization flags */
    RCU_FLAG_PLL2STB       = RCU_REGIDX_BIT(CTL_REG_OFFSET, 29U),           /*!< PLL2 stabilization flags */
#endif /* GD32F30X_CL */
    RCU_FLAG_LXTALSTB      = RCU_REGIDX_BIT(BDCTL_REG_OFFSET, 1U),          /*!< LXTAL stabilization flags */
    RCU_FLAG_IRC40KSTB     = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 1U),         /*!< IRC40K stabilization flags */
    RCU_FLAG_IRC48MSTB     = RCU_REGIDX_BIT(ADDCTL_REG_OFFSET, 17U),        /*!< IRC48M stabilization flags */
    /* reset source flags */
    RCU_FLAG_EPRST         = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 26U),        /*!< external PIN reset flags */
    RCU_FLAG_PORRST        = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 27U),        /*!< power reset flags */
    RCU_FLAG_SWRST         = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 28U),        /*!< software reset flags */
    RCU_FLAG_FWDGTRST      = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 29U),        /*!< FWDGT reset flags */
    RCU_FLAG_WWDGTRST      = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 30U),        /*!< WWDGT reset flags */
    RCU_FLAG_LPRST         = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 31U),        /*!< low-power reset flags */
}rcu_flag_enum;

/* clock stabilization and ckm interrupt flags */
typedef enum
{
    RCU_INT_FLAG_IRC40KSTB = RCU_REGIDX_BIT(INT_REG_OFFSET, 0U),            /*!< IRC40K stabilization interrupt flag */
    RCU_INT_FLAG_LXTALSTB  = RCU_REGIDX_BIT(INT_REG_OFFSET, 1U),            /*!< LXTAL stabilization interrupt flag */
    RCU_INT_FLAG_IRC8MSTB  = RCU_REGIDX_BIT(INT_REG_OFFSET, 2U),            /*!< IRC8M stabilization interrupt flag */
    RCU_INT_FLAG_HXTALSTB  = RCU_REGIDX_BIT(INT_REG_OFFSET, 3U),            /*!< HXTAL stabilization interrupt flag */
    RCU_INT_FLAG_PLLSTB    = RCU_REGIDX_BIT(INT_REG_OFFSET, 4U),            /*!< PLL stabilization interrupt flag */
#ifdef GD32F30X_CL
    RCU_INT_FLAG_PLL1STB   = RCU_REGIDX_BIT(INT_REG_OFFSET, 5U),            /*!< PLL1 stabilization interrupt flag */
    RCU_INT_FLAG_PLL2STB   = RCU_REGIDX_BIT(INT_REG_OFFSET, 6U),            /*!< PLL2 stabilization interrupt flag */
#endif /* GD32F30X_CL */
    RCU_INT_FLAG_CKM       = RCU_REGIDX_BIT(INT_REG_OFFSET, 7U),            /*!< HXTAL clock stuck interrupt flag */
    RCU_INT_FLAG_IRC48MSTB = RCU_REGIDX_BIT(ADDINT_REG_OFFSET, 6U),         /*!< IRC48M stabilization interrupt flag */
}rcu_int_flag_enum;

/* clock stabilization and stuck interrupt flags clear */
typedef enum
{
    RCU_INT_FLAG_IRC40KSTB_CLR = RCU_REGIDX_BIT(INT_REG_OFFSET, 16U),       /*!< IRC40K stabilization interrupt flags clear */
    RCU_INT_FLAG_LXTALSTB_CLR  = RCU_REGIDX_BIT(INT_REG_OFFSET, 17U),       /*!< LXTAL stabilization interrupt flags clear */
    RCU_INT_FLAG_IRC8MSTB_CLR  = RCU_REGIDX_BIT(INT_REG_OFFSET, 18U),       /*!< IRC8M stabilization interrupt flags clear */
    RCU_INT_FLAG_HXTALSTB_CLR  = RCU_REGIDX_BIT(INT_REG_OFFSET, 19U),       /*!< HXTAL stabilization interrupt flags clear */
    RCU_INT_FLAG_PLLSTB_CLR    = RCU_REGIDX_BIT(INT_REG_OFFSET, 20U),       /*!< PLL stabilization interrupt flags clear */
#ifdef GD32F30X_CL
    RCU_INT_FLAG_PLL1STB_CLR   = RCU_REGIDX_BIT(INT_REG_OFFSET, 21U),       /*!< PLL1 stabilization interrupt flags clear */
    RCU_INT_FLAG_PLL2STB_CLR   = RCU_REGIDX_BIT(INT_REG_OFFSET, 22U),       /*!< PLL2 stabilization interrupt flags clear */
#endif /* GD32F30X_CL */
    RCU_INT_FLAG_CKM_CLR       = RCU_REGIDX_BIT(INT_REG_OFFSET, 23U),       /*!< CKM interrupt flags clear */
    RCU_INT_FLAG_IRC48MSTB_CLR = RCU_REGIDX_BIT(ADDINT_REG_OFFSET, 22U),    /*!< internal 48 MHz RC oscillator stabilization interrupt clear */
}rcu_int_flag_clear_enum;

/* clock stabilization interrupt enable or disable */
typedef enum
{
    RCU_INT_IRC40KSTB       = RCU_REGIDX_BIT(INT_REG_OFFSET, 8U),           /*!< IRC40K stabilization interrupt */
    RCU_INT_LXTALSTB        = RCU_REGIDX_BIT(INT_REG_OFFSET, 9U),           /*!< LXTAL stabilization interrupt */
    RCU_INT_IRC8MSTB        = RCU_REGIDX_BIT(INT_REG_OFFSET, 10U),          /*!< IRC8M stabilization interrupt */
    RCU_INT_HXTALSTB        = RCU_REGIDX_BIT(INT_REG_OFFSET, 11U),          /*!< HXTAL stabilization interrupt */
    RCU_INT_PLLSTB          = RCU_REGIDX_BIT(INT_REG_OFFSET, 12U),          /*!< PLL stabilization interrupt */
#ifdef GD32F30X_CL
    RCU_INT_PLL1STB         = RCU_REGIDX_BIT(INT_REG_OFFSET, 13U),          /*!< PLL1 stabilization interrupt */
    RCU_INT_PLL2STB         = RCU_REGIDX_BIT(INT_REG_OFFSET, 14U),          /*!< PLL2 stabilization interrupt */
#endif /* GD32F30X_CL */
    RCU_INT_IRC48MSTB       = RCU_REGIDX_BIT(ADDINT_REG_OFFSET, 14U),       /*!< internal 48 MHz RC oscillator stabilization interrupt */
}rcu_int_enum;

/* oscillator types */
typedef enum
{
    RCU_HXTAL      = RCU_REGIDX_BIT(CTL_REG_OFFSET, 16U),                   /*!< HXTAL */
    RCU_LXTAL      = RCU_REGIDX_BIT(BDCTL_REG_OFFSET, 0U),                  /*!< LXTAL */
    RCU_IRC8M      = RCU_REGIDX_BIT(CTL_REG_OFFSET, 0U),                    /*!< IRC8M */
    RCU_IRC48M     = RCU_REGIDX_BIT(ADDCTL_REG_OFFSET, 16U),                /*!< IRC48M */
    RCU_IRC40K     = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 0U),                 /*!< IRC40K */
    RCU_PLL_CK     = RCU_REGIDX_BIT(CTL_REG_OFFSET, 24U),                   /*!< PLL */
#ifdef GD32F30X_CL
    RCU_PLL1_CK    = RCU_REGIDX_BIT(CTL_REG_OFFSET, 26U),                   /*!< PLL1 */
    RCU_PLL2_CK    = RCU_REGIDX_BIT(CTL_REG_OFFSET, 28U),                   /*!< PLL2 */
#endif /* GD32F30X_CL */
}rcu_osci_type_enum;

/* rcu clock frequency */
typedef enum
{
    CK_SYS      = 0,                                                        /*!< system clock */
    CK_AHB,                                                                 /*!< AHB clock */
    CK_APB1,                                                                /*!< APB1 clock */
    CK_APB2,                                                                /*!< APB2 clock */
}rcu_clock_freq_enum;

/* RCU_CFG0 register bit define */
/* system clock source select */
#define CFG0_SCS(regval)                (BITS(0,1) & ((uint32_t)(regval) << 0))
#define RCU_CKSYSSRC_IRC8M              CFG0_SCS(0)                         /*!< system clock source select IRC8M */
#define RCU_CKSYSSRC_HXTAL              CFG0_SCS(1)                         /*!< system clock source select HXTAL */
#define RCU_CKSYSSRC_PLL                CFG0_SCS(2)                         /*!< system clock source select PLL */

/* system clock source select status */
#define CFG0_SCSS(regval)               (BITS(2,3) & ((uint32_t)(regval) << 2))
#define RCU_SCSS_IRC8M                  CFG0_SCSS(0)                        /*!< system clock source select IRC8M */
#define RCU_SCSS_HXTAL                  CFG0_SCSS(1)                        /*!< system clock source select HXTAL */
#define RCU_SCSS_PLL                    CFG0_SCSS(2)                        /*!< system clock source select PLLP */

/* AHB prescaler selection */
#define CFG0_AHBPSC(regval)             (BITS(4,7) & ((uint32_t)(regval) << 4))
#define RCU_AHB_CKSYS_DIV1              CFG0_AHBPSC(0)                      /*!< AHB prescaler select CK_SYS */
#define RCU_AHB_CKSYS_DIV2              CFG0_AHBPSC(8)                      /*!< AHB prescaler select CK_SYS/2 */
#define RCU_AHB_CKSYS_DIV4              CFG0_AHBPSC(9)                      /*!< AHB prescaler select CK_SYS/4 */
#define RCU_AHB_CKSYS_DIV8              CFG0_AHBPSC(10)                     /*!< AHB prescaler select CK_SYS/8 */
#define RCU_AHB_CKSYS_DIV16             CFG0_AHBPSC(11)                     /*!< AHB prescaler select CK_SYS/16 */
#define RCU_AHB_CKSYS_DIV64             CFG0_AHBPSC(12)                     /*!< AHB prescaler select CK_SYS/64 */
#define RCU_AHB_CKSYS_DIV128            CFG0_AHBPSC(13)                     /*!< AHB prescaler select CK_SYS/128 */
#define RCU_AHB_CKSYS_DIV256            CFG0_AHBPSC(14)                     /*!< AHB prescaler select CK_SYS/256 */
#define RCU_AHB_CKSYS_DIV512            CFG0_AHBPSC(15)                     /*!< AHB prescaler select CK_SYS/512 */

/* APB1 prescaler selection */
#define CFG0_APB1PSC(regval)            (BITS(8,10) & ((uint32_t)(regval) << 8))
#define RCU_APB1_CKAHB_DIV1             CFG0_APB1PSC(0)                     /*!< APB1 prescaler select CK_AHB */
#define RCU_APB1_CKAHB_DIV2             CFG0_APB1PSC(4)                     /*!< APB1 prescaler select CK_AHB/2 */
#define RCU_APB1_CKAHB_DIV4             CFG0_APB1PSC(5)                     /*!< APB1 prescaler select CK_AHB/4 */
#define RCU_APB1_CKAHB_DIV8             CFG0_APB1PSC(6)                     /*!< APB1 prescaler select CK_AHB/8 */
#define RCU_APB1_CKAHB_DIV16            CFG0_APB1PSC(7)                     /*!< APB1 prescaler select CK_AHB/16 */

/* APB2 prescaler selection */
#define CFG0_APB2PSC(regval)            (BITS(11,13) & ((uint32_t)(regval) << 11))
#define RCU_APB2_CKAHB_DIV1             CFG0_APB2PSC(0)                     /*!< APB2 prescaler select CK_AHB */
#define RCU_APB2_CKAHB_DIV2             CFG0_APB2PSC(4)                     /*!< APB2 prescaler select CK_AHB/2 */
#define RCU_APB2_CKAHB_DIV4             CFG0_APB2PSC(5)                     /*!< APB2 prescaler select CK_AHB/4 */
#define RCU_APB2_CKAHB_DIV8             CFG0_APB2PSC(6)                     /*!< APB2 prescaler select CK_AHB/8 */
#define RCU_APB2_CKAHB_DIV16            CFG0_APB2PSC(7)                     /*!< APB2 prescaler select CK_AHB/16 */

/* ADC prescaler select */
#define RCU_CKADC_CKAPB2_DIV2           ((uint32_t)0x00000000U)             /*!< ADC prescaler select CK_APB2/2 */
#define RCU_CKADC_CKAPB2_DIV4           ((uint32_t)0x00000001U)             /*!< ADC prescaler select CK_APB2/4 */
#define RCU_CKADC_CKAPB2_DIV6           ((uint32_t)0x00000002U)             /*!< ADC prescaler select CK_APB2/6 */
#define RCU_CKADC_CKAPB2_DIV8           ((uint32_t)0x00000003U)             /*!< ADC prescaler select CK_APB2/8 */
#define RCU_CKADC_CKAPB2_DIV12          ((uint32_t)0x00000005U)             /*!< ADC prescaler select CK_APB2/12 */
#define RCU_CKADC_CKAPB2_DIV16          ((uint32_t)0x00000007U)             /*!< ADC prescaler select CK_APB2/16 */
#define RCU_CKADC_CKAHB_DIV5            ((uint32_t)0x00000008U)             /*!< ADC prescaler select CK_AHB/5 */
#define RCU_CKADC_CKAHB_DIV6            ((uint32_t)0x00000009U)             /*!< ADC prescaler select CK_AHB/6 */
#define RCU_CKADC_CKAHB_DIV10           ((uint32_t)0x0000000AU)             /*!< ADC prescaler select CK_AHB/10 */
#define RCU_CKADC_CKAHB_DIV20           ((uint32_t)0x0000000BU)             /*!< ADC prescaler select CK_AHB/20 */

/* PLL clock source selection */
#define RCU_PLLSRC_IRC8M_DIV2           ((uint32_t)0x00000000U)             /*!< IRC8M/2 clock selected as source clock of PLL */
#define RCU_PLLSRC_HXTAL_IRC48M         RCU_CFG0_PLLSEL                     /*!< HXTAL or IRC48M selected as source clock of PLL */

/* PLL clock multiplication factor */
#define PLLMF_4                         RCU_CFG0_PLLMF_4                    /* bit 4 of PLLMF */
#define PLLMF_5                         RCU_CFG0_PLLMF_5                    /* bit 5 of PLLMF */
#define PLLMF_4_5                       (PLLMF_4 | PLLMF_5)                 /* bit 4 and 5 of PLLMF */

#define CFG0_PLLMF(regval)              (BITS(18,21) & ((uint32_t)(regval) << 18))
#define RCU_PLL_MUL2                    CFG0_PLLMF(0)                       /*!< PLL source clock multiply by 2 */
#define RCU_PLL_MUL3                    CFG0_PLLMF(1)                       /*!< PLL source clock multiply by 3 */
#define RCU_PLL_MUL4                    CFG0_PLLMF(2)                       /*!< PLL source clock multiply by 4 */
#define RCU_PLL_MUL5                    CFG0_PLLMF(3)                       /*!< PLL source clock multiply by 5 */
#define RCU_PLL_MUL6                    CFG0_PLLMF(4)                       /*!< PLL source clock multiply by 6 */
#define RCU_PLL_MUL7                    CFG0_PLLMF(5)                       /*!< PLL source clock multiply by 7 */
#define RCU_PLL_MUL8                    CFG0_PLLMF(6)                       /*!< PLL source clock multiply by 8 */
#define RCU_PLL_MUL9                    CFG0_PLLMF(7)                       /*!< PLL source clock multiply by 9 */
#define RCU_PLL_MUL10                   CFG0_PLLMF(8)                       /*!< PLL source clock multiply by 10 */
#define RCU_PLL_MUL11                   CFG0_PLLMF(9)                       /*!< PLL source clock multiply by 11 */
#define RCU_PLL_MUL12                   CFG0_PLLMF(10)                      /*!< PLL source clock multiply by 12 */
#define RCU_PLL_MUL13                   CFG0_PLLMF(11)                      /*!< PLL source clock multiply by 13 */
#define RCU_PLL_MUL14                   CFG0_PLLMF(12)                      /*!< PLL source clock multiply by 14 */
#if(defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define RCU_PLL_MUL15                   CFG0_PLLMF(13)                      /*!< PLL source clock multiply by 15 */
#elif defined(GD32F30X_CL)
#define RCU_PLL_MUL6_5                  CFG0_PLLMF(13)                      /*!< PLL source clock multiply by 6.5 */
#endif /* GD32F30X_HD and GD32F30X_XD */
#define RCU_PLL_MUL16                   CFG0_PLLMF(14)                      /*!< PLL source clock multiply by 16 */
#define RCU_PLL_MUL17                   (PLLMF_4 | CFG0_PLLMF(0))           /*!< PLL source clock multiply by 17 */
#define RCU_PLL_MUL18                   (PLLMF_4 | CFG0_PLLMF(1))           /*!< PLL source clock multiply by 18 */
#define RCU_PLL_MUL19                   (PLLMF_4 | CFG0_PLLMF(2))           /*!< PLL source clock multiply by 19 */
#define RCU_PLL_MUL20                   (PLLMF_4 | CFG0_PLLMF(3))           /*!< PLL source clock multiply by 20 */
#define RCU_PLL_MUL21                   (PLLMF_4 | CFG0_PLLMF(4))           /*!< PLL source clock multiply by 21 */
#define RCU_PLL_MUL22                   (PLLMF_4 | CFG0_PLLMF(5))           /*!< PLL source clock multiply by 22 */
#define RCU_PLL_MUL23                   (PLLMF_4 | CFG0_PLLMF(6))           /*!< PLL source clock multiply by 23 */
#define RCU_PLL_MUL24                   (PLLMF_4 | CFG0_PLLMF(7))           /*!< PLL source clock multiply by 24 */
#define RCU_PLL_MUL25                   (PLLMF_4 | CFG0_PLLMF(8))           /*!< PLL source clock multiply by 25 */
#define RCU_PLL_MUL26                   (PLLMF_4 | CFG0_PLLMF(9))           /*!< PLL source clock multiply by 26 */
#define RCU_PLL_MUL27                   (PLLMF_4 | CFG0_PLLMF(10))          /*!< PLL source clock multiply by 27 */
#define RCU_PLL_MUL28                   (PLLMF_4 | CFG0_PLLMF(11))          /*!< PLL source clock multiply by 28 */
#define RCU_PLL_MUL29                   (PLLMF_4 | CFG0_PLLMF(12))          /*!< PLL source clock multiply by 29 */
#define RCU_PLL_MUL30                   (PLLMF_4 | CFG0_PLLMF(13))          /*!< PLL source clock multiply by 30 */
#define RCU_PLL_MUL31                   (PLLMF_4 | CFG0_PLLMF(14))          /*!< PLL source clock multiply by 31 */
#define RCU_PLL_MUL32                   (PLLMF_4 | CFG0_PLLMF(15))          /*!< PLL source clock multiply by 32 */
#define RCU_PLL_MUL33                   (PLLMF_5 | CFG0_PLLMF(0))           /*!< PLL source clock multiply by 33 */
#define RCU_PLL_MUL34                   (PLLMF_5 | CFG0_PLLMF(1))           /*!< PLL source clock multiply by 34 */
#define RCU_PLL_MUL35                   (PLLMF_5 | CFG0_PLLMF(2))           /*!< PLL source clock multiply by 35 */
#define RCU_PLL_MUL36                   (PLLMF_5 | CFG0_PLLMF(3))           /*!< PLL source clock multiply by 36 */
#define RCU_PLL_MUL37                   (PLLMF_5 | CFG0_PLLMF(4))           /*!< PLL source clock multiply by 37 */
#define RCU_PLL_MUL38                   (PLLMF_5 | CFG0_PLLMF(5))           /*!< PLL source clock multiply by 38 */
#define RCU_PLL_MUL39                   (PLLMF_5 | CFG0_PLLMF(6))           /*!< PLL source clock multiply by 39 */
#define RCU_PLL_MUL40                   (PLLMF_5 | CFG0_PLLMF(7))           /*!< PLL source clock multiply by 40 */
#define RCU_PLL_MUL41                   (PLLMF_5 | CFG0_PLLMF(8))           /*!< PLL source clock multiply by 41 */
#define RCU_PLL_MUL42                   (PLLMF_5 | CFG0_PLLMF(9))           /*!< PLL source clock multiply by 42 */
#define RCU_PLL_MUL43                   (PLLMF_5 | CFG0_PLLMF(10))          /*!< PLL source clock multiply by 43 */
#define RCU_PLL_MUL44                   (PLLMF_5 | CFG0_PLLMF(11))          /*!< PLL source clock multiply by 44 */
#define RCU_PLL_MUL45                   (PLLMF_5 | CFG0_PLLMF(12))          /*!< PLL source clock multiply by 45 */
#define RCU_PLL_MUL46                   (PLLMF_5 | CFG0_PLLMF(13))          /*!< PLL source clock multiply by 46 */
#define RCU_PLL_MUL47                   (PLLMF_5 | CFG0_PLLMF(14))          /*!< PLL source clock multiply by 47 */
#define RCU_PLL_MUL48                   (PLLMF_5 | CFG0_PLLMF(15))          /*!< PLL source clock multiply by 48 */
#define RCU_PLL_MUL49                   (PLLMF_4_5 | CFG0_PLLMF(0))         /*!< PLL source clock multiply by 49 */
#define RCU_PLL_MUL50                   (PLLMF_4_5 | CFG0_PLLMF(1))         /*!< PLL source clock multiply by 50 */
#define RCU_PLL_MUL51                   (PLLMF_4_5 | CFG0_PLLMF(2))         /*!< PLL source clock multiply by 51 */
#define RCU_PLL_MUL52                   (PLLMF_4_5 | CFG0_PLLMF(3))         /*!< PLL source clock multiply by 52 */
#define RCU_PLL_MUL53                   (PLLMF_4_5 | CFG0_PLLMF(4))         /*!< PLL source clock multiply by 53 */
#define RCU_PLL_MUL54                   (PLLMF_4_5 | CFG0_PLLMF(5))         /*!< PLL source clock multiply by 54 */
#define RCU_PLL_MUL55                   (PLLMF_4_5 | CFG0_PLLMF(6))         /*!< PLL source clock multiply by 55 */
#define RCU_PLL_MUL56                   (PLLMF_4_5 | CFG0_PLLMF(7))         /*!< PLL source clock multiply by 56 */
#define RCU_PLL_MUL57                   (PLLMF_4_5 | CFG0_PLLMF(8))         /*!< PLL source clock multiply by 57 */
#define RCU_PLL_MUL58                   (PLLMF_4_5 | CFG0_PLLMF(9))         /*!< PLL source clock multiply by 58 */
#define RCU_PLL_MUL59                   (PLLMF_4_5 | CFG0_PLLMF(10))        /*!< PLL source clock multiply by 59 */
#define RCU_PLL_MUL60                   (PLLMF_4_5 | CFG0_PLLMF(11))        /*!< PLL source clock multiply by 60 */
#define RCU_PLL_MUL61                   (PLLMF_4_5 | CFG0_PLLMF(12))        /*!< PLL source clock multiply by 61 */
#define RCU_PLL_MUL62                   (PLLMF_4_5 | CFG0_PLLMF(13))        /*!< PLL source clock multiply by 62 */
#define RCU_PLL_MUL63                   (PLLMF_4_5 | CFG0_PLLMF(14))        /*!< PLL source clock multiply by 63 */

#if(defined(GD32F30X_HD) || defined(GD32F30X_XD))
#define USBPSC_2 RCU_CFG0_USBDPSC_2
#elif defined(GD32F30X_CL)
#define USBPSC_2 RCU_CFG0_USBFSPSC_2
#endif /* GD32F30X_HD and GD32F30X_XD */

/* USBD/USBFS prescaler select */
#define CFG0_USBPSC(regval)             (BITS(22,23) & ((uint32_t)(regval) << 22))
#define RCU_CKUSB_CKPLL_DIV1_5          CFG0_USBPSC(0)                      /*!< USBD/USBFS prescaler select CK_PLL/1.5 */
#define RCU_CKUSB_CKPLL_DIV1            CFG0_USBPSC(1)                      /*!< USBD/USBFS prescaler select CK_PLL/1 */
#define RCU_CKUSB_CKPLL_DIV2_5          CFG0_USBPSC(2)                      /*!< USBD/USBFS prescaler select CK_PLL/2.5 */
#define RCU_CKUSB_CKPLL_DIV2            CFG0_USBPSC(3)                      /*!< USBD/USBFS prescaler select CK_PLL/2 */
#define RCU_CKUSB_CKPLL_DIV3            (USBPSC_2 |CFG0_USBPSC(0))          /*!< USBD/USBFS prescaler select CK_PLL/3 */
#define RCU_CKUSB_CKPLL_DIV3_5          (USBPSC_2 |CFG0_USBPSC(1))          /*!< USBD/USBFS prescaler select CK_PLL/3.5 */
#define RCU_CKUSB_CKPLL_DIV4            (USBPSC_2 |CFG0_USBPSC(2))          /*!< USBD/USBFS prescaler select CK_PLL/4 */

/* CKOUT0 Clock source selection */
#define CFG0_CKOUT0SEL(regval)          (BITS(24,27) & ((uint32_t)(regval) << 24))
#define RCU_CKOUT0SRC_NONE              CFG0_CKOUT0SEL(0)                   /*!< no clock selected */
#define RCU_CKOUT0SRC_CKSYS             CFG0_CKOUT0SEL(4)                   /*!< system clock selected */
#define RCU_CKOUT0SRC_IRC8M             CFG0_CKOUT0SEL(5)                   /*!< internal 8M RC oscillator clock selected */
#define RCU_CKOUT0SRC_HXTAL             CFG0_CKOUT0SEL(6)                   /*!< high speed crystal oscillator clock (HXTAL) selected */
#define RCU_CKOUT0SRC_CKPLL_DIV2        CFG0_CKOUT0SEL(7)                   /*!< CK_PLL/2 clock selected */
#ifdef GD32F30X_CL
#define RCU_CKOUT0SRC_CKPLL1            CFG0_CKOUT0SEL(8)                   /*!< CK_PLL1 clock selected */
#define RCU_CKOUT0SRC_CKPLL2_DIV2       CFG0_CKOUT0SEL(9)                   /*!< CK_PLL2/2 clock selected */
#define RCU_CKOUT0SRC_EXT1              CFG0_CKOUT0SEL(10)                  /*!< EXT1 selected, to provide the external clock for ENET */
#define RCU_CKOUT0SRC_CKPLL2            CFG0_CKOUT0SEL(11)                  /*!< CK_PLL2 clock selected */
#endif /* GD32F30X_CL */

/* LXTAL drive capability */
#define BDCTL_LXTALDRI(regval)          (BITS(3,4) & ((uint32_t)(regval) << 3))
#define RCU_LXTAL_LOWDRI                BDCTL_LXTALDRI(0)                   /*!< lower driving capability */
#define RCU_LXTAL_MED_LOWDRI            BDCTL_LXTALDRI(1)                   /*!< medium low driving capability */
#define RCU_LXTAL_MED_HIGHDRI           BDCTL_LXTALDRI(2)                   /*!< medium high driving capability */
#define RCU_LXTAL_HIGHDRI               BDCTL_LXTALDRI(3)                   /*!< higher driving capability */

/* RTC clock entry selection */
#define BDCTL_RTCSRC(regval)            (BITS(8,9) & ((uint32_t)(regval) << 8))
#define RCU_RTCSRC_NONE                 BDCTL_RTCSRC(0)                     /*!< no clock selected */
#define RCU_RTCSRC_LXTAL                BDCTL_RTCSRC(1)                     /*!< RTC source clock select LXTAL  */
#define RCU_RTCSRC_IRC40K               BDCTL_RTCSRC(2)                     /*!< RTC source clock select IRC40K */
#define RCU_RTCSRC_HXTAL_DIV_128        BDCTL_RTCSRC(3)                     /*!< RTC source clock select HXTAL/128 */

/* PREDV0 division factor */
#define CFG1_PREDV0(regval)             (BITS(0,3) & ((uint32_t)(regval) << 0))
#define RCU_PREDV0_DIV1                CFG1_PREDV0(0)                       /*!< PREDV0 input source clock not divided */
#define RCU_PREDV0_DIV2                CFG1_PREDV0(1)                       /*!< PREDV0 input source clock divided by 2 */
#define RCU_PREDV0_DIV3                CFG1_PREDV0(2)                       /*!< PREDV0 input source clock divided by 3 */
#define RCU_PREDV0_DIV4                CFG1_PREDV0(3)                       /*!< PREDV0 input source clock divided by 4 */
#define RCU_PREDV0_DIV5                CFG1_PREDV0(4)                       /*!< PREDV0 input source clock divided by 5 */
#define RCU_PREDV0_DIV6                CFG1_PREDV0(5)                       /*!< PREDV0 input source clock divided by 6 */
#define RCU_PREDV0_DIV7                CFG1_PREDV0(6)                       /*!< PREDV0 input source clock divided by 7 */
#define RCU_PREDV0_DIV8                CFG1_PREDV0(7)                       /*!< PREDV0 input source clock divided by 8 */
#define RCU_PREDV0_DIV9                CFG1_PREDV0(8)                       /*!< PREDV0 input source clock divided by 9 */
#define RCU_PREDV0_DIV10               CFG1_PREDV0(9)                       /*!< PREDV0 input source clock divided by 10 */
#define RCU_PREDV0_DIV11               CFG1_PREDV0(10)                      /*!< PREDV0 input source clock divided by 11 */
#define RCU_PREDV0_DIV12               CFG1_PREDV0(11)                      /*!< PREDV0 input source clock divided by 12 */
#define RCU_PREDV0_DIV13               CFG1_PREDV0(12)                      /*!< PREDV0 input source clock divided by 13 */
#define RCU_PREDV0_DIV14               CFG1_PREDV0(13)                      /*!< PREDV0 input source clock divided by 14 */
#define RCU_PREDV0_DIV15               CFG1_PREDV0(14)                      /*!< PREDV0 input source clock divided by 15 */
#define RCU_PREDV0_DIV16               CFG1_PREDV0(15)                      /*!< PREDV0 input source clock divided by 16 */

/* PREDV1 division factor */
#define CFG1_PREDV1(regval)             (BITS(4,7) & ((uint32_t)(regval) << 4))
#define RCU_PREDV1_DIV1                CFG1_PREDV1(0)                       /*!< PREDV1 input source clock not divided */
#define RCU_PREDV1_DIV2                CFG1_PREDV1(1)                       /*!< PREDV1 input source clock divided by 2 */
#define RCU_PREDV1_DIV3                CFG1_PREDV1(2)                       /*!< PREDV1 input source clock divided by 3 */
#define RCU_PREDV1_DIV4                CFG1_PREDV1(3)                       /*!< PREDV1 input source clock divided by 4 */
#define RCU_PREDV1_DIV5                CFG1_PREDV1(4)                       /*!< PREDV1 input source clock divided by 5 */
#define RCU_PREDV1_DIV6                CFG1_PREDV1(5)                       /*!< PREDV1 input source clock divided by 6 */
#define RCU_PREDV1_DIV7                CFG1_PREDV1(6)                       /*!< PREDV1 input source clock divided by 7 */
#define RCU_PREDV1_DIV8                CFG1_PREDV1(7)                       /*!< PREDV1 input source clock divided by 8 */
#define RCU_PREDV1_DIV9                CFG1_PREDV1(8)                       /*!< PREDV1 input source clock divided by 9 */
#define RCU_PREDV1_DIV10               CFG1_PREDV1(9)                       /*!< PREDV1 input source clock divided by 10 */
#define RCU_PREDV1_DIV11               CFG1_PREDV1(10)                      /*!< PREDV1 input source clock divided by 11 */
#define RCU_PREDV1_DIV12               CFG1_PREDV1(11)                      /*!< PREDV1 input source clock divided by 12 */
#define RCU_PREDV1_DIV13               CFG1_PREDV1(12)                      /*!< PREDV1 input source clock divided by 13 */
#define RCU_PREDV1_DIV14               CFG1_PREDV1(13)                      /*!< PREDV1 input source clock divided by 14 */
#define RCU_PREDV1_DIV15               CFG1_PREDV1(14)                      /*!< PREDV1 input source clock divided by 15 */
#define RCU_PREDV1_DIV16               CFG1_PREDV1(15)                      /*!< PREDV1 input source clock divided by 16 */

/* PLL1 clock multiplication factor */
#define CFG1_PLL1MF(regval)             (BITS(8,11) & ((uint32_t)(regval) << 8))
#define RCU_PLL1_MUL8                   CFG1_PLL1MF(6)                      /*!< PLL1 source clock multiply by 8 */
#define RCU_PLL1_MUL9                   CFG1_PLL1MF(7)                      /*!< PLL1 source clock multiply by 9 */
#define RCU_PLL1_MUL10                  CFG1_PLL1MF(8)                      /*!< PLL1 source clock multiply by 10 */
#define RCU_PLL1_MUL11                  CFG1_PLL1MF(9)                      /*!< PLL1 source clock multiply by 11 */
#define RCU_PLL1_MUL12                  CFG1_PLL1MF(10)                     /*!< PLL1 source clock multiply by 12 */
#define RCU_PLL1_MUL13                  CFG1_PLL1MF(11)                     /*!< PLL1 source clock multiply by 13 */
#define RCU_PLL1_MUL14                  CFG1_PLL1MF(12)                     /*!< PLL1 source clock multiply by 14 */
#define RCU_PLL1_MUL15                  CFG1_PLL1MF(13)                     /*!< PLL1 source clock multiply by 15 */
#define RCU_PLL1_MUL16                  CFG1_PLL1MF(14)                     /*!< PLL1 source clock multiply by 16 */
#define RCU_PLL1_MUL20                  CFG1_PLL1MF(15)                     /*!< PLL1 source clock multiply by 20 */

/* PLL2 clock multiplication factor */
#define PLL2MF_4                        RCU_CFG1_PLL2MF_4                   /* bit 4 of PLL2MF */

#define CFG1_PLL2MF(regval)             (BITS(12,15) & ((uint32_t)(regval) << 12))
#define RCU_PLL2_MUL8                   CFG1_PLL2MF(6)                      /*!< PLL2 source clock multiply by 8 */
#define RCU_PLL2_MUL9                   CFG1_PLL2MF(7)                      /*!< PLL2 source clock multiply by 9 */
#define RCU_PLL2_MUL10                  CFG1_PLL2MF(8)                      /*!< PLL2 source clock multiply by 10 */
#define RCU_PLL2_MUL11                  CFG1_PLL2MF(9)                      /*!< PLL2 source clock multiply by 11 */
#define RCU_PLL2_MUL12                  CFG1_PLL2MF(10)                     /*!< PLL2 source clock multiply by 12 */
#define RCU_PLL2_MUL13                  CFG1_PLL2MF(11)                     /*!< PLL2 source clock multiply by 13 */
#define RCU_PLL2_MUL14                  CFG1_PLL2MF(12)                     /*!< PLL2 source clock multiply by 14 */
#define RCU_PLL2_MUL15                  CFG1_PLL2MF(13)                     /*!< PLL2 source clock multiply by 15 */
#define RCU_PLL2_MUL16                  CFG1_PLL2MF(14)                     /*!< PLL2 source clock multiply by 16 */
#define RCU_PLL2_MUL20                  CFG1_PLL2MF(15)                     /*!< PLL2 source clock multiply by 20 */
#define RCU_PLL2_MUL18                  (PLL2MF_4 | CFG1_PLL2MF(0))         /*!< PLL2 source clock multiply by 18 */
#define RCU_PLL2_MUL19                  (PLL2MF_4 | CFG1_PLL2MF(1))         /*!< PLL2 source clock multiply by 19 */
#define RCU_PLL2_MUL21                  (PLL2MF_4 | CFG1_PLL2MF(3))         /*!< PLL2 source clock multiply by 21 */
#define RCU_PLL2_MUL22                  (PLL2MF_4 | CFG1_PLL2MF(4))         /*!< PLL2 source clock multiply by 22 */
#define RCU_PLL2_MUL23                  (PLL2MF_4 | CFG1_PLL2MF(5))         /*!< PLL2 source clock multiply by 23 */
#define RCU_PLL2_MUL24                  (PLL2MF_4 | CFG1_PLL2MF(6))         /*!< PLL2 source clock multiply by 24 */
#define RCU_PLL2_MUL25                  (PLL2MF_4 | CFG1_PLL2MF(7))         /*!< PLL2 source clock multiply by 25 */
#define RCU_PLL2_MUL26                  (PLL2MF_4 | CFG1_PLL2MF(8))         /*!< PLL2 source clock multiply by 26 */
#define RCU_PLL2_MUL27                  (PLL2MF_4 | CFG1_PLL2MF(9))         /*!< PLL2 source clock multiply by 27 */
#define RCU_PLL2_MUL28                  (PLL2MF_4 | CFG1_PLL2MF(10))        /*!< PLL2 source clock multiply by 28 */
#define RCU_PLL2_MUL29                  (PLL2MF_4 | CFG1_PLL2MF(11))        /*!< PLL2 source clock multiply by 29 */
#define RCU_PLL2_MUL30                  (PLL2MF_4 | CFG1_PLL2MF(12))        /*!< PLL2 source clock multiply by 30 */
#define RCU_PLL2_MUL31                  (PLL2MF_4 | CFG1_PLL2MF(13))        /*!< PLL2 source clock multiply by 31 */
#define RCU_PLL2_MUL32                  (PLL2MF_4 | CFG1_PLL2MF(14))        /*!< PLL2 source clock multiply by 32 */
#define RCU_PLL2_MUL40                  (PLL2MF_4 | CFG1_PLL2MF(15))        /*!< PLL2 source clock multiply by 40 */

#ifdef GD32F30X_CL
/* PREDV0 input clock source selection */
#define RCU_PREDV0SRC_HXTAL_IRC48M      ((uint32_t)0x00000000U)             /*!< HXTAL or IRC48M selected as PREDV0 input source clock */
#define RCU_PREDV0SRC_CKPLL1            RCU_CFG1_PREDV0SEL                  /*!< CK_PLL1 selected as PREDV0 input source clock */

/* I2S1 clock source selection */
#define RCU_I2S1SRC_CKSYS               ((uint32_t)0x00000000U)             /*!< system clock selected as I2S1 source clock */
#define RCU_I2S1SRC_CKPLL2_MUL2         RCU_CFG1_I2S1SEL                    /*!< (CK_PLL2 x 2) selected as I2S1 source clock */

/* I2S2 clock source selection */
#define RCU_I2S2SRC_CKSYS               ((uint32_t)0x00000000U)             /*!< system clock selected as I2S2 source clock */
#define RCU_I2S2SRC_CKPLL2_MUL2         RCU_CFG1_I2S2SEL                    /*!< (CK_PLL2 x 2) selected as I2S2 source clock */
#endif /* GD32F30X_CL */

/* PLL input clock source selection */
#define RCU_PLLPRESRC_HXTAL             ((uint32_t)0x00000000U)             /*!< HXTAL selected as PLL source clock */
#define RCU_PLLPRESRC_IRC48M            RCU_CFG1_PLLPRESEL                  /*!< CK_PLL selected as PREDV0 input source clock */

/* deep-sleep mode voltage */
#define DSV_DSLPVS(regval)              (BITS(0,2) & ((uint32_t)(regval) << 0))
#define RCU_DEEPSLEEP_V_0               DSV_DSLPVS(0)                      /*!< core voltage is default value in deep-sleep mode */
#define RCU_DEEPSLEEP_V_1               DSV_DSLPVS(1)                      /*!< core voltage is (default value-0.1)V in deep-sleep mode(customers are not recommended to use it) */
#define RCU_DEEPSLEEP_V_2               DSV_DSLPVS(2)                      /*!< core voltage is (default value-0.2)V in deep-sleep mode(customers are not recommended to use it) */
#define RCU_DEEPSLEEP_V_3               DSV_DSLPVS(3)                      /*!< core voltage is (default value-0.3)V in deep-sleep mode(customers are not recommended to use it) */

/* 48MHz clock selection */
#define RCU_CK48MSRC_CKPLL              ((uint32_t)0x00000000U)              /*!< use CK_PLL clock */
#define RCU_CK48MSRC_IRC48M             RCU_ADDCTL_CK48MSEL                 /*!< select IRC48M clock */

/* function declarations */
/* peripherals clock configure functions */
/* deinitialize the RCU */
void rcu_deinit(void);
/* enable the peripherals clock */
void rcu_periph_clock_enable(rcu_periph_enum periph);
/* disable the peripherals clock */
void rcu_periph_clock_disable(rcu_periph_enum periph);
/* enable the peripherals clock when sleep mode */
void rcu_periph_clock_sleep_enable(rcu_periph_sleep_enum periph);
/* disable the peripherals clock when sleep mode */
void rcu_periph_clock_sleep_disable(rcu_periph_sleep_enum periph);
/* reset the peripherals */
void rcu_periph_reset_enable(rcu_periph_reset_enum periph_reset);
/* disable reset the peripheral */
void rcu_periph_reset_disable(rcu_periph_reset_enum periph_reset);
/* reset the BKP domain */
void rcu_bkp_reset_enable(void);
/* disable the BKP domain reset */
void rcu_bkp_reset_disable(void);

/* system and peripherals clock source, system reset configure functions */
/* configure the system clock source */
void rcu_system_clock_source_config(uint32_t ck_sys);
/* get the system clock source */
uint32_t rcu_system_clock_source_get(void);
/* configure the AHB prescaler selection */
void rcu_ahb_clock_config(uint32_t ck_ahb);
/* configure the APB1 prescaler selection */
void rcu_apb1_clock_config(uint32_t ck_apb1);
/* configure the APB2 prescaler selection */
void rcu_apb2_clock_config(uint32_t ck_apb2);
/* configure the CK_OUT0 clock source and divider */
void rcu_ckout0_config(uint32_t ckout0_src);
/* configure the PLL clock source selection and PLL multiply factor */
void rcu_pll_config(uint32_t pll_src, uint32_t pll_mul);
/* configure the PLL clock source preselection */
void rcu_pllpresel_config(uint32_t pll_presel);
#if(defined(GD32F30X_HD) || defined(GD32F30X_XD))
/* configure the PREDV0 division factor and clock source */
void rcu_predv0_config(uint32_t predv0_div);
#elif defined(GD32F30X_CL)
/* configure the PREDV0 division factor and clock source */
void rcu_predv0_config(uint32_t predv0_source, uint32_t predv0_div);
/* configure the PREDV1 division factor */
void rcu_predv1_config(uint32_t predv1_div);
/* configure the PLL1 clock */
void rcu_pll1_config(uint32_t pll_mul);
/* configure the PLL2 clock */
void rcu_pll2_config(uint32_t pll_mul);
#endif /* GD32F30X_HD and GD32F30X_XD */

/* configure the ADC division factor */
void rcu_adc_clock_config(uint32_t adc_psc);
/* configure the USBD/USBFS prescaler factor */
void rcu_usb_clock_config(uint32_t usb_psc);
/* configure the RTC clock source selection */
void rcu_rtc_clock_config(uint32_t rtc_clock_source);
#ifdef GD32F30X_CL
/* configure the I2S1 clock source selection */
void rcu_i2s1_clock_config(uint32_t i2s_clock_source);
/* configure the I2S2 clock source selection */
void rcu_i2s2_clock_config(uint32_t i2s_clock_source);
#endif /* GD32F30X_CL */
/* configure the CK48M clock selection */
void rcu_ck48m_clock_config(uint32_t ck48m_clock_source);


/* get the clock stabilization and periphral reset flags */
FlagStatus rcu_flag_get(rcu_flag_enum flag);
/* clear the reset flag */
void rcu_all_reset_flag_clear(void);
/* get the clock stabilization interrupt and ckm flags */
FlagStatus rcu_interrupt_flag_get(rcu_int_flag_enum int_flag);
/* clear the interrupt flags */
void rcu_interrupt_flag_clear(rcu_int_flag_clear_enum int_flag);
/* enable the stabilization interrupt */
void rcu_interrupt_enable(rcu_int_enum interrupt);
/* disable the stabilization interrupt */
void rcu_interrupt_disable(rcu_int_enum interrupt);

/* LXTAL, IRC8M, PLL and other oscillator configure functions */
/* configure the LXTAL drive capability */
void rcu_lxtal_drive_capability_config(uint32_t lxtal_dricap);
/* wait for oscillator stabilization flags is SET or oscillator startup is timeout */
ErrStatus rcu_osci_stab_wait(rcu_osci_type_enum osci);
/* turn on the oscillator */
void rcu_osci_on(rcu_osci_type_enum osci);
/* turn off the oscillator */
void rcu_osci_off(rcu_osci_type_enum osci);
/* enable the oscillator bypass mode, HXTALEN or LXTALEN must be reset before it */
void rcu_osci_bypass_mode_enable(rcu_osci_type_enum osci);
/* disable the oscillator bypass mode, HXTALEN or LXTALEN must be reset before it */
void rcu_osci_bypass_mode_disable(rcu_osci_type_enum osci);
/* set the IRC8M adjust value */
void rcu_irc8m_adjust_value_set(uint32_t irc8m_adjval);

/* clock monitor configure functions */
/* enable the HXTAL clock monitor */
void rcu_hxtal_clock_monitor_enable(void);
/* disable the HXTAL clock monitor */
void rcu_hxtal_clock_monitor_disable(void);

/* voltage configure and clock frequency get functions */
/* set the deep sleep mode voltage */
void rcu_deepsleep_voltage_set(uint32_t dsvol);
/* get the system clock, bus and peripheral clock frequency */
uint32_t rcu_clock_freq_get(rcu_clock_freq_enum clock);

/* flag & interrupt functions */
/* get the clock stabilization and periphral reset flags */
FlagStatus rcu_flag_get(rcu_flag_enum flag);
/* clear the reset flag */
void rcu_all_reset_flag_clear(void);
/* get the clock stabilization interrupt and ckm flags */
FlagStatus rcu_interrupt_flag_get(rcu_int_flag_enum int_flag);
/* clear the interrupt flags */
void rcu_interrupt_flag_clear(rcu_int_flag_clear_enum int_flag);
/* enable the stabilization interrupt */
void rcu_interrupt_enable(rcu_int_enum interrupt);
/* disable the stabilization interrupt */
void rcu_interrupt_disable(rcu_int_enum interrupt);
#endif /* GD32F30X_RCU_H */
