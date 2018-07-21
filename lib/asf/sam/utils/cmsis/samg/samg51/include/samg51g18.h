/**
 * \file
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAMG51G18_
#define _SAMG51G18_

/** \addtogroup SAMG51G18_definitions SAMG51G18 definitions
  This file defines all structures and symbols for SAMG51G18:
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
/*   CMSIS DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */
/** \addtogroup SAMG51G18_cmsis CMSIS Definitions */
/*@{*/

/**< Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ******************************/
  NonMaskableInt_IRQn   = -14, /**<  2 Non Maskable Interrupt                */
  MemoryManagement_IRQn = -12, /**<  4 Cortex-M4 Memory Management Interrupt */
  BusFault_IRQn         = -11, /**<  5 Cortex-M4 Bus Fault Interrupt         */
  UsageFault_IRQn       = -10, /**<  6 Cortex-M4 Usage Fault Interrupt       */
  SVCall_IRQn           = -5,  /**< 11 Cortex-M4 SV Call Interrupt           */
  DebugMonitor_IRQn     = -4,  /**< 12 Cortex-M4 Debug Monitor Interrupt     */
  PendSV_IRQn           = -2,  /**< 14 Cortex-M4 Pend SV Interrupt           */
  SysTick_IRQn          = -1,  /**< 15 Cortex-M4 System Tick Interrupt       */
