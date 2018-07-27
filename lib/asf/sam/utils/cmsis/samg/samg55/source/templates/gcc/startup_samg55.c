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

#if __FPU_USED /* CMSIS defined value to indicate usage of FPU */
#include "fpu.h"
#endif

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M4 core handlers */
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void SUPC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RSTC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTT_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PMC_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EFC_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMG55_FLEXCOM7_INSTANCE_
void FLEXCOM7_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMG55_FLEXCOM7_INSTANCE_*/
void FLEXCOM0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FLEXCOM1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PDMIC0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FLEXCOM2_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MEM2MEM_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void I2SC0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void I2SC1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PDMIC1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FLEXCOM3_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FLEXCOM4_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FLEXCOM5_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FLEXCOM6_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ARM_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UHP_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UDP_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CRCCU_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

        /* Configure Initial Stack Pointer, using linker-generated symbols */
        .pvStack = (void*) (&_estack),

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

/* TEMPORARY PATCH FOR SCB */
#define SCB_VTOR_TBLBASE_Pos               29                            /*!< SCB VTOR: TBLBASE Position */
#define SCB_VTOR_TBLBASE_Msk               (1UL << SCB_VTOR_TBLBASE_Pos) /*!< SCB VTOR: TBLBASE Mask */

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
        uint32_t *pSrc, *pDest;

        /* Initialize the relocate segment */
        pSrc = &_etext;
        pDest = &_srelocate;

        if (pSrc != pDest) {
                for (; pDest < &_erelocate;) {
                        *pDest++ = *pSrc++;
                }
        }

        /* Clear the zero segment */
        for (pDest = &_szero; pDest < &_ezero;) {
                *pDest++ = 0;
        }

        /* Set the vector table base address */
        pSrc = (uint32_t *) & _sfixed;
        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

#if __FPU_USED
	fpu_enable();
#endif

	if (((uint32_t) pSrc >= IRAM_ADDR) && ((uint32_t) pSrc < IRAM_ADDR + IRAM_SIZE)) {
		SCB->VTOR |= 1 << SCB_VTOR_TBLBASE_Pos;
	}

	/* Initialize the C library */
	__libc_init_array();

        /* Branch to main function */
        main();

        /* Infinite loop */
        while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}
