/**
 * \file
 *
 * \brief Startup file for SAMG55.
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

#include "samg55.h"

typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M4 core handlers */
#pragma weak NMI_Handler=Dummy_Handler
#pragma weak HardFault_Handler=Dummy_Handler
#pragma weak MemManage_Handler=Dummy_Handler
#pragma weak BusFault_Handler=Dummy_Handler
#pragma weak UsageFault_Handler=Dummy_Handler
#pragma weak SVC_Handler=Dummy_Handler
#pragma weak DebugMon_Handler=Dummy_Handler
#pragma weak PendSV_Handler=Dummy_Handler
#pragma weak SysTick_Handler=Dummy_Handler

/* Peripherals handlers */
#pragma weak SUPC_Handler=Dummy_Handler
#pragma weak RSTC_Handler=Dummy_Handler
#pragma weak RTC_Handler=Dummy_Handler
#pragma weak RTT_Handler=Dummy_Handler
#pragma weak WDT_Handler=Dummy_Handler
#pragma weak PMC_Handler=Dummy_Handler
#pragma weak EFC_Handler=Dummy_Handler
#ifdef _SAMG55_FLEXCOM7_INSTANCE_
#pragma weak FLEXCOM7_Handler=Dummy_Handler
#endif /* _SAMG55_FLEXCOM7_INSTANCE_ */
#pragma weak FLEXCOM0_Handler=Dummy_Handler
#pragma weak FLEXCOM1_Handler=Dummy_Handler
#pragma weak PIOA_Handler=Dummy_Handler
#pragma weak PIOB_Handler=Dummy_Handler
#pragma weak PDMIC0_Handler=Dummy_Handler
#pragma weak FLEXCOM2_Handler=Dummy_Handler
#pragma weak MEM2MEM_Handler=Dummy_Handler
#pragma weak I2SC0_Handler=Dummy_Handler
#pragma weak I2SC1_Handler=Dummy_Handler
#pragma weak PDMIC1_Handler=Dummy_Handler
#pragma weak FLEXCOM3_Handler=Dummy_Handler
#pragma weak FLEXCOM4_Handler=Dummy_Handler
#pragma weak FLEXCOM5_Handler=Dummy_Handler
#pragma weak FLEXCOM6_Handler=Dummy_Handler
#pragma weak TC0_Handler=Dummy_Handler
#pragma weak TC1_Handler=Dummy_Handler
#pragma weak TC2_Handler=Dummy_Handler
#pragma weak TC3_Handler=Dummy_Handler
#pragma weak TC4_Handler=Dummy_Handler
#pragma weak TC5_Handler=Dummy_Handler
#pragma weak ADC_Handler=Dummy_Handler
#pragma weak ARM_Handler=Dummy_Handler
#pragma weak UHP_Handler=Dummy_Handler
#pragma weak UDP_Handler=Dummy_Handler
#pragma weak CRCCU_Handler=Dummy_Handler