/******  SAMG51G18 specific Interrupt Numbers *********************************/

  SUPC_IRQn            =  0, /**<  0 SAMG51G18 Supply Controller (SUPC) */
  RSTC_IRQn            =  1, /**<  1 SAMG51G18 Reset Controller (RSTC) */
  RTC_IRQn             =  2, /**<  2 SAMG51G18 Real Time Clock (RTC) */
  RTT_IRQn             =  3, /**<  3 SAMG51G18 Real Time Timer (RTT) */
  WDT_IRQn             =  4, /**<  4 SAMG51G18 Watchdog Timer (WDT) */
  PMC_IRQn             =  5, /**<  5 SAMG51G18 Power Management Controller (PMC) */
  EFC_IRQn             =  6, /**<  6 SAMG51G18 Enhanced Flash Controller (EFC) */
  UART0_IRQn           =  8, /**<  8 SAMG51G18 UART 0 (UART0) */
  UART1_IRQn           =  9, /**<  9 SAMG51G18 UART 1 (UART1) */
  PIOA_IRQn            = 11, /**< 11 SAMG51G18 Parallel I/O Controller A (PIOA) */
  PIOB_IRQn            = 12, /**< 12 SAMG51G18 Parallel I/O Controller B (PIOB) */
  USART_IRQn           = 14, /**< 14 SAMG51G18 USART (USART) */
  MEM2MEM_IRQn         = 15, /**< 15 SAMG51G18 MEM2MEM (MEM2MEM) */
  TWI0_IRQn            = 19, /**< 19 SAMG51G18 Two Wire Interface 0 HS (TWI0) */
  TWI1_IRQn            = 20, /**< 20 SAMG51G18 Two Wire Interface 1 (TWI1) */
  SPI_IRQn             = 21, /**< 21 SAMG51G18 Serial Peripheral Interface (SPI) */
  TWI2_IRQn            = 22, /**< 22 SAMG51G18 Two Wire Interface 2 (TWI2) */
  TC0_IRQn             = 23, /**< 23 SAMG51G18 Timer/Counter 0 (TC0) */
  TC1_IRQn             = 24, /**< 24 SAMG51G18 Timer/Counter 1 (TC1) */
  TC2_IRQn             = 25, /**< 25 SAMG51G18 Timer/Counter 2 (TC2) */
  ADC_IRQn             = 29, /**< 29 SAMG51G18 Analog To Digital Converter (ADC) */
  ARM_IRQn             = 30, /**< 30 SAMG51G18 FPU (ARM) */
  WKUP0_IRQn           = 31, /**< 31 SAMG51G18 External interrupt 0 (WKUP0) */
  WKUP1_IRQn           = 32, /**< 31 SAMG51G18 External interrupt 1 (WKUP1) */
  WKUP2_IRQn           = 33, /**< 31 SAMG51G18 External interrupt 2 (WKUP2) */
  WKUP3_IRQn           = 34, /**< 31 SAMG51G18 External interrupt 3 (WKUP3) */
  WKUP4_IRQn           = 35, /**< 31 SAMG51G18 External interrupt 4 (WKUP4) */
  WKUP5_IRQn           = 36, /**< 31 SAMG51G18 External interrupt 5 (WKUP5) */
  WKUP6_IRQn           = 37, /**< 31 SAMG51G18 External interrupt 6 (WKUP6) */
  WKUP7_IRQn           = 38, /**< 31 SAMG51G18 External interrupt 7 (WKUP7) */
  WKUP8_IRQn           = 39, /**< 31 SAMG51G18 External interrupt 8 (WKUP8) */
  WKUP9_IRQn           = 40, /**< 31 SAMG51G18 External interrupt 9 (WKUP9) */
  WKUP10_IRQn          = 41, /**< 31 SAMG51G18 External interrupt 10 (WKUP10) */
  WKUP11_IRQn          = 42, /**< 31 SAMG51G18 External interrupt 11 (WKUP11) */
  WKUP12_IRQn          = 43, /**< 31 SAMG51G18 External interrupt 12 (WKUP12) */
  WKUP13_IRQn          = 44, /**< 31 SAMG51G18 External interrupt 13 (WKUP13) */
  WKUP14_IRQn          = 45, /**< 31 SAMG51G18 External interrupt 14 (WKUP14) */
  WKUP15_IRQn          = 46, /**< 31 SAMG51G18 External interrupt 15 (WKUP15) */

  PERIPH_COUNT_IRQn    = 47  /**< Number of peripheral IDs */
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
  void* pfnSUPC_Handler;  /*  0 Supply Controller */
  void* pfnRSTC_Handler;  /*  1 Reset Controller */
  void* pfnRTC_Handler;   /*  2 Real Time Clock */
  void* pfnRTT_Handler;   /*  3 Real Time Timer */
  void* pfnWDT_Handler;   /*  4 Watchdog Timer */
  void* pfnPMC_Handler;   /*  5 Power Management Controller */
  void* pfnEFC_Handler;   /*  6 Enhanced Flash Controller */
  void* pvReserved7;
  void* pfnUART0_Handler; /*  8 UART 0 */
  void* pfnUART1_Handler; /*  9 UART 1 */
  void* pvReserved10;
  void* pfnPIOA_Handler;  /* 11 Parallel I/O Controller A */
  void* pfnPIOB_Handler;  /* 12 Parallel I/O Controller B */
  void* pvReserved13;
  void* pfnUSART_Handler; /* 14 USART */
  void* pfnMEM2MEM_Handler; /* 15 MEM2MEM */
  void* pvReserved16;
  void* pvReserved17;
  void* pvReserved18;
  void* pfnTWI0_Handler;  /* 19 Two Wire Interface 0 HS */
  void* pfnTWI1_Handler;  /* 20 Two Wire Interface 1 */
  void* pfnSPI_Handler;   /* 21 Serial Peripheral Interface */
  void* pfnTWI2_Handler;  /* 22 Two Wire Interface 2 */
  void* pfnTC0_Handler;   /* 23 Timer/Counter 0 */
  void* pfnTC1_Handler;   /* 24 Timer/Counter 1 */
  void* pfnTC2_Handler;   /* 25 Timer/Counter 2 */
  void* pvReserved26;
  void* pvReserved27;
  void* pvReserved28;
  void* pfnADC_Handler;   /* 29 Analog To Digital Converter */
  void* pfnARM_Handler;   /* 30 FPU */
  void* pfnWKUP0_Handler;
  void* pfnWKUP1_Handler;
  void* pfnWKUP2_Handler;
  void* pfnWKUP3_Handler;
  void* pfnWKUP4_Handler;
  void* pfnWKUP5_Handler;
  void* pfnWKUP6_Handler;
  void* pfnWKUP7_Handler;
  void* pfnWKUP8_Handler;
  void* pfnWKUP9_Handler;
  void* pfnWKUP10_Handler;
  void* pfnWKUP11_Handler;
  void* pfnWKUP12_Handler;
  void* pfnWKUP13_Handler;
  void* pfnWKUP14_Handler;
  void* pfnWKUP15_Handler;
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
void ADC_Handler        ( void );
void ARM_Handler        ( void );
void EFC_Handler        ( void );
void MEM2MEM_Handler    ( void );
void PIOA_Handler       ( void );
void PIOB_Handler       ( void );
void PMC_Handler        ( void );
void RSTC_Handler       ( void );
void RTC_Handler        ( void );
void RTT_Handler        ( void );
void SPI_Handler        ( void );
void SUPC_Handler       ( void );
void TC0_Handler        ( void );
void TC1_Handler        ( void );
void TC2_Handler        ( void );
void TWI0_Handler       ( void );
void TWI1_Handler       ( void );
void TWI2_Handler       ( void );
void UART0_Handler      ( void );
void UART1_Handler      ( void );
void USART_Handler      ( void );
void WDT_Handler        ( void );
void WKUP0_Handler      ( void );
void WKUP1_Handler      ( void );
void WKUP2_Handler      ( void );
void WKUP3_Handler      ( void );
void WKUP4_Handler      ( void );
void WKUP5_Handler      ( void );
void WKUP6_Handler      ( void );
void WKUP7_Handler      ( void );
void WKUP8_Handler      ( void );
void WKUP9_Handler      ( void );
void WKUP10_Handler     ( void );
void WKUP11_Handler     ( void );
void WKUP12_Handler     ( void );
void WKUP13_Handler     ( void );
void WKUP14_Handler     ( void );
void WKUP15_Handler     ( void );

