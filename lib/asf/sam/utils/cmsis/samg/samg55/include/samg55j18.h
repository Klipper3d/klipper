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

#ifndef _SAMG55J18_
#define _SAMG55J18_

/** \addtogroup SAMG55J18_definitions SAMG55J18 definitions
  This file defines all structures and symbols for SAMG55J18:
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
/*   CMSIS DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */
/** \addtogroup SAMG55J18_cmsis CMSIS Definitions */
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
/******  SAMG55J18 specific Interrupt Numbers *********************************/
  
  SUPC_IRQn            =  0, /**<  0 SAMG55J18 Supply Controller (SUPC) */
  RSTC_IRQn            =  1, /**<  1 SAMG55J18 Reset Controller (RSTC) */
  RTC_IRQn             =  2, /**<  2 SAMG55J18 Real Time Clock (RTC) */
  RTT_IRQn             =  3, /**<  3 SAMG55J18 Real Time Timer (RTT) */
  WDT_IRQn             =  4, /**<  4 SAMG55J18 Watchdog Timer (WDT) */
  PMC_IRQn             =  5, /**<  5 SAMG55J18 Power Management Controller (PMC) */
  EFC_IRQn             =  6, /**<  6 SAMG55J18 Enhanced Flash Controller (EFC) */
	USART7_IRQn = 7, /**< 7 SAMG55J18 FLEXCOM 7 (USART7) */
	SPI7_IRQn =  7, /**<  7 SAMG55J18 FLEXCOM 7 (SPI7) */
	TWI7_IRQn = 7, /**<  7 SAMG55J18 FLEXCOM 7 (TWI7) */
	USART0_IRQn =  8, /**<  8 SAMG55J18 FLEXCOM 0 (USART0) */
	SPI0_IRQn =  8, /**<  8 SAMG55J18 FLEXCOM 0 (SPI0) */
	TWI0_IRQn = 8, /**<  8 SAMG55J18 FLEXCOM 0 (TWI0) */
  USART1_IRQn =  9, /**<  9 SAMG55J18 FLEXCOM 1 (USART1) */
	SPI1_IRQn =  9, /**<  8 SAMG55J18 FLEXCOM 1 (SPI1) */
	TWI1_IRQn = 9, /**<  8 SAMG55J18 FLEXCOM 1 (TWI1) */
  PIOA_IRQn            = 11, /**< 11 SAMG55J18 Parallel I/O Controller A (PIOA) */
  PIOB_IRQn            = 12, /**< 12 SAMG55J18 Parallel I/O Controller B (PIOB) */
  PDMIC0_IRQn          = 13, /**< 13 SAMG55J18 PDM 0 (PDMIC0) */
	 USART2_IRQn = 14, /**< 14 SAMG55J18 FLEXCOM2 (USART2) */
	SPI2_IRQn =  14, /**<  14 SAMG55J18 FLEXCOM 2 (SPI2) */
	TWI2_IRQn = 14, /**< 14 SAMG55J18 FLEXCOM 2 (TWI2) */
  MEM2MEM_IRQn         = 15, /**< 15 SAMG55J18 MEM2MEM (MEM2MEM) */
  I2SC0_IRQn           = 16, /**< 16 SAMG55J18 I2SC0 (I2SC0) */
  I2SC1_IRQn           = 17, /**< 17 SAMG55J18 I2SC1 (I2SC1) */
  PDMIC1_IRQn          = 18, /**< 18 SAMG55J18 PDM 1 (PDMIC1) */
	USART3__IRQn = 19, /**< 19 SAMG55J18 FLEXCOM3 (USART3) */
	SPI3_IRQn =  19, /**<  19 SAMG55J18 FLEXCOM 3 (SPI3) */
	TWI3_IRQn = 19, /**<  19 SAMG55J18 FLEXCOM 3 (TWI3) */
  USART4_IRQn = 20, /**< 20 SAMG55J18 FLEXCOM4 (USART4) */
	SPI4_IRQn =  20, /**<  8 SAMG55J18 FLEXCOM 4 (SPI4) */
	TWI4_IRQn = 20, /**<  8 SAMG55J18 FLEXCOM 4 (TWI4) */
  USART5_IRQn = 21, /**< 21 SAMG55J18 FLEXCOM5 (USART5) */
	SPI5_IRQn =  21, /**<  8 SAMG55J18 FLEXCOM 5 (SPI5) */
	TWI5_IRQn = 21, /**<  8 SAMG55J18 FLEXCOM 5 (TWI5) */
  USART6_IRQn = 22, /**< 22 SAMG55J18 FLEXCOM6 (USART6) */
	SPI6_IRQn =  22, /**<  22 SAMG55J18 FLEXCOM 6 (SPI6) */
	TWI6_IRQn = 22, /**<  22 SAMG55J18 FLEXCOM 6 (TWI6) */
  TC0_IRQn             = 23, /**< 23 SAMG55J18 Timer/Counter 0 (TC0) */
  TC1_IRQn             = 24, /**< 24 SAMG55J18 Timer/Counter 1 (TC1) */
  TC2_IRQn             = 25, /**< 25 SAMG55J18 Timer/Counter 2 (TC2) */
  TC3_IRQn             = 26, /**< 26 SAMG55J18 Timer/Counter 3 (TC3) */
  TC4_IRQn             = 27, /**< 27 SAMG55J18 Timer/Counter 4 (TC4) */
  TC5_IRQn             = 28, /**< 28 SAMG55J18 Timer/Counter 5 (TC5) */
  ADC_IRQn             = 29, /**< 29 SAMG55J18 Analog To Digital Converter (ADC) */
  ARM_IRQn             = 30, /**< 30 SAMG55J18 FPU (ARM) */
	WKUP0_IRQn           = 31, /**< 31 SAMG55J18 External interrupt 0 (WKUP0) */
  WKUP1_IRQn           = 32, /**< 32 SAMG55J18 External interrupt 1 (WKUP1) */
  WKUP2_IRQn           = 33, /**< 33 SAMG55J18 External interrupt 2 (WKUP2) */
  WKUP3_IRQn           = 34, /**< 34 SAMG55J18 External interrupt 3 (WKUP3) */
  WKUP4_IRQn           = 35, /**< 35 SAMG55J18 External interrupt 4 (WKUP4) */
  WKUP5_IRQn           = 36, /**< 36 SAMG55J18 External interrupt 5 (WKUP5) */
  WKUP6_IRQn           = 37, /**< 37 SAMG55J18 External interrupt 6 (WKUP6) */
  WKUP7_IRQn           = 38, /**< 38 SAMG55J18 External interrupt 7 (WKUP7) */
  WKUP8_IRQn           = 39, /**< 39 SAMG55J18 External interrupt 8 (WKUP8) */
  WKUP9_IRQn           = 40, /**< 40 SAMG55J18 External interrupt 9 (WKUP9) */
  WKUP10_IRQn          = 41, /**< 41 SAMG55J18 External interrupt 10 (WKUP10) */
  WKUP11_IRQn          = 42, /**< 42 SAMG55J18 External interrupt 11 (WKUP11) */
  WKUP12_IRQn          = 43, /**< 43 SAMG55J18 External interrupt 12 (WKUP12) */
  WKUP13_IRQn          = 44, /**< 44 SAMG55J18 External interrupt 13 (WKUP13) */
  WKUP14_IRQn          = 45, /**< 45 SAMG55J18 External interrupt 14 (WKUP14) */
  WKUP15_IRQn          = 46, /**< 46 SAMG55J18 External interrupt 15 (WKUP15) */
  UHP_IRQn             = 47, /**< 47 SAMG55J18 USB OHCI (UHP) */
  UDP_IRQn             = 48, /**< 48 SAMG55J18 USB Device FS (UDP) */

  PERIPH_COUNT_IRQn    = 49  /**< Number of peripheral IDs */
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
  void* pfnSUPC_Handler;    /*  0 Supply Controller */
  void* pfnRSTC_Handler;    /*  1 Reset Controller */
  void* pfnRTC_Handler;     /*  2 Real Time Clock */
  void* pfnRTT_Handler;     /*  3 Real Time Timer */
  void* pfnWDT_Handler;     /*  4 Watchdog Timer */
  void* pfnPMC_Handler;     /*  5 Power Management Controller */
  void* pfnEFC_Handler;     /*  6 Enhanced Flash Controller */
  void* pfnUSART7_Handler;       /* USART7 */
  void* pfnSPI7_Handler;       /* SPI7 */
  void* pfnTWI7_Handler;       /* TWI7 */
  void* pfnUSART0_Handler;  /* 8  FLEXCOM 0 */
  void* pfnSPI0_Handler;    /* 8  FLEXCOM 0 */
  void* pfnTWI0_Handler;    /* 8  FLEXCOM 0 */
  void* pfnUSART1_Handler;  /* 9  FLEXCOM 1 */
  void* pfnSPI1_Handler;    /* 9  FLEXCOM 1 */
  void* pfnTWI1_Handler;    /* 9  FLEXCOM 1 */
  void* pvReserved10;
  void* pfnPIOA_Handler;    /* 11 Parallel I/O Controller A */
  void* pfnPIOB_Handler;    /* 12 Parallel I/O Controller B */
  void* pfnPDMIC0_Handler;  /* 13 PDM 0 */
  void* pfnUSART2_Handler;  /* 14  FLEXCOM 2 */
  void* pfnSPI2_Handler;    /* 14  FLEXCOM 2 */
  void* pfnTWI2_Handler;    /* 14  FLEXCOM 2 */
  void* pfnMEM2MEM_Handler; /* 15 MEM2MEM */
  void* pfnI2SC0_Handler;   /* 16 I2SC0 */
  void* pfnI2SC1_Handler;   /* 17 I2SC1 */
  void* pfnPDMIC1_Handler;  /* 18 PDM 1 */
  void* pfnUSART3_Handler;  /* 19  FLEXCOM 3 */
  void* pfnSPI3_Handler;    /* 19  FLEXCOM 3 */
  void* pfnTWI3_Handler;    /* 19  FLEXCOM 3 */
  void* pfnUSART4_Handler;  /* 20  FLEXCOM 4 */
  void* pfnSPI4_Handler;    /* 20  FLEXCOM 4 */
  void* pfnTWI4_Handler;    /* 20  FLEXCOM 4 */
  void* pfnUSART5_Handler;  /* 21  FLEXCOM 5 */
  void* pfnSPI5_Handler;    /* 21  FLEXCOM 5 */
  void* pfnTWI5_Handler;    /* 21  FLEXCOM 5 */
  void* pfnUSART6_Handler;  /* 22  FLEXCOM 6 */
  void* pfnSPI6_Handler;    /* 22  FLEXCOM 6 */
  void* pfnTWI6_Handler;    /* 22  FLEXCOM 6 */
  void* pfnTC0_Handler;     /* 23 Timer/Counter 0 */
  void* pfnTC1_Handler;     /* 24 Timer/Counter 1 */
  void* pfnTC2_Handler;     /* 25 Timer/Counter 2 */
  void* pfnTC3_Handler;     /* 26 Timer/Counter 3 */
  void* pfnTC4_Handler;     /* 27 Timer/Counter 4 */
  void* pfnTC5_Handler;     /* 28 Timer/Counter 5 */
  void* pfnADC_Handler;     /* 29 Analog To Digital Converter */
  void* pfnARM_Handler;     /* 30 FPU */
  void* pvReserved31;
  void* pvReserved32;
  void* pvReserved33;
  void* pvReserved34;
  void* pvReserved35;
  void* pvReserved36;
  void* pvReserved37;
  void* pvReserved38;
  void* pvReserved39;
  void* pvReserved40;
  void* pvReserved41;
  void* pvReserved42;
  void* pvReserved43;
  void* pvReserved44;
  void* pvReserved45;
  void* pvReserved46;
  void* pfnUHP_Handler;     /* 47 USB OHCI */
  void* pfnUDP_Handler;     /* 48 USB Device FS */
  void* pvReserved49;       /*  */
  void* pvReserved50;       /*  */
  void* pvReserved51;       /*  */
  void* pvReserved52;       /*  */
  void* pvReserved53;       /*  */
  void* pvReserved54;       /*  */
  void* pvReserved55;       /*  */
  void* pvReserved56;       /*  */
  void* pvReserved57;       /*  */
  void* pvReserved58;       /*  */
  void* pvReserved59;       /*  */
  void* pvReserved60;       /*  */
  void* pvReserved61;       /*  */
  void* pvReserved62;       /*  */
  void* pvReserved63;       /* UHP */
  void* pvReserved64;       /* UDP */
  void* pvReserved65;       /* CRCCU */
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
void I2SC0_Handler      ( void );
void I2SC1_Handler      ( void );
void MEM2MEM_Handler    ( void );
void PDMIC0_Handler     ( void );
void PDMIC1_Handler     ( void );
void PIOA_Handler       ( void );
void PIOB_Handler       ( void );
void PMC_Handler        ( void );
void RSTC_Handler       ( void );
void RTC_Handler        ( void );
void RTT_Handler        ( void );
void SPI0_Handler        ( void );
void SPI1_Handler        ( void );
void SPI2_Handler        ( void );
void SPI3_Handler        ( void );
void SPI4_Handler        ( void );
void SPI5_Handler        ( void );
void SPI6_Handler        ( void );
void SPI7_Handler        ( void );
void SUPC_Handler       ( void );
void TC0_Handler        ( void );
void TC1_Handler        ( void );
void TC2_Handler        ( void );
void TC3_Handler        ( void );
void TC4_Handler        ( void );
void TC5_Handler        ( void );
void TWI0_Handler       ( void );
void TWI1_Handler       ( void );
void TWI2_Handler       ( void );
void TWI3_Handler       ( void );
void TWI4_Handler       ( void );
void TWI5_Handler       ( void );
void TWI6_Handler       ( void );
void TWI7_Handler       ( void );
void UDP_Handler        ( void );
void UHP_Handler        ( void );
void USART0_Handler      ( void );
void USART1_Handler      ( void );
void USART2_Handler      ( void );
void USART3_Handler      ( void );
void USART4_Handler      ( void );
void USART5_Handler      ( void );
void USART6_Handler      ( void );
void USART7_Handler      ( void );
void WDT_Handler        ( void );

