/**
 * \file
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAM4E8CB_
#define _SAM4E8CB_

/** \addtogroup SAM4E8CB_definitions SAM4E8CB definitions
  This file defines all structures and symbols for SAM4E8CB:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - PIO definitions
*/
/*@{*/

#ifdef __cplusplus
 extern "C" {
#endif 

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#include <stdint.h>
#endif

/* ************************************************************************** */
/*   CMSIS DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */
/** \addtogroup SAM4E8CB_cmsis CMSIS Definitions */
/*@{*/

/**< Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ******************************/
  NonMaskableInt_IRQn   = -14, /**<  2 Non Maskable Interrupt                */
  HardFault_IRQn        = -13, /**<  3 HardFault Interrupt                   */
  MemoryManagement_IRQn = -12, /**<  4 Cortex-M4 Memory Management Interrupt */
  BusFault_IRQn         = -11, /**<  5 Cortex-M4 Bus Fault Interrupt         */
  UsageFault_IRQn       = -10, /**<  6 Cortex-M4 Usage Fault Interrupt       */
  SVCall_IRQn           = -5,  /**< 11 Cortex-M4 SV Call Interrupt           */
  DebugMonitor_IRQn     = -4,  /**< 12 Cortex-M4 Debug Monitor Interrupt     */
  PendSV_IRQn           = -2,  /**< 14 Cortex-M4 Pend SV Interrupt           */
  SysTick_IRQn          = -1,  /**< 15 Cortex-M4 System Tick Interrupt       */
/******  SAM4E8CB specific Interrupt Numbers *********************************/
  
  SUPC_IRQn            =  0, /**<  0 SAM4E8CB Supply Controller (SUPC) */
  RSTC_IRQn            =  1, /**<  1 SAM4E8CB Reset Controller (RSTC) */
  RTC_IRQn             =  2, /**<  2 SAM4E8CB Real Time Clock (RTC) */
  RTT_IRQn             =  3, /**<  3 SAM4E8CB Real Time Timer (RTT) */
  WDT_IRQn             =  4, /**<  4 SAM4E8CB Watchdog/Dual Watchdog Timer (WDT) */
  PMC_IRQn             =  5, /**<  5 SAM4E8CB Power Management Controller (PMC) */
  EFC_IRQn             =  6, /**<  6 SAM4E8CB Enhanced Embedded Flash Controller (EFC) */
  UART0_IRQn           =  7, /**<  7 SAM4E8CB UART 0 (UART0) */
  PIOA_IRQn            =  9, /**<  9 SAM4E8CB Parallel I/O Controller A (PIOA) */
  PIOB_IRQn            = 10, /**< 10 SAM4E8CB Parallel I/O Controller B (PIOB) */
  PIOC_IRQn            = 11, /**< 11 SAM4E8CB Parallel I/O Controller C (PIOC) */
  USART0_IRQn          = 14, /**< 14 SAM4E8CB USART 0 (USART0) */
  USART1_IRQn          = 15, /**< 15 SAM4E8CB USART 1 (USART1) */
  HSMCI_IRQn           = 16, /**< 16 SAM4E8CB Multimedia Card Interface (HSMCI) */
  TWI0_IRQn            = 17, /**< 17 SAM4E8CB Two Wire Interface 0 (TWI0) */
  TWI1_IRQn            = 18, /**< 18 SAM4E8CB Two Wire Interface 1 (TWI1) */
  SPI_IRQn             = 19, /**< 19 SAM4E8CB Serial Peripheral Interface (SPI) */
  DMAC_IRQn            = 20, /**< 20 SAM4E8CB DMAC (DMAC) */
  TC0_IRQn             = 21, /**< 21 SAM4E8CB Timer/Counter 0 (TC0) */
  TC1_IRQn             = 22, /**< 22 SAM4E8CB Timer/Counter 1 (TC1) */
  TC2_IRQn             = 23, /**< 23 SAM4E8CB Timer/Counter 2 (TC2) */
  TC3_IRQn             = 24, /**< 24 SAM4E8CB Timer/Counter 3 (TC3) */
  TC4_IRQn             = 25, /**< 25 SAM4E8CB Timer/Counter 4 (TC4) */
  TC5_IRQn             = 26, /**< 26 SAM4E8CB Timer/Counter 5 (TC5) */
  TC6_IRQn             = 27, /**< 27 SAM4E8CB Timer/Counter 6 (TC6) */
  TC7_IRQn             = 28, /**< 28 SAM4E8CB Timer/Counter 7 (TC7) */
  TC8_IRQn             = 29, /**< 29 SAM4E8CB Timer/Counter 8 (TC8) */
  AFEC0_IRQn           = 30, /**< 30 SAM4E8CB Analog Front End 0 (AFEC0) */
  AFEC1_IRQn           = 31, /**< 31 SAM4E8CB Analog Front End 1 (AFEC1) */
  DACC_IRQn            = 32, /**< 32 SAM4E8CB Digital To Analog Converter (DACC) */
  ACC_IRQn             = 33, /**< 33 SAM4E8CB Analog Comparator (ACC) */
  ARM_IRQn             = 34, /**< 34 SAM4E8CB FPU signals : FPIXC, FPOFC, FPUFC, FPIOC, FPDZC, FPIDC, FPIXC (ARM) */
  UDP_IRQn             = 35, /**< 35 SAM4E8CB USB DEVICE (UDP) */
  PWM_IRQn             = 36, /**< 36 SAM4E8CB PWM (PWM) */
  CAN0_IRQn            = 37, /**< 37 SAM4E8CB CAN0 (CAN0) */
  CAN1_IRQn            = 38, /**< 38 SAM4E8CB CAN1 (CAN1) */
  AES_IRQn             = 39, /**< 39 SAM4E8CB AES (AES) */
  UART1_IRQn           = 45, /**< 45 SAM4E8CB UART (UART1) */

  PERIPH_COUNT_IRQn    = 46  /**< Number of peripheral IDs */
} IRQn_Type;

