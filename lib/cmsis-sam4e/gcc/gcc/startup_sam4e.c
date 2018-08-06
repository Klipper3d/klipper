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

#include "sam4e.h"

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
void EFC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOD_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOE_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART0_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART1_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HSMCI_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWI0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWI1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC8_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AFEC0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AFEC1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DACC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ACC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ARM_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UDP_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PWM_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AES_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GMAC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors"))) __attribute__((externally_visible))
const DeviceVectors exception_table = {

        /* Configure Initial Stack Pointer, using linker-generated symbols */
        (void*) (&_estack),

        (void*) Reset_Handler,
        (void*) NMI_Handler,
        (void*) HardFault_Handler,
        (void*) MemManage_Handler,
        (void*) BusFault_Handler,
        (void*) UsageFault_Handler,
        (void*) (0UL),          /* Reserved */
        (void*) (0UL),          /* Reserved */
        (void*) (0UL),          /* Reserved */
        (void*) (0UL),          /* Reserved */
        (void*) SVC_Handler,
        (void*) DebugMon_Handler,
        (void*) (0UL),          /* Reserved */
        (void*) PendSV_Handler,
        (void*) SysTick_Handler,

        /* Configurable interrupts */
        (void*) SUPC_Handler,   /* 0  Supply Controller */
        (void*) RSTC_Handler,   /* 1  Reset Controller */
        (void*) RTC_Handler,    /* 2  Real Time Clock */
        (void*) RTT_Handler,    /* 3  Real Time Timer */
        (void*) WDT_Handler,    /* 4  Watchdog/Dual Watchdog Timer */
        (void*) PMC_Handler,    /* 5  Power Management Controller */
        (void*) EFC_Handler,    /* 6  Enhanced Embedded Flash Controller */
        (void*) UART0_Handler,  /* 7  UART 0 */
        (void*) Dummy_Handler,  /* 8  Dummy */
        (void*) PIOA_Handler,   /* 9  Parallel I/O Controller A */
        (void*) PIOB_Handler,   /* 10 Parallel I/O Controller B */
        (void*) PIOC_Handler,   /* 11 Parallel I/O Controller C */
        (void*) PIOD_Handler,   /* 12 Parallel I/O Controller D */
        (void*) PIOE_Handler,   /* 13 Parallel I/O Controller E */
        (void*) USART0_Handler, /* 14 USART 0 */
        (void*) USART1_Handler, /* 15 USART 1 */
        (void*) HSMCI_Handler,  /* 16 Multimedia Card Interface */
        (void*) TWI0_Handler,   /* 17 Two Wire Interface 0 */
        (void*) TWI1_Handler,   /* 18 Two Wire Interface 1 */
        (void*) SPI_Handler,    /* 19 Serial Peripheral Interface */
        (void*) DMAC_Handler,   /* 20 DMAC */
        (void*) TC0_Handler,    /* 21 Timer/Counter 0 */
        (void*) TC1_Handler,    /* 22 Timer/Counter 1 */
        (void*) TC2_Handler,    /* 23 Timer/Counter 2 */
        (void*) TC3_Handler,    /* 24 Timer/Counter 3 */
        (void*) TC4_Handler,    /* 25 Timer/Counter 4 */
        (void*) TC5_Handler,    /* 26 Timer/Counter 5 */
        (void*) TC6_Handler,    /* 27 Timer/Counter 6 */
        (void*) TC7_Handler,    /* 28 Timer/Counter 7 */
        (void*) TC8_Handler,    /* 29 Timer/Counter 8 */
        (void*) AFEC0_Handler,  /* 30 Analog Front End 0 */
        (void*) AFEC1_Handler,  /* 31 Analog Front End 1 */
        (void*) DACC_Handler,   /* 32 Digital To Analog Converter */
        (void*) ACC_Handler,    /* 33 Analog Comparator */
        (void*) ARM_Handler,    /* 34 FPU signals : FPIXC, FPOFC, FPUFC, FPIOC, FPDZC, FPIDC, FPIXC */
        (void*) UDP_Handler,    /* 35 USB DEVICE */
        (void*) PWM_Handler,    /* 36 PWM */
        (void*) CAN0_Handler,   /* 37 CAN0 */
        (void*) CAN1_Handler,   /* 38 CAN1 */
        (void*) AES_Handler,    /* 39 AES */
        (void*) Dummy_Handler,  /* 40 Dummy */
        (void*) Dummy_Handler,  /* 41 Dummy */
        (void*) Dummy_Handler,  /* 42 Dummy */
        (void*) Dummy_Handler,  /* 43 Dummy */
        (void*) GMAC_Handler,   /* 44 EMAC */
        (void*) UART1_Handler   /* 45 UART */
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
        // __libc_init_array();

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