/**
 * \brief Configuration of the Cortex-M4 Processor and Core Peripherals 
 */

#define __CM4_REV              0x0001 /**< SAMG55J18 core revision number ([15:8] revision number, [7:0] patch number) */
#define __MPU_PRESENT          1      /**< SAMG55J18 does provide a MPU */
#define __FPU_PRESENT          1      /**< SAMG55J18 does provide a FPU */
#define __NVIC_PRIO_BITS       4      /**< SAMG55J18 uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig 0      /**< Set to 1 if different SysTick Config is used */

/*
 * \brief CMSIS includes
 */

#include <core_cm4.h>
#if !defined DONT_USE_CMSIS_INIT
#include "system_samg55.h"
#endif /* DONT_USE_CMSIS_INIT */

/*@}*/

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR SAMG55J18 */
/* ************************************************************************** */
/** \addtogroup SAMG55J18_api Peripheral Software API */
/*@{*/

#include "component/adc.h"
#include "component/chipid.h"
#include "component/cmcc.h"
#include "component/crccu.h"
#include "component/efc.h"
#include "component/flexcom.h"
#include "component/gpbr.h"
#include "component/i2sc.h"
#include "component/matrix.h"
#include "component/mem2mem.h"
#include "component/pdc.h"
#include "component/pdmic.h"
#include "component/pio.h"
#include "component/pmc.h"
#include "component/rstc.h"
#include "component/rtc.h"
#include "component/rtt.h"
#include "component/spi.h"
#include "component/supc.h"
#include "component/tc.h"
#include "component/twi.h"
#include "component/twihs.h"
#include "component/udp.h"
#include "component/usart.h"
#include "component/wdt.h"
/*@}*/

