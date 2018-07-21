/**
 * \file
 *
 * \brief IAR Startup file for SAM4L.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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
 
#include <stdint.h>
#include "exceptions.h"

/*------------------------------------------------------------------------------
 *         Types
 *------------------------------------------------------------------------------*/
typedef union { IntFunc __fun; void * __ptr; } IntVector;

/*------------------------------------------------------------------------------
 *         ProtoTypes
 *------------------------------------------------------------------------------*/
extern void __iar_program_start( void );

int __low_level_init( void );

/*------------------------------------------------------------------------------
 *         Variables
 *------------------------------------------------------------------------------*/
extern unsigned int __ICFEDIT_vector_start__;


/*------------------------------------------------------------------------------
 *         Exception Table
 *------------------------------------------------------------------------------*/
#pragma language=extended
#pragma segment="CSTACK"

/*  The name "__vector_table" has special meaning for C-SPY: */
/*  it is where the SP start value is found, and the NVIC vector */
/*  table register (VTOR) is initialized to this address if != 0. */

#pragma section = ".vectors"
#pragma location = ".vectors"
const IntVector __vector_table[] =
{
    { .__ptr = __sfe( "CSTACK" ) },
    __iar_program_start,

    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,         /*  Reserved */
    SVC_Handler,
    DebugMon_Handler,
    0,                  /*  Reserved */
    PendSV_Handler,
    SysTick_Handler,
    // Configurable interrupts
    HFLASHC_Handler, /**<  0 SAM4L4 Flash Controller (HFLASHC) */
    PDCA_0_Handler, /**<  1 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_1_Handler, /**<  2 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_2_Handler, /**<  3 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_3_Handler, /**<  4 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_4_Handler, /**<  5 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_5_Handler, /**<  6 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_6_Handler, /**<  7 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_7_Handler, /**<  8 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_8_Handler, /**<  9 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_9_Handler, /**< 10 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_10_Handler, /**< 11 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_11_Handler, /**< 12 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_12_Handler, /**< 13 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_13_Handler, /**< 14 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_14_Handler, /**< 15 SAM4L4 Peripheral DMA Controller (PDCA) */
    PDCA_15_Handler, /**< 16 SAM4L4 Peripheral DMA Controller (PDCA) */
    CRCCU_Handler, /**< 17 SAM4L4 CRC Calculation Unit (CRCCU) */
    USBC_Handler, /**< 18 SAM4L4 USB 2.0 Interface (USBC) */
    PEVC_TR_Handler, /**< 19 SAM4L4 Peripheral Event Controller (PEVC) */
    PEVC_OV_Handler, /**< 20 SAM4L4 Peripheral Event Controller (PEVC) */
    #ifdef AESA
    AESA_Handler, /**< 21 SAM4LC Advanced Encryption Standard (AESA) */
    #else
    Dummy_Handler,
    #endif
    PM_Handler, /**< 22 SAM4L4 Power Manager (PM) */
    SCIF_Handler, /**< 23 SAM4L4 System Control Interface (SCIF) */
    FREQM_Handler, /**< 24 SAM4L4 Frequency Meter (FREQM) */
    GPIO_0_Handler, /**< 25 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_1_Handler, /**< 26 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_2_Handler, /**< 27 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_3_Handler, /**< 28 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_4_Handler, /**< 29 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_5_Handler, /**< 30 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_6_Handler, /**< 31 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_7_Handler, /**< 32 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_8_Handler, /**< 33 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_9_Handler, /**< 34 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_10_Handler, /**< 35 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    GPIO_11_Handler, /**< 36 SAM4L4 General-Purpose Input/Output Controller (GPIO) */
    BPM_Handler, /**< 37 SAM4L4 Backup Power Manager (BPM) */
    BSCIF_Handler, /**< 38 SAM4L4 Backup System Control Interface (BSCIF) */
    AST_ALARM_Handler, /**< 39 SAM4L4 Asynchronous Timer (AST) */
    AST_PER_Handler, /**< 40 SAM4L4 Asynchronous Timer (AST) */
    AST_OVF_Handler, /**< 41 SAM4L4 Asynchronous Timer (AST) */
    AST_READY_Handler, /**< 42 SAM4L4 Asynchronous Timer (AST) */
    AST_CLKREADY_Handler, /**< 43 SAM4L4 Asynchronous Timer (AST) */
    WDT_Handler, /**< 44 SAM4L4 Watchdog Timer (WDT) */
    EIC_1_Handler, /**< 45 SAM4L4 External Interrupt Controller (EIC) */
    EIC_2_Handler, /**< 46 SAM4L4 External Interrupt Controller (EIC) */
    EIC_3_Handler, /**< 47 SAM4L4 External Interrupt Controller (EIC) */
    EIC_4_Handler, /**< 48 SAM4L4 External Interrupt Controller (EIC) */
    EIC_5_Handler, /**< 49 SAM4L4 External Interrupt Controller (EIC) */
    EIC_6_Handler, /**< 50 SAM4L4 External Interrupt Controller (EIC) */
    EIC_7_Handler, /**< 51 SAM4L4 External Interrupt Controller (EIC) */
    EIC_8_Handler, /**< 52 SAM4L4 External Interrupt Controller (EIC) */
    IISC_Handler, /**< 53 SAM4L4 Inter-IC Sound (I2S) Controller (IISC) */
    SPI_Handler, /**< 54 SAM4L4 Serial Peripheral Interface (SPI) */
    TC00_Handler, /**< 55 SAM4L4 Timer/Counter 0 (TC0) */
    TC01_Handler, /**< 56 SAM4L4 Timer/Counter 0 (TC0) */
    TC02_Handler, /**< 57 SAM4L4 Timer/Counter 0 (TC0) */
    TC10_Handler, /**< 58 SAM4L4 Timer/Counter 1 (TC1) */
    TC11_Handler, /**< 59 SAM4L4 Timer/Counter 1 (TC1) */
    TC12_Handler, /**< 60 SAM4L4 Timer/Counter 1 (TC1) */
    TWIM0_Handler, /**< 61 SAM4L4 Two-wire Master Interface 0 (TWIM0) */
    TWIS0_Handler, /**< 62 SAM4L4 Two-wire Slave Interface 0 (TWIS0) */
    TWIM1_Handler, /**< 63 SAM4L4 Two-wire Master Interface 1 (TWIM1) */
    TWIS1_Handler, /**< 64 SAM4L4 Two-wire Slave Interface 1 (TWIS1) */
    USART0_Handler, /**< 65 SAM4L4 Universal Synchronous Asynchronous Receiver Transmitter 0 (USART0) */
    USART1_Handler, /**< 66 SAM4L4 Universal Synchronous Asynchronous Receiver Transmitter 1 (USART1) */
    USART2_Handler, /**< 67 SAM4L4 Universal Synchronous Asynchronous Receiver Transmitter 2 (USART2) */
    USART3_Handler, /**< 68 SAM4LxxC & SAM4LxxB Universal Synchronous Asynchronous Receiver Transmitter 3 (USART3) */
    ADCIFE_Handler, /**< 69 SAM4L4 ADC controller interface (ADCIFE) */
    DACC_Handler, /**< 70 SAM4L4 DAC Controller (DACC) */
    ACIFC_Handler, /**< 71 SAM4L4 Analog Comparator Interface (ACIFC) */
    ABDACB_Handler, /**< 72 SAM4L4 Audio Bitstream DAC (ABDACB) */
    TRNG_Handler, /**< 73 SAM4L4 True Random Number Generator (TRNG) */
    PARC_Handler, /**< 74 SAM4L4 Parallel Capture (PARC) */
    CATB_Handler, /**< 75 SAM4L4 Capacitive Touch Module B (CATB) */
    Dummy_Handler, /**< Not used */
    TWIM2_Handler, /**< 77 SAM4LxxC & SAM4LxxB Two-wire Master Interface 2 (TWIM2) */
    TWIM3_Handler, /**< 78 SAM4LxxC & SAM4LxxB Two-wire Master Interface 3 (TWIM3) */
    #ifdef LCDCA
    LCDCA_Handler  /**< 79 SAM4LC LCD Controller (LCDCA) */
    #else
    Dummy_Handler
    #endif
};

/*------------------------------------------------------------------------------
 *         Exception Table
 *------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
// Vector Table Offset Register address
#ifndef __CORE_CM4_H_GENERIC
#define ARM_NVIC_VTOR_ADDRESS               ((uint32_t *)0xE000ED08)
int __low_level_init( void )
{
    unsigned int * src = __section_begin(".vectors");
    volatile uint32_t *pVTOR = ARM_NVIC_VTOR_ADDRESS; // Vector Table Offset Register
    *pVTOR  = ((unsigned int)(src)) | (0x0 << 7);

    return 1; /*  if return 0, the data sections will not be initialized. */
}
#else
// TODO: move this include at the top of this file when all code is CMSIS-compliant.
#include "sam4l.h"
int __low_level_init( void )
{
    unsigned int * pSrc = __section_begin(".vectors");
    volatile SCB_Type *pSCB = (SCB_Type *)SCB; // System Control Block (SCB) offset
    pSCB->VTOR = ( (uint32_t)pSrc & SCB_VTOR_TBLOFF_Msk );

    return 1; /*  if return 0, the data sections will not be initialized. */
}
#endif
