/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

#include "sam4s.h"

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
void SUPC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RSTC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTT_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PMC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EFC0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAM4S_EFC1_INSTANCE_
void EFC1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_EFC1_INSTANCE_ */
void UART0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAM4S_PIOC_INSTANCE_
void PIOC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_PIOC_INSTANCE_ */
void USART0_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAM4S_USART1_INSTANCE_
void USART1_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_USART1_INSTANCE_ */
#ifdef _SAM4S_HSMCI_INSTANCE_
void HSMCI_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_HSMCI_INSTANCE_ */
void TWI0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWI1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SSC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAM4S_TC1_INSTANCE_
void TC3_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
void TC4_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
void TC5_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_TC1_INSTANCE_ */
void ADC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAM4S_DACC_INSTANCE_
void DACC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAM4S_DACC_INSTANCE_ */
void PWM_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CRCCU_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ACC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UDP_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors"))) __attribute__((externally_visible))
const DeviceVectors exception_table = {

        /* Configure Initial Stack Pointer, using linker-generated symbols */
        .pvStack = (void*) (&_estack),

        .pfnReset_Handler      = (void*) Reset_Handler,
        .pfnNMI_Handler        = (void*) NMI_Handler,
        .pfnHardFault_Handler  = (void*) HardFault_Handler,
        .pfnMemManage_Handler  = (void*) MemManage_Handler,
        .pfnBusFault_Handler   = (void*) BusFault_Handler,
        .pfnUsageFault_Handler = (void*) UsageFault_Handler,
        .pfnReserved1_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved2_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved3_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved4_Handler  = (void*) (0UL),          /* Reserved */
        .pfnSVC_Handler        = (void*) SVC_Handler,
        .pfnDebugMon_Handler   = (void*) DebugMon_Handler,
        .pfnReserved5_Handler  = (void*) (0UL),          /* Reserved */
        .pfnPendSV_Handler     = (void*) PendSV_Handler,
        .pfnSysTick_Handler    = (void*) SysTick_Handler,

        /* Configurable interrupts */
        .pfnSUPC_Handler   = (void*) SUPC_Handler,   /* 0  Supply Controller */
        .pfnRSTC_Handler   = (void*) RSTC_Handler,   /* 1  Reset Controller */
        .pfnRTC_Handler    = (void*) RTC_Handler,    /* 2  Real Time Clock */
        .pfnRTT_Handler    = (void*) RTT_Handler,    /* 3  Real Time Timer */
        .pfnWDT_Handler    = (void*) WDT_Handler,    /* 4  Watchdog Timer */
        .pfnPMC_Handler    = (void*) PMC_Handler,    /* 5  Power Management Controller */
        .pfnEFC0_Handler   = (void*) EFC0_Handler,   /* 6  Enhanced Embedded Flash Controller 0 */
#ifdef _SAM4S_EFC1_INSTANCE_
        .pfnEFC1_Handler   = (void*) EFC1_Handler,   /* 7  Enhanced Embedded Flash Controller 1 */
#else
        .pvReserved7       = (void*) (0UL),          /* 7  Reserved */
#endif /* _SAM4S_EFC1_INSTANCE_ */
        .pfnUART0_Handler  = (void*) UART0_Handler,  /* 8  UART 0 */
        .pfnUART1_Handler  = (void*) UART1_Handler,  /* 9  UART 1 */
        .pvReserved10      = (void*) (0UL),          /* 10 Reserved */
        .pfnPIOA_Handler   = (void*) PIOA_Handler,   /* 11 Parallel I/O Controller A */
        .pfnPIOB_Handler   = (void*) PIOB_Handler,   /* 12 Parallel I/O Controller B */
#ifdef _SAM4S_PIOC_INSTANCE_
        .pfnPIOC_Handler   = (void*) PIOC_Handler,   /* 13 Parallel I/O Controller C */
#else
        .pvReserved13      = (void*) (0UL),          /* 13 Reserved */
#endif /* _SAM4S_PIOC_INSTANCE_ */
        .pfnUSART0_Handler = (void*) USART0_Handler, /* 14 USART 0 */
#ifdef _SAM4S_USART1_INSTANCE_
        .pfnUSART1_Handler = (void*) USART1_Handler, /* 15 USART 1 */
#else
        .pvReserved15      = (void*) (0UL),          /* 15 Reserved */
#endif /* _SAM4S_USART1_INSTANCE_ */
        .pvReserved16      = (void*) (0UL),          /* 16 Reserved */
        .pvReserved17      = (void*) (0UL),          /* 17 Reserved */
#ifdef _SAM4S_HSMCI_INSTANCE_
        .pfnHSMCI_Handler  = (void*) HSMCI_Handler,  /* 18 Multimedia Card Interface */
#else
        .pvReserved18      = (void*) (0UL),          /* 18 Reserved */
#endif /* _SAM4S_HSMCI_INSTANCE_ */
        .pfnTWI0_Handler   = (void*) TWI0_Handler,   /* 19 Two Wire Interface 0 */
        .pfnTWI1_Handler   = (void*) TWI1_Handler,   /* 20 Two Wire Interface 1 */
        .pfnSPI_Handler    = (void*) SPI_Handler,    /* 21 Serial Peripheral Interface */
        .pfnSSC_Handler    = (void*) SSC_Handler,    /* 22 Synchronous Serial Controller */
        .pfnTC0_Handler    = (void*) TC0_Handler,    /* 23 Timer/Counter 0 */
        .pfnTC1_Handler    = (void*) TC1_Handler,    /* 24 Timer/Counter 1 */
        .pfnTC2_Handler    = (void*) TC2_Handler,    /* 25 Timer/Counter 2 */
#ifdef _SAM4S_TC1_INSTANCE_
        .pfnTC3_Handler    = (void*) TC3_Handler,    /* 26 Timer/Counter 3 */
#else
        .pvReserved26      = (void*) (0UL),          /* 26 Reserved */
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
        .pfnTC4_Handler    = (void*) TC4_Handler,    /* 27 Timer/Counter 4 */
#else
        .pvReserved27      = (void*) (0UL),          /* 27 Reserved */
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
        .pfnTC5_Handler    = (void*) TC5_Handler,    /* 28 Timer/Counter 5 */
#else
        .pvReserved28      = (void*) (0UL),          /* 28 Reserved */
#endif /* _SAM4S_TC1_INSTANCE_ */
        .pfnADC_Handler    = (void*) ADC_Handler,    /* 29 Analog To Digital Converter */
#ifdef _SAM4S_DACC_INSTANCE_
        .pfnDACC_Handler   = (void*) DACC_Handler,   /* 30 Digital To Analog Converter */
#else
        .pvReserved30      = (void*) (0UL),          /* 30 Reserved */
#endif /* _SAM4S_DACC_INSTANCE_ */
        .pfnPWM_Handler    = (void*) PWM_Handler,    /* 31 Pulse Width Modulation */
        .pfnCRCCU_Handler  = (void*) CRCCU_Handler,  /* 32 CRC Calculation Unit */
        .pfnACC_Handler    = (void*) ACC_Handler,    /* 33 Analog Comparator */
        .pfnUDP_Handler    = (void*) UDP_Handler     /* 34 USB Device Port */
};

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