/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */
/** \addtogroup SAMG55J18_reg Registers Access Definitions */
/*@{*/

#include "instance/i2sc0.h"
#include "instance/i2sc1.h"
#include "instance/flexcom5.h"
#include "instance/usart5.h"
#include "instance/spi5.h"
#include "instance/twi5.h"
#include "instance/flexcom0.h"
#include "instance/usart0.h"
#include "instance/spi0.h"
#include "instance/twi0.h"
#include "instance/tc0.h"
#include "instance/tc1.h"
#include "instance/flexcom3.h"
#include "instance/usart3.h"
#include "instance/spi3.h"
#include "instance/twi3.h"
#include "instance/flexcom4.h"
#include "instance/usart4.h"
#include "instance/spi4.h"
#include "instance/twi4.h"
#include "instance/flexcom1.h"
#include "instance/usart1.h"
#include "instance/spi1.h"
#include "instance/twi1.h"
#include "instance/flexcom2.h"
#include "instance/usart2.h"
#include "instance/spi2.h"
#include "instance/twi2.h"
#include "instance/mem2mem.h"
#include "instance/pdmic0.h"
#include "instance/pdmic1.h"
#include "instance/flexcom7.h"
#include "instance/usart7.h"
#include "instance/spi7.h"
#include "instance/twi7.h"
#include "instance/adc.h"
#include "instance/cmcc.h"
#include "instance/flexcom6.h"
#include "instance/usart6.h"
#include "instance/spi6.h"
#include "instance/twi6.h"
#include "instance/udp.h"
#include "instance/crccu.h"
#include "instance/uhp.h"
#include "instance/matrix.h"
#include "instance/pmc.h"
#include "instance/chipid.h"
#include "instance/efc.h"
#include "instance/pioa.h"
#include "instance/piob.h"
#include "instance/rstc.h"
#include "instance/supc.h"
#include "instance/rtt.h"
#include "instance/wdt.h"
#include "instance/rtc.h"
#include "instance/gpbr.h"
/*@}*/

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */
/** \addtogroup SAMG55J18_id Peripheral Ids Definitions */
/*@{*/