typedef struct _DeviceVectors
{
  /* Stack pointer */
  void* pvStack;
  
  /* Cortex-M handlers */
  void* pfnReset_Handler;
  void* pfnNMI_Handler;
  void* pfnHardFault_Handler;
  void* pfnMemManage_Handler;
  void* pfnBusFault_Handler;
  void* pfnUsageFault_Handler;
  void* pfnReserved1_Handler;
  void* pfnReserved2_Handler;
  void* pfnReserved3_Handler;
  void* pfnReserved4_Handler;
  void* pfnSVC_Handler;
  void* pfnDebugMon_Handler;
  void* pfnReserved5_Handler;
  void* pfnPendSV_Handler;
  void* pfnSysTick_Handler;

  /* Peripheral handlers */
  void* pfnSUPC_Handler;   /*  0 Supply Controller */
  void* pfnRSTC_Handler;   /*  1 Reset Controller */
  void* pfnRTC_Handler;    /*  2 Real Time Clock */
  void* pfnRTT_Handler;    /*  3 Real Time Timer */
  void* pfnWDT_Handler;    /*  4 Watchdog/Dual Watchdog Timer */
  void* pfnPMC_Handler;    /*  5 Power Management Controller */
  void* pfnEFC_Handler;    /*  6 Enhanced Embedded Flash Controller */
  void* pfnUART0_Handler;  /*  7 UART 0 */
  void* pvReserved8;
  void* pfnPIOA_Handler;   /*  9 Parallel I/O Controller A */
  void* pfnPIOB_Handler;   /* 10 Parallel I/O Controller B */
  void* pfnPIOC_Handler;   /* 11 Parallel I/O Controller C */
  void* pvReserved12;
  void* pvReserved13;
  void* pfnUSART0_Handler; /* 14 USART 0 */
  void* pfnUSART1_Handler; /* 15 USART 1 */
  void* pfnHSMCI_Handler;  /* 16 Multimedia Card Interface */
  void* pfnTWI0_Handler;   /* 17 Two Wire Interface 0 */
  void* pfnTWI1_Handler;   /* 18 Two Wire Interface 1 */
  void* pfnSPI_Handler;    /* 19 Serial Peripheral Interface */
  void* pfnDMAC_Handler;   /* 20 DMAC */
  void* pfnTC0_Handler;    /* 21 Timer/Counter 0 */
  void* pfnTC1_Handler;    /* 22 Timer/Counter 1 */
  void* pfnTC2_Handler;    /* 23 Timer/Counter 2 */
  void* pfnTC3_Handler;    /* 24 Timer/Counter 3 */
  void* pfnTC4_Handler;    /* 25 Timer/Counter 4 */
  void* pfnTC5_Handler;    /* 26 Timer/Counter 5 */
  void* pfnTC6_Handler;    /* 27 Timer/Counter 6 */
  void* pfnTC7_Handler;    /* 28 Timer/Counter 7 */
  void* pfnTC8_Handler;    /* 29 Timer/Counter 8 */
  void* pfnAFEC0_Handler;  /* 30 Analog Front End 0 */
  void* pfnAFEC1_Handler;  /* 31 Analog Front End 1 */
  void* pfnDACC_Handler;   /* 32 Digital To Analog Converter */
  void* pfnACC_Handler;    /* 33 Analog Comparator */
  void* pfnARM_Handler;    /* 34 FPU signals : FPIXC, FPOFC, FPUFC, FPIOC, FPDZC, FPIDC, FPIXC */
  void* pfnUDP_Handler;    /* 35 USB DEVICE */
  void* pfnPWM_Handler;    /* 36 PWM */
  void* pfnCAN0_Handler;   /* 37 CAN0 */
  void* pfnCAN1_Handler;   /* 38 CAN1 */
  void* pfnAES_Handler;    /* 39 AES */
  void* pvReserved40;
  void* pvReserved41;
  void* pvReserved42;
  void* pvReserved43;
  void* pvReserved44;
  void* pfnUART1_Handler;  /* 45 UART */
} DeviceVectors;

