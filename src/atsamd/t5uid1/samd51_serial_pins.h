#ifndef __ATSAMD_T5UID1_SAMD51_SERIAL_PINS_H
#define __ATSAMD_T5UID1_SAMD51_SERIAL_PINS_H

#include "autoconf.h"
#include "../internal.h"

// RX definitions
#if CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA01
  #if CONFIG_ATSAMD51_SERIAL_RX_PA01 || CONFIG_ATSAMD51_SERIAL_TX_PA01
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 1)
  #define GPIO_Rx_NAME  "PA1"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA05
  #if CONFIG_ATSAMD51_SERIAL_RX_PA05 || CONFIG_ATSAMD51_SERIAL_TX_PA05
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 5)
  #define GPIO_Rx_NAME  "PA5"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA06
  #if CONFIG_ATSAMD51_SERIAL_RX_PA06 || CONFIG_ATSAMD51_SERIAL_TX_PA06
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 6)
  #define GPIO_Rx_NAME  "PA6"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA07
  #if CONFIG_ATSAMD51_SERIAL_RX_PA07 || CONFIG_ATSAMD51_SERIAL_TX_PA07
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 7)
  #define GPIO_Rx_NAME  "PA7"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA08
  #if CONFIG_ATSAMD51_SERIAL_RX_PA08 || CONFIG_ATSAMD51_SERIAL_TX_PA08
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 8)
  #define GPIO_Rx_NAME  "PA8"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA09
  #if CONFIG_ATSAMD51_SERIAL_RX_PA09 || CONFIG_ATSAMD51_SERIAL_TX_PA09
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 9)
  #define GPIO_Rx_NAME  "PA9"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA10
  #if CONFIG_ATSAMD51_SERIAL_RX_PA10 || CONFIG_ATSAMD51_SERIAL_TX_PA10
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 10)
  #define GPIO_Rx_NAME  "PA10"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA11
  #if CONFIG_ATSAMD51_SERIAL_RX_PA11 || CONFIG_ATSAMD51_SERIAL_TX_PA11
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 11)
  #define GPIO_Rx_NAME  "PA11"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA12
  #if CONFIG_ATSAMD51_SERIAL_RX_PA12 || CONFIG_ATSAMD51_SERIAL_TX_PA12
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 12)
  #define GPIO_Rx_NAME  "PA12"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA13
  #if CONFIG_ATSAMD51_SERIAL_RX_PA13 || CONFIG_ATSAMD51_SERIAL_TX_PA13
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 13)
  #define GPIO_Rx_NAME  "PA13"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA14
  #if CONFIG_ATSAMD51_SERIAL_RX_PA14 || CONFIG_ATSAMD51_SERIAL_TX_PA14
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 14)
  #define GPIO_Rx_NAME  "PA14"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA15
  #if CONFIG_ATSAMD51_SERIAL_RX_PA15 || CONFIG_ATSAMD51_SERIAL_TX_PA15
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 15)
  #define GPIO_Rx_NAME  "PA15"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA16
  #if CONFIG_ATSAMD51_SERIAL_RX_PA16 || CONFIG_ATSAMD51_SERIAL_TX_PA16
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 16)
  #define GPIO_Rx_NAME  "PA16"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA17
  #if CONFIG_ATSAMD51_SERIAL_RX_PA17 || CONFIG_ATSAMD51_SERIAL_TX_PA17
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 17)
  #define GPIO_Rx_NAME  "PA17"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA18
  #if CONFIG_ATSAMD51_SERIAL_RX_PA18 || CONFIG_ATSAMD51_SERIAL_TX_PA18
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 18)
  #define GPIO_Rx_NAME  "PA18"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA19
  #if CONFIG_ATSAMD51_SERIAL_RX_PA19 || CONFIG_ATSAMD51_SERIAL_TX_PA19
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 19)
  #define GPIO_Rx_NAME  "PA19"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA20
  #if CONFIG_ATSAMD51_SERIAL_RX_PA20 || CONFIG_ATSAMD51_SERIAL_TX_PA20
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 20)
  #define GPIO_Rx_NAME  "PA20"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA21
  #if CONFIG_ATSAMD51_SERIAL_RX_PA21 || CONFIG_ATSAMD51_SERIAL_TX_PA21
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 21)
  #define GPIO_Rx_NAME  "PA21"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA22
  #if CONFIG_ATSAMD51_SERIAL_RX_PA22 || CONFIG_ATSAMD51_SERIAL_TX_PA22
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 22)
  #define GPIO_Rx_NAME  "PA22"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA23
  #if CONFIG_ATSAMD51_SERIAL_RX_PA23 || CONFIG_ATSAMD51_SERIAL_TX_PA23
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 23)
  #define GPIO_Rx_NAME  "PA23"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA24
  #if CONFIG_ATSAMD51_SERIAL_RX_PA24 || CONFIG_ATSAMD51_SERIAL_TX_PA24 \
      || CONFIG_USBSERIAL
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 24)
  #define GPIO_Rx_NAME  "PA24"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA25
  #if CONFIG_ATSAMD51_SERIAL_RX_PA25 || CONFIG_ATSAMD51_SERIAL_TX_PA25 \
      || CONFIG_USBSERIAL
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 25)
  #define GPIO_Rx_NAME  "PA25"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA30
  #if CONFIG_ATSAMD51_SERIAL_RX_PA30 || CONFIG_ATSAMD51_SERIAL_TX_PA30
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 30)
  #define GPIO_Rx_NAME  "PA30"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PA31
  #if CONFIG_ATSAMD51_SERIAL_RX_PA31 || CONFIG_ATSAMD51_SERIAL_TX_PA31
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('A', 31)
  #define GPIO_Rx_NAME  "PA31"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB00
  #if CONFIG_ATSAMD51_SERIAL_RX_PB00 || CONFIG_ATSAMD51_SERIAL_TX_PB00
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 0)
  #define GPIO_Rx_NAME  "PB0"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB01
  #if CONFIG_ATSAMD51_SERIAL_RX_PB01 || CONFIG_ATSAMD51_SERIAL_TX_PB01
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 1)
  #define GPIO_Rx_NAME  "PB1"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB03
  #if CONFIG_ATSAMD51_SERIAL_RX_PB03 || CONFIG_ATSAMD51_SERIAL_TX_PB03
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 3)
  #define GPIO_Rx_NAME  "PB3"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB09
  #if CONFIG_ATSAMD51_SERIAL_RX_PB09 || CONFIG_ATSAMD51_SERIAL_TX_PB09
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 9)
  #define GPIO_Rx_NAME  "PB9"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB10
  #if CONFIG_ATSAMD51_SERIAL_RX_PB10 || CONFIG_ATSAMD51_SERIAL_TX_PB10
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 10)
  #define GPIO_Rx_NAME  "PB10"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB11
  #if CONFIG_ATSAMD51_SERIAL_RX_PB11 || CONFIG_ATSAMD51_SERIAL_TX_PB11
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 11)
  #define GPIO_Rx_NAME  "PB11"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB13
  #if CONFIG_ATSAMD51_SERIAL_RX_PB13 || CONFIG_ATSAMD51_SERIAL_TX_PB13
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 13)
  #define GPIO_Rx_NAME  "PB13"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB14
  #if CONFIG_ATSAMD51_SERIAL_RX_PB14 || CONFIG_ATSAMD51_SERIAL_TX_PB14
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 14)
  #define GPIO_Rx_NAME  "PB14"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB15
  #if CONFIG_ATSAMD51_SERIAL_RX_PB15 || CONFIG_ATSAMD51_SERIAL_TX_PB15
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 15)
  #define GPIO_Rx_NAME  "PB15"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB17
  #if CONFIG_ATSAMD51_SERIAL_RX_PB17 || CONFIG_ATSAMD51_SERIAL_TX_PB17
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 17)
  #define GPIO_Rx_NAME  "PB17"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB18
  #if CONFIG_ATSAMD51_SERIAL_RX_PB18 || CONFIG_ATSAMD51_SERIAL_TX_PB18
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 18)
  #define GPIO_Rx_NAME  "PB18"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB19
  #if CONFIG_ATSAMD51_SERIAL_RX_PB19 || CONFIG_ATSAMD51_SERIAL_TX_PB19
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 19)
  #define GPIO_Rx_NAME  "PB19"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB20
  #if CONFIG_ATSAMD51_SERIAL_RX_PB20 || CONFIG_ATSAMD51_SERIAL_TX_PB20
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 20)
  #define GPIO_Rx_NAME  "PB20"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB21
  #if CONFIG_ATSAMD51_SERIAL_RX_PB21 || CONFIG_ATSAMD51_SERIAL_TX_PB21
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 21)
  #define GPIO_Rx_NAME  "PB21"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB22
  #if CONFIG_ATSAMD51_SERIAL_RX_PB22 || CONFIG_ATSAMD51_SERIAL_TX_PB22
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 22)
  #define GPIO_Rx_NAME  "PB22"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB23
  #if CONFIG_ATSAMD51_SERIAL_RX_PB23 || CONFIG_ATSAMD51_SERIAL_TX_PB23
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 23)
  #define GPIO_Rx_NAME  "PB23"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB24
  #if CONFIG_ATSAMD51_SERIAL_RX_PB24 || CONFIG_ATSAMD51_SERIAL_TX_PB24
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 24)
  #define GPIO_Rx_NAME  "PB24"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB25
  #if CONFIG_ATSAMD51_SERIAL_RX_PB25 || CONFIG_ATSAMD51_SERIAL_TX_PB25
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 25)
  #define GPIO_Rx_NAME  "PB25"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB26
  #if CONFIG_ATSAMD51_SERIAL_RX_PB26 || CONFIG_ATSAMD51_SERIAL_TX_PB26
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 26)
  #define GPIO_Rx_NAME  "PB26"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB27
  #if CONFIG_ATSAMD51_SERIAL_RX_PB27 || CONFIG_ATSAMD51_SERIAL_TX_PB27
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 27)
  #define GPIO_Rx_NAME  "PB27"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB28
  #if CONFIG_ATSAMD51_SERIAL_RX_PB28 || CONFIG_ATSAMD51_SERIAL_TX_PB28
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 28)
  #define GPIO_Rx_NAME  "PB28"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB29
  #if CONFIG_ATSAMD51_SERIAL_RX_PB29 || CONFIG_ATSAMD51_SERIAL_TX_PB29
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 29)
  #define GPIO_Rx_NAME  "PB29"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB30
  #if CONFIG_ATSAMD51_SERIAL_RX_PB30 || CONFIG_ATSAMD51_SERIAL_TX_PB30
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 30)
  #define GPIO_Rx_NAME  "PB30"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PB31
  #if CONFIG_ATSAMD51_SERIAL_RX_PB31 || CONFIG_ATSAMD51_SERIAL_TX_PB31
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('B', 31)
  #define GPIO_Rx_NAME  "PB31"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC05
  #if CONFIG_ATSAMD51_SERIAL_RX_PC05 || CONFIG_ATSAMD51_SERIAL_TX_PC05
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 5)
  #define GPIO_Rx_NAME  "PC5"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC06
  #if CONFIG_ATSAMD51_SERIAL_RX_PC06 || CONFIG_ATSAMD51_SERIAL_TX_PC06
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 6)
  #define GPIO_Rx_NAME  "PC6"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC07
  #if CONFIG_ATSAMD51_SERIAL_RX_PC07 || CONFIG_ATSAMD51_SERIAL_TX_PC07
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 7)
  #define GPIO_Rx_NAME  "PC7"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC10
  #if CONFIG_ATSAMD51_SERIAL_RX_PC10 || CONFIG_ATSAMD51_SERIAL_TX_PC10
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 10)
  #define GPIO_Rx_NAME  "PC10"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC11
  #if CONFIG_ATSAMD51_SERIAL_RX_PC11 || CONFIG_ATSAMD51_SERIAL_TX_PC11
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 11)
  #define GPIO_Rx_NAME  "PC11"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC12
  #if CONFIG_ATSAMD51_SERIAL_RX_PC12 || CONFIG_ATSAMD51_SERIAL_TX_PC12
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 12)
  #define GPIO_Rx_NAME  "PC12"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC13
  #if CONFIG_ATSAMD51_SERIAL_RX_PC13 || CONFIG_ATSAMD51_SERIAL_TX_PC13
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 13)
  #define GPIO_Rx_NAME  "PC13"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC14
  #if CONFIG_ATSAMD51_SERIAL_RX_PC14 || CONFIG_ATSAMD51_SERIAL_TX_PC14
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 14)
  #define GPIO_Rx_NAME  "PC14"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC15
  #if CONFIG_ATSAMD51_SERIAL_RX_PC15 || CONFIG_ATSAMD51_SERIAL_TX_PC15
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 15)
  #define GPIO_Rx_NAME  "PC15"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC16
  #if CONFIG_ATSAMD51_SERIAL_RX_PC16 || CONFIG_ATSAMD51_SERIAL_TX_PC16
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 16)
  #define GPIO_Rx_NAME  "PC16"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC17
  #if CONFIG_ATSAMD51_SERIAL_RX_PC17 || CONFIG_ATSAMD51_SERIAL_TX_PC17
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 17)
  #define GPIO_Rx_NAME  "PC17"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC18
  #if CONFIG_ATSAMD51_SERIAL_RX_PC18 || CONFIG_ATSAMD51_SERIAL_TX_PC18
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 18)
  #define GPIO_Rx_NAME  "PC18"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC19
  #if CONFIG_ATSAMD51_SERIAL_RX_PC19 || CONFIG_ATSAMD51_SERIAL_TX_PC19
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 19)
  #define GPIO_Rx_NAME  "PC19"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC22
  #if CONFIG_ATSAMD51_SERIAL_RX_PC22 || CONFIG_ATSAMD51_SERIAL_TX_PC22
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 22)
  #define GPIO_Rx_NAME  "PC22"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC23
  #if CONFIG_ATSAMD51_SERIAL_RX_PC23 || CONFIG_ATSAMD51_SERIAL_TX_PC23
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 23)
  #define GPIO_Rx_NAME  "PC23"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC24
  #if CONFIG_ATSAMD51_SERIAL_RX_PC24 || CONFIG_ATSAMD51_SERIAL_TX_PC24
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 24)
  #define GPIO_Rx_NAME  "PC24"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC25
  #if CONFIG_ATSAMD51_SERIAL_RX_PC25 || CONFIG_ATSAMD51_SERIAL_TX_PC25
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 25)
  #define GPIO_Rx_NAME  "PC25"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PC28
  #if CONFIG_ATSAMD51_SERIAL_RX_PC28 || CONFIG_ATSAMD51_SERIAL_TX_PC28
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('C', 28)
  #define GPIO_Rx_NAME  "PC28"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PD08
  #if CONFIG_ATSAMD51_SERIAL_RX_PD08 || CONFIG_ATSAMD51_SERIAL_TX_PD08
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('D', 8)
  #define GPIO_Rx_NAME  "PD8"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PD09
  #if CONFIG_ATSAMD51_SERIAL_RX_PD09 || CONFIG_ATSAMD51_SERIAL_TX_PD09
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('D', 9)
  #define GPIO_Rx_NAME  "PD9"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PD10
  #if CONFIG_ATSAMD51_SERIAL_RX_PD10 || CONFIG_ATSAMD51_SERIAL_TX_PD10
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('D', 10)
  #define GPIO_Rx_NAME  "PD10"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PD11
  #if CONFIG_ATSAMD51_SERIAL_RX_PD11 || CONFIG_ATSAMD51_SERIAL_TX_PD11
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('D', 11)
  #define GPIO_Rx_NAME  "PD11"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PD20
  #if CONFIG_ATSAMD51_SERIAL_RX_PD20 || CONFIG_ATSAMD51_SERIAL_TX_PD20
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('D', 20)
  #define GPIO_Rx_NAME  "PD20"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_RX_PD21
  #if CONFIG_ATSAMD51_SERIAL_RX_PD21 || CONFIG_ATSAMD51_SERIAL_TX_PD21
    #error "The RX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Rx       GPIO('D', 21)
  #define GPIO_Rx_NAME  "PD21"
