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

#include "samv71.h"

typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M7 core handlers */
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
#pragma weak UART0_Handler=Dummy_Handler
#pragma weak UART1_Handler=Dummy_Handler
#pragma weak PIOA_Handler=Dummy_Handler
#pragma weak PIOB_Handler=Dummy_Handler
#ifdef _SAMV71_PIOC_INSTANCE_
#pragma weak PIOC_Handler=Dummy_Handler
#endif /* _SAMV71_PIOC_INSTANCE_ */
#ifdef _SAMV71_USART0_INSTANCE_
#pragma weak USART0_Handler=Dummy_Handler
#endif /* _SAMV71_USART0_INSTANCE_ */
#ifdef _SAMV71_USART1_INSTANCE_
#pragma weak USART1_Handler=Dummy_Handler
#endif /* _SAMV71_USART1_INSTANCE_ */
#ifdef _SAMV71_USART2_INSTANCE_
#pragma weak USART2_Handler=Dummy_Handler
#endif /* _SAMV71_USART2_INSTANCE_ */
#pragma weak PIOD_Handler=Dummy_Handler
#ifdef _SAMV71_PIOE_INSTANCE_
#pragma weak PIOE_Handler=Dummy_Handler
#endif /* _SAMV71_PIOE_INSTANCE_ */
#ifdef _SAMV71_HSMCI_INSTANCE_
#pragma weak HSMCI_Handler=Dummy_Handler
#endif /* _SAMV71_HSMCI_INSTANCE_ */
#pragma weak TWIHS0_Handler=Dummy_Handler
#pragma weak TWIHS1_Handler=Dummy_Handler
#ifdef _SAMV71_SPI0_INSTANCE_
#pragma weak SPI0_Handler=Dummy_Handler
#endif /* _SAMV71_SPI0_INSTANCE_ */
#pragma weak SSC_Handler=Dummy_Handler
#pragma weak TC0_Handler=Dummy_Handler
#pragma weak TC1_Handler=Dummy_Handler
#pragma weak TC2_Handler=Dummy_Handler
#ifdef _SAMV71_TC1_INSTANCE_
#pragma weak TC3_Handler=Dummy_Handler
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
#pragma weak TC4_Handler=Dummy_Handler
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
#pragma weak TC5_Handler=Dummy_Handler
#endif /* _SAMV71_TC1_INSTANCE_ */
#pragma weak AFEC0_Handler=Dummy_Handler
#ifdef _SAMV71_DACC_INSTANCE_
#pragma weak DACC_Handler=Dummy_Handler
#endif /* _SAMV71_DACC_INSTANCE_ */
#pragma weak PWM0_Handler=Dummy_Handler
#pragma weak ICM_Handler=Dummy_Handler
#pragma weak ACC_Handler=Dummy_Handler
#pragma weak USBHS_Handler=Dummy_Handler
#pragma weak MCAN0_INT0_Handler=Dummy_Handler
#pragma weak MCAN0_INT1_Handler=Dummy_Handler
#ifdef _SAMV71_MCAN1_INSTANCE_
#pragma weak MCAN1_INT0_Handler=Dummy_Handler
#pragma weak MCAN1_INT1_Handler=Dummy_Handler
#endif /* _SAMV71_MCAN1_INSTANCE_ */
#pragma weak GMAC_Handler=Dummy_Handler
#pragma weak AFEC1_Handler=Dummy_Handler
#ifdef _SAMV71_TWIHS2_INSTANCE_
#pragma weak TWIHS2_Handler=Dummy_Handler
#endif /* _SAMV71_TWIHS2_INSTANCE_ */
#ifdef _SAMV71_SPI1_INSTANCE_
#pragma weak SPI1_Handler=Dummy_Handler
#endif /* _SAMV71_SPI1_INSTANCE_ */
#pragma weak QSPI_Handler=Dummy_Handler
#pragma weak UART2_Handler=Dummy_Handler
#ifdef _SAMV71_UART3_INSTANCE_
#pragma weak UART3_Handler=Dummy_Handler
#endif /* _SAMV71_UART3_INSTANCE_ */
#ifdef _SAMV71_UART4_INSTANCE_
#pragma weak UART4_Handler=Dummy_Handler
#endif /* _SAMV71_UART4_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
#pragma weak TC6_Handler=Dummy_Handler
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
#pragma weak TC7_Handler=Dummy_Handler
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
#pragma weak TC8_Handler=Dummy_Handler
#endif /* _SAMV71_TC2_INSTANCE_ */
#pragma weak TC9_Handler=Dummy_Handler
#pragma weak TC10_Handler=Dummy_Handler
#pragma weak TC11_Handler=Dummy_Handler
#pragma weak MLB_Handler=Dummy_Handler
#pragma weak AES_Handler=Dummy_Handler
#pragma weak TRNG_Handler=Dummy_Handler
#pragma weak XDMAC_Handler=Dummy_Handler
#pragma weak ISI_Handler=Dummy_Handler
#pragma weak PWM1_Handler=Dummy_Handler
#pragma weak FPU_Handler=Dummy_Handler
#ifdef _SAMV71_SDRAMC_INSTANCE_
#pragma weak SDRAMC_Handler=Dummy_Handler
#endif /* _SAMV71_SDRAMC_INSTANCE_ */
#pragma weak RSWDT_Handler=Dummy_Handler
#pragma weak CCW_Handler=Dummy_Handler
#pragma weak CCF_Handler=Dummy_Handler
#pragma weak GMAC_Q1_Handler=Dummy_Handler
#pragma weak GMAC_Q2_Handler=Dummy_Handler
#pragma weak IXC_Handler=Dummy_Handler
#ifdef _SAMV71_I2SC0_INSTANCE_
#pragma weak I2SC0_Handler=Dummy_Handler
#endif /* _SAMV71_I2SC0_INSTANCE_ */
#ifdef _SAMV71_I2SC1_INSTANCE_
#pragma weak I2SC1_Handler=Dummy_Handler
#endif /* _SAMV71_I2SC1_INSTANCE_ */
#if (__SAM_M7_REVB == 1)
#pragma weak GMAC_Q3_Handler=Dummy_Handler
#pragma weak GMAC_Q4_Handler=Dummy_Handler
#pragma weak GMAC_Q5_Handler=Dummy_Handler
#endif

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
        .pfnEFC_Handler    = (void*) EFC_Handler,    /* 6  Enhanced Embedded Flash Controller */
        .pfnUART0_Handler  = (void*) UART0_Handler,  /* 7  UART 0 */
        .pfnUART1_Handler  = (void*) UART1_Handler,  /* 8  UART 1 */
        .pvReserved9       = (void*) (0UL),          /* 9  Reserved */
        .pfnPIOA_Handler   = (void*) PIOA_Handler,   /* 10 Parallel I/O Controller A */
        .pfnPIOB_Handler   = (void*) PIOB_Handler,   /* 11 Parallel I/O Controller B */