/* Exception Table */
#pragma language = extended
#pragma segment = "CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
const DeviceVectors __vector_table = {
        (void*) __sfe("CSTACK"),
        
        .pfnReset_Handler      = (void*) Reset_Handler,
        .pfnNMI_Handler        = (void*) NMI_Handler,
        .pfnHardFault_Handler  = (void*) HardFault_Handler,
        .pfnMemManage_Handler  = (void*) MemManage_Handler,
        .pfnBusFault_Handler   = (void*) BusFault_Handler,
        .pfnUsageFault_Handler = (void*) UsageFault_Handler,
        .pfnReserved1_Handler  = (void*) (0UL),           /* Reserved */
        .pfnReserved2_Handler  = (void*) (0UL),           /* Reserved */
        .pfnReserved3_Handler  = (void*) (0UL),           /* Reserved */
        .pfnReserved4_Handler  = (void*) (0UL),           /* Reserved */
        .pfnSVC_Handler        = (void*) SVC_Handler,
        .pfnDebugMon_Handler   = (void*) DebugMon_Handler,
        .pfnReserved5_Handler  = (void*) (0UL),           /* Reserved */
        .pfnPendSV_Handler     = (void*) PendSV_Handler,
        .pfnSysTick_Handler    = (void*) SysTick_Handler,

        /* Configurable interrupts */
        .pfnSUPC_Handler    = (void*) SUPC_Handler,    /* 0  Supply Controller */
        .pfnRSTC_Handler    = (void*) RSTC_Handler,    /* 1  Reset Controller */
        .pfnRTC_Handler     = (void*) RTC_Handler,     /* 2  Real Time Clock */
        .pfnRTT_Handler     = (void*) RTT_Handler,     /* 3  Real Time Timer */
        .pfnWDT_Handler     = (void*) WDT_Handler,     /* 4  Watchdog Timer */
        .pfnPMC_Handler     = (void*) PMC_Handler,     /* 5  Power Management Controller */
        .pfnEFC_Handler     = (void*) EFC_Handler,     /* 6  Enhanced Flash Controller */
#ifdef _SAMG55_FLEXCOM7_INSTANCE_
        .pfnFLEXCOM7_Handler  = (void*) FLEXCOM7_Handler,  /* 7  FLEXCOM 7 */
#else
        .pvReserved7        = (void*) (0UL),           /* 7  Reserved */
#endif  /* _SAMG55_FLEXCOM7_INSTANCE_ */
        .pfnFLEXCOM0_Handler  = (void*) FLEXCOM0_Handler,  /* 8  FLEXCOM 0 */
        .pfnFLEXCOM1_Handler  = (void*) FLEXCOM1_Handler,  /* 9  FLEXCOM 1 */
        .pvReserved10       = (void*) (0UL),           /* 10 Reserved */
        .pfnPIOA_Handler    = (void*) PIOA_Handler,    /* 11 Parallel I/O Controller A */
        .pfnPIOB_Handler    = (void*) PIOB_Handler,    /* 12 Parallel I/O Controller B */
        .pfnPDMIC0_Handler  = (void*) PDMIC0_Handler,  /* 13 PDM 0 */
        .pfnFLEXCOM2_Handler  = (void*) FLEXCOM2_Handler,  /* 14 FLEXCOM2 */
        .pfnMEM2MEM_Handler = (void*) MEM2MEM_Handler, /* 15 MEM2MEM */
        .pfnI2SC0_Handler   = (void*) I2SC0_Handler,   /* 16 I2SC0 */
        .pfnI2SC1_Handler   = (void*) I2SC1_Handler,   /* 17 I2SC1 */
        .pfnPDMIC1_Handler  = (void*) PDMIC1_Handler,  /* 18 PDM 1 */
        .pfnFLEXCOM3_Handler  = (void*) FLEXCOM3_Handler,  /* 19 FLEXCOM3 */
        .pfnFLEXCOM4_Handler  = (void*) FLEXCOM4_Handler,  /* 20 FLEXCOM4 */
        .pfnFLEXCOM5_Handler  = (void*) FLEXCOM5_Handler,  /* 21 FLEXCOM5 */
        .pfnFLEXCOM6_Handler  = (void*) FLEXCOM6_Handler,  /* 22 FLEXCOM6 */
        .pfnTC0_Handler     = (void*) TC0_Handler,     /* 23 Timer/Counter 0 */
        .pfnTC1_Handler     = (void*) TC1_Handler,     /* 24 Timer/Counter 1 */
        .pfnTC2_Handler     = (void*) TC2_Handler,     /* 25 Timer/Counter 2 */
        .pfnTC3_Handler     = (void*) TC3_Handler,     /* 26 Timer/Counter 3 */
        .pfnTC4_Handler     = (void*) TC4_Handler,     /* 27 Timer/Counter 4 */
        .pfnTC5_Handler     = (void*) TC5_Handler,     /* 28 Timer/Counter 5 */
        .pfnADC_Handler     = (void*) ADC_Handler,     /* 29 Analog To Digital Converter */
        .pfnARM_Handler     = (void*) ARM_Handler,     /* 30 FPU */
        .pvReserved31       = (void*) (0UL),           /* 31 Reserved */
        .pvReserved32       = (void*) (0UL),           /* 32 Reserved */
        .pvReserved33       = (void*) (0UL),           /* 33 Reserved */
        .pvReserved34       = (void*) (0UL),           /* 34 Reserved */
        .pvReserved35       = (void*) (0UL),           /* 35 Reserved */
        .pvReserved36       = (void*) (0UL),           /* 36 Reserved */
        .pvReserved37       = (void*) (0UL),           /* 37 Reserved */
        .pvReserved38       = (void*) (0UL),           /* 38 Reserved */
        .pvReserved39       = (void*) (0UL),           /* 39 Reserved */
        .pvReserved40       = (void*) (0UL),           /* 40 Reserved */
        .pvReserved41       = (void*) (0UL),           /* 41 Reserved */
        .pvReserved42       = (void*) (0UL),           /* 42 Reserved */
        .pvReserved43       = (void*) (0UL),           /* 43 Reserved */
        .pvReserved44       = (void*) (0UL),           /* 44 Reserved */
        .pvReserved45       = (void*) (0UL),           /* 45 Reserved */
        .pvReserved46       = (void*) (0UL),           /* 46 Reserved */
        .pfnUHP_Handler     = (void*) UHP_Handler,     /* 47 USB OHCI */
        .pfnUDP_Handler     = (void*) UDP_Handler,     /* 48 USB Device FS */
        .pfnCRCCU_Handler   = (void*) CRCCU_Handler    /* 49 CRCCU */
};

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
        uint32_t *pSrc = __section_begin(".intvec");

        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        return 1; /* if return 0, the data sections will not be initialized */
}

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
void Reset_Handler(void)
{
        __iar_program_start();
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}