/* Cortex-M4 core handlers */
void Reset_Handler      ( void );
void NMI_Handler        ( void );
void HardFault_Handler  ( void );
void MemManage_Handler  ( void );
void BusFault_Handler   ( void );
void UsageFault_Handler ( void );
void SVC_Handler        ( void );
void DebugMon_Handler   ( void );
void PendSV_Handler     ( void );
void SysTick_Handler    ( void );

/* Peripherals handlers */
void ACC_Handler        ( void );
void AES_Handler        ( void );
void AFEC0_Handler      ( void );
void AFEC1_Handler      ( void );
void ARM_Handler        ( void );
void CAN0_Handler       ( void );
void CAN1_Handler       ( void );
void DACC_Handler       ( void );
void DMAC_Handler       ( void );
void EFC_Handler        ( void );
void HSMCI_Handler      ( void );
void PIOA_Handler       ( void );
void PIOB_Handler       ( void );
void PIOC_Handler       ( void );
void PMC_Handler        ( void );
void PWM_Handler        ( void );
void RSTC_Handler       ( void );
void RTC_Handler        ( void );
void RTT_Handler        ( void );
void SPI_Handler        ( void );
void SUPC_Handler       ( void );
void TC0_Handler        ( void );
void TC1_Handler        ( void );
void TC2_Handler        ( void );
void TC3_Handler        ( void );
void TC4_Handler        ( void );
void TC5_Handler        ( void );
void TC6_Handler        ( void );
void TC7_Handler        ( void );
void TC8_Handler        ( void );
void TWI0_Handler       ( void );
void TWI1_Handler       ( void );
void UART0_Handler      ( void );
void UART1_Handler      ( void );
void UDP_Handler        ( void );
void USART0_Handler     ( void );
void USART1_Handler     ( void );
void WDT_Handler        ( void );

/**
 * \brief Configuration of the Cortex-M4 Processor and Core Peripherals 
 */

#define __CM4_REV              0x0001 /**< SAM4E8CB core revision number ([15:8] revision number, [7:0] patch number) */
#define __MPU_PRESENT          1      /**< SAM4E8CB does provide a MPU */
#define __FPU_PRESENT          1      /**< SAM4E8CB does provide a FPU */
#define __NVIC_PRIO_BITS       4      /**< SAM4E8CB uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig 0      /**< Set to 1 if different SysTick Config is used */

/*
 * \brief CMSIS includes
 */

#include <core_cm4.h>
#if !defined DONT_USE_CMSIS_INIT
#include "system_sam4e.h"
#endif /* DONT_USE_CMSIS_INIT */

/*@}*/

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR SAM4E8CB */
/* ************************************************************************** */
/** \addtogroup SAM4E8CB_api Peripheral Software API */
/*@{*/