#ifdef _SAMV71_PIOC_INSTANCE_
        .pfnPIOC_Handler   = (void*) PIOC_Handler,   /* 12 Parallel I/O Controller C */
#else
        .pvReserved12      = (void*) (0UL),          /* 12 Reserved */
#endif /* _SAMV71_PIOC_INSTANCE_ */
#ifdef _SAMV71_USART0_INSTANCE_
        .pfnUSART0_Handler = (void*) USART0_Handler, /* 13 USART 0 */
#else
        .pvReserved13      = (void*) (0UL), 
#endif /* _SAMV71_USART0_INSTANCE_ */
#ifdef _SAMV71_USART1_INSTANCE_
        .pfnUSART1_Handler = (void*) USART1_Handler, /* 14 USART 1 */
#else
        .pvReserved14      = (void*) (0UL), 
#endif /* _SAMV71_USART1_INSTANCE_ */
#ifdef _SAMV71_USART2_INSTANCE_
        .pfnUSART2_Handler = (void*) USART2_Handler, /* 15 USART 2 */
#else
        .pvReserved15      = (void*) (0UL), 
#endif /* _SAMV71_USART2_INSTANCE_ */
        .pfnPIOD_Handler   = (void*) PIOD_Handler,   /* 16 Parallel I/O Controller D */
#ifdef _SAMV71_PIOE_INSTANCE_
        .pfnPIOE_Handler   = (void*) PIOE_Handler,   /* 17 Parallel I/O Controller E */
#else
        .pvReserved17      = (void*) (0UL),          /* 17 Reserved */
#endif /* _SAMV71_PIOE_INSTANCE_ */
#ifdef _SAMV71_HSMCI_INSTANCE_
        .pfnHSMCI_Handler  = (void*) HSMCI_Handler,  /* 18 Multimedia Card Interface */
#else
        .pvReserved18      = (void*) (0UL),          /* 18 Reserved */
#endif /* _SAMV71_HSMCI_INSTANCE_ */
        .pfnTWIHS0_Handler = (void*) TWIHS0_Handler, /* 19 Two Wire Interface 0 HS */
        .pfnTWIHS1_Handler = (void*) TWIHS1_Handler, /* 20 Two Wire Interface 1 HS */