#define ID_SUPC    ( 0) /**< \brief Supply Controller (SUPC) */
#define ID_RSTC    ( 1) /**< \brief Reset Controller (RSTC) */
#define ID_RTC     ( 2) /**< \brief Real Time Clock (RTC) */
#define ID_RTT     ( 3) /**< \brief Real Time Timer (RTT) */
#define ID_WDT     ( 4) /**< \brief Watchdog Timer (WDT) */
#define ID_PMC     ( 5) /**< \brief Power Management Controller (PMC) */
#define ID_EFC     ( 6) /**< \brief Enhanced Flash Controller (EFC) */
#define ID_USART7  ( 7) /**< \brief FLEXCOM 7 (USART7) */
#define ID_SPI7    ( 7) /**< \brief FLEXCOM 7 (SPI7) */
#define ID_TWI7    ( 7) /**< \brief FLEXCOM 7 (TWI7) */
#define ID_USART0  ( 8) /**< \brief FLEXCOM 0 (USART0) */
#define ID_SPI0    ( 8) /**< \brief FLEXCOM 0 (SPI0) */
#define ID_TWI0    ( 8) /**< \brief FLEXCOM 0 (TWI0) */
#define ID_USART1  ( 9) /**< \brief FLEXCOM 1 (USART1) */
#define ID_SPI1    ( 9) /**< \brief FLEXCOM 1 (SPI1) */
#define ID_TWI1    ( 9) /**< \brief FLEXCOM 1 (TWI1) */
#define ID_PIOA    (11) /**< \brief Parallel I/O Controller A (PIOA) */
#define ID_PIOB    (12) /**< \brief Parallel I/O Controller B (PIOB) */
#define ID_PDMIC0  (13) /**< \brief PDM 0 (PDMIC0) */
#define ID_USART2  (14) /**< \brief FLEXCOM2 (USART2) */
#define ID_SPI2    (14) /**< \brief FLEXCOM2 (SPI2) */
#define ID_TWI2    (14) /**< \brief FLEXCOM2 (TWI2) */
#define ID_MEM2MEM (15) /**< \brief MEM2MEM (MEM2MEM) */
#define ID_I2SC0   (16) /**< \brief I2SC0 (I2SC0) */
#define ID_I2SC1   (17) /**< \brief I2SC1 (I2SC1) */
#define ID_PDMIC1  (18) /**< \brief PDM 1 (PDMIC1) */
#define ID_USART3  (19) /**< \brief FLEXCOM3 (USART3) */
#define ID_SPI3    (19) /**< \brief FLEXCOM3 (SPI3) */
#define ID_TWI3    (19) /**< \brief FLEXCOM3 (TWI3) */
#define ID_USART4  (20) /**< \brief FLEXCOM4 (USART4) */
#define ID_SPI4    (20) /**< \brief FLEXCOM4 (SPI4) */
#define ID_TWI4    (20) /**< \brief FLEXCOM4 (TWI4) */
#define ID_USART5  (21) /**< \brief FLEXCOM5 (USART5) */
#define ID_SPI5    (21) /**< \brief FLEXCOM5 (SPI5) */
#define ID_TWI5    (21) /**< \brief FLEXCOM5 (TWI5) */
#define ID_USART6  (22) /**< \brief FLEXCOM6 (USART6) */
#define ID_SPI6    (22) /**< \brief FLEXCOM6 (SPI6) */
#define ID_TWI6    (22) /**< \brief FLEXCOM6 (TWI6) */
#define ID_TC0     (23) /**< \brief Timer/Counter 0 (TC0) */
#define ID_TC1     (24) /**< \brief Timer/Counter 1 (TC1) */
#define ID_TC2     (25) /**< \brief Timer/Counter 2 (TC2) */
#define ID_TC3     (26) /**< \brief Timer/Counter 3 (TC3) */
#define ID_TC4     (27) /**< \brief Timer/Counter 4 (TC4) */
#define ID_TC5     (28) /**< \brief Timer/Counter 5 (TC5) */
#define ID_ADC     (29) /**< \brief Analog To Digital Converter (ADC) */
#define ID_ARM     (30) /**< \brief FPU (ARM) */
#define ID_UHP     (47) /**< \brief USB OHCI (UHP) */
#define ID_UDP     (48) /**< \brief USB Device FS (UDP) */
#define ID_CRCCU   (49) /**< \brief CRCCU (CRCCU) */