#include "component/acc.h"
#include "component/aes.h"
#include "component/afec.h"
#include "component/can.h"
#include "component/chipid.h"
#include "component/cmcc.h"
#include "component/dacc.h"
#include "component/dmac.h"
#include "component/efc.h"
#include "component/gpbr.h"
#include "component/hsmci.h"
#include "component/matrix.h"
#include "component/pdc.h"
#include "component/pio.h"
#include "component/pmc.h"
#include "component/pwm.h"
#include "component/rstc.h"
#include "component/rswdt.h"
#include "component/rtc.h"
#include "component/rtt.h"
#include "component/smc.h"
#include "component/spi.h"
#include "component/supc.h"
#include "component/tc.h"
#include "component/twi.h"
#include "component/uart.h"
#include "component/udp.h"
#include "component/usart.h"
#include "component/wdt.h"
/*@}*/

/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */
/** \addtogroup SAM4E8CB_reg Registers Access Definitions */
/*@{*/

#include "instance/pwm.h"
#include "instance/aes.h"
#include "instance/can0.h"
#include "instance/can1.h"
#include "instance/smc.h"
#include "instance/uart1.h"
#include "instance/hsmci.h"
#include "instance/udp.h"
#include "instance/spi.h"
#include "instance/tc0.h"
#include "instance/tc1.h"
#include "instance/tc2.h"
#include "instance/usart0.h"
#include "instance/usart1.h"
#include "instance/twi0.h"
#include "instance/twi1.h"
#include "instance/afec0.h"
#include "instance/afec1.h"
#include "instance/dacc.h"
#include "instance/acc.h"
#include "instance/dmac.h"
#include "instance/cmcc.h"
#include "instance/matrix.h"
#include "instance/pmc.h"
#include "instance/uart0.h"
#include "instance/chipid.h"
#include "instance/efc.h"
#include "instance/pioa.h"
#include "instance/piob.h"
#include "instance/pioc.h"
#include "instance/rstc.h"
#include "instance/supc.h"
#include "instance/rtt.h"
#include "instance/wdt.h"
#include "instance/rtc.h"
#include "instance/gpbr.h"
#include "instance/rswdt.h"
/*@}*/

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */
/** \addtogroup SAM4E8CB_id Peripheral Ids Definitions */
/*@{*/

#define ID_SUPC   ( 0) /**< \brief Supply Controller (SUPC) */
#define ID_RSTC   ( 1) /**< \brief Reset Controller (RSTC) */
#define ID_RTC    ( 2) /**< \brief Real Time Clock (RTC) */
#define ID_RTT    ( 3) /**< \brief Real Time Timer (RTT) */
#define ID_WDT    ( 4) /**< \brief Watchdog/Dual Watchdog Timer (WDT) */
#define ID_PMC    ( 5) /**< \brief Power Management Controller (PMC) */
#define ID_EFC    ( 6) /**< \brief Enhanced Embedded Flash Controller (EFC) */
#define ID_UART0  ( 7) /**< \brief UART 0 (UART0) */
#define ID_SMC    ( 8) /**< \brief Static Memory Controller (SMC) */
#define ID_PIOA   ( 9) /**< \brief Parallel I/O Controller A (PIOA) */
#define ID_PIOB   (10) /**< \brief Parallel I/O Controller B (PIOB) */
#define ID_PIOC   (11) /**< \brief Parallel I/O Controller C (PIOC) */
#define ID_USART0 (14) /**< \brief USART 0 (USART0) */
#define ID_USART1 (15) /**< \brief USART 1 (USART1) */
#define ID_HSMCI  (16) /**< \brief Multimedia Card Interface (HSMCI) */
#define ID_TWI0   (17) /**< \brief Two Wire Interface 0 (TWI0) */
#define ID_TWI1   (18) /**< \brief Two Wire Interface 1 (TWI1) */
#define ID_SPI    (19) /**< \brief Serial Peripheral Interface (SPI) */
#define ID_DMAC   (20) /**< \brief DMAC (DMAC) */
#define ID_TC0    (21) /**< \brief Timer/Counter 0 (TC0) */
#define ID_TC1    (22) /**< \brief Timer/Counter 1 (TC1) */
#define ID_TC2    (23) /**< \brief Timer/Counter 2 (TC2) */
#define ID_TC3    (24) /**< \brief Timer/Counter 3 (TC3) */
#define ID_TC4    (25) /**< \brief Timer/Counter 4 (TC4) */
#define ID_TC5    (26) /**< \brief Timer/Counter 5 (TC5) */
#define ID_TC6    (27) /**< \brief Timer/Counter 6 (TC6) */
#define ID_TC7    (28) /**< \brief Timer/Counter 7 (TC7) */
#define ID_TC8    (29) /**< \brief Timer/Counter 8 (TC8) */
#define ID_AFEC0  (30) /**< \brief Analog Front End 0 (AFEC0) */
#define ID_AFEC1  (31) /**< \brief Analog Front End 1 (AFEC1) */
#define ID_DACC   (32) /**< \brief Digital To Analog Converter (DACC) */
#define ID_ACC    (33) /**< \brief Analog Comparator (ACC) */
#define ID_ARM    (34) /**< \brief FPU signals : FPIXC, FPOFC, FPUFC, FPIOC, FPDZC, FPIDC, FPIXC (ARM) */
#define ID_UDP    (35) /**< \brief USB DEVICE (UDP) */
#define ID_PWM    (36) /**< \brief PWM (PWM) */
#define ID_CAN0   (37) /**< \brief CAN0 (CAN0) */
#define ID_CAN1   (38) /**< \brief CAN1 (CAN1) */
#define ID_AES    (39) /**< \brief AES (AES) */
#define ID_UART1  (45) /**< \brief UART (UART1) */