#ifdef _SAMV71_SPI0_INSTANCE_
        .pfnSPI0_Handler   = (void*) SPI0_Handler,   /* 21 Serial Peripheral Interface 0 */
#else
        .pvReserved21      = (void*) (0UL),          /* 21 Reserved */
#endif /* _SAMV71_SPI0_INSTANCE_ */
        .pfnSSC_Handler    = (void*) SSC_Handler,    /* 22 Synchronous Serial Controller */
        .pfnTC0_Handler    = (void*) TC0_Handler,    /* 23 Timer/Counter 0 */
        .pfnTC1_Handler    = (void*) TC1_Handler,    /* 24 Timer/Counter 1 */
        .pfnTC2_Handler    = (void*) TC2_Handler,    /* 25 Timer/Counter 2 */
#ifdef _SAMV71_TC1_INSTANCE_
        .pfnTC3_Handler    = (void*) TC3_Handler,    /* 26 Timer/Counter 3 */
#else
        .pvReserved26      = (void*) (0UL),          /* 26 Reserved */
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
        .pfnTC4_Handler    = (void*) TC4_Handler,    /* 27 Timer/Counter 4 */
#else
        .pvReserved27      = (void*) (0UL),          /* 27 Reserved */
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
        .pfnTC5_Handler    = (void*) TC5_Handler,    /* 28 Timer/Counter 5 */
#else
        .pvReserved28      = (void*) (0UL),          /* 28 Reserved */
#endif /* _SAMV71_TC1_INSTANCE_ */
        .pfnAFEC0_Handler  = (void*) AFEC0_Handler,  /* 29 Analog Front End 0 */
#ifdef _SAMV71_DACC_INSTANCE_
        .pfnDACC_Handler   = (void*) DACC_Handler,   /* 30 Digital To Analog Converter */
#else
        .pvReserved30      = (void*) (0UL),          /* 30 Reserved */
#endif /* _SAMV71_DACC_INSTANCE_ */
        .pfnPWM0_Handler   = (void*) PWM0_Handler,   /* 31 Pulse Width Modulation 0 */
        .pfnICM_Handler    = (void*) ICM_Handler,    /* 32 Integrity Check Monitor */
        .pfnACC_Handler    = (void*) ACC_Handler,    /* 33 Analog Comparator */
        .pfnUSBHS_Handler  = (void*) USBHS_Handler,  /* 34 USB Host / Device Controller */
        .pfnMCAN0_INT0_Handler   = (void*) MCAN0_INT0_Handler, /* 35 Controller Area Network */
        .pfnMCAN0_INT1_Handler   = (void*) MCAN0_INT1_Handler, /* 36 Controller Area Network */
#ifdef _SAMV71_MCAN1_INSTANCE_
        .pfnMCAN1_INT0_Handler   = (void*) MCAN1_INT0_Handler, /* 37 Controller Area Network */
        .pfnMCAN1_INT1_Handler   = (void*) MCAN1_INT1_Handler, /* 38 Controller Area Network */
#else
        .pvReserved37      = (void*) (0UL),          /* 37 Reserved */
        .pvReserved38      = (void*) (0UL),          /* 38 Reserved */
#endif /* _SAMV71_MCAN1_INSTANCE_ */
        .pfnGMAC_Handler   = (void*) GMAC_Handler,   /* 39 Ethernet MAC */
        .pfnAFEC1_Handler  = (void*) AFEC1_Handler,  /* 40 Analog Front End 1 */
#ifdef _SAMV71_TWIHS2_INSTANCE_
        .pfnTWIHS2_Handler = (void*) TWIHS2_Handler, /* 41 Two Wire Interface 2 HS */
#else
        .pvReserved41      = (void*) (0UL),          /* 41 Reserved */
#endif /* _SAMV71_TWIHS2_INSTANCE_ */
#ifdef _SAMV71_SPI1_INSTANCE_
        .pfnSPI1_Handler   = (void*) SPI1_Handler,   /* 42 Serial Peripheral Interface 1 */
#else
        .pvReserved42      = (void*) (0UL),          /* 42 Reserved */
#endif /* _SAMV71_SPI1_INSTANCE_ */
        .pfnQSPI_Handler   = (void*) QSPI_Handler,   /* 43 Quad I/O Serial Peripheral Interface */
        .pfnUART2_Handler  = (void*) UART2_Handler,  /* 44 UART 2 */
#ifdef _SAMV71_UART3_INSTANCE_
        .pfnUART3_Handler  = (void*) UART3_Handler,  /* 45 UART 3 */
#else
        .pvReserved45      = (void*) (0UL),          /* 45 Reserved */