#endif

#if CONFIG_ATSAMD_T5UID1_SERIAL_RX_ALT
  #define GPIO_Rx_FUNC  'D'
#else
  #define GPIO_Rx_FUNC  'C'
#endif

#if CONFIG_ATSAMD_T5UID1_SERIAL_RX_PAD1
  #define GPIO_Rx_PINOUT  0x1
#elif CONFIG_ATSAMD_T5UID1_SERIAL_RX_PAD2
  #define GPIO_Rx_PINOUT  0x2
#elif CONFIG_ATSAMD_T5UID1_SERIAL_RX_PAD3
  #define GPIO_Rx_PINOUT  0x3
#endif

// TX definitions
#if CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA00
  #if CONFIG_ATSAMD51_SERIAL_RX_PA00 || CONFIG_ATSAMD51_SERIAL_TX_PA00
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 0)
  #define GPIO_Tx_NAME  "PA0"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA04
  #if CONFIG_ATSAMD51_SERIAL_RX_PA04 || CONFIG_ATSAMD51_SERIAL_TX_PA04
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 4)
  #define GPIO_Tx_NAME  "PA4"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA08
  #if CONFIG_ATSAMD51_SERIAL_RX_PA08 || CONFIG_ATSAMD51_SERIAL_TX_PA08
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 8)
  #define GPIO_Tx_NAME  "PA8"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA09
  #if CONFIG_ATSAMD51_SERIAL_RX_PA09 || CONFIG_ATSAMD51_SERIAL_TX_PA09
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 9)
  #define GPIO_Tx_NAME  "PA9"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA12
  #if CONFIG_ATSAMD51_SERIAL_RX_PA12 || CONFIG_ATSAMD51_SERIAL_TX_PA12
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 12)
  #define GPIO_Tx_NAME  "PA12"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA13
  #if CONFIG_ATSAMD51_SERIAL_RX_PA13 || CONFIG_ATSAMD51_SERIAL_TX_PA13
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 13)
  #define GPIO_Tx_NAME  "PA13"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA16
  #if CONFIG_ATSAMD51_SERIAL_RX_PA16 || CONFIG_ATSAMD51_SERIAL_TX_PA16
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 16)
  #define GPIO_Tx_NAME  "PA16"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA17
  #if CONFIG_ATSAMD51_SERIAL_RX_PA17 || CONFIG_ATSAMD51_SERIAL_TX_PA17
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 17)
  #define GPIO_Tx_NAME  "PA17"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA22
  #if CONFIG_ATSAMD51_SERIAL_RX_PA22 || CONFIG_ATSAMD51_SERIAL_TX_PA22
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 22)
  #define GPIO_Tx_NAME  "PA22"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PA23
  #if CONFIG_ATSAMD51_SERIAL_RX_PA23 || CONFIG_ATSAMD51_SERIAL_TX_PA23
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('A', 23)
  #define GPIO_Tx_NAME  "PA23"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB02
  #if CONFIG_ATSAMD51_SERIAL_RX_PB02 || CONFIG_ATSAMD51_SERIAL_TX_PB02
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 2)
  #define GPIO_Tx_NAME  "PB2"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB08
  #if CONFIG_ATSAMD51_SERIAL_RX_PB08 || CONFIG_ATSAMD51_SERIAL_TX_PB08
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 8)
  #define GPIO_Tx_NAME  "PB8"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB12
  #if CONFIG_ATSAMD51_SERIAL_RX_PB12 || CONFIG_ATSAMD51_SERIAL_TX_PB12
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 12)
  #define GPIO_Tx_NAME  "PB12"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB16
  #if CONFIG_ATSAMD51_SERIAL_RX_PB16 || CONFIG_ATSAMD51_SERIAL_TX_PB16
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 16)
  #define GPIO_Tx_NAME  "PB16"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB20
  #if CONFIG_ATSAMD51_SERIAL_RX_PB20 || CONFIG_ATSAMD51_SERIAL_TX_PB20
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 20)
  #define GPIO_Tx_NAME  "PB20"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB21
  #if CONFIG_ATSAMD51_SERIAL_RX_PB21 || CONFIG_ATSAMD51_SERIAL_TX_PB21
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 21)
  #define GPIO_Tx_NAME  "PB21"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB24
  #if CONFIG_ATSAMD51_SERIAL_RX_PB24 || CONFIG_ATSAMD51_SERIAL_TX_PB24
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 24)
  #define GPIO_Tx_NAME  "PB24"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB25
  #if CONFIG_ATSAMD51_SERIAL_RX_PB25 || CONFIG_ATSAMD51_SERIAL_TX_PB25
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 25)
  #define GPIO_Tx_NAME  "PB25"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB26
  #if CONFIG_ATSAMD51_SERIAL_RX_PB26 || CONFIG_ATSAMD51_SERIAL_TX_PB26
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 26)
  #define GPIO_Tx_NAME  "PB26"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB27
  #if CONFIG_ATSAMD51_SERIAL_RX_PB27 || CONFIG_ATSAMD51_SERIAL_TX_PB27
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 27)
  #define GPIO_Tx_NAME  "PB27"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB30
  #if CONFIG_ATSAMD51_SERIAL_RX_PB30 || CONFIG_ATSAMD51_SERIAL_TX_PB30
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 30)
  #define GPIO_Tx_NAME  "PB30"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PB31
  #if CONFIG_ATSAMD51_SERIAL_RX_PB31 || CONFIG_ATSAMD51_SERIAL_TX_PB31
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('B', 31)
  #define GPIO_Tx_NAME  "PB31"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC04
  #if CONFIG_ATSAMD51_SERIAL_RX_PC04 || CONFIG_ATSAMD51_SERIAL_TX_PC04
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 4)
  #define GPIO_Tx_NAME  "PC4"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC12
  #if CONFIG_ATSAMD51_SERIAL_RX_PC12 || CONFIG_ATSAMD51_SERIAL_TX_PC12
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 12)
  #define GPIO_Tx_NAME  "PC12"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC13
  #if CONFIG_ATSAMD51_SERIAL_RX_PC13 || CONFIG_ATSAMD51_SERIAL_TX_PC13
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 13)
  #define GPIO_Tx_NAME  "PC13"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC16
  #if CONFIG_ATSAMD51_SERIAL_RX_PC16 || CONFIG_ATSAMD51_SERIAL_TX_PC16
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 16)
  #define GPIO_Tx_NAME  "PC16"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC17
  #if CONFIG_ATSAMD51_SERIAL_RX_PC17 || CONFIG_ATSAMD51_SERIAL_TX_PC17
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 17)
  #define GPIO_Tx_NAME  "PC17"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC22
  #if CONFIG_ATSAMD51_SERIAL_RX_PC22 || CONFIG_ATSAMD51_SERIAL_TX_PC22
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 22)
  #define GPIO_Tx_NAME  "PC22"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC23
  #if CONFIG_ATSAMD51_SERIAL_RX_PC23 || CONFIG_ATSAMD51_SERIAL_TX_PC23
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 23)
  #define GPIO_Tx_NAME  "PC23"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PC27
  #if CONFIG_ATSAMD51_SERIAL_RX_PC27 || CONFIG_ATSAMD51_SERIAL_TX_PC27
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('C', 27)
  #define GPIO_Tx_NAME  "PC27"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PD08
  #if CONFIG_ATSAMD51_SERIAL_RX_PD08 || CONFIG_ATSAMD51_SERIAL_TX_PD08
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('D', 8)
  #define GPIO_Tx_NAME  "PD8"
#elif CONFIG_ATSAMD51_T5UID1_SERIAL_TX_PD09
  #if CONFIG_ATSAMD51_SERIAL_RX_PD09 || CONFIG_ATSAMD51_SERIAL_TX_PD09
    #error "The TX pin selected for the T5UID1 screen is already used"
  #endif

  #define GPIO_Tx       GPIO('D', 9)
  #define GPIO_Tx_NAME  "PD9"
#endif

#if CONFIG_ATSAMD_T5UID1_SERIAL_TX_ALT
  #define GPIO_Tx_FUNC  'D'
#else
  #define GPIO_Tx_FUNC  'C'
#endif

#define GPIO_Tx_PINOUT  0x0

#endif // samd51_serial_pins.h