#define ID_PERIPH_COUNT (46) /**< \brief Number of peripheral IDs */
/*@}*/

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */
/** \addtogroup SAM4E8CB_base Peripheral Base Address Definitions */
/*@{*/

#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define PWM        (0x40000000U) /**< \brief (PWM       ) Base Address */
#define PDC_PWM    (0x40000100U) /**< \brief (PDC_PWM   ) Base Address */
#define AES        (0x40004000U) /**< \brief (AES       ) Base Address */
#define CAN0       (0x40010000U) /**< \brief (CAN0      ) Base Address */
#define CAN1       (0x40014000U) /**< \brief (CAN1      ) Base Address */
#define SMC        (0x40060000U) /**< \brief (SMC       ) Base Address */
#define UART1      (0x40060600U) /**< \brief (UART1     ) Base Address */
#define PDC_UART1  (0x40060700U) /**< \brief (PDC_UART1 ) Base Address */
#define HSMCI      (0x40080000U) /**< \brief (HSMCI     ) Base Address */
#define PDC_HSMCI  (0x40080100U) /**< \brief (PDC_HSMCI ) Base Address */
#define UDP        (0x40084000U) /**< \brief (UDP       ) Base Address */
#define SPI        (0x40088000U) /**< \brief (SPI       ) Base Address */
#define PDC_SPI    (0x40088100U) /**< \brief (PDC_SPI   ) Base Address */
#define TC0        (0x40090000U) /**< \brief (TC0       ) Base Address */
#define PDC_TC0    (0x40090100U) /**< \brief (PDC_TC0   ) Base Address */
#define TC1        (0x40094000U) /**< \brief (TC1       ) Base Address */
#define PDC_TC1    (0x40094100U) /**< \brief (PDC_TC1   ) Base Address */
#define TC2        (0x40098000U) /**< \brief (TC2       ) Base Address */
#define USART0     (0x400A0000U) /**< \brief (USART0    ) Base Address */
#define PDC_USART0 (0x400A0100U) /**< \brief (PDC_USART0) Base Address */
#define USART1     (0x400A4000U) /**< \brief (USART1    ) Base Address */
#define PDC_USART1 (0x400A4100U) /**< \brief (PDC_USART1) Base Address */
#define TWI0       (0x400A8000U) /**< \brief (TWI0      ) Base Address */
#define PDC_TWI0   (0x400A8100U) /**< \brief (PDC_TWI0  ) Base Address */
#define TWI1       (0x400AC000U) /**< \brief (TWI1      ) Base Address */
#define PDC_TWI1   (0x400AC100U) /**< \brief (PDC_TWI1  ) Base Address */
#define AFEC0      (0x400B0000U) /**< \brief (AFEC0     ) Base Address */
#define PDC_AFEC0  (0x400B0100U) /**< \brief (PDC_AFEC0 ) Base Address */
#define AFEC1      (0x400B4000U) /**< \brief (AFEC1     ) Base Address */
#define PDC_AFEC1  (0x400B4100U) /**< \brief (PDC_AFEC1 ) Base Address */
#define DACC       (0x400B8000U) /**< \brief (DACC      ) Base Address */
#define PDC_DACC   (0x400B8100U) /**< \brief (PDC_DACC  ) Base Address */
#define ACC        (0x400BC000U) /**< \brief (ACC       ) Base Address */
#define DMAC       (0x400C0000U) /**< \brief (DMAC      ) Base Address */
#define CMCC       (0x400C4000U) /**< \brief (CMCC      ) Base Address */
#define MATRIX     (0x400E0200U) /**< \brief (MATRIX    ) Base Address */
#define PMC        (0x400E0400U) /**< \brief (PMC       ) Base Address */
#define UART0      (0x400E0600U) /**< \brief (UART0     ) Base Address */
#define PDC_UART0  (0x400E0700U) /**< \brief (PDC_UART0 ) Base Address */
#define CHIPID     (0x400E0740U) /**< \brief (CHIPID    ) Base Address */
#define EFC        (0x400E0A00U) /**< \brief (EFC       ) Base Address */
#define PIOA       (0x400E0E00U) /**< \brief (PIOA      ) Base Address */
#define PDC_PIOA   (0x400E0F68U) /**< \brief (PDC_PIOA  ) Base Address */
#define PIOB       (0x400E1000U) /**< \brief (PIOB      ) Base Address */
#define PIOC       (0x400E1200U) /**< \brief (PIOC      ) Base Address */
#define RSTC       (0x400E1800U) /**< \brief (RSTC      ) Base Address */
#define SUPC       (0x400E1810U) /**< \brief (SUPC      ) Base Address */
#define RTT        (0x400E1830U) /**< \brief (RTT       ) Base Address */
#define WDT        (0x400E1850U) /**< \brief (WDT       ) Base Address */
#define RTC        (0x400E1860U) /**< \brief (RTC       ) Base Address */
#define GPBR       (0x400E1890U) /**< \brief (GPBR      ) Base Address */
#define RSWDT      (0x400E1900U) /**< \brief (RSWDT     ) Base Address */
#else
#define PWM        ((Pwm    *)0x40000000U) /**< \brief (PWM       ) Base Address */
#define PDC_PWM    ((Pdc    *)0x40000100U) /**< \brief (PDC_PWM   ) Base Address */
#define AES        ((Aes    *)0x40004000U) /**< \brief (AES       ) Base Address */
#define CAN0       ((Can    *)0x40010000U) /**< \brief (CAN0      ) Base Address */
#define CAN1       ((Can    *)0x40014000U) /**< \brief (CAN1      ) Base Address */
#define SMC        ((Smc    *)0x40060000U) /**< \brief (SMC       ) Base Address */
#define UART1      ((Uart   *)0x40060600U) /**< \brief (UART1     ) Base Address */
#define PDC_UART1  ((Pdc    *)0x40060700U) /**< \brief (PDC_UART1 ) Base Address */
#define HSMCI      ((Hsmci  *)0x40080000U) /**< \brief (HSMCI     ) Base Address */
#define PDC_HSMCI  ((Pdc    *)0x40080100U) /**< \brief (PDC_HSMCI ) Base Address */
#define UDP        ((Udp    *)0x40084000U) /**< \brief (UDP       ) Base Address */
#define SPI        ((Spi    *)0x40088000U) /**< \brief (SPI       ) Base Address */
#define PDC_SPI    ((Pdc    *)0x40088100U) /**< \brief (PDC_SPI   ) Base Address */
#define TC0        ((Tc     *)0x40090000U) /**< \brief (TC0       ) Base Address */
#define PDC_TC0    ((Pdc    *)0x40090100U) /**< \brief (PDC_TC0   ) Base Address */
#define TC1        ((Tc     *)0x40094000U) /**< \brief (TC1       ) Base Address */
#define PDC_TC1    ((Pdc    *)0x40094100U) /**< \brief (PDC_TC1   ) Base Address */
#define TC2        ((Tc     *)0x40098000U) /**< \brief (TC2       ) Base Address */
#define USART0     ((Usart  *)0x400A0000U) /**< \brief (USART0    ) Base Address */
#define PDC_USART0 ((Pdc    *)0x400A0100U) /**< \brief (PDC_USART0) Base Address */
#define USART1     ((Usart  *)0x400A4000U) /**< \brief (USART1    ) Base Address */
#define PDC_USART1 ((Pdc    *)0x400A4100U) /**< \brief (PDC_USART1) Base Address */
#define TWI0       ((Twi    *)0x400A8000U) /**< \brief (TWI0      ) Base Address */
#define PDC_TWI0   ((Pdc    *)0x400A8100U) /**< \brief (PDC_TWI0  ) Base Address */
#define TWI1       ((Twi    *)0x400AC000U) /**< \brief (TWI1      ) Base Address */
#define PDC_TWI1   ((Pdc    *)0x400AC100U) /**< \brief (PDC_TWI1  ) Base Address */
#define AFEC0      ((Afec   *)0x400B0000U) /**< \brief (AFEC0     ) Base Address */
#define PDC_AFEC0  ((Pdc    *)0x400B0100U) /**< \brief (PDC_AFEC0 ) Base Address */
#define AFEC1      ((Afec   *)0x400B4000U) /**< \brief (AFEC1     ) Base Address */
#define PDC_AFEC1  ((Pdc    *)0x400B4100U) /**< \brief (PDC_AFEC1 ) Base Address */
#define DACC       ((Dacc   *)0x400B8000U) /**< \brief (DACC      ) Base Address */
#define PDC_DACC   ((Pdc    *)0x400B8100U) /**< \brief (PDC_DACC  ) Base Address */
#define ACC        ((Acc    *)0x400BC000U) /**< \brief (ACC       ) Base Address */
#define DMAC       ((Dmac   *)0x400C0000U) /**< \brief (DMAC      ) Base Address */
#define CMCC       ((Cmcc   *)0x400C4000U) /**< \brief (CMCC      ) Base Address */
#define MATRIX     ((Matrix *)0x400E0200U) /**< \brief (MATRIX    ) Base Address */
#define PMC        ((Pmc    *)0x400E0400U) /**< \brief (PMC       ) Base Address */
#define UART0      ((Uart   *)0x400E0600U) /**< \brief (UART0     ) Base Address */
#define PDC_UART0  ((Pdc    *)0x400E0700U) /**< \brief (PDC_UART0 ) Base Address */
#define CHIPID     ((Chipid *)0x400E0740U) /**< \brief (CHIPID    ) Base Address */
#define EFC        ((Efc    *)0x400E0A00U) /**< \brief (EFC       ) Base Address */
#define PIOA       ((Pio    *)0x400E0E00U) /**< \brief (PIOA      ) Base Address */
#define PDC_PIOA   ((Pdc    *)0x400E0F68U) /**< \brief (PDC_PIOA  ) Base Address */
#define PIOB       ((Pio    *)0x400E1000U) /**< \brief (PIOB      ) Base Address */
#define PIOC       ((Pio    *)0x400E1200U) /**< \brief (PIOC      ) Base Address */
#define RSTC       ((Rstc   *)0x400E1800U) /**< \brief (RSTC      ) Base Address */
#define SUPC       ((Supc   *)0x400E1810U) /**< \brief (SUPC      ) Base Address */
#define RTT        ((Rtt    *)0x400E1830U) /**< \brief (RTT       ) Base Address */
#define WDT        ((Wdt    *)0x400E1850U) /**< \brief (WDT       ) Base Address */
#define RTC        ((Rtc    *)0x400E1860U) /**< \brief (RTC       ) Base Address */
#define GPBR       ((Gpbr   *)0x400E1890U) /**< \brief (GPBR      ) Base Address */
#define RSWDT      ((Rswdt  *)0x400E1900U) /**< \brief (RSWDT     ) Base Address */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/*@}*/

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */
/** \addtogroup SAM4E8CB_pio Peripheral Pio Definitions */
/*@{*/

