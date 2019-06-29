/* This file is the part of the Lightweight USB device Stack for STM32 microcontrollers
 *
 * Copyright ©2016 Dmitry Filimonchuk <dmitrystu[at]gmail[dot]com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(STM32L052xx) || defined(STM32L053xx) || \
    defined(STM32L062xx) || defined(STM32L063xx) || \
    defined(STM32L072xx) || defined(STM32L073xx) || \
    defined(STM32L082xx) || defined(STM32L083xx) || \
    defined(STM32F042x6) || defined(STM32F048xx) || \
    defined(STM32F070x6) || defined(STM32F070xB) || \
    defined(STM32F072xB) || defined(STM32F078xx) \

    #define USB_EPBASE      0x40005C00
    #define USB_REGBASE     0x40005C40
    #define USB_CNTR        0x00
    #define USB_ISTR        0x04
    #define USB_FNR         0x08
    #define USB_DADDR       0x0C
    #define USB_BTABLE      0x10
    #define USB_BCDR        0x18
    #define USB_PMABASE     0x40006000
    #define RCC_BASE        0x40021000
    #define RCC_USBEN       23
    #if defined(STM32F042x6) || defined(STM32F048xx) || \
        defined(STM32F070x6) || defined(STM32F070xB) || \
        defined(STM32F072xB) || defined(STM32F078xx)
        #define RCC_APB1RSTR    0x10
        #define RCC_APB1ENR     0x1C
        #define UID_BASE        0x1FFFF7AC
    #else
        #define RCC_APB1RSTR    0x28
        #define RCC_APB1ENR     0x38
        #define UID_BASE        0x1FF80050
    #endif

#elif defined(STM32L432xx) || defined(STM32L433xx) || \
      defined(STM32L442xx) || defined(STM32L443xx) || \
      defined(STM32L452xx) || defined(STM32L462xx)
    #define USB_EPBASE      0x40006800
    #define USB_REGBASE     0x40006840
    #define USB_CNTR        0x00
    #define USB_ISTR        0x04
    #define USB_FNR         0x08
    #define USB_DADDR       0x0C
    #define USB_BTABLE      0x10
    #define USB_BCDR        0x18
    #define USB_PMABASE     0x40006C00
    #define RCC_BASE        0x40021000
    #define RCC_APB1RSTR    0x38
    #define RCC_APB1ENR     0x58
    #define UID_BASE        0x1FFF7590
    #define RCC_USBEN       26

#elif defined(STM32L1)
/* common definitions for STM31L100xx STM32L151xx STM32L152xx STM32L162xx */
    #define USB_EPBASE      0x40005C00
    #define USB_REGBASE     0x40005C40
    #define USB_CNTR        0x00
    #define USB_ISTR        0x04
    #define USB_FNR         0x08
    #define USB_DADDR       0x0C
    #define USB_BTABLE      0x10
    #define USB_PMABASE     0x40006000
    #define SYSCFG_BASE     0x40010000
    #define SYSCFG_PMC      0x04
    #define RCC_BASE        0x40023800
    #define RCC_APB1ENR     0x24
    #define RCC_APB1RSTR    0x18
    #define RCC_APB2ENR     0x20
    #if defined(STM32L100xB) || defined(STM32L100xBA) || \
        defined(STM32L151xB) || defined(STM32L151xBA)
    /* Cat.1 and Cat.2 devices */
        #define UID_BASE        0x1FF80050
    #else
        #define UID_BASE        0x1FF800D0
    #endif
#elif defined(STM32F102x6) || defined(STM32F102xB) || \
      defined(STM32F103x6) || defined(STM32F103xB) || \
      defined(STM32F103xE) || defined(STM32F103xG)
    #define USB_EPBASE      0x40005C00
    #define USB_REGBASE     0x40005C40
    #define USB_CNTR        0x00
    #define USB_ISTR        0x04
    #define USB_FNR         0x08
    #define USB_DADDR       0x0C
    #define USB_BTABLE      0x10
    #define USB_PMABASE     0x40006000
    #define RCC_BASE        0x40021000
    #define RCC_APB1RSTR    0x10
    #define RCC_APB2ENR     0x18
    #define RCC_APB1ENR     0x1C
    #define RCC_GPIOAEN     0x02
    #define RCC_GPIOBEN     0x03
    #define RCC_GPIOCEN     0x04
    #define RCC_GPIODEN     0x05
    #define RCC_GPIOEEN     0x06
    #define RCC_GPIOFEN     0x07

    #define UID_BASE        0x1FFFF7E8
    #define GPIOA           0x40010800
    #define GPIOB           0x40010C00
    #define GPIOC           0x40011000
    #define GPIOD           0x40011400
    #define GPIOE           0x40011800
    #define GPIOF           0x40011C00
    #define GPIOG           0x40012000
    #define GPIO_CRL        0x00
    #define GPIO_CRH        0x04
    #define GPIO_IDR        0x08
    #define GPIO_BSRR       0x10


#elif defined(STM32F303x8) || defined(STM32F303xC) || \
      defined(STM32F303xE)

    #define USB_EPBASE      0x40005C00
    #define USB_REGBASE     0x40005C40
    #define USB_CNTR        0x00
    #define USB_ISTR        0x04
    #define USB_FNR         0x08
    #define USB_DADDR       0x0C
    #define USB_BTABLE      0x10
    #define USB_PMABASE     0x40006000

    #define RCC_BASE        0x40021000
    #define RCC_APB1ENR     0x1C
    #define RCC_APB1RSTR    0x10
    #define RCC_AHBENR      0x14
    #define RCC_GPIOAEN     0x11
    #define RCC_GPIOBEN     0x12
    #define RCC_GPIOCEN     0x13
    #define RCC_GPIODEN     0x14
    #define RCC_GPIOEEN     0x15
    #define RCC_GPIOFEN     0x16
    #define RCC_GPIOGEN     0x17
    #define RCC_GPIOHEN     0x10

    #define UID_BASE        0x1FFFF7AC

    #define GPIOA           0x48000000
    #define GPIOB           0x48000400
    #define GPIOC           0x48000800
    #define GPIOD           0x48000C00
    #define GPIOE           0x48001000
    #define GPIOF           0x48001400
    #define GPIO_MODER      0x00
    #define GPIO_BSRR       0x18

#else
    #error Unsupported MCU
#endif