#endif /* _SAMV71_UART3_INSTANCE_ */
#ifdef _SAMV71_UART4_INSTANCE_
        .pfnUART4_Handler  = (void*) UART4_Handler,  /* 46 UART 4 */
#else
        .pvReserved46      = (void*) (0UL),          /* 46 Reserved */
#endif /* _SAMV71_UART4_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
        .pfnTC6_Handler    = (void*) TC6_Handler,    /* 47 Timer/Counter 6 */
#else
        .pvReserved47      = (void*) (0UL),          /* 47 Reserved */
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
        .pfnTC7_Handler    = (void*) TC7_Handler,    /* 48 Timer/Counter 7 */
#else
        .pvReserved48      = (void*) (0UL),          /* 48 Reserved */
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
        .pfnTC8_Handler    = (void*) TC8_Handler,    /* 49 Timer/Counter 8 */
#else
        .pvReserved49      = (void*) (0UL),          /* 49 Reserved */
#endif /* _SAMV71_TC2_INSTANCE_ */
        .pfnTC9_Handler    = (void*) TC9_Handler,    /* 50 Timer/Counter 9 */
        .pfnTC10_Handler   = (void*) TC10_Handler,   /* 51 Timer/Counter 10 */
        .pfnTC11_Handler   = (void*) TC11_Handler,   /* 52 Timer/Counter 11 */
        .pfnMLB_Handler    = (void*) MLB_Handler,    /* 53 MediaLB */
        .pvReserved54      = (void*) (0UL),          /* 54 Reserved */
        .pvReserved55      = (void*) (0UL),          /* 55 Reserved */
        .pfnAES_Handler    = (void*) AES_Handler,    /* 56 AES */
        .pfnTRNG_Handler   = (void*) TRNG_Handler,   /* 57 True Random Generator */
        .pfnXDMAC_Handler  = (void*) XDMAC_Handler,  /* 58 DMA */
        .pfnISI_Handler    = (void*) ISI_Handler,    /* 59 Camera Interface */
        .pfnPWM1_Handler   = (void*) PWM1_Handler,   /* 60 Pulse Width Modulation 1 */
        .pfnFPU_Handler                = (void*) FPU_Handler,    /* 61 Floating Point Unit Registers */
#ifdef _SAMV71_SDRAMC_INSTANCE_
        .pfnSDRAMC_Handler = (void*) SDRAMC_Handler, /* 62 SDRAM Controller */
#else
        .pvReserved62      = (void*) (0UL),          /* 62 Reserved */
#endif /* _SAMV71_SDRAMC_INSTANCE_ */
        .pfnRSWDT_Handler  = (void*) RSWDT_Handler,  /* 63 Reinforced Secure Watchdog Timer */
        .pfnCCW_Handler    = (void*) CCW_Handler,    /* 64 System Control Registers */
        .pfnCCF_Handler    = (void*) CCF_Handler,    /* 65 System Control Registers */
        .pfnGMAC_Q1_Handler= (void*) GMAC_Q1_Handler,/* 66 Gigabit Ethernet MAC */
        .pfnGMAC_Q2_Handler= (void*) GMAC_Q2_Handler,/* 67 Gigabit Ethernet MAC */
        .pfnIXC_Handler    = (void*) IXC_Handler,    /* 68 Floating Point Unit Registers */
#ifdef _SAMV71_I2SC0_INSTANCE_
        .pfnI2SC0_Handler  = (void*) I2SC0_Handler,  /* 69 Inter-IC Sound controller */
#else
        .pvReserved69      = (void*) (0UL),          /* 69 Reserved */
#endif /* _SAMV71_I2SC0_INSTANCE_ */
#ifdef _SAMV71_I2SC1_INSTANCE_
        .pfnI2SC1_Handler  = (void*) I2SC1_Handler,   /* 70 Inter-IC Sound controller */
#else
        .pvReserved70      = (void*) (0UL),           /* 70 Reserved */
#endif /* _SAMV71_I2SC1_INSTANCE_ */
#if (__SAM_M7_REVB == 1)
        .pfnGMAC_Q3_Handler= (void*) GMAC_Q3_Handler,/* 71 Gigabit Ethernet MAC */
        .pfnGMAC_Q4_Handler= (void*) GMAC_Q4_Handler,/* 72 Gigabit Ethernet MAC */
        .pfnGMAC_Q5_Handler= (void*) GMAC_Q5_Handler /* 73 Gigabit Ethernet MAC */
#else
        .pvReserved71      = (void*) (0UL),          /* 71 Reserved */
        .pvReserved72      = (void*) (0UL),          /* 72 Reserved */
        .pvReserved73      = (void*) (0UL)           /* 73 Reserved */
#endif
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