#include "pio/sam4e8cb.h"
/*@}*/

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */

#define IFLASH_SIZE             (0x80000u)
#define IFLASH_PAGE_SIZE        (512u)
#define IFLASH_LOCK_REGION_SIZE (8192u)
#define IFLASH_NB_OF_PAGES      (1024u)
#define IFLASH_NB_OF_LOCK_BITS  (64u)
#define IRAM_SIZE               (0x20000u)

#define IFLASH_ADDR  (0x00400000u) /**< Internal Flash base address */
#define IROM_ADDR    (0x00800000u) /**< Internal ROM base address */
#define IRAM_ADDR    (0x20000000u) /**< Internal RAM base address */
#define EBI_CS0_ADDR (0x60000000u) /**< EBI Chip Select 0 base address */
#define EBI_CS1_ADDR (0x61000000u) /**< EBI Chip Select 1 base address */
#define EBI_CS2_ADDR (0x62000000u) /**< EBI Chip Select 2 base address */
#define EBI_CS3_ADDR (0x63000000u) /**< EBI Chip Select 3 base address */

/* ************************************************************************** */
/*   MISCELLANEOUS DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */

#define CHIP_JTAGID       (0x05B3703FUL)
#define CHIP_CIDR         (0xA3CC0CE1UL)
#define CHIP_EXID         (0x00110308UL)
#define NB_CH_AFE0        (6UL)
#define NB_CH_AFE1        (4UL)
#define NB_CH_DAC         (2UL)
#define USB_DEVICE_MAX_EP (8UL)