#define ID_PERIPH_COUNT (50) /**< \brief Number of peripheral IDs */
/*@}*/

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */
/** \addtogroup SAMG55J18_base Peripheral Base Address Definitions */
/*@{*/

#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define I2SC0       (0x40000000U) /**< \brief (I2SC0      ) Base Address */
#define PDC_I2SC0   (0x40000100U) /**< \brief (PDC_I2SC0  ) Base Address */
#define I2SC1       (0x40004000U) /**< \brief (I2SC1      ) Base Address */
#define PDC_I2SC1   (0x40004100U) /**< \brief (PDC_I2SC1  ) Base Address */
#define FLEXCOM5    (0x40008000U) /**< \brief (FLEXCOM5   ) Base Address */
#define USART5      (0x40008200U) /**< \brief (USART5     ) Base Address */
#define PDC_USART5  (0x40008300U) /**< \brief (PDC_USART5 ) Base Address */
#define SPI5        (0x40008400U) /**< \brief (SPI5       ) Base Address */
#define PDC_SPI5    (0x40008500U) /**< \brief (PDC_SPI5   ) Base Address */
#define TWI5        (0x40008600U) /**< \brief (TWI5       ) Base Address */
#define PDC_TWI5    (0x40008700U) /**< \brief (PDC_TWI5   ) Base Address */
#define FLEXCOM0    (0x4000C000U) /**< \brief (FLEXCOM0   ) Base Address */
#define USART0      (0x4000C200U) /**< \brief (USART0     ) Base Address */
#define PDC_USART0  (0x4000C300U) /**< \brief (PDC_USART0 ) Base Address */
#define SPI0        (0x4000C400U) /**< \brief (SPI0       ) Base Address */
#define PDC_SPI0    (0x4000C500U) /**< \brief (PDC_SPI0   ) Base Address */
#define TWI0        (0x4000C600U) /**< \brief (TWI0       ) Base Address */
#define PDC_TWI0    (0x4000C700U) /**< \brief (PDC_TWI0   ) Base Address */
#define TC0         (0x40010000U) /**< \brief (TC0        ) Base Address */
#define PDC_TC0     (0x40010100U) /**< \brief (PDC_TC0    ) Base Address */
#define TC1         (0x40014000U) /**< \brief (TC1        ) Base Address */
#define FLEXCOM3    (0x40018000U) /**< \brief (FLEXCOM3   ) Base Address */
#define USART3      (0x40018200U) /**< \brief (USART3     ) Base Address */
#define PDC_USART3  (0x40018300U) /**< \brief (PDC_USART3 ) Base Address */
#define SPI3        (0x40018400U) /**< \brief (SPI3       ) Base Address */
#define PDC_SPI3    (0x40018500U) /**< \brief (PDC_SPI3   ) Base Address */
#define TWI3        (0x40018600U) /**< \brief (TWI3       ) Base Address */
#define PDC_TWI3    (0x40018700U) /**< \brief (PDC_TWI3   ) Base Address */
#define FLEXCOM4    (0x4001C000U) /**< \brief (FLEXCOM4   ) Base Address */
#define USART4      (0x4001C200U) /**< \brief (USART4     ) Base Address */
#define PDC_USART4  (0x4001C300U) /**< \brief (PDC_USART4 ) Base Address */
#define SPI4        (0x4001C400U) /**< \brief (SPI4       ) Base Address */
#define PDC_SPI4    (0x4001C500U) /**< \brief (PDC_SPI4   ) Base Address */
#define TWI4        (0x4001C600U) /**< \brief (TWI4       ) Base Address */
#define PDC_TWI4    (0x4001C700U) /**< \brief (PDC_TWI4   ) Base Address */
#define FLEXCOM1    (0x40020000U) /**< \brief (FLEXCOM1   ) Base Address */
#define USART1      (0x40020200U) /**< \brief (USART1     ) Base Address */
#define PDC_USART1  (0x40020300U) /**< \brief (PDC_USART1 ) Base Address */
#define SPI1        (0x40020400U) /**< \brief (SPI1       ) Base Address */
#define PDC_SPI1    (0x40020500U) /**< \brief (PDC_SPI1   ) Base Address */
#define TWI1        (0x40020600U) /**< \brief (TWI1       ) Base Address */
#define PDC_TWI1    (0x40020700U) /**< \brief (PDC_TWI1   ) Base Address */
#define FLEXCOM2    (0x40024000U) /**< \brief (FLEXCOM2   ) Base Address */
#define USART2      (0x40024200U) /**< \brief (USART2     ) Base Address */
#define PDC_USART2  (0x40024300U) /**< \brief (PDC_USART2 ) Base Address */
#define SPI2        (0x40024400U) /**< \brief (SPI2       ) Base Address */
#define PDC_SPI2    (0x40024500U) /**< \brief (PDC_SPI2   ) Base Address */
#define TWI2        (0x40024600U) /**< \brief (TWI2       ) Base Address */
#define PDC_TWI2    (0x40024700U) /**< \brief (PDC_TWI2   ) Base Address */
#define MEM2MEM     (0x40028000U) /**< \brief (MEM2MEM    ) Base Address */
#define PDC_MEM2MEM (0x40028100U) /**< \brief (PDC_MEM2MEM) Base Address */
#define PDMIC0      (0x4002C000U) /**< \brief (PDMIC0     ) Base Address */
#define PDC_PDMIC0  (0x4002C100U) /**< \brief (PDC_PDMIC0 ) Base Address */
#define PDMIC1      (0x40030000U) /**< \brief (PDMIC1     ) Base Address */
#define PDC_PDMIC1  (0x40030100U) /**< \brief (PDC_PDMIC1 ) Base Address */
#define FLEXCOM7    (0x40034000U) /**< \brief (FLEXCOM7   ) Base Address */
#define USART7      (0x40034200U) /**< \brief (USART7     ) Base Address */
#define PDC_USART7  (0x40034300U) /**< \brief (PDC_USART7 ) Base Address */
#define SPI7        (0x40034400U) /**< \brief (SPI7       ) Base Address */
#define PDC_SPI7    (0x40034500U) /**< \brief (PDC_SPI7   ) Base Address */
#define TWI7        (0x40034600U) /**< \brief (TWI7       ) Base Address */
#define PDC_TWI7    (0x40034700U) /**< \brief (PDC_TWI7   ) Base Address */
#define ADC         (0x40038000U) /**< \brief (ADC        ) Base Address */
#define PDC_ADC     (0x40038100U) /**< \brief (PDC_ADC    ) Base Address */
#define CMCC        (0x4003C000U) /**< \brief (CMCC       ) Base Address */
#define FLEXCOM6    (0x40040000U) /**< \brief (FLEXCOM6   ) Base Address */
#define USART6      (0x40040200U) /**< \brief (USART6     ) Base Address */
#define PDC_USART6  (0x40040300U) /**< \brief (PDC_USART6 ) Base Address */
#define SPI6        (0x40040400U) /**< \brief (SPI6       ) Base Address */
#define PDC_SPI6    (0x40040500U) /**< \brief (PDC_SPI6   ) Base Address */
#define TWI6        (0x40040600U) /**< \brief (TWI6       ) Base Address */
#define PDC_TWI6    (0x40040700U) /**< \brief (PDC_TWI6   ) Base Address */
#define UDP         (0x40044000U) /**< \brief (UDP        ) Base Address */
#define CRCCU       (0x40048000U) /**< \brief (CRCCU      ) Base Address */
#define UHP         (0x4004C000U) /**< \brief (UHP        ) Base Address */
#define MATRIX      (0x400E0200U) /**< \brief (MATRIX     ) Base Address */
#define PMC         (0x400E0400U) /**< \brief (PMC        ) Base Address */
#define CHIPID      (0x400E0740U) /**< \brief (CHIPID     ) Base Address */
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
#define I2SC0       ((I2sc     *)0x40000000U) /**< \brief (I2SC0      ) Base Address */
#define PDC_I2SC0   ((Pdc      *)0x40000100U) /**< \brief (PDC_I2SC0  ) Base Address */
#define I2SC1       ((I2sc     *)0x40004000U) /**< \brief (I2SC1      ) Base Address */
#define PDC_I2SC1   ((Pdc      *)0x40004100U) /**< \brief (PDC_I2SC1  ) Base Address */
#define FLEXCOM5    ((Flexcom  *)0x40008000U) /**< \brief (FLEXCOM5   ) Base Address */
#define USART5      ((Usart    *)0x40008200U) /**< \brief (USART5     ) Base Address */
#define PDC_USART5  ((Pdc      *)0x40008300U) /**< \brief (PDC_USART5 ) Base Address */
#define SPI5        ((Spi      *)0x40008400U) /**< \brief (SPI5       ) Base Address */
#define PDC_SPI5    ((Pdc      *)0x40008500U) /**< \brief (PDC_SPI5   ) Base Address */
#define TWI5        ((Twi      *)0x40008600U) /**< \brief (TWI5       ) Base Address */
#define PDC_TWI5    ((Pdc      *)0x40008700U) /**< \brief (PDC_TWI5   ) Base Address */
#define FLEXCOM0    ((Flexcom  *)0x4000C000U) /**< \brief (FLEXCOM0   ) Base Address */
#define USART0      ((Usart    *)0x4000C200U) /**< \brief (USART0     ) Base Address */
#define PDC_USART0  ((Pdc      *)0x4000C300U) /**< \brief (PDC_USART0 ) Base Address */
#define SPI0        ((Spi      *)0x4000C400U) /**< \brief (SPI0       ) Base Address */
#define PDC_SPI0    ((Pdc      *)0x4000C500U) /**< \brief (PDC_SPI0   ) Base Address */
#define TWI0        ((Twi      *)0x4000C600U) /**< \brief (TWI0       ) Base Address */
#define PDC_TWI0    ((Pdc      *)0x4000C700U) /**< \brief (PDC_TWI0   ) Base Address */
#define TC0         ((Tc       *)0x40010000U) /**< \brief (TC0        ) Base Address */
#define PDC_TC0     ((Pdc      *)0x40010100U) /**< \brief (PDC_TC0    ) Base Address */
#define TC1         ((Tc       *)0x40014000U) /**< \brief (TC1        ) Base Address */
#define FLEXCOM3    ((Flexcom  *)0x40018000U) /**< \brief (FLEXCOM3   ) Base Address */
#define USART3      ((Usart    *)0x40018200U) /**< \brief (USART3     ) Base Address */
#define PDC_USART3  ((Pdc      *)0x40018300U) /**< \brief (PDC_USART3 ) Base Address */
#define SPI3        ((Spi      *)0x40018400U) /**< \brief (SPI3       ) Base Address */
#define PDC_SPI3    ((Pdc      *)0x40018500U) /**< \brief (PDC_SPI3   ) Base Address */
#define TWI3        ((Twihs    *)0x40018600U) /**< \brief (TWI3       ) Base Address */
#define PDC_TWI3    ((Pdc      *)0x40018700U) /**< \brief (PDC_TWI3   ) Base Address */
#define FLEXCOM4    ((Flexcom  *)0x4001C000U) /**< \brief (FLEXCOM4   ) Base Address */
#define USART4      ((Usart    *)0x4001C200U) /**< \brief (USART4     ) Base Address */
#define PDC_USART4  ((Pdc      *)0x4001C300U) /**< \brief (PDC_USART4 ) Base Address */
#define SPI4        ((Spi      *)0x4001C400U) /**< \brief (SPI4       ) Base Address */
#define PDC_SPI4    ((Pdc      *)0x4001C500U) /**< \brief (PDC_SPI4   ) Base Address */
#define TWI4        ((Twi      *)0x4001C600U) /**< \brief (TWI4       ) Base Address */
#define PDC_TWI4    ((Pdc      *)0x4001C700U) /**< \brief (PDC_TWI4   ) Base Address */
#define FLEXCOM1    ((Flexcom  *)0x40020000U) /**< \brief (FLEXCOM1   ) Base Address */
#define USART1      ((Usart    *)0x40020200U) /**< \brief (USART1     ) Base Address */
#define PDC_USART1  ((Pdc      *)0x40020300U) /**< \brief (PDC_USART1 ) Base Address */
#define SPI1        ((Spi      *)0x40020400U) /**< \brief (SPI1       ) Base Address */
#define PDC_SPI1    ((Pdc      *)0x40020500U) /**< \brief (PDC_SPI1   ) Base Address */
#define TWI1        ((Twi      *)0x40020600U) /**< \brief (TWI1       ) Base Address */
#define PDC_TWI1    ((Pdc      *)0x40020700U) /**< \brief (PDC_TWI1   ) Base Address */
#define FLEXCOM2    ((Flexcom  *)0x40024000U) /**< \brief (FLEXCOM2   ) Base Address */
#define USART2      ((Usart    *)0x40024200U) /**< \brief (USART2     ) Base Address */
#define PDC_USART2  ((Pdc      *)0x40024300U) /**< \brief (PDC_USART2 ) Base Address */
#define SPI2        ((Spi      *)0x40024400U) /**< \brief (SPI2       ) Base Address */
#define PDC_SPI2    ((Pdc      *)0x40024500U) /**< \brief (PDC_SPI2   ) Base Address */
#define TWI2        ((Twi      *)0x40024600U) /**< \brief (TWI2       ) Base Address */
#define PDC_TWI2    ((Pdc      *)0x40024700U) /**< \brief (PDC_TWI2   ) Base Address */
#define MEM2MEM     ((Mem2mem  *)0x40028000U) /**< \brief (MEM2MEM    ) Base Address */
#define PDC_MEM2MEM ((Pdc      *)0x40028100U) /**< \brief (PDC_MEM2MEM) Base Address */
#define PDMIC0      ((Pdmic    *)0x4002C000U) /**< \brief (PDMIC0     ) Base Address */
#define PDC_PDMIC0  ((Pdc      *)0x4002C100U) /**< \brief (PDC_PDMIC0 ) Base Address */
#define PDMIC1      ((Pdmic    *)0x40030000U) /**< \brief (PDMIC1     ) Base Address */
#define PDC_PDMIC1  ((Pdc      *)0x40030100U) /**< \brief (PDC_PDMIC1 ) Base Address */
#define FLEXCOM7    ((Flexcom  *)0x40034000U) /**< \brief (FLEXCOM7   ) Base Address */
#define USART7      ((Usart    *)0x40034200U) /**< \brief (USART7     ) Base Address */
#define PDC_USART7  ((Pdc      *)0x40034300U) /**< \brief (PDC_USART7 ) Base Address */
#define SPI7        ((Spi      *)0x40034400U) /**< \brief (SPI7       ) Base Address */
#define PDC_SPI7    ((Pdc      *)0x40034500U) /**< \brief (PDC_SPI7   ) Base Address */
#define TWI7        ((Twi      *)0x40034600U) /**< \brief (TWI7       ) Base Address */
#define PDC_TWI7    ((Pdc      *)0x40034700U) /**< \brief (PDC_TWI7   ) Base Address */
#define ADC         ((Adc      *)0x40038000U) /**< \brief (ADC        ) Base Address */
#define PDC_ADC     ((Pdc      *)0x40038100U) /**< \brief (PDC_ADC    ) Base Address */
#define CMCC        ((Cmcc     *)0x4003C000U) /**< \brief (CMCC       ) Base Address */
#define FLEXCOM6    ((Flexcom  *)0x40040000U) /**< \brief (FLEXCOM6   ) Base Address */
#define USART6      ((Usart    *)0x40040200U) /**< \brief (USART6     ) Base Address */
#define PDC_USART6  ((Pdc      *)0x40040300U) /**< \brief (PDC_USART6 ) Base Address */
#define SPI6        ((Spi      *)0x40040400U) /**< \brief (SPI6       ) Base Address */
#define PDC_SPI6    ((Pdc      *)0x40040500U) /**< \brief (PDC_SPI6   ) Base Address */
#define TWI6        ((Twi      *)0x40040600U) /**< \brief (TWI6       ) Base Address */
#define PDC_TWI6    ((Pdc      *)0x40040700U) /**< \brief (PDC_TWI6   ) Base Address */
#define UDP         ((Udp      *)0x40044000U) /**< \brief (UDP        ) Base Address */
#define CRCCU       ((Crccu    *)0x40048000U) /**< \brief (CRCCU      ) Base Address */
#define UHP         ((Uhp      *)0x4004C000U) /**< \brief (UHP        ) Base Address */
#define MATRIX      ((Matrix   *)0x400E0200U) /**< \brief (MATRIX     ) Base Address */
#define PMC         ((Pmc      *)0x400E0400U) /**< \brief (PMC        ) Base Address */
#define CHIPID      ((Chipid   *)0x400E0740U) /**< \brief (CHIPID     ) Base Address */
#define EFC         ((Efc      *)0x400E0A00U) /**< \brief (EFC        ) Base Address */
#define PIOA        ((Pio      *)0x400E0E00U) /**< \brief (PIOA       ) Base Address */
#define PIOB        ((Pio      *)0x400E1000U) /**< \brief (PIOB       ) Base Address */
#define RSTC        ((Rstc     *)0x400E1400U) /**< \brief (RSTC       ) Base Address */
#define SUPC        ((Supc     *)0x400E1410U) /**< \brief (SUPC       ) Base Address */
#define RTT         ((Rtt      *)0x400E1430U) /**< \brief (RTT        ) Base Address */
#define WDT         ((Wdt      *)0x400E1450U) /**< \brief (WDT        ) Base Address */
#define RTC         ((Rtc      *)0x400E1460U) /**< \brief (RTC        ) Base Address */
#define GPBR        ((Gpbr     *)0x400E1490U) /**< \brief (GPBR       ) Base Address */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/*@}*/

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */
/** \addtogroup SAMG55J18_pio Peripheral Pio Definitions */
/*@{*/