/**
 * \brief Configuration of the Cortex-M4 Processor and Core Peripherals
 */

#define __CM4_REV              0x0001 /**< SAMG51G18 core revision number ([15:8] revision number, [7:0] patch number) */
#define __MPU_PRESENT          1      /**< SAMG51G18 does provide a MPU */
#define __FPU_PRESENT          1      /**< SAMG51G18 does provide a FPU */
#define __NVIC_PRIO_BITS       4      /**< SAMG51G18 uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig 0      /**< Set to 1 if different SysTick Config is used */

/*
 * \brief CMSIS includes
 */

#include <core_cm4.h>
#if !defined DONT_USE_CMSIS_INIT
#include "system_samg51.h"
#endif /* DONT_USE_CMSIS_INIT */

/*@}*/

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR SAMG51G18 */
/* ************************************************************************** */
/** \addtogroup SAMG51G18_api Peripheral Software API */
/*@{*/

#include "component/component_adc.h"
#include "component/component_chipid.h"
#include "component/component_efc.h"
#include "component/component_gpbr.h"
#include "component/component_matrix.h"
#include "component/component_mem2mem.h"
#include "component/component_pdc.h"
#include "component/component_pio.h"
#include "component/component_pmc.h"
#include "component/component_rstc.h"
#include "component/component_rtc.h"
#include "component/component_rtt.h"
#include "component/component_spi.h"
#include "component/component_supc.h"
#include "component/component_tc.h"
#include "component/component_twi.h"
#include "component/component_twihs.h"
#include "component/component_uart.h"
#include "component/component_usart.h"
#include "component/component_wdt.h"
/*@}*/

/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */
/** \addtogroup SAMG51G18_reg Registers Access Definitions */
/*@{*/