/* ************************************************************************** */
/*   ELECTRICAL DEFINITIONS FOR SAM4E8CB */
/* ************************************************************************** */

/* Device characteristics */
#define CHIP_FREQ_SLCK_RC_MIN           (20000UL)
#define CHIP_FREQ_SLCK_RC               (32000UL)
#define CHIP_FREQ_SLCK_RC_MAX           (44000UL)
#define CHIP_FREQ_MAINCK_RC_4MHZ        (4000000UL)
#define CHIP_FREQ_MAINCK_RC_8MHZ        (8000000UL)
#define CHIP_FREQ_MAINCK_RC_12MHZ       (12000000UL)
#define CHIP_FREQ_CPU_MAX               (120000000UL)
#define CHIP_FREQ_XTAL_32K              (32768UL)
#define CHIP_FREQ_XTAL_12M              (12000000UL)

/* Embedded Flash Write Wait State */
#define CHIP_FLASH_WRITE_WAIT_STATE     (6U)

/* Embedded Flash Read Wait State (VDDCORE set at 1.20V) */
#define CHIP_FREQ_FWS_0                 (20000000UL)  /**< \brief Maximum operating frequency when FWS is 0 */
#define CHIP_FREQ_FWS_1                 (40000000UL)  /**< \brief Maximum operating frequency when FWS is 1 */
#define CHIP_FREQ_FWS_2                 (60000000UL)  /**< \brief Maximum operating frequency when FWS is 2 */
#define CHIP_FREQ_FWS_3                 (80000000UL)  /**< \brief Maximum operating frequency when FWS is 3 */
#define CHIP_FREQ_FWS_4                 (100000000UL) /**< \brief Maximum operating frequency when FWS is 4 */
#define CHIP_FREQ_FWS_5                 (123000000UL) /**< \brief Maximum operating frequency when FWS is 5 */