#include "pio/samg55j18.h"
/*@}*/

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */

#define IFLASH_SIZE             (0x40000u)
#define IFLASH_PAGE_SIZE        (512u)
#define IFLASH_LOCK_REGION_SIZE (8192u)
#define IFLASH_NB_OF_PAGES      (512u)
#define IFLASH_NB_OF_LOCK_BITS  (32u)
#define IRAM_SIZE               (0x28000u)

#define IFLASH_ADDR (0x00400000u) /**< Internal Flash base address */
#define IROM_ADDR   (0x00800000u) /**< Internal ROM base address */
#define IRAM_ADDR   (0x20000000u) /**< Internal RAM base address */

/* ************************************************************************** */
/*   MISCELLANEOUS DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */

#define CHIP_JTAGID (0x05B3E03FUL)
#define CHIP_CIDR   (0x245709E0UL)
#define CHIP_EXID   (0x0UL)
#define NB_CH_ADC   (8UL)

/* ************************************************************************** */
/*   ELECTRICAL DEFINITIONS FOR SAMG55J18 */
/* ************************************************************************** */

/* Device characteristics */
#define CHIP_FREQ_SLCK_RC_MIN           (20000UL)
#define CHIP_FREQ_SLCK_RC               (32000UL)
#define CHIP_FREQ_SLCK_RC_MAX           (44000UL)
#define CHIP_FREQ_MAINCK_RC_8MHZ        (8000000UL)
#define CHIP_FREQ_MAINCK_RC_16MHZ       (16000000UL)
#define CHIP_FREQ_MAINCK_RC_24MHZ       (24000000UL)
#define CHIP_FREQ_CPU_MAX               (120000000UL)
#define CHIP_FREQ_XTAL_32K              (32768UL)


/* Embedded Flash Write Wait State */
#define CHIP_FLASH_WRITE_WAIT_STATE     (5U)

/* Embedded Flash Read Wait State (VDDIO set at 1.62V, Max value) */
#define CHIP_FREQ_FWS_0                 (20000000UL)  /**< \brief Maximum operating frequency when FWS is 0 */
#define CHIP_FREQ_FWS_1                 (40000000UL)  /**< \brief Maximum operating frequency when FWS is 1 */
#define CHIP_FREQ_FWS_2                 (60000000UL)  /**< \brief Maximum operating frequency when FWS is 2 */
#define CHIP_FREQ_FWS_3                 (80000000UL)  /**< \brief Maximum operating frequency when FWS is 3 */
#define CHIP_FREQ_FWS_4                 (100000000UL) /**< \brief Maximum operating frequency when FWS is 4 */
#define CHIP_FREQ_FWS_5                 (120000000UL) /**< \brief Maximum operating frequency when FWS is 5 */

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* _SAMG55J18_ */