#include "instance/instance_spi.h"
#include "instance/instance_tc0.h"
#include "instance/instance_twi0.h"
#include "instance/instance_twi1.h"
#include "instance/instance_usart.h"
#include "instance/instance_mem2mem.h"
#include "instance/instance_adc.h"
#include "instance/instance_twi2.h"
#include "instance/instance_matrix.h"
#include "instance/instance_pmc.h"
#include "instance/instance_uart0.h"
#include "instance/instance_chipid.h"
#include "instance/instance_uart1.h"
#include "instance/instance_efc.h"
#include "instance/instance_pioa.h"
#include "instance/instance_piob.h"
#include "instance/instance_rstc.h"
#include "instance/instance_supc.h"
#include "instance/instance_rtt.h"
#include "instance/instance_wdt.h"
#include "instance/instance_rtc.h"
#include "instance/instance_gpbr.h"
/*@}*/

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */
/** \addtogroup SAMG51G18_id Peripheral Ids Definitions */
/*@{*/

#define ID_SUPC    ( 0) /**< \brief Supply Controller (SUPC) */
#define ID_RSTC    ( 1) /**< \brief Reset Controller (RSTC) */
#define ID_RTC     ( 2) /**< \brief Real Time Clock (RTC) */
#define ID_RTT     ( 3) /**< \brief Real Time Timer (RTT) */
#define ID_WDT     ( 4) /**< \brief Watchdog Timer (WDT) */
#define ID_PMC     ( 5) /**< \brief Power Management Controller (PMC) */
#define ID_EFC     ( 6) /**< \brief Enhanced Flash Controller (EFC) */
#define ID_UART0   ( 8) /**< \brief UART 0 (UART0) */
#define ID_UART1   ( 9) /**< \brief UART 1 (UART1) */
#define ID_PIOA    (11) /**< \brief Parallel I/O Controller A (PIOA) */
#define ID_PIOB    (12) /**< \brief Parallel I/O Controller B (PIOB) */
#define ID_USART   (14) /**< \brief USART (USART) */
#define ID_MEM2MEM (15) /**< \brief MEM2MEM (MEM2MEM) */
#define ID_TWI0    (19) /**< \brief Two Wire Interface 0 HS (TWI0) */
#define ID_TWI1    (20) /**< \brief Two Wire Interface 1 (TWI1) */
#define ID_SPI     (21) /**< \brief Serial Peripheral Interface (SPI) */
#define ID_TWI2    (22) /**< \brief Two Wire Interface 2 (TWI2) */
#define ID_TC0     (23) /**< \brief Timer/Counter 0 (TC0) */
#define ID_TC1     (24) /**< \brief Timer/Counter 1 (TC1) */
#define ID_TC2     (25) /**< \brief Timer/Counter 2 (TC2) */
#define ID_ADC     (29) /**< \brief Analog To Digital Converter (ADC) */
#define ID_ARM     (30) /**< \brief FPU (ARM) */

#define ID_PERIPH_COUNT (31) /**< \brief Number of peripheral IDs */
/*@}*/

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */
/** \addtogroup SAMG51G18_base Peripheral Base Address Definitions */
/*@{*/