/* HYSTeresis levels: please refer to Electrical Characteristics */
#define ACC_ACR_HYST_50MV_MAX	          (0x01UL)
#define ACC_ACR_HYST_90MV_MAX           (0x11UL)

/* Device characteristics */
#define CHIP_FREQ_SLCK_RC_MIN           (20000UL)
#define CHIP_FREQ_SLCK_RC               (32000UL)
#define CHIP_FREQ_SLCK_RC_MAX           (44000UL)
#define CHIP_FREQ_MAINCK_RC_4MHZ        (4000000UL)
#define CHIP_FREQ_MAINCK_RC_8MHZ        (8000000UL)
#define CHIP_FREQ_MAINCK_RC_12MHZ       (12000000UL)
#define CHIP_FREQ_CPU_MAX               (120000000UL)
#define CHIP_FREQ_XTAL_32K              (32768UL)
#define CHIP_FREQ_XTAL_12M              (12000000UL)

/* Embedded Flash Read Wait State (VDDCORE set at 1.20V) */
#define CHIP_FREQ_FWS_0                 (20000000UL)  /**< \brief Maximum operating frequency when FWS is 0 */
#define CHIP_FREQ_FWS_1                 (40000000UL)  /**< \brief Maximum operating frequency when FWS is 1 */
#define CHIP_FREQ_FWS_2                 (60000000UL)  /**< \brief Maximum operating frequency when FWS is 2 */
#define CHIP_FREQ_FWS_3                 (80000000UL)  /**< \brief Maximum operating frequency when FWS is 3 */
#define CHIP_FREQ_FWS_4                 (100000000UL) /**< \brief Maximum operating frequency when FWS is 4 */
#define CHIP_FREQ_FWS_5                 (123000000UL) /**< \brief Maximum operating frequency when FWS is 5 */

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* _SAM4E8CB_ */