#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define SPI         (0x40008000U) /**< \brief (SPI        ) Base Address */
#define PDC_SPI     (0x40008100U) /**< \brief (PDC_SPI    ) Base Address */
#define TC0         (0x40010000U) /**< \brief (TC0        ) Base Address */
#define PDC_TC0     (0x40010100U) /**< \brief (PDC_TC0    ) Base Address */
#define TWI0        (0x40018000U) /**< \brief (TWI0       ) Base Address */
#define PDC_TWI0    (0x40018100U) /**< \brief (PDC_TWI0   ) Base Address */
#define TWI1        (0x4001C000U) /**< \brief (TWI1       ) Base Address */
#define PDC_TWI1    (0x4001C100U) /**< \brief (PDC_TWI1   ) Base Address */
#define USART       (0x40024000U) /**< \brief (USART      ) Base Address */
#define PDC_USART   (0x40024100U) /**< \brief (PDC_USART  ) Base Address */
#define MEM2MEM     (0x40028000U) /**< \brief (MEM2MEM    ) Base Address */
#define PDC_MEM2MEM (0x40028100U) /**< \brief (PDC_MEM2MEM) Base Address */
#define ADC         (0x40038000U) /**< \brief (ADC        ) Base Address */
#define PDC_ADC     (0x40038100U) /**< \brief (PDC_ADC    ) Base Address */
#define TWI2        (0x40040000U) /**< \brief (TWI2       ) Base Address */
#define PDC_TWI2    (0x40040100U) /**< \brief (PDC_TWI2   ) Base Address */
#define MATRIX      (0x400E0200U) /**< \brief (MATRIX     ) Base Address */
#define PMC         (0x400E0400U) /**< \brief (PMC        ) Base Address */
#define UART0       (0x400E0600U) /**< \brief (UART0      ) Base Address */
#define PDC_UART0   (0x400E0700U) /**< \brief (PDC_UART0  ) Base Address */
#define CHIPID      (0x400E0740U) /**< \brief (CHIPID     ) Base Address */
#define UART1       (0x400E0800U) /**< \brief (UART1      ) Base Address */
#define PDC_UART1   (0x400E0900U) /**< \brief (PDC_UART1  ) Base Address */
#define EFC         (0x400E0A00U) /**< \brief (EFC        ) Base Address */
#define PIOA        (0x400E0E00U) /**< \brief (PIOA       ) Base Address */
#define PIOB        (0x400E1000U) /**< \brief (PIOB       ) Base Address */
#define RSTC        (0x400E1400U) /**< \brief (RSTC       ) Base Address */
#define SUPC        (0x400E1410U) /**< \brief (SUPC       ) Base Address */
#define RTT         (0x400E1430U) /**< \brief (RTT        ) Base Address */
#define WDT         (0x400E1450U) /**< \brief (WDT        ) Base Address */
#define RTC         (0x400E1460U) /**< \brief (RTC        ) Base Address */
#define GPBR        (0x400E1490U) /**< \brief (GPBR       ) Base Address */
#else
#define SPI         ((Spi     *)0x40008000U) /**< \brief (SPI        ) Base Address */
#define PDC_SPI     ((Pdc     *)0x40008100U) /**< \brief (PDC_SPI    ) Base Address */
#define TC0         ((Tc      *)0x40010000U) /**< \brief (TC0        ) Base Address */
#define PDC_TC0     ((Pdc     *)0x40010100U) /**< \brief (PDC_TC0    ) Base Address */
#define TWI0        ((Twihs   *)0x40018000U) /**< \brief (TWI0       ) Base Address */
#define PDC_TWI0    ((Pdc     *)0x40018100U) /**< \brief (PDC_TWI0   ) Base Address */
#define TWI1        ((Twi     *)0x4001C000U) /**< \brief (TWI1       ) Base Address */
#define PDC_TWI1    ((Pdc     *)0x4001C100U) /**< \brief (PDC_TWI1   ) Base Address */
#define USART       ((Usart   *)0x40024000U) /**< \brief (USART      ) Base Address */
#define PDC_USART   ((Pdc     *)0x40024100U) /**< \brief (PDC_USART  ) Base Address */
#define MEM2MEM     ((Mem2mem *)0x40028000U) /**< \brief (MEM2MEM    ) Base Address */
#define PDC_MEM2MEM ((Pdc     *)0x40028100U) /**< \brief (PDC_MEM2MEM) Base Address */
#define ADC         ((Adc     *)0x40038000U) /**< \brief (ADC        ) Base Address */
#define PDC_ADC     ((Pdc     *)0x40038100U) /**< \brief (PDC_ADC    ) Base Address */
#define TWI2        ((Twi     *)0x40040000U) /**< \brief (TWI2       ) Base Address */
#define PDC_TWI2    ((Pdc     *)0x40040100U) /**< \brief (PDC_TWI2   ) Base Address */
#define MATRIX      ((Matrix  *)0x400E0200U) /**< \brief (MATRIX     ) Base Address */
#define PMC         ((Pmc     *)0x400E0400U) /**< \brief (PMC        ) Base Address */
#define UART0       ((Uart    *)0x400E0600U) /**< \brief (UART0      ) Base Address */
#define PDC_UART0   ((Pdc     *)0x400E0700U) /**< \brief (PDC_UART0  ) Base Address */
#define CHIPID      ((Chipid  *)0x400E0740U) /**< \brief (CHIPID     ) Base Address */
#define UART1       ((Uart    *)0x400E0800U) /**< \brief (UART1      ) Base Address */
#define PDC_UART1   ((Pdc     *)0x400E0900U) /**< \brief (PDC_UART1  ) Base Address */
#define EFC         ((Efc     *)0x400E0A00U) /**< \brief (EFC        ) Base Address */
#define PIOA        ((Pio     *)0x400E0E00U) /**< \brief (PIOA       ) Base Address */
#define PIOB        ((Pio     *)0x400E1000U) /**< \brief (PIOB       ) Base Address */
#define RSTC        ((Rstc    *)0x400E1400U) /**< \brief (RSTC       ) Base Address */
#define SUPC        ((Supc    *)0x400E1410U) /**< \brief (SUPC       ) Base Address */
#define RTT         ((Rtt     *)0x400E1430U) /**< \brief (RTT        ) Base Address */
#define WDT         ((Wdt     *)0x400E1450U) /**< \brief (WDT        ) Base Address */
#define RTC         ((Rtc     *)0x400E1460U) /**< \brief (RTC        ) Base Address */
#define GPBR        ((Gpbr    *)0x400E1490U) /**< \brief (GPBR       ) Base Address */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/*@}*/

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */
/** \addtogroup SAMG51G18_pio Peripheral Pio Definitions */
/*@{*/

#include "pio/pio_samg51g18.h"
/*@}*/

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */

#define IFLASH_SIZE             (0x40000u)
#define IFLASH_PAGE_SIZE        (512u)
#define IFLASH_LOCK_REGION_SIZE (8192u)
#define IFLASH_NB_OF_PAGES      (512u)
#define IFLASH_NB_OF_LOCK_BITS  (32u)
#define IRAM_SIZE               (0x10000u)

#define IFLASH_ADDR (0x00400000u) /**< Internal Flash base address */
#define IROM_ADDR   (0x00800000u) /**< Internal ROM base address */
#define IRAM_ADDR   (0x20000000u) /**< Internal RAM base address */

/* ************************************************************************** */
/*   MISCELLANEOUS DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */

#define CHIP_JTAGID (0x05B3A03FUL)
#define CHIP_CIDR   (0x243B09E0UL)
#define CHIP_EXID   (0x0UL)
#define NB_CH_ADC   (8UL)

/* ************************************************************************** */
/*   ELECTRICAL DEFINITIONS FOR SAMG51G18 */
/* ************************************************************************** */

/* Device characteristics */
#define CHIP_FREQ_SLCK_RC_MIN           (20000UL)
#define CHIP_FREQ_SLCK_RC               (32000UL)
#define CHIP_FREQ_SLCK_RC_MAX           (44000UL)
#define CHIP_FREQ_MAINCK_RC_8MHZ        (8000000UL)
#define CHIP_FREQ_MAINCK_RC_16MHZ       (16000000UL)
#define CHIP_FREQ_MAINCK_RC_24MHZ       (24000000UL)
#define CHIP_FREQ_CPU_MAX               (48000000UL)
#define CHIP_FREQ_XTAL_32K              (32768UL)


/* Embedded Flash Write Wait State */
#define CHIP_FLASH_WRITE_WAIT_STATE     (6U)

/* Embedded Flash Read Wait State (VDDIO set at 1.8V, @ 25°C) */
#define CHIP_FREQ_FWS_0                 (15000000UL)  /**< \brief Maximum operating frequency when FWS is 0 */
#define CHIP_FREQ_FWS_1                 (30000000UL)  /**< \brief Maximum operating frequency when FWS is 1 */
#define CHIP_FREQ_FWS_2                 (45000000UL)  /**< \brief Maximum operating frequency when FWS is 2 */
#define CHIP_FREQ_FWS_3                 (60000000UL)  /**< \brief Maximum operating frequency when FWS is 3 */

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* _SAMG51G18_ */
